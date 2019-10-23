#include "Elevator.hpp"

#include "SignalHandler.hpp"

#include "../Common/SafePrint.hpp"

namespace elevator {

Elevator::Elevator(std::string id) {
	m_elevatorId = id;
	m_isStarted = false;
	m_currentFloor = 0;
	m_direction = Direction::NONE;

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

	// Start listening for events
	m_pEventHandler->addListener(this);

	// Start listening for messages
	m_pSignalHandler->startReceiver(std::stoi(m_elevatorId));

	// Start the persistence and retrieve any persisted data
	m_pPersistence->start();
	m_currentFloor = m_pPersistence->getPersistedFloor();

	m_isStarted = true;

	// Let Controller know what floor the elevator started on
	SAFEPRINT("Elevator " + m_elevatorId + " started on floor " + std::to_string(m_currentFloor));
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_FLOOR_STATUS, m_currentFloor);
}

void Elevator::stop() {
	if (!m_isStarted) {
		return;
	}

	SAFEPRINT("Stopping Elevator " + m_elevatorId);

	// Persist any data and stop the persistence
	m_pPersistence->savePersistedFloor(m_currentFloor);
	m_pPersistence->stop();

	// If any timer is ongoing, cancel it
	m_pElevatorTimer->cancelTimer();

	// Stop listening for any events
	m_pEventHandler->removeListener(this);

	// Stop listening for any messages
	m_pSignalHandler->stopReceiver();

	m_isStarted = false;
}

void Elevator::coreTimerCallback(int timerId) {
	if (timerId == TimerIDs::FloorChange) {
		// Called from addDestination or onDestinationReached
		moveElevator();
	}
	else if (timerId == TimerIDs::DoorOpen) {
		// Called from checkForDestination after destination reached
		onDestinationReached();
	} else {
		SAFEPRINT("Elevator::coreTimerCallback received unknown timer id " + std::to_string(timerId));
	}
}

void Elevator::moveElevator() {
	// Update the current floor and alert controller
	m_currentFloor += (m_direction == Direction::UP) ? 1 : -1;
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_FLOOR_STATUS, m_currentFloor);

	// Check to see if destination has been reached
	checkForDestination();
}

void Elevator::checkForDestination() {
	std::lock_guard<std::mutex> guard(m_queueMutex);

	// If the current floor is in the destination set, remove it and start the destination reached event
	std::unordered_set<int>::iterator it = m_destinationQueue.find(m_currentFloor);
	if (it != m_destinationQueue.end()) {
		SAFEPRINT("Elevator " + m_elevatorId + " is opening its door on floor " + std::to_string(m_currentFloor));
		m_destinationQueue.erase(it);
		m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_DESTINATION_REACHED, m_currentFloor);

		// Stop current timer and start single timer to close door
		m_pElevatorTimer->cancelTimer();

		// Add up to a second to simulate "random" people getting off
		int duration = k_destinationReachedDuration + (rand() % 1000);
		m_pElevatorTimer->startSingleTimer(this, duration, static_cast<int>(TimerIDs::DoorOpen));
	}
}

void Elevator::onDestinationReached() {
	// Regardless if there are more destinations, cancel any existing timer
	m_pElevatorTimer->cancelTimer();
	SAFEPRINT("Elevator " + m_elevatorId + " is closing its door on floor " + std::to_string(m_currentFloor));

	// If there are no more destinations, alert controller that the direction is none
	// Otherwise, restart the timer to move floors toward next destination
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
		// Cast to AddDestinationEvent to retrieve destination
		AddDestinationEvent* pDestEvent = static_cast<AddDestinationEvent*>(pEvent);
		addDestination(pDestEvent->getDestination());
	} else {
		SAFEPRINT("Elevator::handleEvent() received unhandled event type " + pEvent->getEventName());
	}
}

void Elevator::addDestination(int destination) {
	if (destination == m_currentFloor) {
		return;
	}

	SAFEPRINT("Elevator " + m_elevatorId + " is adding destination " + std::to_string(destination));

	// Add the destination to the queue
	{
		std::lock_guard<std::mutex> guard(m_queueMutex);
		m_destinationQueue.insert(destination);
	}

	// If the elevator is currently not moving, start a timer to begin movement
	if (m_direction == Direction::NONE) {
		m_direction = (destination < m_currentFloor) ? Direction::DOWN : Direction::UP;
		m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::ELEVATOR_DIRECTION_CHANGE, static_cast<int>(m_direction));

		m_pElevatorTimer->startRepeatingTimer(this, k_floorChangeDuration, static_cast<int>(TimerIDs::FloorChange));
	}
}

} /* elevator */
