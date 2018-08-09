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

/****************************************************************************/
/*                      ZCL Specific initialization                         */
/****************************************************************************/
/* This is the NXP manufacturer code.If creating new a manufacturer         */
/* specific command apply to the Zigbee alliance for an Id for your company */
/* Also update the manufacturer code in .zpscfg: Node Descriptor->misc      */
#define ZCL_MANUFACTURER_CODE                                0x1037

/* Number of endpoints supported by this device */
#define ZCL_NUMBER_OF_ENDPOINTS                              1

/* ZCL has all cooperative task */
#define COOPERATIVE

/* Set this Tue to disable non error default responses from clusters */
#define ZCL_DISABLE_DEFAULT_RESPONSES       (TRUE)
#define ZCL_DISABLE_APS_ACK                 (FALSE)


/* Which Custom commands needs to be supported */
#define ZCL_ATTRIBUTE_READ_SERVER_SUPPORTED
#define ZCL_ATTRIBUTE_WRITE_SERVER_SUPPORTED

#define ZCL_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#define ZCL_CONFIGURE_ATTRIBUTE_REPORTING_SERVER_SUPPORTED
#define ZCL_READ_ATTRIBUTE_REPORTING_CONFIGURATION_SERVER_SUPPORTED

/* Reporting related configuration */
enum
{
 REPORT_ONOFF_SLOT = 0,
 NUMBER_OF_REPORTS
};

#define ZCL_NUMBER_OF_REPORTS     NUMBER_OF_REPORTS
#define MIN_REPORT_INTERVAL       1
#define MAX_REPORT_INTERVAL       0x3d

/* Enable wild card profile */
#define ZCL_ALLOW_WILD_CARD_PROFILE
/****************************************************************************/
/*                             Enable Cluster                               */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to enable         */
/* cluster and their client or server instances                             */
/****************************************************************************/
#define CLD_BASIC
#define BASIC_SERVER

#define CLD_IDENTIFY
#define IDENTIFY_SERVER

#define CLD_GROUPS
#define GROUPS_SERVER

#define CLD_ONOFF
#define ONOFF_SERVER

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


#define CLD_BAS_APP_VERSION         (1)
#define CLD_BAS_STACK_VERSION       (1)
#define CLD_BAS_HARDWARE_VERSION    (1)
#define CLD_BAS_MANUF_NAME_SIZE     (3)
#define CLD_BAS_MODEL_ID_SIZE       (10)
#define CLD_BAS_DATE_SIZE           (8)
#define CLD_BAS_POWER_SOURCE        E_CLD_BAS_PS_SINGLE_PHASE_MAINS
#define CLD_BAS_SW_BUILD_SIZE       (9)

/****************************************************************************/
/*             Identify - Optional Attributes                               */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the identify cluster.                                      */
/****************************************************************************/
#define CLD_IDENTIFY_CMD_TRIGGER_EFFECT
#define CLD_IDENTIFY_10HZ_TICK
/****************************************************************************/
/*             Groups Cluster - Optional Attributes                         */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the scenes cluster.                                        */
/****************************************************************************/
#define CLD_GROUPS_MAX_NUMBER_OF_GROUPS                     16
#define CLD_GROUPS_DISABLE_NAME_SUPPORT

/****************************************************************************/
/*             OnOff Control Cluster - Optional Attributes                 */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* attributes to the time cluster.                                          */
/****************************************************************************/

/****************************************************************************/
/*             Basic Cluster - Optional Commands                            */
/*                                                                          */
/* Add the following #define's to your zcl_options.h file to add optional   */
/* commands to the basic cluster.                                           */
/****************************************************************************/
#define CLD_BAS_CMD_RESET_TO_FACTORY_DEFAULTS

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
    #define OTA_STRING_COMPARE  
    #define OTA_MAX_IMAGES_PER_ENDPOINT             1
#endif
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/


/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/

#endif /* ZCL_OPTIONS_H */
