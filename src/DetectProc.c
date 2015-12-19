
/*
	文件说明: 	被动支路处理实现文件
	作者:		jsf
	时间:		2015.11.17
*/
#include "..\Header.h"

////////////////////////////////////////////////////////////////////////////////
/*
	被动处理初始化函数
	------------------
	初始化被动命令处理、初始化被动调度链表结构
*/
void DetectProcInitialize( void )
{
	printfSafe( "DetectProcInitialize\n" );
    DetectLogRecInit();
	//被动搜索结点缓存功能初始化函数
	DetectSearchNodeCacheBufferInit();
	//被动搜索区域缓存功能初始化函数
	DetectSearchAreaCacheBufferInit();
    //被动命令处理功能初始化函数
    DetectCmdProcInit();
	//被动调度处理初始化
    DetectScheduleInit();
}

void DetectProcReset( void )
{
	DetectScheduleRestart();
}


/*
	被动调度处理函数
	----------------
*/
struct DetectScheduleAntennaNode * DetectScheduleProc( float courseAngle )
{
    struct DetectScheduleAntennaNode *result =	NULL;

	printlog( LOG, "\n-- DetectScheduleProcStart --\n" );
	printlog( LOG, "-- cmd proc --\n" );
    //命令处理
	DetectCmdProc();
    
	printlog( LOG, "-- data proc --\n" );    
    DetectScheduleDataProc( courseAngle );
	printlog( LOG, "-- schedule proc --\n" );    
	result =	DetectScheduleScheduleProc();
    DetectScheduleAntennaNodePrint( LOG, result );
//    DetectScheduleAntennaNodePrint( SCR, result );
    DetectScheduleAntennaNodeSet( result );

	return result;
}

////////////////////////////////////////////////////////////////////////////////


