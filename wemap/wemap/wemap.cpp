// wemap.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "wemap.h"
#include <windows.h>
#include <commctrl.h>
#include <winuserm.h>
#include <shellsdk.h>
#include <iostream>

	const int windows_cnt = 6;
	const wchar_t* windows[windows_cnt] = {L"CID_GUI",L"Wemap",L"iGO 8",L"Navitel Navigator (Auto)",L"7ways",L"AUTOSPUTNIK 5"};


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

HWND hWnd_WEMAP;
HWND hWnd_cid_gui;
HWND hWnd_navitel;
HWND hWndEdit;
HWND hWnd_self;
HWND hWnd_last;

const char *fname;
const char *fnamew;
int Angle;

PROCESS_INFORMATION pi_navitel;

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK enum_proc( HWND hWnd, LPARAM lParam );
UINT PrivateModeChange;

void set_rotation(BOOL fShow);

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
 hWnd_WEMAP = 0;
 hWnd_cid_gui = 0;
 hWnd_navitel = 0;
 hWndEdit = 0;
 hWnd_self = 0;
 hWnd_last = 0;


	fname = "\\Storage Card\\wemap\\wemap_WinMain.txt";
	fnamew = "\\Storage Card\\wemap\\Windows.txt";
	MSG msg;
	FILE * pFileTXT;
	Angle = DMDO_180;

	pFileTXT = fopen (fname,"a");

	UINT MS  = RegisterWindowMessageW(L"Private End Route Voice");
	fprintf (pFileTXT, "Private End Route Voice - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Start Route Voice");
	fprintf (pFileTXT, "Private Start Route Voice - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private Close LCN_Upgrade Process");
	fprintf (pFileTXT, "Private Close LCN_Upgrade Process - %d\n", MS);	


	MS = RegisterWindowMessageW(L"Private Trip Info");
	fprintf (pFileTXT, "Private Trip Info - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private Navi Start");
	fprintf (pFileTXT, "Private Navi Start - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Navi Closed");
	fprintf (pFileTXT, "Private Navi Closed - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Navi Destroyed");
	fprintf (pFileTXT, "Private Navi Destroyed - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Navi Exit");
	fprintf (pFileTXT, "Private Navi Exit - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Navi Volume");
	fprintf (pFileTXT, "Private Navi Volume - %d\n", MS);


	MS = RegisterWindowMessageW(L"Private Navi Button Down");
	fprintf (pFileTXT, "Private Navi Button Down - %d\n", MS);


	MS = RegisterWindowMessageW(L"Private Gps Ver Request");
	fprintf (pFileTXT, "Private Gps Ver Request - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Gps Ver Reply");
	fprintf (pFileTXT, "Private Gps Ver Reply - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private TPEG Request");
	fprintf (pFileTXT, "Private TPEG Request - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private TPEG Reply");
	fprintf (pFileTXT, "Private TPEG Reply - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private Gps Speed S12");
	fprintf (pFileTXT, "Private Gps Speed S12 - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private Go To Main Dialog");
	fprintf (pFileTXT, "Private Go To Main Dialog - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Go To Setting Dialog");
	fprintf (pFileTXT, "Private Go To Setting Dialog - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private Request Light Status");
	fprintf (pFileTXT, "Private Request Light Status - %d\n", MS);

	MS = RegisterWindowMessageW(L"UK SA Message Received");
	fprintf (pFileTXT, "UK SA Message Received - %d\n", MS);
	MS = RegisterWindowMessageW(L"UK Abort SA Client");
	fprintf (pFileTXT, "UK Abort SA Client - %d\n", MS);

	PrivateModeChange = RegisterWindowMessageW(L"Private Mode Change");
	fprintf (pFileTXT, "Private Mode Change - %d\n", PrivateModeChange);

	MS = RegisterWindowMessageW(L"Private Light Off");
	fprintf (pFileTXT, "Private Light Off - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Light On");
	fprintf (pFileTXT, "Private Light On - %d\n", MS);

	fclose (pFileTXT);


	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WEMAP));
	int  ret;
	int i;
	i = 0;
	ret = 1;
	while (ret)
	{
		while (!PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
		{
			if (i++>1000)
			{
				if ((hWnd_navitel<=0) || (hWnd_WEMAP<=0) || (hWnd_cid_gui<=0))
					EnumWindows( enum_proc, (LPARAM)"Блокнот" );
				i=0;

			}
		}
		ret = GetMessage(&msg, NULL, NULL, NULL);

		SYSTEMTIME st;
		GetLocalTime(&st);
		pFileTXT = fopen (fname,"a");
		fprintf(pFileTXT, "---> Date: [%02d, %02d, %d]  Time: [%02d:%02d:%02d] " ,st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		fprintf (pFileTXT, " wnd(%d) msg(%d) wParam(%d-%d) lParam(%d-%d) \n", msg.hwnd,msg.message,LOWORD(msg.wParam), HIWORD(msg.wParam),LOWORD(msg.lParam), HIWORD(msg.lParam));

		fclose (pFileTXT);

			
		if(  ret != -1 ) 
		{
			//else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (msg.message==PrivateModeChange)
			{
				pFileTXT = fopen (fname,"a");
				int wmId;
				wmId    = LOWORD(msg.wParam);
				
				if (wmId==1)
				{
					if (hWnd_navitel>0)
					{
						fprintf (pFileTXT, " Show Navitel \n");
						ShowWindow(hWnd_navitel,SW_SHOW);
						SetForegroundWindow(hWnd_navitel);
						set_rotation(true);
						UpdateWindow(hWnd_navitel);

					} else 
					{
						if (hWnd_WEMAP>0)
						{
							fprintf (pFileTXT, " Show WEMAP \n");
							set_rotation(true);
						}
					}

				} else
				{
					if ((hWnd_navitel>0) || (hWnd_WEMAP>0))
						if (wmId!=5)
						{
							fprintf (pFileTXT, " Hide Navitel(WEMAP) \n");
							set_rotation(false);
							if (hWnd_navitel>0)
								ShowWindow(hWnd_navitel,SW_HIDE);
							else
								ShowWindow(hWnd_WEMAP,SW_HIDE);
							ShowWindow(hWnd_cid_gui,SW_SHOW);
						}
				}
				fclose (pFileTXT);
			}
		}
	}
	return( ret );

}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS wc;

	wc.style         = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hInstance;
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WEMAP));
	wc.hCursor       = 0;
	wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName  = 0;
	wc.lpszClassName = szWindowClass;

	return RegisterClass(&wc);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	TCHAR szTitle[MAX_LOADSTRING];		// title bar text
	TCHAR szWindowClass[MAX_LOADSTRING];	// main window class name

	g_hInst = hInstance; // Store instance handle in our global variable


	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING); 
	LoadString(hInstance, IDC_WEMAP, szWindowClass, MAX_LOADSTRING);


	if (!MyRegisterClass(hInstance, szWindowClass))
	{
		return FALSE;
	}
	EnumWindows( enum_proc, (LPARAM)"Блокнот" );
	if (!hWnd_navitel)
	{
		FILE * pFileTXT;

		SYSTEMTIME st;
		GetLocalTime(&st);
		pFileTXT = fopen (fname,"a");
		fprintf(pFileTXT, "---> Date: [%02d, %02d, %d]  Time: [%02d:%02d:%02d] " ,st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		fprintf (pFileTXT, " navitel not running - ");


		STARTUPINFO cif;
		ZeroMemory(&cif,sizeof(STARTUPINFO));
		/*LPSHELLEXECUTEINFO lpExecInfo;
		ZeroMemory(&SHExecInfo, sizeof(SHExecInfo));
		
		SHExecInfo.cbSize = sizeof(SHExecInfo);
		SHExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		SHExecInfo.nShow = SW_SHOWNORMAL;
		SHExecInfo.lpFile = "\\Storage Card\\navi\\navitel.exe";
		if(!ShellExecuteEx(&SHExecInfo)) {
			*/
		if (CreateProcess(L"\\Doc Disk\\igo8\\igo8.exe",NULL,NULL,NULL,FALSE,NULL,NULL,NULL,&cif,&pi_navitel)==TRUE)
		{
			fprintf (pFileTXT, " navitel runned.\n");
		} else
			fprintf (pFileTXT, " navitel run faied.\n");

		fclose (pFileTXT);
	}

	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}
	hWnd_self = hWnd;
/*
	RegisterHotKey(hWnd, 10001, MOD_KEYUP, VK_APP1);
	RegisterHotKey(hWnd, 10002, MOD_KEYUP, VK_APP2);
	RegisterHotKey(hWnd, 10003, MOD_KEYUP, VK_APP3);
	RegisterHotKey(hWnd, 10004, MOD_KEYUP, VK_APP4);
	RegisterHotKey(hWnd, 10005, MOD_KEYUP, VK_APP5);
	RegisterHotKey(hWnd, 10006, MOD_KEYUP, VK_APP6);
*/
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	if (g_hWndCommandBar)
	{
		CommandBar_Show(g_hWndCommandBar, TRUE);
	}

	return TRUE;
}


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	/*if (message>1000)
	{
	 	FILE * pFileTXT;
	
		SYSTEMTIME st;
		GetLocalTime(&st);
		pFileTXT = fopen (fname,"a");
		fprintf(pFileTXT, "-------> Date: [%02d, %02d, %d]  Time: [%02d:%02d:%02d] " ,st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
		fprintf (pFileTXT, " wnd(%d) msg(%d) wParam(%d-%d) lParam(%d-%d) \n", hWnd,message,LOWORD(wParam), HIWORD(wParam),LOWORD(lParam), HIWORD(lParam));

		fclose (pFileTXT);
	}*/
	switch (message) 
	{
	
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_HELP_ABOUT:

			DialogBox(g_hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, About);
			break;
		case IDM_FILE_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		g_hWndCommandBar = CommandBar_Create(g_hInst, hWnd, 1);
		CommandBar_InsertMenubar(g_hWndCommandBar, g_hInst, IDR_MENU, 0);
		CommandBar_AddAdornments(g_hWndCommandBar, 0, 0);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		CommandBar_Destroy(g_hWndCommandBar);
		if (hWnd_navitel) 
			DestroyWindow(hWnd_navitel);
		//if (pi_navitel.hProcess)
		//	TerminateProcess(pi_navitel.hProcess,NO_ERROR);
		PostQuitMessage(0);
		break;

	default:

		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		RECT rectChild, rectParent;
		int DlgWidth, DlgHeight;	// dialog width and height in pixel units
		int NewPosX, NewPosY;

		// trying to center the About dialog
		if (GetWindowRect(hDlg, &rectChild)) 
		{
			GetClientRect(GetParent(hDlg), &rectParent);
			DlgWidth	= rectChild.right - rectChild.left;
			DlgHeight	= rectChild.bottom - rectChild.top ;
			NewPosX		= (rectParent.right - rectParent.left - DlgWidth) / 2;
			NewPosY		= (rectParent.bottom - rectParent.top - DlgHeight) / 2;

			// if the About box is larger than the physical screen 
			if (NewPosX < 0) NewPosX = 0;
			if (NewPosY < 0) NewPosY = 0;
			SetWindowPos(hDlg, 0, NewPosX, NewPosY,
				0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, message);
		return TRUE;

	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK enum_proc( HWND hWnd, LPARAM lParam )
{
	FILE * pFileTXT;
	SYSTEMTIME st;
	GetLocalTime(&st);

	wchar_t * buffer = new wchar_t[1024+1];
	if ( GetWindowText( hWnd, buffer, 1024 ) ) {

		for (int i=0;i<windows_cnt;i++)
			if (wcsstr(buffer,windows[i])) 
			{
				switch(i)
				{
				case 0:
					hWnd_cid_gui = hWnd;
					break;
				case 1:
					hWnd_WEMAP = hWnd;
					break;
				case 2:
					hWnd_navitel = hWnd;
					break;
				}
				pFileTXT = fopen (fname,"a");
				fprintf(pFileTXT, "---> Date: [%02d, %02d, %d]  Time: [%02d:%02d:%02d] " ,st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
				fprintf (pFileTXT, "Wnd '%ls' handled %d\n",windows[i],hWnd);
				fclose (pFileTXT);
			}
	}
	delete[] buffer;
	buffer = 0;
	return TRUE;
}

void set_rotation(BOOL fShow)
{
	DEVMODE DevMode;

	//int RotationAngles;
	int CurrentAngle;
	int NewAngle;

	FILE * pFileTXT;
	SYSTEMTIME st;
	GetLocalTime(&st);

	//
	// Get the current rotation angle.
	//

	memset(&DevMode, 0, sizeof (DevMode));
	DevMode.dmSize   = sizeof (DevMode);
	DevMode.dmFields = DM_DISPLAYORIENTATION;

	pFileTXT = fopen (fname,"a");
	fprintf(pFileTXT, "---> Date: [%02d, %02d, %d]  Time: [%02d:%02d:%02d] " ,st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
	
	if (0 != EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &DevMode))
	{
		CurrentAngle = DevMode.dmDisplayOrientation;
		fprintf (pFileTXT, "ChangeAngle checked. Current=%d\n",CurrentAngle);
	}
	else
	{ 
		CurrentAngle = -1;
		fprintf (pFileTXT, "ChangeAngle failed. Current=%d\n",CurrentAngle);
	}

	if (CurrentAngle != -1) {
		CurrentAngle = Angle;

		NewAngle = CurrentAngle;

		if ((!fShow)) // && (CurrentAngle==DMDO_0))
				NewAngle = DMDO_0;
		if ((fShow)) // && (CurrentAngle==DMDO_180))
				NewAngle = DMDO_180;

		if (CurrentAngle!=NewAngle)
		{
			Angle = NewAngle;		
			memset(&DevMode, 0, sizeof (DevMode));
			DevMode.dmSize               = sizeof (DevMode);
			DevMode.dmFields             = DM_DISPLAYORIENTATION;
			DevMode.dmDisplayOrientation = NewAngle;
			fprintf (pFileTXT, "ChangeAngle. Current=%d, New=%d",CurrentAngle,Angle);

			if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_RESET, NULL))
				fprintf (pFileTXT, " - changed\n");
			else
				fprintf (pFileTXT, " - error\n");

		}

	}
	fclose (pFileTXT);
}
