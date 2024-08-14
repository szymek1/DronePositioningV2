#include "../include/TelemetryProcessor.h"

TelemetryProcessor::TelemetryProcessor(bool isVerbose) : m_verbose(isVerbose) {
  if (m_verbose) {
    std::cout << "TelemetryProcessor: instantiated\n";
  }
}

void TelemetryProcessor::process_(const std::vector<float> &telemetry) {
  if (m_verbose) {
    std::cout << "TelemetryProcessor received: \n";
    for (const auto &t : telemetry) {
      std::cout << t << " ";
    }
    std::cout << "\n";
  }
}

void TelemetryProcessor::generateReport_() {}

void TelemetryProcessor::onEvent_(const TelemetryEvent &event) {
  process(event.telemetry);
}
