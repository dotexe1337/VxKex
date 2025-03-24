#include "buildcfg.h"
#include "kexdllp.h"

STATIC INLINE NTSTATUS KexpShrinkDllPathLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			TargetLength);

STATIC INLINE NTSTATUS KexpPadDllPathToOriginalLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			OriginalLength);

STATIC INLINE VOID KexpNormalizeDllPathBackslashes(
	IN OUT	PUNICODE_STRING	DllPath);

NTSTATUS KexpAddKex3264ToDllPath(
	VOID)
{
	NTSTATUS Status;
	PUNICODE_STRING DllPath;
	UNICODE_STRING NewDllPath;
	USHORT DllPathOriginalLength;
	ULONG Index;
	
	ASSERT (VALID_UNICODE_STRING(&KexData->Kex3264DirPath));
	ASSERT (KexData->Kex3264DirPath.Length != 0);

	DllPath = &NtCurrentPeb()->ProcessParameters->DllPath;
	DllPathOriginalLength = DllPath->Length;

	KexLogInformationEvent(
		L"Shrinking default loader DLL path\r\n\r\n"
		L"The original DLL path is: \"%wZ\"",
		DllPath);

	//
	// Convert all forward slashes in the DllPath to backslashes.
	// At least one real world case has been observed where a user's computer
	// had the Path environment variable contain forward slashes instead of
	// backslashes for some reason.
	//
	// Without normalizing the path separators, KexpShrinkDllPathLength would
	// fail. This would cause VxKex to not work.
	//

	KexpNormalizeDllPathBackslashes(DllPath);

	//
	// Call a helper function to shrink DllPath by *at least* the length
	// of our prepend string.
	//

	Status = KexpShrinkDllPathLength(DllPath, DllPath->Length - KexData->Kex3264DirPath.Length);
	ASSERT (NT_SUCCESS(Status));
	ASSERT (DllPath->Length < DllPathOriginalLength);
	ASSERT (DllPath->Length <= DllPathOriginalLength - KexData->Kex3264DirPath.Length);

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	//
	// Create a temporary buffer to hold the new DllPath.
	//

	NewDllPath.Length = 0;
	NewDllPath.MaximumLength = DllPath->Length + KexData->Kex3264DirPath.Length;
	NewDllPath.Buffer = StackAlloc(WCHAR, KexRtlUnicodeStringBufferCch(&NewDllPath));

	//
	// Build the new DllPath.
	//

	Status = RtlAppendUnicodeStringToString(&NewDllPath, &KexData->Kex3264DirPath);
	ASSERT (NT_SUCCESS(Status));

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	Status = RtlAppendUnicodeStringToString(&NewDllPath, DllPath);
	ASSERT (NT_SUCCESS(Status));

	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	//
	// Copy new DllPath to old DllPath and finally pad to original length.
	//

	RtlCopyUnicodeString(DllPath, &NewDllPath);
	
	Status = KexpPadDllPathToOriginalLength(DllPath, DllPathOriginalLength);
	//ASSERT (NT_SUCCESS(Status));

	//
	// Remove any embedded nulls in the path. Sometimes the system or another
	// application can add them and it causes a problem with logging.
	//

	for (Index = 0; Index < KexRtlUnicodeStringCch(DllPath); ++Index) {
		if (DllPath->Buffer[Index] == '\0') {
			DllPath->Buffer[Index] = ';';
		}
	}

	return STATUS_SUCCESS;
}

//
// Try to shrink the DllPath length to equal or less than "Length" by
// removing duplicate entries.
//
STATIC INLINE NTSTATUS KexpShrinkDllPathLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			TargetLength)
{
	NTSTATUS Status;
	UNICODE_STRING DllPathAfterCurrentEntry;
	UNICODE_STRING CurrentPathEntry;
	UNICODE_STRING DuplicatePathEntry;
	UNICODE_STRING Semicolon;

	RtlInitConstantUnicodeString(&Semicolon, L";");
	DllPathAfterCurrentEntry = *DllPath;

	until (DllPath->Length <= TargetLength) {

		//
		// Fetch a path entry
		//

		Status = RtlFindCharInUnicodeString(
			0,
			&DllPathAfterCurrentEntry,
			&Semicolon,
			&CurrentPathEntry.Length);

		if (!NT_SUCCESS(Status)) {
			KexLogErrorEvent(
				L"RtlFindCharInUnicodeString returned an error\r\n\r\n"
				L"NTSTATUS error code: %s",
				KexRtlNtStatusToString(Status));
			return Status;
		}

		CurrentPathEntry.Length -= sizeof(WCHAR); // it includes the semicolon - get rid of it
		CurrentPathEntry.Buffer = DllPathAfterCurrentEntry.Buffer;
		CurrentPathEntry.MaximumLength = CurrentPathEntry.Length;
		KexRtlAdvanceUnicodeString(&DllPathAfterCurrentEntry, CurrentPathEntry.Length + sizeof(WCHAR));

		//
		// Look for one or more duplicate entries later in the path.
		//

		while (TRUE) {
			UNICODE_STRING AfterDuplicate;
			UNICODE_STRING StringToSearchFor;

			RtlInitEmptyUnicodeStringFromTeb(&StringToSearchFor);
			RtlAppendUnicodeStringToString(&StringToSearchFor, &Semicolon);
			RtlAppendUnicodeStringToString(&StringToSearchFor, &CurrentPathEntry);
			RtlAppendUnicodeStringToString(&StringToSearchFor, &Semicolon);

			DuplicatePathEntry.Buffer = KexRtlFindUnicodeSubstring(
				&DllPathAfterCurrentEntry,
				&StringToSearchFor,
				TRUE);

			if (!DuplicatePathEntry.Buffer) {
				break;
			}

			DuplicatePathEntry.Buffer += 1;
			DuplicatePathEntry.Length = CurrentPathEntry.Length;
			DuplicatePathEntry.MaximumLength = (USHORT)
				((KexRtlEndOfUnicodeString(DllPath) - DuplicatePathEntry.Buffer) * sizeof(WCHAR));

			//
			// We need to cut this path entry out of the original DllPath and update the
			// length field accordingly. To do this, we will copy all characters from
			// the end of the duplicate path entry over top of the beginning of the
			// duplicate entry.
			//

			AfterDuplicate.Buffer = KexRtlEndOfUnicodeString(&DuplicatePathEntry);
			AfterDuplicate.Length = (USHORT) ((KexRtlEndOfUnicodeString(DllPath) - AfterDuplicate.Buffer) * sizeof(WCHAR));
			AfterDuplicate.MaximumLength = AfterDuplicate.Length;

			// skip over the next semicolon
			ASSERT (AfterDuplicate.Length != 0);
			ASSERT (AfterDuplicate.Buffer[0] == ';');
			KexRtlAdvanceUnicodeString(&AfterDuplicate, sizeof(WCHAR));

			DllPath->Length -= DuplicatePathEntry.Length;
			RtlCopyUnicodeString(&DuplicatePathEntry, &AfterDuplicate);
		}
	}

	return STATUS_SUCCESS;
}

STATIC INLINE NTSTATUS KexpPadDllPathToOriginalLength(
	IN	PUNICODE_STRING	DllPath,
	IN	USHORT			OriginalLength)
{
	PWCHAR Pointer;

	if (DllPath->Length > OriginalLength) {
		return STATUS_INTERNAL_ERROR;
	}

	//
	// Add semicolons to the end until DllPath reaches the correct length.
	//

	Pointer = KexRtlEndOfUnicodeString(DllPath);
	DllPath->Length = OriginalLength;

	while (Pointer < KexRtlEndOfUnicodeString(DllPath)) {
		*Pointer++ = ';';
	}

	return STATUS_SUCCESS;
}

//
// Convert all slashes in the specified DllPath to backslashes.
//
STATIC INLINE VOID KexpNormalizeDllPathBackslashes(
	IN OUT	PUNICODE_STRING	DllPath)
{
	ULONG Index;
	ULONG DllPathCch;

	ASSUME (VALID_UNICODE_STRING(DllPath));

	DllPathCch = KexRtlUnicodeStringCch(DllPath);
	ASSUME (DllPathCch > 0);

	for (Index = 0; Index < DllPathCch; ++Index) {
		if (DllPath->Buffer[Index] == '/') {
			DllPath->Buffer[Index] = '\\';
		}
	}
}