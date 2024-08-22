/**
 * @file ISubscriber.cpp
 * @brief Common code for all subscribers.
 *
 * @details This file contains the declaration of subscriber interface which all 
 *			subscribing objects have to use. It utilizes std::visit to traverse std::variant of events
 *
 * @author Szymon Bogus
 * @date 2024-07-13
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 *
 * @note Hypotheticaly, for a larger number of events included into the variant this 
 *       could be a bottleneck.
 */

#include "ISubscriber.h"


void ISubscriber::onEvent(const Event &event) { 
	std::visit([this](const auto &e) { dispatchEvent(e); }, event);
}