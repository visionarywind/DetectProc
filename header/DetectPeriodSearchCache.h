
#ifndef __DETECT_PERIOD_SEARCH_CACHE_HEADER__
#define __DETECT_PERIOD_SEARCH_CACHE_HEADER__


#define DETECT_SEARCH_CMD_TARGET_MAX_NUM	( 20 )
/*
	被动周期搜索结构
	----------------
	该部分的处理与DetectScheduleTable部分的数据结构联合使用
	在DetectScheduleTable中，每个结点使用DetectSearchNode的指针引用DetectSearchArea
	从而，完成对DetectScheduleTable的更新

	然后再根据批号，在缓存中删除数据
*/
struct DetectPeriodSearchCache
{
	struct ListHead list;	//链表节点

    struct DetectScheduleTable	*pTable;
    
    int calcs;	//计数
    BOOL status;
};

static inline
BOOL DetectPeriodSearchCacheEmpty(
		struct DetectPeriodSearchCache *pDPSCache
								)
{
	assert( NULL != pDPSCache );

	return IsListHeadEmpty( &pDPSCache->list );
}								

/*
	被动周期搜索缓存结构初始化函数
	------------------------------
	pDPSCache -	待初始化的被动周期搜索缓存指针
*/
EXPORT
void DetectPeriodSearchCacheInitialize( 
					struct DetectPeriodSearchCache *pDPSCache 
									);	

EXPORT
void DetectPeriodSearchCacheReset(
					struct DetectPeriodSearchCache *pDPSCache
								);

static inline
void DetectPeriodSearchCacheAdc(
			struct DetectPeriodSearchCache *pDPSCache
							)    
{
	assert( NULL != pDPSCache );
	pDPSCache->calcs ++;
}						
static inline
void DetectPeriodSearchCacheDec(
			struct DetectPeriodSearchCache *pDPSCache
							)  
{
    assert( NULL != pDPSCache );
	pDPSCache->calcs --;
}							

/*
	被动周期搜索缓存结构清空函数
	----------------------------
	pDPSCache -	待初始化的被动周期搜索缓存指针
*/
EXPORT
void DetectPeriodSearchCacheClear( 
				struct DetectPeriodSearchCache *pDPSCache 
								);


/*
	向被动周期搜索缓存结构插入一个搜索区域
	--------------------------------------
	pDOSCache -	待插入的被动周期搜索缓存指针
	pDSArea -	被动搜索区域指针
*/
EXPORT
void DetectPeriodSearchCacheInsert(
				struct DetectPeriodSearchCache *pDPSCache,
				struct DetectSearchArea *pDSArea
								);

/*
	向被动周期搜索缓存结构删除特定批号的搜索区域
	--------------------------------------------
	pDOSCache -	待插入的被动周期搜索缓存指针
	taskNo -	待删除的特定批号
*/
EXPORT
BOOL DetectPeriodSearchCacheDelete(
				struct DetectPeriodSearchCache *pDPSCache,
				unsigned int	taskNo
								);

EXPORT
BOOL DetectTargetSearchCmdProc(
				struct DetectPeriodSearchCache *pDPSCache,
				struct DetectScheduleTable *pDSTable,
				unsigned char 	*pBuf,
				int size
								);

#endif

