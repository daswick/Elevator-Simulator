#include "EventHandler.hpp"

#include "SafePrint.hpp"

#include <iterator>

namespace common {

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

	std::shared_ptr<BaseEvent> pSharedEvent(pEvent);

	for (EventListener* listener : m_listenerSet) {
		listener->handleEvent(pSharedEvent.get());
	}
}

} /* common */
