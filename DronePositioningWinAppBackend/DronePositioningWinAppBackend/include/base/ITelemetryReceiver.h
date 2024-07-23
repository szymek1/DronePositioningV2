#pragma once

#include "IProcessor.h"

/**
* @class ITelemetryReceiver
* @brief Interface defining telemetry receiver.
*/
class ITelemetryReceiver
{
public:

	virtual ~ITelemetryReceiver() = default;

	/**
	* @brief Begin receiving telemetry- call appropriate implementation.
	*/
	void receive();

	/**
	* @brief Stop receiving telemetry- call appropriate implementation.
	*/
	void stop();

private:

	/**
	* @brief Begin receiving telemetry.
	*/
	virtual void receive_() = 0;

	/**
    * @brief Stop receiving telemetry.
    */
    virtual void stop_() = 0;

	/**
	 * @brief Register received telemetry to the EventBus.
	 */
	virtual void registerTelemetryEvent_() = 0;
};
