/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _FSL_TSI_V5_DRIVER_SPECIFIC_H_
#define _FSL_TSI_V5_DRIVER_SPECIFIC_H_

/**
 * \ingroup nt_driver  
 * \{
 */

/**
* Encapsulates SelfCap and Mutual configuration structure for TSI driver.
*
* Use an instance of this structure with NT_TSI_DRV_Init(). This allows you to configure the
* most common settings of the TSI peripheral with a single function call. Settings include:
*
*/
typedef struct {
    tsi_selfCap_config_t            configSelfCap; /*!< Hardware configuration for self capacitance measurement */
    tsi_mutualCap_config_t          configMutual;  /*!< Hardware configuration for mutual capacitance measurement */
    uint16_t                        thresl;        /*!< Low threshold for out-of-range interrupt (wake-up from low-power) */
    uint16_t                        thresh;        /*!< High threshold for out-of-range interrupt (wake-up from low-power) */
}tsi_config_t;
    
/*! @brief TSI status flags. */
typedef enum _tsi_sinc_status_flags
{
    kTSI_SwitchEnable  = TSI_SINC_SWITCH_ENABLE_MASK,    /*!< End-Of-Scan flag */
    kTSI_SincOverflowFlag = TSI_SINC_SINC_OVERFLOW_FLAG_MASK,/*!< Out-Of-Range flag */
    kTSI_SincValid  = TSI_SINC_SINC_VALID_MASK,          /*!< End-Of-Scan flag */
    kTSI_SscControlOut  = TSI_SINC_SSC_CONTROL_OUT_MASK  /*!< End-Of-Scan flag */
} tsi_sinc_status_flags_t;

/*! @brief TSI low power status flags. */
typedef struct _tsi_lpwr_status_flags
{
    uint8_t   TSIScanCompleteFlag;
    uint8_t   TSILowPower;
    uint8_t   SelfLowPowerChannelBuff;
    uint8_t   SelfLowPowerSavedFlag;
    uint16_t  SelfLowPowerCountBuff;
} tsi_lpwr_status_flags_t;

#define TF_TSI_SELF_CAP_CHANNEL_0  0U
#define TF_TSI_SELF_CAP_CHANNEL_1  1U
#define TF_TSI_SELF_CAP_CHANNEL_2  2U
#define TF_TSI_SELF_CAP_CHANNEL_3  3U
#define TF_TSI_SELF_CAP_CHANNEL_4  4U
#define TF_TSI_SELF_CAP_CHANNEL_5  5U
#define TF_TSI_SELF_CAP_CHANNEL_6  6U
#define TF_TSI_SELF_CAP_CHANNEL_7  7U
#define TF_TSI_SELF_CAP_CHANNEL_8  8U
#define TF_TSI_SELF_CAP_CHANNEL_9  9U
#define TF_TSI_SELF_CAP_CHANNEL_10 10U
#define TF_TSI_SELF_CAP_CHANNEL_11 11U
#define TF_TSI_SELF_CAP_CHANNEL_12 12U
#define TF_TSI_SELF_CAP_CHANNEL_13 13U
#define TF_TSI_SELF_CAP_CHANNEL_14 14U
#define TF_TSI_SELF_CAP_CHANNEL_15 15U
#define TF_TSI_SELF_CAP_CHANNEL_16 16U
#define TF_TSI_SELF_CAP_CHANNEL_17 17U
#define TF_TSI_SELF_CAP_CHANNEL_18 18U
#define TF_TSI_SELF_CAP_CHANNEL_19 19U
#define TF_TSI_SELF_CAP_CHANNEL_20 20U
#define TF_TSI_SELF_CAP_CHANNEL_21 21U
#define TF_TSI_SELF_CAP_CHANNEL_22 22U
#define TF_TSI_SELF_CAP_CHANNEL_23 23U
#define TF_TSI_SELF_CAP_CHANNEL_24 24U

#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT 6U
#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_6  6U
#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_7  7U
#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_8  8U
#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_9  9U
#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_10 10U
#define TF_TSI_MUTUAL_CAP_RX_CHANNEL_11 11U

#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_COUNT 6U
#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_0  0U
#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_1  1U
#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_2  2U
#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_3  3U
#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_4  4U
#define TF_TSI_MUTUAL_CAP_TX_CHANNEL_5  5U

#define TF_TSI_SELF_CAP_CHANNELS_MASK 0x0000000001FFFFFFULL
#define TF_TSI_MUTUAL_CAP_CHANNELS_MASK 0x1FFFFFFFFE000000ULL

#define TF_TSI_SELF_CAP_CHANNEL_COUNT 25U  
#define TF_TSI_MUTUAL_CHANNEL_COUNT (TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT * TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT) 
#define TF_TSI_TOTAL_CHANNEL_COUNT (TF_TSI_MUTUAL_CHANNEL_COUNT + TF_TSI_SELF_CAP_CHANNEL_COUNT)

/* This macro transforms mutual RX, TX electrode numbers into a single electrode number */
#define NT_TSI_TRANSFORM_MUTUAL(RX,TX) (((0ULL + (TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT*(TX))) + ((RX)-TF_TSI_MUTUAL_CAP_RX_CHANNEL_COUNT)) + TF_TSI_SELF_CAP_CHANNEL_COUNT)

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#if defined(__cplusplus)
}
#endif

/* \} */
#endif /* _FSL_TSI_V5_DRIVER_SPECIFIC_H_ */
/*******************************************************************************
 * EOF
 ******************************************************************************/

