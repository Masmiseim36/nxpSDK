
/**
 *  \file ethal_debug.c
 *
 *  This file contains source codes for the EtherMind Debug Library
 *  Implementation for Windows (User-mode).
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------------- Header File Inclusion */
#include "ethal_debug_internal.h"

/* --------------------------------------------------- Global Definitions */

/* ------------------------------------------------- Static Global Variables */
#ifdef EM_ENABLE_DISABLE_RUNTIME_DEBUG
/* Runtime debug level */
DECL_STATIC UCHAR em_runtime_debug_level;

/* Module Specific debug enabled/disabled flag */
DECL_STATIC UINT32 em_runtime_debug_flag[EM_MAX_MODULE_PAGE_COUNT];
#endif /* EM_ENABLE_DISABLE_RUNTIME_DEBUG */

/* Hack. TBD */
#define EM_MODULE_ID_DEBUG                  0U
/* ------------------------------------------------- Functions */

void EM_debug_init (void)
{
#ifdef EM_ENABLE_DISABLE_RUNTIME_DEBUG
    UINT32 enable_all_bit_mask, page_index;

    /* Initialize runtime debug level */
    em_runtime_debug_level = EM_DEBUG_LEVEL_ALL;

#ifdef EM_DISABLE_ALL_DEBUG
    (void)em_runtime_debug_level;
#endif /* EM_DISABLE_ALL_DEBUG */    

#ifndef EM_DISABLE_DEBUG_LOG_ON_STARTUP
    /* Enable all module debug log - by default */
    enable_all_bit_mask = 0xFFFFFFFFU;
#else
    enable_all_bit_mask = 0x00000000U;
#endif /* EM_DISABLE_DEBUG_LOG_ON_STARTUP */

    for (page_index = 0U; page_index < EM_MAX_MODULE_PAGE_COUNT; page_index++)
    {
        em_runtime_debug_flag[page_index] = enable_all_bit_mask;
    }
#endif /* EM_ENABLE_DISABLE_RUNTIME_DEBUG */

    return;
}

void EM_debug_shutdown (void)
{
    return;
}

#ifndef EM_DISABLE_ALL_DEBUG
INT32 EM_debug_printf(UCHAR msg_type, UINT32 module_id, const CHAR *fmt, ...)
{
    va_list parg;

#ifdef EM_ENABLE_DISABLE_RUNTIME_DEBUG
    UINT32 page_index;
    UINT32 module_bit_mask;

    /* Check Message Level */
    if (msg_type > em_runtime_debug_level)
    {
        return -1;
    }

    /* Extract Page Index and Module Bit Mask */
    EM_GET_PAGE_IDX_MODULE_BIT_MASK(module_id, page_index, module_bit_mask);

    /* Check if the module debug log is enabled */
    if ((em_runtime_debug_flag[page_index] & module_bit_mask) != module_bit_mask)
    {
        return -1;
    }
#endif /* EM_ENABLE_DISABLE_RUNTIME_DEBUG */

    va_start (parg, fmt);
    (void) DbgConsole_Printf (fmt, parg);
    va_end (parg);

    return 0U;
}

INT32 EM_debug_dump_bytes(UINT32 module_id, UCHAR *buffer, UINT32 length)
{
    char hex_stream[49U];
    char char_stream[17U];
    UINT32 i;
    UINT16 offset, count;
    UCHAR c;

    EM_debug_dump(module_id, "-- Dumping %d Bytes --\n",
    (int)length);

    EM_debug_dump(module_id,
    "-------------------------------------------------------------------\n");

    count = 0U;
    offset = 0U;
    for(i = 0U; i < length; i ++)
    {
        c =  buffer[i];
        (void) sprintf(&hex_stream[offset], "%02X ", c);

        if ( (c >= 0x20U) && (c <= 0x7EU) )
        {
            char_stream[count] = c;
        }
        else
        {
            char_stream[count] = '.';
        }

        count ++;
        offset += 3U;

        if(16U == count)
        {
            char_stream[count] = '\0';
            count = 0U;
            offset = 0U;

            EM_debug_dump(module_id, "%s   %s\n",
            hex_stream, char_stream);

            EM_mem_set(hex_stream, 0U, 49U);
            EM_mem_set(char_stream, 0U, 17U);
        }
    }

    if(0U != offset)
    {
        char_stream[count] = '\0';

        /* Maintain the alignment */
        EM_debug_dump(module_id, "%-48s   %s\n",
        hex_stream, char_stream);
    }

    EM_debug_dump(module_id,
    "-------------------------------------------------------------------\n");

    return 0U;
}


INT32 EM_debug_dump_decimal(UINT32 module_id, UCHAR *buffer, UINT32 length)
{
    char stream[100U];
    UINT32 i;
    UINT16 offset, count;

    EM_debug_dump(module_id, "Dumping %d Bytes (In Decimal): ------>\n",
    (int)length);

    count = 0U;
    offset = 0U;
    for(i = 0U; i < length; i ++)
    {
        (void) sprintf(&stream[offset], "%3d ", (unsigned int)buffer[i]);
        count ++;
        offset += 4U;

        if(16U == count)
        {
            count = 0U;
            offset = 0U;

            EM_debug_dump(module_id, "%s\n", stream);

            EM_mem_set(stream, 0U, 100U);
        }
    }

    if(0U != offset)
    {
        EM_debug_dump(module_id, "%s\n", stream);
    }

    EM_debug_dump(module_id, "<------------------------------------>\n");

    return 0U;
}
#endif /* EM_DISABLE_ALL_DEBUG */

#ifdef EM_LOG_TIMESTAMP
UCHAR * EM_debug_get_current_timestamp (void)
{
    return NULL;
}
#endif /* EM_LOG_TIMESTAMP */


#ifdef EM_ENABLE_DISABLE_RUNTIME_DEBUG
void EM_set_debug_level(UCHAR level)
{
    /* Parameter validation */
    if (EM_DEBUG_LEVEL_ALL < level)
    {
        /* Failure */
        return;
    }

    /* Set runtime debug level */
    em_runtime_debug_level = level;

    return;
}

void EM_update_module_debug_flag(UINT32 module_id, UCHAR flag)
{
    UINT32 page_index;
    UINT32 module_bit_mask;

    /* Parameter Validation */
    if (EM_DEBUG_ENABLE < flag)
    {
        return;
    }

    /* Check if the request is for enable/disable all */
    if (EM_MODULE_ALL == module_id)
    {
        if (EM_DEBUG_ENABLE == flag)
        {
            module_bit_mask = 0xFFFFFFFFU;
        }
        else
        {
            module_bit_mask = 0x00000000U;
        }

        for (page_index = 0U; page_index < EM_MAX_MODULE_PAGE_COUNT; page_index++)
        {
            em_runtime_debug_flag[page_index] = module_bit_mask;
        }

        return;
    }

    /* Extract Page Index and Module Bit Mask */
    EM_GET_PAGE_IDX_MODULE_BIT_MASK(module_id, page_index, module_bit_mask);

    /* Enable/disable based on the flag */
    if (EM_DEBUG_ENABLE == flag)
    {
        em_runtime_debug_flag[page_index] |= (module_bit_mask);
    }
    else
    {
        em_runtime_debug_flag[page_index] &= ~(module_bit_mask);
    }
}
#endif /* EM_ENABLE_DISABLE_RUNTIME_DEBUG */


