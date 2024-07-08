#pragma once

#include "base/ISubscriber.h"


class GenericSubscriber : public ISubscriber {
public:
  GenericSubscriber() = default;
  ~GenericSubscriber() = default;

  /**
   * @brief use std::visit to select proper Event type and dispatch it
   *		an actual method which can handle it
   * @param event Event structure 
   */
  void onEvent(const Event &event) override final;

protected:

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
