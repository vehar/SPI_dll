#ifndef KBDHANDLER_H
#define KBDHANDLER_H

#pragma once

#ifdef WINCE
#include "Communication_DM_STM.h"
#include "Communication_Defines.h"

#include "Task.h"

extern Communication Comm;		//Communication protocol vs STM
extern QueueTask CmdThread;	// Objects, that control slave threads

void               KeypadInit();

extern int DataRefreshed_Flag;
extern int KeyStateFlag;
extern int EncState;
extern int KeyCode;
extern int KeyPressCnt;
extern int EncCNT;

enum  SPI_FLAGS { OK, ERR, INVALID_START, INVALID_CMD, INVALID_DATA, INVALID_XOR, RESEND_REQ };
void SPI_Handling(void);

DWORD WINAPI ThreadKeybProc(LPVOID lpParameter);
DWORD WINAPI ThreadVoltProc(LPVOID lpParameter);
DWORD WINAPI ThreadGPSHandling(LPVOID lpParameter);
DWORD WINAPI ThreadAxelTempProc(LPVOID lpParameter);


void SendKbdMsg(bool pressed, int vk);
int SendMouseMsg(DWORD flag, int data, int dx, int dy);


//+++SPI_cust_protocol++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++


#endif WINCE

#endif KBDHANDLER_H

