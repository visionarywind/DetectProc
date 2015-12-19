
/*
    �ļ�˵��:   ��������״̬����ͷ�ļ�
    �ļ�����:   jsf
    �ļ�ʱ��:   2014.3.12
*/

#ifndef __101_DETECT_SEARCH_FEED_BACK_HEADER__
#define __101_DETECT_SEARCH_FEED_BACK_HEADER__



#define DECTION_WORK_STATE_INFO_FLAG    0xbb01  /* �ֻ����ͱ�ʶ��bb01Ϊ������յ���Ϣͷ */

////////////////////////////////////////////////////////////////////////////////
/*
	������������ֽṹ����
*/

enum DetectSearchFeedBackStatus
{
	DetectSearchFeedBackStatusNoNeedUpdate =	0,
    DetectSearchFeedBackStatusNeedUpdate =	1
};

/*
	����ѭ�������������ṹ
*/
struct DetectCycleSearchFeedBackNode
{
	unsigned short	startAngle;	//��ʼ�Ƕ�
    unsigned short	endAngle;	//�����Ƕ�
    unsigned short	stayPeriod;	//פ��ʱ��
};
/*
	����ѭ�����������ṹ
*/
struct DetectCycleSearchFeedBack
{
	//ѭ������������㻺��
	struct DetectCycleSearchFeedBackNode nodes[DETECT_SEARCH_CMD_CYCLE_MAX_NUM];
    BOOL color[ DETECT_SEARCH_CMD_CYCLE_MAX_NUM ];	//������㻺����
    enum DetectSearchFeedBackStatus status;
};

static inline
void DetectCycleSearchFeedBackStatusSet(
						struct DetectCycleSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
	pFeedBack->status =	status;
}										

static inline
enum DetectSearchFeedBackStatus DetectCycleSearchFeedBackStatusGet(
						const struct DetectCycleSearchFeedBack *pFeedBack
										)
{
	return pFeedBack->status;
}										


static inline
void DetectCycleSearchFeedBackInitialize( 
						struct DetectCycleSearchFeedBack *pFeedBack
										)
{
	int size =	0;
	assert( NULL != pFeedBack );
    size =	sizeof( struct DetectCycleSearchFeedBackNode )*DETECT_SEARCH_CMD_CYCLE_MAX_NUM;
    memset( pFeedBack->nodes, 0, size );
    size =	sizeof( BOOL )*DETECT_SEARCH_CMD_CYCLE_MAX_NUM;
    memset( pFeedBack->color, 0, size );
    pFeedBack->status =	DetectSearchFeedBackStatusNoNeedUpdate;
}										

/*
	�����ص�Ŀ�������������ṹ
*/
struct DetectTargetSearchFeedBackNode
{
	unsigned short	angle;		//�Ƕ�
	unsigned short	stayPeriod;	//פ��ʱ��
};
/*
	�����ص�Ŀ�����������ṹ
*/
struct DetectTargetSearchFeedBack
{
	//�ص�Ŀ������������㻺��
	struct DetectTargetSearchFeedBackNode nodes[DETECT_SEARCH_CMD_TARGET_MAX_NUM];
    BOOL color[DETECT_SEARCH_CMD_TARGET_MAX_NUM];	//������㻺����
    enum DetectSearchFeedBackStatus status;
};

static inline
void DetectTargetSearchFeedBackStatusSet(
						struct DetectTargetSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
	pFeedBack->status =	status;
}										

static inline
enum DetectSearchFeedBackStatus DetectTargetSearchFeedBackStatusGet(
						const struct DetectTargetSearchFeedBack *pFeedBack
										)
{
	return pFeedBack->status;
}		


static inline
void DetectTargetSearchFeedBackInitialize( 
						struct DetectTargetSearchFeedBack *pFeedBack
											)
{
	int size =	0;
    assert( NULL != pFeedBack );
    size =	sizeof( struct DetectTargetSearchFeedBackNode )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->nodes, 0, size );
    size =	sizeof( BOOL )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->color, 0, size );
    pFeedBack->status =	DetectSearchFeedBackStatusNoNeedUpdate;
}											

/*
	�����������������������ṹ
*/
struct DetectPilotSearchFeedBackNode
{
	unsigned short	startAngle;	//��ʼ�Ƕ�
	unsigned short	endAngle;	//�����Ƕ�
	unsigned short	stayPeriod;	//פ��ʱ��
};
/*
	���������������������ṹ
*/
struct DetectPilotSearchFeedBack
{
	//������������������㻺��
	struct DetectPilotSearchFeedBackNode nodes[DETECT_SEARCH_CMD_PILOT_MAX_NUM];
    BOOL color[DETECT_SEARCH_CMD_PILOT_MAX_NUM];	//������㻺����
    enum DetectSearchFeedBackStatus	status;
};

static inline
void DetectPilotSearchFeedBackStatusSet(
						struct DetectPilotSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
	pFeedBack->status =	status;
}										

static inline
enum DetectSearchFeedBackStatus DetectPilotSearchFeedBackStatusGet(
						const struct DetectPilotSearchFeedBack *pFeedBack
										)
{
	return pFeedBack->status;
}		

static inline
void DetectPilotSearchFeedBackInitialize( 
						struct DetectPilotSearchFeedBack *pFeedBack
											)
{
	int size =	0;
    assert( NULL != pFeedBack );
    size =	sizeof( struct DetectPilotSearchFeedBackNode )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->nodes, 0, size );
    size =	sizeof( BOOL )*DETECT_SEARCH_CMD_TARGET_MAX_NUM;
    memset( pFeedBack->color, 0, size );
    pFeedBack->status =	DetectSearchFeedBackStatusNoNeedUpdate;
}											


/*
	�����������������ṹ
*/
struct DetectSearchFeedBack
{
	struct DetectCycleSearchFeedBack	cycleSearch;	//����ѭ�����������ṹ

    struct DetectTargetSearchFeedBack	targetSearch;	//�����ص�Ŀ�����������ṹ
    struct DetectPilotSearchFeedBack	pilotSearch;	//���������������������ṹ

    SEM_ID	sync;	//ͬ���ź��� 
    BOOL	state;	//��ʼ�����
};

static inline
void DetectSearchFeedBackPilotStatusSet(
						struct DetectSearchFeedBack *pFeedBack,
						enum DetectSearchFeedBackStatus status
										)
{
    DetectPilotSearchFeedBackStatusSet( &pFeedBack->pilotSearch, status	);
}

/*
	�����������������ṹ��ʼ������
	pFeedBack -	�������ı����������������ṹָ��
*/
static inline
void DetectSearchFeedBackInitialize( 
					struct DetectSearchFeedBack *pInstance
									)
{
	assert( NULL != pInstance );
    DetectCycleSearchFeedBackInitialize( &pInstance->cycleSearch );
    DetectTargetSearchFeedBackInitialize( &pInstance->targetSearch );
    DetectPilotSearchFeedBackInitialize( &pInstance->pilotSearch );
    
    pInstance->sync =	semBCreate( SEM_Q_FIFO, SEM_FULL );
    pInstance->state =	TRUE;
}

EXPORT
void DetectSearchFeedBackReset(
					struct DetectSearchFeedBack *pInstance
								);

/*
	�������������������	
*/
EXPORT
void DetectSearchCmdFeedBackProc( void );

#endif

