
#ifndef __PRE_DEFINE_HEADER__
#define __PRE_DEFINE_HEADER__


#define __DEBUG__

#define __GNU_C_INLINE__

#ifdef __GNU_C_INLINE__
#define inline __inline
#endif

//#define _MEMORY_DBG_
#define _DBG_


#ifdef _MEMORY_DBG_
#define printfSafe	recMsg
#else

#ifdef _DBG_
#define printfSafe	printf
#else
#define printfSafe
#endif

#endif



#undef EXPORT
#undef IMPORT
#define EXPORT	extern
#define IMPORT	extern

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif




/*
	常用宏定义
*/
#undef BIT_SET
#ifndef BIT_SET
#define BIT_SET( bit, pos )		( bit |= ( 1<<pos ) )
#endif

#undef BIT_TEST
#ifndef BIT_TEST
#define BIT_TEST( bit, pos )	( bit & ( 1<<pos ) )
#endif

/*
	获取数组长度
*/
#define GetArraySize( array )	( sizeof( array )/sizeof( array[0] ) )

/*
	浮点比较函数
*/
#define FLOAT_PRECISION		( 0.000001 )
#define FLOAT_COMPARE_EQUAL	( 0 )
#define FLOAT_COMPARE_BIG	( 1 )
#define FLOAT_COMPARE_SMALL	( -1 )

#define ANGLE_MOD	( 360.0 )

static inline
float FloatLmtGet( float data, float lmt )
{
	return fmod( data+lmt, lmt );
}

static inline 
int FloatCompare( float a, float b )
{
	float result =	a - b;
    if ( fabs( result ) < FLOAT_PRECISION )
    {
    	return FLOAT_COMPARE_EQUAL;
    }
    else if ( result > 0 )
    {
    	return FLOAT_COMPARE_BIG;
    }
    else
    {
    	return FLOAT_COMPARE_SMALL;
    }
}  

#define FLOAT_RANGE_IN		1
#define FLOAT_RANGE_OUT		0
static inline
int FloatRangeCmp( float data, float min, float max )
{
	if ( FLOAT_COMPARE_BIG != FloatCompare( data, max )
        && FLOAT_COMPARE_SMALL != FloatCompare( data, min ) )
    {
 		return FLOAT_RANGE_IN;   	
    }
    return FLOAT_RANGE_OUT;
}

#define GET_MIN( a, b )		( (a) < (b) ? (a) : (b)  )
#define GET_MAX( a, b )		( (a) > (b) ? (a) : (b)  )



#endif

