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
    */
    void sendPosition(const std::vector<float> &telemetry);

private:

	/**
	* @brief Send telemetry via given method.
	*/
    virtual void sendPosition_(const std::vector<float> &telemetry) = 0;
};

