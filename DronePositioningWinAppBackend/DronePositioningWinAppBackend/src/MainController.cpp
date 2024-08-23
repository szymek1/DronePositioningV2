/**
 * @file MainController.cpp
 * @brief Code for general control of application.
 *
 * @details This file contains the declaration of code reponsible for:
 *          - intialization
 *          - objects instantiation
 *          - running the application
 *          - shutting down the application
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
 * @note Unfortunately, for an abrupt, emergency shutdown something doesn't seem to stop
 *       and the while loop in MainController::run doesn't break. I tried implementing different approaches to break it,
 *       but for emergency shutdown they all ended-up with deadlock. Maybe a replacement of the while loop
 *       with a condition variable would help.
 */

#include "../include/MainController.h"


MainController::MainController(const std::filesystem::path &flightConfigPath,
                               EventsBus &bus, const std::string &portCom,
                               bool isVerbose) : m_bus(bus), m_verbose(isVerbose), m_portCom(portCom) {
	if (initialize_(flightConfigPath)) {
		std::cout << "Configuration loaded\n";
        m_isConfigGood = true;
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
  m_publisher = nullptr;
}

void MainController::run() {
  if (m_isConfigGood) {
    m_isRunning.store(true);
    std::cout << "Running MainController\n";

    // try-catch block associated with errors due to serial connection errors
    bool isSerialError{
        false}; // If true, then nothing else has to be instanitated and this
                // method can begin to finish
    try {
      OperatorPosition homePos = m_flightConfig->getOperatorPosition();
      m_telemetryReceiver =
          std::make_shared<TelemetryReceiver>(m_bus, homePos, m_portCom, m_verbose);
    } catch (const std::runtime_error &telemetryRcvrErr) {
      isSerialError = true;
      {
        std::lock_guard<std::mutex> lk(m_isPrematureTerminateMtx);
        m_isPrematureTerminate = true;
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

      if (!m_telemetryReceiverConn) {
        throw std::runtime_error(
            "Couldnt create telemetry utilities. Aborting...");
      }

      m_connectionManager = std::make_shared<ConnectionManager>(
          m_telemetryReceiverConn, m_verbose);

      m_bus.addSubscriber(EventType::TELEMETRY_UPDATE, m_telemetrySender);
      m_bus.addSubscriber(EventType::TELEMETRY_UPDATE, m_telemetryProcessor);
      m_bus.addSubscriber(EventType::CONNECTION_UPDATE, m_connectionManager);
      m_bus.addSubscriber(EventType::APP_TERMINATION, m_connectionManager);

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
  } else {
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
