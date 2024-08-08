#include "../include/TelemetryReceiver.h"


TelemetryReceiver::TelemetryReceiver(EventsBus &bus, const std::string &portCom,
                                     bool isVerbose) 
    : m_portCom(portCom), m_verbose(isVerbose) { 

  m_publisher = bus.getPublisher();
  m_running.store(false);

  // Initializing serial port connection
  m_comSerial = HANDLE(INVALID_HANDLE_VALUE);
  m_dcbSerialParams = {0};

  std::wstring wideComPort(m_portCom.begin(), m_portCom.end());
  LPCWSTR comPortName = wideComPort.c_str();

  m_comSerial = CreateFile(
	  comPortName,
	  GENERIC_READ | GENERIC_WRITE,
	  0,
	  0,
	  OPEN_EXISTING, 
	  FILE_ATTRIBUTE_NORMAL,
	  0
  );

  if (m_comSerial == INVALID_HANDLE_VALUE) {
    ConnectionEvent connEvent(false, "TelemetryReceiver",
                              "Error opening serial port");
    AppTerminationEvent terminationEvent(true);
    m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent); // Incorrect shutdown of entire application
    m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);

  }

  SecureZeroMemory(&m_dcbSerialParams, sizeof(m_dcbSerialParams));
  m_dcbSerialParams.DCBlength = sizeof(m_dcbSerialParams);

  if (!GetCommState(m_comSerial, &m_dcbSerialParams)) {
    ConnectionEvent connEvent(false, "TelemetryReceiver",
                              "Error getting com state: " + GetLastError());
    AppTerminationEvent terminationEvent(true);
    m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
  }

  //  Fill in some DCB values and set the com state:
  //  57,600 bps, 8 data bits, no parity, and 1 stop bit.
  m_dcbSerialParams.BaudRate = CBR_57600;  //  baud rate
  m_dcbSerialParams.ByteSize = 8;          //  data size, xmit and rcv
  m_dcbSerialParams.Parity = NOPARITY;     //  parity bit
  m_dcbSerialParams.StopBits = ONESTOPBIT; //  stop bit

  if (!SetCommState(m_comSerial, &m_dcbSerialParams)) {
    ConnectionEvent connEvent(false, "TelemetryReceiver",
                              "Error setting serial port state: "+GetLastError());
    AppTerminationEvent terminationEvent(true);
    m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
    m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
  }

  if (!GetCommState(m_comSerial, &m_dcbSerialParams)) {
    ConnectionEvent connEvent(false, "TelemetryReceiver",
                              "Error getting serial port state: " +
                                  GetLastError());
    AppTerminationEvent terminationEvent(true);
    m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
    m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
  }

  if (m_verbose) {
    // Weird mutex error occurs when EventsBus is used here
    /*
    ConnectionEvent connEvent(true, "TelemetryReceiver",
                              "Instantiated");
    m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
    */
    std::cout << "TelemetryReceiver: Instanitated" << std::endl;
  }
}

void TelemetryReceiver::receive_() {
	// Launching Processor thread
    m_running.store(true);
	if (m_verbose) {
      ConnectionEvent connEvent(true, "TelemetryReceiver",
                                "Running receiver thread");
      m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
	}

    // Mavlink data structures
    mavlink_message_t message;
    mavlink_status_t status;
    
    // Lambda function for setting up mavlink data interval
    auto requestDataStream = [&](uint16_t messageId, uint32_t intervalUs) {
      mavlink_command_ack_t command_ack;
      uint8_t intervalRequestData[MAVLINK_MAX_PACKET_LEN];

      mavlink_msg_command_long_pack(255, 0, &message, 1, 1,
                                    MAV_CMD_SET_MESSAGE_INTERVAL, 0, messageId,
                                    intervalUs, 0, 0, 0, 0, NULL);
      
      int requestMessageLenght =
          mavlink_msg_to_send_buffer(intervalRequestData, &message);

      DWORD bytesWritten;
      if (!WriteFile(m_comSerial, intervalRequestData, requestMessageLenght,
                     &bytesWritten, NULL)) {
        ConnectionEvent connEvent(false, "TelemetryReceiver",
                                  "Couldnt request data interval: " +
                                      GetLastError());
        AppTerminationEvent terminationEvent(true);
        m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
        m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
      }

      // Wait for acknowledgement
      while (true) {
        DWORD dwBytesReadAcknowledgeRqst = 0;
        uint8_t acknowledgeData;

        if (ReadFile(m_comSerial, &acknowledgeData, 1,
                     &dwBytesReadAcknowledgeRqst, NULL) &&
            dwBytesReadAcknowledgeRqst == 1) {
          if (mavlink_parse_char(MAVLINK_COMM_1, acknowledgeData, &message,
                                 &status) == 1) {
            if (message.msgid == MAVLINK_MSG_ID_COMMAND_ACK) {
              mavlink_msg_command_ack_decode(&message, &command_ack);
              if (command_ack.command == MAV_CMD_SET_MESSAGE_INTERVAL &&
                  command_ack.result == MAV_RESULT_ACCEPTED) {

                ConnectionEvent connEvent(
                    true, "TelemetryReceiver",
                    "Received mavlink request ack from UAV");
                m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);

                break;
              } else {

                ConnectionEvent connEvent(false, "TelemetryReceiver",
                                          "Didnt receive mavlink request ack from UAV");
                AppTerminationEvent terminationEvent(true);
                m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
                m_publisher->publish(EventType::APP_TERMINATION,
                                     terminationEvent);

                break;
              }
            }
          }
        }
      }
    }; // End of lambda function

    /****************************************************
    * Request attitude, GPS, and heartbeat
    * data interval frequency
    ****************************************************/
    requestDataStream(MAVLINK_MSG_ID_ATTITUDE,            100000);  // 10 Hz
    requestDataStream(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, 500000);  // 2 Hz
    requestDataStream(MAVLINK_MSG_ID_HEARTBEAT,           1000000); // 1 Hz

    /****************************************************
    * Main loop for receiving telemetry
    ****************************************************/
	while (m_running.load()) { 
		// Read data
        DWORD dwBytesRead = 0;
        uint8_t raw_data;

        if (ReadFile(m_comSerial, &raw_data, 1, &dwBytesRead, NULL) &&
            dwBytesRead == 1) {
          if (mavlink_parse_char(MAVLINK_COMM_1, raw_data, &message, &status) ==
              1) {
            switch (message.msgid) {
                case MAVLINK_MSG_ID_ATTITUDE: {
                  mavlink_attitude_t attitude;
                  mavlink_msg_attitude_decode(&message, &attitude);
                  float roll  = attitude.roll;
                  float pitch = attitude.pitch;
                  float yaw   = attitude.yaw;
                  m_currTelemetry = {roll, pitch, yaw};
                  registerTelemetryEvent_();
                } break;

                case MAVLINK_MSG_ID_HEARTBEAT: {
                  mavlink_heartbeat_t heartbeat;
                  mavlink_msg_heartbeat_decode(&message, &heartbeat);
                  switch (heartbeat.system_status) {
                      case MAV_STATE_ACTIVE: {
                        ConnectionEvent connEvent(true, "TelemetryReceiver",
                                                  "Mavlink ON");
                        m_publisher->publish(EventType::CONNECTION_UPDATE,
                                             connEvent);
                      } break;

                      case MAV_STATE_EMERGENCY: {
                        ConnectionEvent connEvent(false, "TelemetryReceiver",
                                                  "Mavlink EMERGENCY");
                        m_publisher->publish(EventType::CONNECTION_UPDATE,
                                             connEvent);
                      } break;

                      case MAV_STATE_CRITICAL: {
                        ConnectionEvent connEvent(false, "TelemetryReceiver",
                                                  "Mavlink CRITICAL");
                        m_publisher->publish(EventType::CONNECTION_UPDATE,
                                             connEvent);
                      } break;
                       
                      default: {
                        ConnectionEvent connEvent(false, "TelemetryReceiver",
                                                  "Mavlink UNDEFINED");
                        m_publisher->publish(EventType::CONNECTION_UPDATE,
                                             connEvent);
                      }
                  }
                } break;

                default: {
                  ConnectionEvent connEvent(
                      true, "TelemetryReceiver",
                      "No telemetry data from UAV");
                  m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
                }
            }
          } 

        } else {
          ConnectionEvent connEvent(false, "TelemetryReceiver",
                                    "Serial connection error");
          m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
        }
	}
    
	// This section launches after stop_() shuts down the loop.
	// It's responsible for:
	// cleaning resources, calling Processor to generate the report, shutting down Processor thread
	
}

void TelemetryReceiver::stop_() { 
	if (m_verbose) {
        // Error I cant crack to solve
        /*
        ConnectionEvent connEvent(true, "TelemetryReceiver",
                                  "terminating");
        m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
        */
          std::cout << "TelemetryReceiver: Terminating" << std::endl;
    }
	m_running.store(false);
    CloseHandle(m_comSerial);
}

void TelemetryReceiver::registerTelemetryEvent_() { 
	TelemetryEvent telemetry(m_currTelemetry);
	m_publisher->publish(EventType::TELEMETRY_UPDATE, telemetry); 
}
