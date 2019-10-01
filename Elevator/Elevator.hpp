
#ifndef ELEVATOR_ELEVATOR_HPP
#define EVELATOR_ELEVATOR_HPP

#include "../Common/ComponentBase.hpp"

namespace elevator {

class Elevator : public common::ComponentBase {
public:
	Elevator();
	~Elevator();

	void start() override;
	void stop() override;
};

} /* elevator */

#endif
