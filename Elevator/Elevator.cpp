#ifndef ELEVATOR_ELEVATOR_CPP
#define ELEVATOR_ELEVATOR_CPP

#include "Elevator.hpp"

#include <iostream>

namespace elevator {

Elevator::Elevator() {
	// Not implemented yet
}

Elevator::~Elevator() {
	// Not implemented yet
}

void Elevator::start() {
	std::cout << "Starting Elevator\n";
	m_isStarted = true;
}

void Elevator::stop() {
	std::cout << "Stopping Elevator\n";
	m_isStarted = false;
}

} /* elevator */

#endif
