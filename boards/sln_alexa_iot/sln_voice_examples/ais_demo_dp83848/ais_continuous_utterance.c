/*
 * Copyright 2018 NXP.
 * This software is owned or controlled by NXP and may only be used strictly in accordance with the
 * license terms that accompany it. By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that you have read, and that you
 * agree to comply with and are bound by, such license terms. If you do not agree to be bound by the
 * applicable license terms, then you may not retain, install, activate or otherwise use the software.
 */

#include "ais_continuous_utterance.h"

__attribute__((section(".ocram_non_cacheable_data"))) static uint8_t ring_buffer[RING_BUFFER_SIZE] = {0};

/* memory vs performance trade-off
 * shifting the huge buffer above in order to have a contiguous
 * array of 500ms pre-wake word + wake word would require a tremendous number
 * of memory write operations without a bigger backup buffer ... */
__attribute__((section(".ocram_non_cacheable_data"))) static uint8_t aux_backup_buffer[PRE_UTTERANCE_SIZE] = {0};

static uint32_t ring_buffer_index = 0;
static uint8_t buffer_filled_once = 0;

static void buffer_shift_left(uint32_t shift_size, uint32_t shift_index);
static void buffer_shift_right(uint32_t shift_size, uint32_t shift_index);

uint8_t continuous_utterance_samples_add(uint8_t *samples_buffer, uint32_t samples_buffer_len)
{
    if (samples_buffer_len > RING_BUFFER_SIZE || !samples_buffer)
    {
        return 1;
    }

    for (int i = 0; i < samples_buffer_len; i++)
    {
        if (ring_buffer_index == RING_BUFFER_SIZE)
        {
            ring_buffer_index  = 0;
            buffer_filled_once = 1;
        }
        ring_buffer[ring_buffer_index++] = samples_buffer[i];
    }

    return 0;
}

uint8_t continuous_utterance_buffer_set(uint8_t **buffer, uint32_t *buffer_len, uint16_t wake_word_started)
{
    uint32_t ais_buffer_size           = 0;
    uint32_t wake_word_size            = 0;
    uint32_t wake_word_start_index     = 0;
    uint32_t pre_wake_word_start_index = 0;

    if (!buffer || !buffer_len || !wake_word_started)
    {
        return 1;
    }

    /* calculate length of pre-utterance + wake word */
    wake_word_size  = wake_word_started * 160 * PCM_SAMPLE_SIZE_BYTES;
    ais_buffer_size = PRE_UTTERANCE_SIZE + wake_word_size;

    if (ais_buffer_size > RING_BUFFER_SIZE)
        return 1;

    /* determine wake word start index in ring buffer */
    if (ring_buffer_index >= wake_word_size)
    {
        wake_word_start_index = ring_buffer_index - wake_word_size;
    }
    else
    {
        wake_word_start_index = RING_BUFFER_SIZE - (wake_word_size - ring_buffer_index);
    }

    /* determine pre wake word start index in ring buffer */
    if (wake_word_start_index >= PRE_UTTERANCE_SIZE)
    {
        pre_wake_word_start_index = wake_word_start_index - PRE_UTTERANCE_SIZE;
    }
    else
    {
        pre_wake_word_start_index = RING_BUFFER_SIZE - (PRE_UTTERANCE_SIZE - wake_word_start_index);
    }

    /* need to move array segments until the below values are in ascending order:
     * pre_wake_word_start_index, wake_word_start_index and ring_buffer_index */

    uint32_t to_shift_size  = 0;
    uint32_t to_shift_index = 0;
    uint8_t shift_direction = SHIFT_LEFT;

    if (pre_wake_word_start_index < wake_word_start_index && wake_word_start_index < ring_buffer_index)
    {
        /*
         *        A               B               C                D
         *  |------------|----------------|---------------|-----------------|
         *  0      start - 500ms        start           index             2000ms
         */

        /* one of the rare cases where we're lucky */
        *buffer       = ring_buffer + pre_wake_word_start_index;
        to_shift_size = 0;
    }
    else if (ring_buffer_index <= pre_wake_word_start_index && pre_wake_word_start_index < wake_word_start_index)
    {
        /*
         *        A               B               C                D
         *  |------------|----------------|---------------|-----------------|
         *  0          index        start - 500ms        start            2000ms
         */

        uint32_t seg_A_size = ring_buffer_index;
        uint32_t seg_C_size = PRE_UTTERANCE_SIZE;
        uint32_t seg_D_size = RING_BUFFER_SIZE - wake_word_start_index;

        /* determine if it's more efficient to left shift or right shift */
        if (seg_A_size < seg_C_size + seg_D_size)
        {
            /* left shift */
            to_shift_size   = seg_A_size;
            shift_direction = SHIFT_LEFT;
            to_shift_index  = ring_buffer_index;
            *buffer         = ring_buffer + pre_wake_word_start_index - seg_A_size;

            /* index after shifting*/
            ring_buffer_index = RING_BUFFER_SIZE;
        }
        else
        {
            /* right shift */
            to_shift_size   = seg_C_size + seg_D_size;
            shift_direction = SHIFT_RIGHT;
            to_shift_index  = pre_wake_word_start_index;
            *buffer         = ring_buffer;

            /* index will move right */
            ring_buffer_index += to_shift_size;
        }
    }
    else if (wake_word_start_index < ring_buffer_index && ring_buffer_index <= pre_wake_word_start_index)
    {
        /*
         *        A               B               C                D
         *  |------------|----------------|---------------|-----------------|
         *  0          start            index       start - 500ms         2000ms
         */

        uint32_t seg_D_size = RING_BUFFER_SIZE - pre_wake_word_start_index;

        /* never optimal to shift left, seg_D will never be bigger than 500ms */
        to_shift_size   = seg_D_size;
        shift_direction = SHIFT_RIGHT;
        to_shift_index  = pre_wake_word_start_index;
        *buffer         = ring_buffer;

        /* index will move right */
        ring_buffer_index += to_shift_size;
    }

    if (shift_direction == SHIFT_RIGHT)
    {
        buffer_shift_right(to_shift_size, to_shift_index);
    }
    else
    {
        buffer_shift_left(to_shift_size, to_shift_index);
    }

    *buffer_len = ais_buffer_size;

    return 0;
}

static void buffer_shift_left(uint32_t shift_size, uint32_t shift_index)
{
    while (shift_size)
    {
        uint32_t chunk_size =
            shift_size > sizeof(aux_backup_buffer) ? shift_size % sizeof(aux_backup_buffer) : shift_size;

        /* backup */
        memcpy(aux_backup_buffer, ring_buffer, chunk_size);

        /* shift left using memcpy - should be ok, although overlapping
         * buffers (destination lower than source) */
        memcpy(ring_buffer, ring_buffer + chunk_size, RING_BUFFER_SIZE - chunk_size);

        /* restore */
        memcpy(ring_buffer + RING_BUFFER_SIZE - chunk_size, aux_backup_buffer, chunk_size);

        shift_index -= chunk_size;
        shift_size -= chunk_size;
    }
}

static void buffer_shift_right(uint32_t shift_size, uint32_t shift_index)
{
    while (shift_size)
    {
        uint32_t chunk_size =
            shift_size > sizeof(aux_backup_buffer) ? shift_size % sizeof(aux_backup_buffer) : shift_size;

        /* backup */
        memcpy(aux_backup_buffer, ring_buffer + RING_BUFFER_SIZE - chunk_size, chunk_size);

        /* shift right using memmove -  overlapping
         * buffers (destination greater than source) */
        memmove(ring_buffer + chunk_size, ring_buffer, RING_BUFFER_SIZE - chunk_size);

        /* restore */
        memcpy(ring_buffer, aux_backup_buffer, chunk_size);

        shift_index = shift_index + chunk_size;
        shift_size -= chunk_size;
    }
}

uint8_t *get_ring_buffer(void)
{
    return ring_buffer;
}

uint32_t get_ring_buffer_index(void)
{
    return ring_buffer_index;
}

void reset_ring_buffer(void)
{
    ring_buffer_index = 0;
    memset(ring_buffer, 0, RING_BUFFER_SIZE);
}
