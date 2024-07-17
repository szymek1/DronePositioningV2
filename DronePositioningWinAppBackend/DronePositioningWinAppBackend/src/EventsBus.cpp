#include "../include/EventsBus.h"


void EventsBus::addSubscriber(const EventType eventType,
                              std::shared_ptr<ISubscriber> &subscriber) {
  auto topic_iterator = m_subscriptionsMap.find(eventType);
  if (topic_iterator == m_subscriptionsMap.end()) {
    m_subscriptionsMap[eventType] = SubscribersVec();
  }

  m_subscriptionsMap[eventType].push_back(subscriber);
}

void EventsBus::removeSubscriber(
    const EventType eventType, std::shared_ptr<ISubscriber> &subscriber) {
  auto topic_iterator = m_subscriptionsMap.find(eventType);
  if (topic_iterator != m_subscriptionsMap.end()) {
    m_subscriptionsMap[eventType].erase(
        std::remove_if(
            m_subscriptionsMap[eventType].begin(),
            m_subscriptionsMap[eventType].end(),
            [&subscriber](const std::weak_ptr<ISubscriber> &weak_subscriber) {
              return weak_subscriber.expired() ||
                     weak_subscriber.lock() == subscriber;
            }),
        m_subscriptionsMap[eventType].end());
  }
}

IPublisher *EventsBus::getPublisher() {
  if (!m_publisher) {
    m_publisher = std::make_unique<EventsBusPublisher>(*this);
  }
  return m_publisher.get();
}

void EventsBus::notifySubscribersOnTopic(const EventType eventType,
                                         const Event &event) {
  for (auto weak_observer_it = m_subscriptionsMap[eventType].begin();
       weak_observer_it != m_subscriptionsMap[eventType].end();) {
    if (auto shared_observer = weak_observer_it->lock()) {
      shared_observer->onEvent(event);
      ++weak_observer_it;
    } else {
      weak_observer_it = m_subscriptionsMap[eventType].erase(weak_observer_it);
    }
  }
}

EventsBus::EventsBusPublisher::EventsBusPublisher(EventsBus &bus)
    : m_eventsBus(bus) {}

void EventsBus::EventsBusPublisher::publish_(const EventType eventType,
                                            const Event &event) {
  m_eventsBus.notifySubscribersOnTopic(eventType, event);
}