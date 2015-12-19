
#include "..\Header.h"

/*
	����һ�������ṹ��ʼ������
	------------------------------
*/
void DetectOnceSearchInitialize( 
					struct DetectOnceSearch *pOnceSearch,
					struct DetectOnceSearchCache *pSearchCache
								)
{
	assert( NULL != pOnceSearch );
    ListHeadInitialize( &pOnceSearch->list );
    pOnceSearch->pSearchCache =	pSearchCache;
}
/*
	����һ�������ṹ�����������
	----------------------------
	pOnceSearch -	������ı���һ�������ṹָ��
*/
void DetectOnceSearchClear( 
				struct DetectOnceSearch *pOnceSearch
							)
{
	assert( NULL != pOnceSearch );
    DetectSearchNodeCacheListClear( &pOnceSearch->list );
}		


/*
	����һ�������ṹ������������γɺ���
*/
static BOOL __DetectOnceSearchExplain(
							struct ListHead *pPilotList,
							const struct DetectSearchArea *pSearchArea
										)
{
#define	DETECT_ONCE_SEARCH_PILOT_EXPLAIN_NUMBER	( 5 )
	struct DetectSearchNode * pArr[DETECT_ONCE_SEARCH_PILOT_EXPLAIN_NUMBER];
	int size =	DETECT_ONCE_SEARCH_PILOT_EXPLAIN_NUMBER;
    BOOL ret =	DetectSearchNodeCacheArrInit( pArr, size );

    printlog( REC, "__DetectOnceSearchExplain\n" );

    if ( ret )
    {
    	int i =	0;
        for ( i=0; i<size; ++i )
        {
        	float step =	-4.0+DETECT_SCHEDULE_STEP*i;
            struct DetectSearchNode *pDSNode =	pArr[i];
			DetectSearchNodeExtract( pDSNode, pSearchArea, step );
        }
        for ( i=0; i<size; ++i )
        {
        	struct ListHead *pNode =	&pArr[i]->listHead;
        	ListHeadTailInsert( pPilotList, pNode );
        }
    }

    return ret;
}																	


/*
	����һ�������ṹ ����γɺ���	
	-----------------------------
	Note :  ��������һ���γ�һ�����������Ϣ��
			��������Ӧ��������״̬��־
*/
void DetectOnceSearchUpdate(
			struct DetectOnceSearch *pDOSearch
							)
{
	struct ListHead *pSearchHead =	NULL;
    struct DetectSearchArea *pDSArea =	NULL;
    
	assert( NULL != pDOSearch );
    printlog( REC, "DetectOnceSearchUpdate\n" );

	pSearchHead =	&pDOSearch->list;
	pDSArea =	DetectOnceSearchCacheNodeGet( pDOSearch->pSearchCache );

    if ( NULL != pDSArea )
    {
    	__DetectOnceSearchExplain( pSearchHead, pDSArea );
        DetectSearchAreaCachePut( pDSArea );
    }
	else
    {
    	printfSafe( "DetectOnceSearchUpdate failed\n" );
    }
}									


/*
	����һ�������ṹ ������������ڵ� ��ȡ����
*/
struct DetectSearchNode * DetectOnceSearchNodeGet( 
							struct DetectOnceSearch *pDOSearch
													)
{
	struct ListHead *pHead =    NULL;
    assert( NULL != pDOSearch );
	pHead =	&pDOSearch->list;
    if ( !IsListHeadEmpty( pHead ) )
    {
    	struct ListHead *pLHPtr =	ListHeadRearGet( pHead );
    	struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLHPtr ); 
    	ListHeadDelete( pLHPtr->pPrev, pLHPtr->pNext );
        
        return pDSNode;
    }
    return NULL;
}													


