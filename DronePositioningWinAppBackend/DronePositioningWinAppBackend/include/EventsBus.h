#pragma once

#include <unordered_map>
#include <vector>

#include "base/IPublisher.h"
#include "base/ISubscriber.h"


class EventsBus {
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
  void addSubscriber(EventType eventType,
                     const std::shared_ptr<ISubscriber> &observer);

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
  void removeSubscriber(EventType eventType,
                        const std::shared_ptr<ISubscriber> &observer);

  /**
   * @brief Return publisher in order to acces the bus from an object 
   *		that wants to publish data
   * @return pointer to generic publisher
   */
  IPublisher *getPublisher();

private:
  using SubscribersVec = std::vector<std::weak_ptr<ISubscriber>>;
  using SubscriptionsMap = std::unordered_map<EventType, SubscribersVec>;

  /**
   * @brief notify all subscribers of the given event about an update
   * @param eventType type of event which has been updated
   * @param event data which is the update
   */
  void notifySubscribersOnTopic(const EventType eventType, const Event &event);

  /**
   * @brief Internal publisher which components which want to publish use to communicate
   *        with EventsBus
   */
  class EventsBusPublisher : public IPublisher {
  public:
    EventsBusPublisher(EventsBus &bus);
    ~EventsBusPublisher() = default;

    /**
     * @brief publish data of a particualr event with a correct event type
     * indicator
     * @param eventType on which event the bus should publish new information
     * @param event data passed within the event
     */
    void publish(const EventType eventType, const Event &event) override;

  private:
    EventsBus &m_eventsBus;
  };

  SubscriptionsMap m_subscriptionsMap;
  std::unique_ptr<EventsBusPublisher> m_publisher;

};

