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

	// To allow for the program to run more efficiently, have the timer on its own separate thread
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

		// If the timer was cancelled, a callback should not be processed
		if (m_timerRunning) {
			m_timerListener->coreTimerCallback(m_timerId);
		}

		// If the timer is non-repeating, this allows the loop to only run once
		if (!m_timerRepeating) {
			break;
		}
	}

	m_timerRunning = false;
}

void CoreTimer::sleepFor(int duration) {
	int elapsedTime = 0;

	// Keeping track of the time passed, take microsleeps until rather the requested duration has been met
	// Or the timer gets cancelled
	while (elapsedTime <= duration) {
		std::this_thread::sleep_for(std::chrono::milliseconds(k_timerSleepDuration));
		elapsedTime += k_timerSleepDuration;

		if (!m_timerRunning) {
			break;
		}
	}
}

} /* common */
