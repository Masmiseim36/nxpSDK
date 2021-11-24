/*
 * Copyright 2020-2021 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

/*
 * @brief qn9090 ble wireless uart communication implementation.
 */

#include <FreeRTOS.h>
#include <queue.h>

#include "board_define.h"
#include "board.h"
#include "fsl_lpuart_freertos.h"
#include "fsl_lpuart.h"

#include "fwk_log.h"
#include "fwk_message.h"
#include "fwk_sln_task.h"
#include "fwk_input_manager.h"
#include "fwk_output_manager.h"
#include "fwk_lpm_manager.h"
#include "hal_event_descriptor_face_rec.h"
#include "hal_input_dev.h"
#include "hal_smart_lock_config.h"

/*******************************************************************************
 * Defines
 ******************************************************************************/
#define BLE_WUART_NAME          "qn9090"
#define BLE_WUART_TASK_NAME     "ble_wireless_uart_task"
#define BLE_WUART_TASK_STACK    1024
#define BLE_WUART_TASK_PRIORITY 7

#define BLE_WUART_QUEUE_LENGTH 5

#define BLE_WUART_HEADER_LENGTH 24

#define BLE_WUART_READY     "BLE-READY_"
#define BLE_WUART_CONNECTED "BLE-CONNECTED_"

#define BLE_WUART_MINI_TRANS_UINT 24

typedef enum _hal_ble_connection_status_t
{
    kHALBLEConnectionStatus_ScanningNG    = 0,
    kHALBLEConnectionStatus_ConnectedCTED = 1,
    kHALBLEConnectionStatus_Invalid,
} _hal_ble_connection_status_t;

typedef enum _hal_ble_wuart_status_t
{
    kHALBLEWUARTStatus_Success,
    kHALBLEWUARTStatus_TUMagicError,
    kHALBLEWUARTStatus_TUCRC32Error,
    kHALBLEWUARTStatus_PacketCRC32Error,
    kHALBLEWUARTStatus_PacketValid,
    kHALBLEWUARTStatus_PacketError,
    kHALBLEWUARTStatus_PacketShort,
} hal_ble_wuart_status_t;

typedef enum _hal_ble_wuart_packet_t
{
    kHALBLEWUARTPacket_Header,
    kHALBLEWUARTPacket_Data,
} hal_ble_wuart_packet_t;

typedef struct _hal_ble_wuart_response_t
{
    union
    {
        oasis_lite_registration_result_t remoteRegResult;
    };
} hal_ble_wuart_response_t;

typedef struct _hal_header_transfer_unit_t
{
    uint8_t tuMagic[3];
    uint8_t pktType;
    uint32_t pktLen;
    uint32_t pktId;
    uint32_t pktCrc;
    uint32_t tuCrc;
    uint32_t reserved;
} hal_header_transfer_unit_t;

typedef enum _hal_transfer_packet_type_t
{
    AUTHENTICATION_REQ = 0,
    AUTHENTICATION_RES,
    UPDATE_PASSWORD_REQ,
    UPDATE_PASSWORD_RES,
    REGISTRATION_REQ,
    REGISTRATION_RES,
    DELETE_USER_REQ,
    DELETE_USER_RES,
    GET_USER_COUNT_REQ,
    GET_USER_COUNT_RES,
    GET_USER_INFO_REQ,
    GET_USER_INFO_RES,
    UPDATE_USER_INFO_REQ,
    UPDATE_USER_INFO_RES,
    REGISTRATION_CMD_REQ,
    REGISTRATION_CMD_RES,
    DEREGISTRATION_CMD_REQ,
    DEREGISTRATION_CMD_RES,
    PREVIEWCAMERA_SWITCH_CMD_REQ,
    PREVIEWCAMERA_SWITCH_CMD_RES,
    GET_APP_TYPE_REQ,
    GET_APP_TYPE_RES,
    GET_ALGO_VERSION_REQ,
    GET_ALGO_VERSION_RES,
    FIRMWARE_RESPONSE = 0xff,
} hal_ble_transfer_packet_type_t;

static const uint8_t TU_MAGIC[] = {0x53, 0x79, 0x4c};

typedef enum _hal_wuart_ack_reserved_t
{
    BLE_WUART_ACK_SUCCESS   = 0,
    BLE_WUART_ACK_DUPLICATE = 1,
    BLE_WUART_ACK_ERROR     = -1, // 0xffffffffff
} hal_wuart_ack_reserved_t;

typedef struct _hal_ble_wuart_transfer_t
{
    uint8_t *nibbleBuf;
    uint8_t *packetBuf;
    int nibbleLen;
    int packetLen;
    int readOffset;
    uint8_t stage;
} hal_ble_wuart_transfer_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */
void BOARD_InitBleQn9090Resource(void);
#if defined(__cplusplus)
}
#endif /* __cplusplus */

static hal_input_status_t HAL_InputDev_BleWuartQn9090_Init(input_dev_t *dev, input_dev_callback_t callback);
static hal_input_status_t HAL_InputDev_BleWuartQn9090_Deinit(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_BleWuartQn9090_Start(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_BleWuartQn9090_Stop(const input_dev_t *dev);
static hal_input_status_t HAL_InputDev_BleWuartQn9090_InputNotify(const input_dev_t *dev, void *param);

static hal_output_status_t HAL_OutputDev_BleWuartQn9090_Start(const output_dev_t *dev);
static hal_output_status_t HAL_OutputDev_BleWuartQn9090_InferComplete(const output_dev_t *dev,
                                                                      output_algo_source_t source,
                                                                      void *inferResult);
static hal_output_status_t HAL_OutputDev_BleWuartQn9090_InputNotify(const output_dev_t *dev, void *param);
static hal_ble_wuart_status_t SLN_BLEWUARTSendPacket(
    uint8_t *data, uint32_t len, uint8_t type, uint32_t pktId, uint32_t result);

static lpuart_rtos_handle_t s_LpuartRTOSHandle;
static lpuart_handle_t s_LpuartHandle;
static uint8_t s_LpuartRingBuffer[512];

static uint8_t s_QN9090IsConnected    = kHALBLEConnectionStatus_Invalid;
static uint8_t s_QN9090MacAddress[18] = {0}; // ble mac address xx:xx:xx:xx:xx:xx

static uint8_t s_BLEWUARTNibbleBuffer[BLE_WUART_MINI_TRANS_UINT];
static hal_ble_wuart_transfer_t s_BLEWUARTTransfer;
static hal_header_transfer_unit_t s_HeaderTransferUnit = {0};
static event_face_rec_t s_BLEWUARTEvent;
static input_event_t s_InputEvent;

const static input_dev_operator_t s_InputDev_BLEWUARTQN9090Ops = {
    .init        = HAL_InputDev_BleWuartQn9090_Init,
    .deinit      = HAL_InputDev_BleWuartQn9090_Deinit,
    .start       = HAL_InputDev_BleWuartQn9090_Start,
    .stop        = HAL_InputDev_BleWuartQn9090_Stop,
    .inputNotify = HAL_InputDev_BleWuartQn9090_InputNotify,
};

static input_dev_t s_InputDev_BLEWUARTQN9090 = {
    .id = 1, .name = BLE_WUART_NAME, .ops = &s_InputDev_BLEWUARTQN9090Ops, .cap = {.callback = NULL}};

const static output_dev_event_handler_t s_OutputDev_BLEWUARTQN9090Handler = {
    .inferenceComplete = HAL_OutputDev_BleWuartQn9090_InferComplete,
    .inputNotify       = HAL_OutputDev_BleWuartQn9090_InputNotify,
};

const static output_dev_operator_t s_OutputDev_BLEWUARTQN9090Ops = {
    .init   = NULL,
    .deinit = NULL,
    .start  = HAL_OutputDev_BleWuartQn9090_Start,
    .stop   = NULL,
};

static output_dev_t s_OutputDev_BLEWUARTQN9090 = {
    .name         = BLE_WUART_NAME,
    .attr.type    = kOutputDevType_Other,
    .attr.reserve = NULL,
    .ops          = &s_OutputDev_BLEWUARTQN9090Ops,
};

static hal_lpm_request_t s_LpmReq = {.dev = &s_InputDev_BLEWUARTQN9090, .name = "s_InputDev_BLEWUARTQN9090"};

static uint32_t table[0x100];
static uint32_t s_pckID;

static int HAL_InputDev_BleWuartQn9090_Respond(uint32_t eventId,
                                               void *response,
                                               event_status_t status,
                                               unsigned char isFinished)
{
    if (response == NULL)
    {
        return -1;
    }

    switch (eventId)
    {
        case kEventFaceRecID_GetUserList:
        {
            user_info_event_t usersInfo = *(user_info_event_t *)response;

            if (usersInfo.count == 0)
            {
                SLN_BLEWUARTSendPacket(NULL, 0, GET_USER_INFO_RES, s_pckID, usersInfo.count);
            }
            else
            {
                SLN_BLEWUARTSendPacket((uint8_t *)usersInfo.userInfo, usersInfo.count * sizeof(face_user_info_t),
                                       GET_USER_INFO_RES, s_pckID, usersInfo.count);
            }
        }
        break;

        case kEventFaceRecID_GetUserCount:
        {
            uint8_t count = *(uint8_t *)response;
            SLN_BLEWUARTSendPacket(NULL, 0, GET_USER_COUNT_RES, s_pckID, count);
        }
        break;

        case kEventFaceRecID_UpdateUserInfo:
        {
            if (status == kEventStatus_Ok)
            {
                SLN_BLEWUARTSendPacket(NULL, 0, UPDATE_USER_INFO_RES, s_pckID, BLE_WUART_ACK_SUCCESS);
            }
            else
            {
                SLN_BLEWUARTSendPacket(NULL, 0, UPDATE_USER_INFO_RES, s_pckID, BLE_WUART_ACK_ERROR);
            }
        }
        break;

        case kEventFaceRecID_DelUser:
        {
            if (status == kEventStatus_Ok)
            {
                SLN_BLEWUARTSendPacket(NULL, 0, DELETE_USER_RES, s_pckID, BLE_WUART_ACK_SUCCESS);
            }
            else
            {
                SLN_BLEWUARTSendPacket(NULL, 0, DELETE_USER_RES, s_pckID, BLE_WUART_ACK_ERROR);
            }
        }
        break;

        case kEventFaceRecID_AddUserRemote:
        {
            if (status == kEventStatus_Ok)
            {
                remote_reg_result_t *res = response;
                if (res->result == OASIS_REG_RESULT_OK)
                {
                    SLN_BLEWUARTSendPacket(NULL, 0, REGISTRATION_RES, s_pckID, BLE_WUART_ACK_SUCCESS);
                }
                else if (res->result == OASIS_REG_RESULT_DUP)
                {
                    SLN_BLEWUARTSendPacket((uint8_t *)res->name, FACE_NAME_MAX_LEN + 1, REGISTRATION_RES, s_pckID,
                                           BLE_WUART_ACK_DUPLICATE);
                }
                else
                {
                    SLN_BLEWUARTSendPacket(NULL, 0, REGISTRATION_RES, s_pckID, BLE_WUART_ACK_ERROR);
                }
            }
            else
            {
                LOGE("[ERROR]: BleWirelessUartTask kEventFaceRecID_AddUserRemote error.\r\n");
                SLN_BLEWUARTSendPacket(NULL, 0, REGISTRATION_RES, s_pckID, BLE_WUART_ACK_ERROR);
            }

            vPortFree(s_BLEWUARTEvent.remoteReg.regData);

            s_BLEWUARTEvent.remoteReg.regData      = NULL;
            s_BLEWUARTEvent.remoteReg.dataLen      = 0;
            s_BLEWUARTEvent.remoteReg.isReRegister = -1;
        }
        break;

        default:
            break;
    }

    return 0;
}

static uint32_t crc32_for_byte(uint32_t r)
{
    for (int j = 0; j < 8; ++j)
    {
        r = (r & 1 ? 0 : (uint32_t)0xEDB88320L) ^ r >> 1;
    }

    return r ^ (uint32_t)0xFF000000L;
}

static void crc32(const void *data, size_t numBytes, uint32_t *crc)
{
    memset(table, 0x00, sizeof(table));

    if (!*table)
    {
        for (size_t i = 0; i < 0x100; ++i)
        {
            table[i] = crc32_for_byte(i);
        }
    }

    for (size_t i = 0; i < numBytes; ++i)
    {
        *crc = table[(uint8_t)*crc ^ ((uint8_t *)data)[i]] ^ *crc >> 8;
    }
}

static void PRINTF_HEX(uint8_t *data, int len)
{
    for (int i = 0; i < len; i++)
    {
        LOGD("0x%2x ", data[i]);
    }
}

/* Press QN_WAKEUP to request ble sync current connected status*/
static void SLN_BLEWUARTPressWakePin()
{
    GPIO_PinWrite(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    GPIO_PinWrite(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(50));
    GPIO_PinWrite(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, 1);
}

/* Long press QN_WAKEUP to request ble disconnected with apk, and start scan. */
static void SLN_BLEWUARTLongPressWakePin()
{
    GPIO_PinWrite(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, 1);
    vTaskDelay(pdMS_TO_TICKS(500));
    GPIO_PinWrite(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, 0);
    vTaskDelay(pdMS_TO_TICKS(500));
    GPIO_PinWrite(BOARD_BLE_QN9090_QN_WAKEUP_GPIO, BOARD_BLE_QN9090_QN_WAKEUP_PIN, 1);
}

uint8_t SLN_BLEWUARTIsConnected()
{
    return s_QN9090IsConnected;
}

static void SLN_BLEWUARTCreateHeader(
    uint8_t *htUnit, uint8_t *data, uint32_t len, uint8_t type, uint32_t pktId, uint32_t result)
{
    uint32_t crc_res = 0;

    /* transfer magic */
    memcpy(htUnit, TU_MAGIC, sizeof(TU_MAGIC));

    /* packet type */
    htUnit[3] = type;

    /* packet length */
    *((uint32_t *)(htUnit + 4)) = len;

    /* packet id */
    *((uint32_t *)(htUnit + 8)) = pktId;

    if (len > 0)
    {
        /* packet crc */
        crc32(data, len, &crc_res);
        *((uint32_t *)(htUnit + 12)) = crc_res;
    }
    else
    {
        *((uint32_t *)(htUnit + 12)) = 0;
    }

    /* packet reserved */
    *((uint32_t *)(htUnit + 16)) = result;

    /* transfer unit crc */
    crc_res = 0;
    crc32(htUnit, BLE_WUART_HEADER_LENGTH - 4, &crc_res);
    *((uint32_t *)(htUnit + 20)) = crc_res;
}

static hal_ble_wuart_status_t SLN_BLEWUARTSendPacket(
    uint8_t *data, uint32_t len, uint8_t type, uint32_t pktId, uint32_t result)
{
    hal_ble_wuart_status_t status = kHALBLEWUARTStatus_Success;

    uint8_t htUnit[BLE_WUART_HEADER_LENGTH] = {0};

    SLN_BLEWUARTCreateHeader(htUnit, data, len, type, pktId, result);

    LPUART_RTOS_Send(&s_LpuartRTOSHandle, htUnit, BLE_WUART_HEADER_LENGTH);

    if (len > 0)
    {
        LPUART_RTOS_Send(&s_LpuartRTOSHandle, data, len);
    }

    return status;
}

static hal_ble_wuart_status_t SLN_BLEWUARTParseHeader(uint8_t *headerBuf, hal_header_transfer_unit_t *pHtUnit)
{
    hal_ble_wuart_status_t status = kHALBLEWUARTStatus_Success;
    uint32_t crc_res              = 0;

    /* transfer magic */
    if (memcmp(headerBuf, TU_MAGIC, sizeof(TU_MAGIC)) != 0)
    {
        return kHALBLEWUARTStatus_TUMagicError;
    }
    memcpy(pHtUnit->tuMagic, headerBuf, 3);

    /* packet type */
    pHtUnit->pktType = headerBuf[3];

    /* packet length */
    pHtUnit->pktLen = *((uint32_t *)(headerBuf + 4));

    /* packet id */
    pHtUnit->pktId = *((uint32_t *)(headerBuf + 8));

    /* packet crc */
    pHtUnit->pktCrc = *((uint32_t *)(headerBuf + 12));

    /* packet reserved */
    pHtUnit->reserved = *((uint32_t *)(headerBuf + 16));

    /* transfer unit crc */
    pHtUnit->tuCrc = *((uint32_t *)(headerBuf + 20));
    crc32(headerBuf, BLE_WUART_HEADER_LENGTH - 4, &crc_res);
    if (crc_res != pHtUnit->tuCrc)
    {
        return kHALBLEWUARTStatus_TUCRC32Error;
    }

    return status;
}

static hal_ble_wuart_status_t SLN_BLEWUARTParseData(uint8_t *dataBuf, hal_header_transfer_unit_t *pHtUnit)
{
    hal_ble_wuart_status_t status = kHALBLEWUARTStatus_Success;
    uint32_t crc_res              = 0;
    crc32(dataBuf, pHtUnit->pktLen, &crc_res);
    if (crc_res != pHtUnit->pktCrc)
    {
        return kHALBLEWUARTStatus_PacketCRC32Error;
    }

    s_pckID = pHtUnit->pktId;
    /* handle all kinds of packet type */
    switch (pHtUnit->pktType)
    {
        case AUTHENTICATION_REQ:
        {
            /* compare password */
            if (pHtUnit->pktLen != sizeof(((smart_lock_config_t *)0)->password))
            {
                SLN_BLEWUARTSendPacket(NULL, 0, AUTHENTICATION_RES, pHtUnit->pktId, BLE_WUART_ACK_ERROR);
            }
            else
            {
                uint8_t *pPassword = pvPortMalloc(pHtUnit->pktLen);
                HAL_OutputDev_SmartLockConfig_GetPassword(pPassword);
                if (memcmp(dataBuf, pPassword, pHtUnit->pktLen) == 0)
                {
                    SLN_BLEWUARTSendPacket(NULL, 0, AUTHENTICATION_RES, pHtUnit->pktId, BLE_WUART_ACK_SUCCESS);
                }
                else
                {
                    SLN_BLEWUARTSendPacket(NULL, 0, AUTHENTICATION_RES, pHtUnit->pktId, BLE_WUART_ACK_ERROR);
                }

                vPortFree(pPassword);
            }
        }
        break;

        case UPDATE_PASSWORD_REQ:
        {
            /* update password */
            if (pHtUnit->pktLen != sizeof(((smart_lock_config_t *)0)->password))
            {
                SLN_BLEWUARTSendPacket(NULL, 0, UPDATE_PASSWORD_RES, pHtUnit->pktId, BLE_WUART_ACK_ERROR);
            }
            else
            {
                if (HAL_OutputDev_SmartLockConfig_SetPassword(dataBuf) == kSLNConfigStatus_Success)
                {
                    SLN_BLEWUARTSendPacket(NULL, 0, UPDATE_PASSWORD_RES, pHtUnit->pktId, BLE_WUART_ACK_SUCCESS);
                }
                else
                {
                    SLN_BLEWUARTSendPacket(NULL, 0, UPDATE_PASSWORD_RES, pHtUnit->pktId, BLE_WUART_ACK_ERROR);
                }
            }
        }
        break;

        case REGISTRATION_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;

            s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_AddUserRemote;
            s_BLEWUARTEvent.eventBase.respond = HAL_InputDev_BleWuartQn9090_Respond;

            if (pHtUnit->reserved == 0)
            {
                s_BLEWUARTEvent.remoteReg.isReRegister = 0;
            }
            else
            {
                s_BLEWUARTEvent.remoteReg.isReRegister = 1;
            }

            s_BLEWUARTEvent.remoteReg.dataLen = pHtUnit->pktLen;
            s_BLEWUARTEvent.remoteReg.regData = pvPortMalloc(pHtUnit->pktLen);

            if (s_BLEWUARTEvent.remoteReg.regData != NULL)
            {
                memcpy(s_BLEWUARTEvent.remoteReg.regData, dataBuf, pHtUnit->pktLen);

                uint8_t fromISR = __get_IPSR();
                if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
                {
                    s_InputEvent.inputData = &s_BLEWUARTEvent;
                    s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                           &s_InputEvent, 0, fromISR);
                }
            }
            else
            {
                LOGE("[ERROR]: BleWirelessUartTask REGISTRATION_REQ malloc buffer error.\r\n");
            }
        }
        break;

        case DELETE_USER_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;
            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_DelUser;
                s_BLEWUARTEvent.eventBase.respond = HAL_InputDev_BleWuartQn9090_Respond;
                s_BLEWUARTEvent.delFace.hasName   = false;
                s_BLEWUARTEvent.delFace.hasID     = true;
                s_BLEWUARTEvent.delFace.id        = *(uint32_t *)dataBuf;
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }
        }
        break;

        case GET_USER_COUNT_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;

            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_GetUserCount;
                s_BLEWUARTEvent.eventBase.respond = HAL_InputDev_BleWuartQn9090_Respond;
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }
        }
        break;

        case GET_USER_INFO_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;

            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_GetUserList;
                s_BLEWUARTEvent.eventBase.respond = HAL_InputDev_BleWuartQn9090_Respond;
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }
        }
        break;

        case UPDATE_USER_INFO_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;
            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_UpdateUserInfo;
                s_BLEWUARTEvent.eventBase.respond = HAL_InputDev_BleWuartQn9090_Respond;
                s_BLEWUARTEvent.updateFace        = *(update_user_event_t *)dataBuf;
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }
        }
        break;

        case REGISTRATION_CMD_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;

            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_AddUser;
                s_BLEWUARTEvent.eventBase.respond = HAL_InputDev_BleWuartQn9090_Respond;
                s_BLEWUARTEvent.addFace.hasName   = false;
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }

            SLN_BLEWUARTSendPacket(NULL, 0, REGISTRATION_CMD_RES, pHtUnit->pktId, BLE_WUART_ACK_SUCCESS);
        }
        break;

        case DEREGISTRATION_CMD_REQ:
        {
            uint32_t receiverList = 1 << kFWKTaskID_VisionAlgo;

            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventFaceRecID_DelUser;
                s_BLEWUARTEvent.delFace.hasName   = false;
                s_BLEWUARTEvent.delFace.hasID     = false;
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }

            SLN_BLEWUARTSendPacket(NULL, 0, DEREGISTRATION_CMD_RES, pHtUnit->pktId, BLE_WUART_ACK_SUCCESS);
        }
        break;

        case PREVIEWCAMERA_SWITCH_CMD_REQ:
        {
            SLN_BLEWUARTSendPacket(NULL, 0, PREVIEWCAMERA_SWITCH_CMD_RES, pHtUnit->pktId, 0);
        }
        break;

        case GET_APP_TYPE_REQ:
        {
            SLN_BLEWUARTSendPacket(NULL, 0, GET_APP_TYPE_RES, pHtUnit->pktId, HAL_OutputDev_SmartLockConfig_GetMode());
        }
        break;

        case GET_ALGO_VERSION_REQ:
        {
            uint8_t version[4] = {VERSION_HOTFIX, VERSION_MINOR, VERSION_MAJOR, 0};
            SLN_BLEWUARTSendPacket(NULL, 0, GET_ALGO_VERSION_RES, pHtUnit->pktId, *((uint32_t *)version));
        }
        break;

        /* reserved for sync connection with qn9090 ble firmware. */
        case FIRMWARE_RESPONSE:
        {
            if (memcmp(dataBuf, BLE_WUART_READY, sizeof(BLE_WUART_READY) - 1) == 0)
            {
                if (s_QN9090IsConnected == kHALBLEConnectionStatus_ScanningNG)
                {
                    LOGD("[BleWirelessUartTask]: BLE QN9090 already start scan.");
                    break;
                }

                s_QN9090IsConnected = kHALBLEConnectionStatus_ScanningNG;
                FWK_LpmManager_RuntimeSet(&s_LpmReq, 0);

                if (pHtUnit->pktLen == ((sizeof(s_QN9090MacAddress) - 1) + (sizeof(BLE_WUART_READY) - 1)))
                {
                    memcpy(s_QN9090MacAddress, dataBuf + sizeof(BLE_WUART_READY) - 1, sizeof(s_QN9090MacAddress) - 1);
                }
                else
                {
                    LOGE("[BleWirelessUartTask]: BLE QN9090 MAC Address contained an error.");
                }

                LOGD("[BleWirelessUartTask]: BLE QN9090 start scan.");
            }
            else if (memcmp(dataBuf, BLE_WUART_CONNECTED, sizeof(BLE_WUART_CONNECTED) - 1) == 0)
            {
                if (s_QN9090IsConnected == kHALBLEConnectionStatus_ConnectedCTED)
                {
                    LOGD("[BleWirelessUartTask]: BLE QN9090 already connected.");
                    break;
                }

                s_QN9090IsConnected = kHALBLEConnectionStatus_ConnectedCTED;
                FWK_LpmManager_RuntimeSet(&s_LpmReq, 1);

                if (pHtUnit->pktLen == ((sizeof(s_QN9090MacAddress) - 1) + (sizeof(BLE_WUART_CONNECTED) - 1)))
                {
                    memcpy(s_QN9090MacAddress, dataBuf + sizeof(BLE_WUART_CONNECTED) - 1,
                           sizeof(s_QN9090MacAddress) - 1);
                }
                else
                {
                    LOGE("[BleWirelessUartTask]: BLE QN9090 MAC Address contained an error.");
                }
                LOGD("[BleWirelessUartTask]: BLE QN9090 connected with apk.");
            }
            else
            {
                LOGD("[BleWirelessUartTask]: BLE QN9090 unsupported status.");
                break;
            }

            uint32_t receiverList = 1 << kFWKTaskID_Output;

            if (s_InputDev_BLEWUARTQN9090.cap.callback != NULL)
            {
                s_BLEWUARTEvent.eventBase.eventId = kEventID_SetBLEConnection;
                s_BLEWUARTEvent.wuart.length      = 0;
                s_BLEWUARTEvent.wuart.status      = SLN_BLEWUARTIsConnected();
                uint8_t fromISR                   = __get_IPSR();
                s_InputEvent.inputData            = &s_BLEWUARTEvent;
                s_InputDev_BLEWUARTQN9090.cap.callback(&s_InputDev_BLEWUARTQN9090, kInputEventID_Recv, receiverList,
                                                       &s_InputEvent, 0, fromISR);
            }
        }
        break;

        default:
            break;
    }

    return status;
}

static void SLN_BLEWUARTMsgHandle(void *param)
{
    LOGD("[BleWirelessUartTask] start.");

    hal_ble_wuart_status_t status = kHALBLEWUARTStatus_PacketValid;

    size_t n = 0;

    hal_ble_wuart_transfer_t *transfer = param;

    /* press wakeup pin sync ble latest connection status. */
    SLN_BLEWUARTPressWakePin();

    LPUART_RTOS_Receive(&s_LpuartRTOSHandle, transfer->nibbleBuf, BLE_WUART_HEADER_LENGTH, &n);

    while (1)
    {
        switch (transfer->stage)
        {
            case kHALBLEWUARTPacket_Header:
            {
                status = SLN_BLEWUARTParseHeader(transfer->nibbleBuf, &s_HeaderTransferUnit);
                LOGD("HEADER_PACKET: ");

                if (status == kHALBLEWUARTStatus_Success)
                {
                    transfer->readOffset = 0;
                    transfer->stage      = kHALBLEWUARTPacket_Data;
                    transfer->packetBuf  = pvPortMalloc(s_HeaderTransferUnit.pktLen);
                    if (transfer->packetBuf == NULL)
                    {
                    }

                    transfer->packetLen = s_HeaderTransferUnit.pktLen;
                    LPUART_RTOS_Receive(&s_LpuartRTOSHandle, transfer->packetBuf, transfer->packetLen, &n);
                    LOGD("HEADER PACKET PARSE SUCCESSFUL[%d].", transfer->packetLen);
                }
                else
                {
                    LPUART_RTOS_Receive(&s_LpuartRTOSHandle, transfer->nibbleBuf, BLE_WUART_HEADER_LENGTH, &n);
                    LOGD("HEADER PACKET PARSE FAILED[%d].", transfer->packetLen);
                    PRINTF_HEX(transfer->nibbleBuf, BLE_WUART_HEADER_LENGTH);
                }
            }
            break;

            case kHALBLEWUARTPacket_Data:
            {
                status = SLN_BLEWUARTParseData(transfer->packetBuf, &s_HeaderTransferUnit);
                LOGD("DATA_PACKET: ");

                if (status == kHALBLEWUARTStatus_Success)
                {
                    LOGD("DATA PACKET PARSE SUCCESSFUL[%d].", transfer->packetLen);
                }
                else
                {
                    LOGD("DATA PACKET PARSE FAILED[%d].", transfer->packetLen);
                    PRINTF_HEX(transfer->packetBuf, s_HeaderTransferUnit.pktLen);
                }

                vPortFree(transfer->packetBuf);
                memset(&s_HeaderTransferUnit, 0x00, sizeof(s_HeaderTransferUnit));
                memset(transfer->nibbleBuf, 0x0, transfer->nibbleLen);
                transfer->stage = kHALBLEWUARTPacket_Header;

                /* prepare next count of data need received, it will block task.*/
                LPUART_RTOS_Receive(&s_LpuartRTOSHandle, transfer->nibbleBuf, BLE_WUART_HEADER_LENGTH, &n);
            }
            break;

            default:
                break;
        }
    }
}

static hal_input_status_t HAL_InputDev_BleWuartQn9090_Init(input_dev_t *dev, input_dev_callback_t callback)
{
    hal_input_status_t status = kStatus_HAL_InputSuccess;

    lpuart_rtos_config_t config = {
        .base        = LPUART8,
        .baudrate    = 115200,
        .parity      = kLPUART_ParityDisabled,
        .stopbits    = kLPUART_OneStopBit,
        .buffer      = s_LpuartRingBuffer,
        .buffer_size = sizeof(s_LpuartRingBuffer),
#if defined(FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT) && FSL_FEATURE_LPUART_HAS_MODEM_SUPPORT
        .enableRxRTS = 0,
        .enableTxCTS = 0,
        .txCtsSource = 0,
        .txCtsConfig = 0,
#endif
    };

    config.srcclk = CLOCK_GetRootClockFreq(kCLOCK_Root_Lpuart8);

    BOARD_InitBleQn9090Resource();

    NVIC_SetPriority(LPUART8_IRQn, configMAX_SYSCALL_INTERRUPT_PRIORITY - 1);
    if (kStatus_Success != LPUART_RTOS_Init(&s_LpuartRTOSHandle, &s_LpuartHandle, &config))
    {
        vTaskSuspend(NULL);
    }

    dev->cap.callback = callback;

    return status;
}

static hal_input_status_t HAL_InputDev_BleWuartQn9090_Deinit(const input_dev_t *dev)
{
    hal_input_status_t status = kStatus_HAL_InputSuccess;
    return status;
}

static hal_input_status_t HAL_InputDev_BleWuartQn9090_Start(const input_dev_t *dev)
{
    hal_input_status_t status = kStatus_HAL_InputSuccess;

    s_BLEWUARTTransfer.nibbleBuf  = s_BLEWUARTNibbleBuffer;
    s_BLEWUARTTransfer.nibbleLen  = sizeof(s_BLEWUARTNibbleBuffer);
    s_BLEWUARTTransfer.packetBuf  = NULL;
    s_BLEWUARTTransfer.packetLen  = 0;
    s_BLEWUARTTransfer.readOffset = 0;
    s_BLEWUARTTransfer.stage      = kHALBLEWUARTPacket_Header;

    s_BLEWUARTEvent.eventBase.respond = NULL;

    if (xTaskCreate(SLN_BLEWUARTMsgHandle, BLE_WUART_TASK_NAME, BLE_WUART_TASK_STACK, &s_BLEWUARTTransfer,
                    BLE_WUART_TASK_PRIORITY, NULL) != pdPASS)
    {
        LOGE("[BleWirelessUart] Task creation failed!.");
        while (1)
            ;
    }

    return status;
}

static hal_input_status_t HAL_InputDev_BleWuartQn9090_Stop(const input_dev_t *dev)
{
    hal_input_status_t status = kStatus_HAL_InputSuccess;
    return status;
}

static hal_input_status_t HAL_InputDev_BleWuartQn9090_InputNotify(const input_dev_t *dev, void *param)
{
    hal_input_status_t status = kStatus_HAL_InputSuccess;
    return status;
}

static hal_output_status_t HAL_OutputDev_BleWuartQn9090_Start(const output_dev_t *dev)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    if (FWK_OutputManager_RegisterEventHandler(dev, &s_OutputDev_BLEWUARTQN9090Handler) != 0)
        status = kStatus_HAL_OutputError;
    return status;
}

static hal_output_status_t HAL_OutputDev_BleWuartQn9090_InferComplete(const output_dev_t *dev,
                                                                      output_algo_source_t source,
                                                                      void *inferResult)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    return status;
}

static hal_output_status_t HAL_OutputDev_BleWuartQn9090_InputNotify(const output_dev_t *dev, void *param)
{
    hal_output_status_t status = kStatus_HAL_OutputSuccess;
    event_base_t eventBase     = *(event_base_t *)param;
    if (eventBase.eventId == kEventID_GetBLEConnection)
    {
        event_status_t eventResponse = kEventStatus_Ok;
        ble_address_t bleAddress     = {
            .ssid = {'V', 'N', '\0'},
        };
        if (s_QN9090MacAddress[0] != '\0')
        {
            // mac address string format xx:xx:xx:xx:xx:xx

            char *p    = (char *)s_QN9090MacAddress;
            char *ssid = bleAddress.ssid;
            p          = strchr(p, ':') + 1; /* 1st ':' */
            p          = strchr(p, ':') + 1; /* 2nd ':' */
            p          = strchr(p, ':') + 1; /* 3rd ':' */
            strncat(ssid, p, 2);
            p = strchr(p, ':') + 1; /* 4th ':' */
            strncat(ssid, p, 2);
            p = strchr(p, ':') + 1; /* 5th ':' */
            strncat(ssid, p, 2);
            LOGD("[BLE SSID][%s]", ssid);
            LOGD("[BLE Address][%s]", s_QN9090MacAddress);
        }
        else
        {
            eventResponse = kEventStatus_Error;
            LOGI("[BLE Address] No MacAddress. Check that the BLE has been started.");
        }

        if (eventBase.respond != NULL)
        {
            eventBase.respond(eventBase.eventId, &bleAddress, eventResponse, true);
        }
    }
    return status;
}

int HAL_Dev_BleWuartQn9090_Register()
{
    hal_input_status_t status = kStatus_HAL_InputSuccess;
    LOGD("HAL_Dev_BleWuartQn9090_Register");

    status = FWK_InputManager_DeviceRegister(&s_InputDev_BLEWUARTQN9090);
    if (status)
    {
        return status;
    }

    status = FWK_OutputManager_DeviceRegister(&s_OutputDev_BLEWUARTQN9090);
    if (status)
    {
        return status;
    }

    status = FWK_LpmManager_RegisterRequestHandler(&s_LpmReq);
    return status;
}
