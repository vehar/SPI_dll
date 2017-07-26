#ifndef SPIDRIVER_H
#define SPIDRIVER_H

#pragma once

#include <windows.h>
//#include <cstdio>

#define DBG_SHOW_FUNC DEBUGMSG(TRUE,( TEXT("SPI_DLL: in file %s funk %s at line %d \r\n"), TEXT(__FILE__) , TEXT(__FUNCTION__) , __LINE__ ))
#define DBG_SHOW_DATE DEBUGMSG(TRUE,( TEXT("SPI_DLL: builded: %s at: %s \r\n"), TEXT(__DATE__), TEXT(__TIME__) ))


class SpiDriver
{
public:
	SpiDriver();
	~SpiDriver();

private:
	static HANDLE hDrv;
	static HANDLE Open();
	static void Configure();

public:
	static DWORD SPI_exchange(LPVOID lpInBuf, LPVOID lpOutBuf, int size);
	static DWORD SPI_exchange();
	static HANDLE GetHDrv() {return hDrv;};
};

#endif SPIDRIVER_H