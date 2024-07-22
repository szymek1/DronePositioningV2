#include "../include/TelemetrySender.h"


TelemetrySender::TelemetrySender(bool isVerbose) : m_verbose(isVerbose) {
  if (m_verbose) {
    std::cout << "TelemetrySender: instantiated" << std::endl;
  }
}

void TelemetrySender::onEvent_(const TelemetryEvent &event) {
  sendPosition(event.telemetry);
}

void TelemetrySender::sendPosition_(const std::vector<float> &telemetry) {
  if (m_verbose) {
    for (const auto &t : telemetry) {
      std::cout << t << " ";
    }
    std::cout << "\n";
  }
}
