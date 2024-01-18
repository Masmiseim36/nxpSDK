/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "fsl_common.h"
#include "fwk_fs_abstraction.h"
#include "fwk_file_cache.h"
#include "fsl_os_abstraction.h"

/**
 * \brief Definitions for flags argument provided to FC_CheckBufferInWriteList()
 *
 * If it is a simple check if the buffer is in the write queue, it won't be removed.
 * If the buffer needs to be opened, it will be removed from the write queue if found.
 */
#define FC_CHECK_BUFFER_ONLY             0
#define FC_REMOVE_BUFFER_FROM_WRITE_LIST 1

/* Fix pattern to eventually recognize the wasted space at the end of the ram section */
#define FC_WASTED_SPACE_PATTERN 0xC1C1

#define DEBUG_FWK_FILECACHE 0

#if defined(DEBUG_FWK_FILECACHE) && (DEBUG_FWK_FILECACHE == 2)
#include "fsl_debug_console.h"
#define DBG_PRINTF  PRINTF
#define INFO_PRINTF PRINTF
#elif defined(DEBUG_FWK_FILECACHE) && (DEBUG_FWK_FILECACHE == 1)
#include "fsl_debug_console.h"
#define DBG_PRINTF PRINTF
#define INFO_PRINTF(...)
#else
#define DBG_PRINTF(...)
#define INFO_PRINTF(...)
#endif

#define FC_PRINT_STATS 0

#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
#include "fsl_debug_console.h"
#endif
/*******************************************************************************
 * Type definition
 ******************************************************************************/
typedef struct
{
    void *list_head;
    void *list_tail;
} fc_list_t;

typedef struct fc_buffer_desc_s
{
    uint16_t payload_size;
    uint16_t buffer_size; /* Differentiate buffer size from payload size when open a buffer in write queue */
    struct fc_buffer_desc_s *next;
    struct fc_context_s *    fc_context;
    char                     file_name[FC_FILE_NAME_SIZE_MAX];
} fc_buffer_desc_t;

typedef struct fc_context_s
{
    const fc_config_t *fc_config;
    void *             file_cache_section; // avoid use of mutex
    /* 2 offsets to manage the ram section as a circular buffer */
    uint16_t free_buffer_offset;
    uint16_t used_buffer_offset;
    uint16_t remaining_ram_size; /* Total scratch ram size - fc context */
    uint8_t *buf_start_addr;     /* Address in the ram section from which it is possible to allocate buffers */
} fc_context_t;

typedef struct
{
    fc_list_t fc_write_buffer_list;
#if defined(FC_EnableTaskDelayKeepSchedulerEnabled_d) && (FC_EnableTaskDelayKeepSchedulerEnabled_d == 1)
    fc_buffer_desc_t *being_written_file_desc;
#endif
} fc_config_top_t;

/************************************************************************************
 * Private memory declarations
 ************************************************************************************/

static fc_config_top_t fc_config_top = {
#if defined(FC_EnableTaskDelayKeepSchedulerEnabled_d) && (FC_EnableTaskDelayKeepSchedulerEnabled_d == 1)
    .being_written_file_desc = NULL,
#endif
    .fc_write_buffer_list.list_head = NULL,
    .fc_write_buffer_list.list_tail = NULL,
};
static fc_config_top_t *fc_config_top_p = &fc_config_top;

// other option is to use a mutex here
#define FC_CRITICAL_SECTION_ENTER() \
    ;                               \
    uint32_t __primask = DisableGlobalIRQ();

#define FC_CRITICAL_SECTION_EXIT() \
    ;                              \
    EnableGlobalIRQ(__primask)

#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
static uint16_t buf_in_ram_section = 0;
#endif

/************************************************************************************
 * Private functions definitions
 ************************************************************************************/

#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
static char *FC_GetConfigName(fc_context_t *fc_context_p)
{
    fc_config_t *fc_config_l;
    char *       fc_config_name;

    fc_config_l    = (fc_config_t *)fc_context_p->fc_config;
    fc_config_name = (char *)fc_config_l->folder_name;

    return fc_config_name;
}
#endif

/**
 * \brief Check if there is enough free space in the ram section to allocate a new buffer
 *
 * First, the size required to allocate the buffer and its descriptor is calculated.
 * Then, it is checked if this size is available in the ram section, after the used offset, if this is after the free
 * offset, or between the used offset and the free offset, if the used offset is before the free one.
 * If there isn't enough space after the used offset at the end of the ram section, but there is at the beginning,
 * before the free offset, the used offset is reset to 0. This means that there will be some wasted space at the end of
 * the ram section.
 * Return 1 if there is enough space for the allocation, 0 otherwise.
 *
 * \param fc_context_p [in] context handle returned by FC_InitConfig()
 * \param buf_size [in] size of the buffer that should be allocated
 * \return int : 1 if the buffer can be allocated, 0 if there is not enough space to allocate the buffer
 */
static int FC_CheckRamSectionSpace(fc_context_t *fc_context_p, uint16_t buf_size)
{
    int               ret;
    uint16_t          available_ram_size;
    uint16_t          req_space;
    uint16_t          free_off;
    uint16_t          used_off;
    fc_buffer_desc_t *buf_desc_p;

    assert(fc_context_p != NULL);

    /* Just to have a more leggible code */
    free_off = fc_context_p->free_buffer_offset;
    used_off = fc_context_p->used_buffer_offset;

    ret                = 0;
    available_ram_size = 0;
    /* The space needed to allocate a new buffer includes the size of the descriptor and the effective size of the
     * buffer */
    req_space = sizeof(fc_buffer_desc_t) + buf_size;

    /* If both the offset are set to 0, we are sure that the whole ram section is free */
    if ((used_off > free_off) || ((used_off == free_off) && (used_off == 0)))
    {
        /* Available ram size after the used offset */
        available_ram_size = fc_context_p->remaining_ram_size - used_off;
        /* Keep free the last 4 bytes of the ram section to allocate in the worst case at least the flag that signals
         * the presence of wasted space */
        if (req_space <= (available_ram_size - 4))
        {
            ret = 1;
        }
        else
        {
            /* Check if there is enough space before the free offset */
            if (req_space <= free_off)
            {
                ret = 1;
                /* Some wasted space at the end of the buffer */
                buf_desc_p                       = (fc_buffer_desc_t *)(fc_context_p->buf_start_addr + used_off);
                buf_desc_p->buffer_size          = FC_WASTED_SPACE_PATTERN;
                buf_desc_p->payload_size         = FC_WASTED_SPACE_PATTERN;
                fc_context_p->used_buffer_offset = 0;
            }
        }
        available_ram_size += free_off;
    }
    else if (used_off < free_off)
    {
        /* Available ram size between used offset and free offset */
        available_ram_size = free_off - used_off;
        /* Check if there is enough free space to get a new buffer */
        if (req_space <= available_ram_size)
        {
            ret = 1;
        }
    }
#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
    char *config_name = FC_GetConfigName(fc_context_p);
    PRINTF("Configuration name: %s. Ram memory used (counting also size of context and filecache): %u\r\n", config_name,
           fc_context_p->remaining_ram_size - available_ram_size + sizeof(fc_context_t) + FC_FILE_CACHE_SIZE);
#endif
    return ret;
}

static fc_buffer_desc_t *FC_GetFreeBuffer(fc_context_t *fc_context_p, const char *buffer_name, uint16_t buf_size)
{
    fc_buffer_desc_t *buf_desc_p = NULL;
    uint8_t           space_ok;

    assert(fc_context_p != NULL);

    /* Flag that indicates if there is enough space for the buffer allocation */
    space_ok = FC_CheckRamSectionSpace(fc_context_p, buf_size);

    /* Check if there is enough space in RAM section to allocate the buffer */
    if (space_ok == 1)
    {
        /* There is enough space after the used offset */
        buf_desc_p = (fc_buffer_desc_t *)(fc_context_p->buf_start_addr + fc_context_p->used_buffer_offset);
        /* Update the offset of the used space inside the ram section */
        fc_context_p->used_buffer_offset += sizeof(fc_buffer_desc_t) + buf_size;
        buf_desc_p->fc_context  = fc_context_p;
        buf_desc_p->next        = NULL;
        buf_desc_p->buffer_size = buf_size;

        /* Only used to indicate the size of the buffer when it is added in the write list, always 0 otherwise */
        buf_desc_p->payload_size = 0;

#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
        char *config_name = FC_GetConfigName(fc_context_p);
        buf_in_ram_section++;
        PRINTF("Configuration name: %s. Number of buffers allocated in the ram section: %u\r\n", config_name,
               buf_in_ram_section);
#endif
    }

    return buf_desc_p;
}

static void FC_AddFreeBuffer(fc_buffer_desc_t *buf_desc_p)
{
    fc_context_t *    fc_context_p;
    uint16_t          available_ram_size;
    fc_buffer_desc_t *buf_pointed_by_free;

    assert(buf_desc_p != NULL);
    /* buffer shall not be in any list */
    assert(buf_desc_p->next == NULL);

    fc_context_p = buf_desc_p->fc_context;
    assert(fc_context_p != NULL);

    available_ram_size = fc_context_p->remaining_ram_size - fc_context_p->free_buffer_offset;
    /* This happens when there is some wasted space at the end of the buffer */
    if (available_ram_size < (buf_desc_p->buffer_size + sizeof(fc_buffer_desc_t)))
    {
        fc_context_p->free_buffer_offset = 0;
    }

    /* The free offset could point to a buffer located before the one to be freed within the ram section */
    if (buf_desc_p != (fc_buffer_desc_t *)(fc_context_p->buf_start_addr + fc_context_p->free_buffer_offset))
    {
        while (buf_desc_p != (fc_buffer_desc_t *)(fc_context_p->buf_start_addr + fc_context_p->free_buffer_offset))
        {
            /* Wait until the right buffer is pointed by the free offset */
            OSA_TimeDelay(FC_WAIT_LOOP_MS);
        }
    }

    FC_CRITICAL_SECTION_ENTER();
    assert(buf_desc_p == (fc_buffer_desc_t *)(fc_context_p->buf_start_addr + fc_context_p->free_buffer_offset));
    fc_context_p->free_buffer_offset += (buf_desc_p->buffer_size + sizeof(fc_buffer_desc_t));

    /* If after the free, the free offset points to wasted space, reset it */
    buf_pointed_by_free = (fc_buffer_desc_t *)(fc_context_p->buf_start_addr + fc_context_p->free_buffer_offset);
    if ((buf_pointed_by_free->payload_size == FC_WASTED_SPACE_PATTERN) &&
        (buf_pointed_by_free->buffer_size == FC_WASTED_SPACE_PATTERN))
    {
        fc_context_p->free_buffer_offset = 0;
    }

    /* If the free offset reached the used offset, it means that all the buffers used were freed
       We can start again from the beginning of the scratch ram section */
    if (fc_context_p->used_buffer_offset == fc_context_p->free_buffer_offset)
    {
        fc_context_p->used_buffer_offset = 0;
        fc_context_p->free_buffer_offset = 0;
    }
#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
    buf_in_ram_section--;
#endif
    FC_CRITICAL_SECTION_EXIT();

    return;
}

static void FC_AddToWriteList(fc_buffer_desc_t *buf_desc_p, uint16_t payload_size)
{
    buf_desc_p->payload_size = payload_size;

    FC_CRITICAL_SECTION_ENTER();
    if (fc_config_top_p->fc_write_buffer_list.list_tail == NULL)
    {
        fc_config_top_p->fc_write_buffer_list.list_tail = buf_desc_p;
        fc_config_top_p->fc_write_buffer_list.list_head = buf_desc_p;
    }
    else
    {
        fc_buffer_desc_t *buf_desc_tail = (fc_buffer_desc_t *)fc_config_top_p->fc_write_buffer_list.list_tail;
        buf_desc_tail->next             = buf_desc_p;
        fc_config_top_p->fc_write_buffer_list.list_tail = buf_desc_p;
    }
#if defined(FC_PRINT_STATS) && (FC_PRINT_STATS == 1)
    PRINTF("%s added to write list. Buf size: %d\r\n", buf_desc_p->file_name, buf_desc_p->buffer_size);
#endif
    buf_desc_p->next = NULL;
    FC_CRITICAL_SECTION_EXIT();
}

static fc_buffer_desc_t *FC_GetFromWriteList(void)
{
    fc_buffer_desc_t *buf_desc_p = NULL;

    FC_CRITICAL_SECTION_ENTER();
    if (fc_config_top_p->fc_write_buffer_list.list_head != NULL)
    {
        assert(fc_config_top_p->fc_write_buffer_list.list_head != NULL);

        buf_desc_p = fc_config_top_p->fc_write_buffer_list.list_head;
        if (fc_config_top_p->fc_write_buffer_list.list_head == fc_config_top_p->fc_write_buffer_list.list_tail)
        {
            /* only one element in the queue */
            fc_config_top_p->fc_write_buffer_list.list_head = NULL;
            fc_config_top_p->fc_write_buffer_list.list_tail = NULL;
        }
        else
        {
            fc_config_top_p->fc_write_buffer_list.list_head = buf_desc_p->next;
        }
        buf_desc_p->next = NULL;
    }
    else
    {
        /* no element in the queue */
    }
    FC_CRITICAL_SECTION_EXIT();

    return buf_desc_p;
}

static void FC_WriteImmediatly(fc_buffer_desc_t *buf_desc_p, uint16_t payload_size)
{
    char *            file_name;
    uint8_t *         buffer_addr;
    fc_buffer_desc_t *tmp;

    file_name   = (char *)buf_desc_p->file_name;
    tmp         = buf_desc_p;
    buffer_addr = (uint8_t *)(++tmp);

    FSA_WriteBufferToFile((const char *)file_name, buffer_addr, payload_size);
    FC_AddFreeBuffer(buf_desc_p);
}

/**
 * FC_RemoveFromWriteQueue() shall be called only if we are sure the buffer desc is in the write queue
 */
static void FC_RemoveFromWriteQueue(fc_buffer_desc_t *buf_desc_p)
{
    fc_buffer_desc_t *buf_desc_l;

    /* This is a single chain list, so need to go through the queue again
        Hopefully, this code is not executed very often */

    FC_CRITICAL_SECTION_ENTER();
    buf_desc_l = fc_config_top_p->fc_write_buffer_list.list_head;

    if (buf_desc_p == fc_config_top_p->fc_write_buffer_list.list_head)
    {
        /* first one in the queue , new head pointing to next */
        if (buf_desc_p == fc_config_top_p->fc_write_buffer_list.list_tail)
        {
            /* buffer was only one in the queue */
            fc_config_top_p->fc_write_buffer_list.list_head = NULL;
            fc_config_top_p->fc_write_buffer_list.list_tail = NULL;
        }
        else
        {
            /* new head pointing to next */
            fc_config_top_p->fc_write_buffer_list.list_head = buf_desc_p->next;
        }
    }
    else
    {
        while ((buf_desc_l != NULL) && (buf_desc_p != buf_desc_l->next))
        {
            buf_desc_l = buf_desc_l->next;
        }

        /* should not happen , buf_desc_p shall be in the queue*/
        assert(buf_desc_l != NULL);

        if (fc_config_top_p->fc_write_buffer_list.list_tail == buf_desc_p)
        {
            fc_config_top_p->fc_write_buffer_list.list_tail = buf_desc_l;
        }
        buf_desc_l->next = buf_desc_p->next;
    }
    buf_desc_p->next = NULL;

    FC_CRITICAL_SECTION_EXIT();

    return;
}

static bool FC_IsSameBufName(const char *buffer_name1, const char *buffer_name2)
{
    int  i         = 0;
    bool same_name = true;

    while ((buffer_name1[i] != 0x0) || (buffer_name2[i] != 0x0))
    {
        if (buffer_name1[i] != buffer_name2[i])
        {
            same_name = false;
            break;
        }
        i++;
    }
    return same_name;
}

static fc_buffer_desc_t *FC_CheckBufferInWriteList(const char *buffer_name,
                                                   uint16_t *  buffer_length,
                                                   uint8_t     buffer_flag)
{
    fc_buffer_desc_t *buf_desc_p;

    /* The following operations should not be interrupted, because otherwise the idle task could run the
     * FC_Process() and the buffer found would not be in the write list anymore once come back to the interrupted
     * process */
    FC_CRITICAL_SECTION_ENTER();
    buf_desc_p = fc_config_top_p->fc_write_buffer_list.list_head;

    while ((buf_desc_p != NULL) && (FC_IsSameBufName(buffer_name, buf_desc_p->file_name) == false))
    {
        /* go to the next one */
        buf_desc_p = buf_desc_p->next;
    }

    if (buf_desc_p != NULL)
    {
        /* buffer found but too small */
        if (*buffer_length > buf_desc_p->buffer_size)
        {
            fc_buffer_desc_t *buf_desc_l;

            /* Try to reallocate a new bigger buffer */
            buf_desc_l = FC_GetFreeBuffer(buf_desc_p->fc_context, buffer_name, *buffer_length);
            if (buf_desc_l != NULL)
            {
                /* The allocation of the bigger buffer succeeded */
                /* Remove the old small buffer from the write list */
                FC_RemoveFromWriteQueue(buf_desc_p);
                FC_CRITICAL_SECTION_EXIT();

                /* Copy the content from the old buffer */
                memcpy(buf_desc_l, buf_desc_p, (sizeof(fc_buffer_desc_t) + buf_desc_p->payload_size));
                /* Free the old buffer */
                FC_AddFreeBuffer(buf_desc_p);

                /* Return the new allocated buffer */
                buf_desc_p              = buf_desc_l;
                buf_desc_p->buffer_size = *buffer_length;
            }
            else
            {
                FC_CRITICAL_SECTION_EXIT();

                /* The allocation of the bigger buffer failed */
                /* Return NULL and report the available size */
                *buffer_length = buf_desc_p->buffer_size;
                buf_desc_p     = NULL;
            }
        }
        else
        {
            /* requested Size is ok */
            /* The flag buffer_flag indicates if the caller wants only to check if the buffer is in the write
             * list or if it wants to open it: in this last case the buffer should be removed from the write list to be
             * used */
            if (buffer_flag == FC_REMOVE_BUFFER_FROM_WRITE_LIST)
            {
                /* remove from the write queue */
                FC_RemoveFromWriteQueue(buf_desc_p);
            }
            FC_CRITICAL_SECTION_EXIT();

            /* Return the payload size (as it should be after a read) */
            *buffer_length = buf_desc_p->payload_size;
        }
    }
#if defined(FC_EnableTaskDelayKeepSchedulerEnabled_d) && (FC_EnableTaskDelayKeepSchedulerEnabled_d == 1)
    /* Scheduler was not disabled during the write. This last was interruted and Fopen() was called for this buffer.
        We need to check if the requested buffer is currently being written and if yes, wait for write being completed*/
    else if (fc_config_top_p->being_written_file_desc != NULL) // else of 'if (buf_desc_p != NULL)'
    {
        fc_buffer_desc_t *being_written = fc_config_top_p->being_written_file_desc;

        FC_CRITICAL_SECTION_EXIT();

        /* check if buffer being written into flash from FC_Process() */
        if (FC_IsSameBufName(buffer_name, being_written->file_name) == true)
        {
            /* Wait for 'being written desc' to change to NULL or an other buffer
                This is appropriate for an RTOS. in Bare metal, we are not supposed to execute this peace of code */
            do
            {
                OSA_TimeDelay(FC_WAIT_LOOP_MS);
            } while (being_written == fc_config_top_p->being_written_file_desc);
        }
    }
    else
    {
        FC_CRITICAL_SECTION_EXIT();
    }
#else
    FC_CRITICAL_SECTION_EXIT();
#endif
    return buf_desc_p;
}

/************************************************************************************
 * Public functions
 ************************************************************************************/

void *FC_InitConfig(const fc_config_t *fc_config_p)
{
    void *ret = NULL;

    assert((sizeof(fc_context_t) + FC_FILE_CACHE_SIZE) == FC_CONTEXT_SIZE);
    assert(sizeof(fc_buffer_desc_t) == FC_FILE_DESC_SIZE);

    if (FSA_Init() >= 0)
    {
        fc_context_t *fc_context_p;
        uint16_t      remaining_scratch_ram_size;
        uint8_t *     fc_inc_addr;

        /* Init local variables */
        fc_context_p               = (fc_context_t *)(fc_config_p->scracth_ram_p);
        remaining_scratch_ram_size = fc_config_p->scracth_ram_sz;
        fc_inc_addr                = (uint8_t *)fc_context_p + sizeof(fc_context_t);

        /* Start filling fc_context */
        fc_context_p->fc_config          = fc_config_p;
        fc_context_p->file_cache_section = fc_inc_addr;

        fc_inc_addr += FC_FILE_CACHE_SIZE;
        remaining_scratch_ram_size -= (sizeof(fc_context_t) + FC_FILE_CACHE_SIZE);
        fc_context_p->remaining_ram_size = remaining_scratch_ram_size;
        fc_context_p->free_buffer_offset = 0;
        fc_context_p->used_buffer_offset = 0;
        fc_context_p->buf_start_addr     = fc_inc_addr;

        ret = (void *)fc_context_p;
    }

    return ret;
}

int FC_DeInit(void *fc_context_p)
{
    int               ret;
    int               i;
    fc_buffer_desc_t *buf_desc_p;
    fc_buffer_desc_t *buf_desc_l;
    fc_context_t *    fc_ctx_p;
    uint16_t          free_off;
    uint16_t          used_off;
    uint16_t          req_space;
    uint16_t          buf_len;

    assert(fc_context_p != NULL);

    ret = 0;
    i   = 0;
    /* We want only to check if the buffer is in the write list */
    buf_len  = 0;
    fc_ctx_p = (fc_context_t *)fc_context_p;
    free_off = fc_ctx_p->free_buffer_offset;
    used_off = fc_ctx_p->used_buffer_offset;

    /* Check if the ram section is already free or needs to be freed */
    if (free_off != used_off)
    {
        buf_desc_p = (fc_buffer_desc_t *)(fc_ctx_p->buf_start_addr + free_off);
        do
        {
            /* Check if we are pointing to the wasted space at the end of the buffer */
            if ((buf_desc_p->buffer_size == FC_WASTED_SPACE_PATTERN) &&
                (buf_desc_p->payload_size == FC_WASTED_SPACE_PATTERN))
            {
                /* Restart from the beginning of the section */
                buf_desc_p = (fc_buffer_desc_t *)fc_ctx_p->buf_start_addr;
            }
            req_space = buf_desc_p->buffer_size + sizeof(fc_buffer_desc_t);

#if (FC_DEINIT_WAIT_LOOP_MAX_ITERATIONS_N != 0)
            /* Check if the buffer is in the write list */
            do
            {
                buf_len = 0;
                /* Wait loop until the file is written in flash or the maximum number of iteration is reached */
                /* The flag removeFromWriteQueue is set to 0 because we want only to check if the buffer is in the write
                 * list and not remove it if it is found */
                buf_desc_l = FC_CheckBufferInWriteList(buf_desc_p->file_name, &buf_len, FC_CHECK_BUFFER_ONLY);
                OSA_TimeDelay(FC_WAIT_LOOP_MS);
                i++;
            } while (buf_desc_l != NULL && i < FC_DEINIT_WAIT_LOOP_MAX_ITERATIONS_N);
#else
            buf_desc_l = FC_CheckBufferInWriteList(buf_desc_p->file_name, &buf_len, FC_CHECK_BUFFER_ONLY);
#endif

            /* The buffer has not been freed */
            if (buf_desc_l != NULL)
            {
                ret = -1;
                break;
            }
            buf_desc_p = (fc_buffer_desc_t *)((uint8_t *)(buf_desc_p) + req_space);
        } while (buf_desc_p != (fc_buffer_desc_t *)(fc_ctx_p->buf_start_addr + used_off));
    }
    if (ret == 0)
    {
        FSA_DeInit();
    }
    return ret;
}

/* Returns the effective size of the file */
int FC_GetFileSize(const char *buffer_name, uint16_t *payload_size)
{
    int               ret;
    fc_buffer_desc_t *buf_desc_p;
    uint16_t          buffer_length;

    ret           = 0;
    buffer_length = 0;

    /* Check if the buffer is in the write list or in the being written list*/
    /* Passes 0 as buffer_lenght because we want to know only the payload size of the buffer */
    /* The flag removeFromWriteQueue is set to 0 because we want only to check if the buffer is in the write
     * list and not remove it if it is found */
    buf_desc_p = FC_CheckBufferInWriteList(buffer_name, &buffer_length, FC_CHECK_BUFFER_ONLY);

    /* Check if the buffer is in the flash */
    if (buf_desc_p == NULL)
    {
        ret = FSA_CheckFileSize(buffer_name);
        if (ret < 0)
        {
            if (ret == -2)
            {
                /* The file doesn't exist yet */
                *payload_size = 0;
            }
            ret = -1; /* Error or file not found */
        }
        else
        {
            *payload_size = ret;
        }
    }
    else
    {
        *payload_size = buffer_length;
    }

    return ret;
}

void *FC_Open(
    void *fc_context_p, const char *buffer_name, uint8_t **buffer_addr, uint16_t *buffer_length, uint16_t flag)
{
    fc_context_t *    fc_context_l = (fc_context_t *)fc_context_p;
    fc_buffer_desc_t *buf_desc_p;
    uint16_t          requested_buf_length;

    requested_buf_length = *buffer_length;

    assert(strlen(buffer_name) <= FC_FILE_NAME_SIZE_MAX);

    /* on return of the FC_CheckBufferInWriteList() call, *buffer_length will provide the available buf lenght (being
     * smaller than requested_buf_length)*/
    /* The flag removeFromWriteQueue is set to 1 as we want the buffer to be removed from the write list if it is found
     */
    buf_desc_p = FC_CheckBufferInWriteList(buffer_name, buffer_length, FC_REMOVE_BUFFER_FROM_WRITE_LIST);

    if (buf_desc_p == NULL)
    {
        if (*buffer_length < requested_buf_length)
        {
            /* Buffer was found but too small compared to requested size, just return NULL , *buffer_length will provide
             * the available buffer size */
        }
        else
        {
            /* Request a free buffer of size *buffer_length , will return NULL if no buffer available with this size */
            buf_desc_p = FC_GetFreeBuffer(fc_context_l, buffer_name, *buffer_length);

            if (buf_desc_p != NULL)
            {
                int               read_size;
                fc_buffer_desc_t *buf_desc_l;

                /* Get buffer address */
                buf_desc_l   = buf_desc_p;
                *buffer_addr = (uint8_t *)(++buf_desc_l);
                assert(*buffer_addr != NULL);

                read_size = FSA_ReadBufferFromFile(buffer_name, *buffer_addr, *buffer_length);
                if (read_size >= 0)
                {
                    *buffer_length = read_size;
                }

                strncpy(buf_desc_p->file_name, buffer_name, FC_FILE_NAME_SIZE_MAX);
            }
            else
            {
                /* when returning NULL (as allocation has failed), indicate which size is available */
                *buffer_length = 0; // for now, no buffer left , size NULL
            }
        }
    }
    else
    {
        /* Buffer removed from write queue, provide also the buffer address */
        fc_buffer_desc_t *buf_desc_l;

        buf_desc_l   = buf_desc_p;
        *buffer_addr = (uint8_t *)(++buf_desc_l);
        assert(*buffer_addr != NULL);
    }

    return buf_desc_p;
}

int FC_Close(void *fc_hdl, uint16_t payload_size, uint16_t flag)
{
    assert(fc_hdl != NULL);
    fc_buffer_desc_t *buf_desc_p;

    buf_desc_p = (fc_buffer_desc_t *)fc_hdl;
    if (flag == FC_CLOSE_AND_SAVE_IMMEDIATELY)
    {
        /* we assume FC_Open() for this file is not called from an other task than FC_close(), so need to
            store the being_written_file value */
        FC_WriteImmediatly((fc_buffer_desc_t *)fc_hdl, payload_size);
    }
    else if (flag == FC_CLOSE_AND_SAVE_TO_QUEUE)
    {
        FC_AddToWriteList(fc_hdl, payload_size);
    }
    else
    {
        /* was a simple read, release buffer */
        /* If the buffer is in the write list should not be freed */
        if (buf_desc_p->payload_size != 0)
        {
            FC_AddToWriteList(fc_hdl, buf_desc_p->payload_size);
        }
        else
        {
            FC_AddFreeBuffer((fc_buffer_desc_t *)fc_hdl);
        }
    }

    return 0;
}

int FC_Process(void)
{
    fc_buffer_desc_t *buf_desc_p;
    int               has_processed = 0;

    /* The buffer is no longer in the write list  but not yet in being_written_file, so this section needs to be
     * protected */
    FC_CRITICAL_SECTION_ENTER();

    buf_desc_p = FC_GetFromWriteList();

    if (buf_desc_p != NULL)
    {
#if defined(FC_EnableTaskDelayKeepSchedulerEnabled_d) && (FC_EnableTaskDelayKeepSchedulerEnabled_d == 1)
        /* FC_Process() is likely called from an other task than FC_Open
            We need to indicate that the buffer is being written in case of descheduling and
            FC_Open is called from an other trhead
            First, There should not be any file being written at this stage */
        assert(fc_config_top_p->being_written_file_desc == NULL);

        fc_config_top_p->being_written_file_desc = buf_desc_p;
        FC_CRITICAL_SECTION_EXIT();
        FC_WriteImmediatly(buf_desc_p, buf_desc_p->payload_size);

        fc_config_top_p->being_written_file_desc = NULL;
#elif 1
        FC_WriteImmediatly(buf_desc_p, buf_desc_p->payload_size);

        FC_CRITICAL_SECTION_EXIT();
#else
        // TODO : disable scheduler

#endif

        has_processed++;
    }
    else
    {
        FC_CRITICAL_SECTION_EXIT();
    }
    return has_processed;
}

int FC_Delete(const char *buffer_name)
{
    int               ret;
    uint16_t          buffer_length;
    fc_buffer_desc_t *buf_desc_p;

    buffer_length = 0;

    /* If the buffer is in the write list, it will be removed.
       If the buffer is in the being written list, it will be written in flash, so a delete from flash is required after
       that */
    buf_desc_p = FC_CheckBufferInWriteList(buffer_name, &buffer_length, FC_REMOVE_BUFFER_FROM_WRITE_LIST);
    if (buf_desc_p != NULL)
    {
        /* The buffer has been removed from the write list, but the space in RAM section has not been freed yet */
        /* Remove the buffer from the RAM section */
        buf_desc_p->payload_size = 0;
        FC_Close(buf_desc_p, 0, FC_CLOSE_ONLY);
    }

    /* Delete from flash */
    ret = FSA_DeleteFile(buffer_name);
    if (ret < 0)
    {
        ret = -1;
    }
    else
    {
        ret = 0;
    }
    return ret;
}