
/*
    �ļ�˵��:   ��������ͷ�ļ�
    �ļ�����:   jsf
    �ļ�ʱ��:   2014.3.13
*/

#ifndef __DETECT_SCHEDULE_HEADER__
#define __DETECT_SCHEDULE_HEADER__
							

////////////////////////////////////////////////////////////////////////////////

struct DetectSchedule
{
	struct DetectAntennaArea	scheduleArea;
	struct DetectScheduleTable	table;
    
    struct DetectSearchCmdCache	cmdCache;
	BOOL	cmdCacheStatus;
    
    struct DetectSearchFeedBack	feedBack;

    struct DetectScheduleList	cacheList;

    struct DetectScheduleAntennaNode antennaNode;

    float	courseAngle;

    SEM_ID	sync;
	BOOL status;
};

#ifdef __DEBUG__
static void __attribute__((unused))
DetectSchedulePrint( int log, struct DetectSchedule *pDSchedule )
{
	struct DetectScheduleList *pDSList =    &pDSchedule->cacheList;
    struct DetectCycleSearch *pDCSearch =   &pDSList->cycleSearch;
    struct ListHead *pLhHead =  &pDCSearch->list;
    printlog( log, "Result\n" );
	printlog( log, "DetectSchedulePrint cycle list \n" );
    ListHeadListPrint( log, pLhHead );
    printlog( log, "DetectSchedulePrint scheduleArea\n" );
    DetectAntennaAreaPrint( log, &pDSchedule->scheduleArea );
    
}
#endif


EXPORT
struct DetectSchedule *DetectScheduleInstanceGet( void );

EXPORT
void DetectScheduleInitialize(
				struct DetectSchedule	*pDSchedule,
				float courseAngle
							);   

EXPORT
void DetectScheduleReset(
				struct DetectSchedule	*pDSchedule,
				float courseAngle
						);    

////////////////////////////////////////////////////////////////////////////////
//��ʼ��
EXPORT
void DetectScheduleInit( void );

EXPORT
void DetectScheduleRestart( void );    

////////////////////////////////////////////////////////////////////////////////
//�����
EXPORT
BOOL DetectNavigationInfoCmdProc(
						unsigned char *pBuf,
						int size
								);

EXPORT
BOOL DetectScheduleCycleSearchCmdProc( 
							unsigned char *pBuf, 
							int size 
									);

EXPORT
BOOL DetectScheduleTargetSearchCmdProc(
							unsigned char *pBuf,
							int size
									);

EXPORT
BOOL DetectSchedulePilotSearchCmdProc(
							unsigned char *pBuf,
							int size
									);    

////////////////////////////////////////////////////////////////////////////////
//���ݴ���
EXPORT
void DetectScheduleDataProc( 
					float courseAngle 
							);

//���ȴ���
EXPORT
struct DetectScheduleAntennaNode *DetectScheduleScheduleProc( void );    

#endif

