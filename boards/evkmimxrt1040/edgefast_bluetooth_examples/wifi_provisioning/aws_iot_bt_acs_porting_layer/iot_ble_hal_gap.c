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
 * @file aws_ble_hal_gap.c
 * @brief Hardware Abstraction Layer for GAP ble stack.
 */

#include <stddef.h>
#include <string.h>
#include "FreeRTOS.h"
#include "iot_ble_hal_internals.h"
#include "BT_common.h"
#include "BT_hci_api.h"
#include "BT_device_queue.h"
#include "l2cap.h"

typedef struct _ble_gap_adv_params
{
    uint16_t   advertising_interval_min;
    uint16_t   advertising_interval_max;
    uint8_t    advertising_type;
    uint8_t    own_addr_type;
    uint8_t    peer_addr_type;
    uint8_t *  peer_addr;
    uint8_t    advertising_channel_map;
    uint8_t    advertising_filter_policy;
} ble_gap_adv_params_t;

uint8_t peer_addr[BT_BD_ADDR_SIZE];

uint8_t advertise_data[HCI_MAX_ADVERTISING_DATA_LENGTH];
uint8_t advertise_data_length;

BTBleAdapterCallbacks_t xBTBleAdapterCallbacks;
static ble_gap_adv_params_t xAdv_params;

#define IOT_BLE_ADVERTISING_DURATION_MS    ( 10 )


/** EIR/AD data type definitions */
#define BT_DATA_FLAGS                   0x01 /* AD flags */
#define BT_DATA_UUID16_SOME             0x02 /* 16-bit UUID, more available */
#define BT_DATA_UUID16_ALL              0x03 /* 16-bit UUID, all listed */
#define BT_DATA_UUID32_SOME             0x04 /* 32-bit UUID, more available */
#define BT_DATA_UUID32_ALL              0x05 /* 32-bit UUID, all listed */
#define BT_DATA_UUID128_SOME            0x06 /* 128-bit UUID, more available */
#define BT_DATA_UUID128_ALL             0x07 /* 128-bit UUID, all listed */
#define BT_DATA_NAME_SHORTENED          0x08 /* Shortened name */
#define BT_DATA_NAME_COMPLETE           0x09 /* Complete name */
#define BT_DATA_TX_POWER                0x0a /* Tx Power */
#define BT_DATA_SVC_DATA16              0x16 /* Service data, 16-bit UUID */
#define BT_DATA_GAP_APPEARANCE          0x19 /* GAP appearance */
#define BT_DATA_SVC_DATA32              0x20 /* Service data, 32-bit UUID */
#define BT_DATA_SVC_DATA128             0x21 /* Service data, 128-bit UUID */
#define BT_DATA_MANUFACTURER_DATA       0xff /* Manufacturer Specific Data */

#define BT_LE_AD_LIMITED                0x01 /* Limited Discoverable */
#define BT_LE_AD_GENERAL                0x02 /* General Discoverable */
#define BT_LE_AD_NO_BREDR               0x04 /* BR/EDR not supported */

/* Duration of advertisement. By default advertise for inifinite duration. */
#if 0
static int32_t lAdvDurationMS = BLE_HS_FOREVER;
/*fix build warning: declared but never used.*/
static bool xPrivacy;
#endif

static BTStatus_t prvBTBleAdapterInit( const BTBleAdapterCallbacks_t * pxCallbacks );
static BTStatus_t prvBTRegisterBleApp( BTUuid_t * pxAppUuid );
static BTStatus_t prvBTUnregisterBleApp( uint8_t ucAdapterIf );
static BTStatus_t prvBTGetBleAdapterProperty( BTBlePropertyType_t xType );
static BTStatus_t prvBTSetBleAdapterProperty( const BTBleProperty_t * pxProperty );
static BTStatus_t prvBTGetallBleRemoteDeviceProperties( BTBdaddr_t * pxRremoteAddr );
static BTStatus_t prvBTGetBleRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                                   BTBleProperty_t xType );
static BTStatus_t prvBTSetBleRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                                   const BTBleProperty_t * pxProperty );
static BTStatus_t prvBTScan( bool bStart );
static BTStatus_t prvBTConnect( uint8_t ucAdapterIf,
                                const BTBdaddr_t * pxBdAddr,
                                bool bIsDirect,
                                BTTransport_t ulTransport );
static BTStatus_t prvBTDisconnect( uint8_t ucAdapterIf,
                                   const BTBdaddr_t * pxBdAddr,
                                   uint16_t usConnId );
static BTStatus_t prvBTStartAdv( uint8_t ucAdapterIf );
static BTStatus_t prvBTStopAdv( uint8_t ucAdapterIf );
static BTStatus_t prvBTReadRemoteRssi( uint8_t ucAdapterIf,
                                       const BTBdaddr_t * pxBdAddr );
static BTStatus_t prvBTScanFilterParamSetup( BTGattFiltParamSetup_t xFiltParam );
static BTStatus_t prvBTScanFilterAddRemove( uint8_t ucAdapterIf,
                                            uint32_t ulAction,
                                            uint32_t ulFiltType,
                                            uint32_t ulFiltIndex,
                                            uint32_t ulCompanyId,
                                            uint32_t ulCompanyIdMask,
                                            const BTUuid_t * pxUuid,
                                            const BTUuid_t * pxUuidMask,
                                            const BTBdaddr_t * pxBdAddr,
                                            char cAddrType,
                                            size_t xDataLen,
                                            char * pcData,
                                            size_t xMaskLen,
                                            char * pcMask );
static BTStatus_t prvBTScanFilterEnable( uint8_t ucAdapterIf,
                                         bool bEnable );
static BTTransport_t prvBTGetDeviceType( const BTBdaddr_t * pxBdAddr );
static BTStatus_t prvBTSetAdvData( uint8_t ucAdapterIf,
                                   BTGattAdvertismentParams_t * pxParams,
                                   uint16_t usManufacturerLen,
                                   char * pcManufacturerData,
                                   uint16_t usServiceDataLen,
                                   char * pcServiceData,
                                   BTUuid_t * pxServiceUuid,
                                   size_t xNbServices );
static BTStatus_t prvBTSetAdvRawData( uint8_t ucAdapterIf,
                                      uint8_t * pucData,
                                      uint8_t ucLen );
static BTStatus_t prvBTConnParameterUpdateRequest( const BTBdaddr_t * pxBdAddr,
                                                   uint32_t ulMinInterval,
                                                   uint32_t ulMaxInterval,
                                                   uint32_t ulLatency,
                                                   uint32_t ulTimeout );
static BTStatus_t prvBTSetScanParameters( uint8_t ucAdapterIf,
                                          uint32_t ulScanInterval,
                                          uint32_t ulScanWindow );
static BTStatus_t prvBTMultiAdvEnable( uint8_t ucAdapterIf,
                                       BTGattAdvertismentParams_t * xAdvParams );
static BTStatus_t prvBTMultiAdvUpdate( uint8_t ucAdapterIf,
                                       BTGattAdvertismentParams_t * advParams );
static BTStatus_t prvBTMultiAdvSetInstData( uint8_t ucAdapterIf,
                                            bool bSetScanRsp,
                                            bool bIncludeName,
                                            bool bInclTxpower,
                                            uint32_t ulAppearance,
                                            size_t xManufacturerLen,
                                            char * pcManufacturerData,
                                            size_t xServiceDataLen,
                                            char * pcServiceData,
                                            BTUuid_t * pxServiceUuid,
                                            size_t xNbServices );
static BTStatus_t prvBTMultiAdvDisable( uint8_t ucAdapterIf );
static BTStatus_t prvBTBatchscanCfgStorage( uint8_t ucAdapterIf,
                                            uint32_t ulBatchScanFullMax,
                                            uint32_t ulBatchScanTruncMax,
                                            uint32_t ulBatchScanNotifyThreshold );
static BTStatus_t prvBTBatchscanEndBatchScan( uint8_t ucAdapterIf,
                                              uint32_t ulScanMode,
                                              uint32_t ulScanInterval,
                                              uint32_t ulScanWindow,
                                              uint32_t ulAddrType,
                                              uint32_t ulDiscardRule );
static BTStatus_t prvBTBatchscanDisBatchScan( uint8_t ucAdapterIf );
static BTStatus_t prvBTBatchscanReadReports( uint8_t ucAdapterIf,
                                             uint32_t ulScanMode );
static BTStatus_t prvBTSetPreferredPhy( uint16_t usConnId,
                                        uint8_t ucTxPhy,
                                        uint8_t ucRxPhy,
                                        uint16_t usPhyOptions );
static BTStatus_t prvBTReadPhy( uint16_t usConnId,
                                BTReadClientPhyCallback_t xCb );
static const void * prvBTGetGattClientInterface( void );
static BTStatus_t prvBTBleAdapterInit( const BTBleAdapterCallbacks_t * pxCallbacks );

BTBleAdapter_t xBTLeAdapter =
{
    .pxBleAdapterInit                  = prvBTBleAdapterInit,
    .pxRegisterBleApp                  = prvBTRegisterBleApp,
    .pxUnregisterBleApp                = prvBTUnregisterBleApp,
    .pxGetBleAdapterProperty           = prvBTGetBleAdapterProperty,
    .pxSetBleAdapterProperty           = prvBTSetBleAdapterProperty,
    .pxGetallBleRemoteDeviceProperties = prvBTGetallBleRemoteDeviceProperties,
    .pxGetBleRemoteDeviceProperty      = prvBTGetBleRemoteDeviceProperty,
    .pxSetBleRemoteDeviceProperty      = prvBTSetBleRemoteDeviceProperty,
    .pxScan                            = prvBTScan,
    .pxConnect                         = prvBTConnect,
    .pxDisconnect                      = prvBTDisconnect,
    .pxStartAdv                        = prvBTStartAdv,
    .pxStopAdv                         = prvBTStopAdv,
    .pxReadRemoteRssi                  = prvBTReadRemoteRssi,
    .pxScanFilterParamSetup            = prvBTScanFilterParamSetup,
    .pxScanFilterAddRemove             = prvBTScanFilterAddRemove,
    .pxScanFilterEnable                = prvBTScanFilterEnable,
    .pxGetDeviceType                   = prvBTGetDeviceType,
    .pxSetAdvData                      = prvBTSetAdvData,
    .pxSetAdvRawData                   = prvBTSetAdvRawData,
    .pxConnParameterUpdateRequest      = prvBTConnParameterUpdateRequest,
    .pxSetScanParameters               = prvBTSetScanParameters,
    .pxMultiAdvEnable                  = prvBTMultiAdvEnable,
    .pxMultiAdvUpdate                  = prvBTMultiAdvUpdate,
    .pxMultiAdvSetInstData             = prvBTMultiAdvSetInstData,
    .pxMultiAdvDisable                 = prvBTMultiAdvDisable,
    .pxBatchscanCfgStorage             = prvBTBatchscanCfgStorage,
    .pxBatchscanEndBatchScan           = prvBTBatchscanEndBatchScan,
    .pxBatchscanDisBatchScan           = prvBTBatchscanDisBatchScan,
    .pxBatchscanReadReports            = prvBTBatchscanReadReports,
    .pxSetPreferredPhy                 = prvBTSetPreferredPhy,
    .pxReadPhy                         = prvBTReadPhy,
    .ppvGetGattClientInterface         = prvBTGetGattClientInterface,
    .ppvGetGattServerInterface         = prvBTGetGattServerInterface,
};

/*-----------------------------------------------------------*/

BTStatus_t prvBTBleAdapterInit( const BTBleAdapterCallbacks_t * pxCallbacks )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    /* Prepare the BLE stack */

    if( pxCallbacks != NULL )
    {
        xBTBleAdapterCallbacks = *pxCallbacks;
    }
    else
    {
        xStatus = eBTStatusFail;
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTRegisterBleApp( BTUuid_t * pxAppUuid )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    if( xBTBleAdapterCallbacks.pxRegisterBleAdapterCb != NULL )
    {
        xBTBleAdapterCallbacks.pxRegisterBleAdapterCb( eBTStatusSuccess, 0, pxAppUuid );
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTUnregisterBleApp( uint8_t ucAdapterIf )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGetBleAdapterProperty( BTBlePropertyType_t xType )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTSetBleAdapterProperty( const BTBleProperty_t * pxProperty )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTGetallBleRemoteDeviceProperties( BTBdaddr_t * pxRremoteAddr )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTGetBleRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                            BTBleProperty_t xType )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTSetBleRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                            const BTBleProperty_t * pxProperty )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTScan( bool bStart )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTConnect( uint8_t ucAdapterIf,
                         const BTBdaddr_t * pxBdAddr,
                         bool bIsDirect,
                         BTTransport_t ulTransport )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTDisconnect( uint8_t ucAdapterIf,
                            const BTBdaddr_t * pxBdAddr,
                            uint16_t usConnId )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    API_RESULT retval = API_SUCCESS;

    retval = BT_hci_disconnect( usConnId, 0x13 );

    if( ( API_SUCCESS != retval ) )
    {
        xStatus = eBTStatusFail;
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTStartAdv( uint8_t ucAdapterIf )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    /* Set Normal Advertising Parameters */
    BT_hci_le_set_advertising_parameters
    (
        xAdv_params.advertising_interval_min,
        xAdv_params.advertising_interval_max,
        xAdv_params.advertising_type,
        xAdv_params.own_addr_type,
        xAdv_params.peer_addr_type,
        xAdv_params.peer_addr,
        xAdv_params.advertising_channel_map,
        xAdv_params.advertising_filter_policy
    );

    BT_hci_le_set_advertising_enable(1);

    if( xBTBleAdapterCallbacks.pxAdvStatusCb != NULL )
    {
        xBTBleAdapterCallbacks.pxAdvStatusCb( xStatus, 0, true );
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTStopAdv( uint8_t ucAdapterIf )
{
    BTStatus_t xStatus = eBTStatusSuccess;

    BT_hci_le_set_advertising_enable(0);

    if( xBTBleAdapterCallbacks.pxAdvStatusCb != NULL )
    {
        xBTBleAdapterCallbacks.pxAdvStatusCb( xStatus, ulGattServerIFhandle, false );
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTReadRemoteRssi( uint8_t ucAdapterIf,
                                const BTBdaddr_t * pxBdAddr )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTScanFilterParamSetup( BTGattFiltParamSetup_t xFiltParam )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTScanFilterAddRemove( uint8_t ucAdapterIf,
                                     uint32_t ulAction,
                                     uint32_t ulFiltType,
                                     uint32_t ulFiltIndex,
                                     uint32_t ulCompanyId,
                                     uint32_t ulCompanyIdMask,
                                     const BTUuid_t * pxUuid,
                                     const BTUuid_t * pxUuidMask,
                                     const BTBdaddr_t * pxBdAddr,
                                     char cAddrType,
                                     size_t xDataLen,
                                     char * pcData,
                                     size_t xMaskLen,
                                     char * pcMask )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTScanFilterEnable( uint8_t ucAdapterIf,
                                  bool bEnable )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTTransport_t prvBTGetDeviceType( const BTBdaddr_t * pxBdAddr )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return (BTTransport_t)xStatus;
}

int prvBTFillAdvElement(uint8_t type, uint8_t *data, uint8_t length, uint8_t* outBuffer, uint8_t outbufferLength, uint8_t *outBufferoffset)
{
    uint8_t index;

    if (0 == length)
    {
        return -1;
    }

    if (outbufferLength < ((*outBufferoffset) + 2 + length))
    {
        return -1;
    }

    // AD Element length
    outBuffer[(*outBufferoffset)] = length + 1;
    (*outBufferoffset)++;
    // AD Element type
    outBuffer[(*outBufferoffset)] = type;
    (*outBufferoffset)++;
    // AD Element data
    for (index = 0; index < length; index++)
    {
        outBuffer[(*outBufferoffset)] = data[index];
        (*outBufferoffset)++;
    }

    return 0;
}

/*-----------------------------------------------------------*/
BTStatus_t prvBTSetAdvData( uint8_t ucAdapterIf,
                            BTGattAdvertismentParams_t * pxParams,
                            uint16_t usManufacturerLen,
                            char * pcManufacturerData,
                            uint16_t usServiceDataLen,
                            char * pcServiceData,
                            BTUuid_t * pxServiceUuid,
                            size_t xNbServices )
{
#if 0
    /*fix build warning: declared but never used.*/
    const char * name;
#endif
    int retval;

    BTStatus_t xStatus = eBTStatusSuccess;

    /**
     *  Set the advertisement data included in our advertisements:
     *     o Flags (indicates advertisement type and other general info).
     *     o Advertising tx power.
     *     o Device name.
     *     o 16-bit service UUIDs (alert notifications).
     */

    memset( advertise_data, 0, sizeof(advertise_data) );
    advertise_data_length = 0;

    /* Advertise two flags:
     *     o Discoverability in forthcoming advertisement (general)
     *     o BLE-only (BR/EDR unsupported).
     */
    if ( !pxParams->bSetScanRsp )
    {
        uint8_t flags = BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR;
        retval = prvBTFillAdvElement(BT_DATA_FLAGS, &flags, 1, advertise_data, sizeof(advertise_data), &advertise_data_length);
        if( retval != 0 )
        {
            xStatus = eBTStatusFail;
        }
    }

    if( pxParams->ulAppearance )
    {
        uint16_t appearance = pxParams->ulAppearance;
        retval = prvBTFillAdvElement(BT_DATA_GAP_APPEARANCE, (uint8_t *)&appearance, sizeof(appearance), advertise_data, sizeof(advertise_data), &advertise_data_length);
        if( retval != 0 )
        {
            xStatus = eBTStatusFail;
        }
    }

    /* Indicate that the TX power level field should be included; have the
     * stack fill this value automatically.  This is done by assiging the
     * special value BLE_HS_ADV_TX_PWR_LVL_AUTO.
     */
    if( pxParams->bIncludeTxPower > 0 )
    {
        //TODO: Get TX Power by call function
    }

    if( pxParams->ucName.xType != BTGattAdvNameNone )
    {
        uint8_t gapNameLength = strlen( prvGetDeviceName() );
        uint8_t type = BT_DATA_NAME_COMPLETE;
        if( pxParams->ucName.xType == BTGattAdvNameShort )
        {
            if( pxParams->ucName.ucShortNameLen < strlen( prvGetDeviceName() ) )
            {
                gapNameLength = pxParams->ucName.ucShortNameLen;
                type = BT_DATA_NAME_SHORTENED;
            }
        }
        retval = prvBTFillAdvElement(type, (uint8_t *)prvGetDeviceName(), gapNameLength, advertise_data, sizeof(advertise_data), &advertise_data_length);
        if( retval != 0 )
        {
            xStatus = eBTStatusFail;
        }
    }

    if( usManufacturerLen && pcManufacturerData )
    {
        retval = prvBTFillAdvElement(BT_DATA_MANUFACTURER_DATA, (uint8_t *)pcManufacturerData, usManufacturerLen, advertise_data, sizeof(advertise_data), &advertise_data_length);
        if( retval != 0 )
        {
            xStatus = eBTStatusFail;
        }
    }

    if( ( pxParams->ulMinInterval != 0 ) && ( pxParams->ulMaxInterval != 0 ) )
    {
        xAdv_params.advertising_interval_min = pxParams->ulMinInterval;
        xAdv_params.advertising_interval_max = pxParams->ulMaxInterval;
    }

    if( usServiceDataLen && pcServiceData )
    {
        retval = prvBTFillAdvElement(BT_DATA_SVC_DATA16, (uint8_t *)pcServiceData, usServiceDataLen, advertise_data, sizeof(advertise_data), &advertise_data_length);
        if( retval != 0 )
        {
            xStatus = eBTStatusFail;
        }
    }

    if( pxServiceUuid != NULL )
    {
        uint8_t uuidData[HCI_MAX_ADVERTISING_DATA_LENGTH];
        uint8_t uuidDataOffset = 0;
        uint8_t remaining = HCI_MAX_ADVERTISING_DATA_LENGTH > (advertise_data_length + 2) ? HCI_MAX_ADVERTISING_DATA_LENGTH - (advertise_data_length + 2) : 0;
        for( size_t i = 0; i < xNbServices; i++ )
        {
            if (uuidDataOffset >= remaining)
            {
                break;
            }
            if( pxServiceUuid[ i ].ucType == eBTuuidType16 )
            {
                memcpy(&uuidData[uuidDataOffset], &pxServiceUuid->uu.uu16, sizeof(pxServiceUuid->uu.uu16));
                uuidDataOffset += sizeof(pxServiceUuid->uu.uu16);
            }
        }
        if (uuidDataOffset > 0)
        {
            retval = prvBTFillAdvElement(BT_DATA_UUID16_ALL, uuidData, uuidDataOffset, advertise_data, sizeof(advertise_data), &advertise_data_length);
            if( retval != 0 )
            {
                xStatus = eBTStatusFail;
            }
        }
        uuidDataOffset = 0;
        remaining = HCI_MAX_ADVERTISING_DATA_LENGTH > (advertise_data_length + 2) ? HCI_MAX_ADVERTISING_DATA_LENGTH - (advertise_data_length + 2) : 0;
        for( size_t i = 0; i < xNbServices; i++ )
        {
            if (uuidDataOffset >= remaining)
            {
                break;
            }
            if( pxServiceUuid[ i ].ucType == eBTuuidType32 )
            {
                memcpy(&uuidData[uuidDataOffset], &pxServiceUuid->uu.uu32, sizeof(pxServiceUuid->uu.uu32));
                uuidDataOffset += sizeof(pxServiceUuid->uu.uu32);
            }
        }
        if (uuidDataOffset > 0)
        {
            if (uuidDataOffset > HCI_MAX_ADVERTISING_DATA_LENGTH)
            {
                uuidDataOffset = HCI_MAX_ADVERTISING_DATA_LENGTH;
            }
            retval = prvBTFillAdvElement(BT_DATA_UUID32_ALL, uuidData, uuidDataOffset, advertise_data, sizeof(advertise_data), &advertise_data_length);
            if( retval != 0 )
            {
                xStatus = eBTStatusFail;
            }
        }
        uuidDataOffset = 0;
        remaining = HCI_MAX_ADVERTISING_DATA_LENGTH > (advertise_data_length + 2) ? HCI_MAX_ADVERTISING_DATA_LENGTH - (advertise_data_length + 2) : 0;
        for( size_t i = 0; i < xNbServices; i++ )
        {
            if (uuidDataOffset >= remaining)
            {
                break;
            }
            if( pxServiceUuid[ i ].ucType == eBTuuidType128 )
            {
                memcpy(&uuidData[uuidDataOffset], pxServiceUuid->uu.uu128, sizeof(pxServiceUuid->uu.uu128));
                uuidDataOffset += sizeof(pxServiceUuid->uu.uu128);
            }
        }
        if (uuidDataOffset > 0)
        {
            if (uuidDataOffset > HCI_MAX_ADVERTISING_DATA_LENGTH)
            {
                uuidDataOffset = HCI_MAX_ADVERTISING_DATA_LENGTH;
            }
            retval = prvBTFillAdvElement(BT_DATA_UUID128_ALL, uuidData, uuidDataOffset, advertise_data, sizeof(advertise_data), &advertise_data_length);
            if( retval != 0 )
            {
                xStatus = eBTStatusFail;
            }
        }
    }

    if( pxParams->usAdvertisingEventProperties == BTAdvInd )
    {
        xAdv_params.advertising_type = HCI_ADV_IND;
    }
    else if( pxParams->usAdvertisingEventProperties == BTAdvDirectInd )
    {
        xAdv_params.advertising_type = HCI_ADV_DIRECT_IND;
        /* fixme: set adv_params->high_duty_cycle accordingly */
    }
    else if( pxParams->usAdvertisingEventProperties == BTAdvNonconnInd )
    {
        xAdv_params.advertising_type = HCI_ADV_NONCONN_IND;
    }
    else
    {
        xAdv_params.advertising_type = HCI_ADV_IND;
    }

#if 0
    if( pxParams->usTimeout != 0 )
    {
        lAdvDurationMS = ( int32_t ) ( pxParams->usTimeout * IOT_BLE_ADVERTISING_DURATION_MS );
    }
    else
    {
        lAdvDurationMS = BLE_HS_FOREVER;
    }
#endif

    if( pxParams->xAddrType == BTAddrTypeResolvable )
    {
        xAdv_params.peer_addr_type = BT_BD_RANDOM_ADDRESS_TYPE;
    }
    else
    {
        xAdv_params.peer_addr_type = BT_BD_PUBLIC_ADDRESS_TYPE;
    }

    xAdv_params.advertising_channel_map = 0x07;
    xAdv_params.advertising_filter_policy = 0x00;
    xAdv_params.own_addr_type = BT_BD_PUBLIC_ADDRESS_TYPE;

    if ( !pxParams->bSetScanRsp )
    {
        /* Set Advertising Data */
        BT_hci_le_set_advertising_data(advertise_data_length, advertise_data);
    }
    else
    {
        if (advertise_data_length > 0)
        {
            BT_hci_le_set_scan_response_data(advertise_data_length, advertise_data);
        }
    }

    if( xBTBleAdapterCallbacks.pxSetAdvDataCb != NULL )
    {
        xBTBleAdapterCallbacks.pxSetAdvDataCb( xStatus );
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTSetAdvRawData( uint8_t ucAdapterIf,
                               uint8_t * pucData,
                               uint8_t ucLen )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTConnParameterUpdateRequest( const BTBdaddr_t * pxBdAddr,
                                            uint32_t ulMinInterval,
                                            uint32_t ulMaxInterval,
                                            uint32_t ulLatency,
                                            uint32_t ulTimeout )
{
    DEVICE_HANDLE handle;
    static BT_DEVICE_ADDR peerAddress;
    API_RESULT retval;
    BTStatus_t xStatus = eBTStatusFail;

    peerAddress.type = 0;
    memcpy(peerAddress.addr, &pxBdAddr->ucAddress, sizeof(peerAddress.addr));
    if (API_SUCCESS != device_queue_search_le_remote_addr(&handle, &peerAddress))
    {
        peerAddress.type = 1;
        if (API_SUCCESS != device_queue_search_le_remote_addr(&handle, &peerAddress))
        {
            return eBTStatusFail;
        }
    }

    retval = BT_l2cap_le_connection_param_update_request(&handle, ulMinInterval, ulMaxInterval, ulLatency, ulTimeout);
    if (API_SUCCESS == retval)
    {
        xStatus = eBTStatusSuccess;
    }

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTSetScanParameters( uint8_t ucAdapterIf,
                                   uint32_t ulScanInterval,
                                   uint32_t ulScanWindow )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTMultiAdvEnable( uint8_t ucAdapterIf,
                                BTGattAdvertismentParams_t * xAdvParams )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTMultiAdvUpdate( uint8_t ucAdapterIf,
                                BTGattAdvertismentParams_t * advParams )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTMultiAdvSetInstData( uint8_t ucAdapterIf,
                                     bool bSetScanRsp,
                                     bool bIncludeName,
                                     bool bInclTxpower,
                                     uint32_t ulAppearance,
                                     size_t xManufacturerLen,
                                     char * pcManufacturerData,
                                     size_t xServiceDataLen,
                                     char * pcServiceData,
                                     BTUuid_t * pxServiceUuid,
                                     size_t xNbServices )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTMultiAdvDisable( uint8_t ucAdapterIf )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTBatchscanCfgStorage( uint8_t ucAdapterIf,
                                     uint32_t ulBatchScanFullMax,
                                     uint32_t ulBatchScanTruncMax,
                                     uint32_t ulBatchScanNotifyThreshold )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTBatchscanEndBatchScan( uint8_t ucAdapterIf,
                                       uint32_t ulScanMode,
                                       uint32_t ulScanInterval,
                                       uint32_t ulScanWindow,
                                       uint32_t ulAddrType,
                                       uint32_t ulDiscardRule )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTBatchscanDisBatchScan( uint8_t ucAdapterIf )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTBatchscanReadReports( uint8_t ucAdapterIf,
                                      uint32_t ulScanMode )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTSetPreferredPhy( uint16_t usConnId,
                                 uint8_t ucTxPhy,
                                 uint8_t ucRxPhy,
                                 uint16_t usPhyOptions )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTReadPhy( uint16_t usConnId,
                         BTReadClientPhyCallback_t xCb )
{
    BTStatus_t xStatus = eBTStatusUnsupported;

    return xStatus;
}

/*-----------------------------------------------------------*/

const void * prvBTGetGattClientInterface()
{
    return NULL;
}

/*-----------------------------------------------------------*/

const void * prvGetLeAdapter()
{
    return &xBTLeAdapter;
}
