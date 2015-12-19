
#ifndef __DETECT_SEARCH_AREA_CACHE_BUFFER_HEADER__
#define __DETECT_SEARCH_AREA_CACHE_BUFFER_HEADER__


/*
	�����������򻺴�
*/
#define DETECT_SEARCH_AREA_CACHE_BUFFER_SIZE	( 100 )
struct DetectSearchAreaCacheBuffer
{
	struct DetectSearchArea *pBuffer;	//����ָ��
    int	usedSize;	//��ʹ�ø���
    int	maxSize;	//�����Ŀ
    SEM_ID	sync;	//ͬ���ź���
    struct ListHead freeList;	//��������

    BOOL state;	//��ʼ�����
};

/*
	�ӱ����������򻺳�����һ���ڵ�
*/
EXPORT 
struct ListHead *DetectSearchAreaCacheListHeadGet( void );

/*
	���������������ͷ�һ���ڵ�
*/
EXPORT
BOOL DetectSearchAreaCacheListHeadPut( struct ListHead *pNode );

/*
	�ӱ���������������һ���ڵ�
*/
EXPORT
struct DetectSearchArea *DetectSearchAreaCacheGet( void );

/*
	���������������ͷ�һ���ڵ�
	pSearchArea -	���ͷŵı�����������ڵ�����ָ��
*/
EXPORT
BOOL DetectSearchAreaCachePut( struct DetectSearchArea *pSearchArea );

/*
	������������������պ���
	pHead -	����յı���������������ͷ�ڵ�ָ��
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
	�����������򻺴湦�ܳ�ʼ������
*/
EXPORT
void DetectSearchAreaCacheBufferInit( void );

#endif

