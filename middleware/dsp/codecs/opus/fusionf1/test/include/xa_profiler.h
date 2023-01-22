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

#if !defined(PROFILE) && __XCC__
#define PROFILE 1
#endif

#if !defined(PREFETCH_CTL) && __XCC__
#define PREFETCH_CTL 1
#endif

#ifdef PROFILE
#include <xtensa/hal.h>
#include <xtensa/sim.h>
#include <sys/times.h>

#include "xa_type_def.h"

typedef struct XA_PROFILER {
    struct tms start;
    struct tms stop;
    double Peak;
    double Sum;
    int Peak_frame;
    int frame_count;
    long long total_cycles;
    long long total_samples;
    char *id;
} xa_profiler;

static inline void xa_compute_mcps(xa_profiler *p_prof, WORD32 samples_decoded, UWORD32 samp_freq, long long gen_strm_pos)
{
    double Ave=0.0, Curr;

    if (samples_decoded <= 0)
        return;

    p_prof->total_samples += samples_decoded;
    p_prof->frame_count++;

    clock_t cycles = p_prof->total_cycles;
    p_prof->total_cycles = 0;
    Curr = ((double) cycles / samples_decoded * samp_freq / 1000000);
    
    if (p_prof->frame_count > 1) {
        p_prof->Sum += Curr;
        Ave = p_prof->Sum / (p_prof->frame_count-1);

        if (p_prof->Peak < Curr) {
            p_prof->Peak = Curr;
            p_prof->Peak_frame = (p_prof->frame_count);
        }
    }                

    unsigned long long total_msec =
        (unsigned long long)((double)p_prof->total_samples / samp_freq * 1000.0);
    int msec = (int)(total_msec % 1000);
    unsigned long long total_seconds = total_msec / 1000;
    int seconds = (int)(total_seconds % 60);
    int minutes = (int)(total_seconds / 60);

    fprintf(stdout, "[%d|%lld] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d] %s\n",
        p_prof->frame_count, gen_strm_pos, minutes, seconds, msec,
        Curr, Ave, p_prof->Peak, p_prof->Peak_frame, p_prof->id);

}

#define INIT_XA_PROFILER(x, a)                  do { memset(&x, 0, sizeof(xa_profiler)); \
                                                     x.id = a; } while(0)
#define START_TIME_XA_PROFILER(x)               do { xt_iss_client_command("all", "enable"); \
                                                     times(&x.start); } while(0)
#define STOP_TIME_XA_PROFILER(x)                do { times(&x.stop); \
                                                     xt_iss_client_command("all", "disable"); \
                                                     x.total_cycles += (x.stop.tms_utime - x.start.tms_utime); } while(0)
#define COMPUTE_MHZ_XA_PROFILER(x, a, b, c)        do { xa_compute_mcps(&x, a, b, c); } while(0)
#define SUMMARY_XA_PROFILER(x)                  do { fprintf(stdout,"\n%5s Peak MCPS = %f\n", x.id, x.Peak); \
                                                     fprintf(stdout,"%5s Peak frame = %d\n", x.id, x.Peak_frame); \
                                                     fprintf(stdout,"%5s Average MCPS = %f\n", x.id, (x.frame_count < 2) ? 0 : (x.Sum/(x.frame_count-1))); } while(0) 

#else

typedef struct XA_PROFILER {    
    int place_holder;
} xa_profiler;

#define INIT_XA_PROFILER(x, a)                  do {} while(0)
#define START_TIME_XA_PROFILER(x)               do {} while(0)
#define STOP_TIME_XA_PROFILER(x)                do {} while(0)
#define COMPUTE_MHZ_XA_PROFILER(x, a, b)        do {} while(0)
#define SUMMARY_XA_PROFILER(x)                  do {} while(0)
#endif 

#ifdef PREFETCH_CTL
#define PREFETCH_AGGRESSIVE(x)                  do { x = xthal_set_cache_prefetch ((XTHAL_DCACHE_PREFETCH_HIGH | XTHAL_ICACHE_PREFETCH_HIGH)); \
                                                   } while(0)
#define PREFETCH_RESTORE(x)                     do { xthal_set_cache_prefetch (x); } while(0)
#else
#define PREFETCH_AGGRESSIVE(x)                  do {} while(0)
#define PREFETCH_RESTORE(x)                     do {} while(0)
#endif

#endif /* __XA_PROFILER_H__ */

