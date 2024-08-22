/**
 * @file TelemetrySender.h
 * @brief Concrete implementation of ITelemetrySender interface using UDP protocol for sending data to a remote endpoint.
 *
 * @details This file contains the declaration of a concrete telemetry sender entity implementation.
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

#include <iostream>
#include <vector>
#include <string>

#include <WS2tcpip.h>

#include "base/ITelemetrySender.h"
#include "base/ISubscriber.h"
#include "EventsBus.h"


/**
* @class TelemetrySender
* @brief Implementation of UDP telemetry sender.
*/
class TelemetrySender : public ITelemetrySender, public ISubscriber {
public:

    /**
     * @brief Constructor.
     * @param ip: endpoint ip address.
     * @param port: endpoint port.
     * @param isVerbose: logs verbosity flag.
     */
  explicit TelemetrySender(EventsBus &bus, const std::string &ip,
                           const std::string& port, bool isVerbose = false);

  ~TelemetrySender();

private:

    /**
    * @brief Send telemetry via UDP protocol.
    * @param telemetry: new telemetry extracted from the event.
    */
    void sendPosition_(const std::vector<float> &telemetry) override final;

    /**
     * @brief Send telemetry to external platform.
     * @param event: new telemetry data.
     */
    void onEvent_(const TelemetryEvent &event) override final;

    /****************************************************
    * Networking
    *****************************************************/
    WSADATA m_winSockdata;
    WORD m_winSockVersion;
    SOCKET m_socket;
    sockaddr_in m_remoteTarget;
    const char *m_ip;
    const int m_port;

    /****************************************************
    * Logging
    *****************************************************/
    bool m_verbose;

    /****************************************************
    * Publishing
    *****************************************************/
    IPublisher *m_publisher;
};

