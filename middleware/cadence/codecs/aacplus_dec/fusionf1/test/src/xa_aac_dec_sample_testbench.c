/*
 * Copyright (c) 2006-2022 Cadence Design Systems, Inc.
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


//#define SMALL_INPUT_CHUNK_FEED 1 // SMALL_INPUT_CHUNK_FEED Enabler
#ifdef SMALL_INPUT_CHUNK_FEED
#define CHUNK_SIZE 400
int varChunkSize = CHUNK_SIZE;
#endif

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#ifdef CSTUB_HIFI2
#include "cstub-DC_330HiFi_syn.h"
#else
#include <xtensa/tie/xt_hifi2.h>
#include <xtensa/hal.h>
#endif

#include "xa_type_def.h"
#include "xa_memory_standards.h"
#include "xa_error_handler.h"
#include "aac_dec/xa_aac_dec_api.h"

#ifdef CSTUB_HIFI2
#define  PROFILE 1
#endif

#if !defined(PROFILE)
#define PROFILE 1
#define GMON_PROFILE
#endif /* PROFILE */


#if PROFILE
#include <sys/times.h>
#include <xtensa/sim.h>
#if defined(GMON_PROFILE)^M
#define GMON_FRMSTRT 0
#define GMON_FRMSTP 10000
#define PRINT_STR(str) /*fprintf(stderr, "%s\n", str);*/
#define GMON_PROFILE_ENABLE \
    if ((frame >= GMON_FRMSTRT) && (frame < GMON_FRMSTP)) \
{ PRINT_STR("Enabling profiler");xt_iss_profile_enable(); }
#define GMON_PROFILE_DISABLE \
    if ((frame >= GMON_FRMSTRT) && (frame < GMON_FRMSTP)) \
{ xt_iss_profile_disable(); PRINT_STR("Disabling profiler");}
#else
#define GMON_PROFILE_ENABLE
#define GMON_PROFILE_DISABLE
#endif

#endif /* PROFILE */

 
//#define SCRATCH_TRASH_TEST
//#define INPUT_PING_PONG
//#define OUTPUT_PING_PONG



#if !defined(SHOW_SIZES)
#define SHOW_SIZES 1
#endif /* SHOW_SIZES */

#ifndef STEP_PLAY
#define STEP_PLAY 0
#endif

//#define ENABLE_FRAME_BY_FRAME_DECODE
//#define ENABLE_STREAM_CHANGE_DETECTION

#if STEP_PLAY
/* Play PLAY_TIME_MS milliseconds of audio, mute the output for
   MUTE_TIME_MS milliseconds, skip SKIP_TIME_MS milliseconds of audio,
   repeat. */
#define PLAY_TIME_MS 500 /* ms */
#define MUTE_TIME_MS 100  /* ms */
#define SKIP_TIME_MS 500 /* ms */
#endif /* STEP_PLAY */


#if !defined(DISPLAY_MESSAGE)
#define DISPLAY_MESSAGE 1
#endif /* DISPLAY_MESSAGE */

#define MAX_STACK_PROC 10
#define MAX_MEM_ALLOCS 100
#define XA_MAX_CMD_LINE_LENGTH 300
#define XA_MAX_ARGS 20
#define XA_SCREEN_WIDTH 80
#ifdef AACLOAS_SUPPORT
#define PARAMFILE "paramfilesimple_loas.txt"
#else
#define PARAMFILE "paramfilesimple.txt"
#endif

#define XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED                0xFFFF8000
#define XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED                 0xFFFF8001

#define API_CALL(cmd, idx, pvalue, errstr) \
    err_code = (*p_xa_process_api)(p_xa_process_api_obj, (cmd), (idx), (pvalue));\
_XA_HANDLE_ERROR(p_proc_err_info, (errstr), err_code);

#define SET_CONFIG(idx, pvalue, errstr) API_CALL(XA_API_CMD_SET_CONFIG_PARAM,idx, pvalue, errstr)
#define GET_CONFIG(idx, pvalue, errstr) API_CALL(XA_API_CMD_GET_CONFIG_PARAM,idx, pvalue, errstr)

#ifdef __cplusplus
extern "C" {
#endif

#  ifdef DAB_PLUS
#    define CODEC_FUNC xa_dabplus_dec
#  else
#    define CODEC_FUNC xa_aac_dec
#  endif

    extern DLL_SHARED XA_ERRORCODE
        CODEC_FUNC (xa_codec_handle_t p_xa_aacmch_dec_obj,
                WORD32            i_cmd,
                WORD32            i_idx,
                pVOID             pv_value);
#ifdef __cplusplus
}
#endif

/* DABPLUS_RAW_SIDEINFO parameters. */
int  raw_n_sideinfo = 0; /* Default raw is NOT presented with side info */
FILE *g_pf_side = NULL;  /* Side Info FilePointer for DABPLUS_RAW_SIDEINFO format */

FILE *g_pf_inp=(FILE *)0, *g_pf_out=(FILE *)0;
void MEM_init(void);
void *MEM_malloc (int size, int align);
void MEM_freeall(void);


char pb_input_file_path[XA_MAX_CMD_LINE_LENGTH];
char pb_output_file_path[XA_MAX_CMD_LINE_LENGTH];
char pb_input_file_name[XA_MAX_CMD_LINE_LENGTH];
char pb_output_file_name[XA_MAX_CMD_LINE_LENGTH];

typedef struct _inp_buffer_t
{
    pWORD8 pb_inp_buf;
    UWORD32 ui_inp_size;
    WORD32 i_bytes_consumed, i_bytes_read;
    WORD32 i_buff_size;
    WORD32 i_inp_over;
} inp_buffer_t;
void InitBuffer(inp_buffer_t *pbuf, void *ptr, int size);
void UpdateBuffer(inp_buffer_t *pbuf, int bsfmt);

VOID xa_aac_dec_error_handler_init();
VOID xa_testbench_error_handler_init();

extern xa_error_info_struct xa_aac_dec_error_info;
extern xa_error_info_struct xa_testbench_error_info;
int bsfmt =XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN;

int outnchans = 0;
int chanmap = 0xfffffff;

#ifdef APPLY_ISO_DRC
int user_level_settings = 124; /* default */
int usr_prlFlag = 0; /* default */
#endif /* APPLY_ISO_DRC */




/****************************************
 * PCM / Wave file write functions
 ****************************************/

static void write16_bits_lh(FILE *fp, int i)
{
    putc(i & 0xff, fp);
    putc((i >> 8) & 0xff, fp);
}

static void write32_bits_lh(FILE *fp, int i)
{
    write16_bits_lh(fp, (int)(i & 0xffffL));
    write16_bits_lh(fp, (int)((i >> 16) & 0xffffL));
}

static void write24_bits_lh(FILE *fp, int i)
{
    putc((i >> 8) & 0xff, fp);
    putc((i >> 16) & 0xff, fp);
    putc((i >> 24) & 0xff, fp);
}

    static long
pcm_write (long *pTimeCh, int outchmap, int frameSize, FILE *fp , int pcm_sample_size)
{
    int i,ch, chmap, sample_ctr = 0;

    if(pcm_sample_size != 16) {
        long sample_24;
        for(i=0; i < frameSize; i++) {
            chmap = outchmap;
            for(ch=0 ; ch < 8 ; ch++) {
                /* Check if any channel data is present at this sample offset */
                if ((chmap & 0x0f) == 0x0f) {
                    /* Don't write unused channels. */
                }
                else {
                    sample_24 = pTimeCh[i*outnchans+ch];
                    write24_bits_lh(fp,sample_24);
                    sample_ctr++;
                }
                chmap >>= 4;
            }
        }
    } else {
        short *pTimeChShort = (short *)pTimeCh;
        short sample_16;
        for(i=0; i < frameSize; i++) {
            chmap = outchmap;
            for(ch=0 ; ch < 8; ch++) {
                /* Check if any channel data is present at this sample offset */
                if ((chmap & 0x0f) == 0x0f) {
                    /* Don't write unused channels. */
                }
                else {
                    sample_16 = pTimeChShort[i*outnchans+ch];
                    write16_bits_lh(fp, sample_16);
                    sample_ctr++;
                }
                chmap >>= 4;
            }
        }

    }
    return (sample_ctr);
}


#ifdef WAV_HEADER

int write_wav_header(
        FILE *fp,
        int freq,
        int channels,
        int bits,
        int frameSize,
        int frameCounter,
        int numWritten
        )
{
    int bytes = (bits + 7) / 8;
    fwrite("RIFF", 1, 4, fp); /* label */

    /* write32_bits_lh(fp, (4096 * frameCounter) + 58 - 8); */ /* length in bytes without header */
    write32_bits_lh(fp, (numWritten * bytes) + 58 - 8); /* length in bytes without header */

    fwrite("WAVEfmt ", 1, 8, fp); /* 2 labels */
    write32_bits_lh(fp, 2 + 2 + 4 + 4 + 2 + 2  + 2); /* length of PCM format decl area */
    write16_bits_lh(fp, 1); /* is pcm? */
    write16_bits_lh(fp, channels);
    write32_bits_lh(fp, freq);
    write32_bits_lh(fp, freq * channels * bytes); /* bps */
    write16_bits_lh(fp, channels * bytes);
    write16_bits_lh(fp, bits);
    write16_bits_lh(fp, 0);
    fwrite("fact", 1, 4, fp);
    write32_bits_lh(fp, 4);
    write32_bits_lh(fp, frameSize * frameCounter);
    fwrite("data", 1, 4, fp);

    /*  write32_bits_lh(fp, (4096 * frameCounter)); */
    write32_bits_lh(fp, (numWritten * bytes));

    return (ferror(fp) ? -1 : 0);
}

#endif

/*****************************************************************
 * showusage: show the usage message
 *****************************************************************/

void showusage()
{
    puts(
            "\n"
            "Usage:  <aac_dec> [OPTIONS] -ifile:<inputfile> -ofile:<outputfile>\n"
            "\n"
            "        OPTIONS:\n"
#ifdef DAB_PLUS
            "         -b<bsformat> Input file format (dabplus, rawdabplus)\n"
#else
            "         -b<bsformat> Input file format (none - raw / adts(default) / adif / loas / latm)\n"
            "                     Note libraries for supporting loas formats need to be built with appropriate makefile options\n"
#endif
            "         -d<bdownsample> Downsampled SBR mode (0 - off by default, 1 - on)\n"
            "         -f<int>     Force mono to stereo output (0 - off, 1 - on by default)\n"
            "         -h          help\n"
            "         -m<int>     Stereo to mono downmix in SBR processing (0 - off by default, 1 - on)\n"
            "         -n<int>     Maximum number of channels supported by decoder (limits: library dependent)\n"
            "         -p<int>     Sampling rate of raw bitstream\n"
            "                        default - 44100 Hz, DAB+ - 48000 Hz \n"
            "                     Refer Programmers Guide - Section 1.3 for all supported sampling rates\n"
#ifdef DAB_PLUS
            "         -r<int>     [DABPLUS only] Channel BitRate of DAB+ inputstream in kbps\n"
            "                        (8-192 in steps of 8 kbps; 48 kbps by default)\n"
#endif
            "         -s<int>     Start frame offset to decode (debug only)\n"
            "         -t<int>     Stop frame offset to decode (debug only)\n"
            "         -w<int>     PCM sample size (16 or 24 - default)\n"
            "         -x<int>     SBR mode (0 - disable, 1 - enable, 2 - auto by default)\n"
            "\n"
            "         -ifile:     Input bit stream file\n"
            "         -ofile:     Output Wave file\n"
#ifdef DAB_PLUS
            "         -sfile:     [DABPLUS_RAW_SIDEINFO only] Input side-info file\n"
#endif
            "\n"
            "         -0..7<ch>   Route input channels L, C, R, l, r, Sbl, Sbr, LFE to\n"
            "                     arbitrary interleaved PCM output channels (0..7).\n"
#ifdef APPLY_ISO_DRC
            "\n"
            "         --d<apply_drc>       Enable / Disable flag for applying DRC (default = 0)\n"
            "         --dC<drc_compress>   DRC Compression factor between 0.0 to 100.0 \n"
            "                                default value is 0.0\n"
            "         --dB<dcc_boost>      DRC Boost factor between 0.0 to 100.0 \n"
            "                                default value is 0.0\n"
            "\n"
            "         --p<apply_prl>       Enable / Disable flag for applying Program Ref level (default = 0)\n"
            "         --pL<target_level>   Target Level value between 0 to 127\n"
            "                                 (targetLevel in dB is -0.25*value dB) default value is 124\n"
#endif /* APPLY_ISO_DRC */
#ifdef DAB_PLUS
            "         -cfo<int>    Concealment fade-out slope in frames/AUs. Valid values: 2 to 7. Default: 5\n"
            "         -cmr<int>    Concealment mute release in frames/AUs. Valid values: 1 to 15. Default: 5\n"
            "         -cfi<int>    Concealment fade-in slope in frames/AUs. Valid values: 2 to 7. Default: 5\n"
#endif
            "         -a<int>     MPEG4 Amendment4 (0 - disable, 1 - enable by default)\n"
            "         -rst<int>   Reset internal decoder states in case of sync loss error(0 - disable, 1 - enable by default)\n"
            " \n "
#ifndef DAB_PLUS
            "NOTE: The above parameters are generic. "
            "AAC-LC does not support SBR. "
            "SBR is only supported by AAC plus & AAC plus v2 "
            "\n Please refer PG for more details "
#endif
            );
}



/****************************************
 * Function to parse cmdline parameter and
 *    set config variables in the codec
 *    library
 ****************************************/

int bDownSample = 0;                                   /*!< 0: downsampled sbr off  1: downsampled sbr on */
int startFrame =  0;                                   /*!< number of first frame which output is written to output file */
int stopFrame  = -1;                                   /*!< number of last frame which output is written to output file */

XA_AACDEC_EBITSTREAM_TYPE bsType =XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN;

int parse_cmdline(void *p_hdl, char *argv[], int argc)
{

    int err_code = 0;
    char argtyp = (char)0;
    int argidx = 0;
    char *argval = 0;
    char one = '1';
    int i_value;
    unsigned int ui_value;
    xa_codec_handle_t p_xa_process_api_obj = (xa_codec_handle_t)p_hdl;
    xa_codec_func_t *p_xa_process_api = CODEC_FUNC;
    xa_error_info_struct *p_proc_err_info = &xa_aac_dec_error_info;
    char externalBsFormat[20]="";                           /*!< bitstream format */
    char pb_sideinfo_file_name[XA_MAX_CMD_LINE_LENGTH] = "?";

    int externalSamplingRate =0;                              /*!< external sampling rate for raw decoding */

    /* initialize commandline parser */

    while (1) {
        if ( argidx >= argc ) break;

        /* Check & parse the argument type  & its value */
        if (argv[argidx][0] == '-') {
            argtyp = argv[argidx][1];

            if (strlen(argv[argidx]) == 2) {
                /* Argument type has either no argument value or has argument value after blank-spaces */
                /* default values */
                argval = &one;
                argval[1] = '\0';
                if ( (argidx+1) < argc ) {
                    if (argv[argidx+1][0] != '-') {
                        /* To take care of case where argument-flag and option-value have blank-spaces between them */
                        argidx++;
                        argval = &argv[argidx][0];
                    }
                }
            }
            else {
                argval = &argv[argidx][2];
            }
        }
        else {
            argtyp = ' ';
            argval = (char *)"\0";
        }
        argidx++;

        /* Set config parameters based on argtype */
        switch(argtyp) {
            case 'i':
#ifdef SMALL_INPUT_CHUNK_FEED
                if (!strncmp(argval, "nSize:", 6)) {
                    char *pb_arg_val = argval + 6;
                    varChunkSize = atoi(pb_arg_val);
                    /* FORCE the varChunk size to be even - to allow smooth byte swap operation on the input buffer */
                    /* Also feed no less than 8 bytes at a time */
                    varChunkSize &= ~1;
                    if (varChunkSize < 8) varChunkSize=8;
                }
                else
#endif
                {
                    /* Read File name only if it was not set by  
                        -ifile: option in main wrapper
                        Note: -i option don't use the file path from 
                        paramfile
                    */
                    if ( pb_input_file_name[0] == '\0' )
                    {
                        strcpy(pb_input_file_name,argval);
                    }
                }
                break;

            case 'o':
                /* Read File name only if it was not set by  
                    -ofile: option in main wrapper
                    Note: -o option don't use the file path from 
                    paramfile
                */
                if ( pb_output_file_name[0] == '\0' ) {
                    strcpy(pb_output_file_name,argval);
                }
                break;

            case 'b':

                strcpy(externalBsFormat,argval);

                /* Set bitstream type at the end */
                bsType = XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN;

                if (!strcmp(externalBsFormat,"raw")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_AAC_RAW;
                }
                if (!strcmp(externalBsFormat,"adif")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_AAC_ADIF;
                }
                if (!strcmp(externalBsFormat,"adts")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_AAC_ADTS;
                }
                if (!strcmp(externalBsFormat,"loas")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_AAC_LOAS;
                }
                if (!strcmp(externalBsFormat,"latm")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_AAC_LATM;
                }
                if (!strcmp(externalBsFormat,"dabplus")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_DABPLUS;
                }
                if (!strcmp(externalBsFormat,"rawdabplus")) {
                    bsType = XA_AACDEC_EBITSTREAM_TYPE_DABPLUS_RAW_SIDEINFO;
                    raw_n_sideinfo = 1;
                }

                ui_value = (WORD32)bsType;
                bsfmt = bsType;

                SET_CONFIG(XA_AACDEC_CONFIG_PARAM_EXTERNALBSFORMAT, (void *)&ui_value, "External BS format Configuration Error")

                    break;

            case 'r':
                if(!strncmp(argval, "st", 2)) {
                    char *pb_arg_val = argval + 2;
                    ui_value = atoi(pb_arg_val);
                    SET_CONFIG( XA_AACDEC_CONFIG_PARAM_RESET_STATE_ON_SYNC_LOSS_ERROR, (void *)&ui_value, "Reset States on Sync Loss Error");
                }
                else
                {
                    /* Channel BitRate required as input for DAB_Parser */
                    int externalbitrate = atol(argval);
                    SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALBITRATE, (void *)&externalbitrate, "Invalid External BitRate Setting Error");
                    break;
                } 

                /* The following 2 option are Useful for test and debug */
            case 's':
                if(!strncmp(argval, "file:", 5)) {
                    char *pb_arg_val = argval + 5;
                    strcpy(pb_sideinfo_file_name,pb_arg_val);
                    g_pf_side = fopen(pb_sideinfo_file_name,"rb");
                    break;
                }
                else {
                    startFrame = atol(argval);
                }
                break;

            case 't':
                stopFrame = atol(argval);
                break;

            case 'p':
                i_value = (WORD32)atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALSAMPLINGRATE, (void *)&i_value, "Sample Rate Config Error");
                externalSamplingRate = i_value;
                break;

            case 'f':
                /* Force stereo */
                i_value = (WORD32)atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_TO_STEREO, (void *)&i_value, "Force-Stereo Parameter Error");
                externalSamplingRate = i_value;
                break;

            case 'w':
                /* pcm sample size */
                i_value = (WORD32)atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_PCM_WDSZ, (void *)&i_value, "");
                break;

            case 'x':
                ui_value = (WORD32)atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_SBR_SIGNALING, (void *)&ui_value, "SBR Signaling Configuration Error");
                break;

            case 'd':
                bDownSample = atol(argval);
                i_value = (WORD32)bDownSample;
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_BDOWNSAMPLE, (void *)&i_value, "Set downsample config Error");
                break;

            case 'm':
                i_value = atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_BBITSTREAMDOWNMIX, (void *)&i_value, "Bitstream downmix config Error");
                break;

                /* Input number of channels in BSAC Case */
            case 'c':
#ifdef DAB_PLUS
				/* check for fade_out_frames */
				if(!strncmp(argval, "fo", 2))
				{
					i_value = (WORD32)atol(argval + 2);
		            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_CONCEALMENT_FADE_OUT_FRAMES, (void *)&i_value, "Fade_out_frames Parameter Error");
				}
				/* check for mute_release_frames */
				else if(!strncmp(argval, "mr", 2))
				{
					i_value = (WORD32)atol(argval + 2);
		            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_CONCEALMENT_MUTE_RELEASE_FRAMES, (void *)&i_value, "Mute_release_frames Parameter Error");
				}
				/* check for fade_in_frames */
				else if(!strncmp(argval, "fi", 2))
				{
					i_value = (WORD32)atol(argval + 2);
		            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_CONCEALMENT_FADE_IN_FRAMES, (void *)&i_value, "Fade_in_frames Parameter Error");
				}
				else
				{
			    	i_value = atol(argval);
		    	    SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALCHCONFIG , (void *)&i_value, "");
				}
#else	//#ifdef DAB_PLUS
                i_value = atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_EXTERNALCHCONFIG , (void *)&i_value, "");
#endif	//#ifdef DAB_PLUS
                break;

            case 'l':
                i_value = atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_DECODELAYERS , (void *)&i_value, "");
                break;

            case 'n':
                ui_value = (WORD32)atol(argval);
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_OUTNCHANS, (void *)&ui_value, "Max Num channel Configuration Error");
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                ui_value = ((int)argtyp - (int)('0')) << 8;
                switch(argval[0])
                {
                    case 'L':
                        if ((argval[1] == 'F') && (argval[2] == 'E') && (argval[3] == '\0'))
                            ui_value += 7;  /* LFE channel */
                        else if (argval[1] == '\0')
                            ui_value += 0;  /* L channel */
                        else
                        {
                            ui_value += 0xf;
                        }
                        break;
                    case 'C':
                        if (argval[1] == '\0')
                            ui_value += 1; /* C */
                        else
                        {
                            ui_value += 0xf;
                        }
                        break;
                    case 'R':
                        if (argval[1] == '\0')
                            ui_value += 2; /* R */
                        else
                        {
                            ui_value += 0xf;
                        }
                        break;
                    case 'l':
                        if (argval[1] == '\0')
                            ui_value += 3; /* Ls */
                        else
                        {
                            ui_value += 0xf;
                        }
                        break;
                    case 'r':
                        if (argval[1] == '\0')
                            ui_value += 4; /* Rs */
                        else
                        {
                            ui_value += 0xf;
                        }
                        break;
                    case 'S':
                        if ((argval[1] == 'b') && (argval[2] == 'l') && (argval[3] == '\0'))
                        {
                            ui_value += 5; /* Sbl or Cs */
                        }
                        else if ((argval[1] == 'b') && (argval[2] == 'r') && (argval[3] == '\0'))
                        {
                            ui_value += 6; /* Sbr */
                        }
                        else
                        {
                            ui_value += 0xf;
                        }
                        break;
                    default:
                        ui_value = 0xff;
                        break;
                }
                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_CHANROUTING, (void *)&ui_value, "Error in Channel map");

                break;

            case 'h':
                showusage();
                err_code = 0;
                return err_code;
                break;

#ifdef APPLY_ISO_DRC
            case '-':
                /* Set the DRC and PRL enable flags */
                {
                    int apply_drc = 0;
                    int apply_prl = 0;
                    switch(argval[0])
                    {
                        /* Set DRC and PRL flags */
                        case 'd':
                            if (argval[1] == 'C') {
                                float drc_compress = atof(&argval[2]);
                                int i_drc_compress = (int)(drc_compress/100.0*(1 << 23));
                                fprintf(stderr, "Cut %s : %f : %x\n", &argval[2], drc_compress, i_drc_compress);
                                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_DRC_COMPRESS_FAC, (void *)&i_drc_compress, "Error in setting drc Compress value ");
                            }
                            else if (argval[1] == 'B') {
                                float drc_boost = atof(&argval[2]);
                                int i_drc_boost = (int)(drc_boost/100.0*(1 << 23));
                                fprintf(stderr, "Boost %s : %f : %x\n", &argval[2], drc_boost, i_drc_boost);
                                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_DRC_BOOST_FAC, (void *)&i_drc_boost, "Error in setting drc Boost value ");
                            }
                            else {
                                apply_drc = atoi(&argval[1]);
                                fprintf(stderr, "apply_drc %d\n", apply_drc);
                                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_ENABLE_APPLY_DRC, (void *)&apply_drc, "Error in setting drc enable flag ");
                            }

                            break;

                        case 'p':
                            if (argval[1] == 'L') {
                                int target_level = atoi(&argval[2]);
                                fprintf(stderr, "level %s : %d\n", &argval[2], target_level);
                                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_TARGET_LEVEL, (void *)&target_level, "Error in setting target level value ");

                                user_level_settings = target_level;
                            }
                            else {
                                apply_prl = atoi(&argval[1]);
                                fprintf(stderr, "apply_prl %d\n", apply_prl);
                                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_ENABLE_APPLY_PRL, (void *)&apply_prl, "Error in setting prl enable flag");

                                usr_prlFlag = apply_prl?1:0;
                            }

                            break;
                        default:
                            if(argidx)
                                printf("\n Testbench Warning!!!!!   Ignoring argument %s at testbench.\n", argv[argidx - 1]);
                            break;
                    }
                    break;
                }
#endif
            case 'a':
                if(isdigit((int)argv[argidx - 1][2]))
                {
                    ui_value = (WORD32)atoi((char *)&argv[argidx - 1][2]);
                }
                else
                {
                    ui_value = (WORD32)(argv[argidx - 1][2]);
                }

                SET_CONFIG( XA_AACDEC_CONFIG_PARAM_MPEG4_AMENDMENT4_ENABLE, (void *)&ui_value, "Set MPEG4 Amendment4  Configuration Error");
                break;

            default:
                if(argidx)
                    printf("\n Testbench Warning!!!!!   Ignoring argument %s at testbench.\n", argv[argidx - 1]);
                break;
        } /* switch */
    } /* while */

    if (argc <= 1) {
        showusage();
    }

    if ((g_pf_side == NULL) && (raw_n_sideinfo == 1)) {
        fprintf(stderr, "fatal error: Unable to open SideInfo file %s for DABPLUS_RAW_SIDEINFO format decoding\n", pb_sideinfo_file_name);
        return -2;
    }

    return err_code;
}


/*****************************************************************************************
 * Function to open bit-stream file
 *     After an initialization and synchronization phase, get_params function is called to
 *     get the parameters available by reading the input bit-stream.
 *****************************************************************************************/

int get_params(void *p_hdl,
        int * p_numChannels,
        int *p_channelMode,
        int *p_sampleRate,
        int *p_sbrType,
        int *p_bsfmt,
        int *p_data_rate,
        int *p_pcm_sample_size)
{
    int err_code = 0;
    int numChannels, acmod, samp_freq;
    int sbrType;
    char pFmt[20] = {""};

    int  fmt_idx;
    int  aac_sampleRate;
    int mpeg4_amd4;


    /* The process API function */
    xa_codec_func_t *p_xa_process_api = CODEC_FUNC;

    /* The process API object */
    xa_codec_handle_t p_xa_process_api_obj = (xa_codec_handle_t)p_hdl;

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info = &xa_aac_dec_error_info;

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_AAC_FORMAT, (void *)&fmt_idx, "Get Mpeg format Error");

    /* Get Number Of Channels in the Bit Stream */
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_NUM_CHANNELS, (void *)&numChannels, "Get num-channels Error");

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_ACMOD, (void *)&acmod, "Get ChannelMode Error");

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_SAMP_FREQ, (void *)&samp_freq, "Get Sample-rate Error");


    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_SBR_TYPE, (void *)&sbrType, "Get SbrEnabled type Error");


    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_AAC_SAMPLERATE, (void *)&aac_sampleRate, "Get AAC Sample-Rate Error");
    
#ifdef DAB_PLUS
    mpeg4_amd4 = 0;
#else
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_MPEG4_AMENDMENT4_ENABLE, (void *)&mpeg4_amd4, "Get MPEG4 Amendment4 Error");
#endif

    switch(fmt_idx)
    {
        case XA_AACDEC_EBITSTREAM_TYPE_AAC_ADIF:
            strcpy(pFmt,"ADIF");
            break;
        case XA_AACDEC_EBITSTREAM_TYPE_AAC_ADTS:
            strcpy(pFmt,"ADTS");
            break;
        case XA_AACDEC_EBITSTREAM_TYPE_AAC_LATM:
        case XA_AACDEC_EBITSTREAM_TYPE_AAC_LATM_OUTOFBAND_CONFIG:
            /* Not supported */
            strcpy(pFmt,"LATM");
            break;
        case XA_AACDEC_EBITSTREAM_TYPE_AAC_LOAS:
            /* Supported if library is built with LOAS_SUPPORT=1 */
            strcpy(pFmt,"LOAS");
            break;
        case XA_AACDEC_EBITSTREAM_TYPE_AAC_RAW:
            strcpy(pFmt,"RAW");
            break;
        case XA_AACDEC_EBITSTREAM_TYPE_DABPLUS_RAW_SIDEINFO:
            /* Supported only if short side-info
               (format: Bit15 = badFramIndicator & Bit 0-14 = payloadSizeInBytes)
               is provided as config parameter for every raw-input frame */
            strcpy(pFmt,"DAB+ RAW+SIDEINFO");
            break;
        case XA_AACDEC_EBITSTREAM_TYPE_DABPLUS:
            strcpy(pFmt,"DAB+");
            break;
        default:
            strcpy(pFmt,"NO");
            err_code = -1;
            break;
    }

    fprintf(stderr, "Bitstream format:   %s\n",pFmt);
    fprintf(stderr, "Format:             ");

    /* Determine the format */
    if (sbrType==2) {
        fprintf(stderr, "aacPlus v2\n");
    }
    else if (sbrType==1) {
        fprintf(stderr, "aacPlus\n");
    }
    else {
        fprintf(stderr, "plain AAC\n");
    }

    fprintf (stderr, "Output Sample rate: %i Hz\n", samp_freq);
    fprintf (stderr, "AAC Sample rate:    %i Hz\n", aac_sampleRate);
    fprintf (stderr, "Input config:       ");

    switch (acmod)
    {
        case XA_AACDEC_CHANNELMODE_MONO:
            fprintf(stderr, "Mono\n");
            break;
        case XA_AACDEC_CHANNELMODE_STEREO:
            fprintf(stderr, "Stereo\n");
            break;
        case XA_AACDEC_CHANNELMODE_DUAL_CHANNEL:
            fprintf(stderr, "Dual mono\n");
            break;
        case XA_AACDEC_CHANNELMODE_PARAMETRIC_STEREO:
            fprintf(stderr, "Parametric Stereo\n");
            break;
        case XA_AACDEC_CHANNELMODE_3_CHANNEL_FRONT:
            fprintf(stderr, "3.0 Multichannel Front (LCR)\n");
            break;
        case XA_AACDEC_CHANNELMODE_3_CHANNEL_SURR:
            fprintf(stderr, "3.0 Multichannel Surround (LRS)\n");
            break;
        case XA_AACDEC_CHANNELMODE_4_CHANNEL_1SURR:
            fprintf(stderr, "4.0 Multichannel MonoSurround (LCR + S)\n");
            break;
        case XA_AACDEC_CHANNELMODE_4_CHANNEL_2SURR:
            fprintf(stderr, "4.0 Multichannel DualSurround (LR + LsRs)\n");
            break;
        case XA_AACDEC_CHANNELMODE_6_CHANNEL:
            fprintf(stderr, "6.0 Multichannel TriSurround (LCR + LsRsCs)\n");
            break;
        case XA_AACDEC_CHANNELMODE_7_CHANNEL:
            fprintf(stderr, "7.0 Multichannel\n");
            break;
        case XA_AACDEC_CHANNELMODE_5_CHANNEL:
            fprintf(stderr, "5.0 Multichannel\n");
            break;
        case XA_AACDEC_CHANNELMODE_2_1_STEREO:
            fprintf(stderr, "2.1 Multichannel (LR + lfe)\n");
            break;
        case XA_AACDEC_CHANNELMODE_3_1_CHANNEL_FRONT:
            fprintf(stderr, "3.1 Multichannel Front (LCR + lfe)\n");
            break;
        case XA_AACDEC_CHANNELMODE_3_1_CHANNEL_SURR:
            fprintf(stderr, "3.1 Multichannel Surround (LRS + lfe)\n");
            break;
        case XA_AACDEC_CHANNELMODE_4_1_CHANNEL_1SURR:
            fprintf(stderr, "4.1 Multichannel MonoSurround (LCR + S + lfe)\n");
            break;
        case XA_AACDEC_CHANNELMODE_4_1_CHANNEL_2SURR:
            fprintf(stderr, "4.1 Multichannel DualSurround (LR + LsRs + lfe)\n");
            break;
        case XA_AACDEC_CHANNELMODE_6_1_CHANNEL:
            fprintf(stderr, "6.1 Multichannel TriSurround (LCR + LsRsCs + lfe)\n");
            break;
        case XA_AACDEC_CHANNELMODE_5_1_CHANNEL:
            fprintf(stderr, "5.1 Multichannel\n");
            break;
        case XA_AACDEC_CHANNELMODE_7_1_CHANNEL:
            if(mpeg4_amd4 == 0)
            fprintf(stderr, "7.1 Multichannel\n");
            else
            fprintf(stderr, "7.1 Multichannel Front\n");
            break;
        case XA_AACDEC_CHANNELMODE_7_1_CHANNEL_BACK:
            fprintf(stderr, "7.1 Multichannel Back\n");
            break;
        case XA_AACDEC_CHANNELMODE_7_1_CHANNEL_TOP:
            fprintf(stderr, "7.1 Multichannel Top\n");
            break;

        default:
            fprintf(stderr, "unkown channel mode\n");
            err_code = -1;
            break;
    }
    fprintf (stderr, "Output Channels:    %d\n", numChannels);

    *p_numChannels = numChannels;
    *p_channelMode = acmod;
    *p_sampleRate = samp_freq;
    *p_bsfmt = fmt_idx;

    *p_sbrType = sbrType;

    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_CHANMAP, (void *)&chanmap, "Get Output Channel-Map Error");

    fprintf (stderr, "Output Channel Map: %08x\n", chanmap);

    /* Get maximum number of output channels in output buffer */
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_OUTNCHANS, (void *)&outnchans, "Get Output Channel-Map Error");
    
    /* Get PCM Sample Size */
    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_PCM_WDSZ, p_pcm_sample_size, "PCM_WDSZ Get Error");

#ifdef DAB_PLUS
    {
	    int fade_out_frames, mute_rel_frames, fade_in_frames, mpeg_surround_present;	

	    /* Get MPEG surround status */
	    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_MPEGSURR_PRESENT, (void *)&mpeg_surround_present, "Get MPG surround status Error");

	    fprintf (stderr, "MPEG Surround:      ");

	    mpeg_surround_present? fprintf(stderr, "Present\n"):
		    fprintf(stderr, "Not Present\n");

	    /* Get fade-out value */
	    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_CONCEALMENT_FADE_OUT_FRAMES, &fade_out_frames, "Fade_out_frames Get Error");
	    fprintf (stderr, "Concealment Fade-out Frames:       %d\n",fade_out_frames);

	    /* Get mute-rel value */
	    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_CONCEALMENT_MUTE_RELEASE_FRAMES, &mute_rel_frames, "mute_rel_frames Get Error");
	    fprintf (stderr, "Concealment Mute Release Frames:   %d\n",mute_rel_frames);

	    /* Get fade-in value */
	    GET_CONFIG( XA_AACDEC_CONFIG_PARAM_CONCEALMENT_FADE_IN_FRAMES, &fade_in_frames, "Fade_in_frames Get Error");
	    fprintf (stderr, "Concealment Fade-in Frames:        %d\n\n",fade_in_frames);
	    
    }
#endif	//#ifdef DAB_PLUS
   
    return err_code;
}

/*****************************************************************
 * decoder main                                                  *
 *****************************************************************/

int xa_aac_dec_main_process(int argc, char *argv[])
{
    int i;
    int  acmod = -1;

    int frameCounter = 0;
    int frameSize = 0;
    int i_num_chan = 2;
    int i_samp_freq = 48000, i_data_rate=0;
    int err_code=0;
    int err_code_exec = 0;
    bsfmt = XA_AACDEC_EBITSTREAM_TYPE_UNKNOWN;
    int sbrType=0;
    int reInitFlag = 0;
    void * ptemp;
    int pcm_sample_size=2;
    xa_codec_handle_t p_xa_process_api_obj = NULL;

    /* The process API function */
    xa_codec_func_t *p_xa_process_api = NULL;

    /* The process error info structure */
    xa_error_info_struct *p_proc_err_info = NULL;

#if PROFILE
    struct tms start, stop;
    double Peak = -1, Ave=0, Sum = 0;
    int frame = 0;
    int Peak_frame = -1;
    unsigned long long total_samples = 0;
    fprintf(stdout, "\n");
#endif /* PROFILE */

    long i_total_bytes = 0;



    /* Memory variables */
    unsigned int n_mems=0;
    unsigned int ui_proc_mem_tabs_size=0;

    /* API size */
    unsigned int ui_api_size=0;

    /* Process initing done query variable */
    unsigned int ui_init_done=0, ui_exec_done=0;
    pWORD8 pb_out_buf = NULL;

    inp_buffer_t inp_buffer = {0};
    inp_buffer.pb_inp_buf = NULL;
    inp_buffer.ui_inp_size = 0;

    WORD32 i_out_bytes=0;

#if DISPLAY_MESSAGE
    /* Library Info and Identification strings */
    WORD8 pb_process_name[XA_SCREEN_WIDTH]    = "";
    WORD8 pb_lib_version[XA_SCREEN_WIDTH]     = "";
#endif


#ifdef SCRATCH_TRASH_TEST
    WORD32 scratch_size;
    void *p_scratch;
#endif

#ifdef INPUT_PING_PONG
    WORD8 *p_input_buffer_ping, *p_input_buffer_pong;
    WORD32 inp_idx;
#endif /* #ifdef INPUT_PING_PONG */

#ifdef OUTPUT_PING_PONG
    WORD8 *p_output_buffer_ping, *p_output_buffer_pong;
    WORD32 out_idx;
#endif /* #ifdef OUTPUT_PING_PONG */

    p_xa_process_api = CODEC_FUNC;
    p_proc_err_info = &xa_aac_dec_error_info;

    p_xa_process_api_obj = NULL;
    /* Call decoder error handler init */
    xa_aac_dec_error_handler_init();

    /* ******************************************************************/
    /* Get the library name, library version and API version            */
    /* ******************************************************************/

#if DISPLAY_MESSAGE
    /* Get the library name string */
    API_CALL(XA_API_CMD_GET_LIB_ID_STRINGS, XA_CMD_TYPE_LIB_NAME, pb_process_name, "Lib-name Error");

    /* Get the library version string */
    API_CALL(XA_API_CMD_GET_LIB_ID_STRINGS, XA_CMD_TYPE_LIB_VERSION, pb_lib_version, "Lib-version Error");

    /* Display the Tensilica identification message */
    fprintf(stderr, "\n%s version %s\n", pb_process_name, pb_lib_version);
    //fprintf(stderr, "Tensilica, Inc. http://www.tensilica.com\n\n");
    fprintf(stderr, "Cadence Design Systems, Inc. https://ip.cadence.com/\n\n");
#endif

    /* ******************************************************************/
    /* Initialize API structure and set config params to default        */
    /* ******************************************************************/

    /* Get the API size */
    API_CALL( XA_API_CMD_GET_API_SIZE, 0, &ui_api_size, "Api Size Error");

    /* Set API object with the memory allocated */
    p_xa_process_api_obj = (xa_codec_handle_t)MEM_malloc(ui_api_size, 4);
    if(p_xa_process_api_obj == NULL)
    {
        xa_error_handler(&xa_testbench_error_info, "malloc fail",
                XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }
   

    /* ******************************************************************/
    /* Set the config params to default values                          */
    /* ******************************************************************/

    API_CALL(XA_API_CMD_INIT, XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS, NULL, "Pre Configuration Init Error");


    /* ******************************************************************/
    /* Set config parameters got from the user from argc argv           */
    /* ******************************************************************/

    err_code = parse_cmdline(p_xa_process_api_obj, argv, argc);
    if ( err_code ) {
        fprintf(stderr, "User Configuration Error\n");
        err_code = -1;
        return err_code;
    }

    /* ******************************************************************/
    /*  Open IO files                                                   */
    /* ******************************************************************/
    if(pb_input_file_name[0] == '\0')
    {
        err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
        printf("Error: Input file name not specified\n");
        printf("\tUse either -i or -ifile: option to specify.\n");
    }
    else
    {
        {
            g_pf_inp = fopen(pb_input_file_name,"rb");
            if (g_pf_inp == NULL)
            {
                err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
                fprintf(stderr,"Input file could not be opened\n");
                fprintf(stderr,"\tFile Name specified: %s\n", pb_input_file_name);
            }
        }
    }

    if(pb_output_file_name[0] == '\0')
    {
        err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
        printf("Error: Output file name not specified\n");
        printf("\tUse either -o or -ofile: option to specify.\n");
    }
    {
        g_pf_out = fopen(pb_output_file_name,"wb");
        if (g_pf_out == NULL) {
            err_code = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;
            fprintf(stderr,"Output file could not be opened\n");
            fprintf(stderr,"\tFile Name specified: %s\n", pb_output_file_name);
        }
    }
    if(err_code)
    {
        printf("Command line usage:\n");
        printf("<decoder> -ifile:<infile> -ofile:<outfile>\n");
        printf("Detailed description of the options: <decoder> -h\n");
        _XA_HANDLE_ERROR(&xa_testbench_error_info,
                "Input or Output File", err_code);
    }


    /* Allocate memory and Initialize */

    /* ******************************************************************/
    /* Initialize Memory info tables                                    */
    /* ******************************************************************/

    /* Get memory info tables size and Set pointer for process memory tables*/
    API_CALL(XA_API_CMD_GET_MEMTABS_SIZE, 0, &ui_proc_mem_tabs_size, "");

    ptemp = MEM_malloc(ui_proc_mem_tabs_size, 4);
    if(ptemp == NULL)
    {
        xa_error_handler(&xa_testbench_error_info, "malloc fail",
                XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
    }

#if SHOW_SIZES
    printf("\nAPI structure size: %u bytes\n", ui_api_size);
    printf("MEMTABS size: %u bytes\n", ui_proc_mem_tabs_size);
#endif

    API_CALL(XA_API_CMD_SET_MEMTABS_PTR, 0, ptemp, "Set mem-tab-ptr error");


    /* initialize the API, post config, fill memory tables */
    API_CALL(XA_API_CMD_INIT, XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS, NULL, "post-config error");


    /*******************************************************************/
    /* Allocate Memory with info from library                           */
    /*******************************************************************/

    /* Get number of memory tables required */
    API_CALL(XA_API_CMD_GET_N_MEMTABS, 0, &n_mems, "Get-n-memtabs error");

    for(i = 0; i < (WORD32)n_mems; i++) {
        int ui_size=0, ui_alignment=1, ui_type=-1;
        pVOID pv_alloc_ptr;

        /* Get memory size */
        API_CALL( XA_API_CMD_GET_MEM_INFO_SIZE, i, &ui_size, "Error in get-mem-info-size");

        /* Get memory alignment */
        API_CALL( XA_API_CMD_GET_MEM_INFO_ALIGNMENT, i, &ui_alignment, "Error in get-mem-info-alignment");

        /* Get memory type */
        API_CALL(XA_API_CMD_GET_MEM_INFO_TYPE, i, &ui_type, "Error in get-mem-info-type");

        pv_alloc_ptr = MEM_malloc(ui_size, ui_alignment);
	if(pv_alloc_ptr == NULL)
	{
		xa_error_handler(&xa_testbench_error_info, "malloc fail",
				XA_TESTBENCH_MFMAN_FATAL_MEM_ALLOC_FAILED);
	}

        /* Set the buffer pointer */
        API_CALL(XA_API_CMD_SET_MEM_PTR, i, pv_alloc_ptr, "Error in set mem-ptr");

        if(ui_type == XA_MEMTYPE_INPUT) {
            InitBuffer(&inp_buffer,pv_alloc_ptr, ui_size );

#ifdef INPUT_PING_PONG
            p_input_buffer_ping = (WORD8 *) pv_alloc_ptr;
            inp_idx = i;

            p_input_buffer_pong = (WORD8 *) MEM_malloc(ui_size, ui_alignment);
#endif /* #ifdef INPUT_PING_PONG */
        }

        if(ui_type == XA_MEMTYPE_OUTPUT) {
            pb_out_buf = (pWORD8)pv_alloc_ptr;

#ifdef OUTPUT_PING_PONG
            p_output_buffer_ping = (pWORD8)pv_alloc_ptr;
            out_idx = i;

            p_output_buffer_pong = (pWORD8) MEM_malloc(ui_size, ui_alignment);
#endif /* #ifdef OUTPUT_PING_PONG */
        }
#ifdef SCRATCH_TRASH_TEST
        if(ui_type == XA_MEMTYPE_SCRATCH) {

            scratch_size = ui_size;
            p_scratch = (void *)pv_alloc_ptr;
        }
#endif /* #ifdef SCRATCH_TRASH_TEST */

        switch (ui_type)
        {
#if SHOW_SIZES
            case XA_MEMTYPE_INPUT:
                printf("Input buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_OUTPUT:
                printf("Output buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_SCRATCH:
                printf("Scratch buffer size: %u bytes\n", ui_size);
                break;
            case XA_MEMTYPE_PERSIST:
                printf("Persistent buffer size: %u bytes\n", ui_size);
                break;
#else
            case XA_MEMTYPE_INPUT:
            case XA_MEMTYPE_OUTPUT:
            case XA_MEMTYPE_SCRATCH:
            case XA_MEMTYPE_PERSIST:
                break;
#endif /* SHOW_SIZES */
            default:
                assert(0);
        }
	
    }
    /* End first part */

    /* Second part        */
    /* Initialize process */
    /* Get config params  */

    /* ******************************************************************/
    /* Initialize process in a loop (to handle junk data at beginning)  */
    /* ******************************************************************/


Init_Point:
    ui_init_done = 0;
#ifdef ENABLE_FRAME_BY_FRAME_DECODE
    // While Initializing, don't look for the next frame to confirm header;
    // This will ensure that the decoder initialized with a single valid ADTS frame.
    {
        int frameByFrameDecode = 1;
        SET_CONFIG( XA_AACDEC_CONFIG_PARAM_ENABLE_FRAME_BY_FRAME_DECODE, &frameByFrameDecode, "Enable Frame By Frame Decode");        
    }
#endif
    /* Seek for zeroth frame (frameCounter == 0) - Perform Initializations & parse the first frame */
    do {
        {
            UpdateBuffer(&inp_buffer,  bsfmt);

            if(inp_buffer.i_inp_over == 1)
            {
                /* Tell that the input is over in this buffer */
                API_CALL(XA_API_CMD_INPUT_OVER, 0, NULL, "Input Over - Error");
            }
            /* Set number of bytes to be processed */
            API_CALL(XA_API_CMD_SET_INPUT_BYTES, 0, &inp_buffer.i_buff_size, "Setting input-Bytes Error");
        }

        /* Initialize the process */
        API_CALL(XA_API_CMD_INIT, XA_CMD_TYPE_INIT_PROCESS, NULL, "Init-processing-error");

        /* Checking for end of initialization */
        API_CALL(XA_API_CMD_INIT, XA_CMD_TYPE_INIT_DONE_QUERY, &ui_init_done, "End of Init detect Error");

        /* How much buffer is used in input buffers */
        API_CALL(XA_API_CMD_GET_CURIDX_INPUT_BUF, 0, &inp_buffer.i_bytes_consumed, "");

        /* Break if init is done */
        if (ui_init_done)  break;

    } while(1); /* Init Loop */
    /* Init end of process condition end */

    /* ******************************************************************/
    /* Get config params from API                                       */
    /* ******************************************************************/

    /* Read parameters dervied by analyzing the input bit-streams */
    err_code = get_params(p_xa_process_api_obj,
            &i_num_chan,
            &acmod,
            &i_samp_freq,
            &sbrType,
            &bsfmt,
            &i_data_rate,
	    &pcm_sample_size);
    if ( err_code ) {
        /* Non-fatal error */
        fprintf(stderr, "Invalid Parameters detected after initialization\n");
    }



#ifdef WAV_HEADER
    /* Write the outputfile wave header */
    write_wav_header(g_pf_out,i_samp_freq,i_num_chan,pcm_sample_size,
            frameSize, frameCounter, i_total_bytes/(pcm_sample_size/8));
#endif

    ui_exec_done = 0;
#ifdef ENABLE_FRAME_BY_FRAME_DECODE
    {
    // while executing, allow the decoder to look ahead
    // This will ensure that the decoder can auto detect the stream change. 
    // This is optional if stream change need not be detected
#ifdef ENABLE_STREAM_CHANGE_DETECTION
    if(ui_init_done)
    {
        int frameByFrameDecode = 0;
        SET_CONFIG( XA_AACDEC_CONFIG_PARAM_ENABLE_FRAME_BY_FRAME_DECODE, &frameByFrameDecode, "Enable Frame By Frame Decode");        
    }
#endif        
    }
#endif

#if STEP_PLAY
    int play_time_ms = 0;
    int mute_bytes = 0;
#endif

    /* Third part for executing each process */
    do /* until we run out of data from input file */
    {

#if STEP_PLAY
        if (play_time_ms >= PLAY_TIME_MS) {
            play_time_ms = 0;

            API_CALL( XA_API_CMD_GET_CONFIG_PARAM, XA_AACDEC_CONFIG_PARAM_DATA_RATE, &i_data_rate, "Data-Rate Read Error");

            /* Mute the output for MUTE_TIME_MS milliseconds to separate
               the steps. */
            mute_bytes = ((MUTE_TIME_MS * i_samp_freq *
                        (pcm_sample_size/8) * outnchans) / 1000);

            i_total_bytes += mute_bytes;

            /* Based on the current bit rate, calculate the number of
               bytes that need to be skipped after consuming the bytes
               from the last EXECUTE call. */
            int skip_bytes = i_data_rate * SKIP_TIME_MS / (8 * 1000);

            /* Based on the number of bytes that need to be skipped,
               adjust the consumed input bytes. */
            int buf_remaining = inp_buffer.i_buff_size - inp_buffer.i_bytes_consumed;
            int buf_skip = (skip_bytes < buf_remaining) ? skip_bytes : buf_remaining;
            inp_buffer.i_bytes_consumed += buf_skip;
            skip_bytes -= buf_skip;

            /* If the input buffer does not contain all bytes that need to
               be skipped, seek forward in the input file. */
            if (skip_bytes > 0)
                fseek(g_pf_inp, skip_bytes, SEEK_CUR);

            fprintf(stderr, "Skip ahead by %d bytes\n", buf_skip+((skip_bytes>0)?skip_bytes:0));

            /* Read and Adjust current stream position */
            {
                unsigned int cur_strm_pos;
                GET_CONFIG(XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "Curr Stream Position Read Error");

                cur_strm_pos += (buf_skip + ((skip_bytes>0)?skip_bytes:0));

                SET_CONFIG( XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "Curr Stream Position Set Error");
            }

            /* Runtime init to avoid clicks and distortions. */
            API_CALL( XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL, "");

        }
#endif /* STEP_PLAY */



        if (raw_n_sideinfo) /* In case of DABPLUS_RAW_SIDEINFO format, read & feed Side info */
        {
            /* Get the side info for the next raw-payload from rawSideInfo file:
               Bits 0-14: Payload Size
               Bit    15: CRC Error
               */
            short raw_au_sideinfo = -1;
            int ret = fread(&raw_au_sideinfo, 2, 1, g_pf_side); (void)ret;
            SET_CONFIG( XA_AACDEC_CONFIG_PARAM_RAW_AU_SIDEINFO, &raw_au_sideinfo, "Set Config RAW_AU_SIDEINFO Error");
        }
        {
            UpdateBuffer(&inp_buffer, bsfmt);

            if(inp_buffer.i_inp_over == 1)
            {
                /* Tell that the input is over in this buffer */
                API_CALL(XA_API_CMD_INPUT_OVER, 0, NULL, "Input Over - Error");
            }
            /* Set number of bytes to be processed */
            API_CALL(XA_API_CMD_SET_INPUT_BYTES, 0, &inp_buffer.i_buff_size, "Setting input-Bytes Error");
        }
#ifdef SCRATCH_TRASH_TEST
        memset(p_scratch, 0xca, scratch_size);
#endif /* #ifdef SCRATCH_TRASH_TEST */
#if PROFILE
        times(&start);
        GMON_PROFILE_ENABLE
#endif
            /* Execute process */
            API_CALL(XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_EXECUTE, NULL, "Decoding Error");
            err_code_exec = err_code;

#if PROFILE
        GMON_PROFILE_DISABLE
            times(&stop);
#endif

        /* Checking for end of processing */
        API_CALL(XA_API_CMD_EXECUTE, XA_CMD_TYPE_DONE_QUERY, &ui_exec_done, "End-exec Check Error");

        /* Get the output bytes */
        API_CALL(XA_API_CMD_GET_OUTPUT_BYTES, 0, &i_out_bytes, "Get Output Bytes Error");

        /* How much buffer is used in input buffers */
        API_CALL(XA_API_CMD_GET_CURIDX_INPUT_BUF, 0, &inp_buffer.i_bytes_consumed, "Input Buffer Consumed Check Error");

        if ( i_out_bytes ) /* Frame decoded */
        {
            if(pcm_sample_size == 16)
                frameSize = (i_out_bytes/2)/outnchans;
            else
                frameSize = (i_out_bytes/4)/outnchans;

#if PROFILE
            /* Assume that if no output was produced, no significant cycles were consumed.  */
            {
                clock_t cycles = stop.tms_utime - start.tms_utime;
                double Curr = ((double)cycles/(frameSize) * (double)(i_samp_freq) / 1000000.0);
                Sum += Curr;

                frame++;
                Ave = Sum / frame;

                if (Peak < Curr)
                {
                    Peak = Curr;
                    Peak_frame = frame;
                }

                /*
                   Calculate the current time based on the total number of
                   samples produced by the decoder and the sample rate:

                   time [ms] = samples / sample_rate [kHz]

                   Alternatively, the current time can be calculated based on
                   the total number of bytes consumed by the decoder and the
                   data rate:

                   time [ms] = 8 * bytes / data_rate [kbps]
                   */
                total_samples += frameSize;

#ifndef DISABLE_MCPS_PRINT                
                unsigned long long total_msec =
                    (unsigned long long)((double)total_samples / i_samp_freq * 1000.0);
                int msec = (int)(total_msec % 1000);
                unsigned long long total_seconds = total_msec / 1000;
                int seconds = (int)(total_seconds % 60);
                int minutes = (int)(total_seconds / 60);
#endif

                /* Read decoded frame stream position */
                int decoded_frame_strm_pos;

                GET_CONFIG( XA_CONFIG_PARAM_GEN_INPUT_STREAM_POS, &decoded_frame_strm_pos, "Get Decoded Frame Stream Position Error");

#ifndef DISABLE_MCPS_PRINT                
                fprintf(stderr, "[%d|%u] %d:%02d.%03d MCPS: %.2f Average: %.2f Peak: %.2f @ [%d]\n",
                        frame, decoded_frame_strm_pos, minutes, seconds, msec,
                        Curr, Ave, Peak, Peak_frame);
#else
                fprintf(stderr, "[%d|%u]\n", frame, decoded_frame_strm_pos);
#endif
            }
#endif /* PROFILE */

            /* write audio channels to pcm file */
            if (frameCounter >= startFrame) {
                i_total_bytes += (pcm_write((long *)pb_out_buf,chanmap,frameSize,
                            g_pf_out,pcm_sample_size))*pcm_sample_size/8;
            }
            frameCounter++;

            if ((stopFrame != -1) && (frameCounter >= stopFrame)) {
                break;
            }

            /* every 16 frames, one can get the bitrate estimate */
            if ((frameCounter & 0xf) == 0) {
                GET_CONFIG( XA_AACDEC_CONFIG_PARAM_DATA_RATE, &i_data_rate, "Data-Rate Read Error");
            }
        }

        /* Check if exec returned a stream-mode change error */
        if ( err_code_exec == XA_AACDEC_EXECUTE_NONFATAL_STREAM_CHANGE ) {
            int reInit_on_streamChange = 0;
            fprintf(stderr, "Re-Init may be required %x\n", err_code_exec);

            if (reInit_on_streamChange) {
                /* ReInit if stream mode change is detected */
                fprintf(stderr, "Re-Init in progress (bytes_consumed=%d)\n", inp_buffer.i_bytes_consumed);
                reInitFlag = 1;
                goto Init_Point;
            }
            else {
                if (inp_buffer.i_bytes_consumed == 0) inp_buffer.i_bytes_consumed++;
                fprintf(stderr, "Ignore Re-Init skip to next frame (bytes_consumed=%d)\n", inp_buffer.i_bytes_consumed);
                /* Not reInit if stream mode change is detected */
                unsigned int cur_strm_pos;
                GET_CONFIG(XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "Curr Stream Position Read Error");
                fprintf(stderr, "Before cur_strm_pos %d\n", cur_strm_pos);

                cur_strm_pos += inp_buffer.i_bytes_consumed;

                SET_CONFIG( XA_CONFIG_PARAM_CUR_INPUT_STREAM_POS, &cur_strm_pos, "Curr Stream Position Set Error");
                fprintf(stderr, "After cur_strm_pos %d\n", cur_strm_pos);
            }
        }
#if 0 //def EXTERN_DWNMIX_DVBINFO
        {
            static FILE * g_pf_dvbinfo = (FILE *)0;
            if (g_pf_dvbinfo == NULL) g_pf_dvbinfo = fopen("dvbDmxData.dat", "w");
            static xa_aac_dec_dwnmix_level_dvb_info_t gDvbDmxData;
            xa_aac_dec_dwnmix_level_dvb_info_t *pDvbDmxInfo = &gDvbDmxData;

            GET_CONFIG( XA_AACDEC_CONFIG_PARAM_DWNMIX_LEVEL_DVB, (void *)&gDvbDmxData, "Get DVB DmxData Error");

            fprintf (g_pf_dvbinfo, "DvbDmxData = (New Data?: %d) cmix:%d,%d surmix:%d,%d\n", \
                    pDvbDmxInfo->new_dvb_downmix_data, \
                    (int)pDvbDmxInfo->center_mix_level_on, (int)pDvbDmxInfo->center_mix_level_value, \
                    (int)pDvbDmxInfo->surround_mix_level_on, (int)pDvbDmxInfo->surround_mix_level_value);

            fprintf (g_pf_dvbinfo, "DvbDmxData = (New Data?: %d) MpegAudioData: %d DolbySurroundMode: %d CoarseTimeCode:%d,%d FineTimeCode:%d,%d\n", \
                    pDvbDmxInfo->new_dvb_downmix_data, \
                    pDvbDmxInfo->mpeg_audio_type, pDvbDmxInfo->dolby_surround_mode, \
                    (int)pDvbDmxInfo->coarse_grain_timecode_on, (unsigned int)pDvbDmxInfo->coarse_grain_timecode_value, \
                    (int)pDvbDmxInfo->fine_grain_timecode_on, (unsigned int)pDvbDmxInfo->fine_grain_timecode_value);

            fprintf (stderr, "DvbDmxData = (New Data?: %d) cmix:%d,%d surmix:%d,%d\n", \
                    pDvbDmxInfo->new_dvb_downmix_data, \
                    (int)pDvbDmxInfo->center_mix_level_on, (int)pDvbDmxInfo->center_mix_level_value, \
                    (int)pDvbDmxInfo->surround_mix_level_on, (int)pDvbDmxInfo->surround_mix_level_value);

            fprintf (stderr, "DvbDmxData = (New Data?: %d) MpegAudioData: %d DolbySurroundMode: %d CoarseTimeCode:%d,%d FineTimeCode:%d,%d\n", \
                    pDvbDmxInfo->new_dvb_downmix_data, \
                    pDvbDmxInfo->mpeg_audio_type, pDvbDmxInfo->dolby_surround_mode, \
                    (int)pDvbDmxInfo->coarse_grain_timecode_on, (unsigned int)pDvbDmxInfo->coarse_grain_timecode_value, \
                    (int)pDvbDmxInfo->fine_grain_timecode_on, (unsigned int)pDvbDmxInfo->fine_grain_timecode_value);
        }
#endif

#if STEP_PLAY
        if(pcm_sample_size == 16) {
            frameSize = (i_out_bytes/2)/outnchans;
        }
        else {
            frameSize = (i_out_bytes/4)/outnchans;
        }
        /* Keep track of the play back time. */
        play_time_ms += (frameSize * 1000) / (i_samp_freq);

        /* Mute the output after ramp-down. */
        if (err_code_exec == XA_NO_ERROR) {
            for (; mute_bytes > 0; mute_bytes--) {
                putc(0, g_pf_out);
            }
        }
#endif /* STEP_PLAY */

#ifdef INPUT_PING_PONG
        if(inp_buffer.pb_inp_buf == p_input_buffer_ping)
        {
            memcpy(p_input_buffer_pong, p_input_buffer_ping, inp_buffer.i_buff_size);
            memset(p_input_buffer_ping, 0x0, inp_buffer.i_buff_size);
            inp_buffer.pb_inp_buf = p_input_buffer_pong;
        }
        else
        {
            memcpy(p_input_buffer_ping, p_input_buffer_pong, inp_buffer.i_buff_size);
            memset(p_input_buffer_pong, 0x0, inp_buffer.i_buff_size);
            inp_buffer.pb_inp_buf = p_input_buffer_ping;
        }

        /* Set Input Buffer Pointer */
        API_CALL(XA_API_CMD_SET_MEM_PTR, inp_idx, inp_buffer.pb_inp_buf, "Error in set mem-ptr");
#endif /* #ifdef INPUT_PING_PONG */

#ifdef OUTPUT_PING_PONG
        if(pb_out_buf == p_output_buffer_pong)
            pb_out_buf = p_output_buffer_ping;
        else
            pb_out_buf = p_output_buffer_pong;

        /* Set the buffer pointers */
        API_CALL(XA_API_CMD_SET_MEM_PTR, out_idx, pb_out_buf, "Error in set mem-ptr");
#endif /* #ifdef OUTPUT_PING_PONG */


        /* Check if done */
        if (ui_exec_done) break ;

    } while (1); /* Loop to feed new data chunks from the input file */

#if PROFILE
    fprintf(stdout,"\nPeak MCPS = %f\n", Peak);
    fprintf(stdout,"Peak frame = %d\n", Peak_frame);
    fprintf(stdout,"Average MCPS = %f\n", Ave);
    fprintf(stdout,"Average Data Rate = %d bps\n", i_data_rate);
#endif

    if (err_code != 0) {
        fprintf(stderr,"error while decoding stream\n");
    }

#ifdef WAV_HEADER
    if(!fseek(g_pf_out,0,SEEK_SET)) {
        write_wav_header(g_pf_out, i_samp_freq, i_num_chan, pcm_sample_size, frameSize, frameCounter, i_total_bytes/(pcm_sample_size/8));
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

/****************************************************************************
 *   Main function for test-wrapper                                         *
 ****************************************************************************/

    int
main (int   argc, char *argv[])
{
    FILE *param_file_id;

    char curr_cmd[XA_MAX_CMD_LINE_LENGTH];
    int fargc, curpos;
    int processcmd = 0;

    char fargv[XA_MAX_ARGS][XA_MAX_CMD_LINE_LENGTH];
    char *pargv[XA_MAX_ARGS];
    
    memset(pb_input_file_path,  0, XA_MAX_CMD_LINE_LENGTH);
    memset(pb_output_file_path, 0, XA_MAX_CMD_LINE_LENGTH);
    memset(pb_input_file_name,  0, XA_MAX_CMD_LINE_LENGTH);
    memset(pb_output_file_name, 0, XA_MAX_CMD_LINE_LENGTH);

    // NOTE: set_wbna() should be called before any other dynamic
    // adjustment of the region attributes for cache.
    set_wbna(&argc, argv);

    xa_testbench_error_handler_init();

    if(argc == 1) {
        if ((param_file_id = fopen(PARAMFILE, "r")) == NULL ) {
            printf("Parameter file \"paramfile.txt\" not found.\n");
            printf("Command line usage:\n");
            printf("<decoder> -ifile:<infile> -ofile:<outfile>\n");
            printf("Detailed description of the options: <decoder> -h\n");
            exit(0);
        }

        /* Process one line at a time */
        while(fgets(curr_cmd, XA_MAX_CMD_LINE_LENGTH, param_file_id)) {
            curpos = 0;
            fargc = 0;
            /* if it is not a param_file command and if */
            /* CLP processing is not enabled */
            if(curr_cmd[0] != '@' && !processcmd) {   /* skip it */
                continue;
            }

            while(sscanf(curr_cmd + curpos, "%s", fargv[fargc]) != EOF) {
                if(fargv[0][0]=='/' && fargv[0][1]=='/')
                    break;
                if(strcmp(fargv[0], "@echo") == 0)
                    break;
                if(strcmp(fargv[fargc], "@New_line") == 0) {
                    char * str = fgets(curr_cmd + curpos, XA_MAX_CMD_LINE_LENGTH,
                            param_file_id); (void)str;
                    continue;
                }
                curpos += strlen(fargv[fargc]);
                while(*(curr_cmd + curpos)==' ' || *(curr_cmd + curpos)=='\t')
                    curpos++;
                fargc++;
            }

            if(fargc < 1) /* for blank lines etc. */
                continue;

            if(strcmp(fargv[0], "@Output_path") == 0) {
                if(fargc > 1) strcpy(pb_output_file_path, fargv[1]);
                else strcpy(pb_output_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Input_path") == 0) {
                if(fargc > 1) strcpy(pb_input_file_path, fargv[1]);
                else strcpy(pb_input_file_path, "");
                continue;
            }

            if(strcmp(fargv[0], "@Start") == 0) {
                processcmd = 1;
                continue;
            }

            if(strcmp(fargv[0], "@Stop") == 0) {
                processcmd = 0;
                continue;
            }

            /* otherwise if this a normal command and its enabled for execution */
            if(processcmd) {
                int i;

                int file_flag = 0;

                printf("\n");
                for(i = 0; i < fargc; i++) {
                    printf("%s ", fargv[i]);
                    pargv[i] = fargv[i];

                    if(!strncmp(fargv[i], "-ifile:", 7)) {
                        char * pb_arg_val = fargv[i] + 7;
                        file_flag++;
                        strcat(pb_input_file_name, pb_input_file_path);
                        strcat(pb_input_file_name, pb_arg_val);
                    }

                    if(!strncmp(fargv[i], "-ofile:", 7)) {
                        char * pb_arg_val = fargv[i] + 7;
                        file_flag++;
                        strcat(pb_output_file_name, pb_output_file_path);
                        strcat(pb_output_file_name, pb_arg_val);
                    }
                }
                MEM_init();
                printf("\n");

                xa_aac_dec_main_process(fargc, pargv);

                MEM_freeall();

                if(g_pf_inp) {
                    fclose(g_pf_inp);
                }
                if(g_pf_out) {
                    fclose(g_pf_out);
                }
                if(g_pf_side) {
                    fclose(g_pf_side);
                }
                memset(pb_input_file_name, 0, XA_MAX_CMD_LINE_LENGTH);
                memset(pb_output_file_name, 0, XA_MAX_CMD_LINE_LENGTH);
            }
        }
    }
    else {
        int i;

        int file_flag = 0;

        printf("\n");
        for(i = 1; i < argc; i++) {
            printf("%s ", argv[i]);
            if(!strcmp(argv[i], "-h")) {
                showusage();
                return(0);
            }
            if(!strncmp(argv[i], "-ifile:", 7)) {
                char * pb_arg_val = argv[i] + 7;
                file_flag++;
                strcat(pb_input_file_name, pb_input_file_path);
                strcat(pb_input_file_name, pb_arg_val);
            }

            if(!strncmp(argv[i], "-ofile:", 7)) {
                char * pb_arg_val = argv[i] + 7;
                file_flag++;
                strcat(pb_output_file_name, pb_output_file_path);
                strcat(pb_output_file_name, pb_arg_val);
            }
        }

        printf("\n");
        MEM_init();


        xa_aac_dec_main_process(argc - 1, &argv[1]);

        MEM_freeall();
        if(g_pf_inp) {
            fclose(g_pf_inp);
        }
        if(g_pf_out) {
            fclose(g_pf_out);
        }
        if(g_pf_side) {
            fclose(g_pf_side);
        }
        memset(pb_input_file_name, 0, XA_MAX_CMD_LINE_LENGTH);
        memset(pb_output_file_name, 0, XA_MAX_CMD_LINE_LENGTH);
    }

    return XA_NO_ERROR;
} /* end main decode function */

pVOID g_pv_arr_alloc_memory[MAX_MEM_ALLOCS];
WORD  g_w_malloc_count;

void MEM_init(void)
{
    g_w_malloc_count = 0;
}

void *MEM_malloc (int size, int align)
{
    if ( size == 0)
    {
        return NULL;
    }
    void *return_ptr;
    /* Allocate memory for API */
    g_pv_arr_alloc_memory[g_w_malloc_count] = malloc(size);

    if (g_pv_arr_alloc_memory[g_w_malloc_count] == NULL)
    {

        return NULL;
    }
    memset(g_pv_arr_alloc_memory[g_w_malloc_count], 0, size);

    /* API object requires 4 bytes (WORD32) alignment;
     * malloc() provides at least 8-byte alignment.
     */
    return_ptr = g_pv_arr_alloc_memory[g_w_malloc_count];
    g_w_malloc_count++;

    return return_ptr;
}

void MEM_freeall(void)
{
    int i;
    for(i = 0; i < g_w_malloc_count; i++)
    {
        if(g_pv_arr_alloc_memory[i])
        {
            free(g_pv_arr_alloc_memory[i]);
        }

    }
}

void InitBuffer(inp_buffer_t *pbuf, void *ptr, int size)
{
    pbuf->pb_inp_buf = (pWORD8)ptr;
    pbuf->ui_inp_size = size;
    pbuf->i_bytes_consumed = 0;
    pbuf->i_buff_size = 0;
    pbuf->i_inp_over = 0;   
}
    void
xa_shift_input_buffer (signed char *buf, int buf_size, int bytes_consumed)
{
    int i;
    ae_p16s *dst;
    ae_p16s *src;

    if (bytes_consumed <= 0)
        return;

    /* Optimize 2-byte aligned data movement. */
    if ((((int)buf | buf_size | bytes_consumed) & 1) == 0) {
        /* Optimize 4-byte aligned data movement. */
        if ((((int)buf | buf_size | bytes_consumed) & 2) == 0) {
            ae_p16x2s *dst = (ae_p16x2s *)buf;
            ae_p16x2s *src = (ae_p16x2s *)&buf[bytes_consumed];
            for (i = 0; i < (buf_size - bytes_consumed) >> 2; i++) {
                dst[i] = src[i];
            }
            return;
        }

        dst = (ae_p16s *)buf;
        src = (ae_p16s *)&buf[bytes_consumed];
        for (i = 0; i < (buf_size - bytes_consumed) >> 1; i++) {
            dst[i] = src[i];
        }
        return;
    }

    /* Default, non-aligned data movement. */
    for (i = 0; i < buf_size - bytes_consumed; i++) {
        buf[i] = buf[i + bytes_consumed];
    }
}
void UpdateBuffer(inp_buffer_t *pbuf, int bsfmt)
{
    xa_shift_input_buffer(pbuf->pb_inp_buf, pbuf->i_buff_size, pbuf->i_bytes_consumed);
    pbuf->i_buff_size -= pbuf->i_bytes_consumed;
    int i_bytes_requested = 0;
#ifdef SMALL_INPUT_CHUNK_FEED
    // Small Input Chunk supported only for ADTS and ADIF !!
    if ((bsfmt == XA_AACDEC_EBITSTREAM_TYPE_AAC_LOAS) || (bsfmt == XA_AACDEC_EBITSTREAM_TYPE_AAC_ADTS))
    {

#define __MIN(A, B) (A<B)? A : B;
        int emptyspace = (int)(pbuf->ui_inp_size-(pbuf->i_buff_size));

        i_bytes_requested = (pbuf->i_bytes_consumed == 0)? varChunkSize:pbuf->i_bytes_consumed;
        i_bytes_requested = __MIN(i_bytes_requested , varChunkSize);
        i_bytes_requested = __MIN(i_bytes_requested, emptyspace);

    }
    else
#endif
    {
        /// Fill all empty space
        i_bytes_requested = ((int)pbuf->ui_inp_size- pbuf->i_buff_size);
    }

    pbuf->i_bytes_read = fread (pbuf->pb_inp_buf+pbuf->i_buff_size, 1, i_bytes_requested, g_pf_inp) ;

    pbuf->i_buff_size += pbuf->i_bytes_read;

    if(
#ifdef DAB_PLUS
            /*(pbuf->i_buff_size <= 0)*/    (pbuf->i_bytes_read != i_bytes_requested)
#else // DAB_PLUS
#ifdef SMALL_INPUT_CHUNK_FEED
            ((i_bytes_requested > 0) && (pbuf->i_bytes_read == 0))
#else // SMALL_INPUT_CHUNK_FEED
            ((pbuf->i_buff_size <= 0) || ((pbuf->i_buff_size < (int)pbuf->ui_inp_size) &&(pbuf->i_bytes_read == 0)))
#endif  // SMALL_INPUT_CHUNK_FEED
#endif // DAB_PLUS
      )
    {
#ifndef SMALL_INPUT_CHUNK_FEED
        if (pbuf->i_buff_size < 0)
            pbuf->i_buff_size = 0;
#endif
        pbuf->i_inp_over = 1;
    }
}

