#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio.hpp>

#include "base/IPublisher.h"
#include "base/ISubscriber.h"


class EventsBus {
public:

  /**
   * @brief Constructor.
   */
  explicit EventsBus() = default;
  ~EventsBus();

  /**
   * @brief Add subscriber to unordered map of subscribers.
   * @param eventType: type of event: 
   *								TELEMETRY_UPDATE,
   * 								CONNECTION_UPDATE,
   *								APP_TERMINATION
   * @param observer: object representing observer:
   *											  ITelemetrySender   (TELEMETRY_UPDATE)
   *                                              IProcessor         (TELEMETRY_UPDATE)
   *                                              ConnectionManager  (CONNECTION_UPDATE, APP_TERMINATION)
   */
  void addSubscriber(const EventType eventType,
                     std::shared_ptr<ISubscriber> &observer);

  /**
   * @brief Remove subscriber.
   * @param eventType: type of event:
   *								TELEMETRY_UPDATE,
   * 								CONNECTION_UPDATE,
   *								APP_TERMINATION
   * @param observer: object representing observer:
   *											  ITelemetrySender  (TELEMETRY_UPDATE) 
   *                                              IProcessor        (TELEMETRY_UPDATE) 
   *											  ConnectionManager (CONNECTION_UPDATE, APP_TERMINATION)
   */
  void removeSubscriber(const EventType eventType,
                        std::shared_ptr<ISubscriber> &observer);

  /**
   * @brief Return publisher in order to acces the bus from an object 
   *		that wants to publish data.
   * @return pointer to generic publisher.
   */
  IPublisher *getPublisher();

private:
  using SubscribersVec = std::vector<std::weak_ptr<ISubscriber>>;
  using SubscriptionsMap = std::unordered_map<EventType, SubscribersVec>;
  using EventsMutexMap = std::unordered_map<EventType, std::shared_ptr<std::mutex>>;

  /**
   * @brief Notify all subscribers of the given event about an update.
   * @param eventType: type of event which has been updated.
   * @param event: data which is the update.
   */
  void notifySubscribersOnTopic(const EventType eventType, const Event &event);

  /**
   * @brief Internal publisher which components which want to publish use to communicate
   *        with EventsBus.
   */
  class EventsBusPublisher : public IPublisher {
  public:
    EventsBusPublisher(EventsBus &bus);
    ~EventsBusPublisher() = default;

  private:

    /**
    * @brief Publish data of a particualr event with a correct event type
    *        indicator.
    * @param eventType: on which event the bus should publish new information.
    * @param event data passed within the event.
    */
    void publish_(const EventType eventType, const Event &event) override final;
    
    EventsBus &m_eventsBus;
  };

  SubscriptionsMap m_subscriptionsMap;
  EventsMutexMap m_eventsMtxMap;

  std::mutex m_getPublisherMtx;
  std::mutex m_addSubMtx;
  std::mutex m_rmvSubMtx;

  std::unique_ptr<EventsBusPublisher> m_publisher;

  boost::asio::thread_pool m_pool{5};
};

