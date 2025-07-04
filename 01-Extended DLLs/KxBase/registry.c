#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI LSTATUS WINAPI RegOpenKeyExA_Ext(
	IN HKEY hKey,
	IN OPTIONAL LPCSTR lpSubKey,
	IN DWORD ulOptions,
	IN REGSAM samDesired,
	OUT PHKEY phkResult)
{
	LSTATUS Status = RegOpenKeyExA(hKey, lpSubKey, ulOptions, samDesired, phkResult);

	if(StrStrA(lpSubKey, "VxKex")) {
		return ERROR_FILE_NOT_FOUND;
	}

	return Status;
}

KXBASEAPI LSTATUS WINAPI RegOpenKeyExW_Ext(
	IN HKEY hKey,
	IN OPTIONAL LPCWSTR lpSubKey,
	IN DWORD ulOptions,
	IN REGSAM samDesired,
	OUT PHKEY phkResult)
{
	LSTATUS Status = RegOpenKeyExW(hKey, lpSubKey, ulOptions, samDesired, phkResult);

	if(StrStrW(lpSubKey, L"VxKex")) {
		return ERROR_FILE_NOT_FOUND;
	}

	return Status;
}

KXBASEAPI LSTATUS WINAPI RegEnumKeyExA_Ext(
	IN HKEY hKey,
	IN DWORD dwIndex,
	OUT LPSTR lpName,
	IN OUT LPDWORD lpcchName,
	LPDWORD lpReserved,
	IN OUT LPSTR lpClass,
	IN OUT OPTIONAL LPDWORD lpcchClass,
	OUT OPTIONAL PFILETIME lpftLastWriteTime)
{
	LSTATUS Status = RegEnumKeyExA(hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);

	if(StrStrA(lpName, "VxKex")) {
		return ERROR_NO_MORE_ITEMS;
	}

	return Status;
}

KXBASEAPI LSTATUS WINAPI RegEnumKeyExW_Ext(
	IN HKEY hKey,
	IN DWORD dwIndex,
	OUT LPWSTR lpName,
	IN OUT LPDWORD lpcchName,
	LPDWORD lpReserved,
	IN OUT LPWSTR lpClass,
	IN OUT OPTIONAL LPDWORD lpcchClass,
	OUT OPTIONAL PFILETIME lpftLastWriteTime)
{
	LSTATUS Status = RegEnumKeyExW(hKey, dwIndex, lpName, lpcchName, lpReserved, lpClass, lpcchClass, lpftLastWriteTime);

	if(StrStrW(lpName, L"VxKex")) {
		return ERROR_NO_MORE_ITEMS;
	}

	return Status;
}