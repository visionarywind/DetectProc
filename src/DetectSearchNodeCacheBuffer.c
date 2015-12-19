
#include "..\Header.h"


/*
	被动搜索节点缓存实例
*/
static struct DetectSearchNodeCacheBuffer gDetectSearchNodeCacheBufferInstance =
		{
			.pBuffer =	NULL,
            .usedSize =	0,
            .maxSize =	DETECT_SEARCH_NODE_CACHE_BUFFER_SIZE,
            .sync =	NULL,
            .freeList =	{ 0 },

            .state =	FALSE,
		};
/*
	初始化被动搜索节点缓冲结构
	pCacheBuffer -	待初始化的被动搜索结点缓冲结构指针
*/
static void DetectSearchNodeCacheBufferInitialize( 
								struct DetectSearchNodeCacheBuffer *pInstance,
								int size
									    		)
{
	int i =	0;
	int mallocSize =	size*sizeof( struct DetectSearchNode );

    assert( NULL != pInstance );

    pInstance->usedSize =	0;
    pInstance->maxSize =	size;
    pInstance->sync =	semBCreate(SEM_Q_FIFO, SEM_FULL);
    pInstance->pBuffer =	(struct DetectSearchNode *)malloc(mallocSize);

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
	获取被动搜索节点缓冲结构指针
*/
static struct DetectSearchNodeCacheBuffer *
								DetectSearchNodeCacheBufferInstanceGet( void )
{
	return &gDetectSearchNodeCacheBufferInstance;
}
/*
	被动搜索结点缓冲计数+1
*/
static void DetectSearchNodeCacheBufferAdc( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
						        		DetectSearchNodeCacheBufferInstanceGet();
    
	if ( pInstance->usedSize > pInstance->maxSize )
	{
		printfSafe("Error - DetectSearchNodeCacheBufferAdc : usedSize > maxSize!\n");
	}
    pInstance->usedSize ++;
    printlog( REC, "DetectSearchNodeCacheBufferAdd usedSize=%d\n",
        		pInstance->usedSize );
}
/*
	被动搜索结点缓冲计数-1
*/
static void DetectSearchNodeCacheBufferDec( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
    									DetectSearchNodeCacheBufferInstanceGet();
	if ( pInstance->usedSize < 0 )
    {
    	printfSafe("Error - DetectSearchNodeCacheBufferDec : usedSize < 0!\n");
    }
    pInstance->usedSize --;
    printlog( REC, "DetectSearchNodeCacheBufferDec usedSize=%d\n", 
        		pInstance->usedSize );
}
/*
	被动搜索结点缓冲申请预判
*/
static BOOL DetectSearchNodeCacheBufferPrefetch( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
    									DetectSearchNodeCacheBufferInstanceGet();
	if ( pInstance->usedSize < 0 )
    {
    	printfSafe("Error - DetectSearchNodeCcheBufferPrefetch : usedSize < 0 !\n");
    }
	return pInstance->usedSize < pInstance->maxSize;
}


////////////////////////////////////////////////////////////////////////////////


/*
	从被动搜索结点缓存预申请若干结点函数
*/
STATUS DetectSearchNodeCachePrefetch( int size )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
        							DetectSearchNodeCacheBufferInstanceGet();
    STATUS status =	semTake( pInstance->sync, NO_WAIT );
    if ( OK == status )
    {
    	int remainder =	pInstance->maxSize - pInstance->usedSize;
        if ( size < remainder )
		{
			status =	ERROR;
		}
        semGive( pInstance->sync );
    }
    return status;
}


/*
	从被动搜索节点缓存中申请一个节点
*/
struct ListHead *DetectSearchNodeCacheListHeadGet( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =	
        								DetectSearchNodeCacheBufferInstanceGet();
	STATUS status =	semTake( pInstance->sync, NO_WAIT );
	struct ListHead *pLhRet =	NULL;
    
    if ( OK == status && DetectSearchNodeCacheBufferPrefetch() )
    {
    	struct ListHead *pRearNode =	ListHeadRearGet( &pInstance->freeList );
        ListHeadDelete( pRearNode->pPrev, pRearNode->pNext );

        DetectSearchNodeCacheBufferAdc();
    	semGive( pInstance->sync );
        
        pLhRet =	pRearNode;
    }
    printlog( REC, "DetectSearchNodeCacheListHeadGet : pNode=%p\n", 
        				pLhRet );

    if ( NULL == pLhRet )
    {
    	printlog( SCR, "DetectSearchNodeCacheListHeadGet failed!\n" );
    }
    
    return pLhRet;	
}

/*
	向被动搜索节点缓存中释放一个节点
*/
BOOL DetectSearchNodeCachePut( 
				struct DetectSearchNode *pSearchNode
							)
{
	struct ListHead *pListHead =	NULL;

    assert( NULL != pSearchNode );

	pListHead =	&pSearchNode->listHead;
    return DetectSearchNodeCacheListHeadPut(pListHead);
}


/*
	从被动搜索节点缓存中申请一个节点
*/
struct DetectSearchNode *DetectSearchNodeCacheGet( void )
{
	struct ListHead *pListHead =	DetectSearchNodeCacheListHeadGet();
    struct DetectSearchNode *pDSNode =	NULL;
    if ( NULL != pListHead )
    {
    	pDSNode =	GetDetectSearchNodePointer(pListHead);
    }

    return pDSNode;
}


/*
	向被动搜索节点缓存中释放一个节点
	pNode -	待释放的被动搜索结点链表指针
*/
BOOL DetectSearchNodeCacheListHeadPut( struct ListHead *pNode )
{
	struct DetectSearchNodeCacheBuffer *pInstance =	
        								DetectSearchNodeCacheBufferInstanceGet();
    STATUS status =	semTake( pInstance->sync, NO_WAIT );
    BOOL ret =	FALSE;
    
    if ( OK == status )
    {
        ListHeadTailInsert( &pInstance->freeList, pNode );

        DetectSearchNodeCacheBufferDec();
    	semGive( pInstance->sync );
        ret =	TRUE;
    }

    printlog( REC, "DetectSearchNodeCacheListHeadPut : pNode=%p, ret=%d\n",
        		pNode, ret );
    
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/*
	被动搜索结点缓存功能初始化函数
*/
void DetectSearchNodeCacheBufferInit( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
        				DetectSearchNodeCacheBufferInstanceGet();
    int size =	DETECT_SEARCH_NODE_CACHE_BUFFER_SIZE;
    DetectSearchNodeCacheBufferInitialize( pInstance, size );
}


