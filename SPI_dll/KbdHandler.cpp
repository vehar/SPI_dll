
#include "KbdHandler.h"
#include "DevisePerifHandler.h"

//вы€снить неработоспособность dll в св€зке с интерфейсом

#ifdef WINCE

//SpiDriver Spi;			//create spi-object
Communication Comm;		//Communication protocol vs STM
QueueTask CmdThread;	// Objects, that control slave threads

#undef DEBUG 
 

extern int DebugOutActive;

int DataRefreshed_Flag = 0;

int KeyStateFlag = 0;
int KeyCode = 0;
int KeyPressCnt = 0;

int EncState = 0;
int EncCNT = 0;


int Key_translate(int KeyCode);

void On_key_Pavlov(void);
void On_mouse (unsigned char* DataBuf);//n.u.
void On_akkum_volt (unsigned char* DataBuf);
void On_GPS (unsigned char* DataBuf);
void On_Axel_Temp (unsigned char* DataBuf);
void On_temperature (unsigned char* DataBuf);
void On_stm_temperature (unsigned char* DataBuf);
//------------------------------------


void KeypadInit(void)
{
	//Comm.CmdPack(KBD_DATA);
	//Comm.DataExchange();//Spi.SPI_exchange(Comm.Rx_buf,Comm.Tx_buf,SPI_BUFF_SIZE);
}


void Parse(int Packed_CMD, unsigned char* DataBuf)
{

	_TCHAR  szData[256];

		switch (Packed_CMD)
		{

		case VOLTAGE_DATA:		On_akkum_volt(DataBuf);		break;
		case KBD_DATA:			On_key_Pavlov();			break;
		case GPS_DATA:			On_GPS(DataBuf);			break;
		case AXEL_TEMP_DATA:	On_Axel_Temp(DataBuf);		break;

		case RAW_DATA:		break;

		default:// All other data redirect to upper (application) level
//			memcpy(szData, Comm.Rx_buf+COMAND_POS, Comm.Packed_SIZE/*STRLEN(DataBuf+1)*2*/);// put data in DataBuf
//			TransferData(WM_DISPLAY_TEXT,szData,STRLEN(szData)*2);
//			printf("Invalid Packed_CMD %d\n", Packed_CMD);  
			break;
		}
}

int prevKey = 0xFF;
int encPrev = 0;
int encCurr= 0;

int KeyPressed = 0;
typedef int (*myTestKey) (int);
extern myTestKey TestKeyF;

void On_key_Pavlov(void)
{
	for(int i = 0; i<=9; ++i) //7 keys + 2 emulation encoder keys Up/Dn
	{
		if(Comm.DataBuf[i] == BTN_CLICK)// short
		{
			if(i != prevKey)
			{
				prevKey = i;
				SendKbdMsg(true, Key_translate(i));

				KeyPressed = i;
			}
		}
		else if(Comm.DataBuf[i] == BTN_RELEASE)
		{
			if(prevKey != 0xFF) //was pressed previously
			{
				Sleep(5);
				SendKbdMsg(false, Key_translate(prevKey));
				prevKey = 0xFF;
			}
		}

		if(Comm.DataBuf[i] == BTN_PRESS) //long
		{
			{
				prevKey = 0xFF;
				SendKbdMsg(true, Key_translate(i));
			}
		}
	}

	// Absolute position
	encCurr = (Comm.DataBuf[10] << 8);//low byte
	encCurr |= Comm.DataBuf[11];//low byte
	if(encPrev != encCurr)
	{
		DEBUGMSG(TRUE, (TEXT("SPI_DLL: encPrev = %u encCurr = %u \r\n"),  encPrev, encCurr));
		
		//if(encPrev > encCurr) {SendKbdMsg(true, Key_translate(KEY_EncDn));}
		//if(encPrev < encCurr) {SendKbdMsg(true, Key_translate(KEY_EncUp));}
		encPrev = encCurr;
	}
}

int Key_translate(int KeyCode)//translate stm key-codes into virtual Win key-codes
{
int vk = 0; 

	switch (KeyCode)
	{   //--------------Matches-----------------------------
		//----Device btn------>Virtual key to system--------
		case KEY_F1:		vk = VK_F1; break;//VK_CONTROL; break; //VK_F1
		case KEY_F2:		vk = VK_F2; break; //VK_F2
		case KEY_F3:		vk = VK_F3; break;//VK_F3
		case KEY_F4:		vk = VK_F4; break;
		case KEY_F5:		vk = VK_F5; break;//VK_ESCAPE; break;
		case KEY_Joint:		vk = VK_F6; break;	//Enter
		case KEY_EncSw:		vk = VK_F12; break;//VK_ESCAPE //VK_RETURN
		case KEY_EncUp:		vk = VK_UP; break;	
		case KEY_EncDn:		vk = VK_DOWN; break;	

		default: vk = 0; break;
	}
return vk;
}


//------------------------------------------------------------------------------


void SPI_Handling(void)//Send cmd from gueue & recieve data
{
CmdThread.ReceiveElement();//get cmd from gueue

Comm.CmdPack(CmdThread.GetCmd());
Comm.DataExchange();//Spi.SPI_exchange(Comm.Rx_buf,Comm.Tx_buf,SPI_BUFF_SIZE);

//Check packet
	if((Comm.PackedCorrect(Comm.Rx_buf)) != 1)//if packed incorrect - exit
		return;

//Cpy & parse it!
 memcpy(Comm.DataBuf, Comm.Rx_buf+DATA_1_POS, Comm.Packed_SIZE);// put data in DataBuf	
 Parse(Comm.Packed_CMD, Comm.DataBuf);
}



//pervii parametr eto 0-down, 1 - up ... vtoroi et konstanta knopki takie kak :VK_F1,VK_HOME ...
void SendKbdMsg(bool pressed, int vk)
{
	if(vk)
	{
	DEBUGMSG(TRUE, (TEXT("SPI_DLL: SendKbdMsg vk = %u state = %u \r\n"),  vk, pressed));
	//TestKeyF(vk);
	}

	KEYBDINPUT kInp; 	//char down;
	INPUT x1;

	kInp.wVk=vk;
	kInp.dwFlags = ((pressed == true) ? 0 : KEYEVENTF_KEYUP);
	x1.type=INPUT_KEYBOARD;
	x1.ki=kInp;
	SendInput(1,&x1,sizeof(INPUT));
	
}


DWORD WINAPI ThreadKeybProc(LPVOID lpParameter)
{
	while(1)
	{
		CmdThread.SetElement(KBD_DATA);
		Sleep(200);
	}
	return 0;
}


DWORD WINAPI ThreadGPSHandling(LPVOID lpParameter)
{
	while(1)
	{
		CmdThread.SetElement(GPS_DATA);		
		Sleep(1000);
	}
	return 0;
}


DWORD WINAPI ThreadVoltProc(LPVOID lpParameter)
{
	while(1)
	{
		CmdThread.SetElement(VOLTAGE_DATA, 0);
		Sleep(700);
	}
	return 0;
}

DWORD WINAPI ThreadAxelTempProc(LPVOID lpParameter)
{
	while(1)
	{
		CmdThread.SetElement(AXEL_TEMP_DATA, 0);
		Sleep(1000);
	}
	return 0;
}


//currently not used
/*
void On_mouse (unsigned char* DataBuf)
{
 SendMouseMsg(DataBuf[0],
						  DataBuf[1],
					      DataBuf[2],
						  DataBuf[3]); 
			 memset(DataBuf,0,CMD_BUFF_SIZE);//clear buff
}


int SendMouseMsg(DWORD flag, int data, int dx, int dy)
{
	MOUSEINPUT  mInp; 
	INPUT xIn;
	int err = 1;

	switch (flag)
	{
	case MOUSEEVENTF_ABSOLUTE:
	mInp.dwFlags = MOUSEEVENTF_ABSOLUTE;
	mInp.dx = dx;
	mInp.dy = dy;
		break;

	case MOUSEEVENTF_LEFTDOWN:
	mInp.dwFlags = MOUSEEVENTF_LEFTDOWN;
		break;
	case MOUSEEVENTF_LEFTUP:
	mInp.dwFlags = MOUSEEVENTF_LEFTUP;
		break;
	case MOUSEEVENTF_RIGHTDOWN:
	mInp.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		break;
	case MOUSEEVENTF_RIGHTUP:
	mInp.dwFlags = MOUSEEVENTF_RIGHTUP;
		break;
	case MOUSEEVENTF_MIDDLEDOWN:
	mInp.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
		break;
	case MOUSEEVENTF_MIDDLEUP:
	mInp.dwFlags = MOUSEEVENTF_MIDDLEUP;
		break;

	case MOUSEEVENTF_WHEEL:
	mInp.dwFlags = MOUSEEVENTF_WHEEL;
	mInp.mouseData = data;
		break;
	case MOUSEEVENTF_MOVE:
	mInp.dwFlags = MOUSEEVENTF_MOVE;
		break;
	default:
		return err;
		break;
	}
	
	xIn.type=INPUT_MOUSE;
	xIn.mi = mInp;
	SendInput(1,&xIn,sizeof(INPUT));
return 0;
}
*/

#endif WINCE