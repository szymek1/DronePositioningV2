#pragma once

#include <vector>
#include <string>
#include <variant>

#include "base/IEvent.h"


/**
 * @brief Event holding new telemetry.
 */
struct TelemetryEvent : public IEvent {

  /**
   * @brief Constructor
   * @param data: vector of telemtry.
   */
  TelemetryEvent(std::vector<float> data);
  const std::vector<float> telemetry;
};

/**
 * @brief Event holding new connection status.
 */
struct ConnectionEvent : public IEvent {

  /**
   * @brief Constructor.
   * @param status: current connection status from a component.
   * @param who: component name.
   */
  ConnectionEvent(bool status, const std::string& who);
  const std::string whichComponent;
  const bool isConnected;
};

/**
 * @brief Event holding app termination call.
 */
struct AppTerminationEvent : public IEvent {

  /**
   * @brief Constructor.
   * @param status: app termination flag.
   */
  AppTerminationEvent(bool status);
  const bool isAppTerminating;
};

using Event =
    std::variant<TelemetryEvent, ConnectionEvent, AppTerminationEvent>;
