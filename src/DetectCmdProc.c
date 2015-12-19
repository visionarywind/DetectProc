
#include "..\Header.h"


static unsigned short	detectCmdIdFilter[] =
{
	0x0d86, 0x0d87, 0x0d89, 0x0d8a
};

BOOL IsDetectProcCmd( unsigned short id )
{
	int size =	GetArraySize( detectCmdIdFilter );
	int i =	0;
    for ( i=0; i<size; ++i )
    {
    	if ( id == detectCmdIdFilter[i] )
        {
        	return TRUE;
        }
    }
    return FALSE;
}


/*
	被动命令处理定义结构数组
*/
static struct DetectCmdProcFilter detectCmdProcFilter[] =
	    {
	    	{ 0x0d86, DetectScheduleCycleSearchCmdProc },
	        { 0x0d87, DetectScheduleCycleSearchCmdProc },
	        { 0x0d89, DetectScheduleTargetSearchCmdProc },
	        { 0x0d8a, DetectSchedulePilotSearchCmdProc },
	        { 0x2, DetectNavigationInfoCmdProc },
	    };

/*
	被动命令处理执行函数
	pBuf -	待处理的被动命令内存指针
	size -	待处理的被动命令长度
*/
static BOOL DetectSearchCmdProcedure( unsigned char *pBuf, int size )
{
	struct ExternalHead *pHead =	NULL;
    unsigned short cmdId =	0;
    int i =	0;
    int filterSize =	GetArraySize(detectCmdProcFilter);	
    BOOL ret =	FALSE;

    assert( NULL != pBuf );

    pHead =	( struct ExternalHead * )pBuf;
    cmdId =	pHead->id;

    for ( i=0; i<filterSize; ++i )
    {
    	if ( cmdId == detectCmdProcFilter[i].cmdId )
        {
        	ret =	detectCmdProcFilter[i].pFuncProc( pBuf, size );
        	break;
        }
    }

    return ret;
}

/*
	被动命令处理缓存功能初始化函数
*/
void DetectCmdProcInit( void )
{	
    //被动命令缓冲结构功能初始化函数
    DetectCmdMsgQCacheInit();

}

/*
	检查当前是否有被动搜索命令，当没有搜索命令时，插入一个默认的全空域搜索命令
*/	
static __attribute__((unused))
void __DetectCmdCheck( void )
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
    struct DetectSearchCmdCache *pDSCCache =	&pDSchedule->cmdCache;

    int empty =	DetectSearchCmdCacheEmpty( pDSCCache );
    BOOL noCycle =	BIT_TEST( empty, 0 );
    BOOL noPilot =	BIT_TEST( empty, 1 );
    BOOL noPeriod =	BIT_TEST( empty, 2 );
    printlog( REC, "__DetectCmdCheck noCmd=%d, noCycle=%d, noPilot=%d, noPeriod=%d\n", 
        			empty, noCycle, noPilot, noPeriod );

    if ( noCycle )
    {
    	if ( noPilot && noPeriod )
        {
        	DetectCycleSearchCmdMonitorFullArea();
        	DetectCmdProc();
        }
    }
    else
    {	//有循环
    	if ( !noPeriod )
        {	//有周期性
        	BOOL isMonitor =	IsDetectCycleSearchCmdMonitorCmd( &pDSCCache->cycleSearch );
            if ( isMonitor )
            {
            	DetectCycleSearchCmdMonitorFullAreaExit();
        		DetectCmdProc();
            }
        }
    }
}    

/*
	被动命令处理函数
	----------------
	从被动缓冲消息队列DetectMsgQCache中提取被动命令进行处理，
	因为在命令分发中已经做了处理，所有的消息均为需要处理的命令
*/
void DetectCmdProc( void )
{
	unsigned char buf[DETECT_CMD_MSG_Q_CACHE_LENGTH];
	int msgNums =	DetectCmdMsgQCacheSizeGet();
    int i =	0;
    BOOL isUpdate =	FALSE;
    for ( i=0; i<msgNums; ++i )
    {
    	int wantedSize =	DETECT_CMD_MSG_Q_CACHE_LENGTH;
        int recvSize =	DetectCmdMsgQCacheRecv( buf, wantedSize );
        if ( ERROR != recvSize )
        {
			isUpdate |=	DetectSearchCmdProcedure( buf, recvSize );
        }
    }

    if ( isUpdate )
    {
    	DetectSearchCmdFeedBackProc();
    }

//    __DetectCmdCheck();
}



