#include <windows.h>
#include <WinIoCtl.h>
#include "SpiDriver.h"
#include "spi.h"
#include "omap35xx_mcspi.h"
#include "debug.h"

HANDLE SpiDriver::hDrv = 0;

SpiDriver::SpiDriver()
{
	DBG_SHOW_FUNC;

	hDrv = 0;
	if(Open() != NULL)
	{
		Configure();
		//printf("Spi open\n");
	}
}

SpiDriver::~SpiDriver()
{
	DBG_SHOW_FUNC;

 if(hDrv)
 {
	 CloseHandle(hDrv);
	 hDrv = 0;
	 //printf("Spi closed\n");
 }
}


HANDLE SpiDriver::Open()
{ 
  if(hDrv==NULL)
  {
	hDrv = CreateFile(L"SPI1:" ,GENERIC_READ | GENERIC_WRITE,
								0, NULL, OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL, NULL);	
  }
  	if ( hDrv == INVALID_HANDLE_VALUE )
	{
		printf("SPI Port Open Failed\n");
		DEBUGMSG(TRUE,( TEXT("!!! SPI Port Open Failed \r\n")));
	    DWORD err = GetLastError();
        printf("Error %d" , err);
		DEBUGMSG(TRUE,( TEXT("Error %u \r\n"), err));
		return NULL;
	} 
  return hDrv;
}

void SpiDriver::Configure(void)
{
	IOCTL_SPI_CONFIGURE_IN spiCfg;
	spiCfg.address	 = 0; // channel №
	spiCfg.config    =	       MCSPI_PHA_ODD_EDGES |			//data
							   MCSPI_POL_ACTIVEHIGH |			//clk
							   MCSPI_CHCONF_CLKD(0x4) |			//clk divider (0x8=256) //9 for slow STM
							   MCSPI_CSPOLARITY_ACTIVELOW |		//clk polarity
							   MCSPI_CHCONF_WL(8) |			    //word lenght (0xf=16)
							   MCSPI_CHCONF_TRM_TXRX |
							   MCSPI_CHCONF_DMAW_DISABLE |
							   MCSPI_CHCONF_DMAR_DISABLE |
							   MCSPI_CHCONF_DPE0;				//pin configuration mode
	 //Use 'SPI_CHCONF_CLKD(8)' for SPI clock freq 187KHz //?
	// Use 'SPI_CHCONF_CLKD(6)' for SPI clock freq 750KHz //?
	DeviceIoControl(hDrv,IOCTL_SPI_CONFIGURE,(BYTE *)&spiCfg, sizeof(IOCTL_SPI_CONFIGURE_IN), 0, 0, 0, 0);
}



DWORD SpiDriver::SPI_exchange(LPVOID lpInBuf, LPVOID lpOutBuf, int size)
{
	DWORD accepted = 0;
	if(hDrv)
	{
		DeviceIoControl(hDrv,IOCTL_SPI_WRITEREAD, lpInBuf,size, lpOutBuf,size, NULL,NULL );
		accepted = 1;//при присваивание DeviceIoControl - нет реакции от системы на вирт. клавиши!  
	}
	return accepted;
}