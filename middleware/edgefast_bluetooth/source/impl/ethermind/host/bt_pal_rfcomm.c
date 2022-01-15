/** zephyr_rfcomm.c - RFCOMM handling */

/*
 * Copyright (c) 2021 Intel Corporation
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <errno/errno.h>

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_RFCOMM)
#define LOG_MODULE_NAME bt_rfcomm
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#if (defined(CONFIG_BT_RFCOMM) && ((CONFIG_BT_RFCOMM) > 0U))
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/addr.h>
#include "BT_rfcomm_api.h"
#include "BT_device_queue.h"
#include <bluetooth/rfcomm.h>

#include "bt_pal_rfcomm_internal.h"
#include "bt_pal_conn_internal.h"
#include "bt_pal_l2cap_internal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
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

#define RFCOMM_DEFAULT_MTU      127

#define RFCOMM_CHANNEL_START    0x01
#define RFCOMM_CHANNEL_END      0x1e

NET_BUF_POOL_FIXED_DEFINE(rfcomm_pool, 1, RFCOMM_DEFAULT_MTU, NULL);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** RFCOMM server list */
static struct bt_rfcomm_server *servers;

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
/** RFCOMM control list */
static sys_slist_t rfcomm_control_list;

/** rfcomm control command callback list */
static sys_slist_t rfcomm_control_cb_list;
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

static uint8_t initialized;

/** RFCOMM Mutex */
static OSA_MUTEX_HANDLE_DEFINE(rfcomm_lock_mutex);
#define EDGEFAST_RFCOMM_LOCK OSA_MutexLock((osa_mutex_handle_t)rfcomm_lock_mutex, osaWaitForever_c)
#define EDGEFAST_RFCOMM_UNLOCK OSA_MutexUnlock((osa_mutex_handle_t)rfcomm_lock_mutex)

/** RFCOMM session entity */
static struct bt_rfcomm_session rfcomm_session[CONFIG_BT_RFCOMM_SESSION_MAX_COUNT];

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/** look up server */
static struct bt_rfcomm_server *rfcomm_server_lookup_channel(uint8_t channel);

/** Let a rfcomm entity servers as a server and waits for incoming rfcomm connection */
static int rfcomm_accept(uint8_t channel);

static int rfcomm_convert_return_value(API_RESULT err);

static struct bt_rfcomm_dlc * rfcomm_get_dlc(RFCOMM_HANDLE * rfcomm_hdl, uint8_t rfcomm_event, uint8_t s_index);

/* rfcomm callback */
static API_RESULT rfcomm_callback( uint8_t event_type, RFCOMM_HANDLE * handle, uint16_t result, uint8_t * data, uint16_t datalen);

static void set_rfcomm_handle_with_dlc(struct bt_rfcomm_dlc *dlc, RFCOMM_HANDLE *hdl);

static struct bt_rfcomm_dlc * rfcomm_dlcs_remove_dlc(struct bt_rfcomm_dlc *dlc, uint8_t index);

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
static struct bt_rfcomm_control * rfcomm_get_control(RFCOMM_HANDLE * rfcomm_hdl, uint8_t rfcomm_event);

static struct bt_rfcomm_control_cb * rfcomm_get_control_callback(struct bt_rfcomm_control *control, uint8_t rfcomm_event);

/* rfcomm control callback */
static API_RESULT rfcomm_control_callback( uint8_t event_type, RFCOMM_HANDLE * handle, uint16_t result, uint8_t * data, uint16_t datalen);

static int bt_rfcomm_send_test(struct bt_rfcomm_control *control);

static int bt_rfcomm_send_flow_control(struct bt_rfcomm_control *control);

static int bt_rfcomm_get_rpn(struct bt_rfcomm_control *control);

static int bt_rfcomm_set_rpn(struct bt_rfcomm_control *control);

static int bt_rfcomm_send_pn(struct bt_rfcomm_control *control);

static int bt_rfcomm_get_local_pn(struct bt_rfcomm_control *control);

static int bt_rfcomm_send_rls(struct bt_rfcomm_control *control);

static int bt_rfcomm_send_msc(struct bt_rfcomm_control *control);
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

/** init rfcomm */
static void rfcomm_init(void);

/** init rfcomm session entity */
static void rfcomm_init_session(uint8_t index);

/** get free rfcomm session entity */
static uint8_t rfcomm_get_free_session(struct bt_conn *conn);

/** check whether rfcomm session entity exists */
static uint8_t rfcomm_is_session_exist(struct bt_conn *conn);

/*******************************************************************************
 * Code
 ******************************************************************************/
/** look up server */
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

static int rfcomm_accept(uint8_t channel)
{
    API_RESULT    ret;
    RFCOMM_HANDLE rfcomm_hdl;

    /** Prepare RFCOMM Handle for Accept */
    RFCOMM_RESET_HANDLE(&rfcomm_hdl);
    RFCOMM_SET_HANDLE_ACCEPT
    (
        &rfcomm_hdl,
        channel,
        rfcomm_callback
    );

    /** Doing an RFCOMM accept to wait for incomming RFCOMM connection */
    ret = BT_rfcomm_accept(&rfcomm_hdl, channel);

    if (API_SUCCESS == ret)
    {
        BT_INFO("[RFCOMM] RFCOMM Accept on Server Channel %d is created successfully, waitting for RFCOMM_ACCEPT Event.\n",channel);
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM Accept on Server Channel %d is created failed, reason = 0x%04X.\n", channel, ret);
    }

    return rfcomm_convert_return_value(ret);
}

/** Convert return value. */
static int rfcomm_convert_return_value(API_RESULT err)
{
    int ret;

    switch(err)
    {
    case API_SUCCESS:
      ret = 0;
      break;

    case RFCOMM_STATE_INCORRECT_FOR_WRITE:
    case RFCOMM_STATE_INCORRECT_FOR_CLOSE:
    case RFCOMM_STATE_INCORRECT_FOR_RPN:
    case RFCOMM_STATE_INCORRECT_FOR_PN:
      ret = -ENOTCONN;
      break;

    case RFCOMM_SESSION_NOT_CONNECTED:
    case RFCOMM_DLCI_NOT_CONNECTED:
      ret = -ENOTCONN;
      break;

    default:
      ret = -EIO;
      break;
    }

    return ret;
}

/**
 *  \fn rfcomm_get_dlc
 *
 *  \brief To find the RFCOMM dlc associated with an RFCOMM Handle
 *
 *  \param Description
 *  This function searches for Handle of RFCOMM dlc associated with
 *  the RFCOMM Handle and RFCOMM Event passed as parameters.
 *
 *  \param rfcomm_handle (IN)
 *         RFCOMM Handle.
 *
 *  \param rfcomm_event (IN)
 *         RFCOMM Event.
 *
 *  \return rfcomm dlc.
 */
static struct bt_rfcomm_dlc * rfcomm_get_dlc(RFCOMM_HANDLE * rfcomm_hdl, uint8_t rfcomm_event, uint8_t s_index)
{
    struct bt_rfcomm_dlc *dlc;

    BT_INFO ("[RFCOMM] > get_rfcomm_handle_with_rfcomm()\n");

    EDGEFAST_RFCOMM_LOCK;
    switch(rfcomm_event)
    {
#if 0
    case RFCOMM_ACCEPT:
      /** no dlc */
      dlc = NULL;
      break;
#endif

    case RFCOMM_OPEN:
      for(dlc = rfcomm_session[s_index].dlcs; dlc; dlc = dlc->_next)
      {
          /** Match state, role, server channel and bd_addr */
          if((RFCOMM_IN_CONNECT == dlc->state) &&
             (BT_RFCOMM_ROLE_INITIATOR == dlc->role) &&
             (rfcomm_hdl->server_channel == dlc->dlci) &&
             (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
          {
              break;
          }
      }
      break;

    case RFCOMM_WRITE:
    case RFCOMM_READ:
    case RFCOMM_RESET:
    case RFCOMM_RECVD_RLS:
    case RFCOMM_RECVD_MSC:
      for(dlc = rfcomm_session[s_index].dlcs; dlc; dlc = dlc->_next)
      {
          /** Match state, server channel, dlci and bd_addr */
          if((RFCOMM_CONNECTED == dlc->state) &&
             (rfcomm_hdl->server_channel == (dlc->dlci>>1U)) &&
             (rfcomm_hdl->dlci == dlc->dlci) &&
             (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
          {
              break;
          }
      }
      break;

    case RFCOMM_CLOSE:
      for(dlc = rfcomm_session[s_index].dlcs; dlc; dlc = dlc->_next)
      {
          /** Match state, server channel, dlci and bd_addr */
          if((RFCOMM_IN_DISCONNECT == dlc->state) &&
             (rfcomm_hdl->server_channel == (dlc->dlci>>1U)) &&
             (rfcomm_hdl->dlci == dlc->dlci) &&
             (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
          {
              break;
          }
      }
      break;

      default:
       dlc = NULL;
      break;
    }

    EDGEFAST_RFCOMM_UNLOCK;
    return dlc;
}

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
static struct bt_rfcomm_control * rfcomm_get_control(RFCOMM_HANDLE * rfcomm_hdl, uint8_t rfcomm_event)
{
    struct bt_rfcomm_control *control;
    struct bt_rfcomm_control *tmp = NULL;
    uint8_t                   control_type;

    if(RFCOMM_SEND_TEST == rfcomm_event)
    {
        control_type = BT_RFCOMM_TEST;
    }
    else if(RFCOMM_SEND_FC_ON == rfcomm_event)
    {
        control_type = BT_RFCOMM_FCON;
    }
    else if(RFCOMM_SEND_FC_OFF == rfcomm_event)
    {
        control_type = BT_RFCOMM_FCOFF;
    }
#if 0
    else if(RFCOMM_SEND_RPN == rfcomm_event)
    {
        control_type = BT_RFCOMM_RPN_REQUEST;
    }
    else if(RFCOMM_SEND_PN == rfcomm_event)
    {
        control_type = BT_RFCOMM_SEND_PN;
    }
#endif
    else if(RFCOMM_SEND_MSC == rfcomm_event)
    {
        control_type = BT_RFCOMM_SEND_MSC;
    }
    else if(RFCOMM_SEND_RLS == rfcomm_event)
    {
        control_type = BT_RFCOMM_SEND_RLS;
    }
    else
    {
        /** invalid event type */
        control_type = RFCOMM_NO_OPERATION;
    }

    switch(rfcomm_event)
    {
    case RFCOMM_SEND_TEST:
    case RFCOMM_SEND_FC_ON:
    case RFCOMM_SEND_FC_OFF:
      /** Scan the control list: Match bd_addr*/
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_list, control, tmp, node, struct bt_rfcomm_control)
      {
          if(NULL != control->dlc)
          {
              if((control_type == control->type) &&
                 (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
              {
                  break;
              }
          }
          else
          {
              if((control_type == control->type) &&
                 (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->conn)->val, BT_BD_ADDR_SIZE)))
              {
                  break;
              }
          }
      }
      break;

    case RFCOMM_SEND_RPN:
      /** Scan the control list */
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_list, control, tmp, node, struct bt_rfcomm_control)
      {
          if(NULL != control->dlc)
          {
              /** Match channel, dlci and bd_addr */
              if(((BT_RFCOMM_RPN_REQUEST == control->type) || (BT_RFCOMM_RPN_COMMAND == control->type)) &&
                 (rfcomm_hdl->server_channel == (control->dlc->dlci>>1U)) &&
                 (rfcomm_hdl->dlci == control->dlc->dlci) &&
                 (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
              {
                  break;
              }
          }
          else
          {
              /** Match channel, role and bd_addr */
              if(((BT_RFCOMM_RPN_REQUEST == control->type) || (BT_RFCOMM_RPN_COMMAND == control->type)) &&
                 (rfcomm_hdl->server_channel == control->channel) &&
                 (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->conn)->val, BT_BD_ADDR_SIZE)))
              {
                  break;
              }
          }
      }
      break;

    case RFCOMM_SEND_PN:
      /** Scan the control list: Match channel and bd_addr */
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_list, control, tmp, node, struct bt_rfcomm_control)
      {
          if(NULL != control->dlc)
          {
              /** Match channel, dlci and bd_addr */
              if((BT_RFCOMM_SEND_PN == control->type) &&
                 (rfcomm_hdl->server_channel == (control->dlc->dlci>>1U)) &&
                 (rfcomm_hdl->dlci == control->dlc->dlci) &&
                 (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
              {
                  break;
              }
          }
          else
          {
              /** Match channel and bd_addr */
              if((BT_RFCOMM_SEND_PN == control->type) &&
                 (rfcomm_hdl->server_channel == control->channel) &&
                 (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->conn)->val, BT_BD_ADDR_SIZE)))
              {
                  break;
              }
          }
      }
      break;

    case RFCOMM_SEND_MSC:
    case RFCOMM_SEND_RLS:
      /** Scan the control list: Match channel, dlci and bd_addr */
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_list, control, tmp, node, struct bt_rfcomm_control)
      {
          if((control_type == control->type) &&
             (rfcomm_hdl->server_channel == (control->dlc->dlci>>1U)) &&
             (rfcomm_hdl->dlci == control->dlc->dlci) &&
             (0 == memcmp(rfcomm_hdl->bd_addr, bt_conn_get_dst_br(control->dlc->session->conn)->val, BT_BD_ADDR_SIZE)))
          {
              break;
          }
      }
      break;

      default:
       control = NULL;
      break;
    }

    return control;
}

static struct bt_rfcomm_control_cb * rfcomm_get_control_callback(struct bt_rfcomm_control *control, uint8_t rfcomm_event)
{
    struct bt_rfcomm_control_cb *ctr_cb;
    struct bt_rfcomm_control_cb *tmp;

    /** if there's only one control cb, return it */
    if((NULL != rfcomm_control_cb_list.head) && (NULL == rfcomm_control_cb_list.head->next))
    {
        return CONTAINER_OF(rfcomm_control_cb_list.head, struct bt_rfcomm_control_cb, node);
    }

    switch(rfcomm_event)
    {
    case RFCOMM_SEND_TEST:
    case RFCOMM_SEND_FC_ON:
    case RFCOMM_SEND_FC_OFF:
      /** Scan the callback list: Match bd_addr */
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_cb_list, ctr_cb, tmp, node, struct bt_rfcomm_control_cb)
      {
          if(NULL != control->dlc)
          {
              if(0 == memcmp(ctr_cb->conn, control->dlc->session->conn, sizeof(struct bt_conn)))
              {
                  break;
              }
          }
          else
          {
              if(0 == memcmp(ctr_cb->conn, control->conn, sizeof(struct bt_conn)))
              {
                  break;
              }
          }
      }
      break;

    case RFCOMM_SEND_RPN:
    case RFCOMM_SEND_PN:
      /** Scan the callback list: Match channel and bd_addr */
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_cb_list, ctr_cb, tmp, node, struct bt_rfcomm_control_cb)
      {
          if(NULL != control->dlc)
          {
              if((ctr_cb->channel == (control->dlc->dlci>>1U)) &&
                 (ctr_cb->role == control->dlc->role) &&
                 (0 == memcmp(ctr_cb->conn, control->dlc->session->conn, sizeof(struct bt_conn))))
              {
                  break;
              }
          }
          else
          {
              if((ctr_cb->channel == control->channel) &&
                 (ctr_cb->role == control->dlc->role) &&
                 (0 == memcmp(ctr_cb->conn, control->conn, sizeof(struct bt_conn))))
              {
                  break;
              }
          }
      }
      break;

    case RFCOMM_SEND_MSC:
    case RFCOMM_SEND_RLS:
      /** Scan the callback list: Match channel, dlci and bd_addr */
      SYS_SLIST_FOR_EACH_CONTAINER_SAFE(&rfcomm_control_cb_list, ctr_cb, tmp, node, struct bt_rfcomm_control_cb)
      {
          if((ctr_cb->channel == (control->dlc->dlci>>1U)) &&
             (ctr_cb->role == control->dlc->role) &&
             (0 == memcmp(ctr_cb->conn, control->dlc->session->conn, sizeof(struct bt_conn))))
          {
              break;
          }
      }
      break;

      default:
       ctr_cb = NULL;
      break;
    }

    return ctr_cb;
}
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

static struct bt_rfcomm_dlc * rfcomm_dlcs_remove_dlc(struct bt_rfcomm_dlc *dlc, uint8_t index)
{
	struct bt_rfcomm_dlc *pre;
    struct bt_rfcomm_dlc *tmp;

    if((NULL == dlc->session) || (index >= CONFIG_BT_RFCOMM_SESSION_MAX_COUNT))
    {
		return NULL;
	}

    EDGEFAST_RFCOMM_LOCK;
    if(0 == memcmp(dlc->session, &rfcomm_session[index], sizeof(struct bt_rfcomm_session)))
    {
        /** If first node is the one to be removed */
        if(0 == memcmp(dlc, rfcomm_session[index].dlcs, sizeof(struct bt_rfcomm_dlc)))
        {
            tmp = rfcomm_session[index].dlcs;
            rfcomm_session[index].dlcs = rfcomm_session[index].dlcs->_next;
            if(NULL == rfcomm_session[index].dlcs)
            {
                rfcomm_init_session(index);
            }
            EDGEFAST_RFCOMM_UNLOCK;
            return tmp;
        }

        for (pre = rfcomm_session[index].dlcs, tmp = rfcomm_session[index].dlcs->_next; tmp; pre = tmp, tmp = tmp->_next)
        {
            if(0 == memcmp(tmp, dlc, sizeof(struct bt_rfcomm_dlc)))
            {
                pre->_next = tmp->_next;
                EDGEFAST_RFCOMM_UNLOCK;
                return tmp;
            }
        }
    }
    else
    {
        EDGEFAST_RFCOMM_UNLOCK;
        return NULL;
    }

    EDGEFAST_RFCOMM_UNLOCK;
	return NULL;
}


/**
 *  \fn rfcomm_callback
 *
 *  \brief To handle RFCOMM callback
 *
 *  \param Description
 *  This callback is called by RFCOMM to indicate RFCOMM events to the protocol.
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
 *  \return 0 on success otherwise an error code
 */
static API_RESULT rfcomm_callback(uint8_t event_type, RFCOMM_HANDLE * handle, uint16_t result, uint8_t * data, uint16_t datalen)
{
    uint8_t         deviceHandle, s_index;
    BT_DEVICE_ADDR  peerAddr;
    struct net_buf *buf;
    struct bt_conn *conn;

    struct bt_rfcomm_server     *server;
    struct bt_rfcomm_dlc        *dlc;
#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
    int             err;
    struct bt_rfcomm_control_cb *ctr_cb;
    struct bt_rfcomm_control     rfcomm_control;
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

    /** Checking the event type. In case of user initiated accept cancel return. */
    if ((RFCOMM_ACCEPT == event_type) &&
        (RFCOMM_ACCEPT_CANCELLED_BY_USER == result))
    {
        return API_SUCCESS;
    }

    /** Get session index from RFCOMM Handle */
    EDGEFAST_RFCOMM_LOCK;
    for(s_index = 0U; s_index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT; s_index++)
    {
        if((1U == rfcomm_session[s_index].state) &&
           (0 == memcmp(handle->bd_addr, bt_conn_get_dst_br(rfcomm_session[s_index].conn)->val, BT_BD_ADDR_SIZE)))
        {
            break;
        }
    }
    EDGEFAST_RFCOMM_UNLOCK;
    if((CONFIG_BT_RFCOMM_SESSION_MAX_COUNT == s_index) && (RFCOMM_ACCEPT != event_type))
    {
        BT_ERR("[RFCOMM] Not find the rfcomm session.\n");
        return API_FAILURE;
    }

    if(s_index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT)
    {
        /** Get dlc from RFCOMM Handle */
        dlc = rfcomm_get_dlc(handle, event_type, s_index);
        if ((NULL == dlc) && (RFCOMM_ACCEPT != event_type))
        {
            BT_ERR("[RFCOMM] FAILED to Find RFCOMM Handle\n");
            return API_FAILURE;
        }
    }

    switch (event_type)
    {
    case RFCOMM_OPEN:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Open Event. Result = 0x%04X\n", result);

        /** Handle RFCOMM Open */
        if (API_SUCCESS == result)
        {
            dlc->dlci  = handle->dlci;
            dlc->state = RFCOMM_CONNECTED;

            BT_INFO("[RFCOMM] Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
            BT_DEVICE_ADDR_ONLY_PRINT_STR (handle->bd_addr));

            /** Get application registered dlc which contains application connected, disconnected, recv callback */
            if ((NULL != dlc->ops) && (NULL != dlc->ops->connected))
            {
                /** Call application registered connected callback */
                dlc->ops->connected(dlc);
            }
        }
        else
        {
            /** remove dlc from dlcs */
            (void)rfcomm_dlcs_remove_dlc(dlc, s_index);

            /** Call application registered connected callback */
            if ((NULL != dlc->ops) && (NULL != dlc->ops->disconnected))
            {
                dlc->ops->disconnected(dlc);
            }
        }
        break;

    case RFCOMM_ACCEPT:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Accept Event. Result = 0x%04X\n",result);

        /** Find server */
        server = rfcomm_server_lookup_channel(handle->server_channel);
        if (NULL == server)
        {
            BT_ERR ("[RFCOMM] RFCOMM CB: Server channel %d isn't registed.", handle->server_channel);
            (void)BT_rfcomm_close(handle);
            return API_FAILURE;
        }

        /** Handle RFCOMM Accept */
        if (API_SUCCESS == result)
        {
            /** get conn handle with remote device */
            peerAddr.type = BT_ADDR_LE_PUBLIC;
            memcpy(&peerAddr.addr[0], handle->bd_addr, BT_BD_ADDR_SIZE);
            device_queue_search_br_edr_remote_addr(&deviceHandle, &peerAddr);

            conn = bt_conn_lookup_device_id(deviceHandle);
            if(NULL == conn)
            {
                BT_ERR("[RFCOMM] FAILED to get conn handle\n");
                (void)BT_rfcomm_close(handle);
                return API_FAILURE;
            }
            else
            {
                bt_conn_unref(conn);
            }

            /** Get application registered dlc which contains application connected, disconnected, recv callback */
            if((NULL != server->accept) && (server->accept(conn, &dlc) < 0))
            {
                BT_ERR("[RFCOMM] Incoming connection rejected");
                (void)BT_rfcomm_close(handle);
                return API_FAILURE;
            }
            else
            {
                if(CONFIG_BT_RFCOMM_SESSION_MAX_COUNT == s_index)
                {
                    s_index = rfcomm_get_free_session(conn);
                    if(CONFIG_BT_RFCOMM_SESSION_MAX_COUNT == s_index)
                    {
                        BT_ERR("[RFCOMM] No free rfcomm session entity.\n");
                        (void)BT_rfcomm_close(handle);
                        return API_FAILURE;
                    }
                    rfcomm_session[s_index].role = BT_RFCOMM_ROLE_ACCEPTOR;
                }

                /** Save dlci and peer addr to indentify a unique RFCOMM connection */
                dlc->session = &rfcomm_session[s_index];
                dlc->role    = BT_RFCOMM_ROLE_ACCEPTOR;
                dlc->dlci    = handle->dlci;
                dlc->state   = RFCOMM_CONNECTED;

                dlc->_next = rfcomm_session[s_index].dlcs;
                rfcomm_session[s_index].dlcs = dlc;

                /** Call application registered connected callback */
                if ((NULL != dlc->ops) && (NULL != dlc->ops->connected))
                {
                    dlc->ops->connected(dlc);
                }
            }
        }
        else
        {
            /** When connection isn't accepted successfully, start rfcomm accept again */
            (void)rfcomm_accept(server->channel);
        }
        break;

    case RFCOMM_CLOSE:
    case RFCOMM_RESET:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Close/Reset. Result = 0x%04X\n",result);

        if (((API_SUCCESS == result) && (RFCOMM_IN_DISCONNECT == dlc->state)) /** RFCOMM CLOSE event for initializing rfcomm disconnect */
            || ((0x2642 == result) && (RFCOMM_CONNECTED == dlc->state)))      /** RFCOMM CLOSE event for accepting rfcomm disconnect, 0x0042-RFCOMM_DLCI_CLOSED_BY_DISC_COMMAND */
        {
            (void)rfcomm_dlcs_remove_dlc(dlc, s_index);

            if (BT_RFCOMM_ROLE_ACCEPTOR == dlc->role)   /** rfcomm server*/
            {
                /** When rfcomm server is disconnected, wait for rfocmm accept again */
                (void)rfcomm_accept((dlc->dlci)>>1U);
            }

            /** Call application registered disconnect callback */
            if ((NULL != dlc->ops) && (NULL != dlc->ops->disconnected))
            {
                dlc->ops->disconnected(dlc);
            }
        }
        break;

    case RFCOMM_WRITE:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Write. Result = 0x%04X\n",result);
        if ((NULL != dlc->ops) && (NULL != dlc->ops->sent))
        {
            buf = bt_rfcomm_create_pdu(&rfcomm_pool);
            net_buf_add_mem(buf, data, strlen((char const *)data));
            /** Call application registered sent callback */
            dlc->ops->sent(dlc, buf);
            net_buf_unref(buf);
        }
        break;

    case RFCOMM_READ:
        BT_INFO ("[RFCOMM] RFCOMM CB: Received RFCOMM Read. Result = 0x%04X, Data Length = %u\n",result, datalen);

        if ((NULL != dlc->ops) && (NULL != dlc->ops->recv))
        {
            if(strlen((char const *)data) > 0)
            {
                buf = bt_rfcomm_create_pdu(&rfcomm_pool);
                net_buf_add_mem(buf, data, strlen((char const *)data));
                /** Call application registered recv callback */
                dlc->ops->recv(dlc, buf);
                net_buf_unref(buf);
            }
        }
        break;

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
    case RFCOMM_RECVD_RLS:
          rfcomm_control.dlc = dlc;
          rfcomm_control.type = BT_RFCOMM_RECVD_RLS;
          rfcomm_control.control_data.rls.dlci = ((RFCOMM_RLS *)data)->dlci;
          rfcomm_control.control_data.rls.line_status = ((RFCOMM_RLS *)data)->line_status;

          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_CONTROL_RESPONSE_ERROR;

          /** Get control cb from RFCOMM Handle */
          ctr_cb = rfcomm_get_control_callback(&rfcomm_control, event_type);
          if (NULL == ctr_cb)
          {
              BT_ERR("[RFCOMM] [RFCOMM] FAILED to Find RFCOMM Handle\n");
              return API_FAILURE;
          }
          if(NULL != ctr_cb->cb)
          {
              ctr_cb->cb(&rfcomm_control, err);
          }
      break;

    case RFCOMM_RECVD_MSC:
          rfcomm_control.dlc  = dlc;
          rfcomm_control.type = BT_RFCOMM_RECVD_MSC;
          memcpy(&rfcomm_control.control_data.msc, ((RFCOMM_MSC *)data), sizeof(struct bt_rfcomm_msc));
          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_CONTROL_RESPONSE_ERROR;

          /** Get control cb from RFCOMM Handle */
          ctr_cb = rfcomm_get_control_callback(&rfcomm_control, event_type);
          if (NULL == ctr_cb)
          {
              BT_ERR("[RFCOMM] [RFCOMM] FAILED to Find RFCOMM Handle\n");
              return API_FAILURE;
          }
          if(NULL != ctr_cb->cb)
          {
              ctr_cb->cb(&rfcomm_control, err);
          } 
      break;
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

    default:
      /** invalid event */
      break;
    }

    return API_SUCCESS;
}

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
static API_RESULT rfcomm_control_callback(uint8_t event_type, RFCOMM_HANDLE * handle, uint16_t result, uint8_t * data, uint16_t datalen)
{
    int                          err;
    struct bt_rfcomm_control    *control;
    struct bt_rfcomm_control_cb *ctr_cb;

    /** Get control from RFCOMM Handle */
    control = rfcomm_get_control(handle, event_type);
    if (NULL == control)
    {
        BT_ERR("[RFCOMM] [RFCOMM] FAILED to Find RFCOMM Handle\n");
        return API_FAILURE;
    }

    /** Get control cb from RFCOMM Handle */
    ctr_cb = rfcomm_get_control_callback(control, event_type);
    if (NULL == ctr_cb)
    {
        BT_ERR("[RFCOMM] [RFCOMM] FAILED to Find RFCOMM Handle\n");
        return API_FAILURE;
    }

    switch (event_type)
    {
    case RFCOMM_SEND_TEST:
          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_TEST_RESPONSE_MISMATCH;
      break;

    case RFCOMM_SEND_FC_ON:
    case RFCOMM_SEND_FC_OFF:
          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_CONTROL_RESPONSE_ERROR;
      break;

    case RFCOMM_SEND_RPN:
          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_CONTROL_RESPONSE_ERROR;
          memcpy(&control->control_data.rpn, &(((RFCOMM_RPN *)data)->dlci), sizeof(struct bt_rfcomm_rpn));
      break;

    case RFCOMM_SEND_PN:
        switch(result)
        {
        case API_SUCCESS:
          err = BT_RFCOMM_CONTROL_RESPONSE_SUCCESS;
          break;

        case RFCOMM_PN_FRAME_SIZE_MISMATCH:
          err = BT_RFCOMM_PN_RESPONSE_FRAME_SIZE_MISMATCH;
          break;

        case RFCOMM_PN_CBFC_MISMATCH:
          err = BT_RFCOMM_PN_RESPONSE_CBFC_MISMATCH;
          break;

        default:
          err = BT_RFCOMM_CONTROL_RESPONSE_ERROR;
          break;
        }

        control->control_data.pn.dlci = handle->dlci;
      break;

    case RFCOMM_SEND_RLS:
          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_CONTROL_RESPONSE_ERROR;
          control->control_data.rls.dlci = handle->dlci;
      break;

    case RFCOMM_SEND_MSC:
          err = (API_SUCCESS == result) ? BT_RFCOMM_CONTROL_RESPONSE_SUCCESS: BT_RFCOMM_CONTROL_RESPONSE_ERROR;
          control->control_data.msc.dlci = handle->dlci;
      break;

    default:
      err = 0;
      break;
    }

    if(NULL != ctr_cb->cb)
    {
        ctr_cb->cb(control, err);
    }

	if (true == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_find_and_remove(&rfcomm_control_list, &control->node);
	}

    return API_SUCCESS;
}
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

static void set_rfcomm_handle_with_dlc(struct bt_rfcomm_dlc *dlc, RFCOMM_HANDLE *hdl)
{
    hdl->server_channel = (dlc->dlci)>>1U;
    hdl->dlci           = dlc->dlci;
    (void)memcpy(hdl->bd_addr, bt_conn_get_dst_br(dlc->session->conn)->val, BT_BD_ADDR_SIZE);
}

static void rfcomm_init(void)
{
    uint8_t index;

    if (0U == initialized)
    {
        /** create rfcomm mutex */
        if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)rfcomm_lock_mutex))
        {
            BT_ERR("[RFCOMM] RFCOMM OSA_MutexCreate() failed!\n");
        }

        /** init rfcomm session entity */
        for (index = 0; index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT; index++)
        {
            rfcomm_init_session(index);
        }

        initialized = 1U;
    }
}

/** init rfcomm session entity */
static void rfcomm_init_session(uint8_t index)
{
    EDGEFAST_RFCOMM_LOCK;
    rfcomm_session[index].state = 0U;
    rfcomm_session[index].role  = BT_RFCOMM_ROLE_ACCEPTOR;
    rfcomm_session[index].dlcs  = NULL;
    rfcomm_session[index].conn  = NULL;
    EDGEFAST_RFCOMM_UNLOCK;
}

/** check whether rfcomm session entity exists */
static uint8_t rfcomm_is_session_exist(struct bt_conn *conn)
{
    uint8_t index;

    EDGEFAST_RFCOMM_LOCK;
    for(index = 0U; index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT; index++)
    {
        if((1U == rfcomm_session[index].state) &&
           (conn == rfcomm_session[index].conn))
        {
            break;
        }
    }

    EDGEFAST_RFCOMM_UNLOCK;
    return index;
}

/** get free rfcomm session entity */
static uint8_t rfcomm_get_free_session(struct bt_conn *conn)
{
    uint8_t index;

    EDGEFAST_RFCOMM_LOCK;
    for(index = 0U; index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT; index++)
    {
        if(0U == rfcomm_session[index].state)
        {
            rfcomm_session[index].state = 1U;
            rfcomm_session[index].conn  = conn;
            break;
        }
    }

    EDGEFAST_RFCOMM_UNLOCK;
    return index;
}

int bt_rfcomm_server_register(struct bt_rfcomm_server *server)
{
    int err;

    rfcomm_init();

    if ((server->channel < RFCOMM_CHANNEL_START) ||
        (server->channel > RFCOMM_CHANNEL_END) ||
        (NULL == server->accept))
    {
        BT_ERR("[RFCOMM] Invalid parameter.\n");
        return -EINVAL;
    }

    /** Check if given channel is already in use */
    if (NULL != rfcomm_server_lookup_channel(server->channel))
    {
        BT_INFO("[RFCOMM] Channel %d is already registered.\n", server->channel);
        return -EADDRINUSE;
    }

    err = rfcomm_accept(server->channel);
    if(0 == err )
    {
        EDGEFAST_RFCOMM_LOCK;
        server->_next = servers;
        servers = server;
        EDGEFAST_RFCOMM_UNLOCK;
    }

    return err;
}

/** @brief Connect RFCOMM channel
 *
 *  Connect RFCOMM dlc by channel, once the connection is completed dlc
 *  connected() callback will be called. If the connection is rejected
 *  disconnected() callback is called instead.
 *
 *  @param conn    Connection object.
 *  @param dlc     Dlc object, dlc->ops filed should be provided.
 *  @param channel Server channel to connect to.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 */
int bt_rfcomm_dlc_connect(struct bt_conn *conn, struct bt_rfcomm_dlc *dlc, uint8_t channel)
{
    int                   err;
    API_RESULT            ret;
    uint8_t               index;
    RFCOMM_HANDLE         rfcomm_hdl;
    struct bt_rfcomm_dlc *dlc_hdl;

    rfcomm_init();

	if ((NULL == dlc) || ((channel < RFCOMM_CHANNEL_START || channel > RFCOMM_CHANNEL_END)))
    {
        BT_ERR("[RFCOMM] Not valid parameter.\n");
		return -EINVAL;
	}

	if (!conn || conn->state != BT_CONN_CONNECTED)
    {
        BT_ERR("[RFCOMM] Not valid state.\n");
		return -ENOTCONN;
	}

    index = rfcomm_is_session_exist(conn);
    if(CONFIG_BT_RFCOMM_SESSION_MAX_COUNT == index)
    {
        /** get free rfcomm session */
        index = rfcomm_get_free_session(conn);
        if(CONFIG_BT_RFCOMM_SESSION_MAX_COUNT == index)
        {
            BT_ERR("[RFCOMM] RFCOMM dlc connect on server %d is created failed, no free rfcomm session entity.\n",rfcomm_hdl.server_channel);
            return -ENOBUFS;
        }
        rfcomm_session[index].role = BT_RFCOMM_ROLE_INITIATOR;
    }
    else
    {
        /** check whether client channel is already connected */
        for(dlc_hdl = rfcomm_session[index].dlcs; dlc_hdl; dlc_hdl = dlc_hdl->_next)
        {
            if((BT_RFCOMM_ROLE_INITIATOR == dlc_hdl->role) &&
               (channel == ((dlc_hdl->dlci)>>1)) &&
               (0 == memcmp(conn, dlc->session->conn, sizeof(struct bt_conn))))
            {
                BT_ERR("[RFCOMM] Client channel %d is already connected.\n", channel);
                return -EINVAL;
            }
        }
    }

    /** Prepare RFCOMM Handle for Open */
    RFCOMM_RESET_HANDLE (&rfcomm_hdl);
    RFCOMM_SET_HANDLE
    (
      &rfcomm_hdl,
      bt_conn_get_dst_br(conn)->val,
      channel,
      rfcomm_callback
    );

    /** Call RFCOMM Open */
    ret = BT_rfcomm_open(rfcomm_hdl.bd_addr, rfcomm_hdl.server_channel, &rfcomm_hdl);
    if (API_SUCCESS == ret)
    {
        BT_INFO("[RFCOMM] RFCOMM dlc connect on server %d is created successfully, waitting for RFCOMM_OPEN Event.\n",rfcomm_hdl.server_channel);
        dlc->session = &rfcomm_session[index];
        dlc->role    = BT_RFCOMM_ROLE_INITIATOR;
        dlc->dlci    = channel;
        dlc->state   = RFCOMM_IN_CONNECT;

        dlc->_next = rfcomm_session[index].dlcs;
        rfcomm_session[index].dlcs = dlc;
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM dlc connect on server %d is created failed, reason = 0x%04X.\n",rfcomm_hdl.server_channel, ret);
        rfcomm_init_session(index);
    }

    err = rfcomm_convert_return_value(ret);

    return err;
}

int bt_rfcomm_dlc_send(struct bt_rfcomm_dlc *dlc, struct net_buf *buf)
{
    int           err;
    API_RESULT    ret;
    RFCOMM_HANDLE rfcomm_hdl;

	if ((!dlc) || (dlc->state != RFCOMM_CONNECTED))
    {
		return -ENOTCONN;
	}

    RFCOMM_RESET_HANDLE (&rfcomm_hdl);
    set_rfcomm_handle_with_dlc(dlc, &rfcomm_hdl);
    rfcomm_hdl.notify_cb = rfcomm_callback;

    ret = BT_rfcomm_write(&rfcomm_hdl, buf->b.data, buf->b.len, NULL);
    if (API_SUCCESS == ret)
    {
        BT_INFO("[RFCOMM] RFCOMM Write data is created successfully, waitting for RFCOMM Write Event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM Write %d data is created failed, reason is 0x%04X.\n",ret);
    }

    err = rfcomm_convert_return_value(ret);

    return err;
}

int bt_rfcomm_dlc_disconnect(struct bt_rfcomm_dlc *dlc)
{
    int           err;
    API_RESULT    ret;
    RFCOMM_HANDLE rfcomm_hdl;

	if ((!dlc) || (dlc->state != RFCOMM_CONNECTED))
    {
		return -ENOTCONN;
	}

    RFCOMM_RESET_HANDLE (&rfcomm_hdl);
    set_rfcomm_handle_with_dlc(dlc, &rfcomm_hdl);
    rfcomm_hdl.notify_cb = rfcomm_callback;

    /** Call RFCOMM close API to initiate disconnection at the RFCOMM level */
    ret = BT_rfcomm_close(&rfcomm_hdl);
    if (API_SUCCESS == ret)
    {
        /** Set RFCOMM Entity State to 'In Disconnect' */
        BT_INFO("[RFCOMM] RFCOMM dlc disconnect is created successfully, waitting for RFCOMM Close event.\n");
        dlc->state = RFCOMM_IN_DISCONNECT;
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM dlc disconnect is created failed, reason is 0x%04X.\n",ret);
    }

    err = rfcomm_convert_return_value(ret);

    return err;
}

struct net_buf *bt_rfcomm_create_pdu(struct net_buf_pool *pool)
{
    /** Length in RFCOMM header can be 2 bytes depending on length of user
    * data
    */
    return bt_conn_create_pdu(pool,
        sizeof(struct bt_l2cap_hdr) +
        sizeof(struct bt_rfcomm_hdr) + 1);
}

#if (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0))
/* register rfcomm control callback */
int bt_rfcomm_register_control_callback(struct bt_rfcomm_control_cb *ctr_cb)
{
    if((NULL == ctr_cb) ||(NULL == ctr_cb->cb))
    {
        return -EINVAL;
    }

	if (false == sys_slist_find(&rfcomm_control_cb_list, &ctr_cb->node))
	{
		sys_slist_prepend(&rfcomm_control_cb_list, &ctr_cb->node);
	}
    else
    {
        return -EINVAL;
    }

    return 0;
}

/** unregister rfcomm control callback */
int bt_rfcomm_unregister_control_callback(struct bt_rfcomm_control_cb *cb)
{
    if(NULL == cb)
    {
        return -EINVAL;;
    }

	if (true == sys_slist_find(&rfcomm_control_cb_list, &cb->node))
	{
		sys_slist_find_and_remove(&rfcomm_control_cb_list, &cb->node);
	}
    else
    {
		return -EINVAL;
    }

    return 0;
#if 0
	/** If first node is the one to be removed */
    if(0 == memcmp(control_cbs, cb, sizeof(struct bt_rfcomm_control_cb)))
    {
        tmp = control_cbs;
        control_cbs = control_cbs->_next;
        return 0;
    }

	for (pre = control_cbs, tmp = control_cbs->_next; tmp; pre = tmp, tmp = tmp->_next)
    {
        if(0 == memcmp(tmp, cb, sizeof(struct bt_rfcomm_control_cb)))
        {
            pre->_next = tmp->_next;
            return 0;
        }
	}
#endif
}

/** @brief bt_rfcomm_send_test
 *
 *  This API enables the User to ask local RFCOMM entity to transmit a Test Command
 *  with the Test Data over the RFCOMM Session with a remote RFCOMM entity.
 *  The Session must be available with the remote RFCOMM entity.The remote RFCOMM
 *  entity should respond to the Test Command with a Test Response containing
 *  the same Test Data as requested by the user.
 *
 *  @param control Control handle.
 *         When dlc is connected, control->dlc, control->type and control->control_data.test should be provided.
 *         When dlc isn't connected, control->conn, control->type and control->control_data.test should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 *
 */
static int bt_rfcomm_send_test(struct bt_rfcomm_control *control)
{
    API_RESULT    ret;
    RFCOMM_TEST   test_data;
    RFCOMM_HANDLE rfcomm_hdl;

    if((NULL == control) || ((NULL == control->dlc) && (NULL == control->conn)))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);

    if(NULL != control->dlc)
    {
        /** set server channel as 0, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->dlc->session->conn)->val, 0U, rfcomm_control_callback);
    }
    else
    {
        /** set server channel as 0, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->conn)->val, 0U, rfcomm_control_callback);
    }

	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    control->type = BT_RFCOMM_TEST;

    test_data.datalen = control->control_data.test->len;

    test_data.data = control->control_data.test->data;

    ret = BT_rfcomm_send_test(&rfcomm_hdl, &test_data);
    if(API_SUCCESS == ret)
    {
        BT_INFO("[RFCOMM] RFCOMM dlc send session test command successfully, waitting for RFCOMM_SEND_TEST event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM dlc send session test command failed, reason is 0x%04X.\n",ret);
    }

    return rfcomm_convert_return_value(ret);
}

/** @brief bt_rfcomm_send_flow_control
 *
 *  This API enables the User to set the Flow Control OFF/ON for the RFCOMM Session
 *  with a remote RFCOMM Entity. The RFCOMM Session Flow Control affects data
 *  transfer over all the open DLCs for that particular Session.
 *
 *  @param control Control handle.
 *         When dlc is connected, control->dlc, control->type and control->control_data.fc_on should be provided.
 *         When dlc isn't connected, control->conn, control->type and control->control_data.fc_on should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 *
 *        Credit Based Flow Control is used by host stack defaultly, so this API can't work.
 *
 */
static int bt_rfcomm_send_flow_control(struct bt_rfcomm_control *control)
{
    API_RESULT    ret;
    uint8_t       operation;
    RFCOMM_HANDLE rfcomm_hdl;

    if((NULL == control) || ((NULL == control->dlc) && (NULL == control->conn)))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);

    if(NULL != control->dlc)
    {
        /** set server channel as 0, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->dlc->session->conn)->val, 0U, rfcomm_control_callback);
    }
    else
    {
        /** set server channel as 0, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->conn)->val, 0U, rfcomm_control_callback);
    }

	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    operation = (0U == control->control_data.fc_on)? BT_RFCOMM_FCOFF : BT_RFCOMM_FCON;
    control->type = operation;

    ret = BT_rfcomm_send_fc(&rfcomm_hdl, operation);
    if(API_SUCCESS == ret)
    {
        BT_INFO("[RFCOMM] RFCOMM send flow control command successfully, waitting for RFCOMM_SEND_FC_ON/OFF event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM send flow control command failed, reason is 0x%04X.\n",ret);
    }

    return rfcomm_convert_return_value(ret);
}

/** @brief bt_rfcomm_get_rpn
 *
 *  This API enables the User to send RFCOMM Remote Port Negotiation request
 *  with the remote RFCOMM Entity to retrieve the Remote RFCOMM entity's port
 *  settings. It can not only retrieve the port setting of an existing dlc which is
 *  created by bt_rfcomm_dlc_connect() or bt_rfcomm_server_register(), but also retrieve
 *  the port setting of a dlc which will be created.
 *
 *  @param control Control handle.
 *         When dlc is connected, control->dlc, and control->type should be provided.
 *         When dlc isn't connected, control->conn, control->channel, control->role and control->type should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 *
 */
static int bt_rfcomm_get_rpn(struct bt_rfcomm_control *control)
{
    API_RESULT    retval;
    uint8_t       index;
    RFCOMM_RPN    rpn_request;
    RFCOMM_HANDLE rfcomm_hdl;

    if((NULL == control) || ((NULL == control->dlc) && (NULL == control->conn)))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);
    if(NULL != control->dlc)
    {
        set_rfcomm_handle_with_dlc(control->dlc, &rfcomm_hdl);
        rfcomm_hdl.notify_cb = rfcomm_control_callback;
    }
    else
    {
        /** set server channel, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->conn)->val, control->channel, rfcomm_control_callback);

        /** set dlci */
        index = rfcomm_is_session_exist(control->conn);
        if(index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT)
        {
            if(BT_RFCOMM_ROLE_ACCEPTOR == control->role)
            {
                rfcomm_hdl.dlci = (((control->channel) << 1) | ((rfcomm_session[index].role)));
            }
            else
            {
                rfcomm_hdl.dlci = (((control->channel) << 1) | (!(rfcomm_session[index].role)));
            }
        }
    }

    /** add control into control list */
	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    memset(&rpn_request, 0, sizeof(RFCOMM_RPN));
    rpn_request.request = RFCOMM_RPN_REQUEST_OPTION;
    retval = BT_rfcomm_send_rpn(&rfcomm_hdl, &rpn_request);
    if (API_SUCCESS == retval)
    {
        BT_INFO("[RFCOMM] RFCOMM Send RPN Request created successfully, waitting for RFCOMM_RPN_REQUEST_OPTION Event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM Send RPN Request created failed, reason is 0x%04X.\n",retval);
    }

    return rfcomm_convert_return_value(retval);
}

/** @brief bt_rfcomm_set_rpn
 *
 *  This API enables the User to send RFCOMM Remote Port Negotiation command
 *  with the remote RFCOMM Entity to set the Remote RFCOMM entity's port
 *  settings. It can not only sets the port setting of an existing dlc which is
 *  created by bt_rfcomm_dlc_connect() or bt_rfcomm_server_register(), but also sets
 *  the port setting of a dlc which will be created.
 *
 *  @param control Control handle.
 *         When dlc is connected, control->dlc, control->type and control->control_data.rpn should be provided.
 *         When dlc isn't connected, control->conn, control->channel, control->role, control->type and
 *         control->control_data.rpn should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 */
static int bt_rfcomm_set_rpn(struct bt_rfcomm_control *control)
{
    API_RESULT    retval;
    RFCOMM_RPN    rpn_command;
    RFCOMM_HANDLE rfcomm_hdl;

    if((NULL == control) || ((NULL == control->dlc) && (NULL == control->conn)))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);

    if(NULL != control->dlc)
    {
        set_rfcomm_handle_with_dlc(control->dlc, &rfcomm_hdl);
        rfcomm_hdl.notify_cb = rfcomm_control_callback;
    }
    else
    {
        /** set server channel, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->conn)->val, control->channel, rfcomm_control_callback);
    }

	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    control->type = BT_RFCOMM_RPN_COMMAND;

    rpn_command.request = RFCOMM_RPN_COMMAND_OPTION;

    memcpy(&rpn_command.dlci, &control->control_data.rpn, sizeof(struct bt_rfcomm_rpn));

    retval = BT_rfcomm_send_rpn(&rfcomm_hdl, &rpn_command);
    if (API_SUCCESS == retval)
    {
        BT_INFO("[RFCOMM] RFCOMM Send RPN Command created successfully, waitting for RFCOMM_RPN_COMMAND_OPTION Event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM Send RPN Command created failed, reason is 0x%04X.\n",retval);
    }

    return rfcomm_convert_return_value(retval);
}

/** @brief bt_rfcomm_send_pn
 *
 *  This API enables the User to initiate RFCOMM Parameter Negotiation with the remote
 *  RFCOMM Entity. The only useful parameter in the Parameter Negotiation(struct bt_rfcomm_pn)
 *  block is mtu. It can only sets the maximum frame size of an existing dlc which is
 *  created by bt_rfcomm_dlc_connect() or bt_rfcomm_server_register(), but also sets the
 *  maximum frame size of a dlc which will be created.
 *
 *  @param control Control handle.
 *         When dlc is connected, control->dlc, control->type and control->control_data.pn should be provided.
 *         When dlc isn't connected, control->conn, control->channel, control->role, control->type and
 *         control->control_data.pn should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 */
static int bt_rfcomm_send_pn(struct bt_rfcomm_control *control)
{
    API_RESULT    retval;
    RFCOMM_PN     pn_req;
    RFCOMM_HANDLE rfcomm_hdl;

    if((NULL == control) || ((NULL == control->dlc) && (NULL == control->conn)))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);

    if(NULL != control->dlc)
    {
        set_rfcomm_handle_with_dlc(control->dlc, &rfcomm_hdl);
        rfcomm_hdl.notify_cb = rfcomm_control_callback;
    }
    else
    {
        /** set server channel, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->conn)->val, control->channel, rfcomm_control_callback);
    }

	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    control->type = BT_RFCOMM_SEND_PN;

    pn_req.frame_size = control->control_data.pn.mtu;
    pn_req.priority = control->control_data.pn.priority;
    pn_req.convergence_layer = control->control_data.pn.flow_ctrl;
    pn_req.window_size = control->control_data.pn.credits;

    retval = BT_rfcomm_send_pn(&rfcomm_hdl, &pn_req);

    if (API_SUCCESS == retval)
    {
        BT_INFO("[RFCOMM] RFCOMM Send PN Command created successfully, waitting for RFCOMM_SEND_PN Event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM Send PN Command created failed, reason is 0x%04X.\n",retval);
    }

    return rfcomm_convert_return_value(retval);
}

/** @brief bt_rfcomm_get_local_pn
 *
 *  This API enables the User to get local device's RFCOMM Parameter Negotiation parameter.
 *  The parameters that can be gotten in the Parameter Negotiation(struct bt_rfcomm_pn)
 *  block is mtu, dlci and priority. It can not only be called over an existing dlc which is
 *  created by bt_rfcomm_dlc_connect() or bt_rfcomm_server_register(), but also be called over
 *  a dlc which will be created.
 *
 *  @param control Control handle.
 *         When dlc is connected, control->dlc, and control->type should be provided.
 *         When dlc isn't connected, control->conn, control->channel, control->role, control->type should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 */
static int bt_rfcomm_get_local_pn(struct bt_rfcomm_control *control)
{
    API_RESULT    retval;
    uint8_t       index;
    RFCOMM_PN     pn_req;
    RFCOMM_HANDLE rfcomm_hdl;

    if((NULL == control) || ((NULL == control->dlc) && (NULL == control->conn)))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);

    if(NULL != control->dlc)
    {
        set_rfcomm_handle_with_dlc(control->dlc, &rfcomm_hdl);
        rfcomm_hdl.notify_cb = NULL;
    }
    else
    {
        /** set server channel, peer device address and callback */
        RFCOMM_SET_HANDLE(&rfcomm_hdl, bt_conn_get_dst_br(control->conn)->val, control->channel, NULL);
        index = rfcomm_is_session_exist(control->conn);
        if(index < CONFIG_BT_RFCOMM_SESSION_MAX_COUNT)
        {
            if(BT_RFCOMM_ROLE_ACCEPTOR == control->role)
            {
                rfcomm_hdl.dlci = (((control->channel) << 1) | ((rfcomm_session[index].role)));
            }
            else
            {
                rfcomm_hdl.dlci = (((control->channel) << 1) | (!(rfcomm_session[index].role)));
            }
        }
    }

    retval = BT_rfcomm_get_local_pn(&rfcomm_hdl, &pn_req);
    control->control_data.pn.dlci     = pn_req.dlci;
    control->control_data.pn.priority = pn_req.priority;
    control->control_data.pn.mtu      = pn_req.frame_size;

#if 0
    pn_req.frame_size = control->control_data.pn.mtu;
    pn_req.priority = control->control_data.pn.priority;
    pn_req.convergence_layer = control->control_data.pn.flow_ctrl;
    pn_req.window_size = control->control_data.pn.credits;
#endif

    if (API_SUCCESS == retval)
    {
        BT_INFO("[RFCOMM] RFCOMM get local PN successfully.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM get local PN failed, reason is 0x%04X.\n",retval);
    }

    return rfcomm_convert_return_value(retval);
}

/** @brief bt_rfcomm_send_rls
 *
 *  This API enables the User to explicitly send local Port Line Status
 *  to the peer RFCOMM entity.
 *
 *  @param control Control handle
 *         control->dlc, control->type and control->control_data.rls should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 */
static int bt_rfcomm_send_rls(struct bt_rfcomm_control *control)
{
    API_RESULT    retval;
    RFCOMM_RLS    rfcomm_rls;
    RFCOMM_HANDLE rfcomm_hdl;

    /** rls must be sent when dlc is connected */
    if((NULL == control) || (NULL == control->dlc) || (RFCOMM_CONNECTED != control->dlc->state))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);
    set_rfcomm_handle_with_dlc(control->dlc, &rfcomm_hdl);
    rfcomm_hdl.notify_cb = rfcomm_control_callback;

	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    rfcomm_rls.line_status = control->control_data.rls.line_status;
    retval = BT_rfcomm_send_rls(&rfcomm_hdl, &rfcomm_rls);
    if (API_SUCCESS == retval)
    {
        BT_INFO("[RFCOMM] RFCOMM Send RLS Command created successfully, waitting for RFCOMM_SEND_RLS Event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM Send RLS Command created failed, reason is 0x%04X.\n",retval);
    }

    return rfcomm_convert_return_value(retval);
}

/** @brief bt_rfcomm_send_msc
 *
 *  This API enables the User to transmit Modem Status Commands to its peer.
 *
 *  @param control Control handle
 *         control->dlc, control->type and control->control_data.msc should be provided.
 *
 *  @return 0 in case of success or negative value in case of error.
 *
 *  Note: When this command is sent, sender will notify appl the completition
 *        by callback registed by bt_rfcomm_register_control_callback().
 */
static int bt_rfcomm_send_msc(struct bt_rfcomm_control *control)
{
    API_RESULT    retval;
    RFCOMM_MSC    rfcomm_msc;
    RFCOMM_HANDLE rfcomm_hdl;

    /** send msc must be sent when dlc is connected */
    if((NULL == control) || (NULL == control->dlc) || (RFCOMM_CONNECTED != control->dlc->state))
    {
        return -EINVAL;
    }

    RFCOMM_RESET_HANDLE(&rfcomm_hdl);
    set_rfcomm_handle_with_dlc(control->dlc, &rfcomm_hdl);
    rfcomm_hdl.notify_cb = rfcomm_control_callback;

	if (false == sys_slist_find(&rfcomm_control_list, &control->node))
	{
		sys_slist_prepend(&rfcomm_control_list, &control->node);
	}

    memcpy(&rfcomm_msc, &control->control_data.msc, sizeof(struct bt_rfcomm_msc));

    retval = BT_rfcomm_send_msc(&rfcomm_hdl, &rfcomm_msc);
    if (API_SUCCESS == retval)
    {
        BT_INFO("[RFCOMM] RFCOMM set MSC successfully, waitting for RFCOMM_SEND_MSC event.\n");
    }
    else
    {
        BT_ERR("[RFCOMM] RFCOMM set MSC failed, reason is 0x%04X.\n",retval);
    }

    return rfcomm_convert_return_value(retval);
}

int bt_rfcomm_send_control(struct bt_rfcomm_control *control)
{
    int err;

    rfcomm_init();

    if(NULL == control)
    {
        return -EINVAL;
    }

    if(BT_RFCOMM_TEST == control->type)
    {
        err = bt_rfcomm_send_test(control);
    }
    else if((BT_RFCOMM_FCON == control->type) || (BT_RFCOMM_FCOFF == control->type))
    {
        err = bt_rfcomm_send_flow_control(control);
    }
    else if(BT_RFCOMM_RPN_REQUEST == control->type)
    {
        err = bt_rfcomm_get_rpn(control);
    }
    else if(BT_RFCOMM_RPN_COMMAND == control->type)
    {
        err = bt_rfcomm_set_rpn(control);
    }
    else if(BT_RFCOMM_GET_LOCAL_PN == control->type)
    {
        err = bt_rfcomm_get_local_pn(control);
    }
    else if(BT_RFCOMM_SEND_PN == control->type)
    {
        err = bt_rfcomm_send_pn(control);
    }
    else if(BT_RFCOMM_SEND_RLS == control->type)
    {
        err = bt_rfcomm_send_rls(control);
    }
    else if(BT_RFCOMM_SEND_MSC == control->type)
    {
        err = bt_rfcomm_send_msc(control);
    }
    else
    {
        /** invalid control type */
        err = -EINVAL;;
    }

    return err;
}
#endif /** (defined(CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD) && (CONFIG_BT_RFCOMM_ENABLE_CONTROL_CMD > 0)) */

void bt_rfcomm_init(void)
{
#if 1
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
#endif  /** (defined(CONFIG_BT_RFCOMM) && ((CONFIG_BT_RFCOMM) > 0U)) */
#endif  /** (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)) */
