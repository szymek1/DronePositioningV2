#pragma once

#include <unordered_map>
#include <vector>

#include "base/IObservable.h"


class EventsBus : public IObservable {
public:
  explicit EventsBus() = default;
  ~EventsBus() = default;

  /**
   * @brief Add subscriber to unordered map of subscribers
   * @param eventType type of event: 
   *								TELEMETRY_UPDATE,
   * 								CONNECTION_UPDATE,
   *								APP_TERMINATION
   * @param observer object representing observer:
   *											  ITelemetrySender   (TELEMETRY_UPDATE)
   *                                              IProcessor         (TELEMETRY_UPDATE)
   *                                              ConnectionManager  (CONNECTION_UPDATE, APP_TERMINATION)
   */
  void addSubscriber(EventType eventType, const std::shared_ptr<IObserver>& observer);

  /**
   * @brief Remove subscriber
   * @param eventType type of event:
   *								TELEMETRY_UPDATE,
   * 								CONNECTION_UPDATE,
   *								APP_TERMINATION
   * @param observer object representing observer:
   *											  ITelemetrySender  (TELEMETRY_UPDATE) 
   *                                              IProcessor        (TELEMETRY_UPDATE) 
   *											  ConnectionManager (CONNECTION_UPDATE, APP_TERMINATION)
   */
  void removeSubscriber(EventType eventType, const std::shared_ptr<IObserver>& observer);

  /**
   * @brief Return publisher in order to acces the bus from an object 
   *		that wants to publish data
   * @return pointer to generic publisher
   */
  IObservable *getPublisher();

private:

  /**
  * @brief Notify on given topic that new data is ready
  * @param eventType type of event:
  *								TELEMETRY_UPDATE,
  * 								CONNECTION_UPDATE,
  *								APP_TERMINATION
  * @param data new data for a topic:
  *  							    TELEMETRY_UPDATE
  *(std::vector<float>), CONNECTION_UPDATE (bool), APP_TERMINATION   (bool)
  */
  void notify(EventType eventType, std::any &data);

  std::unordered_map<EventType, std::vector<std::shared_ptr<IObserver>>> m_observers;

};

