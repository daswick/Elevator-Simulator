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
	// If the key currently exists, return. Otherwise, insert it
	{
		std::lock_guard<std::mutex> guard(m_messageMutex);
		std::set<int>::iterator it = m_messageIds.find(key);
		if (it != m_messageIds.end()) {
			return;
		}
		m_messageIds.insert(key);
	}

	// Create key used for message passing. 0660 indicates it has read/write access
	// And IPC_CREAT signifies it should create it if it does not exist
	key_t msgkey = ftok(".", key);
	int messageId = msgget(msgkey, IPC_CREAT | 0660);

	std::thread messageThread = std::thread(&common::SignalReceiver::receiveMessages, this, messageId);
	messageThread.detach();
}

void SignalReceiver::stop(int key) {
	// If the key does not exist, return. Otherwise, remove it
	{
		std::lock_guard<std::mutex> guard(m_messageMutex);
		std::set<int>::iterator it = m_messageIds.find(key);
		if (it != m_messageIds.end()) {
			return;
		}
		m_messageIds.erase(it);
	}

	key_t msgkey = ftok(".", key);
	int messageId = msgget(msgkey, IPC_CREAT | 0660);

	msgctl(messageId, IPC_RMID, NULL);
}

void SignalReceiver::addListener(ISignalListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	// Since m_pReceiverList is a set, this will not create a duplicate
	m_pReceiverList.insert(pListener);
}

void SignalReceiver::removeListener(ISignalListener* pListener) {
	if (pListener == nullptr) {
		return;
	}

	// If the listener is in the list, remove it
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

	// If the receiving end is closed, this will return -1
	int messageId = msgget(key, 0660);
	if (messageId == ERROR) {
		return;
	}

	msg.m_type = 1;
	msg.m_data[2] = msgkey;

	// This will wake up the receiving thread to read the signal
	if (msgsnd(messageId, &msg, k_bufferSize, 0) == ERROR) {
		SAFEPRINT("Failed to send message");
	}
}

void SignalReceiver::receiveMessages(int messageId) {
	if (messageId == ERROR) {
		return;
	}

	// Attempt to receive messages (blocking) until an error is received
	// An error will be produced when the message queue is terminated
	Signal msg;
	while (true) {
		int status = msgrcv(messageId, &msg, k_bufferSize, 1, 0);
		if (status == ERROR) {
			break;
		}

		// Notify all listeners of a new message
		for (ISignalListener* listener : m_pReceiverList) {
			listener->handleSignal(msg);
		}
	}
}

} /* common */
