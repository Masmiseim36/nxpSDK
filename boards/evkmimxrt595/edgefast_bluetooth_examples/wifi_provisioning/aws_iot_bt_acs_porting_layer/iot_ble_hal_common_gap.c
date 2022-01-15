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
 * @file aws_ble_hal_common_gap.c
 * @brief Hardware Abstraction Layer for GAP ble stack.
 */

#include <string.h>
#include "FreeRTOS.h"
#include "iot_ble_hal_internals.h"
#include "iot_wifi.h"

#ifdef BT_SUPPORT_STACK_VERSION_INFO
#include "BT_version.h"
#endif /* BT_SUPPORT_STACK_VERSION_INFO */

#include "appl_hci.h"
#include "smp_pl.h"
#include "BT_sm_api.h"

#include "controller.h"

/* Configure logs for the functions in this file. */
#ifdef IOT_LOG_LEVEL_GLOBAL
    #define LIBRARY_LOG_LEVEL    IOT_LOG_LEVEL_GLOBAL
#else
    #define LIBRARY_LOG_LEVEL    IOT_LOG_INFO
#endif

#define LIBRARY_LOG_NAME         ( "IOT_BLE_HAL_COMMON_GAP" )
#include "iot_logging_setup.h"

BTProperties_t xProperties;
static BTCallbacks_t xBTCallbacks;
static char bleName[16];

BTStatus_t prvBTManagerInit( const BTCallbacks_t * pxCallbacks );
BTStatus_t prvBtManagerCleanup( void );
BTStatus_t prvBTEnable( uint8_t ucGuestMode );
BTStatus_t prvBTDisable();
BTStatus_t prvBTGetAllDeviceProperties();
BTStatus_t prvBTGetDeviceProperty( BTPropertyType_t xType );
BTStatus_t prvBTSetDeviceProperty( const BTProperty_t * pxProperty );
BTStatus_t prvBTGetAllRemoteDeviceProperties( BTBdaddr_t * pxRemoteAddr );
BTStatus_t prvBTGetRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                         BTPropertyType_t type );
BTStatus_t prvBTSetRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                         const BTProperty_t * property );
BTStatus_t prvBTPair( const BTBdaddr_t * pxBdAddr,
                      BTTransport_t xTransport,
                      bool bCreateBond );
BTStatus_t prvBTCreateBondOutOfBand( const BTBdaddr_t * pxBdAddr,
                                     BTTransport_t xTransport,
                                     const BTOutOfBandData_t * pxOobData );
BTStatus_t prvBTCancelBond( const BTBdaddr_t * pxBdAddr );
BTStatus_t prvBTRemoveBond( const BTBdaddr_t * pxBdAddr );
BTStatus_t prvBTGetConnectionState( const BTBdaddr_t * pxBdAddr,
                                    bool * bConnectionState );
BTStatus_t prvBTPinReply( const BTBdaddr_t * pxBdAddr,
                          uint8_t ucAccept,
                          uint8_t ucPinLen,
                          BTPinCode_t * pxPinCode );
BTStatus_t prvBTSspReply( const BTBdaddr_t * pxBdAddr,
                          BTSspVariant_t xVariant,
                          uint8_t ucAccept,
                          uint32_t ulPasskey );
BTStatus_t prvBTReadEnergyInfo();
BTStatus_t prvBTDutModeConfigure( bool bEnable );
BTStatus_t prvBTDutModeSend( uint16_t usOpcode,
                             uint8_t * pucBuf,
                             size_t xLen );
BTStatus_t prvBTLeTestMode( uint16_t usOpcode,
                            uint8_t * pucBuf,
                            size_t xLen );
BTStatus_t prvBTConfigHCISnoopLog( bool bEnable );
BTStatus_t prvBTConfigClear();
BTStatus_t prvBTReadRssi( const BTBdaddr_t * pxBdAddr );
BTStatus_t prvBTGetTxpower( const BTBdaddr_t * pxBdAddr,
                            BTTransport_t xTransport );
const void * prvGetClassicAdapter();
const void * prvGetLeAdapter();

static BTInterface_t xBTinterface =
{
    .pxBtManagerInit                = prvBTManagerInit,
    .pxBtManagerCleanup             = prvBtManagerCleanup,
    .pxEnable                       = prvBTEnable,
    .pxDisable                      = prvBTDisable,
    .pxGetAllDeviceProperties       = prvBTGetAllDeviceProperties,
    .pxGetDeviceProperty            = prvBTGetDeviceProperty,
    .pxSetDeviceProperty            = prvBTSetDeviceProperty,
    .pxGetAllRemoteDeviceProperties = prvBTGetAllRemoteDeviceProperties,
    .pxGetRemoteDeviceProperty      = prvBTGetRemoteDeviceProperty,
    .pxSetRemoteDeviceProperty      = prvBTSetRemoteDeviceProperty,
    .pxPair                         = prvBTPair,
    .pxCreateBondOutOfBand          = prvBTCreateBondOutOfBand,
    .pxCancelBond                   = prvBTCancelBond,
    .pxRemoveBond                   = prvBTRemoveBond,
    .pxGetConnectionState           = prvBTGetConnectionState,
    .pxPinReply                     = prvBTPinReply,
    .pxSspReply                     = prvBTSspReply,
    .pxReadEnergyInfo               = prvBTReadEnergyInfo,
    .pxDutModeConfigure             = prvBTDutModeConfigure,
    .pxDutModeSend                  = prvBTDutModeSend,
    .pxLeTestMode                   = prvBTLeTestMode,
    .pxConfigHCISnoopLog            = prvBTConfigHCISnoopLog,
    .pxConfigClear                  = prvBTConfigClear,
    .pxReadRssi                     = prvBTReadRssi,
    .pxGetTxpower                   = prvBTGetTxpower,
    .pxGetClassicAdapter            = prvGetClassicAdapter,
    .pxGetLeAdapter                 = prvGetLeAdapter,
};

const UCHAR s_smpIoCapMatrix[] =
{
    SMP_IO_CAPABILITY_NO_INPUT_NO_OUTPUT, /* eBTIONone */
    SMP_IO_CAPABILITY_DISPLAY_ONLY,       /* eBTIODisplayOnly */
    SMP_IO_CAPABILITY_DISPLAY_YESNO,      /* eBTIODisplayYesNo */
    SMP_IO_CAPABILITY_KEYBOARD_ONLY,      /* eBTIOKeyboardOnly */
    SMP_IO_CAPABILITY_KEYBOARD_DISPLAY    /* eBTIOKeyboardDisplay */
};

/*-----------------------------------------------------------*/
BTStatus_t prvBTManagerInit( const BTCallbacks_t * pxCallbacks )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    int xRet;

    WIFI_On();

    controller_init();

    /* Initialize OSAL */
    EM_os_init();
    EM_debug_init();
    EM_timer_init();
    timer_em_init();

    extern int appl_init(void);    /*fix build warning: function declared implicitly.*/
    appl_init();

    BT_ethermind_init();

#ifdef IOT_BLE_SNOOP_ENABLE
    BT_snoop_logging(1);
#endif

    xRet = 0;//Initialize the Bluetooth Stack

    if( xRet == 0 )
    {
        if( pxCallbacks != NULL )
        {
            xBTCallbacks = *pxCallbacks;
        }
        else
        {
            xStatus = eBTStatusFail;
        }
    }
    /*
    else
    {
        xStatus = eBTStatusFail;
    }*/

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBtManagerCleanup()
{
    BTStatus_t xStatus = eBTStatusUnsupported;
    return xStatus;
}


API_RESULT appl_bluetooth_on_complete ( void )
{
#ifdef BT_SUPPORT_STACK_VERSION_INFO
    BT_VERSION_NUMBER version;

    BT_get_version_number (&version);
#endif /* BT_SUPPORT_STACK_VERSION_INFO */

    IotLogInfo("Bluetooth ON Initialization Completed.");

    IotLogInfo("Stack Version - %03d.%03d.%03d.", version.major, version.minor, version.subminor);

#if 0
#ifdef CLASSIC_SEC_MANAGER
    /* Register with BR/EDR Security Module */
    BT_sm_register_user_interface (appl_sm_ui_notify_cb);
#endif /* CLASSIC_SEC_MANAGER */

    /* Set default link policy to support role switch and sniff mode */
    BT_hci_write_default_link_policy_settings(0x05);

    /* Set default scan enable - Disc and Conn */
    BT_hci_write_scan_enable(0x03);
#endif

#if 0
    /* Activate Multiprofile Record */
    appl_mps_activate_record ();
#endif

#ifdef BT_LE
    /* Register LE support in Host with controller */
    BT_hci_write_le_host_support(0x01, 0x00);

    extern void appl_init_complete(void);    /*fix build warning: function declared implicitly.*/
    appl_init_complete();
#endif /* BT_LE */

    /** If status is ok and callback is set, trigger the callback.
     *  If status is fail, not need to trig a callback as original call failed.
     **/
    if( ( xBTCallbacks.pxDeviceStateChangedCb != NULL ) )
    {
        xBTCallbacks.pxDeviceStateChangedCb( eBTstateOn );
    }

    return API_SUCCESS;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTEnable( uint8_t ucGuestMode )
{
    BTStatus_t xStatus = eBTStatusFail;

    API_RESULT ret;

    prvSetDeviceName(IOT_GAP_DEVICE_NAME, strlen(IOT_GAP_DEVICE_NAME));

    ret = BT_bluetooth_on
             (
                 appl_hci_event_indication_callback,
                 appl_bluetooth_on_complete,
                 (CHAR *)prvGetDeviceName()
             );
    if (API_SUCCESS == ret)
    {
        xStatus = eBTStatusSuccess;
    }
    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTDisable()
{
    BTStatus_t xStatus = eBTStatusFail;

    API_RESULT ret = BT_bluetooth_off();

    if (API_SUCCESS == ret)
    {
        xStatus = eBTStatusSuccess;
    }

    /** If status is ok and callback is set, trigger the callback.
     *  If status is fail, no need to trig a callback as original call failed.
     **/
    if( ( xStatus == eBTStatusSuccess ) && ( xBTCallbacks.pxDeviceStateChangedCb != NULL ) )
    {
        xBTCallbacks.pxDeviceStateChangedCb( eBTstateOff );
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGetAllDeviceProperties()
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/
BTStatus_t prvGetBondableDeviceList( void )
{
    return eBTStatusUnsupported;
}


BTStatus_t prvBTGetDeviceProperty( BTPropertyType_t xType )
{
    const char * name = NULL;
    BTStatus_t xStatus = eBTStatusSuccess;
    BTProperty_t xReturnedProperty;

    if( xBTCallbacks.pxAdapterPropertiesCb != NULL )
    {
        xReturnedProperty.xType = xType;

        switch( xType )
        {
            case eBTpropertyAdapterBondedDevices:
                xStatus = prvGetBondableDeviceList();
                break;

            case eBTpropertyBdname:
                xStatus = eBTStatusUnsupported;
                name = prvGetDeviceName();

                if( name )
                {
                    xReturnedProperty.xLen = strlen( name );
                    xReturnedProperty.xType = eBTpropertyBdname;
                    xReturnedProperty.pvVal = ( void * ) name;
                }
                else
                {
                    xStatus = eBTStatusFail;
                }

                xBTCallbacks.pxAdapterPropertiesCb( xStatus, 1, &xReturnedProperty );
                break;

            case eBTpropertyBdaddr:
                xStatus = eBTStatusUnsupported;
                break;

            case eBTpropertyTypeOfDevice:
                xStatus = eBTStatusUnsupported;
                break;

            case eBTpropertyLocalMTUSize:
                xStatus = eBTStatusUnsupported;
                break;

            case eBTpropertyBondable:
                xReturnedProperty.xLen = sizeof( bool );
                xReturnedProperty.xType = eBTpropertyBondable;
                xReturnedProperty.pvVal = ( void * ) &xProperties.bBondable;

                xBTCallbacks.pxAdapterPropertiesCb( eBTStatusSuccess, 1, &xReturnedProperty );
                break;

            case eBTpropertyIO:
                xReturnedProperty.xLen = sizeof( BTIOtypes_t );
                xReturnedProperty.xType = eBTpropertyIO;
                xReturnedProperty.pvVal = ( void * ) &xProperties.xPropertyIO;

                xBTCallbacks.pxAdapterPropertiesCb( eBTStatusSuccess, 1, &xReturnedProperty );
                break;

            case eBTpropertySecureConnectionOnly:
                xReturnedProperty.xLen = sizeof( bool );
                xReturnedProperty.xType = eBTpropertySecureConnectionOnly;
                xReturnedProperty.pvVal = ( void * ) &xProperties.bSecureConnectionOnly;

                xBTCallbacks.pxAdapterPropertiesCb( eBTStatusSuccess, 1, &xReturnedProperty );
                break;

            default:
                xStatus = eBTStatusUnsupported;
        }
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTSetDeviceProperty( const BTProperty_t * pxProperty )
{
    BTStatus_t xStatus = eBTStatusSuccess;
    API_RESULT retval;

    switch( pxProperty->xType )
    {
        case eBTpropertyBdname:

            prvSetDeviceName(pxProperty->pvVal, pxProperty->xLen);
            //xESPStatus = ble_svc_gap_device_name_set(pcName);
            retval = BT_hci_change_local_name ((UCHAR *) prvGetDeviceName(), strlen(prvGetDeviceName()));
            if (API_SUCCESS == retval)
            {
                xStatus = eBTStatusSuccess;
                /* TODO: Add code to check the status of command HCI_CHANGE_LOCAL_NAME_OPCODE */
            }
            else
            {
                xStatus = eBTStatusFail;
            }

            if( ( xBTCallbacks.pxAdapterPropertiesCb != NULL ) && ( xStatus == eBTStatusSuccess ) )
            {
                xBTCallbacks.pxAdapterPropertiesCb( xStatus, 1, ( BTProperty_t * ) pxProperty );
            }

            break;

        case eBTpropertyBdaddr:
            xStatus = eBTStatusUnsupported;
            break;

        case eBTpropertyTypeOfDevice:
            xStatus = eBTStatusUnsupported;
            break;

        case eBTpropertyLocalMTUSize:
            xProperties.ulMtu = *( ( uint32_t * ) pxProperty->pvVal );
            //xESPStatus = ble_att_set_preferred_mtu( xProperties.ulMtu );
            xStatus = eBTStatusSuccess;
#if 0
            if( xStatus != 0 )
            {
                xStatus = eBTStatusFail;
            }
#endif
            if( ( xBTCallbacks.pxAdapterPropertiesCb != NULL ) && ( xStatus == eBTStatusSuccess ) )
            {
                xBTCallbacks.pxAdapterPropertiesCb( xStatus, 1, ( BTProperty_t * ) pxProperty );
            }

            break;

        case eBTpropertyBondable:

            xProperties.bBondable = *( ( bool * ) pxProperty->pvVal ); /* update flag */

            //xStatus = prvToggleBondableFlag( xProperties.bBondable );
            retval = BT_sm_set_pairable( (xProperties.bBondable > 0) ? SM_PAIRABLE_AND_BONDABLE : SM_NON_PAIRABLE);
            if (API_SUCCESS == retval)
            {
                xStatus = eBTStatusSuccess;
            }
            else
            {
                xStatus = eBTStatusFail;
            }
            if( ( xBTCallbacks.pxAdapterPropertiesCb != NULL ) && ( xStatus == eBTStatusSuccess ) )
            {
                xBTCallbacks.pxAdapterPropertiesCb( xStatus, 1, ( BTProperty_t * ) pxProperty );
            }

            break;

        case eBTpropertyIO:

            xProperties.xPropertyIO = *( ( BTIOtypes_t * ) pxProperty->pvVal );
            //xStatus = prvSetIOs( xProperties.xPropertyIO );
            retval = BT_smp_set_io_cap_pl(s_smpIoCapMatrix[xProperties.xPropertyIO]);
            if (API_SUCCESS == retval)
            {
                xStatus = eBTStatusSuccess;
            }
            else
            {
                xStatus = eBTStatusFail;
            }

            if( ( xBTCallbacks.pxAdapterPropertiesCb != NULL ) && ( xStatus == eBTStatusSuccess ) )
            {
                xBTCallbacks.pxAdapterPropertiesCb( xStatus, 1, ( BTProperty_t * ) pxProperty );
            }

            break;

        case eBTpropertySecureConnectionOnly:
            xProperties.bSecureConnectionOnly = *( ( bool * ) pxProperty->pvVal ); /* update flag */
            //xStatus = prvToggleSecureConnectionOnlyMode( xProperties.bSecureConnectionOnly );
            retval = BT_sm_set_secure_connections_only_mode
                     (
                      (UCHAR)((xProperties.bSecureConnectionOnly > 0)? 1: 0)
                     );

            if (API_SUCCESS == retval)
            {
                xStatus = eBTStatusSuccess;
            }
            else
            {
                xStatus = eBTStatusFail;
            }

            if( ( xBTCallbacks.pxAdapterPropertiesCb != NULL ) && ( xStatus == eBTStatusSuccess ) )
            {
                xBTCallbacks.pxAdapterPropertiesCb( xStatus, 1, ( BTProperty_t * ) pxProperty );
            }

            break;

        default:
            xStatus = eBTStatusUnsupported;
    }

    return xStatus;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGetAllRemoteDeviceProperties( BTBdaddr_t * pxRemoteAddr )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGetRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                         BTPropertyType_t type )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTSetRemoteDeviceProperty( BTBdaddr_t * pxRemoteAddr,
                                         const BTProperty_t * property )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTPair( const BTBdaddr_t * pxBdAddr,
                      BTTransport_t xTransport,
                      bool bCreateBond )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTCreateBondOutOfBand( const BTBdaddr_t * pxBdAddr,
                                     BTTransport_t xTransport,
                                     const BTOutOfBandData_t * pxOobData )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTCancelBond( const BTBdaddr_t * pxBdAddr )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTRemoveBond( const BTBdaddr_t * pxBdAddr )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGetConnectionState( const BTBdaddr_t * pxBdAddr,
                                    bool * bConnectionState )
{
    return eBTStatusSuccess;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTPinReply( const BTBdaddr_t * pxBdAddr,
                          uint8_t ucAccept,
                          uint8_t ucPinLen,
                          BTPinCode_t * pxPinCode )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTSspReply( const BTBdaddr_t * pxBdAddr,
                          BTSspVariant_t xVariant,
                          uint8_t ucAccept,
                          uint32_t ulPasskey )
{
#if 0
    int xESPStatus = eBTStatusSuccess;
    BTStatus_t xStatus = eBTStatusSuccess;

    switch( xVariant )
    {
        case eBTsspVariantPasskeyConfirmation:
            pkey.action = BLE_SM_IOACT_NUMCMP;
            pkey.numcmp_accept = ucAccept;
            xESPStatus = ble_sm_inject_io( usGattConnHandle, &pkey );
            break;

        case eBTsspVariantPasskeyEntry:
            pkey.action = BLE_SM_IOACT_INPUT;
            pkey.passkey = ulPasskey;
            xESPStatus = ble_sm_inject_io( usGattConnHandle, &pkey );
            break;

        case eBTsspVariantPasskeyNotification:
            pkey.action = BLE_SM_IOACT_DISP;
            pkey.passkey = ulPasskey;
            xESPStatus = ble_sm_inject_io( usGattConnHandle, &pkey );
            break;

        case eBTsspVariantConsent:
            /* Not used: Placeholder */
            break;

        default:
            break;
    }

    if( xESPStatus != 0 )
    {
        xStatus = eBTStatusFail;
    }
#endif

    return eBTStatusSuccess;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTReadEnergyInfo()
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTDutModeConfigure( bool bEnable )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTDutModeSend( uint16_t usOpcode,
                             uint8_t * pucBuf,
                             size_t xLen )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTLeTestMode( uint16_t usOpcode,
                            uint8_t * pucBuf,
                            size_t xLen )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTConfigHCISnoopLog( bool bEnable )
{
    return eBTStatusUnsupported;
}


/*-----------------------------------------------------------*/

BTStatus_t prvBTConfigClear()
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTReadRssi( const BTBdaddr_t * pxBdAddr )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

BTStatus_t prvBTGetTxpower( const BTBdaddr_t * pxBdAddr,
                            BTTransport_t xTransport )
{
    return eBTStatusUnsupported;
}

/*-----------------------------------------------------------*/

const void * prvGetClassicAdapter()
{
    return NULL;
}

void prvSetDeviceName( char *newName, size_t length )
{
    memcpy( bleName, newName, MIN(length,(sizeof(bleName) - 1)) );
    /* Add NULL termination for name string */
    bleName[ MIN(length, (sizeof(bleName) - 1)) ] = '\0';
}

char * prvGetDeviceName( void )
{
    return bleName;
}

/*-----------------------------------------------------------*/

const BTInterface_t * BTGetBluetoothInterface()
{
    return &xBTinterface;
}
