#ifndef WNDCLIENTSERVER_H
#define WNDCLIENTSERVER_H

// Defines the different messages and the data structure which is 
// to be transferred between the various processes.
#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include <windows.h>
#include <tchar.h>

#ifndef __WMCOPYHDRS__
#define __WMCOPYHDRS__

#define	SETSTRLEN(szString,iNum)	((szString)[(iNum)]='\0')
#define WINSERVERNAME		_T("WMCOPYSERVER#01")
#define WINCLIENTNAME		_T("WMCOPYCLIENT#01")
//#define SERVERWNDCLASSNAME		_T("S#01")
//#define CLIENTWNDCLASSNAME		_T("C#01")
#define SETCONSOLETITLE(szTitle)	(SetConsoleTitle(szTitle))
#define GETMODULEHANDLE			(GetModuleHandle(NULL))
#define	FINDWINDOW(szWndClassName,szWndTitle)	(FindWindow((LPCTSTR)szWndClassName,(LPCTSTR)szWndTitle))
#define STPRINTF(szBuffer,szFormat,szArgs)		(_stprintf((_TCHAR*)szBuffer,(_TCHAR*)szFormat,(unsigned long)szArgs))
#define STRCPY(strDest,strSource,iCount)		(_tcsncpy((_TCHAR*)strDest,(_TCHAR*)strSource,size_t(iCount)))
#define STRLEN(szValue)							(_tcslen((const _TCHAR*)szValue))
#define TPRINTF(szValue)						(_tprintf((_TCHAR*)szValue))
#define MEMCPY(szDest,szTarget,iCount)			(memcpy((void *) szDest,(const void*)szTarget,(size_t)iCount))
#define TCMP(szDest,szTarget)					(_tcsicmp((const _TCHAR*)szDest,(const _TCHAR*)szTarget))
#define WM_ASCENDING			1
#define	WM_DESCENDING			2	
#define WM_DISPLAY_TEXT			3
#define	WM_QUIT_SERVER			4

#define	WM_COMMAND_SET			5

#define	_WM_MAXMESSAGE_SIZE		0x10000
#define _WM_HEADER_SIZE	(2*sizeof(DWORD)+sizeof(HWND))

typedef struct _WM_DATASTRUCTURE{
	DWORD cbSize;
	DWORD iMessage;
	HWND hClient;
	_TCHAR Data[_WM_MAXMESSAGE_SIZE - _WM_HEADER_SIZE];	
}WM_DATASTRUCTURE,*LPWM_DATASTRUCTURE;


DWORD WINAPI ThreadSearchClient(LPVOID lpParameter);
DWORD WINAPI ThreadClient(LPVOID lpParameter);
DWORD WINAPI ThreadServ(LPVOID lpParameter);

BOOL WINAPI TransferData(DWORD dwMsg, const _TCHAR *Buffer, DWORD dwBytes); 
LRESULT CALLBACK WMCOPYWNDPROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 

#endif __WMCOPYHDRS__ /* __WMCOPYHDRS__ */


#endif WNDCLIENTSERVER_H