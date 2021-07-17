/*
 * Copyright 2019-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "frame_list.h"

/*******************************************************************************
 * Code
 ******************************************************************************/
frame_list_t *frame_list_create(uint32_t size)
{
    frame_list_t *list;
    rtp_frame_t *prev = NULL;
    rtp_frame_t *next = NULL;
    int i;

    list = malloc(sizeof(frame_list_t));
    if (list == NULL)
    {
        return NULL;
    }

    memset((void *)list, 0, sizeof(frame_list_t));

    for (i = 0; i < size; i++)
    {
        next = malloc(sizeof(rtp_frame_t));
        if (next == NULL)
        {
            frame_list_destroy(list);
            return NULL;
        }
        memset((void *)next, 0, sizeof(rtp_frame_t));

        if (prev == NULL)
        {
            list->free = next;
        }
        else
        {
            prev->next = next;
            next->prev = prev;
        }

        prev = next;
    }

    return list;
}

void frame_list_destroy(frame_list_t *list)
{
    rtp_frame_t *frame = NULL;

    while (list->head != NULL)
    {
        frame      = list->head;
        list->head = frame->next;
        free(frame);
    }

    while (list->free != NULL)
    {
        frame      = list->free;
        list->free = frame->next;
        free(frame);
    }

    free(list);
}

bool frame_list_is_empty(frame_list_t *list)
{
    return (list->head == NULL);
}

bool frame_list_is_full(frame_list_t *list)
{
    return (list->free == NULL);
}

bool frame_list_add(frame_list_t *list, rtp_header_t *packet, uint32_t packet_size)
{
    int frames    = (packet_size - RTP_HEADER_SIZE + 1) / XA_G711_MAX_NUM_BYTES_PER_FRAME;
    uint8_t *data = ((uint8_t *)packet) + RTP_HEADER_SIZE;
    rtp_frame_t *frame;
    rtp_frame_t *prev;
    xa_g711_law_t law;
    int i;

    switch (packet->payload_type & 0x7F)
    {
        case RTP_PAYLOAD_TYPE_PCMU:
            law = XA_G711_LAW_U;
            break;
        case RTP_PAYLOAD_TYPE_PCMA:
            law = XA_G711_LAW_A;
            break;
        default:
            /* Unsupported payload type */
            return false;
    }

    if (frames < 1)
    {
        /* packet_size > 0 but not divisible by XA_G711_MAX_NUM_BYTES_PER_FRAME is not supported */
        return false;
    }

    if (list->last_played_set && TIMESTAMP_LESS_THAN_OR_EQUAL(packet->timestamp, list->last_played_timestamp))
    {
        /* This time already played */
        return false;
    }

    /* Find if there are enough free frames to accommodate all packet data */

    frame = list->free;

    for (i = 0; ((i < frames) && (frame != NULL)); i++)
    {
        frame = frame->next;
    }

    if (i < frames)
    {
        /* Not enough free frames */
        return false;
    }

    /* Find frame with smaller timestamp to append packet to */

    prev = list->tail;

    while ((prev != NULL) && TIMESTAMP_LESS_THAN(packet->timestamp, prev->timestamp))
    {
        prev = prev->prev;
    }

    if ((prev != NULL) && (prev->timestamp == packet->timestamp))
    {
        /* Duplicate packet */
        return false;
    }

    /* Append packet data to the list */
    for (i = 0; i < frames; i++)
    {
        /* Take free frame */
        frame      = list->free;
        list->free = frame->next;
        if (list->free != NULL)
        {
            list->free->prev = NULL;
        }

        /* Set information about audio data */
        frame->law            = law;
        frame->dealloc_packet = NULL;
        frame->data           = data;
        data += XA_G711_MAX_NUM_BYTES_PER_FRAME;
        frame->timestamp = packet->timestamp + (i * XA_G711_MAX_NUM_BYTES_PER_FRAME);

        frame->prev = prev;

        if (prev == NULL)
        {
            /* The list is empty or its frames have later timestamps */
            frame->next = list->head;
            list->head  = frame;
        }
        else
        {
            /* Append to previous */
            frame->next = prev->next;
            prev->next  = frame;
        }

        if (frame->next != NULL)
        {
            frame->next->prev = frame;
        }
        else
        {
            list->tail = frame;
        }

        /* Next frame will be appended to this one */
        prev = frame;
    }

    /* When frame with last piece of packet data is consumed, release the packet */
    frame->dealloc_packet = packet;

    return true;
}

void frame_list_remove_first(frame_list_t *list)
{
    rtp_frame_t *frame;

    if (list->head == NULL)
    {
        return;
    }

    frame      = list->head;
    list->head = frame->next;

    if (list->head != NULL)
    {
        list->head->prev = NULL;
    }
    else
    {
        list->tail = NULL;
    }

    if (list->free == NULL)
    {
        frame->next = NULL;
        list->free  = frame;
    }
    else
    {
        frame->next       = list->free;
        frame->next->prev = frame;
        list->free        = frame;
    }

    frame->prev           = NULL; // should be already NULL since we are removing from head
    frame->dealloc_packet = NULL;
    frame->data           = NULL;
    frame->timestamp      = 0U;
}

rtp_frame_t *frame_list_first(frame_list_t *list)
{
    return list->head;
}
