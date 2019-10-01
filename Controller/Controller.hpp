#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include "../Common/ComponentBase.hpp"

namespace controller {

class Controller : public common::ComponentBase {
public:
	Controller();
	~Controller();

	void start() override;
	void stop() override;
};

} /* controller */

#endif

