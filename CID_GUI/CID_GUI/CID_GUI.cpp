// CID_GUI.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CID_GUI.h"
#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include <iostream>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND hWnd_WEMAP;
HWND hWnd_cid_gui;
HWND hWnd_navitel;

BOOL CALLBACK enum_proc( HWND hWnd, LPARAM lParam );

int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPTSTR    lpCmdLine,
				   int       nCmdShow)
{
	MSG msg;

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CID_GUI));
	// Main message loop:
	FILE * pFileTXT;

	pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");

	UINT MS  = RegisterWindowMessageW(L"Private End Route Voice");
	fprintf (pFileTXT, "Private End Route Voice - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Start Route Voice");
	fprintf (pFileTXT, "Private Start Route Voice - %d\n", MS);


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

	MS = RegisterWindowMessageW(L"Private Mode Change");
	fprintf (pFileTXT, "Private Mode Change - %d\n", MS);

	MS = RegisterWindowMessageW(L"Private Light Off");
	fprintf (pFileTXT, "Private Light Off - %d\n", MS);
	MS = RegisterWindowMessageW(L"Private Light On");
	fprintf (pFileTXT, "Private Light On - %d\n", MS);

	fclose (pFileTXT);


	fclose (pFileTXT);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}



	return (int) msg.wParam;
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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CID_GUI));
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
	LoadString(hInstance, IDC_CID_GUI, szWindowClass, MAX_LOADSTRING);


	if (!MyRegisterClass(hInstance, szWindowClass))
	{
		return FALSE;
	}

	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}


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

	FILE * pFileTXT;

	pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");

	fprintf (pFileTXT, "WndProc %d %d-%d %d-%d\n", message,LOWORD(wParam), HIWORD(wParam),LOWORD(lParam), HIWORD(lParam));
	fclose (pFileTXT);

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

		// TODO: Add any drawing code here...

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		CommandBar_Destroy(g_hWndCommandBar);
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
		FILE * pFileTXT;
	
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
		EnumWindows( enum_proc, (LPARAM)"Блокнот" );
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		} else
			switch LOWORD(wParam)
		{
			case IDC_BUTTON0:
				pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
				fprintf (pFileTXT, "Sent 0 message\n");
				fclose (pFileTXT);
				if (hWnd_WEMAP) PostMessageW(hWnd_WEMAP,49155,0,0);
				break;
			case IDC_BUTTON1:
				pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
				fprintf (pFileTXT, "Sent 1 message\n");
				fclose (pFileTXT);
				if (hWnd_WEMAP) PostMessageW(hWnd_WEMAP,49177,0,0);
				break;
			case IDC_BUTTON2:
				pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
				fprintf (pFileTXT, "Sent 2 message\n");
				fclose (pFileTXT);
				if (hWnd_WEMAP) PostMessage(hWnd_WEMAP,49164,0,0);
				break;
			case IDC_BUTTON5:	
				pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
				fprintf (pFileTXT, "Sent 5 message\n");
				fclose (pFileTXT);
				if (hWnd_WEMAP) PostMessageW(hWnd_WEMAP,49157,0,0);
				break;
			case IDC_BUTTON7:	
				pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
				fprintf (pFileTXT, "Sent 7 message\n");
				fclose (pFileTXT);
				if (hWnd_WEMAP) PostMessageW(hWnd_WEMAP,49156,7,0);
				break;
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

	wchar_t * buffer = new wchar_t[1024];
	if ( GetWindowText( hWnd, buffer, 1024 ) ) {
		if (wcsstr(buffer,L"CID_GUI")) {
			hWnd_cid_gui = hWnd;
			pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
			fprintf (pFileTXT, "hWnd_cid_gui handled'\n" );
			fclose (pFileTXT);
		}
		if (wcsstr(buffer,L"Navitel Navigator (Auto)")) {
			hWnd_navitel = hWnd;
			pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
			fprintf (pFileTXT, "hWnd_navitel handled'\n" );
			fclose (pFileTXT);
		}
		if (wcsstr(buffer,L"Wemap")) {
			hWnd_WEMAP = hWnd;
			pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
			fprintf (pFileTXT, "hWnd_WEMAP'\n" );
			fclose (pFileTXT);
		}



		pFileTXT = fopen ("\\Storage Card\\cid_gui_log.txt","a");
		fprintf (pFileTXT, "%d '%ls'\n", hWnd,buffer);
		fclose (pFileTXT);
	}
	return TRUE;
}

