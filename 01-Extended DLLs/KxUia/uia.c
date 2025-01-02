#include <Windows.h>
#include "buildcfg.h"

//KXUIAAPI HRESULT WINAPI UiaRaiseNotificationEvent([in] IRawElementProviderSimple *provider, NotificationKind notificationKind, NotificationProcessing notificationProcessing, [in, optional] BSTR displayString, [in] BSTR activityId){
KXUIAAPI HRESULT WINAPI UiaRaiseNotificationEvent(DWORD *provider, DWORD notificationKind, DWORD notificationProcessing, DWORD displayString, DWORD activityId){
	return 0;
}