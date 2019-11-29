/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
     


#ifndef _APP_ZCL_CFG_H
#define _APP_ZCL_CFG_H

/*!
\file       app_zcl_cfg.h
\brief      Application Zigbee Cluster Library Configuration File header
*/

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Profile 'ZDP' */
#define ZDP_PROFILE_ID    (0x0000)
#define ZDP_NWK_ADDR_REQ_CLUSTER_ID    (0x0000)
#define ZDP_IEEE_ADDR_REQ_CLUSTER_ID    (0x0001)
#define ZDP_NODE_DESC_REQ_CLUSTER_ID    (0x0002)
#define ZDP_POWER_DESC_REQ_CLUSTER_ID    (0x0003)
#define ZDP_SIMPLE_DESC_REQ_CLUSTER_ID    (0x0004)
#define ZDP_ACTIVE_EP_REQ_CLUSTER_ID    (0x0005)
#define ZDP_MATCH_DESC_REQ_CLUSTER_ID    (0x0006)
#define ZDP_COMPLEX_DESC_REQ_CLUSTER_ID    (0x0010)
#define ZDP_USER_DESC_REQ_CLUSTER_ID    (0x0011)
#define ZDP_DISCOVERY_CACHE_REQ_CLUSTER_ID    (0x0012)
#define ZDP_DEVICE_ANNCE_CLUSTER_ID    (0x0013)
#define ZDP_USER_DESC_SET_CLUSTER_ID    (0x0014)
#define ZDP_SYSTEM_SERVER_DISCOVERY_REQ_CLUSTER_ID    (0x0015)
#define ZDP_DISCOVERY_STORE_REQ_CLUSTER_ID    (0x0016)
#define ZDP_NODE_DESC_STORE_REQ_CLUSTER_ID    (0x0017)
#define ZDP_POWER_DESC_STORE_REQ_CLUSTER_ID    (0x0018)
#define ZDP_ACTIVE_EP_STORE_REQ_CLUSTER_ID    (0x0019)
#define ZDP_SIMPLE_DESC_STORE_REQ_CLUSTER_ID    (0x001a)
#define ZDP_REMOVE_NODE_CACHE_REQ_CLUSTER_ID    (0x001b)
#define ZDP_FIND_NODE_CHACHE_REQ_CLUSTER_ID    (0x001c)
#define ZDP_EXTENDED_SIMPLE_DESC_REQ_CLUSTER_ID    (0x001d)
#define ZDP_EXTENDED_ACTIVE_EP_REQ_CLUSTER_ID    (0x001e)
#define ZDP_PARENT_ANNCE_REQ_CLUSTER_ID    (0x001f)
#define ZDP_END_DEVICE_BIND_REQ_CLUSTER_ID    (0x0020)
#define ZDP_BIND_REQ_CLUSTER_ID    (0x0021)
#define ZDP_UNBIND_REQ_CLUSTER_ID    (0x0022)
#define ZDP_BIND_REGISTER_REQ_CLUSTER_ID    (0x0023)
#define ZDP_REPLACE_DEVICE_REQ_CLUSTER_ID    (0x0024)
#define ZDP_STORE_BKUP_BIND_ENTRY_REQ_CLUSTER_ID    (0x0025)
#define ZDP_REMOVE_BKUP_BIND_ENTRY_REQ_CLUSTER_ID    (0x0026)
#define ZDP_BACKUP_BIND_TABLE_REQ_CLUSTER_ID    (0x0027)
#define ZDP_RECOVER_BIND_TABLE_REQ_CLUSTER_ID    (0x0028)
#define ZDP_BACKUP_SOURCE_BIND_REQ_CLUSTER_ID    (0x0029)
#define ZDP_RECOVER_SOURCE_BIND_REQ_CLUSTER_ID    (0x002a)
#define ZDP_MGMT_NWK_DISC_REQ_CLUSTER_ID    (0x0030)
#define ZDP_MGMT_LQI_REQ_CLUSTER_ID    (0x0031)
#define ZDP_MGMT_RTG_REQ_CLUSTER_ID    (0x0032)
#define ZDP_MGMT_BIND_REQ_CLUSTER_ID    (0x0033)
#define ZDP_MGMT_LEAVE_REQ_CLUSTER_ID    (0x0034)
#define ZDP_MGMT_DIRECT_JOIN_REQ_CLUSTER_ID    (0x0035)
#define ZDP_MGMT_PERMIT_JOINING_REQ_CLUSTER_ID    (0x0036)
#define ZDP_MGMT_CACHE_REQ_CLUSTER_ID    (0x0037)
#define ZDP_MGMT_NWK_UPDATE_REQ_CLUSTER_ID    (0x0038)
#define ZDP_NWK_ADDR_RSP_CLUSTER_ID    (0x8000)
#define ZDP_IEEE_ADDR_RSP_CLUSTER_ID    (0x8001)
#define ZDP_NODE_DESC_RSP_CLUSTER_ID    (0x8002)
#define ZDP_POWER_DESC_RSP_CLUSTER_ID    (0x8003)
#define ZDP_SIMPLE_DESC_RSP_CLUSTER_ID    (0x8004)
#define ZDP_ACTIVE_EP_RSP_CLUSTER_ID    (0x8005)
#define ZDP_MATCH_DESC_RSP_CLUSTER_ID    (0x8006)
#define ZDP_COMPLEX_DESC_RSP_CLUSTER_ID    (0x8010)
#define ZDP_USER_DESC_RSP_CLUSTER_ID    (0x8011)
#define ZDP_DISCOVERY_CACHE_RSP_CLUSTER_ID    (0x8012)
#define ZDP_USER_DESC_CONF_CLUSTER_ID    (0x8014)
#define ZDP_SYSTEM_SERVER_DISCOVERY_RSP_CLUSTER_ID    (0x8015)
#define ZDP_DISCOVERY_STORE_RSP_CLUSTER_ID    (0x8016)
#define ZDP_NODE_DESC_STORE_RSP_CLUSTER_ID    (0x8017)
#define ZDP_POWER_DESC_STORE_RSP_CLUSTER_ID    (0x8018)
#define ZDP_ACTIVE_EP_STORE_RSP_CLUSTER_ID    (0x8019)
#define ZDP_SIMPLE_DESC_STORE_RSP_CLUSTER_ID    (0x801a)
#define ZDP_REMOVE_NODE_CACHE_RSP_CLUSTER_ID    (0x801b)
#define ZDP_FIND_NODE_CHACHE_RSP_CLUSTER_ID    (0x801c)
#define ZDP_EXTENDED_SIMPLE_DESC_RSP_CLUSTER_ID    (0x801d)
#define ZDP_EXTENDED_ACTIVE_EP_RSP_CLUSTER_ID    (0x801e)
#define ZDP_PARENT_ANNCE_RSP_CLUSTER_ID    (0x801f)
#define ZDP_END_DEVICE_BIND_RSP_CLUSTER_ID    (0x8020)
#define ZDP_BIND_RSP_CLUSTER_ID    (0x8021)
#define ZDP_UNBIND_RSP_CLUSTER_ID    (0x8022)
#define ZDP_BIND_REGISTER_RSP_CLUSTER_ID    (0x8023)
#define ZDP_REPLACE_DEVICE_RSP_CLUSTER_ID    (0x8024)
#define ZDP_STORE_BKUP_BIND_ENTRY_RSP_CLUSTER_ID    (0x8025)
#define ZDP_REMOVE_BKUP_BIND_ENTRY_RSP_CLUSTER_ID    (0x8026)
#define ZDP_BACKUP_BIND_TABLE_RSP_CLUSTER_ID    (0x8027)
#define ZDP_RECOVER_BIND_TABLE_RSP_CLUSTER_ID    (0x8028)
#define ZDP_BACKUP_SOURCE_BIND_RSP_CLUSTER_ID    (0x8029)
#define ZDP_RECOVER_SOURCE_BIND_RSP_CLUSTER_ID    (0x802a)
#define ZDP_MGMT_NWK_DISC_RSP_CLUSTER_ID    (0x8030)
#define ZDP_MGMT_LQI_RSP_CLUSTER_ID    (0x8031)
#define ZDP_MGMT_RTG_RSP_CLUSTER_ID    (0x8032)
#define ZDP_MGMT_BIND_RSP_CLUSTER_ID    (0x8033)
#define ZDP_MGMT_LEAVE_RSP_CLUSTER_ID    (0x8034)
#define ZDP_MGMT_DIRECT_JOIN_RSP_CLUSTER_ID    (0x8035)
#define ZDP_MGMT_PERMIT_JOINING_RSP_CLUSTER_ID    (0x8036)
#define ZDP_MGMT_CACHE_RSP_CLUSTER_ID    (0x8037)
#define ZDP_MGMT_NWK_UPDATE_RSP_CLUSTER_ID    (0x8038)

/* Profile 'HomeAutomation' */
#define HOMEAUTOMATION_PROFILE_ID    (0x0104)
#define HOMEAUTOMATION_BASIC_CLUSTER_ID    (0x0000)
#define HOMEAUTOMATION_IDENTIFY_CLUSTER_ID    (0x0003)
#define HOMEAUTOMATION_GROUPS_CLUSTER_ID    (0x0004)
#define HOMEAUTOMATION_SCENES_CLUSTER_ID    (0x0005)
#define HOMEAUTOMATION_ONOFF_CLUSTER_ID    (0x0006)
#define HOMEAUTOMATION_LEVELCONTROL_CLUSTER_ID    (0x0008)
#define HOMEAUTOMATION_COLOURCONTROL_CLUSTER_ID    (0x0300)
#define HOMEAUTOMATION_ZLL_COMMISSIONING_CLUSTER_ID    (0x1000)
#define HOMEAUTOMATION_DEFAULT_CLUSTER_ID    (0xffff)
#define HOMEAUTOMATION_OTA_CLUSTER_ID    (0x0019)



/* Node 'ExtendedColorLight' */

/* Endpoints */
#define EXTENDEDCOLORLIGHT_ZDO_ENDPOINT    (0)
#define EXTENDEDCOLORLIGHT_LIGHT_ENDPOINT    (1)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

#endif
