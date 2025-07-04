#include "buildcfg.h"
#include "kxcomp.h"
#include <ShObjIdl.h>

//
// This CoCreateInstance hook will work for most applications.
// However, be aware that there are many ways of creating an instance
// of a COM object which do not go through this function. For example,
// CoCreateInstanceEx, CoCreateInstanceFromApp, CoGetClassObject, etc.
//

KXCOMAPI HRESULT WINAPI Ext_CoCreateInstance(
	IN	REFCLSID	RefCLSID,
	IN	LPUNKNOWN	OuterUnknown,
	IN	ULONG		ClassContext,
	IN	REFIID		RefIID,
	OUT	PPVOID		Instance)
{
	HRESULT Result;
	SOleTlsData *OleTlsData;

	//
	// Workaround buggy applications which don't call CoInitialize(Ex) before
	// calling CoCreateInstance. That causes a crash, which sometimes does
	// not appear on Win10/11/etc. for unknown reasons and therefore does not
	// get fixed by application developers.
	//
	// Hall of Shame for apps that need this "fix":
	//  - Firefox and forks
	//  - Thunderbird and forks
	//

	OleTlsData = (SOleTlsData *) NtCurrentTeb()->ReservedForOle;

	if (!OleTlsData) {
		Result = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
		OleTlsData = (SOleTlsData *) NtCurrentTeb()->ReservedForOle;

		ASSERT (SUCCEEDED(Result));
		ASSERT (OleTlsData != NULL);

		KexLogInformationEvent(
			L"Called CoInitializeEx to work around buggy application\r\n\r\n"
			L"CoInitializeEx returned HRESULT: 0x%08lx\r\n"
			L"ReservedForOle = 0x%p",
			Result, OleTlsData);
	}

	Result = CoCreateInstance(
		RefCLSID,
		OuterUnknown,
		ClassContext,
		RefIID,
		Instance);

	if (KexIsDebugBuild) {
		LPOLESTR RefCLSIDAsString;
		LPOLESTR RefIIDAsString;

		StringFromCLSID(RefCLSID, &RefCLSIDAsString);
		StringFromIID(RefIID, &RefIIDAsString);

		KexLogEvent(
			SUCCEEDED(Result) ? LogSeverityDebug : LogSeverityWarning,
			L"%s (%s, %s)\r\n\r\n"
			L"HRESULT error code: 0x%08lx\r\n"
			L"OuterUnknown: %p\r\n"
			L"ClassContext: %lu\r\n"
			L"Instance: %p",
			SUCCEEDED(Result) ? L"Created COM object" : L"Failure to create COM object",
			RefCLSIDAsString, RefIIDAsString,
			Result,
			OuterUnknown,
			ClassContext,
			Instance);

		CoTaskMemFree(RefCLSIDAsString);
		CoTaskMemFree(RefIIDAsString);
	}

	return Result;
}