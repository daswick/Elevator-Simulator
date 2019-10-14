#ifndef CONTROLLER_CONTROLLER_CPP
#define CONTROLLER_CONTROLLER_CPP

#include "Controller.hpp"
#include "../Common/EventHandler.cpp"
#include "../Common/SafePrint.cpp"

namespace controller {

Controller::Controller() {
	m_isStarted = false;
	m_pEventHandler = std::make_shared<common::EventHandler>();
}

void Controller::setElevatorCount(int numElevators) {
	m_numElevators = numElevators;
	m_pSignalHandler = std::make_shared<SignalHandler>(m_pEventHandler, numElevators);
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

#endif
