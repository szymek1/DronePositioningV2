/**
 * @file IPublisher.h
 * @brief Interface for messanges publisher.
 *
 * @details This file contains the delcaration of publisher interface.
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

