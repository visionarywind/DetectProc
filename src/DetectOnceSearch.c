
#include "..\Header.h"

/*
	被动一次搜索结构初始化函数
	------------------------------
*/
void DetectOnceSearchInitialize( 
					struct DetectOnceSearch *pOnceSearch,
					struct DetectOnceSearchCache *pSearchCache
								)
{
	assert( NULL != pOnceSearch );
    ListHeadInitialize( &pOnceSearch->list );
    pOnceSearch->pSearchCache =	pSearchCache;
}
/*
	被动一次搜索结构数据清除函数
	----------------------------
	pOnceSearch -	待清除的被动一次搜索结构指针
*/
void DetectOnceSearchClear( 
				struct DetectOnceSearch *pOnceSearch
							)
{
	assert( NULL != pOnceSearch );
    DetectSearchNodeCacheListClear( &pOnceSearch->list );
}		


/*
	被动一次搜索结构任务引导结点形成函数
*/
static BOOL __DetectOnceSearchExplain(
							struct ListHead *pPilotList,
							const struct DetectSearchArea *pSearchArea
										)
{
#define	DETECT_ONCE_SEARCH_PILOT_EXPLAIN_NUMBER	( 5 )
	struct DetectSearchNode * pArr[DETECT_ONCE_SEARCH_PILOT_EXPLAIN_NUMBER];
	int size =	DETECT_ONCE_SEARCH_PILOT_EXPLAIN_NUMBER;
    BOOL ret =	DetectSearchNodeCacheArrInit( pArr, size );

    printlog( REC, "__DetectOnceSearchExplain\n" );

    if ( ret )
    {
    	int i =	0;
        for ( i=0; i<size; ++i )
        {
        	float step =	-4.0+DETECT_SCHEDULE_STEP*i;
            struct DetectSearchNode *pDSNode =	pArr[i];
			DetectSearchNodeExtract( pDSNode, pSearchArea, step );
        }
        for ( i=0; i<size; ++i )
        {
        	struct ListHead *pNode =	&pArr[i]->listHead;
        	ListHeadTailInsert( pPilotList, pNode );
        }
    }

    return ret;
}																	


/*
	被动一次搜索结构 结点形成函数	
	-----------------------------
	Note :  任务引导一次形成一个结点的相关信息，
			并设置相应的命令缓存的状态标志
*/
void DetectOnceSearchUpdate(
			struct DetectOnceSearch *pDOSearch
							)
{
	struct ListHead *pSearchHead =	NULL;
    struct DetectSearchArea *pDSArea =	NULL;
    
	assert( NULL != pDOSearch );
    printlog( REC, "DetectOnceSearchUpdate\n" );

	pSearchHead =	&pDOSearch->list;
	pDSArea =	DetectOnceSearchCacheNodeGet( pDOSearch->pSearchCache );

    if ( NULL != pDSArea )
    {
    	__DetectOnceSearchExplain( pSearchHead, pDSArea );
        DetectSearchAreaCachePut( pDSArea );
    }
	else
    {
    	printfSafe( "DetectOnceSearchUpdate failed\n" );
    }
}									


/*
	被动一次搜索结构 被动搜索区间节点 获取函数
*/
struct DetectSearchNode * DetectOnceSearchNodeGet( 
							struct DetectOnceSearch *pDOSearch
													)
{
	struct ListHead *pHead =    NULL;
    assert( NULL != pDOSearch );
	pHead =	&pDOSearch->list;
    if ( !IsListHeadEmpty( pHead ) )
    {
    	struct ListHead *pLHPtr =	ListHeadRearGet( pHead );
    	struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLHPtr ); 
    	ListHeadDelete( pLHPtr->pPrev, pLHPtr->pNext );
        
        return pDSNode;
    }
    return NULL;
}													


