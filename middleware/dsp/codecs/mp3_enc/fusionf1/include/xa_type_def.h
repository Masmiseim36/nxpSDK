/*
 * Copyright (c) 2022 Cadence Design Systems, Inc.
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */



#ifndef  __TYPEDEFTEST_H__
#define  __TYPEDEFTEST_H__

#include <xtensa/tie/xt_hifi2.h>

/****************************************************************************/
/*     types               type define    prefix        examples      bytes */
/************************  ***********    ******    ****************  ***** */
typedef signed char             WORD8   ;/* b       WORD8    b_name     1   */
typedef signed char         *   pWORD8  ;/* pb      pWORD8   pb_nmae    1   */
typedef unsigned char           UWORD8  ;/* ub      UWORD8   ub_count   1   */
typedef unsigned char       *   pUWORD8 ;/* pub     pUWORD8  pub_count  1   */

typedef signed short            WORD16  ;/* s       WORD16   s_count    2   */
typedef signed short        *   pWORD16 ;/* ps      pWORD16  ps_count   2   */
typedef unsigned short          UWORD16 ;/* us      UWORD16  us_count   2   */
typedef unsigned short      *   pUWORD16;/* pus     pUWORD16 pus_count  2   */

typedef signed int              WORD24  ;/* k       WORD24   k_count    3   */
typedef signed int          *   pWORD24 ;/* pk      pWORD24  pk_count   3   */
typedef unsigned int            UWORD24 ;/* uk      UWORD24  uk_count   3   */
typedef unsigned int        *   pUWORD24;/* puk     pUWORD24 puk_count  3   */

typedef signed int              WORD32  ;/* i       WORD32   i_count    4   */
typedef signed int          *   pWORD32 ;/* pi      pWORD32  pi_count   4   */
typedef unsigned int            UWORD32 ;/* ui      UWORD32  ui_count   4   */
typedef unsigned int        *   pUWORD32;/* pui     pUWORD32 pui_count  4   */

typedef signed long long        WORD40  ;/* m       WORD40   m_count    5   */
typedef signed long long    *   pWORD40 ;/* pm      pWORD40  pm_count   5   */
typedef unsigned long long      UWORD40 ;/* um      UWORD40  um_count   5   */
typedef unsigned long long  *   pUWORD40;/* pum     pUWORD40 pum_count  5   */

typedef signed long long        WORD64  ;/* h       WORD64   h_count    8   */
typedef signed long long    *   pWORD64 ;/* ph      pWORD64  ph_count   8   */
typedef unsigned long long      UWORD64 ;/* uh      UWORD64  uh_count   8   */
typedef unsigned long long  *   pUWORD64;/* puh     pUWORD64 puh_count  8   */

typedef float                   FLOAT32 ;/* f       FLOAT32  f_count    4   */
typedef float               *   pFLOAT32;/* pf      pFLOAT32 pf_count   4   */
typedef double                  FLOAT64 ;/* d       FLOAT64  d_count    8   */
typedef double              *   pFLOAT64;/* pd      pFLOAT64 pd_count   8   */

typedef void                    VOID    ;/* v       VOID     v_flag     4   */
typedef void                *   pVOID   ;/* pv      pVOID    pv_flag    4   */

/* variable size types: platform optimized implementation */
typedef signed int              BOOL    ;/* bool    BOOL     bool_true      */
typedef unsigned int            UBOOL   ;/* ubool   UBOOL    ubool_true     */
typedef signed int              FLAG    ;/* flag    FLAG     flag_false     */
typedef unsigned int            UFLAG   ;/* uflag   UFLAG    uflag_false    */
typedef signed int              LOOPIDX ;/* lp      LOOPIDX  lp_index       */
typedef unsigned int            ULOOPIDX;/* ulp     ULOOPIDX ulp_index      */
typedef signed int              WORD    ;/* g       WORD     g_count        */
typedef unsigned int            UWORD   ;/* ug      UWORD    ug_count       */

typedef LOOPIDX                 LOOPINDEX; /* lp    LOOPINDEX  lp_index       */
typedef ULOOPIDX                ULOOPINDEX;/* ulp   ULOOPINDEX ulp_index      */

#define PLATFORM_INLINE __inline

typedef struct xa_codec_opaque { } *xa_codec_handle_t;

typedef WORD32 XA_ERRORCODE;

typedef XA_ERRORCODE xa_codec_func_t(xa_codec_handle_t p_xa_module_obj,
				     WORD32            i_cmd,
				     WORD32            i_idx,
				     pVOID             pv_value);

#endif /* __TYPEDEFTEST_H__ */
