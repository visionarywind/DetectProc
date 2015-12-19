
#include "..\Header.h"

void DetectScheduleTableInitialize(
					struct DetectScheduleTable	*pDSTable,
					int size
    								)
{
	int cacheSize =	-1;
    int i =	0;
    
	assert( NULL != pDSTable );

    pDSTable->num =	size;
    cacheSize =	size*sizeof( struct DetectSearchNode * );
    pDSTable->pTable =	(struct DetectSearchNode **)malloc( cacheSize );

	for ( i=0; i<pDSTable->num; ++i )
    {
    	pDSTable->pTable[i] =	NULL;
    }
    
    pDSTable->pos =	0;
}   

void DetectScheduleTableReset(
                struct DetectScheduleTable  *pDSTable
                            )
{
	assert( NULL != pDSTable );
    printlog( REC, "DetectScheduleTableReset : pDSTable=%p\n", pDSTable );

    DetectScheduleTableClear( pDSTable );
}	

void DetectScheduleTableClear(
				struct DetectScheduleTable	*pDSTable
							)
{
    int size =	0;
    int i =	0;
	struct DetectSearchNode *pSearchNode =	NULL;
    
    assert( NULL != pDSTable );
    printlog( REC, "DetectScheduleTableClear : table=%p\n", pDSTable );
    
    size =	pDSTable->num*sizeof( struct DetectSearchNode * );
	for ( i=0; i<pDSTable->num; ++i )
    {
    	pSearchNode =	pDSTable->pTable[i];
        if ( NULL != pSearchNode )
        {
        	DetectSearchNodeCachePut( pSearchNode );
        }
    	pDSTable->pTable[i] =	NULL;
    }
    pDSTable->pos =	0;
}								

static int DetectScheduleTablePositionFind(
    			const struct DetectScheduleTable *pDSTable
    									)
{
	int ret =	-1;
	int pos =	-1;
    struct DetectSearchNode **pTable =	NULL;

	assert( NULL != pDSTable );

    pTable =	pDSTable->pTable;
    
    for ( pos=0; pos<pDSTable->num; pos+=DETECT_SCHEDULE_TABLE_CHANNEL_SIZE )
    {
    	if ( NULL == pTable[pos] )
        {
        	ret =	pos;
            break;
        }
    }
    printlog( LOG, "DetectScheduleTablePositionFind : pos=%d\n", ret );
    
    return ret;
}    		
static BOOL __DetectScheduleTableCheck(
    							struct DetectSearchNode **pStart,
    							struct DetectSearchNode **pEnd
    							)
{
	while ( pStart != pEnd )
    {
    	if ( NULL != *pStart )
        {
        	return FALSE;
        }
        pStart ++;
    }
    return TRUE;
}
static BOOL __DetectScheduleTableMalloc(
    							struct DetectSearchNode **pStart,
    							struct DetectSearchNode **pEnd
    									)
{
	while ( pStart != pEnd )
    {
    	struct DetectSearchNode *pNewNode =	DetectSearchNodeCacheGet();
        printlog( REC, "__DetectScheduleTableMalloc malloc new node\n" );
        if ( NULL != pNewNode )
        {
        	*pStart =	pNewNode;
        }
    	else
        {
        	printfSafe( "__DetectScheduleTableMalloc failed!\n" );
        	return FALSE;
        }
    	pStart ++;
    }
    return TRUE;
}
BOOL DetectScheduleTableInsert(
    				struct DetectScheduleTable *pDSTable,
					const struct DetectSearchArea *pDSArea
								)
{
	int pos =	-1;
    
	assert( NULL != pDSTable );
    assert( NULL != pDSArea );
    printlog( LOG, "DetectScheduleTableInsert : targetNo=%d\n", pDSArea->targetNo );
    
	pos =	DetectScheduleTablePositionFind( pDSTable );
    if ( -1 != pos )
    {
    	struct DetectSearchNode **pStart =	pDSTable->pTable+pos;
        struct DetectSearchNode **pEnd =	pStart+DETECT_SCHEDULE_TABLE_CHANNEL_SIZE;
        BOOL isValid =	__DetectScheduleTableCheck( pStart, pEnd );
        if ( isValid )
        {
        	isValid =	__DetectScheduleTableMalloc( pStart, pEnd );
            if ( isValid )
            {
    	        DetectSearchNodeTargetExtract( pStart, pEnd, pDSArea );
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL DetectScheduleTableDelete(
					struct DetectScheduleTable *pDSTable,
					unsigned int mask
								)
{
	BOOL ret =	FALSE;
	int i =	0;
	struct DetectSearchNode *pSearchNode =	NULL;
    
	assert( NULL != pDSTable );
    
    for ( i=0; i<pDSTable->num; ++i )
    {
		pSearchNode =	pDSTable->pTable[i];
        if ( NULL != pSearchNode )
        {
	        if ( mask == pSearchNode->targetNo )
	        {
	        	pDSTable->pTable[i] =	NULL;
	            DetectSearchNodeCachePut( pSearchNode );
	            ret =	TRUE;
	        }
        }        
    }
//	printlog( REC, "DetectScheduleTableDelete : mask=%d, result=%d\n", mask, ret );
    
    return ret;
}


void DetectScheduleTableTick(
					struct DetectScheduleTable *pDSTable
							)
{
	assert( NULL != pDSTable );
	
    pDSTable->pos ++;
    pDSTable->pos %=	pDSTable->num;
}				

struct DetectSearchNode *DetectScheduleTableDataGet(
					const struct DetectScheduleTable *pDSTable
													)
{
	assert( NULL != pDSTable );
//	printlog( REC, "DetectScheduleTableDataGet\n" );

    return pDSTable->pTable[pDSTable->pos];
}													



