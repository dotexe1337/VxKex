#include "buildcfg.h"
#include "kxbasep.h"

KXBASEAPI ULONG WINAPI PssCaptureSnapshot(
	IN	HANDLE				ProcessHandle,
	IN	PSS_CAPTURE_FLAGS	CaptureFlags,
	IN	ULONG				ThreadContextFlags OPTIONAL,
	OUT	PHPSS				SnapshotHandle)
{
	KexLogWarningEvent(L"Stub API called: PssCaptureSnapshot");
	KexDebugCheckpoint();
	return ERROR_NOT_SUPPORTED;
}

KXBASEAPI ULONG WINAPI PssFreeSnapshot(
	IN	HANDLE				ProcessHandle,
	IN	HPSS				SnapshotHandle)
{
	KexLogWarningEvent(L"Stub API called: PssFreeSnapshot");
	KexDebugCheckpoint();
	return ERROR_NOT_SUPPORTED;
}

KXBASEAPI ULONG WINAPI PssQuerySnapshot(
	IN	HPSS						SnapshotHandle,
	IN	PSS_QUERY_INFORMATION_CLASS	InformationClass,
	OUT	PVOID						Buffer,
	IN	ULONG						BufferLength)
{
	KexLogWarningEvent(L"Stub API called: PssQuerySnapshot");
	KexDebugCheckpoint();
	return ERROR_NOT_SUPPORTED;
}

KXBASEAPI ULONG WINAPI PssWalkMarkerCreate(
	IN	PCPSS_ALLOCATOR	Allocator,
	OUT	HPSSWALK		WalkMarkerHandle)
{
	KexLogWarningEvent(L"Stub API called: PssWalkMarkerCreate");
	KexDebugCheckpoint();
	return ERROR_NOT_SUPPORTED;
}