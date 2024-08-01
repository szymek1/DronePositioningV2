#include "../include/TelemetrySender.h"


TelemetrySender::TelemetrySender(EventsBus &bus, const std::string &ip,
                                 const std::string &port,
                                 bool isVerbose)
    : m_verbose(isVerbose) {

  m_publisher = bus.getPublisher();
  m_winSockVersion = MAKEWORD(2, 2);

  // Starting WinSock
  // TODO: make TelemetrySender publish on ConnectionUpdate failure info
  int wsOk = WSAStartup(m_winSockVersion, &m_winSockdata);
  if (wsOk != 0) {
    std::cout << "Couldnt start WinSock: " << wsOk << std::endl;
    return;
  }

  const char *m_ip = ip.c_str();                    // Workaround as there's no conversion from
                                                    // std::string to PCSTR

  // Connecting to the remote target
  m_remoteTarget.sin_family = AF_INET;              // IPv4 address
  m_remoteTarget.sin_port = htons(std::stoi(port)); // Little to big endian conversion
  inet_pton(AF_INET, m_ip, &m_remoteTarget.sin_addr);

  // Socket creation
  m_socket = socket(AF_INET, SOCK_DGRAM, 0);

  if (m_verbose) {
    std::cout << "TelemetrySender: instantiated" << std::endl;
  }
}

TelemetrySender::~TelemetrySender() {
  closesocket(m_socket);
  WSACleanup();
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
    std::cout << "Couldnt send package: " << WSAGetLastError() << std::endl;
  }

}
