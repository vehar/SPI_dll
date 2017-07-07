// WMCOPYClient.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "WndClientServer.h"

#include "KbdHandler.h"

#ifdef WINCE

#define SERVERWNDCLASSNAME		_T("S#01")
#define CLIENTWNDCLASSNAME		_T("C#01")


HWND hServer;
DWORD dwThreadId;
DWORD WINAPI ThreadClient(LPVOID lpParameter);


BOOL WINAPI TransferData(DWORD dwMsg, const _TCHAR *Buffer, DWORD dwBytes) 
{
  BOOL bSend;
  COPYDATASTRUCT cpStructData;
  LPWM_DATASTRUCTURE lpMsg;

  cpStructData.cbData = dwBytes + _WM_HEADER_SIZE;
  
  lpMsg = (LPWM_DATASTRUCTURE)LocalAlloc(LPTR,cpStructData.cbData);
  lpMsg->hClient = NULL;
  lpMsg->iMessage = dwMsg;
  lpMsg->cbSize = dwBytes;
  
  cpStructData.lpData = lpMsg;
  if(Buffer!=NULL)
  {
	  SETSTRLEN(lpMsg->Data,dwBytes);
	  MEMCPY(lpMsg->Data,Buffer,dwBytes); 
  }

  bSend = SendMessage(hServer,WM_COPYDATA, (WPARAM)hServer,(LPARAM)&cpStructData);
  return(bSend);
}  


LRESULT CALLBACK WMCOPYWNDPROC(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
LPWM_DATASTRUCTURE lpMsg;  
  if(uMsg == WM_COPYDATA)
  {
     lpMsg = (LPWM_DATASTRUCTURE)((COPYDATASTRUCT*)lParam)->lpData;
	 SETSTRLEN(lpMsg->Data,(lpMsg->cbSize)*2);	 
	 switch(lpMsg->iMessage)
	 {
		case	WM_DISPLAY_TEXT:
				TPRINTF(_T("\n"));
				TPRINTF(lpMsg->Data);				
		break;	 
		case	WM_COMMAND_SET:
				CmdThread.SetElement(lpMsg->Data[0], 0);//Add cmd to gueue from other app
				TransferData(WM_COMMAND_SET,lpMsg->Data,STRLEN(lpMsg->Data)*2); //return ACK
		break;	
		case	WM_QUIT_SERVER:
				PostQuitMessage(0);
		break;
	 }
  }		
  else
   return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
/////////////////////////////////////////////////////////////////



DWORD WINAPI ThreadServ(LPVOID lpParameter)
{
	MSG iMsg; 
	BOOL bVal;
	WNDCLASS WMClass;
	WMClass.style = CS_GLOBALCLASS;
	WMClass.lpfnWndProc = WMCOPYWNDPROC;
	
//	SETCONSOLETITLE(WINSERVERNAME);
	if(FINDWINDOW(SERVERWNDCLASSNAME,NULL)!=NULL)
	{
		TPRINTF(_T("WMCOPYSERVER::Server is already running!\n"));
	}
	else{
		WMClass.hInstance = GETMODULEHANDLE;
		WMClass.lpszClassName = SERVERWNDCLASSNAME;
		//WMClass.lpszClassName = _T("S1");
		WMClass.cbClsExtra = 0;
		WMClass.cbWndExtra = 0;
		WMClass.hbrBackground = NULL;
		WMClass.hCursor = NULL;
		WMClass.hIcon = NULL;
		WMClass.lpszMenuName = NULL;
		if(RegisterClass(&WMClass) == 0)
		{
			TPRINTF(_T("WMCOPYSERVER: class registeration failed!\n"));
		}
		else {
			if(CreateWindow((LPCTSTR)SERVERWNDCLASSNAME, NULL, 0, 0, 0, 0, 0,
			//if(CreateWindow((LPCTSTR)_T("S1"), NULL, 0, 0, 0, 0, 0,
				NULL, NULL, WMClass.hInstance, NULL) == NULL)
			{
				TPRINTF(_T("\nWMCOPYSERVER: Can't create window!"));
			}
			else 
			{
			//	TPRINTF(_T("\nWMCOPYSERVER: Server launched successfully!"));
				while((bVal = GetMessage(&iMsg, NULL, 0, 0)) && (bVal != -1))
				DispatchMessage(&iMsg);
			}		  		
		} 
	}
	//TPRINTF(_T("\nWMCOPYSERVER: Press Enter to quit..."));
	//getchar();
printf("Exiting tread SERVER %i \n",lpParameter);
//VOID ExitThread();
//	CloseHandle(hHandle);
	return 0;
}


DWORD WINAPI ThreadSearchClient(LPVOID lpParameter)
{
	while((hServer = FINDWINDOW(CLIENTWNDCLASSNAME, NULL)) == NULL)
	{
		Sleep(500);
	}
printf("Server launched!!! \n");
		HANDLE hClientTread = CreateThread(NULL, 0, ThreadClient, (LPVOID)2, 0, &dwThreadId);  
printf("Client launched!!! \n");
return 0;
}

DWORD WINAPI ThreadClient(LPVOID lpParameter)
{
//DWORD pID;
//	_TCHAR szWndName[256];	
	_TCHAR szData[256];
	_TCHAR szSort[5];

		while(TRUE) 
		{	
				TPRINTF(_T("Enter 3 to display the entered value on the server:\n"));
				TPRINTF(_T("Enter 4 to stop the server:\n"));
			
				_getts(szSort);
         
				if(TCMP(szSort, _T("3")) == 0)
				{
					TPRINTF(_T("Enter the data to be displayed on the server:\n"));
					_getts(szData);
					TransferData(WM_DISPLAY_TEXT,szData,STRLEN(szData)*2);
				}

				if(TCMP(szSort, _T("4")) == 0)
				{
					TPRINTF(_T("Quiting the server and the client:\n"));
					TransferData(WM_QUIT_SERVER,szData,STRLEN(szData));
					//break;
				}
         }
	printf("Exiting tread Client %i \n",lpParameter);
return 0;
}

/*
int main(int argc, char* argv[])
{
DWORD dwThreadId;
HANDLE hServTread = CreateThread(NULL, 0, ThreadServ, (LPVOID)2, 0, &dwThreadId);  
HANDLE hSearchClientTread = CreateThread(NULL, 0, ThreadSearchClient, (LPVOID)2, 0, &dwThreadId);  


/////////////////////////////////////////////////////////////////////////
	while(1)
	{
		Sleep(500);
	}
return 0;
}
*/

#endif WINCE