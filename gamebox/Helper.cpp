#include "stdafx.h"
#include "Helper.h"

//---------------------------------------------------------------------------
HINSTANCE DLL_Handle = NULL;
HINSTANCE Prop_DLL_Handle = NULL;
//---------------------------------------------------------------------------
FP_RtCreateSharedMemory RtCreateSharedMemory;
FP_RtWaitForSingleObject RtWaitForSingleObject;
FP_RtWaitForMultipleObjects RtWaitForMultipleObjects;
FP_RtCreateEvent RtCreateEvent;
FP_RtResetEvent RtResetEvent;
FP_RtSetEvent RtSetEvent;
FP_RtSleepFt RtSleepFt;
FP_RtCloseHandle RtCloseHandle;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//函数名：RTXInstalled
//功能：  检测目标系统中是否安装了RTX(Real-Time Extension), 是一个辅助函数 Helper
//输入参数：无
//返 回 值：True  － 共享内存创建成功
//          false － 共享内存创建失败
//---------------------------------------------------------------------------
bool RTXInstalled(void)
{
	HKEY RegKey;

	bool Result = false;

	if (RegOpenKey(HKEY_LOCAL_MACHINE, _T("C:\\Program Files\\IntervalZero\\RTX"), &RegKey) == ERROR_SUCCESS)
	{
		try
		{
			Result = (RegQueryValueEx(RegKey, _T("ProductVersion"), NULL, NULL, NULL, NULL) == ERROR_SUCCESS);
		}
		catch (...)
		{
			RegCloseKey(RegKey);
		}
	}

	return Result;
}
//---------------------------------------------------------------------------
bool RunRealTimeProcess()
{
	//STARTUPINFO sinfo;
	//PROCESS_INFORMATION pinfo;

	//LPWSTR command_line = _T("RTSSrun RealTime.rtss");

	//bool Result = true;
	//
	//ZeroMemory(&sinfo, sizeof(sinfo));
	//sinfo.cb = sizeof(sinfo);

	//if (!CreateProcess(NULL,           // application name -- not needed
	//	command_line,					// dos command line
	//	NULL,							// process attributes
	//	NULL,							// thread attributes
	//	false,							// handle inheritance flag
	//	DETACHED_PROCESS,				// creation flags
	//	NULL,							// pointer to new env. block
	//	NULL,							// pointer to curr directory name
	//	&sinfo,							// startup info
	//	&pinfo))						// process info
	//{
	//	Result = false;
	//}

	//return Result;
	STARTUPINFOA sinfo;
	PROCESS_INFORMATION pinfo;

	LPSTR command_line = ("RTSSrun RealTime.rtss");

	bool Result = true;

	ZeroMemory(&sinfo, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);

	if (!CreateProcessA(NULL,           // application name -- not needed
		command_line,					// dos command line
		NULL,							// process attributes
		NULL,							// thread attributes
		false,							// handle inheritance flag
		DETACHED_PROCESS,				// creation flags
		NULL,							// pointer to new env. block
		NULL,							// pointer to curr directory name
		&sinfo,							// startup info
		&pinfo))						// process info
	{
		Result = false;
	}

	return Result;
}
//---------------------------------------------------------------------------
BOOL LoadDLL()
{
	BOOL bVal = TRUE;

	DLL_Handle = LoadLibrary(_T("rtapi_w32.dll"));

	if (DLL_Handle != NULL)
	{
		RtCreateSharedMemory = (FP_RtCreateSharedMemory)GetProcAddress(DLL_Handle, "RtCreateSharedMemoryA");
		RtWaitForSingleObject = (FP_RtWaitForSingleObject)GetProcAddress(DLL_Handle, "RtWaitForSingleObject");
		RtWaitForMultipleObjects = (FP_RtWaitForMultipleObjects)GetProcAddress(DLL_Handle, "RtWaitForMultipleObjects");
		RtCreateEvent = (FP_RtCreateEvent)GetProcAddress(DLL_Handle, "RtCreateEventA");
		RtSetEvent = (FP_RtSetEvent)GetProcAddress(DLL_Handle, "RtSetEvent");
		RtResetEvent = (FP_RtResetEvent)GetProcAddress(DLL_Handle, "RtResetEvent");
		RtSleepFt = (FP_RtSleepFt)GetProcAddress(DLL_Handle, "RtSleepFt");
		RtCloseHandle = (FP_RtCloseHandle)GetProcAddress(DLL_Handle, "RtCloseHandle");

		if ((RtCreateSharedMemory == NULL) || (RtWaitForSingleObject == NULL) || (RtWaitForMultipleObjects == NULL) || (RtCreateEvent == NULL) ||
			(RtSetEvent == NULL) || (RtResetEvent == NULL) || (RtSleepFt == NULL) || (RtCloseHandle == NULL))
		{
			bVal = FALSE;
		}
	}


	return bVal;
}
//---------------------------------------------------------------------------
void FreeDLL()
{
	if (DLL_Handle != NULL)
	{
		FreeLibrary(DLL_Handle);
	}
}
//---------------------------------------------------------------------------