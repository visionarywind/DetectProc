
#ifndef __DETECT_SEARCH_NODE_HEADER__
#define __DETECT_SEARCH_NODE_HEADER__


/*
	������������ڵ�
*/
struct DetectSearchNode
{
	float	angle;	//�����Ƕ�
	unsigned short	stayPeriod;	//פ��ʱ��
	unsigned char	type;	//��������

    unsigned int	frequency;	//Ƶ��
    unsigned short 	pulseWidth;	//����

    unsigned int	targetNo;

	BOOL	flag;

    struct ListHead	listHead;	//�ڵ�����ָ��
};

EXPORT
void DetectSearchNodeFlagSet(
			struct DetectSearchNode *pDSNode,
			BOOL flag
							);		


/*
    ͨ��listHead�ڵ��ȡ���������ڵ�ṹ���׵�ַ
*/
#define GetDetectSearchNodePointer(listHeadPtr) \
        GetPointer( (listHeadPtr), struct DetectSearchNode, listHead )


static inline
void DetectSearchNodeCopy( struct DetectSearchNode *pDes,
							const struct DetectSearchNode *pSrc )    
{
	memcpy( pDes, pSrc, sizeof( struct DetectSearchNode ) );
}							

#ifdef __DEBUG__
static void __attribute__((unused))
DetectSearchNodePrint(
			    int log,
				const struct DetectSearchNode *pDSNode
					)
{
	if ( NULL != pDSNode )
    {
    	printlog( log, "\tDetectSearchNode : angle=%f, stayPeriod=%d, type=%d, targetNo=%d, flag=%d\n",
        		pDSNode->angle, pDSNode->stayPeriod, pDSNode->type, pDSNode->targetNo,
        		pDSNode->flag );
    }
	else
	{
		printlog( log, "\tDetectSearchNode NULL\n" );
	}
}	

static void __attribute__((unused))
DetectSearchNodeListPrint(
				int log,
				const struct ListHead *pLhHead
						)
{
	struct ListHead *pLhPtr =	NULL;
	assert( NULL != pLhHead );

	printlog( log, "DetectSearchNodeListPrint : " );
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
void DetectSearchNodeListInsertByOrder(
							struct ListHead *pLhHead,
							struct ListHead *pLhNode
										);


EXPORT
void DetectSearchNodeListRearInsertByOrder(
								struct ListHead *pLhHead,
								struct ListHead *pLhNode
											);

EXPORT
void DetectSearchNodeListTailInsertByOrder(
								struct ListHead *pLhHead,
								struct ListHead *pLhNode
											);

EXPORT
void DetectSearchNodeTargetExtract(
					struct DetectSearchNode **pStart,
					struct DetectSearchNode	**pEnd,
					const struct DetectSearchArea *pDSArea
									);

/*
	������������ڵ�������ȡ����
	------------------------------------------
	pSearchNode -	���γɵı�����������ڵ�ָ��
	pSearchArea -	������������ṹָ��
	offset -	����ڵ�ǰ��ƫ����
*/
EXPORT
void DetectSearchNodeExtract( 
					struct DetectSearchNode *pSearchNode,
					const struct DetectSearchArea *pSearchArea,
					float offset
							);
/*
	��������������ṹ�γ�ִ�к���
	--------------------------------
	pListHead -	���γɱ�������������ṹ��������ָ��
	pSearchArea -	�γɱ�������������ṹ����ı�����������ṹָ��
	step -	�γɵı�������������Ĳ���
*/
EXPORT
void DetectSearchNodeExplain(
					struct ListHead *pListHead,
					const struct DetectSearchArea *pSearchArea,
					float step
							);

////////////////////////////////////////////////////////////////////////////////
EXPORT
int DetectScheduleNodeAntennaJudge(
					struct DetectSearchNode *pDSNode,
					float courseAngle
							);

#endif

