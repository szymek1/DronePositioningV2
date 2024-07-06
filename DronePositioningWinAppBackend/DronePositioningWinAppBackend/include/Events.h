#pragma once

#include <vector>
#include <string>

#include "base/IEvent.h"


struct TelemetryEvent : public IEvent {
  TelemetryEvent(std::vector<float> data);
  const std::vector<float> telemetry;
};

struct ConnectionEvent : public IEvent {
  ConnectionEvent(bool status, const std::string& who);
  const std::string whichComponent;
  const bool isConnected;
};

struct AppTerminationEvent : public IEvent {
  AppTerminationEvent(bool status);
  const bool isAppTerminating;
};