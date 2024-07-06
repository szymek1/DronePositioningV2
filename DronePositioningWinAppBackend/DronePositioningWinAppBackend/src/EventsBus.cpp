#include "../include/EventsBus.h"

void EventsBus::subscribe(EventType eventType,
                          std::shared_ptr<IObserver> observer) {
  m_observers[eventType].push_back(observer);
}

void EventsBus::unsubscribe(EventType eventType,
                            std::shared_ptr<IObserver> observer) {}

void EventsBus::notify(EventType eventType, std::any &data) {
  for (auto const &observer : m_observers[eventType]) {
    observer->onNotify(data); // TODO: visitor pattern has to be added here!!!
  }
}
