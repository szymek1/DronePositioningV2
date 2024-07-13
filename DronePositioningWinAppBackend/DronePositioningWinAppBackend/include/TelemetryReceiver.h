#pragma once

#include <vector>
#include <thread>
#include <memory>
#include <condition_variable>
#include <atomic>

#include "base/ITelemetryReceiver.h"
#include "EventsBus.h"

class TelemetryReceiver : public ITelemetryReceiver {
public:

	explicit TelemetryReceiver(EventsBus &bus);
	~TelemetryReceiver() = default;

	/**
    * @brief Begin receiving telemetry.
    */
	void receive() override final;

	/**
    * @brief Stop receiving telemetry.
    */
	void stop() override final;

private:

	/**
    * @brief Register received telemetry to the EventBus
    */
	void registerTelemetryEvent_() override final;


	/****************************************************
    * Publishing
    *****************************************************/
	IPublisher *m_publisher;
    std::vector<float> m_currTelemetry;
    std::unique_ptr<IProcessor> m_telemetryProcessor;

	/****************************************************
     * Synchronization
     *****************************************************/
    std::atomic_bool m_telemetryRunning;
    std::weak_ptr<std::condition_variable> m_connectionRunningConVar;
    std::weak_ptr<std::atomic_bool> m_isRunningPtr;
};

