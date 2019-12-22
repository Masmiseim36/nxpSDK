/**
* @file SE_HAL_Lib.h
* @author NXP Semiconductors
* @version 1.0
* @par License
* Copyright 2018 NXP
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
* This file provides the SE HAL Library interface for Alibaba.
* @par History
* 1.0   24-jan-2018 : Initial version
*
*****************************************************************************/
#ifndef _SE_HAL_LIB_
#define _SE_HAL_LIB_

#include "a71cl_api.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
* Opens a communication session with the SE.
* @param[in, out] handle  The communication handle
* @retval ::0 - Success; other - an error code.
*/
S32 open_session(void **handle);


/**
* Sends data to the SE and returns the processed data from it.
* @param[in] handle  The communication handle
* @param[in] cmd The command code
* @param[in] input The input data
* @param[in] input_len The input data length
* @param[out] output The output (processed) data
* @param[out] output_len The output data length
* @retval ::0 - Success; other - an error code.
*/
S32 invoke_command(void *handle, U32 cmd,
                    void *input, U32 input_len,
                    void *output, U32 *output_len);

/**
* Closes the communication session with the SE.
* @param[in] handle  The communication handle
* @retval ::0 - Success; other - an error code.
*/
S32 close_session(void *handle);


// Allow up to 16 open handles concurrently
#define MAX_SE_HAL_LIB_HANDLES      16


// Command codes
#define A71CL_SE_HAL_CMD_READ_CL                    0x41 //!< Read CL
#define A71CL_SE_HAL_CMD_UPDATE_CL_PRIV_KEY     0x42 //!< Update CL private Key
#define A71CL_SE_HAL_CMD_RSA_GENERATE_KEY           0x51 //!< Generate RSA Key
#define A71CL_SE_HAL_CMD_RSA_GET_PUB_KEY            0x52 //!< Get RSA PUB_KEY
#define A71CL_SE_HAL_CMD_RSA_PRIV_KEY_SIGN          0x53 //!< Sign using RSA Private Key
#define A71CL_SE_HAL_CMD_RSA_PUB_KEY_VERIFY     0x54 //!< Verify signature using RSA Public key
#define A71CL_SE_HAL_CMD_RSA_PUB_KEY_ENCRYPT        0x55 //!< Encrypt using RSA Public Key
#define A71CL_SE_HAL_CMD_RSA_PRIV_KEY_DECRYPT       0x56 //!< Decrypt using RSA Private Key
#define A71CL_SE_HAL_CMD_3DES_ENCRYPT               0x57 //!< 3DES Encryption
#define A71CL_SE_HAL_CMD_3DES_DECRYPT               0x58 //!< 3DES Decryption
#define A71CL_SE_HAL_CMD_3DES_MAC                   0x59 //!< 3DES Mac calculation
#define A71CL_SE_HAL_CMD_AES_ENCRYPT                0x5A //!< AES Encryption
#define A71CL_SE_HAL_CMD_AES_DECRYPT                0x5B //!< AES Decryption
#define A71CL_SE_HAL_CMD_AES_MAC                    0x5C //!< AES MAC Calculation
#define A71CL_SE_HAL_CMD_SEC_WRITE_DATA         0x61 //!< Secure Storage Write Data
#define A71CL_SE_HAL_CMD_SEC_READ_DATA              0x62 //!< Secure Storage Read Data
#define A71CL_SE_HAL_CMD_SEC_REMOVE_DATA            0x63 //!< Secure Storage Remove Data
#define A71CL_SE_HAL_CMD_HASH                       0x71 //!< Hash calculation

// Error codes
#define A71CL_SE_HAL_ERR_SUCCESS                    0x00 //!< Operation successfull
#define A71CL_SE_HAL_ERR_MEMORY_FAILED              0xC0 //!< Error - Memory Failed
#define A71CL_SE_HAL_ERR_LENGTH                 0xC1 //!< Error - Length
#define A71CL_SE_HAL_ERR_ACCESS_VIOLATION           0xC2 //!< Error - Access Violation, 6982
#define A71CL_SE_HAL_ERR_COND_NOT_SATISFIED     0xC3 //!< Error - Condition not satisfied, 6985
#define A71CL_SE_HAL_ERR_WRONG_INS                  0xC4 //!< Error - Wrong instruction
#define A71CL_SE_HAL_ERR_NOT_SUPPORTED              0xC5 //!< Error - Not Supported
#define A71CL_SE_HAL_ERR_OUT_OF_MEMORY              0xC6 //!< Error - Out of memory
#define A71CL_SE_HAL_ERR_INCORRECT_PARAMS           0xC7 //!< Error - incorrect parameters
#define A71CL_SE_HAL_ERR_FILE_NOT_FOUND         0xC8 //!< Error - 6A82
#define A71CL_SE_HAL_ERR_RECORD_NOT_FOUND           0xC9 //!< Error -   6A83
#define A71CL_SE_HAL_ERR_CHECKSUM                   0xCA //!< Error - Checksum
#define A71CL_SE_HAL_ERR_AUTH_FAILED                0xCB //!< Error - Authentication failed ,63Cx ???
#define A71CL_SE_HAL_ERR_KEY_LOCKED             0xCC //!< Error - key locked , 6983
#define A71CL_SE_HAL_ERR_SM_WRONG_MAC               0xCD //!< Error - Secure Messaging Wrong MAC , 6988
#define A71CL_SE_HAL_ERR_DATA_PARAMS_INCORRECT      0xCE //!< Error - Data Params are incorrect , 6A80
#define A71CL_SE_HAL_ERR_DEVICE_LOCKED              0xCF //!< Error - Function not supported, device locked ,6A81
#define A71CL_SE_HAL_ERR_KEY_NOT_FOUND              0xD0 //!< Error - Key not found ,6A88
#define A71CL_SE_HAL_ERR_ADDRESS_BOUNDARY           0xD1 //!< Error - Starting address beyond the board ,6B00
#define A71CL_SE_HAL_ERR_DATA_INVALID               0xD2 //!< Error - Data invalid(get response)    ,6F00
#define A71CL_SE_HAL_ERR_OPERATION_FAILED           0xD3 //!< Error - Operation failed

#ifdef __cplusplus
}
#endif
#endif //_SE_HAL_LIB_
