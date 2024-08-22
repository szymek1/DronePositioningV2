/**
 * @file ConnectionManager.cpp
 * @brief Code responsible for managing UAV-Proxy machine connection and ConnectionEvent handling.
 *
 * @details This file contains the declaration for the code which is responsible for 
 *          handling the connection between UAV and Proxy machine, as well as handling ConnectionEvent
 *          thath can occur during runtime.
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
 * @note Apparently here is the field for improvement in terms of handling 
 *       AppTerminationEvent that could help fight occasional deadlocks.
 */

#include "../include/ConnectionManager.h"


ConnectionManager::ConnectionManager(
    std::shared_ptr<ITelemetryReceiver> receiver,
    std::shared_ptr<ITelemetrySender> sender,
    bool isVerbose)
    : m_telemetryReceiver(receiver), m_telemetrySender(sender), 
      m_verbose(isVerbose) {

    if (m_verbose) {
        std::cout << "ConnectionManager: instanitated\n";
    }

}

void ConnectionManager::connect() {
  
  m_receiverThread =
      std::jthread(&ITelemetryReceiver::receive, m_telemetryReceiver);  
}

void ConnectionManager::disconnect() { 
    if (m_verbose) {
        std::cout << "ConnectionManager: stopping TelemetryReceiver\n";
    }
    m_telemetryReceiver->stop();
}

void ConnectionManager::onEvent_(const ConnectionEvent &event) {
  std::cout << "CONNECTION STAUTS:\n"
            << "From: " << event.whichComponent << "\n"
            << "Is connected: " << event.isConnected << "\n"
            << "Message: " << event.connMess << "\n";
}

void ConnectionManager::onEvent_(const AppTerminationEvent &event) {
  if (event.isAppTerminating) {
    if (m_verbose) {
      std::cout << "ConnectionManager: terminating"
                << "\n";
    }
    disconnect();
  }
}


