// EnumWindows.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "EnumWindows.h"
#include <windows.h>
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK enum_proc( HWND hWnd, LPARAM lParam );

HWND hWnd_WEMAP;
HWND hWnd_cid_gui;
HWND hWnd_navitel;
HWND hWnd_about;

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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ENUMWINDOWS));
FILE * pFileTXT;
	pFileTXT = fopen ("\\Storage Card\\EnumWindows.txt","a");
	fprintf (pFileTXT, "--------------------------------------------------------------\n");
	fclose (pFileTXT);
	// Main message loop:
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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ENUMWINDOWS));
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
	LoadString(hInstance, IDC_ENUMWINDOWS, szWindowClass, MAX_LOADSTRING);


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

BOOL CALLBACK enum_child( HWND hWnd, int level )
{
	
	FILE * pFileTXT;

	wchar_t * buffer = new wchar_t[1024];
	
	HWND hwnd;
	BOOL is_get;

	char p[100];

	hwnd = GetWindow(hWnd, GW_CHILD);
    while (hwnd !=0)
    {
		is_get = GetWindowText( hwnd, buffer, 1024 ); 
		
		pFileTXT = fopen ("\\Storage Card\\EnumWindows.txt","a");
		for (int i=0; i<level;i++)
			fprintf (pFileTXT, "  ");
		fprintf (pFileTXT, "%d parent for wnd=%d '%ls'\n", hWnd,hwnd,buffer);
		fclose (pFileTXT);
		enum_child(hwnd, level+1);

		hwnd = GetWindow(hwnd, GW_HWNDNEXT);
    }
	return TRUE;
}

BOOL CALLBACK enum_proc( HWND hWnd, LPARAM lParam )
{
	

	wchar_t * buffer = new wchar_t[1024];
	BOOL is_get;
	is_get = GetWindowText( hWnd, buffer, 1024 ); 
	
	FILE * pFileTXT;
	pFileTXT = fopen ("\\Storage Card\\EnumWindows.txt","a");
	fprintf (pFileTXT, "%d '%ls'\n", hWnd,buffer);
	fclose (pFileTXT);

	if ( is_get ) {
		if (wcsstr(buffer,L"CID_GUI")) {
			hWnd_cid_gui = hWnd;
			pFileTXT = fopen ("\\Storage Card\\EnumWindows.txt","a");
			fprintf (pFileTXT, "hWnd_cid_gui handled'\n" );
			fclose (pFileTXT);
		}
		if (wcsstr(buffer,L"Navitel Navigator (Auto)")) {
			hWnd_navitel = hWnd;
			pFileTXT = fopen ("\\Storage Card\\EnumWindows.txt","a");
			fprintf (pFileTXT, "hWnd_navitel handled'\n" );
			fclose (pFileTXT);
		}
		if (wcsstr(buffer,L"Wemap")) {
			hWnd_WEMAP = hWnd;
			pFileTXT = fopen ("\\Storage Card\\EnumWindows.txt","a");
			fprintf (pFileTXT, "hWnd_WEMAP'\n" );
			fclose (pFileTXT);
		}
	}
	enum_child(hWnd, 1);


	return TRUE;
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
		hWnd_about = hDlg;

		EnumWindows( enum_proc, (LPARAM)"Блокнот" );
		if (hWnd_navitel)
		{
			wchar_t msg[200];
			swprintf((LPWSTR)&msg, L"Wnd Navitel =%d", hWnd_navitel);
			SetWindowText(hWnd_about,msg);
		}

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
			if (hWnd_navitel)
			{
					ShowWindow(hWnd_navitel,SW_SHOW);
					Sleep(5000);
					ShowWindow(hWnd_navitel,SW_HIDE);
			}
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
