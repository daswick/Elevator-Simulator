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

void Controller::updateDestination(int floorNumber) {
	int elevatorId = getBestFitElevator(floorNumber);
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::CONTROLLER_ADD_DESTINATION, floorNumber, elevatorId);
}

void Controller::handleEvent(common::BaseEvent* pEvent) {
	if (pEvent == nullptr) {
		return;
	}

	if (pEvent->getEventName().compare("FloorStatusEvent") == 0) {
		FloorStatusEvent* pFloorEvent = static_cast<FloorStatusEvent*>(pEvent);
		m_elevatorStatuses[pFloorEvent->getElevatorId()].currentFloor = pFloorEvent->getFloorStatus();
	} else {
		// TODO: Other events
	}
}

int Controller::getBestFitElevator(int floorNumber) {
	// TODO: Investigate algorithms
	int bestFitIndex = -1;
	int minDiff = 0x10;

	for (int i = 0; i < m_numElevators; i++) {
		int diff = abs(floorNumber - m_elevatorStatuses[i].currentFloor);
		if (diff < minDiff) {
			minDiff = diff;
			bestFitIndex = i;
		}
	}

	return bestFitIndex;
}

} /* controller */
