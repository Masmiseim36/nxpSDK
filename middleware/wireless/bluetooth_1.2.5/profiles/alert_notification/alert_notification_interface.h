/*! *********************************************************************************
* \addtogroup Alert Notification Service
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _ALERT_NOTIFICATION_INTERFACE_H_
#define _ALERT_NOTIFICATION_INTERFACE_H_

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

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef uint8_t ansCommandId_t;

/*! Alert Notification Service - Command Id */
typedef enum
{
    gAns_EnableNewIncomingAlert_c      = 0x00,
    gAns_EnableUnreadCategoryStatus_c,
    gAns_DisableNewIncomingAlert_c,
    gAns_DisableUnreadCategoryStatus_c,
    gAns_NotifyNewIncomingAlertImmediately_c,
    gAns_NotifyUnreadCategoryStatusImmediately_c
} ansCommandId_tag;

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
    uint8_t         text[18];
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
bleResult_t Ans_Start (ansConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Alert Notification Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ans_Stop (ansConfig_t *pServiceConfig);

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
bleResult_t Ans_Unsubscribe();

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
void  Ans_ControlPointHandler (ansConfig_t *pServiceConfig, gattServerAttributeWrittenEvent_t *pEvent);

#ifdef __cplusplus
}
#endif 

#endif /* _ALERT_NOTIFICATION_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
