/**
 * @file ax_util.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
 *
 * @par Description
 * A7x common utility functions.
 * SCP03 utility functions to manage two SCP03 channels (a Host and and Admin channel).
 * Not all A7-series devices support both a Host and an Admin SCP03 channel.
 * @par History
 * 1.0   27-mar-2014 : Initial version
 *
 *****************************************************************************/
#include <stdio.h>
#include <stddef.h>
#include "nxLog_scp.h"
#include <string.h>

#include "ax_api.h"
#include "sm_apdu.h"
#include "sm_errors.h"

static ChannelId_t selectedChannelId = AX_HOST_CHANNEL;
static scp_CommandType_t hostChannelCommandType = NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC;
static scp_CommandType_t adminChannelCommandType = NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC;

#ifdef TGT_EDEV
#ifndef AX_SF
U16 translateCertificateItemToIdentityIndexPair(SST_Item_t certId, U8 *pIdentifier, U8 *pIndex)
{
    U16 rv = SW_OK;

    switch (certId)
    {
        case SST_IDENTITY_CERT:
            *pIdentifier = SST_APDU_IDENTITY_CERT;
            *pIndex = 0x00;
        break;

        default:
            rv = ERR_API_ERROR;
            break;
    }
    return rv;
}
#endif // AX_SF

U16 translateAesKeyItemToIdentityIndexPair(SST_Item_t keyId, U8 *pIdentifier, U8 *pIndex)
{
    U16 rv = SW_OK;

    switch (keyId)
    {
        case SST_HOST_LOCAL_ENC_KEY:
            *pIdentifier = SST_APDU_HOST_LOCAL_ENC_KEY;
            *pIndex = 0x00;
        break;

        case SST_CCLAN_UNWRAPPING_KEY:
            *pIdentifier = SST_APDU_UNWRAPPING_KEY;
            *pIndex = 0x00;
        break;

        case SST_PSK_UNWRAPPING_KEY:
            *pIdentifier = SST_APDU_UNWRAPPING_KEY;
            *pIndex = 0x01;
        break;

        default:
            rv = ERR_API_ERROR;
        break;
    }
    return rv;
}

U16 translatePairKeyItemToIdentityIndexPair(SST_Item_t keyId, U8 *pIdentifier, U8 *pIndex)
{
    U16 rv = SW_OK;

    switch (keyId)
    {
        case SST_IDENTITY_KEY_PAIR:
            *pIdentifier = SST_APDU_IDENTITY_KEY_PAIR;
            *pIndex = 0x00;
        break;

        case SST_IDENTITY_KEY_PAIR_ALT:
            *pIdentifier = SST_APDU_IDENTITY_KEY_PAIR;
            *pIndex = 0x01;
        break;

        case SST_SSH_SERVER_KEY_PAIR:
            *pIdentifier = SST_APDU_SSH_SERVER_KEY_PAIR;
            *pIndex = 0x00;
        break;

        case SST_EPHEMERAL_KEY_PAIR_1:
            *pIdentifier = SST_APDU_EPHEMERAL_KEY_PAIR;
            *pIndex = 0x00;
        break;

        case SST_EPHEMERAL_KEY_PAIR_2:
            *pIdentifier = SST_APDU_EPHEMERAL_KEY_PAIR;
            *pIndex = 0x01;
        break;

        default:
            rv = ERR_API_ERROR;
        break;
    }
    return rv;
}

U16 translatePublicKeyItemToIdentityIndexPair(SST_Item_t keyId, U8 *pIdentifier, U8 *pIndex)
{
    U16 rv = SW_OK;

    switch (keyId)
    {
        case SST_HOST_SW_PUBLIC_KEY_1:
            *pIdentifier = SST_APDU_HOST_SW_PUBLIC_KEY;
            *pIndex = 0x00;
        break;

        case SST_HOST_SW_PUBLIC_KEY_2:
            *pIdentifier = SST_APDU_HOST_SW_PUBLIC_KEY;
            *pIndex = 0x01;
        break;

        case SST_SSH_CLIENT_PUBLIC_KEY:
            *pIdentifier = SST_APDU_SSH_CLIENT_PUBLIC_KEY;
            *pIndex = 0x00;
        break;

        case SST_ROOT_CA_PUBLIC_KEY_1:
            *pIdentifier = SST_APDU_ROOT_CA_PUBLIC_KEY;
            *pIndex = 0x00;
        break;

        case SST_ROOT_CA_PUBLIC_KEY_2:
            *pIdentifier = SST_APDU_ROOT_CA_PUBLIC_KEY;
            *pIndex = 0x01;
        break;

        default:
            rv = ERR_API_ERROR;
        break;
    }
    return rv;
}

U16 translateKeyItemToIdentityIndexPair(SST_Item_t keyId, U8 *pIdentifier, U8 *pIndex)
{
    if (translateAesKeyItemToIdentityIndexPair(keyId, pIdentifier, pIndex) == SW_OK)
    {
        return SW_OK;
    }
    else if (translatePairKeyItemToIdentityIndexPair(keyId, pIdentifier, pIndex) == SW_OK)
    {
        return SW_OK;
    }
    else if (translatePublicKeyItemToIdentityIndexPair(keyId, pIdentifier, pIndex) == SW_OK)
    {
        return SW_OK;
    }

    // Nothing matched
    return ERR_API_ERROR;
}

#ifndef AX_SF
U16 translateGpStorageItemToIdentityIndexPair(SST_Item_t storeId, U8 *pIdentifier, U8 *pIndex)
{
    U16 rv = SW_OK;

    switch (storeId)
    {
        case SST_HOST_CONFIGURATION:
            *pIdentifier = SST_APDU_HOST_CONFIGURATION_DATA;
            *pIndex = 0x00;
        break;

        case SST_NTP_KEYS:
            *pIdentifier = SST_APDU_NTP_KEYS_DATA;
            *pIndex = 0x00;
        break;

        case SST_FUTURE_USE:
            *pIdentifier = SST_APDU_FUTURE_USE_DATA;
            *pIndex = 0x00;
        break;

        default:
            rv = ERR_API_ERROR;
        break;
    }
    return rv;
}
#endif // AX_SF
#endif // TGT_EDEV

/**
 * Clears the cached SCP03 channel status on the Host. Select the Host Channel.
 */
void DEV_ClearChannelState()
{
    selectedChannelId = AX_HOST_CHANNEL;
    hostChannelCommandType = NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC;
    adminChannelCommandType = NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC;
}

/**
 * Forces the cached SCP03 Host channel status to C_MAC_C_ENC_R_MAC_R_ENC.
 * Select the Host Channel.
 * Clears the cached SCP03 Admin channel status.
 */
void DEV_SetResumeHostChannelState()
{
    selectedChannelId = AX_HOST_CHANNEL;
    hostChannelCommandType = C_MAC_C_ENC_R_MAC_R_ENC;
    adminChannelCommandType = NO_C_MAC_NO_C_ENC_NO_R_MAC_NO_R_ENC;
}

/**
 * Select the requested channel \p channelId)
 */
void DEV_SelectChannel(ChannelId_t channelId)
{
    selectedChannelId = channelId;
}

/**
 * Set the cached SCP03 channel status of either the Host or Admin SCP03 channel.
 */
void DEV_SetChannelCommandType(ChannelId_t channelId, scp_CommandType_t commandType)
{
    switch (channelId)
    {
        case AX_HOST_CHANNEL:
            hostChannelCommandType = commandType;
        break;

        case AX_ADMIN_CHANNEL:
            adminChannelCommandType = commandType;
        break;

        default:
            LOG_E("NO other Channel supported");
        break;
    }
}

/**
 * Fetch the cached SCP03 channel status of the active SCP03 channel.
 */
ChannelId_t DEV_GetSelectedChannel(scp_CommandType_t *commandType)
{
    switch (selectedChannelId)
    {
        case AX_HOST_CHANNEL:
            *commandType = hostChannelCommandType;
        break;

        case AX_ADMIN_CHANNEL:
            *commandType = adminChannelCommandType;
        break;

        default:
            LOG_E("NO other Channel supported");
        break;
    }

    return selectedChannelId;
}


void DEV_setDataDerivationArray(U8 ddA[], U16 *pDdALen,
    U8 ddConstant, U16 ddL, U8 iCounter, U8 *context, U16 contextLen)
{
    // SCPO3 spec p9&10
    memset(ddA, 0, DD_LABEL_LEN - 1);
    ddA[DD_LABEL_LEN - 1] = ddConstant;
    ddA[DD_LABEL_LEN] = 0x00; // Separation Indicator
    ddA[DD_LABEL_LEN + 1] = (U8)(ddL >> 8);
    ddA[DD_LABEL_LEN + 2] = (U8)ddL;
    ddA[DD_LABEL_LEN + 3] = iCounter;
    memcpy(&ddA[DD_LABEL_LEN + 4], context, contextLen);
    *pDdALen = DD_LABEL_LEN + 4 + contextLen;
}
