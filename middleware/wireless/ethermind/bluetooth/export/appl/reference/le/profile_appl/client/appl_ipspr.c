
/**
 *  \file appl_ipspr.c
 *
 *  This is a IPSP Router application.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */

/* ----------------------------------------- Header File Inclusion */
#include "BT_hci_api.h"
#include "l2cap.h"

#include "appl_gatt_client.h"

/* This macro needs to be defined in the Preprocessors */
/* #define IPSP_HAVE_6LO_NIFACE */

#ifdef IPSP_HAVE_6LO_NIFACE
#include "ipsp_pl.h"
#endif /* IPSP_HAVE_6LO_NIFACE */

#include "lwip/opt.h"
#include "ping.h"
#include "lwip/mem.h"
#include "lwip/raw.h"
#include "lwip/icmp.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include "lwip/inet_chksum.h"
#include "lwip/prot/ip4.h"
#if PING_USE_SOCKETS
#include "lwip/sockets.h"
#include "lwip/inet.h"
#include <string.h>
#endif /* PING_USE_SOCKETS */

#if (defined ATT && defined IPSPR)

/* ----------------------------------------- Structures/Data Types */
/* IPSP Router Send Data buffer element */
typedef struct _APPL_IPSPR_SENDELT
{
    UCHAR * buf;

    UINT16 len;

}APPL_IPSPR_SENDELT;

/* ----------------------------------------- Macro Defines */
/* Flag to enable Connection Auto Accept */
#define APPL_LE_L2CAP_CONN_AUTO_ACCEPT

/* Flag to enable sending credits automatically, on reaching Low Water Mark */
#define APPL_LE_L2CAP_AUTO_SEND_CREDIT_ON_LWM

/* Flag to enable sending credits automatically, on receiving a single packet */
#define APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT

#define APPL_L2CAP_LE_PSM                               LE_PSM_IPSP
#define APPL_BUFFER_SIZE                                0x0062U

#define APPL_LE_CO_MTU                                  1500U
#define APPL_LE_CO_MPS                                  512U
#define APPL_LE_CREDIT                                  10U

#define APPL_LE_L2CAP_CBFC_WRITE_BUF_SIZE               1024U

#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
#define APPL_RECV_CREDIT_LWM                            0x0001U
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */

#ifdef IPSP_HAVE_6LO_NIFACE
#define APPL_MAC_LE2BE(addrle, addrbe) \
        ipspr_reverse_bytestream_endianness((addrle), (addrbe), BT_BD_ADDR_SIZE)

#define APPL_MAC_BE2LE(addrbe, addrle) \
        ipspr_reverse_bytestream_endianness((addrbe), (addrle), BT_BD_ADDR_SIZE)

static BT_DEVICE_ADDR lmac;
static void lel2ca_ipspr_read(UCHAR *edata, UINT16 edatalen);
#endif /* IPSP_HAVE_6LO_NIFACE */


/* ----------------------------------------- External Global Variables */
extern ATT_HANDLE     appl_gatt_client_handle;

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */

DECL_STATIC UCHAR l2cap_cbfc_write_buffer[APPL_LE_L2CAP_CBFC_WRITE_BUF_SIZE];

DECL_STATIC UCHAR ipspr_client_menu[] =
"\n\
    0 - Exit\n\
    1 - Refresh\n\n\
   10 - Discover IPSP Router Service as Primary Service\n\
    -- IPSP L2CAP CoC Channel --\n\
   20 - Connect to IPSP-Node over LE L2CAP CoC\n\
   21 - Disconnect IPSP-Node LE L2CAP CoC\n\
   22 - Send Data to IPSP-Node\n\
   23 - Ping request\n\
Your Option?\n\
";

/* -- L2CAP CBFC related defines -- */
DECL_STATIC API_RESULT ipspr_l2ca_connect_ind_cb
                       (
                           DEVICE_HANDLE            * handle,
                           UINT16                   lcid,
                           UINT16                   psm,
                           L2CAP_CBFC_CONNECT_PARAM * param
                       );

DECL_STATIC API_RESULT ipspr_l2ca_connect_cnf_cb
                       (
                           DEVICE_HANDLE* handle,
                           UINT16                     lcid,
                           UINT16                     response,
                           L2CAP_CBFC_CONNECT_PARAM* param
                       );

DECL_STATIC API_RESULT ipspr_l2ca_disconnect_ind_cb(UINT16 lcid);
DECL_STATIC API_RESULT ipspr_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason);
DECL_STATIC API_RESULT ipspr_l2ca_data_read_cb(UINT16 lcid, UINT16 result, UCHAR* data, UINT16 datalen);
DECL_STATIC API_RESULT ipspr_l2ca_low_rx_credit_ind_cb(UINT16 lcid, UINT16 credit);
DECL_STATIC API_RESULT ipspr_l2ca_tx_credit_ind_cb(UINT16 lcid, UINT16 result, UINT16 credit);
DECL_STATIC API_RESULT ipspr_l2ca_data_write_cb(UINT16 lcid, UINT16 result, UCHAR* buffer, UINT16 buffer_len);
DECL_STATIC API_RESULT appl_ipspr_data_tx
                       (
                           DEVICE_HANDLE * device_handle,
                           UINT16        cid,
                           UCHAR         * edata,
                           UINT16        edatalen
                       );

DECL_STATIC L2CAP_PSM_CBFC appl_ipspr_l2cap_psm =
{
    ipspr_l2ca_connect_ind_cb,
    ipspr_l2ca_connect_cnf_cb,
    ipspr_l2ca_disconnect_ind_cb,
    ipspr_l2ca_disconnect_cnf_cb,
    ipspr_l2ca_data_read_cb,
    ipspr_l2ca_low_rx_credit_ind_cb,
    ipspr_l2ca_tx_credit_ind_cb,
    APPL_L2CAP_LE_PSM,
#ifdef L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM
    APPL_RECV_CREDIT_LWM,
#endif /* L2CAP_CBFC_SUPPORT_UL_CREDIT_LWM */
    ipspr_l2ca_data_write_cb
};

DECL_STATIC L2CAP_CBFC_CONNECT_PARAM connect_param =
{
    APPL_LE_CO_MTU,
    APPL_LE_CO_MPS,
    APPL_LE_CREDIT
};

DECL_STATIC DEVICE_HANDLE appl_peer_handle;
DECL_STATIC UINT16        appl_le_co_cid;
DECL_STATIC UINT16        appl_le_co_peer_mtu = 0x0000U;
DECL_STATIC UCHAR         appl_ipsp_disc_state = BT_FALSE;

/* Send queue for IPSPR */
/**
 * NOTEs:
 * 1. The Currently Queue, doesn't allow for over-write of packets. Once the
 *    Number of application payload packets that are queued reaches
 *    APPL_IPSPR_SEND_QUEUE_SIZE. The Transmit function appl_ipspr_data_tx()
 *    returns error.
 * 2. Depending on the application usage scenario, this needs to be modified
 *    to have larger Queue or Queue Failure Handling Logic etc.
 * 3. The current application needs to keep polling for which the queue is
 *    non-empty. There will not be any event or notification to application
 *    when there is a empty element in the queue. The ipspr_l2ca_data_write_cb
 *    is where the freeing of an existing element occurs.
 * 4. Currently, this application does not handle to failures encountered in
 *    L2CAP CoC Data Send due to Low Remote Receive Credits. This handling
 *    needs to be updated later depending on the usage scenario.
 */
/* L2CAP CoC Send Queue related defines and globals for IPSPR */
/* Send Queue, Queue Limit */
#define APPL_IPSPR_SEND_QUEUE_SIZE                 8U
DECL_STATIC APPL_IPSPR_SENDELT appl_ipspr_send[APPL_IPSPR_SEND_QUEUE_SIZE];
DECL_STATIC UCHAR appl_ipspr_sendwr;
DECL_STATIC UCHAR appl_ipspr_sendrd;
/* ------------------------------- Functions */
void ipspr_reverse_bytestream_endianness(UCHAR * src, UCHAR * dst, UINT16 len)
{
    UINT16 i;

    for (i = 0U; i < len; i++)
    {
        dst[i] = src[(len - 1U) - i];
    }
}

void ipspr_notify_gatt_conn (void)
{
    UINT32 i;

    /* Place Holder for connection related handling */
    LOG_DEBUG("\n[IPSPR]: ACL Connected with IPSP Node\n");

    /* Initialize the queue pointers on each connection */
    appl_ipspr_sendrd = 0U;
    appl_ipspr_sendwr = 0U;

    for (i = 0U; i < APPL_IPSPR_SEND_QUEUE_SIZE; i++)
    {
        appl_ipspr_send[i].buf = NULL;
        appl_ipspr_send[i].len = 0U;
    }
}

void ipspr_notify_gatt_disconn (void)
{
    /* Place Holder for Disconnection related handling */
    LOG_DEBUG("\n[IPSPR]: ACL Disconnected with IPSP Node\n");
}

void ipspr_notify_gatt_servdata (GATT_SERVICE_PARAM * service, UINT16 size)
{
    BT_IGNORE_UNUSED_PARAM(size);

    /**
     * Print the Service Discover Handle only when Dedicated IPSP Discovery
     * was Initiated.
     */
    if (BT_TRUE == appl_ipsp_disc_state)
    {
        LOG_DEBUG("[IPSPR]: Discovered IPSP Service in the Node!!\n");
        LOG_DEBUG("[IPSPR]: IPSP Node Service Start Handle: 0x%04X\n",
        service->range.start_handle);
        LOG_DEBUG("[IPSPR]: IPSP Node Service End Handle: 0x%04X\n",
        service->range.end_handle);

        /* Reset the IPSP Discovery State */
        appl_ipsp_disc_state = BT_FALSE;
    }
}

void ipspr_profile_operations (void)
{
    API_RESULT   retval;
    ATT_UUID     uuid;
    UINT32       index;
    unsigned int choice, menu_choice;
    static UINT8 first;

    if (0U == first)
    {
#ifdef IPSP_HAVE_6LO_NIFACE
        BT_DEVICE_ADDR bdaddr;

        /* Get the local BD address */
        BT_hci_get_local_bd_addr(bdaddr.addr);
        bdaddr.type = 0U;

        /* Form the MAC */
        APPL_MAC_LE2BE(bdaddr.addr, lmac.addr);
        lmac.type = bdaddr.type;

        /* INit the network interface */
        ipsp_init_pl(&lmac, lel2ca_ipspr_read, BT_TRUE);
#endif /* IPSP_HAVE_6LO_NIFACE */

        /* Un-Register and then Register Again */
        retval = l2cap_cbfc_unregister_psm(LE_PSM_IPSP);
        /* Ignore Failure during Un-register */

        LOG_DEBUG("[IPSPR]: Registering IPSP-Router with L2CAP CoC\n");
        retval = l2cap_cbfc_register_psm(&appl_ipspr_l2cap_psm);

        if (retval != API_SUCCESS)
        {
            LOG_DEBUG("[IPSPR]: ERROR!!! retval = 0x%04X", retval);
        }
        else
        {
            LOG_DEBUG("[IPSPR]: L2CAP CoC Registration SUCCESS!!\n");

            first = 1U;
        }
    }

    BT_LOOP_FOREVER()
    {
        CONSOLE_OUT(
        "%s",ipspr_client_menu);

        CONSOLE_IN ( "%d",&choice);
        menu_choice = choice;

        switch (choice)
        {
        case 0:
            break; /* return; */

        case 1:
            break;

        case 10:
            uuid.uuid_16 = GATT_INT_PS_SERVICE;
            gatt_discover_ps
            (
                &appl_gatt_client_handle,
                uuid,
                ATT_16_BIT_UUID_FORMAT
            );

            /* Set the IPSP Discovery State */
            appl_ipsp_disc_state = BT_TRUE;
            break;

        case 20:
            retval = l2ca_cbfc_connect_req
                     (
                         &appl_gatt_client_handle.device_id,
                         APPL_L2CAP_LE_PSM,
                         APPL_L2CAP_LE_PSM,
                         &connect_param
                      );

            if (API_SUCCESS != retval)
            {
                LOG_DEBUG("[IPSPR]: L2CAP CoC Connect ERROR!!! retval = 0x%04X\n", retval);
            }
            else
            {
                LOG_DEBUG("[IPSPR]: L2CAP CoC Connect SUCCESS!!\n");
            }
            break;

        case 21:
            retval = l2ca_cbfc_disconnect_req(appl_le_co_cid);

            if (retval != API_SUCCESS)
            {
                LOG_DEBUG("[IPSPR]: L2CAP CoC Disconnect ERROR!!! retval = 0x%04X", retval);
            }
            else
            {
                LOG_DEBUG("[IPSPR]: L2CAP CoC Disconnect SUCCESS!!\n");
            }
            break;

        case 22:
            {
                UINT16 l2cap_cbfc_read_len;
                UINT16 count;

                LOG_DEBUG("Enter the Loop Count of Data Send:\n");
                CONSOLE_IN("%d", &choice);
                count = (UINT16)choice;

                LOG_DEBUG("Enter the Length of Data to be sent:\n");
                CONSOLE_IN("%d", &choice);
                l2cap_cbfc_read_len = (UINT16)choice;

                if (APPL_LE_L2CAP_CBFC_WRITE_BUF_SIZE < l2cap_cbfc_read_len)
                {
                    LOG_DEBUG(
                    "[IPSPR]: Length %d out of Range(1 to 1024)!!\n",
                    l2cap_cbfc_read_len);

                    break;
                }

                /**
                 * Pre-Filling the TX Data Buffer with Incremental Values
                 * from Buffer Index 2.
                 * The First 2 Bytes Holds the Count/Index Tag Value of the
                 * Application Payload.
                 */
                for (index = 2U; index < l2cap_cbfc_read_len; index++)
                {
                    l2cap_cbfc_write_buffer[index] = (UCHAR)(index - 1U);
                }

                for (index = 0U; index < count; index++)
                {
                    /* Add the Index in the first 2 Octets */
                    BT_PACK_LE_2_BYTE_VAL (l2cap_cbfc_write_buffer, (index+1U));

                    retval = appl_ipspr_data_tx
                             (
                                 &appl_peer_handle,
                                 appl_le_co_cid,
                                 l2cap_cbfc_write_buffer,
                                 (UINT16)l2cap_cbfc_read_len
                             );

                    if (API_SUCCESS != retval)
                    {
                        APPL_ERR(
                        "[IPSPR]: **ERR** Unable to send data from appl_ipspr_data_tx(), "
                        "retval = 0x%04X. Queue Full?\n", retval);

                        break;
                    }
                }
            }
            break;
        case 23:
            {
            	const char aipaddr[IP4ADDR_STRLEN_MAX];
            	ip_addr_t addr;
            	CONSOLE_OUT("Enter the Ping address in format x.x.x.x\n");
                scanf("%s\n", aipaddr);
                CONSOLE_OUT("aipaddr %s", aipaddr);
                ipaddr_aton(aipaddr, &addr);
                ipspr_ping(&addr);
            }
            break;
        default:
            LOG_DEBUG("Invalid Option!\n");
            break;
        }

        if (0 == menu_choice)
        {
            /* return */
            break;
        }
    }

    return;
}

/* -- L2CAP CBFC related functions -- */
DECL_STATIC API_RESULT ipspr_l2ca_connect_ind_cb
                       (
                           DEVICE_HANDLE            * handle,
                           UINT16                   lcid,
                           UINT16                   psm,
                           L2CAP_CBFC_CONNECT_PARAM * param
                       )
{
    API_RESULT     retval;
    UINT16         response;
    BT_DEVICE_ADDR bdaddr;
    UCHAR          role;

    LOG_DEBUG(
    "[IPSPR]: ipspr_l2ca_connect_ind_cb from Device %02X, lcid %04X, PSM 0x%04X\n",
    (*handle), lcid, psm);

    LOG_DEBUG(
    "[IPSPR]: MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

    appl_peer_handle = (*handle);
    appl_le_co_cid = lcid;
    appl_le_co_peer_mtu = param->mtu;

    retval = API_SUCCESS;
    role   = 0xFFU;

#ifdef APPL_LE_L2CAP_CONN_AUTO_ACCEPT
    /**
     * Check for the Current Link Layer Connection Role.
     *
     * Spec   : INT_IP_Support_Profile_SPEC_V1.0.0.pdf
     * Section: 6. Connection Establishment Procedure
     *
     * "In order to avoid the establishment of two simultaneous L2CAP channels
     *  between two devices, where both support Router and Node roles, the
     *  Router acting as the link layer Master on the given link shall refuse
     *  an LE Credit Based Connection Request packet which has the LE_PSM
     *  field set to the value for IPSP defined in the Bluetooth Assigned
     *  Numbers [2]."
     */
    /* Get the remote bd address */
    retval = device_queue_get_remote_addr(handle, &bdaddr);

    /* Get the Current Connection role */
    retval = BT_hci_le_get_device_role(&bdaddr, &role);

    if (BT_DEVICE_ROLE_MASTER == role)
    {
        /**
         * Setting Error Code for Connection Reject as
         * Unacceptable Parameters.
         * Need to check if there is a specific error response
         * suitable for this scenario.
         */
        response = L2CAP_CONNECTION_REFUSED_UNACCEPTABLE_PARAMETERS;
    }
    else
    {
        /* Accept the Connection */
        response = L2CAP_CONNECTION_SUCCESSFUL;
    }

    retval = l2ca_cbfc_connect_rsp
             (
                 lcid,
                 response,
                 &connect_param
             );
#endif /* APPL_LE_L2CAP_CONN_AUTO_ACCEPT */

    /**
     * The below stored Peer MTU is are currently not used,
     * but this variable is retained for reference.
     */
    BT_IGNORE_UNUSED_PARAM(appl_le_co_peer_mtu);

    return retval;
}

DECL_STATIC API_RESULT ipspr_l2ca_connect_cnf_cb
                       (
                           DEVICE_HANDLE* handle,
                           UINT16                     lcid,
                           UINT16                     response,
                           L2CAP_CBFC_CONNECT_PARAM* param
                       )
{
#ifdef IPSP_HAVE_6LO_NIFACE
    BT_DEVICE_ADDR bdaddr, mac;
#endif /* IPSP_HAVE_6LO_NIFACE */

    LOG_DEBUG(
    "[IPSPR]: ipspr_l2ca_connect_cnf_cb from Device %02X, lcid %04X, response 0x%04X\n",
    (*handle), lcid, response);

    /* Print MTU, MPS and Credit, on success response */
    if (L2CAP_CONNECTION_SUCCESSFUL == response)
    {
        LOG_DEBUG(
        "[IPSPR]: MTU:0x%04X MPS:0x%04X Credit:0x%04X\n", param->mtu, param->mps, param->credit);

        appl_peer_handle = (*handle);
        appl_le_co_cid = lcid;
        appl_le_co_peer_mtu = param->mtu;

#ifdef IPSP_HAVE_6LO_NIFACE
        if (LE_PSM_IPSP == appl_ipspr_l2cap_psm.psm)
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

    }

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipspr_l2ca_disconnect_ind_cb(UINT16 lcid)
{
    LOG_DEBUG("[IPSPR]: ipspr_l2ca_disconnect_ind_cb, lcid %04X\n", lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000U;

#ifdef IPSP_HAVE_6LO_NIFACE
    if (LE_PSM_IPSP == appl_ipspr_l2cap_psm.psm)
    {
        /* Stop the network interface */
        ipsp_stop_pl();
    }
#endif /* IPSP_HAVE_6LO_NIFACE */

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipspr_l2ca_disconnect_cnf_cb(UINT16 lcid, UINT16 reason)
{
    BT_IGNORE_UNUSED_PARAM(reason);

    LOG_DEBUG("[IPSPR]: ipspr_l2ca_disconnect_cnf_cb lcid %04X\n", lcid);

    /* RESET peer MTU */
    appl_le_co_peer_mtu = 0x0000U;

#ifdef IPSP_HAVE_6LO_NIFACE
    if (LE_PSM_IPSP == appl_ipspr_l2cap_psm.psm)
    {
        /* Stop the network interface */
        ipsp_stop_pl();
    }
#endif /* IPSP_HAVE_6LO_NIFACE */

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipspr_l2ca_data_read_cb(UINT16 lcid, UINT16 result, UCHAR* data, UINT16 datalen)
{
    API_RESULT retval;
    UINT16 no_of_credits;

    BT_IGNORE_UNUSED_PARAM(result);

#ifdef APPL_IPSPR_HAVE_CB_DBG_PRINTS
    LOG_DEBUG(
    "[IPSPR]: ipspr_l2ca_data_read_cb, lcid %04X, data len %04X, result 0x%04X\n",
    lcid, datalen, result);

    appl_dump_bytes(data, datalen);
#endif /* APPL_IPSPR_HAVE_CB_DBG_PRINTS */

    retval = API_SUCCESS;

#ifdef APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT
    /**
     * Calculate possible number of LE L2CAP Frames.
     *
     * Maximum size of each frame shall not exceed local MPS.
     * Account for the 2 octet of SDU length.
     */
    no_of_credits = (datalen + 2U + APPL_LE_CO_MPS - 1U) / APPL_LE_CO_MPS;

    retval = l2ca_cbfc_send_flow_control_credit
             (
                 lcid,
                 no_of_credits
             );
#endif /* APPL_LE_L2CAP_AUTO_SEND_INSTANT_CREDIT */

#ifdef IPSP_HAVE_6LO_NIFACE
    ipsp_write_pl(data, datalen);
#else /* IPSP_HAVE_6LO_NIFACE */
    BT_IGNORE_UNUSED_PARAM(data);
#endif /* IPSP_HAVE_6LO_NIFACE */

    return retval;
}

DECL_STATIC API_RESULT ipspr_l2ca_low_rx_credit_ind_cb(UINT16 lcid, UINT16 credit)
{
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(credit);

#ifdef APPL_IPSPR_HAVE_CB_DBG_PRINTS
    LOG_DEBUG(
    "[IPSPR]: ipspr_l2ca_low_rx_credit_ind_cb, lcid %04X, Credit %04X\n",
    lcid, credit);
#endif /* APPL_IPSPR_HAVE_CB_DBG_PRINTS */

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

DECL_STATIC API_RESULT ipspr_l2ca_tx_credit_ind_cb(UINT16 lcid, UINT16 result, UINT16 credit)
{

    BT_IGNORE_UNUSED_PARAM(credit);
    BT_IGNORE_UNUSED_PARAM(result);
    BT_IGNORE_UNUSED_PARAM(lcid);

#ifdef APPL_IPSPR_HAVE_CB_DBG_PRINTS
    LOG_DEBUG(
    "[IPSPR]: ipspr_l2ca_tx_credit_cb, lcid %04X, Credit %04X\n",
    lcid, credit);
#endif /* APPL_IPSPR_HAVE_CB_DBG_PRINTS */

    return API_SUCCESS;
}

DECL_STATIC API_RESULT ipspr_l2ca_data_write_cb
            (
                UINT16 lcid,
                UINT16 result,
                UCHAR  * buffer,
                UINT16 buffer_len
            )
{
    API_RESULT retval;

    BT_IGNORE_UNUSED_PARAM(lcid);
    BT_IGNORE_UNUSED_PARAM(result);
    BT_IGNORE_UNUSED_PARAM(buffer);
    BT_IGNORE_UNUSED_PARAM(buffer_len);

#ifdef APPL_IPSPR_HAVE_CB_DBG_PRINTS
    LOG_DEBUG(
    "[IPSPR]: ipspr_l2ca_data_write_cb, lcid %04X, Result %04X\n",
    lcid, result);
#endif /* APPL_IPSPR_HAVE_CB_DBG_PRINTS */

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
    BT_free_mem(appl_ipspr_send[appl_ipspr_sendrd].buf);
    appl_ipspr_send[appl_ipspr_sendrd].buf = NULL;

    if (API_SUCCESS != result)
    {
        return API_SUCCESS;
    }

    /* Move to the Next Queue element */
    appl_ipspr_sendrd++;
    appl_ipspr_sendrd &= (APPL_IPSPR_SEND_QUEUE_SIZE - 1U);

    if ((appl_ipspr_sendrd != appl_ipspr_sendwr) &&
        (NULL != appl_ipspr_send[appl_ipspr_sendrd].buf))
    {
        retval = l2ca_channel_data_write
                 (
                     &appl_peer_handle,
                     appl_le_co_cid,
                     appl_ipspr_send[appl_ipspr_sendrd].buf,
                     appl_ipspr_send[appl_ipspr_sendrd].len
                 );

        if (API_SUCCESS != retval)
        {
            /* Write Failed */
            APPL_TRC(
            "[IPSPR]: L2CAP Channel Data Write failed - 0x%04x\n",
            retval);

            /* Free the previously allocated memory */
            BT_free_mem(appl_ipspr_send[appl_ipspr_sendrd].buf);
        }
    }

    return API_SUCCESS;
}

#ifdef IPSP_HAVE_6LO_NIFACE
void lel2ca_ipspr_read(UCHAR *edata, UINT16 edatalen)
{
    API_RESULT retval;

    /* Invoke the application data send function */
    retval = appl_ipspr_data_tx
             (
                 &appl_peer_handle,
                 appl_le_co_cid,
                 edata,
                 edatalen
             );

    if (API_SUCCESS != retval )
    {
        APPL_ERR(
        "[IPSPR]: **ERR** lel2ca_ipspr_read L2CAP CoC Write failed - 0x%04x\n",
        retval);
    }

    return;
}
#endif /* IPSP_HAVE_6LO_NIFACE */

DECL_STATIC API_RESULT appl_ipspr_data_tx
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
    if (NULL != appl_ipspr_send[appl_ipspr_sendwr].buf)
    {
        APPL_ERR (
        "[IPSPR]: **ERR** L2CAP CoC Send Queue Full![Queue Size: %d]\n",
        APPL_IPSPR_SEND_QUEUE_SIZE);

        /**
         * TODO:
         * Have a Specific Error Code here?
         */
        return retval;
    }

    /* Allocate Memory for the buffer */
    buf = BT_alloc_mem(edatalen);

    if (NULL == buf)
    {
        APPL_ERR(
        "[IPSPR]: **ERR** Failed to allocate buffer of length %d\n", edatalen);

        return retval;
    }

    /* Reinitialize Retval */
    retval = API_SUCCESS;

    /* Copy the data */
    BT_mem_copy(buf, edata, edatalen);

    /* Add it to the queue */
    appl_ipspr_send[appl_ipspr_sendwr].buf = buf;
    appl_ipspr_send[appl_ipspr_sendwr].len = edatalen;

    /**
     * Is Tx ongoing?
     * Add to Queue if ongoing, else send the data
     * over L2CAP Channel.
     */
    if (appl_ipspr_sendwr == appl_ipspr_sendrd)
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
            "[IPSPR]: **ERR** L2CAP Channel Data Write failed - 0x%04x\n",
            retval);

            /* Free the previously allocated memory */
            BT_free_mem(buf);
        }
    }

    /* Add to Queue */
    if (API_SUCCESS == retval)
    {
        appl_ipspr_sendwr++;
        appl_ipspr_sendwr &= (APPL_IPSPR_SEND_QUEUE_SIZE - 1U);
    }

    return retval;
}
#endif /* (defined ATT && defined IPSPR) */
