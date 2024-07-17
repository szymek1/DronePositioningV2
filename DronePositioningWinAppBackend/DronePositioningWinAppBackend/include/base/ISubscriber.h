#pragma once

#include <vector>
#include <variant>

#include "../Events.h"


/**
 * @class ISubscriber
 * @brief Interface defining base subscriber.
 */
class ISubscriber {
public:
  virtual ~ISubscriber() = default;

  /**
  * @brief Respond to particular event- call appropriate implementation.
  * @param event type of event
  */
  void onEvent(const Event &event);

private:

  /**
   * @brief Send Event of already determined type to proper overload onEvent_
   * @tparam T deduced type of Event
   * @param event object storing event specific data
   */
  template <typename T> void dispatchEvent(const T &event) { onEvent_(event); };

  /****************************************************
   * In order to be able to use templated evetns
   * dispatcher there's a need to supply the compiler
   * with some kind of implementation for each
   * onEvent_ overload. Specific subscriber which will
   * inherit from this class will be able to implement
   * its own way of handling events it subscribes to and
   * it will use the default (do nothing) implementation
   * from this class.
   ****************************************************/

  /**
   * @brief Handle new telemtry
   * @param event new telemetry data
   */
  virtual void onEvent_(const TelemetryEvent &event){};

  /**
   * @brief Handle new connection event
   * @param event new connection status
   */
  virtual void onEvent_(const ConnectionEvent &event){};

  /**
   * @brief Handle app termination call
   * @param event app termination call
   */
  virtual void onEvent_(const AppTerminationEvent &event){};
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


