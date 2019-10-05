#include "ElevatorSimulator.hpp"

#include <thread>

#include "Elevator/EntryPoint.cpp"
#include "Controller/EntryPoint.cpp"

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

ElevatorSimulator::~ElevatorSimulator() {
	for (int i = 0; i < m_elevatorCount; i++) {
		delete m_pElevatorList[i];
	}

	delete m_pController;
}

void ElevatorSimulator::start() {
	for (int i = 0; i < m_elevatorCount; i++) {
		startElevator(i);
	}

	startController();
}

void ElevatorSimulator::stop() {
	for (int i = 0; i < m_elevatorCount; i++) {
		stopElevator(i);
	}

	stopController();
}

void ElevatorSimulator::startElevator(int id) {
	if (m_pElevatorList[id] == nullptr) {
		m_pElevatorList[id] = static_cast<elevator::Elevator*>(elevator::getComponent());
	}

	if (m_pElevatorList[id]->hasStarted()) {
		return;
	}

	m_pElevatorList[id]->setId(std::to_string(id));
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

void ElevatorSimulator::startController() {
	if (m_pController == nullptr) {
		m_pController = static_cast<controller::Controller*>(controller::getComponent());
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
