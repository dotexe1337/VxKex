#include <Windows.h>
#include "buildcfg.h"

KXRPCAPI RPC_STATUS WINAPI RpcServerRegisterIf3(
	IN RPC_IF_HANDLE IfSpec,
	IN OPTIONAL UUID *MgrTypeUuid,
	IN OPTIONAL RPC_MGR_EPV *MgrEpv,
	IN unsigned int Flags,
	IN unsigned int MaxCalls,
	IN unsigned int MaxRpcSize,
	IN OPTIONAL RPC_IF_CALLBACK_FN *IfCallback,
	IN OPTIONAL void *SecurityDescriptor
	)
{
	return RPC_S_CANNOT_SUPPORT;
}