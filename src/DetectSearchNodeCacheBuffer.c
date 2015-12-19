
#include "..\Header.h"


/*
	���������ڵ㻺��ʵ��
*/
static struct DetectSearchNodeCacheBuffer gDetectSearchNodeCacheBufferInstance =
		{
			.pBuffer =	NULL,
            .usedSize =	0,
            .maxSize =	DETECT_SEARCH_NODE_CACHE_BUFFER_SIZE,
            .sync =	NULL,
            .freeList =	{ 0 },

            .state =	FALSE,
		};
/*
	��ʼ�����������ڵ㻺��ṹ
	pCacheBuffer -	����ʼ���ı���������㻺��ṹָ��
*/
static void DetectSearchNodeCacheBufferInitialize( 
								struct DetectSearchNodeCacheBuffer *pInstance,
								int size
									    		)
{
	int i =	0;
	int mallocSize =	size*sizeof( struct DetectSearchNode );

    assert( NULL != pInstance );

    pInstance->usedSize =	0;
    pInstance->maxSize =	size;
    pInstance->sync =	semBCreate(SEM_Q_FIFO, SEM_FULL);
    pInstance->pBuffer =	(struct DetectSearchNode *)malloc(mallocSize);

	//���½������������ӹ�ϵ
    //����ʼ�ڵ�
    pInstance->pBuffer[0].listHead.pPrev =	NULL;
    pInstance->pBuffer[0].listHead.pNext =   
        				&(pInstance->pBuffer[1].listHead);

    for ( i=1; i<size-1; ++i )
    {
    	pInstance->pBuffer[i].listHead.pPrev =
            			&(pInstance->pBuffer[i-1].listHead);
        pInstance->pBuffer[i].listHead.pNext =
            			&(pInstance->pBuffer[i+1].listHead);
    }
    //�������һ���ڵ�
    pInstance->pBuffer[size-1].listHead.pPrev =
    					&(pInstance->pBuffer[size-2].listHead);
    pInstance->pBuffer[size-1].listHead.pNext =	
        				&(pInstance->pBuffer[0].listHead);
	//���¿�������
	ListHeadInitialize( &pInstance->freeList );
    ListHeadRearInsert( &pInstance->pBuffer[0].listHead, &pInstance->freeList );

    pInstance->state =	TRUE;
}

/*
	��ȡ���������ڵ㻺��ṹָ��
*/
static struct DetectSearchNodeCacheBuffer *
								DetectSearchNodeCacheBufferInstanceGet( void )
{
	return &gDetectSearchNodeCacheBufferInstance;
}
/*
	����������㻺�����+1
*/
static void DetectSearchNodeCacheBufferAdc( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
						        		DetectSearchNodeCacheBufferInstanceGet();
    
	if ( pInstance->usedSize > pInstance->maxSize )
	{
		printfSafe("Error - DetectSearchNodeCacheBufferAdc : usedSize > maxSize!\n");
	}
    pInstance->usedSize ++;
    printlog( REC, "DetectSearchNodeCacheBufferAdd usedSize=%d\n",
        		pInstance->usedSize );
}
/*
	����������㻺�����-1
*/
static void DetectSearchNodeCacheBufferDec( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
    									DetectSearchNodeCacheBufferInstanceGet();
	if ( pInstance->usedSize < 0 )
    {
    	printfSafe("Error - DetectSearchNodeCacheBufferDec : usedSize < 0!\n");
    }
    pInstance->usedSize --;
    printlog( REC, "DetectSearchNodeCacheBufferDec usedSize=%d\n", 
        		pInstance->usedSize );
}
/*
	����������㻺������Ԥ��
*/
static BOOL DetectSearchNodeCacheBufferPrefetch( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
    									DetectSearchNodeCacheBufferInstanceGet();
	if ( pInstance->usedSize < 0 )
    {
    	printfSafe("Error - DetectSearchNodeCcheBufferPrefetch : usedSize < 0 !\n");
    }
	return pInstance->usedSize < pInstance->maxSize;
}


////////////////////////////////////////////////////////////////////////////////


/*
	�ӱ���������㻺��Ԥ�������ɽ�㺯��
*/
STATUS DetectSearchNodeCachePrefetch( int size )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
        							DetectSearchNodeCacheBufferInstanceGet();
    STATUS status =	semTake( pInstance->sync, NO_WAIT );
    if ( OK == status )
    {
    	int remainder =	pInstance->maxSize - pInstance->usedSize;
        if ( size < remainder )
		{
			status =	ERROR;
		}
        semGive( pInstance->sync );
    }
    return status;
}


/*
	�ӱ��������ڵ㻺��������һ���ڵ�
*/
struct ListHead *DetectSearchNodeCacheListHeadGet( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =	
        								DetectSearchNodeCacheBufferInstanceGet();
	STATUS status =	semTake( pInstance->sync, NO_WAIT );
	struct ListHead *pLhRet =	NULL;
    
    if ( OK == status && DetectSearchNodeCacheBufferPrefetch() )
    {
    	struct ListHead *pRearNode =	ListHeadRearGet( &pInstance->freeList );
        ListHeadDelete( pRearNode->pPrev, pRearNode->pNext );

        DetectSearchNodeCacheBufferAdc();
    	semGive( pInstance->sync );
        
        pLhRet =	pRearNode;
    }
    printlog( REC, "DetectSearchNodeCacheListHeadGet : pNode=%p\n", 
        				pLhRet );

    if ( NULL == pLhRet )
    {
    	printlog( SCR, "DetectSearchNodeCacheListHeadGet failed!\n" );
    }
    
    return pLhRet;	
}

/*
	�򱻶������ڵ㻺�����ͷ�һ���ڵ�
*/
BOOL DetectSearchNodeCachePut( 
				struct DetectSearchNode *pSearchNode
							)
{
	struct ListHead *pListHead =	NULL;

    assert( NULL != pSearchNode );

	pListHead =	&pSearchNode->listHead;
    return DetectSearchNodeCacheListHeadPut(pListHead);
}


/*
	�ӱ��������ڵ㻺��������һ���ڵ�
*/
struct DetectSearchNode *DetectSearchNodeCacheGet( void )
{
	struct ListHead *pListHead =	DetectSearchNodeCacheListHeadGet();
    struct DetectSearchNode *pDSNode =	NULL;
    if ( NULL != pListHead )
    {
    	pDSNode =	GetDetectSearchNodePointer(pListHead);
    }

    return pDSNode;
}


/*
	�򱻶������ڵ㻺�����ͷ�һ���ڵ�
	pNode -	���ͷŵı��������������ָ��
*/
BOOL DetectSearchNodeCacheListHeadPut( struct ListHead *pNode )
{
	struct DetectSearchNodeCacheBuffer *pInstance =	
        								DetectSearchNodeCacheBufferInstanceGet();
    STATUS status =	semTake( pInstance->sync, NO_WAIT );
    BOOL ret =	FALSE;
    
    if ( OK == status )
    {
        ListHeadTailInsert( &pInstance->freeList, pNode );

        DetectSearchNodeCacheBufferDec();
    	semGive( pInstance->sync );
        ret =	TRUE;
    }

    printlog( REC, "DetectSearchNodeCacheListHeadPut : pNode=%p, ret=%d\n",
        		pNode, ret );
    
    return ret;
}

////////////////////////////////////////////////////////////////////////////////
/*
	����������㻺�湦�ܳ�ʼ������
*/
void DetectSearchNodeCacheBufferInit( void )
{
	struct DetectSearchNodeCacheBuffer *pInstance =
        				DetectSearchNodeCacheBufferInstanceGet();
    int size =	DETECT_SEARCH_NODE_CACHE_BUFFER_SIZE;
    DetectSearchNodeCacheBufferInitialize( pInstance, size );
}


