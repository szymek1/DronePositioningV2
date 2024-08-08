#include "../include/ConnectionManager.h"


ConnectionManager::ConnectionManager(
    std::shared_ptr<ITelemetryReceiver> receiver,
    std::shared_ptr<ITelemetrySender> sender,
    bool isVerbose)
    : m_telemetryReceiver(receiver), m_telemetrySender(sender), 
      m_verbose(isVerbose) {

    // m_isAlreadyTerminated.store(false); // No negative connection status has shutdown application yet

    if (m_verbose) {
        std::cout << "ConnectionManager: instanitated" << std::endl;
    }

}

void ConnectionManager::connect() { // std::mutex &terminationMtx, std::condition_variable &terminationCV
  
  m_receiverThread =
      std::jthread(&ITelemetryReceiver::receive, m_telemetryReceiver);
  /*
  std::unique_lock<std::mutex> lock(terminationMtx);
  terminationCV.wait(lock, [this] {
    return m_isAlreadyTerminated.load() || !m_receiverThread.joinable();
  });

  if (m_isAlreadyTerminated.load()) {
    terminationCV.notify_one();
  }
  */
  
}

void ConnectionManager::disconnect() { 
    if (m_verbose) {
        std::cout << "ConnectionManager: stopping TelemetryReceiver" << std::endl;
    }
    m_telemetryReceiver->stop();
    // m_isAlreadyTerminated.store(true);
}

/*
bool ConnectionManager::isTerminated() const {
  return m_isAlreadyTerminated.load() || !m_receiverThread.joinable();
}
*/

void ConnectionManager::onEvent_(const ConnectionEvent &event) {
  std::cout << "CONNECTION STAUTS:\n"
            << "From: " << event.whichComponent << "\n"
            << "Is connected: " << event.isConnected << "\n"
            << "Message: " << event.connMess << std::endl;
}

void ConnectionManager::onEvent_(const AppTerminationEvent &event) {
  if (event.isAppTerminating) {
    if (m_verbose) {
      std::cout << "ConnectionManager: terminating" << std::endl;
    }
    disconnect();
  }
}


