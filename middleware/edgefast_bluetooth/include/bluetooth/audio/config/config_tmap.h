/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_TMAP_H__
#define __CONFIG_TMAP_H__

/*! @brief Telephony and Media Audio Profile
 * Enabling this will enable TMAP.
 */
#ifndef CONFIG_BT_TMAP
    #define CONFIG_BT_TMAP 0
#endif

#if (defined(CONFIG_BT_TMAP) && (CONFIG_BT_TMAP > 0))

    #if (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0)) || \
        (defined(CONFIG_BT_CAP_INITIATOR) && (CONFIG_BT_CAP_INITIATOR > 0))
        /* PASS */
    #else
        #error CONFIG_BT_TMAP depends on CONFIG_BT_CAP_ACCEPTOR || CONFIG_BT_CAP_INITIATOR.
    #endif

#endif

/*! @brief Telephony and Media Audio Profile debug
 * Use this option to enable Telephony and Media Audio Profile debug
 * logs for the Bluetooth Audio functionality.
 */
#ifndef CONFIG_BT_DEBUG_TMAP
    #define CONFIG_BT_DEBUG_TMAP 0
#endif

#if (defined(CONFIG_BT_DEBUG_TMAP) && (CONFIG_BT_DEBUG_TMAP > 0))

    #if defined(CONFIG_BT_TMAP) && (CONFIG_BT_TMAP > 0)
        /* PASS */
    #else
        #error CONFIG_BT_DEBUG_TMAP depends on CONFIG_BT_TMAP.
    #endif

#endif

#endif /* __CONFIG_TMAP_H__ */