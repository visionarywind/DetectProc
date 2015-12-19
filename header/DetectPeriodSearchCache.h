
#ifndef __DETECT_PERIOD_SEARCH_CACHE_HEADER__
#define __DETECT_PERIOD_SEARCH_CACHE_HEADER__


#define DETECT_SEARCH_CMD_TARGET_MAX_NUM	( 20 )
/*
	�������������ṹ
	----------------
	�ò��ֵĴ�����DetectScheduleTable���ֵ����ݽṹ����ʹ��
	��DetectScheduleTable�У�ÿ�����ʹ��DetectSearchNode��ָ������DetectSearchArea
	�Ӷ�����ɶ�DetectScheduleTable�ĸ���

	Ȼ���ٸ������ţ��ڻ�����ɾ������
*/
struct DetectPeriodSearchCache
{
	struct ListHead list;	//����ڵ�

    struct DetectScheduleTable	*pTable;
    
    int calcs;	//����
    BOOL status;
};

static inline
BOOL DetectPeriodSearchCacheEmpty(
		struct DetectPeriodSearchCache *pDPSCache
								)
{
	assert( NULL != pDPSCache );

	return IsListHeadEmpty( &pDPSCache->list );
}								

/*
	����������������ṹ��ʼ������
	------------------------------
	pDPSCache -	����ʼ���ı���������������ָ��
*/
EXPORT
void DetectPeriodSearchCacheInitialize( 
					struct DetectPeriodSearchCache *pDPSCache 
									);	

EXPORT
void DetectPeriodSearchCacheReset(
					struct DetectPeriodSearchCache *pDPSCache
								);

static inline
void DetectPeriodSearchCacheAdc(
			struct DetectPeriodSearchCache *pDPSCache
							)    
{
	assert( NULL != pDPSCache );
	pDPSCache->calcs ++;
}						
static inline
void DetectPeriodSearchCacheDec(
			struct DetectPeriodSearchCache *pDPSCache
							)  
{
    assert( NULL != pDPSCache );
	pDPSCache->calcs --;
}							

/*
	����������������ṹ��պ���
	----------------------------
	pDPSCache -	����ʼ���ı���������������ָ��
*/
EXPORT
void DetectPeriodSearchCacheClear( 
				struct DetectPeriodSearchCache *pDPSCache 
								);


/*
	�򱻶�������������ṹ����һ����������
	--------------------------------------
	pDOSCache -	������ı���������������ָ��
	pDSArea -	������������ָ��
*/
EXPORT
void DetectPeriodSearchCacheInsert(
				struct DetectPeriodSearchCache *pDPSCache,
				struct DetectSearchArea *pDSArea
								);

/*
	�򱻶�������������ṹɾ���ض����ŵ���������
	--------------------------------------------
	pDOSCache -	������ı���������������ָ��
	taskNo -	��ɾ�����ض�����
*/
EXPORT
BOOL DetectPeriodSearchCacheDelete(
				struct DetectPeriodSearchCache *pDPSCache,
				unsigned int	taskNo
								);

EXPORT
BOOL DetectTargetSearchCmdProc(
				struct DetectPeriodSearchCache *pDPSCache,
				struct DetectScheduleTable *pDSTable,
				unsigned char 	*pBuf,
				int size
								);

#endif

