#ifndef CONTROLLER_FLOORSTATUSEVENT_HPP
#define CONTROLLER_FLOORSTATUSEVENT_HPP

#include "../../Common/BaseEvent.hpp"

namespace controller {

class FloorStatusEvent : public common::BaseEvent {
public:
	FloorStatusEvent(int floor) :
		floorStatus(floor)
	{
		m_eventName = k_eventName;
	}
	~FloorStatusEvent() = default;

	int getFloorStatus() { return floorStatus; }

private:
	int floorStatus;
	const std::string k_eventName = "FloorStatusEvent";
};

} /* controller */

#endif /* CONTROLLER_FLOORSTATUSEVENT_HPP */
