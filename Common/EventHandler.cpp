#include "EventHandler.hpp"

#include "SafePrint.hpp"

#include <iterator>

namespace common {

EventHandler::~EventHandler() {
	// Erase all listeners from the set
	while (m_listenerSet.begin() != m_listenerSet.end()) {
		m_listenerSet.erase(m_listenerSet.begin());
	}
}

void EventHandler::addListener(EventListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	m_listenerSet.insert(pListener);
}

void EventHandler::removeListener(EventListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	// If the requested listener is in the set, remove it
	std::set<EventListener*>::iterator it = m_listenerSet.find(pListener);
	if (it != m_listenerSet.end()) {
		m_listenerSet.erase(it);
	}
}

void EventHandler::publishEvent(BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	// By claiming the raw pointer in this shared pointer, the pointer should be freed automatically
	std::shared_ptr<BaseEvent> pSharedEvent(pEvent);

	// Notify all listeners of the new event
	for (EventListener* listener : m_listenerSet) {
		listener->handleEvent(pSharedEvent.get());
	}
}

} /* common */
