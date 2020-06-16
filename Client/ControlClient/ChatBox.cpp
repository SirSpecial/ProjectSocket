#include "ChatBox.h"
#include "Resource.h"

ChatBox* ChatBox::create(HWND hParent, HINSTANCE hInst, wstring partner)
{
	auto cb = new (std::nothrow) ChatBox();
	cb->m_hParent = hParent;
	cb->m_hInst = hInst;
	cb->m_partnerUsername = partner;
	cb->m_hWnd = CreateWindowW(L"chatbox", (cb->m_partnerUsername + L" - Chat").c_str(), WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, cb->m_hInst, NULL);

	cb->m_hMessageBox = CreateWindowEx(WS_EX_RIGHTSCROLLBAR, L"edit", L"", WS_VISIBLE | WS_VSCROLL | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY | WS_BORDER, 12.5, 12.5, 462.5, 317.5, cb->m_hWnd, NULL, hInst, 0);
	cb->m_hTextBox = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_BORDER, 12.5, 350, 275, 75, cb->m_hWnd, 0, hInst, 0);
	CreateWindow(L"button", L"SEND", WS_VISIBLE | WS_CHILD, 300, 350, 150, 75, cb->m_hWnd, (HMENU)IDC_SEND, cb->m_hInst, 0);

	return cb;
}

wstring ChatBox::getPartner()
{
	return m_partnerUsername;
}

void ChatBox::receiveMessage(WCHAR* message)
{
	WCHAR buffer[10000];
	GetWindowText(m_hMessageBox, buffer, 10000);
	wcscat(buffer, L"\r\n\r\n$[");
	wcscat(buffer, m_partnerUsername.c_str());
	wcscat(buffer, L"]:\r\n");
	wcscat(buffer, message);
	SetWindowText(m_hMessageBox, buffer);

	SendMessageA(m_hMessageBox, EM_SETSEL, 0, -1);
	SendMessageA(m_hMessageBox, EM_SETSEL, -1, -1);
	SendMessageA(m_hMessageBox, EM_SCROLLCARET, 0, 0);
}

int ChatBox::onSend(WCHAR* message)
{
	/*
	* send:	message = [FLAG | sender | NULL | receiver | NULL | content | NULL]
	*/
	WCHAR bufferText[1000];
	WCHAR bufferMessage[10000];

	message[0] = CHAT;
	message[1] = NULL;

	GetWindowText(m_hTextBox, bufferText, 1000);
	SetWindowText(m_hTextBox, L"");
	if (wcscmp(bufferText, L"") == 0) return -1;

	WCHAR* content;
	WCHAR* receiver;
	int len;
	wcscat(message, m_username.c_str());

	len = wcslen(message);
	len++;
	receiver = message + len;
	wcscpy(receiver, m_partnerUsername.c_str());
	len += m_partnerUsername.size() + 1;

	content = message + len;
	wcscpy(content, bufferText);
	len += wcslen(bufferText);

	GetWindowText(m_hMessageBox, bufferMessage, 10000);
	wcscat(bufferMessage, L"\r\n\r\n[You]:\r\n");
	wcscat(bufferMessage, bufferText);
	SetWindowText(m_hMessageBox, bufferMessage);

	SendMessageA(m_hMessageBox, EM_SETSEL, 0, -1);
	SendMessageA(m_hMessageBox, EM_SETSEL, -1, -1);
	SendMessageA(m_hMessageBox, EM_SCROLLCARET, 0, 0);

	if (m_isEndChat)
		return -1;

	return len;
}

void ChatBox::onEndChat()
{
	WCHAR buffer[10000];
	GetWindowText(m_hMessageBox, buffer, 10000);
	wcscat(buffer, L"\r\n\r\n\tThis conversation was ended by your partner.\r\n");
	SetWindowText(m_hMessageBox, buffer);

	SendMessageA(m_hMessageBox, EM_SETSEL, 0, -1);
	SendMessageA(m_hMessageBox, EM_SETSEL, -1, -1);
	SendMessageA(m_hMessageBox, EM_SCROLLCARET, 0, 0);
	m_isEndChat = 1;
}

void ChatBox::setUsername(const wstring& username)
{
	m_username = username;
}
HWND ChatBox::getHWND()
{
	return m_hWnd;
}
wstring ChatBox::getUsername()
{
	return m_username;
}
ChatBox::ChatBox()
{
	m_isEndChat = 0;
}
ChatBox::~ChatBox()
{

}