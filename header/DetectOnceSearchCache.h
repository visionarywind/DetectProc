
#ifndef __DETECT_ONCE_SEARCH_CACHE_HEADER__
#define __DETECT_ONCE_SEARCH_CACHE_HEADER__


#define DETECT_SEARCH_CMD_PILOT_MAX_NUM		( 20 )

/*
	�ص�Ŀ����������������������Ϊһ���������
	�����ݱ�ִ��֮��㲻�ٱ���
	-------------------------------------
	���ص�Ŀ�����������������������������ʽ���б��棬
	����ֱ�Ӷ������ѡ�񣬵�ִ�����֮�󣬽�������������н���ɾ��
	���У�����Ĵ洢��ʽ����ͨ�õ������ʽ����������Ŀ����ʡȥ����ת����ʱ��
	�Լ�����������ڴ澲̬����
*/
struct DetectOnceSearchCache
{
	struct ListHead listHead;	//����ڵ�
    int calcs;	//����
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
	�򱻶�һ����������ṹ����һ����������
	--------------------------------------
	pDOSCache -	������ı���һ����������ָ��
	pDSArea -	������������ָ��
*/
EXPORT
void DetectOnceSearchCacheInsert(
				struct DetectOnceSearchCache *pDOSCache,
				struct DetectSearchArea *pDSArea
								);

/*
	����һ����������ṹ��ʼ������
	------------------------------
	pSearchCache -	����ʼ���ı���һ����������ָ��
*/
EXPORT
void DetectOnceSearchCacheInitialize( 
					struct DetectOnceSearchCache *pSearchCache 
									);	

/*
	����һ����������ṹ��պ���
	----------------------------
	pSearchCache -	����ʼ���ı���һ����������ָ��
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
	����һ�������������
	------------------------
	pDOSCache -	���������ṹָ��
	pBuf -	�����ָ��
	size -	�����
*/
EXPORT
BOOL DetectPilotSearchCmdProc( 
			    struct DetectOnceSearchCache *pDOSCache,
				unsigned char *pBuf, 
				int size
							);


#endif

