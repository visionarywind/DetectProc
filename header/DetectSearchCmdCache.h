
#ifndef __DETECT_SEARCH_CMD_CACHE_HEADER__
#define __DETECT_SEARCH_CMD_CACHE_HEADER__



/*
	被动搜索命令状态枚举值定义
*/
enum DetectSearchCmdStatus
{
	DetectSearchCmdStatusNoNeedUpdate =	0,	//不需要更新状态
    DetectSearchCmdStatusNeedUpdate =	1,	//需要更新状态
    DetectSearchCmdStatusClear =	2,
};


////////////////////////////////////////////////////////////////////////////////
/*
	被动搜索命令缓存结构
*/
struct DetectSearchCmdCache
{	
    struct DetectCycleSearchCache	cycleSearch;	//循环搜索
    struct DetectOnceSearchCache	pilotSearch;	//重点目标搜索缓存
    struct DetectPeriodSearchCache	targetSearch;	//任务引导搜索缓存
	
    SEM_ID	sync;	//同步信号量
    BOOL	state;	//初始化标记
};

EXPORT
int DetectSearchCmdCacheEmpty(
		struct DetectSearchCmdCache *pDSCCache
								);	

/*
	被动命令缓存初始化函数
	pCmdCache -	待初始化的被动命令缓存指针
*/
EXPORT
void DetectSearchCmdCacheInitialize( 
					struct  DetectSearchCmdCache *pInstance
									);

/*
	被动命令缓存重置函数
	pCmdCache -	待重置的被动命令缓存指针
*/
EXPORT
void DetectSearchCmdCacheReset( 
					struct  DetectSearchCmdCache *pInstance
								);



////////////////////////////////////////////////////////////////////////////////

#endif

