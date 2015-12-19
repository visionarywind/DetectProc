
#ifndef __DETECT_CMD_MSG_Q_CACHE_HEADER__
#define __DETECT_CMD_MSG_Q_CACHE_HEADER__

/*
	以下是命令接收缓冲部分
*/
#define DETECT_CMD_MSG_Q_CACHE_MAX_SIZE	1000
#define DETECT_CMD_MSG_Q_CACHE_LENGTH	300
/*
	被动命令缓冲结构
*/
struct DetectCmdMsgQCache
{
	MSG_Q_ID	msgQCache;	//消息队列ID 
    int	maxSize;	//最大数目
    int length;		//消息长度

    SEM_ID	sync;
    BOOL	state;	//被动命令缓冲初始化标记
};

/*
	向被动命令缓冲结构放入数据
	pSendBuf -	待放入的数据指针
	size -	待放入数据的长度
*/
EXPORT
STATUS DetectCmdMsgQCacheSend( unsigned char *pSendBuf, int size );

/*
	从被动命令缓冲结构取出一个数据
	pRecvBuf -	存放取出数据的内存指针
	size -	欲取出数据的长度
	
*/
EXPORT
int DetectCmdMsgQCacheRecv( unsigned char *pRecvBuf, int size );

/*
	从被动命令缓冲结构取出当前存放的数据数目
*/
EXPORT
int DetectCmdMsgQCacheSizeGet( void );

////////////////////////////////////////////////////////////////////////////////
/*
	被动命令缓冲结构功能初始化函数
*/
EXPORT
void DetectCmdMsgQCacheInit( void );

#endif

