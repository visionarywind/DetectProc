
#include "..\Header.h"


/*
	由被动循环搜索命令提取被动搜索区域结构
	--------------------------------------
	pSearchArea -	存放提取结果的被动搜索区域结构指针
	pCycleSearch -	被动循环搜索命令缓存指针
	type -	被动循环搜索类型 DETECT_SEARCH_TYPE_NORMAL、DETECT_SEARCH_TYPE_SLOW
*/
void DetectCycleSearchAreaGetByCmd( 
				struct DetectSearchArea *pSearchArea,
				const struct DetectCycleSearchCmd *pCycleSearch
								)
{
	if ( NULL != pSearchArea && NULL != pCycleSearch )
    {
    	pSearchArea->startAngle =	pCycleSearch->startAngle;	//起始角度
    	pSearchArea->endAngle =	pCycleSearch->endAngle;			//结束角度

		if ( FLOAT_COMPARE_SMALL == 
            	FloatCompare( pSearchArea->startAngle, DETECT_SEARCH_AREA_MIN_ANGLE ) )
        {
        	pSearchArea->startAngle =	DETECT_SEARCH_AREA_MIN_ANGLE;
        }
        if ( FLOAT_COMPARE_BIG == 
            	FloatCompare( pSearchArea->endAngle, DETECT_SEARCH_AREA_MAX_ANGLE ) )
        {
        	pSearchArea->endAngle =	DETECT_SEARCH_AREA_MAX_ANGLE;
        }
        
        pSearchArea->stayPeriod =	pCycleSearch->stayTime;		//驻留时间
        pSearchArea->type =	DetectCycleSearchType;			//搜索类型
        pSearchArea->areaNo =	pCycleSearch->areaCode;			//区号
        pSearchArea->tapPosition =	pCycleSearch->tapPosition;

        pSearchArea->startFrequency =	pCycleSearch->startFrequency;
        pSearchArea->endFrequency =	pCycleSearch->endFrequency;

        printlog( SCR, "DetectCycleSearchAreaGetByCmd : tapPosition=%d, start=%x,"
            			"end=%x\n",
            				pSearchArea->tapPosition, 
            				pSearchArea->startFrequency,
            				pSearchArea->endFrequency
            				);

        ListHeadInitialize( &( pSearchArea->listHead ) );
    }
   	else
    {
    	printfSafe( "Exception - DetectCycleSearchAreaGetByCmd\n" );
    }
}

/*
	由被动重点目标搜索命令提取被动搜索区域结构
	pSearchArea -	待提取的被动搜索区域结构
	pTargetSearch -	被动重点目标搜索命令缓存指针
*/
void DetectTargetSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectTargetSearchCmd *pTargetSearch 
								)
{
	if ( NULL != pSearchArea && NULL != pTargetSearch )
    {
    	pSearchArea->startAngle =	pTargetSearch->angle;	//起始角度
    	pSearchArea->endAngle =	pTargetSearch->angle;			//结束角度
        pSearchArea->stayPeriod =	pTargetSearch->stayTime;		//驻留时间
        pSearchArea->type =	DetectTargetSearchType;			//搜索类型
        pSearchArea->areaNo =	pTargetSearch->targetNo;			//区号
        pSearchArea->targetNo =	pTargetSearch->targetNo;			//区号

//        pSearchArea->startFrequency =	pTargetSearch->startFrequency;
//        pSearchArea->endFrequency =	pTargetSearch->endFrequency;

        ListHeadInitialize( &( pSearchArea->listHead ) );
    }
}

/*
	由被动任务引导搜索命令提取被动搜索区域结构
	pSearchArea -	待提取的被动搜索区域结构
	pTargetSearch -	被动任务引导搜索命令缓存指针
*/
void DetectPilotSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectPilotSearchCmd *pPilotSearch 
								)
{
	if ( NULL != pSearchArea && NULL != pPilotSearch )
    {
    	pSearchArea->startAngle =	pPilotSearch->angle;	//起始角度
    	pSearchArea->endAngle =	pPilotSearch->angle;			//结束角度
        pSearchArea->stayPeriod =	pPilotSearch->stayTime;		//驻留时间
        pSearchArea->type =	DetectPilotSearchType;			//搜索类型
//        pSearchArea->areaNo =	pPilotSearch->targetNo;			//区号

//        pSearchArea->startFrequency =	pPilotSearch->startFrequency;
//        pSearchArea->endFrequency =	pPilotSearch->endFrequency;

        ListHeadInitialize( &( pSearchArea->listHead ) );
    }
}



