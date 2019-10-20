#include "CoreTimer.hpp"

namespace common {

CoreTimer::CoreTimer() {
	m_timerListener = nullptr;
	m_timerRunning = false;
}

CoreTimer::~CoreTimer() {
	cancelTimer();
}

void CoreTimer::startSingleTimer(TimerTask* timerListener, int duration, int timerId) {
	startTimerInternal(timerListener, duration, timerId, false);
}

void CoreTimer::startRepeatingTimer(TimerTask* timerListener, int duration, int timerId) {
	startTimerInternal(timerListener, duration, timerId, true);
}

void CoreTimer::startTimerInternal(TimerTask* timerListener, int duration, int timerId, bool repeating) {
	if (timerListener == nullptr) {
		return;
	}

	if (m_timerRunning) {
		return;
	}

	m_timerListener = timerListener;
	m_timerDuration = duration;
	m_timerId = timerId;
	m_timerRepeating = repeating;

	m_timerThread = std::thread(&common::CoreTimer::runTimerInternal, this);
	m_timerThread.detach();
}

void CoreTimer::cancelTimer() {
	if (!m_timerRunning) {
		return;
	}

	m_timerRunning = false;
	m_timerListener = nullptr;
}

bool CoreTimer::isTimerRunning() {
	return m_timerRunning;
}

void CoreTimer::runTimerInternal() {
	m_timerRunning = true;

	while (m_timerRunning) {
		sleepFor(m_timerDuration);

		if (m_timerRunning) {
			m_timerListener->coreTimerCallback(m_timerId);
		}

		if (!m_timerRepeating) {
			break;
		}
	}

	m_timerRunning = false;
}

void CoreTimer::sleepFor(int duration) {
	int elapsedTime = 0;

	while (elapsedTime <= duration) {
		std::this_thread::sleep_for(std::chrono::milliseconds(k_timerSleepDuration));
		elapsedTime += k_timerSleepDuration;

		if (!m_timerRunning) {
			break;
		}
	}
}

} /* common */
