/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_ASCS_H__
#define __CONFIG_ASCS_H__

/*! @brief Audio Stream Control Service Support
 * This option enables support for Audio Stream Control Service.
 */
#ifndef CONFIG_BT_ASCS
    #define CONFIG_BT_ASCS 0
#endif

#if (defined(CONFIG_BT_ASCS) && (CONFIG_BT_ASCS > 0))

/*! @brief Number of Audio Stream Endpoint Sink Characteristics
 * An ASE Sink characteristic represents the state of an ASE, which is
 * coupled to a single direction of a unicast Audio Stream.
 * 
 * Valid range 0 ~ 2
 */
#ifndef CONFIG_BT_ASCS_ASE_SNK_COUNT
    #define CONFIG_BT_ASCS_ASE_SNK_COUNT 2
#endif

/*! @brief Number of Audio Stream Endpoint Source Characteristics
 * An ASE Source characteristic represents the state of an ASE, which is
 * coupled to a single direction of a unicast Audio Stream.
 * 
 * Valid range 0 ~ 2
 */
#ifndef CONFIG_BT_ASCS_ASE_SRC_COUNT
    #define CONFIG_BT_ASCS_ASE_SRC_COUNT 2
#endif

#ifndef CONFIG_BT_ASCS_ASE_SNK
    #if (defined(CONFIG_BT_ASCS_ASE_SNK_COUNT) && (CONFIG_BT_ASCS_ASE_SNK_COUNT > 0))
        #define CONFIG_BT_ASCS_ASE_SNK 1
    #else
        #define CONFIG_BT_ASCS_ASE_SNK 0
    #endif
#endif

#ifndef CONFIG_BT_ASCS_ASE_SRC
    #if (defined(CONFIG_BT_ASCS_ASE_SRC_COUNT) && (CONFIG_BT_ASCS_ASE_SRC_COUNT > 0))
        #define CONFIG_BT_ASCS_ASE_SRC 1
    #else
        #define CONFIG_BT_ASCS_ASE_SRC 0
    #endif
#endif

#if (defined(CONFIG_BT_ASCS_ASE_SNK) && (CONFIG_BT_ASCS_ASE_SNK > 0))
    #if !(defined(CONFIG_BT_PAC_SNK) && (CONFIG_BT_PAC_SNK > 0))
        #error CONFIG_BT_ASCS_ASE_SNK depends on CONFIG_BT_PAC_SNK.
    #endif

    #if !(defined(CONFIG_BT_AUDIO_RX) && (CONFIG_BT_AUDIO_RX > 0))
        #error CONFIG_BT_ASCS_ASE_SNK depends on CONFIG_BT_AUDIO_RX.
    #endif
#endif

#if (defined(CONFIG_BT_ASCS_ASE_SRC) && (CONFIG_BT_ASCS_ASE_SRC > 0))
    #if !(defined(CONFIG_BT_PAC_SRC) && (CONFIG_BT_PAC_SRC > 0))
        #error CONFIG_BT_ASCS_ASE_SRC depends on CONFIG_BT_PAC_SRC.
    #endif

    #if !(defined(CONFIG_BT_AUDIO_TX) && (CONFIG_BT_AUDIO_TX > 0))
        #error CONFIG_BT_ASCS_ASE_SRC depends on CONFIG_BT_AUDIO_TX.
    #endif
#endif

/*! @brief Number of simultaneously supported ASE sessions
 * The number of simultanesouly supported active ASEs, in particular
 * meaning the number of ASEs that are allowed to be in a non-idle state at
 * a single time.
 * 
 * Valid range 1 ~ 65535
 */
#ifndef CONFIG_BT_ASCS_MAX_ACTIVE_ASES
    #define CONFIG_BT_ASCS_MAX_ACTIVE_ASES CONFIG_BT_ISO_MAX_CHAN
#endif

/*! @brief Milliseconds of delay before ASCS disconnects ISO after stream stop
 * The number of milliseconds ASCS will wait before disconnecting the ISO
 * of a stopped stream. The delay is added as it is the Unicast Client's
 * responsibility, but this is a failsafe to ensure that ISO channel is
 * being properly disconnected.
 * 
 * Valid range 0 ~ 5000
 */
#ifndef CONFIG_BT_ASCS_ISO_DISCONNECT_DELAY
    #define CONFIG_BT_ASCS_ISO_DISCONNECT_DELAY 500
#endif

/*! @brief Milliseconds of timeout when handle concurrent access to the long read ASE buffer
 * The number of milliseconds that the ASCS implementation will maximum wait before rejecting
 * an ASE read or dropping a notification if the ASE state is being accessed by another
 * thread.
 * 
 * Valid range 0 ~ 1000
 */
#ifndef CONFIG_BT_ASCS_ASE_BUF_TIMEOUT
    #define CONFIG_BT_ASCS_ASE_BUF_TIMEOUT 50
#endif

#endif /* CONFIG_BT_ASCS */

#endif /* __CONFIG_ASCS_H__ */