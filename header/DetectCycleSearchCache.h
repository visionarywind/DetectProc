
#ifndef __DETECT_CYCLE_SEARCH_CACHE_HEADER__
#define __DETECT_CYCLE_SEARCH_CACHE_HEADER__

#define DETECT_SEARCH_CMD_CYCLE_MAX_NUM		( 6 )

/*
	被动循环搜索缓存结构
	-------------------------------------
	color数组用于对搜索的区间进行合法性判断
*/
struct DetectCycleSearchCache
{
	struct DetectSearchArea	area[DETECT_SEARCH_CMD_CYCLE_MAX_NUM];	//搜索区域缓存
    BOOL color[DETECT_SEARCH_CMD_CYCLE_MAX_NUM];    //缓存标记
    BOOL status;
};

EXPORT
BOOL DetectCycleSearchCacheEmpty(
			struct DetectCycleSearchCache *pDCSCache
								);	

static inline
void DetectCycleSearchCacheStatusSet( 
					struct DetectCycleSearchCache *pDCSCache,
					BOOL status
									)
{
	assert( NULL != pDCSCache );
	pDCSCache->status =	status;
}									
static inline
BOOL DetectCycleSearchCacheStatusGet(
					const struct DetectCycleSearchCache *pDCSCache
									)
{
	assert( NULL != pDCSCache );
    
	return pDCSCache->status;
}									

/*
	被动循环搜索缓存结构初始化函数
	pSearchCache -	待初始化的被动循环搜索缓存指针
*/
EXPORT
void DetectCycleSearchCacheInitialize( 
						struct DetectCycleSearchCache *pSearchCache 
									);

EXPORT
void DetectCycleSearchCacheClear(
					struct DetectCycleSearchCache *pDCSCache
								);

EXPORT
void DetectCycleSearchCacheReset(
					struct DetectCycleSearchCache *pDCSCache
								);

/*
	被动正常搜索命令处理函数
	------------------------
	pInstance -	被动命令缓存结构指针
	pBuf -	命令缓存指针
	size -	命令长度
*/
EXPORT
BOOL DetectCycleSearchCmdProc( 
			    struct DetectCycleSearchCache *pDCSCache,
				unsigned char *pBuf, 
				int size
							);



#endif

