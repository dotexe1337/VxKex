#pragma once

#include <KexComm.h>
#include <KexDll.h>
#include <KxDx.h>

EXTERN PKEX_PROCESS_DATA KexData;

// roblox studio patch part 1
typedef interface IMFMediaBuffer IMFMediaBuffer; // NOTE: this is a shit fix. PLEASE do the real api in the future
typedef interface IMFDXGIDeviceManager IMFDXGIDeviceManager;