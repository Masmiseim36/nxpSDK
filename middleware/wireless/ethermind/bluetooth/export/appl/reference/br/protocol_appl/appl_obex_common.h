/**
 *  \file appl_obex_common.h
 *
 *  Header File for OBEX Command Line Application
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

#ifndef _H_APPL_OBEX_COMMON_
#define _H_APPL_OBEX_COMMON_

/* ----------------------------------------- Header File Inclusion */
#include "BT_obex_api.h"

/* ----------------------------------------- Global Definitions */

/* ----------------------------------------- Structures/Data Types */

typedef UCHAR  APPL_OBEX_HANDLE;

#define APPL_OBEX_CLI_NUM_INSTANCES               0x01U
#define APPL_OBEX_SER_NUM_INSTANCES               0x01U

#define APPL_OBEX_NUM_INSTANCES                   0x01U

/**
 * OBEX application L2CAP PSM for local client entities
 */
#define APPL_OBEX_L2CAP_PSM                       0x6401U

typedef struct _APPL_OBEX_ENTITY_
{
    /**
     *Application callback
     */
    OBEX_NOTIFY_CB notify_cb;

    /**
     * OBEX Handle
     */
    UCHAR obex_handle;

    /**
     * OBEX Appl. state
     */
    UCHAR appl_state;

    /**
     * OBEX Appl. operation
     */
    UCHAR operation;

    /**
     *RFCOMM Server Channel
     */
    UCHAR rfcomm_sever_ch;

#ifdef OBEX_OVER_L2CAP
    /**
     * Local L2CAP psm
     */
    UINT16 l2cap_lpsm;

    /**
     * Remote L2CAP psm
     */
    UINT16 l2cap_rpsm;
#endif /* OBEX_OVER_L2CAP */

    /**
     * Transaction Length
     */
    UINT16 trans_len;

    /**
     * Bluetooth device address
     */
    UCHAR bd_addr [BT_BD_ADDR_SIZE];

}APPL_OBEX_ENTITY;

typedef struct _APPL_OBEX_INSTANCE
{
    /**
     * OBEX handle
     */
    APPL_OBEX_HANDLE obex_app_handle;

    /**
     * OBEX entity
     */
    APPL_OBEX_ENTITY *obex_entity;

    /**
     * Peer BD Address
     */
    UCHAR bd_addr[BT_BD_ADDR_SIZE];

}APPL_OBEX_INSTANCE;


/**
 * OBEX Header structure, to exchage data with application
 */
typedef struct _APPL_OBEX_HDR_STRUCT
{
    /**
     * Pointer Header value
     */
    UCHAR   * value;

    /**
     * Header value length
     */
    UINT16    length;

} APPL_OBEX_HDR_STRUCT;

/**
 * OBEX Connection Structure
 */
typedef struct _APPL_OBEX_CONNECT_STRUCT
{
    /**
     * Peer BD address
     */
    UCHAR       * bd_addr;

    /**
     * Maximum Receive Size
     */
    UINT16      max_recv_size;

    /**
     * Peer RFCOMM channel
     */
    UINT8       rfcomm_server_ch;

#ifdef OBEX_OVER_L2CAP
    /**
     * Peer L2CAP psm
     */
    UINT16      psm;
#endif /* OBEX_OVER_L2CAP */
} APPL_OBEX_CONNECT_STRUCT;

/**
 * OBEX Request Structure
 */
typedef struct _APPL_OBEX_REQ_STRUCT
{
    /**
     * Name of the object
     */
    APPL_OBEX_HDR_STRUCT   * name;

    /**
     * Object content
     */
    APPL_OBEX_HDR_STRUCT   * body;

} APPL_OBEX_REQ_STRUCT;

/**
 * OBEX Response Structure
 */
typedef struct _APPL_OBEX_RSP_STRUCT
{
    /**
     * Name of the object
     */
    APPL_OBEX_HDR_STRUCT  * name;

    /**
     * Object contents
     */
    APPL_OBEX_HDR_STRUCT  * body;

} APPL_OBEX_RSP_STRUCT;

/* ----------------------------------------- Macros */

/**
 * OBEX Application operations
 */
#define APPL_OBEX_OP_PUT                              0x00U
#define APPL_OBEX_OP_GET                              0x01U
#define APPL_OBEX_OP_SETPATH                          0x02U
#define APPL_OBEX_OP_ACTION                           0x03U
#define APPL_OBEX_OP_NONE                             0xFFU

#define APPL_OBEX_EVENT_INVALID                       0xFFU
#define APPL_OBEX_ENTITY_INVALID                      0xFFU

/* ======================================= Section 'OBEX Application' */


#define APPL_OBEX_ERR_ID                              0xFF00U

#define APPL_OBEX_INVALID_STATE                       (0x0011U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_INVALID_PARAMETERS                  (0x0012U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_ENTITY_START_FAILED                 (0x0013U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_ENTITY_STOP_FAILED                  (0x0014U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_INVALID_CONNECT_REQ_LENGTH          (0x0015U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_GET_OPERATION_ABORTED               (0x0016U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_PUT_OPERATION_ABORTED               (0x0017U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_INVALID_CONNECTION_ID               (0x0018U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_ENTITY_ALLOC_FAILED                 (0x0019U | APPL_OBEX_ERR_ID)
#define APPL_OBEX_ENTITY_SEARCH_FAILED                (0x001AU | APPL_OBEX_ERR_ID)

#define APPL_OBEX_SET_STATE(inst, state) \
    appl_obex_entities[inst].appl_state = (state)

#define APPL_OBEX_GET_STATE(inst, state) \
     (state) = appl_obex_entities[inst].appl_state

/**
 * To Reset a OBEX Applicaition handle
 */
#define APPL_OBEX_RESET_INSTANCE(hdl)    \
        (hdl)->obex_app_handle = APPL_OBEX_ENTITY_INVALID;    \
        BT_mem_set((hdl)->bd_addr, 0x00, BT_BD_ADDR_SIZE);

/**
 * To initialize a Header Structure
 */
#define APPL_OBEX_INIT_HEADER_STRUCT(hs)    \
        (hs).value = NULL;    \
        (hs).length = 0U;

/* ----------------------------------------- Function Declarations */

#endif /* _H_APPL_OBEX_COMMON_ */

