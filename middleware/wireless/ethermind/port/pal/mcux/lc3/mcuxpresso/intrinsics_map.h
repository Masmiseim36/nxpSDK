#ifndef _H_INSTRINSICS_MAP_
#define _H_INSTRINSICS_MAP_

#include "cmsis_gcc.h"

#ifdef __cplusplus
extern "C" {
#endif


static __inline INT32 qdadd(INT32 x, INT32 y)
{
    INT32 ret;
    __asm ("qdadd %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));

    return ret;
}

static __inline INT32 qdsub(INT32 x, INT32 y)
{
    INT32 ret;
    __asm ("qdsub %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));

    return ret;
}


//static __inline INT32 ssat(INT32 x, INT32 y)
//{
//    INT32 ret;
//	__asm ("ssat %0, #16, %1": "=r" (ret) : "r" (x));
//	return ret;
//}

static __inline INT32 clz(INT32 x)
{
    INT32 ret;
	__asm ("clz %0, %1": "=r" (ret) : "r" (x));
    return ret;
}

static __inline INT32 qadd16(INT32 x, INT32 y)
{
    INT32 ret;
    __asm ("qadd16 %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));

    return ret;
}

static __inline INT32 qsub16(INT32 x, INT32 y)
{
    INT32 ret;
    __asm ("qsub16 %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}


#if 1
static __inline  INT32 smlad(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlad %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif


#if 1  //unable to link this , so explicitly defined
static __inline  INT32 smulbb_t(INT32 x, INT32 y )
{
    INT32 ret;
    __asm ("smulbb %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif

#if 1
static __inline  INT32 smultt(INT32 x, INT32 y )
{
    INT32 ret;
    __asm ("smultt %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif

#if 1
static __inline  INT32 smulbt(INT32 x, INT32 y )
{
    INT32 ret;
    __asm ("smulbt %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif

#if 1
static __inline  INT32 smultb(INT32 x, INT32 y )
{
    INT32 ret;
    __asm ("smultb %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif


#if 1
static __inline  INT32 smlabb(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlabb %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif

#if 1
static __inline  INT32 smlabt(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlabt %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif

#if 1
static __inline  INT32 smlatb(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlatb %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif


#if 1  //unable to link this , so explicitly defined
static __inline  INT32 smlatt_t(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlatt %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif


#if 1
static __inline  INT32 smulwb(INT32 x, INT32 y )
{
    INT32 ret;
    __asm ("smulwb %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif

#if 1
static __inline  INT32 smulwt(INT32 x, INT32 y )
{
    INT32 ret;
    __asm ("smulwt %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif

#if 1
static __inline  INT32 smlawb(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlawb %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif


#if 1
static __inline  INT32 smlawt(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smlawt %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif



#if 1
static __inline  INT32 smmul(INT32 x, INT32 y)
{
    INT32 ret;
    __asm ("smmul %0, %1, %2": "=r" (ret) : "r" (x), "r" (y));
    return ret;
}
#endif


#if 1
static __inline  INT32 smmla(INT32 x, INT32 y, INT32 a)
{
    INT32 ret;
    __asm ("smmla %0, %1, %2, %3": "=r" (ret) : "r" (x), "r" (y), "r" (a));
    return ret;
}
#endif

//
//#define __QADD   		qadd
#define __QDADD   	    qdadd
//#define __QSUB   		qsub
#define __QDSUB   	    qdsub
//
#define __QADD16 		qadd16
#define __QSUB16 		qsub16
//
//#define __SSAT          ssat
#define __CLZ           clz
//
#define __SMULBB        smulbb_t
#define __SMULBT		smulbt
#define __SMULTT		smultt
#define __SMULTB		smultb
#define __SMULWB 		smulwb
#define __SMULWT		smulwt
//
#define __SMLABB        smlabb
#define __SMLABT		smlabt
#define __SMLATT		smlatt_t
#define __SMLATB		smlatb
#define __SMLAWB		smlawb
#define __SMLAWT		smlawt
//
#define __SMLAD 		smlad
//
//
#define __SMMUL 		smmul
#define __SMMLA 		smmla


#ifdef __cplusplus
}
#endif

#endif //_H_INSTRINSICS_MAP_
