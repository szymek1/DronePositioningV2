/**
 * @file Events.cpp
 * @brief Events construction code.
 *
 * @details This file contains the declaration for application runtime events.
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

#include "../include/Events.h"


TelemetryEvent::TelemetryEvent(std::vector<float> data) 
	: telemetry(data) {}

ConnectionEvent::ConnectionEvent(bool status, const std::string &who,
                                 const std::string &message)
    : isConnected(status), whichComponent(who), connMess(message) {}

AppTerminationEvent::AppTerminationEvent(bool status)
    : isAppTerminating(status) {}