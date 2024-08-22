/**
 * @file IPublisher.cpp
 * @brief Common code for all publishers.
 *
 * @details This file contains the declaration for publisher interface, which all publishers have to use.
 *
 * @author Szymon Bogus
 * @date 2024-07-13
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 */

#include "IPublisher.h"


void IPublisher::publish(const EventType eventType, const Event& event) {
  publish_(eventType, event);
}