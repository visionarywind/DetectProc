

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
	���������ṹָ���ȡ����
*/
static inline
struct DetectCmdMsgQCache *DetectCmdMsgQCacheInstanceGet( void )
{
	return &gDetectCmdMsgQCacheInstance;
}

/*
	���������ṹ��Ϣ����ID��ȡ����
*/
static inline
MSG_Q_ID DetectCmdMsgQCacheMsgQIdGet( void )
{
	struct DetectCmdMsgQCache *pInstance =	DetectCmdMsgQCacheInstanceGet();
	return pInstance->msgQCache;
}

/*
	��ʼ�����������ṹ
	pMsgQCache -	����ʼ���ı��������ṹָ��
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
	�򱻶������ṹ��������
	pSendBuf -	�����������ָ��
	size -	���������ݵĳ���
*/
STATUS DetectCmdMsgQCacheSend( unsigned char *pSendBuf, int size )
{
	MSG_Q_ID msgQId =	DetectCmdMsgQCacheMsgQIdGet();
    return msgQSend( msgQId, pSendBuf, size, WAIT_FOREVER, MSG_PRI_NORMAL );
}

/*
	�ӱ��������ṹȡ��һ������
	pRecvBuf -	���ȡ�����ݵ��ڴ�ָ��
	size -	��ȡ�����ݵĳ���
	
*/
int DetectCmdMsgQCacheRecv( unsigned char *pRecvBuf, int size )
{
	MSG_Q_ID msgQId =	DetectCmdMsgQCacheMsgQIdGet();
    return msgQReceive( msgQId, pRecvBuf, size, WAIT_FOREVER );
}

/*
	�ӱ��������ṹȡ����ǰ��ŵ�������Ŀ
*/
int DetectCmdMsgQCacheSizeGet( void )
{
	MSG_Q_ID msgQId =	DetectCmdMsgQCacheMsgQIdGet();
	return msgQNumMsgs( msgQId );
}

////////////////////////////////////////////////////////////////////////////////
/*
	���������ṹ���ܳ�ʼ������
*/
void DetectCmdMsgQCacheInit( void )
{
	struct DetectCmdMsgQCache *pInstance =	DetectCmdMsgQCacheInstanceGet();
    DetectCmdMsgQCacheInitialize( pInstance );
}



