// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// - Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// - Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// - Neither the name of nor the names of its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE. THIS SOFTWARE IS
// PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
// BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//==============================================================================
// Author(s): ="Atheros"
//==============================================================================

#include "main.h"
#include <string.h>
#include <stdlib.h>
#include "throughput.h"
#include <atheros_stack_offload.h>
#include "atheros_wifi.h"
#include "atheros_wifi_api.h"
#include "qcom_api.h"

#if ENABLE_STACK_OFFLOAD
#define CLIENT_WAIT_TIME 30000

#define OFFSETOF(type, field) ((size_t)(&((type *)0)->field))

#if ENABLE_SSL
/* Include certificate used by benchrx ssl test*/
#include "cert-kingfisher-inc.h"

/* Include CA list used by benchtx ssl test*/
#include "calist-kingfisher-inc.h"

#define CERT_HEADER_LEN sizeof(CERT_HEADER_T)

typedef struct
{
    char id[4];
    uint32_t length;
    uint8_t data[1]; // NOTE: previously 0
} CERT_HEADER_T;

SSL_INST ssl_inst[MAX_SSL_INST];
uint8_t *ssl_cert_data_buf     = NULL;
uint16_t ssl_cert_data_buf_len = 0;

uint8_t const *ssl_default_cert       = (uint8_t *)sharkSslRSACertKingfisher;
const uint16_t ssl_default_cert_len   = sizeof(sharkSslRSACertKingfisher);
uint8_t const *ssl_default_calist     = (uint8_t *)sharkSslCAList;
const uint16_t ssl_default_calist_len = sizeof(sharkSslCAList);

SSL_ROLE_T ssl_role;
#endif // ENABLE_SSL

/**************************Globals *************************************/
static uint32_t num_traffic_streams = 0; // Counter to monitor simultaneous traffic streams
static uint32_t port_in_use         = 0; // Used to prevent two streams from using same port

extern unsigned char v6EnableFlag;
uint8_t bench_quit = 0;

/*TCP receive timeout*/
const int rx_timeout = ATH_RECV_TIMEOUT;

unsigned char v6EnableFlag = 0;
/************************************************************************
 *     Definitions.
 *************************************************************************/

#define CFG_COMPLETED_STR "IOT Throughput Test Completed" NL
#define CFG_REPORT_STR "."
#define PRINT_SHELL_PROMPT /* PRINTF("shell> "); */

/*************************** Function Declarations **************************/
extern void app_time_get(TIME_STRUCT *time);
// extern void qosal_time_get_elapsed(TIME_STRUCT* time);

static void ath_tcp_tx(THROUGHPUT_CXT *p_tCxt);
static void ath_udp_tx(THROUGHPUT_CXT *p_tCxt);
uint32_t check_test_time(THROUGHPUT_CXT *p_tCxt);
void sendAck(THROUGHPUT_CXT *p_tCxt, A_VOID *address, uint16_t port);
static void ath_tcp_rx(THROUGHPUT_CXT *p_tCxt);
static void ath_udp_rx(THROUGHPUT_CXT *p_tCxt);

unsigned short int ip_flags[4];
#if ENABLE_RAW_SOCKET_DEMO
void sendRawAck(THROUGHPUT_CXT *p_tCxt, A_VOID *local_address, A_VOID *address, int16_t hdrinc, uint16_t protocol);
static void ath_raw_rx(THROUGHPUT_CXT *p_tCxt);
static void ath_raw_tx(THROUGHPUT_CXT *p_tCxt);
#endif
#if MULTI_SOCKET_SUPPORT
#define UDP_MULTI_SOCKET_MAX (MAX_SOCKETS_SUPPORTED)

/// Redcucing the number of sockets with multisockets as we are running out of stack space in the demo application
#define TCP_MULTI_SOCKET_MAX 4 //(MAX_SOCKETS_SUPPORTED

static int ath_create_socket_and_bind(uint32_t *socket, int16_t port, int32_t type);
static int ath_create_socket_and_bind6(uint32_t *socket, int16_t port, int32_t type);
static void ath_udp_rx_multi_socket(int16_t *port, int32_t multi_socket_count);
static void ath_tcp_rx_multi_socket(int16_t *port, int32_t multi_socket_count);
static void ath_tcp_tx_multi_socket(THROUGHPUT_CXT *p_tCxt, int32_t multi_socket_count);
#endif
int32_t test_for_delay(TIME_STRUCT *pCurr, TIME_STRUCT *pBase);
int32_t receive_incoming(THROUGHPUT_CXT *p_tCxt, int32_t *incoming_connections, uint16_t *num_connections);
int32_t handle_mcast_param(THROUGHPUT_CXT *p_tCxt);
int32_t wait_for_response(THROUGHPUT_CXT *p_tCxt, SOCKADDR_T foreign_addr, SOCKADDR_6_T foreign_addr6);
extern int Inet6Pton(char *src, IP6_ADDR_T *ipv6);

/************************************************************************
 * NAME: test_for_quit
 *
 * DESCRIPTION:
 * Parameters: none
 ************************************************************************/
int32_t test_for_quit(void)
{
    return bench_quit;
}

int32_t setBenchMode(int32_t argc, char_ptr argv[])
{
    if (argc < 2)
    {
        PRINTF("Missing Parameters, specify v4 or v6" NL);
        return A_ERROR;
    }

    if (ATH_STRCMP((char const *)argv[1], "v4") == 0)
    {
        v6EnableFlag = 0;
    }
    else if (ATH_STRCMP((char const *)argv[1], "v6") == 0)
    {
        v6EnableFlag = 1;
    }
    else
    {
        PRINTF("Invalid parameter, specify v4 or v6" NL);
        return A_ERROR;
    }
    return A_OK;
}

static void print_header_start(const char *text)
{
    PRINTF("**********************************************************" NL);
    PRINTF("%s" NL, text);
    PRINTF("**********************************************************" NL);
}

static void print_header_end(void)
{
    PRINTF("Type benchquit to terminate test" NL);
    PRINTF("****************************************************" NL);
}

/************************************************************************
 * NAME: print_test_results
 *
 * DESCRIPTION: Print throughput results
 ************************************************************************/
static void print_test_results(THROUGHPUT_CXT *p_tCxt)
{
    /* Print throughput results.*/
    uint32_t throughput     = 0;
    uint32_t total_bytes    = 0;
    uint32_t total_kbytes   = 0;
    uint32_t sec_interval   = (p_tCxt->last_time.SECONDS - p_tCxt->first_time.SECONDS);
    uint32_t ms_interval    = (p_tCxt->last_time.MILLISECONDS - p_tCxt->first_time.MILLISECONDS);
    uint32_t total_interval = sec_interval * 1000 + ms_interval;

    p_tCxt->last_interval = total_interval;

    if (total_interval > 0)
    {
        /*No test was run, or test is terminated, print results for previous test*/
        if (p_tCxt->bytes == 0 && p_tCxt->kbytes == 0)
        {
            /* Take care of wrap around cases. If number of kbytes exceeds
          0x7ffff, it exceeds 32 bits and wraps around resulting in wrong
          throughput number */
            if (p_tCxt->last_kbytes < 0x7FFFF)
            {
                throughput = ((p_tCxt->last_kbytes * 1024 * 8) / (total_interval)) +
                             ((p_tCxt->last_bytes * 8) / (total_interval));
            }
            else
            {
                /* Convert bytes to kb and divide by seconds for this case */
                throughput =
                    ((p_tCxt->last_kbytes * 8) / (sec_interval)) + ((p_tCxt->last_bytes * 8 / 1024) / (sec_interval));
            }

            // throughput = (p_tCxt->last_kbytes*1024*8)/(total_interval) + (p_tCxt->last_bytes*8)/(total_interval);
            total_bytes  = p_tCxt->last_bytes;
            total_kbytes = p_tCxt->last_kbytes;

            /*Reset previous test data*/
            p_tCxt->last_bytes  = 0;
            p_tCxt->last_kbytes = 0;
        }

        else
        {
            /* Take care of wrap around cases. If number of kbytes exceeds
          0x7ffff, it exceeds 32 bits and wraps around resulting in wrong
          throughput number */
            if (p_tCxt->kbytes < 0x7FFFF)
            {
                throughput =
                    ((p_tCxt->kbytes * 1024 * 8) / (total_interval)) + ((p_tCxt->bytes * 8) / (total_interval));
            }
            else
            {
                /* Convert bytes to kb and divide by seconds for this case */
                throughput = ((p_tCxt->kbytes * 8) / (sec_interval)) + ((p_tCxt->bytes * 8 / 1024) / (sec_interval));
            }

            // throughput = (p_tCxt->kbytes*1024*8)/(total_interval) + (p_tCxt->bytes*8)/(total_interval);
            total_bytes         = p_tCxt->bytes;
            total_kbytes        = p_tCxt->kbytes;
            p_tCxt->last_bytes  = p_tCxt->bytes;
            p_tCxt->last_kbytes = p_tCxt->kbytes;
        }
    }
    else
    {
        throughput = 0;
    }

    switch (p_tCxt->test_type)
    {
        case UDP_TX:
            PRINTF(NL "Results for %s test:" NL NL, "UDP Transmit");
            break;

        case UDP_RX:
            PRINTF(NL "Results for %s test:" NL NL, "UDP Receive");
            break;

        case TCP_TX:
            PRINTF(NL "Results for %s test:" NL NL, "TCP Transmit");
            break;

        case TCP_RX:
            PRINTF(NL "Results for %s test:" NL NL, "TCP Receive");
            break;

#if ENABLE_RAW_SOCKET_DEMO
        case RAW_TX:
            PRINTF(NL "Results for %s test:" NL NL, "RAW Transmit");
            break;

        case RAW_RX:
            PRINTF(NL "Results for %s test:" NL NL, "RAW Receive");
            break;
#endif
        case SSL_TX:
            PRINTF(NL "Results for %s test:" NL NL, "SSL Transmit");
            break;

        case SSL_RX:
            PRINTF(NL "Results for %s test:" NL NL, "SSL Receive");
            break;
    }

    PRINTF("\t%d KBytes %d bytes in %d seconds %d ms  " NL NL, (int)total_kbytes, (int)total_bytes,
           (int)total_interval / 1000, (int)total_interval % 1000);
    PRINTF("\t throughput %d kb/sec" NL, (int)throughput);
}

/************************************************************************
 * NAME: tx_command_parser
 *
 * DESCRIPTION:
 ************************************************************************/
int32_t tx_command_parser(int32_t argc, char_ptr argv[])
{ /* Body */
    bool print_usage    = 0;
    int32_t return_code = A_OK;
    THROUGHPUT_CXT tCxt;
    int32_t retval          = -1;
    int32_t max_packet_size = 0;

    memset(&tCxt, 0, sizeof(THROUGHPUT_CXT));
    if (argc < 8)
    {
        /*Incorrect number of params, exit*/
        return_code = A_ERROR;
        print_usage = TRUE;
    }
    else
    {
        if (v6EnableFlag)
        {
            /*Get IPv6 address of Peer*/
            retval = Inet6Pton(argv[1], (IP6_ADDR_T *)tCxt.params.tx_params.v6addr); /* server IP*/
            if (retval == 0)
                retval = -1;
        }
        else
        {
            /*Get IPv4 address of Peer*/
            retval = ath_inet_aton(argv[1], (uint32_t *)&tCxt.params.tx_params.ip_address);
        }
        if (retval == -1)
        {
            /* Port.*/
            tCxt.params.tx_params.port = atoi(argv[2]);

            /*Check port validity*/
            if (port_in_use == tCxt.params.tx_params.port)
            {
                PRINTF("This port is in use, use another Port" NL);
                return_code = A_ERROR;
                goto ERROR;
            }

            port_in_use = tCxt.params.tx_params.port;

            /* Packet size.*/
            tCxt.params.tx_params.packet_size = atoi(argv[4]);
            if (!v6EnableFlag)
                max_packet_size = CFG_PACKET_SIZE_MAX_TX;
            else
                max_packet_size = 1230;

            if ((tCxt.params.tx_params.packet_size == 0) || (tCxt.params.tx_params.packet_size > max_packet_size))
            {
                if (!v6EnableFlag)
                    PRINTF("Invalid packet length for v4, allowed %ld" NL, max_packet_size); /* Print error mesage. */
                else
                    PRINTF("Invalid packet length for v6, allowed %ld" NL, max_packet_size); /* Print error mesage. */

                return_code = A_ERROR;
                print_usage = TRUE;
                goto ERROR;
            }

            /* Test mode.*/
            tCxt.params.tx_params.test_mode = atoi(argv[5]);

            if (tCxt.params.tx_params.test_mode != 0 && tCxt.params.tx_params.test_mode != 1)
            {
                PRINTF("Invalid test mode, please enter 0-time or 1-number of packets" NL); /* Print error mesage. */
                return_code = A_ERROR;
                print_usage = TRUE;
                goto ERROR;
            }

            /* Number of packets OR time period.*/
            if (tCxt.params.tx_params.test_mode == 0)
            {
                tCxt.params.tx_params.tx_time = atoi(argv[6]);
            }
            else if (tCxt.params.tx_params.test_mode == 1)
            {
                tCxt.params.tx_params.packet_number = atoi(argv[6]);
            }

            /* Inter packet interval for Bandwidth control*/
            tCxt.params.tx_params.interval = atoi(argv[7]);

            /* TCP */
            if (strcasecmp("tcp", argv[3]) == 0)
            {
                tCxt.test_type = TCP_TX;
                ath_tcp_tx(&tCxt);
            }
            /* UDP */
            else if (strcasecmp("udp", argv[3]) == 0)
            {
                if (argc == 9)
                {
                    tCxt.params.tx_params.multicast_if = 1;
                    if (v6EnableFlag)
                    {
                        /*Get IPv6 address of Peer*/
                        retval = Inet6Pton(argv[8], (IP6_ADDR_T *)tCxt.params.tx_params.local_v6addr);
                        if (retval == 0)
                            retval = -1;
                    }
                    else
                    {
                        /*Get IPv4 address of Peer*/
                        retval = ath_inet_aton(argv[8], (uint32_t *)&tCxt.params.tx_params.local_address);
                    }
                    if (retval != -1)
                    {
                        PRINTF("Incorrect Local IP address %s" NL, argv[8]);
                        return_code = A_ERROR;
                        print_usage = TRUE;
                        goto ERROR;
                    }
                }
                tCxt.test_type = UDP_TX;
                ath_udp_tx(&tCxt);
            }
#if ENABLE_RAW_SOCKET_DEMO
            /* RAW */
            else if (strcasecmp("raw", argv[3]) == 0)
            {
                if (argc >= 9)
                    retval = ath_inet_aton(argv[8], (uint32_t *)&tCxt.params.tx_params.local_address);
                else
                    retval = 0;

                if (retval != -1)
                {
                    PRINTF("Incorrect Local IP address %s" NL, argv[8]);
                    return_code = A_ERROR;
                    print_usage = TRUE;
                    goto ERROR;
                }
                if (ATH_IN_MULTICAST(tCxt.params.tx_params.ip_address))
                {
                    PRINTF("Multicast" NL);
                    tCxt.params.tx_params.multicast_if = 1;
                }

                if ((argc == 10) && (strcasecmp("ip_hdr_inc", argv[9]) == 0))
                {
                    tCxt.params.tx_params.ip_hdr_inc = 1;
                }

                tCxt.test_type = RAW_TX;
                ath_raw_tx(&tCxt);
            }
#endif
#if ENABLE_SSL
            else if (strcasecmp("ssl", argv[3]) == 0)
            {
                if (ssl_role)
                {
                    PRINTF("ERROR: busy" NL);
                    return_code = A_ERROR;
                    goto ERROR;
                }
                if (ssl_inst[SSL_CLIENT_INST].sslCtx == NULL || ssl_inst[SSL_CLIENT_INST].role != SSL_CLIENT)
                {
                    PRINTF("ERROR: SSL client not stated (Use 'wmiconfig --ssl_start client' first)" NL);
                    return_code = A_ERROR;
                    goto ERROR;
                }

                tCxt.test_type = SSL_TX;
                ssl_role       = SSL_CLIENT;
                ath_tcp_tx(&tCxt);
                ssl_role = (SSL_ROLE_T)0;
            }
#endif

            else
            {
                PRINTF("Invalid protocol %s" NL, argv[3]);
            }
        }
        else
        {
            PRINTF("Incorrect Server IP address %s" NL, argv[1]); /* Wrong throughput Server IP address. */
            return_code = A_ERROR;
            goto ERROR;
        }
    }
ERROR:
    if (print_usage)
    {
#if ENABLE_SSL
        PRINTF(
            "%s <Rx IP> <port> <tcp|udp|ssl> <msg size> <test mode> <number of packets | time (sec)> <delay in msec> "
            "[ssl options]" NL,
            argv[0]);
#else
        PRINTF(
            "%s <Rx IP> <port> <tcp|udp> <msg size> <test mode> <number of packets | time (sec)> <delay in msec> "
            "<local IP*>" NL,
            argv[0]);
#endif
        PRINTF("               where-   test mode = 0  - enter time period in sec" NL);
        PRINTF("                        test mode = 1  - enter number of packets" NL NL);
        PRINTF("                        local IP(optional)  = local interface for sending multicast" NL);
#if ENABLE_RAW_SOCKET_DEMO
        PRINTF(
            "%s <Rx IP> <prot> <raw> <msg size> <test mode> <number of packets | time (sec)> <delay in msec> <local "
            "IP> [ip_hdr_inc*]" NL,
            argv[0]);
#endif
        PRINTF("                        ip_hdr_inc    - To include IP Header in RAW data (optional)" NL);
    }
    port_in_use = 0;
    return return_code;
} /* Endbody */

#if MULTI_SOCKET_SUPPORT
/************************************************************************
 * NAME: rx_command_parser_multi_socket
 *
 * DESCRIPTION: parses parameters of rx command for multi sockets (TCP & UDP)
 ************************************************************************/

int32_t rx_command_parser_multi_socket(int32_t argc, char_ptr argv[])
{
    bool print_usage    = 0;
    int32_t return_code = A_OK;
    // THROUGHPUT_CXT     tCxt1, tCxt2, tCxt3, tCxt4;
    int16_t port[UDP_MULTI_SOCKET_MAX] = {0};
    uint32_t loop = 0, multi_socket_count = 0;

    if (!print_usage)
    {
        if (argc < 5)
        {
            PRINTF("Incomplete parameters" NL);
            return_code = A_ERROR;
            print_usage = TRUE;
        }
        else
        {
            if ((strcasecmp("udp", argv[1]) == 0))
            {
                multi_socket_count = (atoi(argv[4]) - atoi(argv[2])) + 1;
                if (multi_socket_count > UDP_MULTI_SOCKET_MAX)
                {
                    multi_socket_count = 0;
                    PRINTF("Max UDP sockets: %d" NL, UDP_MULTI_SOCKET_MAX);
                    return A_ERROR;
                }
                for (loop = 0; loop < multi_socket_count; loop++)
                {
                    port[loop] = atoi(argv[2]) + loop;
                }
                ath_udp_rx_multi_socket(port, multi_socket_count);
            }
            else if ((strcasecmp("tcp", argv[1]) == 0))
            {
                multi_socket_count = (atoi(argv[4]) - atoi(argv[2])) + 1;
                if (multi_socket_count > TCP_MULTI_SOCKET_MAX)
                {
                    multi_socket_count = 0;
                    PRINTF("Max TCP sockets: %d" NL, TCP_MULTI_SOCKET_MAX);
                    return A_ERROR;
                }
                for (loop = 0; loop < multi_socket_count; loop++)
                {
                    port[loop] = atoi(argv[2]) + loop;
                }
                ath_tcp_rx_multi_socket(port, multi_socket_count);
            }
        }
    }

    if (print_usage)
    {
        PRINTF("Usage: %s [udp] <port1> - <port2> or" NL, argv[0]);
        PRINTF("Usage: %s [tcp] <port1> - <port2>" NL, argv[0]);
        PRINTF("   tcp  = TCP receiver(Max range : %d)" NL, TCP_MULTI_SOCKET_MAX);
        PRINTF("   udp  = UDP receiver(Max range : %d)" NL, UDP_MULTI_SOCKET_MAX);
        PRINTF("   port1, port2 = receiver port numbers" NL);
    }

    return return_code;
}

/************************************************************************
 * NAME: tx_command_parser_multi_socket
 *
 * DESCRIPTION:
 ************************************************************************/
int32_t tx_command_parser_multi_socket(int32_t argc, char_ptr argv[])
{ /* Body */
    bool print_usage    = 0;
    int32_t return_code = A_OK;
    THROUGHPUT_CXT tCxt;
    int32_t retval          = -1;
    int32_t max_packet_size = 0;
    int32_t range;

    memset(&tCxt, 0, sizeof(THROUGHPUT_CXT));
    if (argc != 9)
    {
        /*Incorrect number of params, exit*/
        return_code = A_ERROR;
        print_usage = TRUE;
    }
    else
    {
        if (v6EnableFlag)
        {
            /*Get IPv6 address of Peer*/
            retval = Inet6Pton(argv[1], (IP6_ADDR_T *)tCxt.params.tx_params.v6addr); /* server IP*/
            if (retval == 0)
                retval = -1;
        }
        else
        {
            /*Get IPv4 address of Peer*/
            retval = ath_inet_aton(argv[1], (uint32_t *)&tCxt.params.tx_params.ip_address);
        }
        if (retval == -1)
        {
            /* Port.*/
            tCxt.params.tx_params.port = atoi(argv[2]);

            range = atoi(argv[3]) - tCxt.params.tx_params.port + 1;
            if (range > TCP_MULTI_SOCKET_MAX)
            {
                PRINTF("Max socket supported:%d" NL, TCP_MULTI_SOCKET_MAX);
                return_code = A_ERROR;
                goto ERROR;
            }

            /*Check port validity*/
            if (port_in_use == tCxt.params.tx_params.port)
            {
                PRINTF("This port is in use, use another Port" NL);
                return_code = A_ERROR;
                goto ERROR;
            }

            port_in_use = tCxt.params.tx_params.port;

            /* Packet size.*/
            tCxt.params.tx_params.packet_size = atoi(argv[5]);
            if (!v6EnableFlag)
                max_packet_size = CFG_PACKET_SIZE_MAX_TX;
            else
                max_packet_size = 1230;

            if ((tCxt.params.tx_params.packet_size == 0) || (tCxt.params.tx_params.packet_size > max_packet_size))
            {
                if (!v6EnableFlag)
                    PRINTF("Invalid packet length for v4, allowed %ld" NL, max_packet_size); /* Print error mesage. */
                else
                    PRINTF("Invalid packet length for v6, allowed %ld" NL, max_packet_size); /* Print error mesage. */

                return_code = A_ERROR;
                print_usage = TRUE;
                goto ERROR;
            }

            /* Test mode.*/
            tCxt.params.tx_params.test_mode = atoi(argv[6]);

            if (tCxt.params.tx_params.test_mode != 0 && tCxt.params.tx_params.test_mode != 1)
            {
                PRINTF("Invalid test mode, please enter 0-time or 1-number of packets" NL); /* Print error mesage. */
                return_code = A_ERROR;
                print_usage = TRUE;
                goto ERROR;
            }

            /* Number of packets OR time period.*/
            if (tCxt.params.tx_params.test_mode == 0)
            {
                tCxt.params.tx_params.tx_time = atoi(argv[7]);
            }
            else if (tCxt.params.tx_params.test_mode == 1)
            {
                tCxt.params.tx_params.packet_number = atoi(argv[7]);
            }

            /* Inter packet interval for Bandwidth control*/
            tCxt.params.tx_params.interval = atoi(argv[8]);

            /* TCP */
            if (strcasecmp("tcp", argv[4]) == 0)
            {
                ath_tcp_tx_multi_socket(&tCxt, range);
            }
            else
            {
                PRINTF("Invalid protocol %s" NL, argv[4]);
            }
        }
        else
        {
            PRINTF("Incorrect Server IP address %s" NL, argv[1]); /* Wrong throughput Server IP address. */
            return_code = A_ERROR;
            goto ERROR;
        }
    }
ERROR:
    if (print_usage)
    {
        PRINTF(
            "%s <Rx IP> <start port> <end port> <tcp> <msg size> <test mode> <number of packets | time (sec)> <delay "
            "in msec>" NL,
            argv[0]);
    }
    port_in_use = 0;
    return return_code;
} /* Endbody */

#if T_SELECT_VER1

static void ath_tcp_tx_multi_socket(THROUGHPUT_CXT *p_tCxt, int32_t multi_socket_count)
{
    SOCKADDR_T local_addr;
    SOCKADDR_T foreign_addr;
    char ip_str[16];
    SOCKADDR_6_T foreign_addr6;
    char ip6_str[48];
    TIME_STRUCT start_time, current_time;
    uint32_t packet_size = p_tCxt->params.tx_params.packet_size;
    uint32_t cur_packet_number;
    uint32_t next_prog_report = 0, prog_report_inc = 0;
    uint32_t buffer_offset;
    int32_t send_result = 0, result = 0;
    _ip_address temp_addr;
    uint32_t w_fd, w_fd_working_set;
    int loop;
    uint32_t sock_peer[MAX_SOCKETS_SUPPORTED];
    THROUGHPUT_CXT tCxt[TCP_MULTI_SOCKET_MAX];
    (void)result;

    // init quit flag
    bench_quit = 0;
    num_traffic_streams++;

    if (packet_size > CFG_PACKET_SIZE_MAX_TX) /* Check max size.*/
        packet_size = CFG_PACKET_SIZE_MAX_TX;

    /* ------ Start test.----------- */
    print_header_start("IOT TCP TX Test");

    if (!v6EnableFlag)
    {
        temp_addr = ntohl(p_tCxt->params.tx_params.ip_address);
        PRINTF("Remote IP addr. %s" NL, inet_ntoa(*(uint32_t *)(&temp_addr), ip_str));
    }
    else
    {
        PRINTF("Remote IP addr. %s" NL, inet6_ntoa((char *)p_tCxt->params.tx_params.v6addr, (void *)ip6_str));
    }
    PRINTF("Remote port: ");
    for (loop = 0; loop < multi_socket_count; loop++)
        PRINTF("%d, ", p_tCxt->params.tx_params.port + loop);
    PRINTF(NL "Message size%d" NL, p_tCxt->params.tx_params.packet_size);
    PRINTF("Number of messages %d" NL, p_tCxt->params.tx_params.packet_number);
    print_header_end();

    if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
    {
        app_time_get(&start_time);
        prog_report_inc = p_tCxt->params.tx_params.tx_time / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = start_time.SECONDS + prog_report_inc;
    }
    else if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
    {
        /* generate 20 progress characters across screen to provide progress feedback */
        prog_report_inc = p_tCxt->params.tx_params.packet_number / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = prog_report_inc;
    }

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        /* Create socket */
        if (!v6EnableFlag)
        {
            if ((sock_peer[loop] = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
            {
                PRINTF("ERROR: Unable to create socket" NL);
                goto ERROR_1;
            }
            memset(&foreign_addr, 0, sizeof(local_addr));
            foreign_addr.sin_addr.s_addr = p_tCxt->params.tx_params.ip_address;
            foreign_addr.sin_port        = p_tCxt->params.tx_params.port + loop;
            foreign_addr.sin_family      = ATH_AF_INET;

            PRINTF("Connecting." NL);
            /* Connect to the server.*/
            if (qcom_connect(sock_peer[loop], (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr)) == A_ERROR)
            {
                PRINTF("Connection failed" NL);
                goto ERROR_2;
            }
        }
        else
        {
            if ((sock_peer[loop] = qcom_socket(ATH_AF_INET6, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
            {
                PRINTF("ERROR: Unable to create socket" NL);
                goto ERROR_1;
            }
            memset(&foreign_addr6, 0, sizeof(foreign_addr6));
            memcpy(&foreign_addr6.sin6_addr, p_tCxt->params.tx_params.v6addr, 16);
            ;
            foreign_addr6.sin6_port   = p_tCxt->params.tx_params.port + loop;
            foreign_addr6.sin6_family = ATH_AF_INET6;

            PRINTF("Connecting." NL);
            /* Connect to the server.*/
            if (qcom_connect(sock_peer[loop], (struct sockaddr *)(&foreign_addr6), sizeof(foreign_addr6)) == A_ERROR)
            {
                PRINTF("Connection failed" NL);
                goto ERROR_2;
            }
        }
    }

    /* Sending.*/
    PRINTF("Sending." NL);

    /*Reset all counters*/
    p_tCxt->bytes       = 0;
    p_tCxt->kbytes      = 0;
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;
    cur_packet_number   = 0;
    buffer_offset       = 0;

    app_time_get_elapsed(&p_tCxt->first_time);
    app_time_get_elapsed(&p_tCxt->last_time);
    // TIME_STRUCT block_time = p_tCxt->first_time;

    A_MDELAY(20);

    // Form the w_fd mask
    w_fd = 0;
    for (loop = 0; loop < multi_socket_count; loop++)
    {
        memcpy(&(tCxt[loop]), p_tCxt, sizeof(THROUGHPUT_CXT));
        FD_Set(sock_peer[loop], &w_fd);
    }

    while (1)
    {
        if (test_for_quit())
        {
            break;
        }

        w_fd_working_set = w_fd;

        /* Break when all the sockets are closed */
        if (!w_fd)
        {
            break;
        }

        assert((void *)handle != NULL);
        if ((result = (t_select_ver1((void *)handle, multi_socket_count, NULL, &w_fd_working_set, NULL,
                                     1 /*UDP_CONNECTION_WAIT_TIME_MULTI_SOCK*/))) > 0)
        {
            for (loop = 0; loop < multi_socket_count; loop++)
            {
                /* If the select is not set */
                if (!FD_IsSet(sock_peer[loop], w_fd_working_set))
                {
                    continue;
                }
                while ((tCxt[loop].buffer = CUSTOM_ALLOC(packet_size)) == NULL)
                {
                    // Wait till we get a buffer
                    if (test_for_quit())
                    {
                        goto ERROR_2;
                    }
                    /*Allow small delay to allow other thread to run*/
                    A_MDELAY(SMALL_TX_DELAY);
                }

                if (!v6EnableFlag)
                {
                    send_result = qcom_send(sock_peer[loop], (tCxt[loop].buffer), packet_size, 0);
                }
                else
                {
                    send_result = qcom_send(sock_peer[loop], (tCxt[loop].buffer), packet_size, 0);
                }
#if !NON_BLOCKING_TX
                /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
                if (tCxt[loop].buffer)
                    CUSTOM_FREE(tCxt[loop].buffer);
#endif
                app_time_get_elapsed(&tCxt[loop].last_time);

                /****Bandwidth control- add delay if user has specified it************/
                if (tCxt[loop].params.tx_params.interval)
                    A_MDELAY(tCxt[loop].params.tx_params.interval);

                if (send_result == A_ERROR)
                {
                    PRINTF("send packet error = %ld" NL, send_result);
                    FD_Clr(sock_peer[loop], &w_fd);
                    continue;
                }
                else if (send_result == A_SOCK_INVALID)
                {
                    /*Socket has been closed by target due to some error, gracefully exit*/
                    PRINTF("Socket closed unexpectedly" NL);
                    FD_Clr(sock_peer[loop], &w_fd);
                    continue;
                }
                else if (send_result)
                {
                    tCxt[loop].bytes += send_result;
                    tCxt[loop].sent_bytes += send_result;
                    buffer_offset += send_result;

                    if (buffer_offset == packet_size)
                    {
                        cur_packet_number++;
                        buffer_offset = 0;
                    }

                    if (tCxt[loop].params.tx_params.test_mode == PACKET_TEST)
                    {
                        /*Test based on number of packets, check if we have reached specified limit*/
                        if (cur_packet_number >= next_prog_report)
                        {
                            PRINTF(".");
                            next_prog_report += prog_report_inc;
                        }

                        if ((cur_packet_number >= tCxt[loop].params.tx_params.packet_number))
                        {
                            /* Test completed, print throughput results.*/
                            FD_Clr(sock_peer[loop], &w_fd);
                            continue;
                        }
                    }
                    else if (tCxt[loop].params.tx_params.test_mode == TIME_TEST)
                    {
                        /*Test based on time interval, check if we have reached specified limit*/
                        app_time_get(&current_time);

                        if (current_time.SECONDS >= next_prog_report)
                        {
                            PRINTF(".");
                            next_prog_report += prog_report_inc;
                        }

                        if (check_test_time(&tCxt[loop]))
                        {
                            /* Test completed, print test results.*/
                            FD_Clr(sock_peer[loop], &w_fd);
                            continue;
                        }
                    }
                } /* send_result */
            }
        } /* select */
    }     /* while loop */

ERROR_2:
    PRINTF(NL); // new line to separate from progress line

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        tCxt[loop].kbytes    = tCxt[loop].bytes / 1024;
        tCxt[loop].bytes     = tCxt[loop].bytes % 1024;
        tCxt[loop].test_type = TCP_TX;
        print_test_results(&tCxt[loop]);

        if (send_result != A_SOCK_INVALID)
            qcom_socket_close(sock_peer[loop]);
    }

ERROR_1:
    PRINTF(CFG_COMPLETED_STR);
    PRINTF(NL);

    return;
}
#endif

#endif // MULTI_SOCKET_SUPPORT

/************************************************************************
 * NAME: rx_command_parser
 *
 * DESCRIPTION: parses parameters of receive command (both TCP & UDP)
 ************************************************************************/
int32_t rx_command_parser(int32_t argc, char_ptr argv[])
{ /* Body */
    bool print_usage = 0, shorthelp = FALSE;
    int32_t return_code = A_OK;
    THROUGHPUT_CXT tCxt;
    uint32_t tmp_addr = 0;

    memset(&tCxt, 0, sizeof(THROUGHPUT_CXT));

    if (argc < 3)
    {
        PRINTF("Incomplete parameters" NL);
        return_code = A_ERROR;
        print_usage = TRUE;
    }
    else
    {
#if ENABLE_RAW_SOCKET_DEMO
        if (strcasecmp("raw", argv[1]) == 0)
        {
            if (strcasecmp("eapol", argv[2]) == 0)
            {
                tCxt.params.rx_params.raw_mode = ETH_RAW;
            }
            else
                tCxt.params.rx_params.raw_mode = IP_RAW;
        }
#endif
        if (strcasecmp("eapol", argv[2]) != 0)
        {
            tCxt.params.rx_params.port = atoi(argv[2]);

            /*Check port validity*/
            if (port_in_use == tCxt.params.rx_params.port)
            {
                PRINTF("This port is in use, use another Port" NL);
                return A_ERROR;
            }
            port_in_use = tCxt.params.rx_params.port;
        }

        if (argc > 3)
        {
            if (argc == 4)
            {
                if (strcasecmp("ip_hdr_inc", argv[3]) == 0)
                {
                    tCxt.params.rx_params.ip_hdr_inc = 1;
                }
                else /* Local address is provided */
                {
                    if (!v6EnableFlag)
                    {
                        ath_inet_aton(argv[3], (uint32_t *)&tCxt.params.rx_params.local_address);
                    }
                    else
                    {
                        Inet6Pton(argv[3], (IP6_ADDR_T *)tCxt.params.rx_params.local_v6addr);
                    }
                    tCxt.params.rx_params.local_if = 1;
                }
            }
            else if (argc >= 5)
            {
                if (argc == 6)
                {
                    if (strcasecmp("ip_hdr_inc", argv[5]) == 0)
                    {
                        tCxt.params.rx_params.ip_hdr_inc = 1;
                    }
                }
                if ((argc == 6) || (strcasecmp("ip_hdr_inc", argv[4]) != 0))
                {
                    // Multicast address is provided
                    if (!v6EnableFlag)
                    {
                        ath_inet_aton(argv[3], (uint32_t *)&tmp_addr);
                        tCxt.params.rx_params.group.imr_multiaddr = tmp_addr;
                        ath_inet_aton(argv[4], (uint32_t *)&tmp_addr);
                        tCxt.params.rx_params.group.imr_interface = tmp_addr;
                    }
                    else
                    {
                        Inet6Pton(argv[3], (IP6_ADDR_T *)tCxt.params.rx_params.group6.ipv6mr_multiaddr.s6_addr);
                        Inet6Pton(argv[4], (IP6_ADDR_T *)tCxt.params.rx_params.group6.ipv6mr_interface.s6_addr);
                    }
                    tCxt.params.rx_params.mcastEnabled = 1;
                }
                if (strcasecmp("ip_hdr_inc", argv[4]) == 0)
                {
                    tCxt.params.rx_params.ip_hdr_inc = 1;
                    if (!v6EnableFlag)
                    {
                        ath_inet_aton(argv[3], (uint32_t *)&tCxt.params.rx_params.local_address);
                    }
                    else
                    {
                        Inet6Pton(argv[3], (IP6_ADDR_T *)tCxt.params.rx_params.local_v6addr);
                    }
                    tCxt.params.rx_params.local_if = 1;
                }
            }
            else
            {
                PRINTF("ERROR: Incomplete Multicast Parameters provided" NL);
                return_code = A_ERROR;
                goto ERROR;
            }
        }

        if (strcasecmp("tcp", argv[1]) == 0)
        {
            tCxt.test_type = TCP_RX;
            ath_tcp_rx(&tCxt);
        }
        else if (strcasecmp("udp", argv[1]) == 0)
        {
            tCxt.test_type = UDP_RX;
            ath_udp_rx(&tCxt);
        }
#if ENABLE_RAW_SOCKET_DEMO
        else if (strcasecmp("raw", argv[1]) == 0)
        {
            ath_raw_rx(&tCxt);
        }
#endif
#if ENABLE_SSL
        else if ((argc == 1) || (strcasecmp("ssl", argv[1]) == 0))
        {
            if (ssl_role)
            {
                PRINTF("ERROR: busy" NL);
                return_code = A_ERROR;
                goto ERROR;
            }
            if (ssl_inst[SSL_SERVER_INST].sslCtx == NULL || ssl_inst[SSL_SERVER_INST].role != SSL_SERVER)
            {
                PRINTF("ERROR: SSL server not stated (Use 'wmiconfig --ssl_start server' first)" NL);
                return_code = A_ERROR;
                goto ERROR;
            }
            tCxt.test_type = SSL_RX;
            ssl_role       = SSL_SERVER;
            ath_tcp_rx(&tCxt);
            ssl_role = (SSL_ROLE_T)0;
        }
#endif
        else
        {
            PRINTF("Invalid parameter" NL);
        }
    }

ERROR:
    if (print_usage)
    {
        if (shorthelp)
        {
#if ENABLE_SSL
            PRINTF("%s [tcp|udp|ssl] <port>" NL, argv[0]);
#else
            PRINTF("%s [tcp|udp] <port>" NL, argv[0]);
#endif
        }
        else
        {
#if ENABLE_SSL
            PRINTF("Usage: %s [tcp|udp|ssl] <port> <multicast address*> <local address*>" NL, argv[0]);
#else
            PRINTF(
                "Usage: %s [tcp|udp] <port> <multicast address/local address for unicast*> <local address*> "
                "[ip_hdr_inc]" NL,
                argv[0]);
#endif
            PRINTF("   tcp  = TCP receiver" NL);
            PRINTF("   udp  = UDP receiver" NL);
#if ENABLE_SSL
            PRINTF("   ssl  = SSL receiver (TCP + SSL security on top)" NL);
#endif
            PRINTF("   port = receiver port" NL);
            PRINTF("   multicast address  = IP address of multicast group to join (optional)" NL);
            PRINTF("   local address      = IP address of interface (optional)" NL);
            PRINTF("   ip_hdr_inc      =    To include IP Header in UDP data (optional))" NL);
            PRINTF(
                "Usage: %s raw <prot> (multicast address/local address for unicast> <local address*> [ip_hdr_inc]" NL,
                argv[0]);
            PRINTF("   prot = Protocol to be used in RAW socket" NL);
        }
    }

    port_in_use = 0;

    return return_code;
} /* Endbody */

////////////////////////////////////////////// TX //////////////////////////////////////

/************************************************************************
 * NAME: ath_tcp_tx
 *
 * DESCRIPTION: Start TCP Transmit test.
 ************************************************************************/
static void ath_tcp_tx(THROUGHPUT_CXT *p_tCxt)
{
    SOCKADDR_T local_addr;
    SOCKADDR_T foreign_addr;
    char ip_str[16];
    SOCKADDR_6_T foreign_addr6;
    char ip6_str[48];
    TIME_STRUCT start_time, current_time;
    uint32_t packet_size = p_tCxt->params.tx_params.packet_size;
    uint32_t cur_packet_number;
    uint32_t next_prog_report = 0, prog_report_inc = 0;
    uint32_t buffer_offset;
    int32_t send_result = 0, result;
    _ip_address temp_addr;
#if ENABLE_SSL
    SSL_INST *ssl = &ssl_inst[SSL_CLIENT_INST];
#endif

    // init quit flag
    bench_quit = 0;
    num_traffic_streams++;

    if (packet_size > CFG_PACKET_SIZE_MAX_TX) /* Check max size.*/
        packet_size = CFG_PACKET_SIZE_MAX_TX;

    /* ------ Start test.----------- */
    PRINTF(NL "**********************************************************" NL);
#if ENABLE_SSL
    PRINTF("IOT %s TX Test" NL, p_tCxt->test_type == SSL_TX ? "SSL" : "TCP");
#else
    PRINTF("IOT TCP TX Test" NL);
#endif
    PRINTF("**********************************************************" NL);

    if (!v6EnableFlag)
    {
        temp_addr = ntohl(p_tCxt->params.tx_params.ip_address);
        memset(ip_str, 0, sizeof(ip_str));
        PRINTF("Remote IP addr. %s" NL, inet_ntoa(*(uint32_t *)(&temp_addr), ip_str));
    }
    else
    {
        PRINTF("Remote IP addr. %s" NL, inet6_ntoa((char *)p_tCxt->params.tx_params.v6addr, (void *)ip6_str));
    }
    PRINTF("Remote port %d" NL, p_tCxt->params.tx_params.port);
    PRINTF("Message size%d" NL, p_tCxt->params.tx_params.packet_size);
    PRINTF("Number of messages %d" NL, p_tCxt->params.tx_params.packet_number);
    print_header_end();

    if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
    {
        app_time_get(&start_time);
        prog_report_inc = p_tCxt->params.tx_params.tx_time / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = start_time.SECONDS + prog_report_inc;
    }
    else if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
    {
        /* generate 20 progress characters across screen to provide progress feedback */
        prog_report_inc = p_tCxt->params.tx_params.packet_number / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = prog_report_inc;
    }

    /* Create socket */
    if (!v6EnableFlag)
    {
        if ((p_tCxt->sock_peer = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Unable to create socket" NL);
            goto ERROR_1;
        }
    }
    else
    {
        if ((p_tCxt->sock_peer = qcom_socket(ATH_AF_INET6, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Unable to create socket" NL);
            goto ERROR_1;
        }
    }

    /*Allow small delay to allow other thread to run*/
    A_MDELAY(TX_DELAY);

    PRINTF("Connecting." NL);

    if (!v6EnableFlag)
    {
        memset(&foreign_addr, 0, sizeof(local_addr));
        foreign_addr.sin_addr.s_addr = p_tCxt->params.tx_params.ip_address;
        foreign_addr.sin_port        = p_tCxt->params.tx_params.port;
        foreign_addr.sin_family      = ATH_AF_INET;

        /* Connect to the server.*/
        if (qcom_connect(p_tCxt->sock_peer, (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr)) == A_ERROR)
        {
            PRINTF("Connection failed" NL);
            goto ERROR_2;
        }
    }
    else
    {
        memset(&foreign_addr6, 0, sizeof(foreign_addr6));
        memcpy(&foreign_addr6.sin6_addr, p_tCxt->params.tx_params.v6addr, 16);
        ;
        foreign_addr6.sin6_port   = p_tCxt->params.tx_params.port;
        foreign_addr6.sin6_family = ATH_AF_INET6;

        /* Connect to the server.*/
        if (qcom_connect(p_tCxt->sock_peer, (struct sockaddr *)(&foreign_addr6), sizeof(foreign_addr6)) == A_ERROR)
        {
            PRINTF("Connection failed" NL);
            goto ERROR_2;
        }
    }

#if ENABLE_SSL
    if (p_tCxt->test_type == SSL_TX)
    {
        if (ssl->ssl == NULL)
        {
            // Create SSL connection object
            ssl->ssl = qcom_SSL_new(ssl->sslCtx);
            if (ssl->ssl == NULL)
            {
                PRINTF("ERROR: Unable to create SSL context" NL);
                goto ERROR_2;
            }

            // configure the SSL connection
            if (ssl->config_set)
            {
                result = qcom_SSL_configure(ssl->ssl, &ssl->config);
                if (result < A_OK)
                {
                    PRINTF("ERROR: SSL configure failed (%d)" NL, (int)result);
                    goto ERROR_2;
                }
            }
        }

        // Add socket handle to SSL connection
        result = qcom_SSL_set_fd(ssl->ssl, p_tCxt->sock_peer);
        if (result < 0)
        {
            PRINTF("ERROR: Unable to add socket handle to SSL (%d)" NL, (int)result);
            goto ERROR_2;
        }

        // SSL handshake with server
        result = qcom_SSL_connect(ssl->ssl);
        if (result < 0)
        {
            if (result == ESSL_TRUST_CertCnTime)
            {
                /** The peer's SSL certificate is trusted, CN matches the host name, time is valid */
                PRINTF("The certificate is trusted" NL);
            }
            else if (result == ESSL_TRUST_CertCn)
            {
                /** The peer's SSL certificate is trusted, CN matches the host name, time is expired */
                PRINTF("ERROR: The certificate is expired" NL);
                goto ERROR_2;
            }
            else if (result == ESSL_TRUST_CertTime)
            {
                /** The peer's SSL certificate is trusted, CN does NOT match the host name, time is valid */
                PRINTF("ERROR: The certificate is trusted, but the host name is not valid" NL);
                goto ERROR_2;
            }
            else if (result == ESSL_TRUST_Cert)
            {
                /** The peer's SSL certificate is trusted, CN does NOT match host name, time is expired */
                PRINTF("ERROR: The certificate is expired and the host name is not valid" NL);
                goto ERROR_2;
            }
            else if (result == ESSL_TRUST_None)
            {
                /** The peer's SSL certificate is NOT trusted */
                PRINTF("ERROR: The certificate is NOT trusted" NL);
                goto ERROR_2;
            }
            else
            {
                PRINTF("ERROR: SSL connect failed (%d)" NL, (int)result);
                goto ERROR_2;
            }
        }
    }
#endif /* Sending.*/
    PRINTF("Sending." NL);

    /*Reset all counters*/
    p_tCxt->bytes       = 0;
    p_tCxt->kbytes      = 0;
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;
    cur_packet_number   = 0;
    buffer_offset       = 0;

    app_time_get_elapsed(&p_tCxt->first_time);
    app_time_get_elapsed(&p_tCxt->last_time);
    // TIME_STRUCT block_time = p_tCxt->first_time;

    A_MDELAY(20);

    while (1)
    {
        if (test_for_quit())
        {
            break;
        }

        while ((p_tCxt->buffer = CUSTOM_ALLOC(packet_size)) == NULL)
        {
            // Wait till we get a buffer
            if (test_for_quit())
            {
                goto ERROR_2;
            }
            /*Allow small delay to allow other thread to run*/
            A_MDELAY(SMALL_TX_DELAY);
        }
        {
            uint8_t data = 0;
            int i;

            p_tCxt->buffer[0] = packet_size & 0xFF;
            p_tCxt->buffer[1] = packet_size >> 8;
            for (i = 2; i < packet_size; ++i)
            {
                p_tCxt->buffer[i] = data++;
            }
        }
#if ENABLE_SSL
        if (p_tCxt->test_type == SSL_TX)
        {
            send_result = qcom_SSL_write(ssl->ssl, p_tCxt->buffer, packet_size);
        }
        else
#endif
            if (!v6EnableFlag)
        {
            send_result = qcom_send(p_tCxt->sock_peer, (p_tCxt->buffer), packet_size, 0);
        }
        else
        {
            send_result = qcom_send(p_tCxt->sock_peer, (p_tCxt->buffer), packet_size, 0);
        }
#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        if (p_tCxt->buffer)
            CUSTOM_FREE(p_tCxt->buffer);
#endif
        app_time_get_elapsed(&p_tCxt->last_time);
#if 0
            if(test_for_delay(&p_tCxt->last_time, &block_time)){
            	/* block to give other tasks an opportunity to run */
            	A_MDELAY(TX_DELAY);
            	app_time_get_elapsed(&block_time);
            }
#endif
        /****Bandwidth control- add delay if user has specified it************/
        if (p_tCxt->params.tx_params.interval)
            A_MDELAY(p_tCxt->params.tx_params.interval);

        if (send_result == A_ERROR)
        {
            PRINTF("send packet error = %d" NL, send_result);
            // resetTarget();
            goto ERROR_2;
        }
        else if (send_result == A_SOCK_INVALID)
        {
            /*Socket has been closed by target due to some error, gracefully exit*/
            PRINTF("Socket closed unexpectedly" NL);
            break;
        }
        else if (send_result)
        {
            p_tCxt->bytes += send_result;
            p_tCxt->sent_bytes += send_result;
            buffer_offset += send_result;

            if (buffer_offset == packet_size)
            {
                cur_packet_number++;
                buffer_offset = 0;
            }

            if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
            {
                /*Test based on number of packets, check if we have reached specified limit*/
                if (cur_packet_number >= next_prog_report)
                {
                    PRINTF(".");
                    next_prog_report += prog_report_inc;
                }

                if ((cur_packet_number >= p_tCxt->params.tx_params.packet_number))
                {
                    /* Test completed, print throughput results.*/
                    break;
                }
            }
            else if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
            {
                /*Test based on time interval, check if we have reached specified limit*/
                app_time_get(&current_time);

                if (current_time.SECONDS >= next_prog_report)
                {
                    PRINTF(".");
                    next_prog_report += prog_report_inc;
                }

                if (check_test_time(p_tCxt))
                {
                    /* Test completed, print test results.*/

                    break;
                }
            }
        }
    }

ERROR_2:
    PRINTF(NL); // new line to separate from progress line
    p_tCxt->kbytes    = p_tCxt->bytes / 1024;
    p_tCxt->bytes     = p_tCxt->bytes % 1024;
    p_tCxt->test_type = TCP_TX;
    print_test_results(p_tCxt);

    if (send_result != A_SOCK_INVALID)
        qcom_socket_close(p_tCxt->sock_peer);

#if ENABLE_SSL
    if (ssl_role == SSL_CLIENT && ssl->ssl != NULL)
    {
        qcom_SSL_shutdown(ssl->ssl);
        ssl->ssl = NULL;
    }
#endif

ERROR_1:
    PRINTF(CFG_COMPLETED_STR);
    PRINTF(NL);
    num_traffic_streams--;
}

/************************************************************************
 * NAME: ath_udp_tx
 *
 * DESCRIPTION: Start TX UDP throughput test.
 ************************************************************************/
static void ath_udp_tx(THROUGHPUT_CXT *p_tCxt)
{
    char ip_str[16];
    TIME_STRUCT start_time, current_time;
    uint32_t next_prog_report = 0, prog_report_inc = 0;
    SOCKADDR_T foreign_addr;
    SOCKADDR_T local_addr;
    SOCKADDR_6_T foreign_addr6;
    SOCKADDR_6_T local_addr6;
    char ip6_str[48];
    int32_t send_result, result;
    uint32_t packet_size = p_tCxt->params.tx_params.packet_size;
    uint32_t cur_packet_number;
    _ip_address temp_addr;

    bench_quit = 0;

    if (packet_size > CFG_PACKET_SIZE_MAX_TX) /* Check max size.*/
        packet_size = CFG_PACKET_SIZE_MAX_TX;

    temp_addr = ntohl(p_tCxt->params.tx_params.ip_address);

    /* ------ Start test.----------- */
    print_header_start(" UDP TX Test");
    if (!v6EnableFlag)
    {
        PRINTF("Remote IP addr. %s" NL, inet_ntoa(*(uint32_t *)(&temp_addr), ip_str));
    }
    else
    {
        PRINTF("Remote IP addr. %s" NL, inet6_ntoa((char *)p_tCxt->params.tx_params.v6addr, ip6_str));
    }
    PRINTF("Remote port %d" NL, p_tCxt->params.tx_params.port);
    PRINTF("Message size %d" NL, p_tCxt->params.tx_params.packet_size);
    PRINTF("Number of messages %d" NL, p_tCxt->params.tx_params.packet_number);
    print_header_end();

    if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
    {
        app_time_get(&start_time);

        prog_report_inc = p_tCxt->params.tx_params.tx_time / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = start_time.SECONDS + prog_report_inc;
    }
    else if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
    {
        /* generate 20 progress characters across screen to provide progress feedback */
        prog_report_inc = p_tCxt->params.tx_params.packet_number / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = prog_report_inc;
    }

    /* Create UDP socket */
    if (!v6EnableFlag)
    {
        /* Create IPv4 socket */
        if ((p_tCxt->sock_peer = qcom_socket(ATH_AF_INET, SOCK_DGRAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            goto ERROR_1;
        }
        if (p_tCxt->params.tx_params.multicast_if)
        {
            _ip_address local_address = A_CPU2BE32(p_tCxt->params.tx_params.local_address);
            if (qcom_setsockopt(p_tCxt->sock_peer, ATH_IPPROTO_IP, IP_MULTICAST_IF, (uint8_t *)(&local_address),
                                sizeof(int)) != A_OK)
            {
                PRINTF("SetsockOPT error:IP_MULTICAST_IF" NL);
                goto ERROR_1;
            }
        }
    }
    else
    {
        /* Create IPv6 socket */
        if ((p_tCxt->sock_peer = qcom_socket(ATH_AF_INET6, SOCK_DGRAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            goto ERROR_1;
        }
        if (p_tCxt->params.tx_params.multicast_if)
        {
            if (qcom_setsockopt(p_tCxt->sock_peer, ATH_IPPROTO_IP, IPV6_MULTICAST_IF,
                                (uint8_t *)(&(p_tCxt->params.tx_params.local_v6addr[0])), sizeof(IP6_ADDR_T)) != A_OK)
            {
                PRINTF("SetsockOPT error:IP6_MULTICAST_IF" NL);
                goto ERROR_1;
            }
        }
    }

    /* Bind to the server.*/
    PRINTF("Connecting." NL);
    memset(&foreign_addr, 0, sizeof(local_addr));
    foreign_addr.sin_addr.s_addr = p_tCxt->params.tx_params.ip_address;
    foreign_addr.sin_port        = p_tCxt->params.tx_params.port;
    foreign_addr.sin_family      = ATH_AF_INET;

    if (test_for_quit())
    {
        goto ERROR_2;
    }

    /*Allow small delay to allow other thread to run*/
    A_MDELAY(TX_DELAY);

    if (!v6EnableFlag)
    {
        if (qcom_connect(p_tCxt->sock_peer, (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr)) == A_ERROR)
        {
            PRINTF("Conection failed" NL);
            goto ERROR_2;
        }
    }
    else
    {
        memset(&foreign_addr6, 0, sizeof(local_addr6));
        memcpy(&foreign_addr6.sin6_addr, p_tCxt->params.tx_params.v6addr, 16);
        foreign_addr6.sin6_port   = p_tCxt->params.tx_params.port;
        foreign_addr6.sin6_family = ATH_AF_INET6;
        if (qcom_connect(p_tCxt->sock_peer, (struct sockaddr *)(&foreign_addr6), sizeof(foreign_addr6)) == A_ERROR)
        {
            PRINTF("Conection failed" NL);
            goto ERROR_2;
        }
    }

    /* Sending.*/
    PRINTF("Sending." NL);

    /*Reset all counters*/
    p_tCxt->bytes       = 0;
    p_tCxt->kbytes      = 0;
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;
    p_tCxt->sent_bytes  = 0;
    cur_packet_number   = 0;

    app_time_get_elapsed(&p_tCxt->first_time);
    app_time_get_elapsed(&p_tCxt->last_time);
    // TIME_STRUCT block_time = p_tCxt->first_time;

    while (1)
    {
        if (test_for_quit())
        {
            p_tCxt->test_type = UDP_TX;
            print_test_results(p_tCxt);
            break;
        }

        while ((p_tCxt->buffer = CUSTOM_ALLOC(packet_size)) == NULL)
        {
            // Wait till we get a buffer
            if (test_for_quit())
            {
                p_tCxt->test_type = UDP_TX;
                print_test_results(p_tCxt);
                goto ERROR_2;
            }
            /*Allow small delay to allow other thread to run*/
            A_MDELAY(SMALL_TX_DELAY);
        }

        if (!v6EnableFlag)
        {
            send_result = qcom_sendto(p_tCxt->sock_peer, (&p_tCxt->buffer[0]), packet_size, 0,
                                      (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr));
        }
        else
        {
            send_result = qcom_sendto(p_tCxt->sock_peer, (&p_tCxt->buffer[0]), packet_size, 0,
                                      (struct sockaddr *)(&foreign_addr6), sizeof(foreign_addr6));
        }

#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        if (p_tCxt->buffer)
        {
            CUSTOM_FREE(p_tCxt->buffer);
        }
#endif
        app_time_get_elapsed(&p_tCxt->last_time);
#if 0
          if(test_for_delay(&p_tCxt->last_time, &block_time)){
              /* block to give other tasks an opportunity to run */
              A_MDELAY(TX_DELAY);
              app_time_get_elapsed(&block_time);
          }
#endif
        /****Bandwidth control***********/
        if (p_tCxt->params.tx_params.interval)
            A_MDELAY(p_tCxt->params.tx_params.interval);

        if (send_result == A_ERROR)
        {
            // PRINTF("socket_error = %d" NL, sock_err);

            p_tCxt->test_type = UDP_TX;

            /* Print throughput results.*/
            print_test_results(p_tCxt);
            break;
        }
        else
        {
            p_tCxt->bytes += send_result;
            p_tCxt->sent_bytes += send_result;
            cur_packet_number++;

            /*Test mode can be "number of packets" or "fixed time duration"*/
            if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
            {
                if (cur_packet_number >= next_prog_report)
                {
                    PRINTF(".");
                    next_prog_report += prog_report_inc;
                }

                if ((cur_packet_number >= p_tCxt->params.tx_params.packet_number))
                {
                    /*Test completed, print throughput results.*/
                    // print_test_results(p_tCxt);
                    break;
                }
            }
            else if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
            {
                app_time_get(&current_time);
                if (current_time.SECONDS >= next_prog_report)
                {
                    PRINTF(".");
                    next_prog_report += prog_report_inc;
                }

                if (check_test_time(p_tCxt))
                {
                    /* Test completed, print throughput results.*/
                    // print_test_results(p_tCxt);
                    break;
                }
            }
        }
    }

    result = wait_for_response(p_tCxt, foreign_addr, foreign_addr6);

    if (result != A_OK)
    {
        PRINTF("UDP Transmit test failed, did not receive Ack from Peer" NL);
    }
    else
    {
        p_tCxt->test_type = UDP_TX;
        print_test_results(p_tCxt);
    }

ERROR_2:
    qcom_socket_close(p_tCxt->sock_peer);

ERROR_1:
    PRINTF("*************IOT Throughput Test Completed **************" NL);
}

#if ENABLE_RAW_SOCKET_DEMO
/************************************************************************
 * NAME: ath_raw_tx
 *
 * DESCRIPTION: Start TX RAW throughput test.
 ************************************************************************/
static void ath_raw_tx(THROUGHPUT_CXT *p_tCxt)
{
    char ip_str[16];
    TIME_STRUCT start_time, current_time;
    uint32_t next_prog_report = 0, prog_report_inc = 0;
    SOCKADDR_T foreign_addr;
    SOCKADDR_6_T foreign_addr6;
    SOCKADDR_T local_addr;
    int32_t send_result, result, proto = p_tCxt->params.tx_params.port;
    uint32_t packet_size = p_tCxt->params.tx_params.packet_size;
    uint32_t cur_packet_number;
    _ip_address temp_addr;
    ip_header *iphdr;

    bench_quit = 0;

    memset(&foreign_addr6, 0, sizeof(SOCKADDR_6_T));
    /* Adding ip header */
    if (p_tCxt->params.tx_params.ip_hdr_inc == 1)
    {
        packet_size += 20;
    }

    if (packet_size > CFG_PACKET_SIZE_MAX_TX) /* Check max size.*/
        packet_size = CFG_PACKET_SIZE_MAX_TX;

    temp_addr = ntohl(p_tCxt->params.tx_params.ip_address);

    /* ------ Start test.----------- */
    print_header_start(" RAW TX Test");
    PRINTF("Remote IP addr. %s" NL, inet_ntoa(*(uint32_t *)(&temp_addr), ip_str));
    PRINTF("Message size %d" NL, p_tCxt->params.tx_params.packet_size);
    PRINTF("Number of messages %d" NL, p_tCxt->params.tx_params.packet_number);

    print_header_end();

    if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
    {
        app_time_get(&start_time);

        prog_report_inc = p_tCxt->params.tx_params.tx_time / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = start_time.SECONDS + prog_report_inc;
    }
    else if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
    {
        /* generate 20 progress characters across screen to provide progress feedback */
        prog_report_inc = p_tCxt->params.tx_params.packet_number / 20;

        if (prog_report_inc == 0)
            prog_report_inc = 1;

        next_prog_report = prog_report_inc;
    }

    /* Create IPv4 socket */
    if ((p_tCxt->sock_peer = qcom_socket(ATH_AF_INET, SOCK_RAW_TYPE, proto)) == (uint32_t)A_ERROR)
    {
        goto ERROR_1;
    }

    if (p_tCxt->params.tx_params.ip_hdr_inc == 1)
    {
        /* Set IP_HDRINCL socket option if need to receive the packet with IP header */
        if (qcom_setsockopt(p_tCxt->sock_peer, ATH_IPPROTO_IP, IP_HDRINCL,
                            (uint8_t *)(&(p_tCxt->params.tx_params.ip_hdr_inc)), sizeof(int)) != A_OK)
        {
            PRINTF("SetsockOPT error:IP_HDRINCL" NL);
            goto ERROR_2;
        }
        /* Get the local IP address for filling IP header */
        temp_addr = p_tCxt->params.tx_params.local_address;
    }

    if (p_tCxt->params.tx_params.multicast_if)
    {
        _ip_address local_address = A_CPU2BE32(p_tCxt->params.tx_params.local_address);
        if (qcom_setsockopt(p_tCxt->sock_peer, ATH_IPPROTO_IP, IP_MULTICAST_IF, (uint8_t *)(&local_address),
                            sizeof(int)) != A_OK)
        {
            PRINTF("SetsockOPT error:IP_MULTICAST_IF" NL);
            goto ERROR_1;
        }
    }

    /* Bind to the server.*/
    PRINTF("Connecting." NL);
    memset(&foreign_addr, 0, sizeof(local_addr));
    foreign_addr.sin_addr.s_addr = p_tCxt->params.tx_params.ip_address;
    foreign_addr.sin_family      = ATH_AF_INET;

    if (test_for_quit())
    {
        goto ERROR_2;
    }

    /*Allow small delay to allow other thread to run*/
    A_MDELAY(TX_DELAY);

    /* Sending.*/
    PRINTF("Sending." NL);

    /*Reset all counters*/
    p_tCxt->bytes       = 0;
    p_tCxt->kbytes      = 0;
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;
    p_tCxt->sent_bytes  = 0;
    cur_packet_number   = 0;

    app_time_get_elapsed(&p_tCxt->first_time);
    app_time_get_elapsed(&p_tCxt->last_time);
    // TIME_STRUCT block_time = p_tCxt->first_time;

    while (1)
    {
        if (test_for_quit())
        {
            p_tCxt->test_type = RAW_TX;
            print_test_results(p_tCxt);
            break;
        }

        while ((p_tCxt->buffer = CUSTOM_ALLOC(packet_size)) == NULL)
        {
            // Wait till we get a buffer
            if (test_for_quit())
            {
                p_tCxt->test_type = RAW_TX;
                print_test_results(p_tCxt);
                goto ERROR_2;
            }
            /*Allow small delay to allow other thread to run*/
            A_MDELAY(SMALL_TX_DELAY);
        }

        /* Add IP header */
        if (p_tCxt->params.tx_params.ip_hdr_inc == 1)
        {
            iphdr = (ip_header *)p_tCxt->buffer;

            iphdr->iph_ihl   = IPV4_HEADER_LENGTH / sizeof(uint32_t);
            iphdr->iph_ver   = 4; /* IPv4 */
            iphdr->iph_tos   = 0;
            iphdr->iph_len   = htons(packet_size);
            iphdr->iph_ident = htons(0);
            /* Zero (1 bit) */
            ip_flags[0] = 0;
            /* Do not fragment flag (1 bit) */
            ip_flags[1] = 0;
            /* More fragments following flag (1 bit) */
            ip_flags[2] = 0;
            /* Fragmentation offset (13 bits) */
            ip_flags[3] = 0;

            iphdr->iph_offset = htons(((ip_flags[0] << 15) | (ip_flags[1] << 14) | (ip_flags[2] << 13) | ip_flags[3]));

            /* Time-to-Live (8 bits): default to maximum value */
            iphdr->iph_ttl = 255;

            /* Transport layer protocol (8 bits): 17 for UDP */
            iphdr->iph_protocol = proto;

            iphdr->iph_sourceip = htonl(temp_addr);
            iphdr->iph_destip   = htonl(p_tCxt->params.tx_params.ip_address);
        }

        send_result = qcom_sendto(p_tCxt->sock_peer, (&p_tCxt->buffer[0]), packet_size, 0,
                                  (struct sockaddr *)(&foreign_addr), sizeof(foreign_addr));

#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        if (p_tCxt->buffer)
        {
            CUSTOM_FREE(p_tCxt->buffer);
        }
#endif
        app_time_get_elapsed(&p_tCxt->last_time);
#if 0
        if(test_for_delay(&p_tCxt->last_time, &block_time)){
            /* block to give other tasks an opportunity to run */
            A_MDELAY(TX_DELAY);
            app_time_get_elapsed(&block_time);
        }
#endif
        /****Bandwidth control***********/
        if (p_tCxt->params.tx_params.interval)
            A_MDELAY(p_tCxt->params.tx_params.interval);

        if (send_result == A_ERROR)
        {
            p_tCxt->test_type = RAW_TX;
            /* Print throughput results.*/
            print_test_results(p_tCxt);
            break;
        }
        else
        {
            p_tCxt->bytes += send_result;
            p_tCxt->sent_bytes += send_result;
            cur_packet_number++;

            /*Test mode can be "number of packets" or "fixed time duration"*/
            if (p_tCxt->params.tx_params.test_mode == PACKET_TEST)
            {
                if (cur_packet_number >= next_prog_report)
                {
                    PRINTF(".");
                    next_prog_report += prog_report_inc;
                }

                if ((cur_packet_number >= p_tCxt->params.tx_params.packet_number))
                {
                    /*Test completed, print throughput results.*/
                    // print_test_results(p_tCxt);
                    break;
                }
            }
            else if (p_tCxt->params.tx_params.test_mode == TIME_TEST)
            {
                app_time_get(&current_time);
                if (current_time.SECONDS >= next_prog_report)
                {
                    PRINTF(".");
                    next_prog_report += prog_report_inc;
                }

                if (check_test_time(p_tCxt))
                {
                    /* Test completed, print throughput results.*/
                    // print_test_results(p_tCxt);
                    break;
                }
            }
        }
    }

    result = wait_for_response(p_tCxt, foreign_addr, foreign_addr6);

    if (result != A_OK)
    {
        PRINTF("RAW Transmit test failed, did not receive Ack from Peer" NL);
    }
    else
    {
        p_tCxt->test_type = RAW_TX;
        print_test_results(p_tCxt);
    }

ERROR_2:
    qcom_socket_close(p_tCxt->sock_peer);

ERROR_1:
    PRINTF("*************IOT Throughput Test Completed **************" NL);
}
#endif

#if MULTI_SOCKET_SUPPORT
/************************************************************************
 * NAME: ath_create_socket_and_bind
 *
 * DESCRIPTION: Create a IPv4 socket and bind a address.
 ************************************************************************/
static int ath_create_socket_and_bind(uint32_t *socket, int16_t port, int32_t type)
{
    SOCKADDR_T local_addr;

    if ((*socket = qcom_socket(ATH_AF_INET, type, 0)) == (uint32_t)A_ERROR)
    {
        PRINTF("ERROR: Socket creation error" NL);
        return A_ERROR;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_port   = port;
    local_addr.sin_family = ATH_AF_INET;

    if (qcom_bind(*socket, (struct sockaddr *)(&local_addr), sizeof(local_addr)) != A_OK)
    {
        PRINTF("ERROR: Socket bind error" NL);
        return A_ERROR;
    }
    return A_OK;
}

/************************************************************************
 * NAME: ath_create_socket_and_bind
 *
 * DESCRIPTION: Create a IPv4 socket and bind a address.
 ************************************************************************/
static int ath_create_socket_and_bind6(uint32_t *socket, int16_t port, int32_t type)
{
    SOCKADDR_6_T local_addr6;

    if ((*socket = qcom_socket(ATH_AF_INET6, type, 0)) == (uint32_t)A_ERROR)
    {
        PRINTF("ERROR: Socket creation error" NL);
        return A_ERROR;
    }

    memset(&local_addr6, 0, sizeof(local_addr6));
    local_addr6.sin6_port   = port;
    local_addr6.sin6_family = ATH_AF_INET6;

    if (qcom_bind(*socket, (struct sockaddr *)(&local_addr6), sizeof(local_addr6)) != A_OK)
    {
        PRINTF("ERROR: Socket bind error" NL);
        return A_ERROR;
    }

    return A_OK;
}

/************************************************************************
 * NAME: ath_udp_rx_multi_socket
 *
 * DESCRIPTION: Start throughput UDP server.
 ************************************************************************/
static void ath_udp_rx_multi_socket(int16_t *port, int32_t multi_socket_count)
{
    socklen_t addr_len;
    int32_t received;
    int32_t conn_sock = 0;
    int32_t is_first;
    SOCKADDR_T addr;
    SOCKADDR_6_T addr6;
    uint8_t *ip_str = NULL;
    uint32_t temp_addr;
    TIME_STRUCT block_time;
    uint32_t loop;
    uint32_t error;
    uint32_t sock_local[UDP_MULTI_SOCKET_MAX];
    THROUGHPUT_CXT tCxt;
    (void)conn_sock;

    bench_quit = 0;
// Parse remaining parameters
#if !ZERO_COPY
    if ((tCxt.buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        goto ERROR;
    }
#endif

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        sock_local[loop] = 0;
        if (!v6EnableFlag)
        {
            error = ath_create_socket_and_bind(&(sock_local[loop]), port[loop], SOCK_DGRAM_TYPE);
        }
        else
        {
            error = ath_create_socket_and_bind6(&(sock_local[loop]), port[loop], SOCK_DGRAM_TYPE);
        }

        if (error == (uint32_t)A_ERROR)
        {
            goto ERROR;
        }
    }

    /* ------ Start test.----------- */
    print_header_start(" UDP RX Test");
    PRINTF("Local ports ");
    for (loop = 0; loop < multi_socket_count; loop++)
        PRINTF("%d ", port[loop]);
    PRINTF(NL);
    print_header_end();

    /*Initialize start, stop times*/
    app_time_get_elapsed(&(tCxt.last_time));
    app_time_get_elapsed(&(tCxt.first_time));
    tCxt.last_bytes  = 0;
    tCxt.last_kbytes = 0;
    block_time       = tCxt.first_time;

    while (test_for_quit() == 0) /* Main loop */
    {
        // PRINTF("Waiting." NL);
        for (loop = 0; loop < multi_socket_count; loop++)
        {
            assert((void *)handle != NULL);
            if ((conn_sock = t_select((void *)handle, sock_local[loop], UDP_CONNECTION_WAIT_TIME)) == A_OK)
            {
                app_time_get_elapsed(&(tCxt.last_time));
                app_time_get_elapsed(&(tCxt.first_time));
                tCxt.last_bytes  = 0;
                tCxt.last_kbytes = 0;
                block_time       = tCxt.first_time;
                tCxt.bytes       = 0;
                tCxt.kbytes      = 0;
                tCxt.sent_bytes  = 0;
                tCxt.sock_local  = sock_local[loop];
                is_first         = 1;

                while (test_for_quit() == 0)
                {
#if ZERO_COPY
                    if (!v6EnableFlag)
                    {
                        addr_len = sizeof(SOCKADDR_T);
                        received = qcom_recvfrom(sock_local[loop], (&tCxt.buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                                 (struct sockaddr *)&addr, &addr_len);
                    }
                    else
                    {
                        addr_len = sizeof(SOCKADDR_6_T);
                        received = qcom_recvfrom(sock_local[loop], (&tCxt.buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                                 (struct sockaddr *)&addr6, &addr_len);
                    }
                    if (received > 0)
                    {
                        zero_copy_free(tCxt.buffer);
                    }
#else
                    if (!v6EnableFlag)
                    {
                        received = qcom_recvfrom(sock_local[loop], (char *)(&tCxt.buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0,
                                                 &addr, &addr_len);
                    }
                    else
                    {
                        addr_len = sizeof(SOCKADDR_6_T);
                        received = qcom_recvfrom(sock_local[loop], (char *)(&tCxt.buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0,
                                                 &addr6, &addr_len);
                    }
#endif

                    if (received >= sizeof(EOT_PACKET))
                    {
                        /* Reset timeout. */
                        if (received > sizeof(EOT_PACKET))
                        {
                            taskYIELD();
                            app_time_get_elapsed(&tCxt.last_time);
                        }
                        if (is_first)
                        {
                            if (received > sizeof(EOT_PACKET))
                            {
                                temp_addr = ntohl(addr.sin_addr.s_addr);
                                ip_str    = A_MALLOC(sizeof(uint8_t) * 48, MALLOC_ID_TEMPORARY);
                                if (NULL == ip_str)
                                {
                                    PRINTF("Out of Memory error" NL);
                                    goto ERROR;
                                }

                                if (!v6EnableFlag)
                                {
                                    PRINTF("Receiving from %s:%d" NL,
                                           inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str), addr.sin_port);
                                }
                                else
                                {
                                    PRINTF("Receiving from %s:%d" NL,
                                           inet6_ntoa((char *)&addr6.sin6_addr, (char *)ip_str), addr6.sin6_port);
                                }

                                if (NULL != ip_str)
                                    A_FREE(ip_str, MALLOC_ID_TEMPORARY);

                                app_time_get_elapsed(&tCxt.first_time);
                                is_first = 0;
                            }
                        }
                        else /* if(is_first) */
                        {
                            tCxt.bytes += received;
                            tCxt.sent_bytes += received;
                            if (test_for_delay(&tCxt.last_time, &block_time))
                            {
                                /* block to give other tasks an opportunity to run */
                                A_MDELAY(SMALL_TX_DELAY);
                                app_time_get_elapsed(&block_time);
                            }
                            if (received == sizeof(EOT_PACKET)) /* End of transfer. */
                            {
                                /* Send throughput results to Peer so that it can display correct results*/
                                if (!v6EnableFlag)
                                {
                                    sendAck(&tCxt, (A_VOID *)&addr, port[loop]);
                                }
                                else
                                {
                                    sendAck(&tCxt, (A_VOID *)&addr6, port[loop]);
                                }

                                tCxt.kbytes    = tCxt.bytes / 1024;
                                tCxt.bytes     = tCxt.bytes % 1024;
                                tCxt.test_type = UDP_RX;
                                print_test_results(&tCxt);
                                break;

                            } /* End of transfer. */
                        }     /* if(is_first) */
                    }         /* if(received >= sizeof(EOT_PACKET)) */
                }             /* while(test_for_quit()==0) */
            } /* if((conn_sock  =  t_select((void*)handle, sock_local[loop],UDP_CONNECTION_WAIT_TIME_MULTI_SOCK)) ==
                 A_OK) */
        }     /* for (loop = 0; loop < multi_socket_count; loop++) */
    }         /* Main loop */

ERROR:
    // qcom_socket_close(p_tCxt->sock_local);
    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if ((0 != sock_local[loop]) && ((uint32_t)A_ERROR != sock_local[loop]))
        {
            qcom_socket_close(sock_local[loop]);
        }
    }

    PRINTF(CFG_COMPLETED_STR);
    PRINT_SHELL_PROMPT

#if !ZERO_COPY
    if (tCxt.buffer)
        A_FREE(tCxt.buffer, MALLOC_ID_CONTEXT);
#endif
    // num_traffic_streams = 0;
    return;
}

/************************************************************************
 * NAME: ath_tcp_rx_multi_socket
 *
 * DESCRIPTION: Start throughput TCP server.
 ************************************************************************/

#if T_SELECT_VER1
static void ath_tcp_rx_multi_socket(int16_t *port, int32_t multi_socket_count)
{
    uint8_t *ip_str    = NULL;
    int32_t result     = 0;
    socklen_t addr_len = 0;
    int32_t received = 0, loop = 0, error = A_OK;
    uint8_t is_first[TCP_MULTI_SOCKET_MAX], count;
    uint32_t r_fd, r_fd_working_set;
    _ip_address temp_addr;
    typedef union
    {
        SOCKADDR_T foreign_addr;
        SOCKADDR_6_T foreign_addr6;
    } SockAddr_t;
    SockAddr_t u_addr;
    THROUGHPUT_CXT tCxt[TCP_MULTI_SOCKET_MAX];
    (void)result;

    bench_quit = 0;
    for (loop = 0; loop < multi_socket_count; loop++)
    {
        tCxt[loop].sock_peer = 0;
    }

#if !ZERO_COPY
    if ((tCxt.buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        goto ERROR;
    }
#endif

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if (!v6EnableFlag)
        {
            error = ath_create_socket_and_bind(&(tCxt[loop].sock_local), port[loop], SOCK_STREAM_TYPE);
        }
        else
        {
            error = ath_create_socket_and_bind6(&(tCxt[loop].sock_local), port[loop], SOCK_STREAM_TYPE);
        }

        if (error == A_ERROR)
        {
            goto ERROR;
        }
    }

    /* ------ Start test.----------- */
    PRINTF("****************************************************" NL);
    PRINTF(" TCP Multi RX Test" NL);
    PRINTF("****************************************************" NL);

    PRINTF("Local ports ");
    for (loop = 0; loop < multi_socket_count; loop++)
        PRINTF("%d ", port[loop]);
    PRINTF(NL);
    print_header_end();

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        app_time_get_elapsed(&tCxt[loop].first_time);
        app_time_get_elapsed(&tCxt[loop].last_time);
        tCxt[loop].last_bytes  = 0;
        tCxt[loop].last_kbytes = 0;
        is_first[loop]         = 1;
    }

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if (qcom_listen(tCxt[loop].sock_local, 1) == A_ERROR)
        {
            PRINTF("ERROR: Socket listen error" NL);
            goto ERROR;
        }
    }

    // Form the r_fd mask
    r_fd = 0;
    for (loop = 0; loop < multi_socket_count; loop++)
    {
        FD_Set(tCxt[loop].sock_local, &r_fd);
    }
    count = multi_socket_count;
    while (test_for_quit() == 0)
    {
        r_fd_working_set = r_fd;
        assert((void *)handle != NULL);
        if ((result = (t_select_ver1((void *)handle, count, &r_fd_working_set, NULL, NULL, UDP_CONNECTION_WAIT_TIME))) >
            0)
        {
            for (loop = 0; loop < multi_socket_count; loop++)
            {
                // Data reception here
                if ((tCxt[loop].sock_peer) && FD_IsSet(tCxt[loop].sock_peer, r_fd_working_set))
                {
#if ZERO_COPY
                    received = qcom_recv(tCxt[loop].sock_peer, (&(tCxt[loop].buffer)), CFG_PACKET_SIZE_MAX_RX, 0);
                    if (received > 0)
                        zero_copy_free(tCxt[loop].buffer);
#else
                    received = qcom_recv(tCxt[loop].sock_peer, (&tCxt[loop].buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0);
#endif
                    if (received > 0)
                    {
                        /*Valid packet received*/
                        if (is_first[loop])
                        {
                            /*This is the first packet, set initial time used to calculate throughput*/
                            app_time_get_elapsed(&(tCxt[loop].first_time));
                            is_first[loop] = 0;
                        }
                        app_time_get_elapsed(&(tCxt[loop].last_time));
                        tCxt[loop].bytes += received;
                        tCxt[loop].sent_bytes += received;
                    }
                    else // either the socket is closed or we got some error
                    {
                        FD_Clr(tCxt[loop].sock_peer, &r_fd);
                        // Close the socket.
                        tCxt[loop].sock_peer = 0;
                        /*Test ended, peer closed connection*/
                        tCxt[loop].kbytes    = tCxt[loop].bytes / 1024;
                        tCxt[loop].bytes     = tCxt[loop].bytes % 1024;
                        tCxt[loop].test_type = TCP_RX;
                        /* Print test results.*/
                        print_test_results(&(tCxt[loop]));
                        if (r_fd == 0)
                        {
                            bench_quit = 1;
                            break;
                        }
                    }
                }

                // listener sockets here
                if ((tCxt[loop].sock_local) && (FD_IsSet(tCxt[loop].sock_local, r_fd_working_set)))
                {
                    tCxt[loop].last_bytes  = 0;
                    tCxt[loop].last_kbytes = 0;
                    tCxt[loop].bytes       = 0;
                    tCxt[loop].kbytes      = 0;
                    tCxt[loop].sent_bytes  = 0;

                    /*Accept incoming connection*/
                    // PRINTF("Doing an accept here" NL);
                    if (!v6EnableFlag)
                    {
                        addr_len = sizeof(u_addr.foreign_addr);
                        assert((void *)handle != NULL);
                        tCxt[loop].sock_peer =
                            t_accept_nb((void *)handle, tCxt[loop].sock_local, &(u_addr.foreign_addr), addr_len);
                        if (tCxt[loop].sock_peer == A_OK)
                        {
                            // PRINTF("Accept returned A_OK" NL);
                        }
                    }
                    else
                    {
                        addr_len = sizeof(u_addr.foreign_addr6);
                        assert((void *)handle != NULL);
                        tCxt[loop].sock_peer =
                            t_accept_nb((void *)handle, tCxt[loop].sock_local, &(u_addr.foreign_addr6), addr_len);
                        if (tCxt[loop].sock_peer == A_OK)
                        {
                            // PRINTF("Accept returned A_OK" NL);
                        }
                    }
                    // if a valid socket is found
                    if ((tCxt[loop].sock_peer != (uint32_t)A_ERROR) && (tCxt[loop].sock_peer != A_OK))
                    {
                        // PRINTF("new socket 0x%x" NL, tCxt[loop].sock_peer);
                        ip_str = A_MALLOC(sizeof(uint8_t) * 48, MALLOC_ID_TEMPORARY);
                        if (NULL == ip_str)
                        {
                            PRINTF("Out of Memory error" NL);
                            goto ERROR;
                        }
                        if (!v6EnableFlag)
                        {
                            temp_addr = ntohl(u_addr.foreign_addr.sin_addr.s_addr);
                            PRINTF("Receiving from %s:%d , port:%d" NL,
                                   inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str), u_addr.foreign_addr.sin_port,
                                   port[loop]);
                        }
                        else
                        {
                            PRINTF("Receiving from %s:%d, port:%d" NL,
                                   inet6_ntoa((char *)&(u_addr.foreign_addr6.sin6_addr), (char *)ip_str),
                                   u_addr.foreign_addr6.sin6_port, port[loop]);
                        }
                        if (NULL != ip_str)
                        {
                            A_FREE(ip_str, MALLOC_ID_TEMPORARY);
                            ip_str = NULL;
                        }
                        FD_Set(tCxt[loop].sock_peer, &r_fd);
                        FD_Clr(tCxt[loop].sock_local, &r_fd);
                        PRINTF("Shutting down socket 0x%x" NL, (unsigned int)tCxt[loop].sock_local);
                        assert((void *)handle != NULL);
                        t_shutdown(handle, tCxt[loop].sock_local);
                        tCxt[loop].sock_local = 0;
                    }
                    else if (tCxt[loop].sock_peer == (uint32_t)A_ERROR)
                    {
                        PRINTF("Error in t_accept for socket 0x%x" NL, (unsigned int)tCxt[loop].sock_local);
                        tCxt[loop].sock_peer = 0;
                        FD_Clr(tCxt[loop].sock_local, &r_fd);
                        assert((void *)handle != NULL);
                        t_shutdown(handle, tCxt[loop].sock_local);
                        tCxt[loop].sock_local = 0;
                    }
                }
            }
        }
    }

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        app_time_get_elapsed(&tCxt[loop].last_time);
        tCxt[loop].test_type = TCP_RX;
        /* Print test results.*/
        print_test_results(&tCxt[loop]);
    }

ERROR:
    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if ((0 != tCxt[loop].sock_local) && ((uint32_t)A_ERROR != tCxt[loop].sock_local))
        {
            qcom_socket_close(tCxt[loop].sock_local);
        }
    }

    PRINTF(CFG_COMPLETED_STR);
    PRINT_SHELL_PROMPT

#if !ZERO_COPY
    if (tCxt.buffer)
        A_FREE(tCxt.buffer, MALLOC_ID_CONTEXT);
#endif
    // num_traffic_streams = 0;
    return;
}

#else

static void ath_tcp_rx_multi_socket(int16_t *port, int32_t multi_socket_count)
{
    uint8_t *ip_str = NULL;
    socklen_t addr_len = 0;
    int32_t received = 0, loop = 0, error = A_OK;
    int32_t sock_local[TCP_MULTI_SOCKET_MAX];
    int32_t conn_sock, is_first;
    _ip_address temp_addr;
    SOCKADDR_T foreign_addr;
    SOCKADDR_6_T foreign_addr6;
    TIME_STRUCT block_time;
    THROUGHPUT_CXT tCxt;

    bench_quit = 0;
    tCxt.sock_peer = A_ERROR;

#if !ZERO_COPY
    if ((tCxt.buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        goto ERROR;
    }
#endif

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if (!v6EnableFlag)
        {
            error = ath_create_socket_and_bind(&(sock_local[loop]), port[loop], SOCK_STREAM_TYPE);
        }
        else
        {
            error = ath_create_socket_and_bind6(&(sock_local[loop]), port[loop], SOCK_STREAM_TYPE);
        }

        if (error == A_ERROR)
        {
            goto ERROR;
        }
    }

    /* ------ Start test.----------- */
    print_header_start(" TCP RX Test");
    PRINTF("Local ports ");
    for (loop = 0; loop < multi_socket_count; loop++)
        PRINTF("%d ", port[loop]);
    PRINTF(NL);
    print_header_end();

    app_time_get_elapsed(&tCxt.first_time);
    app_time_get_elapsed(&tCxt.last_time);
    tCxt.last_bytes = 0;
    tCxt.last_kbytes = 0;
    is_first = 1;

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if (qcom_listen(sock_local[loop], 1) == A_ERROR)
        {
            PRINTF("ERROR: Socket listen error" NL);
            goto ERROR;
        }
    }

    while (test_for_quit() == 0)
    {
        for (loop = 0; loop < multi_socket_count; loop++)
        {
            assert((void *)handle != NULL);
            if ((conn_sock = t_select((void *)handle, sock_local[loop], UDP_CONNECTION_WAIT_TIME)) == A_OK)
            {
                tCxt.last_bytes = 0;
                tCxt.last_kbytes = 0;
                tCxt.bytes = 0;
                tCxt.kbytes = 0;
                tCxt.sent_bytes = 0;
                tCxt.sock_local = sock_local[loop];

                /*Accept incoming connection*/
                if (!v6EnableFlag)
                {
                    addr_len = sizeof(foreign_addr);
                    tCxt.sock_peer = qcom_accept(tCxt.sock_local, &foreign_addr, &addr_len);
                }
                else
                {
                    addr_len = sizeof(foreign_addr6);
                    tCxt.sock_peer = qcom_accept(tCxt.sock_local, &foreign_addr6, &addr_len);
                }

                if (tCxt.sock_peer != A_ERROR)
                {
                    ip_str = A_MALLOC(sizeof(uint8_t) * 48, MALLOC_ID_TEMPORARY);
                    if (NULL == ip_str)
                    {
                        PRINTF("Out of Memory error" NL);
                        goto ERROR;
                    }
                    if (!v6EnableFlag)
                    {
                        temp_addr = ntohl(foreign_addr.sin_addr.s_addr);
                        PRINTF("Receiving from %s:%d" NL, inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str),
                               foreign_addr.sin_port);
                    }
                    else
                    {
                        PRINTF("Receiving from %s:%d" NL, inet6_ntoa((char *)&foreign_addr6.sin6_addr, (char *)ip_str),
                               foreign_addr6.sin6_port);
                    }
                    if (NULL != ip_str)
                        A_FREE(ip_str, MALLOC_ID_TEMPORARY);

                    app_time_get_elapsed(&tCxt.first_time);
                    app_time_get_elapsed(&tCxt.last_time);
                    block_time = tCxt.first_time;
                    is_first = 1;

                    while (test_for_quit() == 0) /* Receiving data.*/
                    {
                        if (test_for_quit())
                        {
                            app_time_get_elapsed(&tCxt.last_time);
                            tCxt.kbytes = tCxt.bytes / 1024;
                            tCxt.bytes = tCxt.bytes % 1024;
                            tCxt.test_type = TCP_RX;
                            /* Print test results.*/
                            print_test_results(&tCxt);
                            break;
                        }

                        assert((void *)handle != NULL);
                        conn_sock = t_select((void *)handle, tCxt.sock_peer, UDP_CONNECTION_WAIT_TIME);

                        if (conn_sock == A_OK)
                        {
/*Packet is available, receive it*/
#if ZERO_COPY
                            received = qcom_recv(tCxt.sock_peer, (char **)(&tCxt.buffer), CFG_PACKET_SIZE_MAX_RX, 0);
                            if (received > 0)
                                zero_copy_free(tCxt.buffer);
#else
                            received = qcom_recv(tCxt.sock_peer, (char *)(&tCxt.buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0);
#endif
                            if (received == A_SOCK_INVALID)
                            {
                                /*Test ended, peer closed connection*/
                                tCxt.kbytes = tCxt.bytes / 1024;
                                tCxt.bytes = tCxt.bytes % 1024;
                                tCxt.test_type = TCP_RX;
                                /* Print test results.*/
                                print_test_results(&tCxt);
                                break;
                                // goto ERROR;
                            }
                        }

                        if (conn_sock == A_SOCK_INVALID)
                        {
                            /* Test ended, peer closed connection*/
                            tCxt.kbytes = tCxt.bytes / 1024;
                            tCxt.bytes = tCxt.bytes % 1024;
                            tCxt.test_type = TCP_RX;
                            /* Print test results.*/
                            print_test_results(&tCxt);
                            if (qcom_listen(tCxt.sock_local, 1) == A_ERROR)
                            {
                                PRINTF("ERROR: Socket listen error" NL);
                                goto ERROR;
                            }
                            // goto ERROR;
                            break;
                        } /* if(conn_sock == A_SOCK_INVALID) */
                        else
                        {
                            /*Valid packet received*/
                            if (is_first)
                            {
                                /*This is the first packet, set initial time used to calculate throughput*/
                                app_time_get_elapsed(&tCxt.first_time);
                                is_first = 0;
                            }
                            app_time_get_elapsed(&tCxt.last_time);
                            tCxt.bytes += received;
                            tCxt.sent_bytes += received;

                            if (test_for_delay(&tCxt.last_time, &block_time))
                            {
                                /* block to give other tasks an opportunity to run */
                                A_MDELAY(SMALL_TX_DELAY);
                                app_time_get_elapsed(&block_time);
                            }
                        } /* if(conn_sock == A_SOCK_INVALID) */
                    }     /* while(test_for_quit() == 0) */
                }         /* if(tCxt->sock_peer != A_ERROR) */

                if (test_for_quit())
                {
                    app_time_get_elapsed(&tCxt.last_time);
                    tCxt.kbytes = tCxt.bytes / 1024;
                    tCxt.bytes = tCxt.bytes % 1024;
                    tCxt.test_type = TCP_RX;
                    /* Print test results.*/
                    print_test_results(&tCxt);
                }
            } /* if((conn_sock  =  t_select((void*)handle, tCxt[loop].sock_local,UDP_CONNECTION_WAIT_TIME_MULTI_SOCK))
                 == A_OK) */
        }     /* for (loop = 0; loop < multi_socket_count; loop++) */
    }         /* while(test_for_quit()==0) */

ERROR:

    for (loop = 0; loop < multi_socket_count; loop++)
    {
        if ((0 != sock_local[loop]) && (A_ERROR != sock_local[loop]))
        {
            qcom_socket_close(sock_local[loop]);
        }
    }

    PRINTF(CFG_COMPLETED_STR);
    PRINT_SHELL_PROMPT

#if !ZERO_COPY
    if (tCxt.buffer)
        A_FREE(tCxt.buffer, MALLOC_ID_CONTEXT);
#endif
    // num_traffic_streams = 0;
    return;
}
#endif // #if t_select_ver1
#endif // #if MULTI_SOCKET_SUPPORT

/************************************************************************
 * NAME: ath_tcp_rx
 *
 * DESCRIPTION: Start throughput TCP server.
 ************************************************************************/
static void ath_tcp_rx(THROUGHPUT_CXT *p_tCxt)
{
    uint8_t ip_str[16];
    socklen_t addr_len = 0;
    uint32_t port;
    int32_t received = 0;

    int32_t conn_sock, isFirst = 1;
    _ip_address temp_addr;
    SOCKADDR_T local_addr;
    SOCKADDR_T foreign_addr;
    SOCKADDR_6_T local_addr6;
    SOCKADDR_6_T foreign_addr6;
    uint8_t ip6_str[48];
#if ENABLE_SSL
    SSL_INST *ssl = &ssl_inst[SSL_SERVER_INST];
#endif
    TIME_STRUCT block_time;
    int32_t result;

    port              = p_tCxt->params.rx_params.port;
    bench_quit        = 0;
    p_tCxt->sock_peer = (uint32_t)A_ERROR;

    num_traffic_streams++;

    set_power_mode(MAX_PERF_POWER, TCPL1);

#if !ZERO_COPY
    /*Allocate buffer*/
    if ((p_tCxt->buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        goto ERROR_1;
    }
#endif

    if (!v6EnableFlag)
    {
        /* Create listen socket */
        if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Socket creation error" NL);
            goto ERROR_1;
        }

        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_port   = port;
        local_addr.sin_family = ATH_AF_INET;
        if (p_tCxt->params.rx_params.local_if == 1)
        {
            local_addr.sin_addr.s_addr = p_tCxt->params.rx_params.local_address;
        }

        /* Bind socket.*/
        if (qcom_bind(p_tCxt->sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr)) == A_ERROR)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }
    }
    else
    {
        if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET6, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Socket creation error" NL);
            goto ERROR_1;
        }

        memset(&local_addr6, 0, sizeof(local_addr6));
        local_addr6.sin6_port   = port;
        local_addr6.sin6_family = ATH_AF_INET6;
        if (p_tCxt->params.rx_params.local_if == 1)
        {
            memcpy(&local_addr6.sin6_addr, p_tCxt->params.rx_params.local_v6addr, 16);
        }

        /* Bind socket.*/
        if (qcom_bind(p_tCxt->sock_local, (struct sockaddr *)&local_addr6, sizeof(local_addr6)) == A_ERROR)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }
    }

    /* ------ Start test.----------- */
    PRINTF(NL "****************************************************" NL);
#if ENABLE_SSL
    PRINTF(" %s RX Test" NL, p_tCxt->test_type == SSL_RX ? "SSL" : "TCP");
#else
    PRINTF(" TCP RX Test" NL);
#endif
    PRINTF("****************************************************" NL);

    PRINTF("Local port %d" NL, port);
    print_header_end();

    app_time_get_elapsed(&p_tCxt->first_time);
    app_time_get_elapsed(&p_tCxt->last_time);
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;

    while (1)
    {
        PRINTF("Waiting." NL);

        /* Listen. */
        if (qcom_listen(p_tCxt->sock_local, 1) == A_ERROR)
        {
            PRINTF("ERROR: Socket listen error" NL);
            goto ERROR_2;
        }

        p_tCxt->bytes      = 0;
        p_tCxt->kbytes     = 0;
        p_tCxt->sent_bytes = 0;

        do
        {
            if (test_for_quit())
            {
                goto tcp_rx_QUIT;
            }

            /* block for 500msec or until a packet is received */
            assert((void *)handle != NULL);
            conn_sock = t_select((void *)handle, p_tCxt->sock_local, UDP_CONNECTION_WAIT_TIME);

            if (conn_sock == A_SOCK_INVALID)
                goto tcp_rx_QUIT; // Peer closed connection, socket no longer valid

        } while (conn_sock == A_ERROR);

        /*Accept incoming connection*/
        if (!v6EnableFlag)
        {
            addr_len          = sizeof(foreign_addr);
            p_tCxt->sock_peer = qcom_accept(p_tCxt->sock_local, (struct sockaddr *)&foreign_addr, &addr_len);
        }
        else
        {
            addr_len          = sizeof(foreign_addr6);
            p_tCxt->sock_peer = qcom_accept(p_tCxt->sock_local, (struct sockaddr *)&foreign_addr6, &addr_len);
        }

        if (p_tCxt->sock_peer != (uint32_t)A_ERROR)
        {
#if ENABLE_SSL
            if (p_tCxt->test_type == SSL_RX)
            {
                if (ssl->ssl == NULL)
                {
                    // Create SSL connection object
                    ssl->ssl = qcom_SSL_new(ssl->sslCtx);
                    if (ssl->ssl == NULL)
                    {
                        PRINTF("ERROR: Unable to create SSL context" NL);
                        goto tcp_rx_QUIT;
                    }

                    // configure the SSL connection
                    if (ssl->config_set)
                    {
                        result = qcom_SSL_configure(ssl->ssl, &ssl->config);
                        if (result < A_OK)
                        {
                            PRINTF("ERROR: SSL configure failed (%d)" NL, result);
                            goto tcp_rx_QUIT;
                        }
                    }
                }

                // Add socket handle to SSL connection
                result = qcom_SSL_set_fd(ssl->ssl, p_tCxt->sock_peer);
                if (result < A_OK)
                {
                    PRINTF("ERROR: Unable to add socket handle to SSL" NL);
                    goto tcp_rx_QUIT;
                }

                // SSL handshake with server
                result = SSL_accept(ssl->ssl);
                if (result < 0)
                {
                    PRINTF("ERROR: SSL accept failed (%d)" NL, result);
                    goto tcp_rx_QUIT;
                }
            }
#endif
            memset(ip_str, 0, sizeof(ip_str));
            if (!v6EnableFlag)
            {
                temp_addr = ntohl(foreign_addr.sin_addr.s_addr);
                PRINTF("Receiving from %s:%d" NL, inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str),
                       foreign_addr.sin_port);
            }
            else
            {
                PRINTF("Receiving from %s:%d" NL, inet6_ntoa((char *)&foreign_addr6.sin6_addr, (char *)ip6_str),
                       foreign_addr6.sin6_port);
            }

            app_time_get_elapsed(&p_tCxt->first_time);
            app_time_get_elapsed(&p_tCxt->last_time);
            block_time = p_tCxt->first_time;
            isFirst    = 1;

            while (1) /* Receiving data.*/
            {
                if (test_for_quit())
                {
                    qcom_socket_close(p_tCxt->sock_peer);
                    app_time_get_elapsed(&p_tCxt->last_time);
                    goto tcp_rx_QUIT;
                }

                assert((void *)handle != NULL);
                conn_sock = t_select((void *)handle, p_tCxt->sock_peer, UDP_CONNECTION_WAIT_TIME);

                if (conn_sock == A_OK)
                {
/*Packet is available, receive it*/
#if ZERO_COPY
#if ENABLE_SSL
                    if (p_tCxt->test_type == SSL_RX)
                    {
                        received = SSL_read(ssl->ssl, (void **)&p_tCxt->buffer, CFG_PACKET_SIZE_MAX_RX);
                    }
                    else
#endif
                    {
                        received = qcom_recv(p_tCxt->sock_peer, (&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0);
                    }
                    if (received > 0)
                    {
                        zero_copy_free(p_tCxt->buffer);
                    }
#else
#if ENABLE_SSL
                    if (p_tCxt->ssl_test)
                    {
                        received = SSL_read(ssl->ssl, p_tCxt->buffer, CFG_PACKET_SIZE_MAX_RX);
                    }
                    else
#endif
                    {
                        received =
                            qcom_recv(p_tCxt->sock_peer, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0);
                    }
#endif

                    if (received == A_SOCK_INVALID)
                    {
                        /*Test ended, peer closed connection*/
                        p_tCxt->kbytes = p_tCxt->bytes / 1024;
                        p_tCxt->bytes  = p_tCxt->bytes % 1024;
#if ENABLE_SSL
                        if (ssl_role == SSL_SERVER && ssl->ssl != NULL)
                        {
                            qcom_SSL_shutdown(ssl->ssl);
                            ssl->ssl = NULL;
                        }
#endif

                        /* Print test results.*/
                        print_test_results(p_tCxt);
                        // goto ERROR_2;
                        break;
                    }
                }

                if (conn_sock == A_SOCK_INVALID)
                {
                    /* Test ended, peer closed connection*/
                    p_tCxt->kbytes = p_tCxt->bytes / 1024;
                    p_tCxt->bytes  = p_tCxt->bytes % 1024;
#if ENABLE_SSL
                    if (ssl_role == SSL_SERVER && ssl->ssl != NULL)
                    {
                        qcom_SSL_shutdown(ssl->ssl);
                        ssl->ssl = NULL;
                    }
#endif
                    /* Print test results.*/
                    print_test_results(p_tCxt);
                    // goto ERROR_2;
                    break;
                }
                else
                {
                    /*Valid packet received*/
                    // PRINTF("RX: %d" NL, received);
                    if (isFirst)
                    {
                        /*This is the first packet, set initial time used to calculate throughput*/
                        app_time_get_elapsed(&p_tCxt->first_time);
                        isFirst = 0;
                    }
                    app_time_get_elapsed(&p_tCxt->last_time);
                    p_tCxt->bytes += received;
                    p_tCxt->sent_bytes += received;
                    received = 0;
                    if (test_for_delay(&p_tCxt->last_time, &block_time))
                    {
                        /* block to give other tasks an opportunity to run */
                        A_MDELAY(TX_DELAY);
                        app_time_get_elapsed(&block_time);
                    }
                }
            }
        }

        if (test_for_quit())
        {
            app_time_get_elapsed(&p_tCxt->last_time);
            goto tcp_rx_QUIT;
        }
    }

tcp_rx_QUIT:
    p_tCxt->kbytes = p_tCxt->bytes / 1024;
    p_tCxt->bytes  = p_tCxt->bytes % 1024;
    /* Print test results.*/
    print_test_results(p_tCxt);

    // qcom_socket_close( p_tCxt->sock_peer);

ERROR_2:
    qcom_socket_close(p_tCxt->sock_local);

ERROR_1:
#if ENABLE_SSL
    if (ssl_role == SSL_SERVER && ssl->ssl != NULL)
    {
        qcom_SSL_shutdown(ssl->ssl);
        ssl->ssl = NULL;
    }
#endif
    PRINTF("*************IOT Throughput Test Completed **************" NL);
    PRINT_SHELL_PROMPT
    num_traffic_streams--;
#if !ZERO_COPY
    if (p_tCxt->buffer)
        A_FREE(p_tCxt->buffer, MALLOC_ID_CONTEXT);
#endif
    set_power_mode(REC_POWER, TCPL1);
}

typedef struct ip
{
    unsigned char ip_ver_ihl;    /* 4 bit version, 4 bit hdr len in 32bit words */
    unsigned char ip_tos;        /* Type of Service */
    unsigned short ip_len;       /* Total packet length including header */
    unsigned short ip_id;        /* ID for fragmentation */
    unsigned short ip_flgs_foff; /* mask in flags as needed */
    unsigned char ip_time;       /* Time to live (secs) */
    unsigned char ip_prot;       /* protocol */
    unsigned short ip_chksum;    /* Header checksum */
    unsigned long ip_src;        /* Source name */
    unsigned long ip_dest;       /* Destination name */
} host_ip;

typedef struct ipv6
{
    unsigned char ip_ver_flo[4]; /* 4 bits version (6), 8 bits class, & flow label */
    unsigned short ip_len;       /* payload length */
    unsigned char ip_nexthdr;    /* next header type 6 = TCP, etc */
    unsigned char ip_hopcount;   /* hops left until expires */
    unsigned char ip_src[16];    /* source and dest addresses */
    unsigned char ip_dest[16];
} host_ip6;

/************************************************************************
 * NAME: ath_udp_rx
 *
 * DESCRIPTION: Start throughput UDP server.
 ************************************************************************/
static void ath_udp_rx(THROUGHPUT_CXT *p_tCxt)
{
    uint8_t ip_str[16];
    socklen_t addr_len;
    uint16_t port;
    int32_t received;
    int32_t conn_sock;
    int32_t is_first = 1;
    SOCKADDR_T addr;
    SOCKADDR_T local_addr;
    SOCKADDR_6_T addr6;
    SOCKADDR_6_T local_addr6;
    uint8_t ip6_str[48];
    uint32_t temp_addr;
    TIME_STRUCT block_time;
    int overhead = 0;

    set_power_mode(MAX_PERF_POWER, TCPL1);
    // init quit flag
    bench_quit = 0;

    // Parse remaining parameters
    port = p_tCxt->params.rx_params.port;

    num_traffic_streams++;

#if !ZERO_COPY
    if ((p_tCxt->buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        set_power_mode(REC_POWER, TCPL1);
        return;
    }
#endif

    if (!v6EnableFlag)
    {
        if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET, SOCK_DGRAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Socket creation error" NL);
            goto ERROR_1;
        }

        /* Bind.*/
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_port   = port;
        local_addr.sin_family = ATH_AF_INET;
        if (p_tCxt->params.rx_params.local_if == 1)
        {
            local_addr.sin_addr.s_addr = p_tCxt->params.rx_params.local_address;
        }

        if (qcom_bind(p_tCxt->sock_local, (struct sockaddr *)(&local_addr), sizeof(local_addr)) != A_OK)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }

        /*************Multicast support ************************/
        if (handle_mcast_param(p_tCxt) != A_OK)
            goto ERROR_2;
    }
    else
    {
        if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET6, SOCK_DGRAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Socket creation error" NL);
            goto ERROR_1;
        }

        memset(&local_addr6, 0, sizeof(local_addr6));
        local_addr6.sin6_port   = port;
        local_addr6.sin6_family = ATH_AF_INET6;
        if (p_tCxt->params.rx_params.local_if == 1)
        {
            memcpy(&local_addr6.sin6_addr, p_tCxt->params.rx_params.local_v6addr, sizeof(IP6_ADDR_T));
        }

        if (qcom_bind(p_tCxt->sock_local, (struct sockaddr *)(&local_addr6), sizeof(local_addr6)) != A_OK)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }

        /*************Multicast support ************************/
        if (handle_mcast_param(p_tCxt) != A_OK)
            goto ERROR_2;
    }

    /* ------ Start test.----------- */
    print_header_start(" UDP RX Test");
    PRINTF("Local port %d" NL, port);
    print_header_end();

    /*Initilize start, stop times*/

    app_time_get_elapsed(&p_tCxt->last_time);
    app_time_get_elapsed(&p_tCxt->first_time);
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;
    block_time          = p_tCxt->first_time;

    if (p_tCxt->params.rx_params.ip_hdr_inc == 1)
    {
        if (!v6EnableFlag)
            overhead = 28; /* IP_HDR_SIZE  + UDP_HDR_SIZE */
        else
            overhead = 48; /* IP6_HDR_SIZE + UDP_HDR_SIZE */
    }
    while (test_for_quit() == 0) /* Main loop */
    {
        PRINTF("Waiting." NL);

        p_tCxt->bytes  = 0;
        p_tCxt->kbytes = 0;

        p_tCxt->sent_bytes = 0;

        addr_len = sizeof(SOCKADDR_T);
        is_first = 1;

        while (test_for_quit() == 0)
        {
            do
            {
                if (test_for_quit())
                {
                    goto ERROR_3;
                }
                /* block for 500msec or until a packet is received */
                assert((void *)handle != NULL);
                conn_sock = t_select((void *)handle, p_tCxt->sock_local, UDP_CONNECTION_WAIT_TIME);

                if (conn_sock == A_SOCK_INVALID)
                    goto ERROR_3; // socket no longer valid

            } while (conn_sock == A_ERROR);

/* Receive data */
#if ZERO_COPY
            if (!v6EnableFlag)
            {
                received = qcom_recvfrom(p_tCxt->sock_local, (&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                         (struct sockaddr *)&addr, &addr_len);
            }
            else
            {
                addr_len = sizeof(SOCKADDR_6_T);
                received = qcom_recvfrom(p_tCxt->sock_local, (&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                         (struct sockaddr *)&addr6, &addr_len);
            }
            if (received > 0)
                zero_copy_free(p_tCxt->buffer);
#else
            if (!v6EnableFlag)
            {
                received = qcom_recvfrom(p_tCxt->sock_local, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0,
                                         &addr, &addr_len);
            }
            else
            {
                addr_len = sizeof(SOCKADDR_6_T);
                received = qcom_recvfrom(p_tCxt->sock_local, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0,
                                         &addr6, &addr_len);
            }
#endif
            if (received >= sizeof(EOT_PACKET) + overhead)
            {
                /* Reset timeout. */
                if (received > sizeof(EOT_PACKET))
                {
                    app_time_get_elapsed(&p_tCxt->last_time);
                }

                if (is_first)
                {
                    if (received > (sizeof(EOT_PACKET) + overhead))
                    {
                        if (p_tCxt->params.rx_params.ip_hdr_inc == 1)
                        {
                            if (!v6EnableFlag)
                            {
                                host_ip *ip = NULL;
                                ip          = (host_ip *)(&p_tCxt->buffer[0]);
                                memset(ip_str, 0, sizeof(ip_str));
                                PRINTF("Src IP of RX Pkt %s " NL, inet_ntoa((uint32_t)(ip->ip_dest), (char *)ip_str));
                            }
                            else
                            {
                                host_ip6 *ip = NULL;
                                ip           = (host_ip6 *)(&p_tCxt->buffer[0]);
                                memset(ip6_str, 0, sizeof(ip6_str));
                                PRINTF("SrcIP of RX PKT %s " NL, inet6_ntoa((char *)&ip->ip_dest, (char *)ip6_str));
                            }
                        }

                        temp_addr = ntohl(addr.sin_addr.s_addr);

                        if (!v6EnableFlag)
                        {
                            PRINTF("Receiving from %s:%d" NL, inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str),
                                   addr.sin_port);
                        }
                        else
                        {
                            PRINTF("Receiving from %s:%d" NL, inet6_ntoa((char *)&addr6.sin6_addr, (char *)ip6_str),
                                   addr6.sin6_port);
                        }

                        app_time_get_elapsed(&p_tCxt->first_time);
                        is_first = 0;
                    }
                }
                else
                {
                    p_tCxt->bytes += received;
                    p_tCxt->sent_bytes += received;

                    if (test_for_delay(&p_tCxt->last_time, &block_time))
                    {
                        /* block to give other tasks an opportunity to run */
                        A_MDELAY(SMALL_TX_DELAY);
                        app_time_get_elapsed(&block_time);
                    }
                    if (received == sizeof(EOT_PACKET) + overhead) /* End of transfer. */
                    {
                        /* Send throughput results to Peer so that it can display correct results*/
                        /* Send throughput results to Peer so that it can display correct results*/
                        if (!v6EnableFlag)
                        {
                            sendAck(p_tCxt, (A_VOID *)&addr, port);
                        }
                        else
                        {
                            sendAck(p_tCxt, (A_VOID *)&addr6, port);
                        }

                        break;
                    }
                }
            }
        }

    ERROR_3:
        p_tCxt->kbytes    = p_tCxt->bytes / 1024;
        p_tCxt->bytes     = p_tCxt->bytes % 1024;
        p_tCxt->test_type = UDP_RX;
        print_test_results(p_tCxt);
    }
ERROR_2:
    qcom_socket_close(p_tCxt->sock_local);

ERROR_1:

    PRINTF(CFG_COMPLETED_STR);
    PRINT_SHELL_PROMPT

#if !ZERO_COPY
    if (p_tCxt->buffer)
        A_FREE(p_tCxt->buffer, MALLOC_ID_CONTEXT);
#endif

    num_traffic_streams--;
    set_power_mode(REC_POWER, TCPL1);
}

#if ENABLE_RAW_SOCKET_DEMO
char eapol_pkt[60];

/************************************************************************
 * NAME: ath_raw_rx
 *
 * DESCRIPTION: Start throughput RAW server.
 ************************************************************************/
static void ath_raw_rx(THROUGHPUT_CXT *p_tCxt)
{
    uint8_t ip_str[16];
    socklen_t addr_len;
    int32_t received;
    int32_t conn_sock;
    int32_t is_first = 1;
    SOCKADDR_T local_addr, foreign_addr;
    TIME_STRUCT block_time;
    uint32_t hdrinc;
    uint8_t src_mac[6], dst_mac[6], last_receive_pkt_size = 0;

    // init quit flag
    bench_quit = 0;

    num_traffic_streams++;

#if !ZERO_COPY
    if ((p_tCxt->buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        return;
    }
#endif

    // For EAPOL
    if (p_tCxt->params.rx_params.raw_mode == ETH_RAW)
        p_tCxt->params.rx_params.port = ATH_ETH_P_PAE;

    /* Create socket */
    if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET, SOCK_RAW_TYPE, p_tCxt->params.rx_params.port)) ==
        (uint32_t)A_ERROR)
    {
        PRINTF("ERROR: Socket creation error" NL);
        goto ERROR_1;
    }

    local_addr.sin_addr.s_addr = p_tCxt->params.rx_params.local_address;

    /* Bind.*/
    if (p_tCxt->params.rx_params.local_if == 1)
    {
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_family      = ATH_AF_INET;
        local_addr.sin_addr.s_addr = p_tCxt->params.rx_params.local_address;
        if (qcom_bind(p_tCxt->sock_local, (struct sockaddr *)(&local_addr), sizeof(local_addr)) != A_OK)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }
    }

    if (p_tCxt->params.rx_params.ip_hdr_inc == 1)
    {
        hdrinc = 1;
        PRINTF("Setting hdrinc option" NL);
        /* Set IP_HDRINCL socket option if need to receive the packet with IP header */
        if (qcom_setsockopt(p_tCxt->sock_local, ATH_IPPROTO_IP, IP_HDRINCL, (uint8_t *)(&(hdrinc)), sizeof(int)) !=
            A_OK)
        {
            PRINTF("SetsockOPT error: unable to set IP_HDRINCL" NL);
            goto ERROR_2;
        }

        // t_ipconfig((void*)handle, IPCFG_QUERY, &(local_addr.sin_addr.s_addr), &mask, &gw, NULL, NULL);
    }

    if (p_tCxt->params.rx_params.mcastEnabled == 1)
    {
        p_tCxt->params.rx_params.group.imr_multiaddr = A_CPU2BE32(p_tCxt->params.rx_params.group.imr_multiaddr);
        p_tCxt->params.rx_params.group.imr_interface = A_CPU2BE32(p_tCxt->params.rx_params.group.imr_interface);
        if (qcom_setsockopt(p_tCxt->sock_local, ATH_IPPROTO_IP, IP_ADD_MEMBERSHIP,
                            (uint8_t *)(&(p_tCxt->params.rx_params.group)), sizeof(IP_MREQ_T)) != A_OK)
        {
            PRINTF("SetsockOPT error: unable to add to multicast group" NL);
            goto ERROR_2;
        }
    }

    /* ------ Start test.----------- */
    print_header_start(" RAW RX Test");
    print_header_end();

    /*Initilize start, stop times*/

    app_time_get_elapsed(&p_tCxt->last_time);
    app_time_get_elapsed(&p_tCxt->first_time);
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;
    block_time          = p_tCxt->first_time;

    while (test_for_quit() == 0) /* Main loop */
    {
        PRINTF("Waiting." NL);

        p_tCxt->bytes  = 0;
        p_tCxt->kbytes = 0;

        p_tCxt->sent_bytes = 0;

        addr_len = sizeof(SOCKADDR_T);
        is_first = 1;

        while (test_for_quit() == 0)
        {
            do
            {
                if (test_for_quit())
                {
                    goto ERROR_3;
                }
                /* block for 500msec or until a packet is received */
                assert((void *)handle != NULL);
                conn_sock = t_select((void *)handle, p_tCxt->sock_local, UDP_CONNECTION_WAIT_TIME);

                if (conn_sock == A_SOCK_INVALID)
                    goto ERROR_3; // socket no longer valid

            } while (conn_sock == A_ERROR);

/* Receive data */
#if ZERO_COPY
            received = qcom_recvfrom(p_tCxt->sock_local, (&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                     (struct sockaddr *)&foreign_addr, &addr_len);
            if (received > 0)
            {
                // PRINTF("Recived :%d" NL, received);
                if (p_tCxt->params.rx_params.raw_mode == ETH_RAW)
                {
                    int i;
                    PRINTF("Received :%d" NL, received);

                    memcpy(eapol_pkt, p_tCxt->buffer, received);
                    for (i = 1; i <= received; i++)
                    {
                        PRINTF("%02x ", eapol_pkt[i - 1]);
                        if (i % 16 == 0)
                            PRINTF(NL);
                    }
                    PRINTF(NL);

                    memcpy(dst_mac, &p_tCxt->buffer[0], 6);
                    memcpy(src_mac, &p_tCxt->buffer[6], 6);
                    last_receive_pkt_size = received;
                }
                zero_copy_free(p_tCxt->buffer);
            }
#else
            received = qcom_recvfrom(p_tCxt->sock_local, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0,
                                     &foreign_addr, &addr_len);
#endif
            /* For ETH_P_PAE */
            if (p_tCxt->params.rx_params.raw_mode == ETH_RAW)
            {
                char *pkt_buffer;
                int data_len = received;
                int send_result;

                while ((pkt_buffer = CUSTOM_ALLOC(data_len)) == NULL)
                {
                    /*Allow small delay to allow other thread to run*/
                    A_MDELAY(SMALL_TX_DELAY);
                }
                memcpy(pkt_buffer, eapol_pkt, data_len);
                memcpy(pkt_buffer, src_mac, 6);
                memcpy(&(pkt_buffer[6]), dst_mac, 6);

                send_result = qcom_sendto(p_tCxt->sock_local, (pkt_buffer), data_len, 0, NULL, 0);
#if !NON_BLOCKING_TX
                /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
                if (pkt_buffer)
                {
                    CUSTOM_FREE(pkt_buffer);
                }
#endif
                if (send_result == A_ERROR)
                    PRINTF("Eapol send_error" NL);
                else
                    PRINTF("Eapol sent:%d" NL, data_len);
            }
            else /* For IP raw */
            {
                /* Decrementing the ip header size */
                if (p_tCxt->params.rx_params.ip_hdr_inc == 1)
                    received -= sizeof(ip_header);

                if (received >= sizeof(EOT_PACKET))
                {
                    /* Reset timeout. */
                    if (received > sizeof(EOT_PACKET))
                    {
                        app_time_get_elapsed(&p_tCxt->last_time);
                    }
                    if (is_first)
                    {
                        if (received > sizeof(EOT_PACKET))
                        {
                            uint32_t temp_addr = ntohl(foreign_addr.sin_addr.s_addr);
                            memset(ip_str, 0, sizeof(ip_str));

                            PRINTF("Receiving from %s" NL, inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str));
                            app_time_get_elapsed(&p_tCxt->first_time);
                            is_first = 0;
                        }
                    }
                    else
                    {
                        p_tCxt->bytes += received;
                        p_tCxt->sent_bytes += received;

                        if (test_for_delay(&p_tCxt->last_time, &block_time))
                        {
                            /* block to give other tasks an opportunity to run */
                            A_MDELAY(SMALL_TX_DELAY);
                            app_time_get_elapsed(&block_time);
                        }
                        if (received == sizeof(EOT_PACKET)) /* End of transfer. */
                        {
                            /* Send throughput results to Peer so that it can display correct results*/
                            sendRawAck(p_tCxt, (A_VOID *)&local_addr, (A_VOID *)&foreign_addr,
                                       p_tCxt->params.rx_params.ip_hdr_inc, p_tCxt->params.rx_params.port);
                            // sendAck( p_tCxt, (A_VOID*)&foreign_addr, port);
                            break;
                        } /* End of transfer. */
                    }     /* is_first */
                }
            } /* For IP raw */
        }     /* while(test_for_quit()==0) */

    ERROR_3:
        if (p_tCxt->params.rx_params.raw_mode == ETH_RAW)
        {
            int i;

            if (last_receive_pkt_size != 0)
            {
                PRINTF("Last packet received:" NL);

                for (i = 1; i <= last_receive_pkt_size; i++)
                {
                    PRINTF("%02x ", eapol_pkt[i - 1]);
                    if (i % 16 == 0)
                        PRINTF(NL);
                }
                PRINTF(NL);
            }
            else
            {
                PRINTF("No packet received" NL);
            }
        }
        else
        {
            p_tCxt->kbytes    = p_tCxt->bytes / 1024;
            p_tCxt->bytes     = p_tCxt->bytes % 1024;
            p_tCxt->test_type = RAW_RX;
            print_test_results(p_tCxt);
        }
    }
ERROR_2:
    qcom_socket_close(p_tCxt->sock_local);

ERROR_1:

    PRINTF(CFG_COMPLETED_STR);
    PRINT_SHELL_PROMPT

#if !ZERO_COPY
    if (p_tCxt->buffer)
        A_FREE(p_tCxt->buffer, MALLOC_ID_CONTEXT);
#endif

    num_traffic_streams--;
}
#endif

/************************************************************************
 * NAME: ath_udp_echo
 *
 * DESCRIPTION: A reference implementation of UDP Echo server. It will echo
 *              a packet received on specified port.
 ************************************************************************/
void ath_udp_echo(int32_t argc, char_ptr argv[])
{
    socklen_t addr_len;
    uint16_t port;
    int32_t received, result;
    int32_t conn_sock;
    SOCKADDR_T addr;
    SOCKADDR_T local_addr;
    SOCKADDR_6_T addr6;
    SOCKADDR_6_T local_addr6;
    char *rxBuffer, *txBuffer;

#if !ZERO_COPY
    PRINTF("This example is only supported with zero copy feature" NL);
    return;
#else
    // init quit flag
    bench_quit = 0;

    if (argc < 3)
    {
        PRINTF("Missing UDP port" NL);
        return;
    }
    /*Get listening port*/
    port = atoi(argv[2]);

    if (!v6EnableFlag)
    {
        if ((conn_sock = qcom_socket(ATH_AF_INET, SOCK_DGRAM_TYPE, 0)) == A_ERROR)
        {
            PRINTF("ERROR: Socket creation error" NL);
            goto ERROR_1;
        }

        /* Bind.*/
        memset(&local_addr, 0, sizeof(local_addr));
        local_addr.sin_port = port;
        local_addr.sin_family = ATH_AF_INET;

        if (qcom_bind(conn_sock, (struct sockaddr *)(&local_addr), sizeof(local_addr)) != A_OK)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }
    }
    else
    {
        if ((conn_sock = qcom_socket(ATH_AF_INET6, SOCK_DGRAM_TYPE, 0)) == A_ERROR)
        {
            PRINTF("ERROR: Socket creation error" NL);
            goto ERROR_1;
        }

        memset(&local_addr6, 0, sizeof(local_addr6));
        local_addr6.sin6_port = port;
        local_addr6.sin6_family = ATH_AF_INET6;

        if (qcom_bind(conn_sock, (struct sockaddr *)(&local_addr6), sizeof(local_addr6)) != A_OK)
        {
            PRINTF("ERROR: Socket bind error" NL);
            goto ERROR_2;
        }
    }

    /* ------ Start test.----------- */
    print_header_start(" UDP Echo Server");
    PRINTF("Local port %d" NL, port);
    print_header_end();

    addr_len = sizeof(SOCKADDR_T);

    while (test_for_quit() == 0)
    {
        do
        {
            if (test_for_quit())
            {
                goto ERROR_2;
            }
            /* block for 500msec or until a packet is received */
            assert((void *)handle != NULL);
            result = t_select((void *)handle, conn_sock, UDP_CONNECTION_WAIT_TIME);

            if (result == A_SOCK_INVALID)
                goto ERROR_2; // socket no longer valid

        } while (result == A_ERROR);

        /* Receive data */

        if (!v6EnableFlag)
        {
            received =
                qcom_recvfrom(conn_sock, (&rxBuffer), CFG_PACKET_SIZE_MAX_RX, 0, (struct sockaddr *)&addr, &addr_len);
        }
        else
        {
            addr_len = sizeof(SOCKADDR_6_T);
            received =
                qcom_recvfrom(conn_sock, (&rxBuffer), CFG_PACKET_SIZE_MAX_RX, 0, (struct sockaddr *)&addr6, &addr_len);
        }
        if (received > 0)
        {
            PRINTF("Received %d bytes" NL, received);
            while ((txBuffer = CUSTOM_ALLOC(received)) == NULL)
            {
                // Wait till we get a buffer
                if (test_for_quit())
                {
                    goto ERROR_2;
                }
                /*Allow small delay to allow other thread to run*/
                A_MDELAY(SMALL_TX_DELAY);
            }
            /*Copy received contents in TX buffer*/
            A_MEMCPY(txBuffer, rxBuffer, received);
            /*Free the RX buffer*/
            zero_copy_free(rxBuffer);

            /*Send the received data back to sender*/
            if (!v6EnableFlag)
            {
                if (qcom_sendto(conn_sock, (txBuffer), received, 0, (struct sockaddr *)(&addr), sizeof(addr)) ==
                    A_ERROR)
                {
                    PRINTF("Send failed" NL);
                }
            }
            else
            {
                if (qcom_sendto(conn_sock, (txBuffer), received, 0, (struct sockaddr *)(&addr6), sizeof(addr6)) ==
                    A_ERROR)
                {
                    PRINTF("Send failed" NL);
                }
            }

#if !NON_BLOCKING_TX
            /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
            if (txBuffer)
            {
                CUSTOM_FREE(txBuffer);
            }
#endif
        }
    }
ERROR_2:
    qcom_socket_close(conn_sock);

ERROR_1:

    PRINTF(CFG_COMPLETED_STR);
    PRINT_SHELL_PROMPT
#endif
}

/************************************************************************
 * NAME: test_for_delay
 *
 * DESCRIPTION:  delay for 1% of the time used by this task to give other
 *               tasks an opportunity.
 * Parameters: pointer to current and start time
 ************************************************************************/
int32_t test_for_delay(TIME_STRUCT *pCurr, TIME_STRUCT *pBase)
{
    uint32_t total = (pCurr->SECONDS - pBase->SECONDS) * 1000;
    total += pCurr->MILLISECONDS - pBase->MILLISECONDS;

    if (total > TX_DELAY_INTERVAL)
    {
        return 1;
    }
    return 0;
}

/************************************************************************
 * NAME: check_test_time
 *
 * DESCRIPTION: If test mode is time, check if current time has exceeded
 * test time limit
 * Parameters: pointer to throughput context
 ************************************************************************/
uint32_t check_test_time(THROUGHPUT_CXT *p_tCxt)
{
    uint32_t sec_interval = (p_tCxt->last_time.SECONDS - p_tCxt->first_time.SECONDS);
    uint32_t ms_interval;
    uint32_t total_interval;

    if (sec_interval < p_tCxt->params.tx_params.tx_time)
        return 0;

    ms_interval    = (p_tCxt->last_time.MILLISECONDS - p_tCxt->first_time.MILLISECONDS);
    total_interval = sec_interval * 1000 + ms_interval;

    if (total_interval > p_tCxt->params.tx_params.tx_time * 1000)
        return 1;
    else
        return 0;
}

/************************************************************************
 * NAME: wait_for_response
 *
 * DESCRIPTION: In UDP uplink test, the test is terminated by transmitting
 * end-mark (single byte packet). We have implemented a feedback mechanism
 * where the Peer will reply with receive stats allowing us to display correct
 * test results.
 * Parameters: pointer to throughput context
 ************************************************************************/

int32_t wait_for_response(THROUGHPUT_CXT *p_tCxt, SOCKADDR_T foreign_addr, SOCKADDR_6_T foreign_addr6)
{
    uint32_t received = 0;
    int32_t error     = A_ERROR;
    SOCKADDR_T local_addr;
    SOCKADDR_T addr;
    socklen_t addr_len;
    uint32_t addr_len_raw;
    stat_packet_t *stat_packet;
    SOCKADDR_6_T local_addr6;
    SOCKADDR_6_T addr6;
    char *endmark;
    uint16_t retry_counter = MAX_END_MARK_RETRY;

#if !ZERO_COPY
    if ((stat_packet = A_MALLOC(sizeof(stat_packet_t), MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        return A_ERROR;
    }
#endif

#if ENABLE_RAW_SOCKET_DEMO
    /* The same RAW socket can be used to recv */
    if (p_tCxt->test_type == RAW_TX)
    {
        p_tCxt->sock_local = p_tCxt->sock_peer;
        if (p_tCxt->params.tx_params.ip_hdr_inc == 1)
        {
            addr_len_raw = 0;
            /* Set IP_HDRINCL socket option if need to receive the packet with IP header */
            if (qcom_setsockopt(p_tCxt->sock_peer, ATH_IPPROTO_IP, IP_HDRINCL, (uint8_t *)&addr_len_raw, sizeof(addr_len_raw)) !=
                A_OK)
            {
                PRINTF("SetsockOPT error:IP_HDRINCL" NL);
                goto ERROR_2;
            }
            addr_len = (socklen_t)addr_len_raw;
        }
    }
    else
#endif
    {
        /* For UDP TX create the listen socket */
        if (!v6EnableFlag)
        {
            if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET, SOCK_DGRAM_TYPE, 0)) == (uint32_t)A_ERROR)
            {
                PRINTF("ERROR: Socket creation error" NL);
                goto ERROR_1;
            }

            memset(&local_addr, 0, sizeof(local_addr));
            local_addr.sin_port   = p_tCxt->params.tx_params.port;
            local_addr.sin_family = ATH_AF_INET;

            if (qcom_bind(/*p_tCxt->sock_peer*/ p_tCxt->sock_local, (struct sockaddr *)(&local_addr),
                          sizeof(local_addr)) != A_OK)
            {
                PRINTF("ERROR: Socket bind error" NL);
                goto ERROR_2;
            }
        }
        else
        {
            if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET6, SOCK_DGRAM_TYPE, 0)) == (uint32_t)A_ERROR)
            {
                PRINTF("ERROR: Socket creation error" NL);
                goto ERROR_1;
            }

            memset(&local_addr6, 0, sizeof(local_addr6));
            local_addr6.sin6_port   = p_tCxt->params.tx_params.port;
            local_addr6.sin6_family = ATH_AF_INET6;

            if (qcom_bind(/*p_tCxt->sock_peer*/ p_tCxt->sock_local, (struct sockaddr *)(&local_addr6),
                          sizeof(local_addr6)) != A_OK)
            {
                PRINTF("ERROR: Socket bind error" NL);
                goto ERROR_2;
            }
        }
    }

    while (retry_counter)
    {
        while ((endmark = CUSTOM_ALLOC(sizeof(EOT_PACKET))) == NULL)
        {
            /*Allow small delay to allow other thread to run*/
            A_MDELAY(SMALL_TX_DELAY);
        }
        /* Send End mark.*/
        ((EOT_PACKET *)endmark)->code = HOST_TO_LE_LONG(END_OF_TEST_CODE);
        //((EOT_PACKET*)endmark)->packet_count = HOST_TO_LE_LONG(cur_packet_number);
        if (!v6EnableFlag)
        {
            qcom_sendto(p_tCxt->sock_peer, (endmark), sizeof(EOT_PACKET), 0, (struct sockaddr *)(&foreign_addr),
                        sizeof(foreign_addr));
        }
        else
        {
            qcom_sendto(p_tCxt->sock_peer, (endmark), sizeof(EOT_PACKET), 0, (struct sockaddr *)(&foreign_addr6),
                        sizeof(foreign_addr6));
        }

#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        CUSTOM_FREE(endmark);
#endif

        /* block for xxx msec or until activity on socket */
        assert((void *)handle != NULL);
        if (A_OK == t_select((void *)handle, p_tCxt->sock_local /*p_tCxt->sock_peer*/, 200))
        {
/* Receive data */
#if ZERO_COPY
            if (!v6EnableFlag)
            {
                addr_len = sizeof(SOCKADDR_T);
                received = qcom_recvfrom(p_tCxt->sock_local /*p_tCxt->sock_peer*/, (char **)(&stat_packet),
                                         sizeof(stat_packet_t), 0, (struct sockaddr *)&addr, &addr_len);
            }
            else
            {
                addr_len = sizeof(SOCKADDR_6_T);
                received = qcom_recvfrom(p_tCxt->sock_local /*p_tCxt->sock_peer*/, (char **)(&stat_packet),
                                         sizeof(stat_packet_t), 0, (struct sockaddr *)&addr6, &addr_len);
            }
#else
            if (!v6EnableFlag)
            {
                addr_len = sizeof(SOCKADDR_T);
                received = qcom_recvfrom(p_tCxt->sock_local /*p_tCxt->sock_peer*/, (char *)(stat_packet),
                                         sizeof(stat_packet_t), 0, (struct sockaddr *)&addr, &addr_len);
            }
            else
            {
                addr_len = sizeof(SOCKADDR_6_T);
                received = qcom_recvfrom(p_tCxt->sock_local /*p_tCxt->sock_peer*/, (char *)(stat_packet),
                                         sizeof(stat_packet_t), 0, (struct sockaddr *)&addr6, &addr_len);
            }
#endif
        }
        if (received == sizeof(stat_packet_t))
        {
            PRINTF("received statistics" NL);
            error = A_OK;

            /*Response received from peer, extract test statistics*/
            stat_packet->msec       = HOST_TO_LE_LONG(stat_packet->msec);
            stat_packet->kbytes     = HOST_TO_LE_LONG(stat_packet->kbytes);
            stat_packet->bytes      = HOST_TO_LE_LONG(stat_packet->bytes);
            stat_packet->numPackets = HOST_TO_LE_LONG(stat_packet->numPackets);

            p_tCxt->first_time.SECONDS = p_tCxt->last_time.SECONDS = 0;
            p_tCxt->first_time.MILLISECONDS                        = 0;
            p_tCxt->last_time.MILLISECONDS                         = stat_packet->msec;
            p_tCxt->bytes                                          = stat_packet->bytes;
            p_tCxt->kbytes                                         = stat_packet->kbytes;
            break;
        }
        else
        {
            error = A_ERROR;
            retry_counter--;
            // PRINTF("Did not receive response" NL);
        }
    }

#if ZERO_COPY
    if (received > 0)
        zero_copy_free(stat_packet);
#endif

ERROR_2:
#if !ZERO_COPY
    if (stat_packet)
        A_FREE(stat_packet, MALLOC_ID_CONTEXT);
#endif

#if ENABLE_RAW_SOCKET_DEMO
    if (p_tCxt->test_type == RAW_TX)
    {
        if (p_tCxt->params.tx_params.ip_hdr_inc == 1)
        {
            /* Set IP_HDRINCL socket option if need to receive the packet with IP header */
            if (qcom_setsockopt(p_tCxt->sock_peer, ATH_IPPROTO_IP, IP_HDRINCL,
                                (uint8_t *)(&(p_tCxt->params.tx_params.ip_hdr_inc)), sizeof(int)) != A_OK)
            {
                PRINTF("SetsockOPT error:IP_HDRINCL" NL);
                goto ERROR_2;
            }
        }
        p_tCxt->sock_local = 0;
    }
    else
#endif
        qcom_socket_close(p_tCxt->sock_local);

ERROR_1:
    return error;
}

/************************************************************************
 * NAME: handle_mcast_param
 *
 * DESCRIPTION: Handler for multicast parameters in UDp Rx test
 * Parameters: pointer to throughput context
 ************************************************************************/
int32_t handle_mcast_param(THROUGHPUT_CXT *p_tCxt)
{
    int ip_hdr_inc = 0;
    if (p_tCxt->params.rx_params.ip_hdr_inc == 1)
    {
        PRINTF("set header include option %d " NL, p_tCxt->params.rx_params.ip_hdr_inc);
        ip_hdr_inc = A_CPU2BE32(p_tCxt->params.rx_params.ip_hdr_inc);
        if (qcom_setsockopt(p_tCxt->sock_local, ATH_IPPROTO_IP, IP_HDRINCL, (uint8_t *)(&(ip_hdr_inc)), sizeof(int)) !=
            A_OK)
        {
            PRINTF("SetsockOPT error: unable to set ip hdr inc" NL);
            return A_ERROR;
        }
    }
    if (!v6EnableFlag)
    {
        if (p_tCxt->params.rx_params.group.imr_multiaddr != 0)
        {
            p_tCxt->params.rx_params.group.imr_multiaddr = A_CPU2BE32(p_tCxt->params.rx_params.group.imr_multiaddr);
            p_tCxt->params.rx_params.group.imr_interface = A_CPU2BE32(p_tCxt->params.rx_params.group.imr_interface);
            if (qcom_setsockopt(p_tCxt->sock_local, ATH_IPPROTO_IP, IP_ADD_MEMBERSHIP,
                                (uint8_t *)(&(p_tCxt->params.rx_params.group)), sizeof(IP_MREQ_T)) != A_OK)
            {
                PRINTF("SetsockOPT error: unable to add to multicast group" NL);
                return A_ERROR;
            }
        }
    }
    else
    {
        if (p_tCxt->params.rx_params.mcastEnabled)
        {
            if (qcom_setsockopt(p_tCxt->sock_local, ATH_IPPROTO_IP, IPV6_JOIN_GROUP,
                                (uint8_t *)(&(p_tCxt->params.rx_params.group6)), sizeof(IPV6_MREQ_T)) != A_OK)
            {
                PRINTF("SetsockOPT error: unable to add to multicast group" NL);
                return A_ERROR;
            }
        }
    }
    return A_OK;
}

/************************************************************************
 * NAME: sendAck
 *
 * DESCRIPTION: In UDP receive test, the test is terminated on receiving
 * end-mark (single byte packet). We have implemented a feedback mechanism
 * where the Client will reply with receive stats allowing Peer to display correct
 * test results. The Ack packet will contain time duration and number of bytes
 * received. Implementation details-
 * 1. Peer sends endMark packet, then waits for 500 ms for a response.
 * 2. Client, on receiving endMark, sends ACK (containing RX stats), and waits for
 *    1000 ms to check for more incoming packets.
 * 3. If the Peer receives this ACK, it will stop sending endMark packets.
 * 4. If the client does not see the endMark packet for 1000 ms, it will assume SUCCESS
 *    and exit gracefully.
 * 5. Each side makes 20 attempts.
 * Parameters: pointer to throughput context, specified address, specified port
 ************************************************************************/

void sendAck(THROUGHPUT_CXT *p_tCxt, A_VOID *address, uint16_t port)
{
    int send_result;
    uint32_t received  = 0;
    socklen_t addr_len = 0;
    uint16_t retry     = MAX_ACK_RETRY;
    stat_packet_t *statPacket;
    uint32_t sec_interval   = (p_tCxt->last_time.SECONDS - p_tCxt->first_time.SECONDS);
    uint32_t ms_interval    = (p_tCxt->last_time.MILLISECONDS - p_tCxt->first_time.MILLISECONDS);
    uint32_t total_interval = sec_interval * 1000 + ms_interval;
    SOCKADDR_T *addr        = (SOCKADDR_T *)address;
    SOCKADDR_6_T *addr6     = (SOCKADDR_6_T *)address;

    if (!v6EnableFlag)
    {
        addr_len = sizeof(addr);
    }
    else
    {
        addr_len = sizeof(addr6);
    }

    while (retry)
    {
        while ((statPacket = (stat_packet_t *)CUSTOM_ALLOC(sizeof(stat_packet_t))) == NULL)
        {
            /*Allow small delay to allow other thread to run*/
            A_MDELAY(SMALL_TX_DELAY);
            if (test_for_quit())
            {
                return;
            }
        }

        statPacket->kbytes = HOST_TO_LE_LONG(p_tCxt->kbytes);
        statPacket->bytes  = HOST_TO_LE_LONG(p_tCxt->bytes);
        statPacket->msec   = HOST_TO_LE_LONG(total_interval);

        if (!v6EnableFlag)
        {
            addr->sin_port = port;
            send_result    = qcom_sendto(p_tCxt->sock_local, (char *)(statPacket), sizeof(stat_packet_t), 0,
                                      (struct sockaddr *)addr, addr_len);
        }
        else
        {
            addr6->sin6_port = port;
            send_result      = qcom_sendto(p_tCxt->sock_local, (char *)(statPacket), sizeof(stat_packet_t), 0,
                                      (struct sockaddr *)addr6, addr_len);
        }

#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        CUSTOM_FREE(statPacket);
#endif
        if (send_result == A_ERROR)
        {
            PRINTF("error while sending stat packet" NL);
            break;
        }
        else
        {
            assert((void *)handle != NULL);
            if (t_select((void *)handle, p_tCxt->sock_local, 1000) == A_OK)
            {
#if ZERO_COPY
                if (!v6EnableFlag)
                {
                    received = qcom_recvfrom(p_tCxt->sock_local, (&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                             (struct sockaddr *)addr, &addr_len);
                }
                else
                {
                    received = qcom_recvfrom(p_tCxt->sock_local, (&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                             (struct sockaddr *)addr6, &addr_len);
                }
                zero_copy_free(p_tCxt->buffer);
#else
                if (!v6EnableFlag)
                {
                    received = qcom_recvfrom(p_tCxt->sock_local, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX,
                                             0, addr, &addr_len);
                }
                else
                {
                    received = qcom_recvfrom(p_tCxt->sock_local, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX,
                                             0, (SOCKADDR_T *)addr6, &addr_len);
                }
#endif
                PRINTF("received %d" NL, (int)received);
            }
            else
            {
                PRINTF("ACK success" NL);
                break;
            }
        }
        retry--;
    }
}

#if ENABLE_RAW_SOCKET_DEMO
/************************************************************************
 * NAME: sendRawAck
 *
 * DESCRIPTION: In RAW receive test, the test is terminated on receiving
 * end-mark (single byte packet). We have implemented a feedback mechanism
 * where the Client will reply with receive stats allowing Peer to display correct
 * test results. The Ack packet will contain time duration and number of bytes
 * received. Implementation details-
 * 1. Peer sends endMark packet, then waits for 500 ms for a response.
 * 2. Client, on receiving endMark, sends ACK (containing RX stats), and waits for
 *    1000 ms to check for more incoming packets.
 * 3. If the Peer receives this ACK, it will stop sending endMark packets.
 * 4. If the client does not see the endMark packet for 1000 ms, it will assume SUCCESS
 *    and exit gracefully.
 * 5. Each side makes 20 attempts.
 * Parameters: pointer to throughput context, specified address, specified port
 ************************************************************************/
void sendRawAck(THROUGHPUT_CXT *p_tCxt, A_VOID *local_address, A_VOID *address, int16_t hdrinc, uint16_t protocol)
{
    int send_result, size;
    uint32_t received  = 0;
    socklen_t addr_len = 0;
    uint16_t retry     = MAX_ACK_RETRY;
    char *buffer;
    stat_packet_t *statPacket;
    uint32_t sec_interval   = (p_tCxt->last_time.SECONDS - p_tCxt->first_time.SECONDS);
    uint32_t ms_interval    = (p_tCxt->last_time.MILLISECONDS - p_tCxt->first_time.MILLISECONDS);
    uint32_t total_interval = sec_interval * 1000 + ms_interval;
    SOCKADDR_T *addr        = (SOCKADDR_T *)address;
    SOCKADDR_T *local_addr  = (SOCKADDR_T *)local_address;
    ip_header *iphdr;

    // PRINTF("sendRawAck" NL);

    size = sizeof(stat_packet_t);
    if (hdrinc == 1)
    {
        size += sizeof(ip_header);
    }

    while (retry)
    {
        while ((buffer = CUSTOM_ALLOC(size)) == NULL)
        {
            /*Allow small delay to allow other thread to run*/
            A_MDELAY(SMALL_TX_DELAY);
            if (test_for_quit())
            {
                return;
            }
        }

        if (hdrinc == 1)
        {
            iphdr = (ip_header *)buffer;

            iphdr->iph_ihl = IPV4_HEADER_LENGTH / sizeof(uint32_t);
            iphdr->iph_ver = 4; /* IPv4 */
            iphdr->iph_tos = 0;

            iphdr->iph_len   = htons(size);
            iphdr->iph_ident = htons(0);

            /* Zero (1 bit) */
            ip_flags[0] = 0;
            /* Do not fragment flag (1 bit) */
            ip_flags[1] = 0;
            /* More fragments following flag (1 bit) */
            ip_flags[2] = 0;
            /* Fragmentation offset (13 bits) */
            ip_flags[3] = 0;

            iphdr->iph_offset = htons(((ip_flags[0] << 15) | (ip_flags[1] << 14) | (ip_flags[2] << 13) | ip_flags[3]));

            /* Time-to-Live (8 bits): default to maximum value */
            iphdr->iph_ttl = 255;

            /* Transport layer protocol (8 bits): 17 for UDP */
            iphdr->iph_protocol = protocol;

            iphdr->iph_sourceip = htonl(local_addr->sin_addr.s_addr);
            iphdr->iph_destip   = htonl(addr->sin_addr.s_addr);

            statPacket = (stat_packet_t *)(buffer + sizeof(ip_header));
        }
        else
            statPacket = (stat_packet_t *)buffer;

        statPacket->kbytes = HOST_TO_LE_LONG(p_tCxt->kbytes);
        statPacket->bytes  = HOST_TO_LE_LONG((long)p_tCxt->bytes);
        statPacket->msec   = HOST_TO_LE_LONG(total_interval);

        send_result = qcom_sendto(p_tCxt->sock_local, buffer, size, 0, (struct sockaddr *)addr, sizeof(*addr));

#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        CUSTOM_FREE(buffer);
#endif
        if (send_result == A_ERROR)
        {
            PRINTF("error while sending stat packet" NL);
            break;
        }
        else
        {
            assert((void *)handle != NULL);
            if (t_select((void *)handle, p_tCxt->sock_local, 1000) == A_OK)
            {
#if ZERO_COPY
                received = qcom_recvfrom(p_tCxt->sock_local, (char **)(&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0,
                                         (struct sockaddr *)addr, &addr_len);

                zero_copy_free(p_tCxt->buffer);
#else

                received = qcom_recvfrom(p_tCxt->sock_local, (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0,
                                         (struct sockaddr *)addr, &addr_len);

#endif
                PRINTF("received %d" NL, (int)received);
            }
            else
            {
                PRINTF("ACK success" NL);
                break;
            }
        }
        retry--;
    }
}
#endif

/************************************************************************
 * NAME: ath_tcp_rx
 *
 * DESCRIPTION: Start throughput TCP server.
 ************************************************************************/
void ath_tcp_rx_multi_TCP(int port)
{
    uint8_t ip_str[16];
    socklen_t addr_len        = 0;
    uint16_t open_connections = 0;
    int32_t conn_sock;
    _ip_address temp_addr;
    SOCKADDR_T local_addr;
    SOCKADDR_T foreign_addr;
    int i, call_listen = 1;
    THROUGHPUT_CXT tCxt;
    THROUGHPUT_CXT *p_tCxt     = &tCxt;
    int32_t incoming_socket[3] = {0};

    bench_quit        = 0;
    p_tCxt->sock_peer = (uint32_t)A_ERROR;

#if !ZERO_COPY
    /*Allocate buffer*/
    if ((p_tCxt->buffer = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT)) == NULL)
    {
        PRINTF("Out of memory error" NL);
        goto ERROR_1;
    }
#endif

    /* Create listen socket */
    if ((p_tCxt->sock_local = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
    {
        PRINTF("ERROR: Socket creation error" NL);
        goto ERROR_1;
    }

    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_port   = port;
    local_addr.sin_family = ATH_AF_INET;

    /* Bind socket.*/
    if (qcom_bind(p_tCxt->sock_local, (struct sockaddr *)&local_addr, sizeof(local_addr)) == A_ERROR)
    {
        PRINTF("ERROR: Socket bind error" NL);
        goto ERROR_2;
    }

    /* ------ Start test.----------- */
    print_header_start(" TCP RX Test");
    PRINTF("Local port %d" NL, port);
    print_header_end();

    app_time_get_elapsed(&p_tCxt->first_time);
    app_time_get_elapsed(&p_tCxt->last_time);
    p_tCxt->last_bytes  = 0;
    p_tCxt->last_kbytes = 0;

    PRINTF("Waiting." NL);

    while (1)
    {
        if (test_for_quit())
        {
            goto tcp_rx_QUIT;
        }

        if (call_listen)
        {
            /* Listen. */
            if (qcom_listen(p_tCxt->sock_local, 1) == A_ERROR)
            {
                PRINTF("ERROR: Socket listen error" NL);
                goto ERROR_2;
            }
            call_listen = 0;
        }
        /* block for 100msec or until a connection is received */
        assert((void *)handle != NULL);
        conn_sock = t_select((void *)handle, p_tCxt->sock_local, 100);

        if (conn_sock == A_SOCK_INVALID)
            goto tcp_rx_QUIT; // Peer closed connection, socket no longer valid

        if (conn_sock == A_OK)
        {
            /*Accept incoming connection*/
            addr_len = sizeof(foreign_addr);
            incoming_socket[open_connections] =
                qcom_accept(p_tCxt->sock_local, (struct sockaddr *)&foreign_addr, &addr_len);

            if (incoming_socket[open_connections] != A_ERROR)
            {
                temp_addr = ntohl(foreign_addr.sin_addr.s_addr);
                PRINTF("Receiving from %s:%d" NL, inet_ntoa(*(uint32_t *)(&temp_addr), (char *)ip_str),
                       foreign_addr.sin_port);
                open_connections++;
                call_listen = 1;
            }
        }

        if (open_connections)
        {
            if (receive_incoming(p_tCxt, incoming_socket, &open_connections) != A_OK)
                break;
        }
    }

tcp_rx_QUIT:

    for (i = 0; i < open_connections; i++)
    {
        qcom_socket_close(incoming_socket[i]);
    }
ERROR_2:
    qcom_socket_close(p_tCxt->sock_local);

ERROR_1:

    PRINT_SHELL_PROMPT;

#if !ZERO_COPY
    if (p_tCxt->buffer)
        A_FREE(p_tCxt->buffer, MALLOC_ID_CONTEXT);
#endif
}

int32_t receive_incoming(THROUGHPUT_CXT *p_tCxt, int32_t *incoming_connections, uint16_t *num_connections)
{
    int i           = 0, conn_sock;
    A_STATUS result = A_OK;
    int received    = 0;

    for (i = 0; i < 3; i++) /* Receiving data.*/
    {
        if (test_for_quit())
        {
            result = A_ERROR;
            break;
        }
        if (incoming_connections[i] > 0)
        {
            /*Check for packet*/
            assert((void *)handle != NULL);
            conn_sock = t_select((void *)handle, incoming_connections[i], 100);

            if (conn_sock == A_OK)
            {
/*Packet is available, receive it*/
#if ZERO_COPY
                received = qcom_recv(incoming_connections[i], (char **)(&p_tCxt->buffer), CFG_PACKET_SIZE_MAX_RX, 0);
                if (received > 0)
                    zero_copy_free(p_tCxt->buffer);
#else
                received = qcom_recv(incoming_connections[i], (char *)(&p_tCxt->buffer[0]), CFG_PACKET_SIZE_MAX_RX, 0);
#endif
                if (received == A_SOCK_INVALID)
                {
                    /*socket closed*/
                    PRINTF("Socket %d closed" NL, incoming_connections[i]);
                    qcom_socket_close(incoming_connections[i]);
                    incoming_connections[i] = 0;
                    (*num_connections)--;
                    break;
                }
            }

            if (conn_sock == A_SOCK_INVALID)
            {
                PRINTF("Socket %d closed" NL, incoming_connections[i]);
                qcom_socket_close(incoming_connections[i]);
                incoming_connections[i] = 0;
                (*num_connections)--;
                break;
            }
            else
            {
                /*Valid packaet received*/
                PRINTF("Rx of socket %d" NL, incoming_connections[i]);
            }
        }
    }

    return result;
}

#if ENABLE_SSL

int32_t ssl_get_cert_handler(int32_t argc, char *argv[])
{
    int32_t res = A_ERROR;
    DNC_CFG_CMD dnsCfg;
    DNC_RESP_INFO dnsRespInfo;
    SOCKADDR_T hostAddr;
    uint32_t socketHandle = 0;
    int reqLen;
    CERT_HEADER_T *req;
    CERT_HEADER_T *header;
    uint8_t *buf;
    int certNameLen, numRead = 0, index, port = 1443;
    char *host, *certName, *flashName = NULL;

    // Free certificate buffer if allocated
    if (ssl_cert_data_buf)
    {
        A_FREE(ssl_cert_data_buf, MALLOC_ID_TEMPORARY);
    }
    ssl_cert_data_buf_len = 0;

    // Parse the arguments
    if (argc < 3)
    {
        if (argc > 1)
        {
            PRINTF("Incomplete parameters" NL);
        }
        PRINTF("Usage: %s <name> <host> -p <port -s <fname>" NL, argv[0]);
        PRINTF("  <name>  = Name of the certificate or CA list file to retrieve" NL);
        PRINTF("  <host>  = Host name or IP address of certificate server" NL);
        PRINTF("  <port>  = Optional TCP port number" NL);
        PRINTF("  <fname> = Optional file name used if certificate is stored in FLASH" NL);
        return A_ERROR;
    }
    certName = argv[1];
    host     = argv[2];
    for (index = 3; index < argc; index++)
    {
        if (argv[index][0] == '-')
        {
            switch (argv[index][1])
            {
                case 'p':
                    index++;
                    port = atoi(argv[index]);
                    break;
                case 's':
                    index++;
                    flashName = argv[index];
                    break;
                default:
                    PRINTF("Unknown option: %s" NL, argv[index]);
                    return A_ERROR;
            }
        }
    }

    do
    {
        // resolve the IP address of the certificate server
        uint32_t tmp_addrs_list = 0; // packed issue
        if (0 == ath_inet_aton(host, &tmp_addrs_list))
        {
            dnsRespInfo.ipaddrs_list[0] = tmp_addrs_list;
            if (strlen(host) >= sizeof(dnsCfg.ahostname))
            {
                PRINTF("ERROR: host name too long" NL);
                break;
            }
            strncpy((char *)dnsCfg.ahostname, host, sizeof(dnsCfg.ahostname) - 1);
            dnsCfg.domain = ATH_AF_INET;
            dnsCfg.mode   = RESOLVEHOSTNAME;

            assert((void *)handle != NULL);
            if (A_OK != custom_ip_resolve_hostname(handle, &dnsCfg, &dnsRespInfo))
            {
                PRINTF("ERROR: Unable to resolve server name" NL);
                break;
            }
        }

        // Create socket
        if ((socketHandle = qcom_socket(ATH_AF_INET, SOCK_STREAM_TYPE, 0)) == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Unable to create socket" NL);
            break;
        }

        // Connect to certificate server
        memset(&hostAddr, 0, sizeof(hostAddr));
        hostAddr.sin_addr.s_addr = dnsRespInfo.ipaddrs_list[0];
        hostAddr.sin_port        = port;
        hostAddr.sin_family      = ATH_AF_INET;
        res                      = qcom_connect(socketHandle, (struct sockaddr *)(&hostAddr), sizeof(hostAddr));
        if (res != A_OK)
        {
            PRINTF("ERROR: Connection failed (%d)" NL, res);
            break;
        }

        // Build and send request
        certNameLen = strlen(certName);
        reqLen      = CERT_HEADER_LEN + certNameLen;
        req         = (CERT_HEADER_T *)CUSTOM_ALLOC(reqLen);

        if (req == NULL)
        {
            PRINTF("ERROR: Out of memory" NL);
            break;
        }
        req->id[0]  = 'C';
        req->id[1]  = 'R';
        req->id[2]  = 'T';
        req->id[3]  = '0';
        req->length = A_CPU2BE32(certNameLen);
        memcpy(&req->data[0], certName, certNameLen);

        assert((void *)handle != NULL);
        res = t_send(handle, socketHandle, (uint8_t *)req, reqLen, 0);
#if !NON_BLOCKING_TX
        /*Free the buffer only if NON_BLOCKING_TX is not enabled*/
        CUSTOM_FREE(req);
#endif
        if (res == A_ERROR)
        {
            PRINTF("ERROR: send error = %d" NL, res);
            break;
        }
        else if (res == A_SOCK_INVALID)
        {
            /*Socket has been closed by target due to some error, gracefully exit*/
            PRINTF("ERROR: Socket closed unexpectedly" NL);
            break;
        }
    } while (0);

    // Read the response
    do
    {
        assert((void *)handle != NULL);
        res = t_select((void *)handle, socketHandle, CLIENT_WAIT_TIME);
        if (res == A_OK)
        {
            res = t_recv(handle, socketHandle, (void **)&buf, CFG_PACKET_SIZE_MAX_RX, 0);
            PRINTF("RX: %d" NL, res);
            if (res > 0)
            {
                if (ssl_cert_data_buf_len == 0)
                {
                    if (buf[0] != 'C' || buf[1] != 'R' || buf[2] != 'T')
                    {
                        PRINTF("ERROR: Bad MAGIC received in header" NL);
                        break;
                    }
                    header         = (CERT_HEADER_T *)buf;
                    header->length = A_BE2CPU32(header->length);
                    if (header->length == 0)
                    {
                        zero_copy_free(buf);
                        break;
                    }
                    ssl_cert_data_buf = A_MALLOC(header->length, MALLOC_ID_TEMPORARY);
                    if (ssl_cert_data_buf == NULL)
                    {
                        zero_copy_free(buf);
                        PRINTF("ERROR: Out of memory error" NL);
                        res = A_ERROR;
                        break;
                    }
                    ssl_cert_data_buf_len = header->length;
                    res -= OFFSETOF(CERT_HEADER_T, data);
                    memcpy(ssl_cert_data_buf, header->data, res);
                    numRead = res;
                }
                else
                {
                    if (res + numRead <= ssl_cert_data_buf_len)
                    {
                        memcpy(&ssl_cert_data_buf[numRead], buf, res);
                        numRead += res;
                        res = ssl_cert_data_buf_len;
                    }
                    else
                    {
                        zero_copy_free(buf);
                        PRINTF("ERROR: read failed" NL);
                        res = A_ERROR;
                        break;
                    }
                }
                zero_copy_free(buf);
            }
            else
            {
                PRINTF("ERROR: no response" NL);
                res = A_ERROR;
                break;
            }
        }
        else
        {
            if (res == A_SOCK_INVALID)
            {
                PRINTF("ERROR: no response" NL);
                res = A_ERROR;
                break;
            }
        }
    } while (numRead < ssl_cert_data_buf_len);

    if (socketHandle)
    {
        qcom_socket_close(socketHandle);
    }

    if (res == ssl_cert_data_buf_len)
    {
        PRINTF("Received %d bytes from %s:%d" NL, ssl_cert_data_buf_len, host, port);

        if (flashName != NULL)
        {
            // store certificate in FLASH
            if (A_OK == SSL_storeCert(flashName, ssl_cert_data_buf, ssl_cert_data_buf_len))
            {
                PRINTF("'%s' is stored in FLASH" NL, flashName);
            }
            else
            {
                PRINTF("ERROR: failed to store in %s" NL, flashName);
                res = A_ERROR;
            }
        }
    }
    return res;
}

#endif // ENABLE_SSL

#endif // ENABLE_STACK_OFFLOAD
