
/*
	被动缓存结构头文件
*/
#ifndef __DETECT_SEARCH_NODE_CACHE_BUFFER_HEADER__
#define __DETECT_SEARCH_NODE_CACHE_BUFFER_HEADER__


/*
	被动搜索节点缓存
*/
//该宏的大小可能需要进行调整控制，否则占用的内存过多
#define DETECT_SEARCH_NODE_CACHE_BUFFER_SIZE	( 360*6+400+50 )
struct DetectSearchNodeCacheBuffer
{
	struct DetectSearchNode *pBuffer;	//缓存指针
    int	usedSize;	//已使用个数
    int	maxSize;	//最大数目
    SEM_ID	sync;	//同步信号量
    struct ListHead freeList;	//空闲链表

    BOOL state;	//初始化标记
};

/*
	从被动搜索结点缓存预申请若干结点函数
*/
EXPORT
BOOL DetectSearchNodeCachePrefetch( int size );

/*
	从被动搜索节点缓冲申请一个节点
*/
EXPORT
struct ListHead *DetectSearchNodeCacheListHeadGet( void );

/*
	往被动搜索节点缓冲释放一个节点
*/
EXPORT
BOOL DetectSearchNodeCacheListHeadPut( struct ListHead *pNode );

////////////////////////////////////////////////////////////////////////////////


/*
	从被动搜索节点缓存中申请一个节点
*/
EXPORT
struct DetectSearchNode *DetectSearchNodeCacheGet( void );

/*
	从被动搜索结点缓存中申请一组结点
*/
static inline
BOOL DetectSearchNodeCacheArrInit( 
					struct DetectSearchNode *arr[],
					int size
								)
{
	BOOL ret =	FALSE;
    assert( NULL != arr );
    ret =	DetectSearchNodeCachePrefetch( size );
    if ( ret )
    {
    	int i =	0;
        for ( i=0; i<size; ++i )
        {
        	arr[i] =	DetectSearchNodeCacheGet();
        }
    }
    
    return ret;
}											

/*
	向被动搜索节点缓存中释放一个节点
*/
EXPORT
BOOL DetectSearchNodeCachePut( 
				struct DetectSearchNode *pSearchNode
							);

/*
	被动搜索节点链表清空函数
	pHead -	待清空的被动搜索节点链表头节点指针
*/
static inline
void DetectSearchNodeCacheListClear( struct ListHead *pHead )
{
    assert( NULL != pHead );
    
    printlog( REC, "DetectSearchNodeCacheListClear\n" );
    ListHeadClear( pHead, DetectSearchNodeCacheListHeadPut );
}


////////////////////////////////////////////////////////////////////////////////
/*
	被动搜索结点缓存功能初始化函数
*/
EXPORT
void DetectSearchNodeCacheBufferInit( void );



#endif

