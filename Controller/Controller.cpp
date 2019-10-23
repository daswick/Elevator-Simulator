#include "Controller.hpp"

#include "../Common/EventHandler.hpp"
#include "../Common/SafePrint.hpp"

namespace controller {

Controller::Controller(int numElevators) {
	m_isStarted = false;
	m_numElevators = numElevators;
	m_pEventHandler = std::make_shared<common::EventHandler>();
	m_pSignalHandler = std::make_shared<SignalHandler>(m_pEventHandler, numElevators);

	// Initialize all statuses to be invalid data as to not be considered until changed
	for (int i = 0; i < numElevators; i++) {
		ElevatorStatus status;
		status.currentFloor = -1;
		status.direction = 0;
		m_elevatorStatuses.push_back(status);
	}
}

void Controller::start() {
	SAFEPRINT("Starting Controller");
	m_isStarted = true;
	m_pEventHandler->addListener(this);
	m_pSignalHandler->startReceivers();
}

void Controller::stop() {
	SAFEPRINT("Stopping Controller");
	m_isStarted = false;
	m_pEventHandler->removeListener(this);
	m_pSignalHandler->stopReceivers();
}

bool Controller::updateDestination(int floorNumber) {
	int elevatorId = getBestFitElevator(floorNumber);

	if (elevatorId == -1) {
		return false;
	}

	// Increment the destination count and send the signal to the respective elevator
	m_elevatorStatuses[elevatorId].numDestinations++;
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::CONTROLLER_ADD_DESTINATION, floorNumber, elevatorId);
	return true;
}

void Controller::handleEvent(common::BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	if (pEvent->getEventName().compare("FloorStatusEvent") == 0) {
		// Cast to FloorStatusEvent to retrieve elevator id and floor status
		FloorStatusEvent* pFloorEvent = static_cast<FloorStatusEvent*>(pEvent);
		m_elevatorStatuses[pFloorEvent->getElevatorId()].currentFloor = pFloorEvent->getFloorStatus();
	}
	else if (pEvent->getEventName().compare("DestinationReachedEvent") == 0) {
		// Cast to DestinationReachedEvent to retrieve elevator id. Destination reached unused
		DestinationReachedEvent* pDestEvent = static_cast<DestinationReachedEvent*>(pEvent);
		m_elevatorStatuses[pDestEvent->getElevatorId()].numDestinations--;
	}
	else if (pEvent->getEventName().compare("DirectionChangedEvent") == 0) {
		// Case to DirectionChangedEvent to retrieve elevator id and direction
		DirectionChangedEvent* pDirEvent = static_cast<DirectionChangedEvent*>(pEvent);
		m_elevatorStatuses[pDirEvent->getElevatorId()].direction = pDirEvent->getDirection();

		// If there are any destinations that could not find a suitable elevator, reattempt the search
		std::vector<int>::iterator it;
		for (it = m_remainingDestinations.begin(); it != m_remainingDestinations.end(); it++) {
			if (updateDestination(m_remainingDestinations[*it])) {
				m_remainingDestinations.erase(it);
			}
		}
	} else {
		SAFEPRINT("Controller::handleEvent() received unhandled event type " + pEvent->getEventName());
	}
}

int Controller::getBestFitElevator(int floorNumber) {
	// Assume there is no elevator that is best-fit and the minimum difference is the number of floors
	int bestFitIndex = -1;
	int minDiff = 0x100;

	// Iterate over all of the elevators, updating the best fit accordingly
	for (int i = 0; i < m_numElevators; i++) {
		int diff = floorNumber - m_elevatorStatuses[i].currentFloor;

		// If the elevator is going in the opposite direction of the floor, disregard it
		if (m_elevatorStatuses[i].direction == static_cast<int>(ElevatorDirection::UP) && diff < 0) {
			continue;
		}
		else if (m_elevatorStatuses[i].direction == static_cast<int>(ElevatorDirection::DOWN) && diff > 0) {
			continue;
		}

		// Factor in number of destinations to reduce wait time and starvation
		int absDiff = abs(diff);
		int scaledDiff = (k_floorChange * absDiff) + (k_destinationReached * m_elevatorStatuses[i].numDestinations);

		if (scaledDiff < minDiff) {
			minDiff = scaledDiff;
			bestFitIndex = i;
		}
	}

	// If no elevator was found, -1 is returned, indicating the elevator should be added to the remaining destinations list
	return bestFitIndex;
}

} /* controller */
