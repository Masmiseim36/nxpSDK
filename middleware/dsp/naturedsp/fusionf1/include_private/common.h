/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (�Cadence    */
/* Libraries�) are the copyrighted works of Cadence Design Systems Inc.	    */
/* Cadence IP is licensed for use with Cadence processor cores only and     */
/* must not be used for any other processors and platforms. Your use of the */
/* Cadence Libraries is subject to the terms of the license agreement you   */
/* have entered into with Cadence Design Systems, or a sublicense granted   */
/* to you by a direct Cadence licensee.                                     */
/* ------------------------------------------------------------------------ */
/*  IntegrIT, Ltd.   www.integrIT.com, info@integrIT.com                    */
/*                                                                          */
/* DSP Library                                                              */
/*                                                                          */
/* This library contains copyrighted materials, trade secrets and other     */
/* proprietary information of IntegrIT, Ltd. This software is licensed for  */
/* use with Cadence processor cores only and must not be used for any other */
/* processors and platforms. The license to use these sources was given to  */
/* Cadence, Inc. under Terms and Condition of a Software License Agreement  */
/* between Cadence, Inc. and IntegrIT, Ltd.                                 */
/* ------------------------------------------------------------------------ */
/*          Copyright (C) 2015-2016 IntegrIT, Limited.                      */
/*                      All Rights Reserved.                                */
/* ------------------------------------------------------------------------ */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <assert.h>

#include "NatureDSP_Signal.h"
#ifdef __RENAMING__
//#include "__renaming__.h"
#endif

#if defined COMPILER_XTENSA
  #include <xtensa/config/core-isa.h>
  #include <xtensa/tie/xt_core.h>
  #include <xtensa/tie/xt_misc.h>
  #include <xtensa/tie/xt_hifi3.h>
#if (XCHAL_HAVE_FUSION_FP)
  #include <xtensa/tie/xt_FP.h>
#endif
#else
  #include "core-isa.h"
#if defined COMPILER_MSVC
  #pragma warning( disable :4800 4244)
#endif
/* the code below causes inclusion of file "cstub-"XTENSA_CORE".h" */
#define PPCAT_NX(A, B) A-B
#define PPCAT(A, B) PPCAT_NX(A, B)      /* Concatenate preprocessor tokens A and B after macro-expanding them. */
#define STRINGIZE_NX(A) #A              /* Turn A into a string literal without expanding macro definitions */
#define STRINGIZE(A) STRINGIZE_NX(A)    /*  Turn A into a string literal after macro-expanding it. */
//#include STRINGIZE(PPCAT(cstub,XTENSA_CORE).h)
#include STRINGIZE(PPCAT(PPCAT(cstub,XTENSA_CORE),c.h))
#endif

//-----------------------------------------------------
// C99 pragma wrapper
//-----------------------------------------------------

#ifdef COMPILER_XTENSA
#define __Pragma(a) _Pragma(a)
#else
#define __Pragma(a)
#endif

#define IS_ALIGN(p) ((((int)(p))&0x7) == 0) 

#ifdef _MSC_VER
    #define ALIGN(x)    _declspec(align(x)) 
#else
    #define ALIGN(x)    __attribute__((aligned(x))) 
#endif

#define INV_TBL_BITS 7
extern const int32_t tab_invQ30[128];

#if XCHAL_HAVE_NSA
  #define NSA(n) XT_NSA(n)
#else
  inline_ int32_t NSA(int32_t n)
  {
    ae_q56s t;
    if (!n) return 31;
    t = AE_CVTQ48A32S(n);
    return AE_NSAQ56S(t)-8;
  }
#endif

// special XCC type casting of pointers
#ifdef __cplusplus
#define castxcc(type_,ptr)  (ptr)
#else
#define castxcc(type_,ptr)  (type_ *)(ptr)
#endif

// return 64-bit data converting from ae_int64
#ifdef __cplusplus
#define return_int64(x) return vai;
#else
#define return_int64(x) {  union {ae_int64  ai;int64_t   i; } r; r.ai = vai;  return r.i; }
#endif

#if  defined (__cplusplus) || defined(COMPILER_XTENSA)

#else
#error sorry, C compiler is not supported excluding the XCC
#endif


#ifdef COMPILER_MSVC
#define MSC_ALIGNED ALIGN(8)
#define GCC_ALIGNED
#else
#define MSC_ALIGNED
#define GCC_ALIGNED ALIGN(8)
#endif

typedef const int32_t * cint32_ptr;
typedef const uint64_t * cuint64_ptr;
typedef const short * cint16_ptr;


//-----------------------------------------------------
// Conditionalization support
//-----------------------------------------------------
/* place DISCARD_FUN(retval_type,name) instead of function definition for functions
   to be discarded from the executable 
   THIS WORKS only for external library functions declared as extern "C" and
   not supported for internal references without "C" qualifier!
*/
#ifdef COMPILER_MSVC
#pragma section( "$DISCARDED_FUNCTIONS" , execute, discard )
#pragma section( "$$$$$$$$$$" , execute, discard )
#define DISCARD_FUN(retval_type,name,arglist) __pragma (alloc_text( "$DISCARDED_FUNCTIONS",name))\
__pragma(section( "$DISCARDED_FUNCTIONS" , execute, discard ))\
__pragma (warning(push))\
__pragma (warning( disable : 4026 4716))\
retval_type name arglist {}\
__pragma (warning(pop))
#endif

#if defined (COMPILER_GNU)
#define F_UNDERSCORE " "
#define DISCARD_FUN(retval_type,name,arglist)    \
__asm__                        \
(                              \
".section unused_section\n"    \
".globl " F_UNDERSCORE STRINGIZE(name) "\n" \
".type "F_UNDERSCORE STRINGIZE(name)", @function \n"\
F_UNDERSCORE STRINGIZE(name) ":\n"          \
".text"                        \
);
#endif

#if defined(COMPILER_XTENSA)
#define DISCARD_FUN(retval_type,name,arglist)  __asm__(".type "#name", @object\n\t.global "#name"\n\t.align 4\n\t"#name":\n\t.long 0x49438B96,0x4D73F192\n\t");
#endif 

#ifdef __cplusplus
#define externC extern "C" 
#else
#define externC extern 
#endif

/* maximum size (in bytes) allocated storage on stack by temporary arrays inside library functions */
#define MAX_ALLOCA_SZ 512

#define AE_MULSSFD24_HH_LL_S2 AE_MULSSFD24_HH_LL
#define AE_MULAF32R_LL_S2 AE_MULAF32R_LL

#if XCHAL_HAVE_FUSION_AVS==0 /* defined in core-isa.h */
  #define AE_MULAFP32X2RAS(d,d0,d1)\
  {                                \
    ae_f32x2 __dd;                       \
    ae_f64 __q0,__q1;                    \
    __q0=AE_MULF32S_HH(d0, d1);          \
    __q1=AE_MULF32S_LL(d0, d1);          \
    __dd=AE_ROUND32X2F64SASYM(__q0,__q1);\
    d=AE_ADD32S(d,__dd);                 \
  }

  #define AE_MULFP32X2RAS(d0,d1) __AE_MULFP32X2RAS(&d0,&d1)

  inline_ ae_f32x2 __AE_MULFP32X2RAS(const void* pd0,const void* pd1)
  {
      ae_f32x2 d,d0,d1;
      ae_f64 q0,q1;
      d0=*(ae_f32x2*)pd0;
      d1=*(ae_f32x2*)pd1;
      q0=AE_MULF32S_HH(d0, d1);     
      q1=AE_MULF32S_LL(d0, d1);     
      d=AE_ROUND32X2F64SASYM(q0,q1);
      return d;
  }
  #define AE_MULSFP32X2RAS(d,d0,d1)      \
  {                                      \
    ae_f32x2 __dd;                       \
    ae_f64 __q0,__q1;                    \
    __q0=AE_MULF32S_HH(d0, d1);          \
    __q1=AE_MULF32S_LL(d0, d1);          \
    __dd=AE_ROUND32X2F64SASYM(__q0,__q1);\
    d=AE_SUB32S(d,__dd);                 \
  }

#define AE_MULFP16X4RAS(x,y) __AE_MULFP16X4RAS(&x,&y)

  inline_ ae_f16x4 __AE_MULFP16X4RAS(const void* px,const void* py)
  {
    ae_f16x4 z,x,y;
    ae_f32x2 xh,xl,d0,d1;
    x=*(const ae_f16x4*)px;
    y=*(const ae_f16x4*)py;
    xh=AE_CVT32X2F16_32(x);          
    xl=AE_CVT32X2F16_10(x);         
    d0=AE_MULFP32X16X2RAS_H(xh,y);  
    d1=AE_MULFP32X16X2RAS_L(xl,y);  
    z=AE_ROUND16X4F32SASYM(d0,d1);
    return z;
  }

#define AE_MULFP16X4S_vector(x,y) AE_MULFP16X4S(x,y)

/* emulate AVS intrinsics */
#define AE_MULFD24X2_FIR_H(q0,q1,d0,d1,c)  \
{                                          \
    ae_f24x2 __d2;                         \
    q0=AE_MULZAAFD24_HH_LL(d0, c);         \
    __d2=AE_SEL24_LH(d0,d1);               \
    q1=AE_MULZAAFD24_HH_LL(__d2, c);       \
}

#define AE_MULAFD24X2_FIR_H(q0,q1,d0,d1,c) \
{                                          \
    ae_f24x2 __d2;                         \
    AE_MULAAFD24_HH_LL(q0, d0, c);         \
    __d2=AE_SEL24_LH(d0,d1);               \
    AE_MULAAFD24_HH_LL(q1, __d2, c);       \
}

#define AE_MULAFD32X16X2_FIR_HH(q0,q1,d0,d1,c) \
{                                              \
    ae_f32x2 __d2;                             \
    AE_MULAAFD32X16_H3_L2(q0,d0,c);            \
    __d2=AE_SEL32_LH(d0,d1);                   \
    AE_MULAAFD32X16_H3_L2(q1,__d2,c);          \
}

#define AE_MULAFD32X16X2_FIR_HL(q0,q1,d0,d1,c) \
{                                              \
    ae_f32x2 __d2;                             \
    AE_MULAAFD32X16_H1_L0(q0,d0,c);            \
    __d2=AE_SEL32_LH(d0,d1);                   \
    AE_MULAAFD32X16_H1_L0(q1,__d2,c);          \
}

#define AE_MULFD32X16X2_FIR_HH(q0,q1,d0,d1,c)  \
{                                              \
    ae_f32x2 __d2;                             \
    q0=AE_MULZAAFD32X16_H3_L2(d0,c);           \
    __d2=AE_SEL32_LH(d0,d1);                   \
    q1=AE_MULZAAFD32X16_H3_L2(__d2,c);         \
}

#define AE_MULFD32X16X2_FIR_HL(q0,q1,d0,d1,c) \
{                                             \
    ae_f32x2 __d2;                            \
    q0=AE_MULZAAFD32X16_H1_L0(d0,c);          \
    __d2=AE_SEL32_LH(d0,d1);                  \
    q1=AE_MULZAAFD32X16_H1_L0(__d2,c);        \
}

#define AE_MULAF16X4SS(d0,d1,d2,d3)      \
{                                        \
    ae_f32x2 wd2_h,wd2_l;                \
    wd2_h=AE_CVT32X2F16_32(d2);          \
    wd2_l=AE_CVT32X2F16_10(d2);;         \
    AE_MULAFP32X16X2RAS_H(d0,wd2_h,d3);  \
    AE_MULAFP32X16X2RAS_L(d1,wd2_l,d3);  \
}

#define AE_MULSF16X4SS(d0,d1,d2,d3)      \
{                                        \
    ae_f32x2 wd2_h,wd2_l;                \
    wd2_h=AE_CVT32X2F16_32(d2);          \
    wd2_l=AE_CVT32X2F16_10(d2);;         \
    AE_MULSFP32X16X2RAS_H(d0,wd2_h,d3);  \
    AE_MULSFP32X16X2RAS_L(d1,wd2_l,d3);  \
}

#define AE_MULFC24RA(d0,d1) __AE_MULFC24RA(&d0,&d1)

inline_ ae_f24x2 __AE_MULFC24RA(const void*  pd0,const void* pd1)
{
    ae_f64 q0,q1;
    ae_f24x2 d,d0,d1;
    d0=*(const ae_f24x2*)pd0;
    d1=*(const ae_f24x2*)pd1;
    /*
        d.re=d0.re*d1.re-d0.im*d1.im;
        d.im=d0.re*d1.im+d0.im*d1.re;
    */
     q0=AE_MULZASFD24_HH_LL(d0,d1);
     q1=AE_MULZAAFD24_HL_LH(d0,d1);
     d=AE_ROUND24X2F48SASYM(q0,q1);
     return d;
}

#define AE_MULC32X16_L(d0,d1) __AE_MULC32X16_L(&d0,&d1)

inline_ ae_int32x2 __AE_MULC32X16_L(const void * pd0, const void * pd1)
{
    ae_int32x2 d;
    ae_int64 q0,q1;
    ae_int32x2 d0;
    ae_int16x4 d1;
    d0=*(const ae_int32x2*)pd0;
    d1=*(const ae_int16x4*)pd1;
    q0=AE_MULZASD32X16_H1_L0(d0,d1);
    q1=AE_MULZAAD32X16_H0_L1(d0,d1);
    d=AE_SEL32_LL(AE_MOVINT32X2_FROMINT64(q0),AE_MOVINT32X2_FROMINT64(q1));
    return d;
}
#endif

#if 0 //XCHAL_HAVE_FUSION_AVS==0 /* defined in core-isa.h */

#define AE_MULSFP32X2RAS(d,d0,d1)      \
{                                      \
  ae_f32x2 __dd;                       \
  ae_f64 __q0,__q1;                    \
  __q0=AE_MULF32S_HH(d0, d1);          \
  __q1=AE_MULF32S_LL(d0, d1);          \
  __dd=AE_ROUND32X2F64SASYM(__q0,__q1);\
  d=AE_SUB32S(d,__dd);                 \
}

#define AE_MULAFP32X2RAS(d,d0,d1)\
{                                \
  ae_f32x2 __dd;                       \
  ae_f64 __q0,__q1;                    \
  __q0=AE_MULF32S_HH(d0, d1);          \
  __q1=AE_MULF32S_LL(d0, d1);          \
  __dd=AE_ROUND32X2F64SASYM(__q0,__q1);\
  d=AE_ADD32S(d,__dd);                 \
}

#define AE_MULFP32X2RAS(d0,d1) __AE_MULFP32X2RAS(&d0,&d1)

inline_ ae_f32x2 __AE_MULFP32X2RAS(const void* pd0,const void* pd1)
{
    ae_f32x2 d,d0,d1;
    ae_f64 q0,q1;
    d0=*(ae_f32x2*)pd0;
    d1=*(ae_f32x2*)pd1;
    q0=AE_MULF32S_HH(d0, d1);     
    q1=AE_MULF32S_LL(d0, d1);     
    d=AE_ROUND32X2F64SASYM(q0,q1);
    return d;
}

#ifndef AE_MULFC32X16RAS_L
#define AE_MULFC32X16RAS_L(d0,d1) __AE_MULFC32X16RAS_L(&d0,&d1)
#endif

#define AE_MULFC32X16RAS_H(d0,d1) __AE_MULFC32X16RAS_H(&d0,&d1)

inline_ ae_f32x2 __AE_MULFC32X16RAS_L(const void  * pd0,const void  * pd1)
{
    ae_f64 q0,q1;
    ae_f32x2 d;
    ae_f32x2 d0;
    ae_f16x4 d1;
    d0=*(const ae_f32x2 *)pd0;
    d1=*(const ae_f16x4 *)pd1;

    q0=AE_MULZASFD32X16_H1_L0(d0,d1);
    q1=AE_MULZAAFD32X16_H0_L1(d0,d1);
    d=AE_ROUND32X2F48SASYM(q0,q1);
    return d;
}

inline_ ae_f32x2 __AE_MULFC32X16RAS_H(const void   * pd0,const void  * pd1)
{
    ae_f64 q0,q1;
    ae_f32x2 d;
    ae_f32x2 d0;
    ae_f16x4 d1;
    d0=*(const ae_f32x2 *)pd0;
    d1=*(const ae_f16x4 *)pd1;

    q0=AE_MULZASFD32X16_H3_L2(d0,d1);
    q1=AE_MULZAAFD32X16_H2_L3(d0,d1);
    d=AE_ROUND32X2F48SASYM(q0,q1);
    return d;
}

#define AE_MULFP16X4RAS(x,y) __AE_MULFP16X4RAS(&x,&y)

inline_ ae_f16x4 __AE_MULFP16X4RAS(const void* px,const void* py)
{
    ae_f16x4 z,x,y;
    ae_f32x2 xh,xl,d0,d1;
    x=*(const ae_f16x4*)px;
    y=*(const ae_f16x4*)py;
    xh=AE_CVT32X2F16_32(x);          
    xl=AE_CVT32X2F16_10(x);         
    d0=AE_MULFP32X16X2RAS_H(xh,y);  
    d1=AE_MULFP32X16X2RAS_L(xl,y);  
    z=AE_ROUND16X4F32SASYM(d0,d1);
    return z;
}


#define AE_MULFP16X4S_vector(x,y) AE_MULFP16X4S(x,y)

#define AE_MUL16X4(y0,y1,x0,x1)   \
{                                 \
    y0=AE_SEXT32X2D16_32(x0);     \
    y1=AE_SEXT32X2D16_10(x0);     \
    y0=AE_MULP32X16X2_H(y0,x1);   \
    y1=AE_MULP32X16X2_L(y1,x1);   \
}

#endif
/* -----------------------------------------------------------------*/
/* redefine some xtfloat2 stuff - to be removed                     */
/* -----------------------------------------------------------------*/
#define XT_AE_MOVXTFLOAT_FROMINT32(x) XT_xtfloatx2_rtor_xtfloat( XT_AE_MOVXTFLOATX2_FROMINT32X2( AE_MOVINT32X2_FROMINT32( x ) ) )
#define XT_AE_MOVINT32_FROMXTFLOAT(x) AE_MOVINT32_FROMINT32X2( XT_AE_MOVINT32X2_FROMXTFLOATX2( XT_xtfloat_rtor_xtfloatx2( x ) ) );

#if 0
/* -----------------------------------------------------------------*/
/* redefine some xtfloat2 stuff - to be removed                     */
/* -----------------------------------------------------------------*/
#define XT_MADD_SX2(r,s,t)                         \
{                                                  \
    xtfloat rh,rl;                                 \
    rh=XT_HIGH_S(r);                               \
    rl=XT_LOW_S (r);                               \
    XT_MADD_LHH_S(rh,s,t);                         \
    XT_MADD_S    (rl,s ,t);                        \
    r=XT_SEL32_LL_SX2((xtfloatx2)rh,(xtfloatx2)rl);\
}

#define XT_MSUB_SX2(r,s,t)                         \
{                                                  \
    xtfloat rh,rl;                                 \
    rh=XT_HIGH_S(r);                               \
    rl=XT_LOW_S (r);                               \
    XT_MSUB_LHH_S(rh,s,t);                         \
    XT_MSUB_S    (rl,s ,t);                        \
    r=XT_SEL32_LL_SX2((xtfloatx2)rh,(xtfloatx2)rl);\
}

#define XT_MADDN_SX2(r,s,t)                        \
{                                                  \
    xtfloat rh,rl;                                 \
    rh=XT_HIGH_S(r);                               \
    rl=XT_LOW_S (r);                               \
    XT_MADDN_S(rh,XT_HIGH_S(s),XT_HIGH_S(t));      \
    XT_MADDN_S(rl,s ,t);       \
    r=XT_SEL32_LL_SX2((xtfloatx2)rh,(xtfloatx2)rl);\
}

#define XT_MSUBN_SX2(r,s,t)                        \
{                                                  \
    xtfloat rh,rl;                                 \
    rh=XT_HIGH_S(r);                               \
    rl=XT_LOW_S (r);                               \
    XT_MSUBN_S(rh,XT_HIGH_S(s),XT_HIGH_S(t));      \
    XT_MSUBN_S(rl,s ,t);                           \
    r=XT_SEL32_LL_SX2((xtfloatx2)rh,(xtfloatx2)rl);\
}

#define XT_ADD_SX2(b,c) __XT_ADD_SX2(&b,&c)
#define XT_SUB_SX2(b,c) __XT_SUB_SX2(&b,&c)
#define XT_MUL_SX2(b,c) __XT_MUL_SX2(&b,&c)
#define XT_MIN_SX2(b,c) __XT_MIN_SX2(&b,&c)
#define XT_MAX_SX2(b,c) __XT_MAX_SX2(&b,&c)

inline_ xtfloatx2 __XT_ADD_SX2(const void*  pb,const void*  pc)
{
    xtfloat ah,al;
    xtfloatx2 b,c;
    b=*(const xtfloatx2*)pb;
    c=*(const xtfloatx2*)pc;
    ah=XT_ADD_LHH_S (b,c);
    al=XT_ADD_S(XT_LOW_S (b),XT_LOW_S (c));
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}
inline_ xtfloatx2 __XT_SUB_SX2(const void*  pb,const void*  pc)
{
    xtfloat ah,al;
    xtfloatx2 b,c;
    b=*(const xtfloatx2*)pb;
    c=*(const xtfloatx2*)pc;
    ah=XT_SUB_LHH_S(b,c);
    al=XT_SUB_S(XT_LOW_S (b),XT_LOW_S (c));
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}
inline_ xtfloatx2 __XT_MUL_SX2(const void*  pb,const void*  pc)
{
    xtfloat ah,al;
    xtfloatx2 b,c;
    b=*(const xtfloatx2*)pb;
    c=*(const xtfloatx2*)pc;
    ah=XT_MUL_LHH_S(b,c);
    al=XT_MUL_S(XT_LOW_S (b),XT_LOW_S (c));
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}

#define XT_NEG_SX2(b) __XT_NEG_SX2(&b)
inline_ xtfloatx2 __XT_NEG_SX2(const void*  pb)
{
    xtfloat ah,al;
    xtfloatx2 b;
    b=*(const xtfloatx2*)pb;
    ah=XT_NEG_LH_S(b);
    al=XT_NEG_S   (b);
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}

#define XT_MAX_S(x,y) __XT_MAX_S(&x,&y)
#define XT_MIN_S(x,y) __XT_MIN_S(&x,&y)

inline_ xtfloat __XT_MAX_S(const void*  px,const void* py)
{
    xtbool c;
    xtfloat x,y;
    x=*(const xtfloat*)px;
    y=*(const xtfloat*)py;
    c=XT_OLT_S(y,x);
    XT_MOVT_S(y,x,c);
    return y;
}

inline_ xtfloat __XT_MIN_S(const void*  px,const void*  py)
{
    xtbool c;
    xtfloat x,y;
    x=*(const xtfloat*)px;
    y=*(const xtfloat*)py;
    c=XT_OLT_S(x,y);
    XT_MOVT_S(y,x,c);
    return y;
}

inline_ xtfloatx2 __XT_MIN_SX2(const void*  pb,const void*  pc)
{
    xtfloat ah,al,bh,bl,ch,cl;
    xtfloatx2 b,c;
    b=*(const xtfloatx2*)pb;
    c=*(const xtfloatx2*)pc;
    bh=XT_HIGH_S(b);
    bl=XT_LOW_S (b);
    ch=XT_HIGH_S(c);
    cl=XT_LOW_S (c);
    ah=XT_MIN_S(bh,ch);
    al=XT_MIN_S(bl,cl);
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}

inline_ xtfloatx2 __XT_MAX_SX2(const void*  pb,const void*  pc)
{
    xtfloat ah,al,bh,bl,ch,cl;
    xtfloatx2 b,c;
    b=*(const xtfloatx2*)pb;
    c=*(const xtfloatx2*)pc;
    bh=XT_HIGH_S(b);
    bl=XT_LOW_S (b);
    ch=XT_HIGH_S(c);
    cl=XT_LOW_S (c);
    ah=XT_MAX_S(bh,ch);
    al=XT_MAX_S(bl,cl);
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}

#define XT_AE_MOVXTFLOATX2_FROMF32X2(x) XT_AE_MOVXTFLOATX2_FROMINT32X2(AE_MOVF32X2_FROMINT32X2(x))

#define XT_AE_MOVINT32_FROMXTFLOAT(x) AE_MOVINT32_FROMINT32X2( XT_AE_MOVINT32X2_FROMXTFLOATX2( XT_xtfloat_rtor_xtfloatx2( x ) ) );
#define XT_AE_MOVXTFLOAT_FROMINT32(x) XT_xtfloatx2_rtor_xtfloat( XT_AE_MOVXTFLOATX2_FROMINT32X2( AE_MOVINT32X2_FROMINT32( x ) ) )

#define XT_MULC_S(x,y) __XT_MULC_S(&x,&y)
#define XT_MULCCONJ_S(x,y) __XT_MULCCONJ_S(&x,&y)

inline_ xtfloatx2 __XT_MULC_S(const void*  px,const void*  py)
{
    xtfloatx2 z,x,y;
    xtfloat zre,zim;
    x=*(const xtfloatx2*)px;
    y=*(const xtfloatx2*)py;
    zre=XT_MUL_LHH_S(x,y);
    XT_MSUB_S(zre,x,y);
    zim=XT_MUL_LLH_S(y,x);
    XT_MADD_LLH_S(zim,x,y);
    z=XT_SEL32_LL_SX2((xtfloatx2)zre,(xtfloatx2)zim);
    return z;
}

inline_ xtfloatx2 __XT_MULCCONJ_S(const void*  px,const void*  py)
{
    xtfloatx2 z,x,y;
    xtfloat zre,zim;
    x=*(const xtfloatx2*)px;
    y=*(const xtfloatx2*)py;
    zre=XT_MUL_LHH_S(x,y);
    XT_MADD_S(zre,x,y);
    zim=XT_MUL_LLH_S(x,y);
    XT_MSUB_LLH_S(zim,y,x);
    z=XT_SEL32_LL_SX2((xtfloatx2)zre,(xtfloatx2)zim);
    return z;
}

#define XT_MADDMUX_S(a,b,c,d)                                             \
{                                                                         \
    xtfloat are,aim;                                                      \
    are=XT_HIGH_S(a);                                                     \
    aim=XT_LOW_S (a);                                                     \
    switch(d)                                                             \
    {                                                                     \
    /* MADDMUX.S a, b, c, 0;  */                                          \
    /* MADDMUX.S a, b, c, 1;  */                                          \
    /* bre*cre-bim*cim        */                                          \
    /* bim*cre+bre*cim        */                                          \
    case 0: XT_MADD_LHH_S(are,b,c); XT_MADD_LLH_S(aim,c,b); break;        \
    case 1: XT_MSUB_S    (are,b,c); XT_MADD_LLH_S(aim,b,c); break;        \
    /* MADDMUX.S a, b, c, 2;  */                                          \
    /* MADDMUX.S a, b, c, 3;  */                                          \
    /* -bre*cre+bim*cim       */                                          \
    /* -bim*cre-bre*cim       */                                          \
    case 2:XT_MSUB_LHH_S(are,b,c); XT_MSUB_LLH_S(aim,c,b); break;         \
    case 3:XT_MADD_S    (are,b,c); XT_MSUB_LLH_S(aim,b,c); break;         \
    /* MADDMUX.S a, b, c, 4;  */                                          \
    /* MADDMUX.S a, b, c, 5;  */                                          \
    /* bre*cre+bim*cim        */                                          \
    /* bim*cre-bre*cim        */                                          \
    case 4:XT_MADD_LHH_S(are,b,c);XT_MSUB_LLH_S(aim,c,b); break;          \
    case 5:XT_MADD_S    (are,b,c);XT_MADD_LLH_S(aim,b,c); break;          \
    /* MADDMUX.S a, b, c, 6;  */                                          \
    /* MADDMUX.S a, b, c, 7;  */                                          \
    /* -bre*cre-bim*cim        */                                         \
    /* -bim*cre+bre*cim        */                                         \
    case 6:XT_MSUB_LHH_S(are,b,c); XT_MADD_LLH_S(aim,c,b);  break;        \
    case 7:XT_MSUB_S    (are,b,c); XT_MSUB_LLH_S(aim,b,c);  break;        \
    }                                                                     \
    a=XT_SEL32_LL_SX2((xtfloatx2)are,(xtfloatx2)aim);                     \
}


#define XT_MADDCCONJ_S(z,x,y)        \
{                                    \
    xtfloat                 zre,zim; \
    zre=XT_HIGH_S(z);                \
    zim=XT_LOW_S (z);                \
    XT_MADD_LHH_S(zre,x,y);          \
    XT_MADD_S    (zre,x,y);          \
    XT_MADD_LLH_S(zim,x,y);          \
    XT_MSUB_LLH_S(zim,y,x);          \
    z=XT_SEL32_LL_SX2((xtfloatx2)zre,(xtfloatx2)zim);      \
}

#define XT_RMAX_S(x) __XT_RMAX_S(&x)

inline_ xtfloat __XT_RMAX_S(const void*  px)
{
    xtbool c;
    xtfloatx2 x;
    xtfloat xhi,xlo;
    x=*(const xtfloatx2*)px;
    xhi=XT_HIGH_S(x);
    xlo=XT_LOW_S(x);
    c=XT_OLT_S(xlo,xhi);
    XT_MOVT_S(xlo,xhi,c);
    return xlo;
}


#define XT_RMIN_S(x) __XT_RMIN_S(&x)

inline_ xtfloat __XT_RMIN_S(const void*  px)
{
    xtbool c;
    xtfloatx2 x;
    xtfloat xhi,xlo;
    x=*(const xtfloatx2*)px;
    xhi=XT_HIGH_S(x);
    xlo=XT_LOW_S(x);
    c=XT_OLT_S(xhi,xlo);
    XT_MOVT_S(xlo,xhi,c);
    return xlo;
}

#define XT_MADDC_S(z,x,y)                              \
{                                                      \
    xtfloat                 zre,zim;                   \
    zre=XT_HIGH_S(z);                                  \
    zim=XT_LOW_S (z);                                  \
    XT_MADD_LHH_S(zre,x,y);                            \
    XT_MSUB_S    (zre,x,y);                            \
    XT_MADD_LLH_S(zim,y,x);                            \
    XT_MADD_LLH_S(zim,x,y);                            \
    z=XT_SEL32_LL_SX2((xtfloatx2)zre,(xtfloatx2)zim);  \
}

#define XT_RADD_SX2(x) XT_ADD_LLH_S((x),(x))

#define XT_RECIP_SX2(b) __XT_RECIP_SX2(&b)
inline_ xtfloatx2 __XT_RECIP_SX2(const void*  pb)
{
    xtfloat ah,al;
    xtfloatx2 b;
    b=*(const xtfloatx2*)pb;
    ah=XT_RECIP_S(XT_HIGH_S(b));
    al=XT_RECIP_S(XT_LOW_S (b));
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}

#define XT_ABS_SX2(b) __XT_ABS_SX2(&b)
inline_ xtfloatx2 __XT_ABS_SX2(const void*  pb)
{
    xtfloat ah,al;
    xtfloatx2 b;
    b=*(const xtfloatx2*)pb;
    ah=XT_ABS_S(XT_HIGH_S(b));
    al=XT_ABS_S(XT_LOW_S (b));
    return XT_SEL32_LL_SX2((xtfloatx2)ah,(xtfloatx2)al);
}
#endif
#endif
