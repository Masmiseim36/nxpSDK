/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_TBS_H__
#define __CONFIG_TBS_H__

#if (defined(CONFIG_BT_AUDIO) && (CONFIG_BT_AUDIO > 0))

//#####################  Telephone Bearer Service #####################

/*! @brief Telephone Bearer Service Support
 * This option enables support for Telephone Bearer Service.
 */
#ifndef CONFIG_BT_TBS
    #define CONFIG_BT_TBS 0
#endif

#if (defined(CONFIG_BT_TBS) && (CONFIG_BT_TBS > 0))

    #if (!defined(CONFIG_BT_CCID) || !(CONFIG_BT_CCID > 0))
        #error CONFIG_BT_TBS depends on CONFIG_BT_CCID.
    #endif

    #if (!defined(CONFIG_BT_GATT_DYNAMIC_DB) || !(CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_TBS depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

#endif /* CONFIG_BT_TBS */

#if (defined(CONFIG_BT_TBS) && (CONFIG_BT_TBS > 0))

/*! @brief Generic Telephone Bearer Service Support
 * This option enables support for Generic Telephone Bearer Service.
 */
#ifndef CONFIG_BT_GTBS
    #define CONFIG_BT_GTBS 1
#endif

/*! @brief Telephone Bearer Service Provider Name
 * Sets the name of the service provider for the bearer.
 */
#ifndef CONFIG_BT_TBS_PROVIDER_NAME
    #define CONFIG_BT_TBS_PROVIDER_NAME "Unknown"
#endif

/*! @brief Telephone Bearer Service Uniform Caller Identifier (UCI)
 * Sets the UCI of the bearer. See
 *   https://www.bluetooth.com/specifications/assigned-numbers/uniform-caller-identifiers/
 *   for a table of valid UCIs.
 */
#ifndef CONFIG_BT_TBS_UCI
    #define CONFIG_BT_TBS_UCI "un000"
#endif

/*! @brief Telephone Bearer Service Technology
 * Sets the technology used for the bearer, e.g. GSM, LTE and 5G.
 *  1 : 3G
 *  2 : 4G
 *  3 : LTE
 *  4 : Wi-Fi
 *  5 : 5G
 *  6 : GSM
 *  7 : CDMA
 *  8 : 2G
 *  9 : WCDMA
 *  10: IP
 *
 *  Valid range 1 ~ 10
 */
#ifndef CONFIG_BT_TBS_TECHNOLOGY
    #define CONFIG_BT_TBS_TECHNOLOGY 0
#endif

/*! @brief Telephone Bearer Service URI schemes Supported List
 * Sets a list of URI schemes that are supported by the bearer,
 * e.g. "tel" or "skype".
 * Multiple values shall be comma (,) separated, e.g. "tel,skype".
 */
#ifndef CONFIG_BT_TBS_URI_SCHEMES_LIST
    #define CONFIG_BT_TBS_URI_SCHEMES_LIST "tel,skype"
#endif

/*! @brief Telephone Bearer Service Signal Strength Reporting Interval
 * Sets the interval of reporting the signal strength in seconds.
 *   If the value is 0, the signal will not be reported.
 *
 *  Valid range 0 ~ 255
 */
#ifndef CONFIG_BT_TBS_SIGNAL_STRENGTH_INTERVAL
    #define CONFIG_BT_TBS_SIGNAL_STRENGTH_INTERVAL 0
#endif

/*! @brief Telephone Bearer Service Features and Status value
 * Bitfield to set feature and status flags.
 *   Bit 0: In-band ringtone
 *   Bit 1: Silent mode
 *   Bits 2-15: Reserved for future use
 *
 *  Valid range 0 ~ 3
 */
#ifndef CONFIG_BT_TBS_STATUS_FLAGS
    #define CONFIG_BT_TBS_STATUS_FLAGS 0
#endif

/*! @brief Telephone Bearer Service Supported Features
 * Bitfield to set supported features of the bearer.
 *   Bit 0: Local Hold and Retrieve
 *   Bit 1: Join calls within Telephone Bearer Service
 *
 *  Valid range 0 ~ 3
 */
#ifndef CONFIG_BT_TBS_SUPPORTED_FEATURES
    #define CONFIG_BT_TBS_SUPPORTED_FEATURES 1
#endif

/*! @brief Telephone Bearer Service Maximum Number Of Calls Supported
 * Sets the maximum number of calls the service supports per bearer.
 *
 *  Valid range 1 ~ 16
 */
#ifndef CONFIG_BT_TBS_MAX_CALLS
    #define CONFIG_BT_TBS_MAX_CALLS 3
#endif

/*! @brief How many bearer instances the device instantiates
 * Sets the number of TBS instances that are instantiated.
 *
 *  Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_TBS_BEARER_COUNT
    #define CONFIG_BT_TBS_BEARER_COUNT 1
#endif

/*! @brief Number of instantiated bearer service instances
 * Sets the number of TBS service instances that are instantiated.
 *
 * range 0 CONFIG_BT_TBS_BEARER_COUNT if CONFIG_BT_GTBS
 * range CONFIG_BT_TBS_BEARER_COUNT ~ CONFIG_BT_TBS_BEARER_COUNT
 */
#ifndef CONFIG_BT_TBS_SERVICE_COUNT
    #define CONFIG_BT_TBS_SERVICE_COUNT CONFIG_BT_TBS_BEARER_COUNT
#endif

/*! @brief The maximum length of the URI scheme list
 * Sets the maximum length of the URI scheme list. If BT_GTBS is enabled,
 * then the maximum length should be maximum 512 / BT_TBS_BEARER_COUNT.
 *
 *  Valid range 0 ~ 512
 */
#ifndef CONFIG_BT_TBS_MAX_SCHEME_LIST_LENGTH
    #define CONFIG_BT_TBS_MAX_SCHEME_LIST_LENGTH 30
#endif

/*! @brief TBS authorization requirement
 * If set to true, then any writable characteristics will require
 * authorization per connection.
 */
#ifndef CONFIG_BT_TBS_AUTHORIZATION
    #define CONFIG_BT_TBS_AUTHORIZATION 0
#endif

#endif /* CONFIG_BT_TBS */

//##################### Call Control Client #####################

/*! @brief Telephone Bearer Service client GTBS support
 * This option enables support for the generic TBS for the Call Control client.
 */
#ifndef CONFIG_BT_TBS_CLIENT_GTBS
    #define CONFIG_BT_TBS_CLIENT_GTBS 0
#endif

/*! @brief Telephone Bearer Service client support
 * This option enables support for the TBS-oriented Call Control client.
 */
#ifndef CONFIG_BT_TBS_CLIENT_TBS
    #define CONFIG_BT_TBS_CLIENT_TBS 0
#endif

#ifndef CONFIG_BT_TBS_CLIENT
    #if ((defined(CONFIG_BT_TBS_CLIENT_GTBS) && (CONFIG_BT_TBS_CLIENT_GTBS > 0)) || \
         (defined(CONFIG_BT_TBS_CLIENT_TBS) && (CONFIG_BT_TBS_CLIENT_TBS > 0)))
        #define CONFIG_BT_TBS_CLIENT 1
    #else
        #define CONFIG_BT_TBS_CLIENT 0
    #endif
#endif

#if (defined(CONFIG_BT_TBS_CLIENT) && (CONFIG_BT_TBS_CLIENT > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_TBS_CLIENT depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_TBS_CLIENT depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_TBS_CLIENT */

#if (defined(CONFIG_BT_TBS_CLIENT) && (CONFIG_BT_TBS_CLIENT > 0))

/*! @brief Maximum Number Of Calls Supported
 * Sets the maximum number of calls the client supports per TBS instance.
 */
#ifndef CONFIG_BT_TBS_CLIENT_MAX_CALLS
    #define CONFIG_BT_TBS_CLIENT_MAX_CALLS 1
#endif

/*! @brief Maximum number of TBS instances to setup
 * Sets the maximum number of Telephone Bearer Service (TBS)
 * instances to setup and use.
 *
 * Valid range 1 ~ 3
 */
#ifndef CONFIG_BT_TBS_CLIENT_MAX_TBS_INSTANCES
    #define CONFIG_BT_TBS_CLIENT_MAX_TBS_INSTANCES 1
#endif

/*! @brief Minimal TBS Client without optional procedures
 * This option disables all optional procedures in the TBS Client.
 */
#ifndef CONFIG_BT_TBS_CLIENT_MINIMAL
    #define CONFIG_BT_TBS_CLIENT_MINIMAL 0
#endif

/*! @brief Support reading Bearer Provider Name
 * This option enables support for reading Bearer Provider Name.
 */
#ifndef CONFIG_BT_TBS_CLIENT_BEARER_PROVIDER_NAME
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_BEARER_PROVIDER_NAME 0
    #else
        #define CONFIG_BT_TBS_CLIENT_BEARER_PROVIDER_NAME 1
    #endif
#endif

/*! @brief Support reading Bearer UCI
 * This option enables support for reading Bearer UCI.
 */
#ifndef CONFIG_BT_TBS_CLIENT_BEARER_UCI
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_BEARER_UCI 0
    #else
        #define CONFIG_BT_TBS_CLIENT_BEARER_UCI 1
    #endif
#endif

/*! @brief Support reading Bearer Technology
 * This option enables support for reading Bearer Technology.
 */
#ifndef CONFIG_BT_TBS_CLIENT_BEARER_TECHNOLOGY
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_BEARER_TECHNOLOGY 0
    #else
        #define CONFIG_BT_TBS_CLIENT_BEARER_TECHNOLOGY 1
    #endif
#endif

/*! @brief Support reading Bearer URI Schemes Supported List
 * This option enables support for reading Bearer URI Schemes Supported List.
 */
#ifndef CONFIG_BT_TBS_CLIENT_BEARER_URI_SCHEMES_SUPPORTED_LIST
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_BEARER_URI_SCHEMES_SUPPORTED_LIST 0
    #else
        #define CONFIG_BT_TBS_CLIENT_BEARER_URI_SCHEMES_SUPPORTED_LIST 1
    #endif
#endif

/*! @brief Support reading Bearer Signal Strength
 * This option enables support for reading Bearer Signal Strength.
 */
#ifndef CONFIG_BT_TBS_CLIENT_BEARER_SIGNAL_STRENGTH
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_BEARER_SIGNAL_STRENGTH 0
    #else
        #define CONFIG_BT_TBS_CLIENT_BEARER_SIGNAL_STRENGTH 1
    #endif
#endif

/*! @brief Support reading Bearer Signal Strength Reporting Interval
 * This option enables support for reading Bearer Signal Strength Reporting Interval.
 */
#ifndef CONFIG_BT_TBS_CLIENT_READ_BEARER_SIGNAL_INTERVAL
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_READ_BEARER_SIGNAL_INTERVAL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_READ_BEARER_SIGNAL_INTERVAL 1
    #endif
#endif

/*! @brief Support setting Bearer Signal Strength Reporting Interval
 * This option enables support for setting Bearer Signal Strength Reporting Interval.
 */
#ifndef CONFIG_BT_TBS_CLIENT_SET_BEARER_SIGNAL_INTERVAL
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_SET_BEARER_SIGNAL_INTERVAL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_SET_BEARER_SIGNAL_INTERVAL 1
    #endif
#endif

/*! @brief Support reading Bearer List Current Calls
 * This option enables support for reading Bearer List Current Calls.
 */
#ifndef CONFIG_BT_TBS_CLIENT_BEARER_LIST_CURRENT_CALLS
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_BEARER_LIST_CURRENT_CALLS 0
    #else
        #define CONFIG_BT_TBS_CLIENT_BEARER_LIST_CURRENT_CALLS 1
    #endif
#endif

/*! @brief Support reading Content Control ID
 * This option enables support for reading Content Control ID.
 */
#ifndef CONFIG_BT_TBS_CLIENT_CCID
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_CCID 0
    #else
        #define CONFIG_BT_TBS_CLIENT_CCID 1
    #endif
#endif

/*! @brief Support reading Incoming Call Target Bearer URI
 * This option enables support for reading Incoming Call Target Bearer URI.
 */
#ifndef CONFIG_BT_TBS_CLIENT_INCOMING_URI
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_INCOMING_URI 0
    #else
        #define CONFIG_BT_TBS_CLIENT_INCOMING_URI 1
    #endif
#endif

/*! @brief Support reading Status Flags
 * This option enables support for reading Status Flags.
 */
#ifndef CONFIG_BT_TBS_CLIENT_STATUS_FLAGS
    #if (defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0))
        #define CONFIG_BT_TBS_CLIENT_STATUS_FLAGS 0
    #else
        #define CONFIG_BT_TBS_CLIENT_STATUS_FLAGS 1
    #endif
#endif

/* This hidden option indicates that there are at least one control
 *  point procedure available.
 *  When this option is disabled it indicates that the control point can
 *  be optimized away.
 */
#ifndef CONFIG_BT_TBS_CLIENT_CP_PROCEDURES
    #define CONFIG_BT_TBS_CLIENT_CP_PROCEDURES 1
#endif

#if (defined(CONFIG_BT_TBS_CLIENT_CP_PROCEDURES) && (CONFIG_BT_TBS_CLIENT_CP_PROCEDURES > 0))
      #if !( (defined(CONFIG_BT_TBS_CLIENT_ACCEPT_CALL) && (CONFIG_BT_TBS_CLIENT_ACCEPT_CALL > 0)) || \
             (defined(CONFIG_BT_TBS_CLIENT_TERMINATE_CALL) && (CONFIG_BT_TBS_CLIENT_TERMINATE_CALL > 0)) || \
             (defined(CONFIG_BT_TBS_CLIENT_HOLD_CALL) && (CONFIG_BT_TBS_CLIENT_HOLD_CALL > 0)) || \
             (defined(CONFIG_BT_TBS_CLIENT_RETRIEVE_CALL) && (CONFIG_BT_TBS_CLIENT_RETRIEVE_CALL > 0)) || \
             (defined(CONFIG_BT_TBS_CLIENT_ORIGINATE_CALL) && (CONFIG_BT_TBS_CLIENT_ORIGINATE_CALL > 0)) || \
             (defined(CONFIG_BT_TBS_CLIENT_JOIN_CALLS) && (CONFIG_BT_TBS_CLIENT_JOIN_CALLS > 0)) \
            )
            #error CONFIG_BT_TBS_CLIENT_CP_PROCEDURES depends on at least one of: \
                      CONFIG_BT_TBS_CLIENT_ACCEPT_CALL, CONFIG_BT_TBS_CLIENT_TERMINATE_CALL, CONFIG_BT_TBS_CLIENT_HOLD_CALL, \
                      CONFIG_BT_TBS_CLIENT_RETRIEVE_CALL, CONFIG_BT_TBS_CLIENT_ORIGINATE_CALL, CONFIG_BT_TBS_CLIENT_JOIN_CALLS.
      #endif
#endif

/*! @brief Support Accept Call
 * This option enables support for answering an incoming call.
 */
#ifndef CONFIG_BT_TBS_CLIENT_ACCEPT_CALL
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_ACCEPT_CALL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_ACCEPT_CALL 1
    #endif
#endif

/*! @brief Support Terminate Call
 * This option enables support for terminating a call.
 */
#ifndef CONFIG_BT_TBS_CLIENT_TERMINATE_CALL
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_TERMINATE_CALL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_TERMINATE_CALL 1
    #endif
#endif

/*! @brief Support Hold Call
 * This option enables support for putting a call on hold.
 */
#ifndef CONFIG_BT_TBS_CLIENT_HOLD_CALL
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_HOLD_CALL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_HOLD_CALL 1
    #endif
#endif

/*! @brief Support Retrieve Call
 * This option enables support for retrieving a call on hold.
 */
#ifndef CONFIG_BT_TBS_CLIENT_RETRIEVE_CALL
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_RETRIEVE_CALL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_RETRIEVE_CALL 1
    #endif
#endif

/*! @brief Support Originate Call
 * This option enables support for originating a call.
 */
#ifndef CONFIG_BT_TBS_CLIENT_ORIGINATE_CALL
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_ORIGINATE_CALL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_ORIGINATE_CALL 1
    #endif
#endif

/*! @brief Support Join Calls
 * This option enables support for joining calls.
 */
#ifndef CONFIG_BT_TBS_CLIENT_JOIN_CALLS
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_JOIN_CALLS 0
    #else
        #define CONFIG_BT_TBS_CLIENT_JOIN_CALLS 1
    #endif
#endif

#if (defined(CONFIG_BT_TBS_CLIENT_JOIN_CALLS) && (CONFIG_BT_TBS_CLIENT_JOIN_CALLS > 0))

    #if !(defined(CONFIG_BT_TBS_CLIENT_MAX_CALLS) && (CONFIG_BT_TBS_CLIENT_MAX_CALLS > 1))
        #error CONFIG_BT_TBS_CLIENT_JOIN_CALLS depends on CONFIG_BT_TBS_CLIENT_MAX_CALLS > 1.
    #endif

#endif

/*! @brief Support reading Optional Opcodes
 * This option enables support for reading Optional Opcodes.
 */
#ifndef CONFIG_BT_TBS_CLIENT_OPTIONAL_OPCODES
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_OPTIONAL_OPCODES 0
    #else
        #define CONFIG_BT_TBS_CLIENT_OPTIONAL_OPCODES 1
    #endif
#endif

/*! @brief Support reading Incoming Call
 * This option enables support for reading Incoming Call.
 */
#ifndef CONFIG_BT_TBS_CLIENT_INCOMING_CALL
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_INCOMING_CALL 0
    #else
        #define CONFIG_BT_TBS_CLIENT_INCOMING_CALL 1
    #endif
#endif

/*! @brief Support reading Call Friendly Name
 * This option enables support for reading Call Friendly Name.
 */
#ifndef CONFIG_BT_TBS_CLIENT_CALL_FRIENDLY_NAME
    /* default !BT_TBS_CLIENT_MINIMAL */
    #if defined(CONFIG_BT_TBS_CLIENT_MINIMAL) && (CONFIG_BT_TBS_CLIENT_MINIMAL > 0)
        #define CONFIG_BT_TBS_CLIENT_CALL_FRIENDLY_NAME 0
    #else
        #define CONFIG_BT_TBS_CLIENT_CALL_FRIENDLY_NAME 1
    #endif
#endif

#endif /* CONFIG_BT_TBS_CLIENT */

#if (defined(CONFIG_BT_TBS) && (CONFIG_BT_TBS > 0)) || \
    (defined(CONFIG_BT_TBS_CLIENT) && (CONFIG_BT_TBS_CLIENT > 0))

/*! @brief The maximum length of the call URI supported
 * Sets the maximum length of the call URI supported. Note that if this
 * value is lower than a call URI, the call request will be rejected.
 *
 * Valid range 4 ~ 253
 */
#ifndef CONFIG_BT_TBS_MAX_URI_LENGTH
    #define CONFIG_BT_TBS_MAX_URI_LENGTH 30
#endif

/*! @brief The maximum length of the bearer provider name
 * Sets the maximum length of the bearer provider name.
 *
 * Valid range 0 ~ 512
 */
#ifndef CONFIG_BT_TBS_MAX_PROVIDER_NAME_LENGTH
    #define CONFIG_BT_TBS_MAX_PROVIDER_NAME_LENGTH 30
#endif

#endif /* CONFIG_BT_TBS || CONFIG_BT_BASS_CLIENT */

#endif /* CONFIG_BT_AUDIO */

#endif /* __CONFIG_TBS_H__ */