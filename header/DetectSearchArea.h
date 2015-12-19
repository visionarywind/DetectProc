
#ifndef __DETECT_SEARCH_AREA_HEADER__
#define __DETECT_SEARCH_AREA_HEADER__

/*
	被动搜索类型
*/
enum DetectSearchType
{
    DetectCycleSearchType =	0,		//被动循环搜索
    DetectTargetSearchType =	1,	//被动重点目标搜索
    DetectPilotSearchType =	2,		//被动任务引导搜索
    DetectWaitStatusType =	3,
    DetectPilotStatusType =	4,
    DetectUnknownSearchType =	5	//未知搜索类型
};
#define DETECT_PILOT_STATUS_TYPE	( 4 )

#define DETECT_SEARCH_AREA_SLOW_TAP_POSITION	1
#define DETECT_SEARCH_AREA_QUICK_TAP_POSITION	0

#define DETECT_SEARCH_STAY_PERIOD		( 50.0 )

#define DETECT_SEARCH_AREA_MIN_ANGLE	( 0.0 )
#define DETECT_SEARCH_AREA_MAX_ANGLE	( 359.99 )
/* 
    被动搜索区域结构
*/
struct DetectSearchArea
{
	float	startAngle;	//起始角度
    float	endAngle;	//结束角度
    unsigned short	stayPeriod;	//驻留时间

    enum DetectSearchType	type;		//搜索类型
    unsigned char   tapPosition;    //档位

    unsigned short	areaNo;		//区号	 
    unsigned int	targetNo;		//任务批号

    unsigned int	startFrequency;	//起始频率
   	unsigned int	endFrequency;	//结束频率

	struct ListHead listHead;		//链表指针
};


#define GetDetectSearchAreaPointer( ptr )	\
			GetPointer(ptr, struct DetectSearchArea, listHead)


#ifdef __DEBUG__
static void __attribute__((unused))
DetectSearchAreaPrint(
			    int log,
				const struct DetectSearchArea *pDSArea
					)
{
	if ( NULL != pDSArea )
    {
    	printlog( log, "DetectSearchArea : angle=%f, stayPeriod=%d, type=%d, targetNo=%d\n",
        		pDSArea->startAngle, pDSArea->stayPeriod, pDSArea->type, pDSArea->targetNo );
    }
	else
	{
		printlog( log, "DetectSearchArea NULL\n" );
	}
}	

static void __attribute__((unused))
DetectSearchAreaListPrint(
				int log,
				const struct ListHead *pLhHead
						)
{
	struct ListHead *pLhPtr =	NULL;
	assert( NULL != pLhHead );

	printlog( log, "DetectSearchAreaListPrint : \n" );
    pLhPtr =	pLhHead->pNext;
    while ( pLhPtr != pLhHead )
    {
    	struct ListHead *pLhNext =	pLhPtr->pNext;
        struct DetectSearchArea *pDSArea =	GetDetectSearchAreaPointer( pLhPtr );

        DetectSearchAreaPrint( log, pDSArea );
        pLhPtr =	pLhNext;
    }
}			
#endif


static inline
BOOL DetectSearchAreaNeedsExpand( 
							const struct  DetectSearchArea *pDSArea
											)
{
	BOOL ret =	FALSE;
	assert( NULL != pDSArea );
    
    if ( DetectCycleSearchType == pDSArea->type )
    {
    	if ( DETECT_SEARCH_AREA_SLOW_TAP_POSITION == pDSArea->tapPosition )
        {
        	ret =	TRUE;
        }
    }
    
//    printlog( REC, "DetectSearchAreaNeedExpands ret = %d\n", ret );

    return ret;
}


/*
	被动搜索区域初始化函数
	pSearchArea -	待初始化的被动搜索区域指针
*/
static inline
void DetectSearchAreaInitialize( 
								struct DetectSearchArea *pSearchArea 
													)
{
	memset( pSearchArea, 0, sizeof( struct DetectSearchArea ) );
}
/*
	被动搜索区域拷贝函数
	pDesArea -	待拷贝的被动搜索区域指针
	pSrcArea -	被动搜索区域拷贝源数据指针
*/
static inline
void DetectSearchAreaCopy( 
					struct DetectSearchArea *pDesArea,
					const struct DetectSearchArea *pSrcArea 
						)
{
	assert( NULL != pDesArea && NULL != pSrcArea );
    memcpy( pDesArea, pSrcArea, sizeof( struct DetectSearchArea ) );
}

/*
	由被动循环搜索命令提取被动搜索区域结构
	--------------------------------------
	pSearchArea -	存放提取结果的被动搜索区域结构指针
	pCycleSearch -	被动循环搜索命令缓存指针
	type -	被动循环搜索类型 DETECT_SEARCH_TYPE_NORMAL、DETECT_SEARCH_TYPE_SLOW
*/
EXPORT
void DetectCycleSearchAreaGetByCmd( 
				struct DetectSearchArea *pSearchArea,
				const struct DetectCycleSearchCmd *pCycleSearch
								);
/*
	由被动重点目标搜索命令提取被动搜索区域结构
	pSearchArea -	待提取的被动搜索区域结构
	pTargetSearch -	被动重点目标搜索命令缓存指针
*/
EXPORT
void DetectTargetSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectTargetSearchCmd *pTargetSearch 
								);
/*
	由被动任务引导搜索命令提取被动搜索区域结构
	pSearchArea -	待提取的被动搜索区域结构
	pTargetSearch -	被动任务引导搜索命令缓存指针
*/
EXPORT
void DetectPilotSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectPilotSearchCmd *pPilotSearch 
								);



#endif

