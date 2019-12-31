/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright 2016-2019 NXP
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
#include "ble_config.h"

#include "board.h"
#include "MemManager.h"
#include "SecLib.h"

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
#endif
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
STATIC void BleConnManager_MCUInfoToSmpKeys(void);
STATIC void BleConnManager_DataLengthUpdateProcedure(deviceId_t peerDeviceId);
#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
STATIC bleResult_t BleConnManager_EnablePrivacyInternal(bool_t bCheckNewBond);
#endif
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static bleDeviceAddress_t   maBleDeviceAddress;
STATIC uint32_t             mSupportedFeatures;

#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
uint8_t gcBondedDevices = 0;
#if gAppUsePrivacy_d
STATIC uint8_t mcDevicesInResolvingList = 0;
STATIC bool_t  mbPrivacyEnabled = FALSE;
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

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

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

        case gInternalError_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
        {
            ; /* No action required */
        }
        break;
    }
}

void BleConnManager_GapPeripheralEvent(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
#if (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
            bool_t isBonded = FALSE;

            /* Copy peer device address information */
            mPeerDeviceAddressType = pConnectionEvent->eventData.connectedEvent.peerAddressType;
            FLib_MemCpy(maPeerDeviceAddress, pConnectionEvent->eventData.connectedEvent.peerAddress, sizeof(bleDeviceAddress_t));

            /* Perform pairing if peer is not bonded or resolution procedure for its address failed */
            if ((gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded) && FALSE == isBonded) ||
                (Ble_IsPrivateResolvableDeviceAddress(maPeerDeviceAddress) &&
                    FALSE == pConnectionEvent->eventData.connectedEvent.peerRpaResolved))
#endif
            {
                (void)Gap_SendSlaveSecurityRequest(peerDeviceId, &gPairingParameters);
            }
#endif

#if gConnUpdateAlwaysAccept_d
            (void)Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
#endif
            /* Initiate Data Length Update Procedure */
            BleConnManager_DataLengthUpdateProcedure(peerDeviceId);
        }
        break;

        case gConnEvtPairingRequest_c:
        {
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
            gPairingParameters.centralKeys = pConnectionEvent->eventData.pairingEvent.centralKeys;
            (void)Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
#else
            (void)Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;

#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
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
            break;
        }

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
#if gAppUseBonding_d
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful &&
                pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.withBonding)
            {
                /* If a bond is created, write device address in controller's White List */
                (void)Gap_AddDeviceToWhiteList(mPeerDeviceAddressType, maPeerDeviceAddress);
#if gAppUsePrivacy_d
                (void)BleConnManager_DisablePrivacy();
                (void)BleConnManager_EnablePrivacyInternal(TRUE);
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

        case gConnEvtLongTermKeyRequest_c:
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

#endif /* gAppUsePairing_d */

        default:
        {
            ; /* No action required */
        }
        break;
    }
}


void BleConnManager_GapCentralEvent(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
#if (defined(gAppUsePairing_d) && (gAppUsePairing_d == 1U))
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

            if ((gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded)) &&
                (isBonded))
            {
                (void)Gap_EncryptLink(peerDeviceId);
            }
            else
            {
                (void)Gap_Pair(peerDeviceId, &gPairingParameters);
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
            /* Display on a screen or simply ignore */
            break;

        case gConnEvtLeScDisplayNumericValue_c:
            (void)pConnectionEvent->eventData.numericValueForDisplay;
            /* Display on a screen for user confirmation then validate/invalidate based on value. */
            (void)Gap_LeScValidateNumericValue(peerDeviceId, TRUE);
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
            break;
        }

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
        }
        break;

        case gConnEvtPairingComplete_c:
        {
#if gAppUseBonding_d
            if (pConnectionEvent->eventData.pairingCompleteEvent.pairingSuccessful &&
                pConnectionEvent->eventData.pairingCompleteEvent.pairingCompleteData.withBonding)
            {
                /* If a bond is created, write device address in controller's White List */
                (void)Gap_AddDeviceToWhiteList(mPeerDeviceAddressType, maPeerDeviceAddress);
#if gAppUsePrivacy_d
                (void)BleConnManager_DisablePrivacy();
                (void)BleConnManager_EnablePrivacyInternal(TRUE);
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

bleResult_t BleConnManager_EnablePrivacy(void)
{
#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    bleResult_t result = gBleSuccess_c;

    if( FALSE == mbPrivacyEnabled )
    {
        result = BleConnManager_EnablePrivacyInternal(FALSE);
    }

    return result;
#else
    return gBleFeatureNotSupported_c;
#endif
}

bleResult_t BleConnManager_DisablePrivacy(void)
{
#if (defined(gAppUsePrivacy_d) && (gAppUsePrivacy_d == 1U)) && (defined(gAppUseBonding_d) && (gAppUseBonding_d == 1U))
    bleResult_t result = gBleSuccess_c;

    if( mbPrivacyEnabled )
    {
        if( mcDevicesInResolvingList == 0U )
        {
            result = Gap_EnableHostPrivacy(FALSE, NULL);
        }
        else
        {
            result = Gap_EnableControllerPrivacy(FALSE, NULL, 0, NULL);
        }

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
    /* Set local passkey. If not defined, passkey will be generated random in SMP */
    (void)Gap_SetLocalPasskey(gPasskeyValue_c);
#endif

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
    (void)BleConnManager_EnablePrivacyInternal(FALSE);
#else
    /* Use Non-Resolvable Private Addresses */
    (void)Gap_EnableHostPrivacy(TRUE, NULL);
#endif /* gAppUseBonding_d */
#endif /* gAppUsePrivacy_d */

#ifdef gBleUseHSClock2MbpsPhy_c
#if gBleUseHSClock2MbpsPhy_c
    (void)Gap_LeSetPhy(TRUE, 0, 0, gLeTxPhy1MFlag_c | gLeTxPhy2MFlag_c, gLeRxPhy1MFlag_c | gLeRxPhy2MFlag_c, 0);
#else
    /* If HS Clock is not enabled, set only 1M Phy as preference */
    (void)Gap_LeSetPhy(TRUE, 0, 0, gLeTxPhy1MFlag_c, gLeRxPhy1MFlag_c, 0);
#endif
#endif
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
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

        /* generate IRK */
        uid[len - 1U]++;
        SHA256_Hash (uid, len, sha256Output);
        FLib_MemCpy (gSmpKeys.aIrk, sha256Output, gcSmpIrkSize_c);

        /* generate CRSK */
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
STATIC bleResult_t BleConnManager_EnablePrivacyInternal(bool_t bCheckNewBond)
{
    gapIdentityInformation_t *pOutIdentityAddresses = NULL;
    uint8_t                  identitiesCount = 0;
    bleResult_t              result = gBleSuccess_c;

    pOutIdentityAddresses = (gapIdentityInformation_t *)MEM_BufferAlloc((uint32_t)gMaxResolvingListSize_c * sizeof(gapIdentityInformation_t));

    if( NULL != pOutIdentityAddresses )
    {
        result = Gap_GetBondedDevicesIdentityInformation(pOutIdentityAddresses,
                                                         gMaxResolvingListSize_c,
                                                         &identitiesCount);

        if( gBleSuccess_c == result && identitiesCount > 0U )
        {
            if( bCheckNewBond )
            {
                if( identitiesCount != mcDevicesInResolvingList + 1U )
                {
                    /* This is not a new bond, leave privacy as is */
                    (void)MEM_BufferFree(pOutIdentityAddresses);
                    return gBleSuccess_c;
                }
            }

            mcDevicesInResolvingList = identitiesCount;

            if( mcDevicesInResolvingList > (uint8_t)gMaxResolvingListSize_c )
            {
                mcDevicesInResolvingList = gMaxResolvingListSize_c;
            }

            result = Gap_EnableControllerPrivacy(TRUE, gSmpKeys.aIrk, mcDevicesInResolvingList, pOutIdentityAddresses);
        }
        else
        {
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

/*! *********************************************************************************
* @}
********************************************************************************** */
