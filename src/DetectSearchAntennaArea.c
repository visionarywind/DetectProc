
/*
	被动调度阵面搜索处理实现文件
	----------------------------
	说明: 该文件的通过被动调度链表形成四个阵面的搜索区域
	提供如下的接口处理函数
	DetectSearchAntennaAreaInitialize()
	
*/
#include "..\Header.h"

/*
    阵面区域角度结构初始化函数
*/
void AntennaAreaAngleInitialize( 
                            struct AntennaAreaAngle *pAAAngle,
                            float startAngle,
                            float endAngle
                                )
{
    assert( NULL != pAAAngle );
    
    pAAAngle->start = startAngle;
    pAAAngle->end =   endAngle;
    pAAAngle->startAssistant =    0.0;
    pAAAngle->endAssistant =  0.0;
    pAAAngle->assist =    FALSE;
    pAAAngle->lastAssist =	FALSE;
    pAAAngle->flag =	FALSE;
}

/*
    判断角度是否在阵面区域角度结构中
    --------------------------------
    pAngle -    阵面区域角度结构指针
    angle - 待判断的角度
*/
BOOL AntennaAreaAngleRangeCmp(
                        const struct AntennaAreaAngle *pAngle,
                        float angle
                                            )
{
    if ( pAngle->assist )
    {
        if ( FLOAT_RANGE_IN == FloatRangeCmp( angle, pAngle->start,
                                                pAngle->end ) 
            || FLOAT_RANGE_IN == FloatRangeCmp( angle, pAngle->startAssistant,
                                                    pAngle->endAssistant ) )
        {
            return TRUE;
        }
    }
    else
    {
        if ( FLOAT_RANGE_IN == FloatRangeCmp( angle, pAngle->start,
                                                pAngle->end ) )
        {
            return TRUE;
        }
    }
    return FALSE;
}                                           
  										

void AntennaAreaNodeClear(
			struct AntennaAreaNode *pAANode
						)
{
	struct ListHead *pLhHead =	NULL;
    
	assert( NULL != pAANode );
	printlog( REC, "AntennaAreaNodeClear\n" );
    
    pLhHead =	&pAANode->list;
	DetectSearchNodeCacheListClear( pLhHead );
}


struct DetectSearchNode *AntennaAreaNodeDataGet(
						struct AntennaAreaNode *pAANode							
												)
{
	struct DetectSearchNode *pDSNode =	NULL;
	struct ListHead *pLhPtr =	NULL;
    struct ListHead *pLhHead =	NULL;

    assert( NULL != pAANode );
//    printlog( SCR, "AntennaAreaNodeDataGet\n" );

    pLhHead =	&pAANode->list;
    pLhPtr =    pLhHead->pNext;


    while ( pLhPtr != pLhHead )
    {
        struct ListHead *pLhNext =	pLhPtr->pNext;
		pDSNode =	GetDetectSearchNodePointer( pLhPtr );
        
        if ( !pDSNode->flag )
        {	//如果找到未搜索的结点
        	return pDSNode;
        }

     	pLhPtr =	pLhNext;   
    }

    return NULL;
}


BOOL AntennaAreaNodeDataEmpty(
		const struct AntennaAreaNode *pAANode
							)
{
	assert( NULL != pAANode );
    return IsListHeadEmpty( &pAANode->list );
}							


/*
	阵面区间节点结构初始化函数
	--------------------------
	pNode -	待初始化的阵面区间节点结构指针
	start -	阵面区间起始角度
	end -	阵面区间结束角度
*/
void AntennaAreaNodeInitialize( 
						struct AntennaAreaNode *pNode, 
						float start,
						float end
								)
{
	pNode->start =	start;
    pNode->end =	end;

    AntennaAreaAngleInitialize( &pNode->realAngle,
        						pNode->start,
        						pNode->end );
        
    ListHeadInitialize( &pNode->list );
    ListHeadInitialize( &pNode->links );
}

/*
	阵面区间节点结构上一节点获取函数
	--------------------------------
	pAreaNode -	待获取的阵面区间节点结构指针
*/    	
struct AntennaAreaNode *AntennaAreaNodePrevGet( 
					const struct AntennaAreaNode *pAreaNode
												)
{
	struct ListHead *pLhHead =	NULL;
    struct ListHead *pPrev =	NULL;
    
	assert( NULL != pAreaNode );
    
    pLhHead =	(struct ListHead *)&pAreaNode->links;
    pPrev =	ListHeadTailGet( pLhHead );
    
	if ( NULL != pPrev )
    {	
     	struct AntennaAreaNode *pAreaNode =	GetAntennaAreaNodePointer( pPrev );
        return pAreaNode;
    }
    else
    {
    	return NULL;
    }
}															

/*
	阵面区间节点结构下一节点获取函数
	--------------------------------
	pAreaNode -	待获取的阵面区间节点结构指针	
*/    	
struct AntennaAreaNode *AntennaAreaNodeNextGet( 
							const struct AntennaAreaNode *pAreaNode
												)
{
	struct ListHead *pLhHead =	NULL;
    struct ListHead *pNext =	NULL;
    
	assert( NULL != pAreaNode );
    
    pLhHead =	(struct ListHead *)&pAreaNode->links;
    pNext =	ListHeadRearGet( pLhHead );
    
	if ( NULL != pNext )
    {	
     	struct AntennaAreaNode *pAreaNode =	GetAntennaAreaNodePointer( pNext );
        return pAreaNode;
    }
    else
    {
    	return NULL;
    }
}															

/*
	判断角度是否在阵面区间节点结构中
	--------------------------------
	pAngle -	阵面区间节点结构指针
	angle -	待判断的角度
*/
BOOL AntennaAreaNodeRangeCmp(
					const struct AntennaAreaNode *pAreaNode,
					float angle
							)
{
	assert( NULL != pAreaNode );
    return AntennaAreaAngleRangeCmp( &pAreaNode->realAngle, angle );
}    			

/*
	阵面区间节点结构角度区间更新函数
	----------------------------
	pAreaNode -	待更新的阵面区间节点结构指针
	offset -	偏移角度，也就是航向角
*/
void AntennaAreaNodeUpdate(
				struct AntennaAreaNode *pAANode,
				float offset
							)
{
	float start =	0.0;
    float end =	0.0;
    struct AntennaAreaAngle *pAAAngle =	&pAANode->realAngle;

    assert( NULL != pAANode );

    start =	(float)fmod( pAANode->start + offset, 360.0 );
    end =	(float)fmod( pAANode->end + offset, 360.0 );

//	printlog( REC, "start=%f, end=%f\n", start, end );
    
    if ( FLOAT_COMPARE_SMALL == FloatCompare( end, start ) )
    {
    	//表明跨区了
    	pAAAngle->assist =	TRUE;

        pAAAngle->start =	start;
        pAAAngle->end =	DETECT_SEARCH_AREA_MAX_ANGLE;
        pAAAngle->startAssistant =	DETECT_SEARCH_AREA_MIN_ANGLE;
        pAAAngle->endAssistant =	end;
    }
    else
    {
    	//未跨区
        pAAAngle->assist =	FALSE;

    	pAAAngle->start =	start;
        pAAAngle->end =	end;
    }
    
//    printlog( REC, "assist=%d, lastAssist=%d\n", 
//        		pAAAngle->assist, pAAAngle->lastAssist );
}											

int AntennaAreaNodeDelete(
				struct AntennaAreaNode *pAANode,
				unsigned int	mask
							)
{
	struct ListHead *pLhHead =	NULL;
    int ret =	0;
    
	assert( NULL != pAANode );
	
    pLhHead =	&pAANode->list;

	if ( !IsListHeadEmpty( pLhHead ) )
    {
    	struct ListHead *pLhPtr =	ListHeadNextPtrGet( pLhHead );
		struct DetectSearchNode *pDSNode =	NULL;
        
	    while ( pLhPtr != pLhHead )
	    {
	    	struct ListHead *pLhNext =	ListHeadNextPtrGet( pLhPtr );
	    	pDSNode =	GetDetectSearchNodePointer( pLhPtr );
            if ( mask == pDSNode->targetNo )
            {
            	ListHeadDelete( pLhPtr->pPrev, pLhPtr->pNext );
                DetectSearchNodeCacheListHeadPut( pLhPtr );
            }

            pLhPtr =	pLhNext;
	    }	
    }
    
    return ret;
}


/*
	阵面区间节点结构角度区间插入函数
	--------------------------------
	说明:
		函数在执行插入操作时，根据当前的角度情况决定从前往后插入还是逆向
	pAreaNode -	待插入的阵面区间节点结构指针
	pSearchNode -	待插入的被动搜索结点指针
*/
void AntennaAreaNodePut(
				struct AntennaAreaNode *pAANode,
				struct DetectSearchNode *pSearchNode
						)
{
	struct ListHead *pLhHead =	NULL;
	struct ListHead *pLhPtr =	NULL;
    BOOL crossArea =	FALSE;
    BOOL lastCrossArea =	FALSE;
    
    assert( NULL != pAANode );
    assert( NULL != pSearchNode );

	pLhHead =	&pAANode->list;
	pLhPtr =	&pSearchNode->listHead;
	crossArea =	AntennaAreaNodeCrossArea( pAANode );
	lastCrossArea =	AntennaAreaNodeCrossAreaTrack( pAANode );
//	printlog( REC, "AntennaAreaNodePut : AntennaAreaNode ptr=%p, angle=%f, crossArea=%d\n",
//        		&pAANode->realAngle, pSearchNode->angle, crossArea );
    if ( crossArea || lastCrossArea )
    {
    	//当发生了跨区后，判断当前结点落在区间前面，还是区间后面
    	//跨区的情况为 x~359.99, 0.00~( 90-359.99-x )
    	float angle =	pSearchNode->angle;
    	BOOL atRear =	FloatRangeCmp( angle, 270.0, DETECT_SEARCH_AREA_MAX_ANGLE );
        if ( atRear )
        {
        	//落在前面的区间
        	DetectSearchNodeListRearInsertByOrder( pLhHead, pLhPtr );
        }
        else
        {
        	//落在后面的区间
        	DetectSearchNodeListTailInsertByOrder( pLhHead, pLhPtr );
        }
    }
    else
    {
		DetectSearchNodeListInsertByOrder( pLhHead, pLhPtr );
    }
}	

////////////////////////////////////////////////////////////////////////////////
/*
	阵面区间结构初始化函数
	----------------------
	pArea -	待初始化的阵面区间结构指针
	offset -	偏移量
*/
void DetectAntennaAreaInitialize( 
					struct DetectAntennaArea *pDAArea,
					float offset
								)
{
	int i =	0;
   	struct ListHead *pHead =	NULL;
    struct ListHead *pNode =	NULL;
    struct AntennaAreaNode *pAANode =	NULL;
    
	assert( NULL != pDAArea );


	//初始化阵面角度信息
    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	float start =	ANTENNA_AREA_START_ANGLE + ANTENNA_AREA_ANGLE_AREA*i;
        float end =	ANTENNA_AREA_END_ANGLE + ANTENNA_AREA_ANGLE_AREA*i;
    	AntennaAreaNodeInitialize( pDAArea->areaNode+i, start, end );
    }

	//建立链接关系
	pAANode =	pDAArea->areaNode;
	pHead =	&( pAANode->links );
    for ( i=1; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	pAANode =	pDAArea->areaNode+i;
		pNode =	&pAANode->links;
        ListHeadTailInsert( pHead, pNode );
    }

    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	pDAArea->color[i] =	FALSE;
    }

    pDAArea->empty =	TRUE;
    pDAArea->offset =	offset;
}

void DetectAntennaAreaReset(
					struct DetectAntennaArea *pDAArea,
					float offset
							)
{
	assert( NULL != pDAArea );
    printlog( REC, "DetectAntennaAreaReset : pDAArea=%p\n", pDAArea );

	DetectAntennaAreaClear( pDAArea );
    pDAArea->offset =	offset;
}							


void DetectAntennaAreaClear(
			struct DetectAntennaArea *pDAArea
							)
{
	int i =	0;
    struct AntennaAreaNode *pAANode =	NULL;
    
	assert( NULL != pDAArea );
    printlog( REC, "DetectAntennaAreaClear\n" );
    DetectAntennaAreaPrint( REC, pDAArea );
    
    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	pAANode =	pDAArea->areaNode + i;
    	AntennaAreaNodeClear( pAANode );
        pDAArea->color[i] =	FALSE;
    }
    pDAArea->empty =	TRUE;
}

static void __AntennaAreaNodeListFlagReset( 
    					struct AntennaAreaNode *pAANode
    										)    
{
	struct ListHead *pLhHead =	&pAANode->list;
    struct ListHead *pLhPtr =	pLhHead->pNext;
    struct DetectSearchNode *pDSNode =	NULL;

	printlog( REC, "__AntennaAreaNodeListFlagReset\n" );
    ListHeadPrint( REC, pLhHead );
    ListHeadPrint( REC, pLhPtr );
    
    while ( pLhPtr != pLhHead )
    {
    	pDSNode =	GetDetectSearchNodePointer( pLhPtr );
        pDSNode->flag =	FALSE;
        pLhPtr =	pLhPtr->pNext;
    }
}  

void DetectAntennaAreaFlagReset(
				struct DetectAntennaArea *pDAArea
								)
{
	int i =	0;
    struct AntennaAreaNode *pAANode =	NULL;
    
	assert( NULL != pDAArea );
    printlog( LOG, "\tDetectAntennaAreaFlagReset : ALL node had been scheduled\n" );
    
    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	pAANode =	pDAArea->areaNode+i;
        __AntennaAreaNodeListFlagReset( pAANode );
    }
}								

void DetectAntennaAreaColorSet(
					struct DetectAntennaArea *pDAArea
								)
{
	int i =	0;
    BOOL empty =	TRUE;
    
    assert( NULL != pDAArea );

    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	struct AntennaAreaNode *pAANode =	pDAArea->areaNode+i;
        if ( AntennaAreaNodeDataEmpty( pAANode ) )
        {
        	pDAArea->color[i] =	FALSE;
        }
        else
        {
        	pDAArea->color[i] =	TRUE;
            empty =	FALSE;
        }
        //标记之前的状态信息
    	AntennaAreaAngleTrack( &pAANode->realAngle );
    }
    pDAArea->empty =	empty;
}								


/*
	阵面区间结构角度更新函数
	------------------------
	pArea -	 待更新的阵面区间结构指针
	courseAngle -	当前的航向角
*/
BOOL DetectAntennaAreaOffsetUpdate( 
			struct DetectAntennaArea *pDAArea,
			float courseAngle
						)
{
	BOOL ret =	FALSE;
    
	assert( NULL != pDAArea );
    
	if ( FLOAT_COMPARE_EQUAL != FloatCompare( pDAArea->offset, courseAngle ) )
    {
		int i =	0;
        
	    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
	    {
	    	struct AntennaAreaNode *pAreaNode =	pDAArea->areaNode + i;
	        AntennaAreaNodeUpdate( pAreaNode, courseAngle );
	    }

        DetectAntennaAreaOffsetSet( pDAArea, courseAngle );

   		ret =	TRUE;     
    }

//    printlog( REC, "DetectAntennaAreaOffsetUpdate : ret=%d\n", ret );

    return ret;
}	

/*
	阵面区间节点结构更新执行函数
	----------------------------
	pAreaNode -	待更新的阵面区间节点结构指针
*/
static BOOL __DetectAntennaAreaUpdate(
					struct DetectAntennaArea *pDAArea,
					struct AntennaAreaNode *pAANode
									)
{
	struct ListHead *pListHead =	&pAANode->list;
    BOOL ret =	FALSE;

	printlog( LOG, "__DetectAntennaAreaUpdate\n" );
    if ( !IsListHeadEmpty( pListHead ) )
    {
    	struct AntennaAreaNode *pAANodePrev =	AntennaAreaNodePrevGet( pAANode );
        struct AntennaAreaNode *pAANodeNext =	AntennaAreaNodeNextGet( pAANode );
        
    	//处理从前往后的部分
		struct ListHead *pLhPtr =	ListHeadNextPtrGet( pListHead );
//        printlog( REC, "Formal Part\n" );
        while ( pLhPtr != pListHead )
        {
        	struct ListHead *pLhNext =	ListHeadNextPtrGet( pLhPtr );
        	struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhPtr );
            if ( !AntennaAreaNodeRangeCmp( pAANode, pDSNode->angle ) )
            {
            	ListHeadDelete( pLhPtr->pPrev, pLhPtr->pNext );
                //这里如果航向角发生非常大的变化，也就是180度的变化，对航向角要进行保护
                //否则，这里的程序会出现混乱
				if ( AntennaAreaNodeRangeCmp( pAANodePrev, pDSNode->angle ) )
                {
                	AntennaAreaNodePut( pAANodePrev, pDSNode );
                }
            	else
                {
                	printfSafe( "__DetectAntennaAreaUpdate exception\n" );
                	DetectAntennaAreaPut( pDAArea, pDSNode );
                }
            }
            
            pLhPtr =	pLhNext;
        }
        
//        printlog( REC, "Back Part\n" );
        //处理从后往前的部分
        pLhPtr =	ListHeadPrevPtrGet( pListHead );
        while ( pLhPtr != pListHead )
        {
        	struct ListHead *pLhNext =	ListHeadPrevPtrGet( pLhPtr );
			struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhPtr );
            if ( !AntennaAreaNodeRangeCmp( pAANode, pDSNode->angle ) )
            {
            	ListHeadDelete( pLhPtr->pPrev, pLhPtr->pNext );
                //处理同上
                if ( AntennaAreaNodeRangeCmp( pAANodeNext, pDSNode->angle ) )
                {
                	AntennaAreaNodePut( pAANodeNext, pDSNode );
                }
                else
                {
                	printfSafe( "__DetectAntennaAreaUpdate reverse exception" );
                    DetectAntennaAreaPut( pDAArea, pDSNode );
                }
            }

            pLhPtr =	pLhNext;
        }

        ret =	TRUE;        
    }

    return ret;
}

/*
	阵面区间结构节点更新函数
	------------------------
	pArea -	 待更新的阵面区间结构指针
	courseAngle -	当前的航向角
*/
void DetectAntennaAreaUpdate( 
			struct DetectAntennaArea *pDAArea
						)
{
	struct AntennaAreaNode *pAANode =	NULL;
	int i =	0;

	assert( NULL != pDAArea );
    printlog( LOG, "DetectAntennaAreaUpdate\n" );

    if ( !pDAArea->empty )
    {
    	pAANode =	pDAArea->areaNode;
	    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
	    {
	    	if ( !__DetectAntennaAreaUpdate( pDAArea, pAANode+i ) )
	        {
	        	printlog( LOG, "DetectAntennaAreaUpdate null area!\n" );
	        }
	    }
        
	    DetectAntennaAreaColorSet( pDAArea );
    }
    else
    {
    	printlog( LOG, "No DetectAntennaAreaUpdate\n" );
    }
}

BOOL DetectAntennaAreaListPut(
			struct DetectAntennaArea *pDAArea,
			struct ListHead *pLhHead
							)
{
	struct ListHead *pLhPtr =	NULL;
	struct DetectSearchNode *pDSNode =	NULL;
    BOOL putTag =	FALSE;
    
	assert( NULL != pDAArea );
    assert( NULL != pLhHead );
//    printlog( REC, "DetectAntennaAreaListPut\n" );

    pLhPtr =	pLhHead->pNext;
    while ( pLhPtr != pLhHead )
    {
    	struct ListHead *pLhNext =	pLhPtr->pNext;
		pDSNode =	GetDetectSearchNodePointer( pLhPtr );
        ListHeadDelete( pLhPtr->pPrev, pLhPtr->pNext );
		putTag =	DetectAntennaAreaPut( pDAArea, pDSNode );
        if ( !putTag )
        {
        	printfSafe( "DetectAntennaAreaListPut failed!\n" );
            DetectSearchNodeCachePut( pDSNode );
        }
        
        pLhPtr =	pLhNext;
    }

    return TRUE;
}							


/*
	阵面区间结构搜索节点插入函数
	----------------------------------
	pArea -	待加入的阵面区间结构指针
	pNode -	待加入的搜索节点struct ListHead结构指针
*/
BOOL DetectAntennaAreaPut(
			struct DetectAntennaArea *pArea,
			struct DetectSearchNode *pDSNode
					)
{
	struct AntennaAreaNode *pAANode =	NULL;
    int i =	0;
    
	assert( NULL != pArea );
    assert( NULL != pDSNode );
//    printlog( REC, "DetectAntennaAreaPut\n" );
//    DetectSearchNodePrint( REC, pDSNode );

    pAANode =	pArea->areaNode;
    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	//这里调用 阵面区间节点结构角度区间插入执行函数，减少不必要的打印
    	if ( AntennaAreaNodeRangeCmp( pAANode+i, pDSNode->angle ) )
        {
        	AntennaAreaNodePut( pAANode+i, pDSNode );
            return TRUE;
        }
    }
    printfSafe( "%s, %d - AntennaAreaPut failed : pSearchNode->angle = %f\n",
        			__FILE__, __LINE__, pDSNode->angle );
    return FALSE;
}

BOOL DetectAntennaAreaDelete(
			struct DetectAntennaArea *pDAArea,
			unsigned int	mask
							)
{
	int i =	0;
	struct AntennaAreaNode *pAANode =	NULL;
    
    assert( NULL != pDAArea );

    pAANode =	pDAArea->areaNode;
    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
		AntennaAreaNodeDelete( pAANode+i, mask );
    }
	
	return TRUE;
}				
			




