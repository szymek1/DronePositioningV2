/**
 * @file ITelemetrySender.cpp
 * @brief Common code for telemetry senders.
 *
 * @details This file contains the declaration of telemetry sedner interface, which
 *			all telemetry senders have to use.
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

#include "ITelemetrySender.h"


void ITelemetrySender::sendPosition(const std::vector<float> &telemetry) { 
	sendPosition_(telemetry); 
}