#include "buildcfg.h"
#include "kexdllp.h"

STATIC BYTE BasicHookTemplate[BASIC_HOOK_LENGTH] = {
#ifdef KEX_ARCH_X64
	0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,				// JMP [FuncPtr]
	0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC	// FuncPtr: DQ 0xCCCCCCCCCCCCCCCC
#else
	0x68, 0xCC, 0xCC, 0xCC, 0xCC,					// PUSH 0xCCCCCCCC
	0xC3											// RET
#endif
};

//
// Install a hook on a given API routine.
//
//   ApiAddress
//     Address of a function that you want to hook.
//
//   RedirectedAddress
//     Address of your function with an identical call signature that you
//     want to get called instead of the hooked function.
//
//   HookContext
//     Optional parameter that allows you to undo the hook later, e.g. if
//     you want to uninstall the hook or if you want to call the original API.
//     If this parameter is NULL, the hook is permanent.
//
KEXAPI NTSTATUS NTAPI KexHkInstallBasicHook(
	IN		PVOID					ApiAddress,
	IN		PVOID					RedirectedAddress,
	OUT		PKEX_BASIC_HOOK_CONTEXT	HookContext OPTIONAL)
{
	NTSTATUS Status;
	PVOID ApiPageAddress;
	SIZE_T HookLength;
	ULONG OldProtect;

	ASSERT (ApiAddress != NULL);
	ASSERT (RedirectedAddress != NULL);

	if (!ApiAddress) {
		return STATUS_INVALID_PARAMETER_1;
	}
	
	if (!RedirectedAddress) {
		return STATUS_INVALID_PARAMETER_2;
	}

	ApiPageAddress = ApiAddress;
	HookLength = sizeof(BasicHookTemplate);
	
	//
	// Fill out hook context fields, copy the original bytes (so
	// we can undo the hook later), and prepare the hook template for
	// writing.
	//

	if (HookContext) {
		HookContext->OriginalApiAddress = ApiAddress;
		
		KexRtlCopyMemory(
			HookContext->OriginalInstructions,
			ApiAddress,
			sizeof(BasicHookTemplate));
	}

	*(PPVOID) (&BasicHookTemplate[BASIC_HOOK_DESTINATION_OFFSET]) = RedirectedAddress;
	
	//
	// Let us write to the address of the hooked API.
	// We use the KexNt* private syscall stub because otherwise we risk changing
	// memory protection on the NtProtectVirtualMemory stub itself, and therefore
	// causing recursive access violation exceptions upon return from the system
	// call. (This actually happened, not just theoretical.)
	//

	Status = KexNtProtectVirtualMemory(
		NtCurrentProcess(),
		&ApiPageAddress,
		&HookLength,
		PAGE_READWRITE,
		&OldProtect);

	ASSERT (NT_SUCCESS(Status));

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	//
	// Hook the function and restore old page protections.
	//

	KexRtlCopyMemory(ApiAddress, BasicHookTemplate, sizeof(BasicHookTemplate));

	Status = KexNtProtectVirtualMemory(
		NtCurrentProcess(),
		&ApiPageAddress,
		&HookLength,
		OldProtect,
		&OldProtect);

	ASSERT (NT_SUCCESS(Status));

	return STATUS_SUCCESS;
}

KEXAPI NTSTATUS NTAPI KexHkRemoveBasicHook(
	IN		PKEX_BASIC_HOOK_CONTEXT	HookContext)
{
	NTSTATUS Status;
	PVOID ApiPageAddress;
	SIZE_T HookLength;
	ULONG OldProtect;

	ASSERT (HookContext != NULL);

	if (!HookContext) {
		return STATUS_INVALID_PARAMETER_1;
	}

	ApiPageAddress = HookContext->OriginalApiAddress;
	HookLength = sizeof(BasicHookTemplate);

	Status = NtProtectVirtualMemory(
		NtCurrentProcess(),
		&ApiPageAddress,
		&HookLength,
		PAGE_READWRITE,
		&OldProtect);

	ASSERT (NT_SUCCESS(Status));

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	RtlCopyMemory(
		HookContext->OriginalApiAddress,
		HookContext->OriginalInstructions,
		sizeof(BasicHookTemplate));

	Status = NtProtectVirtualMemory(
		NtCurrentProcess(),
		&ApiPageAddress,
		&HookLength,
		OldProtect,
		&OldProtect);

	ASSERT (NT_SUCCESS(Status));

	return STATUS_SUCCESS;
}