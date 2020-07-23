/*! *********************************************************************************
* \addtogroup HID Profile
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* This file is the interface file for the HID Profile
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef HID_INTERFACE_H
#define HID_INTERFACE_H

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/
#define gGattService_HumanInterfaceDevice_c 0x1812

/*! HID Service - Control Point Values (hidControlPointValues_t) */
#define gHid_Suspend_c                      0x00U
#define gHid_ExitSuspend_c                  0x01U

/*! HID Service - Protocol Mode Values (hidProtocolMode_t) */
#define gHid_BootProtocolMode_c             0x00U
#define gHid_ReportProtocolMode_c           0x01U
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! HID Service - Control Point Values*/
typedef uint8_t hidControlPointValues_t;

/*! HID Service - Protocol Mode Values*/
typedef uint8_t hidProtocolMode_t;

/*! HID Service - Report Type */
typedef uint8_t hidReportType_t;
typedef enum
{
    gHid_InputReport_c = 0x01,
    gHid_OutputReport_c = 0x02,
    gHid_FeatureReport_c = 0x03,
}hidReportType_tag;

/*! HID Service - HID Info Flags */
typedef uint8_t hidInfoFlags_t;
typedef enum
{
    gHid_InfoFlagsNone_C        = 0x00,
    gHid_RemoteWake_c           = BIT0,
    gHid_NormallyConnectable_c  = BIT1
}hidInfoFlags_tag;

/*! HID Service - HID Information */
typedef struct hidInfo_tag
{
    uint16_t        bcdHID;
    uint8_t         bCountryCode;
    hidInfoFlags_t  flags;
} hidInfo_t;

/*! HID Service - Configuration */
typedef struct hidConfig_tag
{
    uint16_t            serviceHandle;
    hidProtocolMode_t   protocolMode;
    hidInfo_t           hidInfo;
    uint8_t             *pReportMap;
} hidConfig_t;


/*! HID Client - Configuration */
typedef struct hidcConfig_tag
{
    uint16_t    hService;
    uint16_t    hHidReport;
    uint16_t    hHidReportCccd; 
} hidcConfig_t;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!**********************************************************************************
* \brief        Starts HID Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_Start(hidConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops HID Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_Stop(hidConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the HID service
*
* \param[in]    clientDeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the HID service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Sets the Protocol Mode value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]   protocolMode    Protocol Mode value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_SetProtocolMode(uint16_t serviceHandle, hidProtocolMode_t protocolMode);

/*!**********************************************************************************
* \brief        Gets the Protocol Mode value on a specified service.
*
* \param[in]    serviceHandle       Service handle.
* \param[out]   pOutProtocolMode    Pointer to store the Protocol Mode value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_GetProtocolMode(uint16_t serviceHandle, hidProtocolMode_t *pOutProtocolMode);

/*!**********************************************************************************
* \brief        Sends an HID input report on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    reportlen       Length of report.
* \param[in]    pInReport       Pointer to report value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_SendInputReport(uint16_t serviceHandle, uint16_t reportlen, void* pInReport);

/*!**********************************************************************************
* \brief        Sends an HID boot mouse input report on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    reportlen       Length of report.
* \param[in]    pInReport       Pointer to report value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Hid_SendBootMouseInputReport(uint16_t serviceHandle, uint16_t reportlen, void* pInReport);
/*!**********************************************************************************
* \brief        Handles command on the HID Control Point
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    value           Command Value.
*
* \return       gAttErrCodeNoError_c or error.
*************************************************************************************/
uint8_t Hid_ControlPointHandler (uint16_t serviceHandle, hidControlPointValues_t value);

#ifdef __cplusplus
}
#endif 

#endif /* HID_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
