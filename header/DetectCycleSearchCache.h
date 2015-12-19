
#ifndef __DETECT_CYCLE_SEARCH_CACHE_HEADER__
#define __DETECT_CYCLE_SEARCH_CACHE_HEADER__

#define DETECT_SEARCH_CMD_CYCLE_MAX_NUM		( 6 )

/*
	����ѭ����������ṹ
	-------------------------------------
	color�������ڶ�������������кϷ����ж�
*/
struct DetectCycleSearchCache
{
	struct DetectSearchArea	area[DETECT_SEARCH_CMD_CYCLE_MAX_NUM];	//�������򻺴�
    BOOL color[DETECT_SEARCH_CMD_CYCLE_MAX_NUM];    //������
    BOOL status;
};

EXPORT
BOOL DetectCycleSearchCacheEmpty(
			struct DetectCycleSearchCache *pDCSCache
								);	

static inline
void DetectCycleSearchCacheStatusSet( 
					struct DetectCycleSearchCache *pDCSCache,
					BOOL status
									)
{
	assert( NULL != pDCSCache );
	pDCSCache->status =	status;
}									
static inline
BOOL DetectCycleSearchCacheStatusGet(
					const struct DetectCycleSearchCache *pDCSCache
									)
{
	assert( NULL != pDCSCache );
    
	return pDCSCache->status;
}									

/*
	����ѭ����������ṹ��ʼ������
	pSearchCache -	����ʼ���ı���ѭ����������ָ��
*/
EXPORT
void DetectCycleSearchCacheInitialize( 
						struct DetectCycleSearchCache *pSearchCache 
									);

EXPORT
void DetectCycleSearchCacheClear(
					struct DetectCycleSearchCache *pDCSCache
								);

EXPORT
void DetectCycleSearchCacheReset(
					struct DetectCycleSearchCache *pDCSCache
								);

/*
	�������������������
	------------------------
	pInstance -	���������ṹָ��
	pBuf -	�����ָ��
	size -	�����
*/
EXPORT
BOOL DetectCycleSearchCmdProc( 
			    struct DetectCycleSearchCache *pDCSCache,
				unsigned char *pBuf, 
				int size
							);



#endif

