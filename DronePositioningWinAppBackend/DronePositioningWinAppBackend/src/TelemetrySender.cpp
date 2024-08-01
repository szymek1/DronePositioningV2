#include "../include/TelemetrySender.h"


TelemetrySender::TelemetrySender(const std::string& ip, const std::string& port,
                                 bool isVerbose)
    : m_verbose(isVerbose), m_ioService(),
      m_socket(m_ioService,
              boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {

  boost::asio::ip::udp::resolver resolver(m_ioService);
  boost::asio::ip::udp::resolver::results_type endpoints = resolver.resolve(ip, port);
  m_remoteEndpoint = *endpoints.begin();

  if (m_verbose) {
    std::cout << "TelemetrySender: instantiated" << std::endl;
  }
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

  // Async execution
  m_socket.async_send_to(boost::asio::buffer(message), m_remoteEndpoint,
                        [this](const boost::system::error_code &error,
                               std::size_t bytes_transferred) {
                          if (m_verbose) {
                            if (!error) {
                              std::cout
                                  << "Telemetry sent: " << bytes_transferred
                                  << " bytes" << std::endl;
                            } else {
                              std::cout << "Error sending telemetry: "
                                        << error.message() << std::endl;
                            }
                          }
                        });
}
