#ifndef COMMON_SIGNALRECEIVER_HPP
#define COMMON_SIGNALRECEIVER_HPP

#define ERROR -1

#include <set>
#include <string>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <thread>

namespace common {

struct Signal {
	long m_type;
	char m_data[3];
};

class SignalReceiver {
public:
	SignalReceiver();
	~SignalReceiver();

	void start(int key);
	void stop();

	class ISignalListener {
	public:
		ISignalListener() = default;
		~ISignalListener() = default;

		virtual void handleSignal(Signal msg) { /* Not implemented */ }
	};

	void addListener(ISignalListener* pListener);
	void removeListener(ISignalListener* pListener);

private:
	std::thread m_messageThread;
	key_t m_key;
	bool m_isRunning;
	int m_messageId;

	std::set<ISignalListener*> m_pReceiverList;

	void receiveMessages();

};


} /* common */

#endif
