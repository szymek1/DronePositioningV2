/**
 * @file EventsBus.cpp
 * @brief Code for the EventsBus.
 *
 * @details This file contains the declaration of EventsBus- entity which handles
 *          transfer of events from publishing objects to subscribers. It uses Publisher-Subscriber pattern.
 *
 * @author Szymon Bogus
 * @date 2024-05-22
 *
 * @copyright Copyright 2024 Szymon Bogus
 * @license Apache License, Version 2.0 (see
 * https://www.apache.org/licenses/LICENSE-2.0)
 *
 * @version 1.0
 *
 * @note Mutex errors occur in EventsBus::notifySubscribersOnTopic when application abruptly atempt
 *       to terminate, but I'm not sure how to diagnose it and fix it.
 */

#include "../include/EventsBus.h"


EventsBus::~EventsBus() { m_pool.join(); }

void EventsBus::addSubscriber(const EventType eventType,
                              std::shared_ptr<ISubscriber> &subscriber) {
  
  std::lock_guard<std::mutex> lock(m_addSubMtx);
  auto topic_iterator = m_subscriptionsMap.find(eventType);
  if (topic_iterator == m_subscriptionsMap.end()) {
    m_subscriptionsMap[eventType] = SubscribersVec();
    m_eventsMtxMap[eventType] = std::make_shared<std::mutex>();
  }

  m_subscriptionsMap[eventType].push_back(subscriber);
}

void EventsBus::removeSubscriber(
    const EventType eventType, std::shared_ptr<ISubscriber> &subscriber) {
  
  std::lock_guard<std::mutex> lock(m_rmvSubMtx);
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
  std::lock_guard<std::mutex> lock(m_getPublisherMtx);
  if (!m_publisher) {
    m_publisher = std::make_unique<EventsBusPublisher>(*this);
  }
  return m_publisher.get();
}


void EventsBus::notifySubscribersOnTopic(const EventType eventType,
                                         const Event &event) {
  std::lock_guard<std::mutex> lock(*m_eventsMtxMap[eventType]);
  for (auto weak_observer_it = m_subscriptionsMap[eventType].begin();
       weak_observer_it != m_subscriptionsMap[eventType].end();) {
    if (auto shared_observer = weak_observer_it->lock()) {
      boost::asio::post(m_pool, [shared_observer, event]() {
        shared_observer->onEvent(event);
      });
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