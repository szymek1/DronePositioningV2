#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <atomic>

#include "base/ITelemetryReceiver.h"
#include "EventsBus.h"


/**
 * @class TelemetryReceiver
 * @brief Telemetry receiver implementation.
 */
class TelemetryReceiver : public ITelemetryReceiver {
public:

	explicit TelemetryReceiver(EventsBus &bus, std::shared_ptr<IProcessor> processor, std::atomic_bool& appStatus);
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
    * @brief Register received telemetry to the EventBus
    */
	void registerTelemetryEvent_() override final;


	/****************************************************
    * Publishing
    *****************************************************/
	IPublisher *m_publisher;
    std::vector<float> m_currTelemetry;
    std::shared_ptr<IProcessor> m_telemetryProcessor;

	/****************************************************
     * Synchronization
     *****************************************************/
    std::atomic_bool& m_isRunning;
};

