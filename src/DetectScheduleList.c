

/*
	������������ʵ���ļ�
	--------------------
*/

#include "..\Header.h"


////////////////////////////////////////////////////////////////////////////////

/*
	�������ȴ�������ṹ��ʼ������
	------------------------------
	pScheduleList -	����ʼ���ı������ȴ�������ṹָ��
	pTargetCache -	����һ�������ص�Ŀ�껺��ָ��
	pPilotCache -	����һ������������������ָ��
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
	�������ȴ�������ṹ��պ���
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
	���µ�����Ϊ:
		1 ��ѭ��������Ҫ����
		2 ���1��������һ����������Ϊ�գ���һ��������Ҫ����
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



