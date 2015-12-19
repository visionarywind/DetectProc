
#include "..\Header.h"

/*
	被动循环搜索缓存结构初始化函数
	pSearchCache -	待初始化的被动循环搜索缓存指针
*/
void DetectCycleSearchCacheInitialize( 
			struct DetectCycleSearchCache *pSearchCache 
									)
{
	assert( NULL != pSearchCache );
    
	memset( pSearchCache->area, 0, 
        		sizeof( struct DetectSearchArea )*DETECT_SEARCH_CMD_CYCLE_MAX_NUM );
    memset( pSearchCache->color, 0, sizeof( BOOL )*DETECT_SEARCH_CMD_CYCLE_MAX_NUM );
	DetectCycleSearchCacheStatusSet( pSearchCache, FALSE );
}

BOOL DetectCycleSearchCacheEmpty(
			struct DetectCycleSearchCache *pDCSCache
								)
{
	int i =	0;

    assert( NULL != pDCSCache );

    for ( i=0; i<DETECT_SEARCH_CMD_CYCLE_MAX_NUM; ++i )
    {
    	if ( pDCSCache->color[i] )
        {
        	return FALSE;
        }
    }
    return TRUE;
}


void DetectCycleSearchCacheClear(
					struct DetectCycleSearchCache *pDCSCache
								)
{
	assert( NULL != pDCSCache );

//    printlog( REC, "DetectCycleSearchCacheClear : pDCSCache=%p\n", pDCSCache );
	DetectCycleSearchCacheInitialize( pDCSCache );
}

void DetectCycleSearchCacheReset(
					struct DetectCycleSearchCache *pDCSCache
								)
{
	assert( NULL != pDCSCache );

//    printlog( REC, "DetectCycleSearchCacheReset : pDCSCache=%p\n", pDCSCache );
    DetectCycleSearchCacheClear( pDCSCache );
}


static __attribute__((unused))
void __DetectCycleSearchCmdAddPreProc(
	    			struct DetectCycleSearchCache *pDCSCache
    										)
{
	BOOL isCycle =	IsDetectCycleSearchCmdMonitorCmd( pDCSCache );
    if ( isCycle )
    {
    	DetectCycleSearchCmdMonitorReset( pDCSCache );
    }
}    										

/*
	将被动循环搜索命令增加到被动命令缓存结构中
	------------------------------------------
	pInstance -	被动命令缓存结构指针
	pCycleSearch -	被动循环搜索命令指针
*/
static BOOL __DetectCycleSearchCmdAdd( 
				struct DetectCycleSearchCache *pDCSCache,								
				const struct DetectCycleSearchCmd *pCycleSearch
									)
{
	BOOL ret =	FALSE;
	int areaCode =	-1;

    assert( NULL != pDCSCache );
    assert( NULL != pCycleSearch );

    areaCode =	pCycleSearch->areaCode - 1;
    printlog( LOG, "__DetectCycleSearchCmdAdd : pDCSCache=%p, areaCode=%d\n",
        		pDCSCache, areaCode );

    if ( areaCode > -1 && areaCode < 6 )
    {
    	BOOL color =	FALSE;

//        __DetectCycleSearchCmdAddPreProc( pDCSCache );
        
        color =	pDCSCache->color[areaCode];
        
        if ( !color )
        {
        	struct DetectSearchArea *pSearchArea =	( pDCSCache->area + areaCode );
        	        
        	DetectCycleSearchAreaGetByCmd( pSearchArea, pCycleSearch );
            printlog( LOG, "\tSearchArea[cycle] : start=%f, end=%f\n",
                		pSearchArea->startAngle, pSearchArea->endAngle );

            pDCSCache->color[areaCode] =	TRUE;
            DetectCycleSearchCacheStatusSet( pDCSCache, TRUE );
            ret =	TRUE;
        }
        else
        {
        	//该区间已经设置了，新的命令错误
        	printfSafe( "Error - DetectNormalCmdProc : area was already setted!\n" );
        }
    }
    else
    {
    	printfSafe( "Error - DetectCycleCmdProc areaCode = %d\n",
            		areaCode );
    }

    return ret;
}

/*
	将被动循环搜索命令从被动命令缓存结构中删除
	------------------------------------------
	pInstance -	被动命令缓存结构指针
	pCycleSearch -	被动循环搜索命令指针
*/
static BOOL __DetectCycleSearchCmdRemove( 
					struct DetectCycleSearchCache *pDCSCache,
					const struct DetectCycleSearchCmd *pCycleSearch
										)
{
	BOOL ret =	FALSE;
	short areaCode =	-1;
    
    assert( NULL != pDCSCache );
    assert( NULL != pCycleSearch );

    areaCode =	pCycleSearch->areaCode - 1;
	printlog( LOG, "__DetectCycleSearchCmdRemove : pDCSCache=%p, areaCode=%d\n", 
        			pDCSCache, areaCode );

    if ( areaCode > -1 && areaCode < 6 )
    {
    	pDCSCache->color[ areaCode ] =	FALSE;
    	DetectCycleSearchCacheStatusSet( pDCSCache, TRUE );
	    ret =	TRUE;    	
    }
    else
    {
    	printfSafe( "__DetectCycleSearchCmdRemove failed!\n" );
    }
    
    return ret;
}

/*
	将被动循环搜索命令从被动命令缓存结构中全部删除
	----------------------------------------------
	pInstance -	被动命令缓存结构指针
	pCycleSearch -	被动循环搜索命令指针
	type -	搜索类型
*/
static BOOL __DetectCycleSearchCmdRemoveAll( 
					struct DetectCycleSearchCache *pDCSCache
										)
{
    int i = 0;

    assert( NULL != pDCSCache );
    printlog( LOG, "__DetectCycleSearchCmdRemoveAll : pDCSCache=%p\n", pDCSCache );
    
    for ( i=0; i<DETECT_SEARCH_CMD_CYCLE_MAX_NUM; ++i )
    {
    	pDCSCache->color[ i ] =	FALSE;
    }

    DetectCycleSearchCacheStatusSet( pDCSCache, TRUE );

    return TRUE;
}

/*
	被动正常搜索命令处理函数
	------------------------
	pInstance -	被动命令缓存结构指针
	pBuf -	命令缓存指针
	size -	命令长度
*/
BOOL DetectCycleSearchCmdProc( 
                struct DetectCycleSearchCache *pDCSCache,
                unsigned char *pBuf, 
                int size
                            )
{
	BOOL ret =	FALSE;
	struct DetectCycleSearchCmd *pCycleSearch =	NULL;
    unsigned short operateType =	-1;

	assert( NULL != pDCSCache );
    assert( NULL != pBuf );

    pCycleSearch =	(struct DetectCycleSearchCmd *)pBuf;
	operateType =	pCycleSearch->operateType;

    printlog( SCR, "DetectCycleSearchCmdProc : pDCSCache=%p, operateType=%d\n",
        			pDCSCache, operateType );

	switch ( operateType )
    {
    case DETECT_CYCLE_SEARCH_OPERATE_CREATE:
        ret =	__DetectCycleSearchCmdAdd( pDCSCache, pCycleSearch );
        break;
    case DETECT_CYCLE_SEARCH_OPERATE_REMOVE:
        ret =	__DetectCycleSearchCmdRemove( pDCSCache, pCycleSearch );
        break;
    case DETECT_CYCLE_SEARCH_OPERATE_REMOVEALL:
        ret =	__DetectCycleSearchCmdRemoveAll( pDCSCache );
        break;
    }
    
    return ret;
}

////////////////////////////////////////////////////////////////////////////////


