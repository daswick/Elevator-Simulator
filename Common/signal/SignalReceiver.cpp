#ifndef COMMON_SIGNALRECEIVER_CPP
#define COMMON_SIGNALRECEIVER_CPP

#include "SignalReceiver.hpp"

namespace common {

SignalReceiver::SignalReceiver() {
	m_isRunning = false;
	m_messageId = ERROR;
}

SignalReceiver::~SignalReceiver() {
	// Do nothing
}

void SignalReceiver::start(int key) {
	if (m_isRunning) {
		return;
	}

	SAFEPRINT("SignalHandler::start()");

	m_key = ftok(".", key);
	m_isRunning = true;
	m_messageId = msgget(m_key, IPC_CREAT | 0660);

	m_messageThread = std::thread(&common::SignalReceiver::receiveMessages, this);
	m_messageThread.detach();
}

void SignalReceiver::stop() {
	if (!m_isRunning) {
		return;
	}

	SAFEPRINT("SignalReceiver::stop()");

	if (m_messageId == ERROR) {
		return;
	}

	msgctl(m_messageId, IPC_RMID, NULL);

	m_isRunning = false;
}

void SignalReceiver::receiveMessages() {
	if (m_messageId == ERROR) {
		return;
	}

	Signal msg;
	while (true) {
		int status = msgrcv(m_messageId, &msg, 5, 1, 0);
		if (status == ERROR) {
			break;
		}

		SAFEPRINT("Received message of type " + std::to_string(int(msg.m_data[0])) + " and data " + std::to_string(int(msg.m_data[1])));

		// Send signal
	}
}

} /* common */

#endif
