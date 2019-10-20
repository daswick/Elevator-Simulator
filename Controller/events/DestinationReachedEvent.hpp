#ifndef CONTROLLER_DESTINATIONREACHEDEVENT_HPP
#define CONTROLLER_DESTINATIONREACHEDEVENT_HPP

#include "../../Common/BaseEvent.hpp"

namespace controller {

class DestinationReachedEvent : public common::BaseEvent {
public:
	DestinationReachedEvent(int floor, int elevator) :
		floorReached(floor), elevatorId(elevator)
	{
		m_eventName = k_eventName;
	}
	~DestinationReachedEvent() = default;

	int getFloorReached() { return floorReached; }
	int getElevatorId() { return elevatorId; }

private:
	int floorReached;
	int elevatorId;
	const std::string k_eventName = "DestinationReachedEvent";
};

} /* controller */

#endif /* CONTROLLER_DESTINATIONREACHEDEVENT_HPP */
