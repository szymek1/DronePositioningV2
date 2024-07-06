#pragma once

#include "IProcessor.h"
#include "IObservable.h"

/**
* @class ITelemetryReceiver
* @brief Interface defining telemetry receiver.
*/
class ITelemetryReceiver
{
public:

	virtual ~ITelemetryReceiver() = default;

	/**
	* @brief Begin receiving telemetry.
	*/
	virtual void receive() = 0;

	/**
	* @brief Stop receiving telemetry.
	*/
	virtual void stop() = 0;

protected:

	/**
	 * @brief Register received telemetry to the EventBus
	 */
	virtual void registerTelemetryEvent_() = 0;
};
