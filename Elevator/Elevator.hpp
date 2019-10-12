#ifndef ELEVATOR_ELEVATOR_HPP
#define EVELATOR_ELEVATOR_HPP

#include "../Common/ComponentBase.hpp"
#include "../Common/CoreTimer.cpp"
#include "../Common/EventHandler.cpp"

#include "PersistedData.cpp"
#include "SignalHandler.cpp"

#include <memory>
#include <queue>

namespace elevator {

class Elevator : public common::ComponentBase,
                 public common::CoreTimer::TimerTask,
				 public common::EventHandler::EventListener {
public:
	Elevator();
	~Elevator();

	void setId(std::string id);

	void start() override;
	void stop() override;

	void coreTimerCallback() override;
	void handleEvent(common::BaseEvent* pEvent) override;

private:
	enum Direction {
		NONE = 0x0,
		UP = 0x1,
		DOWN = 0x2
	};

	const int k_floorChangeDuration = 2000;

	std::string m_elevatorId;
	int m_currentFloor;
	Direction m_direction;

	std::priority_queue<int, std::vector<int>, std::greater<int>> m_destinationQueue;

	std::shared_ptr<PersistedData> m_pPersistence;
	std::unique_ptr<SignalHandler> m_pSignalHandler;
	std::unique_ptr<common::CoreTimer> m_pElevatorTimer;
	std::shared_ptr<common::EventHandler> m_pEventHandler;
};

} /* elevator */

#endif
