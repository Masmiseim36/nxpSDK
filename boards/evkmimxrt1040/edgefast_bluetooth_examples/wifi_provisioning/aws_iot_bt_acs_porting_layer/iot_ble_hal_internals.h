/*
* FreeRTOS
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/**
 * @file aws_ble_hal_internals.h
 * @brief Internally shared functions and variable for HAL ble stack.
 */

#ifndef _AWS_BLE_INTERNALS_H_
#define _AWS_BLE_INTERNALS_H_

#define TAG    "AFR-BLE"
#define IOT_GAP_DEVICE_NAME "NXPBLE"

#include "BT_common.h"
#include "bt_hal_manager_types.h"
#include "bt_hal_manager_adapter_ble.h"
#include "bt_hal_gatt_server.h"
#include "BT_att_api.h"

typedef struct
{
    BTIOtypes_t xPropertyIO;
    bool bBondable;
    bool bSecureConnectionOnly;
    uint32_t ulMtu;
} BTProperties_t;

extern uint32_t ulGAPEvtMngHandle;
extern BTBleAdapterCallbacks_t xBTBleAdapterCallbacks;
extern uint32_t ulGattServerIFhandle;
extern BTProperties_t xProperties;
extern BTGattServerCallbacks_t xGattServerCb;
extern uint16_t usGattConnHandle;
extern uint16_t gattOffset;

const void * prvBTGetGattServerInterface();
extern const void * prvGetLeAdapter();
uint16_t prvGattFromDevHandle( uint16_t handle );

void prvSetDeviceName( char *newName, size_t length );
char * prvGetDeviceName( void );
BTStatus_t prvToggleBondableFlag( bool bEnable );
BTStatus_t prvToggleSecureConnectionOnlyMode( bool bEnable );
void prvGattGetSemaphore();
void prvGattGiveSemaphore();

void prvGapConnect(uint16_t connectHandle, BT_DEVICE_ADDR *peerAddr);
void prvGattConnect(uint16_t connectHandle, ATT_HANDLE * handle);
void prvGapDisconnect(uint16_t connectHandle);
uint16_t prvGattMqttExchangeReq(ATT_HANDLE * handle, uint16_t mtu);
void prvGattMqttExchangeRsp(ATT_HANDLE * handle, uint16_t mtu);
#endif /* ifndef _AWS_BLE_INTERNALS_H_ */
