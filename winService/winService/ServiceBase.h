#ifndef __ServiceBase_H__
#define __ServiceBase_H__

#include <stdio.h>
#include <Windows.h>

namespace RorService {

enum ARRAY_LENGTH {
	SERVICE_NAME_LENGTH = 32,
};

}

class ServiceBase
{
public:
	~ServiceBase();

	BOOL RegisterService();
	BOOL RemoveService();
	BOOL StartRegisterService();

	BOOL RunService();

	char* GetServiceName() { return m_serviceName; };
	char* GetDisplayName() { return m_displayName; };

	virtual void OnStart(DWORD argc, TCHAR* argv[]) = 0;
	virtual void OnStop() {}
	virtual void OnPause() {}
	virtual void OnContinue() {}
	virtual void OnShutdown() {}

protected:
	ServiceBase(char * serviceName, char * displayName);

	BOOL runService(ServiceBase* svc);
	BOOL isInstalled();
	void logEvent(LPCTSTR pFormat, ...);
	void setServiceStatus(DWORD dwState, DWORD dwErrCode = NO_ERROR, DWORD dwWait = 0);

	void start(DWORD argc, TCHAR* argv[]);
	void stop();
	void pause();
	void resume();
	void shutdown();

private:
	static void WINAPI service_main(DWORD dwArgc, LPTSTR *lpszArgv);
	static void WINAPI serviceCtrlHandler(DWORD opcode);

private:
	char m_serviceName[RorService::SERVICE_NAME_LENGTH];
	char m_displayName[RorService::SERVICE_NAME_LENGTH];
	SERVICE_STATUS m_serviceStatus;
	SERVICE_STATUS_HANDLE m_serviceStatusHandle;

	static ServiceBase* m_service;
};

#endif // !__ServiceBase_H__
