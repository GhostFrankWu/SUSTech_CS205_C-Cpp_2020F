#pragma once
//�������
#define IDNUM 100
//NAME�ֽ���
#define NAMESIZE 80
enum RATID{CMD_REQUSET,CMD_REPLY,DESK_VIEW};

//Э��
struct RatProto{
	RATID RatId;		//����ID
	int	  RatCsm;		//��Կ
	int	  RatLen;		//���ݲ��ֳ���
};