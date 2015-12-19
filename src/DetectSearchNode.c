
#include "..\Header.h"

void DetectSearchNodeFlagSet(
			struct DetectSearchNode *pDSNode,
			BOOL flag
							)    
{
	if ( NULL != pDSNode )
    {
    	pDSNode->flag =	flag;
        if ( DetectPilotSearchType == pDSNode->type )
        {
	        DetectSearchNodeCachePut( pDSNode );
        }
    }  
}


void DetectSearchNodeListInsertByOrder(
							struct ListHead *pLhHead,
							struct ListHead *pLhNode
										)
{
	struct ListHead *pLhPtr =	NULL;
    struct DetectSearchNode *pDSNode =	NULL;
    
	assert( NULL != pLhHead );
    assert( NULL != pLhNode );

    pLhPtr =	pLhHead->pNext;
	pDSNode =	GetDetectSearchNodePointer( pLhNode );
    while ( pLhPtr != pLhHead )
    {
    	struct DetectSearchNode *pDSNPtr =	GetDetectSearchNodePointer( pLhPtr );
		int ret =	FloatCompare( pDSNPtr->angle, pDSNode->angle );
        if ( FLOAT_COMPARE_BIG == ret )
        {
        	break;
        }
        pLhPtr =	pLhPtr->pNext;
    }
    ListHeadTailInsert( pLhPtr, pLhNode );
}


static inline
BOOL __DetectSearchNodeInFrontPart(
					struct DetectSearchNode *pDSNode
								)    
{
	int ret =	FloatRangeCmp( pDSNode->angle, 270.0, 359.99 );

    return ( FLOAT_RANGE_IN == ret );
}								

static BOOL __DetectSearchNodeListHasFrontPart( 
    							struct ListHead *pLhHead
    											)
{
	struct ListHead *pLhPtr =	ListHeadTailGet( pLhHead );
	if ( NULL != pLhPtr )
    {
    	struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhPtr );
		return __DetectSearchNodeInFrontPart( pDSNode );
    }
    else
    {
    	printfSafe( "Exception __DetectSearchNodeListHasBackPart\n" );
    }
    return FALSE;
}


void DetectSearchNodeListRearInsertByOrder(
								struct ListHead *pLhHead,
								struct ListHead *pLhNode
											)
{
	assert( NULL != pLhHead );
    assert( NULL != pLhNode );


    if ( IsListHeadEmpty( pLhHead ) )
	{	//链表为空，将结点插入链表的前半部分
		ListHeadRearInsert( pLhHead, pLhNode );
	}
    else if ( __DetectSearchNodeListHasFrontPart( pLhHead ) )
    {	//链表前半部分为空
    	ListHeadRearInsert( pLhHead, pLhNode );
    }
    else
    {	//链表不空且前半部分不为空，
    	//从前面开始选择合适的位置，选择第一个大于当前角度的位置
    	struct ListHead *pLhPtr =	pLhHead->pNext;
	    struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhNode );
        while ( pLhPtr != pLhHead )
        {
		    struct DetectSearchNode *pDSNPtr =	GetDetectSearchNodePointer( pLhPtr );
            int result =	FloatCompare( pDSNPtr->angle, pDSNode->angle );	
            if ( FLOAT_COMPARE_BIG == result )
            {
            	break;
            }
            if ( !__DetectSearchNodeInFrontPart( pDSNPtr ) )
            {
            	break;
            }
                
            pLhPtr =	pLhPtr->pNext;
        }
		
        ListHeadTailInsert( pLhPtr, pLhNode );
    }
}											

static inline
BOOL __DetectSearchNodeInBackPart(
					struct DetectSearchNode *pDSNode
								)    
{
	int ret =	FloatRangeCmp( pDSNode->angle, 0.0, 89.99 );

    return ( FLOAT_RANGE_IN == ret );
}								

static BOOL __DetectSearchNodeListHasBackPart( 
    							struct ListHead *pLhHead
    											)
{
	struct ListHead *pLhPtr =	ListHeadTailGet( pLhHead );
	if ( NULL != pLhPtr )
    {
    	struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhPtr );
		return __DetectSearchNodeInBackPart( pDSNode );
    }
    else
    {
    	printfSafe( "Exception __DetectSearchNodeListHasBackPart\n" );
    }
    return FALSE;
}

void DetectSearchNodeListTailInsertByOrder(
								struct ListHead *pLhHead,
								struct ListHead *pLhNode
											)
{
	assert( NULL != pLhHead );
    assert( NULL != pLhNode );


    if ( IsListHeadEmpty( pLhHead ) )
	{	//链表为空，将结点插入链表的后半部分
		ListHeadTailInsert( pLhHead, pLhNode );
	}
    else if ( !__DetectSearchNodeListHasBackPart( pLhHead ) )
	{	//链表后半部分为空，将结点插入链表的后半部分
		ListHeadTailInsert( pLhHead, pLhNode );
	}        
    else
    {	//链表不空且后部分不为空，则从后面开始选择合适的位置，选择第一个小于当前角度的位置
    	struct ListHead *pLhPtr =	pLhHead->pPrev;
	    struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhNode );
        while ( pLhPtr != pLhHead )
        {
		    struct DetectSearchNode *pDSNPtr =	GetDetectSearchNodePointer( pLhPtr );
            int result =	FloatCompare( pDSNPtr->angle, pDSNode->angle );
            if ( FLOAT_COMPARE_SMALL == result )
            {
            	break;
            }
            if ( !__DetectSearchNodeInBackPart( pDSNPtr ) )
            {
            	ListHeadRearInsert( pLhPtr, pLhNode );
            	return ;
            }
            pLhPtr =	pLhPtr->pPrev;
        }
		
        ListHeadRearInsert( pLhPtr, pLhNode );
    }
}


static void __DetectSearchNodeSearchAreaExtract(
					struct DetectSearchNode *pSearchNode,
					const struct DetectSearchArea *pDSArea
											)
{
	assert( NULL != pSearchNode );
    assert( NULL != pDSArea );

    pSearchNode->angle =	pDSArea->startAngle;
	pSearchNode->type =	pDSArea->type;
    pSearchNode->stayPeriod =	DETECT_SEARCH_STAY_PERIOD;
    pSearchNode->targetNo =	pDSArea->targetNo;
    pSearchNode->flag =	FALSE;

    ListHeadInitialize( &pSearchNode->listHead );
}
void DetectSearchNodeTargetExtract(
					struct DetectSearchNode **pStart,
					struct DetectSearchNode	**pEnd,
					const struct DetectSearchArea *pDSArea
									)
{
	assert( NULL != pStart );
	assert( NULL != pEnd );
    assert( NULL != pDSArea );

//    printlog( REC, "DetectSearchNodeTargetExtract\n" );
	while ( pStart != pEnd )
    {
    	__DetectSearchNodeSearchAreaExtract( *pStart, pDSArea );
        pStart ++;
    }
}									


/*
	被动搜索区间节点数据提取函数
	------------------------------------------
	pSearchNode -	待形成的被动搜索区间节点指针
	pSearchArea -	被动搜索区域结构指针
	offset -	相对于当前的偏移量
*/
void DetectSearchNodeExtract( 
					struct DetectSearchNode *pSearchNode,
					const struct DetectSearchArea *pSearchArea,
					float offset
							)
{
	assert( NULL != pSearchNode );
    assert( NULL != pSearchArea );

    pSearchNode->angle =	pSearchArea->startAngle + offset;
	pSearchNode->angle =	FloatLmtGet( pSearchNode->angle, ANGLE_MOD );

//    printlog( REC, "DetectSearchNodeExtract angle=%f\n", pSearchNode->angle );

	pSearchNode->type =	pSearchArea->type;
    pSearchNode->stayPeriod =	DETECT_SEARCH_STAY_PERIOD;//pSearchArea->stayPeriod;

    pSearchNode->targetNo =	pSearchArea->targetNo;

    pSearchNode->flag =	FALSE;
    
    ListHeadInitialize( &pSearchNode->listHead );
}

/*
	被动搜索区间结点结构形成执行函数
	--------------------------------
	pListHead -	待形成被动搜索区间结点结构的链表结点指针
	pSearchArea -	形成被动搜索区间结点结构链表的被动搜索区域结构指针
	step -	形成的被动搜索区间结点的步长
*/
void DetectSearchNodeExplain(
					struct ListHead *pListHead,
					const struct DetectSearchArea *pSearchArea,
					float step
							)
{
	float offset =	0.0;
    float end =	0.0;

    assert( NULL != pListHead );
    assert( NULL != pSearchArea );
//    printlog( REC, "DetectSearchNodeExplain\n" );
    
    end =	pSearchArea->endAngle - pSearchArea->startAngle;
//    printlog( REC, " %f %f  \n", offset, end );
    
    for ( ; FLOAT_COMPARE_SMALL != FloatCompare(end, offset); offset += step )
    {
    	struct DetectSearchNode *pNewNode =	DetectSearchNodeCacheGet();
        if ( NULL != pNewNode )
        {
        	DetectSearchNodeExtract( pNewNode, pSearchArea, offset );
            ListHeadTailInsert( pListHead, &pNewNode->listHead );
            
            if ( DetectSearchAreaNeedsExpand( pSearchArea ) )
            {
            	struct DetectSearchNode *pNewNode2 =	DetectSearchNodeCacheGet();
                if ( NULL != pNewNode2 )
                {
                	DetectSearchNodeExtract( pNewNode2, pSearchArea, offset );
            		ListHeadTailInsert( pListHead, &pNewNode2->listHead );
                }
            	else
                {
                	printfSafe( "DetectSearchNodeExplain Failed!\n" );
                }
            }
        }
    	else
        {
        	printfSafe( " Exception %s, %d -	DetectSearchNodeValExplainFromSearchArea "
                		"out of memory!\n", 
                			__FILE__, __LINE__ );
            break;	
        }
    }
}				

////////////////////////////////////////////////////////////////////////////////
int DetectScheduleNodeAntennaJudge(
					struct DetectSearchNode *pDSNode,
					float courseAngle
							)
{
	float angle =	0.0;
    int ret =	-1;
    
	assert( NULL != pDSNode );

	angle =	pDSNode->angle - courseAngle;
//    printlog( REC, "DetectScheduleNodeAntennaJudge : pDSNode->angle=%f, courseAngle=%f\n",
//        		pDSNode->angle, courseAngle );

    if ( FLOAT_COMPARE_SMALL == FloatCompare( angle, DETECT_SEARCH_AREA_MIN_ANGLE ) )
    {
    	angle +=	360.0;
    }
//    printlog( REC, "\tangle = %f\n", angle );

    if ( FLOAT_RANGE_IN == FloatRangeCmp( angle, DETECT_SEARCH_AREA_MIN_ANGLE, 89.99 ) )
    {
    	ret =	0;
    }
    else if ( FLOAT_RANGE_IN == FloatRangeCmp( angle, 89.99, 179.99 ) )
    {
    	ret =	1;
    }
    else if ( FLOAT_RANGE_IN == FloatRangeCmp( angle, 179.99, 269.99 ) )
    {
    	ret =	2;
    }
    else if ( FLOAT_RANGE_IN == FloatRangeCmp( angle, 269.99, DETECT_SEARCH_AREA_MAX_ANGLE ) )
    {
    	ret =	3;
    }
    else
    {
    	ret =	0;
    	printfSafe( "Exception DetectScheduleNodeAntennaJudge failed - angle=%f!\n",
            			angle );
    }
	return ret;
}



