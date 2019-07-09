/*
  * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of
  * Cypress Semiconductor Corporation. All Rights Reserved.
  *
  * This software, associated documentation and materials ("Software"),
  * is owned by Cypress Semiconductor Corporation
  * or one of its subsidiaries ("Cypress") and is protected by and subject to
  * worldwide patent protection (United States and foreign),
  * United States copyright laws and international treaty provisions.
  * Therefore, you may use this Software only as provided in the license
  * agreement accompanying the software package from which you
  * obtained this Software ("EULA").
  * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
  * non-transferable license to copy, modify, and compile the Software
  * source code solely for use in connection with Cypress's
  * integrated circuit products. Any reproduction, modification, translation,
  * compilation, or representation of this Software except as specified
  * above is prohibited without the express written permission of Cypress.
  *
  * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
  * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
  * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
  * reserves the right to make changes to the Software without notice. Cypress
  * does not assume any liability arising out of the application or use of the
  * Software or any product or circuit described in the Software. Cypress does
  * not authorize its products for use in any products where a malfunction or
  * failure of the Cypress product may reasonably be expected to result in
  * significant property damage, injury or death ("High Risk Product"). By
  * including Cypress's product in a High Risk Product, the manufacturer
  * of such system or application assumes all risk of such use and in doing
  * so agrees to indemnify Cypress against all liability.
  */

#ifdef WICED_BLUETOOTH_PLATFORM /** Define at project level for Bluetooth applications */
#include "wiced_bluetooth_result.h"
#endif

#include "wwd_constants.h"

/*
 * @file: WICED Platform Header that ports all the WICED dependent constants of WWD.
 *
 * */
#ifndef WWD_WWD_WICED_H_
#define WWD_WWD_WICED_H_

#define WICED_DEEP_SLEEP_SAVED_VAR( var )                                               var
#define WICED_WIFI_MAX_CHANNELS 2

/******************************************************
 *                   Enumerations
 ******************************************************/

/**
 * WPS Connection Mode
 */
typedef enum
{
    WICED_WPS_PBC_MODE = 1,  /**< Push button mode */
    WICED_WPS_PIN_MODE = 2   /**< PIN mode         */
} wiced_wps_mode_t;

/**
 * WPS Device Category from the WSC2.0 spec
 */
typedef enum
{
    WICED_WPS_DEVICE_COMPUTER               = 1,     /**< COMPUTER               */
    WICED_WPS_DEVICE_INPUT                  = 2,     /**< INPUT                  */
    WICED_WPS_DEVICE_PRINT_SCAN_FAX_COPY    = 3,     /**< PRINT_SCAN_FAX_COPY    */
    WICED_WPS_DEVICE_CAMERA                 = 4,     /**< CAMERA                 */
    WICED_WPS_DEVICE_STORAGE                = 5,     /**< STORAGE                */
    WICED_WPS_DEVICE_NETWORK_INFRASTRUCTURE = 6,     /**< NETWORK_INFRASTRUCTURE */
    WICED_WPS_DEVICE_DISPLAY                = 7,     /**< DISPLAY                */
    WICED_WPS_DEVICE_MULTIMEDIA             = 8,     /**< MULTIMEDIA             */
    WICED_WPS_DEVICE_GAMING                 = 9,     /**< GAMING                 */
    WICED_WPS_DEVICE_TELEPHONE              = 10,    /**< TELEPHONE              */
    WICED_WPS_DEVICE_AUDIO                  = 11,    /**< AUDIO                  */
    WICED_WPS_DEVICE_OTHER                  = 0xFF,  /**< OTHER                  */
} wiced_wps_device_category_t;

/**
 * WPS Configuration Methods from the WSC2.0 spec
 */
typedef enum
{
    WPS_CONFIG_USBA                  = 0x0001,  /**< USBA                 */
    WPS_CONFIG_ETHERNET              = 0x0002,  /**< ETHERNET             */
    WPS_CONFIG_LABEL                 = 0x0004,  /**< LABEL                */
    WPS_CONFIG_DISPLAY               = 0x0008,  /**< DISPLAY              */
    WPS_CONFIG_EXTERNAL_NFC_TOKEN    = 0x0010,  /**< EXTERNAL_NFC_TOKEN   */
    WPS_CONFIG_INTEGRATED_NFC_TOKEN  = 0x0020,  /**< INTEGRATED_NFC_TOKEN */
    WPS_CONFIG_NFC_INTERFACE         = 0x0040,  /**< NFC_INTERFACE        */
    WPS_CONFIG_PUSH_BUTTON           = 0x0080,  /**< PUSH_BUTTON          */
    WPS_CONFIG_KEYPAD                = 0x0100,  /**< KEYPAD               */
    WPS_CONFIG_VIRTUAL_PUSH_BUTTON   = 0x0280,  /**< VIRTUAL_PUSH_BUTTON  */
    WPS_CONFIG_PHYSICAL_PUSH_BUTTON  = 0x0480,  /**< PHYSICAL_PUSH_BUTTON */
    WPS_CONFIG_VIRTUAL_DISPLAY_PIN   = 0x2008,  /**< VIRTUAL_DISPLAY_PIN  */
    WPS_CONFIG_PHYSICAL_DISPLAY_PIN  = 0x4008   /**< PHYSICAL_DISPLAY_PIN */
} wiced_wps_configuration_method_t;

/**
 * WICED SoftAP events
 */
typedef enum
{
    WICED_AP_UNKNOWN_EVENT,         /**< Unknown SoftAP event       */
    WICED_AP_STA_JOINED_EVENT,      /**< a STA joined our SoftAP    */
    WICED_AP_STA_LEAVE_EVENT,       /**< a STA left our SoftAP      */
} wiced_wifi_softap_event_t;

/**
 * WICED Result Type
 */
/*
 * Values: 0 - 999
 */
#define WICED_RESULT_LIST( prefix ) \
    RESULT_ENUM( prefix, SUCCESS,                        0 ),   /**< Success */                        \
    RESULT_ENUM( prefix, PENDING,                        1 ),   /**< Pending */                        \
    RESULT_ENUM( prefix, TIMEOUT,                        2 ),   /**< Timeout */                        \
    RESULT_ENUM( prefix, PARTIAL_RESULTS,                3 ),   /**< Partial results */                \
    RESULT_ENUM( prefix, ERROR,                          4 ),   /**< Error */                          \
    RESULT_ENUM( prefix, BADARG,                         5 ),   /**< Bad Arguments */                  \
    RESULT_ENUM( prefix, BADOPTION,                      6 ),   /**< Mode not supported */             \
    RESULT_ENUM( prefix, UNSUPPORTED,                    7 ),   /**< Unsupported function */           \
    RESULT_ENUM( prefix, OUT_OF_HEAP_SPACE,              8 ),   /**< Dynamic memory space exhausted */ \
    RESULT_ENUM( prefix, NOTUP,                          9 ),   /**< Interface is not currently Up */  \
    RESULT_ENUM( prefix, UNFINISHED,                    10 ),   /**< Operation not finished yet */     \
    RESULT_ENUM( prefix, CONNECTION_LOST,               11 ),   /**< Connection to server lost */      \
    RESULT_ENUM( prefix, NOT_FOUND,                     12 ),   /**< Item not found */                 \
    RESULT_ENUM( prefix, PACKET_BUFFER_CORRUPT,         13 ),   /**< Packet buffer corrupted */        \
    RESULT_ENUM( prefix, ROUTING_ERROR,                 14 ),   /**< Routing error */                  \
    RESULT_ENUM( prefix, BADVALUE,                      15 ),   /**< Bad value */                      \
    RESULT_ENUM( prefix, WOULD_BLOCK,                   16 ),   /**< Function would block */           \
    RESULT_ENUM( prefix, ABORTED,                       17 ),   /**< Operation aborted */              \
    RESULT_ENUM( prefix, CONNECTION_RESET,              18 ),   /**< Connection has been reset */      \
    RESULT_ENUM( prefix, CONNECTION_CLOSED,             19 ),   /**< Connection is closed */           \
    RESULT_ENUM( prefix, NOT_CONNECTED,                 20 ),   /**< Connection is not connected */    \
    RESULT_ENUM( prefix, ADDRESS_IN_USE,                21 ),   /**< Address is in use */              \
    RESULT_ENUM( prefix, NETWORK_INTERFACE_ERROR,       22 ),   /**< Network interface error */        \
    RESULT_ENUM( prefix, ALREADY_CONNECTED,             23 ),   /**< Socket is already connected */    \
    RESULT_ENUM( prefix, INVALID_INTERFACE,             24 ),   /**< Interface specified in invalid */ \
    RESULT_ENUM( prefix, SOCKET_CREATE_FAIL,            25 ),   /**< Socket creation failed */         \
    RESULT_ENUM( prefix, INVALID_SOCKET,                26 ),   /**< Socket is invalid */              \
    RESULT_ENUM( prefix, CORRUPT_PACKET_BUFFER,         27 ),   /**< Packet buffer is corrupted */     \
    RESULT_ENUM( prefix, UNKNOWN_NETWORK_STACK_ERROR,   28 ),   /**< Unknown network stack error */    \
    RESULT_ENUM( prefix, NO_STORED_AP_IN_DCT,           29 ),   /**< DCT contains no AP credentials */ \
    RESULT_ENUM( prefix, STA_JOIN_FAILED,               30 ),   /**< Join failed */                    \
    RESULT_ENUM( prefix, PACKET_BUFFER_OVERFLOW,        31 ),   /**< Packet buffer overflow */         \
    RESULT_ENUM( prefix, ALREADY_INITIALIZED,           32 ),   /**< Module has already been inited */


#define FILESYSTEM_RESULT_LIST( prefix ) \
        RESULT_ENUM( prefix, SUCCESS,                       0 ),   /**< Success */               \
        RESULT_ENUM( prefix, PENDING,                       1 ),   /**< Pending */               \
        RESULT_ENUM( prefix, TIMEOUT,                       2 ),   /**< Timeout */               \
        RESULT_ENUM( prefix, PARTIAL_RESULTS,               3 ),   /**< Partial results */       \
        RESULT_ENUM( prefix, ERROR,                         4 ),   /**< Error */                 \
        RESULT_ENUM( prefix, BADARG,                        5 ),   /**< Bad Arguments */         \
        RESULT_ENUM( prefix, BADOPTION,                     6 ),   /**< Mode not supported */    \
        RESULT_ENUM( prefix, UNSUPPORTED,                   7 ),   /**< Unsupported function */  \
        RESULT_ENUM( prefix, DISK_ERROR,                10008 ),   /**< Low level error accessing media */        \
        RESULT_ENUM( prefix, PATH_NOT_FOUND,            10009 ),   /**< Path was not found in filesystem */        \
        RESULT_ENUM( prefix, MEDIA_NOT_READY,           10010 ),   /**< Media is not present or ready for access */          \
        RESULT_ENUM( prefix, ACCESS_DENIED,             10011 ),   /**< Access denied due to permissions  */      \
        RESULT_ENUM( prefix, WRITE_PROTECTED,           10012 ),   /**< Media is write protected */    \
        RESULT_ENUM( prefix, OUT_OF_SPACE,              10013 ),   /**< No free space left on media  */ \
        RESULT_ENUM( prefix, FILENAME_BUFFER_TOO_SMALL, 10014 ),   /**< Filename buffer was too small when retrieving directory contents  */ \
        RESULT_ENUM( prefix, END_OF_RESOURCE,           10015 ),   /**< End of file/directory reached  */ \
        RESULT_ENUM( prefix, FILESYSTEM_INVALID,        10016 ),   /**< Filesystem has an unrecoverable error */ \
        RESULT_ENUM( prefix, BLOCK_SIZE_BAD,            10017 ),   /**< Block size is invalid - not a multiple or sub-multiple of DEFAULT_SECTOR_SIZE */


typedef enum
{
    WICED_RESULT_LIST     ( WICED_            )  /*     0 -   999 */
    WWD_RESULT_LIST       ( WICED_WWD_        )  /*  1000 -  1999 */
    WLAN_RESULT_LIST      ( WICED_WLAN_       )  /*  2000 -  2999 */
#ifdef WICED_BLUETOOTH_PLATFORM
    BT_RESULT_LIST        ( WICED_BT_         )  /*  8000 -  8999 */
#endif
} wiced_result_t;

/**
 * Misc WiFi flags stored in DCT
 * structure
 *      platform_dct_misc_config_t
 *    field
 *      wifi_flags
 */
#define WIFI_FLAG_MESH                      (1 << 0)  /**< MESH master enable/disable */
#define WIFI_FLAG_MESH_MCAST_REBROADCAST    (1 << 1)  /**< MESH mcast rebroadcast */
#define WIFI_MESH_DHCP_IP                   (1 << 4)
#define WIFI_FLAG_MESH_GATEWAY              (1 << 5)  /**< This machine a Gateway with an additional STA interface (to connect to AP) */

#define WIFI_FLAG_MESH_ALL_FLAGS            (WIFI_FLAG_MESH | WIFI_FLAG_MESH_MCAST_REBROADCAST | WIFI_MESH_DHCP_IP | WIFI_FLAG_MESH_GATEWAY)

/**
 * Semaphore wait time constants
 */
#define WICED_NEVER_TIMEOUT                         (0xFFFFFFFF)
#define WICED_WAIT_FOREVER                          (0xFFFFFFFF)
#define WICED_NO_WAIT                               (0)

#define WICED_WRITE_16( pointer, value )      (*((uint16_t*)pointer) = value)
#define WICED_WRITE_32( pointer, value )      (*((uint32_t*)pointer) = value)
#define WICED_READ_16( pointer )              *((uint16_t*)pointer)
#define WICED_READ_32( pointer )              *((uint32_t*)pointer)

#define NULL_MAC( a )  (((((unsigned char*)a)[0])==0)&& \
                            ((((unsigned char*)a)[1])==0)&& \
                            ((((unsigned char*)a)[2])==0)&& \
                            ((((unsigned char*)a)[3])==0)&& \
                            ((((unsigned char*)a)[4])==0)&& \
                            ((((unsigned char*)a)[5])==0))

#define wiced_htol16_ua_store( val, bytes ) \
    do \
    { \
        uint16_t _val = (val); \
        uint8_t* _bytes = (uint8_t* )(bytes); \
        _bytes[0] = (uint8_t)((uint16_t)_val & 0xff); \
        _bytes[1] = (uint8_t)((uint16_t)_val >> 8); \
    } while(0)
/**
 * Maximum digits after decimal point for float output to string
 */
#define FLOAT_TO_STRING_MAX_FRACTION_SUPPORTED      (6)

/**
 * Size of ascii printable string for a 6 octet ethernet address (including ending NULL)
 */
#define WICED_ETHER_ADDR_STR_LEN                    (18)
#define WICED_ETHER_ADDR_LEN                        ( 6)

#endif /* WWD_WWD_WICED_H_ */
