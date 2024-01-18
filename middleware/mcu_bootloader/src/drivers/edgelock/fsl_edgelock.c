/*
 * Copyright 2020 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_edgelock.h"

#ifdef FSL_RTOS_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Component ID definition, used by tools. */
#ifndef FSL_COMPONENT_ID
#define FSL_COMPONENT_ID "middleware.mcu-boot.drivers.edgelock"
#endif

/*! @brief Generate each command packet header. */
#define EDGELOCK_CMD_HEADER_PING EDGELOCK_CMD_HEADER(EDGELOCK_CMD_PING, sizeof(ping_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_WRITE_SECURE_FUSE \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_WRITE_SECURE_FUSE, sizeof(write_secure_fuse_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_FORWARD_LIFECYCLE_UPDATE \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_FORWARD_LIFECYCLE_UPDATE, sizeof(forward_lifecycle_update_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_READ_COMMON_FUSE \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_READ_COMMON_FUSE, sizeof(read_common_fuse_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_GET_FW_VERSION \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_GET_FW_VERSION, sizeof(get_firmware_version_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_RETURN_LIFECYCLE_UPDATE \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_RETURN_LIFECYCLE_UPDATE, sizeof(return_lifecycle_update_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_GET_FW_STATUS \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_GET_FW_STATUS, sizeof(get_firmware_status_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_WRITE_FUSE(words)    \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_WRITE_FUSE, \
                        sizeof(write_fuse_message_packet_t) / 4 - WRITE_FUSE_PAYLOAD_MAX_WORDS + (words))
#define EDGELOCK_CMD_HEADER_START_RNG \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_START_RNG, sizeof(start_rng_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_GET_RNG \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_GET_RNG_STATE, sizeof(get_trng_state_message_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_LOAD_KEY_BLOB \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_LOAD_KEY_BLOB, sizeof(load_keyblob_response_packet_t) / 4)
#define EDGELOCK_CMD_HEADER_GENERATE_KEY_BLOB \
    EDGELOCK_CMD_HEADER(EDGELOCK_CMD_GENERATE_KEY_BLOB, sizeof(generate_keyblob_message_packet_t) / 4)

/*! @brief The message packet format of the ping command. */
typedef struct _ping_message_packet_
{
    message_packet_t msg; /* message packet. */
} ping_message_packet_t;

/*! @brief The response packet format of the ping command. */
typedef struct _ping_response_packet_
{
    response_packet_t rsp; /* Response packet. */
} ping_response_packet_t;

/*! @brief The message packet format of the write secure fuse command. */
typedef struct _write_secure_fuse_message_packet_
{
    signed_message_packet_t msg; /* Message packet. */
} write_secure_fuse_message_packet_t;

/*! @brief The response packet format of the write secure fuse command. */
typedef struct _write_secure_fuse_response_packet_
{
    response_packet_t rsp; /* Response packet. */
    uint32_t index; /* In case of failure indicated in the response code, fuse row index of the failing fuse which
                       resulted in the command abortion. Set to 0 in case of success. */
} write_secure_fuse_response_packet_t;

/*! @brief The message packet format of the forward lifecycle update command. */
typedef struct _forward_lifecycle_update_message_packet_
{
    message_packet_t msg; /* Message packet. */
    uint16_t lifecycle;   /* Lifecycle to switch to. */
    uint8_t reserved[2];  /* Reserved. */
} forward_lifecycle_update_message_packet_t;

/*! @brief The response packet format of the forward lifecycle update command. */
typedef struct _forward_lifecycle_update_response_packet_
{
    response_packet_t rsp; /* Response packet. */
} forward_lifecycle_update_response_packet_t;

/*! @brief The message packet format of the read common fuse command. */
typedef struct _read_common_fuse_packet_
{
    message_packet_t msg; /* Message packet. */
    uint16_t fuseId;      /* Fuse index. */
    uint8_t reserved[2];  /* Reserved. */
} read_common_fuse_message_packet_t;

/*! @brief The response packet format of the read common fuse command. */
typedef struct _read_common_fuse_response_packet_
{
    response_packet_t rsp;                               /* Response packet. */
    uint32_t value[READ_COMMON_FUSE_RESPONSE_MAX_WORDS]; /* fuse value. */
} read_common_fuse_response_packet_t;

/*! @brief The message packet format of the get firmware version command. */
typedef struct _get_firmware_version_message_packet_
{
    message_packet_t msg; /* Message packet. */
} get_firmware_version_message_packet_t;

/*! @brief The response packet format of the get firmware version command. */
typedef struct _get_firmware_version_response_packet_
{
    response_packet_t rsp; /* Response packet. */
    uint32_t version;      /* Edgelock firmware version. */
    uint32_t commitSha1;   /* First 4 bytes of the git commit Id (little endian). */
} get_firmware_version_response_packet_t;

/*! @brief The message packet format of the forward lifecycle update command. */
typedef struct _return_lifecycle_update_message_packet_
{
    signed_message_packet_t msg; /* Message packet. */
} return_lifecycle_update_message_packet_t;

/*! @brief The response packet format of the return lifecycle update command. */
typedef struct _return_lifecycle_update_response_packet_
{
    response_packet_t rsp; /* Response packet. */
} return_lifecycle_update_response_packet_t;

/*! @brief The message packet format of the get firmware status command. */
typedef struct _get_firmware_status_message_packet_
{
    message_packet_t msg; /* Message packet. */
} get_firmware_status_message_packet_t;

/*! @brief The response packet format of the get firmware status command. */
typedef struct _get_firmware_status_response_packet_
{
    response_packet_t rsp; /* Response packet. */
    uint8_t status;        /* Firmware status. 0: no Edgelock FW, 1: Edgelock FW in operation. */
    uint8_t reserved[3];   /* Reserved. */
} get_firmware_status_response_packet_t;

/*! @brief The message packet format of the write fuse command. */
typedef struct _write_fuse_message_packet_
{
    message_packet_t msg;    /* Message packet. */
    uint16_t bitPosition;    /* Fuse identifier expressed as its position in bit in the fuse map. */
    uint16_t bitLength : 15; /* Number of bits to be written. */
    uint16_t lock : 1;       /* Write lock requirement. */
    uint32_t payload[WRITE_FUSE_PAYLOAD_MAX_WORDS]; /* Data to bewritten  in fuse. */
} write_fuse_message_packet_t;

/*! @brief The response packet format of the write fuse command. */
typedef struct _write_fuse_response_packet_
{
    response_packet_t rsp; /* Response packet. */
    uint16_t processedIdx; /* Last processed fuse word index. */
    uint8_t reserved[2];   /* Reserved. */
} write_fuse_response_packet_t;

/*! @brief The response packet format of the generate keyblob command. */
typedef struct _generate_keyblob_response_packet_
{
    response_packet_t rsp; /* Response packet. */
} generate_keyblob_response_packet_t;

/*! @brief The message packet format of the start RNG command. */
typedef struct _start_rng_message_packet_
{
    message_packet_t msg; /* Message packet. */
} start_rng_message_packet_t;

/*! @brief The response packet format of the start RNG command. */
typedef struct _start_rng_response_packet_
{
    response_packet_t rsp; /* Response packet. */
} start_rng_response_packet_t;

/*! @brief The message packet format of the get TRNG state command. */
typedef struct _get_trng_state_message_packet_
{
    message_packet_t msg; /* Message packet. */
} get_trng_state_message_packet_t;

/*! @brief The response packet format of the get TRNG state command. */
typedef struct _get_trng_state_response_packet_
{
    response_packet_t rsp; /* Response packet. */
    uint8_t trngState;
    uint8_t csalState;
    uint8_t reserved[2];
} get_trng_state_response_packet_t;

/*! @brief The message packet format of the load keyblob command. */
typedef struct _load_keyblob_message_packet_
{
    message_packet_t msg;   /* Message packet. */
    uint32_t keyIdentifier; /* Key Identifier. */
    uint32_t reserved;      /* In case of 64 bits address, this field contains the 4 MSB of the 64 bits load address. */
    uint32_t loadAddress;   /* Address where the input data can be found. Must be 64 bits aligned. */
} load_keyblob_message_packet_t;

/*! @brief The response packet format of the load keyblob command. */
typedef struct _load_keyblob_response_packet_
{
    response_packet_t rsp; /* Response packet. */
} load_keyblob_response_packet_t;

/*! @brief The message packet format of the generate keyblob command. */
typedef struct _generate_keyblob_message_packet_
{
    message_packet_t msg;   /* Message packet. */
    uint32_t keyIdentifier; /* Key Identifier. */
    uint32_t reserved1;     /* In case of 64 bits address, this field contains the 4 MSB of the 64 bits load address. */
    uint32_t loadAddress;   /* Address where the input data can be found. Must be 64 bits aligned. */
    uint32_t reserved2; /* In case of 64 bits address, this field contains the 4 MSB of the 64 bits export address. */
    uint32_t exportAddress; /* Address where the blob can be stored. Must be 64 bits aligned. */
    uint16_t maxExportSize; /* The maximum memory available in bytes at the provided address. */
    uint16_t reserved3;     /* Unused. */
    uint32_t xorCheck;      /* XOR of the words from msg to resrved3. */
} generate_keyblob_message_packet_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Initialize MU interface for Edgelock access.
 *
 * @param base Edgelock Message Unit base.
 * @return kStatus_Success for success, otherwise return error code.
 */
status_t EDGELOCK_Init(SxMU_Type *base)
{
    base->TCR = 0U; /* Disable all transmit interrupts. */
    base->RCR = 0U; /* Disable all receive interrupts. */

    return kStatus_Success;
}

/*!
 * @brief Deinitialize MU interface for Edgelock access.
 *
 * @param base Edgelock Message Unit base.
 * @return kStatus_Success for success, otherwise return error code.
 */
status_t EDGELOCK_Deinit(SxMU_Type *base)
{
    /* Nothing to do. */
    return kStatus_Success;
}

/*!
 * @brief Send message to Edgelock.
 *
 * @param base Edgelock Message Unit base.
 * @param pMsg Message pointer.
 * @param msgWords Message word count.
 * @return kStatus_Success for success, otherwise return error code.
 */
status_t EDGELOCK_SendMessage(SxMU_Type *base, const uint32_t *pMsg, uint32_t msgWords)
{
    uint32_t i;

    if ((!pMsg) || (!msgWords))
    {
        return kStatus_InvalidArgument;
    }

    for (i = 0U; i < msgWords; i++)
    {
        uint32_t slot = i % ARRAY_SIZE(base->TR);
        while ((base->TSR & (MU_TSR_TE0_MASK << slot)) == 0U)
        {
            /* Wait transmit register empty */
        }
        base->TR[slot] = pMsg[i];
    }

    return kStatus_Success;
}

/*!
 * @brief Receive message from Edgelock.
 *
 * @param base Edgelock Message Unit base.
 * @param pMsg Pointer to save response message.
 * @param msgWords Message buffer word count.
 * @return kStatus_Success for success, otherwise return error code.
 */
status_t EDGELOCK_ReceiveMessage(SxMU_Type *base, uint32_t *pMsg, uint32_t msgWords)
{
    uint32_t msgSize;
    uint32_t i;

    if ((!pMsg) || (msgWords < (sizeof(response_packet_t) / 4)))
    {
        return kStatus_InvalidArgument;
    }

    while ((base->RSR & MU_RSR_RF0_MASK) == 0U)
    {
        /* Wait receive register full */
    }

    /* Read the message header. */
    pMsg[0] = base->RR[0];

    /* Get the message size. */
    msgSize = ((response_packet_t *)pMsg)->header.size;
    assert(msgSize >= 1U);

    for (i = 1U; i < msgSize; i++)
    {
        uint32_t slot = i % ARRAY_SIZE(base->RR);

        while ((base->RSR & (MU_RSR_RF0_MASK << slot)) == 0U)
        {
            /* Wait receive register full */
        }

        if (i < msgWords)
        {
            pMsg[i] = base->RR[slot];
        }
        else
        {
            /* Read the left message words out even the buffer is not enough. */
            volatile uint32_t word = base->RR[i];
        }
    }

    if (msgWords < msgSize)
    {
        return kStatus_OutOfRange;
    }
    else
    {
        /* Save received param count. */
        return kStatus_Success;
    }
}

/*!
 * @brief Check the response message from Edgelock.
 *
 * @param base Edgelock Message Unit base.
 * @param pRespMsg Edgelock response message pointer.
 * @param respMsgWords Edgelock response message buffer word count.
 * @return kStatus_Success for success, otherwise return error code.
 */
status_t EDGELOCK_CheckResponse(SxMU_Type *base, const uint32_t *pRespMsg, uint32_t respMsgWords)
{
    response_packet_t *rsp = (response_packet_t *)pRespMsg;

    if (!rsp)
    {
        return kStatus_InvalidArgument;
    }

    /* Failure status. */
    if (rsp->response.B.status != kResponseStatus_Success)
    {
        if (rsp->response.B.indication == kResponseInd_Abort)
        {
            return kStatus_Edgelock_Abort;
        }
        else if (rsp->response.B.indication == kResponseInd_OtpProgramFailure)
        {
            return kStatus_Edgelock_OtpProgramFailure;
        }
        else if (rsp->response.B.indication == kResponseInd_OtpLocked)
        {
            return kStatus_Edgelock_OtpLocked;
        }
        else if (rsp->response.B.indication == kResponseInd_OtpInvalidIdx)
        {
            return kStatus_Edgelock_OtpInvalidIdx;
        }
        else if (rsp->response.B.indication == kResponseInd_InvalidLifeCycle)
        {
            return kStatus_Edgelock_InvalidLifeCycle;
        }
        else
        {
            return kStatus_Edgelock_ResponseError;
        }
    }

    /* Invalid response tag. */
    if (rsp->header.tag != EDGELOCK_RESPONSE_HEADER_TAG)
    {
        return kStatus_Edgelock_InvaidResponse;
    }
    /* Invalid version. */
    if (rsp->header.version != EDGELOCK_MESSAGE_VERSION)
    {
        return kStatus_Edgelock_InvaidResponse;
    }

    /* Invalid response packet size. */
    if (rsp->header.size != respMsgWords)
    {
        return kStatus_Edgelock_InvaidResponse;
    }

    return kStatus_Success;
}

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
    SxMU_Type *base, const uint32_t *pCmdMsg, uint32_t cmdMsgWords, uint32_t *pRespMsg, uint32_t respMsgWords)
{
    status_t status;

#ifdef FSL_RTOS_FREE_RTOS
    /* Suspends the scheduler to make sure there's only one rpc call ongoing at a time. */
    vTaskSuspendAll();
#endif

    if ((!pCmdMsg) || (!cmdMsgWords) || (pRespMsg && (respMsgWords < (sizeof(response_packet_t) / 4))))
    {
        return kStatus_InvalidArgument;
    }

    status = EDGELOCK_SendMessage(base, pCmdMsg, cmdMsgWords);
    if (status != kStatus_Success)
    {
        return status;
    }

    if (pRespMsg)
    {
        status = EDGELOCK_ReceiveMessage(base, pRespMsg, respMsgWords);
        if (status != kStatus_Success)
        {
            return status;
        }

        status = EDGELOCK_CheckResponse(base, pRespMsg, respMsgWords);
    }

#ifdef FSL_RTOS_FREE_RTOS
    (void)xTaskResumeAll();
#endif

    return status;
}

/*!
 * @brief Ping Edgelock to see if it is alive.
 *
 * @param base Edgelock Message Unit base.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_Ping(SxMU_Type *base)
{
    ping_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_PING,
    };
    ping_response_packet_t respMsg;

    return EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(ping_message_packet_t) / 4, (uint32_t *)&respMsg,
                            sizeof(ping_response_packet_t) / 4);
}

/*!
 * @brief Write secure fuse.
 *
 * @param base Edgelock Message Unit base.
 * @param blockAddr signed message block address.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_WriteSecureFuse(SxMU_Type *base, uint32_t blockAddr)
{
    status_t status;

    write_secure_fuse_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_WRITE_SECURE_FUSE,
        .msg.blockAddr = blockAddr,
        .msg.blockAddrExt = 0,
    };
    write_secure_fuse_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);
    if (status == kStatus_Success)
    {
        if (respMsg.index != 0)
        {
            return kStatus_Edgelock_OperationFailed;
        }
    }

    return status;
}

/*!
 * @brief Switch to common lifecycle.
 *
 * @param base Edgelock Message Unit base.
 * @param lifeCycle LifeCycle to switch to.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_ForwardLifecycleUpdate(SxMU_Type *base, uint16_t lifeCycle)
{
    forward_lifecycle_update_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_FORWARD_LIFECYCLE_UPDATE,
        .lifecycle = lifeCycle,
    };
    forward_lifecycle_update_response_packet_t respMsg = { 0 };

    return EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);
}

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
status_t EDGELOCK_ReadCommonFuse(SxMU_Type *base, uint8_t fuseId, uint32_t *pFuseValue, uint32_t *pFuseWords)
{
    status_t status;

    read_common_fuse_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_READ_COMMON_FUSE,
        .fuseId = fuseId,
    };
    read_common_fuse_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg,
                              sizeof(respMsg.rsp) / 4 + *pFuseWords);
    if (status == kStatus_Success)
    {
        uint32_t fuseSize = respMsg.rsp.header.size - (sizeof(respMsg.rsp) / 4);
        assert(fuseSize < READ_COMMON_FUSE_RESPONSE_MAX_WORDS);

        if (*pFuseWords < fuseSize)
        {
            return kStatus_OutOfRange;
        }

        for (int i = 0; i < fuseSize; i++)
        {
            pFuseValue[i] = respMsg.value[0];
        }
        *pFuseWords = fuseSize;
    }

    return status;
}

/*!
 * @brief Get Edgelock firmware version.
 *
 * @param pVersion Pointer to save firmware version.
 * @param pCommitSha1 Pointer to save first 4 bytes of the git commit ID.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_GetFirmwareVersion(SxMU_Type *base, uint32_t *pVersion, uint32_t *pCommitSha1)
{
    status_t status;

    get_firmware_version_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_GET_FW_VERSION,
    };
    get_firmware_version_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);
    if (status == kStatus_Success)
    {
        *pVersion = respMsg.version;
        *pCommitSha1 = respMsg.commitSha1;
    }

    return status;
}

/*!
 * @brief Switch to returned lifecycle.
 *
 * @param base Edgelock Message Unit base.
 * @param blockAddr signed message block address.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_ReturnLifecycleUpdate(SxMU_Type *base, uint32_t blockAddr)
{
    return_lifecycle_update_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_RETURN_LIFECYCLE_UPDATE,
        .msg.blockAddr = blockAddr,
        .msg.blockAddrExt = 0,
    };
    forward_lifecycle_update_response_packet_t respMsg = { 0 };

    return EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);
}

/*!
 * @brief Get Edgelock firmware status.
 *
 * @param pStatus Pointer to save firmware status. 0: No firmware in place. 1: Firmware authenticated and operational.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_GetFirmwareStatus(SxMU_Type *base, uint8_t *pStatus)
{
    status_t status;

    get_firmware_status_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_GET_FW_STATUS,
    };
    get_firmware_status_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);
    if (status == kStatus_Success)
    {
        *pStatus = respMsg.status;
    }

    return status;
}

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
    SxMU_Type *base, uint16_t bitPosition, uint16_t bitLength, bool lock, uint32_t *fuseData, uint32_t fuseWords)
{
    status_t status;

    if (bitLength > 32)
    {
        if ((bitPosition % 32) || (fuseWords <= 1))
        {
            return kStatus_InvalidArgument;
        }
    }
    else
    {
        if (fuseWords > 1)
        {
            return kStatus_InvalidArgument;
        }
    }

    write_fuse_message_packet_t cmdMsg = {
        .msg.header.U = EDGELOCK_CMD_HEADER_WRITE_FUSE(fuseWords),
        .bitPosition = bitPosition,
        .bitLength = bitLength,
        .lock = lock,
    };

    memcpy(&cmdMsg.payload[0], fuseData, fuseWords * 4);

    write_fuse_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4 - WRITE_FUSE_PAYLOAD_MAX_WORDS + fuseWords,
                              (uint32_t *)&respMsg, sizeof(respMsg) / 4);
    if (status == kStatus_Success)
    {
        /* Check if it is extended command. */
        if (((bitPosition % 32 /* word bits */) + bitLength) > 32 /* word bits */)
        {
            uint32_t lastWordIndex = (bitPosition + bitLength + 32 /* word bits */ - 1) / 32 /* word bits */;
            if (respMsg.processedIdx != lastWordIndex)
            {
                return kStatus_Edgelock_OperationFailed;
            }
        }
    }

    return status;
}

/*!
 * @brief Start TRNG.
 *
 * @param base Edgelock Message Unit base.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_StartRNG(SxMU_Type *base)
{
    status_t status = kStatus_Fail;

    /* Prepare the message packet. */
    start_rng_message_packet_t cmdMsg = { 0 };
    cmdMsg.msg.header.U = EDGELOCK_CMD_HEADER_START_RNG;

    /* Receive the response packet. */
    start_rng_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);

    return status;
}

/*!
 * @brief Return TRNG state.
 *
 * @param base Edgelock Message Unit base.
 * @param trngState Return the TRNG state.
 * @param csalState Return the CSAL state.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_GetTRNGState(SxMU_Type *base, uint8_t *trngState, uint8_t *csalState)
{
    status_t status = kStatus_Fail;

    /* Prepare the message packet. */
    get_trng_state_message_packet_t cmdMsg = { 0 };
    cmdMsg.msg.header.U = EDGELOCK_CMD_HEADER_GET_RNG;

    /* Receive the response packet. */
    get_trng_state_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);
    if (status == kStatus_Success)
    {
        *trngState = respMsg.trngState;
        *csalState = respMsg.csalState;
    }

    return status;
}

/*!
 * @brief Load key blob.
 *
 * @param base Edgelock Message Unit base.
 * @param keyIdentifier Identifier of the input key
 * @param loadAddr Address where the input key data can be found.
 * @return 0 for success, otherwise return error code.
 */
status_t EDGELOCK_LoadKeyblob(SxMU_Type *base, uint32_t keyIdentifier, uint32_t loadAddr)
{
    status_t status = kStatus_Fail;

    if (loadAddr % KEY_BLOB_ALIGN_SIZE)
    {
        return kStatus_InvalidArgument;
    }

    /* Prepare the message packet. */
    load_keyblob_message_packet_t cmdMsg = { 0 };
    cmdMsg.msg.header.U = EDGELOCK_CMD_HEADER_LOAD_KEY_BLOB;
    cmdMsg.keyIdentifier = keyIdentifier;
    cmdMsg.loadAddress = loadAddr;

    /* Receive the response packet. */
    load_keyblob_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);

    return status;
}

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
    SxMU_Type *base, uint32_t keyIdentifier, uint32_t loadAddr, uint32_t exportAddr, uint32_t exportSize)
{
    status_t status;

    if ((loadAddr % KEY_BLOB_ALIGN_SIZE) || (exportAddr % KEY_BLOB_ALIGN_SIZE) || (exportSize == 0U))
    {
        return kStatus_InvalidArgument;
    }

    /* Prepare the message packet. */
    generate_keyblob_message_packet_t cmdMsg = { 0 };
    cmdMsg.msg.header.U = EDGELOCK_CMD_HEADER_GENERATE_KEY_BLOB;
    cmdMsg.keyIdentifier = keyIdentifier;
    cmdMsg.loadAddress = loadAddr;
    cmdMsg.exportAddress = exportAddr;
    cmdMsg.maxExportSize = exportSize;
    /* Calculate the XOR check value. */
    cmdMsg.xorCheck = 0U;
    for (uint32_t *pMsgWord = (uint32_t *)&cmdMsg; pMsgWord < (uint32_t *)&cmdMsg.xorCheck; pMsgWord++)
    {
        cmdMsg.xorCheck ^= *pMsgWord;
    }

    /* Receive the response packet. */
    generate_keyblob_response_packet_t respMsg = { 0 };

    status = EDGELOCK_Command(base, (uint32_t *)&cmdMsg, sizeof(cmdMsg) / 4, (uint32_t *)&respMsg, sizeof(respMsg) / 4);

    return status;
}
