/**
* @file ax_crypto_aes_key.c
* @author NXP Semiconductors
* @version 1.0
* @section LICENSE
* ----------------------------------------------------------------------------
 * Copyright 2016 NXP
 *
 * This software is owned or controlled by NXP and may only be used
 * strictly in accordance with the applicable license terms.  By expressly
 * accepting such terms or by downloading, installing, activating and/or
 * otherwise using the software, you are agreeing that you have read, and
 * that you agree to comply with and are bound by, such license terms.  If
 * you do not agree to be bound by the applicable license terms, then you
 * may not retain, install, activate or otherwise use the software.
* ----------------------------------------------------------------------------
* @section DESCRIPTION
*
* ----------------------------------------------------------------------------
* @section HISTORY
* 1.0   20-feb-2012 : Initial version
*
*****************************************************************************/
#include <stddef.h>
#include "nxLog.h"
#include <nxEnsure.h>

#include "scp.h"
#include "ax_api.h"
#include "sm_apdu.h"
#include "ax_util.h"
#include "sm_errors.h"

#ifdef INS_AX_AES_GENERATE_KEY
/**
* @function AES_GenerateKey
* @description Generates a random AES key.
* @param keyId The identifier of the secure storage item.
* @return status.
*/
U16 AES_GenerateKey(SST_Item_t keyId)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 identifier = 0;
    U8 index = 0;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_AES_GENERATE_KEY;
    pApdu->p1    = P1_AES_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

#ifdef TGT_EDEV
    rv = translateAesKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    // Skip generating key if keyId does not correspond to a valid AES key
    if (rv == SW_OK)
    {
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

        scp_Transceive(pApdu, SCP_MODE);

        // no response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

#endif /* INS_AX_AES_GENERATE_KEY */

#ifdef TGT_EDEV
U16 AES_Wrap(SST_Item_t keyId, U8 *keyData, U16 keyDataLen, U8 *wrappedKeyData, U16 *wrappedKeyDataLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 isOk = 0;
    U8 identifier = 0;
    U8 index = 0;
    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_AES_WRAP;
    pApdu->p1    = P1_WRAP;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateAesKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    // Skip if keyId does not correspond to a valid AES key
    if (rv == SW_OK)
    {
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_INPUT_DATA, keyDataLen, keyData);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U16 retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, wrappedKeyDataLen, wrappedKeyData);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}

U16 AES_UnWrap(SST_Item_t keyId, U8 *wrappedKeyData, U16 wrappedKeyDataLen, U8 *keyData, U16 *keyDataLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 isOk = 0;
    U8 identifier = 0;
    U8 index = 0;
    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_AES_WRAP;
    pApdu->p1    = P1_UNWRAP;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateAesKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    // Skip if keyId does not correspond to a valid AES key
    if (rv == SW_OK)
    {
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_INPUT_DATA, wrappedKeyDataLen, wrappedKeyData);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);
        if (isOk)
        {
            U16 retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, keyDataLen, keyData);

            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif

#ifdef INS_AX_AES_GENERATE_KEY
/**
* @function AES_DeriveKeyData
* @description Generates key data from a stored shared secret ECC point and other info according to the NIST SP-800-56a Rev2 Single Step KDF
* @param keyId           AES key identifier
* @param otherInfo          Pointer to the otherInfo.
* @param otherInfoLen       Length of the given otherInfo
* @param numBytes           Amount (in bytes) of requested key data
* @param keyData            Pointer to retrieved key data
* @param keyDataLen         Length of the retrieved key data
* @return status
*/
U16 AES_DeriveKeyData(SST_Item_t keyId, U8 *otherInfo, U16 otherInfoLen, U16 numBytes, U8 *keyData, U16 *keyDataLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 keyIdentifier = 0x00;
    U8 keyIndex = 0;
    U8 isOk = 0;

    ENSURE_OR_GO_EXIT(otherInfo != NULL);
    ENSURE_OR_GO_EXIT(keyData != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_AES_GENERATE_KEY;
    pApdu->p1    = P1_DERIVE_KEY_DATA;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateAesKeyItemToIdentityIndexPair(keyId, &keyIdentifier, &keyIndex);
#else
    keyIdentifier = (U8) (keyId >> 8);
    keyIndex = (U8) keyId;
#endif

    if (rv == SW_OK)
    {
        U8 tmp[2];
        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);
        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8 *)&keyIdentifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8 *)&keyIndex);
        AddTlvItem(pApdu, TAG_INPUT_DATA, otherInfoLen, otherInfo);
        tmp[0] = numBytes >> 8;
        tmp[1] = numBytes & 0xff;
        AddTlvItem(pApdu, TAG_SIZE, 2, tmp);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);

        if (isOk == 1)
        {
            rv = ParseResponse(pApdu, TAG_OUTPUT_DATA, keyDataLen, keyData);
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}

#endif /* INS_AX_AES_GENERATE_KEY */
