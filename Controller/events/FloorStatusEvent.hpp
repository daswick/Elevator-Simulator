#ifndef CONTROLLER_FLOORSTATUSEVENT_HPP
#define CONTROLLER_FLOORSTATUSEVENT_HPP

#include "../../Common/BaseEvent.hpp"

namespace controller {

class FloorStatusEvent : public common::BaseEvent {
public:
	FloorStatusEvent(int floor, int elevator) :
		floorStatus(floor), elevatorId(elevator)
	{
		m_eventName = k_eventName;
	}
	~FloorStatusEvent() = default;

	int getFloorStatus() { return floorStatus; }
	int getElevatorId() { return elevatorId; }

private:
	int floorStatus;
	int elevatorId;
	const std::string k_eventName = "FloorStatusEvent";
};

} /* controller */

#endif /* CONTROLLER_FLOORSTATUSEVENT_HPP */
