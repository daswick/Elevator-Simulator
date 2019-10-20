#ifndef CONTROLLER_DIRECTIONCHANGEDEVENT_HPP
#define CONTROLLER_DIRECTIONCHANGEDEVENT_HPP

#include "../../Common/BaseEvent.hpp"

namespace controller {

class DirectionChangedEvent : public common::BaseEvent {
public:
	DirectionChangedEvent(int direction, int elevator) :
		m_direction(direction), m_elevatorId(elevator)
	{
		m_eventName = k_eventName;
	}
	~DirectionChangedEvent() = default;

	int getDirection() { return m_direction; }
	int getElevatorId() { return m_elevatorId; }

private:
	int m_direction;
	int m_elevatorId;
	const std::string k_eventName = "DirectionChangedEvent";
};

} /* controller */

#endif /* CONTROLLER_DIRECTIONCHANGEDEVENT_HPP */
