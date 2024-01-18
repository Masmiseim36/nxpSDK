/*
 * Copyright 2022 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_OTS_H__
#define __CONFIG_OTS_H__

/*! @brief Object Transfer Service (OTS)
 * Enable Object Transfer Service.
 */
#ifndef CONFIG_BT_OTS
    #define CONFIG_BT_OTS 0
#endif

#if (defined(CONFIG_BT_OTS) && (CONFIG_BT_OTS > 0))

    #if (!defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) || !(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
        #error CONFIG_BT_OTS depends on CONFIG_BT_L2CAP_DYNAMIC_CHANNEL.
    #endif

    #if (!defined(CONFIG_BT_GATT_DYNAMIC_DB) || !(CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_OTS depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

    #if (!defined(CONFIG_BT_SMP) || !(CONFIG_BT_SMP > 0))
        #error CONFIG_BT_OTS depends on CONFIG_BT_SMP.
    #endif

#endif /* CONFIG_BT_OTS */

#if (defined(CONFIG_BT_OTS) && (CONFIG_BT_OTS > 0))

/*! @brief Enables the Directory Listing Object
 * Enables the Directory Listing Object, which is an object that contains all the metadata
 * from all other objects, for easy exposure to a client. Enabling this will use one of the
 * objects given by BT_OTS_MAX_OBJ_CNT.
 */
#ifndef CONFIG_BT_OTS_DIR_LIST_OBJ
    #define CONFIG_BT_OTS_DIR_LIST_OBJ 0
#endif

/*! @brief The object name of the Directory Listing Object
 * The name of the Directory Listing Object when it is read by a client.
 */
#if (defined(CONFIG_BT_OTS_DIR_LIST_OBJ) && (CONFIG_BT_OTS_DIR_LIST_OBJ > 0))
    #ifndef CONFIG_BT_OTS_DIR_LIST_OBJ_NAME
        #define CONFIG_BT_OTS_DIR_LIST_OBJ_NAME "Directory"
    #endif
#endif

/*! @brief Maximum number of available OTS instances
 * 
 * Valid range 1 ~ 1 if !CONFIG_BT_OTS_SECONDARY_SVC
 */
#ifndef CONFIG_BT_OTS_MAX_INST_CNT
    #define CONFIG_BT_OTS_MAX_INST_CNT 1
#endif

/*! @brief Register OTS as Secondary Service
 */
#ifndef CONFIG_BT_OTS_SECONDARY_SVC
    #define CONFIG_BT_OTS_SECONDARY_SVC 0
#endif

/*! @brief Support OACP Create Operation
 */
#ifndef CONFIG_BT_OTS_OACP_CREATE_SUPPORT
    #define CONFIG_BT_OTS_OACP_CREATE_SUPPORT 0
#endif

#if (defined(CONFIG_BT_OTS_OACP_CREATE_SUPPORT) && (CONFIG_BT_OTS_OACP_CREATE_SUPPORT > 0))

    #if (!defined(CONFIG_BT_OTS_OACP_WRITE_SUPPORT) || !(CONFIG_BT_OTS_OACP_WRITE_SUPPORT > 0))
        #error CONFIG_BT_OTS_OACP_CREATE_SUPPORT depends on CONFIG_BT_OTS_OACP_WRITE_SUPPORT.
    #endif

    #if (!defined(CONFIG_BT_OTS_OBJ_NAME_WRITE_SUPPORT) || !(CONFIG_BT_OTS_OBJ_NAME_WRITE_SUPPORT > 0))
        #error CONFIG_BT_OTS_OACP_CREATE_SUPPORT depends on CONFIG_BT_OTS_OBJ_NAME_WRITE_SUPPORT.
    #endif

#endif /* CONFIG_BT_OTS_OACP_CREATE_SUPPORT */

/*! @brief Support OACP Calculate Checksum operation
 */
#ifndef CONFIG_BT_OTS_OACP_CHECKSUM_SUPPORT
    #define CONFIG_BT_OTS_OACP_CHECKSUM_SUPPORT 0
#endif

/*! @brief Support OACP Delete Operation
 */
#ifndef CONFIG_BT_OTS_OACP_DELETE_SUPPORT
    #define CONFIG_BT_OTS_OACP_DELETE_SUPPORT 0
#endif

/*! @brief Support OACP Read Operation
 */
#ifndef CONFIG_BT_OTS_OACP_READ_SUPPORT
    #define CONFIG_BT_OTS_OACP_READ_SUPPORT 1
#endif

/*! @brief Support OACP Write Operation
 */
#ifndef CONFIG_BT_OTS_OACP_WRITE_SUPPORT
    #define CONFIG_BT_OTS_OACP_WRITE_SUPPORT 0
#endif

/*! @brief Support patching of objects
 */
#ifndef CONFIG_BT_OTS_OACP_PATCH_SUPPORT
    #define CONFIG_BT_OTS_OACP_PATCH_SUPPORT 0
#endif

#if (defined(CONFIG_BT_OTS_OACP_PATCH_SUPPORT) && (CONFIG_BT_OTS_OACP_PATCH_SUPPORT > 0))

    #if (!defined(CONFIG_BT_OTS_OACP_WRITE_SUPPORT) || !(CONFIG_BT_OTS_OACP_WRITE_SUPPORT > 0))
        #error CONFIG_BT_OTS_OACP_PATCH_SUPPORT depends on CONFIG_BT_OTS_OACP_WRITE_SUPPORT.
    #endif

#endif /* CONFIG_BT_OTS_OACP_PATCH_SUPPORT */

/*! @brief Support OLCP Go To Operation
 */
#ifndef CONFIG_BT_OTS_OLCP_GO_TO_SUPPORT
    #define CONFIG_BT_OTS_OLCP_GO_TO_SUPPORT 1
#endif

/*! @brief Support object name write
 */
#ifndef CONFIG_BT_OTS_OBJ_NAME_WRITE_SUPPORT
    #define CONFIG_BT_OTS_OBJ_NAME_WRITE_SUPPORT 0
#endif

#endif /* CONFIG_BT_OTS */

//############## Object Transfer Service Client ######################

/*! @brief Object Transfer Service Client Support
 * This option enables support for the Object Transfer Service Client.
 */
#ifndef CONFIG_BT_OTS_CLIENT
    #define CONFIG_BT_OTS_CLIENT 0
#endif

#if (defined(CONFIG_BT_OTS_CLIENT) && (CONFIG_BT_OTS_CLIENT > 0))

    #if (!defined(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL) || !(CONFIG_BT_L2CAP_DYNAMIC_CHANNEL > 0))
        #error CONFIG_BT_OTS_CLIENT depends on CONFIG_BT_L2CAP_DYNAMIC_CHANNEL.
    #endif

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_OTS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_OTS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_OTS_CLIENT */

#if (defined(CONFIG_BT_OTS) && (CONFIG_BT_OTS > 0)) || \
    (defined(CONFIG_BT_OTS_CLIENT) && (CONFIG_BT_OTS_CLIENT > 0))

/*! @brief Maximum number of objects that each service instance can store
 * 
 * Valid range 0x02 ~ 0x17D if CONFIG_BT_OTS_DIR_LIST_OBJ
 * Valid range 0x01 ~ 0xFFFFFFFFFFFFFEFF
 */
#ifndef CONFIG_BT_OTS_MAX_OBJ_CNT
    #define CONFIG_BT_OTS_MAX_OBJ_CNT 0x05
#endif

/*! @brief Size of TX MTU for Object Transfer Channel
 *
 * Valid range 21 ~ 65533
 */
#ifndef CONFIG_BT_OTS_L2CAP_CHAN_TX_MTU
    #define CONFIG_BT_OTS_L2CAP_CHAN_TX_MTU 256
#endif

/*! @brief Size of RX MTU for Object Transfer Channel
 *
 * Valid range 21 ~ CONFIG_BT_BUF_ACL_RX_SIZE
 */
#ifndef CONFIG_BT_OTS_L2CAP_CHAN_RX_MTU
    #define CONFIG_BT_OTS_L2CAP_CHAN_RX_MTU CONFIG_BT_BUF_ACL_RX_SIZE
#endif

/*! @brief Maximum object name length
 *
 * Valid range 1 ~ 120
 */
#ifndef CONFIG_BT_OTS_OBJ_MAX_NAME_LEN
    #define CONFIG_BT_OTS_OBJ_MAX_NAME_LEN 120
#endif

#endif /* CONFIG_BT_OTS || CONFIG_BT_OTS_CLIENT */

#endif /* __CONFIG_OTS_H__ */