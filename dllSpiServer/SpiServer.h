#ifndef SPISERVER_H
#define SPISERVER_H

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <memory.h> 
//#include <string.h>
#include "KbdHandler.h"
#include <winbase.h>
#include <WinIoCtl.h>
#include "spi.h"

#include "DevisePerifHandler.h"

#define VOLT_GET_DATA 1
#define GPS_GET_DATA 2
#define AXEL_GET_DATA 3

#define    F_KEY      4
//int testKey(int);

#define S_IN 0
#define S_OUT 1


DWORD WINAPI ThreadKeypadReinit	(LPVOID lpParameter);
DWORD WINAPI ThreadSPIHandling	(LPVOID lpParameter);
DWORD WINAPI ThreadEncProc		(LPVOID lpParameter);

int DebugOutActive = 0; //if(DebugOutActive) if(DebugOutActive) printf

   
typedef int (*myTestKey) (int);

union buffIO
{
	OnBoardParams volt;
	GpsInfoType GpsInfo;
	AxelInfoType AxelInfo;
	myTestKey    testKey; 
};
#endif SPISERVER_H