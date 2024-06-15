#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <memory>
#include <iostream>

#include "ConnectionManager.h"
#include "ConfigurationManager.h"
#include "EventsBus.h"
#include "TelemetryReceiver.h"
#include "TelemetrySender.h"
#include "TelemetryProcessor.h"

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
	* @brief Construct a new Main Controller object
	*
	* @param flightConfigPath - path to the flight configuration file
	*/
	explicit MainController(const std::filesystem::path& flightConfigPath, bool isVerbose = false);

	/**
	 * @brief Deconstructor for the Main Controller object
	 */
	~MainController() = default; // temporary

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

	/****************************************************
	 * Telemetry Utilities
	 ****************************************************/
	std::unique_ptr<configuration::FlightConfig> m_flightConfig;
	// TelemetryReceiver m_telemetryReceiverClient;
	// TelemetrySender  m_telemetrySenderServer;
	// TelemetryProcessor m_telemetryProcessor;

	/****************************************************
	* Connection & data exchange utilities
	****************************************************/
	ConnectionManager m_connectionManager;
	EventsBus m_eventBus;

	/****************************************************
	* Threading
	*****************************************************/
	std::thread m_connectionManagerThread;
	std::thread m_eventBusThread;
	std::atomic<bool> m_isRunning{ false }; // Flag to indicate if the application is running

	/****************************************************
	* Logging
	*****************************************************/
	bool m_verbose;
};