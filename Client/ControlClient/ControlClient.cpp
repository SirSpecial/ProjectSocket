// ControlClient.cpp : Defines the entry point for the application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "ControlClient.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(WNDPROC, WCHAR*, HINSTANCE);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChangePasswordProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    SetupInforProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    CheckUserProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChatBoxProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_CONTROLCLIENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(WndProc, szWindowClass, hInstance);
	MyRegisterClass(ChangePasswordProc, (WCHAR*)L"changePassword", hInstance);
	MyRegisterClass(SetupInforProc, (WCHAR*)L"setupInfor", hInstance);
	MyRegisterClass(CheckUserProc, (WCHAR*)L"checkUser", hInstance);
	MyRegisterClass(ChatBoxProc, (WCHAR*)L"chatBox", hInstance);
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONTROLCLIENT));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(WNDPROC WndProc, WCHAR* szClassName, HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CONTROLCLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = /*MAKEINTRESOURCEW(IDC_CONTROLCLIENT)*/ NULL;
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 400, 400, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		string serverAddress = loadServerAddress();
		GlobalClient.init(serverAddress.c_str(), 8080);

		if (GlobalClient.isOnline() == 0)
		{
			MessageBeep(MB_ICONERROR);
			MessageBox(hWnd, L"Unable to connect to server address", L"ERROR", MB_OK | MB_ICONERROR);
			SendMessage(hWnd, WM_DESTROY, 0, 0);
			return 0;
		}

		GlobalClient.setHWND(hWnd);
		AfxBeginThread(receiveMessageThread, 0);

		stUsername = CreateWindowW(L"static", L"Username:", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 50, 100, 20, hWnd, NULL, hInst, NULL);
		edUsername = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 100, 80, 200, 20, hWnd, NULL, hInst, NULL);

		stPassword = CreateWindowW(L"static", L"Password:", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 130, 100, 20, hWnd, NULL, hInst, NULL);
		edPassword = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER | ES_PASSWORD, 100, 160, 200, 20, hWnd, NULL, hInst, NULL);

		btLogin = CreateWindowW(L"button", L"LOGIN", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 210, 100, 20, hWnd, (HMENU)IDC_LOGIN, hInst, NULL);
		btRegister = CreateWindowW(L"button", L"REGISTER", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 250, 100, 20, hWnd, (HMENU)IDC_REGISTER, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_RECEIVE:
		{
			WCHAR* message = (WCHAR*)lParam;
			WCHAR send[DEFAULT_BUFLEN];
			switch (message[0])
			{
			case REGISTER_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Register Successfull!", L"Congratulations", MB_OK | MB_ICONINFORMATION);
				break;
			}
			case REGISTER_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				err(hWnd, (WCHAR*)L"Register Failed!");
				break;
			}
			case LOGIN_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Login Successfull!", L"Congratulations", MB_OK | MB_ICONINFORMATION);

				DestroyWindow(stUsername);
				DestroyWindow(edUsername);
				DestroyWindow(stPassword);
				DestroyWindow(edPassword);
				DestroyWindow(btLogin);
				DestroyWindow(btRegister);
				InvalidateRect(hWnd, 0, TRUE);

				CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 75, 30, 240, 210, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"button", L"CHANGE PASSWORD", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 45, 200, 20, hWnd, (HMENU)IDC_CHANGEPASSWORD, hInst, NULL);
				CreateWindowW(L"button", L"SETUP INFORMATION", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 85, 200, 20, hWnd, (HMENU)IDC_SETUPINFORMATION, hInst, NULL);
				CreateWindowW(L"button", L"CHECK UESER", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 125, 200, 20, hWnd, (HMENU)IDC_CHECKUSER, hInst, NULL);
				CreateWindowW(L"button", L"UPLOAD FILE", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 165, 200, 20, hWnd, (HMENU)IDC_UPLOAD, hInst, NULL);
				CreateWindowW(L"button", L"DOWNLOAD FILE", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 205, 200, 20, hWnd, (HMENU)IDC_DOWNLOAD, hInst, NULL);
				CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 75, 245, 240, 80, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"static", L"Partner: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 90, 262, 50, 20, hWnd, NULL, hInst, NULL);
				edPartner = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 145, 260, 160, 20, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"button", L"CHAT USRER", WS_VISIBLE | WS_CHILD | SS_CENTER, 115, 290, 160, 20, hWnd, (HMENU)IDC_CHAT, hInst, NULL);

				break;
			}
			case LOGIN_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				err(hWnd, (WCHAR*)L"Username or password is incorrect!");
				GlobalClient.setUsername(L"");
				break;
			}
			case CHANGE_PASSWORD_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Change password successfull!", L"Congratulations", MB_OK | MB_ICONINFORMATION);
				break;
			}
			case CHANGE_PASSWORD_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				err(hWnd, (WCHAR*)L"Change password fail!");
				break;
			}
			case USER_FIND_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database has this user", L"Information", 0);
				break;
			}
			case USER_FIND_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database hasn't this user", L"Information", 0);
				break;
			}
			case USER_ONLINE_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | 0 / 1 | NULL]		- Success
				*/
				if (message[1] == 0)
				{
					MessageBeep(MB_ICONINFORMATION);
					MessageBox(hWnd, L"User is offline", L"Information", 0);
				}
				else
				{
					MessageBeep(MB_ICONINFORMATION);
					MessageBox(hWnd, L"User is online", L"Information", 0);
				}
				break;
			}
			case USER_ONLINE_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database hasn't this user", L"Information", 0);
				break;
			}
			case USER_SHOW_DATE_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | date     | NULL]	- Success
				*/

				WCHAR date[250];
				date[0] = NULL;
				wcscat(date, L"Date: ");
				wcscat(date, message + 1);
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, date, L"Information", 0);
				break;
			}
			case USER_SHOW_DATE_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database hasn't this user", L"Information", 0);
				break;
			}
			case USER_SHOW_FULLNAME_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | fullname | NULL]	- Success
				*/
				WCHAR fullname[250];
				fullname[0] = NULL;
				wcscat(fullname, L"Fullname: ");
				wcscat(fullname, message + 1);
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, fullname, L"Information", 0);
				break;
			}
			case USER_SHOW_FULLNAME_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database hasn't this user", L"Information", 0);
				break;
			}
			case USER_SHOW_NOTE_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | note	   | NULL]	- Success
				*/
				WCHAR note[250];
				note[0] = NULL;
				wcscat(note, L"Note: ");
				wcscat(note, message + 1);
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, note, L"Information", 0);
				break;
			}
			case USER_SHOW_NOTE_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database hasn't this user", L"Information", 0);
				break;
			}
			case USER_SHOW_ALL_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | 0 / 1 | fullname | NULL | date | NULL | note | NULL]	- Success
				*/
				WCHAR all[4096];
				WCHAR* temp;
				all[0] = NULL;
				if (message[1] == 0) wcscat(all, L"User is offline\n");
				else wcscat(all, L"User is online\n");

				int i = 2;
				wcscat(all, L"Fullname: ");
				wcscat(all, message + i);
				wcscat(all, L"\n");
				temp = message + i;
				i += (wcslen(temp) + 1);

				wcscat(all, L"Date: ");
				wcscat(all, message + i);
				wcscat(all, L"\n");
				temp = message + i;
				i += (wcslen(temp) + 1);

				wcscat(all, L"Note: ");
				wcscat(all, message + i);
				wcscat(all, L"\n");
				temp = message + i;
				i += (wcslen(temp) + 1);

				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, all, L"Information", 0);
				break;
			}
			case USER_SHOW_ALL_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Database hasn't this user", L"Information", 0);
				break;
			}
			case SETUP_INFO_FULLNAME_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Setup full name successfull!", L"Congratulations", MB_OK | MB_ICONINFORMATION);
				break;
			}
			case SETUP_INFO_FULLNAME_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				err(hWnd, (WCHAR*)L"Setup name fail");
				break;
			}
			case SETUP_INFO_DATE_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Setup date successfull!", L"Congratulations", MB_OK | MB_ICONINFORMATION);
				break;
			}
			case SETUP_INFO_DATE_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				err(hWnd, (WCHAR*)L"Setup date fail");
				break;
			}
			case SETUP_INFO_NOTE_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"Setup note successfull!", L"Congratulations", MB_OK | MB_ICONINFORMATION);
				break;
			}
			case SETUP_INFO_NOTE_FAILURE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				err(hWnd, (WCHAR*)L"Setup note fail");
				break;
			}
			case CREATE_CHAT_SUCCESS:
			{
				/*
				* receive:	message = [FLAG | partner | NULL]
				*/
				message++;
				int	len = wcslen(message);
				MessageBeep(MB_ICONINFORMATION);
				MessageBox(hWnd, L"The user is online", L"CHAT", MB_ICONINFORMATION | MB_OK);
				for (auto chatbox : chatBoxList)
				{
					if (wcscmp(chatbox->getPartner().c_str(), message) == 0)
					{
						return 0;
					}
				}
				auto cb = ChatBox::create(hWnd, hInst, message);

				cb->setUsername(GlobalClient.getUsername());
				chatBoxList.push_back(cb);
				break;
			}
			case CREATE_CHAT_FAILURE:
			{
				/*
				* receive:	message = [FLAG]
				*/
				err(hWnd, (WCHAR*)L"The user is offline or Database hasn't username");
				break;
			}
			case CHAT:
			{
				/*
				* receive:	message = [FLAG | sender | NULL | receiver | NULL | content | NULL]
				*/

				int len = wcslen(message);
				WCHAR* sender = message + 1;
				WCHAR* content;
				content = message + len + 1 + GlobalClient.getUsername().size() + 1;
				for (auto chat : chatBoxList)
				{
					if (wcscmp(chat->getPartner().c_str(), sender) == 0)
					{
						chat->receiveMessage(content);
						SetForegroundWindow(chat->getHWND());
						SetActiveWindow(chat->getHWND());
						return 0;
					}
				}
				auto cb = ChatBox::create(hWnd, hInst, sender);
				cb->setUsername(GlobalClient.getUsername());
				chatBoxList.push_back(cb);
				cb->receiveMessage(content);
				SetForegroundWindow(cb->getHWND());
				SetActiveWindow(cb->getHWND());
				break;
			}
			case END_CHAT:
			{
				/*
				* receive:	message = [FLAG | sender | NULL | receiver | NULL]
				*/
				WCHAR* sender;
				sender = message + 1;

				for (auto chat : chatBoxList)
				{
					if (wcscmp(chat->getPartner().c_str(), sender) == 0)
					{
						chat->onEndChat();
						break;
					}
				}
				break;
			}
			}
			break;
		}
		case IDC_REGISTER:
		{
			/*
			* send:		message = [FLAG | username | NULL | password | NULL]
			*/
			WCHAR username[50];
			WCHAR password[50];
			WCHAR message[100];
			message[0] = REGISTER;
			message[1] = NULL;

			GetWindowText(edUsername, username, 50);
			if (wcscmp(username, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Username is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Username is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			GetWindowText(edPassword, password, 50);
			if (wcscmp(password, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Password is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Password is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			wcscat_s(message, username);
			int len = wcslen(message);
			len++;
			int i;
			for (i = 0; password[i] != NULL; i++)
			{
				message[len + i] = password[i];
			}
			len += i;
			message[len] = NULL;
			GlobalClient.sendMessage(message, len);
			break;
		}
		case IDC_CHANGEPASSWORD:
		{
			changePassDlg = CreateWindowW(L"changePassword", L"Change Password", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				100, 100, 500, 220, hWnd, NULL, hInst, NULL);
			//ShowWindow(changePassDlg, NULL);
			break;
		}
		case IDC_SETUPINFORMATION:
		{
			setupInforDlg = CreateWindowW(L"setupInfor", L"Setup Information", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				100, 100, 350, 200, hWnd, NULL, hInst, NULL);
			break;
		}
		case IDC_CHECKUSER:
		{
			setupInforDlg = CreateWindowW(L"checkUser", L"Check User", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
				100, 100, 350, 250, hWnd, NULL, hInst, NULL);
			break;
		}
		case IDC_LOGIN:
		{
			/*
			* send:		message = [FLAG | username | NULL | password | NULL]
			*/
			WCHAR username[50];
			WCHAR password[50];
			WCHAR message[100];
			message[0] = LOGIN;
			message[1] = NULL;

			GetWindowText(edUsername, username, 50);
			if (wcscmp(username, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Username is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Username is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			GetWindowText(edPassword, password, 50);
			if (wcscmp(password, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Password is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Password is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}
			GlobalClient.setUsername(username);
			wcscat_s(message, username);
			int len = wcslen(message);
			len++;
			int i;
			for (i = 0; password[i] != NULL; i++)
			{
				message[len + i] = password[i];
			}
			len += i;
			message[len] = NULL;
			GlobalClient.sendMessage(message, len);
			break;
		}
		case IDC_CHAT:
		{
			/*
			* send:		message = [FLAG | partner | NULL]
			*/
			WCHAR message[100];
			WCHAR buffer[100];
			message[0] = CREATE_CHAT;
			message[1] = NULL;
			GetWindowText(edPartner, buffer, 100);
			if (wcscmp(buffer, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Username partner is empty");
				return 0;
			}
			if (wcscmp(buffer, GlobalClient.getUsername().c_str()) == 0)
			{
				err(hWnd, (WCHAR*)L"Can't create chat by your username");
				return 0;
			}
			wcscat(message, buffer);
			int len = wcslen(message);
			GlobalClient.sendMessage(message, len);
			SetWindowText(edPartner, L"");
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK ChangePasswordProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindow(L"static", L"CHANGE PASSWORD", WS_VISIBLE | WS_CHILD | ES_CENTER, 150, 10, 150, 20, hWnd, 0, hInst, 0);

		CreateWindow(L"static", L"Old Password:", WS_VISIBLE | WS_CHILD | ES_CENTER, 80, 40, 150, 20, hWnd, 0, hInst, 0);
		edOldPass = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_PASSWORD | WS_BORDER, 240, 40, 180, 20, hWnd, 0, hInst, 0);

		CreateWindow(L"static", L"New Password:", WS_VISIBLE | WS_CHILD | ES_CENTER, 80, 70, 150, 20, hWnd, 0, hInst, 0);
		edNewPass = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_PASSWORD | WS_BORDER, 240, 70, 180, 20, hWnd, 0, hInst, 0);

		CreateWindow(L"static", L"Old Password:", WS_VISIBLE | WS_CHILD | ES_CENTER, 80, 100, 150, 20, hWnd, 0, hInst, 0);
		edConfirmPass = CreateWindow(L"edit", L"", WS_VISIBLE | WS_CHILD | ES_CENTER | ES_PASSWORD | WS_BORDER, 240, 100, 180, 20, hWnd, 0, hInst, 0);

		CreateWindow(L"button", L"ENTER", WS_VISIBLE | WS_CHILD | SS_CENTER, 200, 140, 100, 20, hWnd, (HMENU)IDC_ENTER, hInst, 0);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_ENTER:
		{
			/*
			send: message = [FLAG | username | NULL | oldpass | NULL | newpass | NULL]
			*/
			WCHAR oldPass[50];
			WCHAR newPass[50];
			WCHAR confirmPass[50];
			GetWindowText(edOldPass, oldPass, 50);
			if (wcscmp(oldPass, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Old password is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Username is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			GetWindowText(edNewPass, newPass, 50);
			if (wcscmp(newPass, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"New password is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Password is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			GetWindowText(edConfirmPass, confirmPass, 50);
			if (wcscmp(confirmPass, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Comfirm password is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Password is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			if (wcscmp(confirmPass, newPass) != 0)
			{
				err(hWnd, (WCHAR*)L"New password and confirm password is different");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Password is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}

			if (wcscmp(newPass, oldPass) == 0)
			{
				err(hWnd, (WCHAR*)L"New password and old password is a same");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Password is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}


			WCHAR message[200];
			message[0] = CHANGE_PASSWORD;
			message[1] = NULL;
			wcscat_s(message, GlobalClient.getUsername().c_str());
			int len = wcslen(message);
			len++;
			int i;
			for (i = 0; oldPass[i] != NULL; i++)
			{
				message[len + i] = oldPass[i];
			}
			len += i;
			message[len] = NULL;
			len++;
			for (i = 0; newPass[i] != NULL; i++)
			{
				message[len + i] = newPass[i];
			}
			len += i;
			message[len] = NULL;
			GlobalClient.sendMessage(message, len);

			SetWindowText(edOldPass, L"");
			SetWindowText(edNewPass, L"");
			SetWindowText(edConfirmPass, L"");
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK SetupInforProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowW(L"static", L"Enter:", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 20, 50, 20, hWnd, NULL, hInst, 0);
		edEnter = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 80, 20, 230, 20, hWnd, NULL, hInst, 0);

		CreateWindowW(L"button", L"Name", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 40, 60, 60, 20, hWnd, (HMENU)IDC_NAME, hInst, 0);
		CreateWindowW(L"button", L"Date", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 40, 90, 60, 20, hWnd, (HMENU)IDC_DATE, hInst, 0);
		CreateWindowW(L"button", L"Note", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 40, 120, 60, 20, hWnd, (HMENU)IDC_NOTE, hInst, 0);
		CreateWindowW(L"button", L"SETUP INFORMATION", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 90, 150, 20, hWnd, (HMENU)IDC_ENTER, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_ENTER:
		{
			WCHAR enter[150];
			GetWindowText(edEnter, enter, 150);
			if (wcscmp(enter, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Your information is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Your information is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}
			WCHAR message[200];
			if (IsDlgButtonChecked(hWnd, IDC_NAME))
			{
				/*
				send: message = [FLAG | username | NULL | fullname | NULL]
				*/
				message[0] = SETUP_INFO_FULLNAME;
				message[1] = NULL;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_DATE))
			{
				/*
				send: message = [FLAG | username | NULL | date | NULL]
				*/
				message[0] = SETUP_INFO_DATE;
				message[1] = NULL;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_NOTE))
			{
				/*
				send: message = [FLAG | username | NULL | note | NULL]
				*/
				message[0] = SETUP_INFO_NOTE;
				message[1] = NULL;
			}
			else
			{
				err(hWnd, (WCHAR*)L"Please check type of information");
				break;
			}
			wcscat_s(message, GlobalClient.getUsername().c_str());
			int len = wcslen(message);
			len++;
			int i;
			for (i = 0; enter[i] != NULL; i++)
			{
				message[len + i] = enter[i];
			}
			len += i;
			message[len] = NULL;
			GlobalClient.sendMessage(message, len);
			SetWindowText(edEnter, L"");
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK CheckUserProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindow(L"static", L"Username:", WS_VISIBLE | WS_CHILD | ES_CENTER, 20, 20, 70, 20, hWnd, NULL, hInst, NULL);
		edUsername = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 100, 20, 210, 20, hWnd, NULL, hInst, 0);

		CreateWindowW(L"button", L"Find", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 35, 60, 115, 20, hWnd, (HMENU)IDC_FIND, hInst, 0);
		CreateWindowW(L"button", L"Online", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 35, 90, 115, 20, hWnd, (HMENU)IDC_ONLINE, hInst, 0);
		CreateWindowW(L"button", L"Show fullname", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 35, 120, 115, 20, hWnd, (HMENU)IDC_SHOWNAME, hInst, 0);

		CreateWindowW(L"button", L"Show date", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 180, 60, 115, 20, hWnd, (HMENU)IDC_SHOWDATE, hInst, 0);
		CreateWindowW(L"button", L"Show note", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 180, 90, 115, 20, hWnd, (HMENU)IDC_SHOWNOTE, hInst, 0);
		CreateWindowW(L"button", L"Show all", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 180, 120, 115, 20, hWnd, (HMENU)IDC_SHOWALL, hInst, 0);
		CreateWindowW(L"button", L"CHECK USER", WS_VISIBLE | WS_CHILD | SS_CENTER, 125, 160, 100, 20, hWnd, (HMENU)IDC_ENTER, hInst, 0);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_ENTER:
		{
			WCHAR username[150];
			GetWindowText(edUsername, username, 150);
			if (wcscmp(username, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Your information is empty");
				//MessageBeep(MB_ICONERROR);
				//MessageBox(hWnd, L"Your information is empty", 0, MB_OK | MB_ICONERROR);
				return 0;
			}
			if (IsDlgButtonChecked(hWnd, IDC_FIND))
			{
				/*
				send: message = [FLAG | username | NULL]
				*/
				WCHAR message[200];
				message[0] = CHECK_USER_FIND;
				message[1] = NULL;
				wcscat_s(message, username);
				int len = wcslen(message);
				GlobalClient.sendMessage(message, len);
			}
			else if (IsDlgButtonChecked(hWnd, IDC_ONLINE))
			{
				/*
				send: message = [FLAG | username | NULL]
				*/

				WCHAR message[200];
				message[0] = CHECK_USER_ONLINE;
				message[1] = NULL;
				wcscat_s(message, username);
				int len = wcslen(message);
				GlobalClient.sendMessage(message, len);
			}
			else if (IsDlgButtonChecked(hWnd, IDC_SHOWNAME))
			{
				/*
				send: message = [FLAG | username | NULL]
				*/

				WCHAR message[200];
				message[0] = CHECK_USER_SHOW_FULLNAME;
				message[1] = NULL;
				wcscat_s(message, username);
				int len = wcslen(message);
				GlobalClient.sendMessage(message, len);
			}
			else if (IsDlgButtonChecked(hWnd, IDC_SHOWDATE))
			{
				/*
				send: message = [FLAG | username | NULL]
				*/

				WCHAR message[200];
				message[0] = CHECK_USER_SHOW_DATE;
				message[1] = NULL;
				wcscat_s(message, username);
				int len = wcslen(message);
				GlobalClient.sendMessage(message, len);
			}
			else if (IsDlgButtonChecked(hWnd, IDC_SHOWNOTE))
			{
				/*
				send: message = [FLAG | username | NULL]
				*/
				WCHAR message[200];
				message[0] = CHECK_USER_SHOW_NOTE;
				message[1] = NULL;
				wcscat_s(message, username);
				int len = wcslen(message);
				GlobalClient.sendMessage(message, len);
			}
			else if (IsDlgButtonChecked(hWnd, IDC_SHOWALL))
			{
				/*
				send: message = [FLAG | username | NULL]
				*/

				WCHAR message[200];
				message[0] = CHECK_USER_SHOW_ALL;
				message[1] = NULL;
				wcscat_s(message, username);
				int len = wcslen(message);
				GlobalClient.sendMessage(message, len);
			}
			else
			{
				err(hWnd, (WCHAR*)L"Please check type of information");
			}
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChatBoxProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_SEND:
		{
			/*
			* send:		message = [FLAG | sender | NULL | receiver | NULL | content | NULL]
			*/
			for (auto chatbox : chatBoxList)
			{
				if (chatbox->getHWND() == hWnd)
				{
					WCHAR buffer[1000];
					int len = chatbox->onSend(buffer);
					if (len == -1)
					{
						err(hWnd, (WCHAR*)L"Can't send message");
						return 0;
					}
					GlobalClient.sendMessage(buffer, len);
					break;
				}
			}
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			/*
			* send:		message = [FLAG | sender | NULL | receiver | NULL]
			*/
			for (auto chatbox : chatBoxList)
			{
				if (chatbox->getHWND() == hWnd)
				{
					chatBoxList.remove(chatbox);
					WCHAR message[101];
					WCHAR* sender;
					message[0] = END_CHAT;
					message[1] = NULL;
					wcscat(message, chatbox->getUsername().c_str());
					sender = message + chatbox->getUsername().size() + 2;
					wcscpy(sender, chatbox->getPartner().c_str());
					int len = chatbox->getPartner().size() + chatbox->getPartner().size() + 3;
					GlobalClient.sendMessage(message, len);
					break;
				}
			}
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroyWindow(hWnd);
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


void err(HWND hWnd, WCHAR* error)
{
	MessageBeep(MB_ICONERROR);
	MessageBox(hWnd, (LPCWSTR)error, L"ERROR", MB_OK | MB_ICONERROR);
}