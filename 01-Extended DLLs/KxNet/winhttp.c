#include "buildcfg.h"
#include "kxnetp.h"

KXNETAPI ULONG WINAPI WinHttpCreateProxyResolver(
	IN	HINTERNET	SessionHandle,
	OUT	HINTERNET	*Resolver)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	KexDebugCheckpoint();
	*Resolver = (HINTERNET) 0x12345678;
	return ERROR_SUCCESS;
}

KXNETAPI ULONG WINAPI WinHttpGetProxyForUrlEx(
	IN	HINTERNET	Resolver,
	IN	PCWSTR		Url,
	IN	PVOID		AutoProxyOptions,
	IN	ULONG_PTR	Context)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	KexDebugCheckpoint();
	return ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT;
}

KXNETAPI ULONG WINAPI WinHttpGetProxyResult(
	IN	HINTERNET	Resolver,
	OUT	PVOID		ProxyResult)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	KexDebugCheckpoint();
	return ERROR_WINHTTP_INCORRECT_HANDLE_STATE;
}

KXNETAPI VOID WINAPI WinHttpFreeProxyResult(
	IN OUT	PVOID	ProxyResult)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	KexDebugCheckpoint();
}

KXNETAPI DWORD WINAPI WinHttpWebSocketClose(
	IN HINTERNET hWebSocket,
	IN USHORT usStatus,
	IN OPTIONAL PVOID pvReason,
	IN DWORD len)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
    return ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT;
}

KXNETAPI DWORD WINAPI WinHttpWebSocketReceive(
	IN HINTERNET hWebSocket,
	OUT PVOID pvBuffer,
	IN DWORD dwBufferLength,
	OUT DWORD *pdwBytesRead,
	OUT enum WINHTTP_WEB_SOCKET_BUFFER_TYPE *peBufferType)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	return ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT;
}

KXNETAPI DWORD WINAPI WinHttpWebSocketSend(
	IN HINTERNET hWebSocket,
	IN enum WINHTTP_WEB_SOCKET_BUFFER_TYPE *eBufferType,
	IN PVOID pvBuffer,
	IN DWORD dwBufferLength)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	return ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT;
}

KXNETAPI DWORD WINAPI WinHttpWebSocketCompleteUpgrade(
	IN HINTERNET hRequest,
	IN OPTIONAL DWORD_PTR pContext)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	return ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT;
}

KXNETAPI DWORD WINAPI WinHttpWebSocketShutdown(
	IN HINTERNET hWebSocket,
	IN USHORT    usStatus,
	IN OPTIONAL PVOID     pvReason,
	IN DWORD     dwReasonLength)
{
	KexLogWarningEvent(L"Unimplemented WinHTTP function called");
	return ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT;
}