
#ifndef __DETECT_SCHEDULE_NODE_HEADER__
#define __DETECT_SCHEDULE_NODE_HEADER__


/*
	被动阵面调度节点信息
*/
struct DetectScheduleAntennaNode
{
	struct DetectSearchNode *pL1;
    struct DetectSearchNode *pL2;
    struct DetectSearchNode *pL3;
    struct DetectSearchNode *pL4;

    struct DetectScheduleTable	*pTable;
    struct DetectOnceSearch		*pPilotSearch;
    struct DetectAntennaArea	*pDAArea;
};

static inline
void DetectScheduleAntennaNodeSet(
			struct DetectScheduleAntennaNode *pDSANode
								)    
{
	assert( NULL != pDSANode );
    DetectSearchNodeFlagSet( pDSANode->pL1, TRUE );
    DetectSearchNodeFlagSet( pDSANode->pL2, TRUE );
    DetectSearchNodeFlagSet( pDSANode->pL3, TRUE );
    DetectSearchNodeFlagSet( pDSANode->pL4, TRUE );    
}								

#ifdef __DEBUG__
static void  __attribute__((unused))
DetectScheduleAntennaNodePrint(
			int log,
			const struct DetectScheduleAntennaNode *pDSANode
								)
{
	assert( NULL != pDSANode );
    printlog( log, "DetectScheduleAntennaNode Print:\n" );
    DetectSearchNodePrint( log, pDSANode->pL1 );
    DetectSearchNodePrint( log, pDSANode->pL2 );
    DetectSearchNodePrint( log, pDSANode->pL3 );
    DetectSearchNodePrint( log, pDSANode->pL4 );
}
#endif

static inline
BOOL DetectScheduleAntennaNodeEmpty(
			const struct DetectScheduleAntennaNode *pDSANode
									)    
{
	assert( NULL != pDSANode );
    return ( 
        NULL == pDSANode->pL1 &&
		NULL == pDSANode->pL2 &&
		NULL == pDSANode->pL3 &&
		NULL == pDSANode->pL4
    		);
}									

/*
	被动阵面调度节点信息初始化函数
	------------------------------
	pScheduleNode -	待初始化的被动阵面调度节点信息指针
*/
EXPORT
void DetectScheduleAntennaNodeInitialize( 
					struct DetectScheduleAntennaNode *pDSANode,
					struct DetectScheduleTable *pDSTable,
					struct DetectOnceSearch	*pPilotSearch,
					struct DetectAntennaArea *pDAArea
										);

EXPORT
void DetectScheduleAntennaNodeReset(
					struct DetectScheduleAntennaNode *pDSANode
									);    

EXPORT
void DetectScheduleAntennaNodeFree(
					struct DetectScheduleAntennaNode *pDSANode
									);    

EXPORT
void DetectScheduleAntennaNodeExtract(
					struct DetectScheduleAntennaNode *pDSANode,
					float courseAngle
										);
#endif

