#ifndef CONTROLLER_SIGNALHANDLER_CPP
#define CONTROLLER_SIGNALHANDLER_CPP

#include "SignalHandler.hpp"

namespace controller {

SignalHandler::SignalHandler(std::shared_ptr<common::EventHandler> pEventHandler, int numElevators) {
	m_isRunning = false;
	m_elevatorCount = numElevators;
	m_messageId = 0;

	m_pSignalSender = std::make_unique<common::SignalSender>();
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
	m_pSignalSender->sendMessage(elevatorId, msg);
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
	SAFEPRINT("Controller received signal " + m_pSignalReceiver->getSignalName(int(msg.m_data[0]))
			+ " with data " + std::to_string(int(msg.m_data[1])));
}

} /* controller */

#endif
