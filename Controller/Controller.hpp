#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include "SignalHandler.hpp"

#include "../Common/ComponentBase.hpp"

namespace controller {

class Controller : public common::ComponentBase {
public:
	Controller() = delete;
	explicit Controller(int numElevators);
	virtual ~Controller() = default;

	void start() override;
	void stop() override;

private:
	struct ElevatorStatus {
		int currentFloor;
		int direction;
	};

	int m_numElevators;
	std::shared_ptr<SignalHandler> m_pSignalHandler;
	std::shared_ptr<common::EventHandler> m_pEventHandler;
	std::vector<ElevatorStatus> m_elevatorStatuses;

	void updateDestination(int floorNumber);
	int getBestFitElevator(int floorNumber);
};

} /* controller */

#endif /* CONTROLLER_CONTROLLER_HPP */

