// SPI_dll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <windows.h>
#include <commctrl.h>
#include "SPI_dll.h"

#include "KbdHandler.h"

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
//SetThreadPriority(hSPI_Handling, THREAD_PRIORITY_TIME_CRITICAL);
}

DWORD WINAPI ThreadSPIHandling(LPVOID lpParameter)
{
	int i = 0;
	while(1)
	{
		i++;
		Sleep(100);
		if(i%2 == 0)
		{
			SPI_Handling();
		}
		if(i%4 == 0)
		{
			//CmdThread.SetElement(KBD_DATA);
		}
		if(i%10 == 0)
		{
			CmdThread.SetElement(VOLTAGE_DATA);
		}
		if(i%11 == 0)
		{
			CmdThread.SetElement(AXEL_TEMP_DATA);	
		}
		if(i%12 == 0)
		{
			CmdThread.SetElement(ONE_WIRE_DATA);	
		}
		if(i%13 == 0)
		{
			CmdThread.SetElement(GPS_DATA);	
		}
	}
	return 0;
}


myTestKey TestKeyF = NULL;
extern int KeyPressed;

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

		case BP_ID_GET_DATA:
			{
				if(IO == 1)
				{
					DEBUGMSG(TRUE,( TEXT("SPI_DLL: BPEP_ID_GET_DATA \r\n") ));
					buff.AxelInfo = AxelInfo;
				}	
			}
		break;


		case F_KEY:
			{
				if(IO == 1)
				{
					DEBUGMSG(TRUE,( TEXT("SPI_DLL: KEY_GET_DATA \r\n") ));
					TestKeyF = buff.testKey;
					buff.testKey(999);
				}	
			}
		break;

		default: DEBUGMSG(TRUE,( TEXT("SPI_DLL: datatype %d - UNNOWN \r\n", datatype)));
		break;
		}
 }  