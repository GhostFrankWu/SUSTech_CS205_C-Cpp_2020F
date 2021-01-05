#pragma once
//命令个数
#define IDNUM 100
//NAME字节数
#define NAMESIZE 80
enum RATID{CMD_REQUSET,CMD_REPLY,DESK_VIEW};

//协议
struct RatProto{
	RATID RatId;		//命令ID
	int	  RatCsm;		//密钥
	int	  RatLen;		//数据部分长度
};