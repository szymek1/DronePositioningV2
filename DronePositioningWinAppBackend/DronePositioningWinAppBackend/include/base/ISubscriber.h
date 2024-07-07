#pragma once

#include <vector>

#include "../Events.h"


/**
 * @class IObserver
 * @brief Interface defining base observer.
 */
class ISubscriber {
public:
  virtual ~ISubscriber() = default;

  virtual void onEvent(const Event &event) = 0;
};

/****************************************************
 * An example of subscriber object
 ****************************************************/

/*
#include "base/ISubscriber.hpp"


class GenericSub : public ISubscriber {
public:
    GenericSub() = default;
    ~GenericSub() = default;

    void onEvent(const Event& event) override {
        std::visit([this](const auto& e) {
        using T = std::decay_t<decltype(e)>;
        if constexpr (std::is_same_v<T, TelemetryEvent>) {
            onEvent_(e);
        } else if constexpr (std::is_same_v<T, ConnectionEvent>) {
            onEvent_(e);
        } 
        else {
            std::cerr << "Unexpected event type received." << std::endl;
        }
    }, event);
    }

private:
    void onEvent_(const TelemetryEvent& data) {
        for (auto const& d : data.telemetry) {
            std::cout << d << std::endl;
        }
    }
    void onEvent_(const ConnectionEvent& status) {
        std::cout << "Status: " << status.isConnected << std::endl;
    }
};
*/


