
/*
	�ļ�˵��: 	����֧·����ͷ�ļ�
	����:		jsf
	ʱ��:		2015.11.17
*/

#ifndef __DETECT_PROC_HEADER__
#define __DETECT_PROC_HEADER__

/*
	���������ʼ������
	------------------
	��ʼ�������������ʼ��������������ṹ
*/
EXPORT
void DetectProcInitialize( void );

EXPORT
void DetectProcReset( void );

/*
	�������ȴ�����
	----------------
*/
EXPORT
struct DetectScheduleAntennaNode * DetectScheduleProc( float courseAngle );


#endif

