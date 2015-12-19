
/*
	�ļ�˵��: 	����֧·����ʵ���ļ�
	����:		jsf
	ʱ��:		2015.11.17
*/
#include "..\Header.h"

////////////////////////////////////////////////////////////////////////////////
/*
	���������ʼ������
	------------------
	��ʼ�������������ʼ��������������ṹ
*/
void DetectProcInitialize( void )
{
	printfSafe( "DetectProcInitialize\n" );
    DetectLogRecInit();
	//����������㻺�湦�ܳ�ʼ������
	DetectSearchNodeCacheBufferInit();
	//�����������򻺴湦�ܳ�ʼ������
	DetectSearchAreaCacheBufferInit();
    //����������ܳ�ʼ������
    DetectCmdProcInit();
	//�������ȴ����ʼ��
    DetectScheduleInit();
}

void DetectProcReset( void )
{
	DetectScheduleRestart();
}


/*
	�������ȴ�����
	----------------
*/
struct DetectScheduleAntennaNode * DetectScheduleProc( float courseAngle )
{
    struct DetectScheduleAntennaNode *result =	NULL;

	printlog( LOG, "\n-- DetectScheduleProcStart --\n" );
	printlog( LOG, "-- cmd proc --\n" );
    //�����
	DetectCmdProc();
    
	printlog( LOG, "-- data proc --\n" );    
    DetectScheduleDataProc( courseAngle );
	printlog( LOG, "-- schedule proc --\n" );    
	result =	DetectScheduleScheduleProc();
    DetectScheduleAntennaNodePrint( LOG, result );
//    DetectScheduleAntennaNodePrint( SCR, result );
    DetectScheduleAntennaNodeSet( result );

	return result;
}

////////////////////////////////////////////////////////////////////////////////


