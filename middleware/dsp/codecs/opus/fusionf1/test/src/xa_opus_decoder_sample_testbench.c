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
/* Opus decoder test program */
/*****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef SCRATCH_ANALYSIS
int scratch_max_size = 0;
int scratch_curr_size = 0;
int scratch_beg = 0;
int scratch_lnew_size = 0;
int scratch_new_size = 0;
#endif

//#define INPUT_PING_PONG
//#define OUTPUT_PING_PONG
//#define SCRATCH_PING_PONG

/* Tensilica specific includes. */
#include "opus_codec/xa_opus_codec_api.h"
#ifdef ENABLE_OGG_SUPPORT
#include "ogg_lib/xa_ogg_lib_api.h"
#include "opus_header.h"
#include "xa_profiler.h"
#endif
#include "xa_type_def.h"
#include "xa_error_handler.h"

#ifdef ENABLE_OGG_SUPPORT
typedef enum 
{
    XA_RAW_OPUS_STREAM = 0,
    XA_OGG_OPUS_STREAM = 1
} xa_opus_stream_type_t;
#endif

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

#define Opus_memmove(a, b, c)  memmove((a), (b), (c))    /* Dest, Src, ByteCount */

#define OPUS_RAND(seed)                   ((WORD64)(((WORD64)907633515) +  (WORD32)(seed * 196314165)))

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
int profile_frame = 0;//1;//2822;//5488;
#endif

#ifdef __XCC__
#include <xtensa/hal.h>
#endif

#define XA_MAX_CMD_LINE_LENGTH 1024
#define XA_MAX_ARGS 50
#define PARAMFILE "paramfilesimple_decode.txt"

/* Application and testbench error handler */
#define XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED                0xFFFF8000
#define XA_TESTBENCH_FATAL_FILE_OPEN_FAILED                0xFFFF8001
#define XA_TESTBENCH_FATAL_OPUS_HDR_READ_FAILED            0xFFFF8002

#ifdef ENABLE_OGG_SUPPORT
#define OGG_INP_SIZE    4*1024
#endif

/* These are codec library error*/
VOID xa_opus_codec_error_handler_init();
extern  xa_error_info_struct xa_opus_codec_error_info;
#ifdef ENABLE_OGG_SUPPORT
VOID xa_ogg_lib_error_handler_init();
extern  xa_error_info_struct xa_ogg_lib_error_info;
#endif

/* These are testbench error*/
VOID xa_testbench_error_handler_init();
extern xa_error_info_struct xa_testbench_error_info;


/* Function to convert a little endian int16 to a */
/* big endian int16 or vica verca                 */
void swap_endian(
    WORD16       vec[],
    WORD         len
)
{
    WORD i;
    WORD16 tmp;
    UWORD8 *p1, *p2;

    for( i = 0; i < len; i++ ){
        tmp = vec[ i ];
        p1 = (UWORD8 *)&vec[ i ]; p2 = (UWORD8 *)&tmp;
        p1[ 0 ] = p2[ 1 ]; p1[ 1 ] = p2[ 0 ];
    }
}

#ifndef _SYSTEM_IS_BIG_ENDIAN
static WORD32 word16_swap(WORD32 data)
{
    return (data << 16)+ ((data >> 16) & 0xFFFF);
}
#endif

static void print_usage( char* argv[] )
{
    printf( "Usage: %s -fs:<sampling rate (8000/12000/16000/24000/48000) Hz; default: 48000> -numch:<channels (1-8); default: 2> "
        "[options] -ifile:<input_file> -ofile:<output_file>\n\n", argv[0]);
    printf( "Options:\n" );
    printf( "-inbandfec:<fec_flag (0/1)>  : enable or disable SILK inband FEC (non-zero value is treated as 1)\n" );
    printf( "-loss:<perc>                 : simulate packet loss, in percent (0-100); default: 0\n" );
#ifdef ENABLE_OGG_SUPPORT
    printf( "-btype:<ogg/raw>             : bitstream type, ogg or raw; default: raw\n" );
    printf( "-maxpage:<n [1,1024]>        : max page size - n in kBytes; default: 64\n");
#endif
    printf( "-strmap:<map>                : Output channel map, string of max. 8 characters, range for each character 0-8\n" );
    printf( "                             : 0-7 to route the corresponding channel from decoder output, 8 for silence,\n" );
    printf( "                             : for streams with less than 8 channels (n) range becomes 0-(n-1)\n" );
}

#ifdef ENABLE_OGG_SUPPORT
#define readint(buf, base) (((buf[base+3]<<24)&0xff000000)| \
                           ((buf[base+2]<<16)&0xff0000)| \
                           ((buf[base+1]<<8)&0xff00)| \
                           (buf[base]&0xff))

static void print_comments(char *comments, int length)
{
   char *c=comments;
   int len, i, nb_fields, err=0;

   if (length<(8+4+4))
   {
      fprintf (stderr, "Invalid/corrupted comments\n");
      return;
   }
   if (strncmp((char *)c, "OpusTags", 8) != 0)
   {
      fprintf (stderr, "Invalid/corrupted comments\n");
      return;
   }
   c += 8;
   fprintf(stderr, "Encoded with ");
   len=readint(c, 0);
   c+=4;
   if (len < 0 || len>(length-16))
   {
      fprintf (stderr, "Invalid/corrupted comments\n");
      return;
   }
   err&=fwrite(c, 1, len, stderr)!=(unsigned)len;
   c+=len;
   fprintf (stderr, "\n");
   /*The -16 check above makes sure we can read this.*/
   nb_fields=readint(c, 0);
   c+=4;
   length-=16+len;
   if (nb_fields < 0 || nb_fields>(length>>2))
   {
      fprintf (stderr, "Invalid/corrupted comments\n");
      return;
   }
   for (i=0;i<nb_fields;i++)
   {
      if (length<4)
      {
         fprintf (stderr, "Invalid/corrupted comments\n");
         return;
      }
      len=readint(c, 0);
      c+=4;
      length-=4;
      if (len < 0 || len>length)
      {
         fprintf (stderr, "Invalid/corrupted comments\n");
         return;
      }
      err&=fwrite(c, 1, len, stderr)!=(unsigned)len;
      c+=len;
      length-=len;
      fprintf (stderr, "\n");
   }
}
#endif


#ifdef WIN32
#define ALIGN(x) _declspec(align(x))
#else
#define ALIGN(x) __attribute__((aligned(x)))
#endif
int xa_opus_dec_main_process( int argc, char* argv[] )
{
    size_t counter = 0;
    WORD32 args = 0, totPackets = 0, i = 0;
    WORD16 len = 0, tot_len = 0;
    WORD32 nBytes = 0;
    pWORD8 payload = NULL;
    pWORD8 payloadEnd = NULL, payloadToDec = NULL;
    WORD16 nBytesPerPacket[ XA_OPUS_MAX_PACKET_IN_INP_BUF ];
    pWORD16 outPtr= NULL;
    char speechOutFileName[ 1024 ] = "NULL", bitInFileName[ 1024 ] = "NULL";
    FILE *bitInFile= NULL, *speechOutFile=NULL;
    WORD32 frames = 0, frame = 0, lost;
    
    /* default settings */
    WORD32 packetLoss_perc = 0;

    WORD32 handle_size = 0, scratch_size = 0, enc_speech_size = 0, synth_speech_size = 0;
    xa_codec_handle_t speech_decoder_state = NULL;
    pVOID scratch = NULL;
#ifdef SCRATCH_PING_PONG
    pVOID scratch_ping = NULL, scratch_pong = NULL;
    WORD32 scratch_ping_pong_flag = 0;
#endif
    pWORD16 synth_speech = NULL;
#ifdef OUTPUT_PING_PONG
    pWORD16 synth_speech_ping = NULL, synth_speech_pong = NULL;
#endif
    pUWORD8 enc_speech = NULL;
#ifdef INPUT_PING_PONG
    pUWORD8 enc_speech_ping = NULL, enc_speech_pong = NULL;
#endif
    xa_opus_dec_control_t dec_control;
    xa_error_info_struct *p_proc_codec_err_info;
    xa_error_info_struct *p_proc_testbench_err_info;
    XA_ERRORCODE error_code = XA_NO_ERROR;
#ifdef ENABLE_OGG_SUPPORT
    xa_error_info_struct *p_proc_ogg_err_info;
    xa_opus_stream_type_t stream_type;
    xa_codec_handle_t ogg_handle = NULL;
    xa_ogg_parse_init_cfg_t ogg_cfg;
    pVOID ogg_scratch = NULL;
    WORD32 read_ogg_inp = 1;
    WORD32 ogg_handle_size = 0, ogg_scratch_size = 0;
    pUWORD8 p_ogg_inp_buf = NULL;
    /* Temporary buffer for processing header and comment packets from ogg stream, size should be at least max of header and comment packet size */
    UWORD8 tmp_ogg_out_buf[3*1024];
    WORD32 input_over = 0;
    OpusHeader opus_hdr;
    WORD32 gran_offset = 0;
    WORD32 preskip = 0, tmp_skip = 0;
    WORD16 tot_samps = 0;
    WORD64 page_gran_pos[2] = {0};
    WORD64 max_out = 0;
    WORD64 link_out = 0;
    xa_ogg_parse_param_id_t param_id;
    WORD32 eos;
    XA_ERRORCODE cfg_err;
    xa_profiler ogg_prof;
#endif
    UWORD32 seed = 0;
    WORD32 stream_map_set = 0;

#ifdef __PROFILE__
    double u_seconds;
    struct tms start, stop;
    clock_t cycles;
    double mcps_peak = 0, mcps_curr = 0, mcps_ave = 0, mcps_sum = 0;
    int peak_frame = 0;
#endif
        
    /* ******************************************************************/
    /* Initialize the error handler testbench                           */
    /* ******************************************************************/
    p_proc_testbench_err_info = NULL;
    p_proc_testbench_err_info = (xa_error_info_struct*)&xa_testbench_error_info;
    xa_testbench_error_handler_init();

    /* ******************************************************************/
    /* Initialize the error handler library                             */
    /* ******************************************************************/
    p_proc_codec_err_info = NULL;
    p_proc_codec_err_info = (xa_error_info_struct*)&xa_opus_codec_error_info;
    xa_opus_codec_error_handler_init();
#ifdef  ENABLE_OGG_SUPPORT
    p_proc_ogg_err_info = NULL;
    p_proc_ogg_err_info = (xa_error_info_struct*)&xa_ogg_lib_error_info;
    xa_ogg_lib_error_handler_init();
    INIT_XA_PROFILER(ogg_prof, (char *) "OGG-PARSE");
#endif

    /* ******************************************************************/
    /* Library Info print.                                              */
    /* ******************************************************************/
    fprintf(stderr, "\n");
    fprintf(stderr, "%s library version %s\n", xa_opus_get_lib_name_string(),
        xa_opus_get_lib_version_string());
    fprintf(stderr, "API version: %s\n", xa_opus_get_lib_api_version_string());
    fprintf(stderr, "Cadence Design Systems, Inc. http://www.cadence.com\n");
    fprintf(stderr, "\n");

    /* Check for No Of arguments */
    if( argc < 3 ) 
    {
        print_usage( argv );
        exit( 0 );
    }

    /************************************************/
    /* Reset config and control structures          */
    /************************************************/
    memset( &dec_control, 0, sizeof(dec_control) );
    memset( dec_control.stream_map, 255, sizeof(dec_control.stream_map) );
#ifdef ENABLE_OGG_SUPPORT
    memset(&ogg_cfg, 0, sizeof(xa_ogg_parse_init_cfg_t));
    memset(&opus_hdr, 0, sizeof(OpusHeader));
    /* Default stream type - raw */
    stream_type = XA_RAW_OPUS_STREAM;
    /* Default Ogg page size */
    ogg_cfg.max_page_size = 64;      /* in KB */
#endif

    /************************************************/
    /* Get arguments                                */
    /************************************************/

    args = 1;

    while( args < argc ) 
    {
        if( strncmp( argv[ args ], "-fs:",4 ) == 0 )
        {
            sscanf( argv[ args ] + 4, "%d", &dec_control.API_sampleRate );
			if (dec_control.API_sampleRate != 8000  && dec_control.API_sampleRate != 12000 &&
			dec_control.API_sampleRate != 16000 && dec_control.API_sampleRate != 24000 &&
			dec_control.API_sampleRate != 48000)
			{
				fprintf(stderr, "Sampling frequency not supported: %d\n", dec_control.API_sampleRate);
                exit(0);
			}
            args++;
        } 
        else if( strncmp( argv[ args ], "-numch:",7 ) == 0 )
        {
            sscanf( argv[ args ] + 7, "%d", &dec_control.API_numChannels );
            args++;
			if(dec_control.API_numChannels < 1 || dec_control.API_numChannels > 8)
			{
				printf("Error: Invalid number of channels setting: %d\n", dec_control.API_numChannels);
				exit(0);
			}	
        }
        else if( strncmp( argv[ args ], "-inbandfec:",11 ) == 0 )
        {
            //INBandFEC_enabled = 1;
            sscanf( argv[ args ] + 11, "%d", &dec_control.SILK_InBandFEC_enabled );
            args++;
        } 
        else if( strncmp( argv[ args ], "-loss:",6 ) == 0 ) 
        {
            sscanf( argv[ args ] + 6, "%d", &packetLoss_perc );
            args++;
            if (packetLoss_perc < 0 || packetLoss_perc > 100) 
            {
                printf("Error: Invalid packet loss setting: %d\n", packetLoss_perc);
                exit (0);
            }
        } 
        else if( strncmp( argv[ args ], "-ifile:",7 ) == 0 ) 
        {
			if((strlen(argv[ args ])-7) > 1024) 
   			{
        		printf( "Error: Input file name length too large, max allowed length is 1024\n");
        		exit( 0 );
    		}
            sscanf( argv[ args ] + 7, "%s", bitInFileName );
            args++;
        } 
        else if( strncmp( argv[ args ], "-ofile:",7 ) == 0 ) 
        {
			if((strlen(argv[ args ])-7) > 1024) 
   			{
        		printf( "Error: Output file name length too large, max allowed length is 1024\n");
        		exit( 0 );
    		}
            sscanf( argv[ args ] + 7, "%s", speechOutFileName );
            args++;
        }
#ifdef ENABLE_OGG_SUPPORT
        else if( strcmp( argv[ args ], "-btype:ogg" ) == 0 )
        {
            stream_type = XA_OGG_OPUS_STREAM;
            dec_control.no_range_dec_state = 1;
            args++;
        }
        else if( strcmp( argv[ args ], "-btype:raw" ) == 0 )
        {
            stream_type = XA_RAW_OPUS_STREAM;
            args++;
        }
        else if( strncmp( argv[ args ], "-maxpage:",9 ) == 0 )
        {
            sscanf( argv[ args ] + 9, "%d", &ogg_cfg.max_page_size );
			if (ogg_cfg.max_page_size < 1 || ogg_cfg.max_page_size > 1024) 
            {
                printf("Error: Max Ogg page size setting: %d\n", ogg_cfg.max_page_size);
                exit (0);
            }
            args++;
        }
        else if( strncmp( argv[ args ], "-strmap:",8 ) == 0 )
        {
            UWORD8 stream_map[9];
            sscanf( argv[ args ] + 8, "%s", stream_map );
            int idx;
            int map_len = strlen((char *)stream_map);
            map_len = map_len <= 8 ? map_len : 8;
            for( idx = 0; idx < map_len; idx++ )
            {
                if(stream_map[idx] == '8')
                    dec_control.stream_map[idx] = 255;
                else
                    dec_control.stream_map[idx] = stream_map[idx] - '0';
            }
            stream_map_set = 1;
            args++;
        }
#endif 
        else 
        {
            printf( "Error: unrecognized setting: %s\n\n", argv[ args ] );
            print_usage( argv );
            exit( 0 );
        }
    }
    
    printf( "Input:                       %s\n", bitInFileName );
    printf( "Output:                      %s\n", speechOutFileName );
    
    /************************************************************************
    * Opening Input bitstream File                                          *
    *************************************************************************/
    /* Open files */
    bitInFile = fopen( bitInFileName, "rb" );
    if( bitInFile == NULL ) 
    {
        printf( "Error: could not open input file %s\n", bitInFileName );
        exit( 0 );
    }

    /************************************************************************/
    /*  Opening Output speech File                                          */
    /************************************************************************/
    speechOutFile = fopen( speechOutFileName, "wb" );
    if( speechOutFile == NULL ) 
    {
        printf( "Error: could not open output file %s\n", speechOutFileName );
        exit( 0 );
    }

    
    /* There is no losing of packet at start time */
    dec_control.lostFlag = 0;

    /************************************************************
    *               Get Memory sizes from Ogg Parser            *
    ************************************************************/
#ifdef ENABLE_OGG_SUPPORT
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        ogg_handle_size = xa_ogg_parse_get_handle_byte_size(&ogg_cfg);
        ogg_scratch_size = xa_ogg_parse_get_scratch_byte_size(&ogg_cfg);
    }
#endif
#ifdef ENABLE_OGG_SUPPORT
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        fprintf(stdout, "\nOgg Parser runtime memory usage:");
        fprintf(stdout, "\nPersistent state size: %6d bytes\n", ogg_handle_size);
        fprintf(stdout, "Scratch buffer size:   %6d bytes\n", ogg_scratch_size);
    }
#endif

    /************************************************************
    *             Allocate Memory for Ogg Parser                *
    ************************************************************/
#ifdef ENABLE_OGG_SUPPORT
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        ogg_handle = (xa_codec_handle_t)malloc(ogg_handle_size);
        if(ogg_handle == NULL) 
        {
            _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
        }
        if(ogg_scratch_size != 0)
        {
            ogg_scratch = (pVOID)malloc(ogg_scratch_size);
            if(ogg_scratch == NULL) 
            {
                _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
            }
        }
        p_ogg_inp_buf = (pUWORD8)malloc(OGG_INP_SIZE);
        if(p_ogg_inp_buf == NULL)
        {
            _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
        }
    }
#endif
    /* Allocate memory for opus decoder input buffer which is used as output buffer for ogg parser,
    allocation is done here because it doesn't depend on any runtime parameter and is used for getting 
    header and comment packets from ogg parser */

    /*******************************************************************
     *                  Initialization of Ogg Parser                   *
     *******************************************************************/
#ifdef ENABLE_OGG_SUPPORT
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        error_code = xa_ogg_parse_init(ogg_handle, ogg_scratch, &ogg_cfg);
        _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);
    }
#endif

#ifdef ENABLE_OGG_SUPPORT
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        WORD32 packet_count = 0;
        read_ogg_inp = 1;
        while(1)
        {
            nBytes = sizeof(tmp_ogg_out_buf);
            counter = 0;
            if(read_ogg_inp && !input_over)
            {
                counter = fread(p_ogg_inp_buf, sizeof(WORD8), OGG_INP_SIZE, bitInFile);
                read_ogg_inp = 0;
                if(counter < OGG_INP_SIZE)
                {
                    input_over = 1;
                }
            }
            error_code = xa_ogg_parse_process(ogg_handle,
                                              p_ogg_inp_buf,
                                              (pUWORD8)tmp_ogg_out_buf,
                                              (pWORD32)&counter,
                                              &nBytes);
            _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);

            if(error_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                read_ogg_inp = 1;

            if(nBytes > 0)
            {
                if(packet_count == 0)
                {
                    /* parse header */
                    if (!opus_header_parse((pUWORD8)tmp_ogg_out_buf, nBytes, &opus_hdr))
                    {
                        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "", XA_TESTBENCH_FATAL_OPUS_HDR_READ_FAILED);
                    }
                    /* If sample rate is not passed on command line, pick input_sample_rate from ogg stream header */
                    if(dec_control.API_sampleRate == 0)
                        dec_control.API_sampleRate = opus_hdr.input_sample_rate;
                    if(dec_control.API_numChannels == 0)
                        dec_control.API_numChannels = opus_hdr.channels;
                    dec_control.nb_streams         = opus_hdr.nb_streams;
                    dec_control.nb_coupled         = opus_hdr.nb_coupled;
                    dec_control.gain               = opus_hdr.gain;
                    dec_control.version            = opus_hdr.version;
                    /* Channel mapping 1 implies multistream input, 0 implies single stream input */
                    dec_control.channel_mapping    = opus_hdr.channel_mapping;
                    /* Set stream_map from opus header if not passed on command line */
                    if(stream_map_set == 0)
                        memcpy(dec_control.stream_map, opus_hdr.stream_map, 8);

                    gran_offset                    = opus_hdr.preskip;
                    preskip                        = opus_hdr.preskip;
                }
                else if(packet_count == 1)
                {
                    fprintf(stdout, "\nOgg Stream Comments: \n");
                    /* Print comments */
                    print_comments((char *)tmp_ogg_out_buf, nBytes);
                    break;
                }
                packet_count++;
            }
            if(input_over && read_ogg_inp)
            {
                /* File ended, break */
                printf( "Error: Ran out of input without finding header and comment packets");
                exit(0);
            }
        }
    }
    else
#endif /* #ifdef ENABLE_OGG_SUPPORT */
    {
        /* If stream type is XA_RAW_OPUS_STREAM and sample rate and number of channels are not passed from command line revert to default */
        if(dec_control.API_sampleRate == 0)
            dec_control.API_sampleRate = 48000;     /* Default for raw opus stream */
        if(dec_control.API_numChannels == 0)
            dec_control.API_numChannels = 2;        /* Default for raw opus stream */
    }
    /* Channel mapping 0 (default) implies single stream input, nb_streams and nb_coupled should be set according to API_numChannels */
    if(dec_control.channel_mapping == 0)
    {
        dec_control.nb_streams = 1;
        if(dec_control.API_numChannels == 2)
            dec_control.nb_coupled = 1;
        else
            dec_control.nb_coupled = 0;
    }

    printf( "API sampling rate:                             %d Hz\n",  dec_control.API_sampleRate );
    printf( "API number of channels:                        %d\n",     dec_control.API_numChannels );

    /************************************************************
    *            Get Memory sizes from Opus library             *
    ************************************************************/
    handle_size = xa_opus_dec_get_handle_byte_size(dec_control.nb_streams, dec_control.nb_coupled);
    scratch_size = xa_opus_dec_get_scratch_byte_size(dec_control.channel_mapping);
#ifdef SCRATCH_ANALYSIS
    /* Add extra scratch size to be safe */
    scratch_size += 30*1024;
#endif
    enc_speech_size = XA_OPUS_MAX_DEC_INP_BYTES*dec_control.nb_streams;
    synth_speech_size = XA_OPUS_MAX_BYTES_CHANNEL_PACKET * dec_control.API_numChannels;

    fprintf(stdout, "\nOpus Decoder runtime memory usage:");
    fprintf(stdout, "\nPersistent state size: %6d bytes\n", handle_size);
    fprintf(stdout, "Scratch buffer size:   %6d bytes\n", scratch_size);
    fprintf(stdout, "Input buffer size:     %6d bytes\n", enc_speech_size);
    fprintf(stdout, "Output buffer size:    %6d bytes\n\n", synth_speech_size);

    /************************************************************
    *                     Allocate Memory                       *
    ************************************************************/
    speech_decoder_state = (xa_codec_handle_t)malloc(handle_size);
    if(speech_decoder_state == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API state alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }

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

#ifndef INPUT_PING_PONG
    enc_speech = (pUWORD8)malloc(enc_speech_size);
    if(enc_speech == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
#else
    enc_speech_ping = (pUWORD8)malloc(enc_speech_size);
    if(enc_speech_ping == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    /* To avoid ferrets while switching buffers for first time */
    memset(enc_speech_ping, 0xce, enc_speech_size);
    enc_speech_pong = (pUWORD8)malloc(enc_speech_size);
    if(enc_speech_pong == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API input alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    enc_speech = enc_speech_ping;
#endif

#ifndef OUTPUT_PING_PONG
    synth_speech = (pWORD16)malloc(synth_speech_size);
    if(synth_speech == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
#else
    synth_speech_ping = (pWORD16)malloc(synth_speech_size);
    if(synth_speech_ping == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    synth_speech_pong = (pWORD16)malloc(synth_speech_size);
    if(synth_speech_pong == NULL) 
    {
        _XA_HANDLE_ERROR(p_proc_testbench_err_info, "API output alloc", XA_TESTBENCH_FATAL_MEM_ALLOC_FAILED);
    }
    synth_speech = synth_speech_ping;
#endif

#ifdef SCRATCH_ANALYSIS
    scratch_beg = (int)scratch;
#endif

    /*******************************************************************
     *                  Initialization of Opus Decoder                 *
     *******************************************************************/
    error_code = xa_opus_dec_init(speech_decoder_state, &dec_control);
    _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);

    /* Set i/p buffer pointer for opus decoder */
    totPackets = 0;
    payloadEnd = payload = (pWORD8)(enc_speech);

    /* Initialized array with -1 */
    for( i = 0; i < XA_OPUS_MAX_PACKET_IN_INP_BUF; i++ ) 
    {
        nBytesPerPacket[i] = -1;
    }

    /* set output buffer pointer for opus decoder */
    outPtr = synth_speech;

#ifdef ENABLE_OGG_SUPPORT
    /* Put one packet in input buffer to opus decoder */
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        read_ogg_inp = 0;
        while(1)
        {
            nBytes = enc_speech_size;
            counter = 0;
            if(read_ogg_inp && !input_over)
            {
                counter = fread(p_ogg_inp_buf, sizeof(WORD8), OGG_INP_SIZE, bitInFile);
                read_ogg_inp = 0;
                if(counter < OGG_INP_SIZE)
                {
                    input_over = 1;
                }
            }
            error_code = xa_ogg_parse_process(ogg_handle,
                                              p_ogg_inp_buf,
                                              (pUWORD8)payloadEnd,
                                              (pWORD32)&counter,
                                              &nBytes);
            _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);

            if(error_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                read_ogg_inp = 1;

            if(nBytes > 0)
            {
                /* Got one packet, break */
                break;
            }
            if(input_over && read_ogg_inp) 
            {
                /* File ended, exit */
                printf( "Error: Ran out of input, no opus packet found \n");
                exit(0);
            }
        }
        seed = OPUS_RAND(seed);
        if (nBytes == 0 || (packetLoss_perc>0 && seed%100 < (UWORD32)packetLoss_perc))
        {
            nBytesPerPacket[ 0 ] = 0;
        }
        else
        {
            nBytesPerPacket[ 0 ] = nBytes;
            payloadEnd          += nBytes;
        }
        param_id.type = XA_OGG_PARSE_PARAM_PAGE_GRANULE;
        cfg_err = xa_ogg_parse_get_param(ogg_handle, param_id, &page_gran_pos[0]);
        _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", cfg_err);
    }
    else
#endif /* #ifdef ENABLE_OGG_SUPPORT */
    {
        /* Simulate the jitter buffer holding MAX_FEC_DELAY + 1 packets. Preload the first packet */
        /* Read payload size */
        counter = fread( &nBytes, sizeof( WORD32 ), 1, bitInFile );

#ifndef _SYSTEM_IS_BIG_ENDIAN
        swap_endian( (WORD16*) &nBytes, 2 );
        nBytes = word16_swap(nBytes);
#endif


        if ((nBytes < 0) || (nBytes > (XA_OPUS_MAX_BYTES_ENCODED_PACKET - 4)))
        {
            printf( "Error: Invalid Payload size: %d\n", nBytes);
            exit( 0 );
        }

        /* Actual payload size is 4 bytes longer */
        nBytes += 4;

        /* Read payload */
        counter = fread( payloadEnd, sizeof( UWORD8 ), nBytes, bitInFile );
        if( ( WORD16 )counter < nBytes ) 
        {
            printf( "Error: Ran out of input. Expecting %d bytes, got %d \n", nBytes, counter);
            exit( 0 );
        }
        seed = OPUS_RAND(seed);
        if ((nBytes-4) == 0 || (packetLoss_perc>0 && seed%100 < (UWORD32)packetLoss_perc))
        {
            nBytesPerPacket[ 0 ] = 0;
        }
        else
        {
            nBytesPerPacket[ 0 ] = nBytes;
            payloadEnd          += nBytes;
        }
    }

    /*******************************************************************
     *                    Execution of Opus Decoder                    *
     *******************************************************************/

    while( 1 ) 
    {
#ifdef INPUT_PING_PONG
        if((WORD32)enc_speech == (WORD32)enc_speech_ping)
        {
            enc_speech = enc_speech_pong;
            memcpy(enc_speech, enc_speech_ping, enc_speech_size);
            memset(enc_speech_ping, 0xac, enc_speech_size);
        }
        else
        {
            enc_speech = enc_speech_ping;
            memcpy(enc_speech, enc_speech_pong, enc_speech_size);
            memset(enc_speech_pong, 0xac, enc_speech_size);
        }
#endif
        /* Set Input Pointer */
        payloadEnd = (pWORD8)enc_speech + (payloadEnd-payload);
        payload = (pWORD8)(enc_speech);

#ifdef ENABLE_OGG_SUPPORT
        if(stream_type == XA_OGG_OPUS_STREAM)
        {
            read_ogg_inp = 0;
            while(1)
            {
                nBytes = enc_speech_size;
                counter = 0;
                if(read_ogg_inp && !input_over)
                {
                    counter = fread(p_ogg_inp_buf, sizeof(WORD8), OGG_INP_SIZE, bitInFile);
                    read_ogg_inp = 0;
                    if( counter < OGG_INP_SIZE ) 
                    {
                        input_over = 1;
                    }
                }
                START_TIME_XA_PROFILER(ogg_prof);                
                error_code = xa_ogg_parse_process(ogg_handle,
                                                p_ogg_inp_buf,
                                                (pUWORD8)payloadEnd,
                                                (pWORD32)&counter,
                                                &nBytes);
                STOP_TIME_XA_PROFILER(ogg_prof);
                _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);

                if(error_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                {
                    if(input_over == 1)
                    {
                        break;
                    }
                    read_ogg_inp = 1;
                }

                if(nBytes > 0)
                {
                    /* Got one packet, break */
                    break;
                }
                
                param_id.type = XA_OGG_PARSE_PARAM_END_OF_STREAM;
                cfg_err = xa_ogg_parse_get_param(ogg_handle, param_id, &eos);
                _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", cfg_err);
                if (eos) fprintf(stderr, "End of stream reached\n"); 

            }
            if(error_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA && input_over == 1)
                break;
            /* Simulate losses */
            seed = OPUS_RAND(seed);
            if (nBytes == 0 || (packetLoss_perc>0 && seed%100 < (UWORD32)packetLoss_perc))
            {
                nBytesPerPacket[ 1 ] = 0;
            }
            else
            {
                nBytesPerPacket[ 1 ] = nBytes;
                payloadEnd          += nBytes;
            }
            param_id.type = XA_OGG_PARSE_PARAM_PAGE_GRANULE;
            cfg_err = xa_ogg_parse_get_param(ogg_handle, param_id, &page_gran_pos[1]);
            _XA_HANDLE_ERROR(p_proc_ogg_err_info, "", cfg_err);
        }
        else
#endif /* #ifdef ENABLE_OGG_SUPPORT */
        {
            /* Read payload size */
            counter = fread( &nBytes, sizeof( WORD32 ), 1, bitInFile );

#ifndef _SYSTEM_IS_BIG_ENDIAN
            swap_endian( (WORD16*) &nBytes, 2 );
            nBytes = word16_swap(nBytes);
#endif
            if( counter < 1 ) 
            {
                break;
            }


            /* Read payload */
            if ((nBytes < 0) || (nBytes > (XA_OPUS_MAX_BYTES_ENCODED_PACKET - 4)))
            {
                printf( "Error: Invalid Payload size: %d\n", nBytes);
                break;
            }

            /* Actual payload size is 4 bytes longer */
            nBytes += 4;
    
            counter = fread( payloadEnd, sizeof( UWORD8 ), nBytes, bitInFile );
            if( ( WORD16 )counter < nBytes ) 
            {
                break;
            }
            /* Simulate losses */
            seed = OPUS_RAND(seed);
            if ((nBytes-4) == 0 || (packetLoss_perc>0 && seed%100 < (UWORD32)packetLoss_perc))
            {
                nBytesPerPacket[ 1 ] = 0;
            }
            else
            {
                nBytesPerPacket[ 1 ] = nBytes;
                payloadEnd          += nBytes;
            }
        }

        if( nBytesPerPacket[ 0 ] == 0 ) 
        {
            lost = 1;
            nBytes = nBytesPerPacket[ 1 ];
        }
        else
        {
            lost = 0;
            nBytes = nBytesPerPacket[ 0 ];
        }

        payloadToDec = payload;

        /* Opus decoder */
        outPtr = synth_speech;
        tot_len = 0;
        
        if( lost == 0 ) 
        {
            /* No Loss: Decode all frames in the packet */
            frames = 0;
            do 
            {

                /* Decode frame */
                dec_control.lostFlag = 0; // Indicate no-loss frame
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
#ifdef __PROFILE__
                if ((profile_frame == 0) || (profile_frame > 0 && profile_frame == (frame+1)))
                {
                    xt_iss_client_command("all", "enable");
                    times(&start);
                }
#endif
                
                error_code = xa_opus_dec( speech_decoder_state, 
                                          (pUWORD8)payloadToDec, 
                                          outPtr, nBytes, &dec_control, &len, scratch );

#ifdef __PROFILE__
                if ((profile_frame == 0) || (profile_frame > 0 && profile_frame == (frame+1)))
                {
                    times(&stop);
                    xt_iss_client_command("all", "disable");
                }
#endif

                _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
                frame++;

#ifdef __PROFILE__
                if (len != 0 && ((profile_frame == 0) || (profile_frame > 0 && profile_frame == frame)))
                {
                    cycles = stop.tms_utime - start.tms_utime;
                    u_seconds = ((double) len / dec_control.API_numChannels * 1000000) / dec_control.API_sampleRate;
                    mcps_curr = (double)cycles / u_seconds;
                    mcps_sum += mcps_curr;
                    mcps_ave = mcps_sum / frame;
                    if (mcps_peak < mcps_curr) 
                    {
                        mcps_peak = mcps_curr;
                        peak_frame = frame;
                    }
                }

                fprintf(stdout, "[%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",frame,mcps_curr, mcps_ave, mcps_peak, peak_frame);

                /* Stop after profiling the specified frame */
                if ( profile_frame > 0 && profile_frame == frame)
                    exit(0);
#else
                fprintf(stderr, "[%d] \n",frame);

#endif
                frames++;
                outPtr  += len;
                tot_len += len;
                if( frames > XA_OPUS_MAX_FRAMES_PER_PACKET )
                {
                    /* Hack for corrupt stream that could generate too many frames */
                    outPtr  = synth_speech;
                    tot_len = 0;
                    frames  = 0;
                }
                /* Until last 20 ms frame of packet has been decoded */
            } while( dec_control.moreInternalDecoderFrames );
        } 
        else 
        {
            /* Loss: Decode enough frames to cover one packet duration */
            for( i = 0; i < dec_control.framesPerPacket; i++ ) 
            {
                /* Decode frame */
                dec_control.lostFlag = 1; // indicate loss packet.

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
#ifdef __PROFILE__
                if ((profile_frame == 0) || (profile_frame > 0 && profile_frame == (frame+1)))
                {
                    xt_iss_client_command("all", "enable");
                    times(&start);
                }
#endif
                error_code = xa_opus_dec( speech_decoder_state, 
                                          (pUWORD8)payloadToDec, outPtr, nBytes, &dec_control, &len, scratch );

#ifdef __PROFILE__
                if ((profile_frame == 0) || (profile_frame > 0 && profile_frame == (frame+1)))
                {
                    times(&stop);
                    xt_iss_client_command("all", "disable");
                }
#endif
                _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
                frame++;

#ifdef __PROFILE__
                if (len != 0 && ((profile_frame == 0) || (profile_frame > 0 && profile_frame == frame)))
                {
                    cycles = stop.tms_utime - start.tms_utime;
                    u_seconds = ((double) len / dec_control.API_numChannels * 1000000) / dec_control.API_sampleRate;
                    if (u_seconds) mcps_curr = (double)cycles / u_seconds;
                    mcps_sum += mcps_curr;
                    mcps_ave = mcps_sum / frame;
                    if (mcps_peak < mcps_curr) 
                    {
                        mcps_peak = mcps_curr;
                        peak_frame = frame;
                    }
                }

                fprintf(stdout, "Lost:[%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",frame,mcps_curr, mcps_ave, mcps_peak, peak_frame);

                /* Stop after profiling the specified frame */
                if ( profile_frame > 0 && profile_frame == frame)
                    exit(0);
#else
                fprintf(stderr, "Lost:[%d] \n",frame);

#endif
                outPtr  += len;
                tot_len += len;
            }
        }
        totPackets++;

#ifdef ENABLE_OGG_SUPPORT
        if(stream_type == XA_OGG_OPUS_STREAM)
        {
            static int prv_output_samples = 0;
            COMPUTE_MHZ_XA_PROFILER(ogg_prof, prv_output_samples, dec_control.API_sampleRate, 0);
            prv_output_samples = tot_len/dec_control.API_numChannels;
        }

        if(stream_type == XA_OGG_OPUS_STREAM)
        {
            tot_samps = tot_len/dec_control.API_numChannels;
            tmp_skip = preskip<tot_samps?preskip:tot_samps;
            preskip -= tmp_skip;
            tot_samps -= tmp_skip;
            /* Granule position and preskip are always assuming 48 KHz sampling rate */
            max_out = (((page_gran_pos[0]-gran_offset)*dec_control.API_sampleRate)/48000)-link_out;
            max_out = max_out>0?max_out:0;

            tot_samps = tot_samps<max_out?tot_samps:max_out;

            link_out += tot_samps;
            page_gran_pos[0] = page_gran_pos[1];
            tot_len = tot_samps*dec_control.API_numChannels;
        }
#endif
#ifdef SCRATCH_PING_PONG
        if(scratch_ping_pong_flag == 1)
        {
            fprintf(stderr, "Scratch Ping Pong Failed \n");
            break;
        }
#endif
        /* Write output to file */
#ifdef _SYSTEM_IS_BIG_ENDIAN
        swap_endian( synth_speech, tot_len );
#endif
#ifdef ENABLE_OGG_SUPPORT
        if(stream_type == XA_OGG_OPUS_STREAM)
        {
            pWORD16 out_speech = (pWORD16)synth_speech;
            out_speech += tmp_skip*dec_control.API_numChannels;
            fwrite( out_speech, sizeof( WORD16 ), tot_len, speechOutFile );
        }
        else
#endif
        {
            fwrite( synth_speech, sizeof( WORD16 ), tot_len, speechOutFile );
        }
        fflush( speechOutFile );

#ifdef OUTPUT_PING_PONG
        if((WORD32)synth_speech == (WORD32)synth_speech_ping)
        {
            synth_speech = synth_speech_pong;
        }
        else
        {
            synth_speech = synth_speech_ping;
        }
#endif

        /* Update buffers */
        Opus_memmove( payload, &payload[ nBytesPerPacket[ 0 ] ], nBytesPerPacket[ 1 ] * sizeof( UWORD8 ) );
        payloadEnd -= nBytesPerPacket[ 0 ];
        nBytesPerPacket[ 0 ] = nBytesPerPacket[ 1 ];
    }

    /* Empty the recieve buffer */
    if (dec_control.SILK_InBandFEC_enabled == 0)
    {
        if( nBytesPerPacket[ 0 ] == 0 ) 
        {
            lost = 1;
            nBytes = nBytesPerPacket[ 1 ];
        }
        else
        {
            lost = 0;
            nBytes = nBytesPerPacket[ 0 ];
        }

        payloadToDec = payload;

        /* Opus decoder */
        outPtr  = synth_speech;
        tot_len = 0;

        if( lost == 0 ) 
        {
            /* No loss: Decode all frames in the packet */
            frames = 0;
            do 
            {
#ifdef __PROFILE__
                xt_iss_client_command("all", "enable");
                times(&start);
#endif
                /* Decode 20 ms */
                dec_control.lostFlag = 0;
                error_code = xa_opus_dec(speech_decoder_state, (pUWORD8)payloadToDec, outPtr, nBytes, &dec_control, &len, scratch );

#ifdef __PROFILE__
                times(&stop);
                xt_iss_client_command("all", "disable");
#endif
                _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
                frame++;

#ifdef __PROFILE__
                if ((profile_frame == 0) || (profile_frame > 0 && profile_frame == frame))
                {
                    cycles = stop.tms_utime - start.tms_utime;
                    u_seconds = ((double) len / dec_control.API_numChannels * 1000000) / dec_control.API_sampleRate;
                    if (u_seconds) mcps_curr = (double)cycles / u_seconds;
                    mcps_sum += mcps_curr;
                    mcps_ave = mcps_sum / frame;
                    if (mcps_peak < mcps_curr) 
                    {
                        mcps_peak = mcps_curr;
                        peak_frame = frame;
                    }
                }

                fprintf(stdout, "[%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",frame,mcps_curr, mcps_ave, mcps_peak, peak_frame);
#else
                fprintf(stdout, "Last:[%d] \n",frame);
#endif
                frames++;
                outPtr  += len;
                tot_len += len;
                if( frames > XA_OPUS_MAX_FRAMES_PER_PACKET ) 
                {
                    /* Hack for corrupt stream that could generate too many frames */
                    outPtr  = synth_speech;
                    tot_len = 0;
                    frames  = 0;
                }
                
                /* Until last 20 ms frame of packet has been decoded */
            } while( dec_control.moreInternalDecoderFrames );
        } 
        else 
        {
            /* Loss: Decode enough frames to cover one packet duration */

            /* Generate 20 ms */
            for( i = 0; i < dec_control.framesPerPacket; i++ ) 
            {
#ifdef __PROFILE__
                xt_iss_client_command("all", "enable");
                times(&start);
#endif
                dec_control.lostFlag = 1;
                error_code = xa_opus_dec(speech_decoder_state, (pUWORD8)payloadToDec, outPtr,  nBytes, &dec_control, &len, scratch);

#ifdef __PROFILE__
                times(&stop);
                xt_iss_client_command("all", "disable");
#endif
                _XA_HANDLE_ERROR(p_proc_codec_err_info, "", error_code);
                frame++;

#ifdef __PROFILE__
                if ((profile_frame == 0) || (profile_frame > 0 && profile_frame == frame))
                {
                    cycles = stop.tms_utime - start.tms_utime;
                    u_seconds = ((double) len / dec_control.API_numChannels * 1000000) / dec_control.API_sampleRate;
                    if (u_seconds) mcps_curr = (double)cycles / u_seconds;
                    mcps_sum += mcps_curr;
                    mcps_ave = mcps_sum / frame;
                    if (mcps_peak < mcps_curr) 
                    {
                        mcps_peak = mcps_curr;
                        peak_frame = frame;
                    }
                }

                fprintf(stdout, "Last lost: [%d] MCPS: %f Average: %f Peak: %f @ [%d]\n",frame,mcps_curr, mcps_ave, mcps_peak, peak_frame);
#else
                fprintf(stderr, "Last Lost:[%d] \n",frame);
#endif
                frames++;
                outPtr  += len;
                tot_len += len;
            }
        }
        totPackets++;

#ifdef ENABLE_OGG_SUPPORT
        if(stream_type == XA_OGG_OPUS_STREAM)
        {
            tot_samps = tot_len/dec_control.API_numChannels;
            tmp_skip = preskip<tot_samps?preskip:tot_samps;
            preskip -= tmp_skip;
            tot_samps -= tmp_skip;
            /* Granule position and preskip are always assuming 48 KHz sampling rate */
            max_out = (((page_gran_pos[0]-gran_offset)*dec_control.API_sampleRate)/48000)-link_out;
            max_out = max_out>0?max_out:0;

            tot_samps = tot_samps<max_out?tot_samps:max_out;

            link_out += tot_samps;
            tot_len = tot_samps*dec_control.API_numChannels;
        }
#endif
        /* Write output to file */
#ifdef _SYSTEM_IS_BIG_ENDIAN
        swap_endian( synth_speech, tot_len );
#endif
#ifdef ENABLE_OGG_SUPPORT
        if(stream_type == XA_OGG_OPUS_STREAM)
        {
            pWORD16 out_speech = (pWORD16)synth_speech;
            out_speech += tmp_skip*dec_control.API_numChannels;
            fwrite( out_speech, sizeof( WORD16 ), tot_len, speechOutFile );
        }
        else
#endif
        {
            fwrite( synth_speech, sizeof( WORD16 ), tot_len, speechOutFile );
        }
    }

#ifdef __PROFILE__
    fprintf(stdout, "Peak MCPS = %f\n", mcps_peak);
    fprintf(stdout, "Peak frame = %d\n", peak_frame);
    fprintf(stdout, "Average MCPS = %f\n", mcps_ave);
    fprintf(stdout, "Total frames  = %d\n", frame);
#endif

#ifdef SCRATCH_ANALYSIS
    fprintf(stdout, "Max scratch usage = %d\n", scratch_max_size);
#endif

    /* Close files */
    fclose( speechOutFile );
    fclose( bitInFile );

    /* Free allocated memory. */
    free(speech_decoder_state);
#ifndef SCRATCH_PING_PONG
    free(scratch);
#else
    free(scratch_ping);
    free(scratch_pong);
#endif
#ifndef INPUT_PING_PONG
    free(enc_speech);
#else
    free(enc_speech_ping);
    free(enc_speech_pong);
#endif
#ifndef OUTPUT_PING_PONG
    free(synth_speech);
#else
    free(synth_speech_ping);
    free(synth_speech_pong);
#endif

#ifdef ENABLE_OGG_SUPPORT
    if(stream_type == XA_OGG_OPUS_STREAM)
    {
        SUMMARY_XA_PROFILER(ogg_prof);

        free(ogg_handle);
        if (ogg_scratch_size) free(ogg_scratch);
        free(p_ogg_inp_buf);
    }
#endif

    return 0;
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
                xa_opus_dec_main_process(fargc+1, pargv);

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
        err_code = xa_opus_dec_main_process(argc, argv);
        return err_code;
    }
}
