#ifndef CONTROLLER_CONTROLLER_HPP
#define CONTROLLER_CONTROLLER_HPP

#include "events/DestinationReachedEvent.hpp"
#include "events/DirectionChangedEvent.hpp"
#include "events/FloorStatusEvent.hpp"
#include "SignalHandler.hpp"

#include "../Common/ComponentBase.hpp"

#include <memory>

namespace controller {

class Controller : public common::ComponentBase,
                   public common::EventHandler::EventListener {
public:
	Controller() = delete;
	explicit Controller(int numElevators);
	virtual ~Controller() = default;

	void start() override;
	void stop() override;

	void handleEvent(common::BaseEvent* pEvent);

private:
	enum ElevatorDirection {
		NONE = 0x0,
		UP = 0x1,
		DOWN = 0x2
	};

	struct ElevatorStatus {
		int currentFloor;
		int direction;
		int numDestinations;
	};

	const int k_floorChange = 2;
	const int k_destinationReached = 5;

	int m_numElevators;
	std::shared_ptr<SignalHandler> m_pSignalHandler;
	std::shared_ptr<common::EventHandler> m_pEventHandler;
	std::vector<ElevatorStatus> m_elevatorStatuses;
	std::vector<int> m_remainingDestinations;

	bool updateDestination(int floorNumber);
	int getBestFitElevator(int floorNumber);
};

} /* controller */

#endif /* CONTROLLER_CONTROLLER_HPP */

