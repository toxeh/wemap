// lsdir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "lsdir.h"
#include <windows.h>
#include <commctrl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// The one and only application object

CWinApp theApp;

using namespace std;

CStdioFile destFile;
CFileException ex;


int GetDirectories (CString szPath)
{
	int returnvalue = 0;
	WIN32_FIND_DATA findData;
	HANDLE findHandle;


	if ( (findHandle=FindFirstFile(szPath+L"\\*.*", &findData)) != INVALID_HANDLE_VALUE )
	{
		do
		{

			if ( wcscmp(findData.cFileName,L".") &&
				wcscmp(findData.cFileName,L"..") ) // Skip Root (..) and current (.) dir
			{
				FILETIME ftCreationTime = findData.ftCreationTime;
				SYSTEMTIME stUTC;
				FileTimeToSystemTime(&ftCreationTime, &stUTC);

				DWORD filesize = /*(findData.nFileSizeHigh * (MAXDWORD+1)) + */findData.nFileSizeLow;

				DWORD dwSize = 500;
				LPTSTR file_line = new TCHAR[500]; // LPCTSTR
				DWORD dwRet = StringCchPrintfW(file_line, 500, L"%s\\%s;%ul;%02d/%02d/%d %02d:%02d:%02d\n",szPath,findData.cFileName, filesize,stUTC.wDay, stUTC.wMonth,  stUTC.wYear,stUTC.wHour, stUTC.wMinute,stUTC.wSecond);

				destFile.WriteString(file_line);
				
				delete[] file_line;
				
				if ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) 
					GetDirectories(szPath+'\\'+findData.cFileName);
			}
		}while ( FindNextFile(findHandle, &findData) );
	}
	return returnvalue;
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs

		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		TCHAR szAppPath[MAX_PATH];

		GetModuleFileName( NULL, szAppPath, MAX_PATH );
		CString strAppDirectory  = szAppPath;
		strAppDirectory = strAppDirectory.Left(strAppDirectory.ReverseFind(L'\\'));

		LPCTSTR pFileName = strAppDirectory+L"\\device_list_files.txt";

		if (!destFile.Open(pFileName, CFile::modeCreate
			| CFile::modeWrite | CFile::typeText |CFile::modeNoTruncate, &ex))
		{
			TCHAR szError[1024];
			ex.GetErrorMessage(szError, 1024);

			int x = MessageBox(
				0,szError, L"Error open file", 
				MB_ICONERROR | MB_OK);

			return 1;
		}
		GetDirectories(L"\\DOC Disk");

		GetDirectories(L"\\Windows");

		destFile.Close();

		int x = MessageBox(
			0,L"Список файлов сформирован в device_list_files.txt", L"Уведомление", 
			MB_ICONERROR | MB_OK);

	}

	return nRetCode;
}
