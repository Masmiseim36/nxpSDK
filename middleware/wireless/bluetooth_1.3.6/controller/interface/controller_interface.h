/*! *********************************************************************************
 * \defgroup CONTROLLER - Controller Interface
 * @{
 ********************************************************************************** */
/*! *********************************************************************************
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef CONTROLLER_INTERFACE_H
#define CONTROLLER_INTERFACE_H

#ifndef CPU_QN908X

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "fsl_os_abstraction.h"
#include "ble_general.h"


/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

#define Controller_SetAdvertisingTxPowerLevel(level) \
    Controller_SetTxPowerLevel(level,gAdvTxChannel_c)

#define Controller_SetConnectionTxPowerLevel(level) \
    Controller_SetTxPowerLevel(level,gConnTxChannel_c)

/************************************************************************************
*************************************************************************************
* Structures/Data Types
*************************************************************************************
************************************************************************************/

typedef enum txChannelType_tag {
    gAdvTxChannel_c,
    gConnTxChannel_c
} txChannelType_t;

typedef enum dtmBaudrate_tag {
    gDTM_BaudRate_1200_c = 0,
    gDTM_BaudRate_2400_c,
    gDTM_BaudRate_9600_c,
    gDTM_BaudRate_14400_c,
    gDTM_BaudRate_19200_c,
    gDTM_BaudRate_38400_c,
    gDTM_BaudRate_57600_c,
    gDTM_BaudRate_115200_c
} dtmBaudrate_t;

typedef struct bleCtrlNotificationEvent_tag
{
    uint16_t event_type; /*! bleNotificationEventType_t */
    uint16_t conn_handle;
    uint8_t  rssi;
    uint8_t  channel_index;
    uint16_t conn_ev_counter;
    uint16_t timestamp;
    uint8_t  adv_handle;
} bleCtrlNotificationEvent_t;

typedef void (*bleCtrlNotificationCallback_t)
( 
    bleCtrlNotificationEvent_t *pNotificationEvent
);
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern osaEventId_t mControllerTaskEvent;

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
 * \brief  Sets the BD_ADDR.
 * \param[in]  packetType HCI packet Type
 *
 * \return gBleSuccess_c, gBleInvalidState_c or gBleInvalidParameter_c
 ********************************************************************************** */
bleResult_t Controller_SetDeviceAddress(uint8_t* bdAddr);

/*! *********************************************************************************
 * \brief  Get the distance to the next LL activity
 * \param[in]  pNextInstant Pointer to the instant of the next LL activity 
 *                          in BT clock slot (0x0000 to 0xFFFF)
 * \return uint16_t Distance to the next activity in number of BT clock slots
 *
 * \remarks If LL is busy, the returned distance will be 0 and *pNextInstant will be 
 * updated with the slot of the next instant. If the returned distance is not zero, 
 * the actual distance to the next activity can have an error of +/- 1 slot.
 ********************************************************************************** */
uint16_t Controller_GetNextInstance(uint16_t* pNextInstant);

/*! *********************************************************************************
 * \brief  Controller Get Inactivity Duration
 *
 * \return uint32_t
 ********************************************************************************** */
uint32_t Controller_GetInactivityDuration(void);

/*! *********************************************************************************
 * \brief  Sets the TX Power on the advertising or connection channel.
 * \param[in]  level    Power level (range 0-15) as defined in the table bellow.
 * \param[in]  channel  Advertising or connection channel.
 *
 * \return gBleSuccess_c or error.
 *
 * \remarks This function executes synchronously.
 *
 * \remarks For MKW40Z BLE controller there are 16 possible power levels 0 <= N <= 15
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet.
 *
 * \remarks For MKW41Z BLE controller there are 32 possible power levels 0 <= N <= 31
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet.
 *
 * \remarks For MKW36 BLE controller there are 32 possible power levels 0 <= N <= 31
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet.
 ********************************************************************************** */
bleResult_t Controller_SetTxPowerLevel(uint8_t level, txChannelType_t channel);

/*! *********************************************************************************
 * \brief  Configure enhanced notifications on advertising, scannning and connection events 
 *         on the controller.
 * \param[in]  eventType       Event type selection as specified by bleNotificationEventType_t.
 * \param[in]  conn_handle     Connection handle of the peer, used only for connection events.
 *
 * \return gBleSuccess_c or error.
 ********************************************************************************** */
bleResult_t Controller_ConfigureEnhancedNotification
(
    uint16_t eventType,
    uint16_t conn_handle
);

/*! *********************************************************************************
 * \brief  Controller Register Enhanced Notification Event Callback
 * \param[in]  notificationCallback Callback to be executed in adv/scan/conn ISR context
 *
 * \return gBleSuccess_c or error.
 ********************************************************************************** */
bleResult_t Controller_RegisterEnhancedEventCallback
(
    bleCtrlNotificationCallback_t notificationCallback
);

/*! *********************************************************************************
 * \brief  Set the delay between the beginnings of two consecutive advertising PDU’s
 *         inside the same advertising event.
 * \param[in]  adv_pkt_interval Advertising interval in units of 625us. This interval
 *                              should be long enough to fix the advertising frame and
 *                              eventual the scan request and scan response frames
 *                              if applicable. The bluetooth specification fixes the
 *                              maximum interval to 10ms. This corresponds to
 *                              maximum adv_pkt_interval value of 16.
 * \param[in]  adv_set_identifier Advertising set identifier, used on if multiple
 *                                adverting sets are supported.
 *
 * \return gBleSuccess_c, gBleInvalidParameter_c or gBleFeatureNotSupported_c.
 *
 * \remarks The caller needs to make sure that the adv_pkt_interval value
 * has to be consistent with the advertising interval with:
 * T_advEvent = advInterval (20ms to 10.24s) + advDelay ( 0 to 10ms).
 *
 ********************************************************************************** */
bleResult_t Controller_SetAdvPktInterval(uint8_t adv_pkt_interval,
                                         uint8_t adv_set_identifier);

#ifdef __cplusplus
}
#endif



/* ******************************************************************************** */
/* ******************************************************************************** */
/* ******************************************************************************** */
#else /* CPU_QN908X */

#include "hci_transport.h"
#include "fsl_power.h"

void        Controller_TaskHandler(void* handler);

void        BLE_Init(uint8_t max_ble_devices);

/*
 * @brief Send deep sleep command to controller
 * @retunr: the sleep mode the controller will enter
 */
power_mode_t BLE_sleep(void);

/*
 * @brief Get the sleep mode allowed by BLE Controller
 *        No sleep command is executed.
 * @return: the sleep mode the controller will enter
 */
power_mode_t BLE_get_sleep_mode(void);

/*
 * @brief Informs the controller deep sleep is not yet permited as wake-up is not finished.
 * The BLE controller will allow deep sleep once wake-up is finished.
 */
void BLE_prevent_sleep_on_wakeup(void);

/*
 * @brief Informs the controller deep sleep is permited.
 */
void BLE_enable_sleep(void);

/*
 * @brief Informs the controller deep sleep is NOT permited.
 */
void BLE_disable_sleep(void);

/*
 * @brief Enable the hardware WiFi coexistence mechanism in BLE controller.
 */
void BLE_EnableWlanCoex(void);

#endif /* CPU_QN908X */


#define BD_ADDR_SIZE 6

/************************************************************************************
 ************************************************************************************
 *                  Common header section, for all platforms                        *
 ************************************************************************************
 ***********************************************************************************/

typedef bleResult_t (*gHostHciRecvCallback_t)
(
    hciPacketType_t packetType,
    void* pHciPacket,
    uint16_t hciPacketLength
);

/*! *********************************************************************************
 * \brief  Performs initialization of the Controller.
 * \param[in]  callback HCI Host Receive Callback
 *
 * \return osaStatus_Success or osaStatus_Error
 ********************************************************************************** */
osaStatus_t Controller_Init(gHostHciRecvCallback_t callback);

/*! *********************************************************************************
 * \brief  Controller Receive Interface
 * \param[in]  packetType HCI packet Type
 * \param[in]  pPacket    data buffer
 * \param[in]  packetSize data buffer length
 *
 * \return gBleSuccess_c, gBleOutOfMemory_c or gBleInvalidParameter_c
 ********************************************************************************** */
bleResult_t Hci_SendPacketToController( hciPacketType_t packetType, void* pPacket,
                                uint16_t packetSize);

/*! *********************************************************************************
 * \brief  Controller Set DTM Baudrate
 *
 * \return void
 ********************************************************************************** */
bleResult_t Controller_SetDTMBaudrate(dtmBaudrate_t baudrate);

#endif /* CONTROLLER_INTERFACE_H */

/*! *********************************************************************************
* @}
********************************************************************************** */

