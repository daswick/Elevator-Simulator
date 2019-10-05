#include "ElevatorSimulator.cpp"

int main() {
	ElevatorSimulator simulator;

	simulator.start();

	std::this_thread::sleep_for(std::chrono::milliseconds(5000));

	simulator.stop();

	return 0;
}
