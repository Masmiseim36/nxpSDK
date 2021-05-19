/**************************************************************************/
/*                                                                        */
/*       Copyright (c) Microsoft Corporation. All rights reserved.        */
/*                                                                        */
/*       This software is licensed under the Microsoft Software License   */
/*       Terms for Microsoft Azure RTOS. Full text of the license can be  */
/*       found in the LICENSE file at https://aka.ms/AzureRTOS_EULA       */
/*       and in the root directory of this software.                      */
/*                                                                        */
/**************************************************************************/


/**************************************************************************/
/**************************************************************************/
/**                                                                       */
/** NetX Utility                                                          */
/**                                                                       */
/**   NetX/NetX Duo IPerf Test Program                                    */
/**                                                                       */
/**************************************************************************/
/**************************************************************************/

#include   "tx_api.h"
#include   "nx_api.h"
#include   "nx_iperf.h"
#ifndef NX_HTTP_NO_FILEX
#include   "fx_api.h"
#else
#include   "filex_stub.h"
#endif

#ifdef __PRODUCT_NETXDUO__
#include   "nxd_http_server.h"
#else
#include   "nx_http.h"
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
#include   "tx_execution_profile.h"
#endif /* TX_ENABLE_EXECUTION_CHANGE_NOTIFY */

NX_HTTP_SERVER      my_server;
FX_MEDIA            ram_disk;
NX_IP               *_iperf_test_ip;
NX_PACKET_POOL      *_iperf_test_pool;
UCHAR               *_iperf_stack_area;
ULONG               _iperf_stack_area_size;
ULONG               _iperf_test_error_counter;
UINT   _nx_http_server_number_convert(UINT, CHAR* );
UINT   END_TICK = 100;
static ULONG  error_counter;

/* Define the counters used in the demo application...  */

NX_IP                   *_iperf_test_ip;
ULONG                   _iperf_test_error_counter;
ctrl_info               iperf_ctrl_info;
ctrl_info               *iperf_ctrlInfo_ptr;

#ifdef __PRODUCT_NETXDUO__
static NXD_ADDRESS       udp_tx_ip_address;
static NXD_ADDRESS       tcp_tx_ip_address;
#else
static ULONG       udp_tx_ip_address;
static ULONG       tcp_tx_ip_address;
#endif

static ULONG udp_tx_port = CLIENT_DEFAULT_PORT;
static ULONG tcp_tx_port = CLIENT_DEFAULT_PORT;

static UINT udp_tx_packet_size = 1470;
static UINT udp_tx_test_time = 10;
static UINT udp_rx_test_time = 10;
static UINT tcp_tx_test_time = 10;
static UINT tcp_rx_test_time = 10;

#ifdef __PRODUCT_NETXDUO__
static NXD_ADDRESS      iperf_ip;
#else
static ULONG            iperf_ip;
#endif

static void print_tcp_rx_results(NX_HTTP_SERVER *server_ptr);
static void print_tcp_tx_results(NX_HTTP_SERVER *server_ptr);
static void print_udp_rx_results(NX_HTTP_SERVER *server_ptr);
static void print_udp_tx_results(NX_HTTP_SERVER *server_ptr);
static void print_ping_results(NX_HTTP_SERVER *server_ptr);

void    tcp_rx_test(UCHAR *, ULONG);
void    tcp_tx_test(UCHAR *, ULONG);
void    udp_rx_test(UCHAR *, ULONG);
void    udp_tx_test(UCHAR *, ULONG);

void thread_tcp_rx_cleanup(void);
void thread_tcp_tx_cleanup(void);
void thread_udp_rx_cleanup(void);
void thread_udp_tx_cleanup(void);

void ping_cleanup(void);

void    nx_test_info_parse(NX_HTTP_SERVER *server_ptr);
void    ping_test(NX_HTTP_SERVER *server_ptr);

NX_TCP_SOCKET             tcp_server_socket;
NX_TCP_SOCKET             tcp_client_socket;
NX_UDP_SOCKET             udp_server_socket;
NX_UDP_SOCKET             udp_client_socket;
ULONG                     thread_tcp_rx_counter;
ULONG                     thread_tcp_tx_counter;
ULONG                     thread_udp_rx_counter;
ULONG                     thread_udp_tx_counter;
static TX_THREAD  thread_tcp_rx_iperf;
static TX_THREAD  thread_tcp_tx_iperf;
static TX_THREAD  thread_udp_rx_iperf;
static TX_THREAD  thread_udp_tx_iperf;
void    thread_tcp_rx_connect_received(NX_TCP_SOCKET *socket_ptr, UINT port);
void    thread_tcp_rx_disconnect_received(NX_TCP_SOCKET *socket_ptr);

extern ULONG _tx_timer_system_clock;
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
EXECUTION_TIME thread_time = 0;
EXECUTION_TIME isr_time = 0;
EXECUTION_TIME idle_time = 0;
extern TX_THREAD *_tx_thread_created_ptr;
#endif

UINT    my_authentication_check(struct NX_HTTP_SERVER_STRUCT *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm);
UINT    my_get_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr);
void    nx_iperf_entry(NX_PACKET_POOL *pool_ptr, NX_IP *ip_ptr, UCHAR* http_stack, ULONG http_stack_size, UCHAR *iperf_stack, ULONG iperf_stack_size) ;

#ifdef __PRODUCT_NETXDUO__
char *get_ip_addr_string(NXD_ADDRESS ip);
#else
char *get_ip_addr_string(ULONG ip);
#endif
void    thread_tcp_rx_entry(ULONG thread_input);
void    thread_tcp_tx_entry(ULONG thread_input);
void    thread_udp_tx_entry(ULONG thread_input);
void    thread_udp_rx_entry(ULONG thread_input);

void    nx_iperf_entry(NX_PACKET_POOL *pool_ptr, NX_IP *ip_ptr, UCHAR* http_stack, ULONG http_stack_size, UCHAR *iperf_stack, ULONG iperf_stack_size)
{
UINT    status;

    /* Create the HTTP Server.  */
    status =  nx_http_server_create(&my_server, "My HTTP Server", ip_ptr, &ram_disk, http_stack, http_stack_size, pool_ptr, my_authentication_check, my_get_notify);

    /* Check the status.  */
    if (status)
    {

        /* Update the error counter and return.  */
        _iperf_test_error_counter++;
        return;
    }

    /* Set the Iperf Stack and Size.  */
    _iperf_stack_area = iperf_stack;
    _iperf_stack_area_size = iperf_stack_size;

    /* Set the IP instance and Packet Pool.  */
    _iperf_test_ip = ip_ptr;
    _iperf_test_pool = pool_ptr;

    /* Start the HTTP Server.  */
    status =  nx_http_server_start(&my_server);

    /* Check the status.  */
    if (status)
    {

        /* Update the error counter and return.  */
        _iperf_test_error_counter++;
        return;
    }
}

UINT    my_authentication_check(struct NX_HTTP_SERVER_STRUCT *server_ptr, UINT request_type, CHAR *resource, CHAR **name, CHAR **password, CHAR **realm)
{


#ifdef NX_IPERF_AUTH_ENABLE

    NX_PARAMETER_NOT_USED(server_ptr);
    NX_PARAMETER_NOT_USED(request_type);
    NX_PARAMETER_NOT_USED(resource);
    *name =  "name5";
    *password = "password6";
    *realm =  "test.htm";

    return(NX_HTTP_BASIC_AUTHENTICATE);
#else
    NX_PARAMETER_NOT_USED(server_ptr);
    NX_PARAMETER_NOT_USED(request_type);
    NX_PARAMETER_NOT_USED(resource);
    NX_PARAMETER_NOT_USED(name);
    NX_PARAMETER_NOT_USED(password);
    NX_PARAMETER_NOT_USED(realm);

    return(NX_SUCCESS);
#endif
}

static CHAR    string[100];

static CHAR    device_ip_addr_string[40];
#ifdef __PRODUCT_NETXDUO__
char *get_ip_addr_string(NXD_ADDRESS ip)
{
    memset(device_ip_addr_string, 0, sizeof(device_ip_addr_string));

#ifdef FEATURE_NX_IPV6
    if(ip.nxd_ip_version == NX_IP_VERSION_V6)
    {

        sprintf(device_ip_addr_string, "%0x:%0x", (UINT)ip.nxd_ip_address.v6[0] >> 16, (UINT)ip.nxd_ip_address.v6[0] & 0xFFFF);
        device_ip_addr_string[strlen(device_ip_addr_string)] = ':';
        sprintf(&device_ip_addr_string[strlen(device_ip_addr_string)], "%0x:%0x", (UINT)ip.nxd_ip_address.v6[1] >> 16, (UINT)ip.nxd_ip_address.v6[1] & 0xFFFF);
        device_ip_addr_string[strlen(device_ip_addr_string)] = ':';
        sprintf(&device_ip_addr_string[strlen(device_ip_addr_string)], "%0x:%0x", (UINT)ip.nxd_ip_address.v6[2] >> 16, (UINT)ip.nxd_ip_address.v6[2] & 0xFFFF);
        device_ip_addr_string[strlen(device_ip_addr_string)] = ':';
        sprintf(&device_ip_addr_string[strlen(device_ip_addr_string)], "%0x:%0x", (UINT)ip.nxd_ip_address.v6[3] >> 16, (UINT)ip.nxd_ip_address.v6[3] & 0xFFFF);
    }
    else
#endif
    {

        _nx_http_server_number_convert(ip.nxd_ip_address.v4 >> 24, device_ip_addr_string);
        device_ip_addr_string[strlen(device_ip_addr_string)] = '.';
        _nx_http_server_number_convert(((ip.nxd_ip_address.v4 >> 16) & 0xFF), &device_ip_addr_string[strlen(device_ip_addr_string)]);
        device_ip_addr_string[strlen(device_ip_addr_string)] = '.';
        _nx_http_server_number_convert(((ip.nxd_ip_address.v4 >> 8) & 0xFF), &device_ip_addr_string[strlen(device_ip_addr_string)]);
        device_ip_addr_string[strlen(device_ip_addr_string)] = '.';
        _nx_http_server_number_convert((ip.nxd_ip_address.v4 & 0xFF), &device_ip_addr_string[strlen(device_ip_addr_string)]);

    }
    return device_ip_addr_string;
}
#else
char *get_ip_addr_string(ULONG ip)
{
    memset(device_ip_addr_string, 0, sizeof(device_ip_addr_string));
    _nx_http_server_number_convert(ip >> 24, device_ip_addr_string);
    device_ip_addr_string[strlen(device_ip_addr_string)] = '.';
    _nx_http_server_number_convert(((ip >> 16) & 0xFF), &device_ip_addr_string[strlen(device_ip_addr_string)]);
    device_ip_addr_string[strlen(device_ip_addr_string)] = '.';
    _nx_http_server_number_convert(((ip >> 8) & 0xFF), &device_ip_addr_string[strlen(device_ip_addr_string)]);
    device_ip_addr_string[strlen(device_ip_addr_string)] = '.';
    _nx_http_server_number_convert((ip & 0xFF), &device_ip_addr_string[strlen(device_ip_addr_string)]);

    return device_ip_addr_string;
}
#endif

/* This function takes the token/value pair, and stores the information in the ctrl_info_ptr. */
/* For example, a token/value pair can be: "TestType"="TC_Rx", and the ctrl_info_ptr stores the information. */
static void check_token_value(char* token, char *value_ptr, ctrl_info *ctrl_info_ptr)
{
UINT val;
#ifdef __PRODUCT_NETXDUO__
UINT i;
#endif /* __PRODUCT_NETXDUO__ */

    /* Check for name. */
    if(strncmp(token, "TestType", strlen("TestType")) == 0)
    {
        /* Check for value. */
        ctrl_info_ptr -> ctrl_sign = UNKNOWN_TEST;
        if(value_ptr)
        {
            if(strncmp(value_ptr, TCP_Rx, strlen(TCP_Rx)) == 0)
                ctrl_info_ptr -> ctrl_sign = TCP_RX_START;
            else if(strncmp(value_ptr, TCP_Tx, strlen(TCP_Tx)) == 0)
                ctrl_info_ptr -> ctrl_sign = TCP_TX_START;
            else if(strncmp(value_ptr, UDP_Rx, strlen(UDP_Rx)) == 0)
                ctrl_info_ptr -> ctrl_sign = UDP_RX_START;
            else if(strncmp(value_ptr, UDP_Tx, strlen(UDP_Tx)) == 0)
                ctrl_info_ptr -> ctrl_sign = UDP_TX_START;
            else if(strncmp(value_ptr, Ping_Test, strlen(Ping_Test)) == 0)
                ctrl_info_ptr -> ctrl_sign = PING_TEST;
        }
    }
    else if(strncmp(token, "ip", strlen("ip")) == 0)
    {
        char *ptr = value_ptr;
#ifdef __PRODUCT_NETXDUO__
        int colon_sum, colon_count;
#endif
        ctrl_info_ptr -> ip = 0;
        val = 0;
#ifdef __PRODUCT_NETXDUO__
        colon_sum = 0;
        colon_count = 0;
#endif
        if(value_ptr == 0)
        {
#ifdef __PRODUCT_NETXDUO__
            ctrl_info_ptr -> version = NX_IP_VERSION_V4;
#endif
            ctrl_info_ptr -> ip = DEFAULT_IPERF_IP;
        }
        while(ptr && (*ptr != 0))
        {
            if(*ptr == '.')
            {
#ifdef __PRODUCT_NETXDUO__
                ctrl_info_ptr -> version = NX_IP_VERSION_V4;
#endif
                while(value_ptr && (*value_ptr != 0))
                {
                    if(*value_ptr == '.')
                    {
                        ctrl_info_ptr -> ip = (ctrl_info_ptr -> ip << 8) + val;
                        val = 0;
                    }
                    else
                        val = val * 10 + ((UINT)(*value_ptr - '0'));
                    value_ptr++;
                }
                ctrl_info_ptr -> ip = (ctrl_info_ptr -> ip << 8) + val;
                break;
            }
#ifdef __PRODUCT_NETXDUO__
            else if(*ptr == '%')
            {
                if((*(++ptr) == '3') && (*(++ptr) == 'A'))
                {
                    ctrl_info_ptr -> version = NX_IP_VERSION_V6;
                    colon_sum++;
                }
            }
#endif
            ptr++;
        }
#ifdef __PRODUCT_NETXDUO__
        while(value_ptr && (*value_ptr != 0) && (colon_sum != 0))
        {
            if(*value_ptr == '%')
            {
                if((*(++value_ptr) == '3') && (*(++value_ptr) == 'A'))
                {
                    ctrl_info_ptr -> ipv6[colon_count/2] = (ctrl_info_ptr -> ipv6[colon_count/2] << 16) + val;
                    colon_count++;

                    if(*(value_ptr + 1) == '%')
                    {
                        value_ptr++;
                        if((*(++value_ptr) == '3') && (*(++value_ptr) == 'A'))
                            for(i = 0; i <= (UINT)(7 - colon_sum); i++)
                            {
                                ctrl_info_ptr -> ipv6[colon_count/2] = ctrl_info_ptr -> ipv6[colon_count/2] << 16;
                                colon_count++;
                            }
                    }
                    val = 0;
                }
            }
            else
            {
                if(*value_ptr >= '0' && *value_ptr <='9')
                    val = val * 16 + ((UINT)(*value_ptr - '0'));
                else if(*value_ptr >= 'a' && *value_ptr <='f')
                    val = val * 16 + ((UINT)(*value_ptr - 'a')) + 10;
                else if(*value_ptr >= 'A' && *value_ptr <='F')
                    val = val * 16 + ((UINT)(*value_ptr - 'A')) + 10;
            }
            value_ptr++;
        }
        if (ctrl_info_ptr -> version == NX_IP_VERSION_V6)
            ctrl_info_ptr -> ipv6[3] = (ctrl_info_ptr -> ipv6[3] << 16) + val;
#endif
    }
    else if(strncmp(token, "test_time", strlen("test_time")) == 0)
    {
        ctrl_info_ptr -> TestTime = 0;
        while(value_ptr && (*value_ptr != 0))
        {
            ctrl_info_ptr -> TestTime = ctrl_info_ptr -> TestTime * 10 + ((UINT)(*value_ptr - '0'));
            value_ptr++;
        }
        ctrl_info_ptr -> TestTime = ctrl_info_ptr -> TestTime * NX_IP_PERIODIC_RATE;
    }

    else if(strncmp(token, "ping_rate", strlen("ping_rate")) == 0)
    {
      ctrl_info_ptr -> PingRate = 0;
      while(value_ptr && (*value_ptr != 0))
      {
          ctrl_info_ptr -> PingRate = ctrl_info_ptr -> PingRate * 10 + ((UINT)(*value_ptr - '0'));
          value_ptr++;
      }
    }
    else if(strncmp(token, "total", strlen("total")) == 0)
    {
      ctrl_info_ptr -> TotalPings = 0;
      while(value_ptr && (*value_ptr != 0))
      {
          ctrl_info_ptr -> TotalPings = ctrl_info_ptr -> TotalPings * 10 + ((UINT)(*value_ptr - '0'));
          value_ptr++;
      }
    }
    else if(strncmp(token, "rate", strlen("rate")) == 0)
    {
      ctrl_info_ptr -> Rate = 0;
      while(value_ptr && (*value_ptr != 0))
      {
          ctrl_info_ptr -> Rate = ctrl_info_ptr -> Rate * 10 + ((UINT)(*value_ptr - '0'));
          value_ptr++;
      }
    }
    else if(strncmp(token, "size", strlen("size")) == 0)
    {
      ctrl_info_ptr -> PacketSize = 0;
      while(value_ptr && (*value_ptr != 0))
      {
          ctrl_info_ptr -> PacketSize = ctrl_info_ptr -> PacketSize * 10 + ((UINT)(*value_ptr - '0'));
          value_ptr++;
      }
    }
    else if(strncmp(token, "port", strlen("port")) == 0)
    {
      ctrl_info_ptr -> port = 0;
      while(value_ptr && (*value_ptr != 0))
      {
          ctrl_info_ptr -> port = ctrl_info_ptr -> port * 10 + ((UINT)(*value_ptr - '0'));
          value_ptr++;
      }
    }
}

/* This function parses the incoming HTTP command line.  For each token/value pair, it
   invokes the check_token_value routine to parse the values. */
static void parse_command(NX_PACKET *packet_ptr, ctrl_info* ctrl_info_ptr)
{
    /* Clear ctrl_info block. */
    UCHAR *cmd_string = packet_ptr -> nx_packet_prepend_ptr;

    UCHAR *token = NX_NULL;
    UCHAR *end_cmd;
    UCHAR *next_token;
    UCHAR *value_ptr;
    /* At this point, cmd_string points to the beginning of the HTTP request,
       which takes the form of:
       "GET /test.htm?TestType=xxxxx&ip=....&rxed_pkts=xxxx&test_time=xxxx&tputs=xxxx" */

    /* First skip the "Get /test.html?" string. */
    cmd_string += (strlen("GET /test.htm?"));

    /* Find the end of the cmd string, */
    end_cmd = cmd_string;
    while(end_cmd < packet_ptr -> nx_packet_append_ptr)
    {
        if(*end_cmd == ' ')
            break;
        end_cmd++;
    }
    *end_cmd = 0;

    /* The first token starts from cmd_string. */
    token = cmd_string;
    next_token = cmd_string;
    while(next_token < end_cmd)
    {
        /* Find the next token .*/
        while(next_token < end_cmd)
        {
            if(*next_token == '=')
                break;
            next_token++;
        }

        if(*next_token == '=')
        {
            /* Find a name=value pair. Now we need to find the "=" sign. */
            *next_token = 0;
            value_ptr = next_token + 1;
            next_token++;

            while(next_token < end_cmd)
            {
                if(*next_token == '&')
                {
                    *next_token = 0;

                    break;
                }
                next_token++;
            }

            if(value_ptr == next_token)
            {
                /* There is no value string.  */
                value_ptr = NX_NULL;
            }

            /* Move next_token beyond the NULL terminator. */
            next_token++;


            check_token_value((char*)token, (char*)value_ptr, ctrl_info_ptr);
        }
        token = next_token;
    }

    /* Finished parsing the whole command. */
}

static CHAR          mytempstring[30];
static VOID print_main_test_window(NX_HTTP_SERVER *server_ptr)
{
    NX_PACKET *resp_packet_ptr;
    UINT status;
#ifdef __PRODUCT_NETXDUO__
    NXD_ADDRESS server_ip;
    UINT        address_index;
    ULONG       prefix_length;
    UINT        interface_index;
#endif
    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);

    /* write HTML code into the packet */
    /* htmlwrite(p,s)  (nx_packet_data_append(p,s,strlen(s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER)) */

    status += htmlwrite(resp_packet_ptr, outtermosttable);
    status += htmlwrite(resp_packet_ptr, maintabletag);

    /* print the IP address line. */
    status += htmlwrite(resp_packet_ptr, h1line1);

#ifdef __PRODUCT_NETXDUO__
    server_ip.nxd_ip_version = NX_IP_VERSION_V4;
    server_ip.nxd_ip_address.v4 = _iperf_test_ip -> nx_ip_interface[0].nx_interface_ip_address;
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(server_ip));
    status += htmlwrite(resp_packet_ptr, "\n");

#ifdef FEATURE_NX_IPV6
    address_index = 0;

    /* Loop to output the IPv6 address.  */
    while(1)
    {

        /* Get the IPv6 address.  */
        if (nxd_ipv6_address_get(_iperf_test_ip, address_index, &server_ip, &prefix_length, &interface_index) == NX_SUCCESS)
        {
            status += htmlwrite(resp_packet_ptr, get_ip_addr_string(server_ip));
            status += htmlwrite(resp_packet_ptr, "\n");
            address_index ++;
        }
        else
            break;
    }
#endif
#else
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(_iperf_test_ip -> nx_ip_interface[0].nx_interface_ip_address));
#endif
    status += htmlwrite(resp_packet_ptr, h1line2);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);
    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, udptxsubmittag1);
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(udp_tx_ip_address));
    status += htmlwrite(resp_packet_ptr, udptxsubmittag2);
    _nx_http_server_number_convert(udp_tx_port, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, udptxsubmittag3);
    _nx_http_server_number_convert(udp_tx_test_time, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, udptxsubmittag4);
    _nx_http_server_number_convert(udp_tx_packet_size, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, udptxsubmittag5);
    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, udprxsubmittag1);
    _nx_http_server_number_convert(udp_rx_test_time, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, udprxsubmittag2);
    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, tcptxsubmittag1);
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(tcp_tx_ip_address));
    status += htmlwrite(resp_packet_ptr, tcptxsubmittag2);
    _nx_http_server_number_convert(tcp_tx_port, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, tcptxsubmittag3);
    _nx_http_server_number_convert(tcp_tx_test_time, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, tcptxsubmittag4);
    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, tcprxsubmittag1);
     _nx_http_server_number_convert(tcp_rx_test_time, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, tcprxsubmittag2);
    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, tableendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }
}

static void print_end_of_page(NX_HTTP_SERVER *server_ptr)
{
    UINT status;
    NX_PACKET *resp_packet_ptr;
    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    /* End of the page. */
    status += htmlwrite(resp_packet_ptr, tableendtag);/* outtermost table. */
    status += htmlwrite(resp_packet_ptr, doublebr);
    status += htmlwrite(resp_packet_ptr, centerendtag);
    status += htmlwrite(resp_packet_ptr, bodyendtag);
    status += htmlwrite(resp_packet_ptr, htmlendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }
}

static void print_header(NX_HTTP_SERVER *server_ptr)
{
    NX_PACKET *resp_packet_ptr;
    UINT status;

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                &resp_packet_ptr,
                                NX_TCP_PACKET,
                                NX_WAIT_FOREVER);


    /* write HTML code into the packet */
    /* htmlwrite(p,s)  (nx_packet_data_append(p,s,strlen(s), server_ptr-> nx_http_server_packet_pool_ptr,NX_WAIT_FOREVER)) */

    status += htmlwrite(resp_packet_ptr, htmlresponse);
    status += htmlwrite(resp_packet_ptr, htmltag);
    status += htmlwrite(resp_packet_ptr, titleline);
    status += htmlwrite(resp_packet_ptr, bodytag);
    status += htmlwrite(resp_packet_ptr, logo_area);
    status += htmlwrite(resp_packet_ptr, hrline);
    status += htmlwrite(resp_packet_ptr, centertag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }
}


static void print_test_inprogress(NX_HTTP_SERVER *server_ptr, char *msg)
{
    NX_PACKET *resp_packet_ptr;
    UINT status;

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                       &resp_packet_ptr,
                       NX_TCP_PACKET,
                       NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, tdcentertag);
    status += htmlwrite(resp_packet_ptr, fontcolortag);
    status += htmlwrite(resp_packet_ptr, msg);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);

    status += nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }
}

static void print_empty_line(NX_HTTP_SERVER *server_ptr)
{
    NX_PACKET *resp_packet_ptr;
    UINT status = 0;
    status += nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr, &resp_packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);
    status += htmlwrite(resp_packet_ptr, choosetesttag);
    status += htmlwrite(resp_packet_ptr, trendtag);
    status += nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }
}

static void send_test_result_info(NX_HTTP_SERVER *server_ptr)
{
    char *response_string = NX_NULL;

    if(iperf_ctrl_info.TestStatus == 0)
    {
        print_empty_line(server_ptr);
        /* No test is running.  Do nothing. */
        return;
    }
    else if(iperf_ctrl_info.TestStatus == 1)
    {
        switch(iperf_ctrl_info.ctrl_sign)
        {
        case UDP_RX_START:
            response_string = "UDP Receive Test started.  After the iperf test finishes, click <a href=\"/\">here</a> to get results.";
            break;
        case TCP_RX_START:
            response_string = "TCP Receive Test started.  After the iperf test finishes, click <a href=\"/\">here</a> to get results.";
            break;
        case UDP_TX_START:
            response_string = "UDP Transmit Test starts in 2 seconds.  After iperf test is done, click <a href=\"/\">here</a> to get results.";
            break;
        case TCP_TX_START:
            response_string = "TCP Transmit test starts in 2 seconds.  After iperf test is done, click <a href=\"/\">here</a> to get results.";
            break;
        }
        print_test_inprogress(server_ptr, response_string);
    }
    else if(iperf_ctrl_info.TestStatus == 2)
    {

#ifdef IPERF_TEST
        /* Check the ThroughPut value and StartTime,
           if the StartTime is zero means no connection.
           if throughput value is zero means maybe user interrupt the test.
           recalcuate the ThroughPut before Interrupt.  */
        if ((!iperf_ctrl_info.ThroughPut) && (iperf_ctrl_info.StartTime))
        {
            /* Calculate the run time and Throughput(Mbps).  */
            iperf_ctrl_info.RunTime = tx_time_get() - iperf_ctrl_info.StartTime;

            /* Check the run time.  */
            if(iperf_ctrl_info.RunTime > iperf_ctrl_info.TestTime)
                iperf_ctrl_info.RunTime = iperf_ctrl_info.TestTime;

            /* Calculate Throughput(Mbps).  */
            iperf_ctrl_info.ThroughPut = (iperf_ctrl_info.BytesTxed + iperf_ctrl_info.BytesRxed) / iperf_ctrl_info.RunTime * NX_IP_PERIODIC_RATE / 125000;
        }
#endif

#ifdef __PRODUCT_NETXDUO__
        iperf_ip.nxd_ip_version = iperf_ctrl_info.version;

#ifdef FEATURE_NX_IPV6
        if (iperf_ip.nxd_ip_version == NX_IP_VERSION_V6)
        {
            iperf_ip.nxd_ip_address.v6[0] = iperf_ctrl_info.ipv6[0];
            iperf_ip.nxd_ip_address.v6[1] = iperf_ctrl_info.ipv6[1];
            iperf_ip.nxd_ip_address.v6[2] = iperf_ctrl_info.ipv6[2];
            iperf_ip.nxd_ip_address.v6[3] = iperf_ctrl_info.ipv6[3];
        }
        else
#endif
        {
            iperf_ip.nxd_ip_address.v4 = iperf_ctrl_info.ip;
        }
#else
        iperf_ip = iperf_ctrl_info.ip;
#endif
        switch(iperf_ctrl_info.ctrl_sign)
        {
        case UDP_RX_START:
            print_udp_rx_results(server_ptr);
            break;
        case TCP_RX_START:
            print_tcp_rx_results(server_ptr);
            break;
        case UDP_TX_START:
            print_udp_tx_results(server_ptr);
            break;
        case TCP_TX_START:
            print_tcp_tx_results(server_ptr);
            break;
        case PING_TEST:
            print_ping_results(server_ptr);
            break;
        }
        memset(&iperf_ctrl_info, 0, sizeof(iperf_ctrl_info));
    }
}

static void send_image(NX_HTTP_SERVER *server_ptr, UCHAR* img, UINT imgsize);

UINT    my_get_notify(NX_HTTP_SERVER *server_ptr, UINT request_type, CHAR *resource, NX_PACKET *packet_ptr)
{
    ctrl_info   new_cmd;

    NX_PARAMETER_NOT_USED(request_type);

    memset(&new_cmd, 0, sizeof(ctrl_info));

    udp_tx_ip_address = server_ptr -> nx_http_server_socket.nx_tcp_socket_connect_ip;
    tcp_tx_ip_address = server_ptr -> nx_http_server_socket.nx_tcp_socket_connect_ip;

    if((strcmp(resource,"/test.htm") == 0) ||
       ((strlen(resource) == 1) && *resource == '/'))
    {

        nx_http_server_query_get(packet_ptr, 0, string, sizeof(string));

        /* obtain a packet for our html code to be sent to the client */
        iperf_ctrlInfo_ptr = &iperf_ctrl_info;

        /* Printer the header.  */
        print_header(server_ptr);

        /* Parse the command.  */
        parse_command(packet_ptr, &new_cmd);

        /* If the current test is still running, and we have a new command,
           we need to clean up the current one. */
        if(new_cmd.ctrl_sign)
        {
            if((iperf_ctrl_info.TestStatus != 0) || ((iperf_ctrl_info.ctrl_sign & CLEAN_UP_MASK) == 1))
            {

                switch(iperf_ctrl_info.ctrl_sign)
                {
                case TCP_RX_START:
                    thread_tcp_rx_cleanup();
                    break;
                case TCP_TX_START:
                    thread_tcp_tx_cleanup();
                    break;
                case UDP_RX_START:
                    thread_udp_rx_cleanup();
                    break;
                case UDP_TX_START:
                    thread_udp_tx_cleanup();
                    break;
                case PING_TEST:
                    ping_cleanup();
                    break;
                default:
                    break;
                }
                memset(&iperf_ctrl_info, 0, sizeof(iperf_ctrl_info));
            }

            memcpy(&iperf_ctrl_info, &new_cmd, sizeof(ctrl_info));

            /* Create the test thread and run the test.  */
            nx_test_info_parse(server_ptr);

            /* Update the TestStatus.  */
            iperf_ctrl_info.TestStatus = 1;
        }

#ifdef __PRODUCT_NETXDUO__
        /* Check the status, set the default value.  */
        if (iperf_ctrl_info.TestStatus == 0)
        {

            /* Check the IP version.  */
            if (udp_tx_ip_address.nxd_ip_version == NX_IP_VERSION_V4)
                udp_tx_packet_size = 1470;
            else
                udp_tx_packet_size = 1450;
        }
#endif

        /* Print the main window.  */
        print_main_test_window(server_ptr);

        /* If there is a new command, show the result of launching the command. */
        send_test_result_info(server_ptr);

        print_end_of_page(server_ptr);

        /* Update the TestStatus.  */
        if (iperf_ctrl_info.TestStatus == 1)
            iperf_ctrl_info.TestStatus = 2;

        return(NX_HTTP_CALLBACK_COMPLETED);
    }
    /* send the logo files */
    if(strcmp(resource,"/nxlogo.png")==0)
    {
        send_image(server_ptr, (UCHAR*)nxlogo_png, nxlogo_png_size);
        return(NX_HTTP_CALLBACK_COMPLETED);
    }

    if(strcmp(resource,"/ellogo.jpg")==0)
    {
        send_image(server_ptr, (UCHAR*)ellogo_jpg, ellogo_jpg_size);
        /* nx_http_server_callback_data_send(server_ptr, (void *)ellogo_jpg, ellogo_jpg_size);*/
        return(NX_HTTP_CALLBACK_COMPLETED);
    }

   return(NX_SUCCESS);
}

static void send_image(NX_HTTP_SERVER *server_ptr, UCHAR* img, UINT imgsize)
{

    UINT remaining;
    UCHAR *position;
    UINT max_size;

#ifdef NX_IPSEC_ENABLE
    max_size = 1380;
#else
    max_size = 1460;
#endif

    remaining = imgsize;
    position = img;
    while(remaining)
    {
        if(remaining > max_size)
        {
            nx_http_server_callback_data_send(server_ptr, (void *)position, max_size);
            position += max_size;
            remaining -= max_size;
        }
        else
        {
            nx_http_server_callback_data_send(server_ptr, (void *)position, remaining);
            remaining = 0;
        }
    }
}


void  nx_test_info_parse(NX_HTTP_SERVER *server_ptr)
{

    /* Check the sign and set the related parameters.  */
    switch ((iperf_ctrlInfo_ptr-> ctrl_sign) & CTRL_SIGN_MASK)
    {

        case TCP_RX_START:
        {
            if(iperf_ctrlInfo_ptr -> TestTime == 0)
                iperf_ctrlInfo_ptr -> TestTime = 10 * NX_IP_PERIODIC_RATE;
            tcp_rx_test_time = (iperf_ctrlInfo_ptr -> TestTime) / NX_IP_PERIODIC_RATE;

            tcp_rx_test(_iperf_stack_area, _iperf_stack_area_size);
            break;
        }

        case TCP_TX_START:
        {

            if(iperf_ctrlInfo_ptr -> TestTime == 0)
                iperf_ctrlInfo_ptr -> TestTime = 10 * NX_IP_PERIODIC_RATE;
            tcp_tx_test_time = (iperf_ctrlInfo_ptr -> TestTime) / NX_IP_PERIODIC_RATE;

#ifdef __PRODUCT_NETXDUO__
            /* Set the transmit ip address.  */
            if (iperf_ctrlInfo_ptr -> version == NX_IP_VERSION_V4)
            {
                tcp_tx_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
                tcp_tx_ip_address.nxd_ip_address.v4 = iperf_ctrlInfo_ptr -> ip;
            }

#ifdef FEATURE_NX_IPV6
            else
            {
                tcp_tx_ip_address.nxd_ip_version = NX_IP_VERSION_V6;
                tcp_tx_ip_address.nxd_ip_address.v6[0] = iperf_ctrlInfo_ptr -> ipv6[0];
                tcp_tx_ip_address.nxd_ip_address.v6[1] = iperf_ctrlInfo_ptr -> ipv6[1];
                tcp_tx_ip_address.nxd_ip_address.v6[2] = iperf_ctrlInfo_ptr -> ipv6[2];
                tcp_tx_ip_address.nxd_ip_address.v6[3] = iperf_ctrlInfo_ptr -> ipv6[3];
            }
#endif
#else
            tcp_tx_ip_address = iperf_ctrlInfo_ptr -> ip;
#endif
            tcp_tx_port = iperf_ctrlInfo_ptr -> port;

            tcp_tx_test(_iperf_stack_area, _iperf_stack_area_size);
            break;
        }

        case UDP_RX_START:
        {
            if(iperf_ctrlInfo_ptr -> TestTime == 0)
                iperf_ctrlInfo_ptr -> TestTime = 10 * NX_IP_PERIODIC_RATE;
            udp_rx_test_time = (iperf_ctrlInfo_ptr -> TestTime) / NX_IP_PERIODIC_RATE;

            udp_rx_test(_iperf_stack_area, _iperf_stack_area_size);
            break;
        }

        case UDP_TX_START:
        {
            if(iperf_ctrlInfo_ptr -> TestTime == 0)
                iperf_ctrlInfo_ptr -> TestTime =  10 * NX_IP_PERIODIC_RATE;
            udp_tx_test_time = (iperf_ctrlInfo_ptr -> TestTime) / NX_IP_PERIODIC_RATE;

            if(iperf_ctrlInfo_ptr -> PacketSize == 0)
                iperf_ctrlInfo_ptr -> PacketSize = 10 * NX_IP_PERIODIC_RATE;
            else if(iperf_ctrlInfo_ptr -> PacketSize > 1470)
                iperf_ctrlInfo_ptr -> PacketSize = 1470;
            udp_tx_packet_size = iperf_ctrlInfo_ptr -> PacketSize;
            udp_tx_port = iperf_ctrlInfo_ptr -> port;

#ifdef __PRODUCT_NETXDUO__
            /* Set the transmit ip address.  */
            if (iperf_ctrlInfo_ptr -> version == NX_IP_VERSION_V4)
            {
                udp_tx_ip_address.nxd_ip_version = NX_IP_VERSION_V4;
                udp_tx_ip_address.nxd_ip_address.v4 = iperf_ctrlInfo_ptr -> ip;
            }

#ifdef FEATURE_NX_IPV6
            else
            {
                udp_tx_ip_address.nxd_ip_version = NX_IP_VERSION_V6;
                udp_tx_ip_address.nxd_ip_address.v6[0] = iperf_ctrlInfo_ptr -> ipv6[0];
                udp_tx_ip_address.nxd_ip_address.v6[1] = iperf_ctrlInfo_ptr -> ipv6[1];
                udp_tx_ip_address.nxd_ip_address.v6[2] = iperf_ctrlInfo_ptr -> ipv6[2];
                udp_tx_ip_address.nxd_ip_address.v6[3] = iperf_ctrlInfo_ptr -> ipv6[3];
                if (udp_tx_packet_size > 1450)
                    udp_tx_packet_size = 1450;
            }
#endif
#else
            udp_tx_ip_address = iperf_ctrlInfo_ptr -> ip;
#endif

            if(iperf_ctrlInfo_ptr -> Rate == 0)
                iperf_ctrlInfo_ptr -> Rate = 10;

            udp_tx_test(_iperf_stack_area, _iperf_stack_area_size);
            break;
        }

        case PING_TEST:
        {
            if(iperf_ctrlInfo_ptr -> ip == 0)
                iperf_ctrlInfo_ptr -> ip = DEFAULT_IPERF_IP;
            if(iperf_ctrlInfo_ptr -> PingRate == 0)
                iperf_ctrlInfo_ptr -> PingRate = 10;
            if(iperf_ctrlInfo_ptr -> TotalPings == 0)
                iperf_ctrlInfo_ptr -> TotalPings = 4;

            ping_test(server_ptr);
            break;
        }

        default:
        {
            break;
        }
    }
}

static void print_tcp_rx_results(NX_HTTP_SERVER *server_ptr)
{

    UINT status;
    NX_PACKET *resp_packet_ptr;

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                                &resp_packet_ptr,
                                NX_TCP_PACKET,
                                NX_WAIT_FOREVER);

    htmlwrite(resp_packet_ptr, toptdtag);
    htmlwrite(resp_packet_ptr, tabletag);
    status +=  htmlwrite(resp_packet_ptr, rightspanline);
    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    htmlwrite(resp_packet_ptr, fonttag);
    htmlwrite(resp_packet_ptr, "TCP Receive Test Done:");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status +=  htmlwrite(resp_packet_ptr, rightspanline);
    status +=  htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Source IP Address: ");
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(iperf_ip));
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Test Time(milliseconds): ");
    _nx_http_server_number_convert(iperf_ctrl_info.RunTime * 1000 / NX_IP_PERIODIC_RATE, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Packets Received: ");
    _nx_http_server_number_convert(iperf_ctrl_info.PacketsRxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Bytes Received: ");
    _nx_http_server_number_convert(iperf_ctrl_info.BytesRxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Throughput(Mbps): ");
    _nx_http_server_number_convert(iperf_ctrl_info.ThroughPut, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    _nx_http_server_number_convert(iperf_ctrl_info.idleTime, mytempstring);
    status += htmlwrite(resp_packet_ptr, "Idle Time: ");
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, "%");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);
#endif

    htmlwrite(resp_packet_ptr, tableendtag);
    htmlwrite(resp_packet_ptr, tdendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    /* Delete the receive thread. */
    thread_tcp_rx_cleanup();
}

static void print_tcp_tx_results(NX_HTTP_SERVER *server_ptr)
{
    UINT status;

    NX_PACKET *resp_packet_ptr;

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                              &resp_packet_ptr,
                              NX_TCP_PACKET,
                              NX_WAIT_FOREVER);

    htmlwrite(resp_packet_ptr, toptdtag);
    htmlwrite(resp_packet_ptr, tabletag);
    status +=  htmlwrite(resp_packet_ptr, rightspanline);
    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    htmlwrite(resp_packet_ptr, fonttag);
    htmlwrite(resp_packet_ptr, "TCP Transmit Test Done:");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, rightspanline);
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Destination IP Address: ");
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(iperf_ip));
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Destination Port: ");
    _nx_http_server_number_convert(iperf_ctrl_info.port, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Test Time(milliseconds): ");
    _nx_http_server_number_convert(iperf_ctrl_info.RunTime * 1000 / NX_IP_PERIODIC_RATE, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Packets Transmitted: ");
    _nx_http_server_number_convert(iperf_ctrl_info.PacketsTxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Bytes Transmitted: ");
    _nx_http_server_number_convert(iperf_ctrl_info.BytesTxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Throughput(Mbps): ");
    _nx_http_server_number_convert(iperf_ctrl_info.ThroughPut, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    _nx_http_server_number_convert(iperf_ctrl_info.idleTime, mytempstring);
    status += htmlwrite(resp_packet_ptr, "Idle Time: ");
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, "%");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);
#endif

    status += htmlwrite(resp_packet_ptr, tableendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    /* Delete the receive thread. */
    thread_tcp_tx_cleanup();
}

static void print_udp_rx_results(NX_HTTP_SERVER *server_ptr)
{
    UINT status;

    NX_PACKET *resp_packet_ptr;
    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                              &resp_packet_ptr,
                              NX_TCP_PACKET,
                              NX_WAIT_FOREVER);

    /* now send the data back to the client.  */
    htmlwrite(resp_packet_ptr, toptdtag);
    htmlwrite(resp_packet_ptr, tabletag);
    status +=  htmlwrite(resp_packet_ptr, rightspanline);
    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    htmlwrite(resp_packet_ptr, fonttag);
    htmlwrite(resp_packet_ptr, "UDP Receive Test Done:");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, rightspanline);
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Source IP Address: ");
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(iperf_ip));
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Test Time(milliseconds): ");
    _nx_http_server_number_convert(iperf_ctrl_info.RunTime * 1000 / NX_IP_PERIODIC_RATE, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Packets Received: ");
    _nx_http_server_number_convert(iperf_ctrl_info.PacketsRxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Bytes Received: ");
    _nx_http_server_number_convert(iperf_ctrl_info.BytesRxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Throughput(Mbps):");
    _nx_http_server_number_convert(iperf_ctrl_info.ThroughPut, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    _nx_http_server_number_convert(iperf_ctrl_info.idleTime, mytempstring);
    status += htmlwrite(resp_packet_ptr, "Idle Time: ");
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, "%");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);
#endif

    status += htmlwrite(resp_packet_ptr, tableendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    thread_udp_rx_cleanup();
}

static void print_udp_tx_results(NX_HTTP_SERVER *server_ptr)
{
    UINT status;

    NX_PACKET *resp_packet_ptr;
    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                              &resp_packet_ptr,
                              NX_TCP_PACKET,
                              NX_WAIT_FOREVER);

    /* now send the data back to the client.  */
    htmlwrite(resp_packet_ptr, toptdtag);
    htmlwrite(resp_packet_ptr, tabletag);
    status +=  htmlwrite(resp_packet_ptr, rightspanline);
    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    htmlwrite(resp_packet_ptr, fonttag);
    htmlwrite(resp_packet_ptr, "UDP Transmit Test Done:");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, rightspanline);
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Destination IP Address: ");
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(iperf_ip));
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Destination Port: ");
    _nx_http_server_number_convert(iperf_ctrl_info.port, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Test Time(milliseconds): ");
    _nx_http_server_number_convert(iperf_ctrl_info.RunTime * 1000 / NX_IP_PERIODIC_RATE, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Packets Transmitted: ");
    _nx_http_server_number_convert(iperf_ctrl_info.PacketsTxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Bytes Transmitted: ");
    _nx_http_server_number_convert(iperf_ctrl_info.BytesTxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Throughput(Mbps):");
    _nx_http_server_number_convert(iperf_ctrl_info.ThroughPut, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    _nx_http_server_number_convert(iperf_ctrl_info.idleTime, mytempstring);
    status += htmlwrite(resp_packet_ptr, "Idle Time: ");
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, "%");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);
#endif

    status += htmlwrite(resp_packet_ptr, tableendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }

    thread_udp_tx_cleanup();
}

void  ping_test(NX_HTTP_SERVER *server_ptr)
{
    UINT  status;
    NX_PACKET *response_ptr;
    UINT          counter;

    NX_PARAMETER_NOT_USED(server_ptr);
    iperf_ctrlInfo_ptr -> PacketsRxed = 0;
    iperf_ctrlInfo_ptr -> PacketsTxed = 0;
    iperf_ctrlInfo_ptr -> TestStatus = 1;

    counter = iperf_ctrlInfo_ptr -> TotalPings;

    while(counter)
    {
        iperf_ctrlInfo_ptr -> PacketsTxed++;
        status = nx_icmp_ping(_iperf_test_ip, iperf_ctrlInfo_ptr -> ip, "abcd", 4, &response_ptr, 1);
        if(status == NX_SUCCESS)
            iperf_ctrlInfo_ptr -> PacketsRxed++;

        counter--;
        tx_thread_sleep(iperf_ctrlInfo_ptr -> PingRate);
    }

    iperf_ctrlInfo_ptr -> TestStatus = 2;
}

static void  print_ping_results(NX_HTTP_SERVER *server_ptr)
 {
    UINT status;
    NX_PACKET *resp_packet_ptr;

    status = nx_packet_allocate(server_ptr -> nx_http_server_packet_pool_ptr,
                              &resp_packet_ptr,
                              NX_TCP_PACKET,
                              NX_WAIT_FOREVER);

    /* now send the data back to the client.  */

    htmlwrite(resp_packet_ptr, toptdtag);
    htmlwrite(resp_packet_ptr, tabletag);
    status +=  htmlwrite(resp_packet_ptr, rightspanline);
    htmlwrite(resp_packet_ptr, trtag);
    htmlwrite(resp_packet_ptr, tdtag);
    htmlwrite(resp_packet_ptr, fonttag);
    htmlwrite(resp_packet_ptr, "PING Test Done:");
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, rightspanline);
    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Destination IP Address: ");
    status += htmlwrite(resp_packet_ptr, get_ip_addr_string(iperf_ip));
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Ping Sent: ");
    _nx_http_server_number_convert(iperf_ctrl_info.PacketsTxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, trtag);
    status += htmlwrite(resp_packet_ptr, tdtag);
    status += htmlwrite(resp_packet_ptr, fonttag);
    status += htmlwrite(resp_packet_ptr, "Number of Ping Received: ");
    _nx_http_server_number_convert(iperf_ctrl_info.PacketsRxed, mytempstring);
    status += htmlwrite(resp_packet_ptr, mytempstring);
    status += htmlwrite(resp_packet_ptr, fontendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);
    status += htmlwrite(resp_packet_ptr, trendtag);

    status += htmlwrite(resp_packet_ptr, tableendtag);
    status += htmlwrite(resp_packet_ptr, tdendtag);

    status +=  nx_tcp_socket_send(&(server_ptr -> nx_http_server_socket), resp_packet_ptr, NX_HTTP_SERVER_TIMEOUT);

    if(status)
    {
        nx_packet_release(resp_packet_ptr);
        _iperf_test_error_counter++;
    }
}

void ping_cleanup(void)
{

}

void    thread_tcp_rx_entry(ULONG thread_input)
{
    UINT            status;
    NX_PACKET      *packet_ptr;
    ULONG           actual_status;
    ULONG           expire_time;
    ctrl_info      *ctrlInfo_ptr;

    /* Set the pointer.  */
    ctrlInfo_ptr = (ctrl_info *)thread_input;

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    /* Update the time.  */
    thread_time = 0;
    isr_time = 0 ;
    idle_time = 0;
#endif

#ifdef IPERF_TEST
    /* Update the test result.  */
    ctrlInfo_ptr -> PacketsRxed = 0;
    ctrlInfo_ptr -> BytesRxed = 0;
    ctrlInfo_ptr -> ThroughPut = 0;
    ctrlInfo_ptr -> StartTime = 0;
    ctrlInfo_ptr -> RunTime = 0;
    ctrlInfo_ptr -> ErrorCode = 0;
#endif

    /* Ensure the IP instance has been initialized.  */
    status =  nx_ip_status_check(_iperf_test_ip, NX_IP_INITIALIZE_DONE, &actual_status, NX_IP_PERIODIC_RATE);

    /* Check status...  */
    if (status != NX_SUCCESS)
    {
        error_counter++;
        return;
    }

    /* Create a socket.  */
    status =  nx_tcp_socket_create(_iperf_test_ip, &tcp_server_socket, "TCP Server Socket",
                                NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 32*1024,
                                NX_NULL, thread_tcp_rx_disconnect_received);

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Setup this thread to listen.  */
    status =  nx_tcp_server_socket_listen(_iperf_test_ip, TCP_RX_PORT, &tcp_server_socket, 5, thread_tcp_rx_connect_received);

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Increment thread tcp rx's counter.  */
    thread_tcp_rx_counter++;

    /* Accept a client socket connection.  */
    status =  nx_tcp_server_socket_accept(&tcp_server_socket, NX_WAIT_FOREVER);

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /*Get source ip address*/
#ifdef __PRODUCT_NETXDUO__
    ctrlInfo_ptr -> version = tcp_server_socket.nx_tcp_socket_connect_ip.nxd_ip_version;
    if (ctrlInfo_ptr -> version == NX_IP_VERSION_V4)
        ctrlInfo_ptr -> ip = tcp_server_socket.nx_tcp_socket_connect_ip.nxd_ip_address.v4;

#ifdef FEATURE_NX_IPV6
    else if (ctrlInfo_ptr -> version == NX_IP_VERSION_V6)
        memcpy(ctrlInfo_ptr -> ipv6, tcp_server_socket.nx_tcp_socket_connect_ip.nxd_ip_address.v6, sizeof(ULONG) * 4);
#endif

#else
    ctrlInfo_ptr -> ip = tcp_server_socket.nx_tcp_socket_connect_ip;
#endif

#ifdef IPERF_TEST
    /* Set the test start time.  */
    ctrlInfo_ptr -> StartTime = tx_time_get();
    expire_time = ctrlInfo_ptr -> StartTime + (ctrlInfo_ptr -> TestTime) + 20;
#define CONDITION  (tx_time_get() < expire_time)
#else
#define CONDITION  1
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_reset();
    _tx_execution_isr_time_reset();
    _tx_execution_idle_time_reset();
#endif

    while(CONDITION)
    {
        /* Receive a TCP message from the socket.  */
        status =  nx_tcp_socket_receive(&tcp_server_socket, &packet_ptr, NX_WAIT_FOREVER);

        /* Check for error.  */
        if (status)
        {
            error_counter++;
            break;
        }

#ifdef IPERF_TEST
        /* Update the counter.  */
        ctrlInfo_ptr -> PacketsRxed ++;
        ctrlInfo_ptr -> BytesRxed += packet_ptr -> nx_packet_length;
#endif

        /* Release the packet.  */
        nx_packet_release(packet_ptr);

    }

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_get(&thread_time);
    _tx_execution_isr_time_get(&isr_time);
    _tx_execution_idle_time_get(&idle_time);
#endif

#ifdef IPERF_TEST
    /* Calculate the test time and Throughput(Mbps).  */
    ctrlInfo_ptr -> RunTime = tx_time_get() - ctrlInfo_ptr -> StartTime;
    ctrlInfo_ptr -> ThroughPut = ctrlInfo_ptr -> BytesRxed / ctrlInfo_ptr -> RunTime * NX_IP_PERIODIC_RATE / 125000;
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    ctrlInfo_ptr -> idleTime = (ULONG)((unsigned long long)idle_time * 100 / ((unsigned long long)thread_time + (unsigned long long)isr_time + (unsigned long long)idle_time));
#endif

    /* Disconnect the server socket.  */
    status =  nx_tcp_socket_disconnect(&tcp_server_socket, 10);

    /* Check for error.  */
    if (status)
        error_counter++;

    /* Unaccept the server socket.  */
    status =  nx_tcp_server_socket_unaccept(&tcp_server_socket);
    status += nx_tcp_server_socket_unlisten(_iperf_test_ip, TCP_RX_PORT);

    /* Check for error.  */
    if (status)
        error_counter++;

    if(error_counter)
        ctrlInfo_ptr -> ErrorCode = error_counter;

    /* Delete the socket.  */
    nx_tcp_socket_delete(&tcp_server_socket);
}

void  thread_tcp_rx_connect_received(NX_TCP_SOCKET *socket_ptr, UINT port)
{
    /* Check for the proper socket and port.  */
    if ((socket_ptr != &tcp_server_socket) || (port != TCP_RX_PORT))
        error_counter++;
}

void  thread_tcp_rx_disconnect_received(NX_TCP_SOCKET *socket)
{
    /* Check for proper disconnected socket.  */
    if (socket != &tcp_server_socket)
        error_counter++;
}

void thread_tcp_rx_cleanup(void)
{
    nx_tcp_socket_disconnect(&tcp_server_socket, NX_NO_WAIT);
    nx_tcp_server_socket_unaccept(&tcp_server_socket);
    nx_tcp_server_socket_unlisten(_iperf_test_ip, TCP_RX_PORT);
    nx_tcp_socket_delete(&tcp_server_socket);

    tx_thread_terminate(&thread_tcp_rx_iperf);
    tx_thread_delete(&thread_tcp_rx_iperf);
}

void tcp_rx_test(UCHAR* stack_space, ULONG stack_size)
{

    UINT status;

    status = tx_thread_create(&thread_tcp_rx_iperf, "thread tcp rx",
                              thread_tcp_rx_entry,
                              (ULONG)&iperf_ctrl_info,
                              stack_space, stack_size, TCP_RX_THREAD_PRIORITY, TCP_RX_THREAD_PRIORITY,
                              TX_NO_TIME_SLICE, TX_AUTO_START);

    if (status)
    {
        iperf_ctrl_info.ErrorCode = 1;
    }
    return;
}

void    thread_tcp_tx_entry(ULONG thread_input)
{
    UINT
        status;
    UINT            is_first = NX_TRUE;
    NX_PACKET      *my_packet = NX_NULL;
    NX_PACKET      *packet_ptr;
    NX_PACKET      *last_packet;
    ULONG           expire_time;
    ctrl_info      *ctrlInfo_ptr;
    ULONG           packet_size;
    ULONG           remaining_size;
#ifdef __PRODUCT_NETXDUO__
    NXD_ADDRESS     server_ip;
#endif

    /* Set the pointer.  */
    ctrlInfo_ptr = (ctrl_info *)thread_input;

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    thread_time = 0;
    isr_time = 0;
    idle_time = 0;
#endif

#ifdef IPERF_TEST
    ctrlInfo_ptr -> PacketsTxed = 0;
    ctrlInfo_ptr -> BytesTxed = 0;
    ctrlInfo_ptr -> ThroughPut = 0;
    ctrlInfo_ptr -> StartTime = 0;
    ctrlInfo_ptr -> RunTime = 0;
    ctrlInfo_ptr -> ErrorCode = 0;

#ifdef __PRODUCT_NETXDUO__
    server_ip.nxd_ip_version = ctrlInfo_ptr -> version;

#ifdef FEATURE_NX_IPV6
    if(ctrlInfo_ptr -> version == NX_IP_VERSION_V6)
    {
        server_ip.nxd_ip_address.v6[0] = ctrlInfo_ptr -> ipv6[0];
        server_ip.nxd_ip_address.v6[1] = ctrlInfo_ptr -> ipv6[1];
        server_ip.nxd_ip_address.v6[2] = ctrlInfo_ptr -> ipv6[2];
        server_ip.nxd_ip_address.v6[3] = ctrlInfo_ptr -> ipv6[3];
    }
    else if(ctrlInfo_ptr -> version == NX_IP_VERSION_V4)
#endif
        server_ip.nxd_ip_address.v4 = ctrlInfo_ptr -> ip;
#endif
#endif

    /* TCP Transmit Test Starts in 2 seconds.  */
    tx_thread_sleep(200);

    /* Create the socket.  */
    status =  nx_tcp_socket_create(_iperf_test_ip, &tcp_client_socket, "TCP Client Socket",
                                   NX_IP_NORMAL, NX_FRAGMENT_OKAY, NX_IP_TIME_TO_LIVE, 32*1024,
                                   NX_NULL, NX_NULL);

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Bind the socket.  */
    status =  nx_tcp_client_socket_bind(&tcp_client_socket, TCP_TX_PORT, NX_WAIT_FOREVER);

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Attempt to connect the socket.  */
#ifdef __PRODUCT_NETXDUO__
    status =  nxd_tcp_client_socket_connect(&tcp_client_socket, &server_ip, ctrlInfo_ptr -> port, NX_WAIT_FOREVER);
#else
    status =  nx_tcp_client_socket_connect(&tcp_client_socket, ctrlInfo_ptr -> ip, ctrlInfo_ptr -> port, NX_WAIT_FOREVER);
#endif

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

#ifdef IPERF_TEST
    /* Set the test start time.  */
    ctrlInfo_ptr -> StartTime = tx_time_get();
    expire_time = ctrlInfo_ptr -> StartTime + (ctrlInfo_ptr -> TestTime);
#define CONDITION  (tx_time_get() < expire_time)
#else
#define CONDITION  1
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_reset();
    _tx_execution_isr_time_reset();
    _tx_execution_idle_time_reset();
#endif

    /* Set the packet size.  */
    status = nx_tcp_socket_mss_get(&tcp_client_socket, &packet_size);

    /* Check for error.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Loop to transmit the packet.  */
    while(CONDITION)
    {

        /* Allocate a packet.  */
        status =  nx_packet_allocate(_iperf_test_pool, &my_packet, NX_TCP_PACKET, NX_WAIT_FOREVER);

        /* Check status.  */
        if (status != NX_SUCCESS)
            break;

        /* Write ABCs into the packet payload!  */
        /* Adjust the write pointer.  */
        my_packet -> nx_packet_length =  packet_size;
        if (my_packet -> nx_packet_prepend_ptr + packet_size <= my_packet -> nx_packet_data_end)
        {
            my_packet -> nx_packet_append_ptr =  my_packet -> nx_packet_prepend_ptr + packet_size;
            remaining_size = 0;
        }
        else
        {
            my_packet -> nx_packet_append_ptr = my_packet -> nx_packet_data_end;
            remaining_size = packet_size - (ULONG)(my_packet -> nx_packet_append_ptr - my_packet -> nx_packet_prepend_ptr);
            last_packet = my_packet;
        }

        while (remaining_size)
        {
            /* Allocate a packet.  */
            status =  nx_packet_allocate(_iperf_test_pool, &packet_ptr, NX_TCP_PACKET, NX_WAIT_FOREVER);

            /* Check status.  */
            if (status != NX_SUCCESS)
                break;

            last_packet -> nx_packet_next = packet_ptr;
            last_packet = packet_ptr;
            if (remaining_size < (ULONG)(packet_ptr -> nx_packet_data_end - packet_ptr -> nx_packet_prepend_ptr))
            {
                packet_ptr -> nx_packet_append_ptr =  packet_ptr -> nx_packet_prepend_ptr + remaining_size;
            }
            else
            {
                packet_ptr -> nx_packet_append_ptr =  packet_ptr -> nx_packet_data_end;
            }
            remaining_size = remaining_size - (ULONG)(packet_ptr -> nx_packet_append_ptr - packet_ptr -> nx_packet_prepend_ptr);
        }

        if (is_first)
        {
            memset(my_packet -> nx_packet_prepend_ptr, 0, (UINT)(my_packet -> nx_packet_data_end - my_packet -> nx_packet_prepend_ptr));
            is_first = NX_FALSE;
        }

        /* Send the packet out!  */
        status =  nx_tcp_socket_send(&tcp_client_socket, my_packet, NX_WAIT_FOREVER);

        /* Determine if the status is valid.  */
        if (status)
        {
            error_counter++;
            nx_packet_release(my_packet);
            break;
        }
#ifdef IPERF_TEST
        else
        {

            /* Update the counter.  */
            ctrlInfo_ptr -> PacketsTxed ++;
            ctrlInfo_ptr -> BytesTxed += packet_size;
        }
#endif
    }

#ifdef IPERF_TEST
    /* Calculate the test time and Throughput(Mbps).  */
    ctrlInfo_ptr -> RunTime = tx_time_get() - ctrlInfo_ptr -> StartTime;
    ctrlInfo_ptr -> ThroughPut = ctrlInfo_ptr -> BytesTxed / ctrlInfo_ptr -> RunTime * NX_IP_PERIODIC_RATE / 125000;
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_get(&thread_time);
    _tx_execution_isr_time_get(&isr_time);
    _tx_execution_idle_time_get(&idle_time);
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    ctrlInfo_ptr -> idleTime = (ULONG)((unsigned long long)idle_time * 100 / ((unsigned long long)thread_time + (unsigned long long)isr_time + (unsigned long long)idle_time));
#endif

    /* Disconnect this socket.  */
    status =  nx_tcp_socket_disconnect(&tcp_client_socket, NX_NO_WAIT);

    /* Determine if the status is valid.  */
    if (status)
        error_counter++;

    /* Unbind the socket.  */
    status =  nx_tcp_client_socket_unbind(&tcp_client_socket);

    /* Check for error.  */
    if (status)
        error_counter++;

    if(error_counter)
        ctrlInfo_ptr -> ErrorCode = error_counter;

    /* Delete the socket.  */
    nx_tcp_socket_delete(&tcp_client_socket);
}

void thread_tcp_tx_cleanup(void)
{
    nx_tcp_socket_disconnect(&tcp_client_socket, NX_NO_WAIT);
    nx_tcp_client_socket_unbind(&tcp_client_socket);
    nx_tcp_socket_delete(&tcp_client_socket);

    tx_thread_terminate(&thread_tcp_tx_iperf);
    tx_thread_delete(&thread_tcp_tx_iperf);
}

void  tcp_tx_test(UCHAR* stack_space, ULONG stack_size)
{
    UINT         status;

    status = tx_thread_create(&thread_tcp_tx_iperf, "thread tcp tx",
                              thread_tcp_tx_entry,
                              (ULONG)&iperf_ctrl_info,
                              stack_space, stack_size, NX_HTTP_SERVER_PRIORITY + 1, NX_HTTP_SERVER_PRIORITY + 1,
                              TX_NO_TIME_SLICE, TX_AUTO_START);

    if (status)
    {
        iperf_ctrl_info.ErrorCode = 1;
    }
    return;
}


void   thread_udp_rx_entry(ULONG thread_input)
{
    UINT           status;
    ULONG          expire_time;
    NX_PACKET     *my_packet;
    ctrl_info     *ctrlInfo_ptr;
    int            packetID = 0;
    UINT           sender_port;
    ULONG           tmp;
#ifdef __PRODUCT_NETXDUO__
    NXD_ADDRESS    source_ip_address;
#endif

    /* Set the pointer.  */
    ctrlInfo_ptr = (ctrl_info *)thread_input;

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    thread_time = 0;
    isr_time = 0 ;
    idle_time = 0;
#endif

#ifdef  IPERF_TEST
    /* Update the test result.  */
    ctrlInfo_ptr -> PacketsRxed = 0;
    ctrlInfo_ptr -> BytesRxed = 0;
    ctrlInfo_ptr -> ThroughPut = 0;
    ctrlInfo_ptr -> StartTime = 0;
    ctrlInfo_ptr -> RunTime = 0;
    ctrlInfo_ptr -> ErrorCode = 0;
#endif

   /* Create a UDP socket.  */
    status = nx_udp_socket_create(_iperf_test_ip, &udp_server_socket, "UDP Server Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Bind the UDP socket to the IP port.  */
    status = nx_udp_socket_bind(&udp_server_socket, UDP_RX_PORT, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Disable checksum for UDP. */
    nx_udp_socket_checksum_disable(&udp_server_socket);

    /* Receive a UDP packet.  */
    status = nx_udp_socket_receive(&udp_server_socket, &my_packet, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

#ifdef __PRODUCT_NETXDUO__
    /*Get source ip address*/
    nxd_udp_source_extract(my_packet, &source_ip_address, &sender_port);

    /* Set the IP address Version.  */
    ctrlInfo_ptr -> version = source_ip_address.nxd_ip_version;

    if (ctrlInfo_ptr -> version == NX_IP_VERSION_V4)
        ctrlInfo_ptr -> ip = source_ip_address.nxd_ip_address.v4;

#ifdef FEATURE_NX_IPV6
    else if (ctrlInfo_ptr -> version == NX_IP_VERSION_V6)
    {
        ctrlInfo_ptr -> ipv6[0] = source_ip_address.nxd_ip_address.v6[0];
        ctrlInfo_ptr -> ipv6[1] = source_ip_address.nxd_ip_address.v6[1];
        ctrlInfo_ptr -> ipv6[2] = source_ip_address.nxd_ip_address.v6[2];
        ctrlInfo_ptr -> ipv6[3] = source_ip_address.nxd_ip_address.v6[3];
    }
#endif

#else
    /*Get source ip address*/
    nx_udp_source_extract(my_packet, &(ctrlInfo_ptr -> ip), &sender_port);

    /* Set the IP address Version.  */
    ctrlInfo_ptr -> version = 0x4;
#endif

    /* Release the packet.  */
    nx_packet_release(my_packet);

#ifdef IPERF_TEST
    /* Set the test start time.  */
    ctrlInfo_ptr -> StartTime = tx_time_get();
    expire_time = ctrlInfo_ptr -> StartTime + (ctrlInfo_ptr -> TestTime) + 5;   /* Wait 5 more ticks to synchronize. */
#define CONDITION  (tx_time_get() < expire_time)
#else
#define CONDITION  1
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_reset();
    _tx_execution_isr_time_reset();
    _tx_execution_idle_time_reset();
#endif

    while(CONDITION)
    {
        /* Receive a UDP packet.  */
        status =  nx_udp_socket_receive(&udp_server_socket, &my_packet, TX_WAIT_FOREVER);

        /* Check status.  */
        if (status != NX_SUCCESS)
        {
          error_counter++;
          break;
        }

#ifdef IPERF_TEST
        /* Update the counter.  */
        ctrlInfo_ptr -> PacketsRxed++;
        ctrlInfo_ptr -> BytesRxed += my_packet -> nx_packet_length;
#endif

        /* Detect the end of the test signal. */
        packetID = *(int*)(my_packet -> nx_packet_prepend_ptr);

        tmp = (ULONG)packetID;
        NX_CHANGE_ULONG_ENDIAN(tmp);
        packetID = (int)tmp;


        /* Check the packet ID.  */
        if(packetID < 0)
        {

            /* Test has finished. */
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
            _tx_execution_thread_total_time_get(&thread_time);
            _tx_execution_isr_time_get(&isr_time);
            _tx_execution_idle_time_get(&idle_time);
#endif

#ifdef IPERF_TEST
            /* Calculate the test time and Throughput.  */
            ctrlInfo_ptr -> RunTime = tx_time_get() - ctrlInfo_ptr -> StartTime;
            ctrlInfo_ptr -> ThroughPut = ctrlInfo_ptr -> BytesRxed / ctrlInfo_ptr -> RunTime * NX_IP_PERIODIC_RATE / 125000;
#endif

            /* received end of the test signal */

            /* Send the UDP packet.  */
#ifdef __PRODUCT_NETXDUO__
            status = nxd_udp_socket_send(&udp_server_socket, my_packet, &source_ip_address, sender_port);
#else
            status = nx_udp_socket_send(&udp_server_socket, my_packet, ctrlInfo_ptr -> ip, sender_port);
#endif

            /* Check the status.  */
            if(status)
            {

                /* Release the packet.  */
                nx_packet_release(my_packet);
            }
            else
            {

                /* Loop to receive the end of the test signal.  */
                while(1)
                {

                    /* Receive a UDP packet.  */
                    status =  nx_udp_socket_receive(&udp_server_socket, &my_packet, 20);

                    /* Check the status.  */
                    if(status)
                        break;

                    /* Send the UDP packet.  */
#ifdef __PRODUCT_NETXDUO__
                    status = nxd_udp_socket_send(&udp_server_socket, my_packet, &source_ip_address, sender_port);
#else
                    status = nx_udp_socket_send(&udp_server_socket, my_packet, ctrlInfo_ptr -> ip, sender_port);
#endif

                    /* Check the status.  */
                    if(status)
                    {

                        /* Release the packet.  */
                        nx_packet_release(my_packet);
                    }
                }
            }
            break;
        }
        else
        {

            /* Release the packet.  */
            nx_packet_release(my_packet);
        }
    }

    if(packetID >= 0)
    {

        /* Test is not sychronized. */
#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
        _tx_execution_thread_total_time_get(&thread_time);
        _tx_execution_isr_time_get(&isr_time);
        _tx_execution_idle_time_get(&idle_time);
#endif

#ifdef IPERF_TEST
        /* Calculate the test time and Throughput.  */
        ctrlInfo_ptr -> RunTime = tx_time_get() - ctrlInfo_ptr -> StartTime;
        ctrlInfo_ptr -> ThroughPut = ctrlInfo_ptr -> BytesRxed / ctrlInfo_ptr -> RunTime * NX_IP_PERIODIC_RATE / 125000;
#endif
    }

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    ctrlInfo_ptr -> idleTime = (ULONG)((unsigned long long)idle_time * 100 / ((unsigned long long)thread_time + (unsigned long long)isr_time + (unsigned long long)idle_time));
#endif

    /* Unbind and Delete the socket.  */
    nx_udp_socket_unbind(&udp_server_socket);
    nx_udp_socket_delete(&udp_server_socket);

    /* Check error counter.  */
    if(error_counter)
        ctrlInfo_ptr -> ErrorCode = error_counter;
}

void thread_udp_rx_cleanup(void)
{
    nx_udp_socket_unbind(&udp_server_socket);
    nx_udp_socket_delete(&udp_server_socket);

    tx_thread_terminate(&thread_udp_rx_iperf);
    tx_thread_delete(&thread_udp_rx_iperf);
}

void  udp_rx_test(UCHAR* stack_space, ULONG stack_size)
{
    UINT         status;

    status = tx_thread_create(&thread_udp_rx_iperf, "thread udp rx",
                              thread_udp_rx_entry,
                              (ULONG)&iperf_ctrl_info,
                              stack_space, stack_size, NX_HTTP_SERVER_PRIORITY + 1, NX_HTTP_SERVER_PRIORITY + 1,
                              TX_NO_TIME_SLICE, TX_AUTO_START);

    if (status)
    {
        iperf_ctrl_info.ErrorCode = 1;
    }
    return;
}

static void send_udp_packet(int udp_id, ctrl_info *ctrlInfo_ptr)
{

    UINT             status;
    NX_PACKET       *my_packet = NX_NULL;
    udp_payload     *payload_ptr;
    ULONG            tmp;
#ifndef __PRODUCT_NETXDUO__
    UINT             max_payload_length;
    UINT             length;
    NX_PACKET       *packet_ptr;
    NX_PACKET       *last_pkt_ptr = NX_NULL;
#endif

#ifdef __PRODUCT_NETXDUO__
    NXD_ADDRESS server_ip;
    server_ip.nxd_ip_version = ctrlInfo_ptr -> version;

#ifdef FEATURE_NX_IPV6
    if (ctrlInfo_ptr -> version == NX_IP_VERSION_V6)
    {
        server_ip.nxd_ip_address.v6[0] = ctrlInfo_ptr -> ipv6[0];
        server_ip.nxd_ip_address.v6[1] = ctrlInfo_ptr -> ipv6[1];
        server_ip.nxd_ip_address.v6[2] = ctrlInfo_ptr -> ipv6[2];
        server_ip.nxd_ip_address.v6[3] = ctrlInfo_ptr -> ipv6[3];
    }
    else if (ctrlInfo_ptr -> version == NX_IP_VERSION_V4)
#endif

        server_ip.nxd_ip_address.v4 = ctrlInfo_ptr -> ip;

    /* Send the end of test indicator. */
    nx_packet_allocate(_iperf_test_pool, &my_packet, NX_IPv6_UDP_PACKET, TX_WAIT_FOREVER);
    my_packet -> nx_packet_append_ptr =  my_packet -> nx_packet_prepend_ptr + ctrlInfo_ptr -> PacketSize;
#else
    length = ctrlInfo_ptr -> PacketSize;
    while(length)
    {
        nx_packet_allocate(_iperf_test_pool, &packet_ptr, NX_UDP_PACKET, TX_WAIT_FOREVER);

        max_payload_length = (UINT)(packet_ptr -> nx_packet_data_end - packet_ptr -> nx_packet_prepend_ptr);

        if(my_packet == NX_NULL)
            my_packet = packet_ptr;
        else
            last_pkt_ptr -> nx_packet_next = packet_ptr;
        last_pkt_ptr = packet_ptr;

        if(length < max_payload_length)
        {
            packet_ptr -> nx_packet_append_ptr = packet_ptr -> nx_packet_prepend_ptr + length;
            break;
        }
        else
        {
            packet_ptr -> nx_packet_append_ptr = packet_ptr -> nx_packet_prepend_ptr + max_payload_length;
            length -= max_payload_length;
        }
    }
#endif

    payload_ptr = (udp_payload *)my_packet -> nx_packet_prepend_ptr;
    payload_ptr -> udp_id = udp_id;
    payload_ptr -> tv_sec = _tx_timer_system_clock / NX_IP_PERIODIC_RATE;
    payload_ptr -> tv_usec = _tx_timer_system_clock / NX_IP_PERIODIC_RATE * 1000000;

    tmp = (ULONG)payload_ptr -> udp_id;
    NX_CHANGE_ULONG_ENDIAN(tmp);
    payload_ptr -> udp_id = (int)tmp;

    NX_CHANGE_ULONG_ENDIAN(payload_ptr -> tv_sec);
    NX_CHANGE_ULONG_ENDIAN(payload_ptr -> tv_usec);

    /* Adjust the write pointer.  */
    my_packet -> nx_packet_length = ctrlInfo_ptr -> PacketSize;

    /* Send the UDP packet.  */
#ifdef __PRODUCT_NETXDUO__
    status = nxd_udp_socket_send(&udp_client_socket, my_packet, &server_ip, ctrlInfo_ptr -> port);
#else
    status = nx_udp_socket_send(&udp_client_socket, my_packet, ctrlInfo_ptr -> ip, ctrlInfo_ptr -> port);
#endif

    /* Check the status.  */
    if(status)
    {

        /* Release the packet.  */
        nx_packet_release(my_packet);
        return;
    }
}

void  thread_udp_tx_entry(ULONG thread_input)
{
    UINT            status;
    ULONG           expire_time;
    ctrl_info       *ctrlInfo_ptr;
    NX_PACKET       *my_packet;
    int              i;
    long             udp_id;

    /* Initialize the value.  */
    udp_id = 0;
    ctrlInfo_ptr = (ctrl_info *)thread_input;

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    thread_time = 0;
    isr_time = 0 ;
    idle_time = 0;
#endif
#ifdef IPERF_TEST
    ctrlInfo_ptr -> PacketsTxed = 0;
    ctrlInfo_ptr -> BytesTxed = 0;
    ctrlInfo_ptr -> ThroughPut = 0;
    ctrlInfo_ptr -> StartTime = 0;
    ctrlInfo_ptr -> RunTime = 0;
    ctrlInfo_ptr -> ErrorCode = 0;
#endif

    /* UDP Transmit Test Starts in 2 seconds.  */
    tx_thread_sleep(200);

    /* Create a UDP socket.  */
    status = nx_udp_socket_create(_iperf_test_ip, &udp_client_socket, "UDP Client Socket", NX_IP_NORMAL, NX_FRAGMENT_OKAY, 0x80, 5);

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Bind the UDP socket to the IP port.  */
    status =  nx_udp_socket_bind(&udp_client_socket, UDP_TX_PORT, TX_WAIT_FOREVER);

    /* Check status.  */
    if (status)
    {
        error_counter++;
        return;
    }

    /* Do not calculate checksum for UDP. */
    nx_udp_socket_checksum_disable(&udp_client_socket);

#ifdef IPERF_TEST
    /* Set the test start time.  */
    ctrlInfo_ptr -> StartTime = tx_time_get();
    expire_time = ctrlInfo_ptr -> StartTime + (ctrlInfo_ptr -> TestTime);
#define CONDITION  (tx_time_get() < expire_time)
#else
#define CONDITION  1
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_reset();
    _tx_execution_isr_time_reset();
    _tx_execution_idle_time_reset();
#endif

    while(CONDITION)
    {

        /* Write ABCs into the packet payload!  */
        send_udp_packet(udp_id, ctrlInfo_ptr);

        /* Update the ID.  */
        udp_id = (udp_id + 1) & 0x7FFFFFFF;

#ifdef IPERF_TEST
        /* Update the counter.  */
        ctrlInfo_ptr -> PacketsTxed++;
        ctrlInfo_ptr -> BytesTxed += ctrlInfo_ptr -> PacketSize;
#endif
    }

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    _tx_execution_thread_total_time_get(&thread_time);
    _tx_execution_isr_time_get(&isr_time);
    _tx_execution_idle_time_get(&idle_time);
#endif

#ifdef IPERF_TEST
    /* Calculate the test time and Throughput.  */
    ctrlInfo_ptr -> RunTime = tx_time_get() - ctrlInfo_ptr -> StartTime;
    ctrlInfo_ptr -> ThroughPut = ctrlInfo_ptr -> BytesTxed / ctrlInfo_ptr -> RunTime * NX_IP_PERIODIC_RATE / 125000;
#endif

#ifdef TX_ENABLE_EXECUTION_CHANGE_NOTIFY
    ctrlInfo_ptr -> idleTime = (ULONG)((unsigned long long)idle_time * 100 / ((unsigned long long)thread_time + (unsigned long long)isr_time + (unsigned long long)idle_time));
#endif

    if(error_counter)
        ctrlInfo_ptr -> ErrorCode = error_counter;

    ctrlInfo_ptr -> PacketSize = 100;
    for(i = 0; i < 10; i++)
    {
        /* Send the end of the test signal*/
        send_udp_packet((0 - udp_id), ctrlInfo_ptr);

        /* Receive the packet.  s*/
        if(nx_udp_socket_receive(&udp_client_socket, &my_packet, 10) == NX_SUCCESS)
        {
            nx_packet_release(my_packet);
            break;
        }
    }

    /* Unbind and Delete the socket.  */
    nx_udp_socket_unbind(&udp_client_socket);
    nx_udp_socket_delete(&udp_client_socket);
}

void thread_udp_tx_cleanup(void)
{
    nx_udp_socket_unbind(&udp_client_socket);
    nx_udp_socket_delete(&udp_client_socket);
    tx_thread_terminate(&thread_udp_tx_iperf);
    tx_thread_delete(&thread_udp_tx_iperf);
}

void  udp_tx_test(UCHAR* stack_space, ULONG stack_size)
{
    UINT         status;

    status = tx_thread_create(&thread_udp_tx_iperf, "thread udp tx",
                              thread_udp_tx_entry,
                              (ULONG)&iperf_ctrl_info,
                              stack_space, stack_size, NX_HTTP_SERVER_PRIORITY + 1, NX_HTTP_SERVER_PRIORITY + 1,
                              TX_NO_TIME_SLICE, TX_AUTO_START);

    if (status)
    {
        iperf_ctrl_info.ErrorCode = 1;
    }
    return;
}
