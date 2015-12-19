
#include "..\Header.h"

/*
	����ѭ�������ṹ��ʼ������
	--------------------------
*/
void DetectCycleSearchInitialize(
				struct DetectCycleSearch *pDCSearch,
				struct DetectCycleSearchCache	*pDCSCache
								)
{
	assert( NULL != pDCSearch );
    ListHeadInitialize( &pDCSearch->list );
    pDCSearch->pSearchCache =	pDCSCache;
}    										


/*
	����ѭ�������ṹ�����պ���
	----------------------------
	pCycleSearch -	����յı���ѭ�������ṹָ��
*/
void DetectCycleSearchClear(
			struct DetectCycleSearch *pCycleSearch
							)
{
	assert( NULL != pCycleSearch );    
    DetectSearchNodeCacheListClear( &pCycleSearch->list );
}


/*
	����ѭ�������ṹ�����뺯��
	----------------------------
	pCycleSearch -	������ı���ѭ�������ṹָ��
	pNodeArea -	������ı�����������ָ��
*/
static void __DetectCycleSearchAreaInsert(
					struct DetectCycleSearch *pCycleSearch,
					struct DetectSearchArea *pDSArea
							)
{

	struct ListHead *pHead =	NULL;
	struct ListHead *pNode =	NULL;

	assert( NULL != pCycleSearch );
    assert( NULL != pDSArea );
    printlog( REC, "__DetectCycleSearchAreaInsert\n" );
    
    pHead =	&pCycleSearch->list;
	pNode =	&pDSArea->listHead;
    
	if ( !IsListHeadEmpty( pHead ) )
    {
    	struct ListHead *pListNode =	ListHeadRearGet( pHead );
        
        while ( pListNode != pHead )
        {
			struct DetectSearchArea *pListNodeArea =	
                						GetDetectSearchAreaPointer( pListNode );
            //�����ǰ�ڵ����ʼֵ��С�ڴ�����Ľڵ���ʼֵ��
            //�򽫽ڵ���뵽��ǰ�ڵ�֮ǰ
        	if ( FLOAT_COMPARE_SMALL != 
                	FloatCompare( pListNodeArea->startAngle, pDSArea->startAngle ) )
	    	{
                ListHeadInsert( pListNode->pPrev, pListNode, pNode );
                return ;
	    	}
			
            pListNode =	pListNode->pNext;
        }

        ListHeadInsert( pListNode->pPrev, pListNode, pNode );
    }
    else
    {
    	ListHeadRearInsert( pHead, pNode );
    }
}    									


/*
	����ѭ�������ṹ��ȡ����
	------------------------
	pCycleSearch -	����ȡ���ݵı���ѭ�������ṹָ��
	pSearchCache -	����ѭ�������ṹ��ȡ��ѭ����������Դ
*/
static void __DetectCycleSearchExtract(
					struct DetectCycleSearch *pCycleSearch
										)
{
    int i = 0;
    int size =  DETECT_SEARCH_CMD_CYCLE_MAX_NUM;
    struct DetectCycleSearchCache *pSearchCache =	pCycleSearch->pSearchCache;

    for ( i=0; i<size; ++i )
    {
    	BOOL color =	pSearchCache->color[i];
        if ( color )   //������Ч
        {
            struct DetectSearchArea *pNewArea =	DetectSearchAreaCacheGet();
            if ( pNewArea )
            {
                //���½ڵ�ֵ
                DetectSearchAreaCopy( pNewArea, pSearchCache->area+i );

                printlog( REC, "__DetectCycleSearchExtract : angle=%f\n",
                    				pNewArea->startAngle );

				__DetectCycleSearchAreaInsert( pCycleSearch, pNewArea );
            }
            else
            {
                printfSafe("Exception %s, %d -  DetectSearchArea is out of use\n",
                   				__FILE__, __LINE__);
                break;
            }
        }
    }

}
							


/*
	����ѭ�������ṹ����γɺ���
*/
static void __DetectCycleSearchNodeExplain(
							struct DetectCycleSearch *pCycleSearch,
							float step
											)
{
    struct ListHead *pCycleHead =	&pCycleSearch->list;

    if ( !IsListHeadEmpty( pCycleHead ) )
    {
        struct ListHead *pCycleNode =    pCycleHead->pNext;
        struct ListHead tempList;
        ListHeadInitialize( &tempList );
        
        while ( pCycleNode != pCycleHead )
        {
        	struct ListHead *pLhNext =	pCycleNode->pNext;
            struct DetectSearchArea *pSearchArea =  
                                        GetDetectSearchAreaPointer( pCycleNode );

            DetectSearchNodeExplain( &tempList, pSearchArea, DETECT_SCHEDULE_STEP );
            ListHeadDelete( pCycleNode->pPrev, pCycleNode->pNext );
            DetectSearchAreaCachePut( pSearchArea );
            pCycleNode = pLhNext;
        }

		DetectSearchAreaCacheListClear( pCycleHead );
        
        ListHeadMove( pCycleHead, &tempList );
    }
}											

/*
	����ѭ�������ṹ�γɺ���
*/
void DetectCycleSearchUpdate( 
				struct DetectCycleSearch *pCycleSearch
								)
{
	assert( NULL != pCycleSearch );

 //   printlog( REC, "DetectCycleSearchUpdate\n" );

	if ( !DetectCycleSearchEmpty( pCycleSearch ) )
    {
    	DetectCycleSearchClear( pCycleSearch );
        printfSafe( "Exception DetectCycleSearchUpdate\n" );
    }

    //��ȡѭ����������
    __DetectCycleSearchExtract( pCycleSearch );
    //�γ��������
    __DetectCycleSearchNodeExplain( pCycleSearch, DETECT_SCHEDULE_STEP );


//    DetectCycleSearchPrint( REC, pCycleSearch );
}


