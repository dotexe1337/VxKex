#define NEED_VERSION_DEFS
#include "kexsetup.h"

BOOLEAN Is64BitOS;
KEXSETUP_OPERATION_MODE OperationMode;
BOOLEAN SilentUnattended;
BOOLEAN PreserveConfig;
WCHAR KexDir[MAX_PATH];
ULONG ExistingVxKexVersion;
ULONG InstallerVxKexVersion;

VOID EntryPoint(
	VOID)
{
	KexgApplicationFriendlyName = FRIENDLYAPPNAME;
	ExistingVxKexVersion = KexSetupGetInstalledVersion();
	InstallerVxKexVersion = KEX_VERSION_DW;
	
	Is64BitOS = IsWow64();
	GetDefaultInstallationLocation(KexDir);
	ProcessCommandLineOptions();

	if (SilentUnattended) {
		KexSetupPerformActions();
	} else {
		DisplayInstallerGUI();
	}

	ExitProcess(STATUS_SUCCESS);
}