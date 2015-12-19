
/*
    文件说明:   被动分区状态反馈实现文件
    文件作者:   jsf
    文件时间:   2014.3.12
*/

#include "..\Header.h"



/*
	从被动循环搜索命令中提取被动循环搜索命令反馈函数
	pFeedBack -	待提取的被动循环搜索命令反馈结构
	pSearch -	被动循环搜索缓存结构数据源
*/
static void __attribute__((unused))
__DetectCycleSearchCmdFeedBackExtract(
		    					struct DetectCycleSearchFeedBack *pFeedBack,
		    					const struct DetectCycleSearchCache *pSearch
    									)
{
	
}

/*
	从被动重点搜索命令中提取被动重点搜索命令反馈函数
	pTargetFeedBack -	待获取的被动重点目标搜索命令反馈结构
	pTargetSearch -	被动重点目标搜索缓存结构数据源
*/
static void __attribute__((unused))
__DetectTargetSearchCmdFeedBackExtract(
							struct DetectTargetSearchFeedBack *pFeedBack,
							const struct DetectOnceSearchCache *pSearch
										)
{

}

/*
	被动任务引导搜索命令反馈函数
	pPilotFeedBack -	待获取的被动任务引导搜索命令反馈结构
	pPilotSearch -	被动任务引导搜索缓存结构数据源
*/
static void __attribute__((unused))
__DetectPilotSearchCmdFeedBackExtract(
							struct DetectPilotSearchFeedBack *pFeedBack,
							const struct DetectOnceSearchCache *pSearch
										)
{
	
}

/*
	被动搜索命令反馈处理函数	
*/
void DetectSearchCmdFeedBackProc( void )
{
	printlog( REC, "DetectSearchCmdFeedBackProc\n" );
}

void DetectSearchFeedBackReset(
					struct DetectSearchFeedBack *pInstance
								)
{
	assert( NULL != pInstance );
    printlog( REC, "DetectSearchFeedBackReset : pInstance=%p\n", pInstance );
}




