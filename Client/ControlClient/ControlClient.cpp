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
LRESULT CALLBACK    ChangeNameUploadProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    MultiFileUploadProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    NormalDownloadProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChangeNameDownloadProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    MultiFileDownloadProc(HWND, UINT, WPARAM, LPARAM);

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


	MyRegisterClass(ChangeNameUploadProc, (WCHAR*)L"changeNameUpload", hInstance);
	MyRegisterClass(MultiFileUploadProc, (WCHAR*)L"multiUpload", hInstance);

	MyRegisterClass(NormalDownloadProc, (WCHAR*)L"normalDownload", hInstance);
	MyRegisterClass(ChangeNameDownloadProc, (WCHAR*)L"changeNameDownload", hInstance);
	MyRegisterClass(MultiFileDownloadProc, (WCHAR*)L"multiDownload", hInstance);
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

				CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 75, 15, 240, 100, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"button", L"CHANGE PASSWORD", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 25, 200, 20, hWnd, (HMENU)IDC_CHANGEPASSWORD, hInst, NULL);
				CreateWindowW(L"button", L"SETUP INFORMATION", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 55, 200, 20, hWnd, (HMENU)IDC_SETUPINFORMATION, hInst, NULL);
				CreateWindowW(L"button", L"CHECK UESER", WS_VISIBLE | WS_CHILD | SS_CENTER, 95, 85, 200, 20, hWnd, (HMENU)IDC_CHECKUSER, hInst, NULL);

				CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 75, 125, 240, 80, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"static", L"Partner: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 90, 143, 50, 20, hWnd, NULL, hInst, NULL);
				edPartner = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 145, 140, 160, 20, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"button", L"CHAT USRER", WS_VISIBLE | WS_CHILD | SS_CENTER, 115, 170, 160, 20, hWnd, (HMENU)IDC_CHAT, hInst, NULL);

				CreateWindowW(L"static", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 75, 215, 240, 130, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"static", L"Option: ", WS_VISIBLE | WS_CHILD | SS_CENTER, 90, 250, 50, 20, hWnd, NULL, hInst, NULL);
				CreateWindowW(L"button", L"Normal", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 160, 225, 110, 20, hWnd, (HMENU)IDC_NORMAL, hInst, NULL);
				CreateWindowW(L"button", L"Change name", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 160, 250, 110, 20, hWnd, (HMENU)IDC_CHANGENAME, hInst, NULL);
				CreateWindowW(L"button", L"Multi files", WS_VISIBLE | WS_CHILD | SS_CENTER | BS_AUTORADIOBUTTON, 160, 275, 110, 20, hWnd, (HMENU)IDC_MULTI, hInst, NULL);
				CreateWindowW(L"button", L"UPLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 90, 305, 100, 20, hWnd, (HMENU)IDC_UPLOAD, hInst, NULL);
				CreateWindowW(L"button", L"DOWNLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 200, 305, 100, 20, hWnd, (HMENU)IDC_DOWNLOAD, hInst, NULL);				
				
				wcscpy(file, L"");
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
			case SEND_FILE:
			{
				/*
				* receive:	message = [FLAG | file name | NULL | file size | NULL]
				* send:		message = [FLAG | file name | NULL | file size | NULL]
				*/
				WCHAR* filename;
				if (wcscmp(file, L"") != 0)
					filename = file;
				else filename = message + 1;
				int len = wcslen(message);
				DWORD size;

				len++;
				size = *(message + len) << 16;
				len++;
				size = *(message + len);
				len++;
				len++;
				
				bool result = myCreateSaveFile(hWnd, filename);
				if (!result)
				{
					message[0] = SEND_FILE_CANCEL;
					GlobalClient.sendMessage(message, len);
					break;
				}
				message[0] = SEND_FILE_ACCEPT;

				hReceiveFile = CreateFile(filename, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
				recFileSize = size;
				totalRecSize = 0;
				wcscpy(file, L"");
				GlobalClient.sendMessage(message, len);
				break;
			}
			case SEND_FILE_ACCEPT:
			{
				/*
				* receive:	message = [FLAG | file name | NULL | file size | NULL]
				* send:		message = [FLAG | size | NULL | content | NULL]
				*/
				WCHAR* filename = message + 1;
				
				WCHAR send[1024];
				int len = onSendFile(send);
				
				GlobalClient.sendMessage(send, len);
				break;
			}
			case SEND_FILE_CANCEL:
			{
				/*
				* receive:	message = [FLAG | file name | NULL | file size | NULL | sender | NULL| receiver | NULL]
				*/
				WCHAR mess[200];
				wcscpy(mess, L"Server don't accept you file name: ");
				wcscat(mess, message + 1);
				err(hWnd, (WCHAR*) mess);
				CloseHandle(hSentFile);
				hSentFile = NULL;
				break;
			}
			case FILE_DATA:
			{
				/*
				* receive:	message = [FLAG | size | NULL | content]
				*send:		message = [FLAG | NULL]
				*/
				DWORD size = message[1];
				WCHAR* content = message + 3;

				DWORD dwBytesWrite;
				BOOL result = WriteFile(hReceiveFile, content, size, &dwBytesWrite, NULL);
				if (!result)
					err(hWnd, (WCHAR*)L"Failed write");
				
				
				totalRecSize += dwBytesWrite;
				if (totalRecSize >= recFileSize)
				{
					CloseHandle(hReceiveFile);
					totalRecSize = 0;
					recFileSize = 0;
					result = 0;
				}
				else result = 1;

				WCHAR send[100];
				if (result)
					send[0] = CONTINUE;
				else send[0] = STOP;
				send[1] = NULL;
				GlobalClient.sendMessage(send, 1);
				break;
			}
			case CONTINUE:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				WCHAR send[1024];
				int len = onSendFile(send);
				GlobalClient.sendMessage(send, len);
				break;
			}
			case  STOP:
			{
				/*
				* receive:	message = [FLAG | NULL]
				*/
				CloseHandle(hSentFile);
				hSentFile = NULL;
				wcscpy_s(file, L"");
				if (listMultiFile.size() != 0)
				{
					/*
					* send:		message = [FLAG | file name | NULL | file size | NULL]
					*/
					WCHAR buffer[1000];

					WCHAR send[1000];
					send[0] = SEND_FILE;
					send[1] = NULL;
					if (listMultiFile.size() == 0)
					{
						err(hWnd, (WCHAR*)L"File name is empty");
						return 0;
					}

					wcscpy(buffer, *listMultiFile.begin());
					listMultiFile.erase(listMultiFile.begin());

					if (hSentFile != NULL) err(hWnd, (WCHAR*)L"Wait! Your are sending a file");

					int lenBuf = wcslen(buffer);
					int i;
					for (i = lenBuf - 1; buffer[i] != L'\\' && i >= 0; i--);
					wcscat_s(send, &buffer[i + 1]);
					hSentFile = CreateFile(buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hSentFile == INVALID_HANDLE_VALUE)
						return 0;
					DWORD highSize;
					sentFileSize = GetFileSize(hSentFile, &highSize);

					DWORD size = sentFileSize;
					int len = wcslen(send);
					len++;
					send[len++] = size >> 16;
					send[len++] = size;
					send[len++] = NULL;
					GlobalClient.sendMessage(send, len);
				}
				break;
			}
			case DOWN_FILE_CANCEL:
			{
				WCHAR mess[200];
				wcscpy(mess, L"Server don't have you file name: ");
				wcscat(mess, message + 1);
				err(hWnd, (WCHAR*)mess);
				wcscpy_s(file, L"");
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
		case IDC_UPLOAD:
		{
			if (IsDlgButtonChecked(hWnd, IDC_NORMAL))
			{
				/*
				* send:		message = [FLAG | file name | NULL | file size | NULL]
				*/
				WCHAR buffer[1000];
				WCHAR send[1000];
				send[0] = SEND_FILE;
				send[1] = NULL;
				BOOL isOpenFile = myCreateOpenFile(hWnd, buffer);
				if (!isOpenFile) break;
				if (hSentFile != NULL) err(hWnd, (WCHAR*)L"Wait! Your are sending a file");
				
				int lenBuf = wcslen(buffer);
				int i;
				for (i = lenBuf - 1; buffer[i] != L'\\' && i >= 0; i--);
				wcscat_s(send, &buffer[i + 1]);
				hSentFile = CreateFile(buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (hSentFile == INVALID_HANDLE_VALUE)
					return 0;
				DWORD highSize;
				sentFileSize = GetFileSize(hSentFile, &highSize);

				DWORD size = sentFileSize;
				int len = wcslen(send);
				len++;
				send[len++] = size >> 16;
				send[len++] = size;
				send[len++] = NULL;
				GlobalClient.sendMessage(send, len);
				break;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_CHANGENAME))
			{
				changeNameUploadDlg = CreateWindowW(L"changeNameUpload", L"Change name upload", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
					100, 100, 250, 150, hWnd, NULL, hInst, NULL);
				break;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_MULTI))
			{
				MultiUploadDlg = CreateWindowW(L"multiUpload", L"Multi file upload", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
					100, 100, 300, 250, hWnd, NULL, hInst, NULL);
				break;
			}
			else
			{
				err(hWnd, (WCHAR*)L"Please choose your option upload");
				break;
			}
			break;
		}
		case IDC_DOWNLOAD:
		{
			if (IsDlgButtonChecked(hWnd, IDC_NORMAL))
			{
				changeNameUploadDlg = CreateWindowW(L"normalDownload", L"Normal Download", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
					100, 100, 250, 150, hWnd, NULL, hInst, NULL);
				break;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_CHANGENAME))
			{
				changeNameUploadDlg = CreateWindowW(L"changeNameDownload", L"Change name upload", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
					100, 100, 250, 200, hWnd, NULL, hInst, NULL);
				break;
			}
			else if (IsDlgButtonChecked(hWnd, IDC_MULTI))
			{
				MultiUploadDlg = CreateWindowW(L"multiDownload", L"Multi file download", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
					100, 100, 300, 250, hWnd, NULL, hInst, NULL);
				break;
			}
			else
			{
				err(hWnd, (WCHAR*)L"Please choose your option upload");
				break;
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

		CreateWindow(L"static", L"Confirm Password:", WS_VISIBLE | WS_CHILD | ES_CENTER, 80, 100, 150, 20, hWnd, 0, hInst, 0);
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
		//PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChangeNameUploadProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowW(L"static", L"File name:", WS_VISIBLE | WS_CHILD | SS_CENTER, 15, 10, 100, 20, hWnd, NULL, hInst, NULL);
		edFileName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 15, 35, 200, 20, hWnd, NULL, hInst, NULL);
		CreateWindowW(L"button", L"UPLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 70, 70, 100, 20, hWnd, (HMENU)IDC_UPLOAD, hInst, NULL);

		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_UPLOAD:
		{
			WCHAR filename[150];
			GetWindowText(edFileName, filename, 150);
			if (wcscmp(filename, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"Your file name is empty");
				return 0;
			}
				/*
				* send:		message = [FLAG | file name | NULL | file size | NULL]
				*/
			WCHAR buffer[1000];
			WCHAR send[1000];
			send[0] = SEND_FILE;
			send[1] = NULL;
			BOOL isOpenFile = myCreateOpenFile(hWnd, buffer);
			if (!isOpenFile) break;
			if (hSentFile != NULL) err(hWnd, (WCHAR*)L"Wait! Your are sending a file");

			int lenBuf = wcslen(buffer);
			
			wcscat_s(send, filename);
			
			hSentFile = CreateFile(buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hSentFile == INVALID_HANDLE_VALUE)
				return 0;
			DWORD highSize;
			sentFileSize = GetFileSize(hSentFile, &highSize);

			DWORD size = sentFileSize;
			int len = wcslen(send);
			len++;
			send[len++] = size >> 16;
			send[len++] = size;
			send[len++] = NULL;
			GlobalClient.sendMessage(send, len);
			SetWindowText(edFileName, L"");
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

LRESULT CALLBACK MultiFileUploadProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowW(L"static", L"Multi file name:", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 15, 100, 20, hWnd, NULL, hInst, NULL);
		edFileName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER | ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY, 20, 40, 248, 130, hWnd, NULL, hInst, NULL);
		CreateWindowW(L"button", L"ADD", WS_VISIBLE | WS_CHILD | SS_CENTER, 50, 180, 80, 20, hWnd, (HMENU)IDC_ADD, hInst, NULL);
		CreateWindowW(L"button", L"UPLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 150, 180, 80, 20, hWnd, (HMENU)IDC_UPLOAD, hInst, NULL);

		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_ADD:
		{
			WCHAR* buffer = new WCHAR[100];
			BOOL isOpenFile = myCreateOpenFile(hWnd, buffer);
			if (!isOpenFile) break;
			int len = wcslen(buffer);

			listMultiFile.push_back(buffer);

			int lenBuf = wcslen(buffer);
			int i;
			for (i = lenBuf - 1; buffer[i] != L'\\' && i >= 0; i--);

			WCHAR buff[10000];
			
			GetWindowText(edFileName, buff, 10000);
			wcscat_s(buff, L"\r\n");
			wcscat_s(buff, &buffer[i+1]);
			SetWindowText(edFileName, buff);

			SendMessageA(edFileName, EM_SETSEL, 0, -1);
			SendMessageA(edFileName, EM_SETSEL, -1, -1);
			SendMessageA(edFileName, EM_SCROLLCARET, 0, 0);
			break;
		}
		case IDC_UPLOAD:
		{
			
			/*
			* send:		message = [FLAG | file name | NULL | file size | NULL]
			*/
			WCHAR buffer[1000];

			WCHAR send[1000];
			send[0] = SEND_FILE;
			send[1] = NULL;
			if (listMultiFile.size() == 0)
			{
				err(hWnd, (WCHAR*)L"File name is empty");
				return 0;
			}

			wcscpy(buffer, *listMultiFile.begin());
			listMultiFile.erase(listMultiFile.begin());
			if (hSentFile != NULL) err(hWnd, (WCHAR*)L"Wait! Your are sending a file");

			int lenBuf = wcslen(buffer);
			int i;
			for (i = lenBuf - 1; buffer[i] != L'\\' && i >= 0; i--);
			wcscat_s(send, &buffer[i + 1]);
			hSentFile = CreateFile(buffer, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hSentFile == INVALID_HANDLE_VALUE)
				return 0;
			DWORD highSize;
			sentFileSize = GetFileSize(hSentFile, &highSize);

			DWORD size = sentFileSize;
			int len = wcslen(send);
			len++;
			send[len++] = size >> 16;
			send[len++] = size;
			send[len++] = NULL;

			GlobalClient.sendMessage(send, len);
			SetWindowText(edFileName, L"");
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

LRESULT CALLBACK NormalDownloadProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowW(L"static", L"File name:", WS_VISIBLE | WS_CHILD | SS_CENTER, 15, 10, 100, 20, hWnd, NULL, hInst, NULL);
		edFileName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 15, 35, 200, 20, hWnd, NULL, hInst, NULL);
		CreateWindowW(L"button", L"DOWNLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 70, 70, 100, 20, hWnd, (HMENU)IDC_DOWNLOAD, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_DOWNLOAD:
		{
			/*
			send: [FLAG |  filename | NULL]
			*/
			WCHAR buffer[100];
			GetWindowText(edFileName, buffer, 100);
			if (wcscmp(buffer, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"File name is empty");
				break;
			}
			WCHAR send[200];
			send[0] = DOWN_FILE;
			send[1] = NULL;
			wcscat_s(send, buffer);
			wcscpy_s(file, buffer);
			GlobalClient.sendMessage(send, wcslen(send));
			SetWindowText(edFileName, L"");
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

LRESULT CALLBACK ChangeNameDownloadProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowW(L"static", L"File name:", WS_VISIBLE | WS_CHILD | SS_CENTER, 15, 10, 100, 20, hWnd, NULL, hInst, NULL);
		edFileName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 15, 35, 200, 20, hWnd, NULL, hInst, NULL);
		CreateWindowW(L"static", L"Change name:", WS_VISIBLE | WS_CHILD | SS_CENTER, 15, 60, 100, 20, hWnd, NULL, hInst, NULL);
		edChangeName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER, 15, 85, 200, 20, hWnd, NULL, hInst, NULL);
		CreateWindowW(L"button", L"DOWNLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 70, 120, 100, 20, hWnd, (HMENU)IDC_DOWNLOAD, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_DOWNLOAD:
		{
			WCHAR bufferFile[100], bufferChange[100];
			GetWindowText(edFileName, bufferFile, 100);
			if (wcscmp(bufferFile, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"File name is empty");
				return 0;
			}

			GetWindowText(edChangeName, bufferChange, 100);
			if (wcscmp(bufferChange, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"File name is empty");
				return 0;
			}

			wcscpy_s(file, bufferChange);

			/*
			send: [FLAG |  filename | NULL]
			*/
			WCHAR send[200];
			send[0] = DOWN_FILE;
			send[1] = NULL;
			wcscat_s(send, bufferFile);
			GlobalClient.sendMessage(send, wcslen(send));
			//SetWindowText(edFileName, L"");
			//SetWindowText(edChangeName, L"");
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

LRESULT CALLBACK MultiFileDownloadProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		CreateWindowW(L"static", L"Multi file name:", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 15, 100, 20, hWnd, NULL, hInst, NULL);
		edFileName = CreateWindowW(L"edit", L"", WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 20, 40, 248, 130, hWnd, NULL, hInst, NULL);
		CreateWindowW(L"button", L"DOWNLOAD", WS_VISIBLE | WS_CHILD | SS_CENTER, 100, 180, 100, 20, hWnd, (HMENU)IDC_DOWNLOAD, hInst, NULL);
		break;
	}
	case WM_COMMAND:
	{
	
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_DOWNLOAD:
		{
			WCHAR buffer[1000];
			GetWindowText(edFileName, buffer, 1000);
			if (wcscmp(buffer, L"") == 0)
			{
				err(hWnd, (WCHAR*)L"File name is empty");
				return 0;
			}
			wstring buff;
			buff.append(buffer);
			list<WCHAR*> listFile;
			while (buff.find(L"\r") != -1)
			{
				int k = buff.find(L"\r");
				WCHAR* temp = new WCHAR[100];
				wcscpy(temp,buff.substr(0, k).c_str());
				listFile.push_back(temp);
				buff = buff.substr(k + 2, buff.size());
			}
			WCHAR* temp = new WCHAR[100];
			wcscpy(temp, buff.substr(0, buff.size()).c_str());
			listFile.push_back(temp);

			WCHAR send[1024];
			send[0] = DOWN_MULTI;
			send[1] = listFile.size();
			send[2] = NULL;
			int len = 2;
			/*
			send: message = [FLAG | number | file1 | NULL | filen | NULL]
			*/
			for (auto list : listFile)
			{
				wcscpy(send + len, list);
				len += wcslen(list);
				send[len] = NULL;
				len++;
			}
			GlobalClient.sendMessage(send, len);
			listMultiFile.clear();
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

BOOL myCreateOpenFile(HWND hwnd, WCHAR* filename)
{
	OPENFILENAMEW ofn = { 0 };
	WCHAR szFile[260];
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (!GetOpenFileName(&ofn))
		return FALSE;
	wcscpy(filename, ofn.lpstrFile);
	return TRUE;
}

BOOL myCreateSaveFile(HWND hwnd, WCHAR* path)
{
	OPENFILENAMEW ofn = { 0 };
	WCHAR szFile[260];
	WCHAR ex[10];
	wcscpy(szFile, path);
	int j;
	for (j = wcslen(szFile) - 1; j > 0 && szFile[j] != L'.'; j--) {}
	if (j != 0)
	{
		wcscpy(ex, szFile + j);
	}
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("All Files (*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	if (GetSaveFileName(&ofn) == FALSE)
		return FALSE;
	wcscpy(path, ofn.lpstrFile);
	int i;
	for (i = wcslen(path) - 1; i > 0 && path[i] != L'.'; i--) {}

	if (i == 0)
	{
		if (j != 0)
		{
			wcscat(path, ex);
		}
	}
	return TRUE;
}

LPWSTR convertSize(DWORD size)
{
	WCHAR* buffer = new WCHAR[9];
	if (size / 1073741824 > 0)
	{
		wsprintf(buffer, L"%d.%d GB", size / 1073741824, (int)((double)(size % 1073741824) / (double)1073741824)) * 10;
	}
	else if (size / 1048576 > 0)
	{
		wsprintf(buffer, L"%d.%d MB", size / 1048576, (int)((float)(size % 1048576) / (float)1048576)) * 10;
	}
	else if (size / 1024 > 0)
	{
		wsprintf(buffer, L"%d.%d KB", size / 1024, (int)((float)(size % 1024) / (float)1024)) * 10;
	}
	else
	{
		wsprintf(buffer, L"%d Byte", size);
	}
	return buffer;
}

DWORD onSendFile(WCHAR* message)
{
	int total = 0;
	DWORD dwBytesRead = 0;
	WCHAR buffer[1024];
	BOOL result = ReadFile(hSentFile, buffer, 1024, &dwBytesRead, NULL);
	if (dwBytesRead == 0)
		return 0;
	message[0] = FILE_DATA;
	message[1] = dwBytesRead;
	message[2] = NULL;
	int len = 3;

	if (dwBytesRead % 2 != 0)
	{
		dwBytesRead++;
	}
	int wsize = dwBytesRead / 2;

	int i;
	for (i = 0; i < wsize; i++)
	{
		message[len + i] = buffer[i];
	}
	len += wsize;
	return len;
}