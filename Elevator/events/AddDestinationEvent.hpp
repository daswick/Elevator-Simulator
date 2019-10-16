#ifndef ELEVATOR_ADDDESTINATIONEVENT_HPP
#define ELEVATOR_ADDDESTINATIONEVENT_HPP

#include "../../Common/BaseEvent.hpp"

namespace elevator {

class AddDestinationEvent : public common::BaseEvent {
public:
	AddDestinationEvent(int destination) :
		m_destination(destination)
	{
		m_eventName = k_eventName;
	}
	~AddDestinationEvent() = default;

	int getDestination() { return m_destination; }

private:
	int m_destination;
	const std::string k_eventName = "AddDestinationEvent";
};

} /* elevator */

#endif /* ELEVATOR_ADDDESTINATIONEVENT_HPP */
