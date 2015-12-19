
/*
	被动调度阵面搜索处理头文件
	--------------------------
*/

#ifndef __DETECT_SEARCH_ANTENNA_AREA_HEADER__
#define __DETECT_SEARCH_ANTENNA_AREA_HEADER__

/*
	阵面区域角度结构
*/
struct AntennaAreaAngle
{
	float	start;	//阵面真实起始角度
    float	end;	//阵面真实结束角度
    
    float	startAssistant;	//阵面真实起始角度附加
    float	endAssistant;	//阵面真实结束角度附加
    
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
	判断角度是否在阵面区域角度结构中
	--------------------------------
	pAngle -	阵面区域角度结构指针
	angle -	待判断的角度
*/
EXPORT
BOOL AntennaAreaAngleRangeCmp(
    					const struct AntennaAreaAngle *pAngle,
    					float angle
							);			

/*
	阵面区域角度结构初始化函数
*/
EXPORT
void AntennaAreaAngleInitialize( 
			    			struct AntennaAreaAngle *pAreaAngle,
			    			float startAngle,
			    			float endAngle
    							);

/*
	阵面区间节点结构
*/
struct AntennaAreaNode
{
	float	start;	//阵面起始角度
    float	end;	//阵面结束角度

	struct AntennaAreaAngle	realAngle;
    
    struct ListHead	list;	//调度链表头
    
    struct ListHead	links;	//链接链表
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
	阵面区间节点结构指针获取宏
*/
#define GetAntennaAreaNodePointer( ptr )	\
    	GetPointer( (ptr), struct AntennaAreaNode, links )

/*
	阵面区间节点结构上一节点获取函数
	--------------------------------
	pAreaNode -	待获取的阵面区间节点结构指针
*/    	
EXPORT
struct AntennaAreaNode *AntennaAreaNodePrevGet( 
							const struct AntennaAreaNode *pAreaNode
												);												

/*
	阵面区间节点结构下一节点获取函数
	--------------------------------
	pAreaNode -	待获取的阵面区间节点结构指针	
*/    	
EXPORT
struct AntennaAreaNode *AntennaAreaNodeNextGet( 
							const struct AntennaAreaNode *pAreaNode
												);

/*
	判断角度是否在阵面区间节点结构中
	--------------------------------
	pAngle -	阵面区间节点结构指针
	angle -	待判断的角度
*/
EXPORT
BOOL AntennaAreaNodeRangeCmp(
				const struct AntennaAreaNode *pAreaNode,
				float angle
							);

/*
	阵面区间节点结构角度区间更新函数
	----------------------------
	pAreaNode -	待更新的阵面区间节点结构指针
	offset -	偏移角度，也就是航向角
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
	阵面区间节点结构角度区间插入函数
	--------------------------------
	pAreaNode -	待插入的阵面区间节点结构指针
	pSearchNode -	待插入的被动搜索结点指针
*/
EXPORT
void AntennaAreaNodePut(
			struct AntennaAreaNode *pAreaNode,
			struct DetectSearchNode *pSearchNode
						);	

/*
	阵面区间节点结构初始化函数
	--------------------------
	pNode -	待初始化的阵面区间节点结构指针
	start -	阵面区间起始角度
	end -	阵面区间结束角度
*/
EXPORT
void AntennaAreaNodeInitialize( 
						struct AntennaAreaNode *pNode, 
						float start,
						float end
								);

/*
	阵面区间结构
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
	阵面区间结构初始化函数
	----------------------
	pArea -	待初始化的阵面区间结构指针
	offset -	偏移量
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
	阵面区间结构角度更新函数
	------------------------
	pArea -	 待更新的阵面区间结构指针
	courseAngle -	当前的航向角
*/
EXPORT
BOOL DetectAntennaAreaOffsetUpdate( 
			struct DetectAntennaArea *pArea,
			float courseAngle
						);
/*
	阵面区间结构节点更新函数
	------------------------
	pArea -	 待更新的阵面区间结构指针
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
	将一个搜索节点加入到阵面区间结构中
	----------------------------------
	pArea -	待加入的阵面区间结构指针
	pNode -	待加入的搜索节点struct ListHead结构指针
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

