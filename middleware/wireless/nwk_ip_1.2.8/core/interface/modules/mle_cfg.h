/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef  _MLE_CFG_H
#define  _MLE_CFG_H

/*!
\file       mle_cfg.h

\brief      This is a configuration header file for the Path MTU Discovery module.

\details    This file contains the following configuration options:

            MLE_SECURITY_ENABLED                                0 | 1   (default is 1)

            MLE_AUTO_REGISTRATION_TO_SESSION_TASK_ENABLED       0 | 1   (default is 1)
            

            MLE_CMD_LINK_REQ_ENABLED                            0 | 1   (default is 1)
            MLE_CMD_LINK_ACCEPT_ENABLED                         0 | 1   (default is 1)
            MLE_CMD_LINK_ACCEPT_REQ_ENABLED                     0 | 1   (default is 1)
            MLE_CMD_LINK_REJECT_ENABLED                         0 | 1   (default is 1)
            MLE_CMD_ADV_ENABLED                                 0 | 1   (default is 1)
            MLE_CMD_UPDATE_ENABLED                              0 | 1   (default is 1)
            MLE_CMD_UPDATE_REQ_ENABLED                          0 | 1   (default is 1)
            MLE_CMD_DATA_REQ_ENABLED                            0 | 1   (default is 1)
            MLE_CMD_DATA_REQ_ENABLED                            0 | 1   (default is 1)
            MLE_CMD_DATA_RES_ENABLED                            0 | 1   (default is 1)

            MLE_TLV_SRC_ADDR_ENABLED                            0 | 1   (default is 1)
            MLE_TLV_MODE_ENABLED                                0 | 1   (default is 1)
            MLE_TLV_TIMEOUT_ENABLED                             0 | 1   (default is 1)
            MLE_TLV_CHALLENGE_ENABLED                           0 | 1   (default is 1)
            MLE_TLV_RESPONSE_ENABLED                            0 | 1   (default is 1)
            MLE_TLV_LL_FRAME_COUNTER_ENABLED                    0 | 1   (default is 1)
            MLE_TLV_LINK_QUALITY_ENABLED                        0 | 1   (default is 1)
            MLE_TLV_NWK_PARAM_ENABLED                           0 | 1   (default is 1)
            MLE_TLV_MLE_FRAME_CTR_ENABLED                       0 | 1   (default is 1)

            MLE_TLV_ROUTE_ENABLED                               0 | 1   (default is 1)
            MLE_TLV_SHORT_ADDR_ENABLED                          0 | 1   (default is 1)
            MLE_TLV_LEADER_DATA_ENABLED                         0 | 1   (default is 1)
            MLE_TLV_NWK_DATA_ENABLED                            0 | 1   (default is 1)
            MLE_TLV_SERVER_DATA_ENABLED                         0 | 1   (default is 1)
            MLE_TLV_REQUEST_ENABLED                             0 | 1   (default is 1)
            MLE_TLV_PREFIX_ENABLED                              0 | 1   (default is 1)
            MLE_TLV_CONTEXT_ID_ENABLED                          0 | 1   (default is 1)
            MLE_TLV_DHCP_SERVER_ENABLED                         0 | 1   (default is 1)
*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include "stack_config.h"

/*==================================================================================================
Public macros
==================================================================================================*/

/*! Description of the MLE_SECURITY_ENABLED configuration option: set to 1 to enable
    the MLE security */
#ifndef MLE_SECURITY_ENABLED
    #define MLE_SECURITY_ENABLED 1
#endif

/*! Description of the MLE_CMD_LINK_REQ_ENABLED configuration option: set to 1 to enable
    the MLE auto registration to session task */
#ifndef MLE_AUTO_REGISTRATION_TO_SESSION_TASK_ENABLED
    #define MLE_AUTO_REGISTRATION_TO_SESSION_TASK_ENABLED 1
#endif

/****************/
/* MLE Commands */
/****************/

/*! Description of the MLE_CMD_LINK_REQ_ENABLED configuration option: set to 1 to enable
    the MLE command link request */
#ifndef MLE_CMD_LINK_REQ_ENABLED
    #define MLE_CMD_LINK_REQ_ENABLED 1
#endif

/*! Description of the MLE_CMD_LINK_ACCEPT_ENABLED configuration option: set to 1 to enable
    the MLE command link accept */
#ifndef MLE_CMD_LINK_ACCEPT_ENABLED
    #define MLE_CMD_LINK_ACCEPT_ENABLED 1
#endif

/*! Description of the MLE_CMD_LINK_ACCEPT_REQ_ENABLED configuration option: set to 1 to enable
    the command link accept request */
#ifndef MLE_CMD_LINK_ACCEPT_REQ_ENABLED
    #define MLE_CMD_LINK_ACCEPT_REQ_ENABLED 1
#endif

/*! Description of the MLE_CMD_LINK_REJECT_ENABLED configuration option: set to 1 to enable
    the MLE command link reject */
#ifndef MLE_CMD_LINK_REJECT_ENABLED
    #define MLE_CMD_LINK_REJECT_ENABLED 1
#endif

/*! Description of the MLE_CMD_ADV_ENABLED configuration option: set to 1 to enable
    the MLE command advertisement */
#ifndef MLE_CMD_ADV_ENABLED
    #define MLE_CMD_ADV_ENABLED 1
#endif

/*! Description of the MLE_CMD_UPDATE_ENABLED configuration option: set to 1 to enable
    the MLE command update */
#ifndef MLE_CMD_UPDATE_ENABLED
    #define MLE_CMD_UPDATE_ENABLED 1
#endif

/*! Description of the MLE_CMD_UPDATE_REQ_ENABLED configuration option: set to 1 to enable
    the MLE command update request */
#ifndef MLE_CMD_UPDATE_REQ_ENABLED
    #define MLE_CMD_UPDATE_REQ_ENABLED 1
#endif

/*! Description of the MLE_CMD_DATA_REQ_ENABLED configuration option: set to 1 to enable
    the MLE command data request */
#ifndef MLE_CMD_DATA_REQ_ENABLED
    #define MLE_CMD_DATA_REQ_ENABLED 1
#endif

/*! Description of the MLE_CMD_DATA_RES_ENABLED configuration option: set to 1 to enable
    the MLE command */
#ifndef MLE_CMD_DATA_RES_ENABLED
    #define MLE_CMD_DATA_RES_ENABLED 1
#endif

/***************/
/* TLV Options */
/***************/

/*! Description of the MLE_TLV_SRC_ADDR_ENABLED configuration option: set to 1 to enable
    the MLE TLV source address */
#ifndef MLE_TLV_SRC_ADDR_ENABLED
    #define MLE_TLV_SRC_ADDR_ENABLED 1
#endif

/*! Description of the MLE_TLV_MODE_ENABLED configuration option: set to 1 to enable
    the MLE TLV mode */
#ifndef MLE_TLV_MODE_ENABLED
    #define MLE_TLV_MODE_ENABLED 1
#endif

/*! Description of the MLE_TLV_TIMEOUT_ENABLED configuration option: set to 1 to enable
    the MLE TLV timeout */
#ifndef MLE_TLV_TIMEOUT_ENABLED
    #define MLE_TLV_TIMEOUT_ENABLED 1
#endif

/*! Description of the MLE_TLV_CHALLENGE_ENABLED configuration option: set to 1 to enable
    the MLE TLV challenge */
#ifndef MLE_TLV_CHALLENGE_ENABLED
    #define MLE_TLV_CHALLENGE_ENABLED 1
#endif

/*! Description of the MLE_TLV_RESPONSE_ENABLED configuration option: set to 1 to enable
    the MLE TLV response */
#ifndef MLE_TLV_RESPONSE_ENABLED
    #define MLE_TLV_RESPONSE_ENABLED 1
#endif

/*! Description of the MLE_TLV_LL_FRAME_COUNTER_ENABLED configuration option: set to 1 to enable
    the MLE TLV link layer frame counter */
#ifndef MLE_TLV_LL_FRAME_COUNTER_ENABLED
    #define MLE_TLV_LL_FRAME_COUNTER_ENABLED 1
#endif

/*! Description of the MLE_TLV_LINK_QUALITY_ENABLED configuration option: set to 1 to enable
    the MLE TLV link quality */
#ifndef MLE_TLV_LINK_QUALITY_ENABLED
    #define MLE_TLV_LINK_QUALITY_ENABLED 1
#endif

/*! Description of the MLE_TLV_NWK_PARAM_ENABLED configuration option: set to 1 to enable
    the MLE TLV network parameters */
#ifndef MLE_TLV_NWK_PARAM_ENABLED
    #define MLE_TLV_NWK_PARAM_ENABLED 1
#endif

/*! Description of the MLE_TLV_MLE_FRAME_CTR_ENABLED configuration option: set to 1 to enable
    the MLE TLV frame counter */
#ifndef MLE_TLV_MLE_FRAME_CTR_ENABLED
    #define MLE_TLV_MLE_FRAME_CTR_ENABLED 1
#endif

/*! Description of the MLE_TLV_ROUTE_ENABLED configuration option: set to 1 to enable
    the MLE TLV route */
#ifndef MLE_TLV_ROUTE_ENABLED
    #define MLE_TLV_ROUTE_ENABLED 1
#endif

/*! Description of the MLE_TLV_SHORT_ADDR_ENABLED configuration option: set to 1 to enable
    the MLE TLV short address */
#ifndef MLE_TLV_SHORT_ADDR_ENABLED
    #define MLE_TLV_SHORT_ADDR_ENABLED 1
#endif

/*! Description of the MLE_TLV_LEADER_DATA_ENABLED configuration option: set to 1 to enable
    the MLE TLV leader data */
#ifndef MLE_TLV_LEADER_DATA_ENABLED
    #define MLE_TLV_LEADER_DATA_ENABLED 1
#endif

/*! Description of the MLE_TLV_NWK_DATA_ENABLED configuration option: set to 1 to enable
    the MLE TLV network data */
#ifndef MLE_TLV_NWK_DATA_ENABLED
    #define MLE_TLV_NWK_DATA_ENABLED 1
#endif

/*! Description of the MLE_TLV_SERVER_DATA_ENABLED configuration option: set to 1 to enable
    the MLE TLV server data */
#ifndef MLE_TLV_SERVER_DATA_ENABLED
    #define MLE_TLV_SERVER_DATA_ENABLED 1
#endif

/*! Description of the MLE_TLV_REQUEST_ENABLED configuration option: set to 1 to enable
    the MLE TLV request */
#ifndef MLE_TLV_REQUEST_ENABLED
    #define MLE_TLV_REQUEST_ENABLED 1
#endif

/*! Description of the MLE_TLV_PREFIX_ENABLED configuration option: set to 1 to enable
    the MLE TLV prefix */
#ifndef MLE_TLV_PREFIX_ENABLED
    #define MLE_TLV_PREFIX_ENABLED 1
#endif

/*! Description of the MLE_TLV_CONTEXT_ID_ENABLED configuration option: set to 1 to enable
    the MLE TLV context ID */
#ifndef MLE_TLV_CONTEXT_ID_ENABLED
    #define MLE_TLV_CONTEXT_ID_ENABLED 1
#endif

/*! Description of the MLE_TLV_DHCP_SERVER_ENABLED configuration option: set to 1 to enable
    the MLE TLV DHCP server */
#ifndef MLE_TLV_DHCP_SERVER_ENABLED
    #define MLE_TLV_DHCP_SERVER_ENABLED 1
#endif

/*==================================================================================================
Public type definitions
==================================================================================================*/

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public function prototypes
==================================================================================================*/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif
/*================================================================================================*/
#endif  /*  _MLE_CFG_H */
