#include "TestService.h"
#include <mutex>

MTask TestService::m_task;

TestService::TestService(char * serviceName, char * displayName)
	:ServiceBase(serviceName, displayName)
{
}

TestService::~TestService()
{
}

void TestService::OnStart(DWORD argc, TCHAR * argv[])
{
	std::thread service_thread(service_main_fun);
	service_thread.detach();
}

void TestService::OnStop()
{
	kill_service_thread();
}

void TestService::OnShutdown()
{
	kill_service_thread();
}

void TestService::kill_service_thread()
{
	m_task.Exit();
}

void TestService::service_main_fun()
{
	task_guard guard(m_task);

	while (true)
	{
		if (m_task.IsExit())
		{
			break;
		}
		if (m_task.IsPausing())
		{
			continue;
		}

		// do something here
		int i = 0;
	}
}
