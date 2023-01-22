/*
 * Copyright 2018-2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "srtm_config.h"
#include "srtm_utils.h"

#include "fsl_common.h"
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
#include "fsl_memory.h"
#endif

#include <xtensa/config/core.h>
#include <xtensa/xos.h>
#include "xaf-api.h"

#include "fsl_gpio.h"

#include "dsp_config.h"
#include "board_fusionf1.h"
#include "fsl_inputmux.h"
#include "fsl_dma.h"
#include "fsl_i2s.h"
#include "pin_mux.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define INIT_DEBUG_CONSOLE 0

#define APP_RPMSG_READY_EVENT_DATA    (1)
#define APP_RPMSG_EP_READY_EVENT_DATA (2)

#define NOTIF_EVT_RPMSG_RECEIVED_DATA (1 << 0)
#define NOTIF_EVT                     (NOTIF_EVT_RPMSG_RECEIVED_DATA)
#define BOARD_XTAL_SYS_CLK_HZ 24000000U /*!< Board xtal_sys frequency in Hz */
#define BOARD_XTAL32K_CLK_HZ  32768U    /*!< Board xtal32K frequency in Hz */
#define DSP_THREAD_STACK_SIZE (10 * 1024)
#define DSP_THREAD_PRIORITY   (XOS_MAX_PRIORITY - 3)

#define AUDIO_BUFFER_SIZE (32 * 1024)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* SRTM prototypes */
int srtm_decoder(dsp_handle_t *dsp, unsigned int *pCmdParams, unsigned int dec_name);
int srtm_encoder(dsp_handle_t *dsp, unsigned int *pCmdParams, unsigned int enc_name);
int srtm_src(dsp_handle_t *dsp, unsigned int *pCmdParams);
int srtm_pcm_gain(dsp_handle_t *dsp, unsigned int *pCmdParams);
int srtm_file_dec_create(dsp_handle_t *dsp, srtm_audio_component_t type);
#if XA_CLIENT_PROXY
int client_proxy_filter(dsp_handle_t *dsp, int filterOn);
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

dsp_handle_t dsp;
static uint8_t dsp_thread_stack[DSP_THREAD_STACK_SIZE];

/*******************************************************************************
 * Code
 ******************************************************************************/
static void BOARD_InitClock(void)
{
    CLOCK_SetXtalFreq(BOARD_XTAL_SYS_CLK_HZ); /* sets external XTAL OSC freq */

    CLOCK_EnableClock(kCLOCK_InputMux);

    /* DSP_INT0_SEL18 = DMA1 */
    INPUTMUX_AttachSignal(INPUTMUX, 18U, kINPUTMUX_Dmac1ToDspInterrupt);
    /* MUB interrupt signal is selected for DSP interrupt input 1 */
    INPUTMUX_AttachSignal(INPUTMUX, 1U, kINPUTMUX_MuBToDspInterrupt);

    /* I2S clocks */
    /* attach AUDIO PLL clock to FLEXCOMM1 (I2S1) for codec output*/
    CLOCK_AttachClk(kAUDIO_PLL_to_FLEXCOMM1);
    CLOCK_AttachClk(kFRO_DIV4_to_FLEXCOMM4);
}

static void XOS_Init(void)
{
    xos_set_clock_freq(XOS_CLOCK_FREQ);

    xos_start_system_timer(-1, 0);
}



static void DSP_XAF_Init(dsp_handle_t *dsp)
{
    uint8_t *version[3];

    xos_mutex_create(&dsp->audioMutex, XOS_MUTEX_WAIT_PRIORITY, 0);
    xos_mutex_create(&dsp->rpmsgMutex, XOS_MUTEX_WAIT_PRIORITY, 0);
    xos_cond_create(&dsp->ipc_waiting_cond);
    xos_cond_create(&dsp->read_waiting_cond);
    dsp->file_frame_size = 0;

    dsp->audioBuffer = ringbuf_create(AUDIO_BUFFER_SIZE);
    if (!dsp->audioBuffer)
    {
        DSP_PRINTF("[DSP_XAF_Init] ringbuffer allocation failed\r\n");
    }

    xos_event_create(&dsp->pipeline_event, 0xFF, XOS_EVENT_AUTO_CLEAR);

    xaf_get_verinfo(version);

    DSP_PRINTF("\r\n");
    DSP_PRINTF("Cadence Xtensa Audio Framework\r\n");
    DSP_PRINTF("  Library Name    : %s\r\n", version[0]);
    DSP_PRINTF("  Library Version : %s\r\n", version[1]);
    DSP_PRINTF("  API Version     : %s\r\n", version[2]);
    DSP_PRINTF("\r\n");
}

static int handleMSG_GENERAL(dsp_handle_t *dsp, srtm_message *msg)
{
    switch (msg->head.command)
    {
        /* Return SDK key component versions to host*/
        case SRTM_Command_ECHO:
            // 0 Audio Framework version high 16 bits major, lower 16 bits minor
            msg->param[0] = ((2) << 16 | (0));
            // 1 Audio Framework API version high 16 bits major, lower 16 bits minor
            msg->param[1] = ((1) << 16 | (3));
            // 2 NatureDSP Lib version high 16 bits major, lower 16 bits minor
            msg->param[2] = ((4) << 16 | (0));
            // 3 NatureDSP Lib API version high 16 bits major, lower 16 bits minor
            msg->param[3] = ((4) << 16 | (10));
            // 4 MP3 Decoder version high 16 bits major, lower 16 bits minor
            msg->param[4] = ((3) << 16 | (18));
            // 5 AAC Decoder version high 16 bits major, lower 16 bits minor
            msg->param[5] = ((3) << 16 | (7));
#if XA_VORBIS_DECODER
            // 6 VORBIS Decoder version high 16 bits major, lower 16 bits minor
            msg->param[6] = ((1) << 16 | (12));
#endif
            // 7 OPUS Codec version high 16 bits major, lower 16 bits minor
            msg->param[7] = ((1) << 16 | (8));
            // 8 SBC Decoder version high 16 bits major, lower 16 bits minor
            msg->param[8] = ((1) << 16 | (4));
            // 9 SBC Encoder version high 16 bits major, lower 16 bits minor
            msg->param[9] = ((1) << 16 | (5));
            break;

        /* Unknown message. */
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
            break;
    }

    return 0;
}

static int handleMSG_AUDIO(dsp_handle_t *dsp, srtm_message *msg)
{
    char *remote_addr;

    dsp->event_occur = false;

    switch (msg->head.command)
    {
        case SRTM_Command_MP3:
            /* Param 0 MP3 input buffer address*/
            /* Param 1 MP3 input buffer size*/
            /* Param 2 PCM output buffer address*/
            /* Param 3 PCM output buffer size*/
            /* Param 4 decode output location */
            /* Param 5 return parameter, actual read size */
            /* Param 6 return parameter, actual write size */
            DSP_PRINTF("MP3 input buffer addr 0x%X, buffer size %d, output buffer addr 0x%X, output buffer size %d\r\n",
                       msg->param[0], msg->param[1], msg->param[2], msg->param[3]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_decoder(dsp, &msg->param[0], SRTM_Command_MP3);
            break;

        case SRTM_Command_OPUS_DEC:
            /* Param 0 OPUS input buffer address*/
            /* Param 1 OPUS input buffer size*/
            /* Param 2 PCM output buffer address*/
            /* Param 3 PCM output buffer size*/
            /* Param 4 decode output location */
            /* Param 5 return parameter, actual read size */
            /* Param 6 return parameter, actual write size */
            DSP_PRINTF("OPUS input buffer addr 0x%X, buffer size %d, output buffer addr 0x%X, output buffer size %d\n",
                       msg->param[0], msg->param[1], msg->param[2], msg->param[3]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_decoder(dsp, &msg->param[0], SRTM_Command_OPUS_DEC);
            break;

        case SRTM_Command_OPUS_ENC:
            /* Param 0 PCM input buffer address*/
            /* Param 1 PCM input buffer size*/
            /* Param 2 OPUS output buffer address*/
            /* Param 3 OPUS output buffer size*/
            /* Param 4 return parameter, actual read size */
            /* Param 5 return parameter, actual write size */
            DSP_PRINTF(
                "PCM input buffer addr 0x%X, buffer size %d, OPUS output buffer addr 0x%X, output buffer size %d\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_encoder(dsp, &msg->param[0], SRTM_Command_OPUS_ENC);
            break;

        case SRTM_Command_SBC_DEC:
            /* Param 0 SBC input buffer address*/
            /* Param 1 SBC input buffer size*/
            /* Param 2 PCM output buffer address*/
            /* Param 3 PCM output buffer size*/
            /* Param 4 decode output location */
            /* Param 5 return parameter, actual read size */
            /* Param 6 return parameter, actual write size */
            DSP_PRINTF("SBC input buffer addr 0x%X, buffer size %d, output buffer addr 0x%X, output buffer size %d\n",
                       msg->param[0], msg->param[1], msg->param[2], msg->param[3]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_decoder(dsp, &msg->param[0], SRTM_Command_SBC_DEC);
            break;

        case SRTM_Command_SBC_ENC:
            /* Param 0 PCM input buffer address*/
            /* Param 1 PCM input buffer size*/
            /* Param 2 SBC output buffer address*/
            /* Param 3 SBC output buffer size*/
            /* Param 4 return parameter, actual read size */
            /* Param 5 return parameter, actual write size */
            DSP_PRINTF(
                "PCM input buffer addr 0x%X, buffer size %d, SBC output buffer addr 0x%X, output buffer size %d\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_encoder(dsp, &msg->param[0], SRTM_Command_SBC_ENC);
            break;

        case SRTM_Command_AAC:
            /* Param 0 AAC input buffer address*/
            /* Param 1 AAC input buffer size*/
            /* Param 2 PCM output buffer address*/
            /* Param 3 PCM output buffer size*/
            /* Param 4 decode output location */
            /* Param 5 return parameter, actual read size */
            /* Param 6 return parameter, actual write size */
            DSP_PRINTF("AAC input buffer addr 0x%X, buffer size %d, output buffer addr 0x%X, output buffer size %d\r\n",
                       msg->param[0], msg->param[1], msg->param[2], msg->param[3]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_decoder(dsp, &msg->param[0], SRTM_Command_AAC);
            break;

        case SRTM_Command_VORBIS:
            /* Param 0 VORBIS input buffer address */
            /* Param 1 VORBIS input buffer size */
            /* Param 2 PCM output buffer address */
            /* Param 3 PCM output buffer size */
            /* Param 4 decode output location */
            /* Param 5 return parameter, actual read size */
            /* Param 6 return parameter, actual write size */
            /* Param 7 VORBIS input buffer is raw (1) or OGG encapsulated (0) */
            DSP_PRINTF(
                "VORBIS input buffer addr 0x%X, buffer size %d, "
                "output buffer addr 0x%X, output buffer size %d, "
                "output_renderer %d, raw_input %d\r\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3], msg->param[4], msg->param[7]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
                break;
            }
            msg->error = srtm_decoder(dsp, &msg->param[0], SRTM_Command_VORBIS);
            break;

        case SRTM_Command_SRC:
            /* Param 0 SRC input buffer 1 address*/
            /* Param 1 SRC input buffer 1 size*/
            /* Param 2 SRC input sampling rate*/
            /* Param 3 SRC input channels*/
            /* Param 4 SRC input sample width*/
            /* Param 5 SRC output buffer address*/
            /* Param 6 SRC output buffer size*/
            /* Param 7 SRC output sampling rate*/
            /* Param 8 return paramter, actual read size of input*/
            /* Param 9 return paramter, actual write size of output*/
            DSP_PRINTF(
                "Input buffer addr 0x%X, size %d, rate %d, channels %d, sample width %d, output buffer \
                addr 0x%X, size %d, rate %d\r\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3], msg->param[4], msg->param[5], msg->param[6],
                msg->param[7]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0) ||
                (msg->param[4] == 0) || (msg->param[5] == 0) || (msg->param[6] == 0) || (msg->param[7] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_ServiceNotFound;
                break;
            }
            msg->error = srtm_src(dsp, &msg->param[0]);
            break;

        case SRTM_Command_GAIN:
            /* Param 0 PCM input buffer address*/
            /* Param 1 PCM input buffer size*/
            /* Param 2 PCM output buffer address*/
            /* Param 3 PCM output buffer size*/
            /* Param 4 PCM sampling rate, default 44100*/
            /* Param 5 PCM number of channels, only 1 or 2 supported, default 1*/
            /* Param 6 PCM sample width, default 16*/
            /* Param 7 Gain control index, default is 4, range is 0 to 6 -> {0db, -6db, -12db, -18db, 6db,
            12db,
             * 18db}*/
            /* Param 8 return parameter, actual read bytes*/
            /* Param 9 return parameter, actual written bytes*/
            msg->param[10] = msg->param[0];
            msg->param[11] = msg->param[1];
            msg->param[12] = msg->param[2];
            msg->param[13] = msg->param[3];
            msg->param[14] = msg->param[4];
            msg->param[15] = msg->param[5];
            msg->param[16] = msg->param[6];
            msg->param[17] = msg->param[7];
            DSP_PRINTF(
                "Input buffer addr 0x%X, size %d, output buffer addr 0x%X, size %d, sampling rate %d, num \
                of channels %d, pcm sample width %d, gain control index %d\r\n",
                msg->param[0], msg->param[1], msg->param[2], msg->param[3], msg->param[4], msg->param[5], msg->param[6],
                msg->param[7]);
            if ((msg->param[0] == 0) || (msg->param[1] == 0) || (msg->param[2] == 0) || (msg->param[3] == 0) ||
                (msg->param[4] == 0) || (msg->param[5] == 0) || (msg->param[6] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            else
            {
                msg->error = srtm_pcm_gain(dsp, &msg->param[0]);
            }
            break;

        case SRTM_Command_FileStart:
            /* Param 0 File buffer */
            /* Param 1 File buffer size */
            if ((msg->param[0] == 0) || (msg->param[1] == 0))
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            /* Check to see if file already playing */
            else if (dsp->file_playing)
            {
                msg->error = SRTM_Status_InvalidState;
            }
            else
            {
                DSP_PRINTF("File playback start, initial buffer size: %d\r\n", msg->param[1]);
                /* Clear ringbuffer from previous playback */
                ringbuf_clear(dsp->audioBuffer);

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
                remote_addr = (char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA);
#else
                remote_addr = (char *)msg->param[0];
#endif

                /* Write initial data chunk to the ringbuffer */
                DSP_AudioWriteRing(dsp, remote_addr, msg->param[1]);
                /* Initialize pipeline */
                dsp->eof         = msg->param[2];
                dsp->ipc_waiting = false;
                msg->error       = srtm_file_dec_create(dsp, msg->param[3]);
                msg->param[0]    = dsp->channel_num;
            }
            break;

        case SRTM_Command_FileData:
            /* Param 0 File buffer */
            /* Param 1 File buffer size */
            /* Param 2 File EOF status (boolean true/false) */
            if (msg->param[0] == 0)
            {
                msg->head.type = SRTM_MessageTypeNotification;
                msg->error     = SRTM_Status_InvalidParameter;
            }
            else
            {
#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
                remote_addr = (char *)MEMORY_ConvertMemoryMapAddress(msg->param[0], kMEMORY_Local2DMA);
#else
                remote_addr = (char *)msg->param[0];
#endif

                DSP_AudioWriteRing(dsp, remote_addr, msg->param[1]);
                /* Signal EOF to processing thread if set by remote */
                if (msg->param[2])
                {
                    dsp->eof = true;
                }
            }

            dsp->ipc_waiting = false;

            /* Send signal to wake DSP_BufferThread */
            xos_cond_signal(&dsp->ipc_waiting_cond, 0);
            break;

        case SRTM_Command_FileStop:
            /* File must be already playing */
            if (!dsp->file_playing)
            {
                msg->error = SRTM_Status_InvalidState;
            }
            else
            {
                DSP_PRINTF("File playback stop\r\n");
                xos_event_set(&dsp->pipeline_event, DSP_EVENT_STOP);
            }
            break;

#if XA_CLIENT_PROXY
        case SRTM_Command_FilterCfg:
            msg->error = client_proxy_filter(dsp, msg->param[0]);
            break;
#endif

        /* Unknown message. */
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
    }

    return 0;
}

static int DSP_MSG_Process(dsp_handle_t *dsp, srtm_message *msg)
{
    srtm_message_type_t input_type = msg->head.type;

    /* Sanity check */
    if ((msg->head.majorVersion != SRTM_VERSION_MAJOR) || (msg->head.minorVersion != SRTM_VERSION_MINOR))
    {
        DSP_PRINTF("SRTM version doesn't match!\r\n");
        return -1;
    }

    /* Process message */
    switch (msg->head.category)
    {
        case SRTM_MessageCategory_GENERAL:
            handleMSG_GENERAL(dsp, msg);
            break;
        case SRTM_MessageCategory_AUDIO:
            handleMSG_AUDIO(dsp, msg);
            break;
        default:
            msg->head.type = SRTM_MessageTypeNotification;
            msg->error     = SRTM_Status_InvalidMessage;
    }

    /* Send response message, if request was received */
    if (input_type == SRTM_MessageTypeRequest)
    {
        /* Prepare response msg */
        msg->head.type = SRTM_MessageTypeResponse;

        /* Send response */
        xos_mutex_lock(&dsp->rpmsgMutex);
        rpmsg_lite_send(dsp->rpmsg, dsp->ept, MCU_EPT_ADDR, (char *)msg, sizeof(srtm_message), RL_DONT_BLOCK);
        xos_mutex_unlock(&dsp->rpmsgMutex);
    }

    return 0;
}

/*!
 * @brief Main function
 */
int DSP_Main(void *arg, int wake_value)
{
    dsp_handle_t *dsp = (dsp_handle_t *)arg;
    void *rpmsg_shmem_base;
    srtm_message msg;
    int status;

    DSP_XAF_Init(dsp);

    DSP_PRINTF("[DSP_Main] start\r\n");

#if (defined(FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET) && FSL_FEATURE_MEMORY_HAS_ADDRESS_OFFSET)
    rpmsg_shmem_base = (void *)MEMORY_ConvertMemoryMapAddress((uint32_t)RPMSG_LITE_SHMEM_BASE, kMEMORY_Local2DMA);
#else
    rpmsg_shmem_base = RPMSG_LITE_SHMEM_BASE;
#endif

    dsp->rpmsg       = rpmsg_lite_remote_init(rpmsg_shmem_base, RPMSG_LITE_LINK_ID, RL_NO_FLAGS);
    dsp->rpmsg_queue = rpmsg_queue_create(dsp->rpmsg);

    rpmsg_lite_wait_for_link_up(dsp->rpmsg, RL_BLOCK);

    DSP_PRINTF("[DSP_Main] established RPMsg link\r\n");

    dsp->ept = rpmsg_lite_create_ept(dsp->rpmsg, DSP_EPT_ADDR, rpmsg_queue_rx_cb, (void *)dsp->rpmsg_queue);

    while (1)
    {
        /* Block until receive message from ARM core */
        status =
            rpmsg_queue_recv(dsp->rpmsg, dsp->rpmsg_queue, NULL, (char *)&msg, sizeof(srtm_message), NULL, RL_BLOCK);
        if (status != RL_SUCCESS)
        {
            xos_fatal_error(status, "Failed to get item from RPMsg queue.\r\n");
        }

        /* Process request */
        status = DSP_MSG_Process(dsp, &msg);
    }

    rpmsg_lite_destroy_ept(dsp->rpmsg, dsp->ept);
    rpmsg_lite_deinit(dsp->rpmsg);

    return 0;
}

int main(void)
{
    XosThread thread_main;

    XOS_Init();
    BOARD_InitPins();
    BOARD_InitClock();
#if INIT_DEBUG_CONSOLE || APP_DSP_ONLY
    BOARD_InitDebugConsole();
#endif

#ifdef XA_CLIENT_PROXY
    /* Dummy I2S init for EAP */
    i2s_config_t s_TxConfig;
    I2S_TxGetDefaultConfig(&s_TxConfig);
    I2S_TxInit(I2S1, &s_TxConfig);
#endif
    /* Iniitalize DMA1 which will be shared by capturer and renderer. */
    DMA_Init(DMA1);

    /* Map DMA IRQ handler to INPUTMUX selection DSP_INT0_SEL18
     * EXTINT19 = DSP INT 23 */
    xos_register_interrupt_handler(XCHAL_EXTINT19_NUM, (XosIntFunc *)DMA_IRQHandle, DMA1);
    xos_interrupt_enable(XCHAL_EXTINT19_NUM);

    xos_thread_create(&thread_main, NULL, DSP_Main, &dsp, "DSP Main", dsp_thread_stack, DSP_THREAD_STACK_SIZE,
                      DSP_THREAD_PRIORITY, 0, 0);

    /* Start XOS scheduler - does not return */
    xos_start(0);

    /* Should not reach this statement. */
    return 0;
}
