
/*
	��������ṹͷ�ļ�
*/
#ifndef __DETECT_SEARCH_NODE_CACHE_BUFFER_HEADER__
#define __DETECT_SEARCH_NODE_CACHE_BUFFER_HEADER__


/*
	���������ڵ㻺��
*/
//�ú�Ĵ�С������Ҫ���е������ƣ�����ռ�õ��ڴ����
#define DETECT_SEARCH_NODE_CACHE_BUFFER_SIZE	( 360*6+400+50 )
struct DetectSearchNodeCacheBuffer
{
	struct DetectSearchNode *pBuffer;	//����ָ��
    int	usedSize;	//��ʹ�ø���
    int	maxSize;	//�����Ŀ
    SEM_ID	sync;	//ͬ���ź���
    struct ListHead freeList;	//��������

    BOOL state;	//��ʼ�����
};

/*
	�ӱ���������㻺��Ԥ�������ɽ�㺯��
*/
EXPORT
BOOL DetectSearchNodeCachePrefetch( int size );

/*
	�ӱ��������ڵ㻺������һ���ڵ�
*/
EXPORT
struct ListHead *DetectSearchNodeCacheListHeadGet( void );

/*
	�����������ڵ㻺���ͷ�һ���ڵ�
*/
EXPORT
BOOL DetectSearchNodeCacheListHeadPut( struct ListHead *pNode );

////////////////////////////////////////////////////////////////////////////////


/*
	�ӱ��������ڵ㻺��������һ���ڵ�
*/
EXPORT
struct DetectSearchNode *DetectSearchNodeCacheGet( void );

/*
	�ӱ���������㻺��������һ����
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
	�򱻶������ڵ㻺�����ͷ�һ���ڵ�
*/
EXPORT
BOOL DetectSearchNodeCachePut( 
				struct DetectSearchNode *pSearchNode
							);

/*
	���������ڵ�������պ���
	pHead -	����յı��������ڵ�����ͷ�ڵ�ָ��
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
	����������㻺�湦�ܳ�ʼ������
*/
EXPORT
void DetectSearchNodeCacheBufferInit( void );



#endif

