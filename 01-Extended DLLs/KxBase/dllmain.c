#include "buildcfg.h"
#include "kxbasep.h"
#include <KexW32ML.h>

PKEX_PROCESS_DATA KexData = NULL;

BOOL WINAPI DllMain(
	IN	PVOID		DllBase,
	IN	ULONG		Reason,
	IN	PCONTEXT	Context)
{
	if (Reason == DLL_PROCESS_ATTACH) {
		LdrDisableThreadCalloutsForDll(DllBase);

		KexDataInitialize(&KexData);
		KexLogDebugEvent(L"DllMain called with DLL_PROCESS_ATTACH");

		//
		// Get the base address of Kernel32 and store it in KexData.
		//

		BaseGetBaseDllHandle();
		ASSERT (KexData->BaseDllBase != NULL);

		//
		// If we are doing SharedUserData-based version spoofing, we need to
		// hook GetSystemTime and GetSystemTimeAsFileTime.
		//

		if (KexData->IfeoParameters.StrongVersionSpoof & KEX_STRONGSPOOF_SHAREDUSERDATA) {
			KexHkInstallBasicHook(GetSystemTime, KxBasepGetSystemTimeHook, NULL);
			KexHkInstallBasicHook(GetSystemTimeAsFileTime, KxBasepGetSystemTimeAsFileTimeHook, NULL);
		}

		//
		// Patch subsystem version check inside CreateProcessInternalW.
		//

		KexPatchCpiwSubsystemVersionCheck();

		//
		// Get base named object directories and put handles to them in KexData.
		//

		KexData->BaseNamedObjects = BaseGetNamedObjectDirectory();
		KexData->UntrustedNamedObjects = BaseGetUntrustedNamedObjectDirectory();
	}

	return TRUE;
}