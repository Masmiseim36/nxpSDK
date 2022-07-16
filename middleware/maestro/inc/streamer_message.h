/*
 * Copyright 2018-2022 NXP.
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

/**
 * @brief Build a message
 *
 *
 * @param msg: Message id
 * @param type: message type
 *
 * @details
 * Max ID of message is 8. Each ID can have 24 types.
 * Bits 31-24: Message ID
 * Bits 23-0:  Message type
 *
 *
 */
#define BLD_MESSAGE(msg, type) ((uint32_t)(((uint32_t)(msg) << 24) | ((uint32_t)(type))))

/**
 * @brief Get message ID
 *
 */
#define MESSAGE_ID(msg) (uint32_t)((msg) >> 24)

/**
 * @brief Get message type
 *
 */
#define MESSAGE_TYPE(msg) (uint32_t)((msg)&0xFFF)

/*-------------------------------- Messages ---------------------------------*/

/** @brief Null message */
#define MSG_NULL (uint32_t)0

/** @brief End of stream message from the pipeline. */
#define MSG_EOS BLD_MESSAGE(MESSAGE_EOS, 0)

/** @brief Actual error type will be decoder dependent passed on in message data field. */
#define MSG_ERROR_AUDIO BLD_MESSAGE(MESSAGE_ERROR, ERROR_AUDIO_FATAL)

/** @brief Actual warning type will be decoder dependent passed on in message data field */
#define MSG_WARNING_AUDIO BLD_MESSAGE(MESSAGE_ERROR, WARNING_AUDIO)

/** @brief Message data will contain the bitrate in bits per second. */
#define MSG_INFO_AUDIO_BITRATE BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_BITRATE)

/** @brief Message data will contain the number of channels supported. */
#define MSG_INFO_AUDIO_CHANNELS BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_CHANNELS)

/** @brief Message data will contain the audio sampling rate in Hz. */
#define MSG_INFO_AUDIO_SAMPLERATE BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_SAMPLERATE)

/** @brief Message data will contain the type of audio. Audio type depends on the decoders supported. */
#define MSG_INFO_AUDIO_TYPE BLD_MESSAGE(MESSAGE_INFO, INFO_AUDIO_TYPE)

/** @brief Message data will contain the pointer to a string of codec info */
#define MSG_INFO_CODEC_INFO BLD_MESSAGE(MESSAGE_INFO, INFO_CODEC_INFO)

/** @brief Message data will contain the total duration of playback in milliseconds. */
#define MSG_INFO_DURATION BLD_MESSAGE(MESSAGE_INFO, INFO_DURATION)

/** @brief Nothing in message data.  get tag function should be called to get the required tag. */
#define MSG_INFO_ALBUM BLD_MESSAGE(MESSAGE_INFO, INFO_ALBUM)

/** @brief Nothing in message data.  get tag function should be called to get the required tag. */
#define MSG_INFO_ARTIST BLD_MESSAGE(MESSAGE_INFO, INFO_ARTIST)

/** @brief Nothing in message data.  get tag function should be called to get the required tag. */
#define MSG_INFO_TITLE BLD_MESSAGE(MESSAGE_INFO, INFO_TITLE)

/** @brief Nothing in message data.  get tag function should be called to get the required tag. */
#define MSG_INFO_GENRE BLD_MESSAGE(MESSAGE_INFO, INFO_GENRE)

/** @brief Nothing in message data.  get tag function should be called to get the required tag. */
#define MSG_INFO_YEAR BLD_MESSAGE(MESSAGE_INFO, INFO_YEAR)

/** @brief Nothing in message data.  get tag function should be called to get the required tag. */
#define MSG_INFO_TRACK BLD_MESSAGE(MESSAGE_INFO, INFO_TRACK)

/** @brief Message data will contain the total duration of playback in milliseconds. */
#define MSG_INFO_ALBUMART BLD_MESSAGE(MESSAGE_INFO, INFO_ALBUMART_IMAGE)

/** @brief Message data will contain the current time of playback in milliseconds. */
#define MSG_INFO_POSITION BLD_MESSAGE(MESSAGE_INFO, INFO_POSITION)

/** @brief Nothing in message data. indicate that all stream information is updated */
#define MSG_STREAM_INFO_UPDATED BLD_MESSAGE(MESSAGE_INFO, INFO_STREAM_INFO_UPDATE)

/** @brief Nothing in message data. indicate that all metadata information is updated */
#define MSG_META_INFO_UPDATED BLD_MESSAGE(MESSAGE_INFO, INFO_META_INFO_UPDATE)

/** @brief Message data will contain the error type. */
#define MSG_STREAM_ERROR BLD_MESSAGE(MESSAGE_INFO, INFO_ERROR)

#endif /* STREAMER_MESSAGE_H */
