#ifndef COMMON_SIGNALRECEIVER_HPP
#define COMMON_SIGNALRECEIVER_HPP

#define ERROR -1

#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <thread>

namespace common {

struct Signal {
	long m_type;
	char m_data[5];
};

class SignalReceiver {
public:
	SignalReceiver();
	~SignalReceiver();

	void start(int key);
	void stop();

	class ISignalReceiver {
	public:
		ISignalReceiver() = default;
		~ISignalReceiver() = default;

		virtual void handleSignal(Signal msg) { /* Not implemented */ }
	};

private:
	std::thread m_messageThread;
	key_t m_key;
	bool m_isRunning;
	int m_messageId;

	void receiveMessages();

};


} /* common */

#endif
