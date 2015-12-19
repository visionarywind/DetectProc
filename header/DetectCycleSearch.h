
#ifndef __DETECT_CYCLE_SEARCH_HEADER__
#define __DETECT_CYCLE_SEARCH_HEADER__

/*
	被动循环搜索结构
*/
struct DetectCycleSearch
{
	struct ListHead	list;	//循环搜索链表

    struct DetectCycleSearchCache	*pSearchCache;	//命令缓冲
};

#ifdef __DEBUG__
static void __attribute__((unused))
DetectCycleSearchPrint(
			int log,
			struct DetectCycleSearch *pDCSearch
					)
{
    struct ListHead *pLhHead =	NULL;
    
	assert( NULL != pDCSearch );
    printlog( log, "DetectCycleSearchPrint\n" );

    pLhHead =	&pDCSearch->list;
    DetectSearchNodeListPrint( log, pLhHead );
}		
#endif

static inline
BOOL DetectCycleSearchEmpty(
			const struct DetectCycleSearch *pDCSearch
							)    
{
	assert( NULL != pDCSearch );
    return IsListHeadEmpty( &pDCSearch->list );
}							

/*
	被动循环搜索结构初始化函数
	--------------------------
*/
EXPORT
void DetectCycleSearchInitialize(
				struct DetectCycleSearch *pDCSearch,
				struct DetectCycleSearchCache	*pDCSCache
								);				

/*
	被动循环搜索结构结点清空函数
	----------------------------
	pCycleSearch -	待清空的被动循环搜索结构指针
*/
EXPORT
void DetectCycleSearchClear(
		struct DetectCycleSearch *pCycleSearch
							);


#define DETECT_SCHEDULE_STEP	( 2.0 )
/*
	被动循环搜索结构形成函数
*/
EXPORT
void DetectCycleSearchUpdate(
			    struct DetectCycleSearch *pCycleSearch
								);

static inline
BOOL DetectCycleSearchNeedsUpdate(
				const struct DetectCycleSearch *pCycleSearch,
				BOOL status
							)
{
	BOOL ret =	FALSE;
	struct DetectCycleSearchCache *pDCSCache =	NULL;
    
	assert( NULL != pCycleSearch );
    assert( NULL != pCycleSearch->pSearchCache );

	pDCSCache =	pCycleSearch->pSearchCache;
    ret =	DetectCycleSearchCacheStatusGet( pDCSCache );
    DetectCycleSearchCacheStatusSet( pDCSCache, status );

    return ret;
}

#endif

