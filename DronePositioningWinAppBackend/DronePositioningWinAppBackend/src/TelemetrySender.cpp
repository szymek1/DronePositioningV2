#include "../include/TelemetrySender.h"


TelemetrySender::TelemetrySender() {
  std::cout << "TelemetrySender instantiated" << std::endl;
}

void TelemetrySender::onEvent_(const TelemetryEvent &event) {
  sendPosition_(event.telemetry);
}

void TelemetrySender::sendPosition_(const std::vector<float> &telemetry) {
  for (const auto &t : telemetry) {
    std::cout << t << " ";
  }
  std::cout << "\n";
}
