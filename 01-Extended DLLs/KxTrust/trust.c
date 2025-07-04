#include "buildcfg.h"
#include "kxtrustp.h"
#include <SoftPub.h>

KXTRUSTAPI LONG WINAPI WinVerifyTrust_Ext(IN HWND hwnd, IN GUID *pgActionID, IN LPVOID pWVTData) {
	GUID guid = WINTRUST_ACTION_GENERIC_VERIFY_V2;

    if (pgActionID && !memcmp(pgActionID, &guid, sizeof(GUID))) {
        return ERROR_SUCCESS;
    }

    return WinVerifyTrust(hwnd, pgActionID, pWVTData);
}