
#ifndef __DETECT_SEARCH_STRUCTURE__
#define __DETECT_SEARCH_STRUCTURE__

enum 
{
	UnknownSearch =	0,	//δ֪��������
    CycleSearch =	1,	//ѭ����������
    TargetSearch =	2,	//�ص�Ŀ����������
    PilotSearch =	3	//����������������
} DetectScheduleType;

struct ExternalHead
{
	unsigned short	id;			//��Ϣ��ʶ
    unsigned short	len;		//��Ϣ����
    unsigned short	no;			//��Ϣ���
    unsigned short	packageNo;	//������
    unsigned int	reserve1;	//����1
    unsigned int	reserve2;	//����2
} __attribute__( (packed) );

#define DETECT_SEARCH_TYPE_CYCLE		( 1 )
#define DETECT_SEARCH_TYPE_TARGET		( 2 )
#define DETECT_SEARCH_TYPE_PILOT		( 3 )

#define DETECT_CYCLE_SEARCH_OPERATE_CREATE		( 1 )
#define DETECT_CYCLE_SEARCH_OPERATE_REMOVE		( 2 )
#define DETECT_CYCLE_SEARCH_OPERATE_REMOVEALL	( 3 )
/*
	ѭ�������ṹ
*/
struct DetectCycleSearchCmd
{
	struct ExternalHead	head;	//��Ϣͷ

    unsigned short	waveType;		//��������    	0��L  1��s  2:C  3:X
	unsigned short	operateType;	//�����������	1,����  2������  3��ȫ������
	unsigned short	areaCode;		//����	1-6
	
	unsigned short	stayTime;		//פ��ʱ��
	
	unsigned int	startFrequency;	//��ʼƵ��
	unsigned int	endFrequency;	//����Ƶ��
	
	unsigned short	startAngle;		//��ʼ��λ
	unsigned short	endAngle;		//������λ
	unsigned short	tapPosition;
	unsigned short	reserve;		//����
};


#define DETECT_TARGET_SEARCH_OPERATE_CREATE		( 1 )
#define DETECT_TARGET_SEARCH_OPERATE_REMOVE		( 2 )
#define DETECT_TARGET_SEARCH_OPERATE_REMOVEALL	( 3 )
/*
	�������Ӿ��ص�Ŀ��̽��
*/
struct DetectTargetSearchCmd
{
	struct ExternalHead	head;

    unsigned short	waveType;		//��������    	0��L  1��s  2:C  3:X
	unsigned short	operateType;		//����
	unsigned short	targetNo;		//Ŀ������
	
	unsigned short	stayTime;		//פ��ʱ��

    unsigned int	stayStartTime;	//פ����ʼʱ��
    unsigned int	stayStartTimeOffset;	//פ����ʼʱ�����ƫ��

    unsigned short	angle;	//��λ lsb:0.01��
	unsigned short	reserve2;	//����
    
	unsigned int	stratFrequency;	//��ʼƵ��	lsb 0.1m  
	unsigned int	endFrequency;	//����Ƶ��	lsb 0.1m  

	unsigned int	reserve3;		//����
} __attribute__( (packed) ) ;


/*
	�������Ӿ���������̽��
*/
struct DetectPilotSearchCmd
{
	struct ExternalHead	head;	//��Ϣͷ

    unsigned short	waveType;		//��������    	0��L  1��s  2:C  3:X
	unsigned short	operateType;	//�������ñ�ʶ		1������ 2������
	unsigned short	areaCode;		//����	������	[1-4]
	unsigned short	angle;			//��λ ��λ����,LSB��0.01�� [0~360)��
	
	unsigned short	stayTime;		//פ��ʱ��
	unsigned short	reserve1;		//����
	unsigned short	reserve2;		//����
	unsigned short	reserve3;		//����
} __attribute__( (packed) ) ;


#endif

