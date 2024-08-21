/**
 * @file IProcessor.h
 * @brief Interface for telemetry processor.
 *
 * @details This file contains the declaration for an interface of telemetry processor.
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
 * @class IProcessor
 * @brief Interface defining telemetry processor.
 */
class IProcessor
{
public:

	virtual ~IProcessor() = default;

	/**
    * @brief Process telemetry- call appropriate implementation.
    * @param telemetry: new telemetry to process.
    */
    void process(const std::vector<float> &telemetry);

private:

    /**
    * @brief Process telemetry.
    * @param telemetry: new telemetry to process.
    */
    virtual void process_(const std::vector<float> &telemetry) = 0;

	/**
	* @brief Generate report.
	*/
	virtual void generateReport_() = 0;
};

