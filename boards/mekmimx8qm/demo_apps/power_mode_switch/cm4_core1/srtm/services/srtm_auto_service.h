/*
 * Copyright (c) 2018, NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SRTM_AUTO_SERVICE_H__
#define __SRTM_AUTO_SERVICE_H__

#include "srtm_service.h"

/*!
 * @addtogroup srtm_service
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** @brief Switch to disable Auto service debugging messages. */
#ifndef SRTM_AUTO_SERVICE_DEBUG_OFF
#define SRTM_AUTO_SERVICE_DEBUG_OFF (0)
#endif

#if SRTM_AUTO_SERVICE_DEBUG_OFF
#undef SRTM_DEBUG_VERBOSE_LEVEL
#define SRTM_DEBUG_VERBOSE_LEVEL SRTM_DEBUG_VERBOSE_NONE
#endif

/* Protocol definition */
#define SRTM_USER_DATA_LENGTH (11)

/* Auto Service Notification Command definition */

typedef enum
{
    /* Auto Service Request Command definition */
    SRTM_AUTO_CMD_REGISTER = 0U,
    SRTM_AUTO_CMD_UNREGISTER,
    SRTM_AUTO_CMD_CONTROL,
    SRTM_AUTO_CMD_POWER_REPORT,
    SRTM_AUTO_CMD_GET_INFO,
    /* Send commands to remote processor/core */
    SRTM_AUTO_CMD_BOOT_REASON,
    SRTM_AUTO_CMD_POWER_CTRL,
    SRTM_AUTO_CMD_VEHICLE_STATE,
} srtm_auto_cmd_t;

/**
 * @brief SRTM Auto payload structure
 */
SRTM_ANON_DEC_BEGIN
SRTM_PACKED_BEGIN struct _srtm_auto_payload
{
    uint32_t clientId;
    union
    {
        uint8_t reserved; /* used in request packet */
        uint8_t retCode;  /* used in response packet */
    };
    uint8_t data[SRTM_USER_DATA_LENGTH];
} SRTM_PACKED_END;
SRTM_ANON_DEC_END

/**
 * @brief SRTM Auto adapter structure pointer.
 */
typedef struct _srtm_auto_adapter *srtm_auto_adapter_t;

/**
 * @brief SRTM Auto adapter structure
 */
struct _srtm_auto_adapter
{
    /* Bound service */
    srtm_service_t service;

    /* Interfaces implemented by Auto adapter. reqData and respData point to the data area of _srtm_auto_payload */
    srtm_status_t (*registerEvent)(srtm_auto_adapter_t adapter,
                                   uint32_t clientId,
                                   uint8_t *reqData,
                                   uint32_t reqLen,
                                   uint8_t *respData,
                                   uint32_t respLen);
    srtm_status_t (*unregisterEvent)(srtm_auto_adapter_t adapter,
                                     uint32_t clientId,
                                     uint8_t *reqData,
                                     uint32_t reqLen,
                                     uint8_t *respData,
                                     uint32_t respLen);
    srtm_status_t (*control)(srtm_auto_adapter_t adapter,
                             uint32_t clientId,
                             uint8_t *reqData,
                             uint32_t reqLen,
                             uint8_t *respData,
                             uint32_t respLen);
    srtm_status_t (*powerReport)(srtm_auto_adapter_t adapter,
                                 uint32_t clientId,
                                 uint8_t *reqData,
                                 uint32_t reqLen,
                                 uint8_t *respData,
                                 uint32_t respLen);
    srtm_status_t (*getInfo)(srtm_auto_adapter_t adapter,
                             uint32_t clientId,
                             uint8_t *reqData,
                             uint32_t reqLen,
                             uint8_t *respData,
                             uint32_t respLen);
};

typedef void (*srtm_auto_cmd_cb_t)(srtm_service_t service,
                                   uint32_t clientId,
                                   srtm_auto_cmd_t cmd,
                                   uint8_t *cmdParam,
                                   uint32_t paramLen,
                                   uint8_t retCode,
                                   uint8_t *result,
                                   uint32_t resultLen,
                                   void *userParam);

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Create auto service.
 *
 * @param adapter Auto adapter to provide real auto features.
 * @return SRTM service handle on success and NULL on failure.
 */
srtm_service_t SRTM_AutoService_Create(srtm_auto_adapter_t adapter);

/*!
 * @brief Destroy auto service.
 *
 * @param service SRTM service to destroy.
 */
void SRTM_AutoService_Destroy(srtm_service_t service);

/*!
 * @brief Reset auto service. This is used to stop sending events and return to initial state.
 *
 * @param service SRTM service to reset.
 * @param core Identify which core is to be reset.
 */
void SRTM_AutoService_Reset(srtm_service_t service, srtm_peercore_t core);

/*!
 * @brief Send AUTO command to client specified by client ID, and wait for result until time out.
 *  Note: the function cannot be called in ISR or SRTM dispatcher task.
 *
 * @param service SRTM AUTO service.
 * @param clientId AUTO client ID.
 * @param cmd AUTO command to be sent.
 * @param cmdParam AUTO command parameter to be sent.
 * @param paramLen AUTO command parameter length in bytes.
 * @param result AUTO command result buffer pointer.
 * @param resultLen AUTO command result length in bytes.
 * @param timeout Maximum milliseconds to wait for each client, must bigger than 0.
 * @return SRTM_Status_Success on success and others on failure.
 */
srtm_status_t SRTM_AutoService_SendCommand(srtm_service_t service,
                                           uint32_t clientId,
                                           srtm_auto_cmd_t cmd,
                                           uint8_t *cmdParam,
                                           uint32_t paramLen,
                                           uint8_t *result,
                                           uint32_t resultLen,
                                           uint32_t timeout);

/*!
 * @brief Send AUTO command to client specified by client ID, and return without waiting for the result.
 *  Result can be returned via command callback.
 *
 * @param service SRTM AUTO service.
 * @param clientId AUTO client ID.
 * @param cmd AUTO command to be sent.
 * @param cmdParam AUTO command parameter to be sent.
 * @param paramLen AUTO command parameter length in bytes.
 * @param callback Function pointer to be called after result is ready.
 * @param userParam User parameter used in the callback.
 * @return SRTM_Status_Success on success and others on failure.
 */
srtm_status_t SRTM_AutoService_DeliverCommand(srtm_service_t service,
                                              uint32_t clientId,
                                              srtm_auto_cmd_t cmd,
                                              uint8_t *cmdParam,
                                              uint32_t paramLen,
                                              srtm_auto_cmd_cb_t callback,
                                              void *userParam);

#ifdef __cplusplus
}
#endif

/*! @} */

#endif /* __SRTM_AUTO_SERVICE_H__ */
