

#include "..\Header.h"

////////////////////////////////////////////////////////////////////////////////
static struct DetectCmdMsgQCache gDetectCmdMsgQCacheInstance =
    	{
    		.msgQCache =	NULL,
            .maxSize =	DETECT_CMD_MSG_Q_CACHE_MAX_SIZE,
            .length =	DETECT_CMD_MSG_Q_CACHE_LENGTH,
            .sync =	NULL,
            .state =	FALSE,
    	};

/*
	被动命令缓冲结构指针获取函数
*/
static inline
struct DetectCmdMsgQCache *DetectCmdMsgQCacheInstanceGet( void )
{
	return &gDetectCmdMsgQCacheInstance;
}

/*
	被动命令缓冲结构消息队列ID获取函数
*/
static inline
MSG_Q_ID DetectCmdMsgQCacheMsgQIdGet( void )
{
	struct DetectCmdMsgQCache *pInstance =	DetectCmdMsgQCacheInstanceGet();
	return pInstance->msgQCache;
}

/*
	初始化被动命令缓冲结构
	pMsgQCache -	待初始化的被动命令缓冲结构指针
*/
static void DetectCmdMsgQCacheInitialize( struct DetectCmdMsgQCache *pInstance )
{
    int maxSize =	pInstance->maxSize;
    int length =	pInstance->length;
    pInstance->msgQCache =	msgQCreate( maxSize, length, MSG_Q_FIFO );

	pInstance->sync =	semBCreate( SEM_Q_FIFO, SEM_FULL );
    pInstance->state =	TRUE;
}

/*
	向被动命令缓冲结构放入数据
	pSendBuf -	待放入的数据指针
	size -	待放入数据的长度
*/
STATUS DetectCmdMsgQCacheSend( unsigned char *pSendBuf, int size )
{
	MSG_Q_ID msgQId =	DetectCmdMsgQCacheMsgQIdGet();
    return msgQSend( msgQId, pSendBuf, size, WAIT_FOREVER, MSG_PRI_NORMAL );
}

/*
	从被动命令缓冲结构取出一个数据
	pRecvBuf -	存放取出数据的内存指针
	size -	欲取出数据的长度
	
*/
int DetectCmdMsgQCacheRecv( unsigned char *pRecvBuf, int size )
{
	MSG_Q_ID msgQId =	DetectCmdMsgQCacheMsgQIdGet();
    return msgQReceive( msgQId, pRecvBuf, size, WAIT_FOREVER );
}

/*
	从被动命令缓冲结构取出当前存放的数据数目
*/
int DetectCmdMsgQCacheSizeGet( void )
{
	MSG_Q_ID msgQId =	DetectCmdMsgQCacheMsgQIdGet();
	return msgQNumMsgs( msgQId );
}

////////////////////////////////////////////////////////////////////////////////
/*
	被动命令缓冲结构功能初始化函数
*/
void DetectCmdMsgQCacheInit( void )
{
	struct DetectCmdMsgQCache *pInstance =	DetectCmdMsgQCacheInstanceGet();
    DetectCmdMsgQCacheInitialize( pInstance );
}



