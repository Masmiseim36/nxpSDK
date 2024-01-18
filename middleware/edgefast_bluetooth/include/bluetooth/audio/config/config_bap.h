/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_BAP_H__
#define __CONFIG_BAP_H__

/*! @brief Unicast Audio function
 */
#ifndef CONFIG_BT_BAP_UNICAST
    #define CONFIG_BT_BAP_UNICAST 0
#endif

#if (defined(CONFIG_BT_BAP_UNICAST) && (CONFIG_BT_BAP_UNICAST > 0))
    
    #if (!defined(CONFIG_BT_CONN) || !(CONFIG_BT_CONN > 0))
        #error CONFIG_BT_CONN must be enabled for CONFIG_BT_BAP_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_SMP) || !(CONFIG_BT_SMP > 0))
        #error CONFIG_BT_SMP must be enabled for CONFIG_BT_BAP_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_ISO_UNICAST) || !(CONFIG_BT_ISO_UNICAST > 0))
        #error CONFIG_BT_ISO_UNICAST must be enabled for CONFIG_BT_BAP_UNICAST.
    #endif

#endif /* CONFIG_BT_BAP_UNICAST */

/*! @brief Bluetooth Unicast Audio Server Support
 * This option enables support for Bluetooth Unicast Audio Server using Isochronous channels.
 */
#if (defined(CONFIG_BT_BAP_UNICAST_SERVER) && (CONFIG_BT_BAP_UNICAST_SERVER > 0))

    #if (!defined(CONFIG_BT_PERIPHERAL) || !(CONFIG_BT_PERIPHERAL > 0))
        #error CONFIG_BT_BAP_UNICAST_SERVER depands on CONFIG_BT_PERIPHERAL.
    #endif

    #if (!defined(CONFIG_BT_BAP_UNICAST) || !(CONFIG_BT_BAP_UNICAST > 0))
        #error CONFIG_BT_BAP_UNICAST_SERVER depands on CONFIG_BT_BAP_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_ISO_PERIPHERAL) || !(CONFIG_BT_ISO_PERIPHERAL > 0))
        #error CONFIG_BT_BAP_UNICAST_SERVER depands on CONFIG_BT_ISO_PERIPHERAL.
    #endif

    #if (!defined(CONFIG_BT_GATT_DYNAMIC_DB) || !(CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_BAP_UNICAST_SERVER depands on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

    #if (!defined(CONFIG_BT_GATT_CACHING) || !(CONFIG_BT_GATT_CACHING > 0))
        #error CONFIG_BT_BAP_UNICAST_SERVER depands on CONFIG_BT_GATT_CACHING.
    #endif

    #if (!defined(CONFIG_BT_ASCS) || !(CONFIG_BT_ASCS > 0))
        #error CONFIG_BT_BAP_UNICAST_SERVER depands on CONFIG_BT_ASCS.
    #endif

#endif /* CONFIG_BT_BAP_UNICAST_SERVER */

/*! @brief Bluetooth Unicast Audio Client Support
 * This option enables support for Bluetooth Unicast Audio Client using Isochronous channels.
 */
#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT) && (CONFIG_BT_BAP_UNICAST_CLIENT > 0))

    #if (!defined(CONFIG_BT_BAP_UNICAST) || !(CONFIG_BT_BAP_UNICAST > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT depands on CONFIG_BT_BAP_UNICAST.
    #endif

    #if (!defined(CONFIG_BT_ISO_CENTRAL) || !(CONFIG_BT_ISO_CENTRAL > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT depands on CONFIG_BT_ISO_CENTRAL.
    #endif

    #if (!defined(CONFIG_BT_CENTRAL) || !(CONFIG_BT_CENTRAL > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT depands on CONFIG_BT_CENTRAL.
    #endif

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT depands on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT depands on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_UPDATE_MTU) || !(CONFIG_BT_GATT_AUTO_UPDATE_MTU > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT depands on CONFIG_BT_GATT_AUTO_UPDATE_MTU.
    #endif

#endif /* CONFIG_BT_BAP_UNICAST_CLIENT */

/*! @brief Codec Specific Configuration Data Size
 * Number of octets to support for Codec Specific Configuration data.
 * The default value 19 matches the required fields for the LC3 codec.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_AUDIO_CODEC_CFG_MAX_DATA_SIZE
    #define CONFIG_BT_AUDIO_CODEC_CFG_MAX_DATA_SIZE 19
#endif

/*! @brief Codec Specific Configuration Metadata Size
 * Number of octets to support for Codec Specific Configuration metadata.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE
    #define CONFIG_BT_AUDIO_CODEC_CFG_MAX_METADATA_SIZE 4
#endif

/*! @brief Codec Capabilities Data Size
 * Number of octets to support for Codec Specific Capabilities data.
 * The default value 19 matches the required fields for the LC3 codec.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_AUDIO_CODEC_CAP_MAX_DATA_SIZE
    #define CONFIG_BT_AUDIO_CODEC_CAP_MAX_DATA_SIZE 19
#endif

/*! @brief Codec Capabilities Metadata Size
 * Number of octets to support for Codec Specific Capabilities metadata.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_AUDIO_CODEC_CAP_MAX_METADATA_SIZE
    #define CONFIG_BT_AUDIO_CODEC_CAP_MAX_METADATA_SIZE 4
#endif

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT) && (CONFIG_BT_BAP_UNICAST_CLIENT > 0))

/*! @brief Basic Audio Unicast Group count
 * This option sets the number of connected audio groups to support as the unicast client.
 *
 * Valid range 1 ~ CONFIG_BT_ISO_MAX_CIG
 */
#ifndef CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT
    #define CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT CONFIG_BT_ISO_MAX_CIG
#endif

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT) && (CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT > 0))

    #if (!defined(CONFIG_BT_BAP_UNICAST) || !(CONFIG_BT_BAP_UNICAST > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT depends on CONFIG_BT_BAP_UNICAST.
    #endif

#endif

/*! @brief Basic Audio Profile Unicast Group Connected Isochronous Stream (CIS) count
 * This option sets the maximum number of CIS per unicast group to support.
 * Since BAP streams are unidirectional, two BAP streams may use a single CIS, the number of
 * BAP audio streams per group may be up to twice of this value.
 *
 * Valid range 1 ~ 31
 * Valid range 1 ~ CONFIG_BT_ISO_MAX_CHAN if CONFIG_BT_ISO_MAX_CHAN < 31
 */
#ifndef CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_STREAM_COUNT
    #define CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_STREAM_COUNT 1
#endif

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_STREAM_COUNT) && (CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_STREAM_COUNT > 0))

    #if (!defined(CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT) || !(CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT > 0))
        #error CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_STREAM_COUNT depends on CONFIG_BT_BAP_UNICAST_CLIENT_GROUP_COUNT > 0.
    #endif

#endif

/*! @brief Basic Audio Profile ASE Sink count
 * This option enables caching a number of Audio Stream Endpoint Sink
 * instances for Basic Audio Profile on a per connection basis.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT
    #define CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT 2
#endif

/*! @brief Basic Audio Profile ASE Source count
 * This option enables caching a number of Audio Stream Endpoint Source
 * instances for Basic Audio Profile on a per connection basis.
 *
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT
    #define CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT 2
#endif

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT) && (CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT > 0))

    #ifndef CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK
        #define CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK 1
    #endif

#endif /* CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK_COUNT */

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK) && (CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK > 0))

    #if (defined(CONFIG_BT_AUDIO_TX) && (CONFIG_BT_AUDIO_TX > 0))
        /* PASS */
    #else
        #error CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK deponds on CONFIG_BT_AUDIO_TX.
    #endif

#endif /* CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SNK */

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT) && (CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT > 0))

    #ifndef CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC
        #define CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC 1
    #endif

#endif /* CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC_COUNT */

#if (defined(CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC) && (CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC > 0))

    #if (defined(CONFIG_BT_AUDIO_RX) && (CONFIG_BT_AUDIO_RX > 0))
        /* PASS */
    #else
        #error CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC deponds on CONFIG_BT_AUDIO_RX.
    #endif

#endif /* CONFIG_BT_BAP_UNICAST_CLIENT_ASE_SRC */


#endif /* CONFIG_BT_BAP_UNICAST_CLIENT */

/*! @brief Bluetooth Broadcast Source Audio Support
 * This option enables support for Bluetooth Broadcast Source Audio using
 * Isochronous channels.
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SOURCE
    #define CONFIG_BT_BAP_BROADCAST_SOURCE 0
#endif

#if (defined(CONFIG_BT_BAP_BROADCAST_SOURCE) && (CONFIG_BT_BAP_BROADCAST_SOURCE > 0))

    #if (!defined(CONFIG_BT_ISO_BROADCASTER) || !(CONFIG_BT_ISO_BROADCASTER > 0))
        #error CONFIG_BT_BAP_BROADCAST_SOURCE depands on CONFIG_BT_ISO_BROADCASTER.
    #endif

    #if (!defined(CONFIG_BT_AUDIO_TX) || !(CONFIG_BT_AUDIO_TX > 0))
        #error CONFIG_BT_BAP_BROADCAST_SOURCE depands on CONFIG_BT_AUDIO_TX.
    #endif

#endif /* CONFIG_BT_BAP_BROADCAST_SOURCE */

#if (defined(CONFIG_BT_BAP_BROADCAST_SOURCE) && (CONFIG_BT_BAP_BROADCAST_SOURCE > 0))

/*! @brief Basic Audio Broadcast Source subgroup count
 * This option sets the maximum number of subgroups per broadcast source to support.
 *
 * Valid range 1 ~ 31
 * Valid range 1 CONFIG_BT_ISO_MAX_CHAN if CONFIG_BT_ISO_MAX_CHAN < 31
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SRC_SUBGROUP_COUNT
    #define CONFIG_BT_BAP_BROADCAST_SRC_SUBGROUP_COUNT 1
#endif

/*! @brief Basic Audio Broadcaster source count
 * This option sets the number of broadcast sources to support.
 * One broadcast source can send multiple streams
 * (up to BT_AUDIO_BROADCAST_SRC_STREAM_COUNT per broadcast source).
 *
 * Valid range 0 ~ CONFIG_BT_ISO_MAX_BIG
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SRC_COUNT
    #define CONFIG_BT_BAP_BROADCAST_SRC_COUNT 1
#endif

/*! @brief Basic Audio Broadcast Source Stream count
 * This option sets the maximum number of streams per broadcast source to support.
 *
 * Valid range 0 ~ CONFIG_BT_ISO_MAX_CHAN
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT
    #define CONFIG_BT_BAP_BROADCAST_SRC_STREAM_COUNT 1
#endif

#endif /* CONFIG_BT_BAP_BROADCAST_SOURCE */

/*! @brief Bluetooth Broadcast Sink Audio Support
 * This option enables support for Bluetooth Broadcast Sink Audio using Isochronous channels.
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SINK
    #define CONFIG_BT_BAP_BROADCAST_SINK 0
#endif

#if (defined(CONFIG_BT_BAP_BROADCAST_SINK) && (CONFIG_BT_BAP_BROADCAST_SINK > 0))

    #if (!defined(CONFIG_BT_ISO_SYNC_RECEIVER) || !(CONFIG_BT_ISO_SYNC_RECEIVER > 0))
        #error CONFIG_BT_BAP_BROADCAST_SINK depands on CONFIG_BT_ISO_SYNC_RECEIVER.
    #endif

    #if (!defined(CONFIG_BT_AUDIO_RX) || !(CONFIG_BT_AUDIO_RX > 0))
        #error CONFIG_BT_BAP_BROADCAST_SINK depands on CONFIG_BT_AUDIO_RX.
    #endif

    #if (!defined(CONFIG_BT_PERIPHERAL) || !(CONFIG_BT_PERIPHERAL > 0))
        #error CONFIG_BT_BAP_BROADCAST_SINK depands on CONFIG_BT_PERIPHERAL.
    #endif

    #if (!defined(CONFIG_BT_PAC_SNK) || !(CONFIG_BT_PAC_SNK > 0))
        #error CONFIG_BT_BAP_BROADCAST_SINK depands on CONFIG_BT_PAC_SNK.
    #endif

    #if (!defined(CONFIG_BT_BAP_SCAN_DELEGATOR) || !(CONFIG_BT_BAP_SCAN_DELEGATOR > 0))
        #error CONFIG_BT_BAP_BROADCAST_SINK depands on CONFIG_BT_BAP_SCAN_DELEGATOR.
    #endif

#endif /* CONFIG_BT_BAP_BROADCAST_SINK */

#if (defined(CONFIG_BT_BAP_BROADCAST_SINK) && (CONFIG_BT_BAP_BROADCAST_SINK > 0))

/*! @brief Basic Audio Profile Broadcast Sink subgroup count
 * This option sets the maximum number of subgroups per broadcast sink to support.
 *
 * Valid range 1 ~ 31
 * Valid range 1 CONFIG_BT_ISO_MAX_CHAN if CONFIG_BT_ISO_MAX_CHAN < 31
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SNK_SUBGROUP_COUNT
    #define CONFIG_BT_BAP_BROADCAST_SNK_SUBGROUP_COUNT 1
#endif

/*! @brief Basic Audio Broadcaster Sink count
 * This option sets the number of broadcast sinks to support.
 * One broadcast sink can receive multiple streams
 * (up to BT_AUDIO_BROADCAST_SNK_STREAM_COUNT per broadcast sink).
 *
 * Valid range 0 ~ CONFIG_BT_ISO_MAX_BIG
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SNK_COUNT
    #define CONFIG_BT_BAP_BROADCAST_SNK_COUNT 1
#endif

/*! @brief Basic Audio Broadcast Sink Stream count
 * This option sets the maximum number of streams per broadcast sink to support.
 *
 * Valid range 1 ~ CONFIG_BT_ISO_MAX_CHAN
 */
#ifndef CONFIG_BT_BAP_BROADCAST_SNK_STREAM_COUNT
    #define CONFIG_BT_BAP_BROADCAST_SNK_STREAM_COUNT 1
#endif

#if (defined(CONFIG_BT_BAP_BROADCAST_SNK_STREAM_COUNT) && (CONFIG_BT_BAP_BROADCAST_SNK_STREAM_COUNT > 0))

    #if (!defined(CONFIG_BT_BAP_BROADCAST_SNK_COUNT) || !(CONFIG_BT_BAP_BROADCAST_SNK_COUNT > 0))
        #error CONFIG_BT_BAP_BROADCAST_SNK_STREAM_COUNT depends on CONFIG_BT_BAP_BROADCAST_SNK_COUNT > 0.
    #endif

#endif

#endif /* CONFIG_BT_BAP_BROADCAST_SINK */

/*! @brief Basic Audio Profile Scan Delegator role support
 * This option enables support for the Scan Delegator role and the
 * Broadcast Audio Scan Service (BASS).
 */
#ifndef CONFIG_BT_BAP_SCAN_DELEGATOR
    #define CONFIG_BT_BAP_SCAN_DELEGATOR 0
#endif

#if (defined(CONFIG_BT_BAP_SCAN_DELEGATOR) && (CONFIG_BT_BAP_SCAN_DELEGATOR > 0))
    #if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_SCAN_DELEGATOR depends on CONFIG_BT_OBSERVER.
    #endif
    #if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_SCAN_DELEGATOR depends on CONFIG_BT_EXT_ADV.
    #endif
    #if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_SCAN_DELEGATOR depends on CONFIG_BT_PER_ADV_SYNC.
    #endif
    #if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_SCAN_DELEGATOR depends on CONFIG_BT_ISO_SYNC_RECEIVER.
    #endif
#endif /* CONFIG_BT_BAP_SCAN_DELEGATOR */

#if (defined(CONFIG_BT_BAP_SCAN_DELEGATOR) && (CONFIG_BT_BAP_SCAN_DELEGATOR > 0))

/*! @brief Scan Delegator Receive State Count
 * Sets the number of receive state characteristics present on the
 * server. Each characteristic may hold information to sync to a
 * periodic advertise or a broadcast isochronous stream.
 * 
 * Valid range 1 ~ 3
 */
#ifndef CONFIG_BT_BAP_SCAN_DELEGATOR_RECV_STATE_COUNT
    #define CONFIG_BT_BAP_SCAN_DELEGATOR_RECV_STATE_COUNT 1
#endif

/*! @brief Scan Delegator Maximum Metadata Length
 * The maximum metadata length support by the BASS server.
 * 
 * Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_BAP_SCAN_DELEGATOR_MAX_METADATA_LEN
    #define CONFIG_BT_BAP_SCAN_DELEGATOR_MAX_METADATA_LEN 32
#endif

/*! @brief Scan Delegator Maximum Number of Subgroups support
 * The maximum number of BIS subgroups supported.
 * 
 * Valid range 0 ~ 31
 */
#ifndef CONFIG_BT_BAP_SCAN_DELEGATOR_MAX_SUBGROUPS
    #define CONFIG_BT_BAP_SCAN_DELEGATOR_MAX_SUBGROUPS 1
#endif

#endif /* CONFIG_BT_BAP_SCAN_DELEGATOR */

/*! @brief Basic Audio Profile Broadcast Assistant role support
 * This option enables support for the Broadcast Assistant role.
 */
#ifndef CONFIG_BT_BAP_BROADCAST_ASSISTANT
    #define CONFIG_BT_BAP_BROADCAST_ASSISTANT 0
#endif

#if (defined(CONFIG_BT_BAP_BROADCAST_ASSISTANT) && (CONFIG_BT_BAP_BROADCAST_ASSISTANT > 0))
    #if (defined(CONFIG_BT_OBSERVER) && (CONFIG_BT_OBSERVER > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_OBSERVER.
    #endif
    #if (defined(CONFIG_BT_EXT_ADV) && (CONFIG_BT_EXT_ADV > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_EXT_ADV.
    #endif
    #if (defined(CONFIG_BT_PER_ADV_SYNC) && (CONFIG_BT_PER_ADV_SYNC > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_PER_ADV_SYNC.
    #endif
    #if (defined(CONFIG_BT_ISO_SYNC_RECEIVER) && (CONFIG_BT_ISO_SYNC_RECEIVER > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_ISO_SYNC_RECEIVER.
    #endif
    #if (defined(CONFIG_BT_GATT_CLIENT) && (CONFIG_BT_GATT_CLIENT > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_GATT_CLIENT.
    #endif
    #if (defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) && (CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif
    #if (defined(CONFIG_BT_GATT_AUTO_UPDATE_MTU) && (CONFIG_BT_GATT_AUTO_UPDATE_MTU > 0))
        /* PASS. */
    #else
        #error CONFIG_BT_BAP_BROADCAST_ASSISTANT depends on CONFIG_BT_GATT_AUTO_UPDATE_MTU.
    #endif
#endif /* CONFIG_BT_BAP_BROADCAST_ASSISTANT */

#if (defined(CONFIG_BT_BAP_BROADCAST_ASSISTANT) && (CONFIG_BT_BAP_BROADCAST_ASSISTANT > 0))

/*! @brief Broadcast Assistant Max Receive State Count
 * Sets the number of maximum receive stat characteristics that will be
 * discovered and ready to use. Each characteristic may hold information
 * to sync to a periodic advertise or a broadcast isochronous stream.
 * 
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_BAP_BROADCAST_ASSISTANT_RECV_STATE_COUNT
    #define CONFIG_BT_BAP_BROADCAST_ASSISTANT_RECV_STATE_COUNT 1
#endif

#endif /* CONFIG_BT_BAP_BROADCAST_ASSISTANT */

/*! @brief Bluetooth Audio Stream data debug
 * Use this option to enable Bluetooth Audio Stream data debug logs for
 * the Bluetooth Audio functionality. This will enable debug logs for all
 * audio data received and sent.
 */
#ifndef CONFIG_BT_BAP_DEBUG_STREAM_DATA
    #define CONFIG_BT_BAP_DEBUG_STREAM_DATA 0
#endif

#if (defined(CONFIG_BT_ASCS) && (CONFIG_BT_ASCS > 0)) || \
    (defined(CONFIG_BT_BAP_UNICAST_CLIENT) && (CONFIG_BT_BAP_UNICAST_CLIENT > 0)) || \
    (defined(CONFIG_BT_BAP_BROADCAST_SOURCE) && (CONFIG_BT_BAP_BROADCAST_SOURCE > 0)) || \
    (defined(CONFIG_BT_BAP_BROADCAST_SINK) && (CONFIG_BT_BAP_BROADCAST_SINK > 0))

    #ifndef CONFIG_BT_BAP_STREAM
        #define CONFIG_BT_BAP_STREAM 1
    #endif

#endif

#include "config_pacs.h"
#include "config_ascs.h"

#endif /* __CONFIG_BAP_H__ */