// SPI_app.cpp : Defines the entry point for the console application.
//
#include "KbdHandler.h"

DWORD WINAPI ThreadKeypadReinit(LPVOID lpParameter);
DWORD WINAPI ThreadSPIHandling(LPVOID lpParameter);
DWORD WINAPI ThreadEncProc(LPVOID lpParameter);
DWORD WINAPI ThreadGPSHandling(LPVOID lpParameter);
DWORD WINAPI Thread1WProc(LPVOID lpParameter);
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

HANDLE g_hTimer;
 
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent,DWORD dwTime)
{
    printf("timer expired\n");
}
 

VOID CALLBACK VoltGet(HWND hwnd, UINT uMsg, UINT_PTR idEvent,DWORD dwTime)
{
printf("1\n");
    //CmdThread.SetElement(raw_volt_DATA, 0);
}

VOID CALLBACK CurrtGet(HWND hwnd, UINT uMsg, UINT_PTR idEvent,DWORD dwTime)
{
printf("2\n");
    //CmdThread.SetElement(raw_curr_DATA, 0);
}


int _tmain(int argc, _TCHAR* argv[])
{
MSG Msg;

//SetTimer(NULL,0,1000,VoltGet);
//SetTimer(NULL,1, 500,CurrtGet);

DWORD dwThreadId;
HANDLE hSPI_Handling = CreateThread(NULL, 0, ThreadSPIHandling, (LPVOID)0, 0, &dwThreadId); 
SetThreadPriority(hSPI_Handling, THREAD_PRIORITY_TIME_CRITICAL);

HANDLE hKbdTread = CreateThread(NULL, 0, ThreadKeybProc, (LPVOID)2, 0, &dwThreadId);  
SetThreadPriority(hKbdTread, THREAD_PRIORITY_TIME_CRITICAL);

//HANDLE hGPSTread = CreateThread(NULL, 0, ThreadGPSHandling, (LPVOID)0, 0, &dwThreadId); 
//SetThreadPriority(hGPSTread, THREAD_PRIORITY_TIME_CRITICAL);

//HANDLE hVoltTread = CreateThread(NULL, 0, ThreadVoltProc, (LPVOID)2, 0, &dwThreadId); 
HANDLE hVoltTread = CreateThread(NULL, 0, Thread1WProc, (LPVOID)2, 0, &dwThreadId); 
SetThreadPriority(hVoltTread, THREAD_PRIORITY_TIME_CRITICAL);
//HANDLE hCurrTread = CreateThread(NULL, 0, ThreadCurrProc, (LPVOID)2, 0, &dwThreadId); 
//HANDLE hEncTread = CreateThread(NULL, 0, ThreadEncProc, (LPVOID)2, 0, &dwThreadId); 

	while(1)
	{
	Sleep(1000); 
	    /* Process all messages */
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
	}  

return 0;
} 

DWORD WINAPI ThreadSPIHandling(LPVOID lpParameter)
{
	while(1)
	{
	SPI_Handling();
	Sleep(25); 
	}
	return 0;
}
