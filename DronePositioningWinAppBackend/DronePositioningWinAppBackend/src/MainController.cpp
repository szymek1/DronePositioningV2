#include "../include/MainController.h"

MainController::MainController(const std::filesystem::path &flightConfigPath,
                               EventsBus &bus, const std::string &portCom,
                               bool isVerbose) : m_bus(bus), m_verbose(isVerbose), m_portCom(portCom) {
	if (initialize_(flightConfigPath)) {
		std::cout << "Configuration loaded" << std::endl;
        m_publisher = m_bus.getPublisher();

		m_isRunning.store(false);

        if (isVerbose) {
            m_flightConfig->displayLoadedConfiguration();
        }
    } else {
        std::cout << "Couldnt load the configuration" << std::endl;
	}

	
	// TODO: prepare run method: focus on threading and proper shutdown of them, next move one to proper callback
	//		 with EventBus
}

MainController::~MainController() {
  m_bus.removeSubscriber(EventType::TELEMETRY_UPDATE, m_telemetrySender);
  m_bus.removeSubscriber(EventType::TELEMETRY_UPDATE, m_telemetryProcessor);
  m_bus.removeSubscriber(EventType::CONNECTION_UPDATE, m_connectionManager);
  m_bus.removeSubscriber(EventType::APP_TERMINATION, m_connectionManager);
}

void MainController::run() {
	m_isRunning.store(true);
    std::cout << "Running MainController" << std::endl;

	m_telemetryProcessor = std::make_shared<TelemetryProcessor>(m_verbose);
    m_telemetryReceiver = std::make_shared<TelemetryReceiver>(
        m_bus, m_portCom, m_verbose); // m_isRunning, 

    ConnectionConfigurationInfo connectionInfo =
        m_flightConfig->getConnectionConfigurationInfo();
    
    m_telemetrySender = std::make_shared<TelemetrySender>(m_bus,
        connectionInfo.remoteIp, std::to_string(connectionInfo.port), m_verbose);
    // TelemetryProcessor was removed from TelemetryReceiver constructor
    // due to appraently errors with types. It might be launched from here or there's a need
    // for a workaround

    auto m_telemetryReceiverConn =
        std::dynamic_pointer_cast<ITelemetryReceiver>(m_telemetryReceiver);
    auto m_telemetrySenderConn =
        std::dynamic_pointer_cast<ITelemetrySender>(m_telemetrySender);

    if (!m_telemetryReceiverConn || !m_telemetrySenderConn) {
      throw std::runtime_error("Couldnt create telemetry utilities. Aborting...");
    }

	m_connectionManager = std::make_shared<ConnectionManager>(
        m_telemetryReceiverConn, m_telemetrySenderConn, m_verbose);
        

	m_bus.addSubscriber(EventType::TELEMETRY_UPDATE, m_telemetrySender);
    m_bus.addSubscriber(EventType::TELEMETRY_UPDATE, m_telemetryProcessor);
    m_bus.addSubscriber(EventType::CONNECTION_UPDATE, m_connectionManager);
    m_bus.addSubscriber(EventType::APP_TERMINATION, m_connectionManager);

    // TODO: launch threads etc...
    /*
    std::mutex terminationMtx;
    std::condition_variable terminationCV;
    bool isPrematureTerminated {false};
    */
    

    auto connMgr =
        std::dynamic_pointer_cast<ConnectionManager>(m_connectionManager);
    
    if (connMgr) {
      m_connectionManagerThread =
          std::jthread(&ConnectionManager::connect, connMgr); // ,std::ref(terminationMtx), std::ref(terminationCV)
      
           
      while (m_isRunning.load()) {
        /*
        std::unique_lock<std::mutex> lock(terminationMtx);
        terminationCV.wait(
            lock, [connMgr] {
              return connMgr->isTerminated(); // deadlock somewhere here
            });
        if (connMgr->isTerminated()) {
          m_isRunning.store(false);
          isPrematureTerminated = true;
          break;
        }
        */
        
      }

      // connMgr->disconnect();
      // if (!isPrematureTerminated) {
        // AppTerminationEvent is necessary in this section
        // only if ConnectionManager hasnt been shutdown yet
        AppTerminationEvent terminationEvent{true};
        m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);
      // }
      
    } else {
        throw std::runtime_error("Couldnt create telemetry utilities. Aborting...");
    }
    
    std::cout << "Shutting down..." << std::endl;
}

bool MainController::shutdown() { 
	m_isRunning.store(false);

	// Announce shutdown
    AppTerminationEvent terminationEvent(true);
    m_publisher->publish(EventType::APP_TERMINATION, terminationEvent);

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
