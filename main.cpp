#include <iostream>

#include "Elevator/EntryPoint.cpp"
#include "Controller/EntryPoint.cpp"

void getElevator() {
	std::cout << "Retreiving elevator component\n";
	common::ComponentBase* ev1 = elevator::getComponent();

	std::cout << "Starting elevator component\n";
	ev1->start();

	std::cout << "Stopping elevator component\n";
	ev1->stop();

	std::cout << "Deleting elevator component\n";
	delete ev1;
}

void getController() {
	std::cout << "Retreiving controller component\n";
	common::ComponentBase* ctr = controller::getComponent();

	std::cout << "Starting controller component\n";
	ctr->start();

	std::cout << "Stopping controller component\n";
	ctr->stop();

	std::cout << "Deleting controller component\n";
	delete ctr;
}

int main() {
	getElevator();

	getController();

	return 0;
}
