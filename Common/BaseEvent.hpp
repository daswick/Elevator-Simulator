#ifndef COMMON_BASEEVENT_HPP
#define COMMON_BASEEVENT_HPP

namespace common {

class BaseEvent {
public:
	BaseEvent() = default;
	~BaseEvent() = default;

	std::string getEventName() { return m_eventName; }
	void setEventName(std::string name) { m_eventName = name; }

private:
	std::string m_eventName;
};

} /* common */

#endif
