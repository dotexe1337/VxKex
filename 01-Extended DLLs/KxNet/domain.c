#include "buildcfg.h"
#include "kxnetp.h"

KXNETAPI void WINAPI NetFreeAadJoinInformation(
	IN OPTIONAL PDSREG_JOIN_INFO pJoinInfo)
{ }

KXNETAPI HRESULT WINAPI NetGetAadJoinInformation(
	IN OPTIONAL LPCWSTR pcszTenantId,
	OUT PDSREG_JOIN_INFO ppJoinInfo)
{
	return ERROR_INVALID_PARAMETER;
}