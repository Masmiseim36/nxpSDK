
/**
 *  \file appl_ipspn.c
 *
 *  This file contains the Sample Internet Protocol Service Node application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "appl_service.h"

#ifdef IPSP_HAVE_6LO_NIFACE
#include "ipsp_pl.h"
#endif /* IPSP_HAVE_6LO_NIFACE */

#if (defined ATT && defined IPSPN)

/* ----------------------------------------- Structures/Data Types */
/* IPSP Router Send Data buffer element */
typedef struct _APPL_IPSPN_SENDELT
{
    UCHAR * buf;

    UINT16 len;

}APPL_IPSPN_SENDELT;

/* ----------------------------------------- Macro Defines */
/* Flag to enable Connection Auto Accept */
#define APPL_LE_L2CAP_CONN_AUTO_ACCEPT

/* Flag to enable sending credits automatically, on reaching Low Water Mark */
#define APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM

/* Flag to enable sending credits automatically, on receiving a single packet */
#define APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT

/* Flag to control Data Dump of Received Data over L2CAP CoC */
#define APPL_LE_IPSPN_HAVE_L2CAP_DATA_DUMP

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Static Global Variables */

/* -- L2CAP CBFC related defines -- */
DECL_STATIC API_RESULT ipsp_l2ca_connect_ind_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     psm,
               L2CAP_CBFC_CONNECT_PARAM * param
           );

DECL_STATIC API_RESULT ipsp_l2ca_connect_cnf_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     response,
               L2CAP_CBFC_CONNECT_PARAM * param
           );

DECL_STATIC API_RESULT ipsp_l2ca_disconnect_ind_cb(UINT16 lcid);
DECL_STATIC API_RESULT ipsp_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason);
DECL_STATIC API_RESULT ipsp_l2ca_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen);
DECL_STATIC API_RESULT ipsp_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit);
DECL_STATIC API_RESULT ipsp_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit);
DECL_STATIC API_RESULT ipsp_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len);
DECL_STATIC API_RESULT appl_ipspn_data_tx
                       (
                           DEVICE_HANDLE * device_handle,
                           UINT16        cid,
                           UCHAR         * edata,
                           UINT16        edatalen
                       );

#define APPL_L2CAP_LE_PSM LE_PSM_IPSP
#define APPL_BUFFER_SIZE  0x0062U

#define APPL_LE_CO_MTU    1500U
#define APPL_LE_CO_MPS    512U
#define APPL_LE_CREDIT    10U

#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
#define APPL_RECV_CREDIT_LWM                            0x0001U
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */

#ifdef IPSP_HAVE_6LO_NIFACE
#define APPL_MAC_LE2BE(addrle, addrbe) \
        appl_reverse_bytestream_endianness((addrle), (addrbe), BT_BD_ADDR_SIZE)

#define APPL_MAC_BE2LE(addrbe, addrle) \
        appl_reverse_bytestream_endianness((addrbe), (addrle), BT_BD_ADDR_SIZE)

static BT_DEVICE_ADDR lmac;
static void lel2ca_ipspn_read(UCHAR *edata, UINT16 edatalen);
#endif /* IPSP_HAVE_6LO_NIFACE */

DECL_STATIC L2CAP_PSM_CBFC appl_l2cap_psm =
{
    ipsp_l2ca_connect_ind_cb,
    ipsp_l2ca_connect_cnf_cb,
    ipsp_l2ca_disconnect_ind_cb,
    ipsp_l2ca_disconnect_cnf_cb,
    ipsp_l2ca_data_read_cb,
    ipsp_l2ca_low_rx_credit_ind_cb,
    ipsp_l2ca_tx_credit_ind_cb,
    APPL_L2CAP_LE_PSM,
#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
    APPL_RECV_CREDIT_LWM,
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */

    ipsp_l2ca_data_write_cb
};

DECL_STATIC L2CAP_CBFC_CONNECT_PARAM connect_param =
{
    APPL_LE_CO_MTU,
    APPL_LE_CO_MPS,
    APPL_LE_CREDIT
};

DECL_STATIC DEVICE_HANDLE appl_peer_handle;
DECL_STATIC UINT16        appl_le_co_cid;
DECL_STATIC UINT16 appl_le_co_peer_mtu = 0x0000U;

/**
 * NOTEs:
 * 1. The Currently Queue, doesn't allow for over-write of packets. Once the
 *    Number of application payload packets that are queued reaches
 *    APPL_IPSPN_SEND_QUEUE_SIZE. The Transmit function appl_ipspn_data_tx()
 *    returns error.
 * 2. Depending on the application usage scenario, this needs to be modified
 *    to have larger Queue or Queue Failure Handling Logic etc.
 * 3. The current application needs to keep polling for which the queue is
 *    non-empty. There will not be any event or notification to application
 *    when there is a empty element in the queue. The ipspn_l2ca_data_write_cb
 *    is where the freeing of an existing element occurs.
 * 4. Currently, this application does not handle to failures encountered in
 *    L2CAP CoC Data Send due to Low Remote Receive Credits. This handling
 *    needs to be updated later depending on the usage scenario.
 */
/* L2CAP CoC Send Queue related defines and globals for IPSPN */
/* Send Queue, Queue Limit */
#define APPL_IPSPN_SEND_QUEUE_SIZE         8U

DECL_STATIC APPL_IPSPN_SENDELT appl_ipspn_send[APPL_IPSPN_SEND_QUEUE_SIZE];
DECL_STATIC UCHAR appl_ipspn_sendwr;
DECL_STATIC UCHAR appl_ipspn_sendrd;
/* --------------------------------------------- Functions */
void appl_reverse_bytestream_endianness(UCHAR * src, UCHAR * dst, UINT16 len)
{
    UINT16 i;

    for (i = 0U; i < len; i++)
    {
        dst[i] = src[(len - 1U) - i];
    }
}

void appl_ipspn_init(void)
{
    API_RESULT retval;
    UINT32     i;

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    GATT_DB_SIGNATURE appl_ipspn_db_sign;
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

#ifdef IPSP_HAVE_6LO_NIFACE
    BT_DEVICE_ADDR bdaddr;

    /* Get the local BD address */
    BT_hci_get_local_bd_addr(bdaddr.addr);
    bdaddr.type = 0U;

    /* Form the MAC */
    APPL_MAC_LE2BE(bdaddr.addr, lmac.addr);
    lmac.type = bdaddr.type;

    /* INit the network interface */
    ipsp_init_pl(&lmac, lel2ca_ipspn_read, BT_FALSE);
#endif /* IPSP_HAVE_6LO_NIFACE */

    /* Initialize the queue pointers on each connection */
    appl_ipspn_sendrd = 0U;
    appl_ipspn_sendwr = 0U;
    for (i = 0U; i < APPL_IPSPN_SEND_QUEUE_SIZE; i++)
    {
        appl_ipspn_send[i].buf = NULL;
        appl_ipspn_send[i].len = 0U;
    }

    retval = l2cap_cbfc_register_psm (&appl_l2cap_psm);
    if(retval != API_SUCCESS)
    {
        APPL_TRC("[IPSPN]: ERROR!!! retval = 0x%04X", retval);
    }
    else
    {
        APPL_TRC("[IPSPN]: PSM Registration with L2CAP Successful!!\n");
    }

#ifdef GATT_DB_DYNAMIC

    (BT_IGNORE_RETURN_VALUE) appl_ipspn_add_gaps();
    (BT_IGNORE_RETURN_VALUE) appl_ipspn_add_gatts();
    (BT_IGNORE_RETURN_VALUE) appl_ipspn_add_bas();
    (BT_IGNORE_RETURN_VALUE) appl_ipspn_add_ipspn();

#ifdef GATT_DB_HAVE_DB_SIGNATURE
    /* Set the IPSPN GATT DB Signature */
    appl_ipspn_db_sign = APPL_IPSPN_GATT_DB_SIGN;
    retval = BT_gatt_db_dyn_set_db_signature(appl_ipspn_db_sign);
#endif /* GATT_DB_HAVE_DB_SIGNATURE */

    retval = BT_gatt_db_dyn_register();

    APPL_TRC(
    "[IPSPN]: GATT Database Registration Status: 0x%04X\n", retval);

    GATT_DB_MAX_ATTRIBUTES = BT_gatt_db_get_attribute_count();
#else
    appl_ipspn_gatt_db_register();
#endif /* GATT_DB_DYNAMIC */

    /* Register IPSPN GATT DB Handler with PL Extension */
    gatt_db_init_pl(gatt_db_ipspn_gatt_char_handler);
}

void appl_ipspn_bt_init(void)
{
    APPL_TRC("[IPSPN]: In appl_ipspn_bt_init\n");

    /* Set GAP Role */
    APPL_SET_GAP_PERIPHERAL_ROLE();

    appl_ipspn_server_reinitialize ();
}

void appl_ipspn_bt_deinit(void)
{
    APPL_TRC("[IPSPN]: In appl_ipspn_bt_deinit\n");
}

void appl_ipspn_connect(DEVICE_HANDLE  * dq_handle)
{
    BT_IGNORE_UNUSED_PARAM(dq_handle);
}

void appl_ipspn_server_reinitialize (void)
{
#if ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL))
    if (BT_TRUE == APPL_IS_GAP_PERIPHERAL_ROLE())
    {
        /* Configure and Enable Advertising */
        appl_service_configure_adv(APPL_GAP_PROC_NORMAL, HCI_ADV_IND, 0x00U, 0x00U, NULL, 0x00U);
        appl_service_enable_adv(0x01U);
    }
#endif /* ((defined APPL_GAP_BROACASTER) || defined (APPL_GAP_PERIPHERAL)) */

#if ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL))
    if (BT_TRUE == APPL_IS_GAP_CENTRAL_ROLE())
    {
        /* Configure and Enable Scanning */
        appl_service_configure_scan (APPL_GAP_PROC_NORMAL, 0x00U, 0x00U, 0x00U);
        appl_service_enable_scan(0x01U);
    }
#endif /* ((defined APPL_GAP_OBSERVER) || (defined APPL_GAP_CENTRAL)) */
}


/* -- L2CAP CBFC related functions -- */
DECL_STATIC API_RESULT ipsp_l2ca_connect_ind_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     psm,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
#ifdef IPSP_HAVE_6LO_NIFACE
    BT_DEVICE_ADDR bdaddr, mac;
#endif /* IPSP_HAVE_6LO_NIFACE */

    API_RESULT retval;
    UINT16 response;

    APPL_TRC (
    "\n[IPSPN]: ipsp_l2ca_connect_ind_cb from Device %02X, lcid %04X, PSM 0x%04X\n",
    (*handle), lcid, psm);

    APPL_TRC (
    "\n[IPSPN]: MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

    appl_peer_handle = (*handle);
    appl_le_co_cid = lcid;
    appl_le_co_peer_mtu = param->mtu;

    BT_IGNORE_UNUSED_PARAM(appl_le_co_peer_mtu);

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_CONN_AUTO_ACCEPT
    response = 0x0000U;
    retval = l2ca_cbfc_connect_rsp
             (
                 lcid,
                 response,
                 &connect_param
             );

#ifdef IPSP_HAVE_6LO_NIFACE
    if (LE_PSM_IPSP == appl_l2cap_psm.psm)
    {
        /* Get the remote bd address */
        device_queue_get_remote_addr(handle, &bdaddr);

        /* Form the MAC */
        APPL_MAC_LE2BE(bdaddr.addr, mac.addr);
        mac.type = bdaddr.type;

        /* Start the network interface */
        ipsp_start_pl(&mac);
    }
#endif /* IPSP_HAVE_6LO_NIFACE */
#endif /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */

    return retval;
}

DECL_STATIC API_RESULT ipsp_l2ca_connect_cnf_cb
           (
               DEVICE_HANDLE        * handle,
               UINT16                     lcid,
               UINT16                     response,
               L2CAP_CBFC_CONNECT_PARAM * param
           )
{
    APPL_TRC (
    "\n[IPSPN]: ipsp_l2ca_connect_cnf_cb from Device %02X, lcid %04X, response 0x%04X\n",
    (*handle), lcid, response);

    /* Print MTU, MPS and Credit, on success response */
    if (L2CAP_CONNECTION_SUCCESSFUL == response)
    {
        APPL_TRC (
        "\n[IPSPN]: MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

        appl_peer_handle = (*handle);
        appl_le_co_cid = lcid;
        appl_le_co_peer_mtu = param->mtu;
    }

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipsp_l2ca_disconnect_ind_cb(UINT16 lcid)
{
    APPL_TRC ("\n[IPSPN]: ipsp_l2ca_disconnect_ind_cb, lcid %04X\n",lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000U;

#ifdef IPSP_HAVE_6LO_NIFACE
    if (LE_PSM_IPSP == appl_l2cap_psm.psm)
    {
        /* Stop the network interface */
        ipsp_stop_pl();
    }
#endif /* IPSP_HAVE_6LO_NIFACE */

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipsp_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason)
{
    APPL_TRC ("\n[IPSPN]: ipsp_l2ca_disconnect_cnf_cb lcid %04X\n", lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000U;

#ifdef IPSP_HAVE_6LO_NIFACE
    if (LE_PSM_IPSP == appl_l2cap_psm.psm)
    {
        /* Stop the network interface */
        ipsp_stop_pl();
    }
#endif /* IPSP_HAVE_6LO_NIFACE */

    BT_IGNORE_UNUSED_PARAM(reason);

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipsp_l2ca_data_read_cb (UINT16 lcid, UINT16 result, UCHAR * data, UINT16 datalen)
{
    API_RESULT retval;
    UINT16 no_of_credits;

#ifdef APPL_LE_IPSPN_HAVE_L2CAP_DATA_DUMP
    APPL_TRC (
    "\n[IPSPN]: ipsp_l2ca_data_read_cb, lcid %04X, data len %04X, result 0x%04X\n",
    lcid, datalen, result);

    appl_dump_bytes(data, datalen);
#endif /* APPL_LE_IPSPN_HAVE_L2CAP_DATA_DUMP */

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT
    /**
     * Calculate possible number of LE L2CAP Frames.
     *
     * Maximum size of each frame shall not exceed local MPS.
     * Account for the 2 octet of SDU length.
     */
    no_of_credits = (datalen + 2U + APPL_LE_CO_MPS - 1U)/APPL_LE_CO_MPS;

    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 no_of_credits
             );
#endif /* APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT */

#ifdef IPSP_HAVE_6LO_NIFACE
    ipsp_write_pl(data, datalen);
#endif /* IPSP_HAVE_6LO_NIFACE */

    BT_IGNORE_UNUSED_PARAM(data);
    BT_IGNORE_UNUSED_PARAM(result);

    return retval;
}

DECL_STATIC API_RESULT ipsp_l2ca_low_rx_credit_ind_cb (UINT16 lcid, UINT16 credit)
{
    API_RESULT retval;

    APPL_TRC (
    "\n[IPSPN]: ipsp_l2ca_low_rx_credit_ind_cb, lcid %04X, Credit %04X\n",
    lcid,credit);

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM
    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 APPL_LE_CREDIT
             );
#endif /* APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM */

    return retval;
}

DECL_STATIC API_RESULT ipsp_l2ca_tx_credit_ind_cb (UINT16 lcid, UINT16 result, UINT16 credit)
{

#if 0
    APPL_TRC (
    "\n[IPSPN]: ipsp_l2ca_tx_credit_cb, lcid %04X, Credit %04X\n",
    lcid,credit);
#endif /* 0 */

    BT_IGNORE_UNUSED_PARAM(lcid);
    BT_IGNORE_UNUSED_PARAM(result);
    BT_IGNORE_UNUSED_PARAM(credit);

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipsp_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR *buffer, UINT16 buffer_len)
{
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(lcid);
    BT_IGNORE_UNUSED_PARAM(result);
    BT_IGNORE_UNUSED_PARAM(buffer);
    BT_IGNORE_UNUSED_PARAM(buffer_len);

    CONSOLE_OUT(
    "\n[IPSPR]: ipsp_l2ca_data_write_cb, lcid %04X, Result %04X\n",
    lcid, result);

    /*
     * NOTE:
     * L2CAP does not Pass any Identifier related to the buffer pointer
     * to match from the Queue's book-keeping.
     *
     * Currently only one peer device's Data is entertained by this
     * application and is queued up in the Transmit Queue.
     * Later, additional members can be introduced to this Transmit queue
     * element and used for multiple peer device related Data TX Handling.
     */
    /* Free the stored allocated buffer */
    BT_free_mem(appl_ipspn_send[appl_ipspn_sendrd].buf);
    appl_ipspn_send[appl_ipspn_sendrd].buf = NULL;

    retval = API_SUCCESS;

    /* Move to the Next Queue element */
    appl_ipspn_sendrd++;
    appl_ipspn_sendrd &= (APPL_IPSPN_SEND_QUEUE_SIZE - 1U);

    if ((appl_ipspn_sendrd != appl_ipspn_sendwr) &&
        (NULL != appl_ipspn_send[appl_ipspn_sendrd].buf))
    {
        retval = l2ca_channel_data_write
                 (
                     &appl_peer_handle,
                     appl_le_co_cid,
                     appl_ipspn_send[appl_ipspn_sendrd].buf,
                     appl_ipspn_send[appl_ipspn_sendrd].len
                 );

        if (API_SUCCESS != retval)
        {
            /* Write Failed */
            APPL_TRC(
            "[IPSPR]: L2CAP Channel Data Write failed - 0x%04x\n",
            retval);

            /* Free the previously allocated memory */
            BT_free_mem(appl_ipspn_send[appl_ipspn_sendrd].buf);
        }
    }

    return API_SUCCESS;
}

#ifdef IPSP_HAVE_6LO_NIFACE
void lel2ca_ipspn_read(UCHAR *edata, UINT16 edatalen)
{
    API_RESULT retval;

    /* Invoke the application data send function */
    retval = appl_ipspn_data_tx
             (
                 &appl_peer_handle,
                 appl_le_co_cid,
                 edata,
                 edatalen
             );

    if (API_SUCCESS != retval )
    {
        APPL_ERR(
        "[IPSPR]: lel2ca_ipspn_read L2CAP CoC Write failed - 0x%04x\n",
        retval);
    }

    return;
}
#endif /* IPSP_HAVE_6LO_NIFACE */

DECL_STATIC API_RESULT appl_ipspn_data_tx
                       (
                           DEVICE_HANDLE * device_handle,
                           UINT16        cid,
                           UCHAR         * edata,
                           UINT16        edatalen
                       )
{
    API_RESULT retval;
    UCHAR      * buf;

    /* Initialize */
    retval = API_FAILURE;

    /* TODO : Have a connection State Check? */

    /**
     * Check if there is already pending Data in the Current
     * Queue element. If so, return Queue Full condition.
     */
    if (NULL != appl_ipspn_send[appl_ipspn_sendwr].buf)
    {
        APPL_ERR (
        "[IPSPR]: L2CAP CoC Send Queue Full![Queue Size: %d]\n",
        APPL_IPSPN_SEND_QUEUE_SIZE);

        /**
         * TODO:
         * Have a Specific Error Code here?
         */
        /* return retval; */
    }
    else
    {
        /* Allocate Memory for the buffer */
        buf = BT_alloc_mem(edatalen);

        if (NULL == buf)
        {
            APPL_ERR(
            "[IPSPR]: **ERR** Failed to allocate buffer of length %d\n", edatalen);

            /* return retval; */
        }
        else
        {

            /* Reinitialize Retval */
            retval = API_SUCCESS;

            /* Copy the data */
            BT_mem_copy(buf, edata, edatalen);

            /* Add it to the queue */
            appl_ipspn_send[appl_ipspn_sendwr].buf = buf;
            appl_ipspn_send[appl_ipspn_sendwr].len = edatalen;

            /**
             * Is Tx ongoing?
             * Add to Queue if ongoing, else send the data
             * over L2CAP Channel.
             */
            if (appl_ipspn_sendwr == appl_ipspn_sendrd)
            {
                retval = l2ca_channel_data_write
                         (
                             device_handle,
                             cid,
                             buf,
                             edatalen
                         );

                if (API_SUCCESS != retval)
                {
                    /**
                     * TODO:
                     * Check if the Write Fails due to Unavailability of
                     * Receive Credit from Remote Side.
                     * If So, Assign a specific Error Code?
                     */
                    /* Write Failed */
                    APPL_ERR(
                    "[IPSPR]: L2CAP Channel Data Write failed - 0x%04x\n", retval);

                    /* Free the previously allocated memory */
                    BT_free_mem(buf);
                }
            }

            /* Add to Queue */
            if (API_SUCCESS == retval)
            {
                appl_ipspn_sendwr++;
                appl_ipspn_sendwr &= (APPL_IPSPN_SEND_QUEUE_SIZE - 1U);
            }
        }
    }

    return retval;
}

#endif /* (defined ATT && defined IPSPN) */

