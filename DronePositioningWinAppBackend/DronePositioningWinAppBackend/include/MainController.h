/**
 * @file MainController.h
 * @brief Main contorller of the application.
 *
 * @details This file contains the declaration of MainController object which steers entire application.
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

#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <iostream>

#include "base/IPublisher.h"
#include "base/ITelemetryReceiver.h"
#include "base/ITelemetrySender.h"
#include "base/IProcessor.h"
#include "TelemetryProcessor.h"
#include "TelemetryReceiver.h"
#include "TelemetrySender.h"
#include "ConnectionManager.h"
#include "ConfigurationManager.h"
#include "EventsBus.h"

/**
 * @class MainController
 * @brief This class is responsible for managing the main application logic.
 * That includes:
 * - Initializing the application with the flight configuration file.
 * - Injecting TelemetryReceiver and TelemetrySender to ConnnectionManager and handling subscription to the events.
 * - Launching in the separate threads: ConnectionManager, EventsBus.
 * - Terminating the application.
 */
class MainController
{
public:

	/**
	* @brief Construct a new Main Controller object.
	*
	* @param flightConfigPath: path to the flight configuration file.
	*/
    explicit MainController(const std::filesystem::path &flightConfigPath, EventsBus& bus, const std::string& portCom,
                          bool isVerbose = false);

	/**
	 * @brief Deconstructor for the Main Controller object.
	 */
    ~MainController();

	/**
	* @brief Run the main application. Begin with initialization: reading and parsing flight configuration file.
	*		 Then launch threads.
	*/
	void run();

	/**
	 * @brief Shutdown the application. Stop all threads and clean up resources.
	 * @return Boolean value indicating if the shutdown was successful.
	 */
	bool shutdown();

private:

	/**
	 * @brief Call ConfigurationManager  read and parse, then load necessary flight data.
	 * @return Boolean value indicating if the initialization was successful.
	 */
	bool initialize_(const std::filesystem::path& flightConfigPath);

	bool m_isConfigGood{false}; // flag for signaling that config file indeed
                               // exists and could be loaded
						 // by ConfigurationManager

	/****************************************************
	* Telemetry Utilities
	****************************************************/
	std::unique_ptr<configuration::FlightConfig> m_flightConfig;
    std::shared_ptr<ITelemetryReceiver> m_telemetryReceiver;
    std::shared_ptr<ISubscriber> m_telemetryProcessor; 
    std::shared_ptr<ISubscriber> m_telemetrySender;
    const std::string m_portCom;


	/****************************************************
	* Connection & data exchange utilities
	****************************************************/
    std::shared_ptr<ISubscriber> m_connectionManager;
    EventsBus& m_bus;
    IPublisher *m_publisher;

	/****************************************************
	* Threading
	*****************************************************/
	std::jthread m_connectionManagerThread;
	std::atomic_bool m_isRunning; // Flag to indicate if the application is running
    std::mutex m_isPrematureTerminateMtx;
    bool m_isPrematureTerminate; // Flag which indicates that program will terminate prematurely.
								 // It is used to avoid MainController access EventsBus methods when
								 // no subscription has been done up to this point (used when serial connection errors occur).
								 // Such situation of trying to remove subscriber of nonexisting type from the map could result in deadlock.

	/****************************************************
	* Logging
	*****************************************************/
	bool m_verbose;
};