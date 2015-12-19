
/*
	被动结点数据文件
*/
#ifndef __DETECT_PACKING_HEADER__
#define __DETECT_PACKING_HEADER__



#define QUANTIZED_VALUE_PRCISE	( 65535 )

#define QUANTIZED_VALUE	( 360.0 )
#define GetQuantizedValue( data, qv )	( (data)/(qv)*QUANTIZED_VALUE_PRCISE )
#define GetQuantizedValueReverse( data, qv )	( (data)*1.0/QUANTIZED_VALUE_PRCISE*(qv) )

//每个 node 20字节
struct PackingAntennaNode
{
	unsigned int	angle;	//角度
    unsigned short	period;	//驻留时间
    unsigned char	type;	//搜索类型 0,循环搜索 1,一次搜索 2,重点目标搜索 3,待命 4，领示 5，未知
    unsigned int	targetNo;	//任务批号
    unsigned int	frequency;	//频率
    unsigned int	pulseWidth;	//脉宽
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
	unsigned int	isEmpty:1;           //标记当前是否有调度结点
    unsigned int	priority:5;           
	unsigned int	isA1Empty:1;          
	unsigned int	isA2Empty:1;          

    unsigned int	isA3Empty:1;          
	unsigned int	isA4Empty:1;         
	unsigned int	attri:2;	//结点属性	0,领示 1，待命 3,战斗
	unsigned int	freq_No:4;            /* number of frequency,*/

    unsigned int	waveform_No:4;        /* number of beam form */
    //next area
    unsigned int	PD_PCM_No:15;          /* cw2/d3～d0 4 nonius frequency used for PD; 16 codes used for PCM */
    unsigned int	ZMNo:2;    //选择阵面号
	unsigned int	sZMNo:2; 			//搜索阵面号
	unsigned int	simuArrNo:2; 		//模拟目标所在绝对阵面号
	
	unsigned int	beam_width:5;         /* the width of beam 
                                               1--normal
                                               2--25  degree 
                                               3--15  degree
                                               4--25  degree
                                              26--2.6 degree */
    unsigned int	instruction_session:5;/* indicate working phase
                                            */
	unsigned int	mc4ToMc2Flag:1;    //判断是否转2*2截获阶段
	unsigned int	noise_statistics:1;   /* 0--noise statistics working, 1--noise statistics not working */
	//12
    unsigned char	id_num;               /* number the targers to distinguish them */
	unsigned char	mode_num;             /* channel number 0<=mode_num<=7 */
    unsigned short	missile_addr;         /* the code of missile address */
    //16
	unsigned char	allowInduce;			//用于判断模拟器交班
	unsigned char  m_class;				/*0:改进型;1:基本型*/
	unsigned char  jiandui_num;
	//20
	float          alfa;                 /* azimuth in SINE coordinated system */
    //24
    struct PackingAntennaNode	l1;
    struct PackingAntennaNode	l2;
    struct PackingAntennaNode	l3;
    struct PackingAntennaNode	l4;
    
    //104	下面的时间位置不能改变，否则将影响程序的对齐
	char hour;           /* 时 */
    char minute;         /* 分 */
    char second;         /* 秒 */
    short int micros;    /* 毫秒 */ 
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

//被动调度字结构
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

