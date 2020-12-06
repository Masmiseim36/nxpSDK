/*
 * Copyright 2019 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/* FreeRTOS kernel includes */
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include <string.h>
#include "board.h"
#include "mbedtls/base64.h"
#include "fsl_dcp.h"

#include "cJSON.h"

#include "comms_message_handler_fwupdate_server.h"
#include "comms_message_handler_fwupdate_common.h"
#include "flash_ica_driver.h"
#include "sln_flash_mgmt.h"
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTA_TCP
#include "sln_ota.h"
#endif
#if COMMS_MESSAGE_HANDLER_FWUPDATE_METHOD_OTW_UART
#include "sln_otw.h"
#endif
#include "sln_RT10xx_RGB_LED_driver.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _sln_comms_fwupdate_block
{
    uint32_t offset;
    uint32_t blockSize;
    uint32_t encodedSize;
    int32_t crc;
    uint8_t *data;
    char *encodedData;

} sln_comms_fwupdate_block;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static sln_comms_message_status processFwUpdateStopReq(void *clientInstance, cJSON *json);
static sln_comms_message_status processFwUpdateBlockReq(void *clientInstance, cJSON *json);
static sln_comms_message_status processFwUpdateStartSelfCheckReq(void *clientInstance, cJSON *json);
static sln_comms_message_status processFwUpdateActivateImgReq(void *clientInstance, cJSON *json);

/*******************************************************************************
 * Variables
 ******************************************************************************/

static dcp_handle_t dcpCrcHhandle = {.channel = kDCP_Channel0, .keySlot = kDCP_KeySlot0, .swapConfig = kDCP_NoSwap};

/*******************************************************************************
 * Public Functions
 ******************************************************************************/

sln_comms_message_status processFwUpdateServerReq(void *clientInstance, cJSON *json)
{
    cJSON *messageType;
    cJSON *messageContents;
    sln_comms_message_status status = kComms_Success;

    messageContents = cJSON_GetObjectItemCaseSensitive(json, "fwupdate_server_message");

    messageType = cJSON_GetObjectItemCaseSensitive(messageContents, "messageType");

    if (!cJSON_IsNumber(messageType))
    {
        status = kComms_InvalidParameter;
    }

    if (status == kComms_Success)
    {
        /* Parse the FwUpdate server message */
        switch (messageType->valueint)
        {
            case kCommsFwUpdateBlock:
                status = processFwUpdateBlockReq(clientInstance, messageContents);
                break;

            case kCommsFwUpdateStop:
                status = processFwUpdateStopReq(clientInstance, messageContents);
                break;

            case kCommsFwUpdateStartSelfCheck:
                status = processFwUpdateStartSelfCheckReq(clientInstance, messageContents);
                break;

            case kCommsFwUpdateActivateImage:
                status = processFwUpdateActivateImgReq(clientInstance, messageContents);
                break;

            default:
                status = kComms_UnknownMessage;
                break;
        }
    }

    return status;
}

sln_comms_message_status handleServerFwUpdateStartReq(uint32_t img_type)
{
    sln_comms_message_status status = kComms_Success;
    int32_t fica_ret                = SLN_FLASH_NO_ERROR;

    /* Create an entry for the new image */
    fica_ret = FICA_app_program_ext_init(img_type);

    if (SLN_FLASH_NO_ERROR != fica_ret)
    {
        configPRINTF(("FICA_app_program_ext_init failed, error %d.\r\n", fica_ret));
        status = kComms_FailedProcessing;
    }

    return status;
}
/*******************************************************************************
 * Private Functions
 ******************************************************************************/

/**
 * @brief Process the Start Self Check request
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateStartSelfCheckReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonError;

    sln_comms_message_status status = kComms_Success;
    int32_t imgType                 = FICA_IMG_TYPE_NONE;
    int32_t ficaStatus              = SLN_FLASH_NO_ERROR;
    bool savedCert                  = false;

    /* Accept the image since it was a good transfer
     * and networking and services are all working.
     */
    configPRINTF(("Received eFwUpdate_JobEvent_StartTest callback from FwUpdate Agent.\r\n"));

    /* Get new image type */
    ficaStatus = FICA_get_new_app_img_type(&imgType);

    configPRINTF(("New image type %d.\r\n", imgType));

    if (SLN_FLASH_NO_ERROR == ficaStatus)
    {
        /* Check if there is a mismatch between intended update bank
         * and the actual entry point address
         */
        ficaStatus = FICA_Verify_OTA_Image_Entry_Point(imgType);

        if (SLN_FLASH_NO_ERROR != ficaStatus)
        {
            configPRINTF(("Image entry point check failed, firmware not built for the specified bank.\r\n"));
        }
    }

    if (SLN_FLASH_NO_ERROR != ficaStatus)
    {
        status = kComms_FailedProcessing;
    }

#if DISABLE_IMAGE_VERIFICATION
#warning "IMAGE VERIFICATION DISABLED! THIS IS NOT RECOMMENDED."
// NOTE: This is not recommended for a production application
#else
    if (SLN_FLASH_NO_ERROR == ficaStatus)
    {
        if (SLN_FLASH_NO_ERROR == ficaStatus)
        {
            /* Verify Certificate */
            ficaStatus = FICA_Verify_Certificate(imgType);
        }

        if (SLN_FLASH_NO_ERROR == ficaStatus)
        {
            /* Save Certificate */
            ficaStatus = FICA_Save_Certificate(imgType);

            if (SLN_FLASH_MGMT_EOVERFLOW == ficaStatus || SLN_FLASH_MGMT_EOVERFLOW2 == ficaStatus)
            {
                /* Handle file header overflow */
                ficaStatus = FICA_Erase_Certificate(imgType);

                if (SLN_FLASH_NO_ERROR == ficaStatus)
                {
                    /*  Save Certificate again */
                    ficaStatus = FICA_Save_Certificate(imgType);
                }
            }
        }

        if (SLN_FLASH_NO_ERROR == ficaStatus)
        {
            savedCert = true;
        }

        if (SLN_FLASH_NO_ERROR == ficaStatus)
        {
            /*  Verify Image Signature */
            ficaStatus = FICA_Verify_Signature(imgType);
        }

        configPRINTF(("Signature verification status: %d.\r\n", ficaStatus));
    }

    if (SLN_FLASH_NO_ERROR != ficaStatus)
    {
        status = kComms_FailedProcessing;
    }

    if (kComms_Success != status)
    {
        /* Invalidate certificate */
        if (savedCert)
        {
            ficaStatus = FICA_Invalidate_Certificate(imgType);

            if (SLN_FLASH_MGMT_EOVERFLOW == ficaStatus || SLN_FLASH_MGMT_EOVERFLOW2 == ficaStatus)
            {
                /* Handle file header overflow */
                ficaStatus = FICA_Erase_Certificate(imgType);

                if (SLN_FLASH_NO_ERROR == ficaStatus)
                {
                    /* Invalidate certificate again */
                    ficaStatus = FICA_Invalidate_Certificate(imgType);
                }
            }

            if (SLN_FLASH_NO_ERROR != ficaStatus)
            {
                configPRINTF(("Failed to invalidate certificate, error %d.\r\n", ficaStatus));
            }
        }
    }
#endif

    if (kComms_Success == status)
    {
        /* Image is valid, switch the bank*/
        ficaStatus = FICA_app_program_ext_set_reset_vector();
        if (SLN_FLASH_NO_ERROR != ficaStatus)
        {
            status = kComms_FailedProcessing;
        }
    }

    if(kComms_Success != status)
    {
        configPRINTF(("The self check request failed\r\n"));
        /* Turn on awesome red led show here */
        ErrorBlinkLED();
    }

    /* Explicitly send the message, it won't send an error by default due to the reset */
    jsonMessage = cJSON_CreateObject();

    jsonError = cJSON_CreateNumber(status);

    cJSON_AddItemToObject(jsonMessage, "error", jsonError);

    status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);

    /* Free the memory */
    cJSON_Delete(jsonMessage);
    selfTestCleanup();

    /* give time to logging task to print last logs */
    vTaskDelay(1000);

    /* Reset the board in the working image */
    NVIC_SystemReset();

    return status;
}

/**
 * @brief Process the incoming FwUpdate Stop JSON message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateStopReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonError;

    sln_comms_message_status status                 = kComms_Success;
    int32_t fica_status                             = SLN_FLASH_NO_ERROR;
    sln_comms_fwupdate_job_desc *currentFwUpdateJob = getCurrentFwUpdateJob();

    if (currentFwUpdateJob == NULL)
    {
        status = kComms_UnexpectedMessage;
    }

    /* Ensure the size that was indicated matches the number of bytes written */
    if (currentFwUpdateJob->imageSize == currentFwUpdateJob->dataWritten)
    {
        fica_status = FICA_Save_Signature((uint8_t *)currentFwUpdateJob->signature);
        if (SLN_FLASH_NO_ERROR != fica_status)
        {
            configPRINTF(("FICA_Save_Signature failed, error %d.\r\n", fica_status));
            status = kComms_FailedProcessing;
        }

        fica_status = FICA_app_program_ext_finalize();
        if (SLN_FLASH_NO_ERROR != fica_status)
        {
            configPRINTF(("FICA_app_program_ext_finalize failed, error %d.\r\n", fica_status));
            status = kComms_FailedProcessing;
        }
    }
    else
    {
        status = kComms_FailedProcessing;
    }

    if (kComms_Success == status)
    {
        jsonMessage = cJSON_CreateObject();

        jsonError = cJSON_CreateNumber(status);

        cJSON_AddItemToObject(jsonMessage, "error", jsonError);

        status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);

        /* Free the memory */
        cJSON_Delete(jsonMessage);
    }

    return status;
}

/**
 * @brief Process the incoming Activate Image JSON message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateActivateImgReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonError;
    sln_comms_message_status status = kComms_Success;
    int32_t fica_status             = SLN_FLASH_NO_ERROR;

    /* Clear the bit and enable the FWUPDATE */
    fica_status = FICA_clr_comm_flag(FICA_COMM_FWUPDATE_BIT);

    if (SLN_FLASH_NO_ERROR != fica_status)
    {
        status = kComms_FailedProcessing;
    }

    configPRINTF(("Activating the new MCU image.\r\n"));

    if (kComms_Success != status)
    {
        /* Reset NAP bit; restore PRDB context;
         * FWUPDATE bit is already set at FwUpdate task initialization phase
         */
        selfTestCleanup();
    }

    /* Need to send the status before resetting */
    jsonMessage = cJSON_CreateObject();

    jsonError = cJSON_CreateNumber(status);

    cJSON_AddItemToObject(jsonMessage, "error", jsonError);

    status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);

    /* Free the memory */
    cJSON_Delete(jsonMessage);

    /* reset MCU */
    configPRINTF(("Stop Request Actioned\r\n"));

    /* give time to logging task to print last logs */
    vTaskDelay(1000);

    NVIC_SystemReset();

    return status;
}

/**
 * @brief Process the incoming FwUpdate block JSON message
 *
 * @param clientInstance: The client instance data (received data and connection context)
 * @param json: Incoming payload to be processed
 *
 * @return          sln_comms_message_status
 */
static sln_comms_message_status processFwUpdateBlockReq(void *clientInstance, cJSON *json)
{
    cJSON *jsonMessage, *jsonFwUpdateBlockOffset, *jsonFwUpdateBlockSize, *jsonFwUpdateBlockData,
        *jsonFwUpdateEncodedSize, *jsonError;
    sln_common_connection_recv_context *readContext = clientInstance;
    sln_comms_message_status status                 = kComms_Success;
    int32_t fica_status                             = SLN_FLASH_NO_ERROR;
    sln_comms_fwupdate_job_desc *currentFwUpdateJob = getCurrentFwUpdateJob();
    sln_comms_fwupdate_block blockReq;
    uint32_t decodeSize  = 0;
    uint32_t recvCrc     = 0;
    uint32_t calcCrc     = 0;
    uint32_t calcCrcSize = COMMS_PACKET_CRC_SIZE;

    if (currentFwUpdateJob == NULL)
    {
        status = kComms_UnexpectedMessage;
    }

    /* Check CRC */
    if (kComms_Success == status)
    {
        memcpy(&recvCrc, readContext->data + COMMS_PACKET_CRC_OFFSET(readContext->packet_size), COMMS_PACKET_CRC_SIZE);

        DCP_HASH(DCP, &dcpCrcHhandle, kDCP_Crc32, readContext->data, readContext->packet_size - COMMS_PACKET_CRC_SIZE,
                 (uint8_t *)&calcCrc, &calcCrcSize);

        if (recvCrc != calcCrc)
        {
            status = kComms_CrcMismatch;
        }
    }

    /* Parse the offset to write to */
    if (kComms_Success == status)
    {
        jsonFwUpdateBlockOffset = cJSON_GetObjectItemCaseSensitive(json, "offset");
        if (!cJSON_IsNumber(jsonFwUpdateBlockOffset) ||
            jsonFwUpdateBlockOffset->valueint >= currentFwUpdateJob->imageSize)
        {
            status = kComms_InvalidParameter;
        }
        else
        {
            blockReq.offset = jsonFwUpdateBlockOffset->valueint;
        }
    }

    /* Parse the block size */
    if (kComms_Success == status)
    {
        jsonFwUpdateBlockSize = cJSON_GetObjectItemCaseSensitive(json, "block_size");
        if (!cJSON_IsNumber(jsonFwUpdateBlockSize))
        {
            status = kComms_InvalidParameter;
        }
        else
        {
            blockReq.blockSize = jsonFwUpdateBlockSize->valueint;
        }
    }

    /* Parse the encoded data size */
    if (kComms_Success == status)
    {
        jsonFwUpdateEncodedSize = cJSON_GetObjectItemCaseSensitive(json, "encoded_size");
        if (!cJSON_IsNumber(jsonFwUpdateEncodedSize))
        {
            status = kComms_InvalidParameter;
        }
        else
        {
            blockReq.encodedSize = jsonFwUpdateEncodedSize->valueint;
        }
    }

    /* Parse the encoded block data */
    if (kComms_Success == status)
    {
        jsonFwUpdateBlockData = cJSON_GetObjectItemCaseSensitive(json, "block");
        if (!cJSON_IsString(jsonFwUpdateBlockData))
        {
            status = kComms_InvalidParameter;
        }
        else
        {
            blockReq.encodedData = jsonFwUpdateBlockData->valuestring;
        }
    }

    /* Decode the data ready to write to flash */
    if (kComms_Success == status)
    {
        uint32_t mbedStatus = 0;
        blockReq.data       = pvPortMalloc(blockReq.blockSize);

        if (blockReq.data != NULL)
        {
            mbedStatus = mbedtls_base64_decode(blockReq.data, blockReq.blockSize, &decodeSize,
                                               (const unsigned char *)blockReq.encodedData, blockReq.encodedSize);

            if (mbedStatus != 0)
            {
                status = kComms_InvalidParameter;
            }
        }
        else
        {
            status = kComms_FailedProcessing;
        }
    }
    if (kComms_Success == status)
    {
        if (blockReq.blockSize != decodeSize)
        {
            status = kComms_InvalidParameter;
        }
    }
    /* TODO: Added CRC check */
#if 0
    if (kComms_Success == status)
    {
        status = DCP_HASH(DCP, handle, kDCP_Crc32, in, inSize, (uint8_t *)out, outSize);
    }
#endif

    /* Check if the address inside the image is the same with the address computed from the bank type*/
    if ((blockReq.offset == 0) && (blockReq.blockSize > COMMS_PACKET_IMG_ADDR_IDX) && (kComms_Success == status))
    {
        uint32_t addrFromImg = 0; // The base programming address from the passed image reset vector
        uint32_t addrRecv    = 0; // The base programming address from the received bank type

        addrFromImg |= (blockReq.data[COMMS_PACKET_IMG_ADDR_IDX] << 16);
        status = FICA_get_app_img_start_addr(currentFwUpdateJob->appBankType, &addrRecv);

        if ((kComms_Success == status) && (addrRecv != addrFromImg))
        {
            status = kComms_InvalidParameter;
        }
    }

    if (kComms_Success == status)
    {
        /* Write the data to flash */
        fica_status = FICA_app_program_ext_abs(blockReq.offset, (void *)blockReq.data, blockReq.blockSize);
        if (SLN_FLASH_NO_ERROR != fica_status)
        {
            configPRINTF(("FICA_app_program_ext_abs failed, error %d.\r\n", fica_status));
            status = kComms_FailedProcessing;
        }

        if (kComms_Success == status)
        {
            currentFwUpdateJob->dataWritten += blockReq.blockSize;
        }

        if (blockReq.data != NULL)
        {
            vPortFree(blockReq.data);
        }
    }

    if (kComms_Success == status)
    {
        jsonMessage = cJSON_CreateObject();

        jsonError = cJSON_CreateNumber(status);

        cJSON_AddItemToObject(jsonMessage, "error", jsonError);

        status = SLN_COMMS_MESSAGE_Send(clientInstance, jsonMessage);

        /* Free the memory */
        cJSON_Delete(jsonMessage);
    }

    return status;
}
