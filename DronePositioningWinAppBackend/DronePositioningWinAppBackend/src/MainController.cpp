#include "../include/MainController.h"

MainController::MainController(const std::filesystem::path &flightConfigPath,
                               EventsBus &bus, const std::string &portCom,
                               bool isVerbose) : m_bus(bus), m_verbose(isVerbose), m_portCom(portCom) {
	if (initialize_(flightConfigPath)) {
		std::cout << "Configuration loaded\n";
        m_publisher = m_bus.getPublisher();

		m_isRunning.store(false);
        {
          std::lock_guard<std::mutex> lk(m_isPrematureTerminateMtx);
          m_isPrematureTerminate = false;
        }

        if (isVerbose) {
            m_flightConfig->displayLoadedConfiguration();
        }
    } else {
        std::cout << "Couldnt load the configuration\n";
	}
}

MainController::~MainController() {
  std::lock_guard<std::mutex> lk(m_isPrematureTerminateMtx);
  if (!m_isPrematureTerminate) {
    m_bus.removeSubscriber(EventType::TELEMETRY_UPDATE,  m_telemetrySender);
    m_bus.removeSubscriber(EventType::TELEMETRY_UPDATE,  m_telemetryProcessor);
    m_bus.removeSubscriber(EventType::CONNECTION_UPDATE, m_connectionManager);
    m_bus.removeSubscriber(EventType::APP_TERMINATION,   m_connectionManager);
  }
}

void MainController::run() {
	m_isRunning.store(true);
    std::cout << "Running MainController\n";

    // try-catch block associated with errors due to serial connection errors
    bool isSerialError{false}; // If true, then nothing else has to be instanitated and this 
                               // method can begin to finish
    try { 
        m_telemetryReceiver = std::make_shared<TelemetryReceiver>(
            m_bus, m_portCom, m_verbose);
    } catch (const std::runtime_error &telemetryRcvrErr) {
        isSerialError = true;
        {
          std::lock_guard<std::mutex> lk(m_isPrematureTerminateMtx);
          m_isPrematureTerminate = false;
        }
        std::cout << telemetryRcvrErr.what() << std::endl;
    }

    if (!isSerialError) {
      ConnectionConfigurationInfo connectionInfo =
          m_flightConfig->getConnectionConfigurationInfo();

      m_telemetryProcessor = std::make_shared<TelemetryProcessor>(m_verbose);

      m_telemetrySender = std::make_shared<TelemetrySender>(
          m_bus, connectionInfo.remoteIp, std::to_string(connectionInfo.port),
          m_verbose);

      auto m_telemetryReceiverConn =
          std::dynamic_pointer_cast<ITelemetryReceiver>(m_telemetryReceiver);
      auto m_telemetrySenderConn =
          std::dynamic_pointer_cast<ITelemetrySender>(m_telemetrySender);

      if (!m_telemetryReceiverConn || !m_telemetrySenderConn) {
        throw std::runtime_error(
            "Couldnt create telemetry utilities. Aborting...");
      }

      m_connectionManager = std::make_shared<ConnectionManager>(
          m_telemetryReceiverConn, m_telemetrySenderConn, m_verbose);

      m_bus.addSubscriber(EventType::TELEMETRY_UPDATE,  m_telemetrySender);
      m_bus.addSubscriber(EventType::TELEMETRY_UPDATE,  m_telemetryProcessor);
      m_bus.addSubscriber(EventType::CONNECTION_UPDATE, m_connectionManager);
      m_bus.addSubscriber(EventType::APP_TERMINATION,   m_connectionManager);

      auto connMgr =
          std::dynamic_pointer_cast<ConnectionManager>(m_connectionManager);

      if (connMgr) {
        m_connectionManagerThread =
            std::jthread(&ConnectionManager::connect, connMgr);

        while (m_isRunning.load()) {
        }

      } else {
        throw std::runtime_error(
            "Couldnt create telemetry utilities. Aborting...");
      }
    }
	
    std::cout << "Shutting down...\n";
    if (isSerialError) {
      // Just to be clear, even though this is a shutdown it is not intended
      // to end up like this, thus excpetion is being thorwn
      throw std::runtime_error("Premature termination");
    }
}

bool MainController::shutdown() {
    { 
      std::lock_guard<std::mutex> lk(m_isPrematureTerminateMtx);
      if (!m_isPrematureTerminate) {
        m_isRunning.store(false);

        // Announce shutdown
        AppTerminationEvent terminationEvent(true);
        m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
      }
    }
	return true; 
}

bool MainController::initialize_(const std::filesystem::path& flightConfigPath)
{
	try {
		m_flightConfig = configuration::ConfigurationManager::loadConfig(flightConfigPath);
		return true;
	}
	catch (const std::runtime_error& configuration_load_err) {
		std::cout << configuration_load_err.what() << std::endl;
		return false;
	}
}
