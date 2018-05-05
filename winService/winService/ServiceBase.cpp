#include <assert.h>
#include "ServiceBase.h"

ServiceBase* ServiceBase::m_service = NULL;

ServiceBase::ServiceBase(char * serviceName, char * displayName)
{
	strcpy_s(m_serviceName, serviceName);
	strcpy_s(m_displayName, displayName);

	m_serviceStatusHandle = NULL;
	m_serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	m_serviceStatus.dwCurrentState = SERVICE_STOPPED;
	m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	m_serviceStatus.dwWin32ExitCode = 0;
	m_serviceStatus.dwServiceSpecificExitCode = 0;
	m_serviceStatus.dwCheckPoint = 0;
	m_serviceStatus.dwWaitHint = 0;
}


ServiceBase::~ServiceBase()
{
}

BOOL ServiceBase::RegisterService()
{
	if (isInstalled())
		return TRUE;

	// open service control manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, TEXT("Couldn't open service manager"), TEXT(m_serviceName), MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	// create service
	SC_HANDLE hService = ::CreateService(
		hSCM, TEXT(m_serviceName), TEXT(m_displayName),
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, TEXT(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, TEXT("Couldn't create service"), TEXT(m_serviceName), MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}

BOOL ServiceBase::RemoveService()
{
	if (!isInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, TEXT("Couldn't open service manager"), TEXT(m_serviceName), MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, TEXT(m_serviceName), SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, TEXT("Couldn't open service"), TEXT(m_serviceName), MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	// delete service
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	logEvent(TEXT("Service could not be deleted"));

	return FALSE;
}

BOOL ServiceBase::RunService()
{
	return runService(this);
}

BOOL ServiceBase::runService(ServiceBase* svc)
{
	if (svc == NULL)
	{
		return FALSE;
	}

	m_service = svc;
	char* svcName = m_service->GetServiceName();

	SERVICE_TABLE_ENTRY serviceTable[] =
	{
		{ TEXT(svcName),(LPSERVICE_MAIN_FUNCTION)service_main },
		{ NULL,NULL }
	};

	if (!StartServiceCtrlDispatcher(serviceTable))
	{
		DWORD nError = GetLastError();
		printf("Start service control dispatcher failed. %d\n", nError);
		return FALSE;
	}

	return TRUE;
}

BOOL ServiceBase::isInstalled()
{
	BOOL bResult = FALSE;

	// open service control manager
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		// open service
		SC_HANDLE hService = ::OpenService(hSCM, TEXT(m_serviceName), SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}

void WINAPI ServiceBase::service_main(DWORD dwArgc, LPTSTR * lpszArgv)
{
	assert(m_service);
	m_service->m_serviceStatusHandle = ::RegisterServiceCtrlHandler(TEXT(m_service->GetServiceName()), serviceCtrlHandler);
	if (!(m_service->m_serviceStatusHandle))
	{
		DWORD nError = GetLastError();
		printf_s("handler register failed. %d\n", nError);
		return;
	}

	m_service->start(dwArgc, lpszArgv);
}

void WINAPI ServiceBase::serviceCtrlHandler(DWORD opcode)
{
	switch (opcode)
	{
	case SERVICE_CONTROL_STOP:
		m_service->stop();
		break;
	case SERVICE_CONTROL_PAUSE:
		m_service->pause();
		break;
	case SERVICE_CONTROL_CONTINUE:
		m_service->resume();
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		m_service->shutdown();
		break;
	default:
		break;
	}

	return;
}

void ServiceBase::logEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	sprintf_s(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, TEXT(m_serviceName));
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*)&lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

void ServiceBase::setServiceStatus(DWORD dwState, DWORD dwErrCode, DWORD dwWait)
{
	m_serviceStatus.dwCurrentState = dwState;
	m_serviceStatus.dwWin32ExitCode = dwErrCode;
	m_serviceStatus.dwWaitHint = dwWait;

	if (!SetServiceStatus(m_serviceStatusHandle, &m_serviceStatus))
	{
		DWORD nError = GetLastError();
		printf_s("service status change fail. %d\n", nError);
	}
}

void ServiceBase::start(DWORD argc, TCHAR* argv[])
{
	logEvent("service start pending.");

	setServiceStatus(SERVICE_START_PENDING);
	OnStart(argc, argv);
	setServiceStatus(SERVICE_RUNNING);

	logEvent("service start.");
}

void ServiceBase::stop()
{
	logEvent("service stop pending.");

	setServiceStatus(SERVICE_STOP_PENDING);
	OnStop();
	setServiceStatus(SERVICE_STOPPED);

	logEvent("service stop.");
}

void ServiceBase::pause()
{
	logEvent("service pause pending.");

	setServiceStatus(SERVICE_PAUSE_PENDING);
	OnPause();
	setServiceStatus(SERVICE_PAUSED);

	logEvent("service pause.");
}

void ServiceBase::resume()
{
	logEvent("service resume pending.");

	setServiceStatus(SERVICE_CONTINUE_PENDING);
	OnContinue();
	setServiceStatus(SERVICE_RUNNING);

	logEvent("service resume.");
}

void ServiceBase::shutdown()
{
	logEvent("service shutdown pending.");

	OnShutdown();
	setServiceStatus(SERVICE_STOPPED);

	logEvent("service shutdown.");
}
