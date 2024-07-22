#pragma once

#include <iostream>

#include "base/IProcessor.h"
#include "base/ISubscriber.h"


class TelemetryProcessor : public ISubscriber, public IProcessor {
public:
  TelemetryProcessor(bool isVerbose=false);

private:
  /**
  * @brief Process telemetry.
  * @param telemetry new telemetry to process.
  */
  void process_(const std::vector<float> &telemetry) override final;

  /**
  * @brief Generate report.
  */
  void generateReport_() override final;

  /**
  * @brief Handle new telemtry
  * @param event new telemetry data
  */
  void onEvent_(const TelemetryEvent &event) override final;

  /****************************************************
  * Logging
  *****************************************************/
  bool m_verbose;
};

