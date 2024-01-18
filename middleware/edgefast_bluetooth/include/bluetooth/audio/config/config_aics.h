/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_AICS_H__
#define __CONFIG_AICS_H__

//##################### Audio Input Control Service #####################

/*! @brief Audio Input Control Service max instance count
 * This option sets the maximum number of instances of Audio Input
 * Control Services.
 *
 *  Valid range 0 ~ 15
 */
#ifndef CONFIG_BT_AICS_MAX_INSTANCE_COUNT
    #define CONFIG_BT_AICS_MAX_INSTANCE_COUNT 0
#endif

/*! @brief Audio Input Control Service max instance count
 * This option sets the maximum number of instances of Audio Input
 * Control Services.
 *
 *  default 1 if BT_AICS_MAX_INSTANCE_COUNT > 0
 */
#if (defined(CONFIG_BT_AICS_MAX_INSTANCE_COUNT) && (CONFIG_BT_AICS_MAX_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_AICS
        #define CONFIG_BT_AICS 1
    #endif

    #if (defined(CONFIG_BT_GATT_DYNAMIC_DB) && (CONFIG_BT_GATT_DYNAMIC_DB > 0))
        /* PASS */
    #else
        #error CONFIG_BT_AICS depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif
#endif

#if (defined(CONFIG_BT_AICS) && (CONFIG_BT_AICS > 0))

/*! @brief Audio Input Control Service max input description size
 * This option sets the maximum input description size in octets.
 *
 *  Valid range 0 ~ 512
 */
#ifndef CONFIG_BT_AICS_MAX_INPUT_DESCRIPTION_SIZE
    #define CONFIG_BT_AICS_MAX_INPUT_DESCRIPTION_SIZE 32
#endif

#endif /* CONFIG_BT_AICS */

//##################### Audio Input Control Service Client #####################

/*! @brief Audio Input Control Service client max instance count
 * This option sets the maximum number of instances of Audio Input
 * Control Services.
 *
 *  Valid range 0 ~ 15
 */
#ifndef CONFIG_BT_AICS_CLIENT_MAX_INSTANCE_COUNT
    #define CONFIG_BT_AICS_CLIENT_MAX_INSTANCE_COUNT 0
#endif

#if (defined(CONFIG_BT_AICS_CLIENT_MAX_INSTANCE_COUNT) && (CONFIG_BT_AICS_CLIENT_MAX_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_AICS_CLIENT
        #define CONFIG_BT_AICS_CLIENT 1
    #endif
#endif

#endif /* __CONFIG_AICS_H__ */