/*! *********************************************************************************
 * \addtogroup BLE
 * @{
 ********************************************************************************** */
/*!
* Copyright 2016-2018 NXP
* All rights reserved.
*
* file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

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

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
static void BleConnManager_GapCommonConfig(void);
static void BleConnManager_MCUInfoToSmpKeys(void);
#if gAppUsePrivacy_d
static bleResult_t BleConnManager_EnablePrivacyInternal(bool_t bCheckNewBond);
#endif
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static bleDeviceAddress_t   maBleDeviceAddress;

#if gAppUseBonding_d
uint8_t gcBondedDevices = 0;
#if gAppUsePrivacy_d
static uint8_t mcDevicesInResolvingList = 0;
static bool_t  mbPrivacyEnabled = FALSE;
#endif /* gAppUsePrivacy_d */
#endif /* gAppUseBonding_d */

#if gAppUsePairing_d
static bleDeviceAddress_t   maPeerDeviceAddress;
static bleAddressType_t     mPeerDeviceAddressType;
static uint8_t              mSuccessfulPairings;
static uint8_t              mFailedPairings;
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
            BleConnManager_MCUInfoToSmpKeys();
        }
        break;
        case gPublicAddressRead_c:
        {
            /* Use address read from the controller */
            FLib_MemCpy(maBleDeviceAddress, pGenericEvent->eventData.aAddress, sizeof(bleDeviceAddress_t));
#if gAppUsePairing_d
            gSmpKeys.addressType = gBleAddrTypePublic_c;
            gSmpKeys.aAddress = maBleDeviceAddress;
#endif
        }
        break;

#if gAppUsePairing_d
        case gLeScPublicKeyRegenerated_c:
        {
            /* Key pair regenerated -> reset pairing counters */
            mFailedPairings = mSuccessfulPairings = 0;
        }
        break;
#endif

        case gInternalError_c:
        {
            panic(0,0,0,0);
        }
        break;

        default:
            break;
    }
}

void BleConnManager_GapDualRoleConfig(void)
{
    BleConnManager_GapCommonConfig();

#if gAppUsePrivacy_d
   gAdvParams.peerAddressType = gBleAddrTypeRandom_c;
   gScanParams.ownAddressType = gBleAddrTypeRandom_c;
#endif

    /* Setup Advertising and scanning data */
    Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
}

void BleConnManager_GapPeripheralConfig(void)
{
    BleConnManager_GapCommonConfig();

#if gAppUsePrivacy_d
   gAdvParams.peerAddressType = gBleAddrTypeRandom_c;
#endif

    /* Setup Advertising and scanning data */
    Gap_SetAdvertisingData(&gAppAdvertisingData, &gAppScanRspData);
}

void BleConnManager_GapCentralConfig(void)
{
#if gAppUsePrivacy_d
    gScanParams.ownAddressType = gBleAddrTypeRandom_c;
#endif

    BleConnManager_GapCommonConfig();
}

void BleConnManager_GapPeripheralEvent(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
#if gAppUsePairing_d
#if gAppUseBonding_d
            bool_t isBonded = FALSE;

            /* Copy peer device address information */
            mPeerDeviceAddressType = pConnectionEvent->eventData.connectedEvent.peerAddressType;
            FLib_MemCpy(maPeerDeviceAddress, pConnectionEvent->eventData.connectedEvent.peerAddress, sizeof(bleDeviceAddress_t));

            if (gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded) &&
                FALSE == isBonded)
#endif
            {
                Gap_SendSlaveSecurityRequest(peerDeviceId,
                                gPairingParameters.withBonding,
                                gPairingParameters.securityModeAndLevel);
            }
#endif

#if gConnUpdateAlwaysAccept_d
                Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
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
                            (pUpdateReq->timeoutMultiplier > gConnUpdateIntervalMin_d) &&
                            (pUpdateReq->timeoutMultiplier > gConnUpdateIntervalMin_d);

            Gap_EnableUpdateConnectionParameters(peerDeviceId, result);
#else
            Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
#endif
        }
        break;

        case gConnEvtPairingRequest_c:
        {
#if gAppUsePairing_d
            gPairingParameters.centralKeys = pConnectionEvent->eventData.pairingEvent.centralKeys;
            Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
#else
            Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;

#if gAppUsePairing_d
        case gConnEvtKeyExchangeRequest_c:
        {
            gapSmpKeys_t sentSmpKeys = gSmpKeys;

            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gLtk_c))
            {
                sentSmpKeys.aLtk = NULL;
                /* When the LTK is NULL EDIV and Rand are not sent and will be ignored. */
            }

            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gIrk_c))
            {
                sentSmpKeys.aIrk = NULL;
                /* When the IRK is NULL the Address and Address Type are not sent and will be ignored. */
            }

            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gCsrk_c))
            {
                sentSmpKeys.aCsrk = NULL;
            }

            Gap_SendSmpKeys(peerDeviceId, &sentSmpKeys);
            break;
        }

        case gConnEvtKeysReceived_c:
        {
            /* Copy peer device address information when IRK is used */
            if (pConnectionEvent->eventData.keysReceivedEvent.pKeys->aIrk != NULL)
            {
                mPeerDeviceAddressType = pConnectionEvent->eventData.keysReceivedEvent.pKeys->addressType;
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
                /* If a bond is created, write device address in controller’s White List */
                Gap_AddDeviceToWhiteList(mPeerDeviceAddressType, maPeerDeviceAddress);
#if gAppUsePrivacy_d
                BleConnManager_DisablePrivacy();
                BleConnManager_EnablePrivacyInternal(TRUE);
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
                    Gap_LeScRegeneratePublicKey();
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
                Gap_ProvideLongTermKey(peerDeviceId, gSmpKeys.aLtk, gSmpKeys.cLtkSize);
            }
            else
            /* EDIV or RAND size did not match */
            {
                Gap_DenyLongTermKey(peerDeviceId);
            }
        }
        break;

        case gConnEvtLeScDisplayNumericValue_c:
        {
            (void) pConnectionEvent->eventData.numericValueForDisplay;
            /* Display on a screen for user confirmation then validate/invalidate based on value. */
            Gap_LeScValidateNumericValue(peerDeviceId, TRUE);
        }
        break;

#endif /* gAppUsePairing_d */

    default:
        break;
    }
}


void BleConnManager_GapCentralEvent(deviceId_t peerDeviceId, gapConnectionEvent_t* pConnectionEvent)
{
    switch (pConnectionEvent->eventType)
    {
        case gConnEvtConnected_c:
        {
#if gConnUpdateAlwaysAccept_d
            Gap_EnableUpdateConnectionParameters(peerDeviceId, TRUE);
#endif
        }
    break;
        case gConnEvtSlaveSecurityRequest_c:
        {
#if gAppUsePairing_d
            bool_t isBonded = FALSE;

            if ((gBleSuccess_c == Gap_CheckIfBonded(peerDeviceId, &isBonded)) &&
                (isBonded))
            {
                Gap_EncryptLink(peerDeviceId);
            }
            else
            {
                Gap_Pair(peerDeviceId, &gPairingParameters);
            }
#else
            Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;

        case gConnEvtPairingRequest_c:
        {
#if gAppUsePairing_d
            gPairingParameters.peripheralKeys = pConnectionEvent->eventData.pairingEvent.peripheralKeys;
            Gap_AcceptPairingRequest(peerDeviceId, &gPairingParameters);
#else
            Gap_RejectPairing(peerDeviceId, gPairingNotSupported_c);
#endif
        }
        break;

#if gAppUsePairing_d
        case gConnEvtPasskeyRequest_c:
            Gap_EnterPasskey(peerDeviceId, gPasskeyValue_c);
            break;

        case gConnEvtPasskeyDisplay_c:
            /* Display on a screen or simply ignore */
            break;

        case gConnEvtLeScDisplayNumericValue_c:
            (void) pConnectionEvent->eventData.numericValueForDisplay;
            /* Display on a screen for user confirmation then validate/invalidate based on value. */
            Gap_LeScValidateNumericValue(peerDeviceId, TRUE);
        break;

        case gConnEvtKeyExchangeRequest_c:
        {
            gapSmpKeys_t sentSmpKeys = gSmpKeys;

            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gLtk_c))
            {
                sentSmpKeys.aLtk = NULL;
                /* When the LTK is NULL EDIV and Rand are not sent and will be ignored. */
            }

            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gIrk_c))
            {
                sentSmpKeys.aIrk = NULL;
                /* When the IRK is NULL the Address and Address Type are not sent and will be ignored. */
            }

            if (!(pConnectionEvent->eventData.keyExchangeRequestEvent.requestedKeys & gCsrk_c))
            {
                sentSmpKeys.aCsrk = NULL;
            }

            Gap_SendSmpKeys(peerDeviceId, &sentSmpKeys);
            break;
        }

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
                /* If a bond is created, write device address in controller’s White List */
                Gap_AddDeviceToWhiteList(mPeerDeviceAddressType, maPeerDeviceAddress);
#if gAppUsePrivacy_d
                BleConnManager_DisablePrivacy();
                BleConnManager_EnablePrivacyInternal(TRUE);
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
                    Gap_LeScRegeneratePublicKey();
                }
            }
        }
        break;
#endif /* gAppUsePairing_d */

    default:
        break;
    }
}

bleResult_t BleConnManager_EnablePrivacy(void)
{
#if gAppUsePrivacy_d
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
#if gAppUsePrivacy_d
    bleResult_t result = gBleSuccess_c;

    if( mbPrivacyEnabled )
    {
        if( mcDevicesInResolvingList == 0 )
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

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
static void BleConnManager_GapCommonConfig(void)
{
    /* Read public address from controller */
    Gap_ReadPublicDeviceAddress();

#if gAppUsePairing_d
    /* Register security requirements if pairing is used */
    Gap_RegisterDeviceSecurityRequirements(&deviceSecurityRequirements);

#ifdef gPasskeyValue_c
    /* Set local passkey. If not defined, passkey will be generated random in SMP */
    Gap_SetLocalPasskey(gPasskeyValue_c);
#endif

#endif /* gAppUsePairing_d */

    /* Populate White List if bonding is supported */
#if gAppUseBonding_d
    bleDeviceAddress_t aBondedDevAdds[gMaxBondedDevices_c];
    bleResult_t result = Gap_GetBondedStaticAddresses(aBondedDevAdds, gMaxBondedDevices_c, &gcBondedDevices);

    if (gBleSuccess_c == result && gcBondedDevices > 0)
    {
        for (uint8_t i = 0; i < gcBondedDevices; i++)
        {
            Gap_AddDeviceToWhiteList(gBleAddrTypePublic_c, aBondedDevAdds[i]);
        }
    }
#endif

#if gAppUsePrivacy_d
#if gAppUseBonding_d
    BleConnManager_EnablePrivacyInternal(FALSE);
#else
    /* Use Non-Resolvable Private Addresses */
    Gap_EnableHostPrivacy(TRUE, NULL);
#endif /* gAppUseBonding_d */
#endif /* gAppUsePrivacy_d */
}

static void BleConnManager_MCUInfoToSmpKeys(void)
{
    sha256Context_t mCtx;
    uint8_t uid[16] = {0};
    uint8_t len = 0;

    BOARD_GetMCUUid(uid, &len);

    if(len > 0)
    {
        /* generate LTK. LTK size always smaller than SHA1 hash size */
        uid[len - 1]++;
        SHA256_Hash(&mCtx, uid, len);
        FLib_MemCpy(gSmpKeys.aLtk, (uint8_t*)(&mCtx.hash), gSmpKeys.cLtkSize);

        /* generate IRK */
        uid[len - 1]++;
        SHA256_Hash(&mCtx, uid, len);
        FLib_MemCpy(gSmpKeys.aIrk, (uint8_t*)(&mCtx.hash), gcSmpIrkSize_c);

        /* generate CRSK */
        uid[len - 1]++;
        SHA256_Hash(&mCtx, uid, len);
        FLib_MemCpy(gSmpKeys.aCsrk, (uint8_t*)(&mCtx.hash), gcSmpCsrkSize_c);

        /* generate ediv and rand */
        uid[len - 1]++;
        SHA256_Hash(&mCtx, uid, len);
        gSmpKeys.ediv = (uint16_t)mCtx.hash[0];
        FLib_MemCpy(gSmpKeys.aRand, (uint8_t*)&mCtx.hash[1], gSmpKeys.cRandSize);
    }
}

#if gAppUsePrivacy_d
static bleResult_t BleConnManager_EnablePrivacyInternal(bool_t bCheckNewBond)
{
    gapIdentityInformation_t *pOutIdentityAddresses = NULL;
    uint8_t                  identitiesCount = 0;
    bleResult_t              result = gBleSuccess_c;

    pOutIdentityAddresses = (gapIdentityInformation_t *)MEM_BufferAlloc(gMaxResolvingListSize_c * sizeof(gapIdentityInformation_t));

    if( NULL != pOutIdentityAddresses )
    {
        result = Gap_GetBondedDevicesIdentityInformation(pOutIdentityAddresses,
                                                         gMaxResolvingListSize_c,
                                                         &identitiesCount);

        if( gBleSuccess_c == result && identitiesCount > 0 )
        {
            if( bCheckNewBond )
            {
                if( identitiesCount != mcDevicesInResolvingList + 1 )
                {
                    /* This is not a new bond, leave privacy as is */
                    MEM_BufferFree(pOutIdentityAddresses);
                    return gBleSuccess_c;
                }
            }

            mcDevicesInResolvingList = identitiesCount;
            if( mcDevicesInResolvingList > gMaxResolvingListSize_c )
            {
                mcDevicesInResolvingList = gMaxResolvingListSize_c;
            }

            result = Gap_EnableControllerPrivacy(TRUE, gSmpKeys.aIrk, mcDevicesInResolvingList, pOutIdentityAddresses);
        }
        else
        {
            result = Gap_EnableHostPrivacy(TRUE, gSmpKeys.aIrk);
        }

        MEM_BufferFree(pOutIdentityAddresses);

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
#endif

/*! *********************************************************************************
* @}
********************************************************************************** */
