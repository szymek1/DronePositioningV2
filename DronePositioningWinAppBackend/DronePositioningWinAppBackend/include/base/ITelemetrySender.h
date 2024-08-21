/**
 * @file ITelemetrySender.h
 * @brief Interface for client that sends data to remote endpoint.
 *
 * @details This file contains the declaration of data sending utility interface.
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

#include <vector>

/**
 * @class ITelemetrySender
 * @brief Interface defining telemetry sender.
 */
class ITelemetrySender
{
public:

	virtual ~ITelemetrySender() = default;

	/**
    * @brief Send telemetry via given method- call appropriate implementation.
	* @param telemetry: new telemetry to process.
    */
    void sendPosition(const std::vector<float> &telemetry);

private:

	/**
	* @brief Send telemetry via given method.
	* @param telemetry: new telemetry to process.
	*/
    virtual void sendPosition_(const std::vector<float> &telemetry) = 0;
};

