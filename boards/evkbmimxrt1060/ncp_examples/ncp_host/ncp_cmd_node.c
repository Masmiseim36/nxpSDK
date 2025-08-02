/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include "ncp_host_command.h"
#include "ncp_cmd_node.h"
#include "ncp_adapter.h"
#include "ncp_cmd_common.h"
#include "fsl_os_abstraction.h"

ncp_cmd_node_list_t g_cmd_node_list;
OSA_MUTEX_HANDLE_DEFINE(cmd_node_list_lock);
OSA_MUTEX_HANDLE_DEFINE(cmd_node_seqno_lock);
uint16_t g_cmd_node_seqno = 0;
uint32_t last_cmd_node_sent,last_cmd_node_rcvd;
uint16_t last_cmd_node_seqno_rcvd, last_cmd_node_seqno_sent;

int mcu_get_cmd_node_list_lock()
{
    return OSA_MutexLock((osa_mutex_handle_t)cmd_node_list_lock, osaWaitForever_c);
}

int mcu_put_cmd_node_list_lock()
{
    return OSA_MutexUnlock(cmd_node_list_lock);
}

int mcu_get_cmd_node_seqno_lock()
{
    return OSA_MutexLock((osa_mutex_handle_t)cmd_node_seqno_lock, osaWaitForever_c);
}

int mcu_put_cmd_node_seqno_lock()
{
    return OSA_MutexUnlock(cmd_node_seqno_lock);
}

static void add_cmd_node(ncp_cmd_node_t * cmd_node)
{
    if(cmd_node == NULL)
    {
        PRINTF("cmd_node is NULL, can't be added to the cmd node list!\r\n");
        return;
    }
    
    mcu_get_cmd_node_list_lock();

    /** Insert at the head of the list */    
    cmd_node->next = g_cmd_node_list.head;
    g_cmd_node_list.head  = cmd_node;

    mcu_put_cmd_node_list_lock();
}

static ncp_cmd_node_t * find_cmd_node(ncp_cmd_node_t * target)
{
    if(target == NULL)
    {
        return NULL;
    }

    mcu_get_cmd_node_list_lock();

    ncp_cmd_node_t * current = g_cmd_node_list.head;
    while(current != NULL)
    {
        if(current == target)
        {
            mcu_put_cmd_node_list_lock();
            return current;
        }
        current = current->next;
    }
    
    mcu_put_cmd_node_list_lock();
    return NULL;
}

static ncp_cmd_node_t * match_cmd_node(uint32_t target_cmd_id, uint16_t target_cmd_seqnum)
{
    mcu_get_cmd_node_list_lock();

    ncp_cmd_node_t * current  = g_cmd_node_list.head;
    ncp_cmd_node_t * previous = NULL;

    while (current != NULL)
    {
        if ((GET_CMD_ID(target_cmd_id) == GET_CMD_ID(current->ncp_cmd_id)) && (target_cmd_seqnum == current->seqnum))
        {
            if (previous == NULL)
            {
                g_cmd_node_list.head = current->next;
            }
            else
            {
                previous->next = current->next;
            }
            mcu_put_cmd_node_list_lock();
            return current;
        }

        previous = current;
        current  = current->next;
    }
    
    mcu_put_cmd_node_list_lock();

    return NULL;
}

uint8_t ncp_tlv_send_wait_resp(void * cmd, void * cmd_resp_buf, Handle_respFunc cb)
{
    NCP_COMMAND * ncp_cmd = (NCP_COMMAND *) cmd;
    uint8_t ret = NCP_STATUS_SUCCESS;

    ncp_cmd_node_t * cmd_node = OSA_MemoryAllocate(sizeof(ncp_cmd_node_t));
    if(cmd_node == NULL)
    {
        PRINTF("failed to malloc cmd_node!\r\n");
        return NCP_STATUS_ERROR;
    }
    (void) memset((uint8_t *) cmd_node, 0, sizeof(ncp_cmd_node_t));

    cmd_node->resp_buf        = cmd_resp_buf;
    cmd_node->ncp_cmd_id      = ncp_cmd->cmd;
    cmd_node->send_tlv_buf    = &ncp_cmd;
    cmd_node->handle_resp_cb  = cb;
    cmd_node->next            = NULL;

    /* set cmd seqno */
    ncp_cmd->seqnum  = g_cmd_node_seqno;
    cmd_node->seqnum = g_cmd_node_seqno;

    OSA_SemaphoreCreateBinary((osa_semaphore_handle_t)(cmd_node->sem));

    if (cmd_node->sem == NULL) {
        PRINTF("Failed to create semaphore.\n");
        goto out_clear;
    }

    add_cmd_node(cmd_node);

    if (ncp_tlv_send(cmd, ncp_cmd->size) != NCP_STATUS_SUCCESS)
    {
        PRINTF("ncp_tlv_send failed!\r\n");
        ret = NCP_STATUS_ERROR;
        goto out_clear;
    }

    mcu_get_cmd_node_seqno_lock();

    g_cmd_node_seqno++;
    mcu_put_cmd_node_seqno_lock();
    OSA_SemaphoreWait(cmd_node->sem, portMAX_DELAY);

out_clear:
    OSA_SemaphoreDestroy(cmd_node->sem);
    if(find_cmd_node(cmd_node) != NULL)
    {
        match_cmd_node(cmd_node->ncp_cmd_id, cmd_node->seqnum);
    }
    OSA_MemoryFree(cmd_node);
    return ret;
}

void ncp_cmd_node_wakeup_pending_tasks(void *res)
{
    NCP_COMMAND *cmd_res = (NCP_COMMAND *)res;
    ncp_cmd_node_t * cmd_node  = NULL;

    cmd_node = match_cmd_node(cmd_res->cmd, cmd_res->seqnum);

    if (cmd_node != NULL)
    {
        cmd_node->handle_resp_cb(cmd_res, cmd_node);
        OSA_SemaphorePost(cmd_node->sem);
    }
}

int ncp_cmd_node_list_init()
{
    int ret = 0;
    g_cmd_node_list.head = NULL;
    g_cmd_node_list.tail = NULL;
    g_cmd_node_list.size = 0;
    ret = OSA_MutexCreate((osa_mutex_handle_t)cmd_node_list_lock);
    ret = OSA_MutexCreate((osa_mutex_handle_t)cmd_node_seqno_lock);

    return ret;
}

void ncp_cmd_node_list_deinit()
{
    mcu_get_cmd_node_list_lock();

    ncp_cmd_node_t * current  = g_cmd_node_list.head;
    ncp_cmd_node_t * cmd_node_for_free = NULL;

    while(current != NULL)
    {
        cmd_node_for_free = current;
        current = current->next;
        if(cmd_node_for_free->resp_buf != NULL)
        {
            OSA_MemoryFree(cmd_node_for_free->resp_buf);
        }
        OSA_SemaphoreDestroy(cmd_node_for_free->sem);
        OSA_MemoryFree(cmd_node_for_free);
    }

    mcu_put_cmd_node_list_lock();
    OSA_MutexDestroy(cmd_node_list_lock);
    OSA_MutexDestroy(cmd_node_seqno_lock);
}