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

		virtual void coreTimerCallback() = 0;
	};

	void startSingleTimer(TimerTask* timerListener, int duration);
	void startRepeatingTimer(TimerTask* timerListener, int duration);
	void cancelTimer();
	bool isTimerRunning();

private:
	std::thread m_timerThread;
	bool m_timerRunning;
	const int k_timerSleepDuration = 100;

	TimerTask* m_timerListener;
	int m_timerDuration;
	bool m_timerRepeating;

	void runTimerInternal();
	void sleepFor(int duration);
};

} /* common */

#endif
