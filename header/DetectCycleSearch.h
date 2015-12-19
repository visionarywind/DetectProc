
#ifndef __DETECT_CYCLE_SEARCH_HEADER__
#define __DETECT_CYCLE_SEARCH_HEADER__

/*
	����ѭ�������ṹ
*/
struct DetectCycleSearch
{
	struct ListHead	list;	//ѭ����������

    struct DetectCycleSearchCache	*pSearchCache;	//�����
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
	����ѭ�������ṹ��ʼ������
	--------------------------
*/
EXPORT
void DetectCycleSearchInitialize(
				struct DetectCycleSearch *pDCSearch,
				struct DetectCycleSearchCache	*pDCSCache
								);				

/*
	����ѭ�������ṹ�����պ���
	----------------------------
	pCycleSearch -	����յı���ѭ�������ṹָ��
*/
EXPORT
void DetectCycleSearchClear(
		struct DetectCycleSearch *pCycleSearch
							);


#define DETECT_SCHEDULE_STEP	( 2.0 )
/*
	����ѭ�������ṹ�γɺ���
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

