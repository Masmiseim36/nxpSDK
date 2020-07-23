/*! *********************************************************************************
* \addtogroup Phone Alert Status Service
* @{
********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef _PHONE_ALERT_STATUS_INTERFACE_H_
#define _PHONE_ALERT_STATUS_INTERFACE_H_

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

#define gPass_AlertStatusMaxValue_c         0x07

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/*! Phone Alert Status Service - Alert Status */
typedef uint8_t passAlertStatus_t;

typedef uint8_t passAlertStatusMask_t;

/*! Phone Alert Status Service - Alert Status Mask */
typedef enum
{
    gPass_RingerStateActive_c       = BIT0,
    gPass_VibrateStateActive_c      = BIT1,
    gPass_DisplayAlertStatuActive_c = BIT2
}passAlertStatusMask_tag;


typedef uint8_t passRingerSetting_t;

/*! Phone Alert Status Service - Ringer Setting */
typedef enum
{
    gPass_RingerSilent_c  = 0,
    gPass_RingerNormal_c  = 1
}passRingerSetting_tag;


typedef uint8_t passRingerControlPoint_t;

/*! Phone Alert Status Service - Ringer Control Point */
typedef enum
{
    gPass_SilentMode_c        = 1,
    gPass_MuteOnce_c          = 2,
    gPass_CancelSilentMode_c  = 3
}passRingerControlPoint_tag;


/*! Phone Alert Status Service - Configuration */
typedef struct passConfig_tag
{
    uint16_t    serviceHandle;
} passConfig_t;

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
* \brief        Starts Phone Alert Status Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_Start(void);

/*!**********************************************************************************
* \brief        Stops Phone Alert Status Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_Stop(void);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Phone Alert Status service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes the GATT client from the Phone Alert Status service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_Unsubscribe(void);

/*!**********************************************************************************
* \brief        Records the new Alert Status and notify the subscribed client
*
* \param[in]    serviceHandle   Service handle
* \param[in]    asValue         New Alert Status value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_RecordAlertStatus(uint16_t serviceHandle, passAlertStatus_t asValue);

/*!**********************************************************************************
* \brief        Records the new Ringer Setting and notify the subscribed client
*
* \param[in]    serviceHandle   Service handle
* \param[in]    rsValue         New Ringer Setting value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_RecordRingerSetting(uint16_t serviceHandle, passRingerSetting_t rsValue);

/*!**********************************************************************************
* \brief        Handles write operations on the Control point.
*
* \param[in]    serviceHandle   Service handle
* \param[in]    pEvent          Pointer to the GATT Server event.
* \param[out]   pbOutRingerOn   Pointer to a boolean value which indicates if ringer must be On or Off.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Pass_ControlPointHandler(uint16_t serviceHandle, gattServerAttributeWrittenEvent_t *pEvent, bool_t* pbOutRingerOn);

#ifdef __cplusplus
}
#endif 

#endif /* _PHONE_ALERT_STATUS_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
