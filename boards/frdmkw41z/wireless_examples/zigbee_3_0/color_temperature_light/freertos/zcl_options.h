/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
  


#ifndef ZCL_OPTIONS_H
#define ZCL_OPTIONS_H

/*!
\file       zcl_options.h
\brief      Options Header for ZigBee Cluster Library functions
*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/* This is the NXP manufacturer code.If creating new a manufacturer         */
/* specific command apply to the Zigbee alliance for an Id for your company */
/* Also update the manufacturer code in .zpscfg: Node Descriptor->misc      */
#define ZCL_MANUFACTURER_CODE                                0x1037

/* Sets the number of endpoints that will be created by the ZCL library */
#define ZCL_NUMBER_OF_ENDPOINTS                             1
#define ZLO_NUMBER_DEVICES                                  1
#define APPLICATION_DEVICE_VERSION          1

/* Set this Tue to disable non error default responses from clusters */
#define ZCL_DISABLE_DEFAULT_RESPONSES       (FALSE)
#define ZCL_DISABLE_APS_ACK                 (FALSE)


#define ZCL_NUMBER_OF_APPLICATION_TIMERS     3
#define NUM_ENDPOINT_RECORDS                 1
#define NUM_GROUP_RECORDS                    4

/* Enable wild card profile */
#define ZCL_ALLOW_WILD_CARD_PROFILE


/* Which Custom commands needs to be supported */
#define ZCL_ATTRIBUTE_READ_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_WRITE_SERVER_SUPPORTED

#define ZCL_ATTRIBUTE_DISCOVERY_SERVER_SUPPORTED

#define ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
#define ZCL_CONFIGURE_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#define ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_SERVER_SUPPORTED

// define the number of reports and the places in the array to save them
enum
{
 REPORT_ONOFF_SLOT = 0,
 REPORT_LEVEL_SLOT,
 REPORT_COLOR_COLTEMP_SLOT,
 ZLO_NUMBER_OF_REPORTS = 3
};

#define REPORT_COL_TEMP
#define ZCL_NUMBER_OF_REPORTS         ZLO_NUMBER_OF_REPORTS
#define ZLO_MIN_REPORT_INTERVAL       1
#define ZLO_MAX_REPORT_INTERVAL       61
#define ZCL_SYSTEM_MAX_REPORT_INTERVAL  61

#define CLD_BIND_SERVER
#define MAX_NUM_BIND_QUEUE_BUFFERS            ZCL_NUMBER_OF_REPORTS
#define MAX_PDU_BIND_QUEUE_PAYLOAD_SIZE        (24)

/****************************************************************************/
/*                             Enable Cluster                               */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to enable         */
/* cluster and their client or server instances                             */
/****************************************************************************/
#define CLD_BASIC
#define BASIC_SERVER

#define CLD_ZLL_COMMISSION
#define ZLL_COMMISSION_SERVER

#define CLD_IDENTIFY
#define IDENTIFY_SERVER

#define CLD_GROUPS
#define GROUPS_SERVER

#define CLD_ONOFF
#define ONOFF_SERVER

#define CLD_LEVEL_CONTROL
#define LEVEL_CONTROL_SERVER

#define CLD_SCENES
#define SCENES_SERVER

#define CLD_COLOUR_CONTROL
#define COLOUR_CONTROL_SERVER

#ifdef BUILD_OTA
#define CLD_OTA
#define OTA_CLIENT
#endif

/****************************************************************************/
/*             Basic Cluster - Optional Attributes                          */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the basic cluster.                                         */
/****************************************************************************/
#define CLD_BAS_ATTR_APPLICATION_VERSION
#define CLD_BAS_ATTR_STACK_VERSION
#define CLD_BAS_ATTR_HARDWARE_VERSION
#define CLD_BAS_ATTR_MANUFACTURER_NAME
#define CLD_BAS_ATTR_MODEL_IDENTIFIER
#define CLD_BAS_ATTR_DATE_CODE
#define CLD_BAS_ATTR_SW_BUILD_ID
#define CLD_BAS_ATTR_GENERIC_DEVICE_CLASS
#define CLD_BAS_ATTR_GENERIC_DEVICE_TYPE
#define CLD_BAS_ATTR_PRODUCT_CODE
#define CLD_BAS_ATTR_PRODUCT_URL

// #define CLD_BAS_ATTR_MANUFACTURER_VERSION_DETAILS
// #define CLD_BAS_ATTR_SERIAL_NUMBER
// #define CLD_BAS_ATTR_PRODUCT_LABEL

#define BAS_MODEL_ID_STRING		"ZLO-ColorTemperature"
#define BAS_MANUF_NAME_STRING   "NXP"
#define BAS_DATE_STRING         "20161101"
#define BAS_SW_BUILD_STRING     "1000-0001"
#define BAS_URL_STRING          "www.nxp.com"
#define BAS_PCODE_STRING		{5,6,7,8}

#define CLD_BAS_APP_VERSION         (1)
#define CLD_BAS_STACK_VERSION       (1)
#define CLD_BAS_HARDWARE_VERSION    (1)
#define CLD_BAS_MANUF_NAME_SIZE     sizeof(BAS_MANUF_NAME_STRING)-1
#define CLD_BAS_MODEL_ID_SIZE       sizeof(BAS_MODEL_ID_STRING)-1
#define CLD_BAS_DATE_SIZE           sizeof(BAS_DATE_STRING)-1
#define CLD_BAS_POWER_SOURCE        E_CLD_BAS_PS_SINGLE_PHASE_MAINS
#define CLD_BAS_SW_BUILD_SIZE       sizeof(BAS_SW_BUILD_STRING)-1
#define CLD_BAS_URL_SIZE            sizeof(BAS_URL_STRING)-1
#define CLD_BAS_PCODE_SIZE          (4)
#define CLD_BAS_DEVICE_CLASS        (0)
#define CLD_BAS_MANUFACTURER_VERSION_SIZE  (18)
#define CLD_BAS_SERIAL_NUMBER_SIZE  (4)
#define CLD_BAS_PRODUCT_LABEL_SIZE  (13)

#define CLD_BAS_CMD_RESET_TO_FACTORY_DEFAULTS

/****************************************************************************/
/*             Identify Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Identify cluster.                                 */
/****************************************************************************/
#define CLD_IDENTIFY_TICKS_PER_SECOND    10
#define CLD_IDENTIFY_CMD_TRIGGER_EFFECT
#define CLD_IDENTIFY_10HZ_TICK
#define CLD_IDENTIFY_ATTR_COMMISSION_STATE

/****************************************************************************/
/*             Groups Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Groups cluster.                                 */
/****************************************************************************/
#define CLD_GROUPS_MAX_NUMBER_OF_GROUPS                     16
#define CLD_GROUPS_DISABLE_NAME_SUPPORT

/****************************************************************************/
/*             Scenes Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Scenes cluster.                                 */
/****************************************************************************/
#define CLD_SCENES_MAX_NUMBER_OF_SCENES                     9
#define CLD_SCENES_DISABLE_NAME_SUPPORT
#define CLD_SCENES_MAX_SCENE_NAME_LENGTH                    16
#define CLD_SCENES_MAX_SCENE_STORAGE_BYTES                  24
#define CLD_SCENES_ATTR_LAST_CONFIGURED_BY
#define CLD_SCENES_TABLE_SUPPORT_TRANSITION_TIME_IN_MS
#define  CLD_SCENES_CMD_ENHANCED_ADD_SCENE
#define CLD_SCENES_CMD_ENHANCED_VIEW_SCENE
#define CLD_SCENES_CMD_COPY_SCENE

/****************************************************************************/
/*             OnOff Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the OnOff cluster.                                 */
/****************************************************************************/
#define CLD_ONOFF_CMD_ON_WITH_RECALL_GLOBAL_SCENE
#define CLD_ONOFF_CMD_OFF_WITH_EFFECT
#define CLD_ONOFF_CMD_ON_WITH_TIMED_OFF
/* OnOff start Up Mode
 * 0x00 -> Start Up in Off state
 * 0x01 -> Start Up in On state
 * 0x02 -> Start Up in opposite state to the previous (toggle)
 * 0xFF -> Start Up in previous state
 */
#define CLD_ONOFF_START_MODE                     (0xFF)
#define CLD_ONOFF_START_STATE                    (TRUE)

#define CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL
#define CLD_ONOFF_ATTR_ON_TIME
#define CLD_ONOFF_ATTR_OFF_WAIT_TIME
#define CLD_ONOFF_ATTR_STARTUP_ONOFF

/****************************************************************************/
/*             Level Control Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the level control cluster.                                 */
/****************************************************************************/
#define CLD_LEVELCONTROL_ATTR_ON_OFF_TRANSITION_TIME        (10)
#define CLD_LEVELCONTROL_ATTR_ON_TRANSITION_TIME            (12)
#define CLD_LEVELCONTROL_ATTR_OFF_TRANSITION_TIME           (13)
#define CLD_LEVELCONTROL_TICKS_PER_SECOND                   10
#define CLD_LEVELCONTROL_MIN_LEVEL                          (1)
#define CLD_LEVELCONTROL_MAX_LEVEL                          (0xfe)
/* Level Options
 * Bit 0 -> Set Allow Move if Off (ZHA), Clear disallow move if off (ZLL)
 * Bit 1 -> Set couple Level to Colour Temperature
 */
#define CLD_LEVELCONTROL_OPTIONS                            (0)
/* 0x00 -> Start at Min Level
 * 0x01 -> 0xFE -> start at this level
 * 0xFF -> Start at previous Level
 */
#define CLD_LEVEL_CONTROL_START_MODE                     (0xFF)
#define CLD_LEVEL_CONTROL_STARTUP_LEVEL                     CLD_LEVELCONTROL_MAX_LEVEL
#define CLD_LEVELCONTROL_ATTR_REMAINING_TIME
#define CLD_LEVELCONTROL_ATTR_STARTUP_CURRENT_LEVEL

/****************************************************************************/
/*             Colour Control Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the time cluster.                                          */
/****************************************************************************/
#define CLD_COLOURCONTROL_ATTR_REMAINING_TIME
#define CLD_COLOURCONTROL_ATTR_COLOUR_MODE
#define CLD_COLOURCONTROL_ATTR_ENHANCED_COLOUR_MODE
#define CLD_COLOURCONTROL_ATTR_COLOUR_CAPABILITIES

/* define capabilites of colour temperature bulb */
#define CLD_COLOURCONTROL_COLOUR_CAPABILITIES           (COLOUR_CAPABILITY_COLOUR_TEMPERATURE_SUPPORTED)

/* Max & Min Limits for colour temperature */
#define CLD_COLOURCONTROL_COLOUR_TEMPERATURE_PHY_MIN    (153U)    /* (1E6/153)   = 6500 Kelvin Colour Temperature */
#define CLD_COLOURCONTROL_COLOUR_TEMPERATURE_PHY_MAX    (370U)  /* (1E6/370) = 2700  Kelvin Colour Temperature */

#define CLD_COLOURCONTROL_ATTR_COUPLE_COLOUR_TEMPERATURE_TO_LEVEL_MIN_MIRED
#define CLD_COLOURCONTROL_ATTR_STARTUP_COLOUR_TEMPERATURE_MIRED

#define CLD_COLOURCONTROL_COUPLE_COLOUR_TEMPERATURE_TO_LEVEL_MIN_MIRED  CLD_COLOURCONTROL_COLOUR_TEMPERATURE_PHY_MIN
/* move options
 * 0x00 -> disallow moves if off (ZLL Mode)
 * 0x01 -> allow moves if off (ZHA Mode)
 */
#define CLD_COLOURCONTROL_OPTIONS               (0x00)
/* Colour Start Up Options
 * 0x0000 - 0xffef -> start Up in this colour temperature Value (in mireds)
 * 0xFFFF          -> Start up in the Previous CT value
 *
 */
#define CLD_COLOURCONTROL_START_MODE                     (0xFFFF)
#define CLD_COLOURCONTROL_START_CT                       (300)

/* Defined Primaries Information attribute attribute ID's set (5.2.2.2.2) */
#define CLD_COLOURCONTROL_ATTR_NUMBER_OF_PRIMARIES  3

/* Enable Primary (n) X, Y and Intensity attributes */
#define CLD_COLOURCONTROL_ATTR_PRIMARY_1
#define CLD_COLOURCONTROL_ATTR_PRIMARY_2
#define CLD_COLOURCONTROL_ATTR_PRIMARY_3
#define CLD_COLOURCONTROL_ATTR_PRIMARY_4
#define CLD_COLOURCONTROL_ATTR_PRIMARY_5
#define CLD_COLOURCONTROL_ATTR_PRIMARY_6

#define CLD_COLOURCONTROL_RED_X     (0.68)  // 44564 -> 0xAE14
#define CLD_COLOURCONTROL_RED_Y     (0.31)  // 20316 -> 0x4F5C
#define CLD_COLOURCONTROL_GREEN_X   (0.11)  // 7208  -> 0x1C28
#define CLD_COLOURCONTROL_GREEN_Y   (0.82)  // 53739 -> 0xD1EB
#define CLD_COLOURCONTROL_BLUE_X    (0.13)  // 8519  -> 0x2147
#define CLD_COLOURCONTROL_BLUE_Y    (0.04)  // 2621   -> 0x0A3D
#define CLD_COLOURCONTROL_WHITE_X   (0.33)  // 21626 -> 0x5478
#define CLD_COLOURCONTROL_WHITE_Y   (0.33)  // 21626 -> 0x5478

#define CLD_COLOURCONTROL_PRIMARY_1_X           CLD_COLOURCONTROL_RED_X
#define CLD_COLOURCONTROL_PRIMARY_1_Y           CLD_COLOURCONTROL_RED_Y
#define CLD_COLOURCONTROL_PRIMARY_1_INTENSITY   (254 / 3)

#define CLD_COLOURCONTROL_PRIMARY_2_X           CLD_COLOURCONTROL_GREEN_X
#define CLD_COLOURCONTROL_PRIMARY_2_Y           CLD_COLOURCONTROL_GREEN_Y
#define CLD_COLOURCONTROL_PRIMARY_2_INTENSITY   (254 / 3)

#define CLD_COLOURCONTROL_PRIMARY_3_X           CLD_COLOURCONTROL_BLUE_X
#define CLD_COLOURCONTROL_PRIMARY_3_Y           CLD_COLOURCONTROL_BLUE_Y
#define CLD_COLOURCONTROL_PRIMARY_3_INTENSITY   (254 / 3)

#define CLD_COLOURCONTROL_PRIMARY_4_X           (0)
#define CLD_COLOURCONTROL_PRIMARY_4_Y           (0)
#define CLD_COLOURCONTROL_PRIMARY_4_INTENSITY   (0xff)

#define CLD_COLOURCONTROL_PRIMARY_5_X           (0)
#define CLD_COLOURCONTROL_PRIMARY_5_Y           (0)
#define CLD_COLOURCONTROL_PRIMARY_5_INTENSITY   (0xff)

#define CLD_COLOURCONTROL_PRIMARY_6_X           (0)
#define CLD_COLOURCONTROL_PRIMARY_6_Y           (0)
#define CLD_COLOURCONTROL_PRIMARY_6_INTENSITY   (0xff)


/****************************************************************************/
/*             OTA Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the OTA cluster.                                          */
/****************************************************************************/
#ifdef  CLD_OTA
    #define OTA_DEMO_TIMINGS                            // define this fior the fast timings for edemo purposes

    #define OTA_ACKS_ON FALSE
    #define OTA_MAX_CO_PROCESSOR_IMAGES             0
    #define OTA_CLD_ATTR_CURRENT_FILE_VERSION
    #define OTA_MAX_BLOCK_SIZE                      48      // in multiples of 16 (internal flash requirement)
#ifdef OTA_DEMO_TIMINGS
    #define OTA_TIME_INTERVAL_BETWEEN_RETRIES       5       // Valid only if OTA_TIME_INTERVAL_BETWEEN_REQUESTS not defined
    #define CLD_OTA_MAX_BLOCK_PAGE_REQ_RETRIES      10      // count of block reqest failure befiore abandoning download
#else
    #define OTA_TIME_INTERVAL_BETWEEN_REQUESTS      RND_u32GetRand(10,20)
    #define OTA_TIME_INTERVAL_BETWEEN_RETRIES       RND_u32GetRand(10,20)      // Valid only if OTA_TIME_INTERVAL_BETWEEN_REQUESTS not defined
    #define CLD_OTA_MAX_BLOCK_PAGE_REQ_RETRIES      240                        // count of block reqest failure befiore abandoning download
#endif
    #define OTA_STRING_COMPARE
    #define OTA_MAX_IMAGES_PER_ENDPOINT             1
#endif

/****************************************************************************/
/*             Green Power Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Green Power cluster.                                          */
/****************************************************************************/
#ifdef  CLD_GREENPOWER
    #define GP_IEEE_ADDR_SUPPORT
    #define GREENPOWER_END_POINT_ID                 2
    #define GP_NUMBER_OF_TRANSLATION_TABLE_ENTRIES  5
    #define GP_NUMBER_OF_PROXY_SINK_TABLE_ENTRIES       5
    #define CLD_GP_ATTR_ZGPS_COMMISSIONING_WINDOW
    #define CLD_GP_ATTR_ZGP_SHARED_SECURITY_KEY_TYPE
    #define CLD_GP_ATTR_ZGP_SHARED_SECURITY_KEY
    #define CLD_GP_ATTR_ZGP_LINK_KEY
    /* security level */
    #define GP_SECURITY_LEVEL E_GP_ENC_FULL_FC_FULL_MIC
    /* Possible values are  E_GP_NO_KEY, E_GP_ZIGBEE_NWK_KEY, E_GP_ZGPD_GROUP_KEY,
        E_GP_NWK_KEY_DERIVED_ZGPD_GROUP_KEY, E_GP_OUT_OF_THE_BOX_ZGPD_KEY, E_GP_DERIVED_INDIVIDUAL_ZGPD_KEY*/
    #define GP_KEYTPE            E_GP_ZGPD_GROUP_KEY
    #define GP_SHARED_KEY  { 0xC0 , 0xC1 , 0xC2 , 0xC3 , 0xC4 , 0xC5 , 0xC6 , 0xC7 , 0xC8 , 0xC9 , 0xCa , 0xCb , 0xCc , 0xCd , 0xCe , 0xCf }
    #undef ZCL_NUMBER_OF_ENDPOINTS
    #define ZCL_NUMBER_OF_ENDPOINTS                             2
    #undef CLD_GROUPS_MAX_NUMBER_OF_GROUPS
    #define CLD_GROUPS_MAX_NUMBER_OF_GROUPS                     4
    #define GP_MAX_TRANS_ZB_CMD_PAYLOAD_LENGTH                   2
#endif

/****************************************************************************/
/*             Touchlink Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the Touchlink cluster.                                          */
/****************************************************************************/
/*
 * Use this if you have the ZLL Master Key
 */
//#define TL_SUPPORTED_KEYS ( TL_MASTER_KEY_MASK )
/*
 * Use the Test and Certification Keys
 */
#define TL_SUPPORTED_KEYS (TL_TEST_KEY_MASK | TL_CERTIFICATION_KEY_MASK )
#define TL_LEGACY_PROFILE_ID   (0xc05e);
#define TL_LEGACY_DEVICE_ID    (0x0220);
#define TL_LEGACY_VERSION_ID    (2)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void* psGetDeviceTable(void);
PUBLIC void vSaveScenesNVM(void);
PUBLIC void vLoadScenesNVM(void);

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ZCL_OPTIONS_H */
