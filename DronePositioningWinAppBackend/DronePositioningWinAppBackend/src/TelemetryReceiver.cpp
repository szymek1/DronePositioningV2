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
    std::cout << "TelemetryReceiver: instanitated" << std::endl;
  }
}

void TelemetryReceiver::receive_() {
	// Launching Processor thread
    m_running.store(true);
	if (m_verbose) {
		std::cout << "TelemetryReceiver: running receiver thread" << std::endl;
	}

    // m_currTelemetry = {50.4, 20.4, 30.99};
    mavlink_message_t message;
    mavlink_status_t status;
    
	while (m_running.load()) { 
		// Read data
        // std::this_thread::sleep_for(std::chrono::milliseconds(500));
		// Some condition to say when we have a valid new telemetry
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
                default: {
                  std::cout << "NO TELEMETRY" << std::endl;
                }
            }
          }
        }
	}

	// This section launches after stop_() shuts down the loop.
	// It's responsible for:
	// cleaning resources, calling Processor to generate the report, shutting down Processor thread
	
}

void TelemetryReceiver::stop_() { 
	if (m_verbose) {
		std::cout << "TelemetryReceiver: terminating..." << std::endl;
    }
	m_running.store(false);
}

void TelemetryReceiver::registerTelemetryEvent_() { 
	TelemetryEvent telemetry(m_currTelemetry);
	m_publisher->publish(EventType::TELEMETRY_UPDATE, telemetry); 
}
