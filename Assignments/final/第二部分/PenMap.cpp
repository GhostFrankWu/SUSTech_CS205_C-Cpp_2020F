#include "Header.h"
#pragma once
#pragma comment(lib,"WS2_32.lib")//winsocket依赖项
int threadRegister = 0;
//////////////////////////////////////////////////////////////////////////
//端口器扫描初始化
int main(int argc, char* argv[]) {
	string startIp, endIp;
	DWORD* DStartIp = new DWORD, * DEndIp = new DWORD;
	int startPort, endPort;
	if (argc==5) {//命令行执行
		startIp = argv[1];
		endIp = argv[2];
		startPort = atoi(argv[3]);
		endPort = atoi(argv[4]);
	}
	else {//双击执行
		cout << "请输入起始ip：";
		cin >> startIp;
		cout << "请输入结束ip：";
		cin >> endIp;
		cout << "请输入起始端口：";
		cin >> startPort;
		cout << "请输入结束端口：";
		cin >> endPort;
	}
	if (StrAddr2Dword(startIp, DStartIp) && StrAddr2Dword(endIp, DEndIp)
		&& endPort >= startPort && !(endPort >> 15) && !(startPort >> 15)) {
		threadRegister = (*DEndIp - *DStartIp + 1) * (endPort - startPort + 1);//初始化等待
		StartScanner(*DStartIp, *DEndIp, (DWORD)startPort, (DWORD)endPort);//执行扫描
	}
	else {
		cout << "输入有误，请检查" << endl;
	}
	while (threadRegister) {//类信号量等待扫描结束
		Sleep(1000);
		if (threadRegister == 1)break;
	}
	cout << "扫描完毕" << endl;
	return 0;
}

BOOL StrAddr2Dword(string lpStrAddr, DWORD* pdwAddr){//字符串转端口，修改自CSDN
	char* lpPos = (char*)lpStrAddr.c_str();     //指向当前操作的字符
	int nNumCount = 0;              //数字的个数
	int nPointCount = 0;           //分隔点的个数
	BOOL bResult = FALSE;         //函数是否执行成功
	DWORD dwAddr = 0;
	DWORD Temp = 0;
	if (lpStrAddr=="" || !pdwAddr){//进行参数的有效性检查
		return bResult;
	}
	while (*lpPos && nPointCount < 4){
		if (nNumCount < 3){
			if (*lpPos >= 0x30 && *lpPos <= 0x39){
				bResult = TRUE;
				dwAddr -= Temp;
				Temp *= 10;
				Temp += (*lpPos - 0x30);
				if (Temp & ~0xFF){
					break;   //从这里退出说明单个数超过了255,
				}
				dwAddr += Temp;
				nNumCount++;
			}
			else if (*lpPos != '.'){
				break;     //从这里退出说明有非数字的字符
			}
			else{
				if (!nNumCount){
					break;   //从这里退出说明单个数超过了255,或者是在一个分组中第个字符为点
				}
				else{
					bResult = FALSE;
					Temp = 0;
					dwAddr <<= 8;
					nNumCount = 0;
					nPointCount++;
				}
			}
		}
		else{
			if (*lpPos != '.'){
				break;      //单组数字字符超过了3个
			}
			else{
				bResult = FALSE;
				Temp = 0;
				dwAddr <<= 8;
				nNumCount = 0;
				nPointCount++;
			}
		}
		lpPos++;
	}
	if (!(*lpPos) && nPointCount == 3 && bResult){
		*pdwAddr = dwAddr;
	}
	else{
		bResult = FALSE;
	}
	return bResult;
}

BOOL InitPortScan(){
	WSADATA WsaData;
	WORD WsaVersion = MAKEWORD(2, 2);//构建socket版本信息
	if (WSAStartup(WsaVersion, &WsaData) != 0){//初始化网络
		MessageBoxA(NULL, "WSAStartup fail;", NULL, NULL);
		return FALSE;
	}
	return TRUE;
}

HANDLE hMutex = NULL;//打印互斥量
//////////////////////////////////////////////////////////////////////////
//connect线程函数
DWORD WINAPI PortScanthread(LPVOID LpParam){
	ThreadParam Param;
	MoveMemory(&Param, LpParam, sizeof(Param));//可以进行下一次循环
	SetEvent(Param.hNext);
	SOCKET Sock;
	SOCKADDR_IN SockAddr = { 0 };
	Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建socket
	if (Sock == INVALID_SOCKET){
		MessageBoxA(NULL, "INVALID_SOCKET", NULL, NULL);//创建失败
	}
	SockAddr.sin_family = AF_INET;//填充IP地址及端口信息
	SockAddr.sin_addr.s_addr = htonl(Param.Ip);
	SockAddr.sin_port = htons(Param.Port);
	char* IpChar = inet_ntoa(SockAddr.sin_addr);//将IP地址转换为字符串，这个转换是正常的
	char str[200];
	if (connect(Sock, (SOCKADDR*)&SockAddr, sizeof(SockAddr)) == 0){
		sprintf(str, "%s\t: %d 连接成功\n", IpChar, Param.Port);
		WaitForSingleObject(hMutex, INFINITE);
		cout << str;
		threadRegister--;
		ReleaseMutex(hMutex);//释放互斥量锁
	}
	else{
		sprintf(str, "%s\t: %d 连接失败\n", IpChar, Param.Port);
		WaitForSingleObject(hMutex, INFINITE);
		//cout << str; 失败不输出
		threadRegister--;
		ReleaseMutex(hMutex);//释放互斥量锁
	}
	ReleaseSemaphore(Param.hThreadNum, 1, NULL);//信号量计数-1
	closesocket(Sock);
	return 0;
}
//////////////////////////////////////////////////////////////////////////
//扫描主线程
DWORD WINAPI MainThread(LPVOID LpParam){
	MainThreadParam Param;//将参数复制
	MoveMemory(&Param, LpParam, sizeof(Param));//将Param.hCopyEvent设置为有信号状态
	SetEvent(Param.hCopyEvent);
	ThreadParam threadparam = { 0 };
	HANDLE hThreadCopyOkEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//创建子线程的初始化完成
	threadparam.hNext = hThreadCopyOkEvent;
	HANDLE hThreadNum = CreateSemaphore(NULL, 256, 256, NULL);//信号量控制总线程不超过256
	threadparam.hThreadNum = hThreadNum;
	for (DWORD Ip = Param.StartIp; Ip <= Param.EndIp; Ip++){
		for (DWORD Port = Param.StartPort; Port <= Param.EndPort; Port++){
			//等待直到当前总线程数小于256
			DWORD WaitRes = WaitForSingleObject(hThreadNum, 200);//等待200微秒
			if (WaitRes == WAIT_OBJECT_0){
				threadparam.Ip = Ip;
				threadparam.Port = Port;
				CreateThread(NULL, 0, PortScanthread, &threadparam, 0, NULL);//创建子线程
				WaitForSingleObject(threadparam.hNext, INFINITE);//等待其子线程初始化完成
				ResetEvent(threadparam.hNext);//重置threadparam.hNext为无信号状态
			}
			else if (WaitRes == WAIT_TIMEOUT){//超时
				Port--;
				continue;
			}
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//开始扫描
BOOL StartScanner(DWORD StartIp, DWORD EndIp, DWORD StartPort, DWORD EndPort){
	InitPortScan();//初始化端口扫描器
	MainThreadParam param;//主扫描线程参数
	HANDLE hMainCopyEvent = CreateEvent(NULL, TRUE, FALSE, NULL);//主扫描线程初始化完成
	//填充参数结构体
	param.hCopyEvent = hMainCopyEvent;
	param.StartIp = StartIp;
	param.EndIp = EndIp;
	param.StartPort = StartPort;
	param.EndPort = EndPort;
	CreateThread(NULL, 0, MainThread, (LPVOID*)&param, 0, NULL);//创建主扫描线程参数
	WaitForSingleObject(hMainCopyEvent, INFINITE);//等待hMainCopyEvent变为有信号状态
	ResetEvent(hMainCopyEvent);//重置hMainCopyEvent为无信号状态
	return TRUE;
}



