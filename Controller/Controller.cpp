#ifndef CONTROLLER_CONTROLLER_CPP
#define CONTROLLER_CONTROLLER_CPP

#include "Controller.hpp"
#include "../Common/SafePrint.cpp"

namespace controller {

Controller::Controller() {
	m_isStarted = false;
}

Controller::~Controller() {
	// Not implemented yet
}

void Controller::start() {
	SAFEPRINT("Starting Controller");
	m_isStarted = true;
}

void Controller::stop() {
	SAFEPRINT("Stopping Controller");
	m_isStarted = false;
}

} /* controller */

#endif
