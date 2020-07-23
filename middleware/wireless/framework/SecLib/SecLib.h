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

#ifndef SEC_LIB_H
#define SEC_LIB_H


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "CryptoLibSW.h"

#ifndef gSecLibUseMutex_c
#define gSecLibUseMutex_c   TRUE
#endif

#define mRevertEcdhKeys_d   1

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
/* AES 128 */
#define AES_128_KEY_BITS         128u
#define AES_128_BLOCK_SIZE       16u /* [bytes] */

/* CCM */
#define gSecLib_CCM_Encrypt_c 0u
#define gSecLib_CCM_Decrypt_c 1u

#define AES_BLOCK_SIZE     16u /* [bytes] */
#define AESSW_BLK_SIZE     (AES_BLOCK_SIZE)

/* Hashes */
#define SHA1_HASH_SIZE     20u /* [bytes] */
#define SHA1_BLOCK_SIZE    64u /* [bytes] */

#define SHA256_HASH_SIZE   32u /* [bytes] */
#define SHA256_BLOCK_SIZE  64u /* [bytes] */

#define gHmacIpad_c               0x36u
#define gHmacOpad_c               0x5Cu

/*! Enable or disable SHA-1 functionality in the SecLib module. */
#ifndef gSecLibSha1Enable_d
#define gSecLibSha1Enable_d     1
#endif

/*! Enable or disable SHA-256 functionality in the SecLib module. */
#ifndef gSecLibSha256Enable_d
#define gSecLibSha256Enable_d   1
#endif

#ifndef cPWR_UsePowerDownMode
#define cPWR_UsePowerDownMode 0
#endif

/*! How many steps to use for the EC multiplication procedure */
#ifndef gSecLibEcStepsAtATime
#define gSecLibEcStepsAtATime   16U
#endif

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
typedef enum
{
    gSecSuccess_c           = 0u,
    gSecAllocError_c        = 1u,
    gSecError_c             = 2u,
    gSecInvalidPublicKey_c  = 3u,
    gSecResultPending_c     = 4u
} secResultType_t;

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

typedef void(*secLibCallback_t)(computeDhKeyParam_t* pData);
extern secLibCallback_t pfSecLibMultCallback;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
* \brief  This function performs initialization of the cryptographic HW acceleration.
*
********************************************************************************** */
void SecLib_Init (void);

/*! *********************************************************************************
* \brief  This function performs initialization of the callback used to offload
* elliptic curve multiplication.
*
* \param[in]  pfCallback Pointer to the function used to handle multiplication.
*
********************************************************************************** */
void SecLib_SetExternalMultiplicationCb (secLibCallback_t pfCallback);

/*! *********************************************************************************
* \brief  This function performs calls the multiplication Callback.
*
* \param[in]  pMsg Pointer to the data used in multiplication.
*
********************************************************************************** */
void SecLib_ExecMultiplicationCb (computeDhKeyParam_t *pMsg);

/*! *********************************************************************************
* \brief  This function performs AES-128 encryption on a 16-byte block.
*
* \param[in]  pInput Pointer to the location of the 16-byte plain text block.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
*
********************************************************************************** */
void AES_128_Encrypt (const uint8_t*    pInput,
                      const uint8_t*    pKey,
                      uint8_t*          pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128 decryption on a 16-byte block.
*
* \param[in]  pInput Pointer to the location of the 16-byte plain text block.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the 16-byte ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
*
********************************************************************************** */
void AES_128_Decrypt (const uint8_t*    pInput,
                      const uint8_t*    pKey,
                      uint8_t*          pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-ECB encryption on a message block.
*         This function only accepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
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
void AES_128_ECB_Encrypt (uint8_t*  pInput, 
                          uint32_t  inputLen, 
                          uint8_t*  pKey, 
                          uint8_t*  pOutput);

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
* \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
*
********************************************************************************** */
void AES_128_ECB_Block_Encrypt(uint8_t* pInput,
                               uint32_t numBlocks,
                               const uint8_t* pKey,
                               uint8_t* pOutput);

#ifdef FSL_FEATURE_SOC_AES_HW
/*! *********************************************************************************
* \brief  This function performs AES-128-ECB decryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  numBlocks Input message number of 16-byte blocks.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
*
********************************************************************************** */
void AES_128_ECB_Block_Decrypt(uint8_t* pInput,
                               uint32_t numBlocks,
                               const uint8_t* pKey,
                               uint8_t* pOutput);
#endif /* FSL_FEATURE_SOC_AES_HW */

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC encryption on a message block.
*         This function only accepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
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
void AES_128_CBC_Encrypt (uint8_t*  pInput, 
                          uint32_t  inputLen,
                          uint8_t*  pInitVector, 
                          uint8_t*  pKey, 
                          uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CBC encryption on a message block after
*         padding it with 1 bit of 1 and 0 bits trail.
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
uint32_t AES_128_CBC_Encrypt_And_Pad (uint8_t*  pInput, 
                                      uint32_t  inputLen,
                                      uint8_t*  pInitVector, 
                                      uint8_t*  pKey, 
                                      uint8_t*  pOutput);

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
uint32_t AES_128_CBC_Decrypt_And_Depad (uint8_t*    pInput, 
                                        uint32_t    inputLen,
                                        uint8_t*    pInitVector, 
                                        uint8_t*    pKey, 
                                        uint8_t*    pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CTR encryption on a message block.
*         This function only accepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
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
void AES_128_CTR (uint8_t*  pInput, 
                  uint32_t  inputLen,
                  uint8_t*  pCounter, 
                  uint8_t*  pKey, 
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-OFB encryption on a message block.
*         This function only accepts input lengths which are multiple
*         of 16 bytes (AES 128 block size).
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
void AES_128_OFB (uint8_t*  pInput, 
                  uint32_t  inputLen,
                  uint8_t*  pInitVector, 
                  uint8_t*  pKey, 
                  uint8_t*  pOutput);

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
void AES_128_CMAC (uint8_t* pInput, 
                   uint32_t inputLen, 
                   uint8_t* pKey, 
                   uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CMAC on a message block accepting input data
*         which is in LSB first format and computing the authentication code starting from the end of the data.
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
void AES_128_CMAC_LsbFirstInput (const uint8_t*   pInput,
                                 uint32_t   inputLen,
                                 const uint8_t*   pKey,
                                 uint8_t*   pOutput);

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
void AES_CMAC_PRF_128 (uint8_t* pInput, 
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
secResultType_t AES_128_EAX_Encrypt (uint8_t*   pInput, 
                                     uint32_t   inputLen,
                                     uint8_t*   pNonce,
                                     uint32_t   nonceLen,
                                     uint8_t*   pHeader,
                                     uint8_t    headerLen, 
                                     uint8_t*   pKey, 
                                     uint8_t*   pOutput,
                                     uint8_t*   pTag);

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
secResultType_t AES_128_EAX_Decrypt (uint8_t*   pInput, 
                                     uint32_t   inputLen,
                                     uint8_t*   pNonce,
                                     uint32_t   nonceLen,
                                     uint8_t*   pHeader,
                                     uint8_t    headerLen, 
                                     uint8_t*   pKey, 
                                     uint8_t*   pOutput,
                                     uint8_t*   pTag);
                                    
/*! *********************************************************************************
* \brief  This function performs AES-128-CCM on a message block.
*
* \param[in]  pInput       Pointer to the location of the input message (plaintext or cyphertext).
*
* \param[in]  inputLen     Length of the input plaintext in bytes when encrypting.
*                          Length of the input cyphertext without the MAC length when decrypting.
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
* \param[out]  pOutput     Pointer to the location to store the plaintext data when encrypting.
*                          Pointer to the location to store the cyphertext data when encrypting.
*
* \param[out]  pCbcMac     Pointer to the location to store the Message Authentication Code (MAC) when encrypting.
*                          Pointer to the location where the received MAC can be found when decrypting.
*
* \param[out]  macSize     The size of the MAC.
*
* \param[out]  flags       Select encrypt/decrypt operations (gSecLib_CCM_Encrypt_c, gSecLib_CCM_Decrypt_c)
*
********************************************************************************** */
uint8_t AES_128_CCM (uint8_t*   pInput,
                     uint16_t   inputLen,
                     uint8_t*   pAuthData,
                     uint16_t   authDataLen,
                     uint8_t*   pNonce,
                     uint8_t    nonceSize,
                     uint8_t*   pKey,
                     uint8_t*   pOutput,
                     uint8_t*   pCbcMac,
                     uint8_t    macSize,
                     uint32_t   flags);

/*! *********************************************************************************
* \brief  This function allocates a memory buffer for a SHA1 context structure
*
* \return    Address of the SHA1 context buffer
*            Deallocate using SHA1_FreeCtx()
*
********************************************************************************** */
void* SHA1_AllocCtx (void);

/*! *********************************************************************************
* \brief  This function deallocates the memory buffer for the SHA1 context structure
*
* \param [in]    pContext    Address of the SHA1 context buffer
*
********************************************************************************** */
void SHA1_FreeCtx (void* pContext);

/*! *********************************************************************************
* \brief  This function clones a SHA1 context.
*         Make sure the size of the allocated destination context buffer is appropriate.
*
* \param [in]    pDestCtx    Address of the destination SHA1 context
* \param [in]    pSourceCtx  Address of the source SHA1 context
*
********************************************************************************** */
void SHA1_CloneCtx (void* pDestCtx, void* pSourceCtx);

/*! *********************************************************************************
* \brief  This function initializes the SHA1 context data
*
* \param [in]    pContext    Pointer to the SHA1 context data
*                            Allocated using SHA1_AllocCtx()
*
********************************************************************************** */
void SHA1_Init (void* pContext);

/*! *********************************************************************************
* \brief  This function performs SHA1 on multiple bytes and updates the context data
*
* \param [in]    pContext    Pointer to the SHA1 context data
*                            Allocated using SHA1_AllocCtx()
* \param [in]    pData       Pointer to the input data
* \param [in]    numBytes    Number of bytes to hash
*
********************************************************************************** */
void SHA1_HashUpdate (void*     pContext, 
                      uint8_t*  pData, 
                      uint32_t  numBytes);

/*! *********************************************************************************
* \brief  This function finalizes the SHA1 hash computation and clears the context data.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pContext    Pointer to the SHA1 context data
*                               Allocated using SHA1_AllocCtx()
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA1_HashFinish (void*     pContext, 
                      uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function performs all SHA1 steps on multiple bytes: initialize, 
*         update and finish.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pData       Pointer to the input data
* \param [in]       numBytes    Number of bytes to hash
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA1_Hash (uint8_t*    pData, 
                uint32_t    numBytes, 
                uint8_t*    pOutput);
                    
/*! *********************************************************************************
* \brief  This function allocates a memory buffer for a SHA256 context structure
*
* \return    Address of the SHA256 context buffer
*            Deallocate using SHA256_FreeCtx()
*
********************************************************************************** */
void* SHA256_AllocCtx (void);

/*! *********************************************************************************
* \brief  This function deallocates the memory buffer for the SHA256 context structure
*
* \param [in]    pContext    Address of the SHA256 context buffer
*
********************************************************************************** */
void SHA256_FreeCtx (void* pContext);

/*! *********************************************************************************
* \brief  This function clones a SHA256 context.
*         Make sure the size of the allocated destination context buffer is appropriate.
*
* \param [in]    pDestCtx    Address of the destination SHA256 context
* \param [in]    pSourceCtx  Address of the source SHA256 context
*
********************************************************************************** */
void SHA256_CloneCtx (void* pDestCtx, void* pSourceCtx);

/*! *********************************************************************************
* \brief  This function initializes the SHA256 context data
*
* \param [in]    pContext    Pointer to the SHA256 context data
*                            Allocated using SHA256_AllocCtx()
*
********************************************************************************** */
void SHA256_Init (void* pContext);

/*! *********************************************************************************
* \brief  This function performs SHA256 on multiple bytes and updates the context data
*
* \param [in]    pContext    Pointer to the SHA256 context data
*                            Allocated using SHA256_AllocCtx()
* \param [in]    pData       Pointer to the input data
* \param [in]    numBytes    Number of bytes to hash
*
********************************************************************************** */
void SHA256_HashUpdate (void*       pContext, 
                        uint8_t*    pData, 
                        uint32_t    numBytes);

/*! *********************************************************************************
* \brief  This function finalizes the SHA256 hash computation and clears the context data.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pContext    Pointer to the SHA256 context data
*                               Allocated using SHA256_AllocCtx()
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA256_HashFinish (void*       pContext, 
                        uint8_t*    pOutput);

/*! *********************************************************************************
* \brief  This function performs all SHA256 steps on multiple bytes: initialize, 
*         update and finish.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pData       Pointer to the input data
* \param [in]       numBytes    Number of bytes to hash
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void SHA256_Hash (uint8_t*  pData, 
                  uint32_t  numBytes, 
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function allocates a memory buffer for a HMAC SHA256 context structure
*
* \return    Address of the HMAC SHA256 context buffer
*            Deallocate using HMAC_SHA256_FreeCtx()
*
********************************************************************************** */
void* HMAC_SHA256_AllocCtx (void);

/*! *********************************************************************************
* \brief  This function deallocates the memory buffer for the HMAC SHA256 context structure
*
* \param [in]    pContext    Address of the HMAC SHA256 context buffer
*
********************************************************************************** */
void HMAC_SHA256_FreeCtx (void* pContext);

/*! *********************************************************************************
* \brief  This function performs the initialization of the HMAC SHA256 context data
*
* \param [in]    pContext    Pointer to the HMAC SHA256 context data
*                            Allocated using HMAC_SHA256_AllocCtx()
* \param [in]    pKey        Pointer to the HMAC key
* \param [in]    keyLen      Length of the HMAC key in bytes
*
********************************************************************************** */
void HMAC_SHA256_Init (void*    pContext,
                       uint8_t* pKey,
                       uint32_t keyLen);

/*! *********************************************************************************
* \brief  This function performs HMAC update with the input data.
*
* \param [in]    pContext    Pointer to the HMAC SHA256 context data
*                            Allocated using HMAC_SHA256_AllocCtx()
* \param [in]    pData       Pointer to the input data
* \param [in]    numBytes    Number of bytes to hash
*
********************************************************************************** */
void HMAC_SHA256_Update (void*      pContext,
                         uint8_t*   pData,
                         uint32_t   numBytes);

/*! *********************************************************************************
* \brief  This function finalizes the HMAC SHA256 computation and clears the context data.
*         The final hash value is stored at the provided output location.
*
* \param [in]       pContext    Pointer to the HMAC SHA256 context data
*                               Allocated using HMAC_SHA256_AllocCtx()
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void HMAC_SHA256_Finish (void*      pContext,
                         uint8_t*   pOutput);

/*! *********************************************************************************
* \brief  This function performs all HMAC SHA256 steps on multiple bytes: initialize, 
*         update, finish, and update context data.
*         The final HMAC value is stored at the provided output location.
*
* \param [in]       pKey        Pointer to the HMAC key
* \param [in]       keyLen      Length of the HMAC key in bytes
* \param [in]       pData       Pointer to the input data
* \param [in]       numBytes    Number of bytes to perform HMAC on
* \param [out]      pOutput     Pointer to the output location
*
********************************************************************************** */
void HMAC_SHA256 (uint8_t*  pKey,
                  uint32_t  keyLen,
                  uint8_t*  pData,
                  uint32_t  numBytes,
                  uint8_t*  pOutput);

/*! *********************************************************************************
* \brief  This function calculates XOR of individual byte pairs in two uint8_t arrays.
*         pDst[i] := pDst[i] ^ pSrc[i] for i=0 to n-1
*
* \param[in, out]  pDst First byte array operand for XOR and destination byte array
*
* \param[in]  pSrc Second byte array operand for XOR
*
* \param[in]  n  Length of the byte arrays which will be XORed
*
********************************************************************************** */
void SecLib_XorN (uint8_t*  pDst,   /* First operand and result of XOR operation */
                  uint8_t*  pSrc,   /* Second operand. Not modified. */
                  uint8_t   n);     /* Number of bytes in input arrays. */


/************************************************************************************
* \brief Generates a new ECDH P256 Private/Public key pair
*
* \return gSecSuccess_c or error
* 
************************************************************************************/
secResultType_t ECDH_P256_GenerateKeys (ecdhPublicKey_t*    pOutPublicKey,
                                        ecdhPrivateKey_t*   pOutPrivateKey);

/************************************************************************************
* \brief Generates a new ECDH P256 Private/Public key pair. This function starts the
*        ECDH generate procedure. The pDhKeyData must be allocated and kept
*        allocated for the time of the computation procedure.
*        When the result is gSecResultPending_c the memory should be kept until the
*        last step.
*        In any other result messages the data shall be cleared after this call.
*
* \param[in]  pDhKeyData Pointer to the structure holding information about the
*                        multiplication
*
* \return gSecSuccess_c, gSecResultPending_c or error
*
************************************************************************************/
secResultType_t ECDH_P256_GenerateKeysSeg (computeDhKeyParam_t *pDhKeyData);

/************************************************************************************
* \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair.
*
* \return gSecSuccess_c or error
* 
************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKey (ecdhPrivateKey_t*   pPrivateKey,
                                        ecdhPublicKey_t*    pPeerPublicKey,
                                        ecdhDhKey_t*        pOutDhKey);

/************************************************************************************
* \brief Computes the Diffie-Hellman Key for an ECDH P256 key pair. This function
*        starts the ECDH key pair generate procedure. The pDhKeyData must be
*        allocated and kept allocated for the time of the computation procedure.
*        When the result is gSecResultPending_c the memory should be kept until the
*        last step, when it can be safely freed.
*        In any other result messages the data shall be cleared after this call.
*
* \param[in]  pDhKeyData Pointer to the structure holding information about the
*                        multiplication
*
* \return gSecSuccess_c or error
*
************************************************************************************/
secResultType_t ECDH_P256_ComputeDhKeySeg (computeDhKeyParam_t *pDhKeyData);

/************************************************************************************
* \brief Handle one step of ECDH multiplication depending on the number of steps at
*        a time according to gSecLibEcStepsAtATime. After the last step is completed
*        the function returns TRUE and the upper layer is responsible for clearing
*        pData.
*
* \param[in]  pData Pointer to the structure holding information about the
*                   multiplication
*
* \return TRUE if the multiplication is completed
*         FALSE when the function needs to be called again
*
************************************************************************************/
bool_t SecLib_HandleMultiplyStep (computeDhKeyParam_t *pData);

/*! *********************************************************************************
* \brief  This function performs AES-128-ECB decryption on a message block.
*
* \param[in]  pInput Pointer to the location of the input message.
*
* \param[in]  inputLen Input message length in bytes.
*
* \param[in]  pKey Pointer to the location of the 128-bit key.
*
* \param[out]  pOutput Pointer to the location to store the ciphered output.
*
* \pre All Input/Output pointers must refer to a memory address aligned to 4 bytes!
*
********************************************************************************** */
#ifdef FSL_FEATURE_SOC_AES_HW
void AES_128_ECB_Decrypt (uint8_t* pInput,
                          uint32_t inputLen,
                          uint8_t* pKey,
                          uint8_t* pOutput);

/*! *********************************************************************************
* \brief  This function performs AES-128-CTR decryption on a message block.
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
void AES_128_CTR_Decrypt (uint8_t* pInput,
                          uint32_t inputLen,
                          uint8_t* pCounter,
                          uint8_t* pKey,
                          uint8_t* pOutput);
#endif /* FSL_FEATURE_SOC_AES_HW */

#endif /* SEC_LIB_H */
