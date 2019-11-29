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
#define ZCL_ALLOW_WILD_CARD_PROFILE
#define CLD_BAS_CMD_RESET_TO_FACTORY_DEFAULTS

/* Set this True to disable non error default responses from clusters */
#define ZCL_DISABLE_DEFAULT_RESPONSES                      (FALSE)
#define ZCL_DISABLE_APS_ACK                                (FALSE)
#define OTA_ACKS_ON                                        (TRUE)
#define OTA_CLIENT_DISABLE_DEFAULT_RESPONSE                (FALSE)

// Total number of group addresses required by all sub-device endpoints on the device
#define GROUPS_REQUIRED                                     ( 16 )
#define CLD_GROUPS_MAX_NUMBER_OF_GROUPS                     ( 16  )
/* Sets the number of endpoints that will be created by the ZCL library */
#define ZCL_NUMBER_OF_ENDPOINTS                             ( 1 )
#define ZCL_NUMBER_DEVICES                                  ( 1 )

#define ZCL_MANUFACTURER_CODE                               ( 0x1037 )

#define ZCL_NUMBER_OF_APPLICATION_TIMERS                    ( 0 )
#define TL_LEGACY_PROFILE_ID                                ( 0xc05e )
#define TL_LEGACY_DEVICE_ID                                 ( 0x0840 )
#define TL_LEGACY_VERSION_ID                                ( 2 )
#define ZCL_ATTRIBUTE_DISCOVERY_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_DISCOVERY_CLIENT_SUPPORTED

#define ZCL_ATTRIBUTE_DISCOVERY_EXTENDED_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_DISCOVERY_EXTENDED_CLIENT_SUPPORTED

#define ZCL_COMMAND_DISCOVERY_SUPPORTED
#define ZCL_COMMAND_RECEIVED_DISCOVERY_SERVER_SUPPORTED
#define ZCL_COMMAND_RECEIVED_DISCOVERY_CLIENT_SUPPORTED
#define ZCL_COMMAND_GENERATED_DISCOVERY_SERVER_SUPPORTED
#define ZCL_COMMAND_GENERATED_DISCOVERY_CLIENT_SUPPORTED

/* Clusters used by this application */
#define CLD_BASIC
#define BASIC_SERVER
#define BASIC_CLIENT

#ifdef FULL_FUNC_DEVICE
#define CLD_ZLL_COMMISSION
#define ZLL_COMMISSION_CLIENT
#define ZLL_COMMISSION_SERVER
#define CLD_ZLL_UTILITY
#define ZLL_UTILITY_SERVER
#define ZLL_UTILITY_CLIENT
#endif


#define CLD_IDENTIFY
#define CLD_IDENTIFY_CMD_TRIGGER_EFFECT
#define IDENTIFY_CLIENT
#define IDENTIFY_SERVER
#define CLD_IDENTIFY_TICKS_PER_SECOND   10
#define CLD_IDENTIFY_SUPPORT_ZLL_ENHANCED_COMMANDS

#define CLD_GROUPS
#define GROUPS_CLIENT
#define GROUPS_SERVER

#define CLD_ONOFF
#define CLD_ONOFF_CMD_OFF_WITH_EFFECT
#define CLD_ONOFF_CMD_ON_WITH_TIMED_OFF
#define ONOFF_CLIENT

//#define ONOFF_SERVER
#define CLD_ONOFF_SUPPORT_ZLL_ENHANCED_COMMANDS

#define NUM_ENDPOINT_RECORDS                                  10
#define NUM_GROUP_RECORDS                                     4

/* Enable reports to be configured on remote servers and responses to be received */
#define ZCL_CONFIGURE_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
#define ZCL_ATTRIBUTE_REPORTING_CLIENT_SUPPORTED
#define ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_CLIENT_SUPPORTED

/****************************************************************************/
/*             Basic Cluster - Optional Attributes                          */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the basic cluster.                                         */
/****************************************************************************/

#define ZCL_ATTRIBUTE_READ_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_READ_CLIENT_SUPPORTED
#define ZCL_ATTRIBUTE_WRITE_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_WRITE_CLIENT_SUPPORTED


//#define   CLD_BAS_ATTR_LOCATION_DESCRIPTION
//#define   CLD_BAS_ATTR_PHYSICAL_ENVIRONMENT
//#define   CLD_BAS_ATTR_DEVICE_ENBLED
//#define   CLD_BAS_ATTR_ALARM_MASK

#define CLD_BAS_ATTR_APPLICATION_VERSION
#define CLD_BAS_ATTR_STACK_VERSION
#define CLD_BAS_ATTR_HARDWARE_VERSION
#define CLD_BAS_ATTR_MANUFACTURER_NAME
#define CLD_BAS_ATTR_MODEL_IDENTIFIER
#define CLD_BAS_ATTR_DATE_CODE
#define CLD_BAS_ATTR_SW_BUILD_ID
#define CLD_BAS_ATTR_CONFIG_MODEL_IDENTIFIER

// #define CLD_BAS_ATTR_MANUFACTURER_VERSION_DETAILS
// #define CLD_BAS_ATTR_SERIAL_NUMBER
// #define CLD_BAS_ATTR_PRODUCT_LABEL

#define CLD_BAS_APP_VERSION             (1)
#define CLD_BAS_STACK_VERSION           (1)
#define CLD_BAS_HARDWARE_VERSION        (1)
#define CLD_BAS_MANUF_NAME_SIZE         (3)
#define CLD_BAS_MODEL_ID_SIZE           (17)
#define CLD_BAS_DATE_SIZE               (8)
#define CLD_BAS_POWER_SOURCE            E_CLD_BAS_PS_BATTERY
#define CLD_BAS_SW_BUILD_SIZE           (9)
#define CLD_BAS_CONFIG_MODEL_ID_SIZE    CLD_BAS_MODEL_ID_SIZE
#define CLD_BAS_MANUFACTURER_VERSION_SIZE  (18)
#define CLD_BAS_SERIAL_NUMBER_SIZE  (4)
#define CLD_BAS_PRODUCT_LABEL_SIZE  (13)

/* ZLL attribute in on off cluster */
#define CLD_ONOFF_ATTR_GLOBAL_SCENE_CONTROL
#define CLD_ONOFF_ATTR_ON_TIME
#define CLD_ONOFF_ATTR_OFF_WAIT_TIME

#define ZLL_DISABLE_DEFAULT_RESPONSES FALSE
/* WARNING -- Need to setup bugger apdus and fragmentation to enable ASC */
//#define CLD_ASC_LOG
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
PUBLIC void* psGetDeviceTable(void);
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ZCL_OPTIONS_H */
