#include "SignalHandler.hpp"

#include "events/DestinationReachedEvent.hpp"
#include "events/DirectionChangedEvent.hpp"
#include "events/FloorStatusEvent.hpp"

#include "../Common/SafePrint.hpp"

namespace controller {

SignalHandler::SignalHandler(std::shared_ptr<common::EventHandler> pEventHandler, int numElevators) {
	m_isRunning = false;
	m_elevatorCount = numElevators;
	m_messageId = 0;

	m_pSignalReceiver = std::make_unique<common::SignalReceiver>();
	m_pEventHandler = pEventHandler;
}

void SignalHandler::sendSignal(SignalTypes type, int data, int elevator) {
	if (!m_isRunning) {
		return;
	}

	common::Signal msg;
	msg.m_data[0] = static_cast<int>(type);
	msg.m_data[1] = data;

	int elevatorId = 0x10 | elevator;
	m_pSignalReceiver->sendMessage(elevatorId, msg);
}

void SignalHandler::startReceivers() {
	if (m_isRunning) {
		return;
	}

	m_isRunning = true;
	m_pSignalReceiver->addListener(this);

	for (int i = 0; i < m_elevatorCount; i++) {
		int elevatorID = 0x20 | i;
		m_pSignalReceiver->start(elevatorID);
	}
}

void SignalHandler::stopReceivers() {
	if (!m_isRunning) {
		return;
	}

	for (int i = 0; i < m_elevatorCount; i++) {
		int elevatorId = 0x20 | i;
		m_pSignalReceiver->stop(elevatorId);
	}
}

void SignalHandler::handleSignal(common::Signal msg) {
	int signalType = int(msg.m_data[0]);
	SAFEPRINT("Controller received signal " + m_pSignalReceiver->getSignalName(signalType)
			+ " with data " + std::to_string(int(msg.m_data[1])));

	if (signalType == static_cast<int>(SignalTypes::ELEVATOR_FLOOR_STATUS)) {
		int floor = int(msg.m_data[1]);
		int elevatorId = int(msg.m_data[2]) & 0xF;
		FloorStatusEvent* pEvent = new FloorStatusEvent(floor, elevatorId);
		m_pEventHandler->publishEvent(pEvent);
	}
	else if (signalType == static_cast<int>(SignalTypes::ELEVATOR_DESTINATION_REACHED)) {
		int floor = int(msg.m_data[1]);
		int elevatorId = int(msg.m_data[2]) & 0xF;
		DestinationReachedEvent* pEvent = new DestinationReachedEvent(floor, elevatorId);
		m_pEventHandler->publishEvent(pEvent);
	}
	else if (signalType == static_cast<int>(SignalTypes::ELEVATOR_DIRECTION_CHANGE)) {
		int direction = int(msg.m_data[1]);
		int elevatorId = int(msg.m_data[2]) & 0xF;
		DirectionChangedEvent* pEvent = new DirectionChangedEvent(direction, elevatorId);
		m_pEventHandler->publishEvent(pEvent);
	} else {
		SAFEPRINT("Controller::SignalHandler received unknown signal type " + std::to_string(signalType));
	}
}

} /* controller */
