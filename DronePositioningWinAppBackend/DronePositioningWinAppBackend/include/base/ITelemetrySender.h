#pragma once

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
	void sendPosition();

private:

	/**
	* @brief Send telemetry via given method.
	*/
	virtual void sendPosition_() = 0;
};

