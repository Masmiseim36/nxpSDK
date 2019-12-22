/**
 * @file hcAsn.c
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
 *
 * @par History
 *
 */

#include "sm_types.h"
// #include "tst_sm_util.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "ax_util.h"
#include "nxLog_hostLib.h"
#include "nxEnsure.h"

/// @cond
#define ASN_TAG_SEQUENCE 0x30
#define ASN_TAG_INTEGER  0x02

#define ASN_P192_BYTE_COUNT 0x18
#define ASN_P224_BYTE_COUNT 0x1C
#define ASN_P256_BYTE_COUNT 0x20
/// @endcond

// Control LOG info created
// #define AX_HC_ASN_LOG_VERBOSE

/*
    Example Asn Signature (LEN=71):

    30 45
        02 21
            00 AD D2 2C E9 5B 05 F8 A0 8D 33 33 0F FF 24 7A 55 F4 60 F1 DF 55 27 55 C4 C8 63 53 6A 65 D4 41 74
        02 20
            21 E5 FF FF 6B 6C B3 0D B9 D6 F2 AB 67 5B 4C 8A E5 7C 58 44 F9 55 8D 54 F1 3E F8 FE C5 00 C0 EF

    Corresponding Plain Signature

    AD D2 2C E9 5B 05 F8 A0 8D 33 33 0F FF 24 7A 55 F4 60 F1 DF 55 27 55 C4 C8 63 53 6A 65 D4 41 74
    21 E5 FF FF 6B 6C B3 0D B9 D6 F2 AB 67 5B 4C 8A E5 7C 58 44 F9 55 8D 54 F1 3E F8 FE C5 00 C0 EF
*/

U16 hcAsnToPlainEcc256(U8 *signature, U16 *pSignatureLen, U8 *asnSignature, U16 asnSignatureLen)
{
    U16 rv = ERR_GENERAL_ERROR;
    int i = 0;
    int component = 0;

    ENSURE_OR_GO_EXIT(pSignatureLen != NULL);
    ENSURE_OR_GO_EXIT(asnSignature != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);

    if (*pSignatureLen < 64)
    {
        return ERR_API_ERROR;
    }

    if (asnSignature[0] == ASN_TAG_SEQUENCE)
    {
        if ( (asnSignature[1]+2) == asnSignatureLen)
        {
            i = 2;
            for (component=0; component<2; component++)
            {
                if (asnSignature[i] == ASN_TAG_INTEGER)
                {
                    if ((asnSignature[i+1] == 0x20) || (asnSignature[i+1] == 0x21))
                    {
                        // Conditionally skip leading 0 when copying to plain format
                        memcpy(signature+(component*0x20), &asnSignature[i+2+(asnSignature[i+1]-0x20)], 0x20);
                        i += 2 + asnSignature[i+1];
                    }
                    else
                    {
                        *pSignatureLen = 0;
                        return ERR_WRONG_RESPONSE;
                    }
                }
                else
                {
                    *pSignatureLen = 0;
                    return ERR_WRONG_RESPONSE;
                }
            }
        }
        else
        {
            printf("asnSignature[1] + 2: 0x%02X, asnSignatureLen: 0x%02X", asnSignature[1] + 2, asnSignatureLen); // DEBUG
            *pSignatureLen = 0;
            return ERR_WRONG_RESPONSE;
        }
    }
    else
    {
        *pSignatureLen = 0;
        return ERR_WRONG_RESPONSE;
    }

    *pSignatureLen = 0x40;

    // printBytestring("asnSignature", asnSignature, asnSignatureLen); // DEBUG
    // printBytestring("signature", signature, *pSignatureLen); // DEBUG
    rv = SW_OK;
exit:
    return rv;
}

U16 hcPlainToAsnEcc256(U8 *asnSignature, U16 *asnSignatureLen, U8 *signature, U16 signatureLen)
{
    U16 rv = ERR_GENERAL_ERROR;
    int i = 0;
    int component = 0;

    ENSURE_OR_GO_EXIT(asnSignatureLen != NULL);
    ENSURE_OR_GO_EXIT(asnSignature != NULL);
    ENSURE_OR_GO_EXIT(signature != NULL);

    if (*asnSignatureLen < (64+2+4+2))
    {
        return ERR_API_ERROR;
    }

    if (signatureLen != 64)
    {
        return ERR_API_ERROR;
    }

    asnSignature[0] = ASN_TAG_SEQUENCE;
    asnSignature[1] = 0;
    i = 2;

    for (component = 0; component < 2; component++)
    {
        asnSignature[i] = ASN_TAG_INTEGER;
        if (signature[0 + component*0x20] > 0x7F)
        {
            asnSignature[i+1] = 0x21;
            asnSignature[i+2] = 0x00;
            memcpy(&asnSignature[i+3], signature+component*0x20, 0x20);
            asnSignature[1] += (0x20 + 3);
            i += 0x23;
        }
        else
        {
            asnSignature[i+1] = 0x20;
            memcpy(&asnSignature[i+2], signature+component*0x20, 0x20);
            asnSignature[1] += (0x20 + 2);
            i += 0x22;
        }
    }

    *asnSignatureLen = asnSignature[1] + 2;

    // printBytestring("asnSignature", asnSignature, *asnSignatureLen); // DEBUG
    rv = SW_OK;

exit:
    return rv;
}


/**
  The purpose of this function is to turn the proprietary ECDSA signature format - that
  may be returned by the A7x - into a normalized ASN.1 format.

  The A7x will package the 'r' and 's' integer components of an ECDSA signature either as octet arrays of
  ASN_Pxxx_BYTE_COUNT or (ASN_Pxxx_BYTE_COUNT + 1) with the additional octet added to ensure the
  integer value is a positive number.

  ASN.1 requires an integer value is always encoded in the smallest possible number of octets, a requirement
  not fulfilled by the A7x.

  Note: The implementation of this function only deals with signatures associated with key lengths of 192, 224 and 256 bits

\verbatim
  Example of ECDSA-P256 - TLV format - signature that must be normalized
  30 45
        02
        20
        00 33 30 6B 74 4E 8B FE 3C 22 6F A2 E4 76 F6 62 CE 09 5B DD 5E 54 A3 DD D0 B5 E7 1B 28 4D F1 63
        ^^ ^^
        02
        21
        00 9F 9E 24 81 7F FD CF 37 7E 35 DA 05 D7 17 60 2D C1 05 E7 24 22 EE F5 59 AC 25 9F 40 AB FE EC 56
\endverbatim

 @param[in,out] asnSignature     IN: buffer containing the ECDSA signature in the A7x specific format; OUT: Signature compliant to ASN.1
 @param[in,out] asnSignatureLen  IN: length of ECDSA signature in A7x specific format;
   OUT: Length of signature in ASN.1 format (always less than or equal to length of A7x specific format)
 @returns ::SW_OK upon successfull execution
*/
U16 hcNormalizeAsnSignatureEcc(U8 *asnSignature, U16 *asnSignatureLen)
{
    U16 rv = ERR_GENERAL_ERROR;
    int i = 0;
    int component = 0;
    U8 asnByteCount = ASN_P256_BYTE_COUNT;

    ENSURE_OR_GO_EXIT(asnSignature != NULL);
    ENSURE_OR_GO_EXIT(asnSignatureLen != NULL);

#ifdef AX_HC_ASN_LOG_VERBOSE
    axPrintByteArray("Before normalization", asnSignature, *asnSignatureLen, AX_COLON_32);
#endif

    // Check whether the signature length matches one of the supported keylengths and set
    // the matching asnByteCount.
    // The case where both r and s are extended with a zero byte in position ASN_P256_BYTE_COUNT + 1 must not be handled.
    // Other cases are flagged as an error
    switch (*asnSignatureLen)
    {
        case (2 * (ASN_P192_BYTE_COUNT+2) + 2):
        case (2 * (ASN_P192_BYTE_COUNT+2) + 3):
            asnByteCount = ASN_P192_BYTE_COUNT;
        break;

        case (2 * (ASN_P224_BYTE_COUNT+2) + 2):
        case (2 * (ASN_P224_BYTE_COUNT+2) + 3):
            asnByteCount = ASN_P224_BYTE_COUNT;
        break;

        case (2 * (ASN_P256_BYTE_COUNT+2) + 2):
        case (2 * (ASN_P256_BYTE_COUNT+2) + 3):
            asnByteCount = ASN_P256_BYTE_COUNT;
        break;

        case (2 * (ASN_P192_BYTE_COUNT+2) + 4):
        case (2 * (ASN_P224_BYTE_COUNT+2) + 4):
        case (2 * (ASN_P256_BYTE_COUNT+2) + 4):
            return rv;

        default:
            // printf("Skip normalization because of unexpected SignatureLen.\r\n");
            return ERR_API_ERROR;
    }

    if (asnSignature[0] == ASN_TAG_SEQUENCE)
    {
        if ( (asnSignature[1]+2) == *asnSignatureLen)
        {
            i = 2;
            for (component=0; component<2; component++)
            {
                // printf("Debug (%s - line:%d): component = %d\r\n", __FILE__, __LINE__, component);
                if (asnSignature[i] == ASN_TAG_INTEGER)
                {
                    if (asnSignature[i+1] == asnByteCount)
                    {
                        // Check whether Zero extension byte must be removed
                        if ( (asnSignature[i+2] == 0x00) && (asnSignature[i+3] <= 0x7F) )
                        {
                            U8 j;
                            // Check whether there are more 0x00 bytes to strip (start again from initial position)
                            // 'j' will match the amount of leading 0x00 byte to strip when leaving the loop
                            for (j=0; j<(asnByteCount-1); j++)
                            {
                                if ( (asnSignature[i+2+j] == 0x00) && (asnSignature[i+3+j] <= 0x7F) )
                                {
                                    continue;
                                }
                                break;
                            }
                            // Check on possible buffer overflow
                            if (*asnSignatureLen <= (i + 2 + j))
                            {
                                return ERR_WRONG_RESPONSE;
                            }
                            memmove( &asnSignature[i+2], &asnSignature[i+2+j], *asnSignatureLen - (i + 2 + j) );
                            asnSignature[1] -= j;
                            asnSignature[i+1] -= j;
                            *asnSignatureLen -= j;
                            i += 2 + asnSignature[i+1];
                        }
                        else
                        {
                            i += 2 + asnSignature[i+1];
                            // printf("asnSignature[i+2] = 0x%02X\r\n", asnSignature[i+2]);
                            // printf("asnSignature[i+3] = 0x%02X\r\n", asnSignature[i+3]);
                        }
                    }
                    else
                    {
                        i += 2 + asnSignature[i+1];
                        // printf("asnSignature[i+1] = 0x%02X\r\n", asnSignature[i+1]);
                    }
                }
                else
                {
#ifdef AX_HC_ASN_LOG_VERBOSE
                    printf("Debug (%s - line:%d): Failed to parse ASN_ECDSA_SIGNATURE.\r\n", __FILE__, __LINE__);
#endif
                    *asnSignatureLen = 0;
                    return ERR_WRONG_RESPONSE;
                }
            }
        }
        else
        {
#ifdef AX_HC_ASN_LOG_VERBOSE
            printf("Debug (%s - line:%d): Failed to parse ASN_ECDSA_SIGNATURE.\r\n", __FILE__, __LINE__); // DEBUG
#endif
            *asnSignatureLen = 0;
            return ERR_WRONG_RESPONSE;
        }
    }
    else
    {
#ifdef AX_HC_ASN_LOG_VERBOSE
        printf("Debug (%s - line:%d): Failed to parse ASN_ECDSA_SIGNATURE.\r\n", __FILE__, __LINE__); // DEBUG
#endif
        *asnSignatureLen = 0;
        return ERR_WRONG_RESPONSE;
    }

    rv = SW_OK;

exit:
    return rv;
}
