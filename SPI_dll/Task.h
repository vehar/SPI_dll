#ifndef TASK_H
#define TASK_H


#pragma once

#ifdef WINCE

/************************************************************************
 INCLUDES
************************************************************************/
#include "stdafx.h"
//#include "Data_DAQLoop.h"
#include <queue>
using namespace std;

/************************************************************************
 DEFINES
************************************************************************/
#define QUEUE_MAX_SIZE	30

struct tTransfer
{
	int Cmd;
	int Data;
};

static DWORD ThreadID = 0;

/************************************************************************
 CLASS LOCK
************************************************************************/
class Lock_CriticalSection
{
public:
	Lock_CriticalSection(void) { InitializeCriticalSection(&cs); };
	~Lock_CriticalSection(void){ DeleteCriticalSection(&cs); };
	void Lock() { EnterCriticalSection(&cs); };
	void Unlock() { LeaveCriticalSection(&cs); };

private:
	CRITICAL_SECTION cs; 
};

/************************************************************************
 CLASS SLAVE LOOP SYNCHRONIZATION
************************************************************************/
class QueueTask : public Lock_CriticalSection
{
public:
	QueueTask(void) { hThread = 0; };
	~QueueTask(void) {if (hThread) CloseHandle(hThread); };

private:
	HANDLE hThread;
	tTransfer Tx, Rx;
	queue <tTransfer> qTie;

public:
	bool WasThreadCreated() { return ((hThread == 0) ? false : true); };
	bool IsEmpty() { return qTie.empty(); };
	void SetElement(int Cmd, const int Data = -1);	// -1 --> No Data
	void ReceiveElement();
	int GetCmd() { return Rx.Cmd; } ;
	int GetData() { return Rx.Data; };
	void FinishThread(int Cmd) { SetElement(Cmd); Sleep(30); };
	void RunThread(PTHREAD_START_ROUTINE MFunc) { hThread = CreateThread(NULL, 0, MFunc, 0, 0, &ThreadID); Sleep(100); };
};

#endif WINCE

#endif TASK_H