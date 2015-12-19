
/*
	������������ļ�
*/
#ifndef __DETECT_PACKING_HEADER__
#define __DETECT_PACKING_HEADER__



#define QUANTIZED_VALUE_PRCISE	( 65535 )

#define QUANTIZED_VALUE	( 360.0 )
#define GetQuantizedValue( data, qv )	( (data)/(qv)*QUANTIZED_VALUE_PRCISE )
#define GetQuantizedValueReverse( data, qv )	( (data)*1.0/QUANTIZED_VALUE_PRCISE*(qv) )

//ÿ�� node 20�ֽ�
struct PackingAntennaNode
{
	unsigned int	angle;	//�Ƕ�
    unsigned short	period;	//פ��ʱ��
    unsigned char	type;	//�������� 0,ѭ������ 1,һ������ 2,�ص�Ŀ������ 3,���� 4����ʾ 5��δ֪
    unsigned int	targetNo;	//��������
    unsigned int	frequency;	//Ƶ��
    unsigned int	pulseWidth;	//����
};

static inline
void PackingAntennaNodeInitialize(
				struct PackingAntennaNode *pPANode
								)    
{
	assert( NULL != pPANode );
	memset( pPANode, 0xff, sizeof( struct PackingAntennaNode ) );
}						
#define DETECT_SCHEDULE_PRIORITY	( 0x2a )
#define DETECT_PACKING_NODE_ATTRI_PILOT	( 0 )
#define DETECT_PACKING_NODE_ATTRI_WAIT	( 1 )
#define DETECT_PACKING_NODE_ATTRI_FIGHT	( 3 )
struct DetectPackingNode
{
	unsigned int	isEmpty:1;           //��ǵ�ǰ�Ƿ��е��Ƚ��
    unsigned int	priority:5;           
	unsigned int	isA1Empty:1;          
	unsigned int	isA2Empty:1;          

    unsigned int	isA3Empty:1;          
	unsigned int	isA4Empty:1;         
	unsigned int	attri:2;	//�������	0,��ʾ 1������ 3,ս��
	unsigned int	freq_No:4;            /* number of frequency,*/

    unsigned int	waveform_No:4;        /* number of beam form */
    //next area
    unsigned int	PD_PCM_No:15;          /* cw2/d3��d0 4 nonius frequency used for PD; 16 codes used for PCM */
    unsigned int	ZMNo:2;    //ѡ�������
	unsigned int	sZMNo:2; 			//���������
	unsigned int	simuArrNo:2; 		//ģ��Ŀ�����ھ��������
	
	unsigned int	beam_width:5;         /* the width of beam 
                                               1--normal
                                               2--25  degree 
                                               3--15  degree
                                               4--25  degree
                                              26--2.6 degree */
    unsigned int	instruction_session:5;/* indicate working phase
                                            */
	unsigned int	mc4ToMc2Flag:1;    //�ж��Ƿ�ת2*2�ػ�׶�
	unsigned int	noise_statistics:1;   /* 0--noise statistics working, 1--noise statistics not working */
	//12
    unsigned char	id_num;               /* number the targers to distinguish them */
	unsigned char	mode_num;             /* channel number 0<=mode_num<=7 */
    unsigned short	missile_addr;         /* the code of missile address */
    //16
	unsigned char	allowInduce;			//�����ж�ģ��������
	unsigned char  m_class;				/*0:�Ľ���;1:������*/
	unsigned char  jiandui_num;
	//20
	float          alfa;                 /* azimuth in SINE coordinated system */
    //24
    struct PackingAntennaNode	l1;
    struct PackingAntennaNode	l2;
    struct PackingAntennaNode	l3;
    struct PackingAntennaNode	l4;
    
    //104	�����ʱ��λ�ò��ܸı䣬����Ӱ�����Ķ���
	char hour;           /* ʱ */
    char minute;         /* �� */
    char second;         /* �� */
    short int micros;    /* ���� */ 
    //112
	short int      simu_cwa3_36;
    //112
	short int      simu_cwa3_37;
	short int      simu_cwa3_38;
	short int      simu_cwa3_39;
	short int      simu_cwa3_40;
	short int      simu_cwa3_41;
	short int      simu_cwa3_42;
	short int      simu_cwa3_43;
	short int      simu_cwa3_44;
    //128
	float	       Rc_odd;       /* the oddment remained after the distance value is scaled in the scale that is one 
									to 15 meter when LFM, 30 meter when PD */
	float			fdval;
	float			pianZhoujiao;
    //140
	unsigned char	pianZhouLevel;
    short int      reserve1;             /* d0: start the track of missile for simulation platform */
};

static inline
void DetectPackingNodeInitialize(
			struct DetectPackingNode *pDPNode
								)    
{
	assert( NULL != pDPNode );

	memset( pDPNode, 0xFF, sizeof( struct DetectPackingNode ) );
}					

EXPORT
void DetectScheduleAntennaNodeToPackingAntennaNode(
					struct DetectPackingNode *pDPNode,
					const struct DetectScheduleAntennaNode *pDSANode
										); 

#define DETECT_PACKING_SCHEDULE_NODE_CODE	( 0xd )

//���������ֽṹ
struct DetectPackingScheduleNode
{
	unsigned short	size;
    unsigned short	bk[4];
    unsigned char	code;

#ifdef __PART_OF_FIGHT_WARE__    
    struct common_part	common;
#endif

    unsigned char	beamNum;
    struct DetectPackingNode node;
    unsigned char	edof;
};

#endif

