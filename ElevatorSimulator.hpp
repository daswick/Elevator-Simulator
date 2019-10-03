#ifndef ELEVATORSIMULATOR_HPP
#define ELEVATORSIMULATOR_HPP

#include <vector>

#include "Elevator/Elevator.cpp"
#include "Controller/Controller.cpp"

class ElevatorSimulator {
public:
	ElevatorSimulator();
	ElevatorSimulator(int elevatorCount);
	~ElevatorSimulator();

	void start();
	void stop();

private:
	int m_elevatorCount;
	
	std::vector<elevator::Elevator*> m_pElevatorList;
	controller::Controller* m_pController;
	
	void startElevator(int id);
	void stopElevator(int id);

	void startController();
	void stopController();
};

#endif
