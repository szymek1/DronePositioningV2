#pragma once

#include "base/ITelemetryReceiver.h"
#include "EventsBus.h"


class GenericTelemetryReceiver : public ITelemetryReceiver {
public:

	GenericTelemetryReceiver() = default;
	~GenericTelemetryReceiver() = default;

	virtual void receive() override;
    
	virtual void stop() override;

protected:

	virtual void registerTelemetryEvent_() override;
};
