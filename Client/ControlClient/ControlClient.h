#pragma once
#include "Client.h"
#include "resource.h"
#include "ChatBox.h"
#include "Thread.h"

void err(HWND hWnd, WCHAR* error);
list<ChatBox*> chatBoxList;
list<WCHAR*> listMultiFile;
WCHAR file[100];

HANDLE hSentFile;
HANDLE hReceiveFile;
DWORD sentFileSize;
DWORD recFileSize;
DWORD totalRecSize;



//window 1
HWND stUsername, edUsername;
HWND stPassword, edPassword;
HWND btLogin, btRegister;

//window 2
//HWND btChangePassword, btSetupInfo, btCheckUer, btChat, btUpload, btDownload;
HWND edPartner;

//window changePass
HWND changePassDlg;
HWND edOldPass, edNewPass, edConfirmPass;

//window setupInfor
HWND setupInforDlg;
HWND edEnter;

//window setupInfor
HWND checkUserDlg;

HWND changeNameUploadDlg;
HWND MultiUploadDlg;

HWND edFileName;
HWND edChangeName;

LPWSTR convertSize(DWORD size);
BOOL myCreateOpenFile(HWND hwnd, WCHAR* filename);
BOOL myCreateSaveFile(HWND hwnd, WCHAR* filename);

DWORD onSendFile(WCHAR* message);

