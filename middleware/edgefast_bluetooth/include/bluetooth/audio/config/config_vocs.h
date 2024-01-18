/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_VOCS_H__
#define __CONFIG_VOCS_H__

//##################### Volume Offset Control Service #####################

/*! @brief Volume Offset Control Service max instance count
 * This option sets the maximum number of instances of Volume Offset Control Services.
 *
 *  Valid range 0 ~ 15
 */
#ifndef CONFIG_BT_VOCS_MAX_INSTANCE_COUNT
    #define CONFIG_BT_VOCS_MAX_INSTANCE_COUNT 0
#endif

/* This hidden option enables support for Volume Control Service. */
#if (defined(CONFIG_BT_VOCS_MAX_INSTANCE_COUNT) && (CONFIG_BT_VOCS_MAX_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VOCS
        #define CONFIG_BT_VOCS 1
    #endif
#endif

#if (defined(CONFIG_BT_VOCS) && (CONFIG_BT_VOCS > 0))

/*! @brief Volume Offset Control Service max output description size
 * This option sets the maximum output description size in octets.
 *
 *  Valid range 0 ~ 512
 */
#ifndef CONFIG_BT_VOCS_MAX_OUTPUT_DESCRIPTION_SIZE
    #define CONFIG_BT_VOCS_MAX_OUTPUT_DESCRIPTION_SIZE 32
#endif

#endif /* CONFIG_BT_VOCS */

//##################### Volume Offset Control Service Client #####################

/*! @brief Volume Offset Control Service client max instance count
 * This option sets the maximum number of instances of Volume Offset
 * Control Service clients.
 *
 *  Valid range 0 ~ 15
 */
#ifndef CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT
    #define CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT 0
#endif

/* This hidden option enables support for Volume Offset Control Service. */
#if (defined(CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT) && (CONFIG_BT_VOCS_CLIENT_MAX_INSTANCE_COUNT > 0))
    #ifndef CONFIG_BT_VOCS_CLIENT
        #define CONFIG_BT_VOCS_CLIENT 1
    #endif
#endif

#endif /* __CONFIG_VOCS_H__ */