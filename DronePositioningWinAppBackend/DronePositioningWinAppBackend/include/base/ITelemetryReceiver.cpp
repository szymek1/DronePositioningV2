/**
 * @file ITelemetryReceiver.cpp
 * @brief Common code for all telemetry receivers.
 *
 * @details This file contains the declaration for telemetry receiver interface, which
 *			all receivers have to use.
 *
 * @author Szymon Bogus
 * @date 2024-07-13
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 */

#include "ITelemetryReceiver.h"


void ITelemetryReceiver::receive() { 
	receive_(); 
}

void ITelemetryReceiver::stop() { 
	stop_(); 
}