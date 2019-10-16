#include "ElevatorSimulator.hpp"

int main() {
	ElevatorSimulator simulator;

	simulator.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	simulator.stop();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	return 0;
}
