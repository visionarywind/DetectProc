
#include "..\Header.h"

/*
	被动循环搜索结构初始化函数
	--------------------------
*/
void DetectCycleSearchInitialize(
				struct DetectCycleSearch *pDCSearch,
				struct DetectCycleSearchCache	*pDCSCache
								)
{
	assert( NULL != pDCSearch );
    ListHeadInitialize( &pDCSearch->list );
    pDCSearch->pSearchCache =	pDCSCache;
}    										


/*
	被动循环搜索结构结点清空函数
	----------------------------
	pCycleSearch -	待清空的被动循环搜索结构指针
*/
void DetectCycleSearchClear(
			struct DetectCycleSearch *pCycleSearch
							)
{
	assert( NULL != pCycleSearch );    
    DetectSearchNodeCacheListClear( &pCycleSearch->list );
}


/*
	被动循环搜索结构结点插入函数
	----------------------------
	pCycleSearch -	待插入的被动循环搜索结构指针
	pNodeArea -	待插入的被动搜索区域指针
*/
static void __DetectCycleSearchAreaInsert(
					struct DetectCycleSearch *pCycleSearch,
					struct DetectSearchArea *pDSArea
							)
{

	struct ListHead *pHead =	NULL;
	struct ListHead *pNode =	NULL;

	assert( NULL != pCycleSearch );
    assert( NULL != pDSArea );
    printlog( REC, "__DetectCycleSearchAreaInsert\n" );
    
    pHead =	&pCycleSearch->list;
	pNode =	&pDSArea->listHead;
    
	if ( !IsListHeadEmpty( pHead ) )
    {
    	struct ListHead *pListNode =	ListHeadRearGet( pHead );
        
        while ( pListNode != pHead )
        {
			struct DetectSearchArea *pListNodeArea =	
                						GetDetectSearchAreaPointer( pListNode );
            //如果当前节点的起始值不小于待插入的节点起始值，
            //则将节点插入到当前节点之前
        	if ( FLOAT_COMPARE_SMALL != 
                	FloatCompare( pListNodeArea->startAngle, pDSArea->startAngle ) )
	    	{
                ListHeadInsert( pListNode->pPrev, pListNode, pNode );
                return ;
	    	}
			
            pListNode =	pListNode->pNext;
        }

        ListHeadInsert( pListNode->pPrev, pListNode, pNode );
    }
    else
    {
    	ListHeadRearInsert( pHead, pNode );
    }
}    									


/*
	被动循环搜索结构提取函数
	------------------------
	pCycleSearch -	待提取数据的被动循环搜索结构指针
	pSearchCache -	被动循环搜索结构提取的循环搜索数据源
*/
static void __DetectCycleSearchExtract(
					struct DetectCycleSearch *pCycleSearch
										)
{
    int i = 0;
    int size =  DETECT_SEARCH_CMD_CYCLE_MAX_NUM;
    struct DetectCycleSearchCache *pSearchCache =	pCycleSearch->pSearchCache;

    for ( i=0; i<size; ++i )
    {
    	BOOL color =	pSearchCache->color[i];
        if ( color )   //数据有效
        {
            struct DetectSearchArea *pNewArea =	DetectSearchAreaCacheGet();
            if ( pNewArea )
            {
                //更新节点值
                DetectSearchAreaCopy( pNewArea, pSearchCache->area+i );

                printlog( REC, "__DetectCycleSearchExtract : angle=%f\n",
                    				pNewArea->startAngle );

				__DetectCycleSearchAreaInsert( pCycleSearch, pNewArea );
            }
            else
            {
                printfSafe("Exception %s, %d -  DetectSearchArea is out of use\n",
                   				__FILE__, __LINE__);
                break;
            }
        }
    }

}
							


/*
	被动循环搜索结构结点形成函数
*/
static void __DetectCycleSearchNodeExplain(
							struct DetectCycleSearch *pCycleSearch,
							float step
											)
{
    struct ListHead *pCycleHead =	&pCycleSearch->list;

    if ( !IsListHeadEmpty( pCycleHead ) )
    {
        struct ListHead *pCycleNode =    pCycleHead->pNext;
        struct ListHead tempList;
        ListHeadInitialize( &tempList );
        
        while ( pCycleNode != pCycleHead )
        {
        	struct ListHead *pLhNext =	pCycleNode->pNext;
            struct DetectSearchArea *pSearchArea =  
                                        GetDetectSearchAreaPointer( pCycleNode );

            DetectSearchNodeExplain( &tempList, pSearchArea, DETECT_SCHEDULE_STEP );
            ListHeadDelete( pCycleNode->pPrev, pCycleNode->pNext );
            DetectSearchAreaCachePut( pSearchArea );
            pCycleNode = pLhNext;
        }

		DetectSearchAreaCacheListClear( pCycleHead );
        
        ListHeadMove( pCycleHead, &tempList );
    }
}											

/*
	被动循环搜索结构形成函数
*/
void DetectCycleSearchUpdate( 
				struct DetectCycleSearch *pCycleSearch
								)
{
	assert( NULL != pCycleSearch );

 //   printlog( REC, "DetectCycleSearchUpdate\n" );

	if ( !DetectCycleSearchEmpty( pCycleSearch ) )
    {
    	DetectCycleSearchClear( pCycleSearch );
        printfSafe( "Exception DetectCycleSearchUpdate\n" );
    }

    //抽取循环搜索区域
    __DetectCycleSearchExtract( pCycleSearch );
    //形成搜索结点
    __DetectCycleSearchNodeExplain( pCycleSearch, DETECT_SCHEDULE_STEP );


//    DetectCycleSearchPrint( REC, pCycleSearch );
}


