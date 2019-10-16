#ifndef ELEVATOR_SIGNALHANDLER_HPP
#define ELEVATOR_SIGNALHANDLER_HPP

#include "../Common/EventHandler.hpp"
#include "../Common/signal/SignalReceiver.hpp"

namespace elevator {

class SignalHandler : public common::SignalReceiver::ISignalListener {
public:
	SignalHandler() = delete;
	explicit SignalHandler(std::shared_ptr<common::EventHandler> pEventHandler);
	~SignalHandler() = default;

	enum SignalTypes {
		ELEVATOR_FLOOR_STATUS = 0x01,
		ELEVATOR_DESTINATION_REACHED = 0x02,
		ELEVATOR_DIRECTION_CHANGE = 0x04,
		ELEVATOR_RESERVED_2 = 0x08,
		CONTROLLER_ADD_DESTINATION = 0x10,
		CONTROLLER_RESERVED_1 = 0x20,
		FLOOR_BUTTON_PRESSED = 0x40,
		FLOOR_RESERVED_1 = 0x80
	};

	void sendSignal(SignalTypes type, int data);

	void startReceiver(int elevatorId);
	void stopReceiver();

	void handleSignal(common::Signal msg) override;

private:
	std::unique_ptr<common::SignalReceiver> m_pSignalReceiver;
	std::shared_ptr<common::EventHandler> m_pEventHandler;

	int m_messageId;
	int m_controllerId;
	bool m_isRunning;
};

} /* elevator */

#endif /* ELEVATOR_SIGNALHANDLER_HPP */
