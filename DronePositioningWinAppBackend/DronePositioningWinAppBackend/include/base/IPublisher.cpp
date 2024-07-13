#include "IPublisher.h"


void IPublisher::publish(const EventType eventType, const Event& event) {
  publish_(eventType, event);
}