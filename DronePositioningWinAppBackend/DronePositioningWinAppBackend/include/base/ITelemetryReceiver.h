/**
 * @file ITelemetryReceiver.h
 * @brief Interface for telemetry receiver.
 *
 * @details This file contains the declaration of telemtry receiver interface.
 *
 * @author Szymon Bogus
 * @date 2024-05-22
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 */

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
