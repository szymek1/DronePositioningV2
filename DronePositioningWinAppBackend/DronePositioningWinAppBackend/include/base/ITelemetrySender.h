#pragma once

#include "IObservable.h"

/**
 * @class ITelemetrySender
 * @brief Interface defining telemetry sender.
 */
class ITelemetrySender
{
public:

	/**
	* @brief Default destructor.
	*/
	virtual ~ITelemetrySender() = default;

protected:

	/**
	* @brief Send telemetry via UDP.
	*/
	virtual void sendPosition_() = 0;
};

