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

// roblox studio patch part 2: redifine what this function does, this patch may help with other programs, not just roblox studio.
KXBASEAPI BOOL WINAPI VerifyVersionInfoRedifined(
	IN LPOSVERSIONINFOEXW lpVersionInformation,
	IN DWORD              dwTypeMask,
	IN DWORDLONG          dwlConditionMask
) {
	return TRUE;
}