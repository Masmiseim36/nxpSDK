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

#ifndef _zps_nwk_config_h_
#define _zps_nwk_config_h_

/*!
\file       zps_nwk_config.h
\brief      ZPS NWK Configuration header
*/
     
#ifdef __cplusplus
extern "C" {
#endif

/***********************/
/**** INCLUDE FILES ****/
/***********************/
/************************/
/**** MACROS/DEFINES ****/
/************************/

#ifndef ZPS_NWK_OPT_IN_MAKE


/* Define this for ZigBee feature set, leave undefined for ZigBee PRO feature set (the default) */
//#define ZPS_NWK_OPT_FS_ZIGBEE_FS

/* Default feature set options */

/* Define this if direct join is needed to be supported through primitives */
#define ZPS_NWK_OPT_FS_DIRECT_JOIN
#define ZPS_NWK_OPT_FS_ED_SCAN

//#define ZPS_NWK_OPT_FS_DYNAMIC_SEC_LVL

/*
 * Define this if orphaning should be handled strictly as expressed in 802.15.4-2003
 * In this case, an orphan response should always be sent, even if child wasn't associated
 */
//#define ZPS_NWK_OPT_ORPHAN_STRICT_15_4

/*
 * Define this if the NHLE should be notified if an orphan indication is received and device
 * can't be found. This would allow ZDO to perform some higher level management potentially
 */
//#define ZPS_NWK_OPT_ORPHAN_NHLE_NOTIFY

/*
 * Define this if a parent should send a NLME-NWK-STATUS.indication to the higher layer
 * indicating an address update.
 */
//#define ZPS_NWK_OPT_PARENT_SEND_ADDR_UPDATE_IND

/*
 * Define this if a device changing its own or its ZED child's address should also
 * send a network status showing address conflict.
 *
 */
//#define ZPS_NWK_OPT_LOCAL_CHILD_ADDR_CNFL_SEND_NWK_STATUS_CMD

/*
 * Define this if sibling routers should be added to the neighbour table if heard
 */
//#define ZPS_NWK_OPT_ADD_SIBLING_ROUTERS

/*
 * Define this if PAN ID conflict processing is compiled in
 */
#define ZPS_NWK_OPT_PAN_ID_CNFL

/*
 * Define this if Rejoin discovering is done
 */


/**
 * Device capability
 */

/**
 * Hard-coded stack profile, set all parameters at compile time.
 * The alternative is to hard code all stack profiles parameters in a table, then to
 * hard code the stack profile itself for the JC and to communicate it via the beacon
 * for all devices tree order >1
 */

/**
 * Topology limits
 * These are needed to calculate the magical CSkip formula
 */
#ifdef ZPS_NWK_OPT_FS_ZIGBEE_FS
#define ZPS_NWK_OPT_FS_MAX_DEPTH          5   /**< How deep a network is */
#else
#define ZPS_NWK_OPT_FS_MAX_DEPTH          15  /**< How deep a network is */
#endif
#define ZPS_NWK_OPT_FS_MAX_CHILDREN       20  /**< How many children a ZC/ZR can have - ZigBee FS only */
#define ZPS_NWK_OPT_FS_MAX_ROUTERS        6  /**< How many of the children can be routers */

/**
 * Routing table size
 */
#define ZPS_NWK_OPT_FS_RT_SIZE            8  /**< Size of the routing table */

#endif /* ZPS_NWK_OPT_IN_MAKE */

/* Bitmap of capabilities */
#define ZPS_NWK_OPT_CAP_COORD             4      /**< Enables coordinator functionality */
#define ZPS_NWK_OPT_CAP_ROUTER            2      /**< Enables router functionality */
#define ZPS_NWK_OPT_CAP_END_DEVICE        1      /**< Enables end device functionality */
#define ZPS_NWK_OPT_CAP_CRE               (ZPS_NWK_OPT_CAP_COORD | ZPS_NWK_OPT_CAP_ROUTER | ZPS_NWK_OPT_CAP_END_DEVICE)
#define ZPS_NWK_OPT_CAP_CR                (ZPS_NWK_OPT_CAP_COORD | ZPS_NWK_OPT_CAP_ROUTER)
#define ZPS_NWK_OPT_CAP_RE                (ZPS_NWK_OPT_CAP_ROUTER | ZPS_NWK_OPT_CAP_END_DEVICE)
#define ZPS_NWK_OPT_CAP_E                 ZPS_NWK_OPT_CAP_END_DEVICE

#ifndef ZPS_NWK_OPT_CAPABILITY
#define ZPS_NWK_OPT_CAPABILITY            (ZPS_NWK_OPT_CAP_COORD | ZPS_NWK_OPT_CAP_ROUTER | ZPS_NWK_OPT_CAP_END_DEVICE)
//#define ZPS_NWK_OPT_CAPABILITY            (ZPS_NWK_OPT_CAP_END_DEVICE)
#endif

/**
 * @name NIB table sizes
 * @ingroup g_zps_nwk_nib
 * Suggested table sizes in the NIB
 * @{
 */
#define ZPS_NWK_OPT_NUM_DISC_NT_ENTRIES    16
#define ZPS_NWK_OPT_NUM_ACTV_NT_ENTRIES    (ZPS_NWK_OPT_FS_MAX_CHILDREN + 1) /* Could be more if mesh */
#define ZPS_NWK_OPT_NUM_RT_DISC_ENTRIES    16
#define ZPS_NWK_OPT_NUM_RT_ENTRIES         16
#define ZPS_NWK_OPT_NUM_BTT_ENTRIES        9 /* according to feature set */
#define ZPS_NWK_OPT_NUM_RCT_ENTRIES        4 /* Config */
#define ZPS_NWK_OPT_NUM_ADDR_MAP_ENTRIES   16
#define ZPS_NWK_OPT_NUM_IN_FC_SETS         ZPS_NWK_OPT_NUM_ACTV_NT_ENTRIES /* Equal to Number of NT Entries */
#define ZPS_NWK_OPT_NUM_SEC_MATERIAL_SETS  2 /* according to feature set */

/**************************/
/**** TYPE DEFINITIONS ****/
/**************************/

/****************************/
/**** EXPORTED VARIABLES ****/
/****************************/

/****************************/
/**** EXPORTED FUNCTIONS ****/
/****************************/

#ifdef __cplusplus
};
#endif

#endif /* _zps_nwk_config_h_ */

/* End of file $Id: zps_nwk_config.h 80693 2016-06-20 08:48:06Z nxp29741 $ *******************************************/
