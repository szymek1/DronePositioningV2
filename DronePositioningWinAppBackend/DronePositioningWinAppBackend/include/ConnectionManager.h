#pragma once

#include <thread>
#include <memory>
#include <atomic>

#include "base/ISubscriber.h"
#include "base/ITelemetryReceiver.h"
#include "base/ITelemetrySender.h"
#include "FlightConfig.h"
#include "EventsBus.h"


/**
* @class ConnectionManager
* @brief Class responsible for managing connection UAV-PC and PC-HL2.
*		 It monitors connection and manages threads of TelemetryReceiver, TelemetrySender
*/
class ConnectionManager : public ISubscriber {
public:
  explicit ConnectionManager(
                             std::shared_ptr<ITelemetryReceiver> receiver,
                             std::shared_ptr<ITelemetrySender> sender,
                             bool isVerbose = false);
  ~ConnectionManager() = default;

  /**
  * @brief Connect all services: TelemetryReceiver and TelemetrySender
  */
  void connect();

  /**
  * @brief Disconnect all services: TelemetryReceiver and TelemetrySender
  */
  void disconnect();

private:

   /**
   * @brief Handle new connection event
   * @param event new connection status
   */
  void onEvent_(const ConnectionEvent &event) override final;

  /**
   * @brief Handle app termination call
   * @param event app termination call
   */
  void onEvent_(const AppTerminationEvent &event) override final;

  /****************************************************
  * Telemetry Utilities
  ****************************************************/
  std::shared_ptr<ITelemetryReceiver> m_telemetryReceiver;
  std::shared_ptr<ITelemetrySender> m_telemetrySender;

  /****************************************************
  * Threading
  *****************************************************/
  std::jthread m_receiverThread;

  /****************************************************
  * Logging
  *****************************************************/
  bool m_verbose;

};

