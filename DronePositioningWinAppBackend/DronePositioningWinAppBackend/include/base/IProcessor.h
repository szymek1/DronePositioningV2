#pragma once

#include "IObservable.h"

/**
 * @class IProcessor
 * @brief Interface defining telemetry processor.
 */
class IProcessor
{
public:

	virtual ~IProcessor() = default;

	/**
	* @brief Process telemetry.
	*/
	virtual void process() = 0;

protected:

	/**
	* @brief Generate report.
	*/
	virtual void generateReport_() = 0;
};

