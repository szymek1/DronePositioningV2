#include "../include/ConnectionManager.h"


ConnectionManager::ConnectionManager(
    const configuration::FlightConfig &config,
    std::shared_ptr<ITelemetryReceiver> receiver,
    const std::shared_ptr<ITelemetrySender> sender, std::atomic_bool &appStatus,
    bool isVerbose)
    : m_config(config), m_telemetryReceiver(receiver), m_telemetrySender(sender), 
      m_isRunning(appStatus), m_verbose(isVerbose) {

    if (m_verbose) {
        // Log it
    }

}

void ConnectionManager::connect() {
  m_receiverThread =
      std::jthread(&ITelemetryReceiver::receive, m_telemetryReceiver);

  m_senderThread =
      std::jthread(&ITelemetrySender::sendPosition, m_telemetrySender);
}

void ConnectionManager::disconnect() {}

void ConnectionManager::onEvent_(const ConnectionEvent &event) {}

void ConnectionManager::onEvent_(const AppTerminationEvent &event) {
  disconnect();
}


