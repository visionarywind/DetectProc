
/*
    �ļ�˵��:   ��������״̬����ʵ���ļ�
    �ļ�����:   jsf
    �ļ�ʱ��:   2014.3.12
*/

#include "..\Header.h"



/*
	�ӱ���ѭ��������������ȡ����ѭ���������������
	pFeedBack -	����ȡ�ı���ѭ������������ṹ
	pSearch -	����ѭ����������ṹ����Դ
*/
static void __attribute__((unused))
__DetectCycleSearchCmdFeedBackExtract(
		    					struct DetectCycleSearchFeedBack *pFeedBack,
		    					const struct DetectCycleSearchCache *pSearch
    									)
{
	
}

/*
	�ӱ����ص�������������ȡ�����ص��������������
	pTargetFeedBack -	����ȡ�ı����ص�Ŀ������������ṹ
	pTargetSearch -	�����ص�Ŀ����������ṹ����Դ
*/
static void __attribute__((unused))
__DetectTargetSearchCmdFeedBackExtract(
							struct DetectTargetSearchFeedBack *pFeedBack,
							const struct DetectOnceSearchCache *pSearch
										)
{

}

/*
	�������������������������
	pPilotFeedBack -	����ȡ�ı���������������������ṹ
	pPilotSearch -	��������������������ṹ����Դ
*/
static void __attribute__((unused))
__DetectPilotSearchCmdFeedBackExtract(
							struct DetectPilotSearchFeedBack *pFeedBack,
							const struct DetectOnceSearchCache *pSearch
										)
{
	
}

/*
	�������������������	
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




