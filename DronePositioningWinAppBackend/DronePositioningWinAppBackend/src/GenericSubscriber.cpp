#include "../include/GenericSubscriber.h"


void GenericSubscriber::onEvent(const Event &event) {
  std::visit([this](const auto &e) { dispatchEvent(e); }, event);
}