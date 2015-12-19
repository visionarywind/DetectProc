
#ifndef __DETECT_CMD_MSG_Q_CACHE_HEADER__
#define __DETECT_CMD_MSG_Q_CACHE_HEADER__

/*
	������������ջ��岿��
*/
#define DETECT_CMD_MSG_Q_CACHE_MAX_SIZE	1000
#define DETECT_CMD_MSG_Q_CACHE_LENGTH	300
/*
	���������ṹ
*/
struct DetectCmdMsgQCache
{
	MSG_Q_ID	msgQCache;	//��Ϣ����ID 
    int	maxSize;	//�����Ŀ
    int length;		//��Ϣ����

    SEM_ID	sync;
    BOOL	state;	//����������ʼ�����
};

/*
	�򱻶������ṹ��������
	pSendBuf -	�����������ָ��
	size -	���������ݵĳ���
*/
EXPORT
STATUS DetectCmdMsgQCacheSend( unsigned char *pSendBuf, int size );

/*
	�ӱ��������ṹȡ��һ������
	pRecvBuf -	���ȡ�����ݵ��ڴ�ָ��
	size -	��ȡ�����ݵĳ���
	
*/
EXPORT
int DetectCmdMsgQCacheRecv( unsigned char *pRecvBuf, int size );

/*
	�ӱ��������ṹȡ����ǰ��ŵ�������Ŀ
*/
EXPORT
int DetectCmdMsgQCacheSizeGet( void );

////////////////////////////////////////////////////////////////////////////////
/*
	���������ṹ���ܳ�ʼ������
*/
EXPORT
void DetectCmdMsgQCacheInit( void );

#endif

