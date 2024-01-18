/*
 * Copyright 2020, NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_EDGELOCK_H_
#define _FSL_EDGELOCK_H_

#include "bootloader_common.h"
#include "fsl_common.h"

/*!
 * @addtogroup edgelock
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief edgelock driver version 2.0.1. */
#define FSL_EDGELOCK_DRIVER_VERSION (MAKE_VERSION(2, 0, 1))
/*@}*/

/*! @brief Edgelock status */
enum _edgelock_status_
{
    kStatus_Edgelock_InvaidResponse =
        MAKE_STATUS(kStatusGroup_Edgelock, 0), /*!< Recieved incorret Edgelock response packet. */
    kStatus_Edgelock_ResponseError =
        MAKE_STATUS(kStatusGroup_Edgelock, 1), /*!< There is an error in the Edgelock response packet. */
    kStatus_Edgelock_Abort = MAKE_STATUS(kStatusGroup_Edgelock, 2),             /*!< Edgelock in the Abort mode. */
    kStatus_Edgelock_OperationFailed = MAKE_STATUS(kStatusGroup_Edgelock, 3),   /*!< Edgelock operation is failed. */
    kStatus_Edgelock_OtpProgramFailure = MAKE_STATUS(kStatusGroup_Edgelock, 4), /*!< Fuse program is failed. */
    kStatus_Edgelock_OtpLocked = MAKE_STATUS(kStatusGroup_Edgelock, 5),         /*!< Fuse is locked. */
    kStatus_Edgelock_OtpInvalidIdx = MAKE_STATUS(kStatusGroup_Edgelock, 6),     /*!< Invalid fuse index. */
    kStatus_Edgelock_InvalidLifeCycle = MAKE_STATUS(kStatusGroup_Edgelock, 7),  /*!< Invalid lifecycle. */
};

/*! @brief edgelock command definition. */
#define EDGELOCK_CMD_PING (0x01U)
#define EDGELOCK_CMD_WRITE_SECURE_FUSE (0x91U)
#define EDGELOCK_CMD_FORWARD_LIFECYCLE_UPDATE (0x95U)
#define EDGELOCK_CMD_READ_COMMON_FUSE (0x97U)
#define EDGELOCK_CMD_GET_FW_VERSION (0x9DU)
#define EDGELOCK_CMD_RETURN_LIFECYCLE_UPDATE (0xA0U)
#define EDGELOCK_CMD_GET_FW_STATUS (0xC5U)
#define EDGELOCK_CMD_WRITE_FUSE (0xD6U)
#define EDGELOCK_CMD_START_RNG (0xA3U)
#define EDGELOCK_CMD_GET_RNG_STATE (0xA4U)
#define EDGELOCK_CMD_LOAD_KEY_BLOB (0xA7U)
#define EDGELOCK_CMD_GENERATE_KEY_BLOB (0xAFU)

/*! @brief Version of the EDGELOCK message&response packet. */
#define EDGELOCK_MESSAGE_VERSION (0x06U)
/*! @brief Tag of the EDGELOCK message packet. */
#define EDGELOCK_MESSAGE_HEADER_TAG (0x17U)
/*! @brief Tag of the EDGELOCK signed message block. */
#define EDGELOCK_MESSAGE_BLOCK_TAG (0xE1U)
/*! @brief Tag of the EDGELOCK response packet. */
#define EDGELOCK_RESPONSE_HEADER_TAG (0xE1U)

/*! @brief Generate command packet header. */
#define EDGELOCK_CMD_HEADER(cmd, size) \
    ((EDGELOCK_MESSAGE_HEADER_TAG << 24U) | ((cmd) << 16U) | ((size) << 8U) | EDGELOCK_MESSAGE_VERSION)

/*! @brief Read common fuse command maxium response words. */
#define READ_COMMON_FUSE_RESPONSE_MAX_WORDS (4)

/*! @brief Write fuse command maxium payload words. */
#define WRITE_FUSE_PAYLOAD_MAX_WORDS (8)

/*! @brief The TRNG state: TRNG is in program mode. */
#define TRNG_STATE_UNINITIALIZED (0x1U)
/*! @brief The TRNG state: TRNG is still generating entropy. */
#define TRNG_STATE_INITIALIZING (0x2U)
/*! @brief The TRNG state: TRNG entropy is valid and ready to be read. */
#define TRNG_STATE_COMPLETED (0x3U)
/*! @brief The TRNG state: TRNG encounter an error while generating entropy.. */
#define TRNG_STATE_ERROR (0x4U)
/*! @brief The CSAL state: Crypto Lib random context initialization is not done yet  .*/
#define CSAL_STATE_UNINITIALIZED (0x0U)
/*! @brief The CSAL state: Crypto Lib random context initialization is on-going.*/
#define CSAL_STATE_INITIALIZING (0x1U)
/*! @brief The CSAL state: Crypto Lib random context initialization succeed.*/
#define CSAL_STATE_COMPLETED (0x2U)
/*! @brief The CSAL state: Crypto Lib random context initialization failed.*/
#define CSAL_STATE_ERROR (0x3U)

/*! @brief The key blob block tag: input data is a private key. */
#define KEY_BLOB_TAG_PRIVATEKEY (0x81U)
/*! @brief The key blob block algorithm: AES CBC is used as KeyBlob encryption algorithm. */
#define KEY_BLOB_ALGORITHM_AESCBC (0x03U)
/*! @brief The key blob block algorithm: AES CRT is used as KeyBlob encryption algorithm. */
#define KEY_BLOB_ALGORITHM_AESCTR (0x04U)
/*! @brief The key blob block algorithm: SM4 CBC is used as KeyBlob encryption algorithm. */
#define KEY_BLOB_ALGORITHM_SM4CBC (0x2BU)
/*! @brief The key blob block algorithm: AES XTS is used as KeyBlob encryption algorithm. */
#define KEY_BLOB_ALGORITHM_AESXTS (0x37U)
/*! @brief The key blob block flags: DEK keyblob. */
#define KEY_BLOB_FLAGS_DEK (0x1U)
/*! @brief The key blob block flags: OTFAD keyblob. */
#define KEY_BLOB_FLAGS_OTFAD (0x2U)
/*! @brief The key blob block flags: IEE keyblob. */
#define KEY_BLOB_FLAGS_IEE (0x3U)
/*! @brief Definition of the key blob address alignment size. */
#define KEY_BLOB_ALIGN_SIZE (0x8U) /* 64 bits aligned. */

/*! @brief Definitions of the response status from edgelock. */
enum _edgelock_response_status_
{
    kResponseStatus_Success = 0xD6U,
    kResponseStatus_Failure = 0x29U,
};

/*! @brief Definitions of the response indication from edgelock. */
enum _edgelock_response_indication_
{
    kResponseInd_OtpProgramFailure = 0xAAU,
    kResponseInd_OtpLocked = 0xABU,
    kResponseInd_OtpInvalidIdx = 0xADU,
    kResponseInd_InvalidLifeCycle = 0xF2U,
    kResponseInd_Abort = 0xFFU,
};

/*! @brief The format of the EDGELOCK message packet header. */
typedef union _message_packet_header_
{
    struct
    {
        uint8_t version; /* Version. Must be 0x06. */
        uint8_t size;    /* Size of the message in words, including header and CRC. Must be 1-255*/
        uint8_t command; /* Command, refer enum _edgelock_command_ */
        uint8_t tag;     /* Tag. Must be 0x17. */
    } B;
    uint32_t U;
} message_packet_header_t;

/*! @brief The format of the EDGELOCK response packet header. */
typedef struct _response_packet_header_
{
    uint8_t version; /* Version. Must be 0x06. */
    uint8_t size;    /* Size of the response in words, including header and CRC. Must be 1-255*/
    uint8_t command; /* Command, must be same as the value in the message packet. */
    uint8_t tag;     /* Tag. Must be 0xE1. */
} response_packet_header_t;

/*! @brief The format of the EDGELOCK response packet code. */
typedef union _response_packet_code_
{
    struct
    {
        uint8_t status;     /* Response status. refer to _edgelock_response_status_. */
        uint8_t indication; /* Response indication. refer to _edgelock_response_indication_. */
        uint16_t abortCode; /* Abort code. 0 for normal operation, the other means edgelock is in abort mode. S400
                               ROM/FW team knows the details. */
    } B;
    uint32_t U;
} response_packet_code_t;

/*! @brief The generic message packet format of the edgelock . */
typedef struct _message_packet_
{
    message_packet_header_t header; /* Packet header. */
} message_packet_t;

/*! @brief The format of the EDGELOCK signed message packet. */
typedef struct _signed_message_packet_
{
    message_packet_header_t header; /* Packet header. */
    uint32_t blockAddrExt;          /* The 4 MSB of the 64bit address. */
    uint32_t blockAddr;             /* The address where the signed data block can be found*/
} signed_message_packet_t;

/*! @brief The generic response packet format of the edgelock. */
typedef struct _response_packet_
{
    response_packet_header_t header; /* Packet header. */
    response_packet_code_t response; /* Response code. */
} response_packet_t;

/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C"
{
#endif /*__cplusplus */

    /*!
     * @brief Initialize MU interface for Edgelock access.
     *
     * @param base Edgelock Message Unit base.
     * @return kStatus_Success for success, otherwise return error code.
     */
    status_t EDGELOCK_Init(SxMU_Type *base);

    /*!
     * @brief Deinitialize MU interface for Edgelock access.
     *
     * @param base Edgelock Message Unit base.
     * @return kStatus_Success for success, otherwise return error code.
     */
    status_t EDGELOCK_Deinit(SxMU_Type *base);

    /*!
     * @brief Send message to Edgelock.
     *
     * @param base Edgelock Message Unit base.
     * @param pMsg Message pointer.
     * @param msgWords Message word count.
     * @return kStatus_Success for success, otherwise return error code.
     */
    status_t EDGELOCK_SendMessage(SxMU_Type *base, const uint32_t *pMsg, uint32_t msgWords);

    /*!
     * @brief Receive message from Edgelock.
     *
     * @param base Edgelock Message Unit base.
     * @param pMsg Pointer to save response message.
     * @param msgWords Message buffer word count.
     * @return kStatus_Success for success, otherwise return error code.
     */
    status_t EDGELOCK_ReceiveMessage(SxMU_Type *base, uint32_t *pMsg, uint32_t msgWords);

    /*!
     * @brief Check the response message from Edgelock.
     *
     * @param base Edgelock Message Unit base.
     * @param pRespMsg Edgelock response message pointer.
     * @param respMsgWords Edgelock response message buffer word count.
     * @return kStatus_Success for success, otherwise return error code.
     */
    status_t EDGELOCK_CheckResponse(SxMU_Type *base, const uint32_t *pRespMsg, uint32_t respMsgWords);

    /*!
     * @brief Send command to Edgelock and receive response.
     *
     * @param base Edgelock Message Unit base.
     * @param pCmdMsg Command message pointer.
     * @param cmdMsgWords Command message word count.
     * @param pRespMsg Pointer to save response message..
     * @param respMsgWords Message buffer word count.
     * @return kStatus_Success for success, otherwise return error code.
     */
    status_t EDGELOCK_Command(
        SxMU_Type *base, const uint32_t *pCmdMsg, uint32_t cmdMsgWords, uint32_t *pRespMsg, uint32_t respMsgWords);

    /*!
     * @brief Ping Edgelock to see if it is alive.
     *
     * @param base Edgelock Message Unit base.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_Ping(SxMU_Type *base);

    /*!
     * @brief Write secure fuse.
     *
     * @param base Edgelock Message Unit base.
     * @param blockAddr signed message block address.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_WriteSecureFuse(SxMU_Type *base, uint32_t blockAddr);

    /*!
     * @brief Switch to common lifecycle.
     *
     * @param base Edgelock Message Unit base.
     * @param lifeCycle LifeCycle to switch to.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_ForwardLifecycleUpdate(SxMU_Type *base, uint16_t lifeCycle);

    /*!
     * @brief Read common fuse words.
     *
     * @param base Edgelock Message Unit base.
     * @param fuseId Fuse word ID.
     * @param pFuseValue Fuse value pointer.
     * @param pFuseWords[in] Fuse value buffer word count.
     * @param pFuseWords[out] Read fuse value word count.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_ReadCommonFuse(SxMU_Type *base, uint8_t fuseId, uint32_t *pFuseValue, uint32_t *pFuseWords);

    /*!
     * @brief Get Edgelock firmware version.
     *
     * @param pVersion Pointer to save firmware version.
     * @param pCommitSha1 Pointer to save first 4 bytes of the git commit ID.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_GetFirmwareVersion(SxMU_Type *base, uint32_t *pVersion, uint32_t *pCommitSha1);

    /*!
     * @brief Switch to returned lifecycle.
     *
     * @param base Edgelock Message Unit base.
     * @param blockAddr signed message block address.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_ReturnLifecycleUpdate(SxMU_Type *base, uint32_t blockAddr);

    /*!
     * @brief Get Edgelock firmware status.
     *
     * @param pStatus Pointer to save firmware status. 0: No firmware in place. 1: Firmware authenticated and
     * operational.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_GetFirmwareStatus(SxMU_Type *base, uint8_t *pStatus);

    /*!
     * @brief Read common fuse words.
     *
     * @param base Edgelock Message Unit base.
     * @param bitPosition Fuse identifier expressed as its position in bit.
     * @param bitLength Number of bits to be written.
     * @param lock Write lock requirement. 0: no write lock, 1 write lock.
     * @param fuseData Data to be written in fuse.
     * @param fuseWords Data word count.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_WriteFuse(
        SxMU_Type *base, uint16_t bitPosition, uint16_t bitLength, bool lock, uint32_t *fuseData, uint32_t fuseWords);

    /*!
     * @brief Read fuse words.
     *
     * @param base Edgelock Message Unit base.
     * @param fuseId Fuse word ID.
     * @param pFuseValue Fuse value pointer.
     * @param fuseWords Fuse value word count.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_ReadFuse(SxMU_Type *base, uint8_t fuseId, uint32_t *pFuseValue, uint32_t fuseWords);

    /*!
     * @brief Start TRNG.
     *
     * @param base Edgelock Message Unit base.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_StartRNG(SxMU_Type *base);

    /*!
     * @brief Return TRNG state.
     *
     * @param base Edgelock Message Unit base.
     * @param trngState Return the TRNG state.
     * @param csalState Return the CSAL state.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_GetTRNGState(SxMU_Type *base, uint8_t *trngState, uint8_t *csalState);

    /*!
     * @brief Load key blob.
     *
     * @param base Edgelock Message Unit base.
     * @param keyIdentifier Identifier of the input key
     * @param loadAddr Address where the input key data can be found.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_LoadKeyblob(SxMU_Type *base, uint32_t keyIdentifier, uint32_t loadAddr);

    /*!
     * @brief Generate key blob.
     *
     * @param base Edgelock Message Unit base.
     * @param keyIdentifier Identifier of the input key
     * @param loadAddr Address where the input key data can be found.
     * @param exportAddr Address where the key blob can be stored.
     * @param exportSize available key blob data in bytes.
     * @return 0 for success, otherwise return error code.
     */
    status_t EDGELOCK_GenerateKeyblob(
        SxMU_Type *base, uint32_t keyIdentifier, uint32_t loadAddr, uint32_t exportAddr, uint32_t exportSize);

#if defined(__cplusplus)
}
#endif /*__cplusplus */

/*! @} */

#endif /* _FSL_EDGELOCK_H_ */
