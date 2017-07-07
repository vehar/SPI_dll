// dllSpiServer.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include "dllSpiServer.h"
#include "SpiServer.h"

#include "DevisePerifHandler.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved 
					 )
{
	if(DebugOutActive) printf("In DLL main\n");

	switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
			DisableThreadLibraryCalls((HINSTANCE)hModule);  
			
			DEBUGMSG(TRUE,( TEXT("\r\n\r\n++++++++++++++++++++++++++++++++++++++++++++++++++++++\r\n")));
			DBG_SHOW_DATE;
			DBG_SHOW_FUNC;

			if(DebugOutActive) printf("STM_dll - DLL_PROCESS_ATTACH\n"); 
			if(DebugOutActive) printf("DllMain_build_date: %s at %s\n", __DATE__, __TIME__);

			//SPIinit(); 
			break;
        case DLL_THREAD_ATTACH: 
			if(DebugOutActive) printf("STM_dll - DLL_THREAD_ATTACH\n");
			break;
        case DLL_THREAD_DETACH:
			if(DebugOutActive) printf("STM_dll - DLL_THREAD_DETACH\n");
			break;
        case DLL_PROCESS_DETACH:
			RETAILMSG(TRUE, (TEXT("STM_dll - DLL_PROCESS_DETACH \r\n")));
			if(DebugOutActive) printf("STM_dll - DLL_PROCESS_DETACH\n");
            break;
    }
    return TRUE;
}

void SPIinit() 
{	
	if(DebugOutActive) printf("%s\n", __FUNCTION__ );
	DBG_SHOW_FUNC;

DWORD dwThreadId;

HANDLE hSPI_Handling = CreateThread(NULL, 0, ThreadSPIHandling, (LPVOID)0, 0, &dwThreadId); 
SetThreadPriority(hSPI_Handling, THREAD_PRIORITY_TIME_CRITICAL);

HANDLE hKbdTread = CreateThread(NULL, 0, ThreadKeybProc, (LPVOID)2, 0, &dwThreadId);  
SetThreadPriority(hKbdTread, THREAD_PRIORITY_TIME_CRITICAL);

HANDLE hGPSTread = CreateThread(NULL, 0, ThreadGPSHandling, (LPVOID)0, 0, &dwThreadId); 
SetThreadPriority(hGPSTread, THREAD_PRIORITY_TIME_CRITICAL);

HANDLE hVoltTread = CreateThread(NULL, 0, ThreadVoltProc, (LPVOID)2, 0, &dwThreadId); 
SetThreadPriority(hVoltTread, THREAD_PRIORITY_TIME_CRITICAL);

HANDLE hAxelTempTread = CreateThread(NULL, 0, ThreadAxelTempProc, (LPVOID)2, 0, &dwThreadId); 
SetThreadPriority(hAxelTempTread, THREAD_PRIORITY_TIME_CRITICAL);
}

DWORD WINAPI ThreadSPIHandling(LPVOID lpParameter)
{
	if(DebugOutActive) printf("%s\n", __FUNCTION__ );
	while(1)
	{
		SPI_Handling();
		Sleep(200); 
	}
	return 0;
}

 void __stdcall SpiIo(int IO, int datatype, buffIO& buff) 
 {
DBG_SHOW_FUNC;

		switch (datatype)
		{
		case VOLT_GET_DATA: 
			{
				if(IO == 1)
				{
					DEBUGMSG(TRUE,( TEXT("SPI_DLL: VOLT_GET_DATA \r\n") ));
					buff.volt = boardParams;//отправл€ем готовые данные наверх приложению
				}	
			}
		break;

		case GPS_GET_DATA:
			{
				if(IO == 1)
				{
					DEBUGMSG(TRUE,( TEXT("SPI_DLL: GPS_GET_DATA \r\n") ));
					buff.GpsInfo = GpsInfo;
				}	
			}
		break;

		case AXEL_GET_DATA:
			{
				if(IO == 1)
				{
					DEBUGMSG(TRUE,( TEXT("SPI_DLL: AXEL_GET_DATA \r\n") ));
					buff.AxelInfo = AxelInfo;
				}	
			}
		break;

		default: DEBUGMSG(TRUE,( TEXT("SPI_DLL: datatype %d - UNNOWN \r\n", datatype)));
		break;
		}
 }  