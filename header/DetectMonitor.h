
#ifndef __DETECT_MONITOR_HEADER__
#define __DETECT_MONITOR_HEADER__

#define DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_CODE			( 1 )
#define DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_TAPPOSITION	( 0 )
#define DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_START_FREQ	( 0xCCCCCCCC )	
#define DETECT_CYCLE_SEARCH_MONITOR_FULL_AREA_END_FREQ		( 0x33333333 )

EXPORT
void DetectCycleSearchCmdMonitorFullArea( void );
EXPORT
void DetectCycleSearchCmdMonitorFullAreaExit( void );
EXPORT
BOOL IsDetectCycleSearchCmdMonitorCmd( 
					struct DetectCycleSearchCache *pDCSCache
									);
EXPORT
void DetectCycleSearchCmdMonitorReset(
					struct DetectCycleSearchCache *pDCSCache
									);    

#endif

