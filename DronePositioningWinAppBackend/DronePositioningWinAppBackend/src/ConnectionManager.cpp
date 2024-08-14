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


