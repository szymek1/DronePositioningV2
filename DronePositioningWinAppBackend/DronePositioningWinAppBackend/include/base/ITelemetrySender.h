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
	* @param telemetry new telemetry to process.
    */
    void sendPosition(const std::vector<float> &telemetry);

private:

	/**
	* @brief Send telemetry via given method.
	* @param telemetry new telemetry to process.
	*/
    virtual void sendPosition_(const std::vector<float> &telemetry) = 0;
};

