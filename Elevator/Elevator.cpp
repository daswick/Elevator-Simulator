#ifndef ELEVATOR_ELEVATOR_CPP
#define ELEVATOR_ELEVATOR_CPP

#include "Elevator.hpp"
#include "../Common/SafePrint.cpp"

namespace elevator {

Elevator::Elevator()
{
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pPersistence = nullptr;
	m_pElevatorTimer = nullptr;
	m_isStarted = false;
}

Elevator::~Elevator() {
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pElevatorTimer->cancelTimer();
}

void Elevator::setId(std::string id) {
	m_elevatorId = id;
}

void Elevator::start() {
	SAFEPRINT("Starting Elevator " + m_elevatorId);

	m_isStarted = true;
	m_pPersistence = std::make_shared<PersistedData>(m_elevatorId);
	m_pElevatorTimer = std::make_unique<common::CoreTimer>();
	m_pPersistence->start();

	m_currentFloor = m_pPersistence->getPersistedFloor();

	if (m_currentFloor == -1) {
		m_currentFloor = 0;
	}
}

void Elevator::stop() {
	SAFEPRINT("Stopping Elevator " + m_elevatorId);

	m_isStarted = false;

	m_pPersistence->savePersistedFloor(m_currentFloor);
	m_pElevatorTimer->cancelTimer();
	m_pPersistence->stop();
}

void Elevator::coreTimerCallback() {
	// To be determined
}

} /* elevator */

#endif
