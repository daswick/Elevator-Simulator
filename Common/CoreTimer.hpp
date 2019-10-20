#ifndef COMMON_CORETIMER_HPP
#define COMMON_CORETIMER_HPP

#include <thread>

namespace common {

class CoreTimer {
public:
	CoreTimer();
	~CoreTimer();

	class TimerTask {
	public:
		TimerTask() = default;
		~TimerTask() = default;

		virtual void coreTimerCallback(int id) = 0;
	};

	void startSingleTimer(TimerTask* timerListener, int duration, int timerId);
	void startRepeatingTimer(TimerTask* timerListener, int duration, int timerId);
	void cancelTimer();
	bool isTimerRunning();

private:
	std::thread m_timerThread;
	bool m_timerRunning;
	const int k_timerSleepDuration = 100;

	TimerTask* m_timerListener;
	int m_timerDuration;
	int m_timerId;
	bool m_timerRepeating;

	void startTimerInternal(TimerTask* timerListener, int duration, int timerId, bool repeating);
	void runTimerInternal();
	void sleepFor(int duration);
};

} /* common */

#endif /* COMMON_CORETIMER_HPP */
