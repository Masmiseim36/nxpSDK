/*******************************************************************************
* Copyright (c) 2015-2019 Cadence Design Systems, Inc.
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to use this Software with Cadence processor cores only and
* not with any other processors and platforms, subject to
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

******************************************************************************/

#define MODULE_TAG                      VIT_PRE_PROC

/*******************************************************************************
 * Includes
 ******************************************************************************/
#if (XA_VIT_PRE_PROC)

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/* ...debugging facility */
#include "xf-debug.h"
#include "vit_pre_proc.h"
#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t vit_pre_proc_cycles;
#endif

#include "srtm_utils.h"
#include "srtm_config.h"
#include "rpmsg_lite.h"
#include "dsp_config.h"
#include "fsl_gpio.h"

/* VIT Library, configuration */
#include "PL_platformTypes_HIFI4_FUSIONF1.h"
#include "VIT.h"

#if XA_VOICE_SEEKER
#include "libVoiceSeekerLight.h"
#endif

//VIT Lib
#define NUMBER_OF_CHANNELS          1
#define VIT_OPERATING_MODE          VIT_WAKEWORD_ENABLE | VIT_VOICECMD_ENABLE

// Configure the detection period in second for each command
// VIT will return UNKNOWN if no command is recognized during this time span.
#define VIT_COMMAND_TIME_SPAN           3.0 // in second

#undef DSP_PRINTF
#define DSP_PRINTF CM33_Print

/*******************************************************************************
 * Tracing configuration
 ******************************************************************************/

//TRACE_TAG(INIT, 1);
//TRACE_TAG(PROCESS, 1);

/*******************************************************************************
 * Internal functions definitions
 ******************************************************************************/
/* ...API structure */
typedef struct vit_pre_proc_t
{
    /* ...pcm gain module state */
    UWORD32                 state;

    /* ...number of channels (channel mask?) */
    UWORD32                 channels;

    /* ...PCM sample width */
    UWORD32                 pcm_width;

    /* ...sampling rate */
    UWORD32                 sample_rate;

    /* ...number of bytes in input/output buffer */
    UWORD32                 buffer_size;

    /* ...input buffer */
    void               *input;

    /* ...output buffer */
    void               *output;

    /* ...scratch buffer pointer */
    void               *scratch;

    /* ...number of available bytes in the input buffer */
    UWORD32                 input_avail;

    /* ...number of bytes consumed from input buffer */
    UWORD32                 consumed;

    /* ...number of produced bytes */
    UWORD32                 produced;

    UWORD32 frame_size_us;

}   vit_pre_proc_t;


/*******************************************************************************
 * VIT state flags
 ******************************************************************************/

#define XA_VIT_PRE_PROC_FLAG_PREINIT_DONE      (1 << 0)
#define XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_VIT_PRE_PROC_FLAG_RUNNING           (1 << 2)
#define XA_VIT_PRE_PROC_FLAG_OUTPUT            (1 << 3)
#define XA_VIT_PRE_PROC_FLAG_COMPLETE          (1 << 4)

extern dsp_handle_t dsp;

/*
*   Useful Definitions : not to be changed
*/
// MEMORY
#define MEMORY_ALIGNMENT             8     // in bytes

static VIT_Handle_t              VITHandle = PL_NULL;                      // VIT handle pointer
static VIT_InstanceParams_st     VITInstParams;                            // VIT instance parameters structure
static VIT_ControlParams_st      VITControlParams;                         // VIT control parameters structure
static PL_MemoryTable_st         VITMemoryTable;                           // VIT memory table descriptor
static PL_BOOL                   InitPhase_Error = PL_FALSE;
static PL_INT8                   *pMemory[PL_NR_MEMORY_REGIONS];

static PL_UINT8					 *pMem;		 //Used for storing start address of whole allocation of space for model
static PL_UINT8 				 *VIT_Model; //Used to hold aligned start address of model

VIT_ReturnStatus_en VIT_Initialize(vit_pre_proc_t *d);
void VIT_Deinit(void);


#if XA_VOICE_SEEKER
extern RETUNE_VOICESEEKERLIGHT_plugin_t vsl;
#endif

/*******************************************************************************
 * DSP functions
 ******************************************************************************/

static void CM33_Print(const char* ptr, ...)
{
	//Create buffer with processed string
	static char buf[256];
	int message_length;
	va_list args;
	va_start(args, ptr);
	message_length = vsprintf(buf, ptr, args);
	va_end(args);

	//Create srtm_message for sending string to CM33 side
	srtm_message msg = {0};
	msg.head.type = SRTM_MessageTypeNotification;
	msg.head.majorVersion = SRTM_VERSION_MAJOR;
	msg.head.minorVersion = SRTM_VERSION_MINOR;
	msg.head.category = SRTM_MessageCategory_AUDIO;
	msg.head.command  = SRTM_Print_String;

	//Send string to CM33
	int pos = 0;
	for(int i = 0; i<message_length; i++)
	{
		msg.param[pos++] = buf[i];
		if( ((pos+1)==SRTM_CMD_PARAMS_MAX) || ((i+1)==message_length) )
		{
			msg.param[pos] = '\0';
			pos = 0;
			xos_mutex_lock(&dsp.rpmsgMutex);
			rpmsg_lite_send(dsp.rpmsg, dsp.ept, MCU_EPT_ADDR, (char *)&msg, sizeof(srtm_message), 100);
			xos_mutex_unlock(&dsp.rpmsgMutex);
		}
	}
}

static inline void xa_vit_pre_proc_preinit(vit_pre_proc_t *d)
{
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));

    /* ...set default parameters */
    d->channels = NUMBER_OF_CHANNELS;
    d->pcm_width = 16;
    d->sample_rate = 16000;
    d->frame_size_us = 30000;
}

static XA_ERRORCODE xa_vit_pre_proc_do_execute_16bit(vit_pre_proc_t *d)
{
    WORD32    k, nSize;
    PL_INT16  *pIn = (PL_INT16 *)d->input;
    WORD16    *pOut = (WORD16 *) d->output;
    UWORD32   filled = d->input_avail;
    VIT_ReturnStatus_en VIT_Status;
    VIT_VoiceCommand_st VoiceCommand;                   	         // Voice Command info
    VIT_WakeWord_st WakeWord;                    		             // Wakeword info
    VIT_DetectionStatus_en VIT_DetectionResults = VIT_NO_DETECTION;  // VIT detection result

    nSize = filled >> 1;    //size of each sample is 2 bytes
    k = (WORD32)(d->buffer_size - filled);

    /* ...check I/O buffer */
    XF_CHK_ERR(d->input, XA_VIT_PRE_PROC_EXEC_FATAL_INPUT);
    XF_CHK_ERR(d->output, XA_VIT_PRE_PROC_EXEC_FATAL_INPUT);

    (k > 0 ? memset((void *)pIn + filled, 0x00, k) : 0);

    /* Copy input data to output buffer*/
    memcpy(pOut, pIn, filled);

    /*
    *   VIT Process
    */
    VIT_Status = VIT_Process ( VITHandle,
                           (void*)pIn,                               // temporal audio input data
                           &VIT_DetectionResults
                          );

    if (VIT_Status != VIT_SUCCESS)
    {
        // DSP_PRINTF("VIT_Process error : %d\n", Status);
        return VIT_SYSTEM_ERROR;                                            // will stop processing VIT and go directly to MEM free
    }

    if (VIT_DetectionResults == VIT_WW_DETECTED)
    {
    	// Retrieve id of the WakeWord detected
		// String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model)
    	VIT_Status = VIT_GetWakeWordFound( VITHandle, &WakeWord);
		if (VIT_Status != VIT_SUCCESS)
		{
			DSP_PRINTF("VIT_GetWakeWordFound error : %d\r\n", VIT_Status);
			return VIT_SYSTEM_ERROR;
		}
		else
		{
			DSP_PRINTF(" - WakeWord detected %d", WakeWord.Id);

			// Retrieve WakeWord Name : OPTIONAL
			// Check first if WakeWord string is present
			if (WakeWord.pName != PL_NULL)
			{
				DSP_PRINTF(" %s\r\n", WakeWord.pName);
			}
#if XA_VOICE_SEEKER
			VoiceSeekerLight_TriggerFound(&vsl, WakeWord.StartOffset);
#endif
		}
    }
    else if (VIT_DetectionResults == VIT_VC_DETECTED)
    {
        // Retrieve id of the Voice Command detected
        // String of the Command can also be retrieved (when WW and CMDs strings are integrated in Model)
    	VIT_Status = VIT_GetVoiceCommandFound(VITHandle,
                                              &VoiceCommand
                                              );
        if (VIT_Status != VIT_SUCCESS)
        {
            DSP_PRINTF("VIT_GetVoiceCommandFound error : %d\r\n", VIT_Status);
            return VIT_SYSTEM_ERROR;                                              // will stop processing VIT and go directly to MEM free
        }
        else
        {
            DSP_PRINTF(" - Voice Command detected %d", VoiceCommand.Id);

            // Retrieve CMD Name : OPTIONAL
            // Check first if CMD string is present
            if (VoiceCommand.pName != PL_NULL)
            {
                DSP_PRINTF(" %s\r\n", VoiceCommand.pName);
            }
            else
            {
            	DSP_PRINTF("\r\n");
            }
        }
    }

    /* ...save total number of consumed bytes */
    d->consumed = (UWORD32)filled;

    /* ...save total number of produced bytes */
    d->produced = (UWORD32)filled;

    /* ...put flag saying we have output buffer */
    d->state |= XA_VIT_PRE_PROC_FLAG_OUTPUT;

    TRACE(PROCESS, _b("produced: %u bytes (%u samples)"), d->produced, nSize);

    /* ...return success result code */
    return XA_NO_ERROR;
}

/* ...runtime reset */
static XA_ERRORCODE xa_vit_pre_proc_do_runtime_init(vit_pre_proc_t *d)
{
    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...codec API size query */
static XA_ERRORCODE xa_vit_pre_proc_get_api_size(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_vit_pre_proc_init(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_vit_pre_proc_preinit(d);

        d->state = XA_VIT_PRE_PROC_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        if (VIT_Initialize(d) != VIT_SUCCESS)
        {
            return XA_FATAL_ERROR;
        }

        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        //compute buffer size
        UWORD64 product = (UWORD64)d->sample_rate * NUMBER_OF_CHANNELS * (d->pcm_width>>3) * d->frame_size_us;
        d->buffer_size = (UWORD32)(product/1000000);

        /* Align the computed buffer size to 8-byte */
        d->buffer_size = (d->buffer_size + 7) & ~0x7;

        /* ...mark post-initialization is complete */
        d->state |= XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_PROCESS:
    {
        XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...enter into execution stage */
        d->state |= XA_VIT_PRE_PROC_FLAG_RUNNING;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_VIT_PRE_PROC_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }

    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_vit_pre_proc_deinit()
{
    VIT_Deinit();
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_vit_pre_proc_set_config_param(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_VIT_PRE_PROC_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR((i_value == 16) || (i_value == 32), XA_VIT_PRE_PROC_CONFIG_NONFATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_VIT_PRE_PROC_CONFIG_PARAM_CHANNELS:
        /* ...allow mono/stereo only */
        XF_CHK_ERR((i_value <= VIT_PRE_PROC_MAX_CHANNELS), XA_VIT_PRE_PROC_CONFIG_NONFATAL_RANGE);
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_VIT_PRE_PROC_CONFIG_PARAM_SAMPLE_RATE:
        {
            switch ((UWORD32)i_value)
            {
                case 4000:
                case 8000:
                case 11025:
                case 12000:
                case 16000:
                case 22050:
                case 24000:
                case 32000:
                case 44100:
                case 48000:
                case 64000:
                case 88200:
                case 96000:
                case 128000:
                case 176400:
                case 192000:
                    d->sample_rate = (UWORD32)i_value;
                    break;
                default:
                    XF_CHK_ERR(0, XA_VIT_PRE_PROC_CONFIG_NONFATAL_RANGE);
            }
            return XA_NO_ERROR;
        }

    case XA_VIT_PRE_PROC_CONFIG_PARAM_INPUT_FRAME_SIZE_US:
    case XA_VIT_PRE_PROC_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US:
        d->frame_size_us = (UWORD32)i_value;
        return XA_NO_ERROR;
    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_vit_pre_proc_get_config_param(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_VIT_PRE_PROC_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;

    case XA_VIT_PRE_PROC_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_VIT_PRE_PROC_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_VIT_PRE_PROC_CONFIG_PARAM_PRODUCED:
        /* ...return no.of produced bytes */
        *(WORD32 *)pv_value = d->produced;
        return XA_NO_ERROR;

    case XA_VIT_PRE_PROC_CONFIG_PARAM_INPUT_FRAME_SIZE_US:
    case XA_VIT_PRE_PROC_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US:
        *(WORD32 *)pv_value = d->frame_size_us;
        return XA_NO_ERROR;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_vit_pre_proc_execute(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif

    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        /* ...perform mixing of the channels */
#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = xa_vit_pre_proc_do_execute_16bit(d);
#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        vit_pre_proc_cycles += clk_diff(comp_stop, comp_start);
#endif
        return ret;

    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_VIT_PRE_PROC_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        return xa_vit_pre_proc_do_runtime_init(d);

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_vit_pre_proc_set_input_bytes(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_VIT_PRE_PROC_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;

    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_vit_pre_proc_get_output_bytes(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be zero */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_VIT_PRE_PROC_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = ((d->state & XA_VIT_PRE_PROC_FLAG_OUTPUT) ? d->produced : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_vit_pre_proc_get_curidx_input_buf(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_RUNNING, XA_VIT_PRE_PROC_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_VIT_PRE_PROC_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed;

    /* ...and reset internally managed number of consumed bytes */
    d->consumed = 0;

    return XA_NO_ERROR;
}

/* ...end-of-stream processing */
static XA_ERRORCODE xa_vit_pre_proc_input_over(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_VIT_PRE_PROC_FLAG_COMPLETE;

    TRACE(PROCESS, _b("Input-over-condition signalled"));

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/
/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_vit_pre_proc_get_memtabs_size(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_vit_pre_proc_set_memtabs_ptr(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_vit_pre_proc_get_n_memtabs(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer, 1 output buffer and 1 scratch buffer*/
    *(WORD32 *)pv_value = 3;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_vit_pre_proc_get_mem_info_size(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are of the same length */
    *(WORD32 *)pv_value = (WORD32) d->buffer_size;

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_vit_pre_proc_get_mem_info_alignment(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_vit_pre_proc_get_mem_info_type(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    switch (i_idx)
    {
    case 0:
        /* ...input buffer */
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

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}

/* ...set memory pointer */
static XA_ERRORCODE xa_vit_pre_proc_set_mem_ptr(vit_pre_proc_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_VIT_PRE_PROC_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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
        /* ...scratch buffer */
        d->scratch = pv_value;
        return XA_NO_ERROR;

    default:
        /* ...invalid index */
        return XF_CHK_ERR(0, XA_API_FATAL_INVALID_CMD_TYPE);
    }
}


/*******************************************************************************
 * API command hooks
 ******************************************************************************/

static XA_ERRORCODE (* const xa_vit_pre_proc_api[])(vit_pre_proc_t *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_vit_pre_proc_get_api_size,

    [XA_API_CMD_INIT]                   = xa_vit_pre_proc_init,
    [XA_API_CMD_DEINIT]                 = xa_vit_pre_proc_deinit,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_vit_pre_proc_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_vit_pre_proc_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_vit_pre_proc_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_vit_pre_proc_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_vit_pre_proc_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_vit_pre_proc_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_vit_pre_proc_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_vit_pre_proc_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_vit_pre_proc_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_vit_pre_proc_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_vit_pre_proc_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_vit_pre_proc_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_vit_pre_proc_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_vit_pre_proc_set_mem_ptr,
};

/* ...total number of commands supported */
#define XA_VIT_PRE_PROC_API_COMMANDS_NUM   (sizeof(xa_vit_pre_proc_api) / sizeof(xa_vit_pre_proc_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_vit_pre_proc(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    vit_pre_proc_t *d = (vit_pre_proc_t *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_VIT_PRE_PROC_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_vit_pre_proc_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_vit_pre_proc_api[i_cmd](d, i_idx, pv_value);
}

VIT_ReturnStatus_en VIT_ModelInfo(void)
{
    VIT_ReturnStatus_en     VIT_Status;                             /* Function call status */
    /*
    *   VIT Get Model Info (OPTIONAL)
    *       To retrieve information on the VIT_Model registered in VIT:
    *               - Model Release Number, number of commands supported
    *               - WakeWords supported (when info is present)
    *               - list of commands (when info is present)
    *
    */
    VIT_ModelInfo_st Model_Info;
    VIT_Status = VIT_GetModelInfo(&Model_Info);
    if (VIT_Status != VIT_SUCCESS)
    {
        DSP_PRINTF("VIT_GetModelInfo error : %d\r\n", VIT_Status);
        return VIT_INVALID_MODEL;
    }

    DSP_PRINTF("VIT Model info \r\n");
    DSP_PRINTF("  VIT Model Release = 0x%04x\r\n", Model_Info.VIT_Model_Release);
    if (Model_Info.pLanguage != PL_NULL)
    {
        DSP_PRINTF("  Language supported : %s \r\n", Model_Info.pLanguage);
    }

    DSP_PRINTF("  Number of WakeWords supported : %d \r\n", Model_Info.NbOfWakeWords);
    DSP_PRINTF("  Number of Commands supported : %d \r\n",  Model_Info.NbOfVoiceCmds);

    if (!Model_Info.WW_VoiceCmds_Strings)               // Check here if Model is containing WW and CMDs strings
    {
        DSP_PRINTF("  VIT_Model integrating WakeWord and Voice Commands strings : NO\r\n");
    }
    else
    {
        const char* ptr;

        DSP_PRINTF("  VIT_Model integrating WakeWord and Voice Commands strings : YES\r\n");
        DSP_PRINTF("  WakeWords supported : \r\n");
        ptr = Model_Info.pWakeWord_List;
        if (ptr != PL_NULL)
        {
        	for (PL_UINT16 i = 0; i < Model_Info.NbOfWakeWords; i++)
			{
        		DSP_PRINTF("   '%s' \r\n", ptr);
				ptr += strlen(ptr) + 1;                 // to consider NULL char
			}
        }
        DSP_PRINTF("  Voice commands supported : \r\n");
        ptr = Model_Info.pVoiceCmds_List;
        if (ptr != PL_NULL)
        {
            for (PL_UINT16 i = 0; i < Model_Info.NbOfVoiceCmds; i++)
            {
                DSP_PRINTF("   '%s' \r\n", ptr);
                ptr += strlen(ptr) + 1;                 // to consider NULL char
            }
        }
    }
    return VIT_SUCCESS;
}

VIT_ReturnStatus_en VIT_Initialize(vit_pre_proc_t *d)
{
    VIT_ReturnStatus_en     VIT_Status;                             /* Function call status */

    pMem = malloc(dsp.size_of_VIT_model + VIT_MODEL_ALIGNMENT);
    VIT_Model = (PL_UINT8 *)INSTALLOC_ALIGN(pMem);
    memcpy(VIT_Model, dsp.VITModelCM_33, dsp.size_of_VIT_model);

    VIT_Status = VIT_SetModel(VIT_Model, VIT_MODEL_IN_RAM);

    if (VIT_Status != VIT_SUCCESS)
    {
        return VIT_INVALID_MODEL;
    }

    VIT_Status = VIT_ModelInfo();
    if (VIT_Status != VIT_SUCCESS)
    {
        return VIT_INVALID_MODEL;
    }
    /*
     *   Configure VIT Instance Parameters
     */
    VITInstParams.SampleRate_Hz            = VIT_SAMPLE_RATE;
    VITInstParams.SamplesPerFrame          = d->frame_size_us == 10000 ? VIT_SAMPLES_PER_10MS_FRAME : VIT_SAMPLES_PER_30MS_FRAME;
    VITInstParams.NumberOfChannel          = NUMBER_OF_CHANNELS;
    VITInstParams.DeviceId                 = DEVICE_ID;
    VITInstParams.APIVersion               = VIT_API_VERSION;

    /*
     *   VIT get memory table : Get size info per memory type
     */
    VIT_Status = VIT_GetMemoryTable( PL_NULL,                // VITHandle param should be NULL
                                 &VITMemoryTable,
                                 &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        // DSP_PRINTF("VIT_GetMemoryTable error : %d\n", Status);
        return VIT_INVALID_BUFFER_MEMORY_ALIGNMENT;
    }

    /*
     *   Reserve memory space : Malloc for each memory type
     */
    for (int i = 0; i < PL_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        // DSP_PRINTF("Memory region %d, size %d in Bytes\n", (int)i, (int)VITMemoryTable.Region[i].Size);
        if (VITMemoryTable.Region[i].Size != 0)
        {
            // reserve memory space
            // NB : VITMemoryTable.Region[PL_MEMREGION_PERSISTENT_FAST_DATA] should be alloacted
            //      in the fastest memory of the platform (when possible) - this is not the case in this example.
            pMemory[i] = malloc(VITMemoryTable.Region[i].Size + MEMORY_ALIGNMENT);
            VITMemoryTable.Region[i].pBaseAddress = (void *)pMemory[i];

            // DSP_PRINTF(" Memory region address %p\n", VITMemoryTable.Region[i].pBaseAddress);
        }
    }

    /*
    *    Create VIT Instance
    */
    VITHandle = PL_NULL;                            // force to null address for correct memory initialization
    VIT_Status = VIT_GetInstanceHandle( &VITHandle,
                                        &VITMemoryTable,
                                        &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        InitPhase_Error = PL_TRUE;
        // DSP_PRINTF("VIT_GetInstanceHandle error : %d\n", Status);
    }

    /*
    *    Test the reset (OPTIONAL)
    */
    if (!InitPhase_Error)
    {
    	VIT_Status = VIT_ResetInstance(VITHandle);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            // DSP_PRINTF("VIT_ResetInstance error : %d\n", Status);
        }
    }

    /*
    *   Set and Apply VIT control parameters
    */
    VITControlParams.OperatingMode      = VIT_OPERATING_MODE;
    VITControlParams.Feature_LowRes     = PL_FALSE;
    VITControlParams.Command_Time_Span  = VIT_COMMAND_TIME_SPAN;

    if (!InitPhase_Error)
    {
        VIT_Status = VIT_SetControlParameters(  VITHandle,
                                            &VITControlParams);
        if (VIT_Status != VIT_SUCCESS)
        {
            InitPhase_Error = PL_TRUE;
            // DSP_PRINTF("VIT_SetControlParameters error : %d\n", Status);
        }
    }

    return VIT_Status;
}

void VIT_Deinit(void)
{
    VIT_ReturnStatus_en     VIT_Status;                             /* Function call status */
    // retrieve size of the different MEM tables allocated
    VIT_Status = VIT_GetMemoryTable(   VITHandle,                // Should provide VIT_Handle to retrieve the size of the different MemTabs
                                   &VITMemoryTable,
                                   &VITInstParams);
    if (VIT_Status != VIT_SUCCESS)
    {
        // DSP_PRINTF("VIT_GetMemoryTable error : %d\n", Status);
        return;
    }

    // Free the MEM tables
    for (int i = 0; i<PL_NR_MEMORY_REGIONS; i++)
    {
        if (VITMemoryTable.Region[i].Size != 0)
        {
            free((PL_INT8 *)pMemory[i]);
        }
    }

    //free memory
    free((PL_INT8 *)pMem);
}

#undef DSP_PRINTF
#define DSP_PRINTF printf

#endif
