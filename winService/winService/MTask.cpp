#include "MTask.h"


MTask::MTask()
{
	m_status = em_MASK_NO_EXIT;
	m_taskCounter = 0;
}


MTask::~MTask()
{
}

bool MTask::IsExit()
{
	std::lock_guard<std::mutex> guard(m_statusMutex);
	return m_status == em_MASK_EXIT;
}

bool MTask::IsPausing()
{
	std::lock_guard<std::mutex> guard(m_statusMutex);
	return m_status == em_MASK_PAUSE;
}

void MTask::NoExit()
{
	std::lock_guard<std::mutex> guard(m_statusMutex);
	m_status = em_MASK_NO_EXIT;
}

void MTask::Exit()
{
	exit();
	std::unique_lock<std::mutex> guard(m_counterMutex);
	m_counterCondition.wait(guard, [this] {return isCounterZero();});
}

void MTask::exit()
{
	std::lock_guard<std::mutex> guard(m_statusMutex);
	m_status = em_MASK_EXIT;
}

bool MTask::isCounterZero()
{
	return (m_taskCounter <= 0);
}

void MTask::Pause()
{
	std::lock_guard<std::mutex> guard(m_statusMutex);
	m_status = em_MASK_PAUSE;
}

void MTask::EntryTask()
{
	std::lock_guard<std::mutex> guard(m_counterMutex);
	m_taskCounter++;
}

void MTask::LeaveTask()
{
	std::lock_guard<std::mutex> guard(m_counterMutex);
	m_taskCounter--;
	if (isCounterZero())
	{
		m_counterCondition.notify_one();
	}
}
