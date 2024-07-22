#pragma once

#include "base/IProcessor.h"
#include "base/ISubscriber.h"

class TelemetryProcessor : public ISubscriber, public IProcessor {
public:
  TelemetryProcessor() = default;

private:
  /**
  * @brief Process telemetry.
  */
  void process_() override final;

  /**
  * @brief Generate report.
  */
  void generateReport_() override final;

  /**
  * @brief Handle new telemtry
  * @param event new telemetry data
  */
  void onEvent_(const TelemetryEvent &event) override final;
};

