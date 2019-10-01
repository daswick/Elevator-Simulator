#include "Elevator.cpp"

namespace elevator {

common::ComponentBase* getComponent() {
	return new Elevator();
}

} /* elevator */

