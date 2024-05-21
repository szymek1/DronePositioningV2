#pragma once

/**
 * @enum EventType
 * @brief Enum defining event types tho which observers like:
 * - IProcessor
 * - ITelemetryReceiver
 * - ITelemetrySender
 * can subscribe via EventBus.
 */
enum class EventType
{
	TELEMETRY_UPDATE,
	CONNECTION_UPDATE
};