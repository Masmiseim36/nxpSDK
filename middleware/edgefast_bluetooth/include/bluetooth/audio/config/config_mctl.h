/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_MCTL_H__
#define __CONFIG_MCTL_H__

/*! @brief Support for media player control
 * Enables support for control of local and remote media players
 * To enable support for control of a local media player, support for
 * local media player must be enabled
 */
#ifndef CONFIG_MCTL
    #define CONFIG_MCTL 0
#endif

#if (defined(CONFIG_MCTL) && (CONFIG_MCTL > 0))

/*! @brief Support for control of local media player
 * This option enables support for control of a local media player.
 */
#ifndef CONFIG_MCTL_LOCAL_PLAYER_CONTROL
    #define CONFIG_MCTL_LOCAL_PLAYER_CONTROL 0
#endif

/*! @brief Support for local control of local media player
 * This option enables support for local application control of local media players.
 */
#ifndef CONFIG_MCTL_LOCAL_PLAYER_LOCAL_CONTROL
    #define CONFIG_MCTL_LOCAL_PLAYER_LOCAL_CONTROL 1
#endif

#if (defined(CONFIG_MCTL_LOCAL_PLAYER_LOCAL_CONTROL) && (CONFIG_MCTL_LOCAL_PLAYER_LOCAL_CONTROL > 0))

    #if (!defined(CONFIG_MCTL_LOCAL_PLAYER_CONTROL) || !(CONFIG_MCTL_LOCAL_PLAYER_CONTROL > 0))
        #error CONFIG_MCTL_LOCAL_PLAYER_LOCAL_CONTROL depends on CONFIG_MCTL_LOCAL_PLAYER_CONTROL.
    #endif

#endif /* CONFIG_MCTL_LOCAL_PLAYER_LOCAL_CONTROL */

/*! @brief Support for remote control of local media players
 * This option enables support for remote control of local media players.
 */
#ifndef CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL
    #define CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL 0
#endif

#if (defined(CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL) && (CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL > 0))

    #if (!defined(CONFIG_MCTL_LOCAL_PLAYER_CONTROL) || !(CONFIG_MCTL_LOCAL_PLAYER_CONTROL > 0))
        #error CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL depends on CONFIG_MCTL_LOCAL_PLAYER_CONTROL.
    #endif

#endif /* CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL */

/*! @brief Support for control of remote media player
 * This options enables support for control of a remote media player.
 */
#ifndef CONFIG_MCTL_REMOTE_PLAYER_CONTROL
    #define CONFIG_MCTL_REMOTE_PLAYER_CONTROL 1
#endif

#if (defined(CONFIG_MCTL_REMOTE_PLAYER_CONTROL) && (CONFIG_MCTL_REMOTE_PLAYER_CONTROL > 0))

    #if (!defined(CONFIG_BT_MCC) || !(CONFIG_BT_MCC > 0))
        #error CONFIG_MCTL_REMOTE_PLAYER_CONTROL depends on CONFIG_BT_MCC.
    #endif

#endif /* CONFIG_MCTL_REMOTE_PLAYER_CONTROL */

/*! @brief Support for accessing objects on remote player
 * This options enables support for accessing objects (tracks, groups,
 * search results, ...) on a remote media player.
 */
#ifndef CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS
    #define CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS 1
#endif

#if (defined(CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS) && (CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS > 0))

    #if (!defined(CONFIG_MCTL_REMOTE_PLAYER_CONTROL) || !(CONFIG_MCTL_REMOTE_PLAYER_CONTROL > 0))
        #error CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS depends on CONFIG_MCTL_REMOTE_PLAYER_CONTROL.
    #endif

    #if (!defined(CONFIG_BT_MCC_OTS) || !(CONFIG_BT_MCC_OTS > 0))
        #error CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS depends on CONFIG_BT_MCC_OTS.
    #endif

#endif /* CONFIG_MCTL_REMOTE_PLAYER_CONTROL_OBJECTS */

#endif /* CONFIG_MCTL */

#endif /* __CONFIG_MCTL_H__ */