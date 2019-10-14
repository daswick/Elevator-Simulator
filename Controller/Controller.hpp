#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include "../Common/ComponentBase.hpp"
#include "SignalHandler.cpp"

namespace controller {

class Controller : public common::ComponentBase {
public:
	Controller();
	~Controller() = default;

	void start() override;
	void stop() override;

	void setElevatorCount(int numElevators);

private:
	int m_numElevators;
	std::shared_ptr<SignalHandler> m_pSignalHandler;
	std::shared_ptr<common::EventHandler> m_pEventHandler;

	void updateDestination(int floorNumber);
	int getBestFitElevator(int floorNumber);
};

} /* controller */

#endif

