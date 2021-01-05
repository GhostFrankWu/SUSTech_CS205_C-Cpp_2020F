#pragma once
#include "winsock2.h"
#include <commctrl.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
using namespace std;
//主扫描线程参数结构体
struct MainThreadParam{
	DWORD StartIp;    
	DWORD EndIp;
	DWORD StartPort;
	DWORD EndPort;
	HANDLE hCopyEvent;
};
//connect线程参数结构体
struct ThreadParam{
	DWORD Ip;           //目标ip
	DWORD Port;         //目标端口
	HANDLE hNext;//通知主程序可以和下一个线程通信
	HANDLE hThreadNum;  //信号量 控制线程总数
};
//ip到DWORD转换
BOOL StrAddr2Dword(string lpStrAddr, DWORD* pdwAddr);
//初始化网络
BOOL InitPortScan();
//端口扫描线程函数
DWORD WINAPI PortScanthread(LPVOID LpParam);
//循环调用PortScanthread
DWORD WINAPI MainThread(LPVOID LpParam);
//开始扫描函数  
BOOL StartScanner(DWORD StartIp, DWORD EndIp, DWORD StartPort, DWORD EndPort);