/********************************************************************
	author:		Victor Mariinschii
	created:	2011/05/23
	filename: 	Task.cpp
	purpose:	Thread synchronization
*********************************************************************/

#include "Task.h"

#ifdef WINCE

void QueueTask::SetElement(int Cmd, const int Data /* = -1 */)
{
	bool Need2Wait;
	Tx.Cmd = Cmd;
	Tx.Data = Data;
	Lock();
	qTie.push(Tx);
	Need2Wait = (qTie.size() > QUEUE_MAX_SIZE);
	Unlock();
	if (Need2Wait)	// defense
		Sleep(40);
}

void QueueTask::ReceiveElement()
{
	Lock();
	if (!qTie.empty())
	{
		Rx = qTie.front();
		qTie.pop();
	}
	Unlock();
}

#endif WINCE