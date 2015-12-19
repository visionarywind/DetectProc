
#include "..\Header.h"


////////////////////////////////////////////////////////////////////////////////
/*
	被动搜索区域缓存实例
*/
static struct DetectSearchAreaCacheBuffer gDetectSearchAreaCacheBufferInstance =
    	{
    		.pBuffer =	NULL,
            .usedSize =	0,
            .maxSize =	DETECT_SEARCH_AREA_CACHE_BUFFER_SIZE,
            .sync =	NULL,
            .freeList =	{ 0 },

            .state =	FALSE,
    	};

/*
	初始化被动搜索区域缓存结构
	pCacheBuffer -	待初始化的被动搜索区域缓存结构指针
*/
static void DetectSearchAreaCacheBufferInitialize( 
								struct DetectSearchAreaCacheBuffer *pInstance
												)
{
	int i =	0;
	int size =	pInstance->maxSize;
	int mallocSize =	size * sizeof( struct DetectSearchArea );
	pInstance->sync =	semBCreate(SEM_Q_FIFO, SEM_FULL);
    pInstance->pBuffer =	(struct DetectSearchArea *)malloc( mallocSize );

	//以下建立链表间的链接关系
    //处理开始节点
    pInstance->pBuffer[0].listHead.pPrev =	NULL;
    pInstance->pBuffer[0].listHead.pNext =   
        				&(pInstance->pBuffer[1].listHead);

    for ( i=1; i<size-1; ++i )
    {
    	pInstance->pBuffer[i].listHead.pPrev =
            			&(pInstance->pBuffer[i-1].listHead);
        pInstance->pBuffer[i].listHead.pNext =
            			&(pInstance->pBuffer[i+1].listHead);
    }
    //处理最后一个节点
    pInstance->pBuffer[size-1].listHead.pPrev =
    					&(pInstance->pBuffer[size-2].listHead);
    pInstance->pBuffer[size-1].listHead.pNext =	
        				&(pInstance->pBuffer[0].listHead);

	//更新空闲链表
    ListHeadInitialize( &pInstance->freeList );
    ListHeadRearInsert( &pInstance->pBuffer[0].listHead, &pInstance->freeList );

    pInstance->state =	TRUE;
}

/*
	被动搜索区域缓存结构获取函数
*/
static struct DetectSearchAreaCacheBuffer *
								DetectSearchAreaCacheBufferInstanceGet( void )
{
	return &gDetectSearchAreaCacheBufferInstance;
}
/*
	被动搜索区域缓存计数+1
*/
static inline
void DetectSearchAreaCacheBufferAdc( void )
{	
	struct DetectSearchAreaCacheBuffer *pInstance =	
    									DetectSearchAreaCacheBufferInstanceGet();
 	if ( pInstance->usedSize > pInstance->maxSize )
	{
		printfSafe("Error - DetectSearchAreaCacheBufferAdc : usedSize > maxSize!\n");
	}
    pInstance->usedSize ++;
    printlog( REC, "DetectSearchAreaCache Adc used size=%d\n", pInstance->usedSize );
}
/*
	被动搜索区域缓存计数-1
*/
static inline
void DetectSearchAreaCacheBufferDec( void )
{
	struct DetectSearchAreaCacheBuffer *pInstance =	
    									DetectSearchAreaCacheBufferInstanceGet();
	if ( pInstance->usedSize < 0 )
    {
    	printfSafe("Error - DetectSearchAreaCacheBufferDec : usedSize < 0!\n");
    }
    pInstance->usedSize --;
    printlog( REC, "DetectSearchAreaCache Dec used size=%d\n", pInstance->usedSize );
}
/*
	被动搜索区域缓存申请预判
*/
static inline
BOOL DetectSearchAreaCacheBufferPrefetch( void )
{
	struct DetectSearchAreaCacheBuffer *pInstance =	
    									DetectSearchAreaCacheBufferInstanceGet();
	return pInstance->usedSize < pInstance->maxSize;
}

/*
	从被动搜索区域缓存中申请一个节点
*/
struct ListHead *DetectSearchAreaCacheListHeadGet( void )
{
	struct DetectSearchAreaCacheBuffer *pInstance =	
        								DetectSearchAreaCacheBufferInstanceGet();
	STATUS status =	semTake( pInstance->sync, NO_WAIT );
	struct ListHead *pLhRet =	NULL;
    
    if ( OK == status && DetectSearchAreaCacheBufferPrefetch() )
    {
        struct ListHead *pRearNode =	ListHeadRearGet( &pInstance->freeList );
		ListHeadDelete( pRearNode->pPrev, pRearNode->pNext );

		DetectSearchAreaCacheBufferAdc();        
    	semGive( pInstance->sync );
        
        pLhRet =	pRearNode;
    }
    printlog( REC, "DetectSearchAreaCacheListHeadGet : pNode=%p\n", pLhRet );

	if ( NULL == pLhRet )
    {
    	printlog( SCR, "DetectSearchAreaCacheListHeadGet failed!\n" );
    }
    
    return pLhRet;
}

/*
	往被动搜索区域释放一个节点
	pSearchArea -	待释放的被动搜索区域节点链表指针
*/
BOOL DetectSearchAreaCachePut( struct DetectSearchArea *pSearchArea )
{
    if ( NULL != pSearchArea )
    {
		struct ListHead *pListHead =	&( pSearchArea->listHead );
	    return DetectSearchAreaCacheListHeadPut(pListHead);
    }
    return FALSE;    
}


/*
	从被动搜索区域申请一个节点
*/
struct DetectSearchArea *DetectSearchAreaCacheGet( void )
{
	struct ListHead *pListHead =	NULL;

	printlog( REC, "DetectSearchAreaCacheGet\n" );

    pListHead =	DetectSearchAreaCacheListHeadGet();
    if ( NULL != pListHead )
    {
    	struct DetectSearchArea *pRet =	GetDetectSearchAreaPointer(pListHead);
    	return pRet;
    }
    return NULL;
}


/*
	向被动搜索区域缓存中释放一个节点
	pNode -	待释放的被动搜索区域结点链表指针
*/
BOOL DetectSearchAreaCacheListHeadPut( struct ListHead *pNode )
{
	struct DetectSearchAreaCacheBuffer *pInstance =	
        								DetectSearchAreaCacheBufferInstanceGet();
    STATUS status =	semTake( pInstance->sync, NO_WAIT );
    BOOL ret =	FALSE;
    
    if ( OK == status )
    {
        ListHeadTailInsert( &pInstance->freeList, pNode );

        DetectSearchAreaCacheBufferDec();
    	semGive( pInstance->sync );
        ret =	TRUE;
    }
	printlog( REC, "DetectSearchAreaCacheListHeadPut : pNode=%p, ret=%d\n",
        			pNode, ret );
    
    return ret;
}


////////////////////////////////////////////////////////////////////////////////
/*
	被动搜索区域缓存功能初始化函数
*/
void DetectSearchAreaCacheBufferInit()
{
	struct DetectSearchAreaCacheBuffer *pInstance =	
        				DetectSearchAreaCacheBufferInstanceGet();

    DetectSearchAreaCacheBufferInitialize( pInstance );
}



