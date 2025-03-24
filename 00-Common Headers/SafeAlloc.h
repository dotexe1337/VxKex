#pragma once
#include <NtDll.h>
#include <KexTypes.h>

#define SafeAllocEx(Heap, Flags, Type, NumberOfElements) \
	((Type *) RtlAllocateHeap((Heap), (Flags), sizeof(Type) * (NumberOfElements)))

#define SafeReAllocEx(Heap, Flags, OriginalPointer, Type, NumberOfElements) \
	((Type *) RtlReAllocateHeap((Heap), (Flags), (OriginalPointer), sizeof(Type) * (NumberOfElements)))

#define SafeFreeEx(Heap, Flags, Pointer) \
	do { RtlFreeHeap(Heap, Flags, (Pointer)); (Pointer) = NULL; } while (0)

#define SafeAlloc(Type, NumberOfElements) \
	SafeAllocEx(RtlProcessHeap(), 0, Type, (NumberOfElements))

#define SafeReAlloc(OriginalPointer, Type, NumberOfElements) \
	SafeReAllocEx(RtlProcessHeap(), 0, (OriginalPointer), Type, (NumberOfElements))

#define SafeFree(Pointer) \
	SafeFreeEx(RtlProcessHeap(), 0, (Pointer))

#define SafeAllocSeh(Type, NumberOfElements) \
	SafeAllocEx(RtlProcessHeap(), HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, Type, (NumberOfElements))

#define SafeReAllocSeh(OriginalPointer, Type, NumberOfElements) \
	SafeReAllocEx(RtlProcessHeap(), HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, (OriginalPointer), Type, (NumberOfElements))

#define SafeFreeSeh(Pointer) \
	SafeFreeEx(RtlProcessHeap(), HEAP_GENERATE_EXCEPTIONS, (Pointer))

//
// _alloca() is a compiler intrinsic.
//

#ifndef __cplusplus
PVOID CDECL _alloca(
	IN	SIZE_T NumberOfBytes);
#endif

#define StackAlloc(Type, NumberOfElements) ((Type *) _alloca(sizeof(Type) * (NumberOfElements)))

//
// SafeClose is for handles.
//

#define SafeClose(Handle) do { if (Handle) { NTSTATUS SafeCloseStatus = NtClose(Handle); ASSERT (NT_SUCCESS(SafeCloseStatus)); (Handle) = NULL; } } while(0)

//
// SafeRelease is for COM interfaces.
//

#define SafeRelease(Interface) do { if (Interface) { (Interface)->lpVtbl->Release(Interface); (Interface) = NULL; } } while (0)