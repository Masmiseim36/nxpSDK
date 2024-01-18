/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_MCS_H__
#define __CONFIG_MCS_H__

//##################### Media Control Service #####################

/*! @brief Media Control Service Support
 * This option enables support for the Media Control Service.
 */
#ifndef CONFIG_BT_MCS
    #define CONFIG_BT_MCS 0
#endif

#if (defined(CONFIG_BT_MCS) && (CONFIG_BT_MCS > 0))

    #if (!defined(CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL) || !(CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL > 0))
        #error CONFIG_BT_MCS depends on CONFIG_MCTL_LOCAL_PLAYER_REMOTE_CONTROL.
    #endif

    #if (!defined(CONFIG_UTF8) || !(CONFIG_UTF8 > 0))
        #error CONFIG_BT_MCS depends on CONFIG_UTF8.
    #endif

    #if (!defined(CONFIG_BT_CCID) || !(CONFIG_BT_CCID > 0))
        #error CONFIG_BT_MCS depends on CONFIG_BT_CCID.
    #endif

    #if (!defined(CONFIG_BT_GATT_DYNAMIC_DB) || !(CONFIG_BT_GATT_DYNAMIC_DB > 0))
        #error CONFIG_BT_MCS depends on CONFIG_BT_GATT_DYNAMIC_DB.
    #endif

#endif /* CONFIG_BT_MCS */

//##################### Media Control Client #####################

/*! @brief Media Control Client Support
 * This option enables support for the Media Control Client.
 */
#ifndef CONFIG_BT_MCC
    #define CONFIG_BT_MCC 0
#endif

#if (defined(CONFIG_BT_MCC) && (CONFIG_BT_MCC > 0))

    #if (!defined(CONFIG_BT_GATT_CLIENT) || !(CONFIG_BT_GATT_CLIENT > 0))
        #error CONFIG_BT_MCC depends on CONFIG_BT_GATT_CLIENT.
    #endif

    #if (!defined(CONFIG_BT_GATT_AUTO_DISCOVER_CCC) || !(CONFIG_BT_GATT_AUTO_DISCOVER_CCC > 0))
        #error CONFIG_BT_MCC depends on CONFIG_BT_GATT_AUTO_DISCOVER_CCC.
    #endif

#endif /* CONFIG_BT_MCC */

#if (defined(CONFIG_BT_MCC) && (CONFIG_BT_MCC > 0))

/*! @brief Max length of media player name
 * Sets the maximum number of bytes (including the null termination) of
 * the name of the media player.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MCC_MEDIA_PLAYER_NAME_MAX
    #define CONFIG_BT_MCC_MEDIA_PLAYER_NAME_MAX 20
#endif

/*! @brief Max length of media player icon URL
 * Sets the maximum number of bytes (including the null termination) of
 * the media player icon URL.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MCC_ICON_URL_MAX
    #define CONFIG_BT_MCC_ICON_URL_MAX 40
#endif

/*! @brief Max length of the title of a track
 * Sets the maximum number of bytes (including the null termination) of
 * the title of any track in the media player.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MCC_TRACK_TITLE_MAX
    #define CONFIG_BT_MCC_TRACK_TITLE_MAX 40
#endif

/*! @brief Max length of the name of a track segment
 * Sets the the maximum number of bytes (including the null termination)
 * of the name of any track segment in the media player.
 *
 * Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MCC_SEGMENT_NAME_MAX
    #define CONFIG_BT_MCC_SEGMENT_NAME_MAX 25
#endif

/*! @brief Media Control Client support for Object Transfer Service
 * Use this option to configure support in the Media Control Client for
 * an included Object Transfer Service in the Media Control Service.
 */
#ifndef CONFIG_BT_MCC_OTS
    #define CONFIG_BT_MCC_OTS 0
#endif

#if (defined(CONFIG_BT_MCC_OTS) && (CONFIG_BT_MCC_OTS > 0))

    #if (!defined(CONFIG_BT_OTS_CLIENT) || !(CONFIG_BT_OTS_CLIENT > 0))
        #error CONFIG_BT_MCC_OTS depends on CONFIG_BT_OTS_CLIENT.
    #endif

#endif /* CONFIG_BT_MCC_OTS */

#if (defined(CONFIG_BT_MCC_OTS) && (CONFIG_BT_MCC_OTS > 0))

/*! @brief The size of the buffer used for OTC object in MCC
 * Sets the size (in octets) of the buffer used for receiving the content
 * of objects.
 * Should be set large enough to fit any expected object.
 *
 * Valid range 1 ~ 65536
 */
#ifndef CONFIG_BT_MCC_OTC_OBJ_BUF_SIZE
    #define CONFIG_BT_MCC_OTC_OBJ_BUF_SIZE 512
#endif

/*! @brief Total memory size to use for storing the content of objects
 * Sets the total memory size (in octets) to use for storing the content
 * of objects.
 * This is used for the total memory pool buffer size from which memory
 * is allocated when reading object content.
 *
 * Valid range 0 ~ 65536
 */
#ifndef CONFIG_BT_MCC_TOTAL_OBJ_CONTENT_MEM
    #define CONFIG_BT_MCC_TOTAL_OBJ_CONTENT_MEM 1
#endif

/*! @brief Maximum number of tracks segments in a track segment object
 * Sets the maximum number of tracks segments in a track segment object.
 * If the received object is bigger, the remaining data in the object
 * will be ignored.
 *
 * Valid range 0 ~ 500
 */
#ifndef CONFIG_BT_MCC_TRACK_SEGS_MAX_CNT
    #define CONFIG_BT_MCC_TRACK_SEGS_MAX_CNT 25
#endif

/*! @brief Maximum number of records in a group object
 * Sets the maximum number of records in a group object. If the received
 * group object has more records than this, the remaining records in the
 * object will be ignored.
 *
 * Valid range 0 ~ 500
 */
#ifndef CONFIG_BT_MCC_GROUP_RECORDS_MAX
    #define CONFIG_BT_MCC_GROUP_RECORDS_MAX 25
#endif

#endif /* CONFIG_BT_MCC_OTS */

/*! @brief Media Control Client Shell Support
 * This option enables shell support for the Media Control Client.
 */
#if (defined(CONFIG_BT_SHELL) && (CONFIG_BT_SHELL > 0))
    #ifndef CONFIG_BT_MCC_SHELL
        #define CONFIG_BT_MCC_SHELL 1
    #endif
#endif /* CONFIG_BT_SHELL */

#endif /* CONFIG_BT_MCC */

#endif /* __CONFIG_MCS_H__ */