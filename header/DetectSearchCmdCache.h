
#ifndef __DETECT_SEARCH_CMD_CACHE_HEADER__
#define __DETECT_SEARCH_CMD_CACHE_HEADER__



/*
	������������״̬ö��ֵ����
*/
enum DetectSearchCmdStatus
{
	DetectSearchCmdStatusNoNeedUpdate =	0,	//����Ҫ����״̬
    DetectSearchCmdStatusNeedUpdate =	1,	//��Ҫ����״̬
    DetectSearchCmdStatusClear =	2,
};


////////////////////////////////////////////////////////////////////////////////
/*
	�������������ṹ
*/
struct DetectSearchCmdCache
{	
    struct DetectCycleSearchCache	cycleSearch;	//ѭ������
    struct DetectOnceSearchCache	pilotSearch;	//�ص�Ŀ����������
    struct DetectPeriodSearchCache	targetSearch;	//����������������
	
    SEM_ID	sync;	//ͬ���ź���
    BOOL	state;	//��ʼ�����
};

EXPORT
int DetectSearchCmdCacheEmpty(
		struct DetectSearchCmdCache *pDSCCache
								);	

/*
	����������ʼ������
	pCmdCache -	����ʼ���ı��������ָ��
*/
EXPORT
void DetectSearchCmdCacheInitialize( 
					struct  DetectSearchCmdCache *pInstance
									);

/*
	������������ú���
	pCmdCache -	�����õı��������ָ��
*/
EXPORT
void DetectSearchCmdCacheReset( 
					struct  DetectSearchCmdCache *pInstance
								);



////////////////////////////////////////////////////////////////////////////////

#endif

