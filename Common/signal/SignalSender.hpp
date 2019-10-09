#ifndef COMMON_SIGNALSENDER_HPP
#define COMMON_SIGNALSENDER_HPP

#define ERROR -1

#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "SignalReceiver.hpp"

namespace common {

class SignalSender {
public:
	SignalSender() = default;
	~SignalSender() = default;

	void sendMessage(int msgkey, Signal msg);
};

} /* common */

#endif
