#ifndef ELEVATOR_SIGNALHANDLER_CPP
#define ELEVATOR_SIGNALHANDLER_CPP

#include "SignalHandler.hpp"

namespace elevator {

SignalHandler::SignalHandler() {
	m_isRunning = false;
	m_messageId = 0;

	m_pSignalSender = std::make_unique<common::SignalSender>();
	m_pSignalReceiver = std::make_unique<common::SignalReceiver>();
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

	m_pSignalReceiver->start(m_messageId);

	m_isRunning = true;
}

void SignalHandler::stopReceiver() {
	if (!m_isRunning) {
		return;
	}

	m_pSignalReceiver->stop();

	m_isRunning = false;
}

} /* elevator */

#endif
