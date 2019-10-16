#ifndef COMMON_EVENTHANDLER_HPP
#define COMMON_EVENTHANDLER_HPP

#include "BaseEvent.hpp"

#include <set>
#include <string>

namespace common {

class EventHandler {
public:
	EventHandler() = default;
	~EventHandler();

	class EventListener {
	public:
		EventListener() = default;
		~EventListener() = default;

		virtual inline void handleEvent(BaseEvent* pEvent) { /* not implemented */}

		inline void setListenerName(std::string name) { m_listenerName = name; }
		inline std::string getListenerName() { return m_listenerName; }

	private:
		std::string m_listenerName;
	};

	void addListener(EventListener* pListener);
	void removeListener(EventListener* pListener);

	void publishEvent(BaseEvent* pEvent);

private:
	std::set<EventListener*> m_listenerSet;
};

} /* common */

#endif /* COMMON_EVENTHANDLER_HPP */
