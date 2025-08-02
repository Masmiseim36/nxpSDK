/*
 * Copyright 2021,2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BT_LIST_H_
#define _BT_LIST_H_

#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

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
bool bt_list_find(bt_list_t *list, bt_list_node_t *node, bt_list_node_t **prev);
bt_list_node_t *bt_list_get_not_empty(bt_list_t *list);
void bt_list_remove(bt_list_t *list, bt_list_node_t *prev_node, bt_list_node_t *node);
void bt_list_insert(bt_list_t *list, bt_list_node_t *prev, bt_list_node_t *node);

void bt_list_scan(bt_list_t *list, bool (*cb)(void* node, void* p), void *param);

#define BT_LIST_CONTAINER(__lname, __ln, __cn, __n) \
        ((NULL != __ln) ? CONTAINER_OF(__ln, __typeof__(*(__cn)), __n) : NULL)

#define BT_LIST_PEEK_NEXT_CONTAINER(__lname, __cn, __n, ...)                 \
        ((NULL != (__cn)) ?                                    \
        BT_LIST_CONTAINER(__lname, sys_##__lname##_peek_next(&((__cn)->__n)),  \
        __cn, __n)                                             \
        : NULL)

#define BT_LIST_PEEK_HEAD_CONTAINER(__lname, __l, __cn, __n, ...)            \
        ((NULL != (__l)) ?                                     \
        BT_LIST_CONTAINER(__lname, sys_##__lname##_peek_head(__l),             \
        __cn, __n)                                             \
        : NULL)

#define BT_LIST_PEEK_TAIL_CONTAINER(__lname, __l, __cn, __n, ...)            \
        ((NULL != (__l)) ?                                     \
        BT_LIST_CONTAINER(__lname, sys_##__lname##_peek_tail(__l),             \
        __cn, __n)                                             \
        : NULL)

#define BT_LIST_FOR_EACH_CONTAINER_SAFE(__lname, __l, __cn, __cns, __n, ...)            \
    for ( (__cn) = BT_LIST_CONTAINER(__lname, sys_##__lname##_peek_head(__l), __cn, __n),  \
          (__cns) = BT_LIST_PEEK_NEXT_CONTAINER(__lname, __cn, __n);               \
          NULL != (__cn); (__cn) = (__cns),                               \
          (__cns) = BT_LIST_PEEK_NEXT_CONTAINER(__lname, __cn, __n))

#define BT_LIST_FOR_EACH_CONTAINER(__lname, __l, __cn, __n, ...)                            \
    for ( __cn = BT_LIST_CONTAINER(__lname, sys_##__lname##_peek_head(__l), __cn, __n);        \
          NULL != __cn; __cn = BT_LIST_PEEK_NEXT_CONTAINER(__lname, __cn, __n))

#define BT_LIST_FOR_EACH_NODE(__lname, __l, __n)    \
    for (__n = sys_##__lname##_peek_head(__l); __n != NULL;   \
         __n = sys_##__lname##_peek_next(__n))

#define BT_LIST_FOR_EACH_NODE_SAFE(__lname, __l, __n, __ns)    \
    for (__n = sys_##__lname##_peek_head(__l),                \
             __ns = sys_##__lname##_peek_next(__n);           \
         NULL != __n ; __n = __ns,                    \
             __ns = sys_##__lname##_peek_next(__n))

/* slist support */
typedef bt_list_t sys_slist_t;
typedef bt_list_node_t sys_snode_t;

#define SYS_SLIST_FOR_EACH_CONTAINER_SAFE(...)                   \
        BT_LIST_FOR_EACH_CONTAINER_SAFE(slist, __VA_ARGS__)

#define SYS_SLIST_FOR_EACH_CONTAINER(...)                        \
        BT_LIST_FOR_EACH_CONTAINER(slist, __VA_ARGS__)

#define SYS_SLIST_FOR_EACH_NODE_SAFE(__l, __n, __ns)             \
        BT_LIST_FOR_EACH_NODE_SAFE(slist, __l, __n, __ns)

#define SYS_SLIST_FOR_EACH_NODE(__l, __n)                        \
        BT_LIST_FOR_EACH_NODE(slist, __l, __n)

#define SYS_SLIST_PEEK_HEAD_CONTAINER(...)                       \
        BT_LIST_PEEK_HEAD_CONTAINER(slist, __VA_ARGS__)

#define SYS_SLIST_PEEK_TAIL_CONTAINER(...)                       \
        BT_LIST_PEEK_TAIL_CONTAINER(slist, __VA_ARGS__)

#define SYS_SLIST_PEEK_NEXT_CONTAINER(...)                       \
        BT_LIST_PEEK_NEXT_CONTAINER(slist, __VA_ARGS__)

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

#ifdef __cplusplus
}
#endif

#endif /* _BT_LIST_H_ */

