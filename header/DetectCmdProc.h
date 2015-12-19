
/*
	���������ͷ�ļ�
	------------------
	author:	jsf
*/
#ifndef __DETECT_CMD_PROC_HEADER__
#define __DETECT_CMD_PROC_HEADER__


/*
	�����������ָ��
*/
typedef BOOL ( *pFn )( unsigned char *, int );

/*
	�����������ṹ
*/
struct DetectCmdProcFilter
{
	unsigned short cmdId;	//�����
    pFn	pFuncProc;			//�������
};

/*
	����������湦�ܳ�ʼ������
*/
EXPORT
void DetectCmdProcInit( void );


/*
	�����������
	----------------
	�ӱ���������Ϣ����DetectMsgQCache����ȡ����������д���
	��Ϊ������ַ����Ѿ����˴������е���Ϣ��Ϊ��Ҫ���������
*/
EXPORT
void DetectCmdProc( void );

EXPORT
BOOL IsDetectProcCmd( unsigned short id );    


#endif

