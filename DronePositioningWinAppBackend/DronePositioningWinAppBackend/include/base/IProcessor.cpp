#include "IProcessor.h"


void IProcessor::process(const std::vector<float> &telemetry) { 
	process_(telemetry); 
	// Later figure out when and how to call
	// generateReport()_
}