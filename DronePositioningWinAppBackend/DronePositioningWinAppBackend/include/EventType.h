/**
 * @file EventType.h
 * @brief Enum class with events tags.
 *
 * @details This file contains enum class with possible events tags used for event determination.
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

/**
 * @enum EventType.
 * @brief Enum defining event types tho which observers like:
 * - IProcessor
 * - ITelemetryReceiver
 * - ITelemetrySender
 * can subscribe via EventBus.
 */
enum class EventType
{
	TELEMETRY_UPDATE,
	CONNECTION_UPDATE,
	APP_TERMINATION
};