#ifndef ELEVATOR_ELEVATOR_CPP
#define ELEVATOR_ELEVATOR_CPP

#include "Elevator.hpp"

#include <iostream>

namespace elevator {

Elevator::Elevator()
{
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pPersistence = nullptr;
	m_isStarted = false;
}

Elevator::~Elevator() {
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	delete m_pPersistence;
}

void Elevator::setId(std::string id) {
	m_elevatorId = id;
}

void Elevator::start() {
	std::cout << "Starting Elevator " << m_elevatorId << "\n";
	m_isStarted = true;
	m_pPersistence = new PersistedData(m_elevatorId);
	m_pPersistence->start();

	m_currentFloor = m_pPersistence->getPersistedFloor();

	if (m_currentFloor == -1) {
		m_currentFloor = 0;
	}
}

void Elevator::stop() {
	std::cout << "Stopping Elevator " << m_elevatorId << "\n";
	m_isStarted = false;

	m_pPersistence->savePersistedFloor(m_currentFloor);
	m_pPersistence->stop();
}

} /* elevator */

#endif
