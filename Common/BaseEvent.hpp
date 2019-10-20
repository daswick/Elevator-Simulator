#ifndef COMMON_BASEEVENT_HPP
#define COMMON_BASEEVENT_HPP

#include <string>

namespace common {

class BaseEvent {
public:
	BaseEvent() = default;
	~BaseEvent() = default;

	inline std::string getEventName() { return m_eventName; }

protected:
	std::string m_eventName = "BaseEvent";
};

} /* common */

#endif /* COMMON_BASEEVENT_HPP */
