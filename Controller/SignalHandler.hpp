#ifndef CONTROLLER_SIGNALHANDLER_HPP
#define CONTROLLER_SIGNALHANDLER_HPP

#include "../Common/EventHandler.hpp"
#include "../Common/signal/SignalReceiver.hpp"

#include <vector>

namespace controller {

class SignalHandler : public common::SignalReceiver::ISignalListener {
public:
	SignalHandler() = delete;
	SignalHandler(std::shared_ptr<common::EventHandler> pEventHandler, int numElevators);
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

	void sendSignal(SignalTypes type, int data, int elevator);

	void startReceivers();
	void stopReceivers();

	void handleSignal(common::Signal msg) override;

private:
	std::unique_ptr<common::SignalReceiver> m_pSignalReceiver;
	std::shared_ptr<common::EventHandler> m_pEventHandler;

	int m_messageId;
	int m_elevatorCount;
	std::vector<int> m_elevatorIdList;
	bool m_isRunning;
};

} /* controller */

#endif /* CONTROLLER_SIGNALHANDLER_HPP */
