/**
 * @file TelemetryReceiver.cpp
 * @brief Code of the concrete implementation of ITelemetryReceiver interface.
 *
 * @details This file contains the declaration of the concrete telemetry receiver, which
 *          utilizes:
 *          - serial port connection for receiving telemetry data (windows.h)
 *          - mavlink protocol for receiving telemetry data
 *
 * @author Szymon Bogus
 * @date 2024-05-22
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 *
 * @note Emergency shutdown from TelemetryReceiver::receive_ result in deadlock in DronePositioningWinAppBacked.cpp.
 *       This might not be directly related to some bad code in the method here. However, another issue is the fact that,
 *       from the level of constructor of TelemetryReceiver it cannot use
 *       EventsBus to signal some ConnectionEvent- deadlock as subscription map is empty and there's nothing to lock on. 
 *       This is due to the fact that instantiaion of TelemetryReceiver happend before anything can subscribe to EventsBus- 
 *       - I'm talking about ConnectionManager. Switching the order in the MainController.cpp should help resolve that issue.
 *       However ConnectionManager requires TelemetryReceiver as its constructor argument. Apparently TelemetryReceiver should be 
 *       passed to ConnectionManager::connect, instead of the constructor and some sort of reference to it shoudl be created inside
 *       ConnectionManager so later on it can also call ConnectionManager::disconnect and ask TelemetryReceiver::stop.
 */

#include "../include/TelemetryReceiver.h"


TelemetryReceiver::TelemetryReceiver(EventsBus &bus,
                                     const OperatorPosition &homePos,
                                     const std::string &portCom,
                                     bool isVerbose) 
    : m_portCom(portCom), m_homePos(homePos),
      m_metersPerDegree(
          CoordinatesUtils::getMetersPerLatitudeDegree(homePos.latitude)),
      m_verbose(isVerbose) { 

  m_publisher = bus.getPublisher();
  m_running.store(false);

  // Initializing serial port connection
  // Validating, if port COM works
  int retryCnt = 5;
  DWORD isCOM = 0;
  CHAR lpTargetPath[5000];
  while (retryCnt > 0) {
    isCOM = QueryDosDeviceA(m_portCom.c_str(), lpTargetPath, 5000);
    if (isCOM) {
      break;
    } else {
      std::cout << "You have: " << retryCnt * 5
                << " s to plug in the receiver..\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(5000)); // wait 5 seconds before checking again
      retryCnt--;
    }
  } // wait overall 25 seconds for connection
  
  if (!isCOM) {
    throw std::runtime_error("Receiving device is not connected or port COM "
                             "has been incorrectly specified");
  }

  // Establishing connection
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
    throw std::runtime_error("Error opening serial port");
  }

  SecureZeroMemory(&m_dcbSerialParams, sizeof(m_dcbSerialParams));
  m_dcbSerialParams.DCBlength = sizeof(m_dcbSerialParams);

  if (!GetCommState(m_comSerial, &m_dcbSerialParams)) {
    throw std::runtime_error("Error getting com state");
  }

  //  Fill in some DCB values and set the com state:
  //  57,600 bps, 8 data bits, no parity, and 1 stop bit.
  m_dcbSerialParams.BaudRate = CBR_57600;  //  baud rate
  m_dcbSerialParams.ByteSize = 8;          //  data size, xmit and rcv
  m_dcbSerialParams.Parity = NOPARITY;     //  parity bit
  m_dcbSerialParams.StopBits = ONESTOPBIT; //  stop bit

  if (!SetCommState(m_comSerial, &m_dcbSerialParams)) {
    throw std::runtime_error("Error setting serial port state");
  }

  if (!GetCommState(m_comSerial, &m_dcbSerialParams)) {
    throw std::runtime_error("Error getting serial port state");
  }

  // m_metersPerDegree =
  //     CoordinatesUtils::getMetersPerLatitudeDegree(m_homePos.latitude);

  if (m_verbose) {
    std::cout << "TelemetryReceiver: instanitated\n";
  }
}

TelemetryReceiver::~TelemetryReceiver() { m_publisher = nullptr; }

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
        } else {
          ConnectionEvent connEvent(false, "TelemetryReceiver",
                                    "Serial connection error");
          AppTerminationEvent terminationEvent(true);
          m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
          m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
        }
      }
    }; // End of lambda function

    /****************************************************
    * Request attitude, GPS, and heartbeat
    * data interval frequency
    * IMPORTANT: frequency for both attitude and GPS 
    * must be the same, otherwise if attitude has it
    * higher than GPS, then GPS is not being received.
    * Asio boost serial connection reader would be nice.
    * Read asynchornously both values- TODO.
    ****************************************************/
    requestDataStream(MAVLINK_MSG_ID_ATTITUDE,            10000);   // 10 Hz
    requestDataStream(MAVLINK_MSG_ID_GLOBAL_POSITION_INT, 10000);   // 10 Hz 
    requestDataStream(MAVLINK_MSG_ID_HEARTBEAT,           1000000); // 1 Hz
    
    /****************************************************
    * Main loop for receiving telemetry
    ****************************************************/
	while (m_running.load()) { 
        DWORD dwBytesRead = 0;
        uint8_t raw_data;

        // Telemetry data
        // Angular
        float roll{0.0};
        float pitch{0.0};
        float yaw{0.0};
        
        // GPS
        float lat{0.0};
        float lon{0.0};
        float alt{0.0};
        
        // Read data
        if (ReadFile(m_comSerial, &raw_data, 1, &dwBytesRead, NULL) &&
            dwBytesRead == 1) {
          if (mavlink_parse_char(MAVLINK_COMM_1, raw_data, &message, &status) ==
              1) {
            switch (message.msgid) {
                case MAVLINK_MSG_ID_ATTITUDE: {
                  mavlink_attitude_t attitude;
                  mavlink_msg_attitude_decode(&message, &attitude);
                  roll  = attitude.roll;
                  pitch = attitude.pitch;
                  yaw   = attitude.yaw;
                } break;
                
                case MAVLINK_MSG_ID_GLOBAL_POSITION_INT: {
                  mavlink_global_position_int_t gps;
                  
                  mavlink_msg_global_position_int_decode(&message, &gps);
                  lat = gps.lat / 1E7f; // Latitude in degrees * 1E7
                  lon = gps.lon / 1E7f; // Longitude in degrees * 1E7
                  alt = gps.alt / 1E7f; // Altitude in millimeters (above MSL)
                } break;
                
                case MAVLINK_MSG_ID_HEARTBEAT: {
                  mavlink_heartbeat_t heartbeat;
                  mavlink_msg_heartbeat_decode(&message, &heartbeat);
                  switch (heartbeat.system_status) {
                      case MAV_STATE_ACTIVE: {
                        if (m_verbose) {
                          ConnectionEvent connEvent(true, "TelemetryReceiver",
                                                    "Mavlink ON");
                          m_publisher->publish(EventType::CONNECTION_UPDATE,
                                               connEvent);
                        }
                      } break;

                      case MAV_STATE_EMERGENCY: {
                        ConnectionEvent connEvent(false, "TelemetryReceiver",
                                                  "Mavlink Heartbeat EMERGENCY");
                        m_publisher->publish(EventType::CONNECTION_UPDATE,
                                             connEvent);
                      } break;

                      case MAV_STATE_CRITICAL: {
                        ConnectionEvent connEvent(false, "TelemetryReceiver",
                                                  "Mavlink Heartbeat CRITICAL");
                        m_publisher->publish(EventType::CONNECTION_UPDATE,
                                             connEvent);
                      } break;
                       
                      default: {
                        if (m_verbose) {
                          ConnectionEvent connEvent(
                              true, "TelemetryReceiver",
                              "Mavlink Heartbeat UNDEFINED");
                          m_publisher->publish(EventType::CONNECTION_UPDATE,
                                               connEvent);
                        }
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

            // After collecting mavlink telemetry aggregate them into vector
            // TODO: synchronize angular position with GPS. Right now
            // m_currTelemetry has either angular position and (0,0,0) for GPS or otherwise.
            m_currTelemetry = {roll, pitch, yaw, lat, lon, alt};
            CoordinatesUtils::getGPS2UCS(
                m_currTelemetry[3], // latitude
                m_currTelemetry[4], // longtitude
                m_homePos.latitude,
                m_homePos.longitude, 
                m_metersPerDegree);
            
            registerTelemetryEvent_();
          } 

        } else {
          ConnectionEvent connEvent(false, "TelemetryReceiver",
                                    "Serial connection error");
          m_publisher->publish(EventType::CONNECTION_UPDATE, connEvent);
        }
	}
}

void TelemetryReceiver::stop_() { 
	if (m_verbose) {
        std::cout << "TelemetryReceiver: terminating\n";
    }
	m_running.store(false);
    CloseHandle(m_comSerial);
}

void TelemetryReceiver::registerTelemetryEvent_() { 
	TelemetryEvent telemetry(m_currTelemetry);
	m_publisher->publish(EventType::TELEMETRY_UPDATE, telemetry); 
}
