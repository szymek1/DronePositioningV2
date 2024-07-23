#pragma once

#include <variant>

#include "../EventType.h"
#include "../Events.h"


/**
 * @class IPublisher
 * @brief Interface defining publisher.
 */
class IPublisher {
public:
  virtual ~IPublisher() = default;

  /**
   * @brief Publish data of a particualr event with a correct event type indicator- call appropriate implementation.
   * @param eventType: on which event the bus should publish new information.
   * @param event: data passed within the event.
   */
  void publish(const EventType eventType, const Event& event);

private:

  /**
  * @brief Publish data of a particualr event with a correct event type
  *		   indicator.
  * @param eventType: on which event the bus should publish new information.
  * @param event: data passed within the event.
  */
  virtual void publish_(const EventType eventType, const Event &event) = 0;
};

