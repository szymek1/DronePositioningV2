/**
 * @file IProcessor.cpp
 * @brief Common code for all processors.
 *
 * @details This file contains the declaration for telemetry processor interface,
 *			which all processors have to use.
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

#include "IProcessor.h"


void IProcessor::process(const std::vector<float> &telemetry) { 
	process_(telemetry); 
	// Later figure out when and how to call
	// generateReport()_
}