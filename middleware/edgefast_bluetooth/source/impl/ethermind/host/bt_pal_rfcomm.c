/* zephyr_rfcomm.c - RFCOMM handling */

/*
 * Copyright (c) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <errno.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_CONN)
#define LOG_MODULE_NAME bt_spp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/addr.h>
#include "BT_rfcomm_api.h"
#include <bluetooth/rfcomm.h>

#include "bt_pal_rfcomm_internal.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"

#ifndef RFCOMM_MAX_ENTITY
#define RFCOMM_MAX_ENTITY 2
#endif /* RFCOMM_MAX_ENTITY */

#define RFCOMM_CHANNEL_START 0x01
#define RFCOMM_CHANNEL_END 0x1e

#define RFCOMM_MIN_MTU  BT_RFCOMM_SIG_MIN_MTU
#define RFCOMM_DEFAULT_MTU 127

/* Used to look up a server is registered or not  */
static struct bt_rfcomm_server *servers;

/** RFCOMM Entity States */
typedef enum _rfcomm_state
{
    RFCOMM_STATE_INITIALIZED = 0x0U,
    RFCOMM_STATE_STARTED     = 0x01U,
    RFCOMM_IN_CONNECT        = 0x02U,
    RFCOMM_CONNECTED         = 0x03U,
    RFCOMM_IN_DISCONNECT     = 0x04U,
    RFCOMM_WAIT_4_CONNECT    = 0x05U,
    RFCOMM_STOP_PENDING      = 0x06U,
}rfcomm_state_t;

/**
 *  Data Type to represent a RFCOMM Instance (Service Level Connection)
 */
typedef struct
{
  /* State of RFCOMM Instance */
  rfcomm_state_t state;

  /* RFCOMM Handle */
  RFCOMM_HANDLE rfcomm_handle;

    /* Local Server */
  struct bt_rfcomm_server *server;

  /* Local dlc */
  struct bt_rfcomm_dlc *dlc;

} RFOMM_ENTITY;

static RFOMM_ENTITY rfcomm_entity[RFCOMM_MAX_ENTITY];

/** To set State of RFCOMM Instance/Entity */
#define RFCOMM_SET_ENTITY_STATE(i, ns)     \
    rfcomm_entity[(i)].state = (ns);

/** To initialize a RFCOMM Entity */
#define RFCOMM_INIT_ENTITY(en)             \
{                                          \
    RFCOMM_RESET_HANDLE                    \
    (                                      \
        &rfcomm_entity[(en)].rfcomm_handle \
    );                                     \
                                           \
    RFCOMM_SET_ENTITY_STATE                \
    (                                      \
        (en),                              \
        RFCOMM_STATE_INITIALIZED           \
    );                                     \
                                           \
    rfcomm_entity[(en)].server = NULL;     \
    rfcomm_entity[(en)].dlc = NULL;        \
}

/** To get free RFCOMM Entities */
#define RFCOMM_GET_FREE_ENTITY(instance)                            \
{                                                                   \
        uint32_t i;                                                 \
        for (i = 0; i < RFCOMM_MAX_ENTITY; i ++)                    \
        {                                                           \
            if (RFCOMM_STATE_INITIALIZED == rfcomm_entity[i].state) \
            {                                                       \
                break;                                              \
            }                                                       \
        }                                                           \
        (instance) = i;                                             \
}

/** To get correct RFCOMM Entitiy according to RFCOMM handle */
#define RFCOMM_GET_ENTITY_FROM_HANDLE(handle, instance)                                  \
{                                                                                        \
        uint32_t i;                                                                      \
        for (i = 0; i < RFCOMM_MAX_ENTITY; i ++)                                         \
        {                                                                                \
            if (handle->server_channel == rfcomm_entity[i].rfcomm_handle.server_channel) \
            {                                                                            \
                break;                                                                   \
            }                                                                            \
        }                                                                                \
        (instance) = i;                                                                  \
}

NET_BUF_POOL_FIXED_DEFINE(rfcomm_pool, 1, RFCOMM_DEFAULT_MTU, NULL);

static uint8_t initialized = 0U;

/* RFCOMM Mutex */
static OSA_MUTEX_HANDLE_DEFINE(rfcomm_lock_mutex);
#define EDGEFAST_RFCOMM_LOCK OSA_MutexLock((osa_mutex_handle_t)rfcomm_lock_mutex, osaWaitForever_c)
#define EDGEFAST_RFCOMM_UNLOCK OSA_MutexUnlock((osa_mutex_handle_t)rfcomm_lock_mutex)

static API_RESULT rfcomm_accept(uint32_t entity_index);

static struct bt_rfcomm_server *rfcomm_server_lookup_channel(uint8_t channel)
{
    struct bt_rfcomm_server *server;

    EDGEFAST_RFCOMM_LOCK;
    for (server = servers; server; server = server->_next)
    {
        if (server->channel == channel)
        {
            EDGEFAST_RFCOMM_UNLOCK;
            return server;
        }
    }

    EDGEFAST_RFCOMM_UNLOCK;
    return NULL;
}

static int spp_convert_return_value(API_RESULT retval);

static void rfcomm_get_entity_handle
       (
           /* IN */ RFCOMM_HANDLE * rfcomm_handle,
           /* IN */ uint8_t rfcomm_event,
           /* OUT */ uint32_t * rfcomm_entity_index
       );

static API_RESULT rfcomm_callback
           (
               /* IN */ uint8_t event_type,
               /* IN */ RFCOMM_HANDLE * handle,
               /* IN */ uint16_t result,
               /* IN */ uint8_t * data,
               /* IN */ uint16_t datalen
           );

/**
 *  \fn rfcomm_get_entity_handle
 *
 *  \brief To find the RFCOMM entity associated with an RFCOMM Handle
 *
 *  \param Description
 *  This function searches for Handle of RFCOMM entity associated with
 *  the RFCOMM Handle and RFCOMM Event passed as parameters.
 *
 *  \param rfcomm_handle (IN)
 *         RFCOMM Handle.
 *
 *  \param rfcomm_event (IN)
 *         RFCOMM Event.
 *
 *  \param rfcomm_entity (OUT)
 *         Parameter to be filled with the RFCOMM entity associated with
 *         specified RFCOMM Handle and RFCOMM Event.
 *
 *  \return API_RESULT
 *          API_SUCCESS on success otherwise an error code
 *          from BT_error.h describing the cause of failure.
 */
static void rfcomm_get_entity_handle
       (
           /* IN */ RFCOMM_HANDLE * rfcomm_handle,
           /* IN */ uint8_t rfcomm_event,
           /* OUT */ uint32_t * rfcomm_entity_index
       )
{
    uint32_t i, ret;

    BT_INFO ("[RFCOMM] > rfcomm_get_entity_handle()\n");

    ret = 0;
    for (i = 0; i < RFCOMM_MAX_ENTITY; i++)
    {
        if (RFCOMM_STATE_INITIALIZED == rfcomm_entity[i].state)
        {
            continue;
        }

        if (RFCOMM_ACCEPT == rfcomm_event)
        {
            if (RFCOMM_WAIT_4_CONNECT == rfcomm_entity[i].state)
            {
                /* Match only the Server Channel */
                ret = (rfcomm_handle->server_channel ==
                       rfcomm_entity[i].rfcomm_handle.server_channel);
            }
        }
        else if (RFCOMM_OPEN == rfcomm_event)
        {
            if (RFCOMM_IN_CONNECT == rfcomm_entity[i].state)
            {
                /* Match only server channel and bd_addr */
                ret = ((rfcomm_handle->server_channel ==
                        rfcomm_entity[i].rfcomm_handle.server_channel) &&
                        (0 == BT_mem_cmp(rfcomm_handle->bd_addr, rfcomm_entity[i].rfcomm_handle.bd_addr, BT_BD_ADDR_SIZE)));
            }
        }
        else
        {
            /* Match all element of RFCOMM Handle */
            ret = BT_rfcomm_compare_handles
                  (
                      rfcomm_handle,
                      &rfcomm_entity[i].rfcomm_handle
                  );
        }

        if (ret)
        {
            /* Found RFCOMM Entity Handle */
            break;
        }
    }

    /* Save RFCOMM Entity Handle */
    *rfcomm_entity_index = i;

    BT_INFO (
    "[RFCOMM] < rfcomm_get_entity_handle(). RFCOMM Handle 0x%02X\n",
    (unsigned int)*rfcomm_entity_index);

    return;
}

/**
 *  \fn rfcomm_callback
 *
 *  \brief To handle RFCOMM callback
 *
 *  \param Description
 *  This callback is called by RFCOMM to indicate RFCOMM events to the profile.
 *
 *  \param event_type (IN)
 *         RFCOMM event.
 *
 *  \param handle (IN)
 *         RFCOMM Handle.
 *
 *  \param data (IN)
 *         Pointer to data buffer.
 *
 *  \param datalen (IN)
 *         Length of data.
 *
 *  \return API_RESULT
 *          API_SUCCESS on success otherwise an error code
 *          from BT_error.h describing the cause of failure.
 */
static API_RESULT rfcomm_callback
           (
               /* IN */ uint8_t event_type,
               /* IN */ RFCOMM_HANDLE * handle,
               /* IN */ uint16_t result,
               /* IN */ uint8_t * data,
               /* IN */ uint16_t datalen
           )
{
    uint32_t         rfcomm_entity_index;
    struct net_buf * buf;

    /*
     * Checking the event type.
     * In case of user initiated accept cancel return.
     */
    if ((RFCOMM_ACCEPT == event_type) &&
        (RFCOMM_ACCEPT_CANCELLED_BY_USER == result))
    {
        return API_SUCCESS;
    }

    /* Get RFCOMM Entity index from RFCOMM Handle */
    rfcomm_get_entity_handle (handle, event_type, &rfcomm_entity_index);

    if (RFCOMM_MAX_ENTITY == rfcomm_entity_index)
    {
        BT_ERR("[RFCOMM] FAILED to Find RFCOMM Entity Instance from RFCOMM Handle\n");

        return API_SUCCESS;
    }

    switch (event_type)
    {
    case RFCOMM_OPEN:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Open Event. Result = 0x%04X\n", result);

        /* Handle RFCOMM Open */
        if (API_SUCCESS == result)
        {
            RFCOMM_SET_ENTITY_STATE (rfcomm_entity_index, RFCOMM_CONNECTED);

            /* Initialize RFCOMM Entity */
            RFCOMM_SET_HANDLE
            (
                &rfcomm_entity[rfcomm_entity_index].rfcomm_handle,
                handle->bd_addr,
                handle->server_channel,
                rfcomm_callback
            );

            /* Save RFCOMM DLCI */
            rfcomm_entity[rfcomm_entity_index].rfcomm_handle.dlci = handle->dlci;
            rfcomm_entity[rfcomm_entity_index].dlc->dlci = handle->dlci;

            BT_INFO("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (handle->bd_addr));

            /* Get application registered dlc which contains application connected, disconnected, recv callback */
            if((NULL != rfcomm_entity[rfcomm_entity_index].dlc) && rfcomm_entity[rfcomm_entity_index].dlc->ops && rfcomm_entity[rfcomm_entity_index].dlc->ops->connected)
            {
                /* Call application registered connected callback */
                rfcomm_entity[rfcomm_entity_index].dlc->ops->connected(rfcomm_entity[rfcomm_entity_index].dlc);
            }
        }
        else
        {
            RFCOMM_SET_ENTITY_STATE (rfcomm_entity_index, RFCOMM_STATE_INITIALIZED);
        }
        break;

    case RFCOMM_ACCEPT:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Accept Event. Result = 0x%04X\n",result);

        /* Handle RFCOMM Accept */
        if (API_SUCCESS == result)
        {
            /* Set RFOMM Entity State as 'Connected' */
            RFCOMM_SET_ENTITY_STATE (rfcomm_entity_index, RFCOMM_CONNECTED);

            /* Initialize RFCOMM Handle for RFCOMM Entity */
            RFCOMM_SET_HANDLE
            (
                &rfcomm_entity[rfcomm_entity_index].rfcomm_handle,
                handle->bd_addr,
                handle->server_channel,
                rfcomm_callback
            );

            /* Save RFCOMM DLCI in RFCOMM Handle */
            rfcomm_entity[rfcomm_entity_index].rfcomm_handle.dlci = handle->dlci;

            BT_INFO("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (handle->bd_addr));

            /* Get application registered dlc which contains application connected, disconnected, recv callback */
            if((NULL != rfcomm_entity[rfcomm_entity_index].server) && rfcomm_entity[rfcomm_entity_index].server->accept(NULL, &rfcomm_entity[rfcomm_entity_index].dlc) < 0)
            {
                BT_INFO("Incoming connection rejected");
            }
            else
            {
                /* Save dlci and peer device address to indentify a unique RFCOMM connection */
                rfcomm_entity[rfcomm_entity_index].dlc->dlci = handle->dlci;

                /* get conn */
                //rfcomm_entity[   .dlc->session.br_chan.chan.conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &bd_addr);

                memcpy((void *)rfcomm_entity[rfcomm_entity_index].dlc->session->br_chan.chan.conn->br.dst.val, handle->bd_addr, BT_BD_ADDR_SIZE);

                if (rfcomm_entity[rfcomm_entity_index].dlc->ops && rfcomm_entity[rfcomm_entity_index].dlc->ops->connected)
                {
                    /* Call application registered connected callback */
                    rfcomm_entity[rfcomm_entity_index].dlc->ops->connected(rfcomm_entity[rfcomm_entity_index].dlc);
                }
            }
        }
        else
        {
            /* When connection is accepted successfully, wait for rfcomm accept again */
            (void)rfcomm_accept(rfcomm_entity_index);
        }
        break;

    case RFCOMM_CLOSE:         /* Fall Through */
    case RFCOMM_RESET:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Close/Reset. Result = 0x%04X\n",result);

        if (((API_SUCCESS == result) && (RFCOMM_IN_DISCONNECT == rfcomm_entity[rfcomm_entity_index].state)) /* RFCOMM CLOSE event for initialize rfcomm disconnect */
           || ((0x2642 == result) && (RFCOMM_CONNECTED == rfcomm_entity[rfcomm_entity_index].state))        /* RFCOMM CLOSE event for accepting rfcomm disconnect */
           )
        {
            if (NULL != rfcomm_entity[rfcomm_entity_index].server)
            {
                /* When rfcomm server is disconnected, wait for rfocmm accept again */
                (void)rfcomm_accept(rfcomm_entity_index);
            }
            else
            {
                /* Call application registered disconnect callback */
                if (rfcomm_entity[rfcomm_entity_index].dlc->ops && rfcomm_entity[rfcomm_entity_index].dlc->ops->disconnected)
                {
                    rfcomm_entity[rfcomm_entity_index].dlc->ops->disconnected(rfcomm_entity[rfcomm_entity_index].dlc);
                }
                RFCOMM_INIT_ENTITY(rfcomm_entity_index);
            }
        }
        break;

    case RFCOMM_WRITE:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Write. Result = 0x%04X\n",result);
        break;

    case RFCOMM_READ:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Read. Result = 0x%04X, Data Length = %u\n",result, datalen);

        if (rfcomm_entity[rfcomm_entity_index].dlc->ops && rfcomm_entity[rfcomm_entity_index].dlc->ops->recv)
        {
            buf = bt_rfcomm_create_pdu(&rfcomm_pool);
            net_buf_add_mem(buf, data, strlen((char const *)data));
            /* Call application registered recv callback */
            rfcomm_entity[rfcomm_entity_index].dlc->ops->recv(rfcomm_entity[rfcomm_entity_index].dlc, buf);

            net_buf_unref(buf);
        }
        break;
    }

    return API_SUCCESS;
}

static API_RESULT rfcomm_accept(uint32_t entity_index)
{
    API_RESULT retval;

    /* Prepare RFCOMM Handle for Accept */
    RFCOMM_RESET_HANDLE (&rfcomm_entity[entity_index].rfcomm_handle);
    RFCOMM_SET_HANDLE_ACCEPT
    (
        &rfcomm_entity[entity_index].rfcomm_handle,
        rfcomm_entity[entity_index].server->channel,
        rfcomm_callback
    );

    /* Doing an RFCOMM accept to wait for incomming RFCOMM connection */
    retval = BT_rfcomm_accept
            (
                &rfcomm_entity[entity_index].rfcomm_handle,
                rfcomm_entity[entity_index].rfcomm_handle.server_channel
            );

    if (API_SUCCESS == retval)
    {
        /* Set RFCOMM Entity State as 'Waiting for Connect' */
        RFCOMM_SET_ENTITY_STATE(entity_index, RFCOMM_WAIT_4_CONNECT);

        BT_INFO("RFCOMM Accept on Server Channel %d is created successfully, waitting for RFCOMM ACCEPT Event.\n",rfcomm_entity[entity_index].rfcomm_handle.server_channel);
    }
    else
    {
        BT_ERR("RFCOMM Accept on Server Channel %d is created failed, reason = 0x%04X.\n",retval);
        RFCOMM_INIT_ENTITY(entity_index);
    }

    return retval;
}

/** Initialize RFCOMM entities. */
static void rfcomm_initialize(void)
{
    uint8_t index;

    if (0U == initialized)
    {
        /* init rfcomm entities */
        for (index = 0U; index < RFCOMM_MAX_ENTITY; index++)
        {
            RFCOMM_INIT_ENTITY(index);
        }

        /* create rfcomm Mutex */
        if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)rfcomm_lock_mutex))
        {
            BT_ERR("OSA_MutexCreate() failed!\n");
        }

        initialized = 1U;
    }
}

int bt_rfcomm_server_register(struct bt_rfcomm_server *server)
{
    API_RESULT retval;
    uint32_t   index = 0U;

    rfcomm_initialize();

    /* Check if given channel is already in use */
    if (rfcomm_server_lookup_channel(server->channel))
    {
        BT_INFO("Channel already registered.\n");
        return -EINVAL;
    }

    /*Get free rfcomm entity*/
    RFCOMM_GET_FREE_ENTITY(index);

    if (RFCOMM_MAX_ENTITY == index)
    {
        BT_ERR("No available rfcomm entity.\n");
        return -ENOMEM;
    }

     /*Recore registered application server*/
     rfcomm_entity[index].server = server;
     server->_next = servers;
     servers = server;

     retval = rfcomm_accept(index);

    return spp_convert_return_value(retval);
}

struct net_buf *bt_rfcomm_create_pdu(struct net_buf_pool *pool)
{
    /* Length in RFCOMM header can be 2 bytes depending on length of user
    * data
    */
    return bt_conn_create_pdu(pool,
        sizeof(struct bt_l2cap_hdr) +
        sizeof(struct bt_rfcomm_hdr) + 1);
}

int bt_rfcomm_dlc_send(struct bt_rfcomm_dlc *dlc, struct net_buf *buf)
{
    API_RESULT retval;
    uint16_t   actual;
    uint32_t   index;

    /* Look up rfcomm entity index according dlci */
    for (index = 0U; index < RFCOMM_MAX_ENTITY; index ++)
    {
        if ( dlc->dlci == rfcomm_entity[index].rfcomm_handle.dlci)
        {
            break;
        }
    }

    if (RFCOMM_MAX_ENTITY == index)
    {
        BT_ERR("Didn't find correct rfcomm entity according to this dlc.\n");
        return -EINVAL;
    }

    /* State Check */
    if (RFCOMM_CONNECTED != rfcomm_entity[index].state)
    {
        BT_ERR ("[RFCOMM] Invalid State (0x%04X) for Send String\n",rfcomm_entity[index].state);

        retval = RFCOMM_STATE_INCORRECT_FOR_WRITE;
    }
    else
    {
        /* Send AT Command */
        retval = BT_rfcomm_write
                (
                    &rfcomm_entity[index].rfcomm_handle,
                    buf->b.data,
                    buf->b.len,
                    &actual
                );

        if (API_SUCCESS == retval)
        {
            BT_INFO("RFCOMM Write %d data is created successfully, waitting for RFCOMM Write Event.\n",actual);
        }
        else
        {
            BT_ERR("RFCOMM Write %d data is created failed, reason is 0x%04X.\n",retval);
        }
    }

    return spp_convert_return_value(retval);
}

int bt_rfcomm_dlc_connect(struct bt_conn *conn, struct bt_rfcomm_dlc *dlc,
     uint8_t channel)
{
    API_RESULT retval;
    uint32_t   index = 0U;

    rfcomm_initialize();

    /*Get free rfcomm entity*/
    RFCOMM_GET_FREE_ENTITY(index);

    if (RFCOMM_MAX_ENTITY == index)
    {
        return -ENOMEM;
    }

    /* Prepare RFCOMM Handle for Open */
    RFCOMM_RESET_HANDLE (&rfcomm_entity[index].rfcomm_handle);
    RFCOMM_SET_HANDLE
    (
      &rfcomm_entity[index].rfcomm_handle,
      bt_conn_get_dst_br(conn)->val,
      channel,
      rfcomm_callback
    );

    rfcomm_entity[index].dlc = dlc;

    /* Call RFCOMM Open */
    retval = BT_rfcomm_open
             (
                 rfcomm_entity[index].rfcomm_handle.bd_addr,
                 rfcomm_entity[index].rfcomm_handle.server_channel,
                 &rfcomm_entity[index].rfcomm_handle
             );

    if (API_SUCCESS == retval)
    {
        RFCOMM_SET_ENTITY_STATE (index, RFCOMM_IN_CONNECT);
        BT_INFO("RFCOMM dlc connect on server %d is created successfully, waitting for RFCOMM Open Event.\n",rfcomm_entity[index].rfcomm_handle.server_channel);
    }
    else
    {
        BT_ERR("RFCOMM dlc connect on server %d is created failed, reason = 0x%04X.\n",retval);
        RFCOMM_INIT_ENTITY(index);
    }

    return spp_convert_return_value(retval);
}

int bt_rfcomm_dlc_disconnect(struct bt_rfcomm_dlc *dlc)
{
    API_RESULT retval;
    uint32_t   index;

    for (index = 0U; index < RFCOMM_MAX_ENTITY; index ++)
    {
        if (dlc->dlci == rfcomm_entity[index].rfcomm_handle.dlci)
        {
            break;
        }
    }

    if (RFCOMM_MAX_ENTITY == index)
    {
        BT_ERR("Didn't find correct rfcomm entity according to this dlc.\n");
        return -EINVAL;
    }

    /* State Check */
    if (RFCOMM_CONNECTED != rfcomm_entity[index].state)
    {
        BT_ERR("[RFCOMM] Invalid State (0x%04X) for dlc disconnect\n",rfcomm_entity[index].state);
        retval = RFCOMM_STATE_INCORRECT_FOR_CLOSE;
    }
    else
    {
        /* Call RFCOMM close API to initiate disconnection at the RFCOMM level */
        retval = BT_rfcomm_close
                (
                    &rfcomm_entity[index].rfcomm_handle
                );

        /* Check the retval */
        if (API_SUCCESS == retval)
        {
            /* Set RFCOMM Entity State to 'In Disconnect' */
            RFCOMM_SET_ENTITY_STATE (index, RFCOMM_IN_DISCONNECT);
            BT_INFO("RFCOMM dlc disconnect is created successfully, waitting for RFCOMM Close event.\n");
        }
        else
        {
            BT_ERR("RFCOMM dlc disconnect is created failed, reason is 0x%04X.\n",retval);
        }
    }

    return spp_convert_return_value(retval);
}

void bt_rfcomm_init(void)
{
#if 1  /*implement in MindTree API*/
    /*rfcomm_bt_init(); is implemented in rfcomm_init.c and is called by bluetooth_on_upper_pl()*/
#else  /*implement in zephyr API*/
    static struct bt_l2cap_server server = {
            .psm       = BT_L2CAP_PSM_RFCOMM,
            .accept    = rfcomm_accept,
            .sec_level = BT_SECURITY_L1,
    };

    bt_l2cap_br_server_register(&server);
#endif
}

/** Convert return value. */
static int spp_convert_return_value(API_RESULT retval)
{
    int ret;

    switch(retval)
    {
    case API_SUCCESS:
      ret = 0;
      break;

    case SPP_ERR_NO_FREE_ENTITY:
      ret = -ENOLCK;
      break;

    case SPP_ERR_INVALID_STATE:
      ret = -EINVAL;
      break;

    case RFCOMM_STATE_INCORRECT_FOR_WRITE:
    case RFCOMM_STATE_INCORRECT_FOR_CLOSE:
      ret = -EOPNOTSUPP;
      break;

    default:
      ret = -EIO;
      break;
    }

    return ret;
}
