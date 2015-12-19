
/*
	文件说明: 	被动支路处理头文件
	作者:		jsf
	时间:		2015.11.17
*/

#ifndef __DETECT_PROC_HEADER__
#define __DETECT_PROC_HEADER__

/*
	被动处理初始化函数
	------------------
	初始化被动命令处理、初始化被动调度链表结构
*/
EXPORT
void DetectProcInitialize( void );

EXPORT
void DetectProcReset( void );

/*
	被动调度处理函数
	----------------
*/
EXPORT
struct DetectScheduleAntennaNode * DetectScheduleProc( float courseAngle );


#endif

