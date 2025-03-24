#include "buildcfg.h"
#include <KexComm.h>
#include <KxCfgHlp.h>
#include <KexW32ML.h>

KXCFGDECLSPEC BOOLEAN KXCFGAPI KxCfgGetKexDir(
	OUT	PWSTR	Buffer,
	IN	ULONG	BufferCch)
{
	HKEY VxKexKeyHandle;
	ULONG ErrorCode;

	ASSERT (Buffer != NULL);
	ASSERT (BufferCch != 0);

	Buffer[0] = '\0';

	VxKexKeyHandle = KxCfgOpenVxKexRegistryKey(
		FALSE,
		KEY_READ,
		NULL);

	if (!VxKexKeyHandle) {
		VxKexKeyHandle = KxCfgOpenLegacyVxKexRegistryKey(
			FALSE,
			KEY_READ,
			NULL);

		if (!VxKexKeyHandle) {
			return FALSE;
		}
	}

	ErrorCode = RegReadString(VxKexKeyHandle, NULL, L"KexDir", Buffer, BufferCch);
	RegCloseKey(VxKexKeyHandle);

	if (ErrorCode != ERROR_SUCCESS) {
		SetLastError(ErrorCode);
		return FALSE;
	}

	ASSERT (!PathIsRelative(Buffer));
	if (PathIsRelative(Buffer)) {
		SetLastError(ERROR_INVALID_DATA);
		return FALSE;
	}

	return TRUE;
}