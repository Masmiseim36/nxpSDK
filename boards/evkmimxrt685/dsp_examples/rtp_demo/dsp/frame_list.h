/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __FRAME_LIST_H__
#define __FRAME_LIST_H__

#include <stdbool.h>
#include <stdint.h>
#include <xtensa/xos.h>

#include "rtp.h"
#include "xa_g711_codec_api.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief Compares if timestamp is less than other timestamp, accounting for 32 bit wraparound within 0x100000 samples
 * tolerance (about 131 seconds) */
#define TIMESTAMP_LESS_THAN(timestamp, other)                             \
    (((((uint32_t)(other)) > ((uint32_t)(timestamp))) &&                  \
      ((((uint32_t)(other)) - ((uint32_t)(timestamp))) < 0xFFF00000U)) || \
     ((((uint32_t)(other)) < ((uint32_t)(timestamp))) &&                  \
      ((((uint32_t)(timestamp)) - ((uint32_t)(other))) >= 0xFFF00000U)))

/*! @brief Compares if timestamp is less than or equal to other timestamp, accounting for 32 bit wraparound within
 * 0x100000 samples tolerance (about 131 seconds) */
#define TIMESTAMP_LESS_THAN_OR_EQUAL(timestamp, other) \
    (((timestamp) == (other)) || TIMESTAMP_LESS_THAN((timestamp), (other)))

/*! @brief RTP frame descriptor */
typedef struct _rtp_frame
{
    xa_g711_law_t law;            /*!< Encoding law of the data in this frame */
    uint32_t timestamp;           /*!< Timestamp of the first sample of this frame */
    uint8_t *data;                /*!< Pointer to encoded audio data of this frame */
    rtp_header_t *dealloc_packet; /*!< Address of RTP packet to free after playback, if packet contains more frames, is
                                     set for last frame only */
    struct _rtp_frame *prev;      /*!< Previous frame in a linked list */
    struct _rtp_frame *next;      /*!< Next frame in a linked list */
} rtp_frame_t;

/*! @brief linked list of RTP frame descriptors */
typedef struct _frame_list
{
    rtp_frame_t *head;              /*!< Head of received data */
    rtp_frame_t *tail;              /*!< Tail of received data */
    rtp_frame_t *free;              /*!< List of unused frame descriptors */
    uint32_t last_played_timestamp; /*!< Timestamp of the first sample of the frame recently removed from the list */
    bool last_played_set;           /*!< If last_played_timestamp holds valid value (0 may be legitimate value) */
} frame_list_t;

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*!
 * @brief    Create and initialize a frame list structure
 *
 * @param    size max number of referenced frames
 * @returns  Initialized and allocated pointer to frame list or NULL
 */
frame_list_t *frame_list_create(uint32_t size);

/*!
 * @brief    Destroy and free frame list structure
 *
 * @param    list Pointer to frame list object created with frame_list_create
 */
void frame_list_destroy(frame_list_t *list);

/*!
 * @brief    Return if there is no frame in the list
 *
 * @param    list Pointer to frame list object created with frame_list_create
 * @returns  true if empty, false otherwise
 */
bool frame_list_is_empty(frame_list_t *list);

/*!
 * @brief    Return if there is no space in the list
 *
 * @param    list Pointer to frame list object created with frame_list_create
 * @returns  true if full, false otherwise
 */
bool frame_list_is_full(frame_list_t *list);

/*!
 * @brief    Adds frames from the packet to frame list
 *
 * @param    list Pointer to frame list object created with frame_list_create
 * @param    packet RTP packet
 * @param    packet_size packet length including RTP header
 * @returns  true if any frames from the packet were added, false otherwise
 */
bool frame_list_add(frame_list_t *list, rtp_header_t *packet, uint32_t packet_size);

/*!
 * @brief    Remove frame descriptor with lowest timestamp from frame list
 *
 * @param    list Pointer to frame list object created with frame_list_create
 */
void frame_list_remove_first(frame_list_t *list);

/*!
 * @brief    Peak at frame with the lowest timestamp from frame list without removing it
 *
 * @param    list Pointer to frame list object created with frame_list_create
 * @returns  Reference to the frame or NULL if list is empty
 */
rtp_frame_t *frame_list_first(frame_list_t *list);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* __FRAME_LIST_H__ */
