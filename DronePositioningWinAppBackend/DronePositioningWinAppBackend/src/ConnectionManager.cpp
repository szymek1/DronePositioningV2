#include "../include/ConnectionManager.h"


ConnectionManager::ConnectionManager(
    std::shared_ptr<ITelemetryReceiver> receiver,
    std::shared_ptr<ITelemetrySender> sender,
    bool isVerbose)
    : m_telemetryReceiver(receiver), m_telemetrySender(sender), 
      m_verbose(isVerbose) {

    if (m_verbose) {
        std::cout << "ConnectionManager: instanitated" << std::endl;
    }

}

void ConnectionManager::connect() {
  m_receiverThread =
      std::jthread(&ITelemetryReceiver::receive, m_telemetryReceiver);
}

void ConnectionManager::disconnect() { 
    m_telemetryReceiver->stop();
    if (m_verbose) {
        std::cout << "ConnectionManager: terminating" << std::endl;
    }
}

void ConnectionManager::onEvent_(const ConnectionEvent &event) {}

void ConnectionManager::onEvent_(const AppTerminationEvent &event) {
  disconnect();
}


