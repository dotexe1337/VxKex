#include "buildcfg.h"
#include "kxnetp.h"

KXNETAPI INT WINAPI GetAddrInfoExOverlappedResult(
	LPOVERLAPPED lpOverlapped)
{
	return WSAEINVAL;
}