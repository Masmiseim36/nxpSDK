
/**
 *  \file hps_pl.c
 *
 *  This File contains the platform abstraction routines of LE HTTP Proxy
 *  Service to Transmit and Receive Data over TCP/IP protocol.
 */

/*
 *  Copyright (C) 2013. Mindtree Ltd.
 *  All rights reserved.
 */


/* ----------------------------------------- Header File Inclusion */
#include "hps_pl.h"

/* --------------------------------------------- Global Definitions */

/* Maximum number of bytes to read from TCP Socket port */
/**
 * NOTE: The Buffer size is kept to 2K as the Characteristic Value
 * Size for both Headers and Entity body is only around 512 each.
 * Keep the extra as cushion, which can be trimmed if needed.
 */
#define HPS_BUF_SIZE_PL                     2048U

/* Global HTTP Port Definition */
#define HTTP_PORT                           80U

/* ----------------------------------------- Function Declarations */
/** HPS Transport Read Task */
static BT_THREAD_RETURN_TYPE hps_read_task_pl (BT_THREAD_ARGS args);

/* ----------------------------------------- External Global Variables */
#if 0
extern void lwip_ping_netif_init(void);
extern void lwip_ping_dhcp(void);
#endif /* 0 */

/* ----------------------------------------- Exported Global Variables */

/* ----------------------------------------- Static Global Variables */
/** HPS - TCP Socket Descriptor */
static int hps_pl_sock_fd;

/** HPS - TCP Socket Peer Address */
static struct sockaddr_in peer_addr;

/** HPS - TCP Socket Read Task State */
static UCHAR hps_transport_state_pl;

/* Socket Read Task Synchronization */
BT_DEFINE_MUTEX(hps_sock_mutex)
BT_DEFINE_COND(hps_sock_cond)

/** Buffer to Read the Incoming Data */
static UCHAR hps_sock_read_buf [HPS_BUF_SIZE_PL];

static HPS_TRANSPORT_CB_PL hps_appl_cb = NULL;

/* ----------------------------------------- Functions */
/** HPS PL interface for transport Initialization */
API_RESULT hps_transport_init_pl
           (
               HPS_TRANSPORT_CB_PL cb
           )
{
    BT_thread_type      tid;
    API_RESULT          retval;
    BT_thread_attr_type hps_task_attr;

    niface_stack_setup();
    /*Not applicable for RW610,can not init successful*/
#if !defined(RW610_SERIES) && !defined(RW612_SERIES)
    niface_setup();
#endif

    /* Return if transport already initialized */
    if (NULL != hps_appl_cb)
    {
        HPS_PL_TRC(
        "[HPS-PL] HPS Transport Already INitialized!\n");

        return API_FAILURE;
    }

    /* Initialize */
    retval = API_FAILURE;

    BT_MUTEX_INIT (hps_sock_mutex, HPS_PL);
    BT_COND_INIT  (hps_sock_cond , HPS_PL);

    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&hps_sock_mutex);

    /* Check for NULL */
    if (NULL != cb)
    {
        /* Store the application callback in global */
        hps_appl_cb = cb;

        retval = API_SUCCESS;

        HPS_PL_TRC(
        "[HPS-PL] HPS Transport RX Callback Registration Success!\n");
    }
    else
    {
        HPS_PL_ERR(
        "[HPS-PL] HPS Transport RX Callback Registration Failed!\n");

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&hps_sock_mutex);

        return retval;
    }

    /* Initialize HPS Transport Socket State */
    hps_transport_state_pl = 0x0U;

    HPS_PL_TRC(
    "[HPS-PL] Initialized Platform LWIP Stack and DHCP.\n");

    /* Initialize the HPS Task Attributes */
    hps_task_attr.thread_name       = (DECL_CONST CHAR *)"EtherMind HPS Task";
    hps_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    hps_task_attr.thread_priority   = BT_TASK_PRIORITY;

    /* Create a thread to receive data from Socket and BUFFER it */
    if (BT_thread_create(&tid, &hps_task_attr, hps_read_task_pl, NULL) != 0U)
    {
        HPS_PL_ERR(
        "[HPS-PL] Could NOT Create HPS Transport Read Thread\n");

        retval = API_FAILURE;

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&hps_sock_mutex);

        return retval;
    }

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&hps_sock_mutex);

    HPS_PL_ERR(
    "[HPS-PL] HPS Transport Initialization Complete, retval 0x%04X!\n",
    retval);

    return retval;
}

/** HPS PL interface to Open TCP-IP Transport */
API_RESULT hps_transport_open_pl (UCHAR * ip_addr)
{
    API_RESULT retval;
    INT32 ret, hc_port;

    /* Initialize the Read Buffer */
    BT_mem_set(hps_sock_read_buf, 0x0, sizeof(hps_sock_read_buf));

    /* Connect to Remote Server on port number HTTP_PORT */
    hc_port = HTTP_PORT;

    /* Create Socket connection to remote server with port HTTP_PORT */
    hps_pl_sock_fd = socket
                     (
                         AF_INET,
                         SOCK_STREAM,
                         0U
                     );

    if (0 > hps_pl_sock_fd)
    {
        HPS_PL_ERR(
        "[HPS-PL] FAILED to Create TCP Socket\n");

        return API_FAILURE;
    }

    /* Prepare Socket Address */
    BT_mem_set (&peer_addr, 0, sizeof (peer_addr));
    peer_addr.sin_family      = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr ((const char *)ip_addr);
    peer_addr.sin_port        = htons((UINT16)hc_port);

    HPS_PL_INF(
    "[HPS-PL] HPS Transport connecting to HTTP Server\n");

    /* Connect to remote server */
    ret = connect
          (
              hps_pl_sock_fd,
              (struct sockaddr *) &peer_addr,
              sizeof (peer_addr)
          );

    if (0U != ret)
    {
        HPS_PL_ERR(
        "[HPS-PL] FAILED to Connect to HTTP Server\n");

        return API_FAILURE;
    }
    else
    {
        HPS_PL_INF(
        "[HPS-PL] Connected to HTTP Server\n");

        retval = API_SUCCESS;
    }

    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&hps_sock_mutex);

    /* Set the transport State */
    hps_transport_state_pl = 0x1U;

    /* Signal the waiting Task */
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal(&hps_sock_cond);

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&hps_sock_mutex);

    HPS_PL_INF(
    "[HPS-PL] HTTP Server Communication Established\n");

    return retval;
}

/** HPS PL interface to Close TCP-IP Transport */
void hps_transport_close_pl (void)
{
    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hps_sock_mutex);

    /* Reset the transport State */
    hps_transport_state_pl = 0x0U;

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hps_sock_mutex);

    /* Wait for HPS Transport Read Task to Exit */
    BT_usleep (100U);

    /* Close the socket connection */
    (BT_IGNORE_RETURN_VALUE) closesocket(hps_pl_sock_fd);

    /* Shutdown TCP Client Socket */
    (BT_IGNORE_RETURN_VALUE) shutdown(hps_pl_sock_fd, 2U);

    HPS_PL_INF(
    "[HPS-PL] HTTP Server Communication Destroyed!\n");
}

/** HPS Transport Read Task */
static BT_THREAD_RETURN_TYPE hps_read_task_pl (BT_THREAD_ARGS args)
{
    int                nbytes;
    UCHAR              send_data;
    DECL_STATIC UINT32 marker;
    int                to_rd_len;

    BT_IGNORE_UNUSED_PARAM(args);

    marker    = 0U;
    to_rd_len = HPS_BUF_SIZE_PL;

    BT_LOOP_FOREVER()
    {
        /* Lock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hps_sock_mutex);

        if (0x0U == hps_transport_state_pl)
        {
            HPS_PL_TRC(
            "[HPS-PL] HPS Transport Read Task: Waiting to Service ..\n");

            (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait
            (
                &hps_sock_cond,
                &hps_sock_mutex
            );

            HPS_PL_TRC(
            "[HPS-PL] HPS Transport Read Task: Ready to Service ..\n");

            /* Initialize the variables when ready to read afresh */
            marker    = 0U;
            to_rd_len = HPS_BUF_SIZE_PL;
        }

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hps_sock_mutex);

        if (0x1U == hps_transport_state_pl)
        {
            send_data = BT_TRUE;

            /* Read Data from Socket */
            nbytes = recv
                     (
                         hps_pl_sock_fd,
                         &((char *)hps_sock_read_buf)[marker],
                         to_rd_len,
                         0U
                     );

            /**
             * All possible outcomes from the above recv function:
             *
             * 1. nbtes   < 0
             * 2. nbytes == 0
             * 3. nbytes > 0 but less than the needed length
             * 4. nbytes == needed length
             * 5. nbytes == Max requested Length i.e. HPS_BUF_SIZE_PL
             *
             * case 1. [nbytes  < 0] and
             * case 2. [nbytes == 0]
             *   - call close transport but do not return from the task
             *
             * case 3. [nbytes > 0 but less than needed length] and
             * case 4. [nbytes == needed length]
             *   - Parse and find out what is the needed length.
             *     Try to search till "Content Length" in header and read
             *     remaining Bytes from there.
             *   - Currently the approach is to Loop through and try to read
             *     from the socket transport until the "nbytes" values received
             *     is ZERO.
             *
             * case 5. [nbytes == Max requested Length i.e. HPS_BUF_SIZE_PL]
             *   - Pass the Buffer to the application callback
             *
             * NOTE:
             * Implementations that wishes to use some other dedicate HTTP
             * Parsers modules can plug in here, directly by replacing the
             * below section.
             */
            /* Case 1 and Case 2 */
            if (0U >= nbytes)
            {
                /**
                 * Socket read failed, the data returned from transport
                 * is either Zero or the data receive operation failed.
                 */
                HPS_PL_ERR (
                "[HPS-PL] Socket Read either returned 0 or Failure. nbytes = %d\n",
                nbytes);

                /**
                 * Check if the error occurred in the first read.
                 * If there is something already available, pass that
                 * to upper layer.
                 * Else, pass ZERO(same as current marker) Bytes to the upper layer.
                 */
                if (0U == marker)
                {
                    HPS_PL_ERR (
                    "[HPS-PL] Socket Failure occured during first read! nbytes = %d\n",
                    nbytes);
                }

                /* Give the callback to the application */
            }
            else if (HPS_BUF_SIZE_PL == nbytes)
            {
                HPS_PL_INF (
                "[HPS-PL] Received %d Bytes from socket recv\n", nbytes);

                marker = nbytes;

                HPS_PL_TRC (
                "[HPS-PL] Marking send data to UL as TRUE. Current Marker is: %d\n",
                marker);

                /* do nothing, entire buf to be given to callback */
            }
            else
            {
                /**
                 * Keep Appending the Read Data to the Buffer, and Read from Socket
                 * till a failure is received or the bytes read count reaches 0
                 */
                HPS_PL_INF (
                "[HPS-PL] Before processing: nbytes=%d; marker =%d, to_rd=%d\n",
                nbytes, marker, to_rd_len);

                marker    += nbytes;
                to_rd_len  = ((HPS_BUF_SIZE_PL) - marker);

                HPS_PL_INF (
                "[HPS-PL] After processing: nbytes=%d; marker =%d, to_rd=%d\n",
                nbytes, marker, to_rd_len);

                /* Loop again and try to read remaning bytes from Transport */
                send_data = BT_FALSE;
            }

            if (BT_TRUE == send_data)
            {
                /**
                 * Typically at this point, marker and the String Length of the
                 * Read Buffer should be either same or Marker should be 0.
                 */
                /**
                 * Send received http response from the web server
                 * to the application
                 */
                if (NULL != hps_appl_cb)
                {
                    HPS_PL_INF (
                    "[HPS-PL] Total Bytes to Send to UL is %d, Buffer is \n%s\n",
                    marker, hps_sock_read_buf);

                    (BT_IGNORE_RETURN_VALUE) hps_appl_cb
                    (
                        hps_sock_read_buf,
                        (UINT16) marker
                    );
                }

                /* Reset Send Data Flag */
                send_data = BT_FALSE;

                /* Reset the Read Buffer */
                BT_mem_set(hps_sock_read_buf, 0x0, sizeof(hps_sock_read_buf));
            }
        }
    }
}

/** HPS PL interface to send data over transport */
API_RESULT hps_transport_write_data_pl
           (
               UCHAR  * data,
               UINT16 data_len
           )
{
    API_RESULT retval;

    retval = API_SUCCESS;

    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&hps_sock_mutex);

    /* Send data to socket */
    hps_socket_write_data_pl
    (
        data,
        data_len
    );

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&hps_sock_mutex);

    return retval;
}

/* HPS PL Internal Functions to Send Data */
void hps_socket_write_data_pl (UCHAR *buf, UINT16 length)
{
    UCHAR * tmp_ptr;
    UINT32 written;
    int nbytes;

    written = 0U;
    tmp_ptr = buf;
    while (written < length)
    {
        nbytes = send
                 (
                     hps_pl_sock_fd,
                     (const char *)tmp_ptr,
                     (length - written),
                     0U
                 );

        if (nbytes < 0)
        {
            return;
        }

        tmp_ptr += nbytes;
        written += nbytes;
    }
    HPS_PL_INF(
    "[HPS-PL] Written %d total bytes to Transport\n", length);
}

API_RESULT hps_fetch_ip_from_hostname_pl
           (
               UCHAR * hostname,
               UCHAR * ip
           )
{
    API_RESULT  retval;
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    retval = API_SUCCESS;

    if ((he = gethostbyname((const char *)hostname)) == NULL)
    {
        HPS_PL_TRC("[HPS-PL]gethostbyname failed : %d\n", h_errno);

        return API_FAILURE;
    }

    /**
     * Cast the h_addr_list to in_addr , since h_addr_list also has the
     *  ip address in long format only
     */
    addr_list = (struct in_addr **)(he->h_addr_list);

    for(i = 0U; addr_list[i] != NULL; i++)
    {
        /* Return the first one */
        BT_str_copy(ip, inet_ntoa(*addr_list[i]));
    }

    return retval;
}

