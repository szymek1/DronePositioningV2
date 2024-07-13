#include "ISubscriber.h"


void ISubscriber::onEvent(const Event &event) { 
	onEvent_(event); 
}