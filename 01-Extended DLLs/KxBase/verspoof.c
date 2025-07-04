#include "buildcfg.h"
#include "kxbasep.h"
#include <KexStrSafe.h>

//
// Iertutil.dll contains a version check which will end up causing errors (failure
// to open files with ShellExecute including with shell context menus) if the reported
// OS version is higher than 6.1.
//
// This function un-spoofs the OS version for Windows DLLs, and preserves normal behavior
// for all other situations.
//

KXBASEAPI BOOL WINAPI Ext_GetVersionExA(
	IN OUT	POSVERSIONINFOA	VersionInfo)
{
	BOOL Success;

	Success = GetVersionExA(VersionInfo);

	if (Success && AshModuleIsWindowsModule(ReturnAddress())) {
		VersionInfo->dwMajorVersion = 6;
		VersionInfo->dwMinorVersion = 1;
		VersionInfo->dwBuildNumber = 7601;
	}

	if(Success && (AshExeBaseNameIs(L"RobloxPlayerBeta.exe") || AshModuleBaseNameIs(ReturnAddress(), L"RobloxPlayerBeta.dll")))
	{
		VersionInfo->dwMajorVersion = 10;
		VersionInfo->dwMinorVersion = 0;
		VersionInfo->dwBuildNumber = 19044;
	}

	return Success;
}

KXBASEAPI BOOL WINAPI Ext_VerifyVersionInfoW(
	IN LPOSVERSIONINFOEXW lpVersionInformation,
	IN DWORD              dwTypeMask,
	IN DWORDLONG          dwlConditionMask
) {
	if(AshExeBaseNameIs(L"RobloxStudioBeta.exe"))
	{
		return TRUE;
	}

	return VerifyVersionInfoW(lpVersionInformation, dwTypeMask, dwlConditionMask);
}