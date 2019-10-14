#ifndef COMMON_SIGNALRECEIVER_HPP
#define COMMON_SIGNALRECEIVER_HPP

#define ERROR -1

#include <map>
#include <mutex>
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
	void stop(int key);

	class ISignalListener {
	public:
		ISignalListener() = default;
		~ISignalListener() = default;

		virtual void handleSignal(Signal msg) { /* Not implemented */ }
	};

	void addListener(ISignalListener* pListener);
	void removeListener(ISignalListener* pListener);

	std::string getSignalName(int id);

private:
	bool m_isRunning;

	std::mutex m_messageMutex;
	std::set<int> m_messageIds;
	std::set<ISignalListener*> m_pReceiverList;

	void receiveMessages(int messageId);

	std::map<int, std::string> m_signalNames {
		{0x01, "ELEVATOR_FLOOR_STATUS"},
		{0x02, "ELEVATOR_DESTINATION_REACHED"},
		{0x04, "ELEVATOR_DIRECTION_CHANGE"},
		{0x08, "ELEVATOR_RESERVED_2"},
		{0x10, "CONTROLLER_ADD_DESTINATION"},
		{0x20, "CONTROLLER_RESERVED_1"},
		{0x40, "FLOOR_BUTTON_PRESSED"},
		{0x80, "FLOOR_RESERVED_1"}

	};

};


} /* common */

#endif
