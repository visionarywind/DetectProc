
#ifndef __DETECT_SEARCH_AREA_CACHE_BUFFER_HEADER__
#define __DETECT_SEARCH_AREA_CACHE_BUFFER_HEADER__


/*
	被动搜索区域缓存
*/
#define DETECT_SEARCH_AREA_CACHE_BUFFER_SIZE	( 100 )
struct DetectSearchAreaCacheBuffer
{
	struct DetectSearchArea *pBuffer;	//缓存指针
    int	usedSize;	//已使用个数
    int	maxSize;	//最大数目
    SEM_ID	sync;	//同步信号量
    struct ListHead freeList;	//空闲链表

    BOOL state;	//初始化标记
};

/*
	从被动搜索区域缓冲申请一个节点
*/
EXPORT 
struct ListHead *DetectSearchAreaCacheListHeadGet( void );

/*
	往被动搜索区域释放一个节点
*/
EXPORT
BOOL DetectSearchAreaCacheListHeadPut( struct ListHead *pNode );

/*
	从被动搜索区域申请一个节点
*/
EXPORT
struct DetectSearchArea *DetectSearchAreaCacheGet( void );

/*
	往被动搜索区域释放一个节点
	pSearchArea -	待释放的被动搜索区域节点链表指针
*/
EXPORT
BOOL DetectSearchAreaCachePut( struct DetectSearchArea *pSearchArea );

/*
	被动搜索区域链表清空函数
	pHead -	待清空的被动搜索区域链表头节点指针
*/
static inline
void DetectSearchAreaCacheListClear( struct ListHead *pHead )
{
    assert( NULL != pHead );

    printlog( REC, "DetectSearchAreaCacheListClear : head=%p\n", pHead );
	ListHeadClear( pHead, DetectSearchAreaCacheListHeadPut );
}

////////////////////////////////////////////////////////////////////////////////
/*
	被动搜索区域缓存功能初始化函数
*/
EXPORT
void DetectSearchAreaCacheBufferInit( void );

#endif

