
/*
	被动命令处理头文件
	------------------
	author:	jsf
*/
#ifndef __DETECT_CMD_PROC_HEADER__
#define __DETECT_CMD_PROC_HEADER__


/*
	被动命令处理函数指针
*/
typedef BOOL ( *pFn )( unsigned char *, int );

/*
	被动命令处理定义结构
*/
struct DetectCmdProcFilter
{
	unsigned short cmdId;	//命令号
    pFn	pFuncProc;			//命令处理函数
};

/*
	被动命令处理缓存功能初始化函数
*/
EXPORT
void DetectCmdProcInit( void );


/*
	被动命令处理函数
	----------------
	从被动缓冲消息队列DetectMsgQCache中提取被动命令进行处理，
	因为在命令分发中已经做了处理，所有的消息均为需要处理的命令
*/
EXPORT
void DetectCmdProc( void );

EXPORT
BOOL IsDetectProcCmd( unsigned short id );    


#endif

