
/*
	��������������������ͷ�ļ�
	--------------------------
*/

#ifndef __DETECT_SEARCH_ANTENNA_AREA_HEADER__
#define __DETECT_SEARCH_ANTENNA_AREA_HEADER__

/*
	��������ǶȽṹ
*/
struct AntennaAreaAngle
{
	float	start;	//������ʵ��ʼ�Ƕ�
    float	end;	//������ʵ�����Ƕ�
    
    float	startAssistant;	//������ʵ��ʼ�Ƕȸ���
    float	endAssistant;	//������ʵ�����Ƕȸ���
    
    BOOL	assist;
    BOOL	lastAssist;
    BOOL	flag;
};

static inline
BOOL AntennaAreaAngleCrossArea(
			struct AntennaAreaAngle *pAAAngle
								)    
{
	BOOL ret =	FALSE;
	assert( NULL != pAAAngle );

    if ( pAAAngle->flag )
    {
    	ret =	pAAAngle->lastAssist;	
    }
    else
    {
        ret =   pAAAngle->assist;
    }
    
	printlog( REC, "AntennaAreaAngleCrossArea : ret=%d\n", ret );
    return ret;
}								

static inline
void AntennaAreaAngleTrack(
			struct AntennaAreaAngle *pAAAngle
							)    
{
	assert( NULL != pAAAngle );

    
    pAAAngle->lastAssist =	pAAAngle->assist;
    if ( FALSE == pAAAngle->flag )
    {
    	pAAAngle->flag =	TRUE;
    }
    /*else if ( -1 == pAAAngle->flag )
    {
    	pAAAngle->flag =	0;
    }
    else if ( 0 == pAAAngle->flag )
    {
    	pAAAngle->flag =	1;
    }*/
    printlog( REC, "AntennaAreaAngleTrack : assist=%d, lastAssist=%d\n",
        		pAAAngle->assist, pAAAngle->lastAssist );
}							

#ifdef __DEBUG__
static void __attribute__((unused))
AntennaAreaAnglePrint( 
					int log,
					const struct AntennaAreaAngle *pAngle 
								)
{
	assert( NULL != pAngle );

	printlog( log, "AntennaAreaAngle: assist=%d, start=%f, end=%f, "
        	"startAssistant=%f, endAssistant=%f\n", 
        	pAngle->assist, pAngle->start, pAngle->end,
        	pAngle->startAssistant, pAngle->endAssistant );
}
#endif

/*
	�жϽǶ��Ƿ�����������ǶȽṹ��
	--------------------------------
	pAngle -	��������ǶȽṹָ��
	angle -	���жϵĽǶ�
*/
EXPORT
BOOL AntennaAreaAngleRangeCmp(
    					const struct AntennaAreaAngle *pAngle,
    					float angle
							);			

/*
	��������ǶȽṹ��ʼ������
*/
EXPORT
void AntennaAreaAngleInitialize( 
			    			struct AntennaAreaAngle *pAreaAngle,
			    			float startAngle,
			    			float endAngle
    							);

/*
	��������ڵ�ṹ
*/
struct AntennaAreaNode
{
	float	start;	//������ʼ�Ƕ�
    float	end;	//��������Ƕ�

	struct AntennaAreaAngle	realAngle;
    
    struct ListHead	list;	//��������ͷ
    
    struct ListHead	links;	//��������
};

static inline
BOOL AntennaAreaNodeCrossAreaTrack(
				struct AntennaAreaNode *pAANode
									)    
{
	assert( NULL != pAANode );

   return AntennaAreaAngleCrossArea( &pAANode->realAngle );
}									

static inline
BOOL AntennaAreaNodeCrossArea(
			const struct AntennaAreaNode *pAANode
							)    
{
	assert( NULL != pAANode );
    
    return pAANode->realAngle.assist;
}

#ifdef __DEBUG__
static void AntennaAreaNodePrint(
				int log,
				struct AntennaAreaNode *pAANode
								)
{
	struct ListHead *pLhPtr =	NULL;
    struct ListHead *pLhHead =	NULL;
    
	assert( NULL != pAANode );
	printlog( log, "\tAntennaAreaNodePrint : crossArea=%d\n", 
		        AntennaAreaNodeCrossArea( pAANode ) );
    
    pLhHead =	&pAANode->list;
    pLhPtr =	pLhHead->pNext;
    while ( pLhPtr != pLhHead )
    {
    	struct ListHead *pLhNext =	pLhPtr->pNext;
    	struct DetectSearchNode *pDSNode =	GetDetectSearchNodePointer( pLhPtr );
        DetectSearchNodePrint( log, pDSNode );
        pLhPtr =	pLhNext;
    }
}								
#endif

EXPORT
void AntennaAreaNodeClear(
			struct AntennaAreaNode *pAANode
						);

EXPORT
struct DetectSearchNode *AntennaAreaNodeDataGet(
							struct AntennaAreaNode *pAANode							
												);

EXPORT
BOOL AntennaAreaNodeDataEmpty(
		const struct AntennaAreaNode *pAANode
							);    

/*
	��������ڵ�ṹָ���ȡ��
*/
#define GetAntennaAreaNodePointer( ptr )	\
    	GetPointer( (ptr), struct AntennaAreaNode, links )

/*
	��������ڵ�ṹ��һ�ڵ��ȡ����
	--------------------------------
	pAreaNode -	����ȡ����������ڵ�ṹָ��
*/    	
EXPORT
struct AntennaAreaNode *AntennaAreaNodePrevGet( 
							const struct AntennaAreaNode *pAreaNode
												);												

/*
	��������ڵ�ṹ��һ�ڵ��ȡ����
	--------------------------------
	pAreaNode -	����ȡ����������ڵ�ṹָ��	
*/    	
EXPORT
struct AntennaAreaNode *AntennaAreaNodeNextGet( 
							const struct AntennaAreaNode *pAreaNode
												);

/*
	�жϽǶ��Ƿ�����������ڵ�ṹ��
	--------------------------------
	pAngle -	��������ڵ�ṹָ��
	angle -	���жϵĽǶ�
*/
EXPORT
BOOL AntennaAreaNodeRangeCmp(
				const struct AntennaAreaNode *pAreaNode,
				float angle
							);

/*
	��������ڵ�ṹ�Ƕ�������º���
	----------------------------
	pAreaNode -	�����µ���������ڵ�ṹָ��
	offset -	ƫ�ƽǶȣ�Ҳ���Ǻ����
*/
EXPORT
void AntennaAreaNodeUpdate(
				struct AntennaAreaNode *pAreaNode,
				float offset
							);								

EXPORT 
int AntennaAreaNodeDelete(
				struct AntennaAreaNode *pAANode,
				unsigned int	mask
							);


/*
	��������ڵ�ṹ�Ƕ�������뺯��
	--------------------------------
	pAreaNode -	���������������ڵ�ṹָ��
	pSearchNode -	������ı����������ָ��
*/
EXPORT
void AntennaAreaNodePut(
			struct AntennaAreaNode *pAreaNode,
			struct DetectSearchNode *pSearchNode
						);	

/*
	��������ڵ�ṹ��ʼ������
	--------------------------
	pNode -	����ʼ������������ڵ�ṹָ��
	start -	����������ʼ�Ƕ�
	end -	������������Ƕ�
*/
EXPORT
void AntennaAreaNodeInitialize( 
						struct AntennaAreaNode *pNode, 
						float start,
						float end
								);

/*
	��������ṹ
*/
#define ANTENNA_AREA_START_ANGLE	( 0.0 )
#define ANTENNA_AREA_END_ANGLE		( 89.99 )
#define ANTENNA_AREA_ANGLE_AREA		( 90.0 )
#define ANTENNA_AREA_NUMBER			( (int)(360.0/( ANTENNA_AREA_ANGLE_AREA )) )

struct DetectAntennaArea
{
	struct AntennaAreaNode	areaNode[ANTENNA_AREA_NUMBER];
    BOOL	color[ANTENNA_AREA_NUMBER];
    BOOL	empty;
    float	offset;	
};

#ifdef __DEBUG__
static
void DetectAntennaAreaPrint(
			int log,
			struct DetectAntennaArea *pDAArea
							)
{
	int i =	0;
	assert( NULL != pDAArea );

    printlog( log, "DetectAntennaAreaPrint\n" );
    for ( i=0; i<ANTENNA_AREA_NUMBER; ++i )
    {
    	AntennaAreaNodePrint( log, pDAArea->areaNode+i );

        printlog( log, "DetectAntennaAreaPrint : color[%d]=%d\n",
            			i, pDAArea->color[i] );
    }
}							
#endif

static inline
void DetectAntennaAreaOffsetSet( 
			struct DetectAntennaArea *pDAArea,
			float	offset
						)    
{
	assert( NULL != pDAArea );
    pDAArea->offset =	offset;
}						

EXPORT
void DetectAntennaAreaClear(
			struct DetectAntennaArea *pDAArea
							);

static inline
BOOL DetectAntennaAreaEmpty(
			const struct DetectAntennaArea *pDAArea
							)
{
	assert( NULL != pDAArea );
    return pDAArea->empty;
}							

EXPORT
void DetectAntennaAreaFlagReset(
				struct DetectAntennaArea *pDAArea
								);

EXPORT
void DetectAntennaAreaColorSet(
					struct DetectAntennaArea *pDAArea
								);

/*
	��������ṹ��ʼ������
	----------------------
	pArea -	����ʼ������������ṹָ��
	offset -	ƫ����
*/
EXPORT
void DetectAntennaAreaInitialize( 
					struct DetectAntennaArea *pArea,
					float offset
								);

EXPORT
void DetectAntennaAreaReset(
					struct DetectAntennaArea *pArea,
					float offset
							);    

/*
	��������ṹ�Ƕȸ��º���
	------------------------
	pArea -	 �����µ���������ṹָ��
	courseAngle -	��ǰ�ĺ����
*/
EXPORT
BOOL DetectAntennaAreaOffsetUpdate( 
			struct DetectAntennaArea *pArea,
			float courseAngle
						);
/*
	��������ṹ�ڵ���º���
	------------------------
	pArea -	 �����µ���������ṹָ��
*/
EXPORT
void DetectAntennaAreaUpdate( 
			struct DetectAntennaArea *pDAArea
						);

EXPORT
BOOL DetectAntennaAreaListPut(
			struct DetectAntennaArea *pDAArea,
			struct ListHead *pLhHead
							);    

/*
	��һ�������ڵ���뵽��������ṹ��
	----------------------------------
	pArea -	���������������ṹָ��
	pNode -	������������ڵ�struct ListHead�ṹָ��
*/
EXPORT
BOOL DetectAntennaAreaPut(
			struct DetectAntennaArea *pArea,
			struct DetectSearchNode *pDSNode
					);

EXPORT
BOOL DetectAntennaAreaDelete(
			struct DetectAntennaArea *pDAArea,
			unsigned int	mask
							);  

#endif

