/**********************************************************************
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Copyright (c) 1999 Microsoft Corporation. All Rights Reserved.

MODULE: 
rotation.cpp

ABSTRACT: 
This application code demonstrates the how you can set up the DEVMODE structure and use the ChangeDisplaySettingsEx function to rotate the screen. 
Each time you run the executable for this code, the screen will rotate clockwise by 90 degrees.

**********************************************************************/

#include <windows.h>

int
WINAPI
WinMain(
		HINSTANCE,
		HINSTANCE,

		LPWSTR,

		int
		)
{
	DEVMODE DevMode;

	int RotationAngles;
	int CurrentAngle;
	int NewAngle;

	//
	// Check for rotation support by getting the rotation angles supported.
	//

	memset (&DevMode, 0, sizeof (DevMode));
	DevMode.dmSize   = sizeof (DevMode);
	DevMode.dmFields = DM_DISPLAYQUERYORIENTATION;

	if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL))
	{
		RotationAngles = DevMode.dmDisplayOrientation;
		RETAILMSG(1, (L"ChangeDisplaySettingsEx supports these rotation angles %d", RotationAngles));
	}
	else
	{
		RETAILMSG(1, (L"ChangeDisplaySettingsEx failed to get the supported rotation angles."));
		RotationAngles = -1;
	}

	//
	// Get the current rotation angle.
	//

	memset(&DevMode, 0, sizeof (DevMode));
	DevMode.dmSize   = sizeof (DevMode);
	DevMode.dmFields = DM_DISPLAYORIENTATION;

	if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL))
	{
		CurrentAngle = DevMode.dmDisplayOrientation;
		RETAILMSG(1, (L"ChangeDisplaySettingsEx reports the current rotation as %d", CurrentAngle));
	}
	else
	{ 
		RETAILMSG(1, (L"ChangeDisplaySettingsEx failed to get the current rotation angle."));
		CurrentAngle = -1;
	}

	//
	// Rotate to the "next" angle.
	//
	if (CurrentAngle != -1) {
		if (CurrentAngle == DMDO_0)

			NewAngle = DMDO_90;
		else if (CurrentAngle == DMDO_90)
			NewAngle = DMDO_180;
		else if (CurrentAngle == DMDO_180)
			NewAngle = DMDO_270;
		else if (CurrentAngle == DMDO_270)
			NewAngle = DMDO_0;

		memset(&DevMode, 0, sizeof (DevMode));
		DevMode.dmSize               = sizeof (DevMode);
		DevMode.dmFields             = DM_DISPLAYORIENTATION;
		DevMode.dmDisplayOrientation = NewAngle;

		if (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_RESET, NULL))
		{
			RETAILMSG(1, (L"ChangeDisplaySettingsEx changed rotation angle to %d", NewAngle));
		}
		else
		{
			RETAILMSG(1, (L"ChangeDisplaySettingsEx failed to change the rotation angle to %d", NewAngle));
		}
	}

	return 0;
}
