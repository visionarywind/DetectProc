

/*
	被动调度链表实现文件
	--------------------
*/

#include "..\Header.h"


////////////////////////////////////////////////////////////////////////////////

/*
	被动调度处理链表结构初始化函数
	------------------------------
	pScheduleList -	待初始化的被动调度处理链表结构指针
	pTargetCache -	被动一次搜索重点目标缓存指针
	pPilotCache -	被动一次搜索任务引导缓存指针
*/
void DetectScheduleListInitialize( 
			struct DetectScheduleList *pDSList,
			struct DetectCycleSearchCache *pDCSCache,
			struct DetectOnceSearchCache *pDOSCache
								)
{
	assert( NULL != pDSList );
    assert( NULL != pDCSCache );
    assert( NULL != pDOSCache );

	DetectCycleSearchInitialize( &pDSList->cycleSearch, pDCSCache );
    DetectOnceSearchInitialize( &pDSList->pilotSearch, pDOSCache );

    pDSList->cycleStatus =	FALSE;
    pDSList->pilotStatus =	FALSE;
    
    pDSList->sync =	semBCreate( SEM_Q_FIFO, SEM_FULL );
    pDSList->status =	TRUE;
}

void DetectScheduleListReset( 
			struct DetectScheduleList *pDSList
								)
{
	assert( NULL != pDSList );
//    printlog( REC, "DetectScheduleListReset : pDSList=%p\n", pDSList );

    DetectScheduleListClear( pDSList );
}								


/*
	被动调度处理链表结构清空函数
*/
void DetectScheduleListClear(
			struct DetectScheduleList *pDSList 
								)
{
	assert( NULL != pDSList );
    
    pDSList->cycleStatus =  FALSE;
    pDSList->pilotStatus =  FALSE;

    DetectCycleSearchClear( &pDSList->cycleSearch );
    DetectOnceSearchClear( &pDSList->pilotSearch );
}

void DetectScheduleListCheck(
			struct DetectScheduleList *pDSList
							)
{
	struct DetectOnceSearch *pDOSearch =	NULL;
    
	assert( NULL != pDSList );
//	printlog( REC, "DetectScheduleListCheck\n" );
    
	pDSList->cycleStatus =	DetectCycleSearchNeedsUpdate( &pDSList->cycleSearch, FALSE );

    pDOSearch =	&( pDSList->pilotSearch );
    if ( DetectOnceSearchNeedsUpdate( pDOSearch ) )
	{
    	pDSList->pilotStatus =	TRUE;
    }
    else
    {
    	pDSList->pilotStatus =	FALSE;
    }
}							


/*
	更新的条件为:
		1 有循环搜索需要处理
		2 如果1不成立，一次搜索链表为空，有一次搜索需要解析
*/
BOOL DetectScheduleListNeedsUpdate(
			const struct DetectScheduleList *pDSList
									)
{
	BOOL ret =	FALSE;
    
	assert( NULL != pDSList );

	ret =	( pDSList->cycleStatus || pDSList->pilotStatus );

//    printlog( REC, "DetectScheduleListNeedsUpdate ret=%d\n", ret );
    return ret;
}									

void DetectScheduleListUpdate(
			struct DetectScheduleList *pDSList
								)
{
	assert( NULL != pDSList );
    
//	printlog( REC, "DetectScheduleListUpdate\n" );
    
 	if ( pDSList->cycleStatus )
    {
    	DetectCycleSearchUpdate( &pDSList->cycleSearch );
        pDSList->cycleStatus =	FALSE;
    }
    if ( pDSList->pilotStatus )
    {
    	DetectOnceSearchUpdate( &pDSList->pilotSearch );
        pDSList->pilotStatus =	FALSE;
    }
}								



