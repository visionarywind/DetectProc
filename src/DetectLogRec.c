
#include "..\Header.h"

#define __REMOTE_DEBUG__
#ifdef __REMOTE_DEBUG__
#define LOG_FILE	( "/tgtsvr/log.txt" )
#define REC_FILE	( "/tgtsvr/rec.txt" )
#else
#define LOG_FILE	( "log.txt" )
#define REC_FILE	( "rec.txt" )
#endif

struct DetectLogRec	sDetectLogRecForRec;
struct DetectLogRec	sDetectLogRecForLog;

void DetectLogRecInit( void )
{
	DetectLogRecInitialize( &sDetectLogRecForRec, REC_FILE, "LogRec", 250 );
    DetectLogRecInitialize( &sDetectLogRecForLog, LOG_FILE, "LogLog", 249 );
}

#ifdef __LOG_DEBUG_ONLY__
int LogLog( const char *cFmt, ... )
{
	static char cMsgBuf[1024];
	int ret =	-1;
	va_list vaList;
	int size =	-1;
	va_start(vaList, cFmt);
	size =	vsprintf(cMsgBuf, cFmt, vaList);
	va_end(vaList);

	ret =	msgQSend( sDetectLogRecForLog.msgQId, cMsgBuf, size, NO_WAIT, MSG_PRI_NORMAL );
    if ( ERROR == ret )
    {
    	printf( "LogLog Exception\n" );
    }
    return ret;
}
#else
int LogLog( const char *cFmt, ... )
{
	return -1;
}
#endif

#ifdef __LOG_DEBUG_ONLY__
int LogRec( const char *cFmt, ... )
{
	static char cMsgBuf[1024];
	int ret =	-1;    
	va_list vaList;
	int size =	-1;
	va_start(vaList, cFmt);
	size =	vsprintf(cMsgBuf, cFmt, vaList);
	va_end(vaList);

	ret =	msgQSend( sDetectLogRecForRec.msgQId, cMsgBuf, size, NO_WAIT, MSG_PRI_NORMAL );
    if ( ERROR == ret )
    {
    	printf( "LogRec Exception\n" );
    }
    return ret;
}
#else
int LogRec( const char *cFmt, ... )
{
	return -1;
}
#endif

static void DetectLogRecFunc( struct DetectLogRec *pDLRec )
{
	char buf[1024];
    MSG_Q_ID msgQId =	pDLRec->msgQId;
    FILE *pFile =	pDLRec->pFile;

    if ( NULL == msgQId || NULL == pFile )
    {
    	printf( "DetectLogRecFunc Para Error!\n" );
    }
    
	while ( 1 )
    {
    	int size =	msgQReceive( msgQId, buf, 1024, WAIT_FOREVER );
        if ( ERROR != size )
        {
        	if ( NULL != pFile )
            {
            	fwrite( buf, 1, size, pFile );
	            fflush( pFile );
            }
        }
    }
}

#ifdef __LOG_DEBUG_ONLY__
void DetectLogRecInitialize( 
				struct DetectLogRec *pDLRec, 
				char *pFileName,
				char *pTaskName,
				int priority				
							)
{
	if ( NULL != pDLRec && NULL != pFileName && NULL != pTaskName )
    {
    	pDLRec->msgQId =	msgQCreate( LOG_REC_MAX_SIZE, LOG_REC_MAX_LENGTH, WAIT_FOREVER );
        pDLRec->pFile =	fopen( pFileName, "w+" );
        pDLRec->taskId =	taskSpawn( pTaskName, priority, VX_FP_TASK, 10240,
            							(FUNCPTR)DetectLogRecFunc, 
            							(int)pDLRec, 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    }
    else
    {
    	printf( "DetectLogRecInitialize ERROR!\n" );
    }
}
#else
void DetectLogRecInitialize(
				struct DetectLogRec *pLDRec,
				char *pFileName,
				char *pTaskName,
				int priority
							)
{
}
#endif

void DetectRecLog( int log, unsigned short *pBuf, int size )
{
	int i =	0;
    for ( i=0; i<size; ++i )
    {
    	printlog( log, "%hx ", pBuf[i] );
    }
    printlog( log, "\n" );
}

