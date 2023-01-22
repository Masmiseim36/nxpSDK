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

/***********************************************************************
Copyright (c) 2006-2011, Skype Limited. All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, (subject to the limitations in the disclaimer below)
are permitted provided that the following conditions are met:
- Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
- Neither the name of Skype Limited, nor the names of specific
contributors, may be used to endorse or promote products derived from
this software without specific prior written permission.
NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED
BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
CONTRIBUTORS ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************/


/*****************************/
/* Opus encoder test program */
/*****************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#define INPUT_PING_PONG
//#define OUTPUT_PING_PONG
//#define SCRATCH_PING_PONG

#ifdef SCRATCH_ANALYSIS
int scratch_max_size = 0;
int scratch_curr_size = 0;
int scratch_beg = 0;
int scratch_lnew_size = 0;
int scratch_new_size = 0;
#endif

#include "./opus_codec/xa_opus_codec_api.h"
#include "xa_type_def.h"
#include "xa_error_handler.h"

#ifdef SCRATCH_PING_PONG
#define SCRATCH_TEST(p_scratch_test, pattern) \
    WORD32 nm; \
    pUWORD8 pnt = (pUWORD8)p_scratch_test; \
    for(nm = 0; nm < scratch_size; nm++) \
    { \
        if(pnt[nm] != pattern) \
        { \
            scratch_ping_pong_flag = 1; \
            break; \
        } \
    } \
    if(scratch_ping_pong_flag == 1) \
        break;
#endif

#ifdef __XTENSA_EB__
#define _SYSTEM_IS_BIG_ENDIAN
#endif

#ifdef __XCC__
#define __PROFILE__
#endif

#ifdef CSTUB_HIFI2
#undef __PROFILE__
#endif

#ifdef __PROFILE__
#include <xtensa/tie/xt_hifi2.h>
#include <sys/times.h>
#include <xtensa/sim.h>
/* Following variable shows the frame number that will be profiled */
/* Set profile_frame = 0 to profile all frames */
int profile_frame = 0;
#endif

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#define XA_MAX_CMD_LINE_LENGTH 1024
#define XA_MAX_ARGS 50
#define PARAMFILE "paramfilesimple_encode.txt"

#define OPUS_RAND(seed)                   ((WORD64)(((WORD64)907633515) +  (WORD32)(seed * 196314165)))

/* Application and testbench error handler */
#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED                0xFFFF8000
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED                0xFFFF8001

/* These are codec library error*/
VOID xa_opus_codec_error_handler_init();
extern xa_error_info_struct xa_opus_codec_error_info;

/* These are testbench error*/
VOID xa_testbench_error_handler_init();
extern  xa_error_info_struct xa_testbench_error_info;

/* Define codec specific settings */
#define MAX_LBRR_DELAY              2
#define FRAME_LENGTH_MS             20
#define MAX_API_FS_KHZ              48
#define MAX_IO_FILE_NAME_SIZE      1024 


static const int silk8_test[][4] = {
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 960*3, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 960*2, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 960,   1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 480,   1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 960*3, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 960*2, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 960,   2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND, 480,   2}
};

static const int silk12_test[][4] = {
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 960*3, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 960*2, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 960,   1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 480,   1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 960*3, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 960*2, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 960,   2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_MEDIUMBAND, 480,   2}
};

static const int silk16_test[][4] = {
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 960*3, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 960*2, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 960,   1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 480,   1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 960*3, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 960*2, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 960,   2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND, 480,   2}
};

static const int hybrid24_test[][4] = {
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 960, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 480, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 960, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 480, 2}
};

static const int hybrid48_test[][4] = {
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_FULLBAND, 960, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_FULLBAND, 480, 1},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_FULLBAND, 960, 2},
      {XA_OPUS_MODE_SILK_ONLY, XA_OPUS_BANDWIDTH_FULLBAND, 480, 2}
};

static const int celt_test[][4] = {
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      960, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 960, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      960, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    960, 1},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      480, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 480, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      480, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    480, 1},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      240, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 240, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      240, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    240, 1},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      120, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 120, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      120, 1},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    120, 1},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      960, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 960, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      960, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    960, 2},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      480, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 480, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      480, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    480, 2},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      240, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 240, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      240, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    240, 2},

      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      120, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_SUPERWIDEBAND, 120, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_WIDEBAND,      120, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_NARROWBAND,    120, 2},

};

static const int celt_hq_test[][4] = {
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      960, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      480, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      240, 2},
      {XA_OPUS_MODE_CELT_ONLY, XA_OPUS_BANDWIDTH_FULLBAND,      120, 2},
};


#ifdef _SYSTEM_IS_BIG_ENDIAN
/* Function to convert a little endian int16 to a */
/* big endian int16 or vica verca                 */
void swap_endian
(
    WORD16       vec[],              /*  I/O array of */
    WORD         len                 /*  I   length      */
)
{
    WORD i;
    WORD16 tmp;
    UWORD8 *p1, *p2;

    for( i = 0; i < len; i++ )
    {
        tmp = vec[ i ];
        p1 = (UWORD8 *)&vec[ i ]; p2 = (UWORD8 *)&tmp;
        p1[ 0 ] = p2[ 1 ]; p1[ 1 ] = p2[ 0 ];
    }
}
#endif



void print_usage( char* argv[] )
{
    printf( "Usage: %s -app:<application> -fs:<sampling rate (8000/12000/16000/24000/48000) Hz> -numch:<channels (1/2)> "
        "-bps:<Recommended: 6000..510000 bits per second>  [options] -ifile:<input file> -ofile:<output file>\n", argv[0]);
    printf( "application: voip | audio | restricted-lowdelay\n" );
    printf( "bps: positive values outside recommended range and -1 (maximum possible bitrate based on other parameters) are also accepted \n" );
    printf( "Options:\n" );
    printf( "   -force_mode:<hybrid|silk|celt> : forces the encoding mode; default: auto\n" );
    printf( "   -cbr:<0/1>                     : enable constant bitrate; default: variable bitrate\n" );
    printf( "   -cvbr:<0/1>                    : enable constrained variable bitrate; default: unconstrained\n" );
    printf( "   -bandwidth:<NB|MB|WB|SWB|FB>   : audio bandwidth (from narrowband to fullband); default: sampling rate\n" );
    printf( "   -framesize:<2.5|5|10|20|40|60|80|100|120> : frame size in ms; default: 20 \n" );
    printf( "   -max_payload:<bytes>           : maximum payload size in bytes 1-1500, default: 1500 (values outside range are ignored)\n" );
    printf( "   -complexity:<comp>             : complexity, 0 (lowest) ... 10 (highest); default: 10\n" );
    printf( "   -inbandfec:<0/1>               : enable SILK inband FEC, default: disable\n" );
    printf( "   -forcemono:<0/1>               : force mono encoding, even for stereo input, default: disable\n" );
    printf( "   -dtx:<0/1>                     : enable SILK DTX, default:disable\n" );
    printf( "   -loss:<perc>                   : uplink loss estimate, in percent (0-100), default: 0 \n" );
    printf( "Options to demonstare ability of dynamic changing parameters:\n" );
    printf( "   -sweep:<bps>                   : sweep bitrate mode, bps - bitrate step\n" );
    printf( "   -sweep_max:<bps>               : max. bitrate in sweep bitrate mode \n" );
    printf( "   -random_framesize:<0/1>        : enable random frame size \n" );
    printf( "   -random_fec:<0/1>              : enable random SILK inband FEC \n" );
    printf( "   -silk8k_test:<0/1>             : enable dynamic changing of frame size and number of channels in SILK mode (8 kHz) according to predefined list \n" );
    printf( "   -silk12k_test:<0/1>            : enable dynamic changing of frame size and number of channels in SILK mode (12 kHz) according to predefined list \n" );
    printf( "   -silk16k_test:<0/1>            : enable dynamic changing of frame size and number of channels in SILK mode (16 kHz) according to predefined list \n" );
    printf( "   -hybrid24k_test:<0/1>          : enable dynamic changing of frame size and number of channels in Hybrid mode (24 kHz) according to predefined list \n" );
    printf( "   -hybrid48k_test:<0/1>          : enable dynamic changing of frame size and number of channels in Hybrid mode (48 kHz) according to predefined list \n" );
    printf( "   -celt_test:<0/1>               : enable dynamic changing of frame size, number of channels and bandwidth in CELT mode according to predefined list \n" );
    printf( "   -celt_hq_test:<0/1>            : enable dynamic changing of frame size and number of channels in CELT mode (FB only) according to predefined list \n" );
}



void get_new_random_framesize(int *frame_size, int targetRate_bps, int sweep_bps, int *delayed_celt, int *newsize , xa_opus_enc_control_t* enc_control, pUWORD32 pseed )
{
    if (*delayed_celt)
    {
        *frame_size = *newsize;
        *delayed_celt = 0;
    } 
    else if ((*pseed = OPUS_RAND(*pseed))%20==0)
    {
        *pseed = OPUS_RAND(*pseed);
        *newsize = *pseed%6;
        switch(*newsize)
        {
        case 0: *newsize = enc_control->API_sampleRate/400;  break;
        case 1: *newsize = enc_control->API_sampleRate/200;  break;
        case 2: *newsize = enc_control->API_sampleRate/100;  break;
        case 3: *newsize = enc_control->API_sampleRate/50;   break;
        case 4: *newsize = enc_control->API_sampleRate/25;   break;
        case 5: *newsize = 3*enc_control->API_sampleRate/50; break;
        }
        while (*newsize < enc_control->API_sampleRate/25 && targetRate_bps-abs(sweep_bps) <= 3*12*enc_control->API_sampleRate/(*newsize))
            *newsize *= 2;
        if (*newsize < enc_control->API_sampleRate/100 && *frame_size >= enc_control->API_sampleRate/100)
        {
            enc_control->force_mode = XA_OPUS_MODE_CELT_ONLY;
            *delayed_celt = 1;
        } 
        else 
        {
            *frame_size = *newsize;
        }
    }  
}

int get_bitrate_for_sweep_mode(int *targetRate_bps, int *sweep_bps, int sweep_min, int sweep_max)
{
    *targetRate_bps += *sweep_bps;
    if (sweep_max)
    {
        if (*targetRate_bps > sweep_max)
            *sweep_bps = -*sweep_bps;
        else if (*targetRate_bps < sweep_min)
            *sweep_bps = -*sweep_bps;
    }
    if (*targetRate_bps < 1000)
        *targetRate_bps = 1000;

    return *targetRate_bps;
}


int xa_opus_enc_main_process( int argc, char* argv[] )
{
    WORD16      counter = 0;
    WORD32      args =0, totPackets =0;
    WORD16      out_bytes=0;
    char       speechInFileName[ MAX_IO_FILE_NAME_SIZE ] = "NULL", bitOutFileName[ MAX_IO_FILE_NAME_SIZE ] = "NULL";
    FILE       *bitOutFile=NULL, *speechInFile=NULL;

#ifdef _SYSTEM_IS_BIG_ENDIAN
    //WORD16      nBytes_LE=0;
#endif
  
    pVOID       scratch = NULL;
#ifdef SCRATCH_PING_PONG
    pVOID       scratch_ping = NULL;
    pVOID       scratch_pong = NULL;
    WORD32      scratch_ping_pong_flag = 0;
#endif
    pWORD16     inp_speech = NULL;
#ifdef INPUT_PING_PONG
    pWORD16     inp_speech_ping = NULL, inp_speech_pong = NULL;
#endif
    pUWORD8     enc_speech = NULL;
#ifdef OUTPUT_PING_PONG
    pUWORD8     enc_speech_ping = NULL, enc_speech_pong = NULL;
#endif
    WORD32      handle_size =0, scratch_size =0, inp_speech_size =0, enc_speech_size =0;
    int         frame =0;

    int         stop_processing = 0;

    XA_ERRORCODE error_code = XA_NO_ERROR;
    xa_codec_handle_t speech_encoder_state = NULL;
    xa_opus_enc_control_t enc_control; // Struct for input to encoder
    xa_error_info_struct *p_proc_codec_err_info = NULL;
    xa_error_info_struct *p_proc_testbench_err_info = NULL;

    WORD32 sweep_bps = 0;
    WORD32 random_framesize_enabled = 0;
    WORD32 sweep_max = 0, sweep_min = 0;
    WORD32 random_fec = 0;
    WORD32 delayed_celt = 0, newsize =0;
    UWORD32 seed = 0, seed_fec = 0;
 
    /* Variables to demonstare changing coding mode and framesize */
    int curr_mode=0;
    int curr_mode_count=0;
    int nb_modes_in_list=0;
    int mode_switch_time = 48000;
    const int (*mode_list)[4]=NULL;

#ifdef __PROFILE__
    double u_seconds;
    struct tms start, stop;
    clock_t cycles;
    double mcps_peak = 0, mcps_curr = 0, mcps_ave = 0, mcps_sum = 0;
    int peak_frame = 0;
#endif

    WORD32 frame_size;
	float frame_size_ms;
    WORD32 targetRate_bps;


    /* ******************************************************************/
    /* Initialize the error handler : testbench                           */
    /* ******************************************************************/
    p_proc_testbench_err_info = (xa_error_info_struct*)&xa_testbench_error_info;
    xa_testbench_error_handler_init();

    /* ******************************************************************/
    /* Initialize the error handler : opus codeclibrary                             */
    /* ******************************************************************/
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_opus_codec_error_info;
    xa_opus_codec_error_handler_init();

    /* Library name version etc print */
    fprintf(stderr, "\n");
    fprintf(stderr, "%s library version %s\n",
            xa_opus_get_lib_name_string(),
            xa_opus_get_lib_version_string());
    fprintf(stderr, "API version: %s\n", xa_opus_get_lib_api_version_string());
    fprintf(stderr, "Cadence Design Systems, Inc. http://www.cadence.com\n");
    fprintf(stderr, "\n");

    if( argc < 3 ) 
    {
        print_usage( argv );
        exit( 0 );
    }

    /* ******************************************************************/
    /* Reset all Encoder optional control parameters to default state   */
    /* ******************************************************************/
    memset( &enc_control, 0, sizeof(enc_control) );

    /******************************/
    /* Get arguments              */
    /******************************/

    /* Default settings */
    frame_size = 0;
    enc_control.max_payload = XA_OPUS_MAX_BYTES_ENCODED_PACKET - 4;
    /* Same value as 1.3.1 reference testbench, library default value is 9 in 1.1.3 reference */
    enc_control.complexity = 10;

    /* get arguments */
    args = 1;
    while( args < argc ) 
    {
        if( strncmp( argv[ args ], "-app:",5 ) == 0 )
        {
            if (strcmp(argv[args] + 5, "voip")==0)
                enc_control.application = XA_OPUS_APPLICATION_VOIP;
            else if (strcmp(argv[args] + 5, "restricted-lowdelay")==0)
                enc_control.application = XA_OPUS_APPLICATION_RESTRICTED_LOWDELAY;
            else if (strcmp(argv[args] + 5, "audio")==0) 
                enc_control.application = XA_OPUS_APPLICATION_AUDIO;
            else {
                fprintf(stderr, "unknown application: %s\n", argv[args]);
                exit(0);
            }
            args++;
        } 
        else if( strncmp( argv[ args ], "-fs:",4 ) == 0 )
        {
            sscanf( argv[ args ] + 4, "%d", &enc_control.API_sampleRate );
			if (enc_control.API_sampleRate != 8000  && enc_control.API_sampleRate != 12000 &&
			enc_control.API_sampleRate != 16000 && enc_control.API_sampleRate != 24000 &&
			enc_control.API_sampleRate != 48000)
			{
				fprintf(stderr, "Sampling frequency not supported: %d\n", enc_control.API_sampleRate);
                exit(0);
			}
			args++;
        } 
        else if( strncmp( argv[ args ], "-numch:",7 ) == 0 )
        {
            sscanf( argv[ args ] + 7, "%d", &enc_control.API_numChannels );
			if (enc_control.API_numChannels != 1  && enc_control.API_numChannels != 2)
			{
				fprintf(stderr, "Number of channel not supported: %d\n", enc_control.API_numChannels);
                exit(0);
			}
            args++;
        }
        else if( strncmp( argv[ args ], "-bps:",5 ) == 0 )
        {
            sscanf( argv[ args ] + 5, "%d", &enc_control.bitRate );
            args++;
        }
        else if( strncmp( argv[ args ], "-force_mode:",12) == 0 ) 
        {
            if (strcmp(argv[ args ] + 12, "hybrid")==0)
                enc_control.force_mode = XA_OPUS_MODE_HYBRID;
            else if (strcmp(argv[ args ] + 12, "silk")==0)
                enc_control.force_mode = XA_OPUS_MODE_SILK_ONLY;
            else if (strcmp(argv[ args ] + 12, "celt")==0)
                enc_control.force_mode = XA_OPUS_MODE_CELT_ONLY;
            else {
                fprintf(stderr, "Unknown forced mode %s. "
                    "Supported are hybrid, silk, celt.\n",
                    argv[ args ] + 12);
                return EXIT_FAILURE;
            }
            args++;
        } 
        else if( strncmp( argv[ args ], "-cbr:",5) == 0 ) 
        {
            sscanf( argv[ args ] + 5, "%d", &enc_control.cbr );
			if (enc_control.cbr < 0  || enc_control.cbr > 1)
			{
				fprintf(stderr, "Constant Bitrate Setting not supported: %d\n", enc_control.cbr);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-cvbr:", 6 ) == 0 ) 
        {
            sscanf( argv[ args ] + 6, "%d", &enc_control.cvbr );
			if (enc_control.cvbr < 0  || enc_control.cvbr > 1)
			{
				fprintf(stderr, "Constrained Variable Bitrate Setting not supported: %d\n", enc_control.cvbr);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-bandwidth:",11) == 0 ) 
        {
            if (strcmp(argv[ args ] + 11, "NB")==0)
                enc_control.bandwidth = XA_OPUS_BANDWIDTH_NARROWBAND;
            else if (strcmp(argv[ args ] + 11, "MB")==0)
                enc_control.bandwidth = XA_OPUS_BANDWIDTH_MEDIUMBAND;
            else if (strcmp(argv[ args ] + 11, "WB")==0)
                enc_control.bandwidth = XA_OPUS_BANDWIDTH_WIDEBAND;
            else if (strcmp(argv[ args ] + 11, "SWB")==0)
                enc_control.bandwidth = XA_OPUS_BANDWIDTH_SUPERWIDEBAND;
            else if (strcmp(argv[ args ] + 11, "FB")==0)
                enc_control.bandwidth = XA_OPUS_BANDWIDTH_FULLBAND;
            else {
                fprintf(stderr, "Unknown bandwidth %s. "
                    "Supported are NB, MB, WB, SWB, FB.\n",
                    argv[ args ] + 11);
                return EXIT_FAILURE;
            }
            args++;
        } 
        else if( strncmp( argv[ args ], "-framesize:", 11 ) == 0 ) 
        {
            sscanf( argv[ args ] + 11, "%f", &frame_size_ms );
            if (strcmp(argv[ args ] + 11, "2.5")==0)
                frame_size = enc_control.API_sampleRate/400;
            else if (strcmp(argv[ args ] + 11, "5")==0)
                frame_size = enc_control.API_sampleRate/200;
            else if (strcmp(argv[ args ] + 11, "10")==0)
                frame_size = enc_control.API_sampleRate/100;
            else if (strcmp(argv[ args ] + 11, "20")==0)
                frame_size = enc_control.API_sampleRate/50;
            else if (strcmp(argv[ args ] + 11, "40")==0)
                frame_size = enc_control.API_sampleRate/25;
            else if (strcmp(argv[ args ] + 11, "60")==0)
                frame_size = 3*enc_control.API_sampleRate/50;
            else if (strcmp(argv[ args ] + 11, "80")==0)
                frame_size = 4*enc_control.API_sampleRate/50;
            else if (strcmp(argv[ args ] + 11, "100")==0)
                frame_size = 5*enc_control.API_sampleRate/50;
            else if (strcmp(argv[ args ] + 11, "120")==0)
                frame_size = 6*enc_control.API_sampleRate/50;
            else {
                fprintf(stderr, "Unsupported frame size: %s ms. "
                                "Supported are 2.5, 5, 10, 20, 40, 60, 80, 100, 120.\n",
                                argv[ args ] + 11);
                return EXIT_FAILURE;
            }
            args++;
        } 
        else if( strncmp( argv[ args ], "-max_payload:", 13 ) == 0 ) 
        {
            sscanf( argv[ args ] + 13, "%d", &enc_control.max_payload );
			if (enc_control.max_payload < 0  || enc_control.max_payload > XA_OPUS_MAX_BYTES_ENCODED_PACKET-4)
			{
				fprintf(stderr, "Maximum Payload Size not supported: %d, Considered Default: 1500\n", enc_control.max_payload);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-complexity:", 12 ) == 0 ) 
        {
            sscanf( argv[ args ] + 12, "%d", &enc_control.complexity );
			if (enc_control.complexity < 0  || enc_control.complexity > 10)
			{
				fprintf(stderr, "Complexity Setting not supported: %d\n", enc_control.complexity);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-inbandfec:", 11 ) == 0 ) 
        {
            sscanf( argv[ args ] + 11, "%d", &enc_control.SILK_InBandFEC_enabled );
			if (enc_control.SILK_InBandFEC_enabled != 1  && enc_control.SILK_InBandFEC_enabled != 0)
			{
				fprintf(stderr, "SILK inband FEC Setting not supported: %d\n", enc_control.SILK_InBandFEC_enabled);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-forcemono:", 11 ) == 0 ) 
        {
            sscanf( argv[ args ] + 11, "%d", &enc_control.force_numChannels);
			if (enc_control.force_numChannels != 1  && enc_control.force_numChannels != 0)
			{
				fprintf(stderr, "Force Mono Encoding Setting not supported: %d\n", enc_control.force_numChannels);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-dtx:", 5) == 0 ) 
        {
            sscanf( argv[ args ] + 5, "%d", &enc_control.SILK_DTX_enabled);
			if (enc_control.SILK_DTX_enabled != 1  && enc_control.SILK_DTX_enabled != 0)
			{
				fprintf(stderr, "SILK DTX Setting not supported: %d\n", enc_control.SILK_DTX_enabled);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-loss:",6 ) == 0 ) 
        {
            sscanf( argv[ args ] + 6, "%d", &enc_control.packetLossPercentage );
			if (enc_control.packetLossPercentage < 0  || enc_control.packetLossPercentage > 100)
			{
				fprintf(stderr, "Uplink Loss Estimate Percentage not supported: %d\n", enc_control.packetLossPercentage);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-ifile:",7 ) == 0 ) 
        {
			if((strlen(argv[ args ])-7) > 1024) 
   			{
        		printf( "Error: Input file name length too large, max allowed length is 1024\n");
        		exit( 0 );
    		}
            sscanf( argv[ args ] + 7, "%s", speechInFileName );
            args++;
        } 
        else if( strncmp( argv[ args ], "-ofile:",7 ) == 0 ) 
        {
		    if((strlen(argv[ args ])-7) > 1024) 
   			{
        		printf( "Error: Output file name length too large, max allowed length is 1024\n");
        		exit( 0 );
    		}
            sscanf( argv[ args ] + 7, "%s", bitOutFileName );
            args++;
        } 
        else if( strncmp( argv[ args ], "-sweep:", 7 ) == 0 ) 
        {
            sscanf( argv[ args ] + 7, "%d", &sweep_bps );
            args++;
        } 
        else if( strncmp( argv[ args ], "-sweep_max:", 11 ) == 0 )
        {
            sscanf( argv[ args ] + 11, "%d", &sweep_max );
            args++;
        }
        else if( strncmp( argv[ args ], "-random_framesize:", 18 ) == 0 ) 
        {
            sscanf( argv[ args ] + 18, "%d", &random_framesize_enabled );
            if( (random_framesize_enabled<0) || (random_framesize_enabled>1) )
            {
                printf("Invalid random_framesize value : %s\n", argv[ args ]+18);
                return EXIT_FAILURE;
            }             
            args++;
        } 
        else if( strncmp( argv[ args ], "-random_fec:", 12 ) == 0 ) 
        {
            sscanf( argv[ args ] + 12, "%d", &random_fec );
            if( (random_fec<0) || (random_fec>1) )
            {
                printf("Invalid random_fec value : %s\n", argv[ args ] + 12);
                return EXIT_FAILURE;
            }            
            args++;
        }
        else if( strncmp( argv[ args ], "-silk8k_test:", 13 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 13, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid silk8k_test value : %s\n", argv[ args ] + 13);
                return EXIT_FAILURE;
            }            
            mode_list = (flag == 0) ? NULL : silk8_test;
            nb_modes_in_list = 8;
            args++;
        }
        else if( strncmp( argv[ args ], "-silk12k_test:", 14 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 14, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid silk12k_test value : %s\n", argv[ args ] + 14);
                return EXIT_FAILURE;
            }            
            mode_list = (flag == 0) ? NULL : silk12_test;
            nb_modes_in_list = 8;
            args++;
        }
        else if( strncmp( argv[ args ], "-silk16k_test:", 14 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 14, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid silk16k_test value : %s\n", argv[ args ] + 14);
                return EXIT_FAILURE;
            }            
            mode_list = (flag == 0) ? NULL : silk16_test;
            nb_modes_in_list = 8;
            args++;
        }
        else if( strncmp( argv[ args ], "-hybrid24k_test:", 16 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 16, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid hybrid24k_test value : %s\n", argv[ args ] + 16);
                return EXIT_FAILURE;
            }            
            mode_list = (flag == 0) ? NULL : hybrid24_test;
            nb_modes_in_list = 4;
            args++;
        }
        else if( strncmp( argv[ args ], "-hybrid48k_test:", 16 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 16, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid hybrid48k_test value : %s\n", argv[ args ] + 16);
                return EXIT_FAILURE;
            }            
            mode_list = (flag == 0) ? NULL : hybrid48_test;
            nb_modes_in_list = 4;
            args++;
        }
        else if( strncmp( argv[ args ], "-celt_test:", 11 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 11, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid celt_test value : %s\n", argv[ args ] + 11);
                return EXIT_FAILURE;
            }
            mode_list = (flag == 0) ? NULL : celt_test;
            nb_modes_in_list = 32;
            args++;
        }
        else if( strncmp( argv[ args ], "-celt_hq_test:", 14 ) == 0 )
        {
            int flag;
            sscanf( argv[ args ] + 14, "%d", &flag);
            if( (flag<0) || (flag>1) )
            {
                printf("Invalid celt_hq_test value : %s\n", argv[ args ] + 14);
                return EXIT_FAILURE;
            }
            mode_list = (flag == 0) ? NULL : celt_hq_test;
            nb_modes_in_list = 4;
            args++;
        }
        else 
        {
            printf( "Error: unrecognized setting: %s\n\n", argv[ args ] );
            print_usage( argv );
            exit( 0 );
        }
    }

    /* Set frame size if it is not set through command line */
    if (frame_size == 0)
	{
        frame_size = enc_control.API_sampleRate / 50;
		frame_size_ms = 20;
	}
     
    if(enc_control.force_mode == XA_OPUS_MODE_CELT_ONLY)
    {
        if(frame_size > enc_control.API_sampleRate/50)
            printf("Warning: Incorrect framesize set for CELT mode. Either different framesize or encoding mode will be set.\n\n");
    }
    if(enc_control.force_mode == XA_OPUS_MODE_HYBRID)
    {
        if((frame_size < enc_control.API_sampleRate/100) || (frame_size > enc_control.API_sampleRate/50))
            printf("Warning: Incorrect framesize set for HYBRID mode. Either different framesize or encoding mode will be set.\n\n");
    }
    if(enc_control.force_mode == XA_OPUS_MODE_SILK_ONLY)
    {
        if(frame_size < enc_control.API_sampleRate/100)
            printf("Warning: Incorrect framesize set for SILK mode. Either different framesize or encoding mode will be set.\n\n");
    }

    targetRate_bps = enc_control.bitRate;

		if(enc_control.API_sampleRate < 1)
		{
			printf("Error: Invalid value of sampling rate : %d\n", enc_control.API_sampleRate);
			printf("Supported sampling rates are 8000, 12000, 16000, 24000 and 48000.\n");
			exit(0);
		}	
    /* Print options */
    printf("******************* Opus Encoder ****************\n");
    printf("******************* Compiled for %d bit cpu ********* \n", (int)sizeof(void*) * 8 );
    printf( "Input:                                         %s\n",     speechInFileName );
    printf( "Output:                                        %s\n",     bitOutFileName );
    printf( "Application:                                   %d\n",     enc_control.application );
    printf( "API sampling rate:                             %d Hz\n",  enc_control.API_sampleRate );
    printf( "API number of channels:                        %d\n",     enc_control.API_numChannels );
    printf( "Target bitrate:                                %d bps\n", enc_control.bitRate );
    printf( "Enable constant bitrate:                       %d\n",     enc_control.cbr );
    printf( "Enable constrained variable bitrate:           %d\n",     enc_control.cvbr );
    printf( "Audio bandwidth:                               %d\n",     enc_control.bandwidth );
    printf( "Framesize in ms:                               %1.1f\n",  frame_size_ms );
    printf( "Maximum payload size in bytes:                 %d\n",     enc_control.max_payload );
    printf( "Complexity:                                    %d\n",     enc_control.complexity );
    printf( "Enable SILK inband FEC:                        %d\n",     enc_control.SILK_InBandFEC_enabled );
    printf( "Force mono encoding, even for stereo input:    %d\n",     enc_control.force_numChannels );
    printf( "Enable SILK DTX        :                       %d\n",     enc_control.SILK_DTX_enabled );
    printf( "Simulate packet loss, in percent:              %d\n",     enc_control.packetLossPercentage );
    
    /* Open files */
    speechInFile = fopen( speechInFileName, "rb" );
    if( speechInFile == NULL ) 
    {
        printf( "Error: could not open input file %s\n", speechInFileName );
        exit( 0 );
    }
    bitOutFile = fopen( bitOutFileName, "wb" );
    if( bitOutFile == NULL ) 
    {
        printf( "Error: could not open output file %s\n", bitOutFileName );
        exit( 0 );
    }

    /************************************************************
    *                     Get Memory sizes from library        *
    ************************************************************/

    handle_size = xa_opus_enc_get_handle_byte_size(enc_control.API_numChannels);
    scratch_size = xa_opus_enc_get_scratch_byte_size();
#ifdef SCRATCH_ANALYSIS
    /* Extra scratch to avoid scratch overrun */
    scratch_size += 35*1024;
#endif
    inp_speech_size = XA_OPUS_MAX_BYTES_CHANNEL_PACKET * enc_control.API_numChannels;
    enc_speech_size = XA_OPUS_MAX_BYTES_ENCODED_PACKET;

    fprintf(stdout, "\nPersistent state size: %d bytes\n", handle_size);
    fprintf(stdout, "Scratch buffer size:   %d bytes\n", scratch_size);
    fprintf(stdout, "Input buffer size:     %d bytes\n", inp_speech_size);
    fprintf(stdout, "Output buffer size:    %d bytes\n\n", enc_speech_size);

    /************************************************************
    *                     Allocate Memory                       *
    ************************************************************/

    /* State handle */
    speech_encoder_state = (xa_codec_handle_t)malloc(handle_size);
    if(speech_encoder_state == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

    /* Scratch */
#ifndef SCRATCH_PING_PONG
    scratch = (pVOID)malloc(scratch_size);
    if(scratch == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(scratch,0x7f,scratch_size);
#else
    scratch_ping = (pVOID)malloc(scratch_size);
    if(scratch_ping == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(scratch_ping,0xcb,scratch_size);
    scratch_pong = (pVOID)malloc(scratch_size);
    if(scratch_pong == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API scratch alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    memset(scratch_pong,0xcb,scratch_size);
    scratch = scratch_ping;
#endif

#ifdef SCRATCH_ANALYSIS
    scratch_beg = (int)scratch;
#endif
    /* Input buffer */
#ifndef INPUT_PING_PONG
    inp_speech = (pWORD16)malloc(inp_speech_size);
    if(inp_speech == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc",XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
#else
    inp_speech_ping = (pWORD16)malloc(inp_speech_size);
    if(inp_speech_ping == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc",XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    inp_speech_pong = (pWORD16)malloc(inp_speech_size);
    if(inp_speech_pong == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc",XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    inp_speech = inp_speech_ping;
#endif
#ifndef FERRET_WARNINGS
    memset(inp_speech,0x7f,inp_speech_size);
#endif

    /*Output buffer */
#ifndef OUTPUT_PING_PONG
    enc_speech = (pUWORD8)malloc(enc_speech_size + 12);
    if (enc_speech == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
#else
    enc_speech_ping = (pUWORD8)malloc(enc_speech_size + 12);
    if (enc_speech_ping == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    enc_speech_pong = (pUWORD8)malloc(enc_speech_size + 12);
    if (enc_speech_pong == NULL)
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    enc_speech = enc_speech_ping;
#endif

#ifndef FERRET_WARNINGS
    memset(enc_speech, 0x7f, enc_speech_size + 12);
#endif

      /**********************************************************************
     *                Initialization
     ***********************************************************************/
    error_code = xa_opus_enc_init(speech_encoder_state, &enc_control);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

    if( enc_control.API_sampleRate > MAX_API_FS_KHZ * 1000 || enc_control.API_sampleRate < 0 ) 
    {
        printf( "\nError: API sampling rate = %d out of range, valid range 8000 - 48000 \n \n", enc_control.API_sampleRate );
        exit( 0 );
    }

    // Initialization additional variables (optional command line parameters case)

    if (sweep_max)
       sweep_min = targetRate_bps;

    if (random_fec && ((seed_fec = OPUS_RAND(seed_fec))%30==0))
    {
       seed_fec = OPUS_RAND(seed_fec);
       enc_control.SILK_InBandFEC_enabled = seed_fec%4==0;
    }

    if (mode_list!=NULL)
    {
        int size;
        fseek(speechInFile, 0, SEEK_END);
        size = ftell(speechInFile);
        fseek(speechInFile, 0, SEEK_SET);
        mode_switch_time = size/sizeof(WORD16)/enc_control.API_numChannels/nb_modes_in_list;

        enc_control.bandwidth     = mode_list[curr_mode][1];
        enc_control.force_mode   = mode_list[curr_mode][0];
        enc_control.force_numChannels = mode_list[curr_mode][3];
        frame_size                = mode_list[curr_mode][2];
    }

    totPackets = 0;

    while( !stop_processing ) 
    {
#ifdef INPUT_PING_PONG
        if((WORD32)inp_speech == (WORD32)inp_speech_ping)
        {
            inp_speech = inp_speech_pong;
            memcpy(inp_speech, inp_speech_ping, inp_speech_size);
            memset(inp_speech_ping, 0xca, inp_speech_size);
        }
        else
        {
            inp_speech = inp_speech_ping;
            memcpy(inp_speech, inp_speech_pong, inp_speech_size);
            memset(inp_speech_pong, 0xca, inp_speech_size);
        }
#endif
        /* Processig optional command line parameters -random_framesize (dynamic changing of frame size) */
        /*---------------------------*/
        if(random_framesize_enabled) 
            get_new_random_framesize(&frame_size, targetRate_bps, sweep_bps, &delayed_celt, &newsize , &enc_control, &seed);
        /*---------------------------*/

        /* Read input from file */
        counter = (WORD16)(fread( inp_speech, sizeof( WORD16 ) * enc_control.API_numChannels, frame_size, speechInFile ));

#ifdef _SYSTEM_IS_BIG_ENDIAN
        swap_endian( inp_speech, counter );
#endif

        if( (WORD32)counter < frame_size ) 
        {
            memset(inp_speech + counter * enc_control.API_numChannels, 0, (frame_size - counter) * sizeof( WORD16 ) * enc_control.API_numChannels );
            stop_processing = 1;
        }

#ifdef SCRATCH_PING_PONG
        if(scratch == scratch_ping)
        {
            /* Test other scratch area for corruption */
            SCRATCH_TEST(scratch_pong, 0xcb)
            scratch = scratch_pong;
            memset(scratch_ping, 0xcb, scratch_size);
        }
        else
        {
            /* Test other scratch area for corruption */
            SCRATCH_TEST(scratch_ping, 0xcb)
            scratch = scratch_ping;
            memset(scratch_pong, 0xcb, scratch_size);
        }
#endif
        /* max payload size */
        out_bytes = XA_OPUS_MAX_BYTES_ENCODED_PACKET;

#ifdef __PROFILE__
        xt_iss_client_command("all", "enable");
        times(&start);
#endif

        /* Opus Encoder */
        error_code =  xa_opus_enc(speech_encoder_state, 
                                  inp_speech, 
                                  enc_speech, 
                                  frame_size, 
                                  &enc_control, 
                                  &out_bytes,
                                  scratch);

#ifdef __PROFILE__
        times(&stop);
        xt_iss_client_command("all", "disable");
#endif
        _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
        frame++;
        totPackets++;

#ifdef __PROFILE__
        cycles = stop.tms_utime - start.tms_utime;
        u_seconds = ((double)frame_size * 1000000) / enc_control.API_sampleRate;
        mcps_curr = (double)cycles / u_seconds;
        mcps_sum += mcps_curr;
        mcps_ave = mcps_sum / frame;
        if (mcps_peak < mcps_curr) 
        {
            mcps_peak = mcps_curr;
            peak_frame = frame;
        }
        fprintf(stdout, "[%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",frame,mcps_curr, mcps_ave, mcps_peak, peak_frame);
#else
        //fprintf(stderr, "Frame No:%d ",frame);
        fprintf(stderr, "Packet No:%d \n",totPackets);
#endif

        fwrite( enc_speech, sizeof( UWORD8 ), out_bytes, bitOutFile );
        fflush( bitOutFile );

#ifdef OUTPUT_PING_PONG
        if((WORD32)enc_speech == (WORD32)enc_speech_ping)
        {
            enc_speech = enc_speech_pong;
        }
        else
        {
            enc_speech = enc_speech_ping;
        }
#endif
        /* Processig optional command line parameters -sweep, -sweep_max */

        if (sweep_bps)
        {
            enc_control.bitRate = get_bitrate_for_sweep_mode(&targetRate_bps, &sweep_bps, sweep_min, sweep_max);
        }

        /* Processig optional command line parameters -random_fec */

        if (random_fec && ((seed_fec = OPUS_RAND(seed_fec))%30==0))
        {
           seed_fec = OPUS_RAND(seed_fec);
           enc_control.SILK_InBandFEC_enabled = seed_fec%4==0;
        }

        /* Processig optional command line parameters: -silk8k_test, -silk12k_test, -silk16k_test, -hybrid24k_test, -hybrid48k_test, -celt_test, -celt_hq_test */

        curr_mode_count += frame_size;
        if (curr_mode_count > mode_switch_time && curr_mode < nb_modes_in_list-1)
        {
           curr_mode++;
           curr_mode_count = 0;
        }
        if (mode_list!=NULL)
        {
            enc_control.bandwidth     = mode_list[curr_mode][1];
            enc_control.force_mode   = mode_list[curr_mode][0];
            enc_control.force_numChannels = mode_list[curr_mode][3];
            frame_size                = mode_list[curr_mode][2];
        }

#ifdef __PROFILE__
       /* Stop after profiling the specified frame */
        if ( profile_frame > 0 && profile_frame == frame)
                    exit(0);
#endif
    }
#ifdef SCRATCH_PING_PONG
    if(scratch_ping_pong_flag == 1)
    {
        fprintf(stderr, "Scratch Ping Pong Failed \n");
    }
#endif

#ifdef __PROFILE__
    fprintf(stdout, "Peak MCPS = %f\n", mcps_peak);
    fprintf(stdout, "Peak frame = %d\n", peak_frame);
    fprintf(stdout, "Average MCPS = %f\n", mcps_ave);
#endif

#ifdef SCRATCH_ANALYSIS
    fprintf(stdout, "Max scratch usage = %d\n", scratch_max_size);
#endif


    fclose( speechInFile );
    fclose( bitOutFile );

    /* Free all allocated buffers */
    free(speech_encoder_state);
#ifndef SCRATCH_PING_PONG
    free(scratch);
#else
    free(scratch_ping);
    free(scratch_pong);
#endif
#ifndef INPUT_PING_PONG
    free(inp_speech);
#else
    free(inp_speech_ping);
    free(inp_speech_pong);
#endif
#ifndef OUTPUT_PING_PONG
    free(enc_speech);
#else
    free(enc_speech_ping);
    free(enc_speech_pong);
#endif

    return XA_NO_ERROR;
}

// Set cache attribute to Write Back No Allocate when the last argument is -wbna
void set_wbna(int *argc, char *argv[])
{
    if ( *argc > 1 && !strcmp(argv[*argc-1], "-wbna") ) {
#ifdef __XCC__
        extern char _memmap_cacheattr_wbna_trapnull;

        xthal_set_cacheattr((unsigned)&_memmap_cacheattr_wbna_trapnull);
#endif
        (*argc)--;
    }
}

int main (int   argc, char *argv[])
{
    FILE *param_file_id;
    int err_code = XA_NO_ERROR;

    char curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    WORD32 fargc, curpos;
    WORD32 processcmd = 0;

    char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
    char *pargv[XA_MAX_ARGS];
    char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH] = "";
    char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH] = "";

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

#ifdef __PROFILE__
    xt_iss_client_command("all", "disable");
#endif

    if(argc == 1) 
    {
        param_file_id = fopen(PARAMFILE, "r");
        if (param_file_id == NULL)
        {
            fprintf(stdout, "Error opened Parameter File: %s \n", PARAMFILE);
            exit(1);
        }

        /* Process one line at a time */
        while(fgets(curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id))
        {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd)
            {   /* skip it */
                continue;
            }

            while(sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF)
            {
                if(fargv[0][0]=='/' && fargv[0][1]=='/')
                    break;
                if(strcmp(fargv[0], "@echo") == 0)
                    break;
                if(strcmp(fargv[fargc], "@New_line") == 0)
                {
                    fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                          param_file_id);
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1)    /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0)
            {
                if(fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0)
            {
                if(fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Start") == 0)
            {
                processcmd = 1;
                continue;
            }

            if(strcmp(fargv[0], "@Stop") == 0)
            {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd)
            {
                int j;
                char* pb_arg_val;
                char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH] = "";
                char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH] = "";
             
                /* Copy binary name . Otherwiase if paramfile is wrong & main_process function tries to print
                help menu, it will crash since, it will not get the binary name. */
                pargv[0] = argv[0];

                for (j = 0; j < fargc; j++)
                {
                    if(!strncmp(fargv[j], "-ifile:", 7))
                    {
                        pb_arg_val = fargv[j] + 7;

                        strcpy(pb_input_file_name, "-ifile:");
                        strcat(pb_input_file_name, pb_input_file_path);
                        strcat(pb_input_file_name, pb_arg_val);
                        pargv[j+1] = pb_input_file_name;
                    }
                    else if(!strncmp(fargv[j], "-ofile:", 7))
                    {
                        pb_arg_val = fargv[j] + 7;

                        strcpy(pb_output_file_name, "-ofile:");
                        strcat(pb_output_file_name, pb_output_file_path);
                        strcat(pb_output_file_name, pb_arg_val);
                        pargv[j+1] = pb_output_file_name;
                    }
                    else
                    {
                        pargv[j+1] = fargv[j];
                    }
                }

                fprintf(stdout, "\n");
                xa_opus_enc_main_process(fargc+1, pargv);
            }
        }
        return XA_NO_ERROR;
    }
    else 
    {
        int i;
        
        for(i = 1; i < argc; i++)
        {
            if(!strncmp(argv[i], "-help", 5))
            {
                print_usage(argv );
                return (XA_NO_ERROR);
            }
        }
        err_code = xa_opus_enc_main_process(argc, argv);
        return err_code;
    }
}

