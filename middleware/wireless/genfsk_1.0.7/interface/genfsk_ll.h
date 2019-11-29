/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _GENFSK_LL_H_
#define _GENFSK_LL_H_

#include "EmbeddedTypes.h"

/*!
 * @addtogroup genfsk_ll
 * @{
 */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief GENFSK Timer invalid ID. */
#define gGENFSK_InvalidTimerId_c          (0xFF)
/*! @brief GENFSK Timer guard time in microseconds. */
#define gGENFSK_MinSetupTime_c            (20)
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
enum _GENFSK_interrupt_flags
{
   gGENFSK_SeqEndIrqFlag = GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK,
   gGENFSK_TxIrqFlag = GENFSK_IRQ_CTRL_TX_IRQ_MASK,
   gGENFSK_RxIrqFlag = GENFSK_IRQ_CTRL_RX_IRQ_MASK,
   gGENFSK_NtwAdrIrqFlag = GENFSK_IRQ_CTRL_NTW_ADR_IRQ_MASK,
   gGENFSK_T1IrqFlag = GENFSK_IRQ_CTRL_T1_IRQ_MASK,
   gGENFSK_T2IrqFlag = GENFSK_IRQ_CTRL_T2_IRQ_MASK,
   gGENFSK_PllUnlockIrqFlag = GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK,
   gGENFSK_WakeIrqFlag = GENFSK_IRQ_CTRL_WAKE_IRQ_MASK,
   gGENFSK_RxWatermarkIrqFlag = GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_MASK,
   gGENFSK_AllIrqFlags = GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK | GENFSK_IRQ_CTRL_TX_IRQ_MASK | GENFSK_IRQ_CTRL_RX_IRQ_MASK |
                         GENFSK_IRQ_CTRL_NTW_ADR_IRQ_MASK | GENFSK_IRQ_CTRL_T1_IRQ_MASK | GENFSK_IRQ_CTRL_T2_IRQ_MASK |
                         GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_MASK | GENFSK_IRQ_CTRL_WAKE_IRQ_MASK | GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_MASK                        
};

/*!
 * @brief GENFSK interrupts definition.
 *
 * Definition of the interrupts available in GENFSK LL.
 */
enum _GENFSK_interrupt_enable
{
   gGENFSK_SeqEndIrqEnable = GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK,
   gGENFSK_TxIrqEnable = GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK,
   gGENFSK_RxIrqEnable = GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK,
   gGENFSK_NtwAdrIrqEnable = GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK,
   gGENFSK_T1IrqEnable = GENFSK_IRQ_CTRL_T1_IRQ_EN_MASK,
   gGENFSK_T2IrqEnable = GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK,
   gGENFSK_PllUnlockIrqEnable = GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK,
   gGENFSK_WakeIrqEnable = GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK,
   gGENFSK_RxWatermarkIrqEnable = GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_EN_MASK,
   gGENFSK_AllIrqEnable = GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK | GENFSK_IRQ_CTRL_TX_IRQ_EN_MASK | GENFSK_IRQ_CTRL_RX_IRQ_EN_MASK |
                         GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK | GENFSK_IRQ_CTRL_T1_IRQ_EN_MASK | GENFSK_IRQ_CTRL_T2_IRQ_EN_MASK |
                         GENFSK_IRQ_CTRL_PLL_UNLOCK_IRQ_EN_MASK | GENFSK_IRQ_CTRL_WAKE_IRQ_EN_MASK | GENFSK_IRQ_CTRL_RX_WATERMARK_IRQ_EN_MASK                            
};

/*! @brief GENFSK LL internal states. */
typedef enum _GENFSK_LL_state
{    
    gGENFSK_LL_NoInit = 0U,
    gGENFSK_LL_Idle = 1U,
    gGENFSK_LL_BusyRx = 2U,
    gGENFSK_LL_BusyTx = 3U,
    gGENFSK_LL_BusyPendingRx = 4U,
    gGENFSK_LL_BusyPendingTx = 5U
} GENFSK_LL_state_t;

/*! @brief GENFSK LL task event flags. */ 
typedef enum
{
    gGenfskTxEventFlag_c = (1<<0),
    gGenfskRxEventFlag_c = (1<<1),
    gGenfskH0FailEventFlag_c  = (1<<2),
    gGenfskLengthFailEventFlag_c  = (1<<3),
    gGenfskH1FailEventFlag_c  = (1<<4),
    gGenfskNwkAddressMatchEventFlag_c  = (1<<5),
    gGenfskRxPllUnlockEventFlag_c  = (1<<6),
    gGenfskTxPllUnlockEventFlag_c  = (1<<7),
    gGenfskWakeEventFlag_c  = (1<<8)
}GENFSK_LL_event_flags_t;

/*! @brief GENFSK timeout callback. */
typedef void (*genfskTimeCallback_t) (void);

/*! @brief GENFSK timer ID type. */
typedef uint8_t genfskTimerId_t;

/*! @brief GENFSK time structure. */
typedef struct _GENFSK_TimeEvent
{
    GENFSK_timestamp_t    timestamp;
    genfskTimeCallback_t  callback;
} GENFSK_TimeEvent_t;

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
    uint32_t crcInit;
    uint32_t crcPoly;
    uint32_t crcXorOut;
    uint32_t whitenCfg;
    uint32_t whitenPoly;
    uint32_t whitenSzThr;
    uint32_t bitRate;  
} GENFSK_RegsStruct_t;

/*! @brief GENFSK local structure. */
typedef struct _GENFSK_LocalStruct
{
    GENFSK_LL_state_t genfskState;
    GENFSK_RxLocalStruct_t genfskRxLocal;
    GENFSK_radio_config_t radioConfig;    
    GENFSK_RegsStruct_t genfskRegs;
    uint8_t syncAddrSizeBytes;
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

/*! @} */

#endif /* _GENFSK_LL_H_ */
