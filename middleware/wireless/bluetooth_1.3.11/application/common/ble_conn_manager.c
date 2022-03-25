/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2021 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "Panic.h"

#include "ble_general.h"
#include "gap_types.h"
#include "gap_interface.h"
#include "ble_conn_manager.h"
#include "board.h"

#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
#include "TimersManager.h"
#endif

#include "ble_config.h"
#include "MemManager.h"

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

#if defined(__CC_ARM)
/* These  definitions are used only by the demo applications which do not have
 * advertisement capabilities in order to fix Keil compilation errors.
 * All other demo applications use the definitions present in app_config.c file.
 */
gapAdvertisingData_t            gAppAdvertisingData __attribute__((weak));
gapScanResponseData_t           gAppScanRspData __attribute__((weak));
gapSmpKeys_t                    gSmpKeys __attribute__((weak));
gapScanningParameters_t         gScanParams __attribute__((weak));
gapAdvertisingParameters_t      gAdvParams __attribute__((weak));
#endif
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef struct repeatedAttemptsDevice_tag{
    bleDeviceAddress_t  address;
    uint16_t            baseTimeout;   /* seconds */
    uint16_t            remainingTime; /* seconds */
}repeatedAttemptsDevice_t;

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
STATIC void BleConnManager_MCUInfoToSmpKeys(void);
STATIC void BleConnManager_DataLengthUpdateProcedure(deviceId_t peerDeviceId);
#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
STATIC bleResult_t BleConnManager_ManagePrivacyInternal(bool_t bCheckNewBond);
#endif

#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
STATIC bool_t   RepeatedAttempts_CheckRequest(bleDeviceAddress_t address);
STATIC void     RepeatedAttempts_LogAttempt(gapPairingCompleteEvent_t *pEvent, bleDeviceAddress_t address);
STATIC void     RepeatedAttempts_UpdateTimePassed(uint16_t seconds);
STATIC uint16_t RepeatedAttempts_GetMinTimeToWait();
STATIC void     RepeatedAttempts_TimerCb(void *param);
#endif
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static bleDeviceAddress_t   maBleDeviceAddress;
STATIC uint32_t             mSupportedFeatures;

#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
 /* The number of bonded devices. It is incremented each time a new bond is created,
    on 'gBondCreatedEvent_c' generic event. When a bond or all bonds are removed,
    this variable must be updated accordingly by the application, since there
    is no corresponding 'bond removed' event triggered by the LE Host Stack */
uint8_t gcBondedDevices = 0;
#if gAppUsePrivacy_d
STATIC uint8_t mcDevicesInResolvingList = 0; /* Number of devices present in host resolving list */
STATIC bool_t  mbPrivacyEnabled = FALSE;
STATIC bool_t  mHaveRandomAddress = FALSE;
STATIC bool_t  mSettingRandomAddressFromApplication = FALSE;
#endif /* gAppUsePrivacy_d */
#endif /* gAppUseBonding_d */

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
static bleDeviceAddress_t   maPeerDeviceAddress;
#if gAppUseBonding_d
static bleAddressType_t     mPeerDeviceAddressType;
#endif /* gAppUseBonding_d */
STATIC uint8_t              mSuccessfulPairings;
STATIC uint8_t              mFailedPairings;
#endif

#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
repeatedAttemptsDevice_t    maPairingPeers[gRepeatedAttemptsNoOfDevices_c] = {0};
static bleDeviceAddress_t   maPeerDeviceOriginalAddress;
static tmrTimerID_t         mRepeatedAttemptsTimerId = gTmrInvalidTimerID_c;
static uint16_t             mMinTimeToWait = 0;
#endif
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  Performs common operations on the BLE stack on the generic callback.
*
* \param[in] pGenericEvent  GAP Generic event from the Host Stack .
*
* \return  none
*
********************************************************************************** */
void BleConnManager_GenericEvent(gapGenericEvent_t* pGenericEvent)
{
    switch (pGenericEvent->eventType)
    {
        case gInitializationComplete_c:
        {
            /* Save feature set */
            mSupportedFeatures = pGenericEvent->eventData.initCompleteData.supportedFeatures;

            BleConnManager_MCUInfoToSmpKeys();
        }
        break;

#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && \
    (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))

        case gRandomAddressSet_c:
        {
            mHaveRandomAddress = TRUE;
        }
        break;

        case gRandomAddressReady_c:
        {
            if ((FALSE == mHaveRandomAddress) &&
                (TRUE == mSettingRandomAddressFromApplication))
            {
                mSettingRandomAddressFromApplication = FALSE;
                (void)Gap_SetRandomAddress(pGenericEvent->eventData.addrReady.aAddress);
            }
        }
        break;

#endif /* gAppUseBonding_d && gAppUsePrivacy_d */

        case gPublicAddressRead_c:
        {
            /* Use address read from the controller */
            FLib_MemCpy(maBleDeviceAddress, pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
            gSmpKeys.addressType = gBleAddrTypePublic_c;
            gSmpKeys.aAddress = maBleDeviceAddress;
#endif
        }
        break;

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
        case gLeScPublicKeyRegenerated_c:
        {
            /* Key pair regenerated -> reset pairing counters */
            mFailedPairings = mSuccessfulPairings = 0;
        }
        break;
#endif

#if defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U)
        case gBondCreatedEvent_c:
        {
            /* Stack created a bond after pairing or at app request, update global counter. */
            gcBondedDevices++;
        }
        break;

        /* There is no event for bond removal. This action is driven by the application and is
           synchronous, so it is application's responsibility to update gcBondedDevices if necessary. */
#endif

#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && \
    (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
#if defined(gBleEnableControllerPrivacy_d) && (gBleEnableControllerPrivacy_d > 0)
        case gControllerPrivacyStateChanged_c:
        {
            /* If controller privacy has been disabled, set a new random address when it
               is enabled again - IRK may have changed */
            if (pGenericEvent->eventData.newControllerPrivacyState == FALSE)
            {
                mHaveRandomAddress = FALSE;
            }
        }
        break;
#endif /* gBleEnableControllerPrivacy_d */
#endif /* gAppUseBonding_d && gAppUsePrivacy_d */

        case gInternalError_c:
        {
            /* This is a generic error which is triggered when a dedicated error does not exist for the specific problem.
             * Treated below are some of the most important error cases which could trigger an internal error. Modify at need according to use case.
             * A list of all error codes and sources can be found in ble_general.h - see bleResult_t and gapInternalErrorSource_t enums
             * The error codes for which panic is not triggered are not critical, but are included here for awareness. */
            switch (pGenericEvent->eventData.internalError.errorCode)
            {
                /* An error occurred at the OS level - Host or Controller task creation or initialization failed. */
                case gBleOsError_c:
                /* A "should never get here"-type error - BLE Radio Init failed or an error occured when starting the next
                 * advertising event for the next SID while in Limited Discoverable Mode */
                case gBleUnexpectedError_c:

                /* HCI layer initialization failure. HCI data buffering semaphore allocation error.
                 * HCI layer received an unknown packet type. Serial initialization or write failure. */
                case gHciTransportError_c:

                /* There is a problem with the internal state of the SM. This should not happen during normal operations. */
                case gSmInvalidInternalOperation_c:

                /* Timer allocation failed - consider increasing the number of timers in app_preinclude.h */
                case gBleTimerError_c:
                {
                    panic(0,0,0,0);
                }
                break;

                /* The Host has temporarily run out of buffers. This issue should be handled by the Host by freeing resources after data processing.
                 * If this error code persists on being received, consider modifying the buffer configuration in app_preinclude.h */
                case gBleOutOfMemory_c:

                /* An internal limit is reached:
                 * L2CAP TX queue is full - this issue should be handled by the Host by freeing resources after data processing.
                 * Exceeded the allowed number of connections - consider updating gAppMaxConnections_c in app_preinclude.h according to hardware and use case. */
                case gBleOverflow_c:
                {
                    ; /* For awareness only - no action required */
                }
                break;

                default:
                {
                    ; /* No action required */
                }
                break;
            }
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief  Performs GAP Peripheral operations on the BLE stack on the connection callback.
*
* \param[in] peerDeviceId      The GAP peer Id.
*
* \param[in] pConnectionEvent  GAP Connection event from the Host Stack.
*
* \return  none
*
********************************************************************************** */
void BleConnManager_GapPeripheralEvent(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
#ifndef gCentralInitiatedPairing_d
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
            FLib_MemCpy(maPeerDeviceOriginalAddress, pConnectionEvent->eventData.connectedEvent.peerAddress, sizeof(bleDeviceAddress_t));
#endif
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
            bool_t isBonded = FALSE;
            uint8_t nvmIndex = gInvalidNvmIndex_c;

            /* Copy peer device address information */
            mPeerDeviceAddressType = pConnectionEvent->eventData.connectedEvent.peerAddressType;
            FLib_MemCpy(maPeerDeviceAddress, pConnectionEvent->eventData.connectedEvent.peerAddress, sizeof(bleDeviceAddress_t));

            /* Perform pairing if peer is not bonded or resolution procedure for its address failed */
            if ((gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded, &nvmIndex) && FALSE == isBonded) ||
                (Ble_IsPrivateResolvableDeviceAddress(maPeerDeviceAddress) &&
                    FALSE == pConnectionEvent->eventData.connectedEvent.peerRpaResolved))
#endif
            {
                (void)Gap_SendSlaveSecurityRequest(peerDeviceId, &gPairingParameters);
            }
#endif
#endif

#if gConnUpdateAlwaysAccept_d
            (void)Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
#endif
            /* Initiate Data Length Update Procedure */
            BleConnManager_DataLengthUpdateProcedure(peerDeviceId);
#if gConnInitiatePhyUpdateRequest_c
            if ((mSupportedFeatures & ((uint32_t)gLe2MbPhy_c | (uint32_t)gLeCodedPhy_c)) != 0U)
            {
                (void)Gap_LeSetPhy(FALSE, peerDeviceId, 0, gConnPhyUpdateReqTxPhySettings_c, gConnPhyUpdateReqRxPhySettings_c, (uint16_t)gConnPhyUpdateReqPhyOptions_c);
            }
#endif
        }
        break;

        case gConnEvtPairingRequest_c:
        {
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
            gPairingParameters.centralKeys = pConnectionEvent->eventData.pairingEvent.centralKeys;
#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
            if (RepeatedAttempts_CheckRequest(maPeerDeviceOriginalAddress) == TRUE)
            {
                (void)Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
            }
            else
            {
                (void)Gap_RejectPairing(peerDeviceId, gRepeatedAttempts_c);
            }
#else
            (void)Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
#endif /* gRepeatedAttempts_d */
#else
            (void)Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))

        case gConnEvtPasskeyRequest_c:
        {
            /* Depending on the IO capabilities, the peripheral may be required to input a passkey.
               The passkey should be read here, e.g., from UART/keyboard and passed to the stack by
               calling Gap_EnterPasskey */
               ;
        }
        break;

        case gConnEvtKeyExchangeRequest_c:
        {
            gapSmpKeys_t sentSmpKeys = gSmpKeys;

            if ((pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & (uint8_t)gLtk_c) == 0U)
            {
                sentSmpKeys.aLtk = NULL;
                /* When the LTK is NULL EDIV and Rand are not sent and will be ignored. */
            }

            if ((pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & (uint8_t)gIrk_c) == 0U)
            {
                sentSmpKeys.aIrk = NULL;
                /* When the IRK is NULL the Address and Address Type are not sent and will be ignored. */
            }

            if ((pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & (uint8_t)gCsrk_c) == 0U)
            {
                sentSmpKeys.aCsrk = NULL;
            }

            (void)Gap_SendSmpKeys(peerDeviceId, &sentSmpKeys);
        }
        break;

        case gConnEvtKeysReceived_c:
        {
            /* Copy peer device address information when IRK is used */
            if (pConnectionEvent->eventData.keysReceivedEvent.pKeys->aIrk != NULL)
            {
#if gAppUseBonding_d
                mPeerDeviceAddressType = pConnectionEvent->eventData.keysReceivedEvent.pKeys->addressType;
#endif /* gAppUseBonding_d */
                FLib_MemCpy(maPeerDeviceAddress, pConnectionEvent->eventData.keysReceivedEvent.pKeys->aAddress, sizeof(bleDeviceAddress_t));
            }
        }
        break;

        case gConnEvtPairingComplete_c:
        {
#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
            RepeatedAttempts_LogAttempt(&pConnectionEvent->eventData.pairingCompleteEvent, maPeerDeviceOriginalAddress);
#endif
#if gAppUseBonding_d
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful &&
                pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.withBonding)
            {
                /* If a bond is created, write device address in controller's White List */
                (void)Gap_AddDeviceToWhiteList(mPeerDeviceAddressType, maPeerDeviceAddress);
#if gAppUsePrivacy_d
                (void)BleConnManager_ManagePrivacyInternal(TRUE);
#endif
            }
#endif /* gAppUseBonding_d */

            if (gPairingParameters.leSecureConnectionSupported)
            {
                pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful ?
                    mSuccessfulPairings++ : mFailedPairings++;

                /* Apply recommendations to change key pair after a number of attempts */
                if ((mFailedPairings >= gConnPairFailChangeKeyThreshold_d) ||
                    (mSuccessfulPairings >= gConnPairSuccessChangeKeyThreshold_d) ||
                    (((mFailedPairings * gConnPairFailToSucessCount_c) + mSuccessfulPairings) >=
                        gConnPairSuccessChangeKeyThreshold_d))
                {
                    (void)Gap_LeScRegeneratePublicKey();
                }
            }
        }
        break;

        case    gConnEvtLongTermKeyRequest_c:
        {
            if ((pConnectionEvent->eventData.longTermKeyRequestEvent.ediv == gSmpKeys.ediv) &&
                (pConnectionEvent->eventData.longTermKeyRequestEvent.randSize == gSmpKeys.cRandSize) &&
                (TRUE == FLib_MemCmp(pConnectionEvent->eventData.longTermKeyRequestEvent.aRand, gSmpKeys.aRand, gSmpKeys.cRandSize)))
            {
                /* EDIV and RAND both matched */
                (void)Gap_ProvideLongTermKey(peerDeviceId, gSmpKeys.aLtk, gSmpKeys.cLtkSize);
            }
            else
            /* EDIV or RAND size did not match */
            {
                (void)Gap_DenyLongTermKey(peerDeviceId);
            }
        }
        break;

        case gConnEvtLeScDisplayNumericValue_c:
        {
            (void) pConnectionEvent->eventData.numericValueForDisplay;
            /* Display on a screen for user confirmation then validate/invalidate based on value. */
            (void)Gap_LeScValidateNumericValue(peerDeviceId, TRUE);
        }
        break;

        case gConnEvtPairingNoLtk_c:
        {
#ifndef gCentralInitiatedPairing_d
            /* Slave does not have the LTK for peer Master. Re-initiate pairing. */
            (void)Gap_SendSlaveSecurityRequest(peerDeviceId, &gPairingParameters);
#endif
        }
        break;

#endif /* gAppUsePairing_d */

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief  Performs GAP Central operations on the BLE stack on the connection callback.
*
* \param[in] peerDeviceId      The GAP peer Id.
*
* \param[in] pConnectionEvent  GAP Connection event from the Host Stack.
*
* \return  none
*
********************************************************************************** */
void BleConnManager_GapCentralEvent(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
            FLib_MemCpy(maPeerDeviceOriginalAddress, pConnectionEvent->eventData.connectedEvent.peerAddress, sizeof(bleDeviceAddress_t));
#endif
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
            /* Copy peer device address information */
            mPeerDeviceAddressType = pConnectionEvent->eventData.connectedEvent.peerAddressType;
            FLib_MemCpy(maPeerDeviceAddress, pConnectionEvent->eventData.connectedEvent.peerAddress, sizeof(bleDeviceAddress_t));
#endif /* gAppUseBonding_d */
#endif /* gAppUsePairing_d */
#if gConnUpdateAlwaysAccept_d
            (void)Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
#endif
            /* Initiate Data Length Update Procedure */
            BleConnManager_DataLengthUpdateProcedure(peerDeviceId);
#if gConnInitiatePhyUpdateRequest_c
            if ((mSupportedFeatures & ((uint32_t)gLe2MbPhy_c | (uint32_t)gLeCodedPhy_c)) != 0U)
            {
                (void)Gap_LeSetPhy(FALSE, peerDeviceId, 0, gConnPhyUpdateReqTxPhySettings_c, gConnPhyUpdateReqRxPhySettings_c, (uint16_t)gConnPhyUpdateReqPhyOptions_c);
            }
#endif
        }
        break;

        case gConnEvtParameterUpdateRequest_c:
        {
#if !gConnUpdateAlwaysAccept_d
            gapConnParamsUpdateReq_t *pUpdateReq = &pConnectionEvent->eventData.connectionUpdateRequest;
            bool_t result;

            /* Check values match the configured intervals */
            result = (pUpdateReq->intervalMin > gConnUpdateIntervalMin_d) &&
                     (pUpdateReq->intervalMax < gConnUpdateIntervalMax_d) &&
                     (pUpdateReq->slaveLatency > gConnUpdateLatencyMin_d) &&
                     (pUpdateReq->slaveLatency < gConnUpdateLatencyMax_d) &&
                     (pUpdateReq->timeoutMultiplier > gConnUpdateSuperTimeoutMin_d) &&
                     (pUpdateReq->timeoutMultiplier < gConnUpdateSuperTimeoutMax_d);

            (void)Gap_EnableUpdateConnectionParameters(peerDeviceId, result);
#else
            (void)Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
#endif
        }
        break;

        case gConnEvtSlaveSecurityRequest_c:
        {
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
            bool_t isBonded = FALSE;
            uint8_t nvmIndex = gInvalidNvmIndex_c;

            if ((gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded, &nvmIndex)) &&
                (isBonded))
            {
                (void)Gap_EncryptLink(peerDeviceId);
            }
            else
            {
#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
                if (RepeatedAttempts_CheckRequest(maPeerDeviceOriginalAddress) == TRUE)
                {
                    (void)Gap_Pair(peerDeviceId, &gPairingParameters);
                }
                else
                {
                    (void)Gap_RejectPairing(peerDeviceId, gRepeatedAttempts_c);
                }
#else
                (void)Gap_Pair(peerDeviceId, &gPairingParameters);
#endif /* gRepeatedAttempts_d */
            }
#else
            (void)Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
#if defined(gPasskeyValue_c)
        case gConnEvtPasskeyRequest_c:
            (void)Gap_EnterPasskey(peerDeviceId, gPasskeyValue_c);
            break;
#endif

        case gConnEvtPasskeyDisplay_c:
        {
            ; /* Display on a screen or simply ignore */
        }
        break;

        case gConnEvtLeScDisplayNumericValue_c:
        {
            (void)pConnectionEvent->eventData.numericValueForDisplay;
            /* Display on a screen for user confirmation then validate/invalidate based on value. */
            (void)Gap_LeScValidateNumericValue(peerDeviceId, TRUE);
        }
        break;

        case gConnEvtKeyExchangeRequest_c:
        {
            gapSmpKeys_t sentSmpKeys = gSmpKeys;

            if ((pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & (uint8_t)gLtk_c) == 0U)
            {
                sentSmpKeys.aLtk = NULL;
                /* When the LTK is NULL EDIV and Rand are not sent and will be ignored. */
            }

            if ((pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & (uint8_t)gIrk_c) == 0U)
            {
                sentSmpKeys.aIrk = NULL;
                /* When the IRK is NULL the Address and Address Type are not sent and will be ignored. */
            }

            if ((pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & (uint8_t)gCsrk_c) == 0U)
            {
                sentSmpKeys.aCsrk = NULL;
            }

            (void)Gap_SendSmpKeys(peerDeviceId, &sentSmpKeys);
        }
        break;

        case gConnEvtKeysReceived_c:
        {
            /* Copy peer device address information when IRK is used */
            if (pConnectionEvent->eventData.keysReceivedEvent.pKeys->aIrk != NULL)
            {
#if gAppUseBonding_d
                mPeerDeviceAddressType = pConnectionEvent->eventData.keysReceivedEvent.pKeys->addressType;
#endif /* gAppUseBonding_d */
                FLib_MemCpy(maPeerDeviceAddress, pConnectionEvent->eventData.keysReceivedEvent.pKeys->aAddress, sizeof(bleDeviceAddress_t));
            }
        }
        break;

        case gConnEvtEncryptionChanged_c:
        {
            ; /* Display on a screen or simply ignore */
        }
        break;

        case gConnEvtPairingComplete_c:
        {
#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))
            RepeatedAttempts_LogAttempt(&pConnectionEvent->eventData.pairingCompleteEvent, maPeerDeviceOriginalAddress);
#endif
#if gAppUseBonding_d
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful &&
                pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.withBonding)
            {
                /* If a bond is created, write device address in controller's White List */
                (void)Gap_AddDeviceToWhiteList(mPeerDeviceAddressType, maPeerDeviceAddress);
#if gAppUsePrivacy_d
                (void)BleConnManager_ManagePrivacyInternal(TRUE);
#endif
            }

#endif /* gAppUseBonding_d */

            if (gPairingParameters.leSecureConnectionSupported)
            {
                pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful ?
                    mSuccessfulPairings++ : mFailedPairings++;

                /* Apply recommendations to change key pair after a number of attempts */
                if ((mFailedPairings >= gConnPairFailChangeKeyThreshold_d) ||
                    (mSuccessfulPairings >= gConnPairSuccessChangeKeyThreshold_d) ||
                    (((mFailedPairings * gConnPairFailToSucessCount_c) + mSuccessfulPairings) >=
                        gConnPairSuccessChangeKeyThreshold_d))
                {
                    (void)Gap_LeScRegeneratePublicKey();
                }
            }
        }
        break;
#endif /* gAppUsePairing_d */

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

/*! *********************************************************************************
* \brief  Checks bonding devices and enables controller/host privacy.
*
* \param[in] none
*
* \return    gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t BleConnManager_EnablePrivacy(void)
{
#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    bleResult_t result = gBleSuccess_c;

    if( FALSE == mbPrivacyEnabled )
    {
        result = BleConnManager_ManagePrivacyInternal(FALSE);
    }

    return result;
#else
    return gBleFeatureNotSupported_c;
#endif
}

/*! *********************************************************************************
* \brief  Checks enabled privacy and disables controller/host privacy.
*
* \param[in] none
*
* \return    gBleSuccess_c or error.
*
********************************************************************************** */
bleResult_t BleConnManager_DisablePrivacy(void)
{
#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    bleResult_t result = gBleSuccess_c;

    if( mbPrivacyEnabled )
    {
#if defined(gBleEnableControllerPrivacy_d) && (gBleEnableControllerPrivacy_d > 0)
        if( mcDevicesInResolvingList == 0U )
        {
            result = Gap_EnableHostPrivacy(FALSE, NULL);
        }
        else
        {
            result = Gap_EnableControllerPrivacy(FALSE, NULL, 0, NULL);
        }
#else
        result = Gap_EnableHostPrivacy(FALSE, NULL);
#endif

        if( gBleSuccess_c == result )
        {
            mbPrivacyEnabled = FALSE;
        }
    }

    return result;
#else
    return gBleFeatureNotSupported_c;
#endif
}

void BleConnManager_GapCommonConfig(void)
{
    /* Read public address from controller */
    (void)Gap_ReadPublicDeviceAddress();

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
    /* Register security requirements if pairing is used */
    (void)Gap_RegisterDeviceSecurityRequirements(&deviceSecurityRequirements);

#ifdef gPasskeyValue_c
    /*
     * Set local passkey. If not defined, passkey will be generated random in SMP.
     * Do not set local passkey if device IO capabilities are gIoKeyboardOnly_c.
     * It will need to be input during the pairing process.
     */
    if (gPairingParameters.localIoCapabilities != gIoKeyboardOnly_c)
    {
        (void)Gap_SetLocalPasskey(gPasskeyValue_c);
    }
#endif /* gPasskeyValue_c */

#endif /* gAppUsePairing_d */

    /* Populate White List if bonding is supported */
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    gapIdentityInformation_t aIdentity[gMaxBondedDevices_c];
    bleResult_t result = Gap_GetBondedDevicesIdentityInformation(aIdentity, gMaxBondedDevices_c, &gcBondedDevices);

    if (gBleSuccess_c == result && gcBondedDevices > 0U)
    {
        for (uint8_t i = 0; i < gcBondedDevices; i++)
        {
            (void)Gap_AddDeviceToWhiteList(aIdentity[i].identityAddress.idAddressType, aIdentity[i].identityAddress.idAddress);
        }
    }
#endif

#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U))
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    (void)BleConnManager_ManagePrivacyInternal(FALSE);
#else
    /* Use Non-Resolvable Private Addresses */
    (void)Gap_EnableHostPrivacy(TRUE, NULL);
#endif /* gAppUseBonding_d */
#endif /* gAppUsePrivacy_d */
    if ((mSupportedFeatures & ((uint32_t)gLe2MbPhy_c | (uint32_t)gLeCodedPhy_c)) != 0U)
    {
        (void)Gap_LeSetPhy(TRUE, 0, 0, gConnDefaultTxPhySettings_c, gConnDefaultRxPhySettings_c, 0);
    }
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  Generates LTK, IRK, CSRK, ediv and rand
*
* \param[in] none
*
* \return none
*
********************************************************************************** */
STATIC void BleConnManager_MCUInfoToSmpKeys(void)
{
    uint8_t uid[16] = {0};
    uint8_t len = 0;
    uint8_t sha256Output[SHA256_HASH_SIZE];

    BOARD_GetMCUUid (uid, &len);

    if(len > 0U)
    {
        /* generate LTK. LTK size always smaller than SHA1 hash size */
        uid[len - 1U]++;
        SHA256_Hash (uid, len, sha256Output);
        FLib_MemCpy (gSmpKeys.aLtk, sha256Output, gSmpKeys.cLtkSize);

#if (!defined(gUseCustomIRK_d) || (gUseCustomIRK_d == 0U))
        /* generate IRK */
        uid[len - 1U]++;
        SHA256_Hash (uid, len, sha256Output);
        FLib_MemCpy (gSmpKeys.aIrk, sha256Output, gcSmpIrkSize_c);
#endif

        /* generate CSRK */
        uid[len - 1U]++;
        SHA256_Hash (uid, len, sha256Output);
        FLib_MemCpy (gSmpKeys.aCsrk, sha256Output, gcSmpCsrkSize_c);

        /* generate ediv and rand */
        uid[len - 1U]++;
        SHA256_Hash (uid, len, sha256Output);
        gSmpKeys.ediv = (uint16_t)sha256Output[0];
        FLib_MemCpy (&(gSmpKeys.ediv), &(sha256Output[0]), sizeof(gSmpKeys.ediv));
        FLib_MemCpy (gSmpKeys.aRand, &(sha256Output[sizeof(gSmpKeys.ediv)]), gSmpKeys.cRandSize);
    }
}

/*! *********************************************************************************
* \brief  Initiates Data Length Update Procedure
*
* \param[in] peerDeviceId      The GAP peer Id.
*
* \return  none
*
********************************************************************************** */
STATIC void BleConnManager_DataLengthUpdateProcedure(deviceId_t peerDeviceId)
{
    if ((mSupportedFeatures & (uint32_t)gLeDataPacketLengthExtension_c) != 0U)
    {
        /* Check for Coded PHY support */
        bool_t codedPhySupported = (mSupportedFeatures & (uint32_t)gLeCodedPhy_c) != 0U;

        (void)Gap_UpdateLeDataLength(peerDeviceId, gBleMaxTxOctets_c,
                                     codedPhySupported ? gBleMaxTxTimeCodedPhy_c : gBleMaxTxTime_c);
    }
}

#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))

/*! *********************************************************************************
* \brief  Manage privacy for host or controller based on the available bonding information
*
* \param[in] bCheckNewBond     If FALSE: perform a Privacy list update and restart the
*                              Privacy feature.
*                              If TRUE: the Privacy list update and the restart of the
*                              Privacy feature will be performed only if a new bond was
*                              created.
*
* \return  gBleSuccess_c or error.
********************************************************************************** */
STATIC bleResult_t BleConnManager_ManagePrivacyInternal(bool_t bCheckNewBond)
{
    gapIdentityInformation_t *pOutIdentityAddresses = NULL;
    uint8_t                  identitiesCount = 0;
    bleResult_t              result = gBleSuccess_c;

    pOutIdentityAddresses = (gapIdentityInformation_t *)MEM_BufferAlloc((uint32_t)gMaxResolvingListSize_c * sizeof(gapIdentityInformation_t));

    if( NULL != pOutIdentityAddresses )
    {
        /* Get the bonded devices with a valid IRK, the count of them will be stored in the 'identitiesCount' variable */
        result = Gap_GetBondedDevicesIdentityInformation(pOutIdentityAddresses,
                                                         gMaxResolvingListSize_c,
                                                         &identitiesCount);

        if((gBleSuccess_c == result) && (identitiesCount > 0U ))
        {
            /* Check if it's the case of a new bond or if the privacy list needs to be
               updated and Privacy restarted due to other reasons, like bond(s) removal */
            if ((identitiesCount == mcDevicesInResolvingList + 1U ) || (bCheckNewBond == FALSE))
            {
                /* Disable Privacy */
                (void)BleConnManager_DisablePrivacy();

                /* Update the count of devices from the resolving list based on how many bonds currently are */
                mcDevicesInResolvingList = identitiesCount;

                /* Check not to pass over the maximum allowed resolving list size */
                if( mcDevicesInResolvingList > (uint8_t)gMaxResolvingListSize_c )
                {
                    mcDevicesInResolvingList = gMaxResolvingListSize_c;
                }

                /* Create Random Address if none, the generated address will be set
                   after gRandomAddressReady_c generic event is received.
                   The Host always needs a Random Address for cases where a central is
                   connecting with an unbonded peripheral - if the device starts directly
                   in Controller Privacy that address is created here. */
                if (FALSE == mHaveRandomAddress)
                {
                    mSettingRandomAddressFromApplication = TRUE;
                    (void)Gap_CreateRandomDeviceAddress(gSmpKeys.aIrk, NULL);
                }
#if defined(gBleEnableControllerPrivacy_d) && (gBleEnableControllerPrivacy_d > 0)
                /* Enable Controller Privacy: fill the Controller Resolution Address List with
                   the Identity Addresses and enable Address Resolution */
                result = Gap_EnableControllerPrivacy(TRUE, gSmpKeys.aIrk, mcDevicesInResolvingList, pOutIdentityAddresses);
#else
                /* Enable Host Privacy */
                result = Gap_EnableHostPrivacy(TRUE, gSmpKeys.aIrk);
#endif
            }
            else
            {
                /* This is not a new bond, leave privacy as is */
            }
        }
        else
        {
            /* No bonds with valid IRK were found, reset the number of the devices from the Resolving List */
            mcDevicesInResolvingList = 0;
            /* Enable Host Privacy */
            result = Gap_EnableHostPrivacy(TRUE, gSmpKeys.aIrk);
        }

        (void)MEM_BufferFree(pOutIdentityAddresses);

        if( gBleSuccess_c == result )
        {
            mbPrivacyEnabled = TRUE;
        }
    }
    else
    {
        result = gBleOutOfMemory_c;
    }

    return result;
}
#endif /* (gAppUsePrivacy_d) && (gAppUseBonding_d) */

#if (defined(gRepeatedAttempts_d) && (gRepeatedAttempts_d == 1U))

/*! *********************************************************************************
* \brief  Check if a pairing/slave security request should be accepted.
*
* \param[in] address    The address of the peer.
*
* \return  TRUE if ok
           FALSE if this is a repeated attempt and timeout has not expired.
********************************************************************************** */
STATIC bool_t RepeatedAttempts_CheckRequest(bleDeviceAddress_t address)
{
    bool_t okToProceed = TRUE;
    uint8_t iCount;

    /* Check if this device has been involved in a failed pairing and is still in the waiting period */
    for (iCount = 0; iCount < (uint8_t)gRepeatedAttemptsNoOfDevices_c; iCount++)
    {
        if (FLib_MemCmp(maPairingPeers[iCount].address, address, gcBleDeviceAddressSize_c))
        {
            if (maPairingPeers[iCount].remainingTime != 0U)
            {
                okToProceed = FALSE;
                break;
            }
        }
    }

    return okToProceed;
}

/*! *********************************************************************************
* \brief  Track a pairing attempt.
*
* \param[in] pEvent     Pointer to the pairing complete event.
* \param[in] address    Address of the peer.
*
********************************************************************************** */
STATIC void RepeatedAttempts_LogAttempt(gapPairingCompleteEvent_t *pEvent, bleDeviceAddress_t address)
{
    bool_t found = FALSE;
    uint8_t iCount;
    uint8_t emptySlotIndex = 0xFF, indexToReplace = 0;
    uint16_t maxTimeout = 0;

    /* Stop timer if it is running */
    if ((mRepeatedAttemptsTimerId != gTmrInvalidTimerID_c) && TMR_IsTimerActive(mRepeatedAttemptsTimerId))
    {
        (void)TMR_StopTimer(mRepeatedAttemptsTimerId);
        RepeatedAttempts_UpdateTimePassed(mMinTimeToWait - TMR_GetRemainingTime(mRepeatedAttemptsTimerId)/1000U);
    }

    /* Check if this peer was in the repeated attempts array */
    for (iCount = 0; iCount < (uint8_t)gRepeatedAttemptsNoOfDevices_c; iCount++)
    {
        if (FLib_MemCmp(maPairingPeers[iCount].address, address, gcBleDeviceAddressSize_c))
        {
            found = TRUE;
            if (pEvent->pairingSuccessful == TRUE)
            {
                /* On successful pairing remove device from array */
                FLib_MemSet(&maPairingPeers[iCount], 0x00U, sizeof(repeatedAttemptsDevice_t));
            }
            else
            {
                /* On failed pairing, double waiting period */
                maPairingPeers[iCount].baseTimeout = FLib_GetMin(2U * maPairingPeers[iCount].baseTimeout,
                                                                 gRepeatedAttemptsTimeoutMax_c);
                maPairingPeers[iCount].remainingTime = maPairingPeers[iCount].baseTimeout;
            }
            break;
        }

        /* Memorize first empty slot if this peer is a new device which must be added */
        if ((maPairingPeers[iCount].baseTimeout == 0U) && (emptySlotIndex == 0xFF))
        {
            emptySlotIndex = iCount;
        }

        /* Memorize the longest waiting device in the array if this peer is a new device which must be added and the array is full */
        if (maPairingPeers[iCount].remainingTime > maxTimeout)
        {
            maxTimeout = maPairingPeers[iCount].remainingTime;
            indexToReplace = iCount;
        }
    }

    /* If this is a new peer add it to the array - replace oldest entry if full */
    if ((found == FALSE) && (pEvent->pairingSuccessful == FALSE))
    {
        uint8_t index;

        if (emptySlotIndex != 0xFF)
        {
            index = emptySlotIndex;
        }
        else
        {
            index = indexToReplace;
        }

        FLib_MemCpy(maPairingPeers[index].address, address, gcBleDeviceAddressSize_c);
        maPairingPeers[index].baseTimeout   = gRepeatedAttemptsTimeoutMin_c;
        maPairingPeers[index].remainingTime = gRepeatedAttemptsTimeoutMin_c;
    }

    /* Restart timer */
    mMinTimeToWait = RepeatedAttempts_GetMinTimeToWait();

    if (mMinTimeToWait < (gRepeatedAttemptsTimeoutMax_c + 1U))
    {
        if (mRepeatedAttemptsTimerId == gTmrInvalidTimerID_c)
        {
            mRepeatedAttemptsTimerId = TMR_AllocateTimer();
        }

        if (mRepeatedAttemptsTimerId != gTmrInvalidTimerID_c)
        {

            (void)TMR_StartLowPowerTimer(mRepeatedAttemptsTimerId, gTmrLowPowerSingleShotMillisTimer_c,
                                        TmrSeconds(mMinTimeToWait), RepeatedAttempts_TimerCb, NULL);
        }
    }
}

/*! *********************************************************************************
* \brief  Update array waiting times after stopping timer.
*
* \param[in] seconds    Seconds to subtract from waiting times.
*
********************************************************************************** */
STATIC void RepeatedAttempts_UpdateTimePassed(uint16_t seconds)
{
    uint8_t iCount;

    for (iCount = 0; iCount < (uint8_t)gRepeatedAttemptsNoOfDevices_c; iCount++)
    {
        if (maPairingPeers[iCount].remainingTime > seconds)
        {
            maPairingPeers[iCount].remainingTime -= seconds;
        }
        else
        {
            maPairingPeers[iCount].remainingTime = 0;
        }
    }
}

/*! *********************************************************************************
* \brief  Return minimum waiting time from array.
*
* \param[in] none
*
* \return  Minimum remaining wait time in the array.
********************************************************************************** */
STATIC uint16_t RepeatedAttempts_GetMinTimeToWait()
{
    uint8_t iCount;
    uint16_t minRemainingTime = gRepeatedAttemptsTimeoutMax_c + 1U;

    for (iCount = 0; iCount < (uint8_t)gRepeatedAttemptsNoOfDevices_c; iCount++)
    {
        if ((maPairingPeers[iCount].remainingTime != 0) &&
            (maPairingPeers[iCount].remainingTime < minRemainingTime))
        {
            minRemainingTime = maPairingPeers[iCount].remainingTime;
        }
    }

    return minRemainingTime;
}

/*! *********************************************************************************
* \brief        Handles advertising timer callback.
*
* \param[in]    pParam        Callback parameters.
********************************************************************************** */
STATIC void RepeatedAttempts_TimerCb(void *param)
{
    RepeatedAttempts_UpdateTimePassed(mMinTimeToWait);

    mMinTimeToWait = RepeatedAttempts_GetMinTimeToWait();

    if (mMinTimeToWait < (gRepeatedAttemptsTimeoutMax_c + 1U))
    {
        (void)TMR_StartLowPowerTimer(mRepeatedAttemptsTimerId, gTmrLowPowerSingleShotMillisTimer_c,
                                     TmrSeconds(mMinTimeToWait), RepeatedAttempts_TimerCb, NULL);
    }
    else
    {
        TMR_FreeTimer(mRepeatedAttemptsTimerId);
        mRepeatedAttemptsTimerId = gTmrInvalidTimerID_c;
    }
}
#endif /* gRepeatedAttempts_d */
/*! *********************************************************************************
* @}
********************************************************************************** */
