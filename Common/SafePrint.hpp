#ifndef COMMON_SAFEPRINT_HPP
#define COMMON_SAFEPRINT_HPP

#include <thread>
#include <mutex>
#include <queue>
#include <memory>

namespace common {

class SafePrint {
public:
	SafePrint();
	~SafePrint();

	void printMessage(std::string message);

	static std::shared_ptr<SafePrint> getInstance();

private:
	const int k_sleepDuration = 100;
	bool m_isRunning;

	std::queue<std::string> m_messageQueue;
	std::mutex m_queueMutex;
	std::thread m_queueThread;

	static std::shared_ptr<common::SafePrint> m_pInstance;

	void printMessages();

	void start();
};

} /* common */

#endif
