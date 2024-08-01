#pragma once

#include <string>
#include <thread>
#include <atomic>
#include <memory>
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
    explicit MainController(const std::filesystem::path &flightConfigPath, EventsBus& bus,
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

	/****************************************************
	* Telemetry Utilities
	****************************************************/
	std::unique_ptr<configuration::FlightConfig> m_flightConfig;
    std::shared_ptr<ITelemetryReceiver> m_telemetryReceiver;
    std::shared_ptr<ISubscriber> m_telemetryProcessor; 
    std::shared_ptr<ISubscriber> m_telemetrySender;


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

	/****************************************************
	* Logging
	*****************************************************/
	bool m_verbose;
};