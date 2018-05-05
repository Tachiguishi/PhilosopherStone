#ifndef __MTask_H__
#define __MTask_H__

#include <mutex>
#include <condition_variable>

class MTask
{
public:
	MTask();
	~MTask();

	bool IsExit();
	bool IsPausing();

	void NoExit();
	void Exit();
	void Pause();

	void EntryTask();
	void LeaveTask();

private:
	void exit();
	bool isCounterZero();

private:
	unsigned short m_status;
	int m_taskCounter;
	std::mutex m_statusMutex;
	std::mutex m_counterMutex;
	std::condition_variable m_counterCondition;

	enum MASK_STATUS {
		em_MASK_NO_EXIT = 0,
		em_MASK_EXIT,
		em_MASK_PAUSE,
	};
};

#endif // !__MTask_H__
