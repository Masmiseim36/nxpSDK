/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#ifndef _GENFSK_LL_H_
#define _GENFSK_LL_H_

#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"

#if defined (RADIO_IS_GEN_3P5)
#ifndef XCVR_MISC
#define XCVR_MISC XCVR__MISC
#endif
#ifndef XCVR_RX_DIG
#define XCVR_RX_DIG XCVR
#endif
#ifndef XCVR_TX_DIG
#define XCVR_TX_DIG XCVR__TX_DIG
#endif
#ifndef XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK
#define XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL_MASK XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL_MASK
#endif
#ifndef XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL
#define XCVR_CTRL_XCVR_CTRL_RADIO0_IRQ_SEL XCVR_MISC_XCVR_CTRL_RADIO0_IRQ_SEL
#endif
#ifndef XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK
#define XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL_MASK XCVR_MISC_XCVR_CTRL_RADIO1_IRQ_SEL_MASK
#endif
#ifndef XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL
#define XCVR_CTRL_XCVR_CTRL_RADIO1_IRQ_SEL XCVR_MISC_XCVR_CTRL_RADIO1_IRQ_SEL
#endif
#endif

/*!
 * @addtogroup genfsk_ll
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief GENFSK Timer guard time in microseconds. */
#define gGENFSK_MinSetupTime_c            (20U)
/*! @brief GENFSK Timer Overflow value. */
#define gGENFSK_OverflowTimerUnit_c       (0x1000000UL)
     
/*! Type qualifier - does not affect local variables of integral type */
#ifndef WEAK
#if defined(__GNUC__)
#define WEAK __attribute__ ((__weak__))
#elif defined(__IAR_SYSTEMS_ICC__)
#define WEAK __weak
#endif
#endif

/*!
 * @brief GENFSK interrupts definition.
 *
 * Definition of the interrupt processing to be done for GENFSK.
 */
#define gGENFSK_AllIrqFlags (GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK | GENFSK_IRQ_CTRL_TX_IRQ_MASK | GENFSK_IRQ_CTRL_RX_IRQ_MASK | \
                             GENFSK_IRQ_CTRL_NTW_ADR_IRQ_MASK | GENFSK_IRQ_CTRL_T1_IRQ_MASK | GENFSK_IRQ_CTRL_T2_IRQ_MASK | \
                             GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK | GENFSK_IRQ_CTRL_WAKE_IRQ_MASK | GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_MASK)

/*!
 * @brief GENFSK interrupts definition.
 *
 * Definition of the interrupts available in GENFSK LL.
 */
#define gGENFSK_AllIrqEnable (GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK | GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK | \
                              GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | GENFSK_IRQ_CTRL_T1_IRQ_EN_MASK | GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK | \
                              GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_EN_MASK)

/*! @brief GENFSK LL internal states. */
typedef enum _GENFSK_LL_state
{    
    gGENFSK_LL_NoInit = 0U,
    gGENFSK_LL_Idle = 1U,
    gGENFSK_LL_BusyRx = 2U,
    gGENFSK_LL_BusyTx = 3U,
    gGENFSK_LL_BusyPendingRx = 4U,
    gGENFSK_LL_BusyPendingTx = 5U
#if defined (RADIO_IS_GEN_3P5)
    ,gGENFSK_LL_BusyTxRx = 6U,
    gGENFSK_LL_BusyPendingTxRx = 7U,
    gGENFSK_LL_BusyRxTx = 8U,
    gGENFSK_LL_BusyPendingRxTx = 9U
#endif
    ,gGENFSK_LL_InvalidState = 10U
} GENFSK_LL_state_t;

/*! @brief GENFSK LL task event flags. */ 
#define gGenfskTxEventFlag_c                (1UL<<0)
#define gGenfskRxEventFlag_c                (1UL<<1)
#define gGenfskH0FailEventFlag_c            (1UL<<2)
#define gGenfskLengthFailEventFlag_c        (1UL<<3)
#define gGenfskH1FailEventFlag_c            (1UL<<4)
#define gGenfskNwkAddressMatchEventFlag_c   (1UL<<5)
#define gGenfskRxPllUnlockEventFlag_c       (1UL<<6)
#define gGenfskTxPllUnlockEventFlag_c       (1UL<<7)
#define gGenfskWakeEventFlag_c              (1UL<<8)
#if (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c)
#define gGenfskMwsReleaseEventFlag_c        (1UL<<9)
#endif /* (gGENFSK_MwsControl_c != gGENFSK_NoMwsControl_c) */

/*! @brief GENFSK RX local structure. */
typedef struct _GENFSK_RxLocalStruct
{
    uint8_t *rxPacketBuffer;
    uint16_t rxMaxPacketLength;
} GENFSK_RxLocalStruct_t;

/*! @brief GENFSK saved registers structure.*/
typedef struct _GENFSK_RegsStruct
{
    uint32_t irqCtrl;
    uint32_t xcvrCtrl;
    uint32_t xcvrCfg;
    uint32_t channelNum;
    uint32_t txPower;
    uint32_t ntwAdrCtrl;
    uint32_t ntwAdr0;
    uint32_t ntwAdr1;
    uint32_t ntwAdr2;
    uint32_t ntwAdr3;
    uint32_t packetCfg;
    uint32_t h0Cfg;
    uint32_t h1Cfg;
    uint32_t crcCfg;
#if defined (RADIO_IS_GEN_3P5)
    uint32_t lengthAdj;
    uint32_t enhFeature;
    uint32_t turnaroundTime;
#endif
    uint32_t crcInit;
    uint32_t crcPoly;
    uint32_t crcXorOut;
    uint32_t whitenCfg;
    uint32_t whitenPoly;
    uint32_t whitenSzThr;
#if !defined (RADIO_IS_GEN_3P5)
    uint32_t bitRate;
#endif
} GENFSK_RegsStruct_t;

/*! @brief GENFSK local structure. */
typedef struct _GENFSK_LocalStruct
{
    GENFSK_LL_state_t genfskState;
    GENFSK_RxLocalStruct_t genfskRxLocal;
    GENFSK_radio_config_t radioConfig;    
    GENFSK_RegsStruct_t genfskRegs;
    uint8_t syncAddrSizeBytes;
    uint8_t headerSizeBytes;
    int16_t adjSizeBytes;
    genfskPacketType_t packetType;    
    genfskCrcComputeMode_t crcEnable;
    genfskCrcRecvInvalid_t crcRecvInvalid;
    genfskWhitenMode_t whitenEnable;
    genfskPacketReceivedCallBack_t packetReceivedcallback;
    genfskEventNotifyCallBack_t eventNotifyCallback;
    uint32_t enabledEvents;
} GENFSK_LocalStruct_t;

/*! @brief GENFSK instance status. */
typedef uint8_t GENFSK_InstanceStatus_t;

extern GENFSK_LocalStruct_t genfskLocal[];
extern osaEventId_t mGenfskTaskEvent;
extern genfskTimerId_t rxTimeoutTimer;
extern uint8_t mGenfskActiveInstance;

#if defined(RADIO_IS_GEN_3P0) && (RADIO_IS_GEN_3P0 != 0) && defined(RF_OSC_26MHZ) && (RF_OSC_26MHZ != 0)
extern uint8_t gGenfskRxRecycleHdrError;
extern uint8_t gGenfskRxRecycleCrcError;
#endif

/*! @} */

#endif /* _GENFSK_LL_H_ */
