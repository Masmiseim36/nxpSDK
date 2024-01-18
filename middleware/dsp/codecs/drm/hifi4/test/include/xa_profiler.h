/*
 * Copyright (c) 2014-2022 Cadence Design Systems, Inc.
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


#ifndef __XA_PROFILER_H__
#define __XA_PROFILER_H__

#ifdef PROFILE

#include <sys/times.h>

#ifndef XT_ISS_MEM_BANDWIDTH_PROFILER
#define XT_ISS_MEM_BANDWIDTH_PROFILER
/* Don't enable this macro, the feature is incomplete and under investigation */
// #define INLOOP_STACK_MEASUREMENT
#endif

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

#ifdef INLOOP_STACK_MEASUREMENT
     int stack_cur;
     int stack_max;
#endif // INLOOP_STACK_MEASUREMENT
} profiler_t;


#ifdef XT_ISS_MEM_BANDWIDTH_PROFILER
#include <xtensa/sim.h>
#define XT_ISS_MEM_BANDWIDTH_PROFILER_START {xt_iss_client_command("all", "enable");}
#define XT_ISS_MEM_BANDWIDTH_PROFILER_STOP  {xt_iss_client_command("all", "disable");}
#else
#define XT_ISS_MEM_BANDWIDTH_PROFILER_START
#define XT_ISS_MEM_BANDWIDTH_PROFILER_STOP
#endif

#ifdef INLOOP_STACK_MEASUREMENT
// TODO
#ifdef XT_ISS_MEM_BANDWIDTH_PROFILER
#error "You can't enabled ISS profiler and Stack measurement simultaneiously(Remove this error after testing both\n"
#endif
// Value should not exceed 0x10000, if not, chance the prefix pattern
#define STACK_MEASURE_PEAK            0x00010000
#define STACK_MEASURE_PREFIX_PATTERN  0xDEAD0000
void stack_populate(void);
int stack_detect(void);

#define STACK_MEASURE_OPEN(prof) \
         (prof)->stack_cur = (prof)->stack_max = 0; \

#define STACK_MEASURE_BEGIN(prof) \
        stack_populate();

#define STACK_MEASURE_END(prof) \
    (prof)->stack_cur = stack_detect();\
    if((prof)->stack_cur > (prof)->stack_max) {\
        (prof)->stack_max = (prof)->stack_cur; }

#define STACK_MEASURE_CLOSE(prof) \
    fprintf(stderr,"\t%s : Stack Usage  = %d\n",(prof)->name,  (prof)->stack_max);  \

#else

#define STACK_MEASURE_OPEN(prof)
#define STACK_MEASURE_BEGIN(prof)
#define STACK_MEASURE_END(prof)
#define STACK_MEASURE_CLOSE(prof)

#endif
#ifdef SCRATCH_TRASH_TEST
#define SCRATCH_TRASH \
        {\
              extern int scratch_trash_count;\
              extern int scratch_trash_size;\
              extern char *scratch_trash_ptr_ping;\
              extern char *scratch_trash_ptr_pong;\
              extern char scratch_trash_pattern;\
              extern unsigned short scratch_trash_lfsr;\
              /* lfsr taps: 16 14 13 11; characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */\
              scratch_trash_lfsr = (scratch_trash_lfsr >> 1) ^ (-(scratch_trash_lfsr & 1u) & 0xB400u);\
              scratch_trash_pattern = (char)scratch_trash_lfsr;\
			  if(scratch_trash_count++ & 1)	\
			  {\
				memset(scratch_trash_ptr_ping, scratch_trash_pattern, scratch_trash_size);\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_SCRATCH, scratch_trash_ptr_ping, "Error in set mem-ptr");\
			  }\
			  else\
			  {\
				memset(scratch_trash_ptr_pong, scratch_trash_pattern, scratch_trash_size);\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_SCRATCH, scratch_trash_ptr_pong, "Error in set mem-ptr");\
			  }\
       }
#else
#define SCRATCH_TRASH
#endif

#ifdef OUTPUT_BUF_PING_PONG_TEST
#define OUTPUT_BUF_PING_PONG \
        {\
              extern int output_buf_pp_count;\
              extern int output_buf_pp_size;\
              extern char *output_buf_pp_ptr_ping;\
              extern char *output_buf_pp_ptr_pong;\
              extern char output_buf_pp_pattern;\
              extern unsigned short output_buf_pp_lfsr;\
              /* lfsr taps: 16 14 13 11; characteristic polynomial: x^16 + x^14 + x^13 + x^11 + 1 */\
              output_buf_pp_lfsr = (output_buf_pp_lfsr >> 1) ^ (-(output_buf_pp_lfsr & 1u) & 0xB400u);\
              output_buf_pp_pattern = (char)output_buf_pp_lfsr;\
			  if(output_buf_pp_count++ & 1)	\
			  {\
				memset(output_buf_pp_ptr_ping, output_buf_pp_pattern, output_buf_pp_size);\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_OUTPUT, output_buf_pp_ptr_ping, "Error in set mem-ptr");\
				pobj->outbuf.base_ptr = (signed char *)output_buf_pp_ptr_ping;\
			  }\
			  else\
			  {\
				memset(output_buf_pp_ptr_pong, output_buf_pp_pattern, output_buf_pp_size);\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_OUTPUT, output_buf_pp_ptr_pong, "Error in set mem-ptr");\
				pobj->outbuf.base_ptr = (signed char *)output_buf_pp_ptr_pong;\
			  }\
       }
#else
#define OUTPUT_BUF_PING_PONG
#endif

#ifdef INPUT_BUF_PING_PONG_TEST
#define INPUT_BUF_PING_PONG \
        {\
              extern int input_buf_pp_size;\
              extern char *input_buf_pp_ptr_ping;\
              extern char *input_buf_pp_ptr_pong;\
			  if((pobj->frameCounter & 1))	\
			  {\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_INPUT, input_buf_pp_ptr_ping, "Error in set mem-ptr");\
				pobj->inpbuf.base_ptr = (signed char *)input_buf_pp_ptr_ping;\
			  }\
			  else\
			  {\
				API_CALL( XA_API_CMD_SET_MEM_PTR, XA_MEMTYPE_INPUT, input_buf_pp_ptr_pong, "Error in set mem-ptr");\
				pobj->inpbuf.base_ptr = (signed char *)input_buf_pp_ptr_pong;\
			  }\
       }
#else
#define INPUT_BUF_PING_PONG
#endif


#define PROFILER_OPEN(prof, iname ) {          \
         (prof)->cycles = (prof)->exclude_cycles = 0; \
         (prof)->frame_cnt = (prof)->peak_frame = 0;  \
         (prof)->curr = (prof)->sum  = (prof)->ave = (prof)->peak = 0.0; \
         (prof)->start.tms_utime = 0;  \
         (prof)->stop.tms_utime = 0; \
         strcpy((prof)->name , iname); \
         STACK_MEASURE_OPEN(prof) \
     }

#define PROFILER_START(prof) {     \
          SCRATCH_TRASH\
		  OUTPUT_BUF_PING_PONG\
          STACK_MEASURE_BEGIN(prof) \
          times(&(prof)->start);         \
          XT_ISS_MEM_BANDWIDTH_PROFILER_START \
     }

#define PROFILER_STOP(prof) {     \
         XT_ISS_MEM_BANDWIDTH_PROFILER_STOP \
		 INPUT_BUF_PING_PONG\
         times(&(prof)->stop);           \
         STACK_MEASURE_END(prof) \
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
    STACK_MEASURE_CLOSE(prof)

// TO BE implemented with DV Integration
#define PROFILER_EXCLUDE_ON( pProfiler )
#define PROFILER_EXCLUDE_ON( pProfiler )

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

typedef enum _PROFILER_ID_t
{
    DEC_M = 0,
    DEC_A,
    PCMPROC,
    DDENC
#ifndef DDT_ONLY
    ,DDCFRAMESYNC
#endif /* DDT_ONLY */
} PROFILER_ID_t;

#ifdef SCRATCH_TRASH_TEST	
#define	SCRATCH_TRASH_TEST_ALLOCATE		\
    int scratch_trash_count = 0;		\
	int scratch_trash_size;				\
	char *scratch_trash_ptr_ping;		\
	char *scratch_trash_ptr_pong;		\
	char scratch_trash_pattern;			\
	unsigned short scratch_trash_lfsr;	

#define	SCRATCH_TRASH_TEST_ASSIGN							\
	if(ui_type == XA_MEMTYPE_SCRATCH)						\
	{														\
		scratch_trash_size = ui_size;						\
		scratch_trash_ptr_ping = (char *)pv_alloc_ptr;		\
        pv_alloc_ptr = MEM_malloc(ui_size, ui_alignment);	\
		scratch_trash_ptr_pong = (char *)pv_alloc_ptr;		\
	}

#else	/* #ifdef SCRATCH_TRASH_TEST */
#define	SCRATCH_TRASH_TEST_ALLOCATE
#define	SCRATCH_TRASH_TEST_ASSIGN
#endif	/* #ifdef SCRATCH_TRASH_TEST */

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

#else	/* #ifdef OUTPUT_BUF_PING_PONG_TEST */
#define	OUTPUT_BUF_PING_PONG_TEST_ALLOCATE
#define	OUTPUT_BUF_PING_PONG_TEST_ASSIGN
#endif	/* #ifdef OUTPUT_BUF_PING_PONG_TEST */


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

#else	/* #ifdef INPUT_BUF_PING_PONG_TEST */
#define	INPUT_BUF_PING_PONG_TEST_ALLOCATE
#define	INPUT_BUF_PING_PONG_TEST_ASSIGN
#endif	/* #ifdef INPUT_BUF_PING_PONG_TEST */


#ifdef PROF_ALLOCATE  //define in only one file
#ifdef INLOOP_STACK_MEASUREMENT
unsigned int *dummy;
void stack_populate(void)
{
    int i;
    unsigned int dummy_stack_array[STACK_MEASURE_PEAK];
    for (i=0; i < STACK_MEASURE_PEAK; i++) {
        dummy_stack_array[STACK_MEASURE_PEAK - 1 - i] = (i | STACK_MEASURE_PREFIX_PATTERN );
    }

    dummy= dummy_stack_array;
}
int stack_detect(void)
{
    int i;
    unsigned int dummy_stack_array[STACK_MEASURE_PEAK];
    for (i = (STACK_MEASURE_PEAK-1); i >=0 ; i--) 
    {
        if(dummy_stack_array[STACK_MEASURE_PEAK - 1 - i] != (i | STACK_MEASURE_PREFIX_PATTERN ))
        {
            break;
        }
    }
    if(dummy != dummy_stack_array)
    {
        return -1;
    }
    return (i * 4);
}
#endif
profiler_t gProfiler[8];
#else
extern profiler_t gProfiler[8];
#endif


#endif //__XA_PROFILER_H__


