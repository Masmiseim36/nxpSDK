/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* This is the header file for the security module.
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _SEC_LIB_H_
#define _SEC_LIB_H_


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"

#ifndef gSecLibUseMutex_c
#define gSecLibUseMutex_c   TRUE
#endif

#define mDbgRevertKeys_d    0
/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */

/* CCM operation */
#define gSecLib_CCM_Encrypt_c 0
#define gSecLib_CCM_Decrypt_c 1

#define AES_BLOCK_SIZE     16 /* [bytes] */
#define AESSW_BLK_SIZE     (AES_BLOCK_SIZE)

#define SHA1_HASH_SIZE     20 /* [bytes] */
#define SHA1_BLOCK_SIZE    64 /* [bytes] */

#define SHA256_HASH_SIZE   32 /* [bytes] */
#define SHA256_BLOCK_SIZE  64 /* [bytes] */

#define gHmacIpad_c               0x36
#define gHmacOpad_c               0x5C


/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
    gSecSuccess_c,
    gSecAllocError_c,
    gSecError_c,
    gSecInvalidPublicKey_c
} secResultType_t;

typedef struct sha1Context_tag{
    uint32_t hash[SHA1_HASH_SIZE/sizeof(uint32_t)];
    uint8_t  buffer[SHA1_BLOCK_SIZE];
    uint32_t totalBytes;
    uint8_t  bytes;
}sha1Context_t;

typedef struct sha256Context_tag{
    uint32_t hash[SHA256_HASH_SIZE/sizeof(uint32_t)];
    uint8_t  buffer[SHA256_BLOCK_SIZE];
    uint32_t totalBytes;
    uint8_t  bytes;
}sha256Context_t;

typedef struct HMAC_SHA256_context_tag{
    sha256Context_t shaCtx;
    uint8_t pad[SHA256_BLOCK_SIZE];
}HMAC_SHA256_context_t;

typedef enum ecdhStatus_tag {
    gEcdhSuccess_c,
    gEcdhBadParameters_c,
    gEcdhOutOfMemory_c,
    gEcdhRngError_c,
    gEcdhInvalidState_c,
    gEcdhInvalidPublicKey_c
} ecdhStatus_t;

typedef union ecdhPrivateKey_tag {
    uint8_t  raw_8bit[32];
    uint32_t raw_32bit[8];
} ecdhPrivateKey_t;

typedef PACKED_UNION ecdhPoint_tag {
    uint8_t raw[64];
    PACKED_STRUCT {
        uint8_t x[32];
        uint8_t y[32];
    } components_8bit;
    PACKED_STRUCT {
        uint32_t x[8];
        uint32_t y[8];
    } components_32bit;
} ecdhPoint_t;

typedef ecdhPoint_t ecdhPublicKey_t;

typedef ecdhPoint_t ecdhDhKey_t;

/* Security block definition */
typedef union
{
    uint8_t    au8[AES_BLOCK_SIZE];
    uint32_t   au32[AES_BLOCK_SIZE / 4];
} tuAES_Block;

/* For backwards compatibility */
typedef tuAES_Block AESSW_Block_u;

typedef struct
{
    uint32_t u32register0;
    uint32_t u32register1;
    uint32_t u32register2;
    uint32_t u32register3;
} tsReg128;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
/*! *********************************************************************************
* This is the size of the multiplication buffer that needs to be provided to internal
* ECDH APIs.
********************************************************************************** */
extern const uint32_t gEcP256_MultiplicationBufferSize_c;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function performs initialization of the cryptografic HW acceleration.
*
********************************************************************************** */
void SecLib_Init(void);

/*! *********************************************************************************
* \brief  This function performs AES-128 encryption on a 16-byte block.
*
* \param[in]  pInput Pointer to the location of the 16-byte plain text block.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address alligned to 4 bytes!
*
********************************************************************************** */
void AES_128_Encrypt(const uint8_t* pInput,
                     const uint8_t* pKey,
                     uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128 decryption on a 16-byte block.
*
* \param[in]  pInput Pointer to the location of the 16-byte plain text block.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address alligned to 4 bytes!
*
********************************************************************************** */
void AES_128_Decrypt(const uint8_t* pInput,
                     const uint8_t* pKey,
                     uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-ECB encryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_ECB_Encrypt(uint8_t* pInput,
                         uint32_t inputLen,
                         uint8_t* pKey,
                         uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-ECB encryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  numBlocks Input message number of 16-byte blocks.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address alligned to 4 bytes!
*
********************************************************************************** */
void AES_128_ECB_Block_Encrypt(uint8_t* pInput,
                               uint32_t numBlocks,
                               const uint8_t* pKey,
                               uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC encryption on a message block.
*         This function accepts arbitrary (non-multiple of AES block size) length data
*         in which case it can be used for data signing but the output in this case will
*         not be AES 128 CBC decryptable.
*         To be able to decrypt the output please use an input length which is a multiple of
*         the AES block size.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_CBC_Encrypt(uint8_t* pInput,
                         uint32_t inputLen,
                         uint8_t* pInitVector,
                         uint8_t* pKey,
                         uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC encryption on a message block after
*padding it with 1 bit of 1 and 0 bits trail.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
*             IMPORTANT: User must make sure that input and output
*             buffers have at least inputLen + 16 bytes size
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* Return value: size of output buffer (after padding)
*
********************************************************************************** */
uint32_t AES_128_CBC_Encrypt_And_Pad(uint8_t* pInput,
                                     uint32_t inputLen,
                                     uint8_t* pInitVector,
                                     uint8_t* pKey,
                                     uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC decryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* Return value: size of output buffer (after depadding the 0x80 0x00 ... padding sequence)
*
********************************************************************************** */
uint32_t AES_128_CBC_Decrypt_And_Depad(uint8_t* pInput,
                                       uint32_t inputLen,
                                       uint8_t* pInitVector,
                                       uint8_t* pKey,
                                       uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CTR encryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pCounter Pointer to the location of the 128-bit counter.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_CTR(uint8_t* pInput,
                 uint32_t inputLen,
                 uint8_t* pCounter,
                 uint8_t* pKey,
                 uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-OFB encryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pInitVector Pointer to the location of the 128-bit initialization vector.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
********************************************************************************** */
void AES_128_OFB(uint8_t* pInput,
                 uint32_t inputLen,
                 uint8_t* pInitVector,
                 uint8_t* pKey,
                 uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CMAC on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes. The input data must be provided MSB first.
*
* \param[in]  pKey Pointer to the location of the 128-bit key. The key must be provided MSB first.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code. The code will be generated MSB first.
*
* \remarks This is public open source code! Terms of use must be checked before use!
*
********************************************************************************** */
void AES_128_CMAC(uint8_t* pInput,
                  uint32_t inputLen,
                  uint8_t* pKey,
                  uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CMAC on a message block accepting input data
*         which is in LSB first format and computing the authentication code starting fromt he end of the data.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes. The input data must be provided LSB first.
*
* \param[in]  pKey Pointer to the location of the 128-bit key. The key must be provided MSB first.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code. The code will be generated MSB first.
*
********************************************************************************** */
void AES_128_CMAC_LsbFirstInput (uint8_t* pInput,
                                 uint32_t inputLen,
                                 uint8_t* pKey,
                                 uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES 128 CMAC Pseudo-Random Function (AES-CMAC-PRF-128),
*         according to rfc4615, on a message block.
*
* \details The AES-CMAC-PRF-128 algorithm behaves similar to the AES CMAC 128 algorithm
*          but removes 128 bit key size restriction.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes.
*
* \param[in]  pVarKey Pointer to the location of the variable length key.
*
* \param[in]  varKeyLen Length of the input key in bytes
*
* \param[out]  pOutput Pointer to the location to store the 16-byte pseudo random variable.
*
********************************************************************************** */
void AES_CMAC_PRF_128(uint8_t* pInput,
                      uint32_t inputLen,
                      uint8_t* pVarKey,
                      uint32_t varKeyLen,
                      uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-EAX encryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes.
*
* \param[in]  pNonce Pointer to the location of the nonce.
*
* \param[in]  nonceLen Nonce length in bytes.
*
* \param[in]  pHeader Pointer to the location of header.
*
* \param[in]  headerLen Header length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
*
* \param[out]  pTag Pointer to the location to store the 128-bit tag.
*
********************************************************************************** */
secResultType_t AES_128_EAX_Encrypt(uint8_t* pInput,
                                    uint32_t inputLen,
                                    uint8_t* pNonce,
                                    uint32_t nonceLen,
                                    uint8_t* pHeader,
                                    uint8_t headerLen,
                                    uint8_t* pKey,
                                    uint8_t* pOutput,
                                    uint8_t* pTag);

/*! *********************************************************************************
* \brief  This function performs AES-128-EAX decryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Length of the input message in bytes.
*
* \param[in]  pNonce Pointer to the location of the nonce.
*
* \param[in]  nonceLen Nonce length in bytes.
*
* \param[in]  pHeader Pointer to the location of header.
*
* \param[in]  headerLen Header length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte authentication code.
*
* \param[out]  pTag Pointer to the location to store the 128-bit tag.
*
********************************************************************************** */
secResultType_t AES_128_EAX_Decrypt(uint8_t* pInput,
                                    uint32_t inputLen,
                                    uint8_t* pNonce,
                                    uint32_t nonceLen,
                                    uint8_t* pHeader,
                                    uint8_t headerLen,
                                    uint8_t* pKey,
                                    uint8_t* pOutput,
                                    uint8_t* pTag);

/*! *********************************************************************************
* \brief  This function performs AES-128-CCM on a message block.
*
* \param[in]  pInput       Pointer to the location of the input message (plaintext or cyphertext).
*
* \param[in]  inputLen     Length of the input plaintext in bytes when encrypting.
*                          Length of the input cypertext without the MAC length when decrypting.
*
* \param[in]  pAuthData    Pointer to the additional authentication data.
*
* \param[in]  authDataLen  Length of additional authentication data.
*
* \param[in]  pNonce       Pointer to the Nonce.
*
* \param[in]  nonceSize    The size of the nonce (7-13).
*
* \param[in]  pKey         Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput     Pointer to the location to store the plaintext data when ecrypting.
*                          Pointer to the location to store the cyphertext data when ecrypting.
*
* \param[out]  pCbcMac     Pointer to the location to store the Message Authentication Code (MAC) when encrypting.
*                          Pointer to the location where the received MAC can be found when decrypting.
*
* \param[out]  macSize     The size of the MAC.
*
* \param[out]  flags       Select encrypt/decrypt operations (gSecLib_CCM_Encrypt_c, gSecLib_CCM_Decrypt_c)
*
********************************************************************************** */
uint8_t AES_128_CCM(uint8_t* pInput,
                    uint16_t inputLen,
                    uint8_t* pAuthData,
                    uint16_t authDataLen,
                    uint8_t* pNonce,
                    uint8_t  nonceSize,
                    uint8_t* pKey,
                    uint8_t* pOutput,
                    uint8_t* pCbcMac,
                    uint8_t  macSize,
                    uint32_t flags);

/*! *********************************************************************************
* \brief  This function initializes the SHA1 context data
*
* \param [in]    ctx        Pointer to the SHA1 context data
*
********************************************************************************** */
void SHA1_Init(sha1Context_t* context);

/*! *********************************************************************************
* \brief  This function performs SHA1 on multiple bytes, and updates the context data
*
* \param [in]    pData      Pointer to the input data
* \param [in]    numBytes   Number of bytes to hash
* \param [in]    context    Pointer to the SHA1 context data
*
********************************************************************************** */
void SHA1_HashUpdate(sha1Context_t* context,
                     uint8_t* pData,
                     uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function performs SHA1 on the last bytes of data, and updates the context data.
*         The final hash value is stored on the context data.
*
* \param [in]    pData      Pointer to the input data
* \param [in]    numBytes   Number of bytes to hash
* \param [in]    context    Pointer to the SHA1 context data
*
********************************************************************************** */
void SHA1_HashFinish(sha1Context_t* context,
                     uint8_t* pData,
                     uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function performs all SHA1 steps on multiple bytes: initialize,
*         update, finish, and update context data.
*         The final hash value is stored on the context data.
*
* \param [in]    pData      Pointer to the input data
* \param [in]    numBytes   Number of bytes to hash
* \param [in]    context    Pointer to the SHA1 context data
*
********************************************************************************** */
void SHA1_Hash(sha1Context_t* context,
               uint8_t* pData,
               uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function initializes the SHA256 context data
*
* \param [in]    context        Pointer to the SHA256 context data
*
********************************************************************************** */
void SHA256_Init(sha256Context_t* context);

/*! *********************************************************************************
* \brief  This function performs SHA256 on multiple bytes, and updates the context data
*
* \param [in]    pData      Pointer to the input data
* \param [in]    numBytes   Number of bytes to hash
* \param [in]    context    Pointer to the SHA256 context data
*
********************************************************************************** */
void SHA256_HashUpdate(sha256Context_t* context,
                       uint8_t* pData,
                       uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function performs SHA256 on the last bytes of data, and updates the context data.
*         The final hash value is stored on the context data.
*
* \param [in]    pData      Pointer to the input data
* \param [in]    numBytes   Number of bytes to hash
* \param [in]    context    Pointer to the SHA256 context data
*
********************************************************************************** */
void SHA256_HashFinish(sha256Context_t* context,
                       uint8_t* pData,
                       uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function performs all SHA256 steps on multiple bytes: initialize,
*         update, finish, and update context data.
*         The final hash value is stored on the context data.
*
* \param [in]    pData      Pointer to the input data
* \param [in]    numBytes   Number of bytes to hash
* \param [in]    context    Pointer to the SHA256 context data
*
********************************************************************************** */
void SHA256_Hash(sha256Context_t* context,
                 uint8_t* pData,
                 uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function performs the initialization of the HMAC context data, and
*         performs the initial hash.
*
* \param [in]    context    Pointer to the HMAC context data
* \param [in]    pKey       Pointer to the key
* \param [in]    keyLen     Length of the key
*
********************************************************************************** */
void HMAC_SHA256_Init(HMAC_SHA256_context_t* context,
                      uint8_t* pKey,
                      uint32_t keyLen);

/*! *********************************************************************************
* \brief  This function performs HMAC update on the input message.
*         The intermediar hash value is stored on the context data.
*
* \param [in]    context    Pointer to the HMAC context data
* \param [in]    pMsg       Pointer to the input data
* \param [in]    msgLen     Length of the message
*
********************************************************************************** */
void HMAC_SHA256_Update(HMAC_SHA256_context_t* context,
                        uint8_t* pData,
                        uint32_t numBytes);

/*! *********************************************************************************
* \brief  This function performs the final step of the HMAC
*         The final hash value is stored on the context data.
*
* \param [in]    context        Pointer to the HMAC context data
*
********************************************************************************** */
void HMAC_SHA256_Finish(HMAC_SHA256_context_t* context);

/*! *********************************************************************************
* \brief  This function performs all HMAC steps on multiple bytes: initialize,
*         update, finish, and update context data.
*         The final hash value is stored on the context data.
*
* \param [in]    context    Pointer to the HMAC context data
* \param [in]    pKey       Pointer to the key
* \param [in]    keyLen     Length of the key
* \param [in]    pMsg       Pointer to the input data
* \param [in]    msgLen     Length of the message
*
********************************************************************************** */
void HMAC_SHA256(HMAC_SHA256_context_t* context,
                 uint8_t* pKey,
                 uint32_t keyLen,
                 uint8_t* pMsg,
                 uint32_t msgLen);

/************************************************************************************
* Calculate XOR of individual byte pairs in two uint8_t arrays. I.e.
* pDst[i] := pDst[i] ^ pSrc[i] for i=0 to n-1
*
* Interface assumptions:
*   None
*
* Return value:
*   None
*
************************************************************************************/
void SecLib_XorN(uint8_t* pDst, /* First operand and result of XOR operation */
                 uint8_t* pSrc, /* Second operand. Not modified. */
                 uint8_t n);    /* Number of bytes in input arrays. */


/************************************************************************************
* \brief Seeds the random number generator
*
* \param [in] seed  The new seed for the RNG algorithm
*
* \return 0 if success, 1 on error
*
************************************************************************************/
uint32_t SecLib_set_rng_seed(uint32_t seed);

/************************************************************************************
* \brief Generates a new random number
*
* \return random number
*
************************************************************************************/
uint32_t SecLib_get_random(void);

/************************************************************************************
* \brief Generates a new Private/Public Key Pair
*
* \return gSecSuccess_c or error
*
************************************************************************************/
secResultType_t ECDH_P256_GenerateKeys
(
    ecdhPublicKey_t*    pOutPublicKey,
    ecdhPrivateKey_t*   pOutPrivateKey
);

/************************************************************************************
* \brief Computes Diffie-Hellman key
*
* \return gSecSuccess_c or error
*
************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKey
(
    ecdhPrivateKey_t*   pPrivateKey,
    ecdhPublicKey_t*    pPeerPublicKey,
    ecdhDhKey_t*        pOutDhKey
);

/****************************************************************************
 * \brief Perform an MMO Block Update on the hash
 *        H[j] = E(H[j-1], M[j]) ^ M[j]
 *        where E(K,x) = AES-128 block cipher, K=key, x=text
 *
 * \param [in] psMMO   MMO object
 * \param [in] puBlock Block to hash
 *
 ****************************************************************************/
void AESSW_vMMOBlockUpdate(AESSW_Block_u *puHash,
                           AESSW_Block_u *puBlock);

/****************************************************************************
 * \brief  Perform the final update on the MMO, running text through it
 *         H[0] = 0; H[j] = E(H[j-1], M[j]) ^ M[j] for j=1..t
 *         E(K,x) = AES-128 block cipher, K=key, x=text
 *
 * \param [in] psMMO     MMO object
 * \param [in] pu8Data   Text to run through
 * \param [in] iDataLen  Length of text to run through
 * \param [in] iFinalLen Final length of buffer
 *
 ****************************************************************************/
void AESSW_vMMOFinalUpdate(AESSW_Block_u *puHash,
                           uint8_t *pu8Data,
                           int iDataLen,
                           int iFinalLen);

/****************************************************************************
 * \brief  Perform the HMAC-MMO Keyed Hash Function for Message Authentication
 *         Specified in B.1.4 in ZigBee specification (053474r17)
 *
 * \param [in]  pu8Data  Pointer to data stream
 * \param [in]  iDataLen Length of data stream
 * \param [in]  puKey    Key
 * \param [out] puHash   Output hash
 *
 ****************************************************************************/
void AESSW_vHMAC_MMO(uint8_t *pu8Data,
                     int iDataLen,
                     AESSW_Block_u *puKeyData,
                     AESSW_Block_u *puHash);

/****************************************************************************
 * \brief This function setups the key in the AES.
 *
 * \param [in] psKeyData               tsReg128 key split over 4 words (16 bytes)
 *
 ****************************************************************************/
bool_t bACI_WriteKey(tsReg128 *psKeyData);

/****************************************************************************
 * \brief  This function performs CCM* AES encryption/decryption with checksum
 *         generation/verification. Assumes input buffers are aligned and nonce has a
 *         free byte at the start for the flags field
 *
 * \param [in] bEncrypt               TRUE to encrypt, FALSE to decrypt
 * \param [in] u8M                    Required number of checksum bytes
 * \param [in] u8alength              Length of authentication data, in bytes
 * \param [in] u8mlength              Length of input data, in bytes
 * \param [in] puNonce                Pre-allocated pointer to a structure
 *                                    containing 128-bit Nonce data
 * \param [in] pau8authenticationData Pre-allocated pointer to byte array of
 *                                    authentication data
 * \param [in] pau8Data               Pre-allocated pointer to byte array of input
 *                                    and output data
 * \param [in] pau8checksumData       Pre-allocated pointer to byte array of
 *                                    checksum data
 * \param [in] pbChecksumVerify       Pre-allocated pointer to boolean which in CCM
 *                                    decode mode stores the result of the checksum
 *                                    verification operation
 *
 ****************************************************************************/
void vACI_OptimisedCcmStar(bool_t         bEncrypt,
                           uint8_t        u8M,
                           uint8_t        u8alength,
                           uint8_t        u8mlength,
                           tuAES_Block   *puNonce,
                           uint8_t       *pau8authenticationData,
                           uint8_t       *pau8Data,
                           uint8_t       *pau8checksumData,
                           bool_t        *pbChecksumVerify);
/****************************************************************************
 *
 * NAME:       u32Reverse
 */
/**
 * Reverses 32bit data
 *
 * @ingroup grp_aes_sw
 *
 * @param u32InWord  Input 32-bit data
 *
 * @return u32OutWord Output reversed 32- bit data
 *
 * @note
 *
 ****************************************************************************/
uint32_t u32Reverse(uint32_t u32InWord);

/****************************************************************************
 *
 * NAME:       vSwipeEndian
 */
/**
 * Reverses 1282bit data between AESSW_Block_u and tsReg128
 *
 * @ingroup grp_aes_sw
 *
 * @param puBlock  128 bit data of type AESSW_Block_u
 *
 * @param psReg  128 bit data of type  tsReg128
 *
 * @param bBlockToReg  direction of converesion
 *                      1 = AESSW_Block_u to tsReg128
 *                      0 = tsReg128 to AESSW_Block_u
 *
 * @return
 *
 * @note
 *
 ****************************************************************************/
void vSwipeEndian( AESSW_Block_u *puBlock,
                   tsReg128 *psReg,
                   bool_t bBlockToReg );
#endif /* _SEC_LIB_H_ */
