#ifndef SPI_dll
#define SPI_dll

#ifdef SPI_dll_EXPORTS
#define SPI_dll_API __declspec(dllexport) 
#else
#define SPI_dll_API __declspec(dllimport 
#endif

#include "SpiServer.h"

extern "C"
{
	void SPI_dll_API __stdcall SPIinit(void);
	void SPI_dll_API __stdcall SpiIo(int IO, int datatype, buffIO& buff);
}

#endif SPI_dll