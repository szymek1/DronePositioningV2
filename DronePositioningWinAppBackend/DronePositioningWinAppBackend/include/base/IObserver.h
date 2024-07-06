#pragma once

#include <vector>

#include "../EventType.h"


/**
 * @class IObserver
 * @brief Interface defining base observer.
 */
class IObserver {
public:
  virtual ~IObserver() = default;
};


/**
 * @class INewTelemetryObserver
 * @brief Interface defining observer for EventType::TELEMETRY_UPDATE
 */
class INewTelemetryObserver: public IObserver {
public:
  virtual ~INewTelemetryObserver() = default;
  virtual void onNotify(const std::vector<float> &data) = 0;
};


/**
 * @class IConnectionStatusObserver
 * @brief Interface defining observer for EventType::CONNECTION_UPDATE
 */
class IConnectionStatusObserver: public IObserver {
public:
  virtual ~IConnectionStatusObserver() = default;
  virtual void onNotify(const bool &isConnected) = 0;
};


/**
 * @class IAppTerminateObserver
 * @brief Interface defining observer for EventType::APP_TERMINATION
 */
class IAppTerminateObserver: public IObserver {
public:
  virtual ~IAppTerminateObserver() = default;
  virtual void onNotify() = 0;
};