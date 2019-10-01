#include "Controller.cpp"

namespace controller {

common::ComponentBase* getComponent() {
	return new Controller();
}

} /* controller */

