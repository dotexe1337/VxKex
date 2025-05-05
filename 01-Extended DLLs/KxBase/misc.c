#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI BOOL WINAPI GetOsSafeBootMode(
	OUT	PBOOL	IsSafeBootMode)
{
	*IsSafeBootMode = FALSE;
	return TRUE;
}

KXBASEAPI BOOL WINAPI GetFirmwareType(
	OUT	PFIRMWARE_TYPE	FirmwareType)
{
	*FirmwareType = FirmwareTypeUnknown;
	return TRUE;
}

// roblox studio patch part 2: APPHACK this
// credit: murdle cuz he deshittified my apphack which we wont get into how it was before
KXBASEAPI BOOL WINAPI KexVerifyVersionInfoW(
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
