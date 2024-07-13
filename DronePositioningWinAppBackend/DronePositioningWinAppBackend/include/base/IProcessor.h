#pragma once

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
    */
	void process();

private:

    /**
    * @brief Process telemetry.
    */
    virtual void process_() = 0;

	/**
	* @brief Generate report.
	*/
	virtual void generateReport_() = 0;
};

