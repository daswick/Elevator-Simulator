#ifndef ELEVATOR_ELEVATOR_HPP
#define EVELATOR_ELEVATOR_HPP

#include "events/AddDestinationEvent.hpp"
#include "PersistedData.hpp"
#include "SignalHandler.hpp"

#include "../Common/ComponentBase.hpp"
#include "../Common/CoreTimer.hpp"
#include "../Common/EventHandler.hpp"

#include <memory>
#include <mutex>
#include <unordered_set>

namespace elevator {

class Elevator : public common::ComponentBase,
                 public common::CoreTimer::TimerTask,
                 public common::EventHandler::EventListener {
public:
	Elevator() = delete;
	explicit Elevator(std::string id);
	virtual ~Elevator();

	void start() override;
	void stop() override;

	void coreTimerCallback(int timerId) override;
	void handleEvent(common::BaseEvent* pEvent) override;

private:
	enum Direction {
		NONE = 0x0,
		UP = 0x1,
		DOWN = 0x2
	};

	enum TimerIDs {
		FloorChange = 0x1,
		DoorOpen = 0x2
	};

	const int k_floorChangeDuration = 2000;
	const int k_destinationReachedDuration = 5000;

	std::string m_elevatorId;
	int m_currentFloor;
	Direction m_direction;

	std::mutex m_queueMutex;
	std::unordered_set<int> m_destinationQueue;

	std::shared_ptr<PersistedData> m_pPersistence;
	std::unique_ptr<SignalHandler> m_pSignalHandler;
	std::unique_ptr<common::CoreTimer> m_pElevatorTimer;
	std::shared_ptr<common::EventHandler> m_pEventHandler;

	void addDestination(int destination);
	void moveElevator();
	void checkForDestination();
	void onDestinationReached();
};

} /* elevator */

#endif /* EVELATOR_ELEVATOR_HPP */
