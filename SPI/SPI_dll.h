#ifndef SPI_dll
#define SPI_dll

#ifdef SPI_dll_EXPORTS
#define SPI_dll_API __declspec(dllexport) 
#else
#define SPI_dll_API __declspec(dllimport)
#endif

#include "Communication_Defines.h"

#define VOLT_GET_DATA	1
#define GPS_GET_DATA	2
#define AXEL_GET_DATA	3
#define F_KEY			4
#define BP_ID_GET_DATA	5


#define S_IN			0
#define S_OUT			1


DWORD WINAPI ThreadKeypadReinit	(LPVOID lpParameter);
DWORD WINAPI ThreadSPIHandling	(LPVOID lpParameter);
DWORD WINAPI ThreadEncProc		(LPVOID lpParameter);

int DebugOutActive = 0; //if(DebugOutActive) if(DebugOutActive) printf

   
typedef int (*myTestKey) (int);

union buffIO
{
	OnBoardParams	volt;
	GpsInfoType		GpsInfo;
	AxelInfoType	AxelInfo;
	myTestKey		testKey; 
	//BPIdInfoType    BPIdInfo;
};

extern "C"
{
	void SPI_dll_API __stdcall SPIinit(void);
	void SPI_dll_API __stdcall SpiIo(int IO, int datatype, buffIO& buff);
}

#endif SPI_dll