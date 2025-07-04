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

KXBASEAPI BOOL WINAPI InitializeCriticalSectionEx_Ext(
    OUT LPCRITICAL_SECTION lpCriticalSection,
    IN DWORD dwSpinCount,
    IN DWORD flags)
{
    NTSTATUS Status;

    //
    // Implemented here due to issues with Windows 7 and this function
    // Will return ERROR_INVALID_PARAMETER if 7 cannot figure out what to return
    // Fixes a lot of Chromium-based stuff and Roblox Studio team test for some reason
    //

    Status = RtlInitializeCriticalSectionAndSpinCount(
        (PRTL_CRITICAL_SECTION)lpCriticalSection,
        dwSpinCount);

    if (!NT_SUCCESS(Status)){
        SetLastError(RtlNtStatusToDosError(Status));
        return FALSE;
    }

    return TRUE;
}