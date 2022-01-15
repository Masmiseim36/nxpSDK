/** zephyr_spp.c - SPP handling */

/*
 * Copyright (c) 2021 Intel Corporation
 * Copyright 2020-2021 NXP
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <errno/errno.h>

#include "BT_sdp_api.h"
#include "BT_hci_api.h"
#include "db_gen.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_SPP)
#define LOG_MODULE_NAME bt_spp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#if (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U))
#if (defined(CONFIG_BT_SPP) && ((CONFIG_BT_SPP) > 0U))
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/addr.h>
#include "bt_pal_conn_internal.h"
#include "BT_spp_api.h"
#include "bluetooth/spp.h"
#include "bluetooth/rfcomm.h"
#include "bt_pal_rfcomm_internal.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/** SPP States */
typedef enum _spp_state
{
    SPP_IDLE,
    SPP_ALLOCATED,
    SPP_WAIT_4_CONNECT,
    SPP_IN_CONNECT,
    SPP_CONNECTED,
    SPP_IN_DISCONNECT,
}spp_state_t;

/*! @brief SPP Generic Data Element Value. */
struct bt_spp
{
    /** Channel of SPP Entity */
    uint8_t channel;

    /** Role of SPP Entity */
    bt_spp_role_t role;

    /** State of SPP Entity */
    spp_state_t state;

    /** The BR connection handle */
    struct bt_conn *conn;

    /** The application callback function */
    bt_spp_callback *cb;

    /** The rfcomm dlc */
    struct bt_rfcomm_dlc dlc;
};

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
/** SPP Control States */
typedef enum _spp_control_state
{
    SPP_CONTROL_IDLE,
    SPP_CONTROL_ALLOCATED,
    SPP_CONTROL_CONFIG,
}spp_control_state_t;

/*! @brief SPP Control Generic Data Element Value. */
struct bt_spp_control_entity
{
    /** State of SPP Control Entity */
    spp_control_state_t state;

    /** Handle of SPP Entity */
    struct bt_spp *spp;

    /** The rfcomm dlc */
    struct bt_rfcomm_control rfcomm_ctr;
};
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/*! @brief SPP Server Generic Data Element Value. */
struct bt_spp_server
{
    /** State of SPP Server Entity */
    uint8_t used;

    /** The application callback function */
    bt_spp_callback *cb;

    /** The rfcomm server */
    struct bt_rfcomm_server server;
};

#define SPP_DATA_MTU 48

/** To initialize a spp_entity[] */
#define SPP_INIT_ENTITY(index)                                           \
{                                                                        \
    EDGEFAST_SPP_LOCK;                                                   \
    spp_entity[index].channel    = 0U;                                   \
    spp_entity[index].role       = BT_SPP_ROLE_SERVER;                   \
    spp_entity[index].state      = SPP_IDLE;                             \
    spp_entity[index].conn       = NULL;                                 \
    spp_entity[index].cb         = NULL;                                 \
    memset(&spp_entity[index].dlc, 0U, sizeof(struct bt_rfcomm_dlc));    \
    EDGEFAST_SPP_UNLOCK;                                                 \
}

/** To initialize a spp_server[] */
#define SPP_INIT_SERVER(index)                                                    \
{                                                                                 \
    EDGEFAST_SPP_LOCK;                                                            \
    spp_server[index].used  = 0U;                                                 \
    spp_server[index].cb    = NULL;                                               \
    (void)memset(&spp_server[index].server, 0U, sizeof(struct bt_rfcomm_server)); \
    EDGEFAST_SPP_UNLOCK;                                                          \
}

/*******************************************************************************
 * Variables
 ******************************************************************************/
/** Initialization flag*/
static uint8_t initialized = 0U;

/** SPP Entities */
static struct bt_spp spp_entity[CONFIG_BT_SPP_MAX_CONN];

/** SPP Server Entities */
static struct bt_spp_server spp_server[CONFIG_BT_SPP_SERVER_MAX_COUNT];

/** SPP Mutex */
static OSA_MUTEX_HANDLE_DEFINE(spp_lock_mutex);
#define EDGEFAST_SPP_LOCK OSA_MutexLock((osa_mutex_handle_t)spp_lock_mutex, osaWaitForever_c)
#define EDGEFAST_SPP_UNLOCK OSA_MutexUnlock((osa_mutex_handle_t)spp_lock_mutex)

NET_BUF_POOL_FIXED_DEFINE(spp_pool, 1, SPP_DATA_MTU, NULL);

/** SPP SDP handle */
static SDP_HANDLE sdp_handle;

/** SPP SDP discover callback */
static discover_cb_t *discover_callbacks;

/** Remote SPP server channel */
static uint16_t remote_server_channel[CONFIG_BT_SPP_MAX_CONN];

/** Remote SPP server count */
static uint8_t remote_server_channel_count;

/** SPP SDP discover callback */
static void spp_sdp_cb
     (
         uint8_t    command,
         uint8_t  * data,
         uint16_t   length,
         uint16_t   status
     );

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
/* SPP rfcomm control params */
static struct bt_spp_control_entity spp_control[CONFIG_BT_SPP_MAX_CONN];
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/** Init spp entity and spp server */
static void spp_initialize(void);

/** Get free spp_entity[] */
static uint8_t spp_get_free_entity(void);

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
/* Get free spp_control[] */
static uint8_t spp_get_free_control_entity(void);
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/** SPP SDP Callback */
static void spp_sdp_cb
     (
         uint8_t    command,
         uint8_t  * data,
         uint16_t   length,
         uint16_t   status
     );

/** SPP Server Accept */
static int spp_server_accepted(struct bt_conn *conn, struct bt_rfcomm_dlc **dlc);

/** SPP DLC recv Callback */
static void spp_received(struct bt_rfcomm_dlc *dlc, struct net_buf *buf);

/** SPP DLC sent Callback */
static void spp_sent(struct bt_rfcomm_dlc *dlc, struct net_buf *buf);

/** SPP DLC Connectioned Callback */
static void spp_connected(struct bt_rfcomm_dlc *dlc);

/** SPP DLC Disconnectioned Callback */
static void spp_disconnected(struct bt_rfcomm_dlc *dlc);

static struct bt_rfcomm_dlc_ops spp_ops =
{
    .recv         = spp_received,
    .sent         = spp_sent,
    .connected    = spp_connected,
    .disconnected = spp_disconnected,
};

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
static void init_spp_control(struct bt_spp_control_entity *spp_control);

static void spp_rfcomm_control_callback(struct bt_rfcomm_control *control, int err);

static struct bt_rfcomm_control_cb rfcomm_control_cb =
{
    .cb = spp_rfcomm_control_callback,
};
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/*******************************************************************************
 * Code
 ******************************************************************************/
#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
static void spp_rfcomm_control_callback(struct bt_rfcomm_control *control, int err)
{
    int                           error;
    struct bt_spp_control_entity *spp_control_hdl;
    struct bt_spp_control         spp_ctr;
    struct bt_spp                *spp_handle;

    error = (BT_RFCOMM_CONTROL_RESPONSE_SUCCESS == err) ? 0: -EIO;

    memset(&spp_ctr, 0U, sizeof(struct bt_spp_control));

    /** handle rls receive event, it has no control entity */
    if(BT_RFCOMM_RECVD_RLS == control->type)
    {
        spp_handle   = CONTAINER_OF(control->dlc, struct bt_spp, dlc);

        spp_ctr.spp  = spp_handle;
        spp_ctr.type = SPP_REC_LINE_STATUS;

        spp_ctr.control_data.rls.dlci = control->control_data.rls.dlci;
        if(RFCOMM_RLS_NO_ERROR == control->control_data.rls.line_status)
        {
            spp_ctr.control_data.rls.rls = BT_SPP_RLS_NO_ERROR;
        }
        else if(RFCOMM_RLS_OVERRUN_ERROR == control->control_data.rls.line_status)
        {
            spp_ctr.control_data.rls.rls = BT_SPP_RLS_OVERRUN_ERROR;
        }
        else if(RFCOMM_RLS_PARITY_ERROR == control->control_data.rls.line_status)
        {
            spp_ctr.control_data.rls.rls = BT_SPP_RLS_PARITY_ERROR;
        }
        else if(RFCOMM_RLS_FRAMEING_ERROR == control->control_data.rls.line_status)
        {
            spp_ctr.control_data.rls.rls = BT_SPP_RLS_FRAMEING_ERROR;
        }
        else
        {
            /** invalid rfcomm rls */
        }

        if(NULL != spp_handle->cb->control)
        {
            spp_handle->cb->control(&spp_ctr, error);
        }

        return;
    }

    /** handle msc receive event, it has no control entity */
    if(BT_RFCOMM_RECVD_MSC == control->type)
    {
        spp_handle   = CONTAINER_OF(control->dlc, struct bt_spp, dlc);

        spp_ctr.spp  = spp_handle;
        spp_ctr.type = SPP_REC_MSC;

        memcpy(&spp_ctr.control_data.msc, &control->control_data.msc, sizeof(struct bt_spp_msc));
        spp_ctr.control_data.msc.dlci = control->control_data.msc.dlci;

        if(NULL != spp_handle->cb->control)
        {
            spp_handle->cb->control(&spp_ctr, error);
        }

        return;
    }

    /** find spp control handle */
    spp_control_hdl = CONTAINER_OF(control, struct bt_spp_control_entity, rfcomm_ctr);

    /** spp handle isn't connected */
    if(SPP_CONTROL_CONFIG == spp_control_hdl->state)
    {
        spp_ctr.conn    = control->conn;
        spp_ctr.channel = control->channel;
        spp_ctr.role    = (BT_RFCOMM_ROLE_ACCEPTOR == control->role) ? BT_SPP_ROLE_SERVER: BT_SPP_ROLE_CLIENT;
    }
    else
    {
        spp_ctr.spp = spp_control_hdl->spp;
    }

    switch(control->type)
    {
    case BT_RFCOMM_RPN_REQUEST:
      spp_ctr.type = SPP_REQUEST_PORT_SETTING;
      /** get port setting */
      memcpy(&spp_ctr.control_data.port, &spp_control_hdl->rfcomm_ctr.control_data.rpn, sizeof(struct bt_spp_port));
      spp_ctr.control_data.port.dlci = spp_control_hdl->rfcomm_ctr.control_data.rpn.dlci;
      break;

    case BT_RFCOMM_RPN_COMMAND:
      spp_ctr.type = SPP_NEGOTIATE_PORT_SETTING;
      /** get sent port setting */
      memcpy(&spp_ctr.control_data.port, &spp_control_hdl->rfcomm_ctr.control_data.rpn, sizeof(struct bt_spp_port));
      spp_ctr.control_data.port.dlci = spp_control_hdl->rfcomm_ctr.control_data.rpn.dlci;
      break;

    case BT_RFCOMM_SEND_PN:
      spp_ctr.type = SPP_SEND_PN;
      /** get sent mtu */
      memcpy(&spp_ctr.control_data.pn, &control->control_data.pn, sizeof(struct bt_spp_pn));
      spp_ctr.control_data.pn.mtu = control->control_data.pn.mtu;
      spp_ctr.control_data.pn.dlci = control->control_data.pn.dlci;
      break;

    case BT_RFCOMM_SEND_RLS:
      spp_ctr.type = SPP_SEND_LINE_STATUS;
      spp_ctr.control_data.rls.dlci = control->control_data.rls.dlci;
      if(RFCOMM_RLS_NO_ERROR == control->control_data.rls.line_status)
      {
          spp_ctr.control_data.rls.rls = BT_SPP_RLS_NO_ERROR;
      }
      else if(RFCOMM_RLS_OVERRUN_ERROR == control->control_data.rls.line_status)
      {
          spp_ctr.control_data.rls.rls = BT_SPP_RLS_OVERRUN_ERROR;
      }
      else if(RFCOMM_RLS_PARITY_ERROR == control->control_data.rls.line_status)
      {
          spp_ctr.control_data.rls.rls = BT_SPP_RLS_PARITY_ERROR;
      }
      else if(RFCOMM_RLS_FRAMEING_ERROR == control->control_data.rls.line_status)
      {
          spp_ctr.control_data.rls.rls = BT_SPP_RLS_FRAMEING_ERROR;
      }
      else
      {
          /** invalid rfcomm rls */
      }
      break;

    case BT_RFCOMM_SEND_MSC:
      spp_ctr.type = SPP_SEND_MSC;
      memcpy(&spp_ctr.control_data.msc, &control->control_data.msc, sizeof(struct bt_spp_msc));
      spp_ctr.control_data.msc.dlci = control->control_data.msc.dlci;
      break;

    default:
      break;
    }

    if((NULL != spp_control_hdl->spp->cb) && (NULL != spp_control_hdl->spp->cb->control))
    {
        spp_control_hdl->spp->cb->control(&spp_ctr, error);
    }

    /** if spp isn't connected, free spp entity */
    if(SPP_CONTROL_CONFIG == spp_control_hdl->state)
    {
        spp_control_hdl->spp->state = SPP_IDLE;
        spp_control_hdl->spp->cb    = NULL;
    }

    /** free spp control entity */
    init_spp_control(spp_control_hdl);
}

static void init_spp_control(struct bt_spp_control_entity *spp_control)
{
    spp_control->state  = SPP_CONTROL_IDLE;
    spp_control->spp    = NULL;
    (void)memset(&spp_control->rfcomm_ctr, 0U, sizeof(struct bt_rfcomm_control));
}

#if 0
static void spp_rfcomm_pn(struct bt_rfcomm_control *control, struct bt_rfcomm_pn *pn, int err)
{
    struct bt_spp_control * spp_control;

    /** find spp_control */
    spp_control = CONTAINER_OF(control, struct bt_spp_control, control);

    if((BT_RFCOMM_SEND_PN == spp_control->control.type) && (NULL != spp_control->spp->cb->mtu_set))
    {
        spp_control->spp->cb->mtu_set(spp_control->spp, pn->mtu, err);
    }

    init_spp_control(spp_control);

    if(SPP_IN_CONFIG == spp_control->spp->state)
    {
        spp_control->spp->channel = 0U;
        spp_control->spp->role    = BT_SPP_ROLE_SERVER;
        spp_control->spp->state   = SPP_IDLE;
        spp_control->spp->conn    = NULL;
        spp_control->spp->cb      = NULL;
        memset(&spp_control->spp->dlc, 0U, sizeof(struct bt_rfcomm_dlc));
    }
}

static void spp_rfcomm_msc(struct bt_rfcomm_control *control, struct bt_rfcomm_msc *msc, int err)
{
    struct bt_spp_control * spp_control;
    struct bt_spp_msc       spp_msc;

    memcpy(&spp_msc, &msc->fc, sizeof(struct bt_spp_msc));

    /** find spp_control */
    spp_control = CONTAINER_OF(control, struct bt_spp_control, control);

    if((BT_RFCOMM_SEND_MSC == spp_control->control.type) && (NULL != spp_control->spp->cb->msc_sent))
    {
        spp_control->spp->cb->msc_sent(spp_control->spp, &spp_msc, err);
    }
    else if((BT_RFCOMM_RECVD_MSC == spp_control->control.type) && (NULL != spp_control->spp->cb->msc_rev))
    {
        spp_control->spp->cb->msc_rev(spp_control->spp, &spp_msc, err);
    }
    else
    {
        /** invalid event */
    }

    init_spp_control(spp_control);
}
#endif
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/** Initialize SPP entities. */
static void spp_initialize(void)
{
    uint8_t index;

    if (0U == initialized)
    {
        /** create spp Mutex */
        if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)spp_lock_mutex))
        {
            BT_ERR("[SPP] OSA_MutexCreate() failed!\n");
        }

        /** init spp entity */
        for (index = 0; index < CONFIG_BT_SPP_MAX_CONN; index++)
        {
            SPP_INIT_ENTITY(index);
        }

        /** init spp server entity */
        for (index = 0; index < CONFIG_BT_SPP_SERVER_MAX_COUNT; index++)
        {
            SPP_INIT_SERVER(index);
        }

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
        /* init spp control entity */
        for (index = 0; index < CONFIG_BT_SPP_MAX_CONN; index++)
        {
            init_spp_control(&spp_control[index]);
        }

        /** register rfcomm control callback */
        bt_rfcomm_register_control_callback(&rfcomm_control_cb);
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

        initialized = 1U;
    }
}

/** Get free spp entity. */
static uint8_t spp_get_free_entity(void)
{
    uint8_t index;

    EDGEFAST_SPP_LOCK;
    for (index = 0U; index < CONFIG_BT_SPP_MAX_CONN; index++)
    {
        if (SPP_IDLE == spp_entity[index].state)
        {
            spp_entity[index].state = SPP_ALLOCATED;
            break;
        }
    }

    EDGEFAST_SPP_UNLOCK;
    return index;
}

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
/* Get free spp control entity. */
static uint8_t spp_get_free_control_entity(void)
{
    uint8_t index;

    EDGEFAST_SPP_LOCK;
    for (index = 0U; index < CONFIG_BT_SPP_MAX_CONN; index++)
    {
        if (SPP_CONTROL_IDLE == spp_control[index].state)
        {
            spp_control[index].state = SPP_CONTROL_ALLOCATED;
            break;
        }
    }

    EDGEFAST_SPP_UNLOCK;
    return index;
}
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

/** SDP Callback for SPP */
static void spp_sdp_cb
     (
         uint8_t    command,
         uint8_t  * data,
         uint16_t   length,
         uint16_t   status
     )
{
    S_UUID         uuid;
    uint16_t       num_uuids;
    uint16_t       attrib_id[] = {PROTOCOL_DESC_LIST};
    uint16_t       num_attribute_ids = ARRAY_SIZE(attrib_id);
    uint8_t      * attribute_data = NULL;
    uint8_t      * sdp_query_result;
    API_RESULT     err = API_FAILURE;

    uint16_t       len_attrib_data;

    static uint8_t sdp_close_due2error;

    BT_IGNORE_UNUSED_PARAM(length);
    BT_IGNORE_UNUSED_PARAM(sdp_close_due2error);

    /** Set SPP related UUID variables */
    uuid.uuid_type = UUID_16;
    uuid.uuid_union.uuid_16 = SERIALPORT_UUID;
    num_uuids = 0x01;

    /** It can come only while creating connection. verify */
    if (API_SUCCESS != status)
    {
        BT_INFO("[SPP] STATUS : %X \n", status);
        BT_INFO("[SPP] COMMAND : %X \n", command);

        /** Implies that SDP open was successful but service search failed */
        if (command == SDP_ServiceSearchAttributeResponse)
        {
            sdp_close_due2error = 2;

            BT_INFO("[SPP] SDP CB : Closing SDP \n");

            BT_sdp_close(&sdp_handle);

            return;
        }
    }

    switch(command)
    {
        case SDP_Open : /** SDP open callback */
        {
            attribute_data = (uint8_t *)BT_alloc_mem(100);

            if (NULL == attribute_data)
            {
                BT_ERR("[SPP] SDP CB : Alloc mem failed, closing SDP \n");
                /** Error in SDP operation */
                sdp_close_due2error = 2;

                /** Close SDP */
                BT_sdp_close(&sdp_handle);
                return;
            }

            len_attrib_data = 100;

            /** Do Service Search Request */
            err = BT_sdp_servicesearchattributerequest
                     (
                         &sdp_handle,
                         &uuid,
                         num_uuids,
                         attrib_id,
                         num_attribute_ids,
                         NULL,
                         0x00,
                         attribute_data,
                         &len_attrib_data
                     );

            if (API_SUCCESS != err)
            {
                BT_ERR("[SPP] SDP CB:BT_sdp_servicesearchattributerequest Failed\n");

                BT_free_mem(attribute_data);

                BT_ERR("[SPP] Closing SDP Connection\n");

                /** Error in SDP operation */
                sdp_close_due2error = 2;

                /** Close SDP */
                BT_sdp_close(&sdp_handle);

                return;
            }
            /** SDP operation to continue */
            sdp_close_due2error = 1;

            break;
        }

        case SDP_Close: /** SDP Close callback */
            sdp_close_due2error = 0;

            BT_INFO("[SPP] SDP CB : SDP PROCEDURE DONE... \n");

            break;

        /** Service Search Attribute callback */
        case SDP_ServiceSearchAttributeResponse:
            remote_server_channel_count = 1U;
            sdp_query_result = data;

            while (NULL != sdp_query_result)
            {
                sdp_query_result = BT_sdp_get_Pointer_to_next_AttributeList
                                    (
                                        data,
                                        remote_server_channel_count
                                    );
                if (NULL == sdp_query_result)
                {
                    break;
                }

                err = BT_sdp_get_channel_number
                         (
                             sdp_query_result,
                             (uint8_t *)&remote_server_channel[remote_server_channel_count - 1U]
                         );

                if (API_SUCCESS == err)
                {
                    remote_server_channel_count++;
                }
            }

            remote_server_channel_count -= 1u;
            if (remote_server_channel_count > 0u)
            {
                if (NULL != discover_callbacks)
                {
                    discover_callbacks->cb(discover_callbacks->conn, remote_server_channel_count, remote_server_channel);
                }
            }
            else
            {
                sdp_close_due2error = 2;
            }

            /** Free allocated memory for attribute data */
            BT_free_mem(data);

            /** Close SDP */
            BT_sdp_close(&sdp_handle);

           break;


        case SDP_ErrorResponse:

            BT_INFO("[SPP] SDP_ErrorResponse \n");

            /** Free allocated memory for attribute data */
            BT_free_mem(data);

            sdp_close_due2error = 2;

            BT_sdp_close(&sdp_handle);

            break;

        default: /** Invalid: Nothing to do */
            break;

    }/** switch */

    return;
}

static int spp_server_accepted(struct bt_conn *conn, struct bt_rfcomm_dlc **dlc)
{
    int     err = 0;
    uint8_t index;

    index = spp_get_free_entity();
    if (CONFIG_BT_SPP_MAX_CONN == index)
    {
        BT_ERR("[SPP]: No free spp_entity[].\n");
        err = -ENOMEM;
    }
    else
    {
        spp_entity[index].role = BT_SPP_ROLE_SERVER;
        spp_entity[index].conn = conn;

        spp_entity[index].dlc.ops = &spp_ops;

        *dlc = &spp_entity[index].dlc;
    }

    return err;
}

/** SPP DLC recv Callback */
static void spp_received(struct bt_rfcomm_dlc *dlc, struct net_buf *buf)
{
    struct bt_spp * spp_handle = CONTAINER_OF(dlc, struct bt_spp, dlc);

    if(NULL != spp_handle->cb->data_received)
    {
        spp_handle->cb->data_received(spp_handle, buf->data, buf->len);
    }
}

/** SPP DLC sent Callback */
static void spp_sent(struct bt_rfcomm_dlc *dlc, struct net_buf *buf)
{
    struct bt_spp * spp_handle = CONTAINER_OF(dlc, struct bt_spp, dlc);

    if((NULL != spp_handle->cb) && (NULL != spp_handle->cb->data_sent))
    {
        spp_handle->cb->data_sent(spp_handle, buf->data, buf->len);
    }
}

/** SPP DLC Connectioned Callback */
static void spp_connected(struct bt_rfcomm_dlc *dlc)
{
    uint8_t        index;
    struct bt_spp *spp_handle;

    spp_handle = CONTAINER_OF(dlc, struct bt_spp, dlc);

    EDGEFAST_SPP_LOCK;
    spp_handle->state = SPP_CONNECTED;
    EDGEFAST_SPP_UNLOCK;

    if(BT_SPP_ROLE_SERVER == spp_handle->role)
    {
        /** find spp_server[] */
        EDGEFAST_SPP_LOCK;
        for (index = 0; index < CONFIG_BT_SPP_SERVER_MAX_COUNT; index++)
        {
            if ((1U == spp_server[index].used) &&
                ((dlc->dlci)>>1 == spp_server[index].server.channel))
            {
                break;
            }
        }
        EDGEFAST_SPP_UNLOCK;
        if(CONFIG_BT_SPP_SERVER_MAX_COUNT == index)
        {
            BT_ERR ("[SPP]: Not find registered spp server channel %d\n", (dlc->dlci)>>1);
            return;
        }

        EDGEFAST_SPP_LOCK;
        spp_handle->channel = spp_server[index].server.channel;
        spp_handle->cb      = spp_server[index].cb;
        EDGEFAST_SPP_UNLOCK;
    }

    if((NULL != spp_handle->cb) && (NULL != spp_handle->cb->connected))
    {
        spp_handle->cb->connected(spp_handle, 0);
    }

    return;
}

/** SPP DLC Disconnectioned Callback */
static void spp_disconnected(struct bt_rfcomm_dlc *dlc)
{
    struct bt_spp *spp_handle = CONTAINER_OF(dlc, struct bt_spp, dlc);

    if((NULL != spp_handle->cb) && (NULL != spp_handle->cb->disconnected))
    {
        spp_handle->cb->disconnected(spp_handle, 0);

        spp_handle->channel = 0U;
        spp_handle->role    = BT_SPP_ROLE_SERVER;
        spp_handle->state   = SPP_IDLE;
        spp_handle->conn    = NULL;
        spp_handle->cb      = NULL;
        memset(&spp_handle->dlc, 0U, sizeof(struct bt_rfcomm_dlc));
    }
}

int bt_spp_discover(struct bt_conn *conn, discover_cb_t *cb)
{
     int        err;
     API_RESULT ret;

    if ((NULL == conn) || (NULL == cb))
    {
        err = -EINVAL;
    }
    else
    {
        cb->conn           = conn;
        cb->next           = discover_callbacks;
        discover_callbacks = cb;

        /** Set the SDP Handle */
        SDP_SET_HANDLE(sdp_handle, conn->br.dst.val, spp_sdp_cb);

        /** Call SDP open */
        ret = BT_sdp_open(&sdp_handle);
        if (API_SUCCESS != ret)
        {
            BT_ERR ("SPP SDP open failed, reason = 0x%04X \n", ret);
        }
        else
        {
            BT_INFO("[SPP] SPP SDP open successfully, wait for SDP_Open event.\n");
        }

        err = (int)ret;
    }

    return err;
}

int bt_spp_server_register(uint8_t channel, bt_spp_callback *cb)
{
    int     err;
    uint8_t index;

    /** init spp */
    spp_initialize();

    if (NULL == cb)
    {
        BT_ERR("[SPP] SPP callback shouldn't be NULL.\n");
        err = -EINVAL;
    }
    else
    {
        /** Check whether channel is registered */
        EDGEFAST_SPP_LOCK;
        for (index = 0; index < CONFIG_BT_SPP_SERVER_MAX_COUNT; ++index)
        {
            if ((true == spp_server[index].used) &&
                (channel == spp_server[index].server.channel))
            {
                break;
            }
        }
        EDGEFAST_SPP_UNLOCK;

        if (CONFIG_BT_SPP_SERVER_MAX_COUNT != index)
        {
            BT_ERR("[SPP] Channel %d is already registered.\n", channel);
            err = -EINVAL;
        }
        else
        {
            /** Get free spp_server[] */
            EDGEFAST_SPP_LOCK;
            for (index = 0; index < CONFIG_BT_SPP_SERVER_MAX_COUNT; index++)
            {
                if (0U == spp_server[index].used)
                {
                    spp_server[index].used = 1U;
                    break;
                }
            }
            EDGEFAST_SPP_UNLOCK;

            if (CONFIG_BT_SPP_SERVER_MAX_COUNT == index)
            {
                BT_ERR("[SPP] No free spp_server[] entitiy.\n");
                err = -ENOMEM;
            }
            else
            {
                EDGEFAST_SPP_LOCK;
                spp_server[index].cb             = cb;
                spp_server[index].server.channel = channel;
                spp_server[index].server.accept  = &spp_server_accepted;
                EDGEFAST_SPP_UNLOCK;

                err = bt_rfcomm_server_register(&spp_server[index].server);
                if (0 != err)
                {
                    BT_ERR("[SPP] SPP register server channel %d failed, reason = 0x%04X\n", spp_server[index].server.channel, err);
                    SPP_INIT_SERVER(index);
                }
                else
                {
                    BT_INFO("[SPP] SPP register server %d successfully, waiting for spp_server_accepted.\n", spp_server[index].server.channel);
                }
            }
        }
    }

    return err;
}

int bt_spp_client_connect(struct bt_conn *conn, uint8_t channel, bt_spp_callback *cb, struct bt_spp **spp)
{
    int     err;
    uint8_t index;

    spp_initialize();

    if ((NULL == conn) || (NULL == cb) || (NULL == spp))
    {
        BT_ERR("[SPP] Invalid parameter.\n");
        err = -EINVAL;
    }
    else
    {
        /** Check whether channel is connected */
        EDGEFAST_SPP_LOCK;
        for (index = 0; index < CONFIG_BT_SPP_MAX_CONN; ++index)
        {
            if ((SPP_IDLE != spp_entity[index].state) &&
                (channel == spp_entity[index].channel) &&
                (conn == spp_entity[index].conn) &&
                (BT_SPP_ROLE_CLIENT == spp_entity[index].role))
            {
                break;
            }
        }
        EDGEFAST_SPP_UNLOCK;
        if (CONFIG_BT_SPP_MAX_CONN != index)
        {
            BT_ERR("[SPP] SPP client %d exists.\n", channel);
            return -EINVAL;
        }

        /** get free spp_entity[] */
        index = spp_get_free_entity();
        if (CONFIG_BT_SPP_MAX_CONN == index)
        {
            BT_ERR("[SPP] No free spp_entity[].\n");
            err = -ENOMEM;
        }
        else
        {
            EDGEFAST_SPP_LOCK;
            if (0U == channel)
            {
                spp_entity[index].channel = BT_RFCOMM_CHAN_SPP;
            }
            else
            {
                spp_entity[index].channel = channel;
            }
            spp_entity[index].role    = BT_SPP_ROLE_CLIENT;
            spp_entity[index].conn    = conn;
            spp_entity[index].cb      = cb;
            spp_entity[index].dlc.ops = &spp_ops;
            EDGEFAST_SPP_UNLOCK;

            err = bt_rfcomm_dlc_connect(spp_entity[index].conn, &spp_entity[index].dlc, spp_entity[index].channel);
            if (0 == err)
            {
                BT_INFO("[SPP] SPP connect server %d successfully, waiting for spp_connected callback.\n", spp_entity[index].channel);
                spp_entity[index].state = SPP_IN_CONNECT;
                *spp = &spp_entity[index];
            }
            else
            {
                BT_ERR("[SPP] SPP connect server %d failed, reason = 0x%04X\n", spp_entity[index].channel, err);
                SPP_INIT_ENTITY(index);
                *spp = NULL;
            }
        }
    }

    return err;
}

int bt_spp_data_send(struct bt_spp *spp, uint8_t *data, uint16_t len)
{
    int             err;
    struct net_buf *buf;

    if ((NULL == spp) || (NULL == data) || (0U == len))
    {
        err = -EINVAL;
    }
    else
    {
        if (SPP_CONNECTED == spp->state)
        {
            buf = bt_rfcomm_create_pdu(&spp_pool);
            net_buf_add_mem(buf, data, len);

            err = bt_rfcomm_dlc_send(&spp->dlc, buf);
            if (0 != err)
            {
                BT_ERR("[SPP] SPP send data failed, reason = 0x%04X\n",err);
            }
            else
            {
                BT_INFO("[SPP] SPP send data successfully, waiting for SPP_SEND_CNF event.");
            }
             net_buf_unref(buf);
        }
        else
        {
            err = -EINVAL;
            BT_ERR("[SPP] SPP send data failed, current spp isn't in connection.\n");
        }
    }

    return err;
}

int bt_spp_disconnect(struct bt_spp *spp)
{
    int err;

    if(NULL == spp)
    {
        err = -EINVAL;
    }
    else
    {
        if (SPP_CONNECTED == spp->state)
        {
            err = bt_rfcomm_dlc_disconnect(&spp->dlc);

            if (0 != err)
            {
                BT_ERR("[SPP] SPP disconnect failed, reason = 0x%04X\n",err);
            }
            else
            {
                spp->state = SPP_IN_DISCONNECT;
                BT_INFO("[SPP] SPP disconnect successfully, waiting for SPP_DISCONNECT_CNF event.");
            }
        }
        else
        {
            err = -EINVAL;
            BT_ERR("[SPP] SPP disconnect failed, current spp isn't in connection.\n");
        }
    }

    return err;
}

int bt_spp_get_channel(struct bt_spp *spp, uint8_t *channel)
{
    int err = 0U;

    if(NULL == spp)
    {
        err = -EFAULT;
    }
    else
    {
        *channel = spp->channel;
    }

    return err;
}

int bt_spp_get_role(struct bt_spp *spp, bt_spp_role_t *role)
{
    int err = 0U;

    if(NULL == spp)
    {
        err = -EFAULT;
    }
    else
    {
        *role = spp->role;
    }

    return err;
}

int bt_spp_get_conn(struct bt_spp *spp, struct bt_conn **conn)
{
    int err = 0U;

    if((NULL == spp) || ((SPP_CONNECTED != spp->state) && (SPP_IN_DISCONNECT != spp->state) && (SPP_IN_CONNECT != spp->state)))
    {
        err = -EFAULT;
    }
    else
    {
        *conn = spp->conn;
    }

    return err;
}

#if (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0))
static uint8_t initialize_spp_control_entity(uint8_t ctl_hdl, struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb)
{
    uint8_t index;

    EDGEFAST_SPP_LOCK;
    /** find the spp handle */
    for(index = 0U; index < CONFIG_BT_SPP_MAX_CONN; index++)
    {
        if((SPP_IDLE != spp_entity[index].state) &&
           (conn == spp_entity[index].conn) &&
           (channel == spp_entity[index].channel) &&
           (role == spp_entity[index].role))
        {
            break;
        }
    }
    EDGEFAST_SPP_UNLOCK;

    if(CONFIG_BT_SPP_MAX_CONN != index) /** the spp is connected */
    {
        spp_control[ctl_hdl].rfcomm_ctr.dlc = &spp_entity[index].dlc;
    }
    else /** the spp isn't connected, allocate a spp_entity[] */
    {
        index = spp_get_free_entity();

        if(CONFIG_BT_SPP_MAX_CONN != index)
        {
            EDGEFAST_SPP_LOCK;
            spp_entity[index].cb                    = cb;                 /** set cb */
            spp_control[ctl_hdl].state              = SPP_CONTROL_CONFIG; /** indicate spp_entity is just used to control */
            spp_control[ctl_hdl].rfcomm_ctr.conn    = conn;
            spp_control[ctl_hdl].rfcomm_ctr.channel = channel;
            spp_control[ctl_hdl].rfcomm_ctr.role    = (BT_SPP_ROLE_SERVER == role) ? BT_RFCOMM_ROLE_ACCEPTOR: BT_RFCOMM_ROLE_INITIATOR;
            EDGEFAST_SPP_UNLOCK;
        }
    }

    return index;
}

int bt_spp_request_port_setting(struct bt_conn *conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb)
{
    int     err;
    uint8_t index;
    uint8_t ctl_hdl;

    if((NULL == conn) || (NULL == cb))
    {
        return -EINVAL;
    }

    spp_initialize();

    /** allocate a spp_control[] entity */
    ctl_hdl = spp_get_free_control_entity();
    if(CONFIG_BT_SPP_MAX_CONN == ctl_hdl)
    {
        BT_ERR("[SPP] No available spp control entity.\n");
        return -ENOMEM;
    }

    index = initialize_spp_control_entity(ctl_hdl, conn, channel, role, cb);
    if(CONFIG_BT_SPP_MAX_CONN == index)
    {
        BT_ERR("[SPP] No free spp entity.\n");
        return -ENOMEM;
    }

    spp_control[ctl_hdl].spp             = &spp_entity[index];
    spp_control[ctl_hdl].rfcomm_ctr.type = BT_RFCOMM_RPN_REQUEST;

    err = bt_rfcomm_send_control(&spp_control[ctl_hdl].rfcomm_ctr);
    if (API_SUCCESS == err)
    {
        BT_INFO("[SPP] SPP get remote port setting successfully, waitting for BT_RFCOMM_RPN_REQUEST event.\n");
    }
    else
    {
        BT_ERR("[SPP] SPP get remote port setting failed, reason is 0x%04X.\n",err);
        if(SPP_CONTROL_CONFIG == spp_control[ctl_hdl].state)
        {
            spp_entity[index].state = SPP_IDLE;
            spp_entity[index].cb    = NULL;
        }
        init_spp_control(&spp_control[ctl_hdl]);
    }

    return err;
}

int bt_spp_negotiate_port_setting(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb, struct bt_spp_port *port)
{
    int     err;
    uint8_t index;
    uint8_t ctl_hdl;

    if((NULL == conn) || (NULL == cb) || (NULL == port))
    {
        return -EINVAL;
    }

    spp_initialize();

    /** allocate a spp_control[] entity */
    ctl_hdl = spp_get_free_control_entity();
    if(CONFIG_BT_SPP_MAX_CONN == ctl_hdl)
    {
        BT_ERR("[SPP] No available spp control entity.\n");
        return -ENOMEM;
    }

    index = initialize_spp_control_entity(ctl_hdl, conn, channel, role, cb);
    if(CONFIG_BT_SPP_MAX_CONN == index)
    {
        BT_ERR("[SPP] No available spp entity.\n");
        return -ENOMEM;
    }

    spp_control[ctl_hdl].spp             = &spp_entity[index];
    spp_control[ctl_hdl].rfcomm_ctr.type = BT_RFCOMM_RPN_COMMAND;

    /** covert spp port setting to rfcomm rpn */
    memcpy(&spp_control[ctl_hdl].rfcomm_ctr.control_data.rpn, port, sizeof(struct bt_spp_port));
    err = bt_rfcomm_send_control(&spp_control[ctl_hdl].rfcomm_ctr);

    if (API_SUCCESS == err)
    {
        BT_INFO("[SPP] SPP get remote port setting successfully, waitting for control_message callback.\n");
    }
    else
    {
        BT_ERR("[SPP] SPP get remote port setting failed, reason is 0x%04X.\n",err);
        if(SPP_CONTROL_CONFIG == spp_control[ctl_hdl].state)
        {
            spp_entity[index].state = SPP_IDLE;
            spp_entity[index].cb    = NULL;
        }
        init_spp_control(&spp_control[ctl_hdl]);
    }

    return err;
}

int bt_spp_send_pn(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, bt_spp_callback *cb, struct bt_spp_pn *pn)
{
    int     err;
    uint8_t index;
    uint8_t ctl_hdl;

    if((NULL == conn) || (NULL == cb))
    {
        return -EINVAL;
    }

    spp_initialize();

    /** allocate a spp_control[] entity */
    ctl_hdl = spp_get_free_control_entity();
    if(CONFIG_BT_SPP_MAX_CONN == ctl_hdl)
    {
        BT_ERR("[SPP] No available spp control entity.\n");
        return -ENOMEM;
    }

    index = initialize_spp_control_entity(ctl_hdl, conn, channel, role, cb);
    if(CONFIG_BT_SPP_MAX_CONN == index)
    {
        BT_ERR("[SPP] No available spp entity.\n");
        return index;
    }

    spp_control[ctl_hdl].spp             = &spp_entity[index];
    spp_control[ctl_hdl].rfcomm_ctr.type = BT_RFCOMM_SEND_PN;

    memcpy(&spp_control[ctl_hdl].rfcomm_ctr.control_data.pn, pn, sizeof(struct bt_spp_pn));
    spp_control[ctl_hdl].rfcomm_ctr.control_data.pn.mtu = pn->mtu;

    err = bt_rfcomm_send_control(&spp_control[ctl_hdl].rfcomm_ctr);
    if (0 == err)
    {
        BT_INFO("[SPP] SPP set mtu successfully, waitting for control_message callback.\n");
    }
    else
    {
        BT_ERR("[SPP] SPP set mtu failed, reason is 0x%04X.\n",err);
        if(SPP_CONTROL_CONFIG == spp_control[ctl_hdl].state)
        {
            spp_entity[index].state = SPP_IDLE;
            spp_entity[index].cb    = NULL;
        }
        init_spp_control(&spp_control[ctl_hdl]);
    }

    return err;
}

/** spp must be connected */
int bt_spp_get_local_pn(struct bt_conn * conn, uint8_t channel, bt_spp_role_t role, struct bt_spp_pn *pn)
{
    int     err;
    uint8_t index;
    uint8_t ctl_hdl;

    if (NULL == conn)
    {
        return -EINVAL;
    }

    spp_initialize();

    /** allocate a spp_control[] entity */
    ctl_hdl = spp_get_free_control_entity();
    if(CONFIG_BT_SPP_MAX_CONN == ctl_hdl)
    {
        BT_ERR("[SPP] No available spp control entity.\n");
        return -ENOMEM;
    }

    index = initialize_spp_control_entity(ctl_hdl, conn, channel, role, NULL);
    if(CONFIG_BT_SPP_MAX_CONN == index)
    {
        BT_ERR("[SPP] No available spp entity.\n");
        return index;
    }

    spp_control[ctl_hdl].spp             = &spp_entity[index];
    spp_control[ctl_hdl].rfcomm_ctr.type = BT_RFCOMM_GET_LOCAL_PN;

    err = bt_rfcomm_send_control(&spp_control[ctl_hdl].rfcomm_ctr);
    if (0 != err)
    {
        BT_ERR("[SPP] SPP get local pn failed, reason = 0x%04X\n",err);
    }
    else
    {
        BT_INFO("[SPP] SPP get local pn successfuly.");

        memcpy(pn, &spp_control[ctl_hdl].rfcomm_ctr.control_data.pn, sizeof(struct bt_spp_pn));
        pn->mtu  = spp_control[ctl_hdl].rfcomm_ctr.control_data.pn.mtu;
        pn->dlci = spp_control[ctl_hdl].rfcomm_ctr.control_data.pn.dlci;
    }

    if(SPP_CONTROL_CONFIG == spp_control[ctl_hdl].state)
    {
        spp_entity[index].state = SPP_IDLE;
        spp_entity[index].cb    = NULL;
    }
    init_spp_control(&spp_control[ctl_hdl]);


    return err;
}

int bt_spp_send_rls(struct bt_spp *spp, struct bt_spp_rls *rls)
{
    int     err;
    uint8_t index;

    if ((NULL == spp) || (NULL == rls))
    {
        err = -EINVAL;
    }
    else
    {
        if (SPP_CONNECTED == spp->state)
        {
            /** allocate a spp_control[] entity */
            index = spp_get_free_control_entity();
            if(CONFIG_BT_SPP_MAX_CONN == index)
            {
                BT_ERR("[SPP] No available spp control entity.\n");
                return -ENOMEM;
            }

            spp_control[index].spp = spp;

            spp_control[index].rfcomm_ctr.dlc = &spp->dlc;
            spp_control[index].rfcomm_ctr.type = BT_RFCOMM_SEND_RLS;

            switch(rls->rls)
            {
            case BT_SPP_RLS_NO_ERROR:
              spp_control[index].rfcomm_ctr.control_data.rls.line_status = RFCOMM_RLS_NO_ERROR;
              break;

            case BT_SPP_RLS_OVERRUN_ERROR:
              spp_control[index].rfcomm_ctr.control_data.rls.line_status = RFCOMM_RLS_OVERRUN_ERROR;
              break;

            case BT_SPP_RLS_PARITY_ERROR:
              spp_control[index].rfcomm_ctr.control_data.rls.line_status = RFCOMM_RLS_PARITY_ERROR;
              break;

            case BT_SPP_RLS_FRAMEING_ERROR:
              spp_control[index].rfcomm_ctr.control_data.rls.line_status = RFCOMM_RLS_FRAMEING_ERROR;
              break;

            default:
              /** invalid rls */
              break;
            }

            err = bt_rfcomm_send_control(&spp_control[index].rfcomm_ctr);
            if (0 != err)
            {
                BT_ERR("[SPP] SPP send rls failed, reason = 0x%04X\n",err);
                init_spp_control(&spp_control[index]);
            }
            else
            {
                BT_INFO("[SPP] SPP send rls successfully, waiting for callback event.");
            }
        }
        else
        {
            err = -EINVAL;
            BT_ERR("[SPP] SPP send rls failed, current spp isn't in connection.\n");
        }
    }

    return err;
}

int bt_spp_send_msc(struct bt_spp *spp, struct bt_spp_msc *msc)
{
    int     err;
    uint8_t index;

    if ((NULL == spp) || (NULL == msc))
    {
        err = -EINVAL;
    }
    else
    {
        if (SPP_CONNECTED == spp->state)
        {
            /** allocate a spp_control[] entity */
            index = spp_get_free_control_entity();
            if(CONFIG_BT_SPP_MAX_CONN == index)
            {
                BT_ERR("[SPP] No available spp control entity.\n");
                return -ENOMEM;
            }

            spp_control[index].spp = spp;

            spp_control[index].rfcomm_ctr.dlc = &spp->dlc;
            spp_control[index].rfcomm_ctr.type = BT_RFCOMM_SEND_MSC;
            memcpy(&spp_control[index].rfcomm_ctr.control_data.msc, msc, sizeof(struct bt_spp_msc));

            err = bt_rfcomm_send_control(&spp_control[index].rfcomm_ctr);

            if (0 != err)
            {
                BT_ERR("[SPP] SPP send msc failed, reason = 0x%04X\n",err);
                init_spp_control(&spp_control[index]);
            }
            else
            {
                BT_INFO("[SPP] SPP send msc successfully, waiting for callback event.");
            }
        }
        else
        {
            err = -EINVAL;
            BT_ERR("[SPP] SPP send msc failed, current spp isn't in connection.\n");
        }
    }

    return err;
}
#endif /** (defined(CONFIG_BT_SPP_ENABLE_CONTROL_CMD) && (CONFIG_BT_SPP_ENABLE_CONTROL_CMD > 0)) */

#endif /** (defined(CONFIG_BT_SPP) && ((CONFIG_BT_SPP) > 0U)) */
#endif /** (defined(CONFIG_BT_BREDR) && ((CONFIG_BT_BREDR) > 0U)) */
