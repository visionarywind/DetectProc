
#ifndef __DETECT_LOG_REC_HEADER__
#define __DETECT_LOG_REC_HEADER__

#define EXPORT extern

#define LOG_REC_MAX_SIZE	( 8*1024 )
#define LOG_REC_MAX_LENGTH	( 1024 )


struct DetectLogRec
{
	MSG_Q_ID	msgQId;
    FILE 		*pFile;
    int 		taskId;
};

EXPORT
void DetectLogRecInitialize( 
				struct DetectLogRec *pDLRec, 
				char *pFileName,
				char *pTaskName,
				int priority
							);

#define __LOG_DEBUG_ONLY__

EXPORT
void DetectLogRecInit( void );
EXPORT
int LogRec( const char *fmt, ... );
EXPORT 
int LogLog( const char *fmt, ... );

//#define _DLR_DBG_
#ifdef _DLR_DBG_
#define SCR	( 0	)
#define REC ( 2 )
#define LOG	( 2 )
#else
#define SCR	( 0 )
#define REC ( 1 )
#define LOG	( 2 )
#endif

#ifdef __LOG_DEBUG_ONLY__
#define printlog( level, fmt... )  	if ( 2 == level ){ LogLog(##fmt); } else if ( 1 == level ){ LogRec(##fmt); } else { printf(##fmt); }
#else
#define printlog( level, fmt... )	recMsg( ##fmt )
#endif

//#include "usrDefs.h"
//#define recMsg	printf
EXPORT
void DetectRecLog( int log, unsigned short *pBuf, int size );


#endif

