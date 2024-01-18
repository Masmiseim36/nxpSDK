/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_VCP_H__
#define __CONFIG_VCP_H__

//##################### Volume Control Service #####################

/*! @brief Volume Control Profile Volume Renderer Support
 * This option enables support for Volume Control Profile Volume Renderer
 * role and the Volume Control Service.
 */
#ifndef CONFIG_BT_VCP_VOL_REND
    #define CONFIG_BT_VCP_VOL_REND 0
#endif

#if (defined(CONFIG_BT_VCP_VOL_REND) && (CONFIG_BT_VCP_VOL_REND > 0))

    #if (defined(CONFIG_BT_GATT_DYNAMIC_DB) && (CONFIG_BT_GATT_DYNAMIC_DB > 0))
        /* PASS */
    #else
        #error CONFIG_BT_VCP_VOL_REND depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

#endif

#if (defined(CONFIG_BT_VCP_VOL_REND) && (CONFIG_BT_VCP_VOL_REND > 0))

/*! @brief Volume Offset Control Service instance count
 * This option sets the number of instances of Volume Offset Control Services.
 *
 * Valid range 0 ~ CONFIG_BT_VOCS_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_VCP_VOL_REND_VOCS_INSTANCE_COUNT
    #define CONFIG_BT_VCP_VOL_REND_VOCS_INSTANCE_COUNT 0
#endif

/* This hidden option makes it possible to easily check if VOCS is enabled for VCS. */
#if (defined(CONFIG_BT_VCP_VOL_REND_VOCS_INSTANCE_COUNT) && (CONFIG_BT_VCP_VOL_REND_VOCS_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VCP_VOL_REND_VOCS
        #define CONFIG_BT_VCP_VOL_REND_VOCS 1
    #endif
#endif /* CONFIG_BT_VCP_VOL_REND_VOCS_INSTANCE_COUNT */

/*! @brief Audio Input Control Service instance count for VCS
 * This option sets the number of instances of Audio Input Control Services for VCS.
 *
 * Valid range 0 ~ CONFIG_BT_AICS_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_VCP_VOL_REND_AICS_INSTANCE_COUNT
    #define CONFIG_BT_VCP_VOL_REND_AICS_INSTANCE_COUNT 0
#endif

/* This hidden option makes it possible to easily check if AICS is enabled for VCS. */
#if (defined(CONFIG_BT_VCP_VOL_REND_AICS_INSTANCE_COUNT) && (CONFIG_BT_VCP_VOL_REND_AICS_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VCP_VOL_REND_AICS
        #define CONFIG_BT_VCP_VOL_REND_AICS 1
    #endif
#endif

/*! @brief Volume Flags notifiable support
 * This option enables support for clients to subscribe for notifications
 * on the Volume Flags characteristic.
 */
#ifndef CONFIG_BT_VCP_VOL_REND_VOL_FLAGS_NOTIFIABLE
    #define CONFIG_BT_VCP_VOL_REND_VOL_FLAGS_NOTIFIABLE 0
#endif

#endif /* CONFIG_BT_VCP_VOL_REND */

//##################### Volume Control Profile Client #####################

/*! @brief Volume Control Profile Volume Controller Support
 * This option enables support for Volume Control Profile Volume Controller.
 */
#ifndef CONFIG_BT_VCP_VOL_CTLR
    #define CONFIG_BT_VCP_VOL_CTLR 0
#endif

#if (defined(CONFIG_BT_VCP_VOL_CTLR) && (CONFIG_BT_VCP_VOL_CTLR > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_VCP_VOL_CTLR depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_VCP_VOL_CTLR depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_VCP_VOL_CTLR */

#if (defined(CONFIG_BT_VCP_VOL_CTLR) && (CONFIG_BT_VCP_VOL_CTLR > 0))

/*! @brief Maximum number of VOCS instances to setup
 * Sets the maximum number of Volume Offset Control Service (VOCS)
 * instances to setup and use.
 *
 * Valid range 0 ~ CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT
 */
#ifndef CONFIG_BT_VCP_VOL_CTLR_MAX_VOCS_INST
    #define CONFIG_BT_VCP_VOL_CTLR_MAX_VOCS_INST 0
#endif

/* This hidden option makes it possible to easily check if VOCS is enabled for VCS client. */
#if (defined(CONFIG_BT_VCP_VOL_CTLR_MAX_VOCS_INST) && (CONFIG_BT_VCP_VOL_CTLR_MAX_VOCS_INST > 0))
    #ifndef CONFIG_BT_VCP_VOL_CTLR_VOCS
        #define CONFIG_BT_VCP_VOL_CTLR_VOCS 1
    #endif
#endif

/*! @brief Maximum number of AICS instances to setup
 * Sets the maximum number of Audio Input Control Service (AICS)
 * instances to setup and use.
 *
 * Valid range 0 ~ 3
 */
#ifndef CONFIG_BT_VCP_VOL_CTLR_MAX_AICS_INST
    #define CONFIG_BT_VCP_VOL_CTLR_MAX_AICS_INST 0
#endif

/* This hidden option makes it possible to easily check if AICS is enabled for VCS client. */
#if (defined(CONFIG_BT_VCP_VOL_CTLR_MAX_AICS_INST) && (CONFIG_BT_VCP_VOL_CTLR_MAX_AICS_INST > 0))
    #ifndef CONFIG_BT_VCP_VOL_CTLR_AICS
        #define CONFIG_BT_VCP_VOL_CTLR_AICS 1
    #endif
#endif

#endif /* CONFIG_BT_VCP_VOL_CTLR */

#endif /* __CONFIG_VCP_H__ */