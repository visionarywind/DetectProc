
#include "..\Header.h"




/*
	����������Ƚڵ���Ϣ��ʼ������
	------------------------------
	pScheduleNode -	����ʼ���ı���������Ƚڵ���Ϣָ��
*/
void DetectScheduleAntennaNodeInitialize( 
					struct DetectScheduleAntennaNode *pDSANode,
					struct DetectScheduleTable *pDSTable,
					struct DetectOnceSearch	*pPilotSearch,
					struct DetectAntennaArea *pDAArea
										)
{
	assert( NULL != pDSANode );
	assert( NULL != pDSTable );
    assert( NULL != pPilotSearch );
    assert( NULL != pDAArea );

    pDSANode->pTable =	pDSTable;
    pDSANode->pPilotSearch =	pPilotSearch;
    pDSANode->pDAArea =	pDAArea;

    DetectScheduleAntennaNodeReset( pDSANode );
}		

void DetectScheduleAntennaNodeReset(
					struct DetectScheduleAntennaNode *pDSANode
									)
{
    assert(  NULL != pDSANode );
    pDSANode->pL1 =	NULL;
    pDSANode->pL2 =	NULL;
    pDSANode->pL3 =	NULL;
    pDSANode->pL4 =	NULL;
}

void DetectScheduleAntennaNodeFree(
					struct DetectScheduleAntennaNode *pDSANode
									)
{
	assert( NULL != pDSANode );
    if ( NULL != pDSANode->pL1 )
    {
    	DetectSearchNodeCachePut( pDSANode->pL1 );
    	pDSANode->pL1 =	NULL;
    }
    if ( NULL != pDSANode->pL2 )
    {
    	DetectSearchNodeCachePut( pDSANode->pL2 );
    	pDSANode->pL2 =	NULL;
    }
    if ( NULL != pDSANode->pL3 )
    {
    	DetectSearchNodeCachePut( pDSANode->pL3 );
    	pDSANode->pL3 =	NULL;
    }
    if ( NULL != pDSANode->pL4 )
    {
    	DetectSearchNodeCachePut( pDSANode->pL4 );
    	pDSANode->pL4 =	NULL;
    }
}

static void __DetectScheduleAntennaNodeExtractFromNode(
								struct DetectScheduleAntennaNode *pDSANode,
								struct DetectSearchNode *pDSNode,
								float courseAngle
														)
{
	if ( DetectPilotStatusType == pDSNode->type )
    {
    	printlog( LOG, "Pilot DetectSearchNode Packed\n" );
    	pDSANode->pL1 =	pDSNode;
        pDSANode->pL2 =	pDSNode;
        pDSANode->pL3 =	pDSNode;
        pDSANode->pL4 =	pDSNode;
    }
    else
    {
    	int antennaNo =	DetectScheduleNodeAntennaJudge( pDSNode, courseAngle );

	    switch ( antennaNo )
	    {
		case 0:
	        pDSANode->pL1 =	pDSNode;
	        break;
	    case 1:
	        pDSANode->pL2 =	pDSNode;
	        break;
	    case 2:
	        pDSANode->pL3 =	pDSNode;
	        break;
	    case 3:
	        pDSANode->pL4 =	pDSNode;
	        break;
	    default:
	        break;
	    }
    }	
}

static void __DetectScheduleAntennaNodeExtractFromAntennaArea(
							struct DetectScheduleAntennaNode *pDSANode
    														)
{	
    struct DetectAntennaArea *pDAArea =	pDSANode->pDAArea;

	printlog( REC, "__DetectScheduleAntennaNodeExtractFromAntennaArea\n" );

	pDSANode->pL1 =	AntennaAreaNodeDataGet( pDAArea->areaNode );
    pDSANode->pL2 =	AntennaAreaNodeDataGet( pDAArea->areaNode+1 );
    pDSANode->pL3 =	AntennaAreaNodeDataGet( pDAArea->areaNode+2 );
    pDSANode->pL4 =	AntennaAreaNodeDataGet( pDAArea->areaNode+3 );

    if ( DetectScheduleAntennaNodeEmpty( pDSANode ) )
	{
		DetectAntennaAreaFlagReset( pDAArea );
        __DetectScheduleAntennaNodeExtractFromAntennaArea( pDSANode );
	}        

#ifdef __DEBUG_ONLY__	
	DetectScheduleAntennaNodePrint( REC, pDSANode );
#endif
}    														

static void __DetectScheduleAntennaNodeExtractFormList(
    					struct DetectScheduleAntennaNode *pDSANode,
    					float courseAngle
    													)
{
    //���ȼ��һ�������Ƿ�������
    struct DetectOnceSearch *pDOSearch =	pDSANode->pPilotSearch;
    BOOL isEmpty =	DetectOnceSearchEmpty( pDOSearch );
    
    printlog( REC, "__DetectScheduleAntennaNodeExtractFormList : isEmpty=%d\n",
        		isEmpty );

    if ( isEmpty )
    {	//Ϊ�գ���ѭ��������ѡȡ����
    	struct DetectAntennaArea *pDAArea =	pDSANode->pDAArea;
        if ( !DetectAntennaAreaEmpty( pDAArea ) )
        {
        	__DetectScheduleAntennaNodeExtractFromAntennaArea( pDSANode );
        }
        else
        {
        	printlog( REC, "DetectAntennaAreaEmpty\n" );
            DetectAntennaAreaPrint( REC, pDAArea );
        }
    }
    else
    {
    	//���ݸý���γɿ�����Ϣ
    	struct DetectSearchNode *pDSNode =	DetectOnceSearchNodeGet( pDOSearch );
        __DetectScheduleAntennaNodeExtractFromNode( pDSANode, pDSNode, courseAngle );
    }
}
			    
    
void DetectScheduleAntennaNodeExtract(
					struct DetectScheduleAntennaNode *pDSANode,
					float courseAngle
										)
{
    struct DetectSearchNode *pDSNode =	NULL;

    assert( NULL != pDSANode );
    
    printlog( REC, "DetectScheduleAntennaNodeExtract\n" );
        
	DetectScheduleAntennaNodeReset( pDSANode );
    //���ȴ�������������ѡȡ�����Ƕ�
	pDSNode =	DetectScheduleTableDataGet( pDSANode->pTable );

    if ( NULL != pDSNode )
    {
    	__DetectScheduleAntennaNodeExtractFromNode( pDSANode, pDSNode, courseAngle );
    }
    else
    {
    	__DetectScheduleAntennaNodeExtractFormList( pDSANode, courseAngle );
    }    
}




