/*
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
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
 ZLO_NUMBER_OF_REPORTS
};

#define ZCL_NUMBER_OF_REPORTS         ZLO_NUMBER_OF_REPORTS
#define ZLO_MIN_REPORT_INTERVAL       1
#define ZLO_MAX_REPORT_INTERVAL       61

#define CLD_BIND_SERVER
#define MAX_NUM_BIND_QUEUE_BUFFERS            ZCL_NUMBER_OF_REPORTS
#define MAX_PDU_BIND_QUEUE_PAYLOAD_SIZE       (24)

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

#define BAS_MODEL_ID_STRING		"ZLO-DimmableLight"
#define BAS_MANUF_NAME_STRING   "NXP"
#define BAS_DATE_STRING         "20161101"
#define BAS_SW_BUILD_STRING     "1000-0001"
#define BAS_URL_STRING          "www.nxp.com"
#define BAS_PCODE_STRING		{4,3,2,1}

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
#define CLD_SCENES
#define SCENES_SERVER
#define CLD_SCENES_MAX_NUMBER_OF_SCENES                     16
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
#define CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL
#define CLD_ONOFF_ATTR_ON_TIME
#define CLD_ONOFF_ATTR_OFF_WAIT_TIME
#define CLD_ONOFF_ATTR_STARTUP_ONOFF

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

/****************************************************************************/
/*             Level Control Cluster - Optional Attributes                  */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the level control cluster.                                 */
/****************************************************************************/
#define CLD_LEVELCONTROL_ATTR_REMAINING_TIME
#define CLD_LEVELCONTROL_ATTR_STARTUP_CURRENT_LEVEL

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
#define CLD_LEVEL_CONTROL_STARTUP_LEVEL                  CLD_LEVELCONTROL_MAX_LEVEL

/****************************************************************************/
/*             Colour Control Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the time cluster.                                          */
/****************************************************************************/

/****************************************************************************/
/*             OTA Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the OTA cluster.                                          */
/****************************************************************************/
#ifdef  CLD_OTA
    #define OTA_DEMO_TIMINGS                        // define this fior the fast timings for edemo purposes
    #define OTA_ACKS_ON                             FALSE
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
    //#define OTA_STRING_COMPARE  
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
    #define GP_KEYTPE              E_GP_ZGPD_GROUP_KEY
    #define GP_SHARED_KEY  { 0xC0 , 0xC1 , 0xC2 , 0xC3 , 0xC4 , 0xC5 , 0xC6 , 0xC7 , 0xC8 , 0xC9 , 0xCa , 0xCb , 0xCc , 0xCd , 0xCe , 0xCf}
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
#define TL_LEGACY_DEVICE_ID    (0x0100);
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
