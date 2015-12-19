
#include "..\Header.h"

/*
	被动周期搜索缓存结构初始化函数
	------------------------------
	pDPSCache -	待初始化的被动周期搜索缓存指针
*/
void DetectPeriodSearchCacheInitialize( 
					struct DetectPeriodSearchCache *pDPSCache 
									)
{
	assert( NULL != pDPSCache );
    ListHeadInitialize( &pDPSCache->list );
    pDPSCache->calcs =	0;
    pDPSCache->status =	TRUE;
}									

void DetectPeriodSearchCacheReset(
					struct DetectPeriodSearchCache *pDPSCache
								)
{
	assert( NULL != pDPSCache );

	printlog( REC, "DetectPeriodSearchCacheReset : pDPSCache=%p\n", pDPSCache );
    DetectPeriodSearchCacheClear( pDPSCache );
}								


/*
	被动周期搜索缓存结构清空函数
	----------------------------
	pDPSCache -	待初始化的被动周期搜索缓存指针
*/
void DetectPeriodSearchCacheClear( 
				struct DetectPeriodSearchCache *pDPSCache 
								)
{
	printlog( REC, "DetectPeriodSearchCacheClear : pDPSCache=%p\n", pDPSCache );
    DetectSearchAreaCacheListClear( &pDPSCache->list );
    pDPSCache->calcs =	0;
}

/*
	向被动周期搜索缓存结构插入一个搜索区域
	--------------------------------------
	pDOSCache -	待插入的被动周期搜索缓存指针
	pDSArea -	被动搜索区域指针
*/
void DetectPeriodSearchCacheInsert(
				struct DetectPeriodSearchCache *pDPSCache,
				struct DetectSearchArea *pDSArea
								)
{
	struct ListHead *pHead =	NULL;
    struct ListHead *pNode =	NULL;
	assert( NULL != pDPSCache );
    assert( NULL != pDSArea );

	printlog( REC, "DetectPeriodSearchCacheInsert\n" );
	pHead =	&pDPSCache->list;
    pNode =	&pDSArea->listHead;
    
    ListHeadRearInsert( pHead, pNode );
    DetectPeriodSearchCacheAdc( pDPSCache );    
}								

/*
	向被动周期搜索缓存结构删除特定批号的搜索区域
	--------------------------------------------
	pDOSCache -	待插入的被动周期搜索缓存指针
	taskNo -	待删除的特定批号
*/
BOOL DetectPeriodSearchCacheDelete(
				struct DetectPeriodSearchCache *pDPSCache,
				unsigned int	targetNo
								)
{
	BOOL ret =	FALSE;
	struct ListHead *pHead =	NULL;
    struct ListHead *pPtr =	NULL;

	assert( NULL != pDPSCache );


    pHead =	&pDPSCache->list;
    pPtr =	pHead->pNext;
	while ( pPtr != pHead )
    {
    	struct ListHead *pNext =	pPtr->pNext;
    	struct DetectSearchArea *pSearchArea =	GetDetectSearchAreaPointer( pPtr );
        if ( targetNo == pSearchArea->targetNo )
        {
        	BOOL state =	FALSE;
        	ListHeadDelete( pPtr->pPrev, pPtr->pNext );
            state =	DetectSearchAreaCacheListHeadPut( pPtr );
            if ( !state )
            {
            	printfSafe( "DetectSearchAreaCacheListHeadPut failed!\n" );
            }
            else
            {
            	DetectPeriodSearchCacheDec( pDPSCache );
                ret =	TRUE;
            }
        }
            
    	pPtr =	pNext;
    }

    printlog( REC, "DetectPeriodSearchCacheDelete : targetNo=%d, result=%d\n",
        		targetNo, ret );
    return ret;
}


static BOOL __DetectTargetSearchCmdAdd(
					struct DetectPeriodSearchCache *pDPSCache,
					struct DetectScheduleTable *pDSTable,
					struct DetectTargetSearchCmd *pDTSCmd
	    							)
{
	struct DetectSearchArea *pNewArea =	DetectSearchAreaCacheGet();

    printlog( LOG, "__DetectTargetSearchCmdAdd : pDPSCache=%p\n",
        			pDPSCache );
    
    if ( NULL != pNewArea )
    {
    	BOOL ret =	FALSE;
    	DetectTargetSearchAreaGetByCmd( pNewArea, pDTSCmd );
        printlog( LOG, "\tSearchArea[Target] : angle=%f\n", pNewArea->startAngle );
        ret =   DetectScheduleTableInsert( pDSTable, pNewArea );
		if ( ret )
        {        	
            DetectPeriodSearchCacheInsert( pDPSCache, pNewArea );
        }
        else
        {
        	DetectSearchAreaCachePut( pNewArea );	
        }
        
        return ret;
    }
    else
    {
    	printfSafe( "__DetectTargetSearchCmdAdd Exception\n" );
    }
    return FALSE;
}    					

static BOOL __DetectTargetSearchCmdDelete( 
    				struct DetectPeriodSearchCache *pDPSCache,
    				struct DetectScheduleTable *pDSTable,
    				unsigned short	targetNo
	    								)
{
	printlog( REC, "__DetectTargetSearchCmdDelete : targetNo=%d\n", targetNo );
	DetectScheduleTableDelete( pDSTable, targetNo );
    
	return DetectPeriodSearchCacheDelete( pDPSCache, targetNo );
}	    								

BOOL DetectTargetSearchCmdProc(
				struct DetectPeriodSearchCache *pDPSCache,
				struct DetectScheduleTable *pDSTable,
				unsigned char 	*pBuf,
				int size
								)
{
	struct DetectTargetSearchCmd *pDTSCmd =	NULL;
    unsigned short	operateType =	-1;
	BOOL ret =	FALSE;
    unsigned short	targetNo =	-1;
    assert( NULL != pDPSCache );
    assert( NULL != pBuf );

    pDTSCmd =	( struct DetectTargetSearchCmd * )pBuf;
	operateType =	pDTSCmd->operateType;
	printlog( REC, "DetectTargetSearchCmdProc : pDPSCache=%p, operateType=%d\n",
        			pDPSCache, operateType );

    switch ( operateType )
    {
	case DETECT_TARGET_SEARCH_OPERATE_CREATE:
        ret =	__DetectTargetSearchCmdAdd( pDPSCache, pDSTable, pDTSCmd );
        break;
    case DETECT_TARGET_SEARCH_OPERATE_REMOVE:
        targetNo =	pDTSCmd->targetNo;
        ret =	__DetectTargetSearchCmdDelete( pDPSCache, pDSTable, targetNo  );
        break;
    case DETECT_TARGET_SEARCH_OPERATE_REMOVEALL:
        DetectScheduleTableClear( pDSTable );
        DetectPeriodSearchCacheClear( pDPSCache );
        ret =	TRUE;
        break;
    default:
        printfSafe( "DetectTargetSearchCmdProc exception\n" );
        break;
    }
    
    return ret;
}								


