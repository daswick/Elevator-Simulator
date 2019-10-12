#ifndef ELEVATOR_SIGNALHANDLER_CPP
#define ELEVATOR_SIGNALHANDLER_CPP

#include "SignalHandler.hpp"
#include "events/AddDestinationEvent.hpp"

namespace elevator {

SignalHandler::SignalHandler(std::shared_ptr<common::EventHandler> pEventHandler) {
	m_isRunning = false;
	m_messageId = 0;

	m_pSignalSender = std::make_unique<common::SignalSender>();
	m_pSignalReceiver = std::make_unique<common::SignalReceiver>();
	m_pEventHandler = pEventHandler;
}

SignalHandler::~SignalHandler() {

}

void SignalHandler::sendSignal(SignalTypes type, int data) {
	common::Signal message;
	message.m_data[0] = static_cast<char>(type);
	message.m_data[1] = data;

	m_pSignalSender->sendMessage(m_messageId, message);
}

void SignalHandler::startReceiver(int elevatorId) {
	if (m_isRunning) {
		return;
	}

	m_messageId = 0x10 | elevatorId;

	m_pSignalReceiver->addListener(this);
	m_pSignalReceiver->start(m_messageId);

	m_isRunning = true;
}

void SignalHandler::stopReceiver() {
	if (!m_isRunning) {
		return;
	}

	m_pSignalReceiver->stop();
	m_pSignalReceiver->removeListener(this);

	m_isRunning = false;
}

void SignalHandler::handleSignal(common::Signal msg) {
	if (int(msg.m_data[0]) == static_cast<int>(SignalTypes::CONTROLLER_ADD_DESTINATION)) {
		AddDestinationEvent* pEvent = new AddDestinationEvent(int(msg.m_data[1]));
		m_pEventHandler->publishEvent(pEvent);
	} else {
		// TODO:
	}
}

} /* elevator */

#endif
