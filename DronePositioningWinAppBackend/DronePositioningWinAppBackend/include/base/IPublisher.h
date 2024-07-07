#pragma once

#include <variant>

#include "../EventType.h"
#include "../Events.h"


/**
 * @class IPublisher
 * @brief Interface defining observable.
 */
class IPublisher {
public:
  virtual ~IPublisher() = default;

  /**
   * @brief publish data of a particualr event with a correct event type indicator
   * @param eventType on which event the bus should publish new information
   * @param event data passed within the event
   */
  virtual void publish(const EventType eventType, const Event& event) = 0;
};

