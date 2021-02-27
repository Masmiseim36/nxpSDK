/* ------------------------------------------------------------------------ */
/* Copyright (c) 2016 by Cadence Design Systems, Inc. ALL RIGHTS RESERVED.  */
/* These coded instructions, statements, and computer programs (“Cadence    */
/* Libraries”) are the copyrighted works of Cadence Design Systems Inc.	    */
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
#include "scl_atan_table.h"
#include "common.h"
#ifdef COMPILER_MSVC
const int32_t _declspec(align(8)) atan_table[] =
#else
const int32_t                     atan_table[] __attribute__((aligned(8))) =
#endif
{
0        ,10680490 ,21359677 ,32036257 ,42708931 ,53376400 ,64037370 ,74690552 ,
85334662 ,95968424 ,106590567,117199831,127794963,138374723,148937878,159483210,
170009512,180515592,191000270,201462383,211900781,222314333,232701923,243062454,
253394845,263698036,273970985,284212671,294422091,304598264,314740231,324847054,
334917815,344951620,354947598,364904900,374822699,384700192,394536599,404331165,
414083157,423791866,433456607,443076718,452651562,462180525,471663017,481098471,
490486344,499826117,509117294,518359400,527551986,536694625,545786911,554828461,
563818914,572757932,581645197,590480413,599263303,607993613,616671107,625295572,
633866811,642384648,650848926,659259505,667616264,675919099,684167925,692362671,
700503285,708589730,716621985,724600043,732523913,740393619,748209197,755970700,
763678190,771331745,778931454,786477419,793969754,801408581,808794038,816126269,
823405431,830631690,837805221,844926207,851994843,859011330,865975876,872888700,
879750026,886560084,893319114,900027360,906685073,913292509,919849932,926357607,
932815808,939224811,945584899,951896356,958159473,964374543,970541862,976661731,
982734454 ,988760334 ,994739682 ,1000672808,1006560025,1012401647,1018197992,1023949379,
1029656126,1035318555,1040936988,1046511749,1052043161,1057531550,1062977239,1068380555,
1073741824,1079061371,1084339523
};
