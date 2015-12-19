
/*
    文件说明:   被动分区状态反馈头文件
    文件作者:   jsf
    文件时间:   2014.3.12
*/

#ifndef __101_DETECT_SEARCH_FEED_BACK_HEADER__
#define __101_DETECT_SEARCH_FEED_BACK_HEADER__



#define DECTION_WORK_STATE_INFO_FLAG    0xbb01  /* 分机回送标识，bb01为宽带接收的信息头 */

////////////////////////////////////////////////////////////////////////////////
/*
	被动命令反馈部分结构定义
*/

enum DetectSearchFeedBackStatus
{
	DetectSearchFeedBackStatusNoNeedUpdate =	0,
    DetectSearchFeedBackStatusNeedUpdate =	1
};

/*
	被动循环搜索反馈结点结构
*/
struct DetectCycleSearchFeedBackNode
{
	unsigned short	startAngle;	//开始角度
    unsigned short	endAngle;	//结束角度
    unsigned short	stayPeriod;	//驻留时间
};
/*
	被动循环搜索反馈结构
*/
struct DetectCycleSearchFeedBack
{
	//循环搜索反馈结点缓存
	struct DetectCycleSearchFeedBackNode nodes[DETECT_SEARCH_CMD_CYCLE_MAX_NUM];
    BOOL color[ DETECT_SEARCH_CMD_CYCLE_MAX_NUM ];	//反馈结点缓存标记
    enum DetectSearchFeedBackStatus status;
};

static inline
void DetectCycleSearchFeedBackStatusSet(
						struct DetectCycleSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
	pFeedBack->status =	status;
}										

static inline
enum DetectSearchFeedBackStatus DetectCycleSearchFeedBackStatusGet(
						const struct DetectCycleSearchFeedBack *pFeedBack
										)
{
	return pFeedBack->status;
}										


static inline
void DetectCycleSearchFeedBackInitialize( 
						struct DetectCycleSearchFeedBack *pFeedBack
										)
{
	int size =	0;
	assert( NULL != pFeedBack );
    size =	sizeof( struct DetectCycleSearchFeedBackNode )*DETECT_SEARCH_CMD_CYCLE_MAX_NUM;
    memset( pFeedBack->nodes, 0, size );
    size =	sizeof( BOOL )*DETECT_SEARCH_CMD_CYCLE_MAX_NUM;
    memset( pFeedBack->color, 0, size );
    pFeedBack->status =	DetectSearchFeedBackStatusNoNeedUpdate;
}										

/*
	被动重点目标搜索反馈结点结构
*/
struct DetectTargetSearchFeedBackNode
{
	unsigned short	angle;		//角度
	unsigned short	stayPeriod;	//驻留时间
};
/*
	被动重点目标搜索反馈结构
*/
struct DetectTargetSearchFeedBack
{
	//重点目标搜索反馈结点缓存
	struct DetectTargetSearchFeedBackNode nodes[DETECT_SEARCH_CMD_TARGET_MAX_NUM];
    BOOL color[DETECT_SEARCH_CMD_TARGET_MAX_NUM];	//反馈结点缓存标记
    enum DetectSearchFeedBackStatus status;
};

static inline
void DetectTargetSearchFeedBackStatusSet(
						struct DetectTargetSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
	pFeedBack->status =	status;
}										

static inline
enum DetectSearchFeedBackStatus DetectTargetSearchFeedBackStatusGet(
						const struct DetectTargetSearchFeedBack *pFeedBack
										)
{
	return pFeedBack->status;
}		


static inline
void DetectTargetSearchFeedBackInitialize( 
						struct DetectTargetSearchFeedBack *pFeedBack
											)
{
	int size =	0;
    assert( NULL != pFeedBack );
    size =	sizeof( struct DetectTargetSearchFeedBackNode )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->nodes, 0, size );
    size =	sizeof( BOOL )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->color, 0, size );
    pFeedBack->status =	DetectSearchFeedBackStatusNoNeedUpdate;
}											

/*
	被动任务引导搜索反馈结点结构
*/
struct DetectPilotSearchFeedBackNode
{
	unsigned short	startAngle;	//开始角度
	unsigned short	endAngle;	//结束角度
	unsigned short	stayPeriod;	//驻留时间
};
/*
	被动任务引导搜索反馈结构
*/
struct DetectPilotSearchFeedBack
{
	//任务引导搜索反馈结点缓存
	struct DetectPilotSearchFeedBackNode nodes[DETECT_SEARCH_CMD_PILOT_MAX_NUM];
    BOOL color[DETECT_SEARCH_CMD_PILOT_MAX_NUM];	//反馈结点缓存标记
    enum DetectSearchFeedBackStatus	status;
};

static inline
void DetectPilotSearchFeedBackStatusSet(
						struct DetectPilotSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
	pFeedBack->status =	status;
}										

static inline
enum DetectSearchFeedBackStatus DetectPilotSearchFeedBackStatusGet(
						const struct DetectPilotSearchFeedBack *pFeedBack
										)
{
	return pFeedBack->status;
}		

static inline
void DetectPilotSearchFeedBackInitialize( 
						struct DetectPilotSearchFeedBack *pFeedBack
											)
{
	int size =	0;
    assert( NULL != pFeedBack );
    size =	sizeof( struct DetectPilotSearchFeedBackNode )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->nodes, 0, size );
    size =	sizeof( BOOL )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->color, 0, size );
    pFeedBack->status =	DetectSearchFeedBackStatusNoNeedUpdate;
}											


/*
	被动搜索命令反馈缓存结构
*/
struct DetectSearchFeedBack
{
	struct DetectCycleSearchFeedBack	cycleSearch;	//被动循环搜索反馈结构

    struct DetectTargetSearchFeedBack	targetSearch;	//被动重点目标搜索反馈结构
    struct DetectPilotSearchFeedBack	pilotSearch;	//被动任务引导搜索反馈结构

    SEM_ID	sync;	//同步信号量 
    BOOL	state;	//初始化标记
};

static inline
void DetectSearchFeedBackPilotStatusSet(
						struct DetectSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
    DetectPilotSearchFeedBackStatusSet( &pFeedBack->pilotSearch, status	);
}

/*
	被动搜索命令反馈缓存结构初始化函数
	pFeedBack -	待反馈的被动搜索命令反馈缓存结构指针
*/
static inline
void DetectSearchFeedBackInitialize( 
					struct DetectSearchFeedBack *pInstance
									)
{
	assert( NULL != pInstance );
    DetectCycleSearchFeedBackInitialize( &pInstance->cycleSearch );
    DetectTargetSearchFeedBackInitialize( &pInstance->targetSearch );
    DetectPilotSearchFeedBackInitialize( &pInstance->pilotSearch );
    
    pInstance->sync =	semBCreate( SEM_Q_FIFO, SEM_FULL );
    pInstance->state =	TRUE;
}

EXPORT
void DetectSearchFeedBackReset(
					struct DetectSearchFeedBack *pInstance
								);

/*
	被动搜索命令反馈处理函数	
*/
EXPORT
void DetectSearchCmdFeedBackProc( void );

#endif

