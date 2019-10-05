
#ifndef ELEVATOR_ELEVATOR_HPP
#define EVELATOR_ELEVATOR_HPP

#include "../Common/ComponentBase.hpp"
#include "PersistedData.cpp"

#include <memory>

namespace elevator {

class Elevator : public common::ComponentBase {
public:
	Elevator();
	~Elevator();

	void setId(std::string id);

	void start() override;
	void stop() override;

private:
	enum Direction {
		NONE = 0x0,
		UP = 0x1,
		DOWN = 0x2
	};

	std::string m_elevatorId;
	PersistedData* m_pPersistence;
	int m_currentFloor;
	Direction m_direction;
};

} /* elevator */

#endif
