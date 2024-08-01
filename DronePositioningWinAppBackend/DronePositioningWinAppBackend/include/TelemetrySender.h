#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <boost/asio.hpp>

#include "base/ITelemetrySender.h"
#include "base/ISubscriber.h"


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
  explicit TelemetrySender(std::string ip,
                           std::string port, bool isVerbose = false);

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
    boost::asio::ip::udp::socket m_socket;
    boost::asio::io_service m_ioService;
    boost::asio::ip::udp::endpoint m_remoteEndpoint;

    /****************************************************
    * Logging
    *****************************************************/
    bool m_verbose;
};

