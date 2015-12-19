
#include "..\Header.h"


struct DetectCycleSearchCmd sDetectCycleSearchCmd
#ifdef __GNU_C__
		=
		{
			.head.id =	0x0d86,
            .head.len =	sizeof( struct DetectCycleSearchCmd ),
		}
#endif	
	;
void DetectCycleSearchCmdSet( 
    			unsigned short	operateType, 
    			unsigned short	areaCode,
    			unsigned short	tapPosition,
    			float	start,
    			float 	end
    							)
{
	struct DetectCycleSearchCmd *pDCSCmd =	&sDetectCycleSearchCmd;

    pDCSCmd->head.id =	0x0d86;
    pDCSCmd->head.len =	sizeof( struct DetectCycleSearchCmd );
    
	pDCSCmd->operateType =	operateType;
    pDCSCmd->areaCode =	areaCode,
    pDCSCmd->tapPosition =	tapPosition;
    pDCSCmd->startAngle =	start;
    pDCSCmd->endAngle =	end;
}    							

void DetectCycleSearchCmdSend()
{
    DetectCmdMsgQCacheSend( (unsigned char *)&sDetectCycleSearchCmd,
        						sizeof(struct DetectCycleSearchCmd) );
}

void DetectCycleSearchCmdTestStart()
{
    DetectCycleSearchCmdSet( DETECT_CYCLE_SEARCH_OPERATE_CREATE, 1, 1, 45.0, 180.0 );
    DetectCycleSearchCmdSend();
}

void DetectCycleSearchCmdTestEnd()
{
    DetectCycleSearchCmdSet( DETECT_CYCLE_SEARCH_OPERATE_REMOVE, 1, 1, 45.0, 180.0 );
    DetectCycleSearchCmdSend();
}


void DetectCycleSearchCmdMonitor()
{
	DetectCycleSearchCmdSet( DETECT_CYCLE_SEARCH_OPERATE_CREATE, 1, 1, 45.0, 180.0 );
    DetectCycleSearchCmdSend();

    DetectCycleSearchCmdSet( DETECT_CYCLE_SEARCH_OPERATE_CREATE, 2, 0, 245.0, 280.0 );
    DetectCycleSearchCmdSend();
}    

struct DetectPilotSearchCmd sDetectPilotSearchCmd
#ifdef __GNU_C__
		=
		{
			.head.id =	0x0d8a,
            .head.len =	sizeof( struct DetectPilotSearchCmd ),
            .operateType =	DETECT_CYCLE_SEARCH_OPERATE_CREATE
		}
#endif	
	;

void DetectPilotSearchCmdSet(
    				float	angle
    						)
{
	struct DetectPilotSearchCmd *pDPSCmd =	&sDetectPilotSearchCmd;
    
    pDPSCmd->head.id =	0x0d8a;
    pDPSCmd->head.len =	sizeof( struct DetectPilotSearchCmd );
    pDPSCmd->operateType =	DETECT_CYCLE_SEARCH_OPERATE_CREATE;
    
    pDPSCmd->angle =	angle;
}    						

void DetectPilotSearchCmdSend()
{
    DetectCmdMsgQCacheSend( (unsigned char *)&sDetectPilotSearchCmd, 
        					sizeof( struct DetectPilotSearchCmd ) );   
}

void DetectPilotSearchCmdMonitor()
{
	DetectPilotSearchCmdSet( 91.0 );
    DetectPilotSearchCmdSend();
}    

struct DetectTargetSearchCmd sDetectTargetSearchCmd
#ifdef __GNU_C__
			=
			{
				.head.id =	0x0d89,
				.head.len =	sizeof( struct DetectTargetSearchCmd )
			}
#endif
			;

void DetectPeriodSearchCmdSet(
    			unsigned short	operateType,
    			unsigned short	targetNo,
    			float	angle
    						)
{
	struct DetectTargetSearchCmd *pDTSCmd =	&sDetectTargetSearchCmd;

	pDTSCmd->head.id =	0x0d89;
    pDTSCmd->head.len =	sizeof( struct DetectTargetSearchCmd );
    
    pDTSCmd->operateType =	operateType;
    pDTSCmd->targetNo =	targetNo;
    pDTSCmd->angle =	angle;
}

void DetectPeriodSearchCmdSend()
{
    DetectCmdMsgQCacheSend( (unsigned char *)&sDetectTargetSearchCmd, 
        					sizeof( struct DetectTargetSearchCmd ) );
}

void DetectPeriodSearchCmdMonitor()
{
	DetectPeriodSearchCmdSet( DETECT_CYCLE_SEARCH_OPERATE_CREATE, 0xfeed, 10.0 );
    DetectPeriodSearchCmdSend();
}

static void Test()
{
	float start =	0.0;
    unsigned int calc =	0;
	while ( 1 )
    {
    	if ( FLOAT_COMPARE_BIG == FloatCompare( start, 359.99 ) )
        {
        	start =	0.0;
        }
    	DetectPilotSearchCmdSet( start );
    	DetectPilotSearchCmdSend();
    	start +=	10.0;
        taskDelay( sysClkRateGet() );

        calc ++;
        if ( 100 == calc )
        {
        	DetectProcReset();
            calc =	0;
        }
    }
}

void Step();

static void Run()
{
	while ( 1 )
    {
    	Step();
        taskDelay( sysClkRateGet()/20 );
    }
}

void Start()
{
	DetectLogRecInit();
	DetectProcInitialize();
}

void AutoStart()
{
	printf( "Start to Initialize\n" );
    DetectLogRecInit();
	DetectProcInitialize();
    DetectProcReset();
    DetectPeriodSearchCmdMonitor();
    DetectCycleSearchCmdMonitor();
    DetectPilotSearchCmdMonitor();
    taskSpawn( "test", 1, VX_FP_TASK, 0x10240, 
        			(FUNCPTR)Test, 
        			0, 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    taskSpawn( "Run", 1, VX_FP_TASK, 0x10240, 
        			(FUNCPTR)Run, 
        			0, 1, 2, 3, 4, 5, 6, 7, 8, 9 );
}


void Step()
{
	static float courseAngle =	0.0;
//	printlog( REC, "Start to Procedure\n" );
    
    DetectScheduleProc( courseAngle );
	courseAngle +=	1.0;
}

void Steps( int step )
{
	int i =	0;
    for ( i=0; i<step; ++i )
    {
    	Step();
    }
}

void DetectCycleSearchCmdMonitorFullArea( void )
{
	struct DetectCycleSearchCmd cmd;

    cmd.head.id =	0x0d86;
    cmd.head.len =	sizeof( struct DetectCycleSearchCmd );
    
	cmd.operateType =	DETECT_CYCLE_SEARCH_OPERATE_CREATE;
    cmd.areaCode =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE,
    cmd.tapPosition =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_TAPPOSITION;
    cmd.startAngle =	0.0;
    cmd.endAngle =	360.0;
    cmd.startFrequency =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_START_FREQ;
    cmd.endFrequency =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_END_FREQ;

    DetectCmdMsgQCacheSend( (unsigned char *)&cmd, sizeof(struct DetectCycleSearchCmd) );
}

void DetectCycleSearchCmdMonitorFullAreaExit( void )
{
	struct DetectCycleSearchCmd cmd;

    cmd.head.id =	0x0d86;
    cmd.head.len =	sizeof( struct DetectCycleSearchCmd );
    
	cmd.operateType =	DETECT_CYCLE_SEARCH_OPERATE_REMOVE;
    cmd.areaCode =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE,
    cmd.tapPosition =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_TAPPOSITION;
    cmd.startAngle =	0.0;
    cmd.endAngle =	360.0;
    cmd.startFrequency =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_START_FREQ;
    cmd.endFrequency =	DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_END_FREQ;

    DetectCmdMsgQCacheSend( (unsigned char *)&cmd, sizeof(struct DetectCycleSearchCmd) );
}    

BOOL IsDetectCycleSearchCmdMonitorCmd( 
					struct DetectCycleSearchCache *pDCSCache
									)
{
	int i =	0;
    int cmdNum =	0;

    assert( NULL != pDCSCache );
    
    for ( i=0; i<DETECT_SEARCH_CMD_CYCLE_MAX_NUM; ++i )
    {
    	if ( pDCSCache->color[i] )
        {
        	cmdNum ++;
        }
    }
    printlog( SCR, "preproc cmdNum=%d color=%d\n", 
        	cmdNum, pDCSCache->color[ DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE-1 ]);
    if ( 1 == cmdNum )
    {
    	if ( pDCSCache->color[ DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE-1 ] )
        {
        	struct DetectSearchArea *pDSArea =	
                	pDCSCache->area+DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE-1;
            if ( 
                DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_TAPPOSITION == pDSArea->tapPosition
                && DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_START_FREQ == pDSArea->startFrequency 
                && DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_END_FREQ == pDSArea->endFrequency 
                )
            {
            	return TRUE;
            }
        }
    }
    return FALSE;
}

void DetectCycleSearchCmdMonitorReset(
					struct DetectCycleSearchCache *pDCSCache
									)
{
	assert( NULL != pDCSCache );
    
	pDCSCache->color[ DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE-1 ] =	FALSE;
    DetectCycleSearchCacheStatusSet( pDCSCache, TRUE );
}





