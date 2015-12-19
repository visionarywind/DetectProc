
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
	�����������ṹ����
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
	���������ִ�к���
	pBuf -	������ı��������ڴ�ָ��
	size -	������ı��������
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
	����������湦�ܳ�ʼ������
*/
void DetectCmdProcInit( void )
{	
    //���������ṹ���ܳ�ʼ������
    DetectCmdMsgQCacheInit();

}

/*
	��鵱ǰ�Ƿ��б������������û����������ʱ������һ��Ĭ�ϵ�ȫ������������
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
    {	//��ѭ��
    	if ( !noPeriod )
        {	//��������
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
	�����������
	----------------
	�ӱ���������Ϣ����DetectMsgQCache����ȡ����������д���
	��Ϊ������ַ����Ѿ����˴������е���Ϣ��Ϊ��Ҫ���������
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



