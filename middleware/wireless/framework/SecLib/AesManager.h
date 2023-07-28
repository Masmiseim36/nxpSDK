/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the header file for the AES Manager interface.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef __AES_MANAGER_H__
#define __AES_MANAGER_H__

/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "EmbeddedTypes.h"

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define USE_TASK_FOR_HW_AES (0)

/* The number of AES interfaces to be used.
0 - means that AES Manager is disabled */
#ifndef gAESManagerMaxNumbers_c
#define gAESManagerMaxNumbers_c (6)
#endif

/* AES Manager Task specification */
#ifndef gAESTaskStackSize_c
#define gAESTaskStackSize_c (1024) /* bytes */
#endif
#ifndef gAESTaskPriority_c
#define gAESTaskPriority_c (3)
#endif

/*! *********************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
********************************************************************************** */
#define gAESMgrInvalidIdx_c (0xFF)

#define ENC_NONE     0
#define ENC_PENDING  1
#define ENC_COMPLETE 2

/*! *********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
********************************************************************************** */
/* Define the types of AES type */
typedef enum
{
    gAESMGR_None_c          = 0,
    gAESMGR_ECB_Enc_c       = 1,
    gAESMGR_ECB_Dec_c       = 2,
    gAESMGR_ECB_Block_Enc_c = 3,
    gAESMGR_ECB_Block_Dec_c = 4,
    gAESMGR_CTR_Enc_c       = 5,
    gAESMGR_CTR_Dec_c       = 6,
    gAESMGR_CMAC_Enc_c      = 7,
} AESType_t;

/* AES Manager status codes */
typedef enum
{
    gAES_Success_c           = 0,
    gAES_InvalidParameter_c  = 1,
    gAES_Non_excution_c      = 2,
    gAES_MaxNumbersReached_c = 3,
    gAES_OsError_c           = 4,
    gAES_TypeInUse_c         = 5,
} AESStatus_t;

/* Defines the param of encryption  */
typedef struct AES_param
{
    uint8_t *pPlain;
    uint8_t *pCipher;
    uint8_t *Key;         /* 16 bytes for HW AES128 */
    uint8_t *CTR_counter; /* 16 bytes for HW AES128 */
    uint8_t *pInitVector; /* 16 bytes for HW AES128 */
    uint32_t Len;
    uint32_t Blocks;
} AES_param_t;

/* AES Manager callback type */
typedef void (*pAESCallBack_t)(AES_param_t param);

/* User callback type for AES complete   */
typedef void (*pAESCompleteCallBack_t)(uint8_t *input, uint32_t len);

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

/*! *********************************************************************************
 * \brief   Creates the AESManager's task and initializes internal data structures
 *
 ********************************************************************************** */
void AESM_Initialize(void);

/*! *********************************************************************************
 * \brief   Initialize a AES type
 *
 * \param[in] pAESId    pointer to a location where the encryption Id will be stored
 * \param[in] type      the type of the AES:
 *                        gAESMGR_ECB_Enc_c
 *                        gAESMGR_ECB_Dec_c
 *                        gAESMGR_ECB_Block_Enc_c
 *                        gAESMGR_ECB_Block_Dec_c
 *                        gAESMGR_CTR_Enc_c
 *                        gAESMGR_CTR_Dec_c
 *                        gAESMGR_CMAC_Enc_c
 * \return The AES number if success or gAES_MaxNumbersReached_c when reach max numbers.
 *
 ********************************************************************************** */
AESStatus_t AESM_InitType(uint8_t *pAESId, AESType_t type);

/*! *********************************************************************************
 * \brief   Set a AES encryption/decryption parameter
 *
 * \param[in] pAESId    AES encryption/decryption type Id
 * \param[in] AES_p     the parameter of the AES HW
 * \return    gAES_InvalidParameter_c if an error occurred.
 *
 ********************************************************************************** */
AESStatus_t AESM_SetParam(uint8_t pAESId, AES_param_t AES_p, pAESCallBack_t AEScallback);

/*! *********************************************************************************
 * \brief     start encryption/decryption
 *
 * \param[in] pAESId   AES encryption/decryption type Id
 *
 * \return      gAES_InvalidParameter_c and gAES_Non_excution if an error occurred.
 *
 ********************************************************************************** */
AESStatus_t AESM_Start(uint8_t pAESId);

/*! *********************************************************************************
 * \brief     set complete event when finish encryption/decryption
 *
 * \param[in] pAESId   AES encryption/decryption type Id
 *
 * \return    gAES_InvalidParameter_c and gAES_Non_excution if an error occurred.
 *
 ********************************************************************************** */
AESStatus_t AESM_Complete(uint8_t pAESId);

/*! *********************************************************************************
 * \brief     get state of encryption/decryption
 *
 * \param[in] pAESId   AES encryption/decryption type Id
 *
 * \return    encryption/decryption state
 *
 ********************************************************************************** */
uint8_t AESM_GetState(uint8_t pAESId);

/*! *********************************************************************************
 * \brief     set callback function ,when encryption/decryption complete it will be execute
 *
 * \param[in] pAESId     AES encryption/decryption type Id
 *
 ********************************************************************************** */
void AESM_SetCompletionCallback(uint8_t pAESId, pAESCompleteCallBack_t cb);

#endif /* __AES_MANAGER_H__ */