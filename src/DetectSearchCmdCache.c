
#include "..\Header.h"

////////////////////////////////////////////////////////////////////////////////


/*
	被动命令缓存初始化函数
	pCmdCache -	待初始化的被动命令缓存指针
*/
void DetectSearchCmdCacheInitialize( 
						struct  DetectSearchCmdCache *pInstance
									)
{
	assert( NULL != pInstance );
	printlog( REC, "DetectSearchCmdCacheInitialize : pInstance=%p\n", pInstance );
    
	DetectCycleSearchCacheInitialize( &pInstance->cycleSearch );
    DetectPeriodSearchCacheInitialize( &pInstance->targetSearch );
    DetectOnceSearchCacheInitialize( &pInstance->pilotSearch );
    
    pInstance->sync =	semBCreate( SEM_Q_FIFO, SEM_FULL );
    pInstance->state =	TRUE;
}


int DetectSearchCmdCacheEmpty(
		struct DetectSearchCmdCache *pDSCCache
								)    
{
	struct DetectCycleSearchCache *pDCSCache =	NULL;
    struct DetectOnceSearchCache *pDOSCache =	NULL;
    struct DetectPeriodSearchCache *pDPSCache =	NULL;
    int ret =	0;
    BOOL noCycle =	FALSE;
    BOOL noPilot =	FALSE;
    BOOL noPeriod =	FALSE;

    assert( NULL != pDSCCache );

	pDCSCache =	&pDSCCache->cycleSearch;
    pDOSCache =	&pDSCCache->pilotSearch;
    pDPSCache =	&pDSCCache->targetSearch;

    noCycle =	DetectCycleSearchCacheEmpty( pDCSCache );
	noPilot = DetectOnceSearchCacheEmpty( pDOSCache );
	noPeriod = DetectPeriodSearchCacheEmpty( pDPSCache );

	if ( noCycle )
    {
    	BIT_SET( ret, 0 );
    }
    if ( noPilot )
    {
    	BIT_SET( ret, 1 );
    }
    if ( noPeriod )
    {
    	BIT_SET( ret, 2 );
    }
    
	return	ret;
}


/*
	被动命令缓存重置函数
	pCmdCache -	待重置的被动命令缓存指针
*/
void DetectSearchCmdCacheReset( 
					struct  DetectSearchCmdCache *pInstance
							)
{
	assert( NULL != pInstance );
	printlog( REC, "DetectSearchCmdCacheReset : pInstance=%p\n", pInstance );

    DetectCycleSearchCacheReset( &pInstance->cycleSearch );
    DetectPeriodSearchCacheReset( &pInstance->targetSearch );
    DetectOnceSearchCacheReset( &pInstance->pilotSearch );
}





