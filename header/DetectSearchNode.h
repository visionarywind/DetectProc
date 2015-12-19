
#ifndef __DETECT_SEARCH_NODE_HEADER__
#define __DETECT_SEARCH_NODE_HEADER__


/*
	被动搜索区间节点
*/
struct DetectSearchNode
{
	float	angle;	//搜索角度
	unsigned short	stayPeriod;	//驻留时间
	unsigned char	type;	//搜索类型

    unsigned int	frequency;	//频率
    unsigned short 	pulseWidth;	//脉宽

    unsigned int	targetNo;

	BOOL	flag;

    struct ListHead	listHead;	//节点链表指针
};

EXPORT
void DetectSearchNodeFlagSet(
			struct DetectSearchNode *pDSNode,
			BOOL flag
							);		


/*
    通过listHead节点获取被动搜索节点结构的首地址
*/
#define GetDetectSearchNodePointer(listHeadPtr) \
        GetPointer( (listHeadPtr), struct DetectSearchNode, listHead )


static inline
void DetectSearchNodeCopy( struct DetectSearchNode *pDes,
							const struct DetectSearchNode *pSrc )    
{
	memcpy( pDes, pSrc, sizeof( struct DetectSearchNode ) );
}							

#ifdef __DEBUG__
static void __attribute__((unused))
DetectSearchNodePrint(
			    int log,
				const struct DetectSearchNode *pDSNode
					)
{
	if ( NULL != pDSNode )
    {
    	printlog( log, "\tDetectSearchNode : angle=%f, stayPeriod=%d, type=%d, targetNo=%d, flag=%d\n",
        		pDSNode->angle, pDSNode->stayPeriod, pDSNode->type, pDSNode->targetNo,
        		pDSNode->flag );
    }
	else
	{
		printlog( log, "\tDetectSearchNode NULL\n" );
	}
}	

static void __attribute__((unused))
DetectSearchNodeListPrint(
				int log,
				const struct ListHead *pLhHead
						)
{
	struct ListHead *pLhPtr =	NULL;
	assert( NULL != pLhHead );

	printlog( log, "DetectSearchNodeListPrint : " );
    pLhPtr =	pLhHead->pNext;
    while ( pLhPtr != pLhHead )
    {
    	struct ListHead *pLhNext =	pLhPtr->pNext;
        struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhPtr );

        DetectSearchNodePrint( log, pDSNode );
        pLhPtr =	pLhNext;
    }
}			
#endif


EXPORT
void DetectSearchNodeListInsertByOrder(
							struct ListHead *pLhHead,
							struct ListHead *pLhNode
										);


EXPORT
void DetectSearchNodeListRearInsertByOrder(
								struct ListHead *pLhHead,
								struct ListHead *pLhNode
											);

EXPORT
void DetectSearchNodeListTailInsertByOrder(
								struct ListHead *pLhHead,
								struct ListHead *pLhNode
											);

EXPORT
void DetectSearchNodeTargetExtract(
					struct DetectSearchNode **pStart,
					struct DetectSearchNode	**pEnd,
					const struct DetectSearchArea *pDSArea
									);

/*
	被动搜索区间节点数据提取函数
	------------------------------------------
	pSearchNode -	待形成的被动搜索区间节点指针
	pSearchArea -	被动搜索区域结构指针
	offset -	相对于当前的偏移量
*/
EXPORT
void DetectSearchNodeExtract( 
					struct DetectSearchNode *pSearchNode,
					const struct DetectSearchArea *pSearchArea,
					float offset
							);
/*
	被动搜索区间结点结构形成执行函数
	--------------------------------
	pListHead -	待形成被动搜索区间结点结构的链表结点指针
	pSearchArea -	形成被动搜索区间结点结构链表的被动搜索区域结构指针
	step -	形成的被动搜索区间结点的步长
*/
EXPORT
void DetectSearchNodeExplain(
					struct ListHead *pListHead,
					const struct DetectSearchArea *pSearchArea,
					float step
							);

////////////////////////////////////////////////////////////////////////////////
EXPORT
int DetectScheduleNodeAntennaJudge(
					struct DetectSearchNode *pDSNode,
					float courseAngle
							);

#endif

