#pragma once

#include <iostream>
#include <vector>

#include "base/ITelemetrySender.h"
#include "base/ISubscriber.h"


/**
* @class TelemetrySender
* @brief Implementation of UDP telemetry sender.
*/
class TelemetrySender : public ITelemetrySender, public ISubscriber {
public:
    explicit TelemetrySender();

private:

    /**
    * @brief Send telemetry via UDP protocol.
    * @param telemetry new telemetry extracted from the event
    */
    void sendPosition_(const std::vector<float> &telemetry) override final;

    /**
     * @brief Send telemetry to external platform
     * @param event new telemetry data
     */
    void onEvent_(const TelemetryEvent &event) override final;
};

