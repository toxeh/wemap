// volume_info.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "volume_info.h"
#include <windows.h>
#include <commctrl.h>
#include <winioctl.h>

typedef struct _DEVICE_ID {
        DWORD dwSize;             // Size of the allocated structure and memory for the identifiers. The caller must allocate enough memory for the structure and the identifiers.
        DWORD dwPresetIDOffset;   // Number of bytes from the beginning of the structure to where the Preset identifier is located.
        DWORD dwPresetIDBytes;    // Number of bytes used to store the Preset identifier. If this value is set to zero, no Preset identifier is available.
        DWORD dwPlatformIDOffset; // Number of bytes from the beginning of the structure to where the Platform identifier is located.
        DWORD dwPlatformIDBytes;  // Number of bytes used to store the Platform identifier. If this value is set to zero, no Platform identifier is available.
} DEVICE_ID, *PDEVICE_ID;

#define IOCTL_HAL_GET_DEVICEID    CTL_CODE(FILE_DEVICE_HAL, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_HAL_GET_DEVICE_INFO  CTL_CODE(FILE_DEVICE_HAL, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
  #define IOCTL_HAL_GET_UUID CTL_CODE(FILE_DEVICE_HAL, 13,  METHOD_BUFFERED, FILE_ANY_ACCESS)

  #define IOCTL_DISK_GET_STORAGEID   CTL_CODE(FILE_DEVICE_DISK,0x709,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define TT_ALEN(A) (sizeof(A)/sizeof*(A))
#define SPI_GETOEMINFO 258
extern "C" __declspec(dllimport) BOOL KernelIoControl(
                                                        DWORD dwIoControlCode,
                                                        LPVOID lpInBuf,
                                                        DWORD nInBufSize,
                                                        LPVOID lpOutBuf,
                                                        DWORD nOutBufSize,
                                                        LPDWORD lpBytesReturned
);


  typedef struct _STORAGE_IDENTIFICATION
  {
  DWORD dwSize;
  DWORD dwFlags;
  DWORD dwManufactureIDOffset;
  DWORD dwSerialNumOffset;
  } STORAGE_IDENTIFICATION;
 
  typedef struct _PPC_STOAGEID
  {
  STORAGE_IDENTIFICATION StorageID;
  unsigned char Data[32];
  } PPC_STORAGE_ID;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE			g_hInst;			// current instance
HWND				g_hWndCommandBar;	// command bar handle

// Forward declarations of functions included in this code module:
ATOM			MyRegisterClass(HINSTANCE, LPTSTR);
BOOL			InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VOLUME_INFO));


    WCHAR deviceName[128]={0};
    DWORD spiInfo = SPI_GETOEMINFO;

	FILE * pFileTXT;

				pFileTXT = fopen ("\\Storage Card\\volume_info.txt","a");


    // perform the IOCTL that must be implemented on each platform
    if (KernelIoControl(IOCTL_HAL_GET_DEVICE_INFO, &spiInfo, sizeof(DWORD), deviceName, TT_ALEN(deviceName), 0))
               fprintf(pFileTXT,"Device name:\r\n%s\r\n\r\n", deviceName);
        else
               fprintf(pFileTXT,"KernelIoControl(GET_DEVICE_INFO) failed with %d", GetLastError());
 
        DEVICE_ID devId = {0};
 
        // To request the identifier buffer sizes required for the DEVICE_ID structure, set the dwSize member of the DEVICE_ID structure to zero.
        devId.dwSize=0;
        KernelIoControl(IOCTL_HAL_GET_DEVICEID, NULL, 0, &devId, sizeof(DEVICE_ID), 0);
 
        if (devId.dwSize < sizeof(DEVICE_ID))
        {
               fprintf(pFileTXT,"KernelIoControl(GET_DEVICEID) failed with %d", GetLastError());

        }
 
        BYTE *bOutBuf;
        bOutBuf = new BYTE[devId.dwSize];
 
        DWORD     dwBytesReturned;
        if (KernelIoControl(IOCTL_HAL_GET_DEVICEID, NULL, 0, bOutBuf, devId.dwSize, &dwBytesReturned))
        {
 
               memcpy(&devId, bOutBuf, sizeof(DEVICE_ID)); // Copy the first sizeof(DEVICE_ID) bytes of the output buffer to the DEVICE_ID structure
 

               fprintf(pFileTXT,"KernelIoControl() SUCCESS\r\n\r\nNb bytes returned %d\r\n", devId.dwSize);
               fprintf(pFileTXT,"  PresetID Offset %d Bytes %d\r\n", devId.dwPresetIDOffset, devId.dwPresetIDBytes);
               fprintf(pFileTXT,"PlatformID Offset %d Bytes %d\r\n\r\n", devId.dwPlatformIDOffset, devId.dwPlatformIDBytes);

 
               fprintf(pFileTXT,"MLB Serial:\r\n");
               uint i=devId.dwPresetIDOffset;
               for (;i<devId.dwPresetIDOffset+devId.dwPresetIDBytes; i++)
               {
                       if (bOutBuf[ i ])
                               fprintf(pFileTXT,"%c",bOutBuf[ i ]);
                       else
                               break;
               }
 
               fprintf(pFileTXT,"\r\n\r\nTerm Serial:\r\n");
               for (i++;i<devId.dwPresetIDOffset+devId.dwPresetIDBytes; i++)
               {
                       if (bOutBuf[ i ])
                               fprintf(pFileTXT,"%c",bOutBuf[ i ]);
                       else
                               break;
               }
 
               fprintf(pFileTXT,"\r\n\r\nPlatform ID:\r\n");
               for (i=devId.dwPlatformIDOffset;i<devId.dwPlatformIDOffset+devId.dwPlatformIDBytes; i++)
               {
                       if (bOutBuf[ i ])
                               fprintf(pFileTXT,"%c",bOutBuf[ i ]);
                       else
                               break;
               }
        }
        else
			
               fprintf(pFileTXT,"KernelIoControl(GET_DEVICEID) failed with %d\n", GetLastError());

PPC_STORAGE_ID idSD;
  HANDLE hSD;
  DWORD dwLength;
  DWORD dwError;
  BOOL bResult;
  // I have checked the registry profile that the SD card is DSK1:
  TCHAR sDeviceName[10]=_T("DSK1:");
  GUID guid;
  const int nBuffSize = 4096;
  byte arrOutBuff[nBuffSize];
 
  memset(&idSD,0,sizeof(PPC_STORAGE_ID));
  idSD.StorageID.dwSize=sizeof(PPC_STORAGE_ID);
 
 hSD=CreateFile(sDeviceName,GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,0,NULL);
  if(hSD)
  {
 
 bResult=DeviceIoControl(hSD,IOCTL_DISK_GET_STORAGEID,NULL,0,&idSD,sizeof(PPC_STORAGE_ID),&dwLength,NULL);
  // Most HP, Dell and Acer with PPC2003 return bResult=TRUE, AsusP716 return FALSE
  if(bResult==FALSE)
  dwError=GetLastError(); // Asus P716 return dwError=18
  fprintf(pFileTXT,"DSK1: Storage id error %d", dwError);

  CloseHandle(hSD);
  } else {
	  uint i=0;
               for (;i<32; i++)
               {
                       if (idSD.Data[ i ])
                               fprintf(pFileTXT,"%c",idSD.Data[ i ]);
                       else
                               break;
               }

	 
  }

 
  memset(arrOutBuff,0,nBuffSize);
 	
		
		
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
	wc.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VOLUME_INFO));
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
    LoadString(hInstance, IDC_VOLUME_INFO, szWindowClass, MAX_LOADSTRING);


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

	/*char NameBuffer[MAX_PATH];
	char SysNameBuffer[MAX_PATH];
	DWORD VSNumber;
	DWORD MCLength;
	DWORD FileSF;
	FILE * pFileTXT;

	if (GetVolumeInformation("C:\\",NameBuffer, sizeof(NameBuffer),
		&VSNumber,&MCLength,&FileSF,SysNameBuffer,sizeof(SysNameBuffer)))
	{
				pFileTXT = fopen ("\\Storage Card\\volume_info.txt","a");
				fprintf(pFileTXT, "NameBuffer='%s', SysNameBuffer='%s', VSNumber='%d'\n" ,NameBuffer,SysNameBuffer,VSNumber);
				fprintf (pFileTXT, " navitel not running \n");
				fclose (pFileTXT);
	}*/


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
