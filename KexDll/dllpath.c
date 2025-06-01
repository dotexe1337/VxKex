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
	UNICODE_STRING Semicolon;
	UNICODE_STRING DllPathAfterCurrentEntry;

	RtlInitConstantUnicodeString(&Semicolon, L";");

	//
	// Append a semicolon to DllPath to simplify the code which removes duplicate
	// path elements. This is always possible because of the null terminator at
	// the end of the DllPath string.
	//
	// In order to maintain compatibility with any badly written code elsewhere
	// which depends on this null terminator, we will have to remember to add the
	// null terminator back on.
	//

	Status = RtlAppendUnicodeStringToString(DllPath, &Semicolon);
	ASSERT (NT_SUCCESS(Status));

	DllPathAfterCurrentEntry = *DllPath;

	until (DllPath->Length <= TargetLength) {
		UNICODE_STRING CurrentEntry;
		UNICODE_STRING DllPathAfterDuplicateEntry;

		//
		// Fetch a path entry.
		//

		while (RtlPrefixUnicodeString(&Semicolon, &DllPathAfterCurrentEntry, FALSE)) {
			// Skip past semicolons.
			KexRtlAdvanceUnicodeString(&DllPathAfterCurrentEntry, sizeof(WCHAR));
		}

		Status = RtlFindCharInUnicodeString(
			0,
			&DllPathAfterCurrentEntry,
			&Semicolon,
			&CurrentEntry.Length);

		if (!NT_SUCCESS(Status)) {
			return Status;
		}

		// Set up CurrentPathEntry so that it contains a single path entry with no
		// leading or trailing semicolons.
		CurrentEntry.Buffer = DllPathAfterCurrentEntry.Buffer;
		CurrentEntry.MaximumLength = CurrentEntry.Length;
		CurrentEntry.Length -= sizeof(WCHAR); // it includes the semicolon - get rid of it

		// Set up DllPathAfterCurrentEntry so that it begins right after CurrentPathEntry.
		KexRtlAdvanceUnicodeString(&DllPathAfterCurrentEntry, CurrentEntry.Length);
		ASSERT (DllPathAfterCurrentEntry.Buffer == KexRtlEndOfUnicodeString(&CurrentEntry));

		if (KexRtlUnicodeStringCch(&CurrentEntry) == 0) {
			// Empty path entry. Skip past it so that the code to remove duplicates
			// doesn't have to handle empty path entries.
			continue;
		}

		//
		// Find duplicates of the current path entry and remove them.
		//

		while (TRUE) {
			UNICODE_STRING StringToSearchFor;
			UNICODE_STRING DuplicateEntry;
			USHORT CchRemoved;

			//
			// Find another instance of CurrentEntry. We want the path entry to be
			// complete, so we will search for the current entry both preceded and
			// followed by a semicolon.
			//

			RtlInitEmptyUnicodeStringFromTeb(&StringToSearchFor);

			Status = STATUS_SUCCESS;

			Status |= RtlAppendUnicodeStringToString(&StringToSearchFor, &Semicolon);
			ASSERT (NT_SUCCESS(Status));
			
			Status |= RtlAppendUnicodeStringToString(&StringToSearchFor, &CurrentEntry);
			ASSERT (NT_SUCCESS(Status));

			Status |= RtlAppendUnicodeStringToString(&StringToSearchFor, &Semicolon);
			ASSERT (NT_SUCCESS(Status));

			if (Status != STATUS_SUCCESS) {
				// If any of the three appends failed (e.g. because CurrentEntry is a
				// super long string introduced through an environment variable or
				// something) then just skip this.
				break;
			}

			DuplicateEntry.Buffer = KexRtlFindUnicodeSubstring(
				&DllPathAfterCurrentEntry,
				&StringToSearchFor,
				TRUE);

			if (DuplicateEntry.Buffer == NULL) {
				break;
			}

			//
			// Set up Length and MaximumLength.
			//

			DuplicateEntry.Length = StringToSearchFor.Length - sizeof(WCHAR);
			DuplicateEntry.MaximumLength = StringToSearchFor.Length;

			Status = KexRtlSetUnicodeStringBufferEnd(
				&DuplicateEntry,
				KexRtlEndOfUnicodeString(DllPath));

			ASSERT (NT_SUCCESS(Status));
			ASSERT (!KexRtlUnicodeStringEndsWith(&DuplicateEntry, &Semicolon, FALSE));
			ASSERT (DuplicateEntry.Buffer != CurrentEntry.Buffer);
			ASSERT (KexRtlEndOfUnicodeStringBuffer(&DuplicateEntry) == KexRtlEndOfUnicodeString(DllPath));

			//
			// Set up DllPathAfterDuplicateEntry.6
			//

			DllPathAfterDuplicateEntry = DuplicateEntry;
			DllPathAfterDuplicateEntry.Length = DllPathAfterDuplicateEntry.MaximumLength;
			KexRtlAdvanceUnicodeString(&DllPathAfterDuplicateEntry, DuplicateEntry.Length);

			ASSERT (DllPathAfterDuplicateEntry.Buffer == KexRtlEndOfUnicodeString(&DuplicateEntry));

			//
			// Copy DllPathAfterDuplicateEntry back on top of DuplicateEntry. This reduces
			// the length of the DLL path, so we will first record how many characters we
			// have shortened the DllPath string by so we can appropriately update length
			// values.
			//

			CchRemoved = KexRtlUnicodeStringCch(&DuplicateEntry);

			RtlCopyUnicodeString(&DuplicateEntry, &DllPathAfterDuplicateEntry);
			ASSERT (DuplicateEntry.Length == DllPathAfterDuplicateEntry.Length);

			DllPath->Length -= CchRemoved * sizeof(WCHAR);
			DllPathAfterCurrentEntry.Length -= CchRemoved * sizeof(WCHAR);

			// Remember that RtlCopyUnicodeString will null terminate the resulting buffer.
			// This is just a quick debug check to make sure the length calculation wasn't
			// fucked up somehow.
			ASSERT (wcslen(DllPath->Buffer) == KexRtlUnicodeStringCch(DllPath));
		}
	}

	return Status;
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