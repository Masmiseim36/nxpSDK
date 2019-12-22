/**
* @file ax_sst.c
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
*
*****************************************************************************/
#include <stddef.h>
#include <string.h>

#include "scp.h"
#include "ax_api.h"
#include "axHostCrypto.h"
#include "sm_apdu.h"
#include "ax_util.h"
#include "sm_errors.h"
#include "nxLog_App.h"
#include <nxEnsure.h>

#ifndef AX_SF
#define maxPacketSize (MAX_CHUNK_LENGTH_LINK)
#endif

#ifdef SST_ADMIN_SCP_KEYSET

U16 SST_SetScpKeyset(SST_Item_t keyId, U8 *keyEnc, U16 keyEncLen, U8 *keyMac, U16 keyMacLen, U8 *keyDek, U16 keyDekLen)
{
    U8 keyVersion = 1;
    U16 rv = SCP_PARAMETER_ERROR;
    U8 currentKeyDek[AES_KEY_LEN_nBYTE];
    ChannelId_t channelId;

    ENSURE_OR_GO_EXIT(keyEnc);
    ENSURE_OR_GO_EXIT(keyMac);
    ENSURE_OR_GO_EXIT(keyDek);
    ENSURE_OR_GO_EXIT(keyEncLen == AES_KEY_LEN_nBYTE);
    ENSURE_OR_GO_EXIT(keyMacLen == AES_KEY_LEN_nBYTE);
    ENSURE_OR_GO_EXIT(keyDekLen == AES_KEY_LEN_nBYTE);

    switch (keyId)
    {
    case SST_ADMIN_SCP_KEYSET:
        channelId = AX_ADMIN_CHANNEL;
        break;
    case SST_HOST_SCP_KEYSET:
        channelId = AX_HOST_CHANNEL;
        break;
    default:
        return SCP_UNDEFINED_CHANNEL_ID;
    }

    keyVersion = (U8) keyId;

    // Encrypt the keys
    // Fetch the current keyDek
    SCP_HostLocal_GetKeyDek(channelId, currentKeyDek);

    rv = SCP_GP_PutKeys(keyVersion, keyEnc, keyMac, keyDek, currentKeyDek, AES_KEY_LEN_nBYTE);
    return rv;
}

#endif /* SST_ADMIN_SCP_KEYSET */

U16 SST_SetInitialHostScpKeyset(U8 *keyEnc, U16 keyEncLen, U8 *keyMac, U16 keyMacLen, U8 *keyDek, U16 keyDekLen)
{
    U8 keyVersion = 1;
    U16 rv = SCP_PARAMETER_ERROR;
    SST_Item_t keyId = (SST_Item_t)SST_HOST_SCP_KEYSET;
    U8 *currentKeyDek = NULL;

    ENSURE_OR_GO_EXIT(keyEnc);
    ENSURE_OR_GO_EXIT(keyMac);
    ENSURE_OR_GO_EXIT(keyDek);
    ENSURE_OR_GO_EXIT(keyEncLen == AES_KEY_LEN_nBYTE);
    ENSURE_OR_GO_EXIT(keyMacLen == AES_KEY_LEN_nBYTE);
    ENSURE_OR_GO_EXIT(keyDekLen == AES_KEY_LEN_nBYTE);

#ifdef TGT_EDEV
    keyVersion = (U8) keyId;
#else
    keyVersion = (U8) (keyId >> 8);
#endif

    rv = SCP_GP_PutKeys(keyVersion, keyEnc, keyMac, keyDek, currentKeyDek, AES_KEY_LEN_nBYTE);
exit:
    return rv;
}

#ifdef INS_AX_SST_SET_ECC
/**
* @function SST_SetEccPublicKey
* @description Sets an ECC public key.
* @param keyId           Identifier must be of category ::SST_CAT_PUBLIC_KEY
* @param pPublicKey      Pointer to the supplied public key data (in ANSI X9.62 uncompressed format)
* @param publicKeylength Length of the public key data.
* @return state
*/
U16 SST_SetEccPublicKey(SST_Item_t keyId, U8 *pPublicKey, U16 publicKeyLength)
{
    U16 rv = SCP_PARAMETER_ERROR;
    U32 st = SCP_PARAMETER_ERROR;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 identifier = 0;
    U8 index = 0;

    ENSURE_OR_GO_EXIT(pPublicKey != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_ECC;
    pApdu->p1    = P1_ECC_PUBLIC_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

#ifdef TGT_EDEV
    rv = translatePublicKeyItemToIdentityIndexPair(keyId, &identifier, &index);
    if (rv != SW_OK) {
        goto exit;
    }
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
    AddTlvItem(pApdu, TAG_ECC_PUBLIC_KEY, publicKeyLength, pPublicKey);

    st = scp_Transceive(pApdu, SCP_MODE);
    if (st != SW_OK) {
        rv = (U16) st;
        goto exit;
    }

    // no response data expected
    rv = CheckNoResponseData(pApdu);

exit:
    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif /* INS_AX_SST_SET_ECC */

#define EXT_CASE4_APDU_OVERHEAD       (9)
#define SCP03_OVERHEAD               (24)   // padding (=16) + mac (=8)
#define SET_CERTIFICATE_TLV_OVERHEAD (10)   // TLV_SST_IDENTIFIER (=3) + TLV_SST_INDEX (=3) + TL_CERTIFICATE (=4; T=1, L=3)

#ifndef AX_SF
#ifdef INS_AX_SST_SET_CERTIFICATE
/**
* @function SST_Set_Certificate
* @description Sets a device certificate in secure storage.
*    Splits up the transfer of the certificate in multiple APDU exchanges
* @param certId         Identifier must be of category ::SST_CAT_CERTIFICATE
* @param pCertificate   Pointer to the supplied certificate data.
* @param certificateLen Length of the certificate data.
* @return status
*/
U16 SST_SetCertificate(SST_Item_t certId, const U8 *pCertificate, U16 certificateLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U8 identifier = 0;
    U8 index = 0;
    U8 hashSHA256[32];
    U8 nChunk = 0;
    U16 nSent = 0;
    U16 chunkSize = 0; // Chunk size of certificate
    int sendingChunksOk = 1;
    int maxChunkSize = 0;
    S32 nRet = HOST_CRYPTO_OK;

    ENSURE_OR_GO_EXIT(pCertificate != NULL);

    // The maximum size of a chunk depends on
    //  - the transport layer:                             maxPacketSize
    //  - amount and type of tags                          Defined in AX Host API Spec (take worst case)
    //  - Standard or Extended APDU format; APDU Type:     Extended and Case4 assumed
    // -  SCP03 overhead
    maxChunkSize = maxPacketSize - (EXT_CASE4_APDU_OVERHEAD + SET_CERTIFICATE_TLV_OVERHEAD + SCP03_OVERHEAD);
    ENSURE_OR_GO_EXIT(maxChunkSize > 1);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_CERTIFICATE;
    pApdu->p1    = P1_CERTIFICATE;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateCertificateItemToIdentityIndexPair(certId, &identifier, &index);
    if (rv != SW_OK) {
        goto exit;
    }
#else
    identifier = (U8) (certId >> 8);
    index = (U8) certId;
#endif

    // Skip storing certificate if certId does not correspond to a valid certificate
    while (nSent < certificateLen)
    {
        pApdu->p2    = nChunk;
        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

        chunkSize = (U16)( ((certificateLen - nSent) > maxChunkSize) ? (maxChunkSize) : (certificateLen - nSent) );

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        AddTlvItem(pApdu, TAG_CERTIFICATE, chunkSize, pCertificate+nSent);

        scp_Transceive(pApdu, SCP_MODE);
        // no response data expected
        rv = CheckNoResponseData(pApdu);

        if (rv != SW_OK)
        {
            // No point in continuing. Return failure status after cleaning up
            sendingChunksOk = 0;
            break;
        }

        nChunk++;
        nSent += chunkSize;
    }

    if (sendingChunksOk == 1)
    {
        pApdu->p2 = 0x80 | nChunk;
        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        // Calculate HASH
        nRet = HOST_SHA256_Get(pCertificate, certificateLen, hashSHA256);
        AddTlvItem(pApdu, TAG_HASH, 32, hashSHA256);

        scp_Transceive(pApdu, SCP_MODE);

        // no response data expected
        rv = CheckNoResponseData(pApdu);
    }

    FreeAPDUBuffer(pApdu);

    if (nRet == HOST_CRYPTO_ERROR)
    {
        rv = ERR_CRYPTO_ENGINE_FAILED;
    }
exit:
    return rv;
}
#endif /* INS_AX_SST_SET_CERTIFICATE */

#ifdef TGT_A70CU
U16 SST_IncrementCounter(SST_Item_t countId)
{
    U16 rv;
    U8 identifier = 0;
    U8 index = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_COUNTER;
    pApdu->p1    = P1_INCREMENT_COUNTER;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    identifier = (U8) (countId >> 8);
    index = (U8) countId;

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

    scp_Transceive(pApdu, SCP_MODE);

    // no response data expected
    rv = CheckNoResponseData(pApdu);

    FreeAPDUBuffer(pApdu);
    return rv;
}

U16 SST_SetCounter(SST_Item_t countId, U32 value)
{
    U16 rv;
    U8 identifier = 0;
    U8 index = 0;
    U8 byteArray[MONOTONIC_COUNTER_BYTE_COUNT];

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_COUNTER;
    pApdu->p1    = P1_SET_COUNTER;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    identifier = (U8) (countId >> 8);
    index = (U8) countId;

    byteArray[0] = (U8)(value >> 24);
    byteArray[1] = (U8)(value >> 16);
    byteArray[2] = (U8)(value >> 8);
    byteArray[3] = (U8)value;

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
    AddTlvItem(pApdu, TAG_INPUT_DATA, MONOTONIC_COUNTER_BYTE_COUNT, byteArray);

    scp_Transceive(pApdu, SCP_MODE);

    // no response data expected
    rv = CheckNoResponseData(pApdu);

    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif // TGT_A70CU

#ifdef INS_AX_SST_COPY
U16 SST_CopyKey(SST_Item_t keyIdDst, SST_Item_t keyIdSrc)
{
    apdu_t apdu;
    apdu_t *pApdu = (apdu_t *) &apdu;
    U16 rv_dst = SW_OK;
    U16 rv_src = SW_OK;
    U16 rv = SW_OK;
    U8 dstIdentifier = 0;
    U8 dstIndex = 0;
    U8 srcIdentifier = 0;
    U8 srcIndex = 0;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_COPY;
    pApdu->p1    = P1_COPY_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv_dst = translateKeyItemToIdentityIndexPair(keyIdDst, &dstIdentifier, &dstIndex);
    rv_src = translateKeyItemToIdentityIndexPair(keyIdSrc, &srcIdentifier, &srcIndex);
#else
    dstIdentifier = (U8) (keyIdDst >> 8);
    dstIndex = (U8) keyIdDst;
    srcIdentifier = (U8) (keyIdSrc >> 8);
    srcIndex = (U8) keyIdSrc;
#endif

    if ( (rv_dst == SW_OK) && (rv_src == SW_OK) )
    {
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&srcIdentifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&srcIndex);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER2, 1, (U8*)&dstIdentifier);
        AddTlvItem(pApdu, TAG_SST_INDEX2, 1, (U8*)&dstIndex);

        scp_Transceive(pApdu, SCP_MODE);

        // no response data expected
        rv = CheckNoResponseData(pApdu);
    }
    else
    {
        if (rv_dst != SW_OK) {rv = rv_dst;}
        if (rv_src != SW_OK) {rv = rv_src;}
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif /* INS_AX_SST_COPY */
#endif // SF_AX

#ifdef INS_AX_SST_SET_AES_KEY
/**
* @function SST_SetAesKey
* @description Sets an AES key in secure storage.
* @param keyId      Identifier must be of category ::SST_CAT_AES_KEY
* @param pKeyData   Pointer to the supplied key data.
* @param keylength  Length of the supplied key data.
* @return status
*/
U16 SST_SetAesKey(SST_Item_t keyId, U8 *pKeyData, U16 keyLength)
{
    apdu_t apdu;
    apdu_t *pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U8 identifier = 0;
    U8 index = 0;

    ENSURE_OR_GO_EXIT(pKeyData != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_AES_KEY;

#ifdef TGT_EDEV
    rv  = translateAesKeyItemToIdentityIndexPair(keyId, &identifier, &index);

    if (rv == SW_OK)
    {
        pApdu->p1    = identifier;
        pApdu->p2    = index;

        AllocateAPDUBuffer(pApdu);
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        AddStdCmdData(pApdu, keyLength, pKeyData);

        scp_Transceive(pApdu, SCP_MODE);

        // no response data expected
        rv = CheckNoResponseData(pApdu);
        FreeAPDUBuffer(pApdu);
    }
    else {
        goto exit;
    }

#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;

    pApdu->p1    = P1_AES_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
    AddTlvItem(pApdu, TAG_AES_KEY, keyLength, pKeyData);

    scp_Transceive(pApdu, SCP_MODE);

    // no response data expected
    rv = CheckNoResponseData(pApdu);
    FreeAPDUBuffer(pApdu);
#endif
exit:
    return rv;
}
#endif /* INS_AX_SST_SET_AES_KEY */

#ifdef TGT_A70CU
/**
* @function SST_SetAesKeyWrapped
* @description Sets an AES key in secure storage, the key is wrapped according RFC3394
* @param keyId              Identifier must be of category ::SST_CAT_AES_KEY
* @param wrappedKeyData     Pointer to the supplied - to be unwrapped - key data.
* @param wrappedKeyDataLen  Length of the supplied key data.
* @return status
*/
U16 SST_SetAesKeyWrapped(SST_Item_t keyId, SST_Item_t wrapKeyId, U8 *wrappedKeyData, U16 wrappedKeyDataLen)
{
    apdu_t apdu;
    apdu_t *pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U8 identifier = 0;
    U8 index = 0;
    U8 identifier2 = 0;
    U8 index2 = 0;

    ENSURE_OR_GO_EXIT(wrappedKeyData != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_AES_KEY;
    pApdu->p1    = P1_WRAPPED_AES_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
    identifier2 = (U8) (wrapKeyId >> 8);
    index2 = (U8) wrapKeyId;

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
    AddTlvItem(pApdu, TAG_SST_IDENTIFIER2, 1, (U8*)&identifier2);
    AddTlvItem(pApdu, TAG_SST_INDEX2, 1, (U8*)&index2);
    AddTlvItem(pApdu, TAG_AES_KEY, wrappedKeyDataLen, wrappedKeyData);

    scp_Transceive(pApdu, SCP_MODE);

    // no response data expected
    rv = CheckNoResponseData(pApdu);
    FreeAPDUBuffer(pApdu);

exit:
    return rv;
}
#endif

#define SET_GP_INPUT_DATA_TLV_OVERHEAD (14)   // TLV_SST_IDENTIFIER (=3) + TLV_SST_INDEX (=3) +
                                              // TLV_OFFSET (=4) + TL_INPUT_DATA (=4; T=1, L=3)
#ifndef AX_SF
#ifdef INS_AX_SST_SET_STORAGE
/**
* @function SST_SetGpStorage
* @description   Sets a GP Storage data chunk.
*   Splits up the transfer of the data chunk in multiple APDU exchanges if required
* @param storeId         Identifier must be of category ::SST_CAT_GP_STORAGE
* @param dataOffset      Offset for the data in the GP Storage.
* @param pData           Pointer to the supplied data.
* @param dataLen         Size of the supplied data.
* @return state
*/
U16 SST_SetGpStorage(SST_Item_t storeId, U16 dataOffset, U8 *pData, U16 dataLen)
{
    U16 offset = dataOffset;
    U8 tmp[2];
    U16 nSent = 0;
    U16 chunkSize = 0;
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U8 identifier = 0;
    U8 index = 0;
    int maxChunkSize = 0;

    ENSURE_OR_GO_EXIT(pData != NULL);

    // The maximum size of a chunk depends on
    //  - the transport layer:                             maxPacketSize
    //  - amount and type of tags                          Defined in AX Host API Spec (take worst case)
    //  - Standard or Extended APDU format; APDU Type:     Extended and Case4 assumed
    //  - SCP03 overhead
    maxChunkSize = maxPacketSize - (EXT_CASE4_APDU_OVERHEAD + SET_GP_INPUT_DATA_TLV_OVERHEAD + SCP03_OVERHEAD);
    ENSURE_OR_GO_EXIT(maxChunkSize > 1);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_SET_STORAGE;
    pApdu->p1    = P1_GP_STORAGE;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateGpStorageItemToIdentityIndexPair(storeId, &identifier, &index);
    if (rv != SW_OK) {
        goto exit:
    }
#else
    identifier = (U8) (storeId >> 8);
    index = (U8) storeId;
#endif


    // Skip storing data if storeId does not correspond to a valid general purpose data store
    while (nSent < dataLen)
    {
        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

        if ((dataLen-nSent) > maxChunkSize)
        {
            chunkSize = (U16) maxChunkSize;
        }
        else
        {
            chunkSize = (dataLen-nSent);
        }

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        tmp[0] = offset >> 8;
        tmp[1] = offset & 0xff;
        AddTlvItem(pApdu, TAG_OFFSET, 2, tmp);
        AddTlvItem(pApdu, TAG_INPUT_DATA, chunkSize, &pData[nSent]);

        scp_Transceive(pApdu, SCP_MODE);

        // no response data expected
        rv = CheckNoResponseData(pApdu);

        if (rv != SW_OK)
        {
            // No point in continuing. Return failure status after cleaning up
            break;
        }

        nSent += chunkSize;
        offset += chunkSize;
    }

exit:
    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif /* INS_AX_SST_SET_STORAGE */
#endif // AX_SF

#ifdef INS_AX_SST_GET_ECC
/**
* @function SST_GetECCPublicKey
* @description  Gets an ECC public key from secure storage.
*               Also allows to retrieve the public key from a keypair.
* @param keyId           Identifier must be of category ::SST_CAT_PUBLIC_KEY
* @param pKeyData        Pointer to the public key data.
* @param pKeyLen         Pointer to the length of the public key data.
* @return state
*/
U16 SST_GetEccPublicKey(SST_Item_t keyId, U8 *pKeyData, U16 *pKeyLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U8 isOk = 0;
    U16 rv = ERR_API_ERROR;
    U8 identifier = 0;
    U8 index = 0;

    ENSURE_OR_GO_EXIT(pKeyData != NULL);
    ENSURE_OR_GO_EXIT(pKeyLen != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_ECC;
    pApdu->p1    = P1_ECC_PUBLIC_KEY;
    pApdu->p2    = 0x00;
    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    if ( (rv = translatePairKeyItemToIdentityIndexPair(keyId, &identifier, &index)) != SW_OK )
    {
        rv = translatePublicKeyItemToIdentityIndexPair(keyId, &identifier, &index);
        if (rv != SW_OK) {
            goto exit;
        }
    }
    else {
        goto exit;
    }
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);
    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

    scp_Transceive(pApdu, SCP_MODE);

    rv = smGetSw( pApdu, &isOk);
    if (isOk)
    {
        U16 retval = ParseResponse(pApdu, TAG_ECC_PUBLIC_KEY, pKeyLen, pKeyData);

        if (retval != SW_OK)
        {
            rv = ERR_WRONG_RESPONSE;
        }
    }
exit:
    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif /* INS_AX_SST_GET_ECC */

#ifndef AX_SF
#ifdef INS_AX_SST_GET_CERTIFICATE
/**
* @function SST_Get_Certificate
* @description Retrieves the requested certificate from secure storage.
*    Splits up the retrieval of the certificate in multiple APDU exchanges.
*    Verifies the Hash associated with the certificate
* @param certId            Identifier must be of category ::SST_CAT_CERTIFICATE
* @param pCertificate      Pointer to the certificate data, buffer to be provided by caller
* @param pCertificateLen   IN: Size of buffer provided by caller; OUT: length of the certificate effectively retrieved
* @return state
*/
U16 SST_GetCertificate(SST_Item_t certId, U8 *pCertificate, U16* pCertificateLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U8 isOk = 0;
    U8 identifier = 0;
    U8 index = 0;
    U8 nChunk = 0;
    U16 nReceived = 0;
    U16 chunkSize = 0; // Chunk size of certificate received
    int receivingChunksOk = 1;
    int maxChunkSize = 0;
    U16 actualCertificateLen = 0;
    U8 hashSHA256Received[32];
    U8 hashSHA256Calculated[32];
    U16 retval;
    S32 nRet = HOST_CRYPTO_OK;

    U8 buf[4];
    U16 bufLen = 0;

    ENSURE_OR_GO_EXIT(pCertificate != NULL);
    ENSURE_OR_GO_EXIT(pCertificateLen != NULL);

    // The maximum size of a response chunk depends on
    //  - the transport layer:                             maxPacketSize
    //  - APDU RSP Status overhead                         2
    //  - SCP03 overhead
    maxChunkSize = maxPacketSize - (RSP_APDU_STATUS_OVERHEAD + SCP03_OVERHEAD);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_CERTIFICATE;
    pApdu->p1    = P1_CERTIFICATE;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateCertificateItemToIdentityIndexPair(certId, &identifier, &index);
    if (rv != SW_OK) {
        goto exit;
    }
#else
    identifier = (U8) (certId >> 8);
    index = (U8) certId;
#endif


    // Each command chunk will contain a 'TLV TAG_SIZE'
    // The value of TAG_SIZE will determine the PACKET size of the response chunk

    // Skip fetching certificate if certId does not correspond to a valid certificate

    // Fetching all certificate chunks
    while ( (nChunk == 0) || (nReceived < actualCertificateLen) )
    {
        int i = 0;

        pApdu->p2 = nChunk;
        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        buf[0] = (maxChunkSize >> 8) & 0x00FF;
        buf[1] = maxChunkSize & 0x00FF;
        AddTlvItem(pApdu, TAG_SIZE, 2, (U8*)buf);

        scp_Transceive(pApdu, SCP_MODE);
        rv = smGetSw(pApdu, &isOk);

        if (isOk == 0)
        {
            receivingChunksOk = 0;
            break;
        }

        // The first response chunk contains a 'TLV TAG_SIZE'
        // The value of TAG_SIZE corresponds to the overall Certificate Size
        // If the size of the certificate to be retrieved is bigger than
        // the buffer provided by the caller we will return with an error
        if (nChunk == 0)
        {
            bufLen = sizeof(buf);
            retval = ParseResponse(pApdu, TAG_SIZE, &bufLen, buf);
            if (retval != SW_OK)
            {
                rv = ERR_WRONG_RESPONSE;
                receivingChunksOk = 0;
                break;
            }
            actualCertificateLen = 0;
            for (i=0; i<bufLen; i++)
            {
                actualCertificateLen = (actualCertificateLen << 8) + buf[i];
            }
            // printf("The certificate is %d byte in size\r\n", actualCertificateLen);
            if (actualCertificateLen > *pCertificateLen)
            {
                rv = ERR_BUF_TOO_SMALL;
                receivingChunksOk = 0;
                break;
            }
        }

        chunkSize = (*pCertificateLen - nReceived);
        retval = ParseResponse(pApdu, TAG_CERTIFICATE, &chunkSize, pCertificate+nReceived);
        if (retval != SW_OK)
        {
            rv = ERR_WRONG_RESPONSE;
            receivingChunksOk = 0;
            break;
        }
        nReceived += chunkSize;
        nChunk++;
    }

    if (receivingChunksOk == 1)
    {
        // Is the hash already included in the response?
        int receivedHash = 1;
        U16 hashSize = 0;

        hashSize = sizeof(hashSHA256Received);
        retval = ParseResponse(pApdu, TAG_HASH, &hashSize, hashSHA256Received);
        if (retval != SW_OK)
        {
            // Send out an extra command to fetch the HASH
            pApdu->p2 = nChunk;
            SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

            AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
            AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
            buf[0] = (maxChunkSize >> 8) & 0x00FF;
            buf[1] = maxChunkSize & 0x00FF;
            AddTlvItem(pApdu, TAG_SIZE, 2, (U8*)buf);

            scp_Transceive(pApdu, SCP_MODE);
            rv = smGetSw(pApdu, &isOk);

            if (isOk == 0)
            {
                receivedHash = 0;
            }
            else
            {
                hashSize = sizeof(hashSHA256Received);
                retval = ParseResponse(pApdu, TAG_HASH, &hashSize, hashSHA256Received);
                if (retval != SW_OK)
                {
                    receivedHash = 0;
                    rv = ERR_TLV_MISSING;
                }
            }
        }

        if (receivedHash == 1)
        {
            // Calculate Reference HASH
            nRet = HOST_SHA256_Get(pCertificate, actualCertificateLen, hashSHA256Calculated);

            if (nRet == HOST_CRYPTO_ERROR)
            {
                rv = ERR_CRYPTO_ENGINE_FAILED;
            }
            else
            {
                // Compare HASH received with HASH calculated
                if (memcmp(hashSHA256Calculated, hashSHA256Received, sizeof(hashSHA256Received)) == 0 )
                {
                    // Hashes match
                    rv = SW_OK;
                }
                else
                {
                    rv = ERR_HASH_COMPARE_FAILS;
                }
            }
        }
    }

    if (rv != SW_OK)
    {
        *pCertificateLen = 0;
    }
    else
    {
        *pCertificateLen = actualCertificateLen;
    }
exit:
    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif /* INS_AX_SST_GET_CERTIFICATE */

#ifdef TGT_A70CU
U16 SST_GetCounter(SST_Item_t countId, U32 *pValue)
{
    U16 rv = ERR_API_ERROR;
    U8 cntBuf[MONOTONIC_COUNTER_BYTE_COUNT];
    U8 identifier = 0;
    U8 index = 0;
    U8 isOk = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    ENSURE_OR_GO_EXIT(pValue != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_COUNTER;
    pApdu->p1    = P1_GET_COUNTER;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

    identifier = (U8) (countId >> 8);
    index = (U8) countId;

    AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
    AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

    scp_Transceive(pApdu, SCP_MODE);

    rv = smGetSw(pApdu, &isOk);
    if (isOk)
    {
        U16 length = MONOTONIC_COUNTER_BYTE_COUNT;
        U16 retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, &length, cntBuf);

        if ((retval != SW_OK) || (length != MONOTONIC_COUNTER_BYTE_COUNT))
        {
            *pValue = 0;
            rv = ERR_WRONG_RESPONSE;
        }
        else
        {
            *pValue = (cntBuf[0] << 24) + (cntBuf[1] << 16) + (cntBuf[2] << 8) + cntBuf[3];
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}
#endif // TGT_A70CU

#endif // AX_SF

#if 0
/**
* @function SST_GetAesKey
* @description Retrieves an AES key from secure storage.
* @param keyId      Identifier must be of category ::SST_CAT_AES_KEY
* @param pKeyData   Pointer to the key data, buffer to be provided by caller
* @param pKeylength  IN: Size of buffer provided by caller; OUT: Length of key effectively retrieved
* @return status
*/
U16 SST_GetAesKey(SST_Item_t keyId, U8 *pKeyData, U16 *pKeyLength)
{
    apdu_t apdu;
    apdu_t *pApdu = (apdu_t *) &apdu;
    U16 rv = SW_OK;
    U8 identifier = 0;
    U8 index = 0;
    U16 retval;
    U8 isOk = 0;

    assert(pKeyData != NULL);
    assert(pKeyLength != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_AES_KEY;
    pApdu->p1    = P1_AES_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv  = translateAesKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    if (rv == SW_OK)
    {
        SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);

        scp_Transceive(pApdu, SCP_MODE);

        rv = smGetSw(pApdu, &isOk);

        if (isOk != 0)
        {
            retval = ParseResponse(pApdu, TAG_AES_KEY, pKeyLength, pKeyData);
            if (retval != SW_OK)
            {
                rv = ERR_TLV_MISSING;
            }
        }
    }

    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif

#define GET_GP_RSP_TLV_OVERHEAD (4)   // TL_OUTPUT_DATA (=4; T=1, L=3)

#ifndef AX_SF
#ifdef INS_AX_SST_GET_STORAGE
/**
* @function SST_GetGpStorage
* @description   Retrieves a GP Storage data chunk.
*   Splits up the transfer of the data chunk in multiple APDU exchanges if required
* @param storeId         Identifier must be of category ::SST_CAT_GP_STORAGE
* @param dataOffset      Offset for the data in the GP Storage.
* @param pData           IN: Buffer; OUT: Pointer to the retrieved data.
* @param dataLen         IN: Size of the supplied data buffer ::pData; OUT: Size of the retrieved data.
* @return state
*/
U16 SST_GetGpStorage(SST_Item_t storeId, U16 dataOffset, U8 *pData, U16 *dataLen)
{
    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U8 isOk = 0;
    U8 identifier = 0;
    U8 index = 0;
    U16 nReceived = 0;
    int maxChunkSize = 0;
    int actualChunkSize = 0;
    U16 nReceiveTarget = *dataLen;
    U8 buf[2];

    U16 retval = SW_OK;
    U16 offset = dataOffset;
    U16 bufferOffset = 0;

    ENSURE_OR_GO_EXIT(pData != NULL);

    // The maximum size of a response chunk depends on
    //  - the transport layer:                             maxPacketSize
    //  - APDU RSP Status overhead                         2
    //  - TL of TAG_OUTPUT_DATA
    //  - SCP03 overhead
    maxChunkSize = maxPacketSize - (RSP_APDU_STATUS_OVERHEAD + GET_GP_RSP_TLV_OVERHEAD + SCP03_OVERHEAD);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_STORAGE;
    pApdu->p1    = P1_CERTIFICATE;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

#ifdef TGT_EDEV
    rv = translateGpStorageItemToIdentityIndexPair(storeId, &identifier, &index);
    if (rv != SW_OK) {
        goto exit;
    }
#else
    identifier = (U8) (storeId >> 8);
    index = (U8) storeId;
#endif

    // Each command chunk will contain a 'TLV TAG_SIZE'
    // The value of TAG_SIZE will determine the PACKET size of the response chunk

    // Skip fetching GpStorage if certId does not correspond to a valid certificate

    // Fetching all data until
    // - An error occured (less data is available than requested for)
    // - All data has been fetched
    while ( nReceived < nReceiveTarget )
    {
        U16 outputDataLen = nReceiveTarget - nReceived;

        SetApduHeader(pApdu, USE_EXTENDED_APDU_LEN);

        actualChunkSize = ((nReceiveTarget-nReceived) < maxChunkSize) ? (nReceiveTarget-nReceived) : maxChunkSize;

        AddTlvItem(pApdu, TAG_SST_IDENTIFIER, 1, (U8*)&identifier);
        AddTlvItem(pApdu, TAG_SST_INDEX, 1, (U8*)&index);
        buf[0] = offset >> 8;
        buf[1] = offset & 0xff;
        AddTlvItem(pApdu, TAG_OFFSET, 2, buf);
        buf[0] = (actualChunkSize >> 8) & 0x00FF;
        buf[1] = actualChunkSize & 0x00FF;
        AddTlvItem(pApdu, TAG_SIZE, 2, (U8*)buf);

        scp_Transceive(pApdu, SCP_MODE);
        rv = smGetSw(pApdu, &isOk);

        if (isOk == 0)
        {
            break;
        }

        // TODO: Handle case where pData buffer is too small to store all data
        retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, &outputDataLen, pData+bufferOffset);
        // TODO: Q: Does one also need to check on (actualChunkSize == outputDataLen)?
        if (retval != SW_OK)
        {
            rv = ERR_WRONG_RESPONSE;
            break;
        }

        bufferOffset += outputDataLen;
        offset += outputDataLen;
        nReceived += outputDataLen;
    }

exit:
    FreeAPDUBuffer(pApdu);
    return rv;
}
#endif /* INS_AX_SST_GET_STORAGE */

#ifdef INS_AX_SST_ERASE
/**
* @description Erases the specified certificate from secure storage.
* @param certId Certificate Identifier
* @return state
*/
U16 SST_EraseCertificate(SST_Item_t certId)
{
    U16 rv = SW_OK;
    U8 identifier = 0;
    U8 index = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_ERASE;
    pApdu->p1    = P1_ERASE_CERTIFICATE;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

#ifdef TGT_EDEV
    rv = translateCertificateItemToIdentityIndexPair(certId, &identifier, &index);
#else
    identifier = (U8) (certId >> 8);
    index = (U8) certId;
#endif


    // Skip deleting certificate if certId does not correspond to a valid certificate
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

/**
* @description Erases the specified key from secure storage.
*   The key referred to can be of type AES, Key-pair or Public Key
* @param keyId Key Identifier
* @return state
*/
U16 SST_EraseKey(SST_Item_t keyId)
{
    U16 rv = SW_OK;
    U8 identifier = 0;
    U8 index = 0;

    apdu_t apdu;
    apdu_t * pApdu = (apdu_t *) &apdu;

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_ERASE;
    pApdu->p1    = P1_ERASE_KEY;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);
    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

#ifdef TGT_EDEV
    rv = translateKeyItemToIdentityIndexPair(keyId, &identifier, &index);
#else
    identifier = (U8) (keyId >> 8);
    index = (U8) keyId;
#endif

    // Skip deleting key if keyId does not correspond to a valid key
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
#endif /* INS_AX_SST_ERASE */

#endif // AX_SF

#ifdef INS_AX_SST_GET_IDENTIFIER
/**
 * @brief SST_GetUniqueIdentifier
 * @param id
 * @param idLen
 * @return
 */
U16 SST_GetUniqueIdentifier(U8 *id, U16 *idLen)
{
    apdu_t apdu;
    apdu_t *pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U16 retval;
    U8 isOk = 0;

    ENSURE_OR_GO_EXIT(id != NULL);
    ENSURE_OR_GO_EXIT(idLen != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_IDENTIFIER;
    pApdu->p1    = P1_UNIQUE_IDENTIFIER;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    scp_Transceive(pApdu, SCP_MODE);

    rv = smGetSw(pApdu, &isOk);

    if (isOk != 0)
    {
        retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, idLen, id);
        if (retval != SW_OK)
        {
            rv = ERR_TLV_MISSING;
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}

/**
 * @brief SST_GetCplcData
 * @param cplcData
 * @param cplcDataLen
 * @return
 */
U16 SST_GetCplcData(U8 *cplcData, U16 *cplcDataLen)
{
    apdu_t apdu;
    apdu_t *pApdu = (apdu_t *) &apdu;
    U16 rv = ERR_API_ERROR;
    U16 retval;
    U8 isOk = 0;

    ENSURE_OR_GO_EXIT(cplcData != NULL);
    ENSURE_OR_GO_EXIT(cplcDataLen != NULL);

    pApdu->cla   = AX_CLA;
    pApdu->ins   = INS_AX_SST_GET_IDENTIFIER;
    pApdu->p1    = P1_CPLC_DATA;
    pApdu->p2    = 0x00;

    AllocateAPDUBuffer(pApdu);

    SetApduHeader(pApdu, USE_STANDARD_APDU_LEN);

    scp_Transceive(pApdu, SCP_MODE);

    rv = smGetSw(pApdu, &isOk);

    if (isOk != 0)
    {
        retval = ParseResponse(pApdu, TAG_OUTPUT_DATA, cplcDataLen, cplcData);
        if (retval != SW_OK)
        {
            rv = ERR_TLV_MISSING;
        }
    }

    FreeAPDUBuffer(pApdu);
exit:
    return rv;
}

#endif /* INS_AX_SST_GET_IDENTIFIER*/
