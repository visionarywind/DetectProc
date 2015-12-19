
#ifndef __DETECT_SEARCH_AREA_HEADER__
#define __DETECT_SEARCH_AREA_HEADER__

/*
	������������
*/
enum DetectSearchType
{
    DetectCycleSearchType =	0,		//����ѭ������
    DetectTargetSearchType =	1,	//�����ص�Ŀ������
    DetectPilotSearchType =	2,		//����������������
    DetectWaitStatusType =	3,
    DetectPilotStatusType =	4,
    DetectUnknownSearchType =	5	//δ֪��������
};
#define DETECT_PILOT_STATUS_TYPE	( 4 )

#define DETECT_SEARCH_AREA_SLOW_TAP_POSITION	1
#define DETECT_SEARCH_AREA_QUICK_TAP_POSITION	0

#define DETECT_SEARCH_STAY_PERIOD		( 50.0 )

#define DETECT_SEARCH_AREA_MIN_ANGLE	( 0.0 )
#define DETECT_SEARCH_AREA_MAX_ANGLE	( 359.99 )
/* 
    ������������ṹ
*/
struct DetectSearchArea
{
	float	startAngle;	//��ʼ�Ƕ�
    float	endAngle;	//�����Ƕ�
    unsigned short	stayPeriod;	//פ��ʱ��

    enum DetectSearchType	type;		//��������
    unsigned char   tapPosition;    //��λ

    unsigned short	areaNo;		//����	 
    unsigned int	targetNo;		//��������

    unsigned int	startFrequency;	//��ʼƵ��
   	unsigned int	endFrequency;	//����Ƶ��

	struct ListHead listHead;		//����ָ��
};


#define GetDetectSearchAreaPointer( ptr )	\
			GetPointer(ptr, struct DetectSearchArea, listHead)


#ifdef __DEBUG__
static void __attribute__((unused))
DetectSearchAreaPrint(
			    int log,
				const struct DetectSearchArea *pDSArea
					)
{
	if ( NULL != pDSArea )
    {
    	printlog( log, "DetectSearchArea : angle=%f, stayPeriod=%d, type=%d, targetNo=%d\n",
        		pDSArea->startAngle, pDSArea->stayPeriod, pDSArea->type, pDSArea->targetNo );
    }
	else
	{
		printlog( log, "DetectSearchArea NULL\n" );
	}
}	

static void __attribute__((unused))
DetectSearchAreaListPrint(
				int log,
				const struct ListHead *pLhHead
						)
{
	struct ListHead *pLhPtr =	NULL;
	assert( NULL != pLhHead );

	printlog( log, "DetectSearchAreaListPrint : \n" );
    pLhPtr =	pLhHead->pNext;
    while ( pLhPtr != pLhHead )
    {
    	struct ListHead *pLhNext =	pLhPtr->pNext;
        struct DetectSearchArea *pDSArea =	GetDetectSearchAreaPointer( pLhPtr );

        DetectSearchAreaPrint( log, pDSArea );
        pLhPtr =	pLhNext;
    }
}			
#endif


static inline
BOOL DetectSearchAreaNeedsExpand( 
							const struct  DetectSearchArea *pDSArea
											)
{
	BOOL ret =	FALSE;
	assert( NULL != pDSArea );
    
    if ( DetectCycleSearchType == pDSArea->type )
    {
    	if ( DETECT_SEARCH_AREA_SLOW_TAP_POSITION == pDSArea->tapPosition )
        {
        	ret =	TRUE;
        }
    }
    
//    printlog( REC, "DetectSearchAreaNeedExpands ret = %d\n", ret );

    return ret;
}


/*
	�������������ʼ������
	pSearchArea -	����ʼ���ı�����������ָ��
*/
static inline
void DetectSearchAreaInitialize( 
								struct DetectSearchArea *pSearchArea 
													)
{
	memset( pSearchArea, 0, sizeof( struct DetectSearchArea ) );
}
/*
	�����������򿽱�����
	pDesArea -	�������ı�����������ָ��
	pSrcArea -	�����������򿽱�Դ����ָ��
*/
static inline
void DetectSearchAreaCopy( 
					struct DetectSearchArea *pDesArea,
					const struct DetectSearchArea *pSrcArea 
						)
{
	assert( NULL != pDesArea && NULL != pSrcArea );
    memcpy( pDesArea, pSrcArea, sizeof( struct DetectSearchArea ) );
}

/*
	�ɱ���ѭ������������ȡ������������ṹ
	--------------------------------------
	pSearchArea -	�����ȡ����ı�����������ṹָ��
	pCycleSearch -	����ѭ�����������ָ��
	type -	����ѭ���������� DETECT_SEARCH_TYPE_NORMAL��DETECT_SEARCH_TYPE_SLOW
*/
EXPORT
void DetectCycleSearchAreaGetByCmd( 
				struct DetectSearchArea *pSearchArea,
				const struct DetectCycleSearchCmd *pCycleSearch
								);
/*
	�ɱ����ص�Ŀ������������ȡ������������ṹ
	pSearchArea -	����ȡ�ı�����������ṹ
	pTargetSearch -	�����ص�Ŀ�����������ָ��
*/
EXPORT
void DetectTargetSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectTargetSearchCmd *pTargetSearch 
								);
/*
	�ɱ���������������������ȡ������������ṹ
	pSearchArea -	����ȡ�ı�����������ṹ
	pTargetSearch -	���������������������ָ��
*/
EXPORT
void DetectPilotSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectPilotSearchCmd *pPilotSearch 
								);



#endif

