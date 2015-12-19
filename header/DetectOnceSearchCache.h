
#ifndef __DETECT_ONCE_SEARCH_CACHE_HEADER__
#define __DETECT_ONCE_SEARCH_CACHE_HEADER__


#define DETECT_SEARCH_CMD_PILOT_MAX_NUM		( 20 )

/*
	重点目标搜索和任务引导搜索均为一次搜索命令，
	当数据被执行之后便不再保存
	-------------------------------------
	将重点目标搜索和任务引导搜索以链表的形式进行保存，
	操作直接对其进行选择，当执行完毕之后，将其从链表缓冲区中进行删除
	其中，链表的存储方式采用通用的链表格式，这样做的目的是省去数据转换的时间
	以及进行链表的内存静态管理
*/
struct DetectOnceSearchCache
{
	struct ListHead listHead;	//链表节点
    int calcs;	//计数
    BOOL status;
};

static inline
BOOL DetectOnceSearchCacheEmpty(
			const struct DetectOnceSearchCache *pDOSCache
								)
{
	assert( NULL != pDOSCache );
    return IsListHeadEmpty( &pDOSCache->listHead );
}						

EXPORT
struct DetectSearchArea *DetectOnceSearchCacheNodeGet(
				struct DetectOnceSearchCache	*pDOSCache
													);


static inline
void DetectOnceSearchCacheAdd(
			struct DetectOnceSearchCache *pDOSCache
							)    
{
	assert( NULL != pDOSCache );
	pDOSCache->calcs ++;
}						
static inline
void DetectOnceSearchCacheDel(
			struct DetectOnceSearchCache *pDOSCache
							)  
{
	assert( NULL != pDOSCache );
	pDOSCache->calcs --;
}							

static inline
void DetectOnceSearchCacheStatusSet(
				struct DetectOnceSearchCache *pDOSCache,
				BOOL status
									)
{
	assert( NULL != pDOSCache );
	pDOSCache->status =	status;
}		
static inline
BOOL DetectOnceSearchCacheStatusGet(
			const struct DetectOnceSearchCache *pDOSCache
							)    
{
	assert( NULL != pDOSCache );
	return pDOSCache->status;
}

/*
	向被动一次搜索缓存结构插入一个搜索区域
	--------------------------------------
	pDOSCache -	待插入的被动一次搜索缓存指针
	pDSArea -	被动搜索区域指针
*/
EXPORT
void DetectOnceSearchCacheInsert(
				struct DetectOnceSearchCache *pDOSCache,
				struct DetectSearchArea *pDSArea
								);

/*
	被动一次搜索缓存结构初始化函数
	------------------------------
	pSearchCache -	待初始化的被动一次搜索缓存指针
*/
EXPORT
void DetectOnceSearchCacheInitialize( 
					struct DetectOnceSearchCache *pSearchCache 
									);	

/*
	被动一次搜索缓存结构清空函数
	----------------------------
	pSearchCache -	待初始化的被动一次搜索缓存指针
*/
EXPORT
void DetectOnceSearchCacheClear( 
				struct DetectOnceSearchCache *pSearchCache 
								);

EXPORT
void DetectOnceSearchCacheReset(
				struct DetectOnceSearchCache *pSearchCache 
								);    

/*
	被动一次搜索命令处理函数
	------------------------
	pDOSCache -	被动命令缓存结构指针
	pBuf -	命令缓存指针
	size -	命令长度
*/
EXPORT
BOOL DetectPilotSearchCmdProc( 
			    struct DetectOnceSearchCache *pDOSCache,
				unsigned char *pBuf, 
				int size
							);


#endif

