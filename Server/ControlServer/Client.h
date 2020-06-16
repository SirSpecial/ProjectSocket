#pragma once
#include "Flags.h"

struct Client
{
	SOCKET socket;
	wstring username = L"";
};