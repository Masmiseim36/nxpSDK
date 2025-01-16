/*
 * Copyright (c) 2008-2022 Cadence Design Systems, Inc.
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


#ifndef __XA_BSAC_DEC_COMMON_H__
#define __XA_BSAC_DEC_COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <assert.h>
#include <sys/times.h>

#ifdef __XTENSA__
#include <xtensa/tie/xt_hifi2.h>
#ifdef __XCC__
#include <xtensa/hal.h>
#endif
#endif

#include "xa_type_def.h"
#include "xa_memory_standards.h"
#include "xa_error_handler.h"
#include "xa_apicmd_standards.h"
#include "xa_error_standards.h"

/* Codec specific macros */

#define MAX_CMD_LINE_LENGTH 300
#define MAX_MEM_ALLOCS      100
#define XA_MAX_ARGS         300
#define XA_SCREEN_WIDTH     80

#ifndef PARAMFILE
#define PARAMFILE "paramfilesimple.txt"
#endif

#ifdef PROFILE
    #ifndef XT_ISS_MEM_BANDWIDTH_PROFILER
    #define XT_ISS_MEM_BANDWIDTH_PROFILER
    #endif
#else
    #ifdef XT_ISS_MEM_BANDWIDTH_PROFILER
    #undef XT_ISS_MEM_BANDWIDTH_PROFILER
    #endif
#endif

#ifdef INLOOP_STACK_MEASUREMENT // TODO
    #ifdef XT_ISS_MEM_BANDWIDTH_PROFILER
    #error "You can't enabled ISS profiler and Stack measurement simultaneiously(Remove this error after testing both\n"
    #endif
#endif


#define API_CALL(cmd, idx, pvalue, context_str) \
    err_code = (*(p_xa_process_api))(p_xa_process_api_obj, (cmd), (idx), (pvalue));\
    _XA_HANDLE_ERROR(p_proc_err_info, (context_str), err_code);

#define SET_CONFIG(idx, pvalue, context_str) API_CALL( XA_API_CMD_SET_CONFIG_PARAM,idx, pvalue, context_str)
#define GET_CONFIG(idx, pvalue, context_str) API_CALL( XA_API_CMD_GET_CONFIG_PARAM,idx, pvalue, context_str)

#define TESTBENCH_ERROR(err, context_str) _XA_HANDLE_ERROR(p_proc_err_info, (context_str),  (err));

#define RETURN_ON_FATAL_ERROR(fn) {XA_ERRORCODE err = (fn); if(err & XA_FATAL_ERROR) return err;}


#define xa_class_mfman 0
#define XA_TESTBENCH	2

enum xa_error_fatal_testbench 
{
    XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED   = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 0),
    XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED   = XA_ERROR_CODE(xa_severity_fatal, xa_class_mfman, XA_TESTBENCH, 1)
};

///////////////////////////////////////////////////////////////////////////////
/// Profiler
#ifndef __XTENSA__
#undef PROFILE
#endif
#ifdef PROFILE
#ifdef XT_ISS_MEM_BANDWIDTH_PROFILER
#include <xtensa/sim.h>
#define XT_ISS_MEM_BANDWIDTH_PROFILER_START {xt_iss_client_command("all", "enable");}
#define XT_ISS_MEM_BANDWIDTH_PROFILER_STOP  {xt_iss_client_command("all", "disable");}
#else
#define XT_ISS_MEM_BANDWIDTH_PROFILER_START
#define XT_ISS_MEM_BANDWIDTH_PROFILER_STOP
#endif
#define PROFILER_OPEN(prof, iname ) {          \
         (prof)->cycles = (prof)->exclude_cycles = 0; \
         (prof)->frame_cnt = (prof)->peak_frame = 0;  \
         (prof)->curr = (prof)->sum  = (prof)->ave = (prof)->peak = 0.0; \
         (prof)->start.tms_utime = 0;  \
         (prof)->stop.tms_utime = 0; \
         strcpy((prof)->name , iname); \
     }

#define PROFILER_START(prof) {     \
          SCRATCH_TRASH\
		  OUTPUT_BUF_PING_PONG\
          times(&(prof)->start);         \
          XT_ISS_MEM_BANDWIDTH_PROFILER_START \
     }

#define PROFILER_STOP(prof) {     \
         XT_ISS_MEM_BANDWIDTH_PROFILER_STOP \
		 INPUT_BUF_PING_PONG\
         times(&(prof)->stop);           \
         (prof)->cycles += (prof)->stop.tms_utime - (prof)->start.tms_utime; \
         (prof)->start.tms_utime = 0;  \
         (prof)->stop.tms_utime = 0;   \
     }

#define PROFILE_PRINT(prof) \
        printf( "[%d|%u] MCPS: %.2f Average: %.2f Peak: %.2f @ [%d] %s\n",\
        (prof)->frame_cnt-1, (prof)->streampos, (prof)->curr, (prof)->ave, (prof)->peak, (prof)->peak_frame, (prof)->name);

#define PROFILER_UPDATE( prof, samples, samplerate ) {                         \
             (prof)->cycles = (prof)->cycles - (prof)->exclude_cycles;         \
             (prof)->curr = ( (double)(prof)->cycles / samples ) ;             \
             (prof)->curr = (prof)->curr * ( (double) samplerate / 1000000.0); \
             (prof)->sum += (prof)->curr;                                      \
             (prof)->ave = (prof)->sum / ((prof)->frame_cnt+1);                \
             if ((prof)->peak < (prof)->curr)                                  \
             {                                                                 \
                 (prof)->peak = (prof)->curr;                                  \
                 (prof)->peak_frame = (prof)->frame_cnt;                       \
             }                                                                 \
             (prof)->cycles = (prof)->exclude_cycles = 0;                      \
             (prof)->frame_cnt += 1;\
             PROFILE_PRINT(prof)\
         }

#define PROFILER_CLOSE(prof)    \
    printf( "%s:\n"                , (prof)->name);       \
    printf( "\tPeak MCPS    = %f\n", (prof)->peak);       \
    printf( "\tPeak frame   = %d\n", (prof)->peak_frame); \
    printf( "\tAverage MCPS = %f\n", (prof)->ave);        \

// TODO
#define PROFILER_EXCLUDE_ON( pProfiler ) \
          times(&(prof)->exclude_start);         \
          XT_ISS_MEM_BANDWIDTH_PROFILER_STOP \

#define PROFILER_EXCLUDE_OFF( pProfiler )  {     \
         times(&(prof)->exclude_stop);           \
         (prof)->exclude_cycles += (prof)->exclude_stop.tms_utime - (prof)->exclude_start.tms_utime; \
         (prof)->exclude_start.tms_utime = 0;  \
         (prof)->exclude_stop.tms_utime = 0;   \
         XT_ISS_MEM_BANDWIDTH_PROFILER_START \
     }

typedef struct _profiler_t
    {
         char name[20];

         clock_t cycles;
         struct tms start;
         struct tms stop;

         int frame_cnt, peak_frame;
         double curr, sum, ave, peak;

         int streampos;

         clock_t exclude_cycles;
         clock_t exclude_start;
         clock_t exclude_stop;
    } profiler_t;


#else /* PROFILE */

typedef struct _profiler_t
{
    char dummy;
} profiler_t;

#define PROFILER_OPEN(prof, iname )
#define PROFILER_START( pProfiler )
#define PROFILER_STOP( pProfiler )
#define PROFILER_UPDATE( prof, samples, samplerate )
#define PROFILER_CLOSE(prof)

#define PROFILER_EXCLUDE_ON( pProfiler )
#define PROFILER_EXCLUDE_ON( pProfiler )

#endif /* PROFILE */
#ifdef PROF_ALLOCATE  //define in only one file
profiler_t gProfiler[8];
#else
extern profiler_t gProfiler[8];
#endif
///////////////////////////////////////////////////////////////////////////////
/// Scratch trash test
#ifdef SCRATCH_TRASH_TEST
#define	SCRATCH_TRASH_TEST_ALLOCATE		\
	int scratch_trash_size;				\
	char *scratch_trash_ptr_ping;		\
	char scratch_trash_pattern;			\
	unsigned short scratch_trash_lfsr;	

#define	SCRATCH_TRASH_TEST_ASSIGN							\
	if(ui_type == XA_MEMTYPE_SCRATCH)						\
	{														\
		scratch_trash_size = ui_size;						\
		scratch_trash_ptr_ping = (char *)pv_alloc_ptr;		\
	}
	
/* lfsr taps: 16 14 13 11; characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
#define SCRATCH_TRASH \
    {\
          extern int scratch_trash_size;\
          extern char *scratch_trash_ptr_ping;\
          extern char scratch_trash_pattern;\
          extern unsigned short scratch_trash_lfsr;\
          scratch_trash_lfsr = (scratch_trash_lfsr >> 1) ^ (-(scratch_trash_lfsr & 1u) & 0xB400u);\
          scratch_trash_pattern = (char)scratch_trash_lfsr;\
          memset(scratch_trash_ptr_ping, scratch_trash_pattern, scratch_trash_size);\
   }
#else
#define	SCRATCH_TRASH_TEST_ALLOCATE
#define	SCRATCH_TRASH_TEST_ASSIGN
#define SCRATCH_TRASH
#endif

///////////////////////////////////////////////////////////////////////////////
/// Output Ping Pong
#ifdef OUTPUT_BUF_PING_PONG_TEST
#define	OUTPUT_BUF_PING_PONG_TEST_ALLOCATE		\
    int output_buf_pp_count = 0;				\
	int output_buf_pp_size;						\
	char *output_buf_pp_ptr_ping;				\
	char *output_buf_pp_ptr_pong;				\
	char output_buf_pp_pattern;					\
	unsigned short output_buf_pp_lfsr;	

#define	OUTPUT_BUF_PING_PONG_TEST_ASSIGN					\
	if(ui_type == XA_MEMTYPE_OUTPUT)						\
	{														\
		output_buf_pp_size = ui_size;						\
		output_buf_pp_ptr_ping = (char *)pv_alloc_ptr;		\
        pv_alloc_ptr = MEM_malloc(ui_size, ui_alignment);	\
		output_buf_pp_ptr_pong = (char *)pv_alloc_ptr;		\
	}

/* lfsr taps: 16 14 13 11; characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */
#define OUTPUT_BUF_PING_PONG \
    {\
          extern int output_buf_pp_count;\
          extern int output_buf_pp_size;\
          extern char *output_buf_pp_ptr_ping;\
          extern char *output_buf_pp_ptr_pong;\
          extern char output_buf_pp_pattern;\
          extern unsigned short output_buf_pp_lfsr;\
          output_buf_pp_lfsr = (output_buf_pp_lfsr >> 1) ^ (-(output_buf_pp_lfsr & 1u) & 0xB400u);\
          output_buf_pp_pattern = (char)output_buf_pp_lfsr;\
          if(output_buf_pp_count++ & 1)	\
          {\
            memset(output_buf_pp_ptr_ping, output_buf_pp_pattern, output_buf_pp_size);\
            API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_OUTPUT, output_buf_pp_ptr_ping, "Error in set mem-ptr");\
            pb_out_buf = (signed char *)output_buf_pp_ptr_ping;\
          }\
          else\
          {\
            memset(output_buf_pp_ptr_pong, output_buf_pp_pattern, output_buf_pp_size);\
            API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_OUTPUT, output_buf_pp_ptr_pong, "Error in set mem-ptr");\
            pb_out_buf = (signed char *)output_buf_pp_ptr_pong;\
          }\
   }
#else
#define	OUTPUT_BUF_PING_PONG_TEST_ALLOCATE
#define	OUTPUT_BUF_PING_PONG_TEST_ASSIGN
#define OUTPUT_BUF_PING_PONG
#endif
///////////////////////////////////////////////////////////////////////////////
/// Input Ping Pong
#ifdef INPUT_BUF_PING_PONG_TEST
#define	INPUT_BUF_PING_PONG_TEST_ALLOCATE		\
	int input_buf_pp_size;						\
	char *input_buf_pp_ptr_ping;				\
	char *input_buf_pp_ptr_pong;				

#define	INPUT_BUF_PING_PONG_TEST_ASSIGN					\
	if(ui_type == XA_MEMTYPE_INPUT)						\
	{														\
		input_buf_pp_size = ui_size;						\
		input_buf_pp_ptr_ping = (char *)pv_alloc_ptr;		\
        pv_alloc_ptr = MEM_malloc(ui_size, ui_alignment);	\
		input_buf_pp_ptr_pong = (char *)pv_alloc_ptr;		\
	}
#define INPUT_BUF_PING_PONG \
        {\
              extern int input_buf_pp_size;\
              extern char *input_buf_pp_ptr_ping;\
              extern char *input_buf_pp_ptr_pong;\
			  if((frameCounter & 1))	\
			  {\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_INPUT, input_buf_pp_ptr_ping, "Error in set mem-ptr");\
                memcpy(input_buf_pp_ptr_ping, pb_inp_buf, i_buff_size);\
				pb_inp_buf = (signed char *)input_buf_pp_ptr_ping;\
			  }\
			  else\
			  {\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_INPUT, input_buf_pp_ptr_pong, "Error in set mem-ptr");\
                memcpy(input_buf_pp_ptr_pong, pb_inp_buf, i_buff_size);\
				pb_inp_buf = (signed char *)input_buf_pp_ptr_pong;\
			  }\
       }
#else
#define	INPUT_BUF_PING_PONG_TEST_ALLOCATE
#define	INPUT_BUF_PING_PONG_TEST_ASSIGN
#define INPUT_BUF_PING_PONG
#endif

void MEM_init(void);
void *MEM_malloc (int size, int align);
void MEM_freeall(void);


///////////////////////////////////////////////////////////////////////////////

#ifdef ENABLE_FUNCTION_WRAPPER

#ifdef DECLARE_HERE_WRAPPERTAB
int mytab = 0;
#else
extern int mytab;
#endif

#define TAB(MYTAB) {                      \
            int i;                                          \
            for(i=0;i<MYTAB;i++) {printf( "\t"); }  \
            }

#define ENTERING_FN(MyFnName) {                      \
            int i;                                          \
            int count = mytab++;                            \
            for(i=0;i<count;i++) {printf( "\t"); }  \
            printf( "%d: Enterring %s",__LINE__, MyFnName);      \
            printf( "\n");                          \
            }

#define RETRNING_FN(MyFnName, MYERR) {               \
            int i;                                          \
            int count = --mytab;                            \
            for(i=0;i<count;i++) {printf( "\t"); }  \
            printf( "%d: Returning %s (err: %d)", __LINE__, MyFnName, MYERR);      \
            printf( "\n");                          \
            }
#define RETRNING_FNvoid(MyFnName) {               \
            int i;                                          \
            int count = --mytab;                            \
            for(i=0;i<count;i++) {printf( "\t"); }  \
            printf( "%d: Returning %s", __LINE__, MyFnName);      \
            printf( "\n");                          \
            }

#else
#define TAB() 

#define ENTERING_FN(MyFnName) {                      \
            }

#define RETRNING_FN(MyFnName, MYERR) {               \
            }
#define RETRNING_FNvoid(MyFnName, MYTAB) {               \
            }

#endif


#endif //__XA_BSAC_DEC_COMMON_H__


