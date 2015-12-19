
#ifndef __DETECT_ONCE_SEARCH_HEADER__
#define __DETECT_ONCE_SEARCH_HEADER__

/*
	����һ�������ṹ
	----------------
	ÿ��һ�Σ��򽫵�ǰ������е�һ��������������������ṹ��
*/
struct DetectOnceSearch
{
    struct ListHead list;

	struct DetectOnceSearchCache	*pSearchCache;	//�����
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
	����һ�������ṹ��ʼ������
	------------------------------
*/
EXPORT
void DetectOnceSearchInitialize( 
					struct DetectOnceSearch *pOnceSearch,
					struct DetectOnceSearchCache *pSearchCache
								);
/*
	����һ�������ṹ�����������
	----------------------------
	pOnceSearch -	������ı���һ�������ṹָ��
*/
EXPORT
void DetectOnceSearchClear( 
				struct DetectOnceSearch *pOnceSearch
							);		


/*
	����һ�������ṹ ����γɺ���
	-----------------------------
*/
EXPORT
void DetectOnceSearchUpdate(
			struct DetectOnceSearch *pDOSearch
							);


/*
	����һ�������ṹ ������������ڵ� ��ȡ����
*/
EXPORT
struct DetectSearchNode * DetectOnceSearchNodeGet( 
								struct DetectOnceSearch *pDOSearch
												);


#endif

