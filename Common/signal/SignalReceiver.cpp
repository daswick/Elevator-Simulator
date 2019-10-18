#include "SignalReceiver.hpp"

#include "../SafePrint.hpp"

namespace common {

SignalReceiver::~SignalReceiver() {
	while (m_pReceiverList.begin() != m_pReceiverList.end()) {
		m_pReceiverList.erase(m_pReceiverList.begin());
	}

	while (m_messageIds.begin() != m_messageIds.end()) {
		m_messageIds.erase(m_messageIds.begin());
	}
}

void SignalReceiver::start(int key) {
	{
		std::lock_guard<std::mutex> guard(m_messageMutex);
		std::set<int>::iterator it = m_messageIds.find(key);;
		if (it != m_messageIds.end()) {
			return;
		}
		m_messageIds.insert(key);
	}

	m_isRunning = true;
	key_t msgkey = ftok(".", key);
	int messageId = msgget(msgkey, IPC_CREAT | 0660);

	std::thread messageThread = std::thread(&common::SignalReceiver::receiveMessages, this, messageId);
	messageThread.detach();
}

void SignalReceiver::stop(int key) {
	{
		std::lock_guard<std::mutex> guard(m_messageMutex);
		std::set<int>::iterator it = m_messageIds.find(key);;
		if (it != m_messageIds.end()) {
			return;
		}
		m_messageIds.erase(it);
	}

	key_t msgkey = ftok(".", key);
	int messageId = msgget(msgkey, IPC_CREAT | 0660);

	m_isRunning = (m_messageIds.size() == 0);

	msgctl(messageId, IPC_RMID, NULL);
}

void SignalReceiver::addListener(ISignalListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	m_pReceiverList.insert(pListener);
}

void SignalReceiver::removeListener(ISignalListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	std::set<ISignalListener*>::iterator it = m_pReceiverList.find(pListener);
	if (it != m_pReceiverList.end()) {
		m_pReceiverList.erase(it);
	}
}

std::string SignalReceiver::getSignalName(int id) {
	return m_signalNames[id];
}

void SignalReceiver::sendMessage(int msgkey, Signal msg) {
	key_t key = ftok(".", msgkey);

	int messageId = msgget(key, 0660);
	if (messageId == ERROR) {
		return;
	}

	msg.m_type = 1;
	msg.m_data[2] = msgkey;

	if (msgsnd(messageId, &msg, 5, 0) == ERROR) {
		SAFEPRINT("Failed to send message");
	}
}

void SignalReceiver::receiveMessages(int messageId) {
	if (messageId == ERROR) {
		return;
	}

	Signal msg;
	while (true) {
		int status = msgrcv(messageId, &msg, 5, 1, 0);
		if (status == ERROR) {
			break;
		}

		for (ISignalListener* listener : m_pReceiverList) {
			listener->handleSignal(msg);
		}
	}
}

} /* common */
