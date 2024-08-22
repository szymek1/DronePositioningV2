/**
 * @file IEvent.h
 * @brief Generic Event structure.
 *
 * @details This file contains the interface for a generic application event.
 *
 * @author Szymon Bogus
 * @date 2024-07-06
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 * 
 * @version 1.0
 */

#pragma once


/**
 * @brief Interface defining a generic Event.
 */
struct IEvent {
  virtual ~IEvent() = default;
};
