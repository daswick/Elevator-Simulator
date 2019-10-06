#ifndef COMMON_EVENTHANDLER_CPP
#define COMMON_EVENTHANDLER_CPP

#include "EventHandler.hpp"
#include "SafePrint.cpp"

#include <iterator>

namespace common {

EventHandler::EventHandler() {
	// Do nothing
}

EventHandler::~EventHandler() {
	while (m_listenerSet.begin() != m_listenerSet.end()) {
		m_listenerSet.erase(m_listenerSet.begin());
	}
}

void EventHandler::addListener(EventListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	SAFEPRINT("Added event listener " + pListener->getListenerName());
	m_listenerSet.insert(pListener);
}

void EventHandler::removeListener(EventListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	std::set<EventListener*>::iterator it = m_listenerSet.find(pListener);
	if (it != m_listenerSet.end()) {
		SAFEPRINT("Removed event listener " + pListener->getListenerName());
		m_listenerSet.erase(it);
	}
}

void EventHandler::publishEvent(BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	for (EventListener* listener : m_listenerSet) {
		listener->handleEvent(pEvent);
	}
}

} /* common */

#endif
