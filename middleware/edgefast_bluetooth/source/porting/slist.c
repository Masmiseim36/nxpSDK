/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include <sys/slist.h>

void bt_list_init(bt_list_t *list)
{
    list->head = NULL;
}

void bt_list_append(bt_list_t *list, bt_list_node_t *node)
{
    bt_list_node_t *p;
    unsigned int reg;

    reg = DisableGlobalIRQ();
    p = list->head;

    if (NULL != p)
    {
        while (NULL != p->next)
        {
            if (p == node)
            {
                EnableGlobalIRQ(reg);
                return;
            }
            p = p->next;
        }
        p->next = node;
    }
    else
    {
        list->head = node;
    }
    node->next = NULL;
    EnableGlobalIRQ(reg);
}

void bt_list_append_list(bt_list_t *list, bt_list_node_t *head, bt_list_node_t *tail)
{
    bt_list_node_t *p;
    unsigned int reg;

    reg = DisableGlobalIRQ();
    p = list->head;

    if (NULL != p)
    {
        while (NULL != p->next)
        {
            if (p == head)
            {
                EnableGlobalIRQ(reg);
                return;
            }
            p = p->next;
        }
        p->next = head;
    }
    else
    {
        list->head = head;
    }

    if (NULL != tail)
    {
        tail->next = NULL;
    }
    EnableGlobalIRQ(reg);
}

bt_list_node_t *bt_list_get(bt_list_t *list)
{
    bt_list_node_t *p;
    unsigned int reg;

    reg = DisableGlobalIRQ();
    p = list->head;

    if (NULL != p)
    {
        list->head = list->head->next;
    }
    EnableGlobalIRQ(reg);

    return p;
}

bt_list_node_t *bt_list_get_not_empty(bt_list_t *list)
{
    return bt_list_get(list);
}

void bt_list_prepend(bt_list_t *list, bt_list_node_t *node)
{
    unsigned int reg;

    reg = DisableGlobalIRQ();
    node->next = list->head;
    list->head = node;
    EnableGlobalIRQ(reg);
}

void bt_list_insert(bt_list_t *list, bt_list_node_t *prev, bt_list_node_t *node)
{
    unsigned int reg;

    if (NULL == prev)
    {
        bt_list_prepend(list, node);
    }
    else if (NULL == prev->next)
    {
        bt_list_append(list, node);
    }
    else
    {
        reg = DisableGlobalIRQ();
        node->next = prev->next;
        prev->next = node;
        EnableGlobalIRQ(reg);
    }
}

bool bt_list_is_empty(bt_list_t *list)
{
    return (NULL == list->head) ? true : false;
}

bt_list_node_t *bt_list_peek_next(bt_list_node_t *node)
{
    if (NULL != node)
    {
        return node->next;
    }
    else
    {
        return NULL;
    }
}

bt_list_node_t *bt_list_peek_head(bt_list_t *list)
{
    if (NULL == list)
    {
        return NULL;
    }
    else
    {
        return list->head;
    }
}

bt_list_node_t *bt_list_peek_tail(bt_list_t *list)
{
    bt_list_node_t *p;
    bt_list_node_t *q;
    if (NULL == list)
    {
        return NULL;
    }
    p = list->head;
    q = NULL;
    while (NULL != p)
    {
        q = p;
        p = p->next;
    }
    return q;
}

bool bt_list_find_and_remove(bt_list_t *list, bt_list_node_t *node)
{
    bt_list_node_t *p;
    bt_list_node_t *q;
    unsigned int reg;
    bool ret;

    if ((NULL == list) || (NULL == node) || (NULL == list->head))
    {
        return false;
    }

    reg = DisableGlobalIRQ();
    p = list->head;
    q = NULL;
    while (NULL != p)
    {
        if (p == node)
        {
            break;
        }
        q = p;
        p = p->next;
    }

    if (p == node)
    {
        if (NULL == q)
        {
            list->head = node->next;
        }
        else
        {
            q->next = node->next;
        }
        ret = true;
    }
    else
    {
        ret = false;
    }
    EnableGlobalIRQ(reg);

    return ret;
}

void bt_list_remove(bt_list_t *list, bt_list_node_t *prev_node, bt_list_node_t *node)
{
    unsigned int reg;

    reg = DisableGlobalIRQ();
    if (NULL == prev_node)
    {
        list->head = node->next;
    }
    else
    {
        prev_node->next = node->next;
    }
    EnableGlobalIRQ(reg);

    node->next = NULL;
}

bool bt_list_find(bt_list_t *list, bt_list_node_t *node)
{
    bt_list_node_t *p;
    unsigned int reg;
    bool ret;

    if ((NULL == list) || (NULL == node) || (NULL == list->head))
    {
        return false;
    }

    reg = DisableGlobalIRQ();
    p = list->head;
    while (NULL != p)
    {
        if (p == node)
        {
            break;
        }
        p = p->next;
    }

    if (p == node)
    {
        ret = true;
    }
    else
    {
        ret = false;
    }
    EnableGlobalIRQ(reg);

    return ret;
}
