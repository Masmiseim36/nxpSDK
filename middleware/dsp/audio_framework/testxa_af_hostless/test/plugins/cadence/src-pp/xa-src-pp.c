/*
* Copyright (c) 2015-2022 Cadence Design Systems Inc.
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

/*******************************************************************************
 * xa-src-pp.c
 *
 * SRC plugin - wrapper around SRC-PP library
 ******************************************************************************/
#ifdef XA_SRC_PP_FX

#define MODULE_TAG                      SRCPP

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <string.h>
#include "xa_apicmd_standards.h"
#include "xa_error_standards.h"
#include "audio/xa-audio-decoder-api.h"
#ifndef PACK_WS_DUMMY
#include "xa_src_pp_api.h"
#else //PACK_WS_DUMMY
static XA_ERRORCODE xa_src_pp(xa_codec_handle_t var1, WORD32 var2, WORD32 var3, pVOID var4){return 0;};
#endif //PACK_WS_DUMMY


#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t src_cycles;
#endif

/*******************************************************************************
 * Local Structures
 ******************************************************************************/
typedef struct xa_src_pp
{
    VOID *p_inpbuf;
    VOID *p_outbuf;
    VOID *p_scratch_buf;
    WORD32 in_bytes;
    WORD32 out_bytes;
    WORD32 consumed_bytes;
    WORD32 in_channels;
    WORD32 in_fs;
    WORD32 out_fs;
    WORD32 chunk_size;
    WORD32 pcm_width_bytes;
    WORD32 input_over;
    WORD32 exec_done;
} xa_src_pp_t;

/*******************************************************************************
 * Override GET-CONFIG-PARAM function
 ******************************************************************************/

static inline XA_ERRORCODE xa_src_pp_get_config_param(xa_src_pp_t *p_src_state, xa_codec_handle_t handle, WORD32 i_idx, pVOID pv_value)
{
    if (p_src_state == NULL) 
    {
        *(WORD32 *) pv_value = 0;
        return XA_NO_ERROR;
    }
    
#ifndef PACK_WS_DUMMY
    /* ...translate "standard" parameter index into internal value */
    switch (i_idx)
    {
    case XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS:
    case XA_CODEC_CONFIG_PARAM_CHANNELS:
        *(WORD32 *) pv_value = p_src_state->in_channels;
        break;
        
    case XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE:
    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
        *(WORD32 *) pv_value = p_src_state->out_fs;
        break;
        
    case XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE:
        *(WORD32 *) pv_value = p_src_state->pcm_width_bytes;
        break;

    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
        *(WORD32 *) pv_value = p_src_state->pcm_width_bytes * 8;
        break;
    }
#endif //PACK_WS_DUMMY
    
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Override SET-CONFIG-PARAM function
 ******************************************************************************/

static inline XA_ERRORCODE xa_src_pp_set_config_param(xa_src_pp_t *p_src_state, xa_codec_handle_t handle, WORD32 i_idx, pVOID pv_value)
{
#ifndef PACK_WS_DUMMY
    if (p_src_state != NULL)
    {
        switch (i_idx)
        {
        case XA_SRC_PP_CONFIG_PARAM_INPUT_CHANNELS:
            p_src_state->in_channels = *(WORD32 *) pv_value;
            break;

        case XA_SRC_PP_CONFIG_PARAM_INPUT_SAMPLE_RATE:
            p_src_state->in_fs = *(WORD32 *) pv_value;
            break;

        case XA_SRC_PP_CONFIG_PARAM_OUTPUT_SAMPLE_RATE:
            p_src_state->out_fs = *(WORD32 *) pv_value;
            break;

        case XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE:
            p_src_state->chunk_size = *(WORD32 *) pv_value;
            break;

        case XA_SRC_PP_CONFIG_PARAM_BYTES_PER_SAMPLE:
            p_src_state->pcm_width_bytes = *(WORD32 *) pv_value;
            break;
        }
    }
#endif //PACK_WS_DUMMY
    
    /* ...pass to library */
    return xa_src_pp(handle, XA_API_CMD_SET_CONFIG_PARAM, i_idx, pv_value);       
}

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_src_pp_fx (xa_codec_handle_t p_xa_module_hdl, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif
    xa_src_pp_t *p_src_state;
    xa_codec_handle_t p_xa_module_obj;
 
    p_src_state = (xa_src_pp_t *) p_xa_module_hdl;

    if (p_xa_module_hdl == NULL)
        p_xa_module_obj = NULL;
    else
        p_xa_module_obj = (xa_codec_handle_t) (((WORD32)((UWORD8 *) p_xa_module_hdl + sizeof(xa_src_pp_t) + 7) >> 3) << 3);

    if (i_cmd == XA_API_CMD_GET_CONFIG_PARAM)
    {
        return xa_src_pp_get_config_param(p_src_state, p_xa_module_obj, i_idx, pv_value);
    }
    if (i_cmd == XA_API_CMD_SET_CONFIG_PARAM)
    {
        return xa_src_pp_set_config_param(p_src_state, p_xa_module_obj, i_idx, pv_value);
    }

    if (i_cmd == XA_API_CMD_GET_CURIDX_INPUT_BUF)
    {
        *(WORD32 *) pv_value = p_src_state->consumed_bytes;  
        return XA_NO_ERROR;
    }
   
    if (i_cmd == XA_API_CMD_SET_INPUT_BYTES)
    {
        WORD32 insize = *(WORD32 *) pv_value;
        
        p_src_state->in_bytes = insize;

        insize = insize/p_src_state->in_channels;
        insize = (p_src_state->pcm_width_bytes == 2) ? insize >> 1 : insize >> 2;

#ifndef PACK_WS_DUMMY
        xa_src_pp(p_xa_module_obj, XA_API_CMD_SET_CONFIG_PARAM, XA_SRC_PP_CONFIG_PARAM_INPUT_CHUNK_SIZE, &insize);
#endif //PACK_WS_DUMMY

        /* ... reset states to allow rerun. TENA-2544. */
        p_src_state->exec_done = 0;

        return XA_NO_ERROR;
    }

    if (i_cmd == XA_API_CMD_GET_OUTPUT_BYTES)
    {
        *(WORD32 *) pv_value = p_src_state->out_bytes;
        return XA_NO_ERROR;
    }

    if (i_cmd == XA_API_CMD_INPUT_OVER)
    {
        p_src_state->input_over = 1;
        return XA_NO_ERROR;
    }

    {
        if (i_cmd == XA_API_CMD_INIT && i_idx == XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS)
            memset(p_src_state, 0, sizeof(xa_src_pp_t));             

        if (i_cmd == XA_API_CMD_SET_MEM_PTR)
        {
            if (i_idx == 2 /*INPUT_IDX*/ ) p_src_state->p_inpbuf = pv_value;
            if (i_idx == 3 /*OUTPUT_IDX*/) p_src_state->p_outbuf = pv_value;
        }

#ifndef PACK_WS_DUMMY
        if (i_cmd == XA_API_CMD_EXECUTE && i_idx == XA_CMD_TYPE_DO_EXECUTE)
        {
            p_src_state->out_bytes = 0;
            p_src_state->consumed_bytes = 0;
            xa_src_pp(p_xa_module_obj, XA_API_CMD_SET_CONFIG_PARAM, XA_SRC_PP_CONFIG_PARAM_SET_INPUT_BUF_PTR,  p_src_state->p_inpbuf);
            xa_src_pp(p_xa_module_obj, XA_API_CMD_SET_CONFIG_PARAM, XA_SRC_PP_CONFIG_PARAM_SET_OUTPUT_BUF_PTR, p_src_state->p_outbuf);
        }
#endif //PACK_WS_DUMMY
       
        if (p_src_state != NULL) 
        { 
            if ((p_src_state->input_over == 1) && (p_src_state->in_bytes == 0))
                xa_src_pp(p_xa_module_obj, XA_API_CMD_INPUT_OVER, 0, NULL);
        }

#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = xa_src_pp(p_xa_module_obj, i_cmd, i_idx, pv_value);

#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        src_cycles += clk_diff(comp_stop, comp_start);
#endif
        if (i_cmd == XA_API_CMD_EXECUTE && i_idx == XA_CMD_TYPE_DO_EXECUTE && ret == XA_NO_ERROR)
        {
            WORD32 outsize;

#ifndef PACK_WS_DUMMY
            xa_src_pp(p_xa_module_obj, XA_API_CMD_GET_CONFIG_PARAM, XA_SRC_PP_CONFIG_PARAM_OUTPUT_CHUNK_SIZE, &outsize);
#else //PACK_WS_DUMMY
            outsize=0;
#endif //PACK_WS_DUMMY

            outsize = outsize * p_src_state->in_channels;
            outsize = (p_src_state->pcm_width_bytes == 2) ? outsize << 1 : outsize << 2;
            p_src_state->out_bytes = outsize;
            p_src_state->consumed_bytes = p_src_state->in_bytes;
            p_src_state->in_bytes = 0;

            p_src_state->exec_done = p_src_state->input_over; /* set state exec_done if input_over. TENA-2544 */
            p_src_state->input_over = 0;    /* reset input over, once exec_done is declared. TENA-2544 */
        }

        if (i_cmd == XA_API_CMD_GET_API_SIZE)
        {
            *(WORD32 *) pv_value += (sizeof(xa_src_pp_t) + 7);     
        }

        if((i_cmd == XA_API_CMD_SET_MEM_PTR) && (i_idx == XA_MEMTYPE_SCRATCH) && (ret == XA_NO_ERROR))
        {
            if(p_src_state->p_scratch_buf && ((unsigned int)p_src_state->p_scratch_buf != (unsigned int)pv_value))
            {
                /* ...necessary for the new scratch pointer changed durig run-time can be propagated 
                 * into the library, else previous scratch pointer will be used which can lead to 
                 * undesired side effects. */
                ret = xa_src_pp(p_xa_module_obj, XA_API_CMD_EXECUTE, XA_CMD_TYPE_DO_RUNTIME_INIT, NULL);
            }
            p_src_state->p_scratch_buf = pv_value;
        }

        return ret;
    }
}
#endif
