
#ifndef __DETECT_SCHEDULE_TABLE_HEADER__
#define __DETECT_SCHEDULE_TABLE_HEDAER__

#define DETECT_SCHEDULE_TABLE_SIZE		( 400 )
#define DETECT_SCHEDULE_TABLE_CHANNEL_SIZE	( 20 )
struct DetectScheduleTable
{
	struct DetectSearchNode	**pTable;

    int	num;
    int	pos;
};			

#ifdef __DEBUG__
static void __attribute__((unused))
DetectScheduleTablePrint(
				int log,
				const struct DetectScheduleTable *pDSTable
									)
{
	if ( NULL != pDSTable )
    {
    	int i =	0;
        for ( i=0; i<pDSTable->num; ++i )
        {
        	struct DetectSearchNode *pDSNode =	pDSTable->pTable[i];
        	if ( NULL != pDSNode )
            {
            	DetectSearchNodePrint( log, pDSNode );
            }
        }
    }
}									
#endif

EXPORT
void DetectScheduleTableInitialize(
					struct DetectScheduleTable	*pDSTable,
					int size
    								);

EXPORT
void DetectScheduleTableReset(
                    struct DetectScheduleTable  *pDSTable
                                );
    

EXPORT
void DetectScheduleTableClear(
					struct DetectScheduleTable	*pDSTable
								);

EXPORT
BOOL DetectScheduleTableInsert(
				    struct DetectScheduleTable *pDSTable,
					const struct DetectSearchArea *pDSArea
								);
EXPORT
BOOL DetectScheduleTableDelete(
					struct DetectScheduleTable *pDSTable,
					unsigned int mask
								);

EXPORT
void DetectScheduleTableTick(
					struct DetectScheduleTable *pDSTable
							);    

EXPORT
struct DetectSearchNode *DetectScheduleTableDataGet(
					const struct DetectScheduleTable *pDSTable
													);

#endif

