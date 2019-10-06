#include "ElevatorSimulator.cpp"

int main() {
	ElevatorSimulator simulator;

	simulator.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	simulator.stop();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	return 0;
}
