#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI BOOL WINAPI Ext_DuplicateHandle(
	IN	HANDLE		SourceProcessHandle,
	IN	HANDLE		SourceHandle,
	IN	HANDLE		TargetProcessHandle,
	OUT	PHANDLE		TargetHandle,
	IN	ACCESS_MASK	DesiredAccess,
	IN	BOOL		Inherit,
	IN	ULONG		Options)
{
	return DuplicateHandle(
		SourceProcessHandle,
		SourceHandle,
		TargetProcessHandle,
		TargetHandle,
		DesiredAccess,
		Inherit,
		Options);
}