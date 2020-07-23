/*! *********************************************************************************
 * \defgroup CONTROLLER - Controller Interface
 * @{
 ********************************************************************************** */
/*!
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
*
* \file
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* o Redistributions of source code must retain the above copyright notice, this list
*   of conditions and the following disclaimer.
*
* o Redistributions in binary form must reproduce the above copyright notice, this
*   list of conditions and the following disclaimer in the documentation and/or
*   other materials provided with the distribution.
*
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its
*   contributors may be used to endorse or promote products derived from this
*   software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _CONTROLLER_INTERFACE_H_
#define _CONTROLLER_INTERFACE_H_

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

/* WARNING: Do not change these defines */
#define gLlAdvSetCtxtSize                64U
#define gLlConnectionCtxtSizeBle5       256U    //for lib_ble_kw38a4_controller.a
#define gLlConnectionCtxtSizeBle4_2     200U    //for lib_ble_kw38a4_controller_ble_4_2.a
#define gLlQueueElmtSize                 12U
#define gLlQueueSizeWithoutAclQueue       4U
#define gLlHciTdQueueElmtSize             8U
#define gLlDupFilListInfoSizeBle5        20U
#define gLlDupFilListInfoSizeBle4_2      12U
#define gLlCmdBufferSizeBle5            256U
#define gLlCmdBufferSizeBle4_2           64U
#define gLlGenEventBufferSizeExtAdv     255U
#define gLlGenEventBufferSizeNoExtAdv    68U
#define gLlWlCtxtSize                     8U
#define gLlPeriodicWlCtxtSize             9U
#define gLlSyncInfoCtxtSize              36U
/************************************************************************************
*************************************************************************************
* Structures/Data Types
*************************************************************************************
************************************************************************************/
typedef enum advCodingScheme_tag {
    gAdv_CodingScheme_S8_S8_c = 0x00, /* primary adv coding scheme S8, secondary adv coding scheme S8 */ 
    gAdv_CodingScheme_S8_S2_c = 0x01, /* primary adv coding scheme S8, secondary adv coding scheme S2 */ 
    gAdv_CodingScheme_S2_S8_c = 0x02, /* primary adv coding scheme S2, secondary adv coding scheme S8 */ 
    gAdv_CodingScheme_S2_S2_c = 0x03, /* primary adv coding scheme S2, secondary adv coding scheme S2 */ 
} advCodingScheme_t;
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

/* Structure used to pass configuration parameter to LL. A runtime check will be done
 to make sure parameter are within the allowed range of the linked LL library. */
typedef struct
{
    /* Extended advertising related info */
    uint16_t    ext_adv_max_used_adv_data_length;
    uint8_t     ext_adv_max_used_set;
    uint8_t     adv_set_ctxt_size_used;
    
    uint8_t     max_sync_engine_used;
    uint8_t     sync_info_ctxt_size_used;
    uint8_t     periodic_wl_size_used;
    uint8_t     wl_size_used;
    uint8_t     wl_ctxt_size_used;
    uint8_t     periodic_wl_ctxt_size_used;
    /* Connection related info */
    uint16_t    connection_ctxt_size_used;
    uint8_t     max_connections_used;
    /* duplicated filtering info */
    uint8_t     gBleMaxDupFilInfo;
    uint16_t    dup_fil_ctxt_size_used;
    /* command buffer related info */
    uint16_t    max_used_cmd_param_length_bytes;
    uint8_t     max_used_generic_event_length_bytes;
    /* TX acl packet info */
    uint8_t     max_configured_ll_tx_acl_pkts;
    /* RX acl packet info */
    uint8_t     max_configured_ll_rx_acl_pkts;
    uint8_t     max_configured_num_event_buffers;
    /* feature related info */
    uint8_t     gBle5FeatureMask1_c;
    uint8_t     gBle5FeatureEnable1_c;
    /* other parameter */
    uint8_t     ll_mem_pool_id;
}
bleCtrlConfigParam;

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/
extern osaEventId_t mControllerTaskEvent;
extern bool_t gMCUSleepDuringBleEvents;
extern bool_t gEnableSingleAdvertisement;
#ifndef NXP_FIX
extern bool_t gActivateHybridMode;
extern void (*pfBLE_SignalFromISR)(void);
#endif /* NXP_FIX */
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*! *********************************************************************************
 * \brief  Controller_TaskHandler
 * \param[in]
 *
 ********************************************************************************** */
void * Controller_TaskHandler(osaTaskParam_t arg);

/*! *********************************************************************************
 * \brief  Controller_InterruptHandler
 * \param[in]
 *
 ********************************************************************************** */
void Controller_InterruptHandler(void);

/*! *********************************************************************************
 * \brief  Sets the BD_ADDR.
 * \param[in]  packetType HCI packet Type
 *
 * \return gBleSuccess_c, gBleInvalidState_c or gBleInvalidParameter_c
 ********************************************************************************** */
bleResult_t Controller_SetDeviceAddress(uint8_t* bdAddr);

/*! *********************************************************************************
 * \brief  Controller Get Next instance
 * \param[in]  pNextInstant Pointer to the calculated_instant CE instant
 *   for applying new channel map or for updating connection parameters.
 * \return uint16_t
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
 * \param[in]  level    Power level (range 0-X) as defined in the table bellow.
 *                      (X=15 for MKW40 and X=31 for MKW41 and MKW38)
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
 * \remarks For MKW38 BLE controller there are 32 possible power levels 0 <= N <= 31
 * for which the output power is distributed evenly between minimum and maximum power levels.
 * For further details see the silicon datasheet. For this latter platform, the preferred
 * interface to set the TX power is Controller_SetTxPowerLevelDbm (see below) which does not
 * require any knowledge of the power tables.
 ********************************************************************************** */
bleResult_t Controller_SetTxPowerLevel(uint8_t level, txChannelType_t channel);

/*! *********************************************************************************
 * \brief  Sets the TX Power on the advertising or connection channel.
  * \param[in]  level_dbm    Power level in dBm.
 * \param[in]  channel      Advertising or connection channel.
 *
 * \return gBleSuccess_c or error.
 *
 * \remarks This function executes synchronously.
 ********************************************************************************** */
bleResult_t Controller_SetTxPowerLevelDbm(int8_t level_dbm, txChannelType_t channel);

/*! *********************************************************************************
 * \brief  Sets the perodic whitelist size
 * \param[in]  size    number of whitelist entries
 *
 * \return gBleSuccess_c or error.
 *
 * \remarks This function executes synchronously.
 ********************************************************************************** */
bleResult_t Controller_SetPeriodicWhiteListSize(uint8_t size);

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
 * \brief  Controller_ConfigureAdvCodingScheme
 * \param[in]  configure adv coding scheme
 ********************************************************************************** */
bleResult_t Controller_ConfigureAdvCodingScheme
( 
    advCodingScheme_t coding_scheme
);

/*! *********************************************************************************
* \brief   Reset, Save and restore Hardware Link Layer Initialize
*
********************************************************************************** */
void Controller_ResetLlhState(void);
void Controller_RestoreLlhState(void);
void Controller_RestoreLlhStateComplete(void);
void Controller_SaveLlhState(void);

/*! *********************************************************************************
* \brief   Reprogram the next ADV event time - shall be called during restore process
*            before calling Controller_RestoreLlhStateComplete();
*
********************************************************************************** */
void Controller_UpdateLlhAdvInstant(uint16_t next_instant);

/*! *********************************************************************************
* \brief   Check if power gating lowpower mode is allowed by Link layer
*
********************************************************************************** */
bool_t Controller_IsPowerGatingAllowed(void);

/*! *********************************************************************************
* \brief   Initialize the XCVR module
*
* \return uint32_t , 0 if successful, error if other values.
*
********************************************************************************** */
uint32_t Controller_RadioInit(void);

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


#define BD_ADDR_SIZE 6U

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

#endif /* _CONTROLLER_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */

