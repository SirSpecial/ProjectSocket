#include "Thread.h"

UINT listenThread(LPVOID lParam)
{
	while (TRUE)
		GlobalServer.listenClient();
	return 0;
}

UINT receiveMessageThread(LPVOID lParam)
{
	SOCKET client = (SOCKET)lParam;
	while (TRUE)
		if (GlobalServer.receiveMessage(client)) break;
	return 0;
}