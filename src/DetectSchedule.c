
/*
    �ļ�˵��:   ��������ʵ���ļ�
    �ļ�����:   jsf
    �ļ�ʱ��:   2014.3.13
*/

#include "..\Header.h"


////////////////////////////////////////////////////////////////////////////////
static struct DetectSchedule gDetectSchedule_t
#ifdef __GNU_C__
		=	{
				.scheduleArea =	{ 0 },
                .table =	{ 0 },
                .cmdCache =	{ 0 },
                .feedBack =	{ 0 },
                .cacheList =	{ 0 },
                .sync =	NULL,
                .status =	FALSE,
			}
#endif
;

struct DetectSchedule *DetectScheduleInstanceGet( void )
{
	return &gDetectSchedule_t;
}

void DetectScheduleInit( void )
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
    DetectScheduleInitialize( pDSchedule, 0.0 );
}

void DetectScheduleRestart( void )
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
    DetectScheduleReset( pDSchedule, 0.0 );
}


void DetectScheduleInitialize(
				struct DetectSchedule *pDSchedule,
				float courseAngle
							)
{
	struct DetectAntennaArea *pDAArea =	NULL;
    struct DetectScheduleTable *pTable =	NULL;
    struct DetectScheduleList *pDSList =	NULL;
	struct DetectCycleSearchCache *pDCSCache =	NULL;
    struct DetectOnceSearch *pDOSearch =	NULL;
    struct DetectOnceSearchCache *pDOSCache =	NULL;
	assert( NULL != pDSchedule );

    pDAArea =	&pDSchedule->scheduleArea;
	pTable =	&pDSchedule->table;
    pDSList =	&pDSchedule->cacheList;

	pDCSCache =	&pDSchedule->cmdCache.cycleSearch;
    pDOSCache =	&pDSchedule->cmdCache.pilotSearch;

	//��ʼ�������
    DetectSearchCmdCacheInitialize( &pDSchedule->cmdCache );
	//��ʼ�������
	DetectSearchFeedBackInitialize( &pDSchedule->feedBack );
    //��ʼ����������
	DetectScheduleListInitialize( pDSList, pDCSCache, pDOSCache );
    //��ʼ�����ȱ�
    DetectScheduleTableInitialize( pTable, 401 );
	//��ʼ�������
	pDSchedule->courseAngle =	courseAngle;
    //��ʼ�����������Ϣ
    DetectAntennaAreaInitialize( pDAArea, 0.0 );
	//��ʼ�����Ƚ����Ϣ
    pDOSearch =	&pDSList->pilotSearch;    
	DetectScheduleAntennaNodeInitialize( &pDSchedule->antennaNode, pTable,
        									pDOSearch, pDAArea );

	pDSchedule->sync =	semBCreate( SEM_Q_FIFO, SEM_FULL );
    pDSchedule->status =	TRUE;
}				

void DetectScheduleReset(
				struct DetectSchedule	*pDSchedule,
				float courseAngle
						)
{
    struct DetectAntennaArea *pDAArea = NULL;
    struct DetectScheduleTable *pTable =    NULL;
    struct DetectScheduleList *pDSList =    NULL;

    struct DetectSearchNode *pDSNode =	NULL;
    
    assert( NULL != pDSchedule );
//    printlog( REC, "DetectScheduleReset : pDSchedule=%p\n", pDSchedule );

    pDAArea =   &pDSchedule->scheduleArea;
    pTable =    &pDSchedule->table;
    pDSList =   &pDSchedule->cacheList;

    //���������
    DetectSearchCmdCacheReset( &pDSchedule->cmdCache );
    //���������
    DetectSearchFeedBackReset( &pDSchedule->feedBack );
    //���õ�������
    DetectScheduleListReset( pDSList );
    //���õ��ȱ�
    DetectScheduleTableReset( pTable );
	pDSNode =	DetectSearchNodeCacheGet();
    if ( NULL != pDSNode )
    {
    	pDSNode->type =	DetectPilotStatusType;
    	//�������뵽������������һ��λ��
    	pTable->pTable[400] =	pDSNode;
    }
    else
    {
    	printlog( SCR, "DetectScheduleReset failed - Get DetectSearchNode failed\n" );
    }
    
    //���ú����
    pDSchedule->courseAngle =   courseAngle;
    //�������������Ϣ
    DetectAntennaAreaReset( pDAArea, courseAngle );
    //���õ��Ƚ����Ϣ 
    DetectScheduleAntennaNodeReset( &pDSchedule->antennaNode );
}


struct WarShipPoseAngle
{
	struct ExternalHead	head;		//��Ϣͷ
	unsigned short  validation;     //��Ч���
	unsigned short  reserve3;       // ����
	unsigned int	courseAngle;	//�����
	unsigned int	pitchingAngle;	//��ҡ��
	unsigned int	rollingAngle;	//��ҡ��
}__attribute__( (packed) ) ;

IMPORT float	hxjnet_back;
IMPORT float	zyjnet_back;
IMPORT float	hyjnet_back;
IMPORT unsigned short	hxjnetCount;

#define MAX_HXJ	0.5 /*hxjҰֵ��Χ*/
#define MAX_HYJ	1 /*hyjҰֵ��Χ*/
#define MAX_ZYJ	1 /*zyjҰֵ��Χ*/

//Ұֵ������ú���
IMPORT void FightParValidValueFlagSet( unsigned char flag );
//��������ú���
IMPORT void FightParCourseAngleSet( float angle );
//��ҡ�����ú���
IMPORT void FightParRollingAngleSet( float angle );
//��ҡ�����ú���
IMPORT void FightParPitchingAngleSet( float angle );
IMPORT STATUS FightParSemaTake( int timeOut );
IMPORT STATUS FightParSemaGive();



BOOL DetectNavigationInfoCmdProc(
	    				unsigned char *pBuf,
	    				int size
    							)
{
	struct WarShipPoseAngle *pWSPAngle =	NULL;
	float hxj_temp =	0.0;
    float hyj_temp =	0.0;
    float zyj_temp =	0.0;
    unsigned char flag =	0;
    
	assert( NULL != pBuf );

    pWSPAngle =	( struct WarShipPoseAngle * )pBuf;

    printlog( REC, "DetectNavigationInfoCmdProc\n" );

	hxj_temp = (float)((float)pWSPAngle->courseAngle*180/pow(2,30));
	hyj_temp = (float)((float)pWSPAngle->rollingAngle*90/pow(2,30));
	zyj_temp = (float)((float)pWSPAngle->pitchingAngle*90/pow(2,30));

	//�޳�Ұֵ����
	if ((hxj_temp>360.0)||(hxj_temp<0.0)||(hyj_temp>45.0)||(hyj_temp<-45.0)||(zyj_temp>30.0)||(zyj_temp<-30.0))
	{
		hxj_temp =	hxjnet_back;
		zyj_temp =	zyjnet_back;
		hyj_temp =	hyjnet_back;	
        flag =	1;
	}
	else
	{		
		float temp_cha =	0.0;
						
		temp_cha =	(hxj_temp-hxjnet_back);
		if ( fabs(temp_cha)>180.0 )
		{
			temp_cha=fabs(temp_cha)-360.;
		}
		if ((fabs(temp_cha)>=MAX_HXJ)||fabs(zyj_temp-zyjnet_back)>=MAX_HYJ||fabs(hyj_temp-hyjnet_back)>=MAX_ZYJ)
		{//ʹ����һ�ε�ֵ
			hxjnetCount++;
			if (hxjnetCount>=3)
			{
				hxjnet_back=hxj_temp;
				zyjnet_back=zyj_temp;
				hyjnet_back=hyj_temp;
				
				hxjnetCount=0;

                flag =	0;	//�����̬�ǲ�ΪҰֵ
			}
			else
			{
				hxj_temp=hxjnet_back;
				zyj_temp=zyjnet_back;
				hyj_temp=hyjnet_back;

				flag =	1;	//�����̬��ΪҰֵ						
			}				
		}
		else
		{
			hxjnetCount=0;
			hxjnet_back=hxj_temp;
			zyjnet_back=zyj_temp;
			hyjnet_back=hyj_temp;

			flag =	0;	//�����̬�ǲ���Ұֵ
		}								
	}
#if 0
	//���Ӵ�ģ����������̬�ǹ���
	if ((pcmdb0->valid_flag==0xaaaa)&&(g_fightPar.hxjpara_flag==1))  //��Ч���Ϊ0xaaaa��ʾ�Ǵ����ģ�������͹�������Ϣ
	{
		g_fightPar.flag_wk = 1;
	}
	else     //��14��������������̬�Ǳ��� ��0
	{
		g_fightPar.flag_wk = 0;
	}
#endif

	FightParSemaTake( WAIT_FOREVER );
	FightParValidValueFlagSet( flag );
	FightParCourseAngleSet( hxj_temp );
	FightParRollingAngleSet( hyj_temp );
    FightParPitchingAngleSet( zyj_temp );
    FightParSemaGive();

    return TRUE;
}    							

BOOL DetectScheduleCycleSearchCmdProc( 
							unsigned char *pBuf, 
							int size 
									)
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
    struct DetectCycleSearchCache *pDCSCache =	&pDSchedule->cmdCache.cycleSearch;

    assert( NULL != pBuf );

    return DetectCycleSearchCmdProc( pDCSCache, pBuf, size );
}									

BOOL DetectScheduleTargetSearchCmdProc(
							unsigned char *pBuf,
							int size
									)
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
    struct DetectPeriodSearchCache *pDPSCache =	&pDSchedule->cmdCache.targetSearch;
    struct DetectScheduleTable *pDSTable =	&pDSchedule->table;

	printlog( REC, "DetectScheduleTargetSearchCmdProc\n" );
    return DetectTargetSearchCmdProc( pDPSCache, pDSTable, pBuf, size );
}									

BOOL DetectSchedulePilotSearchCmdProc(
							unsigned char *pBuf,
							int size
									)
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
	struct DetectOnceSearchCache *pDOSCache =	&pDSchedule->cmdCache.pilotSearch;

    return DetectPilotSearchCmdProc( pDOSCache, pBuf, size );
}									

////////////////////////////////////////////////////////////////////////////////
//���ݴ���
void DetectScheduleDataProc( 
					float courseAngle
							)
{
    struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
//    struct DetectPeriodSearchCache *pDPSCache =	&pDSchedule->cmdCache.targetSearch;
    //����Ƿ��б�������
	//�����������
	//����ѭ��������һ����������
	struct DetectScheduleList *pDSList =	&pDSchedule->cacheList;
    struct DetectAntennaArea *pDAArea =	&pDSchedule->scheduleArea; 

//    printlog( REC, "DetectScheduleDataProc\n" );

	//��鵱ǰ��������ĸ����־
    DetectScheduleListCheck( pDSList );
    
    if ( DetectScheduleListNeedsUpdate( pDSList ) )
    {	
    	BOOL cycleStatus =	DetectScheduleListCycleStatusGet( pDSList );
        
        DetectScheduleListUpdate( pDSList );

        //�γ����µ�ѭ����������
    	//���֮ǰ������
    	if ( cycleStatus )
        {
	        //�γɽ����Ϣ�󣬽����еĽ����Ϣת�������������Ϣ��
        	struct DetectCycleSearch *pDCSearch =	&pDSList->cycleSearch;
        	struct ListHead *pLhHead =	&pDCSearch->list;

			DetectAntennaAreaClear( pDAArea );
            
            //����ƫ�ƽǶ�
			DetectAntennaAreaOffsetUpdate( pDAArea, courseAngle );
			
            //�����ݲ��뵽���������Ϣ��
            DetectAntennaAreaListPut( pDAArea, pLhHead );
            //���½���ʶ
            DetectAntennaAreaColorSet( pDAArea );
            
            DetectAntennaAreaPrint( LOG, pDAArea );
        }
    }
    else
    {
    	//����������û�з����仯ʱ�����ݵ�ǰ�ĺ���Ƕ���Ϣ���в���
    	BOOL isChanged =	FALSE;
//        printlog( REC,  "DetectScheduleDataProc\tNo need to update list\n" );
        isChanged =	DetectAntennaAreaOffsetUpdate( pDAArea, courseAngle );
        if ( isChanged )
        {	
        	DetectAntennaAreaUpdate( pDAArea );
        }
    }
    //�����ص�Ŀ����������
    //���ص�Ŀ�괦���ڽ��������ʱһ�������˵������ݴ��������ò���
}

//���ȴ���
struct DetectScheduleAntennaNode *DetectScheduleScheduleProc( void )
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
//    struct DetectSearchNode *pDSNode =	NULL;
    struct DetectScheduleAntennaNode *pDSANode =	&pDSchedule->antennaNode;
	
    struct DetectScheduleTable *pDSTable =	&pDSchedule->table;

//    printlog( REC, "DetectScheduleScheduleProc\n" );

	//��ȡ���ȽǶȽ����Ϣ
	DetectScheduleAntennaNodeExtract( pDSANode, pDSchedule->courseAngle );
    
    //����tick
    DetectScheduleTableTick( pDSTable );

    {
       	struct DetectAntennaArea *pDAArea =	&pDSchedule->scheduleArea;
//        DetectAntennaAreaPrint( REC, pDAArea );
    }
	
	return pDSANode;
}


