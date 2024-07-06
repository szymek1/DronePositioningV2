#pragma once

#include <vector>

#include "../EventType.h"


/**
 * @class IObserver
 * @brief Interface defining base observer.
 */
class ISubscriber {
public:
  virtual ~ISubscriber() = default;
};


/**
 * @class INewTelemetryObserver
 * @brief Interface defining observer for EventType::TELEMETRY_UPDATE
 */
class INewTelemetryObserver: public ISubscriber {
public:
  virtual ~INewTelemetryObserver() = default;
  virtual void onNotify(const std::vector<float> &data) = 0;
};


/**
 * @class IConnectionStatusObserver
 * @brief Interface defining observer for EventType::CONNECTION_UPDATE
 */
class IConnectionStatusObserver: public ISubscriber {
public:
  virtual ~IConnectionStatusObserver() = default;
  virtual void onNotify(const bool &isConnected) = 0;
};


/**
 * @class IAppTerminateObserver
 * @brief Interface defining observer for EventType::APP_TERMINATION
 */
class IAppTerminateObserver: public ISubscriber {
public:
  virtual ~IAppTerminateObserver() = default;
  virtual void onNotify() = 0;
};