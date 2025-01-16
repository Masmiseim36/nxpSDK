/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*${header:start}*/
#include "board.h"
#include "app.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_codec_common.h"
#include "fsl_codec_adapter.h"
#include "fsl_power.h"
#include "fsl_sai.h"
#include "fsl_debug_console.h"
#include "fsl_dsp.h"
#include "fsl_cache.h"
#include "core1_support.h"
/*${header:end}*/

/*${variable:start}*/
codec_handle_t g_codecHandle;
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
                     .sampleRate = kWM8962_AudioSampleRate48KHz,
                     .bitWidth   = kWM8962_AudioBitWidth16bit},
    .masterSlave  = false,
};
codec_config_t g_boardCodecConfig = {.codecDevType = kCODEC_WM8962, .codecDevConfig = &wm8962Config};

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    dsp_copy_image_t vector_image;
    dsp_copy_image_t text_image;
    dsp_copy_image_t data_image;

    vector_image.destAddr = HIFI1_BOOT_ADDRESS;
    text_image.destAddr   = HIFI1_TEXT_ADDRESS;
    data_image.destAddr   = HIFI1_DATA_ADDRESS;

#if defined(__CC_ARM)
    size = (uint32_t)&Image$$DSP_REGION$$Length;
#elif defined(__ICCARM__)
#pragma section = "__hifi1_vector_section"
    vector_image.srcAddr  = HIFI1_IMAGE_VECTOR_START;
    vector_image.size     = HIFI1_IMAGE_VECTOR_SIZE;
#pragma section = "__hifi1_text_section"
    text_image.srcAddr    = HIFI1_IMAGE_TEXT_START;
    text_image.size       = HIFI1_IMAGE_TEXT_SIZE;
#pragma section = "__hifi1_data_section"
    data_image.srcAddr    = HIFI1_IMAGE_DATA_START;
    data_image.size       = HIFI1_IMAGE_DATA_SIZE;
#elif defined(__GNUC__)
    vector_image.srcAddr  = HIFI1_IMAGE_VECTOR_START;
    vector_image.size     = HIFI1_IMAGE_VECTOR_SIZE;
    text_image.srcAddr    = HIFI1_IMAGE_TEXT_START;
    text_image.size       = HIFI1_IMAGE_TEXT_SIZE;
    data_image.srcAddr    = HIFI1_IMAGE_DATA_START;
    data_image.size       = HIFI1_IMAGE_DATA_SIZE;
#endif

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    BOARD_InitAHBSC();

    /* Set Hifi4 as Secure privileged master */
    GlikeyWriteEnable(GLIKEY0, 6U);
    AHBSC0->MASTER_SEC_LEVEL = 0x3;
    AHBSC0->MASTER_SEC_ANTI_POL_REG = 0xFFC;

    CLOCK_EnableClock(kCLOCK_InputMux);
    /* Clear SEMA42 reset */
    RESET_PeripheralReset(kSEMA424_RST_SHIFT_RSTn);

    /* Clear MU4 reset before run DSP core */
    RESET_PeripheralReset(kMU4_RST_SHIFT_RSTn);

    /*Clock setting for LPI2C */
    CLOCK_AttachClk(kFCCLK0_to_FLEXCOMM2);

    POWER_DisablePD(kPDRUNCFG_APD_DMA0_1_PKC_ETF);
    POWER_DisablePD(kPDRUNCFG_PPD_DMA0_1_PKC_ETF);
    POWER_ApplyPD();

    /* SAI clock 368.64 / 15 = 24.576MHz */
    CLOCK_AttachClk(kAUDIO_PLL_PFD3_to_AUDIO_VDD2);
    CLOCK_AttachClk(kAUDIO_VDD2_to_SAI012);
    CLOCK_SetClkDiv(kCLOCK_DivSai012Clk, 15U);

    SYSCON0->SAI0_MCLK_CTRL |= SYSCON0_SAI0_MCLK_CTRL_SAIMCLKDIR_MASK;

    /* SEMA42 init */
    SEMA42_Init(APP_SEMA42);
    /* Reset the sema42 gate */
    SEMA42_ResetAllGates(APP_SEMA42);

#ifdef CORE1_IMAGE_COPY_TO_RAM
    BOARD_CopyCore1Image(CORE1_BOOT_ADDRESS);
#endif
    /* Copy HiFi1 binaries to HiFi1 RAM*/
    DSP_CopyImage(&vector_image);
    DSP_CopyImage(&text_image);
    DSP_CopyImage(&data_image);
    XCACHE_CleanInvalidateCacheByRange((uint32_t)data_image.destAddr, data_image.size);
    BOARD_ReleaseCore1Power();
    BOARD_BootCore1(CORE1_BOOT_ADDRESS, CORE1_BOOT_ADDRESS);

}

int BOARD_CODEC_Init(void)
{
    PRINTF("[CM33 Main] Configure codec\r\n");

    if (CODEC_Init(&g_codecHandle, &g_boardCodecConfig) != kStatus_Success)
    {
        PRINTF("[CM33 Main] Codec failed!\r\n");
        return -1;
    }

    if (CODEC_SetVolume(&g_codecHandle, kCODEC_PlayChannelHeadphoneLeft | kCODEC_PlayChannelHeadphoneRight,
                        DEMO_CODEC_VOLUME) != kStatus_Success)
    {
    	PRINTF("[CM33 Main] Set volume failed!\r\n");
        return -1;
    }

    return 0;
}

void BOARD_MuteRightChannel(bool mute)
{
    /* The CODEC_SetMute() funtion sets the volume to 100 after unmuting */
    CODEC_SetVolume(&g_codecHandle, kCODEC_PlayChannelHeadphoneRight, mute ? 0 : DEMO_CODEC_VOLUME);
}
/*${function:end}*/
