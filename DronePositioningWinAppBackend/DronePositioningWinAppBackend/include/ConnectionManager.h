/**
 * @file ConnectionManager.h
 * @brief Connection manager.
 *
 * @details This file contains the class which manages the connection between telemetry utilities.
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

#include <thread>
#include <memory>
#include <atomic>
#include <iostream>

#include "base/ISubscriber.h"
#include "base/ITelemetryReceiver.h"
#include "EventsBus.h"


/**
* @class ConnectionManager
* @brief Class responsible for managing connection UAV-PC and PC-HL2.
*		 It monitors connection and manages threads of TelemetryReceiver, TelemetrySender.
*/
class ConnectionManager : public ISubscriber {
public:

  /**
   * @brief Constructor.
   * @param receiver: telemetry receiving object.
   * @param sender: telemetry sending object.
   * @param isVerbose: logs verbosity flag.
   */
  explicit ConnectionManager(
                             std::shared_ptr<ITelemetryReceiver> receiver,
                             bool isVerbose = false);
  ~ConnectionManager() = default;

  /**
  * @brief Connect all services: TelemetryReceiver and TelemetrySender.
  */
  void connect(); 

  /**
  * @brief Disconnect all services: TelemetryReceiver and TelemetrySender.
  */
  void disconnect();

private:

   /**
   * @brief Handle new connection event.
   * @param event new connection status.
   */
  void onEvent_(const ConnectionEvent &event) override final;

  /**
   * @brief Handle app termination call.
   * @param event app termination call.
   */
  void onEvent_(const AppTerminationEvent &event) override final;

  /****************************************************
  * Telemetry Utilities
  ****************************************************/
  std::shared_ptr<ITelemetryReceiver> m_telemetryReceiver;

  /****************************************************
  * Threading
  *****************************************************/
  std::jthread m_receiverThread;

  /****************************************************
  * Logging
  *****************************************************/
  bool m_verbose;

};

