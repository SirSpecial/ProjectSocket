#include "Thread.h"

UINT receiveMessageThread(LPVOID lParam)
{
	while (TRUE)
		if (GlobalClient.receiveMessage())
			break;
	return 0;
}