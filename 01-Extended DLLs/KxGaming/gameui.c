#include "buildcfg.h"
#include "kxgamingp.h"
#include "kxgaming.h"

KXGAMINGAPI HRESULT WINAPI ProcessPendingGameUI(
	IN BOOL waitForCompletion)
{
	return ERROR_SUCCESS;
}

KXGAMINGAPI HRESULT WINAPI ShowChangeFriendRelationshipUI(
	IN HSTRING targetUserXuid,
	IN GameUICompletionRoutine completionRoutine,
	IN OPTIONAL void *context)
{
	return ERROR_SUCCESS;
}

KXGAMINGAPI HRESULT WINAPI ShowGameInviteUI(
	IN HSTRING serviceConfigurationId,
	IN HSTRING sessionTemplateName,
	IN HSTRING sessionId,
	IN HSTRING invitationDisplayText,
	IN GameUICompletionRoutine completionRoutine,
	IN OPTIONAL void *context)
{
	return ERROR_SUCCESS;
}

KXGAMINGAPI HRESULT WINAPI ShowPlayerPickerUI(
	IN HSTRING promptDisplayText,
	IN const HSTRING *xuids,
	IN size_t xuidsCount,
	IN OPTIONAL const HSTRING *preSelectedXuids,
	IN size_t preSelectedXuidsCount,
	IN size_t minSelectionCount,
	IN size_t maxSelectionCount,
	IN PlayerPickerUICompletionRoutine completionRoutine,
	IN OPTIONAL void *context)
{
	return ERROR_SUCCESS;
}

KXGAMINGAPI HRESULT WINAPI ShowProfileCardUI(
	IN HSTRING targetUserXuid,
	IN GameUICompletionRoutine completionRoutine,
	IN OPTIONAL void *context)
{
	return ERROR_SUCCESS;
}

KXGAMINGAPI HRESULT WINAPI ShowTitleAchievementsUI(
	IN UINT32 titleId,
	IN GameUICompletionRoutine completionRoutine,
	IN OPTIONAL void *context)
{
	return ERROR_SUCCESS;
}

BOOL TryCancelPendingGameUI()
{
	return 1;
}