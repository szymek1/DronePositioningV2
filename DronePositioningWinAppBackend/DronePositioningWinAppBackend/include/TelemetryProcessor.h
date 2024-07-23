#pragma once

#include <iostream>

#include "base/IProcessor.h"
#include "base/ISubscriber.h"


/**
 * @class TelemetryProcessor
 * @brief Class representing specific telemetry processor implementation.
 *		  This class implements both ISubscriber and IProcessor interfaces
 *		  in order to be able to process data and subscriber to events via EventsBus
 */
class TelemetryProcessor : public ISubscriber, public IProcessor {
public:

  /**
   * @brief Constructor.
   * @param isVerbose: logs verbosity flag. 
   */
  explicit TelemetryProcessor(bool isVerbose=false);

private:
  /**
  * @brief Process telemetry.
  * @param telemetry: new telemetry to process.
  */
  void process_(const std::vector<float> &telemetry) override final;

  /**
  * @brief Generate report.
  */
  void generateReport_() override final;

  /**
  * @brief Handle new telemetry.
  * @param event: new telemetry data.
  */
  void onEvent_(const TelemetryEvent &event) override final;

  /****************************************************
  * Logging
  *****************************************************/
  bool m_verbose;
};

