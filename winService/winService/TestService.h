#ifndef __TestService_H__
#define __TestService_H__

#include "ServiceBase.h"

class TestService : public ServiceBase
{
public:
	TestService(char* serviceName, char* displayName);
	~TestService();

public:
	static void service_main_fun();
	void OnStart(DWORD argc, TCHAR* argv[]);
};

#endif // !__TestService_H__
