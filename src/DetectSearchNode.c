
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
	{	//����Ϊ�գ��������������ǰ�벿��
		ListHeadRearInsert( pLhHead, pLhNode );
	}
    else if ( __DetectSearchNodeListHasFrontPart( pLhHead ) )
    {	//����ǰ�벿��Ϊ��
    	ListHeadRearInsert( pLhHead, pLhNode );
    }
    else
    {	//��������ǰ�벿�ֲ�Ϊ�գ�
    	//��ǰ�濪ʼѡ����ʵ�λ�ã�ѡ���һ�����ڵ�ǰ�Ƕȵ�λ��
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
	{	//����Ϊ�գ�������������ĺ�벿��
		ListHeadTailInsert( pLhHead, pLhNode );
	}
    else if ( !__DetectSearchNodeListHasBackPart( pLhHead ) )
	{	//�����벿��Ϊ�գ�������������ĺ�벿��
		ListHeadTailInsert( pLhHead, pLhNode );
	}        
    else
    {	//�������Һ󲿷ֲ�Ϊ�գ���Ӻ��濪ʼѡ����ʵ�λ�ã�ѡ���һ��С�ڵ�ǰ�Ƕȵ�λ��
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
	������������ڵ�������ȡ����
	------------------------------------------
	pSearchNode -	���γɵı�����������ڵ�ָ��
	pSearchArea -	������������ṹָ��
	offset -	����ڵ�ǰ��ƫ����
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
	��������������ṹ�γ�ִ�к���
	--------------------------------
	pListHead -	���γɱ�������������ṹ��������ָ��
	pSearchArea -	�γɱ�������������ṹ����ı�����������ṹָ��
	step -	�γɵı�������������Ĳ���
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



