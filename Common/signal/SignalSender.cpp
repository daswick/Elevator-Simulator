#ifndef COMMON_SIGNALSENDER_CPP
#define COMMON_SIGNALSENDER_CPP

#include "SignalSender.hpp"

#include "../SafePrint.cpp"

namespace common {

void SignalSender::sendMessage(int msgkey, Signal msg) {
	key_t key = ftok(".", msgkey);

	int messageId = msgget(key, IPC_CREAT | 0660);
	if (messageId == ERROR) {
		return;
	}

	msg.m_type = 1;

	if (msgsnd(messageId, &msg, 5, 0) == ERROR) {
		SAFEPRINT("Failed to send message");
	} else {
		SAFEPRINT("Sent message: " + std::string(msg.m_data));
	}
}

} /* common */

#endif
