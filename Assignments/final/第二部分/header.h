#pragma once
#include "winsock2.h"
#include <commctrl.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
using namespace std;
//��ɨ���̲߳����ṹ��
struct MainThreadParam{
	DWORD StartIp;    
	DWORD EndIp;
	DWORD StartPort;
	DWORD EndPort;
	HANDLE hCopyEvent;
};
//connect�̲߳����ṹ��
struct ThreadParam{
	DWORD Ip;           //Ŀ��ip
	DWORD Port;         //Ŀ��˿�
	HANDLE hNext;//֪ͨ��������Ժ���һ���߳�ͨ��
	HANDLE hThreadNum;  //�ź��� �����߳�����
};
//ip��DWORDת��
BOOL StrAddr2Dword(string lpStrAddr, DWORD* pdwAddr);
//��ʼ������
BOOL InitPortScan();
//�˿�ɨ���̺߳���
DWORD WINAPI PortScanthread(LPVOID LpParam);
//ѭ������PortScanthread
DWORD WINAPI MainThread(LPVOID LpParam);
//��ʼɨ�躯��  
BOOL StartScanner(DWORD StartIp, DWORD EndIp, DWORD StartPort, DWORD EndPort);