#include "buildcfg.h"
#include <KexComm.h>
#include <KxCfgHlp.h>

//
// Open the HKLM or HKCU VxKex key.
// This function does not allow WOW64 redirection.
// Returns NULL on failure. Call GetLastError for more information.
//
KXCFGDECLSPEC HKEY KXCFGAPI KxCfgOpenVxKexRegistryKey(
	IN	BOOLEAN		PerUserKey,
	IN	ACCESS_MASK	DesiredAccess,
	IN	HANDLE		TransactionHandle OPTIONAL)
{
	return KxCfgpOpenKey(
		PerUserKey ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
		L"Software\\VXsoft\\VxKex",
		DesiredAccess,
		TransactionHandle);
}

//
// Open the legacy, pre-rewrite VxKex key.
// This function does not allow WOW64 redirection.
// Returns NULL on failure. Call GetLastError for more information.
//
KXCFGDECLSPEC HKEY KXCFGAPI KxCfgOpenLegacyVxKexRegistryKey(
	IN	BOOLEAN		PerUserKey,
	IN	ACCESS_MASK	DesiredAccess,
	IN	HANDLE		TransactionHandle OPTIONAL)
{
	return KxCfgpOpenKey(
		PerUserKey ? HKEY_CURRENT_USER : HKEY_LOCAL_MACHINE,
		L"Software\\VXsoft\\VxKexLdr",
		DesiredAccess,
		TransactionHandle);
}