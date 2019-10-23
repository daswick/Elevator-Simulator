#include "ElevatorSimulator.hpp"

ElevatorSimulator::ElevatorSimulator() {
	m_elevatorCount = 2;

	for (int i = 0; i < m_elevatorCount; i++) {
		m_pElevatorList.push_back(nullptr);
	}

	m_pController = nullptr;
}

ElevatorSimulator::ElevatorSimulator(int elevatorCount) {
	m_elevatorCount = elevatorCount;

	for (int i = 0; i < m_elevatorCount; i++) {
		m_pElevatorList.push_back(nullptr);
	}

	m_pController = nullptr;
}

void ElevatorSimulator::start() {
	startController(m_elevatorCount);

	for (int i = 0; i < m_elevatorCount; i++) {
		startElevator(i);
	}
}

void ElevatorSimulator::stop() {
	for (int i = 0; i < m_elevatorCount; i++) {
		stopElevator(i);
	}

	stopController();
}

void ElevatorSimulator::startElevator(int id) {
	if (m_pElevatorList[id] == nullptr) {
		m_pElevatorList[id] = std::make_unique<elevator::Elevator>(std::to_string(id));
	}

	if (m_pElevatorList[id]->hasStarted()) {
		return;
	}

	m_pElevatorList[id]->start();
}

void ElevatorSimulator::stopElevator(int id) {
	if (m_pElevatorList[id] == nullptr) {
		return;
	}

	if (!m_pElevatorList[id]->hasStarted()) {
		return;
	}

	m_pElevatorList[id]->stop();
}

void ElevatorSimulator::startController(int numElevators) {
	if (m_pController == nullptr) {
		m_pController = std::make_unique<controller::Controller>(numElevators);
	}

	if (m_pController->hasStarted()) {
		return;
	}

	m_pController->start();
}

void ElevatorSimulator::stopController() {
	if (m_pController == nullptr) {
		return;
	}

	if (!m_pController->hasStarted()) {
		return;
	}

	m_pController->stop();
}
