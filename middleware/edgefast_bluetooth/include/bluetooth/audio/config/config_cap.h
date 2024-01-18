/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_CAP_H__
#define __CONFIG_CAP_H__

#ifndef CONFIG_BT_CAP
    #if (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0)) || \
        (defined(CONFIG_BT_CAP_INITIATOR) && (CONFIG_BT_CAP_INITIATOR > 0))
        #define CONFIG_BT_CAP 1
    #endif
#endif

/*! @brief Common Audio Profile Acceptor Role Support
 * Enabling this will enable the CAP Acceptor role. This instantiates the
 * common audio service (CAS).
 */
#ifndef CONFIG_BT_CAP_ACCEPTOR
    #define CONFIG_BT_CAP_ACCEPTOR 0
#endif

#if (defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))
    #if (defined(CONFIG_BT_BAP_UNICAST_SERVER) && (CONFIG_BT_BAP_UNICAST_SERVER > 0)) || \
        ( (defined(CONFIG_BT_BAP_BROADCAST_SINK) && (CONFIG_BT_BAP_BROADCAST_SINK > 0)) && \
          (defined(CONFIG_BT_BAP_SCAN_DELEGATOR) && (CONFIG_BT_BAP_SCAN_DELEGATOR > 0)) )
        /* PASS. */
    #else
        #error CONFIG_BT_CAP_ACCEPTOR depends on CONFIG_BT_BAP_UNICAST_SERVER || (CONFIG_BT_BAP_BROADCAST_SINK && CONFIG_BT_BAP_SCAN_DELEGATOR).
    #endif

    #if (defined(CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE) && (CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE >= 4))
        /* PASS. */
    #else
        #error CONFIG_BT_CAP_ACCEPTOR depends on CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE >= 4.
    #endif
#endif /* CONFIG_BT_CAP_ACCEPTOR */

/*! @brief Common Audio Profile Acceptor Role Set Member support
 * Enabling this will allow a CAP acceptor to be a set member.
 * Enabling this will require a manual register of the CAS service.
 * Enabling will take one of the allocated CSIS instances (BT_CSIP_MAX_INSTANCE_COUNT).
 *
 */
#ifndef CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER
    #define CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER 0
#endif

#if (defined(CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER) && (CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER > 0))

    #if !(defined(CONFIG_BT_CAP_ACCEPTOR) && (CONFIG_BT_CAP_ACCEPTOR > 0))
        #error CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER depends on CONFIG_BT_CAP_ACCEPTOR.
    #endif

    #if !(defined(CONFIG_BT_CSIP_SET_MEMBER) && (CONFIG_BT_CSIP_SET_MEMBER > 0))
        #error CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER depends on CONFIG_BT_CSIP_SET_MEMBER.
    #endif

    #if !(defined(CONFIG_BT_GATT_DYNAMIC_DB) && (CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

#endif /* CONFIG_BT_CAP_ACCEPTOR_SET_MEMBER */

/*! @brief Common Audio Profile Initiator Role Support
 * Enabling this will enable the CAP Initiator role.
 */
#ifndef CONFIG_BT_CAP_INITIATOR
    #define CONFIG_BT_CAP_INITIATOR 0
#endif

#if (defined(CONFIG_BT_CAP_INITIATOR) && (CONFIG_BT_CAP_INITIATOR > 0))
    #if ( (defined(CONFIG_BT_BAP_UNICAST_CLIENT) && (CONFIG_BT_BAP_UNICAST_CLIENT > 0)) && \
          (defined(CONFIG_BT_CSIP_SET_COORDINATOR) && (CONFIG_BT_CSIP_SET_COORDINATOR > 0)) ) || \
        (defined(CONFIG_BT_BAP_BROADCAST_SOURCE) && (CONFIG_BT_BAP_BROADCAST_SOURCE > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_CAP_INITIATOR depends on (BT_AUDIO_UNICAST_CLIENT && BT_CSIP_SET_COORDINATOR) || BT_AUDIO_BROADCAST_SOURCE.
    #endif

    #if (defined(CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE) && (CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE >= 4))
        /* PASS. */
    #else
        #error CONFIG_BT_CAP_ACCEPTOR depends on CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE >= 4.
    #endif
#endif /* CONFIG_BT_CAP_INITIATOR */

#endif /* __CONFIG_CAP_H__ */