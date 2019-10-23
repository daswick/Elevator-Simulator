#ifndef ELEVATORSIMULATOR_HPP
#define ELEVATORSIMULATOR_HPP

#include "Controller/Controller.hpp"
#include "Elevator/Elevator.hpp"

#include <memory>
#include <vector>

class ElevatorSimulator {
public:
	ElevatorSimulator();
	ElevatorSimulator(int elevatorCount);
	~ElevatorSimulator() = default;

	void start();
	void stop();

private:
	int m_elevatorCount;
	
	std::vector<std::unique_ptr<elevator::Elevator>> m_pElevatorList;
	std::unique_ptr<controller::Controller> m_pController;
	
	void startElevator(int id);
	void stopElevator(int id);

	void startController(int numElevators);
	void stopController();
};

#endif /* ELEVATORSIMULATOR_HPP */
