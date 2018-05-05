#ifndef __TestService_H__
#define __TestService_H__

#include "ServiceBase.h"
#include "MTask.h"

class TestService : public ServiceBase
{
public:
	TestService(char* serviceName, char* displayName);
	~TestService();

public:
	static void service_main_fun();
	void OnStart(DWORD argc, TCHAR* argv[]);
	void OnStop();
	void OnShutdown();

private:
	void kill_service_thread();

private:
	static MTask m_task;
};

#endif // !__TestService_H__
