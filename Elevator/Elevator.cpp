#include "Elevator.hpp"

#include "SignalHandler.hpp"

#include "../Common/SafePrint.hpp"

namespace elevator {

Elevator::Elevator(std::string id) {
	m_elevatorId = id;
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pPersistence = nullptr;
	m_pElevatorTimer = std::make_unique<common::CoreTimer>();
	m_pEventHandler = std::make_shared<common::EventHandler>();
	m_pSignalHandler = std::make_unique<SignalHandler>(m_pEventHandler);
	m_pPersistence = std::make_shared<PersistedData>(m_elevatorId);
}

Elevator::~Elevator() {
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;
	m_pElevatorTimer->cancelTimer();
}

void Elevator::start() {
	if (m_isStarted) {
		return;
	}

	SAFEPRINT("Starting Elevator " + m_elevatorId);

	setListenerName("elev" + m_elevatorId);

	m_pEventHandler->addListener(this);

	m_pPersistence->start();
	m_currentFloor = m_pPersistence->getPersistedFloor();

	SAFEPRINT("Elevator " + m_elevatorId + " started on floor " + std::to_string(m_currentFloor));
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

void Elevator::coreTimerCallback(int timerId) {
	if (timerId == TimerIDs::FloorChange) {
		moveElevator();
	}
	else if (timerId == TimerIDs::DoorOpen) {
		onDestinationReached();
	} else {
		SAFEPRINT("Elevator::coreTimerCallback received unknown timer id " + std::to_string(timerId));
	}
}

void Elevator::moveElevator() {
	m_currentFloor += (m_direction == Direction::UP) ? 1 : -1;
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_FLOOR_STATUS, m_currentFloor);

	checkForDestination();
}

void Elevator::checkForDestination() {
	std::lock_guard<std::mutex> guard(m_queueMutex);

	std::unordered_set<int>::iterator it = m_destinationQueue.find(m_currentFloor);
	if (it != m_destinationQueue.end()) {
		SAFEPRINT("Elevator " + m_elevatorId + " is opening its door on floor " + std::to_string(m_currentFloor));
		m_destinationQueue.erase(it);
		m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_DESTINATION_REACHED, m_currentFloor);
		m_pElevatorTimer->cancelTimer();
		m_pElevatorTimer->startSingleTimer(this, k_destinationReachedDuration, static_cast<int>(TimerIDs::DoorOpen));
	}
}

void Elevator::onDestinationReached() {
	SAFEPRINT("Elevator " + m_elevatorId + " is closing its door on floor " + std::to_string(m_currentFloor));
	m_pElevatorTimer->cancelTimer();
	if (m_destinationQueue.size() == 0) {
		m_direction = Direction::NONE;
		m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_DIRECTION_CHANGE, static_cast<int>(m_direction));
	} else {
		m_pElevatorTimer->startRepeatingTimer(this, k_floorChangeDuration, static_cast<int>(TimerIDs::FloorChange));
	}
}

void Elevator::handleEvent(common::BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	if (pEvent->getEventName() == "AddDestinationEvent") {
		AddDestinationEvent* pDestEvent = static_cast<AddDestinationEvent*>(pEvent);
		SAFEPRINT("Elevator " + m_elevatorId + " received AddDestinationEvent for floor " + std::to_string(pDestEvent->getDestination()));

		addDestination(pDestEvent->getDestination());
	}
}

void Elevator::addDestination(int destination) {
	if (destination == m_currentFloor) {
		return;
	}

	{
		std::lock_guard<std::mutex> guard(m_queueMutex);
		m_destinationQueue.insert(destination);
	}

	if (m_direction == Direction::NONE) {
		m_direction = (destination < m_currentFloor) ? Direction::DOWN : Direction::UP;
		m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_DIRECTION_CHANGE, static_cast<int>(m_direction));
		m_pElevatorTimer->startRepeatingTimer(this, k_floorChangeDuration, static_cast<int>(TimerIDs::FloorChange));
	}
}

} /* elevator */
