/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_SDMMC_HOST_H
#define _FSL_SDMMC_HOST_H

#include "fsl_common.h"
#include "fsl_sdhc.h"
#include "fsl_sdmmc_osa.h"
/*!
 * @addtogroup SDMMCHOST_SDHC
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief Middleware adapter version. */
#define FSL_SDMMC_HOST_ADAPTER_VERSION (MAKE_VERSION(2U, 3U, 0U)) /*2.3.0*/

/*!@brief host capability */
#define SDMMCHOST_SUPPORT_HIGH_SPEED (1U)
#define SDMMCHOST_SUPPORT_SUSPEND_RESUME (1U)
#define SDMMCHOST_SUPPORT_VOLTAGE_3V3 (1U)
#define SDMMCHOST_SUPPORT_VOLTAGE_3V0 (0U)
#define SDMMCHOST_SUPPORT_VOLTAGE_1V8 (0U)
#define SDMMCHOST_SUPPORT_VOLTAGE_1V2 (0U)
#define SDMMCHOST_SUPPORT_4_BIT_WIDTH (1U)
#define SDMMCHOST_SUPPORT_8_BIT_WIDTH (1U)
#define SDMMCHOST_SUPPORT_DDR50 (0U)
#define SDMMCHOST_SUPPORT_SDR104 (0U)
#define SDMMCHOST_SUPPORT_SDR50 (0U)
#define SDMMCHOST_SUPPORT_HS200 (0U)
#define SDMMCHOST_SUPPORT_HS400 (0U)
#define SDMMCHOST_SUPPORT_DETECT_CARD_BY_DATA3 (1U)
#define SDMMCHOST_SUPPORT_DETECT_CARD_BY_CD (0U)
#define SDMMCHOST_SUPPORT_AUTO_CMD12 (1U)
#define SDMMCHOST_SUPPORT_MAX_BLOCK_LENGTH (4096U)
#define SDMMCHOST_SUPPORT_MAX_BLOCK_COUNT (SDHC_MAX_BLOCK_COUNT)

/*!@brief SDMMC host dma descriptor buffer address align size */
#define SDMMCHOST_DMA_DESCRIPTOR_BUFFER_ALIGN_SIZE (4U)

/*!@brief sdmmc host transfer function */
typedef sdhc_transfer_t sdmmchost_transfer_t;
typedef sdhc_command_t sdmmchost_cmd_t;
typedef sdhc_data_t sdmmchost_data_t;
typedef struct _sdmmchost_ SDMMCHOST_CONFIG;
typedef SDHC_Type SDMMCHOST_TYPE;
typedef void sdmmchost_detect_card_t;
typedef sdhc_boot_config_t sdmmchost_boot_config_t;
/*! @brief host Endian mode
 * corresponding to driver define
 */
enum _sdmmchost_endian_mode
{
    kSDMMCHOST_EndianModeBig         = 0U, /*!< Big endian mode */
    kSDMMCHOST_EndianModeHalfWordBig = 1U, /*!< Half word big endian mode */
    kSDMMCHOST_EndianModeLittle      = 2U, /*!< Little endian mode */
};

/*! @brief card power control function pointer */
typedef void (*sdmmchost_pwr_t)(void);
/*! @brief card power control
 * @deprecated Do not use this structure anymore.
 */
typedef struct _sdmmchost_pwr_card
{
    sdmmchost_pwr_t powerOn;  /*!< power on function pointer */
    uint32_t powerOnDelay_ms; /*!< power on delay */

    sdmmchost_pwr_t powerOff;  /*!< power off function pointer */
    uint32_t powerOffDelay_ms; /*!< power off delay */
} sdmmchost_pwr_card_t;

/*!@brief sdmmc host handler  */
typedef struct _sdmmchost_
{
    sdhc_host_t hostController;    /*!< host configuration */
    void *dmaDesBuffer;            /*!< DMA descriptor buffer address */
    uint32_t dmaDesBufferWordsNum; /*!< DMA descriptor buffer size in byte */
    sdhc_handle_t handle;          /*!< host controller handler */

    void *hostEvent; /*!< host event handler pointer */
    void *cd;        /*!< card detect */
    void *cardInt;   /*!< call back function for card interrupt */
} sdmmchost_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @name SDHC host controller function
 * @{
 */

/*!
 * @brief set data bus width.
 * @param host host handler
 * @param dataBusWidth data bus width
 */
void SDMMCHOST_SetCardBusWidth(sdmmchost_t *host, uint32_t dataBusWidth);

/*!
 * @brief Send initilization active 80 clocks to card.
 * @param host host handler
 */
static inline void SDMMCHOST_SendCardActive(sdmmchost_t *host)
{
    SDHC_SetCardActive(host->hostController.base, 100U);
}

/*!
 * @brief Set card bus clock.
 * @param host host handler
 * @param targetClock target clock frequency
 * @retval actual clock frequency can be reach.
 */
static inline uint32_t SDMMCHOST_SetCardClock(sdmmchost_t *host, uint32_t targetClock)
{
    return SDHC_SetSdClock(host->hostController.base, host->hostController.sourceClock_Hz, targetClock);
}

/*!
 * @brief check card status by DATA0.
 * @param host host handler
 * @retval true is busy, false is idle.
 */
static inline bool SDMMCHOST_IsCardBusy(sdmmchost_t *host)
{
    return (SDHC_GetPresentStatusFlags(host->hostController.base) & kSDHC_Data0LineLevelFlag) ==
                   kSDHC_Data0LineLevelFlag ?
               false :
               true;
}

/*!
 * @brief start read boot data.
 * @param host host handler
 * @param hostConfig boot configuration
 * @param cmd boot command
 * @param buffer buffer address
 */
status_t SDMMCHOST_StartBoot(sdmmchost_t *host,
                             sdmmchost_boot_config_t *hostConfig,
                             sdmmchost_cmd_t *cmd,
                             uint8_t *buffer);

/*!
 * @brief read boot data.
 * @param host host handler
 * @param hostConfig boot configuration
 * @param buffer buffer address
 */
status_t SDMMCHOST_ReadBootData(sdmmchost_t *host, sdmmchost_boot_config_t *hostConfig, uint8_t *buffer);

/*!
 * @brief enable boot mode.
 * @param host host handler
 * @param enable true is enable, false is disable
 */
static inline void SDMMCHOST_EnableBoot(sdmmchost_t *host, bool enable)
{
    /* not support */
}

/*!
 * @brief enable card interrupt.
 * @param host host handler
 * @param enable true is enable, false is disable.
 */
static inline void SDMMCHOST_EnableCardInt(sdmmchost_t *host, bool enable)
{
    if (enable)
    {
        SDHC_EnableInterruptStatus(host->hostController.base, kSDHC_CardInterruptFlag);
        SDHC_EnableInterruptSignal(host->hostController.base, kSDHC_CardInterruptFlag);
    }
    else
    {
        SDHC_DisableInterruptStatus(host->hostController.base, kSDHC_CardInterruptFlag);
        SDHC_DisableInterruptSignal(host->hostController.base, kSDHC_CardInterruptFlag);
    }
}

/*!
 * @brief card interrupt function.
 * @param host host handler
 * @param sdioInt card interrupt configuration
 */
status_t SDMMCHOST_CardIntInit(sdmmchost_t *host, void *sdioInt);

/*!
 * @brief card detect init function.
 * @param host host handler
 * @param cd card detect configuration
 */
status_t SDMMCHOST_CardDetectInit(sdmmchost_t *host, void *cd);

/*!
 * @brief Detect card insert, only need for SD cases.
 * @param host host handler
 * @param waitCardStatus status which user want to wait
 * @param timeout wait time out.
 * @retval kStatus_Success detect card insert
 * @retval kStatus_Fail card insert event fail
 */
status_t SDMMCHOST_PollingCardDetectStatus(sdmmchost_t *host, uint32_t waitCardStatus, uint32_t timeout);

/*!
 * @brief card detect status.
 * @param host host handler
 * @retval kSD_Inserted, kSD_Removed
 */
uint32_t SDMMCHOST_CardDetectStatus(sdmmchost_t *host);

/*!
 * @brief Init host controller.
 * @param host host handler
 * @retval kStatus_Success host init success
 * @retval kStatus_Fail event fail
 */
status_t SDMMCHOST_Init(sdmmchost_t *host);

/*!
 * @brief Deinit host controller.
 * @param host host handler
 */
void SDMMCHOST_Deinit(sdmmchost_t *host);

/*!
 * @brief host power off card function.
 * @param host host handler
 * @param enable true is power on, false is power down.
 */
void SDMMCHOST_SetCardPower(sdmmchost_t *host, bool enable);

/*!
 * @brief host transfer function.
 * @param host host handler
 * @param content transfer content.
 */
status_t SDMMCHOST_TransferFunction(sdmmchost_t *host, sdmmchost_transfer_t *content);

/*!
 * @brief host reset function.
 * @deprecated Do not use this function. Application should not call this function, driver is responsible for the host
 reset..

 * @param host host handler
 */
void SDMMCHOST_Reset(sdmmchost_t *host);

/*!
 * @brief wait card detect status
 * @deprecated Do not use this function.It has been superceded by @ref SDMMCHOST_PollingCardDetectStatus..

 * @param hostBase host handler
 * @param cd card detect configuration.
 * @param waitCardStatus status to wait.
 */
status_t SDMMCHOST_WaitCardDetectStatus(SDMMCHOST_TYPE *hostBase,
                                        const sdmmchost_detect_card_t *cd,
                                        bool waitCardStatus);

/*!
 * @brief host power off card function.
 * @deprecated Do not use this function.It has been superceded by @ref SDMMCHOST_SetCardPower..
 *
 * @param base host base address.
 * @param pwr depend on user define power configuration.
 */
void SDMMCHOST_PowerOffCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr);

/*!
 * @brief host power on card function.
 * @deprecated Do not use this function.It has been superceded by @ref SDMMCHOST_SetCardPower..
 * @param base host base address.
 * @param pwr depend on user define power configuration.
 */
void SDMMCHOST_PowerOnCard(SDMMCHOST_TYPE *base, const sdmmchost_pwr_card_t *pwr);

/* @} */

#if defined(__cplusplus)
}
#endif
/* @} */
#endif /* _FSL_SDMMC_HOST_H */
