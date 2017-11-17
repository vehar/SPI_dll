// SPI_app.cpp : Defines the entry point for the console application.
//
#include "Task.h"
#include "KbdHandler.h"
#include "Communication_Defines.h"

DWORD WINAPI ThreadSPIHandling(LPVOID lpParameter);

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

			DEBUGMSG(TRUE,( TEXT("\r\n\r\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n")));

//SetTimer(NULL,0,1000,VoltGet);
//SetTimer(NULL,1, 500,CurrtGet);

DWORD dwThreadId;
HANDLE hSPI_Handling = CreateThread(NULL, 0, ThreadSPIHandling, (LPVOID)0, 0, &dwThreadId); 
SetThreadPriority(hSPI_Handling, THREAD_PRIORITY_TIME_CRITICAL);


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
	int i = 0;
	while(1)
	{
		i++;
		Sleep(10);
		if(i%1 == 0)
		{
			SPI_Handling();
		}
		if(i%4 == 0)
		{
			CmdThread.SetElement(KBD_DATA);
		}
		if(i%50 == 0)
		{
			CmdThread.SetElement(VOLTAGE_DATA);
		}
		if(i%200 == 0)
		{
			CmdThread.SetElement(AXEL_TEMP_DATA);	
		}
		if(i%100 == 0)
		{
			CmdThread.SetElement(ONE_WIRE_DATA);	
		}
		if(i%1000 == 0)
		{
			CmdThread.SetElement(GPS_DATA);	
		}
	}
	return 0;
}





