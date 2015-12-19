
#ifndef __DETECT_SEARCH_STRUCTURE__
#define __DETECT_SEARCH_STRUCTURE__

enum 
{
	UnknownSearch =	0,	//未知搜索类型
    CycleSearch =	1,	//循环搜索类型
    TargetSearch =	2,	//重点目标搜索类型
    PilotSearch =	3	//任务引导搜索类型
} DetectScheduleType;

struct ExternalHead
{
	unsigned short	id;			//信息标识
    unsigned short	len;		//信息长度
    unsigned short	no;			//信息编号
    unsigned short	packageNo;	//打包编号
    unsigned int	reserve1;	//备份1
    unsigned int	reserve2;	//备份2
} __attribute__( (packed) );

#define DETECT_SEARCH_TYPE_CYCLE		( 1 )
#define DETECT_SEARCH_TYPE_TARGET		( 2 )
#define DETECT_SEARCH_TYPE_PILOT		( 3 )

#define DETECT_CYCLE_SEARCH_OPERATE_CREATE		( 1 )
#define DETECT_CYCLE_SEARCH_OPERATE_REMOVE		( 2 )
#define DETECT_CYCLE_SEARCH_OPERATE_REMOVEALL	( 3 )
/*
	循环搜索结构
*/
struct DetectCycleSearchCmd
{
	struct ExternalHead	head;	//信息头

    unsigned short	waveType;		//波段类型    	0：L  1：s  2:C  3:X
	unsigned short	operateType;	//命令控制类型	1,建立  2。撤销  3，全部撤销
	unsigned short	areaCode;		//区号	1-6
	
	unsigned short	stayTime;		//驻留时间
	
	unsigned int	startFrequency;	//起始频率
	unsigned int	endFrequency;	//结束频率
	
	unsigned short	startAngle;		//起始方位
	unsigned short	endAngle;		//结束方位
	unsigned short	tapPosition;
	unsigned short	reserve;		//备份
};


#define DETECT_TARGET_SEARCH_OPERATE_CREATE		( 1 )
#define DETECT_TARGET_SEARCH_OPERATE_REMOVE		( 2 )
#define DETECT_TARGET_SEARCH_OPERATE_REMOVEALL	( 3 )
/*
	被动超视距重点目标探测
*/
struct DetectTargetSearchCmd
{
	struct ExternalHead	head;

    unsigned short	waveType;		//波段类型    	0：L  1：s  2:C  3:X
	unsigned short	operateType;		//备份
	unsigned short	targetNo;		//目标批号
	
	unsigned short	stayTime;		//驻留时间

    unsigned int	stayStartTime;	//驻留起始时刻
    unsigned int	stayStartTimeOffset;	//驻留起始时刻最大偏移

    unsigned short	angle;	//方位 lsb:0.01度
	unsigned short	reserve2;	//备份
    
	unsigned int	stratFrequency;	//起始频率	lsb 0.1m  
	unsigned int	endFrequency;	//结束频率	lsb 0.1m  

	unsigned int	reserve3;		//备份
} __attribute__( (packed) ) ;


/*
	被动超视距任务引导探测
*/
struct DetectPilotSearchCmd
{
	struct ExternalHead	head;	//信息头

    unsigned short	waveType;		//波段类型    	0：L  1：s  2:C  3:X
	unsigned short	operateType;	//引导设置标识		1：设置 2：撤销
	unsigned short	areaCode;		//区号	引导区	[1-4]
	unsigned short	angle;			//方位 单位：度,LSB＝0.01度 [0~360)度
	
	unsigned short	stayTime;		//驻留时间
	unsigned short	reserve1;		//备份
	unsigned short	reserve2;		//备份
	unsigned short	reserve3;		//备份
} __attribute__( (packed) ) ;


#endif

