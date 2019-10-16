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
	m_pSignalHandler->startReceivers();
}

void Controller::stop() {
	SAFEPRINT("Stopping Controller");
	m_isStarted = false;
	m_pSignalHandler->stopReceivers();
}

void Controller::updateDestination(int floorNumber) {
	int elevatorId = getBestFitElevator(floorNumber);
	m_pSignalHandler->sendSignal(SignalHandler::SignalTypes::CONTROLLER_ADD_DESTINATION, floorNumber, elevatorId);
}

int Controller::getBestFitElevator(int floorNumber) {
	// TODO: Investigate algorithms
	return 0;
}

} /* controller */
