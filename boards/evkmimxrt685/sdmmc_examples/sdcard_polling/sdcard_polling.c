/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_sd.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_pca9420.h"
#include "fsl_power.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST

/*! @brief Data block count accessed in card */
#define DATA_BLOCK_COUNT (5U)
/*! @brief Start data block number accessed in card */
#define DATA_BLOCK_START (2U)
/*! @brief Data buffer size. */
#define DATA_BUFFER_SIZE (FSL_SDMMC_DEFAULT_BLOCK_SIZE * DATA_BLOCK_COUNT)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void BOARD_PowerOffSDCARD(void);
void BOARD_PowerOnSDCARD(void);
void BOARD_USDHC_Switch_VoltageTo1V8(void);
void BOARD_USDHC_Switch_VoltageTo3V3(void);
/*!
 * @brief printf the card information log.
 *
 * @param card Card descriptor.
 */
static void CardInformationLog(sd_card_t *card);
/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Card descriptor. */
sd_card_t g_sd;

/* @brief decription about the read/write buffer
 * The size of the read/write buffer should be a multiple of 512, since SDHC/SDXC card uses 512-byte fixed
 * block length and this driver example is enabled with a SDHC/SDXC card.If you are using a SDSC card, you
 * can define the block length by yourself if the card supports partial access.
 * The address of the read/write buffer should align to the specific DMA data buffer address align value if
 * DMA transfer is used, otherwise the buffer address is not important.
 * At the same time buffer address/size should be aligned to the cache line size if cache is supported.
 */
/*! @brief Data written to the card */
SDK_ALIGN(uint8_t g_dataWrite[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));
/*! @brief Data read from the card */
SDK_ALIGN(uint8_t g_dataRead[SDK_SIZEALIGN(DATA_BUFFER_SIZE, SDMMC_DATA_BUFFER_ALIGN_CACHE)],
          MAX(SDMMC_DATA_BUFFER_ALIGN_CACHE, SDMMCHOST_DMA_BUFFER_ADDR_ALIGN));

/*! @brief SDMMC host detect card configuration */
static const sdmmchost_detect_card_t s_sdCardDetect = {
#ifndef BOARD_SD_DETECT_TYPE
    .cdType = kSDMMCHOST_DetectCardByGpioCD,
#else
    .cdType = BOARD_SD_DETECT_TYPE,
#endif
    .cdTimeOut_ms = (~0U),
};

/*! @brief SDMMC card power control configuration */
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
static const sdmmchost_pwr_card_t s_sdCardPwrCtrl = {
    .powerOn          = BOARD_PowerOnSDCARD,
    .powerOnDelay_ms  = 500U,
    .powerOff         = BOARD_PowerOffSDCARD,
    .powerOffDelay_ms = 0U,
};
#endif
#if defined DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
static const sdmmchost_card_switch_voltage_func_t s_sdCardVoltageSwitch = {
    .cardSignalLine1V8 = BOARD_USDHC_Switch_VoltageTo1V8,
    .cardSignalLine3V3 = BOARD_USDHC_Switch_VoltageTo3V3,
};
#endif
/*******************************************************************************
 * Code
 ******************************************************************************/
static pca9420_handle_t pca9420Handle;


void BOARD_USDHC_Switch_VoltageTo1V8(void)
{
    bool result = PCA9420_SwitchMode(&pca9420Handle, kPCA9420_Mode1);
    if (!result)
    {
        assert(false);
    }
}

void BOARD_USDHC_Switch_VoltageTo3V3(void)
{
    bool result = PCA9420_SwitchMode(&pca9420Handle, kPCA9420_Mode0);
    if (!result)
    {
        assert(false);
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
static status_t AccessCard(sd_card_t *card, bool isReadOnly)
{
    if (isReadOnly)
    {
        PRINTF("\r\nRead one data block......\r\n");
        if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
        {
            PRINTF("Read one data block failed.\r\n");
            return kStatus_Fail;
        }

        PRINTF("Read multiple data blocks......\r\n");
        if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
        {
            PRINTF("Read multiple data blocks failed.\r\n");
            return kStatus_Fail;
        }
    }
    else
    {
        memset(g_dataWrite, 0x67U, sizeof(g_dataWrite));

        PRINTF("\r\nWrite/read one data block......\r\n");
        if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, 1U))
        {
            PRINTF("Write one data block failed.\r\n");
            return kStatus_Fail;
        }

        memset(g_dataRead, 0U, sizeof(g_dataRead));
        if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, 1U))
        {
            PRINTF("Read one data block failed.\r\n");
            return kStatus_Fail;
        }

        PRINTF("Compare the read/write content......\r\n");
        if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
        {
            PRINTF("The read/write content isn't consistent.\r\n");
            return kStatus_Fail;
        }
        PRINTF("The read/write content is consistent.\r\n");

        PRINTF("Write/read multiple data blocks......\r\n");
        if (kStatus_Success != SD_WriteBlocks(card, g_dataWrite, DATA_BLOCK_START, DATA_BLOCK_COUNT))
        {
            PRINTF("Write multiple data blocks failed.\r\n");
            return kStatus_Fail;
        }

        memset(g_dataRead, 0U, sizeof(g_dataRead));

        if (kStatus_Success != SD_ReadBlocks(card, g_dataRead, DATA_BLOCK_START, DATA_BLOCK_COUNT))
        {
            PRINTF("Read multiple data blocks failed.\r\n");
            return kStatus_Fail;
        }

        PRINTF("Compare the read/write content......\r\n");
        if (memcmp(g_dataRead, g_dataWrite, FSL_SDMMC_DEFAULT_BLOCK_SIZE))
        {
            PRINTF("The read/write content isn't consistent.\r\n");
            return kStatus_Fail;
        }
        PRINTF("The read/write content is consistent.\r\n");

        PRINTF("Erase multiple data blocks......\r\n");
        if (kStatus_Success != SD_EraseBlocks(card, DATA_BLOCK_START, DATA_BLOCK_COUNT))
        {
            PRINTF("Erase multiple data blocks failed.\r\n");
            return kStatus_Fail;
        }
    }

    return kStatus_Success;
}

/*!
 * @brief Main function
 */
int main(void)
{
    sd_card_t *card = &g_sd;
    char ch         = '0';
    bool isReadOnly;

    pca9420_config_t pca9420Config;
    pca9420_modecfg_t pca9420ModeCfg[2];
    uint32_t i;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /*Make sure USDHC ram buffer has power up*/
    POWER_DisablePD(kPDRUNCFG_APD_USDHC0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PPD_USDHC0_SRAM);
    POWER_DisablePD(kPDRUNCFG_PD_LPOSC);
    POWER_ApplyPD();

    /* PMIC PCA9420 */
    CLOCK_AttachClk(kSFRO_to_FLEXCOMM15);
    BOARD_PMIC_I2C_Init();
    PCA9420_GetDefaultConfig(&pca9420Config);
    pca9420Config.powerGoodEnable = kPCA9420_PGoodDisabled;
    pca9420Config.I2C_SendFunc    = BOARD_PMIC_I2C_Send;
    pca9420Config.I2C_ReceiveFunc = BOARD_PMIC_I2C_Receive;
    PCA9420_Init(&pca9420Handle, &pca9420Config);
    for (i = 0; i < ARRAY_SIZE(pca9420ModeCfg); i++)
    {
        PCA9420_GetDefaultModeConfig(&pca9420ModeCfg[i]);
    }
    pca9420ModeCfg[0].ldo2OutVolt = kPCA9420_Ldo2OutVolt3V300;
    pca9420ModeCfg[1].ldo2OutVolt = kPCA9420_Ldo2OutVolt1V800;
    PCA9420_WriteModeConfigs(&pca9420Handle, kPCA9420_Mode0, &pca9420ModeCfg[0], ARRAY_SIZE(pca9420ModeCfg));

    /* SDIO0 */
    CLOCK_AttachClk(kLPOSC_DIV32_to_32KHZWAKE_CLK);
    CLOCK_AttachClk(kAUX0_PLL_to_SDIO0_CLK);
    CLOCK_SetClkDiv(kCLOCK_DivSdio0Clk, 1);

    card->host.base           = SD_HOST_BASEADDR;
    card->host.sourceClock_Hz = SD_HOST_CLK_FREQ;
    /* card detect type */
    card->usrParam.cd = &s_sdCardDetect;
#if defined DEMO_SDCARD_POWER_CTRL_FUNCTION_EXIST
    card->usrParam.pwr = &s_sdCardPwrCtrl;
#endif
#if defined DEMO_SDCARD_SWITCH_VOLTAGE_FUNCTION_EXIST
    card->usrParam.cardVoltage = &s_sdCardVoltageSwitch;
#endif
    PRINTF("\r\nSDCARD polling example.\r\n");

    /* SD host init function */
    if (SD_HostInit(card) != kStatus_Success)
    {
        PRINTF("\r\nSD host init fail\r\n");
        return -1;
    }

    while (ch != 'q')
    {
        PRINTF("\r\nPlease insert a card into board.\r\n");
        /* power off card */
        SD_PowerOffCard(card->host.base, card->usrParam.pwr);

        if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, true) == kStatus_Success)
        {
            PRINTF("\r\nCard inserted.\r\n");
            /* reset host once card re-plug in */
            SD_HostReset(&(card->host));
            /* power on the card */
            SD_PowerOnCard(card->host.base, card->usrParam.pwr);
        }
        else
        {
            PRINTF("\r\nCard detect fail.\r\n");
            return -1;
        }

        /* Init card. */
        if (SD_CardInit(card))
        {
            PRINTF("\r\nSD card init failed.\r\n");
            return -1;
        }
        /* card information log */
        CardInformationLog(card);

        /* Check if card is readonly. */
        isReadOnly = SD_CheckReadOnly(card);

        PRINTF("\r\nRead/Write/Erase the card continuously until encounter error......\r\n");

        for (;;)
        {
            if (kStatus_Success != AccessCard(card, isReadOnly))
            {
                /* access card fail, due to card remove. */
                if (SD_WaitCardDetectStatus(SD_HOST_BASEADDR, &s_sdCardDetect, false) == kStatus_Success)
                {
                    PRINTF("\r\nCard removed\r\n");
                    PRINTF(
                        "\r\nInput 'q' to quit read/write/erase process.\
                \r\nInput other char to wait card re-insert.\r\n");
                    ch = GETCHAR();
                    PUTCHAR(ch);
                }
                /* access card fail, due to transfer error */
                else
                {
                    ch = 'q';
                }

                break;
            }
            else
            {
                PRINTF(
                    "\r\nInput 'q' to quit read/write/erase process.\
                \r\nInput other char to read/write/erase data blocks again.\r\n");
                ch = GETCHAR();
                PUTCHAR(ch);
                if (ch == 'q')
                {
                    break;
                }
            }
        }
    }

    PRINTF("\r\nThe example will not read/write data blocks again.\r\n");
    SD_Deinit(card);

    while (true)
    {
    }
}

static void CardInformationLog(sd_card_t *card)
{
    assert(card);

    PRINTF("\r\nCard size %d * %d bytes\r\n", card->blockCount, card->blockSize);
    PRINTF("\r\nWorking condition:\r\n");
    if (card->operationVoltage == kCARD_OperationVoltage330V)
    {
        PRINTF("\r\n  Voltage : 3.3V\r\n");
    }
    else if (card->operationVoltage == kCARD_OperationVoltage180V)
    {
        PRINTF("\r\n  Voltage : 1.8V\r\n");
    }

    if (card->currentTiming == kSD_TimingSDR12DefaultMode)
    {
        if (card->operationVoltage == kCARD_OperationVoltage330V)
        {
            PRINTF("\r\n  Timing mode: Default mode\r\n");
        }
        else if (card->operationVoltage == kCARD_OperationVoltage180V)
        {
            PRINTF("\r\n  Timing mode: SDR12 mode\r\n");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR25HighSpeedMode)
    {
        if (card->operationVoltage == kCARD_OperationVoltage180V)
        {
            PRINTF("\r\n  Timing mode: SDR25\r\n");
        }
        else
        {
            PRINTF("\r\n  Timing mode: High Speed\r\n");
        }
    }
    else if (card->currentTiming == kSD_TimingSDR50Mode)
    {
        PRINTF("\r\n  Timing mode: SDR50\r\n");
    }
    else if (card->currentTiming == kSD_TimingSDR104Mode)
    {
        PRINTF("\r\n  Timing mode: SDR104\r\n");
    }
    else if (card->currentTiming == kSD_TimingDDR50Mode)
    {
        PRINTF("\r\n  Timing mode: DDR50\r\n");
    }

    PRINTF("\r\n  Freq : %d HZ\r\n", card->busClock_Hz);
}
