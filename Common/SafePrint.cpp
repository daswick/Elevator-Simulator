#include "SafePrint.hpp"

#include <cstdio>
#include <ctime>

namespace common {

std::shared_ptr<SafePrint> SafePrint::m_pInstance = nullptr;

std::shared_ptr<SafePrint> SafePrint::getInstance() {
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

	while (m_isRunning) {
		while (m_messageQueue.size() > 0) {
			time_t now = time(NULL);
			tm* currentTime = localtime(&now);

			std::lock_guard<std::mutex> guard(m_queueMutex);

			printf("\t[%02d:%02d:%02d]: %s\n", currentTime->tm_hour, currentTime->tm_min, currentTime->tm_sec, m_messageQueue.front().c_str());
			m_messageQueue.pop();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(k_sleepDuration));
	}
}

} /* common */
