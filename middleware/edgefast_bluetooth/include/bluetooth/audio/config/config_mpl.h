/*
 * Copyright 2022-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __CONFIG_MPL_H__
#define __CONFIG_MPL_H__

/*! @brief Support for media player
 * Enables support for media player
 * Note that the provided media player is a sample that only provides a
 * mock-up with no actual media being played.
 * For real media playback, the sample must be extended, hooked up to a
 * real media player or replaced with a real media player.
 */
#if (defined(CONFIG_BT_MPL) && (CONFIG_BT_MPL > 0))

    #if (!defined(CONFIG_BT_CCID) || !(CONFIG_BT_CCID > 0))
        #error CONFIG_BT_MPL depends on CONFIG_BT_CCID.
    #endif

#endif /* CONFIG_BT_MPL */

#if (defined(CONFIG_BT_MPL) && (CONFIG_BT_MPL > 0))

/*! @brief Media Player Name
 * Use this option to set the name of the media player.
 */
#ifndef CONFIG_BT_MPL_MEDIA_PLAYER_NAME
    #define CONFIG_BT_MPL_MEDIA_PLAYER_NAME "Player0"
#endif

/*! @brief Max length of media player name
 * Sets the maximum number of bytes (including the null termination) of
 * the name of the media player.
 *
 *  Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MPL_MEDIA_PLAYER_NAME_MAX
    #define CONFIG_BT_MPL_MEDIA_PLAYER_NAME_MAX 20
#endif

/*! @brief Media player Icon URL
 * Use this option to set the URL of the Media Player Icon.
 */
#ifndef CONFIG_BT_MPL_ICON_URL
    #define CONFIG_BT_MPL_ICON_URL "http://server.some.where/path/icon.png"
#endif

/*! @brief Max length of media player icon URL
 * Sets the maximum number of bytes (including the null termination) of
 * the media player icon URL.
 *
 *  Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MPL_ICON_URL_MAX
    #define CONFIG_BT_MPL_ICON_URL_MAX 40
#endif

/*! @brief Max length of the title of a track
 * Sets the maximum number of bytes (including the null termination) of
 * the title of any track in the media player.
 *
 *  Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MPL_TRACK_TITLE_MAX
    #define CONFIG_BT_MPL_TRACK_TITLE_MAX 40
#endif

/*! @brief Max length of the name of a track segment
 * Sets the the maximum number of bytes (including the null termination)
 * of the name of any track segment in the media player.
 *
 *  Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MPL_SEGMENT_NAME_MAX
    #define CONFIG_BT_MPL_SEGMENT_NAME_MAX 25
#endif

/*! @brief Max length of the title of a group of tracks
 * Sets the maximum number of bytes (including the null termination) of
 * the title of any group in the media player.
 *
 *  Valid range 1 ~ 255
 */
#ifndef CONFIG_BT_MPL_GROUP_TITLE_MAX
    #define CONFIG_BT_MPL_GROUP_TITLE_MAX CONFIG_BT_MPL_TRACK_TITLE_MAX
#endif

/*! @brief Support for media player objects
 * Enables support for objects in the media player
 * Objects are used to give/get more information about e.g. media tracks.
 * Requires the Object Transfer Service.
 */
#ifndef CONFIG_BT_MPL_OBJECTS
    #define CONFIG_BT_MPL_OBJECTS 0
#endif

#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))

    #if (!defined(CONFIG_BT_OTS) || !(CONFIG_BT_OTS > 0))
        #error CONFIG_BT_MPL_OBJECTS depends on CONFIG_BT_OTS.
    #endif

    #if (!defined(CONFIG_BT_MCS) || !(CONFIG_BT_MCS > 0))
        #error CONFIG_BT_MPL_OBJECTS depends on CONFIG_BT_MCS.
    #endif

#endif /* CONFIG_BT_MPL_OBJECTS */

#if (defined(CONFIG_BT_MPL_OBJECTS) && (CONFIG_BT_MPL_OBJECTS > 0))

/*! @brief Total memory size to use for storing the content of objects
 * Sets the total memory size (in octets) to use for storing the content of objects.
 * This is used for the total memory pool buffer size from which memory
 * is allocated when sending object content.
 *
 * Valid range 0 ~ 65536
 */
#ifndef CONFIG_BT_MPL_MAX_OBJ_SIZE
    #define CONFIG_BT_MPL_MAX_OBJ_SIZE 127
#endif

/*! @brief Media player Icon bitmap object size
 * This option sets the maximum size (in octets) of the icon object.
 */
#ifndef CONFIG_BT_MPL_ICON_BITMAP_SIZE
    #define CONFIG_BT_MPL_ICON_BITMAP_SIZE 127
#endif

/*! @brief Maximum size for a track object
 * This option sets the maximum size (in octets) of a track object.
 */
#ifndef CONFIG_BT_MPL_TRACK_MAX_SIZE
    #define CONFIG_BT_MPL_TRACK_MAX_SIZE 127
#endif

#endif /* CONFIG_BT_MPL_OBJECTS */

#endif /* CONFIG_BT_MPL */

#endif /* __CONFIG_MPL_H__ */