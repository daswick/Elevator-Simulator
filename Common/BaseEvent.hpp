#ifndef COMMON_BASEEVENT_HPP
#define COMMON_BASEEVENT_HPP

#include <string>

namespace common {

class BaseEvent {
public:
	BaseEvent() = default;
	~BaseEvent() = default;

	std::string getEventName() { return m_eventName; }

protected:
	std::string m_eventName;
};

} /* common */

#endif
