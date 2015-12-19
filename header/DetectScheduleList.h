
/*
	被动调度链表结构头文件
	----------------------
*/
#ifndef __DETECT_SCHEDULE_LIST_HEADER__
#define __DETECT_SCHEDULE_LIST_HEADER__

/*
	被动调度表结构
*/
struct DetectScheduleList
{
	struct DetectCycleSearch	cycleSearch;
    struct DetectOnceSearch		pilotSearch;

    BOOL	cycleStatus;
    BOOL 	pilotStatus;

    SEM_ID	sync;	//同步信号量
    BOOL 	status;	//初始化状态
};

static inline
BOOL DetectScheduleListCycleStatusGet(
				struct DetectScheduleList *pDSList
									)
{
	assert( NULL != pDSList );
    return pDSList->cycleStatus;
}

static inline
BOOL DetectScheduleListHasCycleNode(
			struct DetectScheduleList *pDSList
									)    
{
	assert( NULL != pDSList );
	
    return !DetectCycleSearchEmpty( &pDSList->cycleSearch );
}									

EXPORT
void DetectScheduleListReset( 
			struct DetectScheduleList *pDSList
								);    

/*
	被动调度处理链表结构初始化函数
	------------------------------
	pScheduleList -	待初始化的被动调度处理链表结构指针
	pTargetCache -	被动一次搜索重点目标缓存指针
	pPilotCache -	被动一次搜索任务引导缓存指针
*/
EXPORT
void DetectScheduleListInitialize( 
			struct DetectScheduleList *pDSList,
			struct DetectCycleSearchCache *pDCSCache,
			struct DetectOnceSearchCache *pDOSCache
								);

/*
	被动调度链表结构清空函数
*/
EXPORT 
void DetectScheduleListClear(
			struct DetectScheduleList *pDSList 
							);

EXPORT
void DetectScheduleListCheck(
			struct DetectScheduleList *pDSList
							);   

EXPORT
BOOL DetectScheduleListNeedsUpdate(
			const struct DetectScheduleList *pDSList
									);

EXPORT
void DetectScheduleListUpdate(
			struct DetectScheduleList *pDSList
								);


#endif

