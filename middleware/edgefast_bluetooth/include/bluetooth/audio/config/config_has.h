/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_HAS_H__
#define __CONFIG_HAS_H__

/*! @brief Hearing Access Service support
 * This option enables support for Hearing Access Service.
 */
#if (defined(CONFIG_BT_HAS) && (CONFIG_BT_HAS > 0))

    #if (!defined(CONFIG_UTF8) || !(CONFIG_UTF8 > 0))
        #error CONFIG_BT_HAS depends on CONFIG_UTF8.
    #endif

    #if (!defined(CONFIG_BT_GATT_DYNAMIC_DB) || !(CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_HAS depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

    #if (!defined(CONFIG_BT_BAP_UNICAST_SERVER) || !(CONFIG_BT_BAP_UNICAST_SERVER > 0))
        #error CONFIG_BT_HAS depends on CONFIG_BT_BAP_UNICAST_SERVER.
    #endif

#endif /* CONFIG_BT_HAS */

#if (defined(CONFIG_BT_HAS) && (CONFIG_BT_HAS > 0))

/*! @brief Hearing Aid Features Notifiable Support
 * This option enables support for clients to subscribe for notifications
 * on the Hearing Aid Features characteristic.
 */
#ifndef CONFIG_BT_HAS_FEATURES_NOTIFIABLE
    #define CONFIG_BT_HAS_FEATURES_NOTIFIABLE 0
#endif

/*! @brief Preset record list size
 * This option sets the number of Hearing Access Service Presets
 * that can be registered. Setting this value to 0 disables Presets support.
 * 
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_HAS_PRESET_COUNT
    #define CONFIG_BT_HAS_PRESET_COUNT 2
#endif

#if (defined(CONFIG_BT_HAS_PRESET_COUNT) && (CONFIG_BT_HAS_PRESET_COUNT > 0))
    #ifndef CONFIG_BT_HAS_PRESET_SUPPORT
        #define CONFIG_BT_HAS_PRESET_SUPPORT 1
    #endif
#endif /* CONFIG_BT_HAS_PRESET_COUNT */

#if (defined(CONFIG_BT_HAS_PRESET_SUPPORT) && (CONFIG_BT_HAS_PRESET_SUPPORT > 0))

/*! @brief Allow to set preset name on runtime
 * Enabling this option allows for runtime configuration of preset name.
 */
#ifndef CONFIG_BT_HAS_PRESET_NAME_DYNAMIC
    #define CONFIG_BT_HAS_PRESET_NAME_DYNAMIC 0
#endif

/*! @brief Preset Control Point Notifiable support
 * This option enables support for clients to subscribe for notifications
 * on the Hearing Aid Preset Control Point characteristic.
 */
#ifndef CONFIG_BT_HAS_PRESET_CONTROL_POINT_NOTIFIABLE
    #define CONFIG_BT_HAS_PRESET_CONTROL_POINT_NOTIFIABLE 0
#endif

#if (defined(CONFIG_BT_HAS_PRESET_CONTROL_POINT_NOTIFIABLE) && (CONFIG_BT_HAS_PRESET_CONTROL_POINT_NOTIFIABLE > 0))
    #if (defined(CONFIG_BT_EATT) && (CONFIG_BT_EATT > 0))
        /* PASS */
    #else
        #error CONFIG_BT_HAS_PRESET_CONTROL_POINT_NOTIFIABLE deponds on CONFIG_BT_EATT.
    #endif
#endif /* CONFIG_BT_HAS_PRESET_CONTROL_POINT_NOTIFIABLE */

/* This option enables the Hearing Aid Active Preset Index characteristic. */
#ifndef CONFIG_BT_HAS_ACTIVE_PRESET_INDEX
    #define CONFIG_BT_HAS_ACTIVE_PRESET_INDEX 1
#endif

#endif /* CONFIG_BT_HAS_PRESET_SUPPORT */

#endif /* CONFIG_BT_HAS */

/*! @brief Hearing Access Service debug
 * This option enables enables Hearing Access Service debug logs.
 */
#ifndef CONFIG_BT_HAS_CLIENT
    #define CONFIG_BT_HAS_CLIENT 0
#endif

#if (defined(CONFIG_BT_HAS_CLIENT) && (CONFIG_BT_HAS_CLIENT > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_UPDATE_MTU) || !(CONFIG_BT_GATT_AUTO_UPDATE_MTU > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_BT_GATT_AUTO_UPDATE_MTU.
    #endif

    #if (!defined(CONFIG_UTF8) || !(CONFIG_UTF8 > 0))
        #error CONFIG_BT_HAS_CLIENT depends on CONFIG_UTF8.
    #endif

#endif /* CONFIG_BT_HAS_CLIENT */

#endif /* __CONFIG_HAS_H__ */