/**
 * @file TelemetryProcessor.cpp
 * @brief Code of the concrete implementation of IProcessor.
 *
 * @details This file contains the declaration of the concrete implementation of
 *          telemetry processor.
 *
 * @author Szymon Bogus
 * @date 2024-05-22
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 *
 * @note This is still a work in progress.
 */

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
