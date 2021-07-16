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
 * @file aws_ble_hal_gatt_server.c
 * @brief Hardware Abstraction Layer for GATT server ble stack.
 */

#include <string.h>
#include "FreeRTOS.h"

#include "iot_ble_config.h"

#include "bt_hal_manager.h"

#include "BT_att_api.h"
#include "BT_gatt_db_api.h"

#include "iot_ble_hal_internals.h"

#define APP_ID          0
#define MAX_SERVICES    20

typedef struct _iot_ble_gatt_services
{
    BTService_t * service;
    uint16_t serviceHandle;
} iot_ble_gatt_services_t;
static iot_ble_gatt_services_t afrServices[ MAX_SERVICES ];

static uint16_t serviceCnt = 0;

typedef struct _iot_ble_gatt_context
{
    uint16_t        connectHandle;
    ATT_HANDLE      attHandle;
    BT_DEVICE_ADDR  peerAddr;
    uint16_t        mtu;
} iot_ble_gatt_context_t;

typedef struct _iot_ble_gatt_request
{
    ATT_HANDLE    * handle;
    uint16_t        attr_handle;
    uint8_t         request_code;
    uint8_t         response_code;
} iot_ble_gatt_request_t;

iot_ble_gatt_context_t gattContext;

static SemaphoreHandle_t xSem;
static bool semInited;
static uint8_t pendingOnWaitResponse = 0;
UINT16 GATT_DB_MAX_ATTRIBUTES;

void prvGattGetSemaphore()
{
    (void)xSemaphoreTake( xSem, portMAX_DELAY );
}

void prvGattGiveSemaphore()
{
    xSemaphoreGive( xSem );
}

BTGattServerCallbacks_t xGattServerCb;
uint32_t ulGattServerIFhandle = 0;
#if 0
/*fix build warning: declared but never used.*/
static uint16_t prvCountCharacteristics( BTService_t * pxService );
static uint16_t prvCountDescriptor( BTService_t * pxService,
                                    uint16_t startHandle );
#endif
static BTStatus_t prvBTRegisterServer( BTUuid_t * pxUuid );
static BTStatus_t prvBTUnregisterServer( uint8_t ucServerIf );
static BTStatus_t prvBTGattServerInit( const BTGattServerCallbacks_t * pxCallbacks );
static BTStatus_t prvBTConnect( uint8_t ucServerIf,
                                const BTBdaddr_t * pxBdAddr,
                                bool bIsDirect,
                                BTTransport_t xTransport );
static BTStatus_t prvBTDisconnect( uint8_t ucServerIf,
                                   const BTBdaddr_t * pxBdAddr,
                                   uint16_t usConnId );
static BTStatus_t prvAddServiceBlob( uint8_t ucServerIf,
                                     BTService_t * pxService );
static BTStatus_t prvBTAddService( uint8_t ucServerIf,
                                   BTGattSrvcId_t * pxSrvcId,
                                   uint16_t usNumHandles );
static BTStatus_t prvBTAddIncludedService( uint8_t ucServerIf,
                                           uint16_t usServiceHandle,
                                           uint16_t usIncludedHandle );
static BTStatus_t prvBTAddCharacteristic( uint8_t ucServerIf,
                                          uint16_t usServiceHandle,
                                          BTUuid_t * pxUuid,
                                          BTCharProperties_t xProperties,
                                          BTCharPermissions_t xPermissions );
static BTStatus_t prvBTSetVal( BTGattResponse_t * pxValue );
static BTStatus_t prvBTAddDescriptor( uint8_t ucServerIf,
                                      uint16_t usServiceHandle,
                                      BTUuid_t * pxUuid,
                                      BTCharPermissions_t xPermissions );
static BTStatus_t prvBTStartService( uint8_t ucServerIf,
                                     uint16_t usServiceHandle,
                                     BTTransport_t xTransport );
static BTStatus_t prvBTStopService( uint8_t ucServerIf,
                                    uint16_t usServiceHandle );
static BTStatus_t prvBTDeleteService( uint8_t ucServerIf,
                                      uint16_t usServiceHandle );
static BTStatus_t prvBTSendIndication( uint8_t ucServerIf,
                                       uint16_t usAttributeHandle,
                                       uint16_t usConnId,
                                       size_t xLen,
                                       uint8_t * pucValue,
                                       bool bConfirm );
static BTStatus_t prvBTSendResponse( uint16_t usConnId,
                                     uint32_t ulTransId,
                                     BTStatus_t xStatus,
                                     BTGattResponse_t * pxResponse );

static BTStatus_t prvBTConfigureMtu( uint8_t ucServerIf,
                                     uint16_t usMtu );


void vESPBTGATTServerCleanup( void );

static BTGattServerInterface_t xGATTServerInterface =
{
    .pxRegisterServer     = prvBTRegisterServer,
    .pxUnregisterServer   = prvBTUnregisterServer,
    .pxGattServerInit     = prvBTGattServerInit,
    .pxConnect            = prvBTConnect,
    .pxDisconnect         = prvBTDisconnect,
    .pxAddServiceBlob     = prvAddServiceBlob,
    .pxAddService         = prvBTAddService,
    .pxAddIncludedService = prvBTAddIncludedService,
    .pxAddCharacteristic  = prvBTAddCharacteristic,
    .pxSetVal             = prvBTSetVal,
    .pxAddDescriptor      = prvBTAddDescriptor,
    .pxStartService       = prvBTStartService,
    .pxStopService        = prvBTStopService,
    .pxDeleteService      = prvBTDeleteService,
    .pxSendIndication     = prvBTSendIndication,
    .pxSendResponse       = prvBTSendResponse,
    .pxConfigureMtu       = prvBTConfigureMtu
};

/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/

BTStatus_t  prvBTRegisterServer( BTUuid_t * pxUuid )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    if( xGattServerCb.pxRegisterServerCb != NULL )
    {
        xGattServerCb.pxRegisterServerCb( eBTStatusSuccess, ulGattServerIFhandle, pxUuid );
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTUnregisterServer( uint8_t ucServerIf )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    if( xGattServerCb.pxUnregisterServerCb != NULL )
    {
        xGattServerCb.pxUnregisterServerCb( eBTStatusSuccess, ulGattServerIFhandle );
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGattServerInit( const BTGattServerCallbacks_t * pxCallbacks )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    if( pxCallbacks != NULL )
    {
        xGattServerCb = *pxCallbacks;
    }
    else
    {
        xStatus = eBTStatusParamInvalid;
    }

    xSem = xSemaphoreCreateBinary();

    if( xSem != NULL )
    {
        semInited = true;
    }
    else
    {
        xStatus = eBTStatusNoMem;
    }

    return xStatus;
}
/*-------------------------------------------------------------------------------------*/
void vESPBTGATTServerCleanup( void )
{
    if( semInited )
    {
        vSemaphoreDelete( xSem );
        semInited = false;
    }
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTConnect( uint8_t ucServerIf,
                         const BTBdaddr_t * pxBdAddr,
                         bool bIsDirect,
                         BTTransport_t xTransport )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTDisconnect( uint8_t ucServerIf,
                            const BTBdaddr_t * pxBdAddr,
                            uint16_t usConnId )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTAddService( uint8_t ucServerIf,
                            BTGattSrvcId_t * pxSrvcId,
                            uint16_t usNumHandles )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTAddIncludedService( uint8_t ucServerIf,
                                    uint16_t usServiceHandle,
                                    uint16_t usIncludedHandle )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTAddCharacteristic( uint8_t ucServerIf,
                                   uint16_t usServiceHandle,
                                   BTUuid_t * pxUuid,
                                   BTCharProperties_t xProperties,
                                   BTCharPermissions_t xPermissions )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTSetVal( BTGattResponse_t * pxValue )
{
    return eBTStatusUnsupported;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTAddDescriptor( uint8_t ucServerIf,
                               uint16_t usServiceHandle,
                               BTUuid_t * pxUuid,
                               BTCharPermissions_t xPermissions )
{
    return eBTStatusUnsupported;
}


BTStatus_t prvBTStartService( uint8_t ucServerIf,
                              uint16_t usServiceHandle,
                              BTTransport_t xTransport )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTStopService( uint8_t ucServerIf,
                             uint16_t usServiceHandle )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    /* It is not supported to stop a GATT service, so we just return success.
     */
    if( xGattServerCb.pxServiceStoppedCb != NULL )
    {
        xGattServerCb.pxServiceStoppedCb( eBTStatusSuccess, ucServerIf, usServiceHandle );
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTDeleteService( uint8_t ucServerIf,
                               uint16_t usServiceHandle )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    /* @TODO: need to be implemented. However, stop is not supported.
     */
    if( xGattServerCb.pxServiceDeletedCb != NULL )
    {
        xGattServerCb.pxServiceDeletedCb( eBTStatusSuccess, ucServerIf, usServiceHandle );
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTSendIndication( uint8_t ucServerIf,
                                uint16_t usAttributeHandle,
                                uint16_t usConnId,
                                size_t xLen,
                                uint8_t * pucValue,
                                bool bConfirm )
{
    ATT_HANDLE_VALUE_PAIR hndl_val_param;
    BTStatus_t xStatus = eBTStatusSuccess;
    API_RESULT            retval = API_FAILURE;

    hndl_val_param.handle =  usAttributeHandle;
    hndl_val_param.value.val = pucValue;
    hndl_val_param.value.len = xLen;

    if( bConfirm )
    {
#ifdef ATT_HNDL_VAL_INDICATION_SUPPORT
        retval = BT_att_send_hndl_val_ind( &gattContext.attHandle, &hndl_val_param );
#endif
    }
    else
    {
#ifdef ATT_HNDL_VAL_NOTIFICATION_SUPPORT
        retval = BT_att_send_hndl_val_ntf( &gattContext.attHandle, &hndl_val_param );
#endif
    }

    if( API_SUCCESS != retval )
    {
        xStatus = eBTStatusFail;
    }

    return xStatus;
}

/*-----------------------------------------------------------*/
static bool prvValidGattRequest()
{
    if( pendingOnWaitResponse )
    {
        return true;
    }

    return false;
}

static bool vSetGattRequest(uint8_t status)
{
    portENABLE_INTERRUPTS();
    pendingOnWaitResponse = status;
    portDISABLE_INTERRUPTS();
    return true;
}

int prvBTGattReadRequest(uint16_t usConnId, uint32_t ulTransId, uint16_t handle, uint16_t offset, uint8_t request_code, uint8_t response_code)
{
    BT_DEVICE_ADDR addr;
    ATT_HANDLE *att_handle = (ATT_HANDLE*)ulTransId;
    iot_ble_gatt_request_t request;

    if (API_SUCCESS == device_queue_get_remote_addr(&att_handle->device_id, &addr))
    {
        if( xGattServerCb.pxRequestReadCb != NULL )
        {
            vSetGattRequest(1);
            request.handle = att_handle;
            request.attr_handle = handle;
            request.request_code = request_code;
            request.response_code = response_code;
            xGattServerCb.pxRequestReadCb( usConnId, ( uint32_t ) &request, ( BTBdaddr_t * ) addr.addr, handle, offset );
            prvGattGetSemaphore();
            vSetGattRequest(0);
            return 0;
        }
    }
    return -1;
}

int prvBTGattWriteRequest(uint16_t usConnId, uint32_t ulTransId, uint16_t handle, uint8_t *buffer, uint32_t length, uint16_t offset, uint8_t request_code, uint8_t response_code)
{
    BT_DEVICE_ADDR addr;
    ATT_HANDLE *att_handle = (ATT_HANDLE*)ulTransId;
    iot_ble_gatt_request_t request;

    if (API_SUCCESS == device_queue_get_remote_addr(&att_handle->device_id, &addr))
    {
        if( xGattServerCb.pxRequestWriteCb != NULL )
        {
            bool rsp = false;
            if (response_code > 0)
            {
                rsp = true;
                vSetGattRequest(1);
            }
            request.handle = att_handle;
            request.attr_handle = handle;
            request.request_code = request_code;
            request.response_code = response_code;
            xGattServerCb.pxRequestWriteCb( usConnId, ( uint32_t ) &request, ( BTBdaddr_t * ) addr.addr, handle, offset, length, rsp, 0, buffer );
            if (rsp)
            {
                prvGattGetSemaphore();
                vSetGattRequest(0);
            }
            return 0;
        }

    }
    return -1;
}

BTStatus_t prvBTSendResponse( uint16_t usConnId,
                              uint32_t ulTransId,
                              BTStatus_t xStatus,
                              BTGattResponse_t * pxResponse )
{
    BTStatus_t xReturnStatus = eBTStatusSuccess;
    iot_ble_gatt_request_t* request = (iot_ble_gatt_request_t *)ulTransId;
    ATT_VALUE att_value;

    if( prvValidGattRequest() )
    {
        if( request && (
            ( request->request_code == ATT_READ_REQ ) ||
            ( request->request_code == ATT_READ_BLOB_REQ ) ||
            ( request->request_code == ATT_WRITE_REQ )
            ) )
        {
            att_value.val = pxResponse->xAttrValue.pucValue;
            att_value.len = pxResponse->xAttrValue.xLen;
            att_value.actual_len = pxResponse->xAttrValue.xLen;
            BT_att_send_pdu(request->handle, request->response_code, (void *)(&att_value));
        }

        prvGattGiveSemaphore();
    }
    else
    {
        xStatus = eBTStatusFail;
    }

    if( xGattServerCb.pxResponseConfirmationCb != NULL )
    {
        xGattServerCb.pxResponseConfirmationCb( xStatus, pxResponse->usHandle );
    }

    return xReturnStatus;
}

/*-----------------------------------------------------------*/
#if 0
/*fix build warning: declared but never used.*/
static uint16_t prvCountCharacteristics( BTService_t * pxService )
{
    uint16_t nbCharacteristics = 0;
    uint16_t index;

    for( index = 0; index < pxService->xNumberOfAttributes; index++ )
    {
        if( pxService->pxBLEAttributes[ index ].xAttributeType == eBTDbCharacteristic )
        {
            nbCharacteristics++;
        }
    }

    return nbCharacteristics;
}

static uint16_t prvCountDescriptor( BTService_t * pxService,
                                    uint16_t startHandle )
{
    uint16_t nbDescriptor = 0;
    uint16_t index;

    for( index = startHandle; index < pxService->xNumberOfAttributes; index++ )
    {
        if( pxService->pxBLEAttributes[ index ].xAttributeType == eBTDbDescriptor )
        {
            nbDescriptor++;
        }
        else if( pxService->pxBLEAttributes[ index ].xAttributeType == eBTDbCharacteristic )
        {
            break;
        }
    }

    return nbDescriptor;
}
#endif

BTStatus_t prvFillUuid(BTUuid_t * uuidSrc, GATT_DB_UUID_TYPE * uuidDst)
{
    BTStatus_t ret = eBTStatusSuccess;
    if (eBTuuidType16 == uuidSrc->ucType)
    {
        uuidDst->uuid_format = ATT_16_BIT_UUID_FORMAT;
        uuidDst->uuid.uuid_16 = uuidSrc->uu.uu16;
    }
    else if (eBTuuidType128 == uuidSrc->ucType)
    {
        uuidDst->uuid_format = ATT_128_BIT_UUID_FORMAT;
        memcpy(uuidDst->uuid.uuid_128.value, uuidSrc->uu.uu128, ATT_128_BIT_UUID_SIZE);
    }
    else
    {
        ret = eBTStatusFail;
    }

    return ret;
}
/* @brief Simple function that creates a full service in one go.
 * The function is big because of error checking but in reality it only does the following:
 * 1. Populate service fields
 * 2. Populate each attributes by looping into pxService.
 * An array for service is available in static memory, however, UUIDs and attributes have to be dynamically allocated.
 * This structure is a compromise between full dynamic allocation and static allocation.
 */
BTStatus_t prvAddServiceBlob( uint8_t ucServerIf,
                              BTService_t * pxService )
{
    uint16_t index;
    uint16_t handle = 0;
    uint16_t attCount = 0;
    uint16_t service_handle = 0;
    BTStatus_t xReturnStatus = eBTStatusSuccess;
    GATT_DB_SERVICE_INFO service_info;

    if( ( NULL == pxService ) || ( pxService->xNumberOfAttributes == 0 ) )
    {
        configPRINTF( ( "Invalid parameters\n" ) );
        xReturnStatus = eBTStatusFail;
    }

    if( eBTStatusSuccess == xReturnStatus )
    {
        if( serviceCnt < MAX_SERVICES )
        {
            afrServices[ serviceCnt ].service = pxService;
        }
        else
        {
            xReturnStatus = eBTStatusNoMem;
        }
    }

    if ( eBTStatusSuccess == xReturnStatus )
    {
        GATT_DB_UUID_TYPE      char_uuid;
        uint16_t               perm;
        uint16_t               property;
        ATT_VALUE              char_value;
        uint16_t               char_handle;

        for( index = 0; index < pxService->xNumberOfAttributes; index++ )
        {
            switch( pxService->pxBLEAttributes[ index ].xAttributeType )
            {
                case eBTDbPrimaryService:
                    if (serviceCnt > 0)
                    {
                        uint16_t preAttCount = afrServices[ serviceCnt - 1 ].service->xNumberOfAttributes;
                        handle = afrServices[ serviceCnt - 1 ].service->pusHandlesBuffer[ preAttCount - 1 ] + 1;
                    }
                    else
                    {
                        handle = 1;
                    }

                    attCount = pxService->xNumberOfAttributes;
                    pxService->pusHandlesBuffer[index] = handle;

                    service_info.link_req = (GATT_DB_SERVICE_DESC)GATT_DB_SER_SUPPORT_ANY_LINK_TYPE;
                    service_info.sec_req = (GATT_DB_SERVICE_DESC)GATT_DB_SER_NO_SECURITY_PROPERTY;

                    if( eBTDbPrimaryService == pxService->pxBLEAttributes[ index ].xAttributeType )
                    {
                        service_info.is_primary = BT_TRUE;
                        xReturnStatus = prvFillUuid(&pxService->pxBLEAttributes[ index ].xServiceUUID, &service_info.uuid);
                    }
                    else if( pxService->pxBLEAttributes[ index ].xAttributeType == eBTDbSecondaryService )
                    {
                        service_info.is_primary = BT_FALSE;
                        xReturnStatus = prvFillUuid(&pxService->pxBLEAttributes[ index ].xServiceUUID, &service_info.uuid);
                    }
                    else
                    {
                        configPRINTF( ( "Invalid service type \n" ) );
                        xReturnStatus = eBTStatusFail;
                    }

                    if( eBTStatusSuccess == xReturnStatus )
                    {
                        if (API_SUCCESS != BT_gatt_db_add_service
                             (
                                 &service_info,
                                 attCount,
                                 &service_handle
                             ))
                        {
                            xReturnStatus = eBTStatusFail;
                        }
                        else
                        {
                            afrServices[ serviceCnt ].serviceHandle = service_handle;
                        }
                    }
                    break;
                case eBTDbCharacteristic:
                    xReturnStatus = prvFillUuid(&pxService->pxBLEAttributes[ index ].xCharacteristic.xUuid, &char_uuid);
                    if ( eBTStatusSuccess == xReturnStatus )
                    {
                        perm = pxService->pxBLEAttributes[ index ].xCharacteristic.xPermissions;
                        property = pxService->pxBLEAttributes[ index ].xCharacteristic.xProperties;
                        char_value.val = NULL;
                        char_value.len = 0;
                        char_value.actual_len = char_value.len;

                        if (API_SUCCESS != BT_gatt_db_add_characteristic
                        (
                            service_handle,
                            &char_uuid,
                            perm,
                            property,
                            &char_value,
                            &char_handle
                        ))
                        {
                            xReturnStatus = eBTStatusFail;
                        }
                        else
                        {
                            handle += 2;
                            pxService->pusHandlesBuffer[ index ] = handle;
                        }
                    }
                    break;

                case eBTDbIncludedService:
                    break;

                case eBTDbDescriptor:
                    xReturnStatus = prvFillUuid(&pxService->pxBLEAttributes[ index ].xCharacteristicDescr.xUuid, &char_uuid);
                    if ( eBTStatusSuccess == xReturnStatus )
                    {
                        perm = pxService->pxBLEAttributes[ index ].xCharacteristic.xPermissions;
                        if ((ATT_16_BIT_UUID_FORMAT == char_uuid.uuid_format) && (GATT_CLIENT_CONFIG == char_uuid.uuid.uuid_16))
                        {
                            UCHAR    cccd_value[2] = { 0x00, 0x00 };
                            char_value.val = cccd_value;
                            char_value.len = sizeof(cccd_value);
                            char_value.actual_len = char_value.len;
                        }
                        else
                        {
                            char_value.val = NULL;
                            char_value.len = 0;
                            char_value.actual_len = char_value.len;
                        }

                        if (API_SUCCESS != BT_gatt_db_add_characteristic_descriptor
                        (
                            service_handle,
                            handle,
                            &char_uuid,
                            perm,
                            &char_value
                        ))
                        {
                            xReturnStatus = eBTStatusFail;
                        }
                        else
                        {
                            handle++;
                            pxService->pusHandlesBuffer[ index ] = handle;
                        }
                    }
                    break;
                default:
                    break;
            }
            /* This loop contains many dynamic allocation. If one fails, clean up and exit. */
            if( xReturnStatus != eBTStatusSuccess )
            {
                configPRINTF( ( "Failed to allocate memory during Attribute creation\n" ) );
                break;
            }
        }
        if (API_SUCCESS != BT_gatt_db_dyn_register())
        {
            xReturnStatus = eBTStatusFail;
        }
        else
        {
            serviceCnt++;
            GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
        }
    }
    return xReturnStatus;
}

static BTStatus_t prvBTConfigureMtu( uint8_t ucServerIf,
                                     uint16_t usMtu )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    API_RESULT retval;

    /* Request */
    retval = BT_att_send_mtu_xcnhg_req( &gattContext.attHandle, &usMtu );

    if( API_SUCCESS != retval )
    {
        xStatus = eBTStatusFail;
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

void prvGapConnect(uint16_t connectHandle, BT_DEVICE_ADDR *peerAddr)
{
    gattContext.connectHandle = connectHandle;
    memcpy(&gattContext.peerAddr, peerAddr, sizeof(gattContext.peerAddr));
    if( xGattServerCb.pxConnectionCb != NULL )
    {
        xGattServerCb.pxConnectionCb( connectHandle, 0, true, ( BTBdaddr_t * ) gattContext.peerAddr.addr );
    }
}

void prvGattConnect(uint16_t connectHandle, ATT_HANDLE * handle)
{
    if (0 == gattContext.connectHandle)
    {
        gattContext.attHandle.att_id = handle->att_id;
        gattContext.attHandle.device_id = handle->device_id;
    }
}

void prvGapDisconnect(uint16_t connectHandle)
{
    if (gattContext.connectHandle == connectHandle)
    {
        if( xGattServerCb.pxConnectionCb != NULL )
        {
            xGattServerCb.pxConnectionCb( connectHandle, 0, false, ( BTBdaddr_t * ) gattContext.peerAddr.addr );
        }
        gattContext.connectHandle = 0;
    }
}

uint16_t prvGattMqttExchangeReq(ATT_HANDLE * handle, uint16_t mtu)
{
    gattContext.mtu = mtu;
    if( xGattServerCb.pxMtuChangedCb != NULL )
    {
        xGattServerCb.pxMtuChangedCb( gattContext.connectHandle, mtu );
    }
    return xProperties.ulMtu;//prvBTConfigureMtu(0, IOT_BLE_HAL_GATT_MTU);
}

void prvGattMqttExchangeRsp(ATT_HANDLE * handle, uint16_t mtu)
{
    gattContext.mtu = mtu;
    if( xGattServerCb.pxMtuChangedCb != NULL )
    {
        xGattServerCb.pxMtuChangedCb( gattContext.connectHandle, mtu );
    }
}
/*-----------------------------------------------------------*/

const void * prvBTGetGattServerInterface()
{
    return &xGATTServerInterface;
}
