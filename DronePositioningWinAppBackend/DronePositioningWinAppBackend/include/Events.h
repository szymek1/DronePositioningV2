/**
 * @file Events.h
 * @brief Possible events which can occur during application run.
 *
 * @details This file contains structures declaring apllication events.
 *
 * @author Szymon Bogus
 * @date 2024-07-06
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 */

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
   * @param message: connection status message.
   */
  ConnectionEvent(bool status, const std::string& who, const std::string& message);
  const bool isConnected;
  const std::string whichComponent;
  const std::string connMess;
  
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
