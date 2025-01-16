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


#include "xa_drm_dec_sample_testbench.h"

#if 1 //DRM_SUPPORTED
#include "xa_drm_dec_drm_utils.h"
#endif
#ifdef RAW_AAC_SUPPORTED
#include "xa_drm_dec_raw_aac_utils.h"
#endif


XA_ERRORCODE get_libconfig(xa_testapp_drm_dec_state *pobj)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;

#ifdef RAW_AAC_SUPPORTED
    GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_INPUT_FORMAT,    (void *)&pobj->cfg.bsfmt.val,   "Get default input format");
#endif
    GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_QMF_MODE,        (void *)&pobj->cfg.qmfLowPower.val,   "Get default qmfLowPower");

    return err_code;
}

static void prefix_dirname(char *fname, char *dirname)
{
    if(strlen(dirname) > 0)
    {
        char tempname[MAX_CMD_LINE_LENGTH];
        strncpy(tempname, fname, MAX_CMD_LINE_LENGTH);
        // Remove additional / slash from directory name
        if(dirname[strlen(dirname)-1] == '/')
        {
            dirname[strlen(dirname)-1] = '\0';
        }
        snprintf(fname,MAX_CMD_LINE_LENGTH, "%s/%s", dirname, tempname);
    }
}

XA_ERRORCODE cmdlineparse(user_config_t *pcfg, int argc, char *argv[])
{
    int argidx;

    if(argc <= 0)
    {
        return XA_TESTBENCH_FATAL_INVALID_ARGUMENT;
    }
    for(argidx = 0; argidx < argc; argidx+=1)
    {
        if(strcmp((argv[argidx]), ("-h")) == 0)
        {
            return XA_TESTBENCH_FATAL_HELP_REQUESTED;
        }
    }

    for(argidx = 0; argidx < (argc-1); argidx+=2)
    {
        ARGTYPE_STR("-i",   ifile);
        ARGTYPE_STR("-if",   ifile);
        ARGTYPE_STR("-o",   ofile);
        ARGTYPE_STR("-of",   ofile);


        ARGTYPE_INT("-y", startFrame);
        ARGTYPE_INT("-z", stopFrame);

#ifdef RAW_AAC_SUPPORTED
        ARGTYPE_INT("-fmt", bsfmt);
#endif
        ARGTYPE_INT("-p",qmfLowPower);
        ARGTYPE_INT("-om",outputMode);
        ARGTYPE_INT("-os", isInHoldOffMode);

        ARGTYPE_INT("-cfi", conceal_fi);
        ARGTYPE_INT("-cfo", conceal_fo);
        ARGTYPE_INT("-cmr", conceal_mr);
        ARGTYPE_INT("-ccn", conceal_cn_level);

        PRINT_INVALID_ARG(argv[argidx])
        return XA_TESTBENCH_FATAL_INVALID_ARGUMENT;
    }

    // check if we have both ifile and ofile, and add path based on paramfile
    if(pcfg->ifile[0] == '\0')
    {
        return XA_TESTBENCH_FATAL_INPUT_NOT_SPECIFIED;
    }
    if(pcfg->ofile[0] == '\0')
    {
        return XA_TESTBENCH_FATAL_OUTPUT_NOT_SPECIFIED;
    }

    prefix_dirname(pcfg->ifile, pb_input_file_path);
    prefix_dirname(pcfg->ofile, pb_output_file_path);

    if(pcfg->startFrame.val < 0 || pcfg->stopFrame.val < 0 || pcfg->stopFrame.val < pcfg->startFrame.val)
    {
        return XA_TESTBENCH_FATAL_INVALID_ARGUMENT;
    }
    return XA_NO_ERROR;

}

void showusage()
{
#define STRING(str) str "\n"
    fprintf(stderr,
            #include "xa_drm_dec_usage.h"
                );
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
    if ((((int)buf | buf_size | bytes_consumed) & 1) == 0)
    {
        /* Optimize 4-byte aligned data movement. */
        if ((((int)buf | buf_size | bytes_consumed) & 2) == 0)
        {
            ae_p16x2s *dst = (ae_p16x2s *)buf;
            ae_p16x2s *src = (ae_p16x2s *)&buf[bytes_consumed];
            for (i = 0; i < (buf_size - bytes_consumed) >> 2; i++)
            {
                dst[i] = src[i];
            }
            return;
        }

        dst = (ae_p16s *)buf;
        src = (ae_p16s *)&buf[bytes_consumed];
        for (i = 0; i < (buf_size - bytes_consumed) >> 1; i++)
        {
            dst[i] = src[i];
        }
        return;
    }

    /* Default, non-aligned data movement. */
    for (i = 0; i < buf_size - bytes_consumed; i++)
    {
        buf[i] = buf[i + bytes_consumed];
    }
}

XA_ERRORCODE INP_Open(HANDLE_INP *p_inp, char *filename
#ifdef RAW_AAC_SUPPORTED
        , int fmt
#endif
        )
{
    XA_ERRORCODE xa_error = XA_TESTBENCH_MFMAN_FATAL_FILE_OPEN_FAILED;

#ifdef RAW_AAC_SUPPORTED
    p_inp->inp_fmt = fmt;
	/* TODO */

    if(fmt == XA_DRM_DEC_INPUT_FORMAT_DRM)
#endif
    {
        xa_error = DRM_InputOpen(&p_inp->drm,filename);
    }
#ifdef RAW_AAC_SUPPORTED
    if(fmt == XA_DRM_DEC_INPUT_FORMAT_RAW_AAC )
    {
        xa_error = RAW_InputOpen(&p_inp->raw,filename);
    }
#endif
    return xa_error;
}

XA_ERRORCODE INP_Read(HANDLE_INP *pinp, buffer_t *pbuf)
{
    XA_ERRORCODE xa_error = XA_TESTBENCH_MFMAN_FATAL_FILE_READ_FAILED;
	/* TODO */
#ifdef RAW_AAC_SUPPORTED
    if(pinp->inp_fmt == XA_DRM_DEC_INPUT_FORMAT_DRM)
#endif
    {
        xa_error = DRM_InputRead(&pinp->drm,pbuf);

    }
#ifdef RAW_AAC_SUPPORTED
    if(pinp->inp_fmt == XA_DRM_DEC_INPUT_FORMAT_RAW_AAC )
    {
        xa_error = RAW_InputRead(&pinp->raw, pbuf );
    }
#endif
    return xa_error;
}

void INP_Close(HANDLE_INP *pinp)
{
	/* TODO */
#ifdef RAW_AAC_SUPPORTED
    if(pinp->inp_fmt == XA_DRM_DEC_INPUT_FORMAT_DRM)
#endif
    {
        DRM_inputClose(&pinp->drm);

    }
#ifdef RAW_AAC_SUPPORTED
 if(pinp->inp_fmt == XA_DRM_DEC_INPUT_FORMAT_RAW_AAC )
    {
        RAW_InputClose(&pinp->raw);
    }
#endif
return ;
}

/*****************************************************************************************
 * Function to get stream parameters
 *     After an initialization and synchronization phase, get_params function is called to
 *     get the parameters available by reading the input bit-stream.
 *****************************************************************************************/

int get_params(xa_testapp_drm_dec_state *pobj,
               int * p_numChannels,
               int *p_sampleRate,
               int * p_frame_size,
               int *p_pcm_width)
{
    int err_code = 0;
    int numChannels = 0, samp_freq = 0, frame_size = 0, pcm_width = 0;

    /* Get Number Of Channels in the Bit Stream */
    GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_OUT_NUM_CHANNELS, (void *)&numChannels, "Get num-channels");
    GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_SAMPLERATE, (void *)&samp_freq, "Get Sample-rate");
    GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_PCM_WDSZ, (void *)&pcm_width, "Get PCM-width");
    GET_CONFIG( XA_DRM_DEC_CONFIG_PARAM_FRAME_SIZE, (void *)&frame_size, "Get Frame-size");

    *p_numChannels = numChannels;
    *p_sampleRate = samp_freq;
    *p_frame_size = frame_size;
    *p_pcm_width = pcm_width;

    return err_code;
}

/*****************************************************************************************
 * Function to set stream parameters
 *     Before an initialization phase, set_params function is called to
 *     set the parameters available by reading the input arguments.
 *****************************************************************************************/

XA_ERRORCODE set_params(xa_testapp_drm_dec_state *pobj)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;

#ifdef RAW_AAC_SUPPORTED
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_INPUT_FORMAT,      &pobj->cfg.bsfmt,                 "Set Input Format  from User info");
#endif
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_QMF_MODE,               &pobj->cfg.qmfLowPower,   "SET qmfLowPower  from User info");
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_OUTPUT_MODE,               &pobj->cfg.outputMode,   "SET outputMode  from User info");

    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_XHEAAC_HOLDOFF_MODE,    &pobj->cfg.isInHoldOffMode,   "SET isInHoldOffMode flag from User info");
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_CONCEAL_FADEIN,    &pobj->cfg.conceal_fi,   "SET concealment fade-in length from User info");
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_CONCEAL_FADEOUT,    &pobj->cfg.conceal_fo,   "SET concealment fade-out length from User info");
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_CONCEAL_MUTE_RELEASE,    &pobj->cfg.conceal_mr,   "SET concealment mute-release length from User info");
    SET_UPDATED_CONFIG( XA_DRM_DEC_CONFIG_PARAM_CONCEAL_COMFORT_NOISE,    &pobj->cfg.conceal_cn_level,   "SET concealment comfort-noise level from User info");

    return err_code;
}

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
    if(((unsigned int) g_pv_arr_alloc_memory[g_w_malloc_count]) & (align-1))
    {
        // Alignment not met; free and return NULL
        free(g_pv_arr_alloc_memory[g_w_malloc_count]);
        return NULL;
    }
    assert( == 0);
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

