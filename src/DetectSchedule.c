
/*
    文件说明:   被动调度实现文件
    文件作者:   jsf
    文件时间:   2014.3.13
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

	//初始化命令缓存
    DetectSearchCmdCacheInitialize( &pDSchedule->cmdCache );
	//初始化命令反馈
	DetectSearchFeedBackInitialize( &pDSchedule->feedBack );
    //初始化调度链表
	DetectScheduleListInitialize( pDSList, pDCSCache, pDOSCache );
    //初始化调度表
    DetectScheduleTableInitialize( pTable, 401 );
	//初始化航向角
	pDSchedule->courseAngle =	courseAngle;
    //初始化阵面调度信息
    DetectAntennaAreaInitialize( pDAArea, 0.0 );
	//初始化调度结点信息
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

    //重置命令缓存
    DetectSearchCmdCacheReset( &pDSchedule->cmdCache );
    //重置命令反馈
    DetectSearchFeedBackReset( &pDSchedule->feedBack );
    //重置调度链表
    DetectScheduleListReset( pDSList );
    //重置调度表
    DetectScheduleTableReset( pTable );
	pDSNode =	DetectSearchNodeCacheGet();
    if ( NULL != pDSNode )
    {
    	pDSNode->type =	DetectPilotStatusType;
    	//将结点插入到调度链表的最后一个位置
    	pTable->pTable[400] =	pDSNode;
    }
    else
    {
    	printlog( SCR, "DetectScheduleReset failed - Get DetectSearchNode failed\n" );
    }
    
    //重置航向角
    pDSchedule->courseAngle =   courseAngle;
    //重置阵面调度信息
    DetectAntennaAreaReset( pDAArea, courseAngle );
    //重置调度结点信息 
    DetectScheduleAntennaNodeReset( &pDSchedule->antennaNode );
}


struct WarShipPoseAngle
{
	struct ExternalHead	head;		//信息头
	unsigned short  validation;     //有效标记
	unsigned short  reserve3;       // 备份
	unsigned int	courseAngle;	//航向角
	unsigned int	pitchingAngle;	//纵摇角
	unsigned int	rollingAngle;	//横摇角
}__attribute__( (packed) ) ;

IMPORT float	hxjnet_back;
IMPORT float	zyjnet_back;
IMPORT float	hyjnet_back;
IMPORT unsigned short	hxjnetCount;

#define MAX_HXJ	0.5 /*hxj野值范围*/
#define MAX_HYJ	1 /*hyj野值范围*/
#define MAX_ZYJ	1 /*zyj野值范围*/

//野值标记设置函数
IMPORT void FightParValidValueFlagSet( unsigned char flag );
//航向角设置函数
IMPORT void FightParCourseAngleSet( float angle );
//横摇角设置函数
IMPORT void FightParRollingAngleSet( float angle );
//纵摇角设置函数
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

	//剔除野值功能
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
		{//使用上一次的值
			hxjnetCount++;
			if (hxjnetCount>=3)
			{
				hxjnet_back=hxj_temp;
				zyjnet_back=zyj_temp;
				hyjnet_back=hyj_temp;
				
				hxjnetCount=0;

                flag =	0;	//标记姿态角不为野值
			}
			else
			{
				hxj_temp=hxjnet_back;
				zyj_temp=zyjnet_back;
				hyj_temp=hyjnet_back;

				flag =	1;	//标记姿态角为野值						
			}				
		}
		else
		{
			hxjnetCount=0;
			hxjnet_back=hxj_temp;
			zyjnet_back=zyj_temp;
			hyjnet_back=hyj_temp;

			flag =	0;	//标记姿态角不是野值
		}								
	}
#if 0
	//增加从模拟器接收姿态角功能
	if ((pcmdb0->valid_flag==0xaaaa)&&(g_fightPar.hxjpara_flag==1))  //有效标记为0xaaaa表示是从武控模拟器发送过来的信息
	{
		g_fightPar.flag_wk = 1;
	}
	else     //从14所来的正常的姿态角报文 置0
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
//数据处理
void DetectScheduleDataProc( 
					float courseAngle
							)
{
    struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
//    struct DetectPeriodSearchCache *pDPSCache =	&pDSchedule->cmdCache.targetSearch;
    //检查是否有被动命令
	//处理调度链表
	//处理循环搜索和一次搜索命令
	struct DetectScheduleList *pDSList =	&pDSchedule->cacheList;
    struct DetectAntennaArea *pDAArea =	&pDSchedule->scheduleArea; 

//    printlog( REC, "DetectScheduleDataProc\n" );

	//检查当前调度链表的各项标志
    DetectScheduleListCheck( pDSList );
    
    if ( DetectScheduleListNeedsUpdate( pDSList ) )
    {	
    	BOOL cycleStatus =	DetectScheduleListCycleStatusGet( pDSList );
        
        DetectScheduleListUpdate( pDSList );

        //形成了新的循环搜索命令
    	//清空之前的数据
    	if ( cycleStatus )
        {
	        //形成结点信息后，将所有的结点信息转移至阵面调度信息中
        	struct DetectCycleSearch *pDCSearch =	&pDSList->cycleSearch;
        	struct ListHead *pLhHead =	&pDCSearch->list;

			DetectAntennaAreaClear( pDAArea );
            
            //设置偏移角度
			DetectAntennaAreaOffsetUpdate( pDAArea, courseAngle );
			
            //将数据插入到阵面调度信息中
            DetectAntennaAreaListPut( pDAArea, pLhHead );
            //更新结点标识
            DetectAntennaAreaColorSet( pDAArea );
            
            DetectAntennaAreaPrint( LOG, pDAArea );
        }
    }
    else
    {
    	//当搜索命令没有发生变化时，根据当前的航向角度信息进行操作
    	BOOL isChanged =	FALSE;
//        printlog( REC,  "DetectScheduleDataProc\tNo need to update list\n" );
        isChanged =	DetectAntennaAreaOffsetUpdate( pDAArea, courseAngle );
        if ( isChanged )
        {	
        	DetectAntennaAreaUpdate( pDAArea );
        }
    }
    //处理重点目标搜索命令
    //因重点目标处理在进行命令处理时一并进行了调度数据处理，不采用操作
}

//调度处理
struct DetectScheduleAntennaNode *DetectScheduleScheduleProc( void )
{
	struct DetectSchedule *pDSchedule =	DetectScheduleInstanceGet();
//    struct DetectSearchNode *pDSNode =	NULL;
    struct DetectScheduleAntennaNode *pDSANode =	&pDSchedule->antennaNode;
	
    struct DetectScheduleTable *pDSTable =	&pDSchedule->table;

//    printlog( REC, "DetectScheduleScheduleProc\n" );

	//获取调度角度结点信息
	DetectScheduleAntennaNodeExtract( pDSANode, pDSchedule->courseAngle );
    
    //更新tick
    DetectScheduleTableTick( pDSTable );

    {
       	struct DetectAntennaArea *pDAArea =	&pDSchedule->scheduleArea;
//        DetectAntennaAreaPrint( REC, pDAArea );
    }
	
	return pDSANode;
}


