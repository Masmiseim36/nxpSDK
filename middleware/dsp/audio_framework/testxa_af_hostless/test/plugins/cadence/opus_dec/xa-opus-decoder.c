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
 * xa-opus-decoder.c
 *
 * OPUS decoder plugin - thin wrapper around OPUS-ENC library
 ******************************************************************************/
#ifdef XA_OPUS_DECODER

#define MODULE_TAG                      OPUS_DEC

/*******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "xa_type_def.h"
/* ...debugging facility */
#include "xf-debug.h"
#include "audio/xa-audio-decoder-api.h"
#include "audio/xa-opus-decoder-api.h"
#ifndef PACK_WS_DUMMY
#include "xa_ogg_lib_api.h"
#include "opus_header.h"
#endif //PACK_WS_DUMMY
#include "xaf-api.h"

#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t dec_cycles;
#endif

#if defined(XA_ZERO_COPY) && (XF_CFG_CORES_NUM > 1)
/* ...prevent instructions reordering */
#define barrier()                           \
    __asm__ __volatile__("": : : "memory")

/* ...memory barrier */
#define XF_IPC_BARRIER()                  \
    __asm__ __volatile__("memw": : : "memory")

#if (XF_CFG_CORES_NUM > 1)
#define XF_IPC_FLUSH(buf, length) \
        ({ if ((length)) { barrier(); xthal_dcache_region_writeback((buf), (length)); XF_IPC_BARRIER(); } buf; })

#define XF_IPC_INVALIDATE(buf, length) \
        ({ if ((length)) { xthal_dcache_region_invalidate((buf), (length)); barrier(); } buf; })
#else
#define XF_IPC_FLUSH(buf, length)   (void)0        
#define XF_IPC_INVALIDATE(buf, length)  (void)0
#endif  //XF_CFG_CORES_NUM > 1
#endif  //XA_ZERO_COPY


#ifndef PACK_WS_DUMMY
typedef struct XA_OPUS_Decoder
{
	   /* ... module state */
	    UWORD32            state;

	    /* ...number of channels (channel mask?) */
	    UWORD32            channels;

	    /* ...PCM sample width */
	    UWORD32            pcm_width;

	    /* ...sampling rate */
	    UWORD32            sample_rate;

	    /* ...number of bytes in input/output buffer */
	    UWORD32            buffer_size;

	    /* ...input buffer */
	    void               *input;

	    /* ...output buffer */
	    void               *output;

	    /* ...scratch buffer pointer */
	    void               *scratch;

	    /* ...number of available bytes in the input buffer */
	    UWORD32            input_avail;

	    /* ...number of bytes consumed from input buffer */
	    UWORD32            consumed;

	    /* ...number of produced bytes */
	    UWORD32            produced;

	    /* ...control structure for opus decoder */
	    xa_opus_dec_control_t dec_control;

	    /* ...opus decoder frame size */
	    UWORD32 			frame_size;

	    /* ...Opus dec handle */
	    UWORD32            internal_data __attribute__((aligned(8)));

        xa_opus_stream_type_t opus_stream_type;

        xa_ogg_parse_init_cfg_t ogg_cfg;

        xa_codec_handle_t  ogg_handle;

        pVOID              ogg_scratch;

        pWORD8             ogg_out_buf;

        pWORD8             ogg_inp_buf;

        WORD32             gran_offset;

        WORD32             preskip;

        WORD32             first_frame_parsing_done;

}XA_OPUS_Decoder;


/*******************************************************************************
 * OPU-Enc state flags
 ******************************************************************************/

#define XA_OPUS_DEC_FLAG_PREINIT_DONE      (1 << 0)
#define XA_OPUS_DEC_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_OPUS_DEC_FLAG_RUNNING           (1 << 2)
#define XA_OPUS_DEC_FLAG_OUTPUT            (1 << 3)
#define XA_OPUS_DEC_FLAG_EOS_RECEIVED      (1 << 4)
#define XA_OPUS_DEC_FLAG_COMPLETE          (1 << 5)

#define MAX_INP_CHANNELS 2 //TBD: BHANU_TEST max channels, need to modify argument for xa_opus_dec_get_handle_byte_size
#define _MIN(a, b)		(((a)<(b))?(a):(b))

static XA_ERRORCODE xa_opus_decoder_get_api_size(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    //printf("xa_opus_decoder_get_api_size: %d, %d\n", d->dec_control.nb_streams, d->dec_control.nb_coupled);
    //*(WORD32 *)pv_value = xa_opus_dec_get_handle_byte_size(d->dec_control.nb_streams, d->dec_control.nb_coupled);
    *(WORD32 *)pv_value = sizeof(XA_OPUS_Decoder);
    return XA_NO_ERROR;
}

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

static XA_ERRORCODE xa_opus_decoder_init(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
        {
            memset(d, 0, sizeof(*d));
            
            /* ...init defaults */
            {
                /* If stream type is XA_RAW_OPUS_STREAM and sample rate and number of channels are not passed from command line revert to default */
                d->dec_control.API_sampleRate = 48000;     /* Default for raw opus stream */
                d->dec_control.API_numChannels = XA_OPUS_MAX_NUM_CHANNELS;        /* Default for raw opus stream */
                d->dec_control.channel_mapping = 1;
                d->dec_control.nb_streams = XA_OPUS_MAX_NUM_STREAMS;
                d->dec_control.nb_coupled = XA_OPUS_MAX_NUM_STREAMS;
                d->ogg_cfg.max_page_size = 64;
                d->opus_stream_type = XA_OGG_OPUS_STREAM;
                d->first_frame_parsing_done = 0;
            }
            d->input_avail = 0;

            /* Mark Opus Enc component Pre-Initialization is DONE */
            d->state |= XA_OPUS_DEC_FLAG_PREINIT_DONE;
            return XA_NO_ERROR;
        }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
        {
            /* ...post-configuration initialization (all parameters are set) */
            XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

            /* Mark post-initialization is complete */
            d->state |= XA_OPUS_DEC_FLAG_POSTINIT_DONE;
            return XA_NO_ERROR;
        }

    case XA_CMD_TYPE_INIT_PROCESS:
        {
            XA_ERRORCODE err_code;
            WORD is_fatal;
            /* ...kick run-time initialization process; make sure AMR dec component is setup */
            XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

            if((d->opus_stream_type == XA_RAW_OPUS_STREAM) || (d->input_avail <= 0))
            {
                /* ...Call the core library initialization function */
                err_code = xa_opus_dec_init((xa_codec_handle_t)(d->internal_data), &d->dec_control);
                is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
                XF_CHK_ERR(!(is_fatal), err_code);
                /* ...enter into execution stage */
                d->state |= XA_OPUS_DEC_FLAG_RUNNING;
            }
            if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
            {
                err_code = xa_ogg_parse_init(d->ogg_handle, d->ogg_scratch, &d->ogg_cfg);
                is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
                XF_CHK_ERR(!(is_fatal), err_code);
            }
#if 1
            if((d->opus_stream_type == XA_OGG_OPUS_STREAM) && (d->input_avail > 0))
            {
                {
                    WORD32 packet_count = 0;
                    WORD32 read_ogg_inp = 1;
                    pWORD8 p_ogg_inp_buf = d->input;
                    int nBytes = 0;
                    int input_size = 0;
                    int input_data_not_consumed = d->input_avail;
                    int input_over = 0;
                    while(1)
                    {
                        nBytes = XA_OPUS_OGG_OUTPUT_BUFFER_SIZE;
                        input_size = 0;
                        if(read_ogg_inp && !(d->state & XA_OPUS_DEC_FLAG_EOS_RECEIVED))
                        {
                            //counter = fread(p_ogg_inp_buf, sizeof(WORD8), OGG_INP_SIZE, bitInFile);
                            read_ogg_inp = 0;
                            if(input_data_not_consumed < XA_OPUS_OGG_INPUT_BUFFER_SIZE)
                            {
                                input_over = 1;
                                input_size = input_data_not_consumed;
                            }
                            else
                            {
                                input_size = XA_OPUS_OGG_INPUT_BUFFER_SIZE;
                            }
                            input_data_not_consumed -= input_size;
                        }
                        err_code = xa_ogg_parse_process(d->ogg_handle,
                                                        (pUWORD8)p_ogg_inp_buf,
                                                        (pUWORD8)d->ogg_out_buf,
                                                        (pWORD32)&input_size,
                                                        &nBytes);
                        //_XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);
                        is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
                        XF_CHK_ERR(!(is_fatal), XA_API_FATAL_INVALID_CMD_TYPE);

                        d->consumed += input_size;

                        if(err_code != XA_NO_ERROR)
                        {
                            if(err_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                                read_ogg_inp = 1;
                            else
                            {
                                p_ogg_inp_buf += input_size;
                            }
                        }

                        if(nBytes > 0)
                        {
                            if(packet_count == 0)
                            {
                                OpusHeader opus_hdr;
                                /* parse header */
                                if (!opus_header_parse((pUWORD8)d->ogg_out_buf, nBytes, &opus_hdr))
                                {
                                    //_XA_HANDLE_ERROR(p_proc_testbench_err_info, "", XA_TESTBENCH_FATAL_OPUS_HDR_READ_FAILED);
                                    is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
                                    XF_CHK_ERR(!(is_fatal), XA_API_FATAL_INVALID_CMD_TYPE);
                                }
                                /* Check if OGG dec has sufficient memory to decode this stream
                                 * This can be verified from OGG header
                                 */
                                if((d->dec_control.API_numChannels   < opus_hdr.channels) ||
                                    (d->dec_control.nb_streams       < opus_hdr.nb_streams) ||
                                    (d->dec_control.nb_coupled       < opus_hdr.nb_coupled) ||
                                    (d->dec_control.channel_mapping  < opus_hdr.channel_mapping))
                                {
                                    printf("Fatal error: Insufficient memory to decode this stream.\n");
                                    return XA_API_FATAL_MEM_ALLOC;
                                }
                                /* If sample rate is not passed on command line, pick input_sample_rate from ogg stream header */
                                d->dec_control.API_sampleRate     = opus_hdr.input_sample_rate;
                                d->dec_control.API_numChannels    = opus_hdr.channels;
                                d->dec_control.nb_streams         = opus_hdr.nb_streams;
                                d->dec_control.nb_coupled         = opus_hdr.nb_coupled;
                                d->dec_control.gain               = opus_hdr.gain;
                                d->dec_control.version            = opus_hdr.version;
                                /* Channel mapping 1 implies multistream input, 0 implies single stream input */
                                d->dec_control.channel_mapping    = opus_hdr.channel_mapping;
                                /* Set stream_map from opus header if not passed on command line */
                                if(d->dec_control.channel_mapping == 1)
                                    memcpy(d->dec_control.stream_map, opus_hdr.stream_map, d->dec_control.API_numChannels);

                                d->gran_offset                    = opus_hdr.preskip;
                                d->preskip                        = opus_hdr.preskip;
                            }
                            else if(packet_count == 1)
                            {
                                fprintf(stdout, "\nOgg Stream Comments: \n");
                                /* Print comments */
                                print_comments((char *)d->ogg_out_buf, nBytes);
                                d->consumed += input_size;
                                packet_count++;
                                break;
                            }
                            packet_count++;
                        }
                        if(err_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                        {
                            if(input_over)
                            {
                                /* File ended, break */
                                printf( "Error: Ran out of input without finding header and comment packets");
                                return XA_OPUS_EXECUTE_FATAL_INIT_FAILURE;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    if((packet_count > 1) && (err_code == XA_NO_ERROR))
                    {
                        err_code = xa_opus_dec_init((xa_codec_handle_t)(d->internal_data), &d->dec_control);
                        is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
                        XF_CHK_ERR(!(is_fatal), err_code);
                        d->first_frame_parsing_done = 1;
                        /* ...enter into execution stage */
                        d->state |= XA_OPUS_DEC_FLAG_RUNNING;
                    }
                }

            }
#endif
            return XA_NO_ERROR;
        }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
        {
            /* ...check if initialization is done; make sure pointer is sane */
            XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

            /* ...put current status */
            *(WORD32 *)pv_value = (d->state & XA_OPUS_DEC_FLAG_RUNNING ? 1 : 0);
            return XA_NO_ERROR;
        }

    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

static XA_ERRORCODE xa_opus_decoder_set_config_param(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case XA_CODEC_CONFIG_PARAM_CHANNELS:
    case XA_OPUS_DEC_CONFIG_PARAM_CHANNELS:
        XF_CHK_ERR((*(WORD32 *)pv_value <= XA_OPUS_MAX_NUM_CHANNELS) && (*(WORD32 *)pv_value > 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.API_numChannels = *(WORD32 *)pv_value;
        d->channels		= *(WORD32 *)pv_value;
        break;

    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
    case XA_OPUS_DEC_CONFIG_PARAM_SAMPLE_RATE:
        d->dec_control.API_sampleRate = *(WORD32 *)pv_value;
        d->sample_rate	= *(WORD32 *)pv_value;
        break;

    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
    case XA_OPUS_DEC_CONFIG_PARAM_PCM_WIDTH:
        d->pcm_width	= *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_GAIN:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 32767) && (*(WORD32 *)pv_value >= (-32768)), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.gain = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_SILK_INBANDFEC_ENABLE:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 1) && (*(WORD32 *)pv_value >= 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
    	d->dec_control.SILK_InBandFEC_enabled = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_DEC_CONFIG_PARAM_FRAME_SIZE: /* deprecated */
    case XA_OPUS_DEC_CONFIG_PARAM_FRAME_SIZE_IN_SAMPLES:
    	d->frame_size = *(WORD32 *)pv_value;
    	break;
    case XA_OPUS_DEC_CONFIG_PARAM_FRAMES_PER_PACKET:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 6) && (*(WORD32 *)pv_value > 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.framesPerPacket = *(WORD32 *)pv_value;
    	break;

    case XA_OPUS_DEC_CONFIG_PARAM_NUM_STREAMS:
        XF_CHK_ERR((*(WORD32 *)pv_value <= XA_OPUS_MAX_NUM_STREAMS) && (*(WORD32 *)pv_value > 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.nb_streams = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_NUM_COUPLED_STREAMS:
        XF_CHK_ERR((*(WORD32 *)pv_value <= XA_OPUS_MAX_NUM_STREAMS) && (*(WORD32 *)pv_value > 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.nb_coupled = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_CHAN_MAPPING:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 1) && (*(WORD32 *)pv_value >= 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.channel_mapping = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_LOST_PACKET:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 1) && (*(WORD32 *)pv_value >= 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.lostFlag = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_EXCLUDE_RANGE_DEC_STATE:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 1) && (*(WORD32 *)pv_value >= 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->dec_control.no_range_dec_state = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_BITSTREAM_VERSION:
        d->dec_control.version = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_STREAM_TYPE:
        XF_CHK_ERR((*(WORD32 *)pv_value <= 1) && (*(WORD32 *)pv_value >= 0), XA_OPUS_DEC_CONFIG_NONFATAL_RANGE);
        d->opus_stream_type = *(WORD32 *)pv_value;
        if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
        {
            d->dec_control.no_range_dec_state = 1;
        }
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_OGG_MAX_PAGE_SIZE:
        d->ogg_cfg.max_page_size = *(WORD32 *)pv_value;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_STREAM_MAP:
    {
        xaf_ext_buffer_t *ext_buf = (xaf_ext_buffer_t *) pv_value;
#if defined(XA_ZERO_COPY) && (XF_CFG_CORES_NUM > 1)
#if XF_LOCAL_IPC_NON_COHERENT
        XF_IPC_INVALIDATE(ext_buf, sizeof(xaf_ext_buffer_t)); 
        XF_IPC_INVALIDATE(ext_buf->data, ext_buf->valid_data_size); 
#endif
#endif
        memcpy(d->dec_control.stream_map, ext_buf->data, ext_buf->valid_data_size);
        break;
    }
    
    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_config_param(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    pUWORD32    pui_value    = (pUWORD32) pv_value;
    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case XA_CODEC_CONFIG_PARAM_PCM_WIDTH:
    case XA_OPUS_DEC_CONFIG_PARAM_PCM_WIDTH:
        /* ...return sample bit-width */
        *pui_value = d->pcm_width;
        break;

    case XA_CODEC_CONFIG_PARAM_SAMPLE_RATE:
    case XA_OPUS_DEC_CONFIG_PARAM_SAMPLE_RATE:
        /* ...update sample rate of the decoded stream */
        d->sample_rate = d->dec_control.API_sampleRate;
        /* ...return output sampling frequency */
        *pui_value = d->sample_rate;
        break;

    case XA_CODEC_CONFIG_PARAM_CHANNELS:
    case XA_OPUS_DEC_CONFIG_PARAM_CHANNELS:
        /* ...update number of channels in the decoded stream */
        d->channels = d->dec_control.API_numChannels;
        /* ...return number of output channels */
        *pui_value = d->channels;
        break;

    case XA_OPUS_DEC_CONFIG_PARAM_STREAM_MAP:
    {
        xaf_ext_buffer_t *ext_buf = (xaf_ext_buffer_t *) pv_value;
#if defined(XA_ZERO_COPY) && (XF_CFG_CORES_NUM > 1)
#if XF_LOCAL_IPC_NON_COHERENT
        XF_IPC_INVALIDATE(ext_buf, sizeof(xaf_ext_buffer_t)); 
#endif
#endif
        memcpy(ext_buf->data, d->dec_control.stream_map, sizeof(d->dec_control.stream_map));
        ext_buf->valid_data_size = sizeof(d->dec_control.stream_map);
#if defined(XA_ZERO_COPY) && (XF_CFG_CORES_NUM > 1)
#if XF_LOCAL_IPC_NON_COHERENT
        XF_IPC_FLUSH(ext_buf->data, ext_buf->valid_data_size); 
        XF_IPC_FLUSH(ext_buf, sizeof(xaf_ext_buffer_t)); 
#endif
#endif
        break;
    }

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }

    return XA_NO_ERROR;
}

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

static XA_ERRORCODE parse_and_verify_ogg_header(XA_OPUS_Decoder *d)
{
    XA_ERRORCODE err_code = XA_NO_ERROR;
    int is_fatal = 0;
    WORD32 packet_count = 0;
    WORD32 read_ogg_inp = 1;
    pWORD8 p_ogg_inp_buf = d->input;
    int nBytes = 0;
    int input_size;
    int input_data_not_consumed = d->input_avail;
    int input_over = 0;
    while(1)
    {
        nBytes = XA_OPUS_OGG_OUTPUT_BUFFER_SIZE;
        input_size = 0;
        if(read_ogg_inp)
        {
            //counter = fread(p_ogg_inp_buf, sizeof(WORD8), OGG_INP_SIZE, bitInFile);
            read_ogg_inp = 0;
            if(input_data_not_consumed < XA_OPUS_OGG_INPUT_BUFFER_SIZE)
            {
                input_over = 1;
                input_size = input_data_not_consumed;
            }
            else
            {
                input_size = XA_OPUS_OGG_INPUT_BUFFER_SIZE;
            }
            input_data_not_consumed -= input_size;
        }
        err_code = xa_ogg_parse_process(d->ogg_handle,
                                        (pUWORD8)p_ogg_inp_buf,
                                        (pUWORD8)d->ogg_out_buf,
                                        (pWORD32)&input_size,
                                        &nBytes);
        //_XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);
        is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
        XF_CHK_ERR(!(is_fatal), err_code);

        d->consumed += input_size;

        if(err_code != XA_NO_ERROR)
        {
            if(err_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                read_ogg_inp = 1;
            else
            {
                p_ogg_inp_buf += input_size;
            }
        }

        if(nBytes > 0)
        {
            if(packet_count == 0)
            {
                OpusHeader opus_hdr;
                /* parse header */
                if (!opus_header_parse((pUWORD8)d->ogg_out_buf, nBytes, &opus_hdr))
                {
                    //_XA_HANDLE_ERROR(p_proc_testbench_err_info, "", XA_TESTBENCH_FATAL_OPUS_HDR_READ_FAILED);
                    is_fatal = (XA_ERROR_SEVERITY(err_code) != 0);
                    XF_CHK_ERR(!(is_fatal), err_code);
                }
                /* If sample rate is not passed on command line, pick input_sample_rate from ogg stream header */
                if((d->dec_control.API_sampleRate != opus_hdr.input_sample_rate) ||
                (d->dec_control.API_numChannels   != opus_hdr.channels) ||
                (d->dec_control.nb_streams        != opus_hdr.nb_streams) ||
                (d->dec_control.nb_coupled        != opus_hdr.nb_coupled) ||
                (d->dec_control.channel_mapping   != opus_hdr.channel_mapping))
                {
                    printf("INIT parameter mismatch with input stream.\nStream parameters:\n");
                    printf("API_sampleRate: %d\n", opus_hdr.input_sample_rate);
                    printf("API_numChannels: %d\n", opus_hdr.channels);
                    printf("nb_streams: %d\n", opus_hdr.nb_streams);
                    printf("nb_coupled: %d\n", opus_hdr.nb_coupled);
                    printf("channel_mapping: %d\n", opus_hdr.channel_mapping);
                    return XA_OPUS_EXECUTE_FATAL_PARAMS_CHANGED;
                }
                if(d->dec_control.channel_mapping != 0)
                {
                    if(memcmp(d->dec_control.stream_map, opus_hdr.stream_map, d->dec_control.API_numChannels))
                    {
                        int i;
                        printf("INIT parameter mismatch with input stream.\nstream_map:");
                        for(i = 0; i < d->dec_control.API_numChannels; i ++)
                        {
                            printf(" %d,", opus_hdr.stream_map[i]);
                        }
                        printf("\n");
                        return XA_OPUS_EXECUTE_FATAL_PARAMS_CHANGED;
                    }
                }
                d->dec_control.gain               = opus_hdr.gain;
                d->dec_control.version            = opus_hdr.version;
                d->gran_offset                    = opus_hdr.preskip;
                d->preskip                        = opus_hdr.preskip;
            }
            else if(packet_count == 1)
            {
                fprintf(stdout, "\nOgg Stream Comments: \n");
                /* Print comments */
                print_comments((char *)d->ogg_out_buf, nBytes);
                packet_count++;
                break;
            }
            packet_count++;
        }
        if(err_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
        {
            if(input_over)
            {
                return err_code;
            }
            else
            {
                break;
            }
        }
    }
    if((packet_count > 1) && (err_code == XA_NO_ERROR))
    {
        return err_code;
    }
    return XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA;
}

static XA_ERRORCODE xa_opus_decoder_execute(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
    {
        XA_ERRORCODE error_code;
        int nBytes = 0;
        int is_fatal = 0;
        pUWORD8 decoderInput = NULL;
    	//WORD16 out_samples = 0;

    	d->produced = 0;
    	d->consumed = 0;
        if(d->opus_stream_type == XA_RAW_OPUS_STREAM)
        {
            nBytes = *(WORD32 *)d->input;
    #ifndef _SYSTEM_IS_BIG_ENDIAN
            swap_endian( (WORD16*) &nBytes, 2 );
            nBytes = word16_swap(nBytes);
    #endif
            if ((nBytes < 0) || (nBytes > (XA_OPUS_MAX_BYTES_ENCODED_PACKET - 4)))
            {
                printf( "Error: Invalid Payload size: 0x%x\n", nBytes);
                return XA_OPUS_EXECUTE_FATAL_DEC_PAYLOAD_ERROR;
            }
            nBytes += 4;
            if(((WORD32)d->input_avail - 4 <= 0) || (((WORD32)d->input_avail - 4)< nBytes))
            {
                if (d->state & XA_OPUS_DEC_FLAG_EOS_RECEIVED) /* Signal done */
                {
                    d->state |= XA_OPUS_DEC_FLAG_COMPLETE;
                    d->state &= ~XA_OPUS_DEC_FLAG_EOS_RECEIVED; /* TENA-2544 */
                    return XA_NO_ERROR;
                }
                printf( "Error: Need more data\n");
                return XA_OPUS_EXECUTE_NONFATAL_INSUFFICIENT_DATA;
            }
            decoderInput = (pUWORD8)d->input + 4;
            d->consumed += 4;
    	    d->consumed += nBytes;
        }
        else if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
        {
            if(!d->first_frame_parsing_done)
            {
                error_code = parse_and_verify_ogg_header(d);
                is_fatal = (XA_ERROR_SEVERITY(error_code) != 0);
                XF_CHK_ERR(!(is_fatal), error_code);

                if(error_code == XA_NO_ERROR)
                {
                    d->first_frame_parsing_done = 1;
                    return XA_NO_ERROR;
                }
                if(error_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                {
                    if (!(d->state & XA_OPUS_DEC_FLAG_EOS_RECEIVED))
                    {
                        d->produced = 0;
                        return XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA;
                    }
                    else
                    {
                        d->state |= XA_OPUS_DEC_FLAG_COMPLETE;
                        d->state &= ~XA_OPUS_DEC_FLAG_EOS_RECEIVED; /* TENA-2544 */
                        return XA_NO_ERROR;
                    }
                }
            }
            else
            {
                //WORD32 packet_count = 0;
                WORD32 read_ogg_inp = 0;
                pWORD8 p_ogg_inp_buf = d->input;
                int input_size = 0;
                int input_data_not_consumed = d->input_avail;
                int input_over = 0;
                //WORD32 stream_map_set = 0;
                int is_fatal = 0;
                while(1)
                {
                    nBytes = XA_OPUS_OGG_OUTPUT_BUFFER_SIZE;
                    input_size = 0;
                    if((read_ogg_inp)&&(!input_over))
                    {
                        //counter = fread(p_ogg_inp_buf, sizeof(WORD8), OGG_INP_SIZE, bitInFile);
                        read_ogg_inp = 0;
                        if(input_data_not_consumed < XA_OPUS_OGG_INPUT_BUFFER_SIZE)
                        {
                            input_over = 1;
                            input_size = input_data_not_consumed;
                            d->produced = 0;
                        }
                        else
                        {
                            input_size = XA_OPUS_OGG_INPUT_BUFFER_SIZE;
                        }
                        input_data_not_consumed -= input_size;
                    }
                    error_code = xa_ogg_parse_process(d->ogg_handle,
                                                    (pUWORD8)p_ogg_inp_buf,
                                                    (pUWORD8)d->ogg_out_buf,
                                                    (pWORD32)&input_size,
                                                    &nBytes);
                    //_XA_HANDLE_ERROR(p_proc_ogg_err_info, "", error_code);
                    is_fatal = (XA_ERROR_SEVERITY(error_code) != 0);
                    XF_CHK_ERR(!(is_fatal), error_code);

                    if(nBytes > 0)
                    {
                        decoderInput = (pUWORD8)d->ogg_out_buf;
                        d->consumed += input_size;
                        break;
                    }

                    if(error_code != XA_NO_ERROR)
                    {
                        p_ogg_inp_buf += input_size;
                        d->consumed += input_size;
                        read_ogg_inp = 1;
                    }

                    if(error_code == XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA)
                    {
                        if(input_over)
                        {
                            if (!(d->state & XA_OPUS_DEC_FLAG_EOS_RECEIVED))
                            {
                                d->produced = 0;
                                return XA_OGG_EXECUTE_NONFATAL_INSUFFICIENT_DATA;
                            }
                            else
                            {
                                d->state |= XA_OPUS_DEC_FLAG_COMPLETE;
                                d->state &= ~XA_OPUS_DEC_FLAG_EOS_RECEIVED; /* TENA-2544 */
                                return XA_NO_ERROR;
                            }
                        }
                    }
                }
            }
        }
        if(d->dec_control.lostFlag == 0)
        {
            pWORD16 output_ptr = (pWORD16)d->output;
            int frames = 0;
            WORD16 output_length = 0;
            d->produced = 0;
            do
            {
                error_code = xa_opus_dec( (xa_codec_handle_t)(d->internal_data), 
                                            ((pUWORD8)decoderInput), 
                                            output_ptr, nBytes, &d->dec_control, (pWORD16)&output_length, d->scratch );
                is_fatal = (XA_ERROR_SEVERITY(error_code) != 0);
                XF_CHK_ERR(!(is_fatal), error_code);
                output_ptr += output_length;
                d->produced += (sizeof(WORD16)*output_length);
                frames++;
                if( frames > XA_OPUS_MAX_FRAMES_PER_PACKET )
                {
                    /* Hack for corrupt stream that could generate too many frames */
                    output_ptr  = (pWORD16)d->output;
                    d->produced = 0;
                    frames  = 0;
                    break;
                }                
            } while (d->dec_control.moreInternalDecoderFrames);
        }
        else
        {
            int i;
            pWORD16 output_ptr = (pWORD16)d->output;
            WORD16 output_length = 0;
            d->produced = 0;
            for( i = 0; i < d->dec_control.framesPerPacket; i++ )
            {
                error_code = xa_opus_dec( (xa_codec_handle_t)(d->internal_data), 
                                            ((pUWORD8)decoderInput), 
                                            output_ptr, nBytes, &d->dec_control, (pWORD16)&output_length, d->scratch );
                is_fatal = (XA_ERROR_SEVERITY(error_code) != 0);
                XF_CHK_ERR(!(is_fatal), error_code);
                output_ptr += output_length;
                d->produced += (sizeof(WORD16)*output_length);
            }
            d->dec_control.lostFlag = 0;
        }

        d->state |= XA_OPUS_DEC_FLAG_OUTPUT;
        
        return error_code;
    }
    case XA_CMD_TYPE_DONE_QUERY:
        *(WORD32 *)pv_value = (d->state & XA_OPUS_DEC_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        /* ...reset opus_dec component operation */
        return xa_opus_dec_init((xa_codec_handle_t)(d->internal_data), &d->dec_control);

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

static XA_ERRORCODE xa_opus_decoder_set_input_bytes(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_OPUS_DEC_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;

    /* ... reset exec-done state of the plugin to enable processing input. TENA-2544 */
    d->state &= ~XA_OPUS_DEC_FLAG_COMPLETE;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_output_bytes(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = d->produced;
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_curidx_input_buf(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return number of bytes consumed */
    *(WORD32 *)pv_value = d->consumed;
    d->consumed = 0;
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_input_over(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_OPUS_DEC_FLAG_EOS_RECEIVED;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_memtabs_size(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check AMR dec component is pre-initialized */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have all our tables inside API handle - good? tbd */
    *(WORD32 *)pv_value = 0;
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_set_memtabs_ptr(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...check AMR dec component is pre-initialized */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...do not do anything; just return success - tbd */
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_n_memtabs(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer, 1 output buffer, 1 persistant buffer and 1 scratch buffer*/
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_mem_info_size(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
#if 0
    	*(WORD32 *)pv_value = XA_OPUS_MAX_BYTES_CHANNEL_PACKET*(d->channels);
#else
        /* ...input buffer size according to the frame_size  */
    	*(WORD32 *)pv_value = XA_OPUS_MAX_DEC_INP_BYTES * d->dec_control.nb_streams;
#endif
    	return XA_NO_ERROR;

    case 1:
        /* ...output buffer */
        *(WORD32 *)pv_value = XA_OPUS_MAX_BYTES_CHANNEL_PACKET * d->dec_control.API_numChannels;
        return XA_NO_ERROR;

    case 2:
        /* ...scratch buffer */
        *(WORD32 *)pv_value = xa_opus_dec_get_scratch_byte_size(d->dec_control.channel_mapping);
        if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
        {
            *(WORD32 *)pv_value += xa_ogg_parse_get_scratch_byte_size(&d->ogg_cfg);
            *(WORD32 *)pv_value += XA_OPUS_OGG_OUTPUT_BUFFER_SIZE;
        }
        return XA_NO_ERROR;

    case 3:
        /* ...persistant buffer */
        *(WORD32 *)pv_value = xa_opus_dec_get_handle_byte_size(d->dec_control.nb_streams, d->dec_control.nb_coupled);
        if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
        {
            *(WORD32 *)pv_value += xa_ogg_parse_get_handle_byte_size(&d->ogg_cfg);
        }
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_mem_info_alignment(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 8-bytes aligned */
    *(WORD32 *)pv_value = 8;

    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_opus_decoder_get_mem_info_type(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
        *(WORD32 *)pv_value = XA_MEMTYPE_INPUT;
        return XA_NO_ERROR;

    case 1:
        /* ...output buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_OUTPUT;
        return XA_NO_ERROR;

    case 2:
        /* ...scratch buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_SCRATCH;
        return XA_NO_ERROR;
    
    case 3:
        /* ...persistant buffer */
        *(WORD32 *)pv_value = XA_MEMTYPE_PERSIST;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

static XA_ERRORCODE xa_opus_decoder_set_mem_ptr(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_OPUS_DEC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...select memory buffer */
    switch (i_idx)
    {
    case 0:
        /* ...input buffers */
        d->input = pv_value;
        return XA_NO_ERROR;

    case 1:
        /* ...output buffer */
        d->output = pv_value;
        return XA_NO_ERROR;

    case 2:
    {
        /* ...scratch buffer */
        int opus_scratch_size = xa_opus_dec_get_scratch_byte_size(d->dec_control.channel_mapping);
        d->scratch = pv_value;
        if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
        {
            pv_value = (pWORD8)pv_value + opus_scratch_size;
            d->ogg_out_buf = (pWORD8)pv_value;
            pv_value = (pWORD8)pv_value + XA_OPUS_OGG_OUTPUT_BUFFER_SIZE;
            d->ogg_scratch = (pVOID)pv_value;
        }
        return XA_NO_ERROR;
    }

    case 3:
    {
        /* ...persistant buffer */
        int opus_handle_size = xa_opus_dec_get_handle_byte_size(d->dec_control.nb_streams, d->dec_control.nb_coupled);
        d->internal_data = (UWORD32)pv_value;
        if(d->opus_stream_type == XA_OGG_OPUS_STREAM)
        {
            pv_value = (pWORD8)pv_value + opus_handle_size;
            d->ogg_handle = (xa_codec_handle_t)pv_value;
        }
        return XA_NO_ERROR;
    }

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }

    return XA_NO_ERROR;
}

static XA_ERRORCODE (* const xa_opus_decoder_api[])(XA_OPUS_Decoder *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_opus_decoder_get_api_size,

    [XA_API_CMD_INIT]                   = xa_opus_decoder_init,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_opus_decoder_set_config_param,

    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_opus_decoder_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_opus_decoder_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_opus_decoder_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_opus_decoder_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_opus_decoder_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_opus_decoder_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_opus_decoder_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_opus_decoder_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_opus_decoder_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_opus_decoder_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_opus_decoder_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_opus_decoder_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_opus_decoder_set_mem_ptr,
};
#else //PACK_WS_DUMMY
typedef struct XA_OPUS_Decoder
{
    void *pdummy;
}XA_OPUS_Decoder;
static XA_ERRORCODE xa_opus_decoder_dummy(XA_OPUS_Decoder *d, WORD32 i_idx, pVOID pv_value){return 0;};

static XA_ERRORCODE (* const xa_opus_decoder_api[])(XA_OPUS_Decoder *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE] = xa_opus_decoder_dummy,
};
#endif //PACK_WS_DUMMY

/* ...total numer of commands supported */
#define XA_OPUS_DEC_API_COMMANDS_NUM   (sizeof(xa_opus_decoder_api) / sizeof(xa_opus_decoder_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_opus_decoder(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif
    XA_OPUS_Decoder    *d = (XA_OPUS_Decoder *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_OPUS_DEC_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_opus_decoder_api[i_cmd], XA_API_FATAL_INVALID_CMD);

#ifdef XAF_PROFILE
    comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
    /* ...execute requested command */
    ret = xa_opus_decoder_api[i_cmd](d, i_idx, pv_value);

#ifdef XAF_PROFILE
    comp_stop = clk_read_stop(CLK_SELN_THREAD);
    dec_cycles += clk_diff(comp_stop, comp_start);
#endif
    
    return ret;
}
#endif
