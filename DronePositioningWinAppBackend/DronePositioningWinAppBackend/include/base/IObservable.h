#pragma once

#include <any>
#include <memory>

#include "IObserver.h"
#include "../EventType.h"


/**
 * @class IObservable
 * @brief Interface defining observable.
 */
class IObservable {
public:
  virtual ~IObservable() = default;

  virtual void subscribe(EventType eventType, std::shared_ptr<IObserver> observer) = 0;
  virtual void unsubscribe(EventType eventType, std::shared_ptr<IObserver> observer) = 0;
  virtual void notify(EventType eventType, std::any &data) = 0;
};

