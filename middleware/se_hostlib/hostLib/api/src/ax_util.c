/**
 * @file ax_util.c
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2016
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
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
#include <assert.h>
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
            assert(1==0);
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
            assert(1==0);
        break;
    }

    return selectedChannelId;
}
