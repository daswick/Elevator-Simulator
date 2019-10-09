#ifndef ELEVATOR_ELEVATOR_CPP
#define ELEVATOR_ELEVATOR_CPP

#include "Elevator.hpp"
#include "../Common/SafePrint.cpp"
#include "../Common/BaseEvent.hpp"

namespace elevator {

Elevator::Elevator()
{
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pPersistence = nullptr;
	m_pElevatorTimer = std::make_unique<common::CoreTimer>();
	m_pEventHandler = std::make_shared<common::EventHandler>();
	m_pSignalReceiver = std::make_unique<common::SignalReceiver>();
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

	setListenerName("elev" + m_elevatorId);

	m_pEventHandler->addListener(this);

	m_pPersistence = std::make_shared<PersistedData>(m_elevatorId);
	m_pPersistence->start();
	m_currentFloor = m_pPersistence->getPersistedFloor();

	int elevatorId = std::stoi(m_elevatorId);
	int elevatorKey = 0x10 | elevatorId;
	m_pSignalReceiver->start(elevatorKey);

	m_isStarted = true;
}

void Elevator::stop() {
	if (!m_isStarted) {
		return;
	}

	SAFEPRINT("Stopping Elevator " + m_elevatorId);

	m_pPersistence->savePersistedFloor(m_currentFloor);
	m_pPersistence->stop();

	m_pElevatorTimer->cancelTimer();

	m_pEventHandler->removeListener(this);

	m_pSignalReceiver->stop();

	m_isStarted = false;
}

void Elevator::coreTimerCallback() {
	// To be determined
}

void Elevator::handleEvent(common::BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	SAFEPRINT("Elevator " + m_elevatorId + " received event " + pEvent->getEventName());

	// TODO: Handle event
}

} /* elevator */

#endif
