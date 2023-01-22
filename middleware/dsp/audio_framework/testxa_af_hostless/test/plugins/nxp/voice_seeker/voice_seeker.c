/*******************************************************************************
* Copyright 2022 NXP
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

#define MODULE_TAG                      VOICE_SEEKER

/*******************************************************************************
 * Includes
 ******************************************************************************/
#if (XA_VOICE_SEEKER)

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

/* ...debugging facility */
#include "xf-debug.h"
#include "voice_seeker.h"
#ifdef XAF_PROFILE
#include "xaf-clk-test.h"
extern clk_t voice_seeker_cycles;
#endif

#include "srtm_utils.h"
#include "srtm_config.h"
#include "rpmsg_lite.h"
#include "dsp_config.h"
#include "fsl_gpio.h"

#include "libVoiceSeekerLight.h"
#include "RdspStatusCodes.h"
#include "RdspDeviceConfig.h"

#if (defined(CPU_MIMXRT685SFVKB_dsp))
#define DEVICE_ID_VS Device_IMXRT600_HIFI4
#elif (defined(CPU_MIMXRT595SFFOC_dsp))
#define DEVICE_ID_VS Device_IMXRT500_FusionF1
#endif

#define RDSP_VOICESEEKER_LIGHT_APP_VERSION_MAJOR 0 // Major version the app
#define RDSP_VOICESEEKER_LIGHT_APP_VERSION_MINOR 2 // Minor version the app
#define RDSP_VOICESEEKER_LIGHT_APP_VERSION_PATCH 8 // Patch version the app

#define RDSP_NUM_RX                1
#define RDSP_NUM_SPK               0
#define RDSP_ENABLE_AEC            0
#define RDSP_ENABLE_DOA            0
#define RDSP_ENABLE_VOICESPOT      0
#define RDSP_ENABLE_PAYLOAD_BUFFER 0
#define RDSP_ENABLE_AGC            0
#define RDSP_BUFFER_LENGTH_SEC     1.5f
#define RDSP_AEC_FILTER_LENGTH_MS  150

#define VOICESEEKER_SAMPLERATE    16000
#define VOICESEEKER_FRAMESIZE_OUT 480   // 30 ms
#define INV_32768   3.0517578125e-05f

#include "libVoiceSeekerLight.h"
#include "RdspStatusCodes.h"
#include "RdspCycleCounter.h"

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
RdspStatus voiceseeker_setup();
void AudioProcess(void);

/* ...API structure */
typedef struct voice_seeker_t
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

}   voice_seeker_t;


/*******************************************************************************
 * VOICE_SEEKER state flags
 ******************************************************************************/

#define XA_VOICE_SEEKER_FLAG_PREINIT_DONE      (1 << 0)
#define XA_VOICE_SEEKER_FLAG_POSTINIT_DONE     (1 << 1)
#define XA_VOICE_SEEKER_FLAG_RUNNING           (1 << 2)
#define XA_VOICE_SEEKER_FLAG_OUTPUT            (1 << 3)
#define XA_VOICE_SEEKER_FLAG_COMPLETE          (1 << 4)


extern dsp_handle_t dsp;

/*******************************************************************************
 * Variables
 ******************************************************************************/
RETUNE_VOICESEEKERLIGHT_plugin_t vsl             = {0};
static rdsp_voiceseekerlight_config_t vsl_config = {0};
static uint32_t framesize_in                     = 0;
static float** mic_in;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static RdspStatus VoiceSeeker_Initialize(voice_seeker_t *d);
static XA_ERRORCODE VoiceSeeker_Execute(voice_seeker_t *d);

/*******************************************************************************
 * DSP functions
 ******************************************************************************/
void DeInterleave(const int16_t *pDataInput, int16_t *pDataOutput, uint16_t FrameSize, uint16_t ChannelNumber);

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

static RdspStatus VoiceSeeker_Initialize(voice_seeker_t *d)
{
    int status = 0;
    uint16_t NumberOfChannel = d->channels;
    /*
     * Query VoiceSeeker library for the supported configuration
     */

    rdsp_voiceseekerlight_constants_t vsl_constants;
    VoiceSeekerLight_GetConstants(&vsl_constants);

    if (vsl_constants.max_num_mics < NumberOfChannel)
    {
        CM33_Print("Error VoiceSeeker is supporting only %d mics", vsl_constants.max_num_mics);
        return GENERAL_ERROR;
    }

    /*
     * VoiceSeekerLight plugin configuration
     */
    vsl_config.num_mics             = NumberOfChannel;
    vsl_config.num_spks             = RDSP_NUM_SPK;
    vsl_config.framesize_out        = VOICESEEKER_FRAMESIZE_OUT;
    vsl_config.create_aec           = RDSP_ENABLE_AEC;
    vsl_config.create_doa           = RDSP_ENABLE_DOA;
    vsl_config.buffer_length_sec    = RDSP_BUFFER_LENGTH_SEC;
    vsl_config.aec_filter_length_ms = RDSP_AEC_FILTER_LENGTH_MS;
    vsl_config.device_id		    = DEVICE_ID_VS;

    // Specify mic geometry
    vsl_config.mic_xyz_mm = (rdsp_xyz_t *)malloc(sizeof(rdsp_xyz_t) * vsl_config.num_mics);
    if (vsl_config.mic_xyz_mm == NULL)
    {
        return MALLOC_FAIL;
    }

#if (DMIC_BOARD_CONNECTED == 1)
    /* 8CH-DMIC board rev B (2MIC, 3MICA, 3MICC Config) */

    // MIC0
    if (vsl_config.num_mics > 0) {
        vsl_config.mic_xyz_mm[0][0] = 35.0f; // X
        vsl_config.mic_xyz_mm[0][1] = 0.0f;  // Y
        vsl_config.mic_xyz_mm[0][2] = 0.0f;  // Z
    }
    // MIC3
    if (vsl_config.num_mics > 1) {
        vsl_config.mic_xyz_mm[1][0] = -35.0f; // X
        vsl_config.mic_xyz_mm[1][1] = 0.0f;   // Y
        vsl_config.mic_xyz_mm[1][2] = 0.0f;   // Z
    }
    // MIC9
    if (vsl_config.num_mics > 2) {
        vsl_config.mic_xyz_mm[2][0] = 0.0f; // X
        vsl_config.mic_xyz_mm[2][1] = -35.0f; // Y
        vsl_config.mic_xyz_mm[2][2] = 0.0f; // Z
    }
    // MIC6
    if (vsl_config.num_mics > 3) {
        vsl_config.mic_xyz_mm[3][0] = 0.0f;   // X
        vsl_config.mic_xyz_mm[3][1] = 0.0f; // Y
        vsl_config.mic_xyz_mm[3][2] = 0.0f;   // Z
    }
#else
#if (DEVICE_ID_VS == Device_IMXRT500_FusionF1)
    /* MIMXRT595-EVK */

    // MIC0 (U120)
    if (vsl_config.num_mics > 0) {
		vsl_config.mic_xyz_mm[0][0] = 0.0f; // X
		vsl_config.mic_xyz_mm[0][1] = 0.0f; // Y
		vsl_config.mic_xyz_mm[0][2] = 0.0f; // Z
	}
    // MIC1 (U121)
	if (vsl_config.num_mics > 1) {
		vsl_config.mic_xyz_mm[1][0] = 6.6f; // X
		vsl_config.mic_xyz_mm[1][1] = 0.0f; // Y
		vsl_config.mic_xyz_mm[1][2] = 0.0f; // Z
	}
#else
    /* MIMXRT685-EVK and MIMXRTRT685-AUD-EVK */

	// MIC0
	if (vsl_config.num_mics > 0) {
		vsl_config.mic_xyz_mm[0][0] = 0.0f; // X
		vsl_config.mic_xyz_mm[0][1] = 0.0f; // Y
		vsl_config.mic_xyz_mm[0][2] = 0.0f; // Z
	}

	// MIC1
	if (vsl_config.num_mics > 1) {
		vsl_config.mic_xyz_mm[1][0] = 95.0f; // X
		vsl_config.mic_xyz_mm[1][1] = 0.0f;  // Y
		vsl_config.mic_xyz_mm[1][2] = 0.0f;  // Z
	}
#endif
#endif

    /*
     * Query how much heap memory is required for the configuration
     */

    uint32_t heap_req_bytes = VoiceSeekerLight_GetRequiredHeapMemoryBytes(&vsl, &vsl_config);
    CM33_Print("VoiceSeekerLight_GetRequiredHeapMemoryBytes: %i bytes\r\n", heap_req_bytes);

    // Allocate needed memory
    void *heap_memory           = malloc(heap_req_bytes);
    vsl.mem.pPrivateDataBase    = heap_memory;
    vsl.mem.pPrivateDataNext    = heap_memory;
    vsl.mem.FreePrivateDataSize = heap_req_bytes;

    /*
     * VoiceSeekerLight creation
     */
    RdspStatus voiceseeker_status = VoiceSeekerLight_Create(&vsl, &vsl_config);
    CM33_Print("VoiceSeekerLight_Create: voiceseeker_status = %d\r\n", voiceseeker_status);
    if (voiceseeker_status != OK)
    {
        return voiceseeker_status;
    }

    /*
     * VoiceSeekerLight initialization
     */
    VoiceSeekerLight_Init(&vsl);

    /*
     * Retrieve VoiceSeekerLight version and configuration
     */
    rdsp_voiceseekerlight_ver_struct_t vsl_version;
    VoiceSeekerLight_GetLibVersion(&vsl, &vsl_version.major, &vsl_version.minor, &vsl_version.patch);
    CM33_Print("VoiceSeekerLight_GetLibVersion: v%i.%i.%i\n", vsl_version.major, vsl_version.minor, vsl_version.patch);

    VoiceSeekerLight_GetConfig(&vsl, &vsl_config);

    // Unpack configuration
    framesize_in = vsl_constants.framesize_in;

    /*
     * Map mic_in pointers to mic buffer
     */
     // Check right config of VoiceSeeker in and out buffers
    if ((vsl_config.framesize_out % framesize_in) != 0)
        printf("VoiceSeeker in = %d not a modulo of VoiceSeeker out = %d", framesize_in, vsl_config.framesize_out);

    // allocate with the Number of channels of the file
    mic_in = (float**)malloc(sizeof(float*) * vsl_config.num_mics);

    VoiceSeekerLight_PrintConfig(&vsl);
    VoiceSeekerLight_PrintMemOverview(&vsl);

    return status;
}

static inline void xa_voice_seeker_preinit(voice_seeker_t *d)
{
    /* ...pre-configuration initialization; reset internal data */
    memset(d, 0, sizeof(*d));

    /* ...set default parameters */
    d->channels = 2;
    d->pcm_width = 16;
    d->sample_rate = 16000;
    d->frame_size_us = 30000;
}

static XA_ERRORCODE VoiceSeeker_Execute(voice_seeker_t *d)
{
    RdspStatus voiceseeker_status = 0;
    WORD32    k = 0;
    int16_t  *pIn = (int16_t *)d->input;
    int32_t  TempData[VOICESEEKER_FRAMESIZE_OUT * VOICE_SEEKER_MAX_CHANNELS] = {0};
    float *InputTempDataFloat = (float*)TempData;
    int16_t *InputTempDataINT16 = (int16_t*)TempData;
    WORD16   *pOut = (WORD16 *) d->output;
    UWORD32 filled = d->input_avail;
    uint32_t framesize_out = vsl_config.framesize_out;       // VoiceSeeker output
    uint32_t framesize_in_mic = vsl_config.framesize_out;    // Number of samples received from the serial port
    float VoiceSeekerIteration = (float)framesize_out / (float)framesize_in;
    float* vsl_out = NULL;

    k = (WORD32)(d->buffer_size - filled);

    /* ...check I/O buffer */
	XF_CHK_ERR(d->input, XA_VOICE_SEEKER_EXEC_FATAL_INPUT);
	XF_CHK_ERR(d->output, XA_VOICE_SEEKER_EXEC_FATAL_INPUT);

    (k > 0 ? memset((void *)pIn + filled, 0x00, k) : 0);

    if (d->channels == 1)
    {
        memcpy(pOut, pIn, filled);
        /* ...save total number of consumed bytes */
        d->consumed = (UWORD32)filled;

        /* ...save total number of produced bytes */
        d->produced = (UWORD32)filled;

        /* ...put flag saying we have output buffer */
        d->state |= XA_VOICE_SEEKER_FLAG_OUTPUT;

        TRACE(PROCESS, _b("produced: %u bytes"), d->produced);
        return voiceseeker_status;
    }

    // De-Interleaved input streams to TempData
    // In place processing not supported
    DeInterleave(pIn, (int16_t*)TempData, vsl_config.framesize_out, d->channels);

    // Convert  INT16 Q15 data to Float data
    for (uint16_t i = vsl_config.framesize_out * d->channels; i > 0; i--)
    {
        InputTempDataFloat[i-1] = ((float)InputTempDataINT16[i-1]) * INV_32768;
    }

    /*
     * VoiceSeeker Light process
     */
    if ((sizeof(float) * vsl_config.framesize_out * d->channels) < (framesize_in_mic * vsl_config.num_mics * sizeof(float)))
        return AFE_VOICESEEKER_ERROR;

    // Initialize multichannel input pointers
    for (uint32_t imic = 0; imic < vsl_config.num_mics; imic++) {
        mic_in[imic] = &((float *)TempData)[imic* framesize_in_mic];
    }

    while (VoiceSeekerIteration)
    {
        voiceseeker_status = VoiceSeekerLight_Process(&vsl, mic_in, NULL, &vsl_out);
        if (voiceseeker_status != OK)
        {
            CM33_Print("VoiceSeekerLight_Process: voiceseeker_status = %d\r\n", (int32_t)voiceseeker_status);
            return AFE_VOICESEEKER_ERROR;
        }

        // Update multichannel input pointers
        // input pointers incremented by Voiceseeker framesize_in (i.e +32 float*)
        for (uint32_t imic = 0; imic < vsl_config.num_mics; imic++) {
            mic_in[imic] += framesize_in;      // increase address by VoiceSeeker input size i.e 32 samples
        }
        VoiceSeekerIteration--;
    }

    if (vsl_out != NULL)
    {
        // Copy VoiceSeeker output to VIT Input buffer
        memcpy(TempData, vsl_out, framesize_out * sizeof(float));

        // Convert VoiceSeeker output from Float data to INT16 Q15 data
        for (uint16_t i = 0; i < vsl_config.framesize_out; i++)
        {
            ((int16_t*)TempData)[i] = (int16_t)((float)InputTempDataFloat[i] * 32768);
        }

        memcpy(pOut, TempData, vsl_config.framesize_out * sizeof(int16_t));
    }
    else
    {
    	CM33_Print("wrong config between VoiceSeeker input = %zd and out = %d samples.", framesize_in, framesize_out);
    }

    /* ...save total number of consumed bytes */
    d->consumed = (UWORD32)filled;

    /* ...save total number of produced bytes */
    d->produced = (UWORD32)vsl_config.framesize_out * sizeof(int16_t);

    /* ...put flag saying we have output buffer */
    d->state |= XA_VOICE_SEEKER_FLAG_OUTPUT;

    TRACE(PROCESS, _b("produced: %u bytes"), d->produced);

    return AFE_VOICESEEKER_SUCCESS;
}


/* ...runtime reset */
static XA_ERRORCODE xa_voice_seeker_do_runtime_init(voice_seeker_t *d)
{
    /* ...no special processing is needed here */
    return XA_NO_ERROR;
}

/*******************************************************************************
 * Commands processing
 ******************************************************************************/

/* ...codec API size query */
static XA_ERRORCODE xa_voice_seeker_get_api_size(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...check parameters are sane */
    XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...retrieve API structure size */
    *(WORD32 *)pv_value = sizeof(*d);

    return XA_NO_ERROR;
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_voice_seeker_init(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process particular initialization type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_INIT_API_PRE_CONFIG_PARAMS:
    {
        /* ...pre-configuration initialization; reset internal data */
        xa_voice_seeker_preinit(d);

        d->state = XA_VOICE_SEEKER_FLAG_PREINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_API_POST_CONFIG_PARAMS:
    {
        if (VoiceSeeker_Initialize(d) != OK)
        {
            return XA_FATAL_ERROR;
        }

        /* ...post-configuration initialization (all parameters are set) */
        XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        //compute buffer size
        UWORD64 product = (UWORD64)d->sample_rate * d->channels * (d->pcm_width>>3) * d->frame_size_us;
        d->buffer_size = (UWORD32)(product/1000000);

        /* Align the computed buffer size to 8-byte */
        d->buffer_size = (d->buffer_size + 7) & ~0x7;

        /* ...mark post-initialization is complete */
        d->state |= XA_VOICE_SEEKER_FLAG_POSTINIT_DONE;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_PROCESS:
    {
        XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...enter into execution stage */
        d->state |= XA_VOICE_SEEKER_FLAG_RUNNING;

        return XA_NO_ERROR;
    }

    case XA_CMD_TYPE_INIT_DONE_QUERY:
    {
        /* ...check if initialization is done; make sure pointer is sane */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

        /* ...put current status */
        *(WORD32 *)pv_value = (d->state & XA_VOICE_SEEKER_FLAG_RUNNING ? 1 : 0);

        return XA_NO_ERROR;
    }

    default:
        /* ...unrecognised command type */
        TRACE(ERROR, _x("Unrecognised command type: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...standard codec initialization routine */
static XA_ERRORCODE xa_voice_seeker_deinit()
{
    free(vsl.mem.pPrivateDataBase);
    free(vsl_config.mic_xyz_mm);
    return XA_NO_ERROR;
}

static XA_ERRORCODE xa_voice_seeker_set_config_param(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     i_value;

    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...get parameter value  */
    i_value = (UWORD32) *(WORD32 *)pv_value;

    /* ...process individual configuration parameter */
    switch (i_idx)
    {
    case XA_VOICE_SEEKER_CONFIG_PARAM_PCM_WIDTH:
        /* ...check value is permitted (16 bits only) */
        XF_CHK_ERR((i_value == 16) || (i_value == 32), XA_VOICE_SEEKER_CONFIG_NONFATAL_RANGE);
        d->pcm_width = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_VOICE_SEEKER_CONFIG_PARAM_CHANNELS:
        /* ...allow mono/stereo only */
        XF_CHK_ERR((i_value <= VOICE_SEEKER_MAX_CHANNELS), XA_VOICE_SEEKER_CONFIG_NONFATAL_RANGE);
        d->channels = (UWORD32)i_value;
        return XA_NO_ERROR;

    case XA_VOICE_SEEKER_CONFIG_PARAM_SAMPLE_RATE:
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
                    XF_CHK_ERR(0, XA_VOICE_SEEKER_CONFIG_NONFATAL_RANGE);
            }
            return XA_NO_ERROR;
        }

    case XA_VOICE_SEEKER_CONFIG_PARAM_INPUT_FRAME_SIZE_US:
    case XA_VOICE_SEEKER_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US:
        d->frame_size_us = (UWORD32)i_value;
        return XA_NO_ERROR;
    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...retrieve configuration parameter */
static XA_ERRORCODE xa_voice_seeker_get_config_param(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...make sure pre-initialization is completed */
    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual configuration parameter */
    switch (i_idx & 0xF)
    {
    case XA_VOICE_SEEKER_CONFIG_PARAM_SAMPLE_RATE:
        /* ...return sample rate */
        *(WORD32 *)pv_value = d->sample_rate;
        return XA_NO_ERROR;

    case XA_VOICE_SEEKER_CONFIG_PARAM_PCM_WIDTH:
        /* ...return current PCM width */
        *(WORD32 *)pv_value = d->pcm_width;
        return XA_NO_ERROR;

    case XA_VOICE_SEEKER_CONFIG_PARAM_CHANNELS:
        /* ...return current channel number */
        *(WORD32 *)pv_value = d->channels;
        return XA_NO_ERROR;

    case XA_VOICE_SEEKER_CONFIG_PARAM_PRODUCED:
        /* ...return no.of produced bytes */
        *(WORD32 *)pv_value = d->produced;
        return XA_NO_ERROR;

    case XA_VOICE_SEEKER_CONFIG_PARAM_INPUT_FRAME_SIZE_US:
    case XA_VOICE_SEEKER_CONFIG_PARAM_OUTPUT_FRAME_SIZE_US:
        *(WORD32 *)pv_value = d->frame_size_us;
        return XA_NO_ERROR;

    default:
        TRACE(ERROR, _x("Invalid parameter: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...execution command */
static XA_ERRORCODE xa_voice_seeker_execute(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    XA_ERRORCODE ret;
#ifdef XAF_PROFILE
    clk_t comp_start, comp_stop;
#endif

    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...process individual command type */
    switch (i_idx)
    {
    case XA_CMD_TYPE_DO_EXECUTE:
        /* ...perform mixing of the channels */
#ifdef XAF_PROFILE
        comp_start = clk_read_start(CLK_SELN_THREAD);
#endif
        ret = VoiceSeeker_Execute(d);
#ifdef XAF_PROFILE
        comp_stop = clk_read_stop(CLK_SELN_THREAD);
        voice_seeker_cycles += clk_diff(comp_stop, comp_start);
#endif
        return ret;

    case XA_CMD_TYPE_DONE_QUERY:
        /* ...check if processing is complete */
        XF_CHK_ERR(pv_value, XA_API_FATAL_INVALID_CMD_TYPE);
        *(WORD32 *)pv_value = (d->state & XA_VOICE_SEEKER_FLAG_COMPLETE ? 1 : 0);
        return XA_NO_ERROR;

    case XA_CMD_TYPE_DO_RUNTIME_INIT:
        return xa_voice_seeker_do_runtime_init(d);

    default:
        /* ...unrecognised command */
        TRACE(ERROR, _x("Invalid index: %X"), i_idx);
        return XA_API_FATAL_INVALID_CMD_TYPE;
    }
}

/* ...set number of input bytes */
static XA_ERRORCODE xa_voice_seeker_set_input_bytes(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    UWORD32     size;

    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...input frame length should not be zero (in bytes) */
    XF_CHK_ERR((size = (UWORD32)*(WORD32 *)pv_value) >= 0, XA_VOICE_SEEKER_EXEC_NONFATAL_INPUT);

    /* ...all is correct; set input buffer length in bytes */
    d->input_avail = size;

    return XA_NO_ERROR;
}

/* ...get number of output bytes */
static XA_ERRORCODE xa_voice_seeker_get_output_bytes(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be zero */
    XF_CHK_ERR(i_idx == 1, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_RUNNING, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...output buffer must exist */
    XF_CHK_ERR(d->output, XA_VOICE_SEEKER_EXEC_NONFATAL_OUTPUT);

    /* ...return number of produced bytes */
    *(WORD32 *)pv_value = ((d->state & XA_VOICE_SEEKER_FLAG_OUTPUT) ? d->produced : 0);

    return XA_NO_ERROR;
}

/* ...get number of consumed bytes */
static XA_ERRORCODE xa_voice_seeker_get_curidx_input_buf(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check - check parameters */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...track index must be valid */
    XF_CHK_ERR(i_idx == 0, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_RUNNING, XA_VOICE_SEEKER_EXEC_FATAL_STATE);

    /* ...input buffer must exist */
    XF_CHK_ERR(d->input, XA_VOICE_SEEKER_EXEC_FATAL_INPUT);

    /* ...return number of bytes consumed (always consume fixed-length chunk) */
    *(WORD32 *)pv_value = d->consumed;

    /* ...and reset internally managed number of consumed bytes */
    d->consumed = 0;

    return XA_NO_ERROR;
}

/* ...end-of-stream processing */
static XA_ERRORCODE xa_voice_seeker_input_over(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...sanity check */
    XF_CHK_ERR(d, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...put end-of-stream flag */
    d->state |= XA_VOICE_SEEKER_FLAG_COMPLETE;

    TRACE(PROCESS, _b("Input-over-condition signalled"));

    return XA_NO_ERROR;
}

/*******************************************************************************
 * Memory information API
 ******************************************************************************/
/* ..get total amount of data for memory tables */
static XA_ERRORCODE xa_voice_seeker_get_memtabs_size(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    *(WORD32 *)pv_value = 0;

    return XA_NO_ERROR;
}

/* ..set memory tables pointer */
static XA_ERRORCODE xa_voice_seeker_set_memtabs_ptr(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_PREINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    return XA_NO_ERROR;
}

/* ...return total amount of memory buffers */
static XA_ERRORCODE xa_voice_seeker_get_n_memtabs(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity checks */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...we have 1 input buffer, 1 output buffer and 1 scratch buffer*/
    *(WORD32 *)pv_value = 3;

    return XA_NO_ERROR;
}

/* ...return memory buffer data */
static XA_ERRORCODE xa_voice_seeker_get_mem_info_size(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are of the same length */
    *(WORD32 *)pv_value = (WORD32) d->buffer_size;

    return XA_NO_ERROR;
}

/* ...return memory alignment data */
static XA_ERRORCODE xa_voice_seeker_get_mem_info_alignment(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...all buffers are 4-bytes aligned */
    *(WORD32 *)pv_value = 4;

    return XA_NO_ERROR;
}

/* ...return memory type data */
static XA_ERRORCODE xa_voice_seeker_get_mem_info_type(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...return frame buffer minimal size only after post-initialization is done */
    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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
static XA_ERRORCODE xa_voice_seeker_set_mem_ptr(voice_seeker_t *d, WORD32 i_idx, pVOID pv_value)
{
    /* ...basic sanity check */
    XF_CHK_ERR(d && pv_value, XA_API_FATAL_INVALID_CMD_TYPE);

    /* ...codec must be initialized */
    XF_CHK_ERR(d->state & XA_VOICE_SEEKER_FLAG_POSTINIT_DONE, XA_API_FATAL_INVALID_CMD_TYPE);

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

static XA_ERRORCODE (* const xa_voice_seeker_api[])(voice_seeker_t *, WORD32, pVOID) =
{
    [XA_API_CMD_GET_API_SIZE]           = xa_voice_seeker_get_api_size,

    [XA_API_CMD_INIT]                   = xa_voice_seeker_init,
    [XA_API_CMD_DEINIT]                 = xa_voice_seeker_deinit,
    [XA_API_CMD_SET_CONFIG_PARAM]       = xa_voice_seeker_set_config_param,
    [XA_API_CMD_GET_CONFIG_PARAM]       = xa_voice_seeker_get_config_param,

    [XA_API_CMD_EXECUTE]                = xa_voice_seeker_execute,
    [XA_API_CMD_SET_INPUT_BYTES]        = xa_voice_seeker_set_input_bytes,
    [XA_API_CMD_GET_OUTPUT_BYTES]       = xa_voice_seeker_get_output_bytes,
    [XA_API_CMD_GET_CURIDX_INPUT_BUF]   = xa_voice_seeker_get_curidx_input_buf,
    [XA_API_CMD_INPUT_OVER]             = xa_voice_seeker_input_over,

    [XA_API_CMD_GET_MEMTABS_SIZE]       = xa_voice_seeker_get_memtabs_size,
    [XA_API_CMD_SET_MEMTABS_PTR]        = xa_voice_seeker_set_memtabs_ptr,
    [XA_API_CMD_GET_N_MEMTABS]          = xa_voice_seeker_get_n_memtabs,
    [XA_API_CMD_GET_MEM_INFO_SIZE]      = xa_voice_seeker_get_mem_info_size,
    [XA_API_CMD_GET_MEM_INFO_ALIGNMENT] = xa_voice_seeker_get_mem_info_alignment,
    [XA_API_CMD_GET_MEM_INFO_TYPE]      = xa_voice_seeker_get_mem_info_type,
    [XA_API_CMD_SET_MEM_PTR]            = xa_voice_seeker_set_mem_ptr,
};

/* ...total number of commands supported */
#define XA_VOICE_SEEKER_API_COMMANDS_NUM   (sizeof(xa_voice_seeker_api) / sizeof(xa_voice_seeker_api[0]))

/*******************************************************************************
 * API entry point
 ******************************************************************************/

XA_ERRORCODE xa_voice_seeker(xa_codec_handle_t p_xa_module_obj, WORD32 i_cmd, WORD32 i_idx, pVOID pv_value)
{
    voice_seeker_t *d = (voice_seeker_t *) p_xa_module_obj;

    /* ...check if command index is sane */
    XF_CHK_ERR(i_cmd < XA_VOICE_SEEKER_API_COMMANDS_NUM, XA_API_FATAL_INVALID_CMD);

    /* ...see if command is defined */
    XF_CHK_ERR(xa_voice_seeker_api[i_cmd], XA_API_FATAL_INVALID_CMD);

    /* ...execute requested command */
    return xa_voice_seeker_api[i_cmd](d, i_idx, pv_value);
}

//  de-Interleave Multichannel signal
//   example:  A1.B1.C1.A2.B2.C2.A3.B3.C3....An.Bn.Cn   (3 Channels case : A, B, C)
//             will become
//             A1.A2.A3....An.B1.B2.B3....Bn.C1.C2.C3....Cn

// Simple helper function for de-interleaving Multichannel stream
// The caller function shall ensure that all arguments are correct.
// In place processing not supported
void DeInterleave(const int16_t *pDataInput, int16_t *pDataOutput, uint16_t FrameSize, uint16_t ChannelNumber)
{

    for (uint16_t ichan = 0; ichan < ChannelNumber; ichan++)
    {
        for (uint16_t i = 0; i < FrameSize; i++)
        {
            pDataOutput[i+(ichan*FrameSize)] = pDataInput[(i*ChannelNumber)+ichan];
        }
    }
    return;
}

#undef DSP_PRINTF
#define DSP_PRINTF printf

#endif
