
#include "Header.h"

static void __DetectSearchNodeToPackingAntennaNode(
					struct PackingAntennaNode *pPANode,
					const struct DetectSearchNode *pDSNode
										)
{
    if ( NULL != pDSNode )
    {
    	pPANode->angle =	GetQuantizedValue( pDSNode->angle, QUANTIZED_VALUE );
        pPANode->period =	pDSNode->stayPeriod;

        pPANode->type =	pDSNode->type;
        
        pPANode->targetNo =	pDSNode->targetNo;
        pPANode->frequency =	pDSNode->frequency;
        pPANode->pulseWidth =	pDSNode->pulseWidth;
    }
}		

void DetectScheduleAntennaNodeToPackingAntennaNode(
					struct DetectPackingNode *pDPNode,
					const struct DetectScheduleAntennaNode *pDSANode
										)
{
	BOOL isDSANodeEmpty =	FALSE;
    
	assert( NULL != pDPNode );
	assert( NULL != pDSANode );

	pDPNode->priority =	DETECT_SCHEDULE_PRIORITY;
    isDSANodeEmpty =	DetectScheduleAntennaNodeEmpty( pDSANode );
	pDPNode->isEmpty =	isDSANodeEmpty;
    
	if ( !isDSANodeEmpty )
    {
    	if ( DetectPilotStatusType == pDSANode->pL1->type )
        {
        	pDPNode->attri =	DETECT_PACKING_NODE_ATTRI_PILOT;
        }
        else
        {
        	pDPNode->attri =	DETECT_PACKING_NODE_ATTRI_FIGHT;
        }
        
    	//阵面1不空
    	if ( NULL != pDSANode->pL1 )
        {
        	struct PackingAntennaNode *pPANode =	&pDPNode->l1;
        	pDPNode->isA1Empty =	FALSE;
            __DetectSearchNodeToPackingAntennaNode( pPANode, pDSANode->pL1 );
        }
        //阵面2不空
    	if ( NULL != pDSANode->pL2 )
        {
        	struct PackingAntennaNode *pPANode =	&pDPNode->l2;
        	pDPNode->isA2Empty =	FALSE;
            __DetectSearchNodeToPackingAntennaNode( pPANode, pDSANode->pL2 );
        }
        //阵面3不空
    	if ( NULL != pDSANode->pL3 )
        {
        	struct PackingAntennaNode *pPANode =	&pDPNode->l3;
        	pDPNode->isA3Empty =	FALSE;
            __DetectSearchNodeToPackingAntennaNode( pPANode, pDSANode->pL3 );
        }
        //阵面4不空
    	if ( NULL != pDSANode->pL4 )
        {
        	struct PackingAntennaNode *pPANode =	&pDPNode->l4;
        	pDPNode->isA4Empty =	FALSE;
            __DetectSearchNodeToPackingAntennaNode( pPANode, pDSANode->pL4 );
        }
    }
    else
    {
    	pDPNode->attri =	DETECT_PACKING_NODE_ATTRI_WAIT;
    }
}										



