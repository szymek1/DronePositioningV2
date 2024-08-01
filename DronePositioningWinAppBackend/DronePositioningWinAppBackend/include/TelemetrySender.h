#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <WS2tcpip.h>

#include "base/ITelemetrySender.h"
#include "base/ISubscriber.h"
#include "EventsBus.h"

// Include the Winsock library (lib) file
#pragma comment(lib, "ws2_32.lib")


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

    /****************************************************
    * Logging
    *****************************************************/
    bool m_verbose;

    /****************************************************
    * Publishing
    *****************************************************/
    IPublisher *m_publisher;
};

