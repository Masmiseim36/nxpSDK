/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "a2dp_pl.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_sai.h"
#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_codec_common.h"
#include "fsl_wm8960.h"
#include "fsl_codec_adapter.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define OVER_SAMPLE_RATE     (256U)
#define BOARD_DEMO_SAI       SAI1
#define DEMO_SAI_IRQ_TX      SAI1_IRQn
#define SAI_UserTxIRQHandler SAI1_IRQHandler
#define  AUDIO_SAMPLING_RATE  44100

#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7))
/* Select Audio/Video PLL (786.48 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (2U)
/* Clock pre divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER (0U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (63U)
/* Get frequency of sai1 clock */

#define DEMO_SAI_CLK_FREQ                                                        \
    (CLOCK_GetFreq(kCLOCK_AudioPllClk) / (DEMO_SAI1_CLOCK_SOURCE_DIVIDER + 1U) / \
     (DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER + 1U))
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
/* Select Audio/Video PLL (393.24 MHz) as sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_SELECT (4U)
/* Clock divider for sai1 clock source */
#define DEMO_SAI1_CLOCK_SOURCE_DIVIDER (32U)
/* Get frequency of sai1 clock */
#define DEMO_SAI_CLK_FREQ (CLOCK_GetFreq(kCLOCK_AudioPll) / DEMO_SAI1_CLOCK_SOURCE_DIVIDER)
#endif
#define BOARD_DEMO_I2C LPI2C1

/* Select USB1 PLL (480 MHz) as master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_SELECT (0U)
/* Clock divider for master lpi2c clock source */
#define DEMO_LPI2C_CLOCK_SOURCE_DIVIDER (5U)
/* Get frequency of lpi2c clock */
#define BOARD_DEMO_I2C_FREQ ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (DEMO_LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

/* DMA */
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7))
#define EXAMPLE_DMAMUX        DMAMUX
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
#define EXAMPLE_DMAMUX        DMAMUX0
#endif
#define EXAMPLE_DMA           DMA0
#define EXAMPLE_DMA_IRQ       DMA0_DMA16_IRQn
#define EXAMPLE_TX_CHANNEL    (0U)
#define EXAMPLE_SAI_TX_SOURCE kDmaRequestMuxSai1Tx

#define AUDIO_INTERRUPT_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY)
/* demo audio data channel */
#define DEMO_AUDIO_DATA_CHANNEL (2U)
/* demo audio bit width */
#define DEMO_AUDIO_BIT_WIDTH kSAI_WordWidth16bits
/* demo audio sample frequency */
#define DEMO_AUDIO_SAMPLING_RATE  (kSAI_SampleRate44100Hz)
#ifdef A2DP_SOURCE
extern UINT32 a2dp_src_sf;
extern UCHAR a2dp_src_bps;
extern UCHAR a2dp_src_nc;
extern UINT16 a2dp_src_size;
extern UINT16 a2dp_src_num_samples;
extern void (* a2dp_src_cb)(const UCHAR *data, UINT16 datalen);
extern UCHAR a2dp_src_playback;
extern BT_timer_handle a2dp_src_timer;
void a2dp_pl_start_playback_timer(void);
void a2dp_init_default_src_pl(void);
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
extern UINT32 a2dp_snk_sf;
extern UCHAR a2dp_snk_bps;
extern UCHAR a2dp_snk_nc;
extern UINT16 a2dp_snk_size;
extern void (* a2dp_snk_cb)(const UCHAR *data, UINT16 datalen);
#endif /* A2DP_SINK */
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern osa_semaphore_handle_t xSemaphoreAudio;

/*******************************************************************************
 * Variables
 ******************************************************************************/


static uint32_t currentFreq = 0;
static uint32_t currentBitWidth;
static uint32_t currentChannelNum;
static uint32_t audioStart, audiosetup;
AT_NONCACHEABLE_SECTION_INIT(sai_edma_handle_t txHandle) = {0};
edma_handle_t dmaTxHandle  = {0};
static uint32_t masterClockHz = 0U;
codec_handle_t codecHandle;
wm8960_config_t wm8960Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = BOARD_CODEC_I2C_CLOCK_FREQ},
    .route     = kWM8960_RoutePlaybackandRecord,
    .rightInputSource = kWM8960_InputDifferentialMicInput2,
    .playSource       = kWM8960_PlaySourceDAC,
    .slaveAddress     = WM8960_I2C_ADDR,
    .bus              = kWM8960_BusI2S,
    .format           = {.mclk_HZ    = 11289750U,
               .sampleRate = kWM8960_AudioSampleRate44100Hz,
               .bitWidth   = kWM8960_AudioBitWidth16bit},
    .master_slave     = false,
};
codec_config_t boardCodecConfig = {.codecDevType = kCODEC_WM8960, .codecDevConfig = &wm8960Config};
/*setting for 44.1Khz*/
const clock_audio_pll_config_t audioPllConfig = {
    .loopDivider = 30,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 106,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 1000, /* 30 bit denominator of fractional loop divider */
};
/*setting for multiple of 8Khz,such as 48Khz/16Khz/32KHz*/
const clock_audio_pll_config_t audioPllConfig1 = {
    .loopDivider = 32,  /* PLL loop divider. Valid range for DIV_SELECT divider value: 27~54. */
    .postDivider = 1,   /* Divider after the PLL, should only be 1, 2, 4, 8, 16. */
    .numerator   = 77,  /* 30 bit numerator of fractional loop divider. */
    .denominator = 100, /* 30 bit denominator of fractional loop divider */
};
/*******************************************************************************
 * Code
 ******************************************************************************/

/* overwrite a2dp_init_pl_ext of a2dp_pl.c.
 * The follow functions can be overwritten too
 * if the actual example need implement them to
 * use different audio data.
 * a2dp_init_pl_ext, a2dp_shutdown_pl_ext,
 * a2dp_setup_pl_ext, a2dp_start_pl_ext,
 * a2dp_stop_pl_ext, a2dp_write_pl_ext.
 */

void BOARD_EnableSaiMclkOutput(bool enable)
{
    if (enable)
    {
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
        IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
        IOMUXC_GPR->GPR0 |= IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK;
#endif
    }
    else
    {
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
        IOMUXC_GPR->GPR1 &= (~IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK);
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
        IOMUXC_GPR->GPR0 &= (~IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK);
#endif
    }
}

void BOARD_Codec_Init(void)
{
    CODEC_Init(&codecHandle, &boardCodecConfig);
}

static void txCallback(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData)
{
    OSA_SemaphorePost(xSemaphoreAudio);
}
void BOARD_USB_Audio_TxInit(uint32_t samplingRate)
{
    sai_transceiver_t config;
    SAI_Init(BOARD_DEMO_SAI);
    SAI_TransferTxCreateHandleEDMA(BOARD_DEMO_SAI, &txHandle, txCallback, NULL, &dmaTxHandle);
    /* I2S mode configurations */
    SAI_GetClassicI2SConfig(&config, DEMO_AUDIO_BIT_WIDTH, kSAI_Stereo, kSAI_Channel0Mask);
    SAI_TransferTxSetConfigEDMA(BOARD_DEMO_SAI, &txHandle, &config);
#if (defined FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER && FSL_FEATURE_SAI_HAS_MCLKDIV_REGISTER) || \
    (defined FSL_FEATURE_PCC_HAS_SAI_DIVIDER && FSL_FEATURE_PCC_HAS_SAI_DIVIDER)
    masterClockHz = OVER_SAMPLE_RATE * sampleRate_Hz;
#else
    masterClockHz = DEMO_SAI_CLK_FREQ;
#endif
    /* set bit clock divider */
    SAI_TxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, samplingRate, DEMO_AUDIO_BIT_WIDTH, DEMO_AUDIO_DATA_CHANNEL);
}

void BOARD_DMA_EDMA_Config(void)
{
    edma_config_t dmaConfig = {0};
    EDMA_GetDefaultConfig(&dmaConfig);
    EDMA_Init(EXAMPLE_DMA, &dmaConfig);
    EDMA_CreateHandle(&dmaTxHandle, EXAMPLE_DMA, EXAMPLE_TX_CHANNEL);
    NVIC_SetPriority((IRQn_Type)EXAMPLE_DMA_IRQ, AUDIO_INTERRUPT_PRIORITY);
    DMAMUX_Init(EXAMPLE_DMAMUX);
    DMAMUX_SetSource(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL, (uint8_t)EXAMPLE_SAI_TX_SOURCE);
    DMAMUX_EnableChannel(EXAMPLE_DMAMUX, EXAMPLE_TX_CHANNEL);
}

void BOARD_DMA_EDMA_Enable_Audio_Interrupts(void)
{
    /* Enable interrupt to handle FIFO error */
    SAI_TxEnableInterrupts(BOARD_DEMO_SAI, kSAI_FIFOErrorInterruptEnable);
    EnableIRQ(DEMO_SAI_IRQ_TX);
}


/*Initialize audio interface and codec.*/
void Init_Board_Audio(void)
{
    printf("Init Audio SAI and CODEC\r\n");

    BOARD_DMA_EDMA_Config();
    BOARD_USB_Audio_TxInit(DEMO_AUDIO_SAMPLING_RATE);
    BOARD_Codec_Init();
    BOARD_DMA_EDMA_Enable_Audio_Interrupts();
    currentFreq = 44100;
    currentBitWidth = 16;
    currentChannelNum = 2;
    (void)currentBitWidth;    /*fix build warning: set but never used.*/
}

void SAI_UserTxIRQHandler(void)
{
    /* Clear the FEF flag */
    SAI_TxClearStatusFlags(BOARD_DEMO_SAI, kSAI_FIFOErrorFlag);
    SAI_TxSoftwareReset(BOARD_DEMO_SAI, kSAI_ResetTypeFIFO);
    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
        exception return operation might vector to incorrect interrupt */
    __DSB();
}

static void BOARD_SwitchAudioFreq()
{
#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7))
                 /*Clock setting for LPI2C*/
                 CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
                 CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

                 /*Clock setting for SAI1*/
                 CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
                 CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
                 CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

                 /* Enable MCLK output */
                 IOMUXC_GPR->GPR1 |= IOMUXC_GPR_GPR1_SAI1_MCLK_DIR_MASK;
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
                 /*Clock setting for LPI2C*/
                 CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c5, 1);

                 /*Clock setting for SAI1*/
                 CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, DEMO_SAI1_CLOCK_SOURCE_SELECT);
                 CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);

                 /* Enable MCLK output */
                 IOMUXC_GPR->GPR0 |= IOMUXC_GPR_GPR0_SAI1_MCLK_DIR_MASK;
#endif
}

static void SAI_Codec_Start(UCHAR ep)
{
   /*initialize hardware for the first time*/
   if ((0 == currentFreq) && (A2DP_EP_SINK == ep))
   {
       CLOCK_DeinitAudioPll();
       CLOCK_InitAudioPll(&audioPllConfig);

#if (defined(CPU_MIMXRT1062DVMAA_cm7) || (CPU_MIMXRT1062DVL6A_cm7)) 
       /*Clock setting for LPI2C*/
       CLOCK_SetMux(kCLOCK_Lpi2cMux, DEMO_LPI2C_CLOCK_SOURCE_SELECT);
       CLOCK_SetDiv(kCLOCK_Lpi2cDiv, DEMO_LPI2C_CLOCK_SOURCE_DIVIDER);

       /*Clock setting for SAI1*/
       CLOCK_SetMux(kCLOCK_Sai1Mux, DEMO_SAI1_CLOCK_SOURCE_SELECT);
       CLOCK_SetDiv(kCLOCK_Sai1PreDiv, DEMO_SAI1_CLOCK_SOURCE_PRE_DIVIDER);
       CLOCK_SetDiv(kCLOCK_Sai1Div, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
#elif defined(CPU_MIMXRT1176DVMAA_cm7)
       /*Clock setting for LPI2C*/
       CLOCK_SetRootClockMux(kCLOCK_Root_Lpi2c5, 1);

       /*Clock setting for SAI1*/
       CLOCK_SetRootClockMux(kCLOCK_Root_Sai1, DEMO_SAI1_CLOCK_SOURCE_SELECT);
       CLOCK_SetRootClockDiv(kCLOCK_Root_Sai1, DEMO_SAI1_CLOCK_SOURCE_DIVIDER);
#endif
       /*Enable MCLK clock*/
       BOARD_EnableSaiMclkOutput(true);
       Init_Board_Audio();

       CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
       CODEC_SetFormat(&codecHandle, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
       CODEC_SetVolume(&codecHandle, kCODEC_VolumeDAC, 100);
       CODEC_SetVolume(&codecHandle, kCODEC_VolumeHeadphoneLeft | kCODEC_VolumeHeadphoneRight, 100);
       CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, false);
  }
#ifdef A2DP_SINK
  (void)a2dp_snk_size;    /*fix build warning: set but never used.*/
  if (A2DP_EP_SINK == ep)
  {
       if((a2dp_snk_sf == currentFreq) && (a2dp_snk_nc == currentChannelNum))
       {
           /*nothing to do, default setting is ok*/
         printf("current SAI and CODEC setting is ok\r\n");
       }
       else
       {
         printf("***change setting**********\r\n");
         SAI_TransferTerminateSendEDMA(BOARD_DEMO_SAI, &txHandle);
         /*re-configure sai, codec, if current frequency is not multiple of new setting, audio pll should be re-configure*/

         /*new setting is 44.1khz*/
         if(44100 == a2dp_snk_sf)
         {
             if(kSAI_SampleRate44100Hz == currentFreq)
             {
                 SAI_TxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);
                 SAI_RxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);

                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
                 CODEC_SetFormat(&codecHandle, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, false);
             }
             else
             {
                 printf("switch to 44.1 \r\n");
                 //masterClockHz =  11289600U;
                 /* set bit clock divider */
                 SAI_TxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);
                 SAI_RxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);


                 CLOCK_DeinitAudioPll();
                 CLOCK_InitAudioPll(&audioPllConfig);

                 BOARD_SwitchAudioFreq();

                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
                 CODEC_SetFormat(&codecHandle, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, false);

             }
         }
         else
         {
           /*new setting is multiple of 8khz*/

           /*current frequency is multiple of 8khz*/
             if(0U == currentFreq%8000)
             {
                 SAI_TxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);
                 SAI_RxSetBitClockRate(BOARD_DEMO_SAI, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);
                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
                 CODEC_SetFormat(&codecHandle, masterClockHz, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, false);
             }
             else
             {
                 /*current frequency is multiple of 44.1Khz and need change to mutiplu of 8K*/
                 //masterClockHz =  12288000;
                 printf("switch to 48khz");
                 /* set bit clock divider */
                 SAI_TxSetBitClockRate(BOARD_DEMO_SAI, DEMO_SAI_CLK_FREQ, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);
                 SAI_RxSetBitClockRate(BOARD_DEMO_SAI, DEMO_SAI_CLK_FREQ, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH,
                     a2dp_snk_nc);


                 CLOCK_DeinitAudioPll();
                 CLOCK_InitAudioPll(&audioPllConfig1);

                 BOARD_SwitchAudioFreq();

                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
                 CODEC_SetFormat(&codecHandle, DEMO_SAI_CLK_FREQ, a2dp_snk_sf, DEMO_AUDIO_BIT_WIDTH);
                 CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, false);
             }
         }
      }
   }
#endif /* A2DP_SINK */
}

static void SAI_Codec_Stop(UCHAR ep)
{
  currentFreq = 0;
  printf("eStop SAI and CODEC\r\n");
  BOARD_EnableSaiMclkOutput(false);
  SAI_Deinit(BOARD_DEMO_SAI);
  DisableIRQ(SAI1_IRQn);
}

void a2dp_init_pl_ext (UCHAR role)
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == role)
    {
        a2dp_init_default_src_pl();
    }
#endif /* A2DP_SOURCE */
}

void a2dp_shutdown_pl_ext ()
{
#ifdef A2DP_SINK
	if(audiosetup == 1)
	{
	    audiosetup = 0;
	    audioStart = 0;
	    /* Stop Audio Player */
	    CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
	    SAI_TransferTerminateSendEDMA(BOARD_DEMO_SAI, &txHandle);
	    SAI_Codec_Stop(A2DP_EP_SINK);
	}
#endif /* A2DP_SINK */
}

void a2dp_setup_pl_ext
     (
         UCHAR ep,
         void (* ep_cb)(const UCHAR *data, UINT16 datalen),
         UINT16 sf,
         UCHAR bps,
         UCHAR nc,
         UINT16 size
     )
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        a2dp_src_cb = ep_cb;
        a2dp_src_sf = sf;
        a2dp_src_bps = bps;
        a2dp_src_nc = nc;
        a2dp_src_size = size;
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        a2dp_snk_cb = ep_cb;
        a2dp_snk_sf = sf;
        a2dp_snk_bps = bps;
        a2dp_snk_nc = nc;
        a2dp_snk_size = size;

        /* Configuring CODEC when received configuration request*/
        SAI_Codec_Start(ep);
        audiosetup = 1;
    }

#endif /* A2DP_SINK */
}
void a2dp_start_pl_ext (UCHAR ep)
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        /* Start Audio Source */
        a2dp_src_timer = BT_TIMER_HANDLE_INIT_VAL;
        a2dp_pl_start_playback_timer();
        a2dp_src_playback = BT_TRUE;
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        /* Start Audio Player */
        //SAI_Codec_Start(ep);
        audioStart = 1;
        //CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, false);
    }
#endif /* A2DP_SINK */
}
void a2dp_stop_pl_ext (UCHAR ep)
{
#ifdef A2DP_SOURCE
    if (A2DP_EP_SOURCE == ep)
    {
        /* Stop Audio Source */
        a2dp_src_playback = BT_FALSE;
        BT_stop_timer(a2dp_src_timer);
        a2dp_src_timer = BT_TIMER_HANDLE_INIT_VAL;
    }
#endif /* A2DP_SOURCE */

#ifdef A2DP_SINK
    if (A2DP_EP_SINK == ep)
    {
        audioStart = 0;
        /* Stop Audio Player */
        //CODEC_SetMute(&codecHandle, kCODEC_PlayChannelHeadphoneRight |kCODEC_PlayChannelHeadphoneLeft, true);
        SAI_TransferTerminateSendEDMA(BOARD_DEMO_SAI, &txHandle);
        //SAI_Codec_Stop(ep);
    }
#endif /* A2DP_SINK */
}
void a2dp_write_pl_ext (UCHAR ep, UCHAR * m_data, UINT16 m_datalen)
{
#ifdef A2DP_SINK
    if (A2DP_EP_SINK != ep)
    {
        return;
    }
    if(0 == audioStart)
    {
        /*return;*/
    }
    sai_transfer_t xfer = {0};

    xfer.dataSize = m_datalen;
    xfer.data     = m_data;
#if 0
    uint32_t sr;
    OSA_EnterCritical(&sr);
    Finished++;
    OSA_ExitCritical(sr);
#endif

    if(kStatus_Success != SAI_TransferSendEDMA(BOARD_DEMO_SAI, &txHandle, &xfer))
    {
#if 0
        OSA_EnterCritical(&sr);
        Finished--;
        OSA_ExitCritical(sr);
        PRINTF("prime %d\r\n", Finished);
#endif
    }

    /* Write to player */
#endif /* A2DP_SINK */
}
