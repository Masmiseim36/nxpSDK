/*! *********************************************************************************
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * \file
 *
 * This is the header file for the OTA Programming Support.
 *
 ** SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _OTA_SUPPORTSERVER_H_
#define _OTA_SUPPORTSERVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "fsl_adapter_flash.h"
#include "fwk_platform.h"

/*!
 * @addtogroup OTA_module
 * The OTA_module
 *
 * OTA_module provides APIs a collection of over the air update features.
 * @{
 */
/*!
 * @addtogroup OTA_server_support
 * The OTA server module
 *
 * OTA_server_support provides APIs a collection of over the air update features.
 * @{
 */

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
/* OTA server - operating modes */
typedef enum
{
    gUpgradeImageOnCurrentDevice_c = 0,
    gUseExternalMemoryForOtaUpdate_c,
    gDoNotUseExternalMemoryForOtaUpdate_c,
    gOtaTestingProcess
} otaMode_t;

typedef otaResult_t (*pfOTA_Callback_t)(uint8_t *pBuffer, uint16_t len);

typedef struct otaServer_AppCB_tag
{
    pfOTA_Callback_t otaServerImgNotifyCnf;
    pfOTA_Callback_t otaServerSetModeCnf;
    pfOTA_Callback_t otaServerQueryImageCnf;
    pfOTA_Callback_t otaServerPushChunkCnf;
    pfOTA_Callback_t otaServerCancelImgCnf;
    pfOTA_Callback_t otaServerAbortProcessCnf;
    pfOTA_Callback_t otaServerSetFileVersPoliciesCnf;
    pfOTA_Callback_t otaServerClientInfoCnf;
} otaServer_AppCB_t;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/*! *********************************************************************************
 * \brief  Registers the OTA component to FSCI
 *
 * \param[in] fsciInterface  The Id of the FSCI interface used by the OTAP Server
 * \param[in] pCB            Pointer to a table of callback function.
 *
 * \return  error code.
 *
 ********************************************************************************** */
otaResult_t OTA_RegisterToFsci(uint32_t fsciInterface, otaServer_AppCB_t *pCB);

/*! *********************************************************************************
 * \brief  This returns to the OTA server application the value of the current upgrade mode
 *
 * \return UpgradeMode value
 *
 ********************************************************************************** */
otaMode_t OTA_GetUpgradeMode(void);

/*! *********************************************************************************
 * \brief  Set parameter allowing updagrade on current device
 *
 * \param[in]  allow value to set
 *
 ********************************************************************************** */
void OTA_AllowUpgradeOnCurrentDevice(bool allow);

/*! *********************************************************************************
 * \brief  This function sends a request for a new image chunk over the FSCI serial interface
 *
 * \param[in] offset  image offset
 * \param[in] len     requested chunk length
 * \param[in] devId   Id of the device
 *
 ********************************************************************************** */
void OTA_ImageChunkReq(uint32_t offset, uint16_t len, uint16_t devId);

/*! *********************************************************************************
 * \brief  This function sends over the FSCI serial interface a query for a specific image
 *
 * \param[in] devId          Id of the device
 * \param[in] manufacturer   The manufacturer code
 * \param[in] imgType        Type of the image
 * \param[in] fileVersion    Minimum version of the image
 *
 ********************************************************************************** */
void OTA_QueryImageReq(uint16_t devId, uint16_t manufacturer, uint16_t imgType, uint32_t fileVersion);

/*! *********************************************************************************
 * \brief
 *
 * \param[in] pClientAddr    Pointer to the client address (16 bytes)
 * \param[in] offset
 * \param[in] devId          Id of the device
 *
 ********************************************************************************** */
void OTA_ClientInfoCnf(uint8_t *pClientAddr, uint32_t offset, uint16_t devId);

#ifdef __cplusplus
}
#endif

/*!
 * @}  end of OTA_server_support addtogroup
 */
/*!
 * @}  end of OTA_module addtogroup
 */

#endif /* _OTA_SUPPORT_H_ */
