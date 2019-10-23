#include "SafePrint.hpp"

#include <cstdio>
#include <ctime>

namespace common {

std::shared_ptr<SafePrint> SafePrint::m_pInstance = nullptr;

std::shared_ptr<SafePrint> SafePrint::getInstance() {
	// To ensure thread-safety, SafePrint is a singleton
	// If the singleton does not exist, create it. Otherwise, return the existing one
	if (m_pInstance == nullptr) {
		m_pInstance = std::make_shared<SafePrint>();
		m_pInstance->start();
	}

	return m_pInstance;
}

SafePrint::SafePrint() {
	m_isRunning = false;
}

SafePrint::~SafePrint() {
	m_isRunning = false;
}

void SafePrint::start() {
	// To ensure a thread does not handle printing on top of its current work, create a thread just for printing
	m_queueThread = std::thread(&common::SafePrint::printMessages, this);
	m_queueThread.detach();

	printMessage("SafePrint started successfully");
}

void SafePrint::printMessage(std::string message) {
	std::lock_guard<std::mutex> guard(m_queueMutex);
	m_messageQueue.push(message);
}

void SafePrint::printMessages() {
	m_isRunning = true;

	// While the printer is running (which can only stop from the destructor)
	while (m_isRunning) {
		// If there are any messages in the queue, process them
		while (m_messageQueue.size() > 0) {
			// Get the current time to populate in the print
			time_t now = time(NULL);
			tm* currentTime = localtime(&now);

			std::lock_guard<std::mutex> guard(m_queueMutex);

			// Print both the current time and the message, then remove the message
			printf("\t[%02d:%02d:%02d]: %s\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, m_messageQueue.front().c_str());
			m_messageQueue.pop();
		}

		// Take a microsleep to wait for more messages
		std::this_thread::sleep_for(std::chrono::milliseconds(k_sleepDuration));
	}
}

} /* common */
