#pragma once

//---------------------------------------------------------------------------
typedef HANDLE(_stdcall *FP_RtCreateSharedMemory)(DWORD flProtect, DWORD MaximumSizeHigh, DWORD MaximumSizeLow, LPCSTR lpName, VOID ** location);
typedef DWORD(_stdcall *FP_RtWaitForSingleObject)(HANDLE Handle, DWORD Milliseconds);
typedef DWORD(_stdcall *FP_RtWaitForMultipleObjects)(DWORD nCount, CONST HANDLE *lpHandles, BOOL fWaitAll, DWORD dwMilliseconds);

typedef HANDLE(_stdcall *FP_RtCreateEvent)(LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL ManualReset, BOOL pInitialState, LPCSTR lpName);
typedef HANDLE(_stdcall *FP_RtResetEvent)(HANDLE hEvent);
typedef BOOL(_stdcall *FP_RtSetEvent)(HANDLE hEvent);
typedef void(_stdcall *FP_RtSleepFt)(PLARGE_INTEGER pDuration);
typedef BOOL(_stdcall *FP_RtCloseHandle)(HANDLE hObject);

//---------------------------------------------------------------------------
extern FP_RtCreateSharedMemory RtCreateSharedMemory;
extern FP_RtWaitForSingleObject RtWaitForSingleObject;
extern FP_RtWaitForMultipleObjects RtWaitForMultipleObjects;
extern FP_RtCreateEvent RtCreateEvent;
extern FP_RtResetEvent RtResetEvent;
extern FP_RtSetEvent RtSetEvent;
extern FP_RtSleepFt RtSleepFt;
extern FP_RtCloseHandle RtCloseHandle;
//---------------------------------------------------------------------------
bool RTXInstalled(void);
bool RunRealTimeProcess();
BOOL LoadDLL();
void FreeDLL();
//---------------------------------------------------------------------------
