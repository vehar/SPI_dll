#ifndef DLLSPISERVER
#define DLLSPISERVER

#ifdef DLLSPISERVER_EXPORTS
#define DLLSPISERVER_API __declspec(dllexport) 
#else
#define DLLSPISERVER_API __declspec(dllimport) 
#endif

#include "SpiServer.h"

extern "C"
{
	void DLLSPISERVER_API __stdcall SPIinit(void);
	void DLLSPISERVER_API __stdcall SpiIo(int IO, int datatype, buffIO& buff);
}

#endif DLLSPISERVER