#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <atomic>
#include <math.h>
#include <iostream>

#include <common/mavlink.h>

#include "base/ITelemetryReceiver.h"
#include "EventsBus.h"


/**
 * @class TelemetryReceiver
 * @brief Telemetry receiver implementation.
 */
class TelemetryReceiver : public ITelemetryReceiver {
public:

	/**
	 * @brief Constructor.
	 * @param bus: EventsBus reference in order to access publisher
	 * @param isVerbose: logs verbosity flag.
	 */
	explicit TelemetryReceiver(EventsBus &bus, const std::string& portCom, bool isVerbose=false); 
	~TelemetryReceiver() = default;


private:

   /**
   * @brief Begin receiving telemetry.
   */
   void receive_() override final;

    /**
    * @brief Stop receiving telemetry.
    */
    void stop_() override final;

	/**
    * @brief Register received telemetry to the EventBus.
    */
	void registerTelemetryEvent_() override final;

    /****************************************************
    * UAV connection specification
    ****************************************************/
    const std::string m_portCom;
    HANDLE m_comSerial;
    DCB m_dcbSerialParams;

    /****************************************************
    * Logging
    *****************************************************/
    bool m_verbose;

	/****************************************************
    * Publishing
    *****************************************************/
	IPublisher *m_publisher;
    std::vector<float> m_currTelemetry;

	/****************************************************
    * Synchronization
    *****************************************************/
    std::atomic_bool m_running; // Flag which indicates if receiver loop should run.
                                // It's set to be atomic_bool in order to be securely 
                                // - set by TelemetryReceiver::stop
                                // - checked by TelemetryReceiver::receive which runs on a separate thread
};

