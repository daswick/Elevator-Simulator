#ifndef ELEVATOR_SIGNALHANDLER_HPP
#define ELEVATOR_SIGNALHANDLER_HPP

#include "../Common/signal/SignalReceiver.cpp"
#include "../Common/signal/SignalSender.cpp"

namespace elevator {

class SignalHandler {
public:
	SignalHandler();
	~SignalHandler();

	enum SignalTypes {
		ELEVATOR_FLOOR_STATUS = 0x01,
		ELEVATOR_DESTINATION_REACHED = 0x02,
		ELEVATOR_RESERVED_1 = 0x04,
		ELEVATOR_RESERVED_2 = 0x08,
		CONTROLLER_ADD_DESTINATION = 0x10,
		CONTROLLER_RESERVED_1 = 0x20,
		FLOOR_BUTTON_PRESSED = 0x40,
		FLOOR_RESERVED_1 = 0x80
	};

	void sendSignal(SignalTypes type, int data);

	void startReceiver(int elevatorId);
	void stopReceiver();

private:
	std::unique_ptr<common::SignalReceiver> m_pSignalReceiver;
	std::unique_ptr<common::SignalSender> m_pSignalSender;

	int m_messageId;
	bool m_isRunning;
};

} /* elevator */

#endif
