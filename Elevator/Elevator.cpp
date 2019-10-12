#ifndef ELEVATOR_ELEVATOR_CPP
#define ELEVATOR_ELEVATOR_CPP

#include "Elevator.hpp"
#include "../Common/SafePrint.cpp"
#include "../Common/BaseEvent.hpp"

#include "SignalHandler.hpp"

namespace elevator {

Elevator::Elevator()
{
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pPersistence = nullptr;
	m_pElevatorTimer = std::make_unique<common::CoreTimer>();
	m_pEventHandler = std::make_shared<common::EventHandler>();
	m_pSignalHandler = std::make_unique<SignalHandler>(m_pEventHandler);
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

	m_pSignalHandler->startReceiver(std::stoi(m_elevatorId));

	m_isStarted = true;

	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_FLOOR_STATUS, m_currentFloor);
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

	m_pSignalHandler->stopReceiver();

	m_isStarted = false;
}

void Elevator::coreTimerCallback() {
	// To be determined
}

void Elevator::handleEvent(common::BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	if (pEvent->getEventName() == "AddDestinationEvent") {
		AddDestinationEvent* pDestEvent = static_cast<AddDestinationEvent*>(pEvent);
		SAFEPRINT("Elevator " + m_elevatorId + " received AddDestinationEvent for floor " + std::to_string(pDestEvent->getDestination()));

	}
}

} /* elevator */

#endif
