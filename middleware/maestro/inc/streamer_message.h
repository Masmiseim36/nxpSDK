/*
 * Copyright 2018-2020 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#ifndef STREAMER_MESSAGE_H
#define STREAMER_MESSAGE_H

/*!
 * @file    streamer_message.h
 * @brief   This file includes definitions for messages used by the Streamer
 *          library
 */

/*
 * CONSTANTS, DEFINES AND MACROS
 */

/* BLD_MESSAGE:
 * @msg: Message id
 * @type: message type
 *
 *   Msg Id                 Message type
 *  ---------------------------------------------------------
 *  |           |                                           |
 *  ---------------------------------------------------------
 *    31-24                    23-0
 *
 * Max ids of message is 8. Each Id can have 24 types.
 */
#define BLD_MESSAGE(msg, type)   \
        ((uint32_t)(((uint32_t)(msg)<<24)|((uint32_t)(type))))

#define MESSAGE_ID(msg)          (uint32_t)((msg) >> 24)
#define MESSAGE_TYPE(msg)        (uint32_t)((msg) & 0xFFF)

/*-------------------------------- Messages ---------------------------------*/
#define MSG_NULL    (uint32_t)0

/*!
 * MSG_EOS
 * End of stream message from the pipeline.
 */
#define MSG_EOS                  BLD_MESSAGE(MESSAGE_EOS, 0)

/*!
 * MSG_ERROR_AUDIO
 * Actual error type will be decoder dependent passed on in message data field.
 */
#define MSG_ERROR_AUDIO          BLD_MESSAGE(MESSAGE_ERROR, ERROR_VIDEO_FATAL)

/*!
 * MSG_ERROR_VIDEO
 * Actual error type will be decoder dependent passed on in message data field.
 */
#define MSG_ERROR_VIDEO          BLD_MESSAGE(MESSAGE_ERROR, ERROR_AUDIO_FATAL)

/*!
 * MSG_WARNING_AUDIO
 * Actual warning type will be decoder dependent passed on in message data field
 */
#define MSG_WARNING_AUDIO        BLD_MESSAGE(MESSAGE_ERROR, WARNING_AUDIO)

/*!
 * MSG_WARNING_VIDEO
 * Actual warning type will be decoder dependent passed on in message data field
 */
#define MSG_WARNING_VIDEO        BLD_MESSAGE(MESSAGE_ERROR, WARNING_VIDEO)

/*!
 * MSG_INFO_VIDEO_BITRATE
 * Message data will contain the bitrate in bits per second.
 */
#define MSG_INFO_VIDEO_BITRATE   BLD_MESSAGE(MESSAGE_INFO, INFO_VIDEO_BITRATE)

/*!
 * MSG_INFO_VIDEO_FRAMERATE
 * Message data will contain the bitrate in frames per second.
 */
#define MSG_INFO_VIDEO_FRAMERATE BLD_MESSAGE(MESSAGE_INFO, INFO_VIDEO_FRAMERATE)

/*!
 * MSG_INFO_VIDEO_TYPE
 * Message data will contain the type of video. Video type depends on the
 * decoders supported.
 */
#define MSG_INFO_VIDEO_TYPE      BLD_MESSAGE(MESSAGE_INFO, INFO_VIDEO_TYPE)

/*!
 * MSG_INFO_AUDIO_BITRATE
 * Message data will contain the bitrate in bits per second.
 */
#define MSG_INFO_AUDIO_BITRATE   BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_BITRATE)

/*!
 * MSG_INFO_AUDIO_CHANNELS
 * Message data will contain the number of channels supported.
 */
#define MSG_INFO_AUDIO_CHANNELS  BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_CHANNELS)

/*!
 * MSG_INFO_AUDIO_SAMPLERATE
 * Message data will contain the audio sampling rate in Hz.
 */
#define MSG_INFO_AUDIO_SAMPLERATE    \
        BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_SAMPLERATE)

/*!
 * MSG_INFO_AUDIO_TYPE
 * Message data will contain the type of audio. Audio type depends on the
 * decoders supported.
 */
#define MSG_INFO_AUDIO_TYPE      BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_TYPE)

/*!
 * MSG_INFO_CODEC_INFO
 * Message data will contain the pointer to a string of codec info
 */
#define MSG_INFO_CODEC_INFO      BLD_MESSAGE(MESSAGE_INFO, INFO_CODEC_INFO)

/*!
 * MSG_INFO_DURATION
 * Message data will contain the total duration of playback in milliseconds.
 */
#define MSG_INFO_DURATION        BLD_MESSAGE(MESSAGE_INFO, INFO_DURATION)

/*!
 * MSG_INFO_ALBUM
 * Nothing in message data.  get tag function should be called to get the
 * required tag.
 */
#define MSG_INFO_ALBUM            BLD_MESSAGE(MESSAGE_INFO, INFO_ALBUM)

/*!
 * MSG_INFO_ARTIST
 * Nothing in message data.  get tag function should be called to get the
 * required tag.
 */
#define MSG_INFO_ARTIST           BLD_MESSAGE(MESSAGE_INFO, INFO_ARTIST)

/*!
 * MSG_INFO_TITLE
 * Nothing in message data.  get tag function should be called to get the
 * required tag.
 */
#define MSG_INFO_TITLE            BLD_MESSAGE(MESSAGE_INFO, INFO_TITLE)

/*!
 * MSG_INFO_GENRE
 * Nothing in message data.  get tag function should be called to get the
 * required tag.
 */
#define MSG_INFO_GENRE            BLD_MESSAGE(MESSAGE_INFO, INFO_GENRE)

/*!
 * MSG_INFO_YEAR
 * Nothing in message data.  get tag function should be called to get the
 * required tag.
 */
#define MSG_INFO_YEAR             BLD_MESSAGE(MESSAGE_INFO, INFO_YEAR)

/*!
 * MSG_INFO_TRACK
 * Nothing in message data.  get tag function should be called to get the
 * required tag.
 */

#define MSG_INFO_TRACK             BLD_MESSAGE(MESSAGE_INFO, INFO_TRACK)

/*!
 * MSG_INFO_ALBUMART
 * Message data will contain the total duration of playback in milliseconds.
 */
#define MSG_INFO_ALBUMART         BLD_MESSAGE(MESSAGE_INFO, INFO_ALBUMART_IMAGE)

/*!
 * MSG_INFO_POSITION
 * Message data will contain the current time of playback in milliseconds.
 */
#define MSG_INFO_POSITION         BLD_MESSAGE(MESSAGE_INFO, INFO_POSITION)

/*!
 * MSG_STREAM_INFO_UPDATED
 * Nothing in message data. indicate that all stream information is updated
 */
#define MSG_STREAM_INFO_UPDATED   \
                            BLD_MESSAGE(MESSAGE_INFO, INFO_STREAM_INFO_UPDATE)

/*!
 * MSG_META_INFO_UPDATED
 * Nothing in message data. indicate that all metadata information is updated
 */
#define MSG_META_INFO_UPDATED   \
                            BLD_MESSAGE(MESSAGE_INFO, INFO_META_INFO_UPDATE)

/*!
 * MSG_STREAM_ERROR
 * Message data will contain the error type.
 */
#define MSG_STREAM_ERROR   \
                            BLD_MESSAGE(MESSAGE_INFO, INFO_ERROR)

#endif /* STREAMER_MESSAGE_H */
