#include "Controller.hpp"

#include "../Common/EventHandler.hpp"
#include "../Common/SafePrint.hpp"

namespace controller {

Controller::Controller(int numElevators) {
	m_isStarted = false;
	m_numElevators = numElevators;
	m_pEventHandler = std::make_shared<common::EventHandler>();
	m_pSignalHandler = std::make_shared<SignalHandler>(m_pEventHandler, numElevators);

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

	m_elevatorStatuses[elevatorId].numDestinations++;
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::CONTROLLER_ADD_DESTINATION, floorNumber, elevatorId);
	return true;
}

void Controller::handleEvent(common::BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	if (pEvent->getEventName().compare("FloorStatusEvent") == 0) {
		FloorStatusEvent* pFloorEvent = static_cast<FloorStatusEvent*>(pEvent);
		m_elevatorStatuses[pFloorEvent->getElevatorId()].currentFloor = pFloorEvent->getFloorStatus();
	}
	else if (pEvent->getEventName().compare("DestinationReachedEvent") == 0) {
		DestinationReachedEvent* pDestEvent = static_cast<DestinationReachedEvent*>(pEvent);
		m_elevatorStatuses[pDestEvent->getElevatorId()].numDestinations--;
	}
	else if (pEvent->getEventName().compare("DirectionChangedEvent") == 0) {
		DirectionChangedEvent* pDirEvent = static_cast<DirectionChangedEvent*>(pEvent);
		m_elevatorStatuses[pDirEvent->getElevatorId()].direction = pDirEvent->getDirection();
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
	int bestFitIndex = -1;
	int minDiff = 0x10;

	for (int i = 0; i < m_numElevators; i++) {
		int diff = floorNumber - m_elevatorStatuses[i].currentFloor;
		int absDiff = abs(diff);

		if (m_elevatorStatuses[i].direction == static_cast<int>(ElevatorDirection::UP) && diff < 0) {
			continue;
		}
		else if (m_elevatorStatuses[i].direction == static_cast<int>(ElevatorDirection::DOWN) && diff > 0) {
			continue;
		}

		// TODO: Should distance vs. remaining queue affect this?
		if (absDiff < minDiff) {
			minDiff = diff;
			bestFitIndex = i;
		}
	}

	return bestFitIndex;
}

} /* controller */
