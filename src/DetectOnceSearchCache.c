
#include "..\Header.h"


/*
	����һ����������ṹ��ʼ������
	------------------------------
	pSearchCache -	����ʼ���ı���һ����������ָ��
*/
void DetectOnceSearchCacheInitialize( 
					struct DetectOnceSearchCache *pDOSCache 
									)
{
	ListHeadInitialize( &pDOSCache->listHead );
    pDOSCache->calcs =	0;
    DetectOnceSearchCacheStatusSet( pDOSCache, FALSE );
}

void DetectOnceSearchCacheReset(
				struct DetectOnceSearchCache *pSearchCache 
								)
{
	assert( NULL != pSearchCache );
    printlog( REC, "DetectOnceSearchCacheReset : pSearchCache=%p\n", pSearchCache );

    DetectOnceSearchCacheClear( pSearchCache );
}



/*
	����һ����������ṹ��պ���
	----------------------------
	pSearchCache -	����ʼ���ı���һ����������ָ��
*/
void DetectOnceSearchCacheClear( 
				struct DetectOnceSearchCache *pDOSCache 
								)
{
	assert( NULL != pDOSCache );
	printlog( REC, "DetectOnceSearchCacheClear : pDOSCache=%p\n", pDOSCache );
    
    DetectSearchAreaCacheListClear( &pDOSCache->listHead );
    pDOSCache->calcs =	0;
    DetectOnceSearchCacheStatusSet( pDOSCache, TRUE );
}

/*
	�򱻶�һ����������ṹ����һ����������
	--------------------------------------
	pDOSCache -	������ı���һ����������ָ��
	pDSArea -	������������ָ��
*/
void DetectOnceSearchCacheInsert(
				struct DetectOnceSearchCache *pDOSCache,
				struct DetectSearchArea *pDSArea
								)
{
	struct ListHead *pHead =	NULL;
    struct ListHead *pNode =	NULL;
	assert( NULL != pDOSCache );
    assert( NULL != pDSArea );

	pHead =	&pDOSCache->listHead;
    pNode =	&pDSArea->listHead;
    
    ListHeadTailInsert( pHead, pNode );
    DetectOnceSearchCacheAdd( pDOSCache );
    DetectOnceSearchCacheStatusSet( pDOSCache, TRUE );
}								

BOOL __DetectPilotSearchCmdAdd(
    				struct DetectOnceSearchCache *pDOSCache,
    				struct DetectPilotSearchCmd *pDPSCmd 
    							)
{
    struct DetectSearchArea *pNewArea =	NULL;
	pNewArea =	DetectSearchAreaCacheGet();

    printlog( LOG, "__DetectPilotSearchCmdAdd : pDOSCache=%p\n", pDOSCache );
    
    if ( NULL != pNewArea )
    {
    	DetectPilotSearchAreaGetByCmd( pNewArea, pDPSCmd );
        printlog( LOG, "\tSearchArea[pilot]: angle=%f\n", pNewArea->startAngle );
        DetectOnceSearchCacheInsert( pDOSCache, pNewArea );
        printlog( SCR, "DetectOnceSearchCacheInsert\n" );
        return TRUE;
    }
    else
    {
    	printfSafe( "DetectOnceSearchCmdProc failed!\n" );	
    }
    return FALSE;
}    							

/*
	����һ�������������
	------------------------
	pDOSCache -	���������ṹָ��
	pBuf -	�����ָ��
	size -	�����
*/
BOOL DetectPilotSearchCmdProc( 
			    struct DetectOnceSearchCache *pDOSCache,
				unsigned char *pBuf, 
				int size
							)
{
    struct DetectPilotSearchCmd *pDPSCmd =	NULL;
    
	assert( NULL != pDOSCache );
    assert( NULL != pBuf );

    pDPSCmd =	( struct DetectPilotSearchCmd * )pBuf;
    printlog( REC, "DetectPilotSearchCmdProc : pDOSCache=%p\n", pDOSCache );

	return __DetectPilotSearchCmdAdd( pDOSCache, pDPSCmd );    
}

struct DetectSearchArea *DetectOnceSearchCacheNodeGet(
				struct DetectOnceSearchCache	*pDOSCache
													)
{
	struct ListHead *pHead =	NULL;

    assert( NULL != pDOSCache );
    pHead =	&pDOSCache->listHead;
	if ( !IsListHeadEmpty( pHead ) )
    {
		struct ListHead *pLHPtr =	ListHeadRearGet( pHead );
        struct DetectSearchArea *pDSArea =	GetDetectSearchAreaPointer( pLHPtr );
		ListHeadDelete( pLHPtr->pPrev, pLHPtr->pNext );
        DetectOnceSearchCacheStatusSet( pDOSCache, TRUE );
        
        return pDSArea;
    }
    return NULL;
}													




