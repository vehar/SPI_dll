#ifndef SPIDRIVER_H
#define SPIDRIVER_H

#pragma once

#include <windows.h>

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