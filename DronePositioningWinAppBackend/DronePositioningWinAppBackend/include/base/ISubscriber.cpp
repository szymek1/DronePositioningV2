#include "ISubscriber.h"


void ISubscriber::onEvent(const Event &event) { 
	std::visit([this](const auto &e) { dispatchEvent(e); }, event);
}