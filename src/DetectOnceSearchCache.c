
#include "..\Header.h"


/*
	被动一次搜索缓存结构初始化函数
	------------------------------
	pSearchCache -	待初始化的被动一次搜索缓存指针
*/
void DetectOnceSearchCacheInitialize( 
					struct DetectOnceSearchCache *pDOSCache 
									)
{
	ListHeadInitialize( &pDOSCache->listHead );
    pDOSCache->calcs =	0;
    DetectOnceSearchCacheStatusSet( pDOSCache, FALSE );
}

void DetectOnceSearchCacheReset(
				struct DetectOnceSearchCache *pSearchCache 
								)
{
	assert( NULL != pSearchCache );
    printlog( REC, "DetectOnceSearchCacheReset : pSearchCache=%p\n", pSearchCache );

    DetectOnceSearchCacheClear( pSearchCache );
}



/*
	被动一次搜索缓存结构清空函数
	----------------------------
	pSearchCache -	待初始化的被动一次搜索缓存指针
*/
void DetectOnceSearchCacheClear( 
				struct DetectOnceSearchCache *pDOSCache 
								)
{
	assert( NULL != pDOSCache );
	printlog( REC, "DetectOnceSearchCacheClear : pDOSCache=%p\n", pDOSCache );
    
    DetectSearchAreaCacheListClear( &pDOSCache->listHead );
    pDOSCache->calcs =	0;
    DetectOnceSearchCacheStatusSet( pDOSCache, TRUE );
}

/*
	向被动一次搜索缓存结构插入一个搜索区域
	--------------------------------------
	pDOSCache -	待插入的被动一次搜索缓存指针
	pDSArea -	被动搜索区域指针
*/
void DetectOnceSearchCacheInsert(
				struct DetectOnceSearchCache *pDOSCache,
				struct DetectSearchArea *pDSArea
								)
{
	struct ListHead *pHead =	NULL;
    struct ListHead *pNode =	NULL;
	assert( NULL != pDOSCache );
    assert( NULL != pDSArea );

	pHead =	&pDOSCache->listHead;
    pNode =	&pDSArea->listHead;
    
    ListHeadTailInsert( pHead, pNode );
    DetectOnceSearchCacheAdd( pDOSCache );
    DetectOnceSearchCacheStatusSet( pDOSCache, TRUE );
}								

BOOL __DetectPilotSearchCmdAdd(
    				struct DetectOnceSearchCache *pDOSCache,
    				struct DetectPilotSearchCmd *pDPSCmd 
    							)
{
    struct DetectSearchArea *pNewArea =	NULL;
	pNewArea =	DetectSearchAreaCacheGet();

    printlog( LOG, "__DetectPilotSearchCmdAdd : pDOSCache=%p\n", pDOSCache );
    
    if ( NULL != pNewArea )
    {
    	DetectPilotSearchAreaGetByCmd( pNewArea, pDPSCmd );
        printlog( LOG, "\tSearchArea[pilot]: angle=%f\n", pNewArea->startAngle );
        DetectOnceSearchCacheInsert( pDOSCache, pNewArea );
        printlog( SCR, "DetectOnceSearchCacheInsert\n" );
        return TRUE;
    }
    else
    {
    	printfSafe( "DetectOnceSearchCmdProc failed!\n" );	
    }
    return FALSE;
}    							

/*
	被动一次搜索命令处理函数
	------------------------
	pDOSCache -	被动命令缓存结构指针
	pBuf -	命令缓存指针
	size -	命令长度
*/
BOOL DetectPilotSearchCmdProc( 
			    struct DetectOnceSearchCache *pDOSCache,
				unsigned char *pBuf, 
				int size
							)
{
    struct DetectPilotSearchCmd *pDPSCmd =	NULL;
    
	assert( NULL != pDOSCache );
    assert( NULL != pBuf );

    pDPSCmd =	( struct DetectPilotSearchCmd * )pBuf;
    printlog( REC, "DetectPilotSearchCmdProc : pDOSCache=%p\n", pDOSCache );

	return __DetectPilotSearchCmdAdd( pDOSCache, pDPSCmd );    
}

struct DetectSearchArea *DetectOnceSearchCacheNodeGet(
				struct DetectOnceSearchCache	*pDOSCache
													)
{
	struct ListHead *pHead =	NULL;

    assert( NULL != pDOSCache );
    pHead =	&pDOSCache->listHead;
	if ( !IsListHeadEmpty( pHead ) )
    {
		struct ListHead *pLHPtr =	ListHeadRearGet( pHead );
        struct DetectSearchArea *pDSArea =	GetDetectSearchAreaPointer( pLHPtr );
		ListHeadDelete( pLHPtr->pPrev, pLHPtr->pNext );
        DetectOnceSearchCacheStatusSet( pDOSCache, TRUE );
        
        return pDSArea;
    }
    return NULL;
}													




