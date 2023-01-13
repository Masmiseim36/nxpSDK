/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BT_LIST_H_
#define _BT_LIST_H_

#include "stdbool.h"

typedef struct bt_list_node
{
    struct bt_list_node *next;
} bt_list_node_t;

typedef struct slist
{
    bt_list_node_t *head;
} bt_list_t;

void bt_list_init(bt_list_t *list);
void bt_list_append(bt_list_t *list, bt_list_node_t *node);
void bt_list_append_list(bt_list_t *list, bt_list_node_t *head, bt_list_node_t *tail);
bt_list_node_t *bt_list_get(bt_list_t *list);
void bt_list_prepend(bt_list_t *list, bt_list_node_t *node);
bool bt_list_is_empty(bt_list_t *list);
bt_list_node_t *bt_list_peek_next(bt_list_node_t *node);
bt_list_node_t *bt_list_peek_head(bt_list_t *list);
bt_list_node_t *bt_list_peek_tail(bt_list_t *list);
bool bt_list_find_and_remove(bt_list_t *list, bt_list_node_t *node);
bool bt_list_find(bt_list_t *list, bt_list_node_t *node);
bt_list_node_t *bt_list_get_not_empty(bt_list_t *list);
void bt_list_remove(bt_list_t *list, bt_list_node_t *prev_node, bt_list_node_t *node);
void bt_list_insert(bt_list_t *list, bt_list_node_t *prev, bt_list_node_t *node);

#define BT_LIST_CONTAINER(__ln, __n, __t) \
        ((NULL != __ln) ? CONTAINER_OF(__ln, __t, __n) : NULL)

#define BT_LIST_PEEK_NEXT_CONTAINER(__cn, __n, __t)            \
        ((NULL != (__cn)) ?                                    \
        BT_LIST_CONTAINER( bt_list_peek_next(&((__cn)->__n)),  \
        __n, __t)                                              \
        : NULL)

#define BT_LIST_PEEK_HEAD_CONTAINER(__l, __cn, __n, __t)       \
        ((NULL != (__l)) ?                                     \
        BT_LIST_CONTAINER( bt_list_peek_head(__l),             \
        __n, __t)                                              \
        : NULL)

#define BT_LIST_PEEK_TAIL_CONTAINER(__l, __cn, __n, __t)       \
        ((NULL != (__l)) ?                                     \
        BT_LIST_CONTAINER( bt_list_peek_tail(__l),             \
        __n, __t)                                              \
        : NULL)

#define BT_LIST_FOR_EACH_CONTAINER_SAFE(__l, __cn, __cns, __n, __t)          \
    for ( (__cn) = BT_LIST_CONTAINER(bt_list_peek_head(__l), __n, __t),        \
          (__cns) = BT_LIST_PEEK_NEXT_CONTAINER(__cn, __n, __t);               \
          NULL != (__cn); (__cn) = (__cns),                                      \
          (__cns) = BT_LIST_PEEK_NEXT_CONTAINER(__cn, __n, __t))

#define BT_LIST_FOR_EACH_CONTAINER(__l, __cn, __n, __t)                      \
    for ( __cn = BT_LIST_CONTAINER(bt_list_peek_head(__l), __n, __t);        \
          NULL != __cn; __cn = BT_LIST_PEEK_NEXT_CONTAINER(__cn, __n, __t))

#define BT_LIST_FOR_EACH_NODE(__l, __n)    \
    for (__n = bt_list_peek_head(__l); __n != NULL;   \
         __n = bt_list_peek_next(__n))

#define BT_LIST_FOR_EACH_NODE_SAFE(__l, __n, __ns)    \
    for (__n = bt_list_peek_head(__l),                \
             __ns = bt_list_peek_next(__n);           \
         NULL != __n ; __n = __ns,                    \
             __ns = bt_list_peek_next(__n))

/* slist support */
typedef bt_list_t sys_slist_t;
typedef bt_list_node_t sys_snode_t;

#define SYS_SLIST_FOR_EACH_CONTAINER_SAFE     BT_LIST_FOR_EACH_CONTAINER_SAFE
#define SYS_SLIST_FOR_EACH_CONTAINER          BT_LIST_FOR_EACH_CONTAINER
#define SYS_SLIST_FOR_EACH_NODE_SAFE          BT_LIST_FOR_EACH_NODE_SAFE
#define SYS_SLIST_FOR_EACH_NODE               BT_LIST_FOR_EACH_NODE
#define SYS_SLIST_PEEK_HEAD_CONTAINER         BT_LIST_PEEK_HEAD_CONTAINER
#define SYS_SLIST_PEEK_TAIL_CONTAINER         BT_LIST_PEEK_TAIL_CONTAINER
#define SYS_SLIST_PEEK_NEXT_CONTAINER         BT_LIST_PEEK_NEXT_CONTAINER

#define SYS_SLIST_STATIC_INIT(ptr_to_list) {NULL}

#define sys_slist_init              bt_list_init
#define sys_slist_append            bt_list_append
#define sys_slist_append_list       bt_list_append_list
#define sys_slist_get               bt_list_get
#define sys_slist_prepend           bt_list_prepend
#define sys_slist_is_empty          bt_list_is_empty
#define sys_slist_peek_next         bt_list_peek_next
#define sys_slist_peek_head         bt_list_peek_head
#define sys_slist_peek_tail         bt_list_peek_tail
#define sys_slist_find_and_remove   bt_list_find_and_remove
#define sys_slist_find              bt_list_find
#define sys_slist_get_not_empty     bt_list_get_not_empty
#define sys_slist_remove            bt_list_remove
#define sys_slist_insert            bt_list_insert

#endif /* _BT_LIST_H_ */

