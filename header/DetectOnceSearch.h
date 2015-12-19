
#ifndef __DETECT_ONCE_SEARCH_HEADER__
#define __DETECT_ONCE_SEARCH_HEADER__

/*
	被动一次搜索结构
	----------------
	每调一次，则将当前命令缓冲中的一个被动命令解析到搜索结构中
*/
struct DetectOnceSearch
{
    struct ListHead list;

	struct DetectOnceSearchCache	*pSearchCache;	//命令缓冲
};

static inline
BOOL DetectOnceSearchEmpty(
				const struct DetectOnceSearch *pDOSearch
							)
{
	assert( NULL != pDOSearch );
    return IsListHeadEmpty( &pDOSearch->list );
}							

static inline
BOOL DetectOnceSearchNeedsUpdate(
				const struct DetectOnceSearch *pDOSearch
									)    
{
	assert( NULL != pDOSearch );
    
    return DetectOnceSearchEmpty( pDOSearch )
        	&& !DetectOnceSearchCacheEmpty( pDOSearch->pSearchCache );
}

/*
	被动一次搜索结构初始化函数
	------------------------------
*/
EXPORT
void DetectOnceSearchInitialize( 
					struct DetectOnceSearch *pOnceSearch,
					struct DetectOnceSearchCache *pSearchCache
								);
/*
	被动一次搜索结构数据清除函数
	----------------------------
	pOnceSearch -	待清除的被动一次搜索结构指针
*/
EXPORT
void DetectOnceSearchClear( 
				struct DetectOnceSearch *pOnceSearch
							);		


/*
	被动一次搜索结构 结点形成函数
	-----------------------------
*/
EXPORT
void DetectOnceSearchUpdate(
			struct DetectOnceSearch *pDOSearch
							);


/*
	被动一次搜索结构 被动搜索区间节点 获取函数
*/
EXPORT
struct DetectSearchNode * DetectOnceSearchNodeGet( 
								struct DetectOnceSearch *pDOSearch
												);


#endif

