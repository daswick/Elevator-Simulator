#ifndef CONTROLLER_CONTROLLER_CPP
#define CONTROLLER_CONTROLLER_CPP

#include "Controller.hpp"

#include <iostream>

namespace controller {

Controller::Controller() {
	// Not implemented yet
}

Controller::~Controller() {
	// Not implemented yet
}

void Controller::start() {
	std::cout << "Starting Controller\n";
	m_isStarted = true;
}

void Controller::stop() {
	std::cout << "Stopping Controller\n";
	m_isStarted = false;
}

} /* controller */

#endif
