#include "TestService.h"
#include <mutex>

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

void TestService::service_main_fun()
{
	while (true)
	{
		int i = 0;
	}
}
