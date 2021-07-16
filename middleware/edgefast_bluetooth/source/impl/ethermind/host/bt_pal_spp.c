/* zephyr_spp.c - SPP handling */

/*
 * Copyright (c) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <porting.h>
#include <errno.h>

#include "BT_sdp_api.h"
#include "BT_hci_api.h"
#include "db_gen.h"

#define LOG_ENABLE IS_ENABLED(CONFIG_BT_DEBUG_CONN)
#define LOG_MODULE_NAME bt_spp
#include "fsl_component_log.h"
LOG_MODULE_DEFINE(LOG_MODULE_NAME, kLOG_LevelTrace);

#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/sdp.h>
#include <bluetooth/addr.h>
#include "bt_pal_conn_internal.h"
#include "BT_spp_api.h"
#include "bluetooth/spp.h"
#include "bluetooth/rfcomm.h"

/** SPP States */
typedef enum _spp_state
{
    SPP_IDLE              = 0x00U,
    SPP_ALLOCATED         = 0x01U,
    SPP_WAIT_4_CONNECT    = 0x02U,
    SPP_IN_CONNECT        = 0x03U,
    SPP_CONNECTED         = 0x04U,
    SPP_IN_DISCONNECT     = 0x05U,
}spp_state_t;

/** @brief SPP Role Value.
 */
typedef enum bt_spp_role {
 BT_SPP_ROLE_SERVER,
 BT_SPP_ROLE_CLIENT
} bt_spp_role_t;

/*! @brief SPP Generic Data Element Value. */
struct bt_spp
{
    /* Channel of SPP Entity */
    uint8_t channel;

    /* Role of SPP Entity */
    bt_spp_role_t role;

    /* State of SPP Entity */
    spp_state_t state;

    /* The index of SPP handle in mindtree spp profile */
    uint32_t index;

    /* The BR connection handle */
    struct bt_conn *conn;

    /* The application callback function */
    bt_spp_callback *cb;
};

/*! @brief SPP Generic Data Element Value. */
typedef struct bt_spp_server
{
    /* Used */
    bool used;
    /* Channel of SPP Entity */
    uint8_t channel;
    /* The index of SPP handle in mindtree spp profile */
    uint32_t index;
    /* The application callback function */
    bt_spp_callback *cb;

    sys_snode_t node;
} bt_spp_server_t;

/* ----------------------------------------- Static Global Variables */
/* Initialization flag*/
static uint8_t initialized = 0U;

/* SPP Entities */
static struct bt_spp spp_instances[SPP_MAX_ENTITY];
static bt_spp_server_t spp_server_channel_instances[SPP_MAX_ENTITY];

/* SPP Mutex */
static OSA_MUTEX_HANDLE_DEFINE(spp_lock_mutex);
#define EDGEFAST_SPP_LOCK OSA_MutexLock((osa_mutex_handle_t)spp_lock_mutex, osaWaitForever_c)
#define EDGEFAST_SPP_UNLOCK OSA_MutexUnlock((osa_mutex_handle_t)spp_lock_mutex)

/* SPP SDP handle */
static SDP_HANDLE sdp_handle;

/* SPP SDP discover callback */
static discover_cb_t *discover_callbacks;

#if 1  /* Use MindTree SDP */
/* Remote SPP server channel */
static uint16_t remote_server_channel[3];

/* Remote SPP server count */
static uint16_t remote_server_channel_count;

/* SPP SDP discover callback */
static void spp_sdp_cb
     (
         uint8_t    command,
         uint8_t  * data,
         uint16_t   length,
         uint16_t   status
     );
#else /* Use zephyr SDP */
#define SDP_CLIENT_USER_BUF_LEN		512
NET_BUF_POOL_FIXED_DEFINE(sdp_spp_client_pool, CONFIG_BT_MAX_CONN,
			  SDP_CLIENT_USER_BUF_LEN, NULL);

static uint8_t sdp_spp_discover(struct bt_conn *conn,
			        struct bt_sdp_client_result *result);

static struct bt_sdp_discover_params discov_spp = {
	.uuid = BT_UUID_DECLARE_16(BT_SDP_SERIAL_PORT_SVCLASS),
	.func = sdp_spp_discover,
	.pool = &sdp_spp_client_pool,
};
#endif

/** To initialize a SPP Entity */
#define SPP_INIT_INSTANCES(index)                          \
{                                                          \
    spp_instances[index].channel = 0U;                     \
    spp_instances[index].role    = BT_SPP_ROLE_SERVER;     \
    spp_instances[index].state   = SPP_IDLE;               \
    spp_instances[index].index   = 0U;                     \
    spp_instances[index].conn    = NULL;                   \
    spp_instances[index].cb      = NULL;                   \
}

/** SPP profile callback receiving notificaiton from rfcomm */
static API_RESULT spp_rfcomm_callback
           (
               /* IN */  SPP_HANDLE spp_handle,
               /* IN */  SPP_EVENTS spp_event,
               /* IN */  API_RESULT status,
               /* IN */  void      *data,
               /* IN */  uint16_t   data_length
           );

/** Look up whether server channel is registered. */
static bt_spp_server_t *spp_server_lookup_channel(uint8_t channel)
{
    EDGEFAST_SPP_LOCK;
    for (uint8_t index = 0; index < SPP_MAX_ENTITY; ++index)
    {
        if ((true == spp_server_channel_instances[index].used) &&
            (channel == spp_server_channel_instances[index].channel))
        {
            EDGEFAST_SPP_UNLOCK;
            return &spp_server_channel_instances[index];
        }
    }

    EDGEFAST_SPP_UNLOCK;
    return NULL;
}

/** Look up whether server channel is registered. */
static bt_spp_server_t *spp_server_lookup_handle(uint32_t spp_handle)
{
    uint32_t index;

    for (index = 0; index < SPP_MAX_ENTITY; index++)
    {
        if ((true == spp_server_channel_instances[index].used) &&
            (spp_handle == spp_server_channel_instances[index].index))
        {
            return &spp_server_channel_instances[index];
        }
    }

    return NULL;
}

/** Look up whether server channel is registered. */
static struct bt_spp *spp_lookup_handle(uint32_t spp_handle)
{
    uint32_t index;

    for (index = 0; index < SPP_MAX_ENTITY; index++)
    {
        if ((SPP_IDLE != spp_instances[index].state) &&
            (spp_handle == spp_instances[index].index))
        {
            return &spp_instances[index];
        }
    }

    return NULL;
}

/** Get free spp entity. */
static struct bt_spp * spp_alloc_new(void)
{
    EDGEFAST_SPP_LOCK;
    for (uint8_t index = 0; index < SPP_MAX_ENTITY; ++index)
    {
        if (SPP_IDLE == spp_instances[index].state)
        {
            spp_instances[index].state = SPP_ALLOCATED;
            EDGEFAST_SPP_UNLOCK;
            return &spp_instances[index];
        }
    }

    EDGEFAST_SPP_UNLOCK;
    return NULL;
}

/** Get free spp server entity. */
static bt_spp_server_t * spp_server_alloc_new(void)
{
    EDGEFAST_SPP_LOCK;
    for (uint8_t index = 0; index < SPP_MAX_ENTITY; ++index)
    {
        if (false == spp_server_channel_instances[index].used)
        {
            spp_server_channel_instances[index].used = true;
            EDGEFAST_SPP_UNLOCK;
            return &spp_server_channel_instances[index];
        }
    }

    EDGEFAST_SPP_UNLOCK;
    return NULL;
}

/** Free spp entity. */
static void spp_free_entity(struct bt_spp *spp)
{
    EDGEFAST_SPP_LOCK;
    if (SPP_IDLE != spp->state)
    {
        spp->state = SPP_IDLE;
    }
    EDGEFAST_SPP_UNLOCK;
}

/** Free spp server entity. */
static void spp_server_free_entity(bt_spp_server_t *spp_server)
{
    EDGEFAST_SPP_LOCK;
    spp_server->used = false;
    spp_server->channel = 0;
    spp_server->index = 0;
    spp_server->cb = NULL;
    EDGEFAST_SPP_UNLOCK;
}

/** Initialize SPP entities. */
static void spp_initialize(void)
{
    uint8_t index;

    if (0U == initialized)
    {
        /* init spp instances */
        for (index = 0; index < SPP_MAX_ENTITY; index++)
        {
            SPP_INIT_INSTANCES(index);
        }

        /* Register SPP callback */
        (void)BT_spp_init(spp_rfcomm_callback);

        /* create spp Mutex */
        if (KOSA_StatusSuccess != OSA_MutexCreate((osa_mutex_handle_t)spp_lock_mutex))
        {
            BT_ERR("OSA_MutexCreate() failed!\n");
        }

        /* init spp server channel instances */
        for (index = 0; index < SPP_MAX_ENTITY; index++)
        {
            spp_server_free_entity(&spp_server_channel_instances[index]);
        }

        initialized = 1U;
    }
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
      ret = -ENOMEM;
      break;

    case SPP_ERR_INVALID_STATE:
      ret = -ENOTCONN;
      break;

    default:
      ret = -EIO;
      break;
    }

    return ret;
}

int bt_spp_server_register(uint8_t channel, bt_spp_callback *cb)
{
    API_RESULT        retval;
    bt_spp_server_t * spp_server_entity;

    if ((NULL == cb))
    {
        return -EINVAL;
    }

    spp_initialize();

    if ((5U != channel)  && (3U != channel))
    {
        /* Just support 2 rfcomm server now. */
        BT_ERR("Channel %d isn't supported, just support channel 5 and 3.\n", channel);
        return -ENOTSUP;
    }

    /* Check whether channel is registered */
    if (spp_server_lookup_channel(channel))
    {
        BT_ERR("Channel already registered.\n");
        return -EINVAL;
    }

#if 1  /* Call MindTree API to register spp server */
    /* Activate DB_RECORD_SPP and DB_RECORD_SPP_VS service record */
    if (5U == channel)
    {
        retval = BT_dbase_activate_record(0x00090002);
        assert(API_SUCCESS == retval);
    }
    else if(3U == channel)
    {
        retval = BT_dbase_activate_record(0x00090003);
        assert(API_SUCCESS == retval);
    }
    else
    {
        /* will support it */
    }
#else  /* Call zephyr API to register spp server */
    bt_sdp_register_service(&spp_rec);
#endif

    spp_server_entity = spp_server_alloc_new();
    if (NULL == spp_server_entity)
    {
        return -ENOMEM;
    }

    spp_server_entity->channel = channel;
    spp_server_entity->cb      = cb;

    retval = BT_spp_start(spp_server_entity->channel, &spp_server_entity->index);
    if (API_SUCCESS != retval)
    {
        BT_ERR("SPP register server channel %d failed. reason = 0x%04X\n", spp_server_entity->channel, retval);
        spp_server_free_entity(spp_server_entity);
    }
    else
    {
        BT_INFO("SPP register server %d successfully, waiting for SPP_CONNECT_IND event.\n", spp_server_entity->channel);
    }
    return spp_convert_return_value(retval);
}

int bt_spp_client_connect(struct bt_conn *conn, uint8_t channel, bt_spp_callback *cb, struct bt_spp **spp)
{
    API_RESULT      retval;
    struct bt_spp * spp_entity;

    if ((NULL == conn) || (NULL == cb) || (NULL == spp))
    {
        return -EINVAL;
    }

    spp_initialize();

    spp_entity = spp_alloc_new();
    if (NULL == spp_entity)
    {
        return -ENOMEM;
    }

    if (!channel)
    {
        spp_entity->channel = BT_RFCOMM_CHAN_SPP;
    }
    else
    {
        spp_entity->channel = channel;
    }
    spp_entity->role = BT_SPP_ROLE_CLIENT;
    spp_entity->conn = conn;
    spp_entity->cb   = cb;

    retval = BT_spp_start(spp_entity->channel, &spp_entity->index);
    if (API_SUCCESS != retval)
    {
        BT_ERR("SPP register server channel %d failed. reason = 0x%04X\n", spp_entity->channel, retval);
        spp_free_entity(spp_entity);
        return -EIO;
    }
    else
    {
        spp_entity->state = SPP_WAIT_4_CONNECT;
    }

    retval = BT_spp_connect(spp_entity->index, spp_entity->conn->br.dst.val, spp_entity->channel);

    if (API_SUCCESS == retval)
    {
        spp_entity->state = SPP_IN_CONNECT;
        BT_INFO("SPP connect successfully, waiting for SPP_CONNECT_CNF event.\n", spp_entity->channel);
    }
    else
    {
        BT_ERR("SPP connect failed, reason = 0x%04X\n", retval);
        (void)BT_spp_stop(spp_entity->index);
        spp_free_entity(spp_entity);
        spp_entity = NULL;
    }
    *spp = spp_entity;

    return spp_convert_return_value(retval);
}

int bt_spp_discover(struct bt_conn *conn, discover_cb_t *cb)
{
    int retval;

    if ((NULL == conn) || (NULL == cb))
    {
        return -EINVAL;
    }

    cb->conn           = conn;
    cb->next           = discover_callbacks;
    discover_callbacks = cb;

#if 1  /* Call MindTree API to discover */
    /* Set the SDP Handle */
    SDP_SET_HANDLE(sdp_handle, conn->br.dst.val, spp_sdp_cb);

    /* Call SDP open */
    retval = BT_sdp_open(&sdp_handle);
    if (API_SUCCESS != retval)
    {
        BT_ERR ("SPP SDP open failed, reason = 0x%04X \n", retval);
    }
    else
    {
        BT_INFO("SPP SDP open successfully, wait for SDP_Open event.\n");
    }
#else /* Call zephyr API to discover */
    retval = bt_sdp_discover(conn, &discov_spp);

    if (!retval)
    {
        BT_INFO("SDP discovery successfully.\n");
        SPP_SET_STATE(spp, SPP_IN_DISCOVER_SDP);

    }
    else
    {
        BT_ERR("SDP discovery failed: reason = %d\n", retval);
    }
#endif

    return spp_convert_return_value(retval);
}

int bt_spp_data_send(struct bt_spp *spp, uint8_t *data, uint16_t len)
{
    API_RESULT retval;

    if ((NULL == spp) || (NULL == data) || (0 == len))
    {
        return -EINVAL;
    }

    if (SPP_CONNECTED == spp->state)
    {
        retval = BT_spp_send(spp->index, data, len);
        if (API_SUCCESS != retval)
        {
            BT_ERR("SPP send data failed, reason = 0x%04X\n",retval);
        }
        else
        {
            BT_INFO("SPP send data successfully, waiting for SPP_SEND_CNF event.");
        }
    }
    else
    {
        retval = SPP_ERR_INVALID_STATE;
        BT_ERR("SPP send data failed, current spp isn't in connection.\n");
    }

    return spp_convert_return_value(retval);
}

int bt_spp_disconnect(struct bt_spp *spp)
{
    API_RESULT retval;

    if ((NULL == spp))
    {
        return -EINVAL;
    }

    if (SPP_CONNECTED == spp->state)
    {
        retval = BT_spp_disconnect(spp->index);

        if (API_SUCCESS != retval)
        {
            BT_ERR("SPP disconnect failed. reason = 0x%04X\n",retval);
        }
        else
        {
            spp->state = SPP_IN_DISCONNECT;
            BT_INFO("SPP disconnect successfully, waiting for SPP_DISCONNECT_CNF event.");
        }
    }
    else
    {
        retval = SPP_ERR_INVALID_STATE;
        BT_ERR("SPP disconnect failed, current spp isn't in connection.\n");
    }

    return spp_convert_return_value(retval);
}

/* SDP Callback for SPP */
#if 0  /* zephyr sdp callback */
static uint8_t sdp_spp_discover(struct bt_conn *conn,
                                struct bt_sdp_client_result *result)
{
    uint8_t addr[6];
    uint16_t channel[2], channelIndex = 0U;
    int res;
    uint8_t index;

    memcpy(addr, conn->br.dst.val , 6);

    if (result)
    {
#if 0
        shell_print(ctx_shell, "SDP A2SRC data@%p (len %u) hint %u from"
                    " remote %s", result->resp_buf,
                    result->resp_buf->len, result->next_record_hint,
                    addr);
#endif

        /*
         * Focus to get BT_SDP_ATTR_PROTO_DESC_LIST attribute item to
         * get RFCOMM Server channel.
         */
        res = bt_sdp_get_proto_param(result->resp_buf,
                                     BT_SDP_PROTO_RFCOMM, &channel[channelIndex]);
        if (res < 0)
        {
            BT_INFO("RFCOMM Server channel not found, err %d\n", res);
            goto done;
        }

        BT_INFO("RFCOMM Server channel 0x%04x\n", channel[channelIndex++]);
    }
    else
    {
        BT_INFO("No SDP SPP data from remote.");
    }

    for(index = 0U; index < SPP_MAX_ENTITY; index++)
    {
        if((SPP_IN_DISCOVER_SDP == spp_instances[index].state) && (0 == memcmp(addr, spp_instances[index].conn->br.dst.val, BT_BD_ADDR_SIZE)))
        {
            if(spp_instances[index].discover)
            {
                spp_instances[index].discover(&spp_instances[index],channelIndex, channel);
            }
            break;
        }
    }

done:
    return BT_SDP_DISCOVER_UUID_CONTINUE;
}
#else  /* MindTree sdp callback */
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
    API_RESULT     retval = API_FAILURE;

    uint16_t       len_attrib_data;

    static uint8_t sdp_close_due2error;

    BT_IGNORE_UNUSED_PARAM(length);
    BT_IGNORE_UNUSED_PARAM(sdp_close_due2error);

    /* Set SPP related UUID variables */
    uuid.uuid_type = UUID_16;
    uuid.uuid_union.uuid_16 = SERIALPORT_UUID;
    num_uuids = 0x01;

    /* It can come only while creating connection. verify */
    if (API_SUCCESS != status)
    {
        BT_INFO("STATUS : %X \n", status);
        BT_INFO("COMMAND : %X \n", command);

        /* Implies that SDP open was successful but service search failed */
        if (command == SDP_ServiceSearchAttributeResponse)
        {
            sdp_close_due2error = 2;

            BT_INFO("SDP CB : Closing SDP \n");

            BT_sdp_close(&sdp_handle);

            return;
        }
    }

    switch(command)
    {
        case SDP_Open : /* SDP open callback */
        {
            attribute_data = BT_alloc_mem(100);

            if (NULL == attribute_data)
            {
                BT_ERR("SDP CB : Alloc mem failed, closing SDP \n");
                /* Error in SDP operation */
                sdp_close_due2error = 2;

                /* Close SDP */
                BT_sdp_close(&sdp_handle);
                return;
            }

            len_attrib_data = 100;

            /* Do Service Search Request */
            retval = BT_sdp_servicesearchattributerequest
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

            if (API_SUCCESS != retval)
            {
                BT_ERR("SDP CB:BT_sdp_servicesearchattributerequest Failed\n");

                BT_free_mem(attribute_data);

                BT_ERR("Closing SDP Connection\n");

                /* Error in SDP operation */
                sdp_close_due2error = 2;

                /* Close SDP */
                BT_sdp_close(&sdp_handle);

                return;
            }
            /* SDP operation to continue */
            sdp_close_due2error = 1;

            break;
        }

        case SDP_Close: /* SDP Close callback */
            sdp_close_due2error = 0;

            BT_INFO("SDP CB : SDP PROCEDURE DONE... \n");

            break;

        /* Service Search Attribute callback */
        case SDP_ServiceSearchAttributeResponse:
            remote_server_channel_count = 0U;
            sdp_query_result = data;

            while (NULL != sdp_query_result)
            {
                sdp_query_result = BT_sdp_get_Pointer_to_next_AttributeList
                                    (
                                        data,
                                        ++remote_server_channel_count
                                    );
                if (NULL == sdp_query_result)
                {
                    break;
                }

                retval = BT_sdp_get_channel_number
                         (
                             sdp_query_result,
                             (uint8_t *)&remote_server_channel[--remote_server_channel_count]
                         );

                if (API_SUCCESS == retval)
                {
                    remote_server_channel_count++;
                }
            }

            if (--remote_server_channel_count)
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

            /* Free allocated memory for attribute data */
            BT_free_mem(data);

            /* Close SDP */
            BT_sdp_close(&sdp_handle);

           break;


        case SDP_ErrorResponse:

            BT_INFO("SDP_ErrorResponse \n");

            /* Free allocated memory for attribute data */
            BT_free_mem(data);

            sdp_close_due2error = 2;

            BT_sdp_close(&sdp_handle);

            break;

        default: /* Invalid: Nothing to do */
            break;

    }/* switch */

    return;
}
#endif

/* Callback function registered with SPP profile */
static API_RESULT spp_rfcomm_callback
           (
               /* IN */  SPP_HANDLE spp_handle,
               /* IN */  SPP_EVENTS spp_event,
               /* IN */  API_RESULT status,
               /* IN */  void     * data,
               /* IN */  uint16_t   data_length
           )
{
    struct bt_spp * spp;
    uint8_t       * l_data = (uint8_t*)( data );

    BT_INFO("\n"\
           "SPP HANDLE : %u\n"\
           "EVENT      : %d\n"\
           "RESULT     : 0x%04X\n",
            (unsigned int) spp_handle, spp_event, status);

    /* According to spp_handle, determine relative spp instance */
    spp = spp_lookup_handle(spp_handle);

    if ((NULL == spp) && (SPP_CONNECT_IND != spp_event))
    {
        BT_INFO("Not found spp_instances whose index is %u\n", spp_handle);
        return API_SUCCESS;
    }

    switch(spp_event)
    {
    case SPP_CONNECT_CNF:  /* SPP client initializes connection complete event */
        BT_INFO("SPP_CONNECT_CNF -> 0x%04X\n", status);
        BT_INFO("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            spp->state = SPP_CONNECTED;
        }

        if (spp->cb->connected)
        {
            spp->cb->connected(spp, status);
        }

        if (API_SUCCESS != status)
        {
            (void)BT_spp_stop(spp->index);
            spp_free_entity(spp);
        }
        break;

    case SPP_CONNECT_IND:  /* SPP server waits for connection success event */
        BT_INFO("SPP_CONNECT_IND -> 0x%04X\n", status);
        BT_INFO("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (API_SUCCESS == status)
        {
            if (NULL == spp)
            {
                bt_spp_server_t *spp_server;

                spp_server = spp_server_lookup_handle(spp_handle);
                if (NULL == spp_server)
                {
                    BT_ERR("Not found spp_instances whose index is %u", spp_handle);
                    return API_SUCCESS;
                }

                spp = spp_alloc_new();
                if (NULL == spp)
                {
                    BT_ERR("Create new spp instance failed");
                    (void)BT_spp_disconnect(spp_server->index);
                    spp_server_free_entity(spp_server);
                    return API_SUCCESS;
                }

                spp->role    = BT_SPP_ROLE_SERVER;
                spp->channel = spp_server->channel;
                spp->cb      = spp_server->cb;
                spp->index   = spp_server->index;
            }

            /* get remote device address */
            bt_addr_le_t bd_addr;
            memcpy(bd_addr.a.val, l_data, data_length);
            /* get conn */
            spp->conn = bt_conn_lookup_addr_le(BT_ID_DEFAULT, &bd_addr);

            spp->state = SPP_CONNECTED;
        }

        if ((NULL != spp) && (spp->cb->connected))
        {
            spp->cb->connected(spp, status);
        }
        break;

    case SPP_DISCONNECT_CNF:   /* initialize disconnect */
    case SPP_DISCONNECT_IND:
        BT_INFO("SPP_DISCONNECT_CNF -> 0x%04X\n", status);
        BT_INFO("Remote device " BT_DEVICE_ADDR_ONLY_FRMT_SPECIFIER " \n",
        BT_DEVICE_ADDR_ONLY_PRINT_STR (l_data));

        if (spp->cb->disconnected)
        {
            spp->cb->disconnected(spp, status);
        }

        if (API_SUCCESS == status)
        {
            if (BT_SPP_ROLE_SERVER == spp->role)
            {
                bt_conn_unref(spp->conn);
            }
            else
            {
                (void)BT_spp_stop(spp->index);
            }
            spp_free_entity(spp);
        }
        break;

    case SPP_SEND_CNF:
        BT_INFO("SPP_SEND_CNF -> 0x%04X\n", status);
        if (spp->cb->data_sent)
        {
            spp->cb->data_sent(spp, l_data, strlen((char const *)l_data), status);
        }
        break;

    case SPP_RECVD_DATA_IND:
        BT_INFO("SPP_RECVD_DATA_IND -> 0x%04X\n", status);
        if (spp->cb->data_received)
        {
            spp->cb->data_received(spp, l_data, strlen((char const *)l_data), status);
        }
        break;

    default:
        BT_INFO("\nUnknown command type\n");
        break;
    } /* switch */

    return API_SUCCESS;
}
