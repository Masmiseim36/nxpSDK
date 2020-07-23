/*! *********************************************************************************
* \addtogroup Alert Notification Service
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef ALERT_NOTIFICATION_INTERFACE_H
#define ALERT_NOTIFICATION_INTERFACE_H

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
/*! Alert Notification Service - Command Id */
#define gAns_EnableNewIncomingAlert_c                   0x00U
#define gAns_EnableUnreadCategoryStatus_c               0x01U
#define gAns_DisableNewIncomingAlert_c                  0x02U
#define gAns_DisableUnreadCategoryStatus_c              0x03U
#define gAns_NotifyNewIncomingAlertImmediately_c        0x04U
#define gAns_NotifyUnreadCategoryStatusImmediately_c    0x05U

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Alert Notification Service - Command Id */
typedef uint8_t ansCommandId_t;

typedef uint8_t ansCategoryId_t;

/*! Alert Notification Service - Category Id */
typedef enum
{
    gAns_SimpleAlert_c      = 0x00,
    gAns_Email_c,
    gAns_News_c,
    gAns_IncomingCall_c,
    gAns_MissedCall_c,
    gAns_SmsMms_c,
    gAns_VoiceMail_c,
    gAns_Schedule_c,
    gAns_HighPriorityAlert_c,
    gAns_InstantMessage_c
} ansCategoryId_tag;

typedef uint16_t ansCategoryIdMask_t;

/*! Alert Notification Service - Category Id Mask*/
typedef enum
{
    gAns_SimpleAlertSupported_c         = BIT0,
    gAns_EmailSupported_c               = BIT1,
    gAns_NewsSupported_c                = BIT2,
    gAns_IncomingCallSupported_c        = BIT3,
    gAns_MissedCallSupported_c          = BIT4,
    gAns_SmsMmsSupported_c              = BIT5,
    gAns_VoiceMailSupported_c           = BIT6,
    gAns_ScheduleSupported_c            = BIT7,
    gAns_HighPriorityAlertSupported_c   = BIT8,
    gAns_InstantMessageSupported_c      = BIT9,
    gAns_SupportAll_c                   = 0x01FF
} ansCategoryIdMask_tag;

/*! Alert Notification Service - Command */
typedef struct ansCommand_tag
{
    ansCommandId_t  commandId;
    ansCategoryId_t categoryId;
} ansCommand_t;

/*! Alert Notification Service - New Alert */
typedef struct ansNewAlert_tag
{
    ansCategoryId_t categoryId;
    uint8_t         numOfNewAlert;
    char            text[18];
    uint8_t         textLength;
} ansNewAlert_t;

/*! Alert Notification Service - Unread Alert Status*/
typedef struct ansUnreadAlertStatus_tag
{
    ansCategoryId_t categoryId;
    uint8_t         unreadCount;
} ansUnreadAlertStatus_t;

/*! Alert Notification Service - Configuration */
typedef struct ansConfig_tag
{
    uint16_t            serviceHandle;
} ansConfig_t;

/*! Alert Notification Service - ATT Error Codes */
enum
{
    gAttErrCodeCmdNotSupported_c = 0xA0
};

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
* \brief        Starts Alert Notification Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_Start(ansConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Alert Notification Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_Stop(ansConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Alert Notification service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes the GATT client from the Alert Notification service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Sends Unread Alert Status to the subscribed client
*
* \param[in]    serviceHandle   Service handle
* \param[in]    pAlert          Pointer to the alert structure.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_SendUnreadAlertStatus(uint16_t serviceHandle, ansUnreadAlertStatus_t *pAlert);

/*!**********************************************************************************
* \brief        Sends New Alert to the subscribed client
*
* \param[in]    serviceHandle   Service handle
* \param[in]    pAlert          Pointer to the alert structure.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_SendNewAlert(uint16_t serviceHandle, ansNewAlert_t *pAlert);

/*!**********************************************************************************
* \brief        Handles write operations on the SC Control point.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server
*                               configuration information.
* \param[in]    pEvent          Pointer to the GATT Server event.
*
************************************************************************************/
void  Ans_ControlPointHandler(ansConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

#ifdef __cplusplus
}
#endif

#endif /* ALERT_NOTIFICATION_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */
