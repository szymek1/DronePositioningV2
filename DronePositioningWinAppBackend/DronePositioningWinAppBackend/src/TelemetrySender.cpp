#include "../include/TelemetrySender.h"


TelemetrySender::TelemetrySender(EventsBus &bus, const std::string &ip,
                                 const std::string &port,
                                 bool isVerbose)
    : m_verbose(isVerbose), m_ip(ip.c_str()), m_port(std::stoi(port)) {

  m_publisher = bus.getPublisher();

  m_winSockVersion = MAKEWORD(2, 2);

  // Starting WinSock
  int wsOk = WSAStartup(m_winSockVersion, &m_winSockdata);
  if (wsOk != 0) {
    std::cout << "Couldnt start WinSock: " << wsOk << "\n";
    return;
  }

  // Connecting to the remote target
  m_remoteTarget.sin_family = AF_INET;              // IPv4 address
  inet_pton(AF_INET, m_ip, &m_remoteTarget.sin_addr);
  m_remoteTarget.sin_port = htons(m_port); 
  
  // Socket creation
  if ((m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == INVALID_SOCKET) {
    std::cout << "Failed to create socket: " << WSAGetLastError() << "\n";
    return;
  }

  if (m_verbose) {
    std::cout << "TelemetrySender: instantiated"
              << "\n";
  }
}

TelemetrySender::~TelemetrySender() {
  closesocket(m_socket);
  WSACleanup();
  m_publisher = nullptr;
}

void TelemetrySender::onEvent_(const TelemetryEvent &event) {
  sendPosition(event.telemetry);
}

void TelemetrySender::sendPosition_(const std::vector<float> &telemetry) {
  // Message preparation
  std::string message;
  for (const float &value : telemetry) {
    message += std::to_string(value) + " ";
  }
  
  int sendOK = sendto(m_socket, message.c_str(), message.size() + 1, 0,
                      (sockaddr *)&m_remoteTarget, sizeof(m_remoteTarget));

  // TODO: send to the bus error message
  if (sendOK == SOCKET_ERROR) {
    std::cout << "Couldnt send package: " << WSAGetLastError() << "\n";
  }

}
