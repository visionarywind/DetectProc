
#include "..\Header.h"


/*
	�ɱ���ѭ������������ȡ������������ṹ
	--------------------------------------
	pSearchArea -	�����ȡ����ı�����������ṹָ��
	pCycleSearch -	����ѭ�����������ָ��
	type -	����ѭ���������� DETECT_SEARCH_TYPE_NORMAL��DETECT_SEARCH_TYPE_SLOW
*/
void DetectCycleSearchAreaGetByCmd( 
				struct DetectSearchArea *pSearchArea,
				const struct DetectCycleSearchCmd *pCycleSearch
								)
{
	if ( NULL != pSearchArea && NULL != pCycleSearch )
    {
    	pSearchArea->startAngle =	pCycleSearch->startAngle;	//��ʼ�Ƕ�
    	pSearchArea->endAngle =	pCycleSearch->endAngle;			//�����Ƕ�

		if ( FLOAT_COMPARE_SMALL == 
            	FloatCompare( pSearchArea->startAngle, DETECT_SEARCH_AREA_MIN_ANGLE ) )
        {
        	pSearchArea->startAngle =	DETECT_SEARCH_AREA_MIN_ANGLE;
        }
        if ( FLOAT_COMPARE_BIG == 
            	FloatCompare( pSearchArea->endAngle, DETECT_SEARCH_AREA_MAX_ANGLE ) )
        {
        	pSearchArea->endAngle =	DETECT_SEARCH_AREA_MAX_ANGLE;
        }
        
        pSearchArea->stayPeriod =	pCycleSearch->stayTime;		//פ��ʱ��
        pSearchArea->type =	DetectCycleSearchType;			//��������
        pSearchArea->areaNo =	pCycleSearch->areaCode;			//����
        pSearchArea->tapPosition =	pCycleSearch->tapPosition;

        pSearchArea->startFrequency =	pCycleSearch->startFrequency;
        pSearchArea->endFrequency =	pCycleSearch->endFrequency;

        printlog( SCR, "DetectCycleSearchAreaGetByCmd : tapPosition=%d, start=%x,"
            			"end=%x\n",
            				pSearchArea->tapPosition, 
            				pSearchArea->startFrequency,
            				pSearchArea->endFrequency
            				);

        ListHeadInitialize( &( pSearchArea->listHead ) );
    }
   	else
    {
    	printfSafe( "Exception - DetectCycleSearchAreaGetByCmd\n" );
    }
}

/*
	�ɱ����ص�Ŀ������������ȡ������������ṹ
	pSearchArea -	����ȡ�ı�����������ṹ
	pTargetSearch -	�����ص�Ŀ�����������ָ��
*/
void DetectTargetSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectTargetSearchCmd *pTargetSearch 
								)
{
	if ( NULL != pSearchArea && NULL != pTargetSearch )
    {
    	pSearchArea->startAngle =	pTargetSearch->angle;	//��ʼ�Ƕ�
    	pSearchArea->endAngle =	pTargetSearch->angle;			//�����Ƕ�
        pSearchArea->stayPeriod =	pTargetSearch->stayTime;		//פ��ʱ��
        pSearchArea->type =	DetectTargetSearchType;			//��������
        pSearchArea->areaNo =	pTargetSearch->targetNo;			//����
        pSearchArea->targetNo =	pTargetSearch->targetNo;			//����

//        pSearchArea->startFrequency =	pTargetSearch->startFrequency;
//        pSearchArea->endFrequency =	pTargetSearch->endFrequency;

        ListHeadInitialize( &( pSearchArea->listHead ) );
    }
}

/*
	�ɱ���������������������ȡ������������ṹ
	pSearchArea -	����ȡ�ı�����������ṹ
	pTargetSearch -	���������������������ָ��
*/
void DetectPilotSearchAreaGetByCmd( 
					struct DetectSearchArea *pSearchArea,
					const struct DetectPilotSearchCmd *pPilotSearch 
								)
{
	if ( NULL != pSearchArea && NULL != pPilotSearch )
    {
    	pSearchArea->startAngle =	pPilotSearch->angle;	//��ʼ�Ƕ�
    	pSearchArea->endAngle =	pPilotSearch->angle;			//�����Ƕ�
        pSearchArea->stayPeriod =	pPilotSearch->stayTime;		//פ��ʱ��
        pSearchArea->type =	DetectPilotSearchType;			//��������
//        pSearchArea->areaNo =	pPilotSearch->targetNo;			//����

//        pSearchArea->startFrequency =	pPilotSearch->startFrequency;
//        pSearchArea->endFrequency =	pPilotSearch->endFrequency;

        ListHeadInitialize( &( pSearchArea->listHead ) );
    }
}



