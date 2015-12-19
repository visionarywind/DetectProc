
/*
	������������ṹͷ�ļ�
	----------------------
*/
#ifndef __DETECT_SCHEDULE_LIST_HEADER__
#define __DETECT_SCHEDULE_LIST_HEADER__

/*
	�������ȱ�ṹ
*/
struct DetectScheduleList
{
	struct DetectCycleSearch	cycleSearch;
    struct DetectOnceSearch		pilotSearch;

    BOOL	cycleStatus;
    BOOL 	pilotStatus;

    SEM_ID	sync;	//ͬ���ź���
    BOOL 	status;	//��ʼ��״̬
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
	�������ȴ�������ṹ��ʼ������
	------------------------------
	pScheduleList -	����ʼ���ı������ȴ�������ṹָ��
	pTargetCache -	����һ�������ص�Ŀ�껺��ָ��
	pPilotCache -	����һ������������������ָ��
*/
EXPORT
void DetectScheduleListInitialize( 
			struct DetectScheduleList *pDSList,
			struct DetectCycleSearchCache *pDCSCache,
			struct DetectOnceSearchCache *pDOSCache
								);

/*
	������������ṹ��պ���
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

