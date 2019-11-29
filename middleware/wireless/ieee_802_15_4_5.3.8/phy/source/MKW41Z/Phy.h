/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __PHY_H__
#define __PHY_H__


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "PhyInterface.h"
#include "fsl_os_abstraction.h"


/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define mFrameCtrlLo_d       (0)
#define mFrameCtrlHi_d       (1)
#define mAddressingFields_d  (3)

#define mShortAddr_d         (2)
#define mExtAddr_d           (3)
#define mNoAddr_d            (0)
#define mPanIdCompression_d  (1 << 6)
#define mDstAddrModeMask_d   (0x0C)
#define mDstAddrModeShift_d  (2)
#define mSrcAddrModeMask_d   (0xC0)
#define mSrcAddrModeShift_d  (6)


/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
 
#ifdef __cplusplus
    extern "C" {
#endif

/* The interval at which the Phy will poll for a new buffer in order to start the RX */
#ifndef gPhyRxRetryInterval_c
#define gPhyRxRetryInterval_c         (60) /* [symbols] */
#endif

#ifndef gPhyUseNeighborTable_d
#define gPhyUseNeighborTable_d        (0)
#endif
       
#ifndef gUsePBTransferThereshold_d
#define gUsePBTransferThereshold_d    (0)
#endif

#define PhyGetSeqState()              PhyPpGetState()
#define PhyPlmeForceTrxOffRequest()   PhyAbort()

#ifndef gPhyDefaultRxWatermarkLevel_d
#define gPhyDefaultRxWatermarkLevel_d (5)
#endif

/* XCVR idle power mode */
#define gPhyDefaultIdlePwrMode_c   gPhyPwrIdle_c

/* Set the default power level to 0dBm */
#ifndef gPhyDefaultTxPowerLevel_d
#define gPhyDefaultTxPowerLevel_d     (22)
#endif

#define gPhyMaxTxPowerLevel_d         (32)
        
/* Tx Power level limit for each individual channel */
#ifndef gChannelTxPowerLimit_c
#define gChannelTxPowerLimit_c { gPhyMaxTxPowerLevel_d,   /* 11 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 12 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 13 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 14 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 15 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 16 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 17 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 18 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 19 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 20 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 21 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 22 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 23 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 24 */ \
                                 gPhyMaxTxPowerLevel_d,   /* 25 */ \
                                 gPhyMaxTxPowerLevel_d }  /* 26 */
#endif
      
/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/* PHY states */
enum {
  gIdle_c,
  gRX_c,
  gTX_c,
  gCCA_c,
  gTR_c,
  gCCCA_c,
};

/* PHY channel state */
enum {
  gChannelIdle_c,
  gChannelBusy_c
};

/* PANCORDNTR bit in PP */
enum {
  gMacRole_DeviceOrCoord_c,
  gMacRole_PanCoord_c
};

/* Cca types */
enum {
  gCcaED_c,            /* energy detect - CCA bit not active, not to be used for T and CCCA sequences */
  gCcaCCA_MODE1_c,     /* energy detect - CCA bit ACTIVE */
  gCcaCCA_MODE2_c,     /* 802.15.4 compliant signal detect - CCA bit ACTIVE */
  gCcaCCA_MODE3_c,     /* 802.15.4 compliant signal detect and energy detect - CCA bit ACTIVE */
  gInvalidCcaType_c    /* illegal type */
};

enum {
  gNormalCca_c,
  gContinuousCca_c
};

typedef struct Phy_PhyLocalStruct_tag
{
    PD_MAC_SapHandler_t         PD_MAC_SapHandler;
    PLME_MAC_SapHandler_t       PLME_MAC_SapHandler;
    msgQueue_t                  macPhyInputQueue;
    uint32_t                    maxFrameWaitTime;
    phyTxParams_t               txParams;
    phyRxParams_t               rxParams;
    phyChannelParams_t          channelParams;
    volatile uint8_t            flags;
    uint8_t                     currentMacInstance;
}Phy_PhyLocalStruct_t;


/*! *********************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
********************************************************************************** */

/* PHY Packet Processor */

/*! *********************************************************************************
 * \brief  Initialize the XCVR HW
 *
 ********************************************************************************** */
void PhyHwInit
( 
  void 
);

/*! *********************************************************************************
 * \brief  Enable/Disable the XCVR promiscuous mode. 
 *         In promiscuous mode, all filtering except CRC are disabled
 * 
 * \param[in] mode  TRUE - Promiscuous On
 *                  FALSE - Promiscous Off
 *
 ********************************************************************************** */
void PhyPpSetPromiscuous
(
  bool_t mode
);

/*! *********************************************************************************
 * \brief  Enable/Disable the XCVR active promiscuous mode. 
 *         In active promiscuous mode, all filtering except CRC are disabled.
 *         Also, the XCVR will send ACK to frames with destination address equal to 
 *         the device's address
 * 
 * \param[in] sate  TRUE - Promiscuous On
 *                  FALSE - Promiscous Off
 *
********************************************************************************** */
void PhySetActivePromiscuous
(
bool_t state
);

/*! *********************************************************************************
* \brief Returns the state of the Active Promiscuous feature
*
********************************************************************************** */
bool_t PhyGetActivePromiscuous
(
void
);

/*! *********************************************************************************
 * \brief Set the device's PAN Id
 *
 * \param
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPpSetPanId
(
  uint8_t *pPanId,
  uint8_t pan
);

/*! *********************************************************************************
 * \brief Set the device's Short Address
 *
 * \param
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPpSetShortAddr
(
  uint8_t *pShortAddr,
  uint8_t pan
);


/*! *********************************************************************************
 * \brief Set the device's IEEE Address
 *
 * \param
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPpSetLongAddr
(
  uint8_t *pLongAddr,
  uint8_t pan
);

/*! *********************************************************************************
 * \brief Set the device's role: Pan coordinator or not
 *
 * \param
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPpSetMacRole
(
  bool_t macRole,
  uint8_t pan
);

/*! *********************************************************************************
 * \brief Get the state of the FP bit sent in last ACK frame
 *
 * \return state of FP bit
 *
 ********************************************************************************** */
bool_t PhyPpIsTxAckDataPending
(
  void
);

/*! *********************************************************************************
 * \brief Get the state of the FP bit of the last received ACK frame
 *
 * \return state of FP bit
 *
 ********************************************************************************** */
bool_t PhyPpIsRxAckDataPending
(
  void
);

/*! *********************************************************************************
 * \brief Set the value of the FP bit for the next ACK frame
 *
 * \param[in] FP value of FP bit
 *
 ********************************************************************************** */
void PhyPpSetFpManually
(
  bool_t FP
);

/*! *********************************************************************************
 * \brief Check if the lasr received packet was a Poll Request
 *
 * \return TRUE if the received packet is a Poll Request
 *
 ********************************************************************************** */
bool_t PhyPpIsPollIndication
(
  void
);

/*! *********************************************************************************
 * \brief Enable/Disable Source Addressing Match feature
 *
 * \param[in] state of the SAM feature
 *
 ********************************************************************************** */
void PhyPpSetSAMState
(
  bool_t state
);

/*! *********************************************************************************
 * \brief Add a new entry into the HW indirect queue
 *
 * \param[in] index The table position
 * \param[in] checkSum The device hash code
 * \param[in] instanceId The instance of the PHY
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPp_IndirectQueueInsert
(
  uint8_t  index,
  uint16_t checkSum,
  instanceId_t instanceId
);

/*! *********************************************************************************
 * \brief Remove an entry from the HW indirect queue
 *
 * \param[in] index The table position
 * \param[in] instanceId The instance of the PHY
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPp_RemoveFromIndirect
(
  uint8_t index,
  instanceId_t instanceId
);

/*! *********************************************************************************
 * \brief Get the current state of the XCVR
 *
 * \return state
 *
 ********************************************************************************** */
uint8_t PhyPpGetState
(
  void
);

/*! *********************************************************************************
 * \brief Abort the current XCVR sequence
 *
 ********************************************************************************** */
void PhyAbort
(
  void
);

/* PHY PLME & DATA primitives */

/*! *********************************************************************************
 * \brief Start a TX sequence
 *
 * \param[in] pTxPacket Pointer to the PD request
 * \param[in] pRxParams Pointer to the Rx parameters
 * \param[in] pTxParams Pointer to the Tx parameters
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPdDataRequest
(  
  pdDataReq_t *pTxPacket,  
  volatile phyRxParams_t *pRxParams, 
  volatile phyTxParams_t *pTxParams
);

/*! *********************************************************************************
 * \brief Start an RX sequence
 *
 * \param[in] phyRxMode Slotted/Unslotted
 * \param[in] pRxParams Pointer to the Rx parameters
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeRxRequest
(
  phySlottedMode_t phyRxMode,
  phyRxParams_t *  pRxParams
);

/*! *********************************************************************************
 * \brief Start a immed RX sequence
 *
 * \param[in] pRxParams Pointer to the Rx parameters
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeTimmedRxRequest( phyRxParams_t *  pRxParams );

/*! *********************************************************************************
 * \brief Start a CCA or ED sequence
 *
 * \param[in] ccaParam  type of CCA
 * \param[in] cccaMode  continuous or single operation
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeCcaEdRequest
(
  phyCCAType_t     ccaParam,
  phyContCCAMode_t cccaMode
);

/*! *********************************************************************************
 * \brief Set the current 802.15.4 channel
 *
 * \param[in] channel number [11-26]
 * \param[in] pan index of the PAN
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeSetCurrentChannelRequest
(
  uint8_t channel,
  uint8_t pan
);

/*! *********************************************************************************
 * \brief Get the current 802.15.4 channel
 *
 * \param[in] pan index of the PAN
 *
 * \return status
 *
 ********************************************************************************** */
uint8_t PhyPlmeGetCurrentChannelRequest
(
  uint8_t pan
);

/*! *********************************************************************************
 * \brief Set the TX power level
 *
 * \param[in] pwrStep  the Tx power level
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeSetPwrLevelRequest
(
  uint8_t pwrStep
);

/*! *********************************************************************************
 * \brief Get the TX power level
 *
 * \return current power level
 *
 ********************************************************************************** */
uint8_t PhyPlmeGetPwrLevelRequest
(
  void
);

/*! *********************************************************************************
 * \brief Set a PHY PIB
 *
 * \param[in] pibId           The Id of the PIB
 * \param[in] pibValue        The value of the PIB
 * \param[in] phyRegistrySet  The index of the PAN
 * \param[in] instanceId      The instance of the PHY
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeSetPIBRequest
(
  phyPibId_t pibId,
  uint64_t pibValue,
  uint8_t phyRegistrySet,
  instanceId_t instanceId
);

/*! *********************************************************************************
 * \brief Get a PHY PIB
 *
 * \param[in] pibId           The Id of the PIB
 * \param[in] pibValue        Pointer to a location where the value will be stored
 * \param[in] phyRegistrySet  The index of the PAN
 * \param[in] instanceId      The instance of the PHY
 *
 * \return status
 *
 ********************************************************************************** */
phyStatus_t PhyPlmeGetPIBRequest
(
  phyPibId_t pibId,
  uint64_t * pibValue,
  uint8_t phyRegistrySet, 
  instanceId_t instanceId
);

/*! *********************************************************************************
 * \brief Set the value of the CCA Threshold
 *
 * \param[in] ccaThreshold  the threshold value
 *
 * \return status
 *
 ********************************************************************************** */

phyStatus_t PhyPpSetCcaThreshold
(
  uint8_t ccaThreshold
);

/* PHY Time */

/*! *********************************************************************************
 * \brief Set the absolute start time of the next sequence
 *
 * \param[in] startTime the sequence's absolute start time
 * 
 ********************************************************************************** */
void PhyTimeSetEventTrigger
(
  phyTime_t startTime
);

/*! *********************************************************************************
 * \brief Set the absolute end time of the next sequence
 *
 * \param[in] pEndTime pointer to the sequence's absolute end time
 * 
 ********************************************************************************** */
void PhyTimeSetEventTimeout
(
  phyTime_t *pEndTime
);

/*! *********************************************************************************
 * \brief Get the absolute end time of the next sequence
 * 
 * \return the sequence timeout
 *
 ********************************************************************************** */
phyTime_t PhyTimeGetEventTimeout( void );

/*! *********************************************************************************
 * \brief Read the XCVR timer
 *
 * \param[out] pRetClk pointer to a location where the current time will be stored
 *
 ********************************************************************************** */
void PhyTimeReadClock
(
  phyTime_t *pRetClk
);

/*! *********************************************************************************
 * \brief Disable the sequence timeout
 * 
 ********************************************************************************** */
void PhyTimeDisableEventTimeout
(
  void
);

/*! *********************************************************************************
 * \brief Disable the sequence start time trigger.
 * 
 ********************************************************************************** */
void PhyTimeDisableEventTrigger
(
  void
);

/*! *********************************************************************************
 * \brief Program a timer to wake up the MCU from low power
 *
 * \param[in] pWakeUpTime  pointer to the timer's value
 * 
 ********************************************************************************** */
void PhyTimeSetWakeUpTime
(
  uint32_t *pWakeUpTime
);

/*! *********************************************************************************
 * \brief Set the value of the XCVR timer
 *
 * \param[in] pAbsTime  pointer to the timer's value
 *
 ********************************************************************************** */
void PhyTimeInitEventTimer
(
  uint32_t *pAbsTime
);

/*! *********************************************************************************
 * \brief Check if the wake-up timed has expired and disable it.
 *
 * \return TRUE if timer has expired, else FALSE
 *
 ********************************************************************************** */
bool_t PhyTimeIsWakeUpTimeExpired
(
  void
);

/*! *********************************************************************************
 * \brief Program an XCVR wait indication
 *
 * \param[in] pWaitTimeout pointer to the absolute time of the event
 *
 ********************************************************************************** */
void PhyTimeSetWaitTimeout
(
  phyTime_t *pWaitTimeout
);

/*! *********************************************************************************
 * \brief Disable XCVR event indication
 *
 ********************************************************************************** */
void PhyTimeDisableWaitTimeout
(
  void
);

/* PHY ISR */

/*! *********************************************************************************
 * \brief Installs the XCVR interrupt
 *
 ********************************************************************************** */
void PHY_InstallIsr
(
  void
);

/*! *********************************************************************************
* \brief  This function forces the PHY IRQ to be triggered to run the ISR
*
********************************************************************************** */
void PHY_ForceIrqPending
(
  void
);

/*! *********************************************************************************
 * \brief Check is an XCVR IRQ is pending
 *
 * \return  TRUE if ISR pending, else FALSE
 *
 ********************************************************************************** */
bool_t PHY_isIrqPending
(
  void
);

/*! *********************************************************************************
 * \brief This is the XCVR ISR
 *
 ********************************************************************************** */
void PHY_InterruptHandler
(
  void
);


/*! *********************************************************************************
 * \brief Set the pointer to the location where packet related information will be stored
 *        Set to NULL if no packet details are needed
 *
 * \param[in] pRxParam pointer to the Rx parameters
 *
 ********************************************************************************** */
void PhyIsrPassRxParams
(
  volatile phyRxParams_t * pRxParam
);

/*! *********************************************************************************
* \brief Returns a random number from PHY
*
* \return random number
* 
********************************************************************************** */
void PhyGetRandomNo
(
  uint32_t *pRandomNo
);

/*! *********************************************************************************
* \brief Set the state of the Dual PAN automatic mode
*
* \param[in] mode of the Dual PAN automatic mode
*
********************************************************************************** */
void PhyPpSetDualPanAuto
(
  bool_t mode
);

/*! *********************************************************************************
* \brief Get the state of the Dual PAN automatic mode
*
* \return the state of the Dual PAN automatic mode
*
********************************************************************************** */
bool_t PhyPpGetDualPanAuto
(
   void
);

/*! *********************************************************************************
* \brief Set the Dual PAN Dwell switch time 
*
********************************************************************************** */
void PhyPpSetDualPanDwell
(
  uint8_t dwell
);

/*! *********************************************************************************
* \brief Get the Dual PAN Dwell switch time 
*
* \return dwell time
*
********************************************************************************** */
uint8_t PhyPpGetDualPanDwell
(
  void
);

/*! *********************************************************************************
* \brief Get the remaining time until a channel switch will occure
*
* \return remaining time until PAN switch wil occur
* 
********************************************************************************** */
uint8_t PhyPpGetDualPanRemain
(
  void
);

/*! *********************************************************************************
* \brief Set the level at which the HW queue is split for the two PANs
*
* \param[in] queue split level
*
********************************************************************************** */
void PhyPpSetDualPanSamLvl
(
  uint8_t level
);

/*! *********************************************************************************
* \brief Returns the level at which the HW queue is split for the two PANs
*
* \return queue split level
*
********************************************************************************** */
uint8_t PhyPpGetDualPanSamLvl
(
  void
);

/*! *********************************************************************************
* \brief Set the active PAN
*
* \param[in] the index of the PAN
*
********************************************************************************** */
void PhyPpSetDualPanActiveNwk
(
  uint8_t nwk
);

/*! *********************************************************************************
* \brief Returns the current NWK on which the PHY is operating
*
* \return the index of the active PAN
* 
********************************************************************************** */
uint8_t PhyPpGetDualPanActiveNwk
(
  void
);

/*! *********************************************************************************
* \brief Return the PAN on which the packet was received (can be receiced on both PANs)
*
* \return the PAN index
* 
********************************************************************************** */
uint8_t PhyPpGetPanOfRxPacket
(
  void
);

/*! *********************************************************************************
* \brief Return the LQI value for the last received packet
*
* \return LQI
* 
********************************************************************************** */
uint8_t PhyGetLastRxLqiValue(void);

/*! *********************************************************************************
* \brief Returns the RSSI value for the last received packet
*
* \return RSSI
*
********************************************************************************** */
uint8_t PhyGetLastRxRssiValue(void);

/*! *********************************************************************************
* \brief  This function converts the LQI reported by the PHY into an signed RSSI value
*
* \param[in]  LQI  the LQI reported by the PHY
*
* \return  the RSSI value in dbm
*
********************************************************************************** */
int8_t PhyConvertLQIToRSSI(uint8_t lqi);

/*! *********************************************************************************
* \brief Enable the FAD function (FAD_EN bit)
*
* \param[in] state , the state of the FAD feature
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetFADStateRequest(bool_t state);

/*! *********************************************************************************
* \brief Correlator threshold at which the FAD will select the antenna
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetFADThresholdRequest(uint8_t FADThreshold);

/*! *********************************************************************************
* \brief Enable the ANT pads
*
* \param[in] antAB_on - 
* \param[in] rxtxSwitch_on - 
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetANTPadStateRequest(bool_t antAB_on, bool_t rxtxSwitch_on);

/*! *********************************************************************************
* \brief Invert the logic of the ANT pads
*
* \param[in] invAntA - invert the ANT_A pad
* \param[in] invAntB - invert the ANT_A pad
* \param[in] invTx   - invert the ANT_TX pad
* \param[in] invRx   - invert the ANT_RX pad
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetANTPadInvertedRequest(bool_t invAntA, bool_t invAntB, bool_t invTx, bool_t invRx);

/*! *********************************************************************************
* \brief ANTX_IN - FAD Antenna start when FAD_EN = 1 or antenna selected
*              when FAD_EN=0
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetANTXStateRequest(bool_t state);

/*! *********************************************************************************
* \brief Antenna selected in FAD of non-FAD mode
*
* \return Chosen antenna by the FAD (FAD_EN = 1) or copy of ANTX_IN
*
********************************************************************************** */
uint8_t PhyPlmeGetANTXStateRequest(void);

/*! *********************************************************************************
* \brief Choose LQI Mode: 1 - LQI Based on RSSI, 
*                         0 - LQI Based on Correlation Peaks
*
* \return gPhySuccess
*
********************************************************************************** */
uint8_t PhyPlmeSetLQIModeRequest(uint8_t lqiMode);

/*! *********************************************************************************
* \brief Returns the RSSI level value, refreshed every 125us
*
* \return RSSI level [dbm]
*
********************************************************************************** */
uint8_t PhyPlmeGetRSSILevelRequest(void);

/*! *********************************************************************************
* \brief Informs the PHY if it should start an RX when entering IDLE or not
*
* \param[in] state      - if TRUE the XCVR will start an Rx when Idle
* \param[in] instanceId - the instance of the PHY
*
********************************************************************************** */
void PhyPlmeSetRxOnWhenIdle( bool_t state, instanceId_t instanceId );

/*! *********************************************************************************
* \brief  Set the amount of time in symbols to wait for an data frame 
*         after receiving an ACK with FP=1
*
* \param[in] time       - the response wait time in symbols
* \param[in] instanceId - the instance of the PHY
*
********************************************************************************** */
void PhyPlmeSetFrameWaitTime( uint32_t maxTime, instanceId_t instanceId );

/*! *********************************************************************************
* \brief Set the start time and end time for an XCVR sequence
*
* \param[in] startTime   - absolute start time in symbols
* \param[in] seqDuration - sequence duration in symbols
*              
********************************************************************************** */
void Phy_SetSequenceTiming(phyTime_t startTime, uint32_t seqDuration);

/*! *********************************************************************************
* \brief  Scales energy level to 0-255
*
* \param[in]  energyLevel  the energy level reported by HW
*
* \return  uint8_t  the energy level scaled in 0x00-0xFF
*
********************************************************************************** */
uint8_t Phy_GetEnergyLevel(uint8_t energyLevel);

/*! *********************************************************************************
* \brief  Disable the XCVR interrupts
*
********************************************************************************** */
void ProtectFromXcvrInterrupt(void);

/*! *********************************************************************************
* \brief  Enable the XCVR interrupts
*
********************************************************************************** */
void UnprotectFromXcvrInterrupt(void);


/* PHY Neihbor Table API */
uint16_t PhyGetChecksum(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);

uint8_t  PhyAddToNeighborTable(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);

uint8_t  PhyRemoveFromNeighborTable(uint8_t *pAddr, uint8_t addrMode, uint16_t PanId);

bool_t   PhyCheckNeighborTable(uint16_t checksum);

void     PhyResetNeighborTable(void);


/* RADIO EVENTS */

void Radio_Phy_PdDataConfirm(instanceId_t instanceId, bool_t framePending);

void Radio_Phy_TimeWaitTimeoutIndication(instanceId_t instanceId);

void Radio_Phy_TimeRxTimeoutIndication(instanceId_t instanceId);

void Radio_Phy_PdDataIndication(instanceId_t instanceId);

void Radio_Phy_TimeStartEventIndication(instanceId_t instanceId);

void Radio_Phy_PlmeCcaConfirm(phyStatus_t phyChannelStatus, instanceId_t instanceId);

void Radio_Phy_PlmeEdConfirm(uint8_t energyLeveldB, instanceId_t instanceId);

void Radio_Phy_PlmeSyncLossIndication(instanceId_t instanceId);

void Radio_Phy_PlmeRxWatermark(instanceId_t instanceId, uint32_t frameLength);

void Radio_Phy_PlmeFilterFailRx(instanceId_t instanceId);

void Radio_Phy_UnexpectedTransceiverReset(instanceId_t instanceId);

void Radio_Phy_DummyEvent(instanceId_t instanceId);

bool_t PhyIsIdleRx( instanceId_t instanceId );

void Radio_Phy_Notify(void);

#if defined(__IAR_SYSTEMS_ICC__)
extern __weak void PWR_AllowXcvrToSleep(void);
extern __weak void PWR_DisallowXcvrToSleep(void);
extern __weak void PhyTime_TMR4Callback(void);
#elif defined(__GNUC__)
extern void PWR_AllowXcvrToSleep(void) __attribute__((weak));
extern void PWR_DisallowXcvrToSleep(void) __attribute__((weak));
extern void PhyTime_TMR4Callback(void) __attribute__((weak));
#endif

#ifdef __cplusplus
}
#endif

#endif /* __PHY_H__ */
