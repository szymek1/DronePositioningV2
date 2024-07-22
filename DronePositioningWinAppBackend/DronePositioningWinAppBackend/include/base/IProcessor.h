#pragma once

#include <vector>


/**
 * @class IProcessor
 * @brief Interface defining telemetry processor.
 */
class IProcessor
{
public:

	virtual ~IProcessor() = default;

	/**
    * @brief Process telemetry- call appropriate implementation.
    * @param telemetry new telemetry to process.
    */
    void process(const std::vector<float> &telemetry);

private:

    /**
    * @brief Process telemetry.
    * @param telemetry new telemetry to process.
    */
    virtual void process_(const std::vector<float> &telemetry) = 0;

	/**
	* @brief Generate report.
	*/
	virtual void generateReport_() = 0;
};

