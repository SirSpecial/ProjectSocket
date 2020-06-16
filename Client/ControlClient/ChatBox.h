#pragma once
#include "Flags.h"

class ChatBox
{
private:
	HWND m_hParent;
	HINSTANCE m_hInst;
	HWND m_hWnd;
	HWND m_hMessageBox;
	HWND m_hTextBox;
	HWND m_hSend;

	wstring m_username;
	wstring m_partnerUsername;
	bool m_isEndChat;
public:
	static ChatBox* create(HWND hParent, HINSTANCE hInst, wstring partner);
	wstring getPartner();

	void receiveMessage(WCHAR* message);
	int onSend(WCHAR* message);

	void onEndChat();
	void setUsername(const wstring& username);
	HWND getHWND();

	wstring getUsername();
	ChatBox();
	~ChatBox();
};