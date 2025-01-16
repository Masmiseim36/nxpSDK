/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "board.h"
#include "clock_config.h"
#include "fsl_adapter_uart.h"

#include "fsl_sai.h"
#include "fsl_wm8962.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_edma_soc.h"
#include "fsl_adapter_audio.h"

#include "controller_hci_uart.h"
#include "usb_host_config.h"
#include "usb_phy.h"
#include "usb_host.h"
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
#include "fsl_cache.h"
#endif /* CONFIG_BT_SMP */
#include "fsl_ctimer.h"
/*${header:end}*/

/*${macro:start}*/

/* GPT - SyncTimer */
#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#define SyncTimer_CTIMER                (CTIMER0)
#define SyncTimer_CTIMER_Irq            (CTIMER0_IRQn)
#define SyncTimer_CTIMER_ClockRoot      (kCLOCK_Root_CTIMER0)
#define SyncTimer_CTIMER_ClockRoot_Mux  (kCLOCK_CTIMER0_ClockRoot_MuxAudioPllOut)
#define SyncTimer_CTIMER_ClockRoot_Div  (16)
#endif
#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
static volatile uint32_t SyncTimer_Trigger_Counter; /* This used to counter the sync trigger signal. */
static void (*SyncTimer_Trigger_Callback)(uint32_t sync_index, uint64_t bclk_count) = NULL;
static uint32_t SyncTimer_Capture1_Value = 0;
static uint32_t SyncTimer_Pre_Capture2_Value = 0;
static uint64_t SyncTimer_Count_Value = 0;
static uint32_t SyncTimer_Count_To_Bclk_Div = 0;
static uint64_t SyncTimer_Bclk_Value = 0;
#endif

#define DEMO_SAI                        SAI0
#define DEMO_SAI_CHANNEL                (0)
#define DEMO_SAI_CLOCK_SOURCE_DIVIDER   (15U)
#define DEMO_SAI_CLK_FREQ               CLOCK_GetSaiClkFreq()

#define DEMO_I2C_CLK_FREQ               (24000000U) /* CLOCK_GetLPI2cClkFreq(2) */

#define DEMO_AUDIO_SAMPLING_RATE        kHAL_AudioSampleRate44100Hz
#define DEMO_AUDIO_DATA_CHANNEL         kHAL_AudioStereo
#define DEMO_AUDIO_BIT_WIDTH            kSAI_WordWidth16bits
#define DEMO_AUDIO_INSTANCE             (0U)

#define DEMO_DMA                        DMA0
#define EXAMPLE_DMAMUX_INSTANCE         (0U)
#define EXAMPLE_DMA_INSTANCE            (0U)
#define EXAMPLE_TX_CHANNEL              (0U)
#define EXAMPLE_RX_CHANNEL              (1U)
#define EXAMPLE_SAI_TX_SOURCE           kDmaRequestMuxSai0Tx
#define EXAMPLE_SAI_RX_SOURCE           kDmaRequestMuxSai0Rx

/* demo audio data channel */
#define EXAMPLE_CODEC_RX_CHANNEL (kHAL_AudioMonoRight)
#define EXAMPLE_CODEC_TX_CHANNEL (kHAL_AudioMonoLeft)

#if defined(__GIC_PRIO_BITS)
#define USB_HOST_INTERRUPT_PRIORITY (25U)
#elif defined(__NVIC_PRIO_BITS) && (__NVIC_PRIO_BITS >= 3)
#define USB_HOST_INTERRUPT_PRIORITY     (6U)
#else
#define USB_HOST_INTERRUPT_PRIORITY     (3U)
#endif
/*${macro:end}*/

/*${variable:start}*/
/* set boardCodecConfig */
wm8962_config_t wm8962Config = {
    .i2cConfig = {.codecI2CInstance = BOARD_CODEC_I2C_INSTANCE, .codecI2CSourceClock = DEMO_I2C_CLK_FREQ},
    .route =
        {
            .enableLoopBack            = false,
            .leftInputPGASource        = kWM8962_InputPGASourceInput1,
            .leftInputMixerSource      = kWM8962_InputMixerSourceInputPGA,
            .rightInputPGASource       = kWM8962_InputPGASourceInput3,
            .rightInputMixerSource     = kWM8962_InputMixerSourceInputPGA,
            .leftHeadphoneMixerSource  = kWM8962_OutputMixerDisabled,
            .leftHeadphonePGASource    = kWM8962_OutputPGASourceDAC,
            .rightHeadphoneMixerSource = kWM8962_OutputMixerDisabled,
            .rightHeadphonePGASource   = kWM8962_OutputPGASourceDAC,
        },
    .slaveAddress = WM8962_I2C_ADDR,
    .bus          = kWM8962_BusI2S,
    .format       = {.mclk_HZ    = 24576000U,
                     .sampleRate = 8000,
                     .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};

codec_config_t boardCodecScoConfig1 = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};

/* set audioTxConfig */
hal_audio_dma_channel_mux_config_t audioTxDmaMuxConfig = {
    .dmaChannelMuxConfig.dmaRequestSource = EXAMPLE_SAI_TX_SOURCE,
};

hal_audio_dma_config_t audioTxDmaConfig = {
    .instance             = EXAMPLE_DMA_INSTANCE,
    .channel              = EXAMPLE_TX_CHANNEL,
    .enablePreemption     = false,
    .enablePreemptAbility = false,
    .priority             = kHAL_AudioDmaChannelPriorityDefault,
    .dmaMuxConfig         = NULL,
    .dmaChannelMuxConfig  = (void *)&audioTxDmaMuxConfig,
};

hal_audio_ip_config_t audioTxIpConfig = {
    .sai.lineMask = 1U << DEMO_SAI_CHANNEL,
    .sai.syncMode = kHAL_AudioSaiModeAsync,
};

hal_audio_config_t codecTxConfig = {
    .dmaConfig         = &audioTxDmaConfig,
    .ipConfig          = (void *)&audioTxIpConfig,
    .srcClock_Hz       = 24576000U,
    .sampleRate_Hz     = (uint32_t)DEMO_AUDIO_SAMPLING_RATE,
    .fifoWatermark     = FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) / 2,
    .masterSlave       = kHAL_AudioMaster,
    .bclkPolarity      = kHAL_AudioSampleOnRisingEdge,
    .frameSyncWidth    = kHAL_AudioFrameSyncWidthHalfFrame,
    .frameSyncPolarity = kHAL_AudioBeginAtRisingEdge,
    .lineChannels      = EXAMPLE_CODEC_TX_CHANNEL,
    .dataFormat        = kHAL_AudioDataFormatI2sClassic,
    .bitWidth          = (uint8_t)DEMO_AUDIO_BIT_WIDTH,
    .instance          = DEMO_AUDIO_INSTANCE,
};

/* set audioRxConfig */
hal_audio_dma_channel_mux_config_t audioRxDmaMuxConfig = {
    .dmaChannelMuxConfig.dmaRequestSource = EXAMPLE_SAI_RX_SOURCE,
};

hal_audio_dma_config_t audioRxDmaConfig = {
    .instance             = EXAMPLE_DMA_INSTANCE,
    .channel              = EXAMPLE_RX_CHANNEL,
    .enablePreemption     = false,
    .enablePreemptAbility = false,
    .priority             = kHAL_AudioDmaChannelPriorityDefault,
    .dmaMuxConfig         = NULL,
    .dmaChannelMuxConfig  = (void *)&audioRxDmaMuxConfig,
};

hal_audio_ip_config_t audioRxIpConfig = {
    .sai.lineMask = 1U << DEMO_SAI_CHANNEL,
    .sai.syncMode = kHAL_AudioSaiModeSync,
};

hal_audio_config_t codecRxConfig = {
    .dmaConfig         = &audioRxDmaConfig,
    .ipConfig          = (void *)&audioRxIpConfig,
    .srcClock_Hz       = 24576000U,
    .sampleRate_Hz     = (uint32_t)DEMO_AUDIO_SAMPLING_RATE,
    .fifoWatermark     = FSL_FEATURE_SAI_FIFO_COUNTn(DEMO_SAI) / 2,
    .masterSlave       = kHAL_AudioMaster,
    .bclkPolarity      = kHAL_AudioSampleOnRisingEdge,
    .frameSyncWidth    = kHAL_AudioFrameSyncWidthHalfFrame,
    .frameSyncPolarity = kHAL_AudioBeginAtRisingEdge,
    .lineChannels      = EXAMPLE_CODEC_RX_CHANNEL,
    .dataFormat        = kHAL_AudioDataFormatI2sClassic,
    .bitWidth          = (uint8_t)DEMO_AUDIO_BIT_WIDTH,
    .instance          = DEMO_AUDIO_INSTANCE,
};

/* set mclkConfig */
sai_master_clock_t mclkConfig = {
    .mclkOutputEnable  = true,
    .mclkHz            = 24576000U,
    .mclkSourceClkHz   = 24576000U,
};

/*
 * AUDIO PLL setting: Frequency = Fref * (MULT + NUM / DENOM)
 *                              = 24 * (20 + 5040/13125)
 *                              = 489.216MHZ
 */
/*setting for 44.1KHZ*/
const clock_audio_pll_config_t audioPllConfig = {
    .audio_pll_src  = kCLOCK_AudioPllOscClk, /* OSC clock */
    .numerator      = 5040,                  /* Numerator of the Audio PLL fractional loop divider is 5040 */
    .denominator    = 13125,                 /* Denominator of the Audio PLL fractional loop divider is 13125 */
    .audio_pll_mult = kCLOCK_AudioPllMult20  /* Divide by 20 */
};

/*
 * AUDIO PLL setting: Frequency = Fref * (MULT + NUM / DENOM)
 *                              = 24 * (22 + 5040/27000)
 *                              = 532.48MHZ
 */
/*setting for multiple of 8KHZ,such as 48KHZ/16KHZ/32KHZ*/
const clock_audio_pll_config_t audioPllConfig1 = {
    .audio_pll_src  = kCLOCK_AudioPllOscClk, /* OSC clock */
    .numerator      = 5040,                  /* Numerator of the Audio PLL fractional loop divider is 5040 */
    .denominator    = 27000,                 /* Denominator of the Audio PLL fractional loop divider is 27000 */
    .audio_pll_mult = kCLOCK_AudioPllMult22  /* Divide by 22 */
};
/*${variable:end}*/

/*${function:start}*/
void BOARD_Init_M2(void)
{
    CLOCK_EnableClock(kCLOCK_Gpio3);
    CLOCK_EnableClock(kCLOCK_Gpio7);
    RESET_PeripheralReset(kGPIO3_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO7_RST_SHIFT_RSTn);
}

void BOARD_Init_BT_UART(void)
{
    CLOCK_AttachClk(BOARD_BT_UART_FCCLK_ATTACH);
    CLOCK_SetClkDiv(BOARD_BT_UART_FCCLK_DIV, 1U);

    /* Attach FC1 clock to LP_FLEXCOMM3(HCI uart). */
    CLOCK_AttachClk(BOARD_BT_UART_CLK_ATTACH);
}

void BOARD_Init_TRNG(void)
{
    GlikeyWriteEnable(GLIKEY3, 1U);                                    /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */

    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                               /* Max 96MHZ with 1.0V nomral supply. */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);
}

void BOARD_Init_I2C(void)
{
    /*Clock setting for LPI2C */
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM2);
}

void BOARD_Init_EDMA(void)
{
    EDMA_EnableRequest(DEMO_DMA, EXAMPLE_SAI_TX_SOURCE);
    EDMA_EnableRequest(DEMO_DMA, EXAMPLE_SAI_RX_SOURCE);
}

uint32_t BOARD_SwitchAudioFreq(uint32_t sampleRate)
{
    /* The AUDIOPLL0NUM and AUDIOPLL0DENOM register can only be changed when the Audio PLL0 is disabled */
    CLOCK_DeinitAudioPll();

    if (0U == sampleRate)
    {
        /* Disable MCLK output */
        SYSCON0->SAI0_MCLK_CTRL &= (~SYSCON0_SAI0_MCLK_CTRL_SAIMCLKDIR_MASK);
    }
    else
    {
        /* Config Audio PLL */
        if (44100 == sampleRate)
        {
            /* 489.216MHZ */
            CLOCK_InitAudioPll(&audioPllConfig);
        }
        else if (0U == sampleRate % 8000)
        {
            /* 532.48MHZ */
            CLOCK_InitAudioPll(&audioPllConfig1);
        }
        else
        {
            /* no action */
        }

        /* SAI clock = audio_pll_pfd3 / 15 */
        CLOCK_AttachClk(kAUDIO_PLL_PFD3_to_AUDIO_VDD2);
        CLOCK_AttachClk(kAUDIO_VDD2_to_SAI012);
        CLOCK_SetClkDiv(kCLOCK_DivSai012Clk, DEMO_SAI_CLOCK_SOURCE_DIVIDER);

        CLOCK_EnableClock(kCLOCK_Sai0);
        RESET_ClearPeripheralReset(kSAI0_RST_SHIFT_RSTn);

        /* Enable MCLK output */
        SYSCON0->SAI0_MCLK_CTRL |= SYSCON0_SAI0_MCLK_CTRL_SAIMCLKDIR_MASK;
        SAI_SetMasterClockConfig(DEMO_SAI, &mclkConfig);
    }
    wm8962Config.format.sampleRate             = sampleRate;
    wm8962Config.format.mclk_HZ                = DEMO_SAI_CLK_FREQ;

    return DEMO_SAI_CLK_FREQ;
}

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
static void ctimer_callback(uint32_t flags);

static ctimer_callback_t ctimer_callbacks[] = { ctimer_callback };

void BOARD_SyncTimer_Init(void (*sync_timer_callback)(uint32_t sync_index, uint64_t bclk_count))
{
    ctimer_config_t config;

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
    /* 2EL use CTIMER as SyncTimer. */
    BOARD_InitCTIMERPins();
#endif
    /* Set SyncTimer clock root to Audio_PLL. */
    CLOCK_AttachClk(kAUDIO_VDD2_to_CTIMER0);
    CLOCK_SetClkDiv(kCLOCK_DivCtimer0Clk, 15U);

    CTIMER_GetDefaultConfig(&config);
    config.mode = kCTIMER_TimerMode;
    config.input = kCTIMER_Capture_0;
    config.prescale = 0;
    CTIMER_Init(SyncTimer_CTIMER, &config);
    
    CTIMER_RegisterCallBack(SyncTimer_CTIMER, ctimer_callbacks, kCTIMER_SingleCallback);

    /* Input Capture 1 capture signal come from SAI FS signal, in order to capture offset between BCLK and FS. */
    CTIMER_SetupCapture(SyncTimer_CTIMER, kCTIMER_Capture_0, kCTIMER_Capture_RiseEdge, true);
    /* Input Capture 2 capture signal come from Sync INT signal, on order to measure CLK draft. */
    CTIMER_SetupCapture(SyncTimer_CTIMER, kCTIMER_Capture_1, kCTIMER_Capture_RiseEdge, true);

    EnableIRQWithPriority(SyncTimer_CTIMER_Irq, 3);

    SyncTimer_Trigger_Callback = sync_timer_callback;
}

void BORAD_SyncTimer_Start(uint32_t sample_rate, uint32_t bits_per_sample)
{
    /* Reset all parameter values. */
    SyncTimer_Trigger_Counter    = 0;
    SyncTimer_Capture1_Value     = 0;
    SyncTimer_Pre_Capture2_Value = 0;
    SyncTimer_Count_Value        = 0;
    SyncTimer_Count_To_Bclk_Div  = CLOCK_GetCTimerClkFreq(0) / (sample_rate * bits_per_sample);
    SyncTimer_Bclk_Value         = 0;

    CTIMER_ClearStatusFlags(SyncTimer_CTIMER, kCTIMER_Capture0Flag | kCTIMER_Capture0Flag);
    CTIMER_EnableInterrupts(SyncTimer_CTIMER, kCTIMER_Capture0InterruptEnable | kCTIMER_Capture1InterruptEnable);
    CTIMER_StartTimer(SyncTimer_CTIMER);
}

void BORAD_SyncTimer_Stop(void)
{
    CTIMER_StopTimer(SyncTimer_CTIMER);
}

static void ctimer_callback(uint32_t flags)
{
    uint32_t current_capture2_value;
    uint32_t capture2_value_delta;

    if(flags & kCTIMER_Capture0Flag)
    {
        /* We only need capture the first SAI FS signal to calculate offset. */
        CTIMER_DisableInterrupts(SyncTimer_CTIMER, kCTIMER_Capture0InterruptEnable);
        /* SAI FS trigged BCLK offset. */
        SyncTimer_Capture1_Value = CTIMER_GetCaptureValue(SyncTimer_CTIMER, kCTIMER_Capture_0);
    }
    if(flags & kCTIMER_Capture1Flag)
    {
        /* Sync INT trigged BCLK count capture. */
        current_capture2_value = CTIMER_GetCaptureValue(SyncTimer_CTIMER, kCTIMER_Capture_1);
        /* Fix GPT overflow issue. */
        if (current_capture2_value < SyncTimer_Pre_Capture2_Value)
        {
            capture2_value_delta = (uint64_t)0x100000000 + current_capture2_value - SyncTimer_Pre_Capture2_Value;
        }
        else
        {
            capture2_value_delta = current_capture2_value - SyncTimer_Pre_Capture2_Value;
        }
        SyncTimer_Pre_Capture2_Value = current_capture2_value;
        SyncTimer_Count_Value += capture2_value_delta;

        /* Calculate BCLK. */
        /* BCLK should be 0 before the SAI_SW start, because we use AUDIO_PLL as clk source. */
        if(SyncTimer_Capture1_Value > 0U)
        {
            SyncTimer_Bclk_Value = (SyncTimer_Count_Value - SyncTimer_Capture1_Value) / SyncTimer_Count_To_Bclk_Div;
        }
        else
        {
            SyncTimer_Bclk_Value = 0;
        }
        /* invoke callback. */
        if(SyncTimer_Trigger_Callback)
        {
            SyncTimer_Trigger_Callback(SyncTimer_Trigger_Counter, SyncTimer_Bclk_Value);
        }

        /* SyncTimer trigger signal count. */
        SyncTimer_Trigger_Counter ++;
    }

    SDK_ISR_EXIT_BARRIER;
}
#endif /* WIFI_IW612_BOARD_MURATA_2EL_M2 */


void  BOARD_InitHardware(void)
{
    BOARD_Init_M2();

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    //BOARD_InitAHBSC();
    BOARD_Init_BT_UART();
    BOARD_Init_I2C();
    BOARD_Init_EDMA();
#if (((defined(CONFIG_BT_SMP)) && (CONFIG_BT_SMP)))
    GlikeyWriteEnable(GLIKEY3, 1U);                                    /* Enable SYSCON0_SEC_CLK_CTRL write */
    SYSCON0->SEC_CLK_CTRL |= SYSCON0_SEC_CLK_CTRL_TRNG_REFCLK_EN_MASK; /* Enable TRNG REF CLOCK */

    CLOCK_AttachClk(kFRO1_DIV2_to_TRNG);                               /* Max 96MHZ with 1.0V nomral supply. */
    CLOCK_SetClkDiv(kCLOCK_DivTrngClk, 1U);
#endif /* CONFIG_BT_SMP */
}

#if (defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2)  || \
      defined(WIFI_IW612_BOARD_MURATA_2EL_M2))      
int controller_hci_uart_get_configuration(controller_hci_uart_config_t *config)
{
    if (NULL == config)
    {
        return -1;
    }
    config->clockSrc        = BOARD_BT_UART_CLK_FREQ;
    config->defaultBaudrate = 115200u;
    config->runningBaudrate = BOARD_BT_UART_BAUDRATE;
    config->instance        = BOARD_BT_UART_INSTANCE;
    config->enableRxRTS = 1u;
    config->enableTxCTS = 1u;
    return 0;
}
#endif

void USB_HostClockInit(void)
{
    uint32_t usbClockFreq = 24000000;
    usb_phy_config_struct_t phyConfig = {
        BOARD_USB_PHY_D_CAL,
        BOARD_USB_PHY_TXCAL45DP,
        BOARD_USB_PHY_TXCAL45DM,
    };

    /* Power on COM VDDN domain for USB or eUSB */
    POWER_DisablePD(kPDRUNCFG_DSR_VDDN_COM);    
        
    if (CONTROLLER_ID == kUSB_ControllerEhci0)
    { 
        /* Power on usb ram araay as need, powered USB0RAM array*/
        POWER_DisablePD(kPDRUNCFG_APD_USB0_SRAM);
        POWER_DisablePD(kPDRUNCFG_PPD_USB0_SRAM);
        /* Apply the config */
        POWER_ApplyPD();
        /* disable the read and write gate */
        SYSCON4->USB0_MEM_CTRL |= (SYSCON4_USB0_MEM_CTRL_MEM_WIG_MASK | SYSCON4_USB0_MEM_CTRL_MEM_RIG_MASK |
                                     SYSCON4_USB0_MEM_CTRL_MEM_STDBY_MASK);
        /* Enable the USBPHY0 CLOCK */
        SYSCON4->USBPHY0_CLK_ACTIVE |= SYSCON4_USBPHY0_CLK_ACTIVE_IPG_CLK_ACTIVE_MASK;
        CLOCK_AttachClk(k32KHZ_WAKE_to_USB);
        CLOCK_AttachClk(kOSC_CLK_to_USB_24MHZ);
        CLOCK_EnableClock(kCLOCK_Usb0);
        CLOCK_EnableClock(kCLOCK_UsbphyRef);
        RESET_PeripheralReset(kUSB0_RST_SHIFT_RSTn);
        RESET_PeripheralReset(kUSBPHY0_RST_SHIFT_RSTn);
        CLOCK_EnableUsbhs0PhyPllClock(kCLOCK_Usbphy480M, usbClockFreq);
        CLOCK_EnableUsbhs0Clock(kCLOCK_Usb480M, usbClockFreq);
        USB_EhciPhyInit(CONTROLLER_ID, BOARD_XTAL_SYS_CLK_HZ, &phyConfig);
    }
}

void USB_HostIsrEnable(void)
{
    uint8_t irqNumber;

    uint8_t usbHOSTEhciIrq[] = USBHS_IRQS;
    irqNumber                = usbHOSTEhciIrq[CONTROLLER_ID - kUSB_ControllerEhci0];
/* USB_HOST_CONFIG_EHCI */

/* Install isr, set priority, and enable IRQ. */
#if defined(__GIC_PRIO_BITS)
    GIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#else
    NVIC_SetPriority((IRQn_Type)irqNumber, USB_HOST_INTERRUPT_PRIORITY);
#endif
    EnableIRQ((IRQn_Type)irqNumber);
}

/*${function:end}*/
