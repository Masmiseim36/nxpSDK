/*
 * Copyright 2018, Cypress Semiconductor Corporation or a subsidiary of 
 * Cypress Semiconductor Corporation. All Rights Reserved.
 * 
 * This software, associated documentation and materials ("Software"),
 * is owned by Cypress Semiconductor Corporation
 * or one of its subsidiaries ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products. Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

/*
 * Wl server for generic RTOS
 */

#include <stdio.h>
#include <string.h>
//#include "wl_tool.h"
//#include "wiced_management.h"
//#include "wiced_rtos.h"
//#include "wiced_utilities.h"
#include "internal/wwd_sdpcm.h"
//#include "internal/bus_protocols/wwd_bus_protocol_interface.h"
//
//#include "internal/wwd_internal.h"
//#include "wwd_assert.h"
//#include "wwd_buffer_interface.h"
//
//#if defined(MFG_TEST_ENABLE_AUDIO_LOOPBACK)
//#include "audio_loopback.h"
//#endif
//#include "wiced_framework.h"
//#if defined(MFG_TEST_ENABLE_IPERF)
//#include <iperf.h>
//#endif /* #if defined(MFG_TEST_ENABLE_IPERF) */
typedef uint32_t uint32;
#include <wlu_cmd.h>

//#include "wifi_utils.h"

#include "wwd_wiced.h"

/******************************************************
 *                    Constants
 ******************************************************/

#if ( WICED_PAYLOAD_MTU < 8192 )
#pragma message ("\n\n" \
                "!!!! WICED_PAYLOAD_MTU NOTE:\n" \
                "!!!!     WICED_PAYLOAD_MTU is too small to work with all WL commands\n" \
                "!!!!     Some, like scanresults will fail\n")
#endif

#define WLU_SERVER_THREAD_PRIORITY          (WICED_DEFAULT_LIBRARY_PRIORITY)
#define WLU_SERVER_STACK_SIZE               (WICED_DEFAULT_APPLICATION_STACK_SIZE)
#define WICED_DEFAULT_NET_CREATE_PRIORITY   (6)

#define SUCCESS                     1
#define FAIL                        -1
#define NO_PACKET                   -2
#define SERIAL_PORT_ERR             -3
#ifdef __cplusplus

#define TYPEDEF_BOOL
#ifndef FALSE
#define FALSE   (false)
#endif
#ifndef TRUE
#define TRUE    (true)
#endif

#else   /* ! __cplusplus */

#define TYPEDEF_BOOL
#ifndef bool
typedef unsigned char   bool;           /* consistent w/BOOL */
#endif
#ifndef FALSE
#define FALSE   (0)
#endif
#ifndef TRUE
#define TRUE    (1)
#endif

#endif  /* ! __cplusplus */

#define BCME_STRLEN        64  /* Max string length for BCM errors */

/******************************************************
 *                    Structures
 ******************************************************/

//typedef struct
//{
//    wiced_thread_t         thread;
//    volatile wiced_bool_t  started;
//    wiced_uart_t           uart_id;
//#ifdef  RWL_SOCKET
//    volatile wiced_bool_t   eth_started;
//    int                     eth_port;
//#endif
//} wiced_wlu_server_t;

//static wiced_wlu_server_t wlu_server =
//{
//    .started            = WICED_FALSE,
//#ifdef  RWL_SOCKET
//    .eth_started        = WICED_FALSE,
//#endif
//};

#ifdef  RWL_SOCKET
wiced_tcp_socket_t      tcp_client_socket;
#endif

typedef struct {
    char *buf;
    int buf_len;
} thread_cmd_params_t;

/******************************************************
 *               Function Declarations
 ******************************************************/

//static void wlu_server_thread( uint32_t thread_input );
wiced_result_t wiced_start_wlu_server( void );
wiced_result_t wiced_stop_wlu_server( void );

extern int remote_server_exec(int argc, char **argv, void *ifr);
extern const char *bcmerrorstr(int bcmerror);

/******************************************************
 *               Variable Definitions
 ******************************************************/

static int ifnum = 0;
//static int                   currently_downloading    = 0;
//static host_semaphore_type_t downloading_semaphore;
//static host_semaphore_type_t download_ready_semaphore;
//static wiced_thread_t        downloading_thread;
#if defined(MFG_TEST_ENABLE_IPERF)
static wiced_thread_t        iperf_thread;
static wiced_bool_t          iperf_thread_is_running = WICED_FALSE;
#endif /* defined(MFG_TEST_ENABLE_IPERF) */
unsigned short defined_debug = 1; //DEBUG_ERR | DEBUG_INFO | DEBUG_DBG;

/******************************************************
 *               Function Definitions
 ******************************************************/

int
rwl_create_dir(void)
{
    /* not supported */
    return 0;
}

int
remote_shell_execute(char *buf_ptr)
{
    /* not supported */
    UNUSED_PARAMETER(buf_ptr);
    return 0;
}

int
remote_shell_get_resp(char* shell_fname, char* buf_ptr, int msg_len)
{
    /* not supported */
    UNUSED_PARAMETER(shell_fname);
    UNUSED_PARAMETER(buf_ptr);
    UNUSED_PARAMETER(msg_len);
    return 0;
}

//int
//rwl_write_serial_port(void* hndle, char* write_buf, unsigned long size, unsigned long *numwritten)
//{
//    wiced_result_t result;
//    wiced_uart_t uart = *((wiced_uart_t*) hndle);
//
//    wiced_assert( "invalid argument", uart < WICED_UART_MAX );
//
//    if ( size == 0 )
//    {
//        return SUCCESS;
//    }
//
//    result = wiced_uart_transmit_bytes( uart, write_buf, size );
//    *numwritten = size;
//
//    return ( result==WICED_SUCCESS )? SUCCESS : SERIAL_PORT_ERR;
//}

void*
rwl_open_transport(int remote_type, char *port, int ReadTotalTimeout, int debug)
{
    /* not invoked for dongle transports */
    UNUSED_PARAMETER(remote_type);
    UNUSED_PARAMETER(ReadTotalTimeout);
    UNUSED_PARAMETER(debug);

#ifdef  RWL_SOCKET
    if (wiced_tcp_accept( &tcp_client_socket ) != WICED_SUCCESS)
    {
        return NULL;
    }
#endif

    return port;
}

int
rwl_close_transport(int remote_type, void* Des)
{
    /* not invoked for dongle transports */
    UNUSED_PARAMETER(remote_type);
    UNUSED_PARAMETER(Des);
    return FAIL;
}

//int
//rwl_read_serial_port(void* hndle, char* read_buf, unsigned int data_size, unsigned int *numread)
//{
//    wiced_result_t result;
//    wiced_uart_t uart = *((wiced_uart_t*) hndle);
//
//    wiced_assert( "invalid argument", uart < WICED_UART_MAX );
//
//    if ( data_size == 0 )
//    {
//        return SUCCESS;
//    }
//
//    result = wiced_uart_receive_bytes( uart, read_buf, (uint32_t*)&data_size, WICED_NEVER_TIMEOUT );
//    *numread = data_size;
//    return ( result==WICED_SUCCESS )? SUCCESS : SERIAL_PORT_ERR;
//}

void
rwl_sleep(int delay)
{
    host_rtos_delay_milliseconds( delay );
}

void
rwl_sync_delay(unsigned int noframes)
{
    if (noframes > 1) {
        rwl_sleep(200);
    }
}

int
wl_ioctl(void *wl, int cmd, void *buf, int len, bool set)
{
    wwd_result_t ret;
    wiced_buffer_t internalPacket;
    wiced_buffer_t response_buffer;

    UNUSED_PARAMETER(wl);

    // Set Wireless Security Type
    void* ioctl_data = wwd_sdpcm_get_ioctl_buffer( &internalPacket, len );
    if (ioctl_data == NULL)
    {
        return WLAN_ENUM_OFFSET - WWD_WLAN_NOMEM;
    }
    memcpy( ioctl_data, buf, len );
    ret = wwd_sdpcm_send_ioctl( (set==TRUE)?SDPCM_SET:SDPCM_GET, cmd, internalPacket, &response_buffer, (wwd_interface_t) ifnum );
    if (ret == WWD_SUCCESS)
    {
        if (set!=TRUE)
        {
            memcpy( buf, host_buffer_get_current_piece_data_pointer( response_buffer ), MIN( host_buffer_get_current_piece_size( response_buffer ), len )  );
        }
        host_buffer_release(response_buffer, WWD_NETWORK_RX);

        return 0;
    } else if (ret <= WLAN_ENUM_OFFSET) {
        ret = WWD_WLAN_ERROR;
    }

    return WLAN_ENUM_OFFSET - ret;
}


/*
 * Function to set interface.
 */
int
set_interface(void *wl, char *intf_name)
{
    if ( sscanf( intf_name, "eth%d", &ifnum ) != 1 )
    {
        ifnum = 0;
        return -1;
    }
    return 0;
}

int
wl_cmd_exec(void *wl, cmd_t *cmd, char **argv)
{
    /* Not supported when called from server side. */
    WPRINT_APP_ERROR(("Cannot run %s via wl; please use %s command.\n", argv[0],
                      argv[0]));
    return -1;
}

/* below is to support iperf from wl command from host */
static int
deserialize_args(char *buf, int buf_len, int *argc_ret, char ***argv_ret, int additional_argc, char **additional_argv)
{
    int err              = 0;
    int argc             = 1;
    char **argv          = NULL;
    char *string_end     = strstr( buf, " " );
    char *string_start   = NULL;
    int index            = 0;
    int additional_index = 0;

    while ( string_end != NULL )
    {
        argc++;
        string_end = strstr( string_end+1, " " );
    }

    /* args mandated for function specific functionality */
    argc += additional_argc;

    argv = calloc(argc, sizeof( *argv ) );
    if (argv == NULL)
    {
        err = -1;
        goto exit;
    }

    string_end   = strstr( buf, " " );
    string_start = buf;
    index        = 0;

    do {
        if ( string_end != NULL )
        {
            *string_end = '\0';
        }

        argv[index] = calloc( strlen(string_start) + 1, sizeof(*string_start) );

        if ( argv[index] == NULL )
        {
            err = -1;
            goto exit;
        }
        strncpy( argv[index], string_start, strlen(string_start) );

        if ( string_end != NULL )
        {
            *string_end = ' ';
            string_end++;
            string_start = string_end;
            string_end = strstr( string_end, " " );
        }

        index++;
    } while ( index < argc - additional_argc );

    for ( additional_index = 0 ; additional_index < additional_argc ; additional_index++ )
    {
        argv[index++] = additional_argv[additional_index];
    }

exit:
    if ( err != 0 )
    {
        if ( argv != NULL )
        {

            for ( index = 0 ; index < argc && argv[index] != NULL; index++ )
            {
                free( argv[index] );
            }
            free( argv );
        }
        *argc_ret = 0;
        *argv_ret = NULL;

    }
    else
    {
        *argc_ret = argc;
        *argv_ret = argv;
    }
    return err;
}

/* When these commands are invoked via wl, it is not valid */
int
wl_iperf_exec(void *wl, cmd_t *cmd, char **argv)
{
    /* When this is called from the server side, then it means should really use
       * the iperf app and iperf command line directly.
       */
    WPRINT_APP_ERROR(("Cannot run iperf via wl; please use iperf command.\n"));
    return -1;
}

int
wl_wwd_join(void *wl, cmd_t *cmd, char **argv)
{
    /* When this is called from the server side, then it means should really use
       * the iperf app and iperf command line directly.
       */
    WPRINT_APP_ERROR(("Cannot run \"wl join\"; please use join command.\n"));
    return -1;
}

int
wl_wwd_ip_state(void *wl, cmd_t *cmd, char **argv)
{
    /* When this is called from the server side, then it means should really use
       * the iperf app and iperf command line directly.
       */
    WPRINT_APP_ERROR(("Cannot run \" wwd_ip_state\"; please use status command.\n"));
    return -1;
}

#if defined(MFG_TEST_ENABLE_IPERF)
static void
iperf_exec( uint32_t arg )
{
    thread_cmd_params_t *iperf_params = (thread_cmd_params_t *)arg;
    char **argv        = NULL;
    int argc           = 0;
    int index          = 0;
    /* Note: extra args added to silence print outs which seem to cause hangs */
    char *silence_argv[] = { "-x", "scdvm" };
    int err            = deserialize_args( iperf_params->buf,
        iperf_params->buf_len, &argc, &argv, 2, silence_argv );

    if ( err == 0 && argc > 1 )
    {
        (void)iperf( argc, argv );
    }

    /* last two args are NOT allocated memory so don't free see silence_argv above */
    for ( index = 0 ; index < argc - 2 ; index++ )
    {
        free( argv[index] );
    }

    free( argv );
    free( iperf_params->buf );
    free( iperf_params );

    WICED_END_OF_CURRENT_THREAD( );
}
#endif /* defined(MFG_TEST_ENABLE_IPERF) */

int start_iperf(char *buffer, int len)
{
#if defined(MFG_TEST_ENABLE_IPERF)
    int err                      = 0;
    thread_cmd_params_t *iperf_params = malloc(sizeof(*iperf_params));
    char *copy_buf               = malloc(len);

    if ( iperf_thread_is_running == WICED_TRUE )
    {
        err = wiced_rtos_delete_thread( &iperf_thread );
        iperf_thread_is_running = WICED_FALSE;
    }

    if ( strncmp( buffer, "iperf_start --kill", strlen("iperf_start --kill") ) == 0 )
    {
        return err;
    }

    if ( wiced_network_is_ip_up( WICED_STA_INTERFACE ) == WICED_FALSE &&
        wiced_network_is_ip_up( WICED_AP_INTERFACE ) == WICED_FALSE )
    {
        err = -1;
    }
    else if ( iperf_params == NULL || copy_buf == NULL )
    {
        err = -27;
    }
    else
    {
        malloc_transfer_to_curr_thread( iperf_params->buf );
        malloc_transfer_to_curr_thread( iperf_params );
        iperf_params->buf = copy_buf;
        iperf_params->buf_len = len;
        memcpy(copy_buf, buffer, iperf_params->buf_len);

        err = wiced_rtos_create_thread( &iperf_thread, WICED_DEFAULT_IPERF_PRIORITY, "iperf", iperf_exec,
            WICED_DEFAULT_IPERF_STACK, iperf_params );
    }

    /* clean up mem if thread not launched ok */
    if ( err != WICED_SUCCESS )
    {
        if ( copy_buf != NULL )
        {
            free(copy_buf);
        }

        if ( iperf_params != NULL )
        {
            free(iperf_params);
        }
    }
    else
    {
        iperf_thread_is_running = WICED_TRUE;
    }
#else
    /* not supported */
    int err = -23;
#endif /* defined(MFG_TEST_ENABLE_IPERF) */
    return err;
}

int control_audio_loopback( char *cmd )
{
#if defined(MFG_TEST_ENABLE_AUDIO_LOOPBACK)
    char arg[8];

    if ( sscanf( cmd, "audio_loopback %7s", arg ) == 1 )
    {
        if ( strcmp( arg, "start") == 0 )
        {
            return ( wiced_audio_loopback_start() == WICED_SUCCESS ) ? 0 : -1;
        }
        else if ( strcmp( arg, "stop" ) == 0 )
        {
            return ( wiced_audio_loopback_stop() == WICED_SUCCESS ) ? 0 : -1;
        }
    }
#endif /* defined(MFG_TEST_ENABLE_IPERF) */

    return -1;
}

//extern wiced_result_t wiced_ip_up( wiced_interface_t interface, wiced_network_config_t config, const wiced_ip_setting_t* ip_settings );
extern size_t strlcpy(char *dst, const char *src, size_t siz);

//static int join_mfgtest( int argc, char* argv[] )
//{
//    char*            ssid               = NULL;
//    wiced_security_t auth_type          = WICED_SECURITY_OPEN;
//    uint8_t*         security_key       = NULL;
//    uint8_t          key_length         = 0;
//    uint8_t          wep_key_buffer[64] = { 0 };
//
//    /* SSID */
//    ssid = argv[1];
//
//    /* Authtype */
//    if ( argc >= 3 )
//    {
//        auth_type = wifi_utils_str_to_authtype( argv[2] );
//    }
//
//    /* too many */
//    if ( argc > 7 )
//    {
//        return FAIL;
//    }
//
//    /* unsupported number */
//    if ( argc > 4 && argc != 7 )
//    {
//        return FAIL;
//    }
//
//    /* unknown security type */
//    if ( auth_type == WICED_SECURITY_UNKNOWN )
//    {
//        return FAIL;
//    }
//
//    if ( auth_type == WICED_SECURITY_WEP_PSK )
//    {
//        int a;
//        wiced_wep_key_t* temp_wep_key = (wiced_wep_key_t*)wep_key_buffer;
//        char temp_string[3];
//        temp_string[2] = 0;
//        key_length = strlen(argv[3])/2;
//
//        /* Setup WEP key 0 */
//        temp_wep_key[0].index = 0;
//        temp_wep_key[0].length = key_length;
//        for (a = 0; a < temp_wep_key[0].length; ++a)
//        {
//            uint32_t tmp_val;
//            memcpy(temp_string, &argv[3][a*2], 2);
//            string_to_unsigned( temp_string, 2, &tmp_val, 1 );
//            temp_wep_key[0].data[a] = (uint8_t) tmp_val;
//        }
//
//        /* Setup WEP keys 1 to 3 */
//        memcpy(wep_key_buffer + 1*(2 + key_length), temp_wep_key, (2 + key_length));
//        memcpy(wep_key_buffer + 2*(2 + key_length), temp_wep_key, (2 + key_length));
//        memcpy(wep_key_buffer + 3*(2 + key_length), temp_wep_key, (2 + key_length));
//        wep_key_buffer[1*(2 + key_length)] = 1;
//        wep_key_buffer[2*(2 + key_length)] = 2;
//        wep_key_buffer[3*(2 + key_length)] = 3;
//
//        security_key = wep_key_buffer;
//        key_length = 4*(2 + key_length);
//    }
//    else if ( ( auth_type != WICED_SECURITY_OPEN ) && ( argc < 4 ) )
//    {
//        /* invalid security type */
//        return FAIL;
//    }
//    else if ( ( auth_type != WICED_SECURITY_OPEN ) )
//    {
//        /* Ensure valid key and key length, as security is not Open */
//        if ( argc >= 4 )
//        {
//            security_key = (uint8_t*)argv[3];
//            key_length = strlen((char*)security_key);
//        }
//        else
//        {
//            return FAIL;
//        }
//    }
//
//    /* IP addresses are being specified */
//    if ( argc == 7 )
//    {
//        /* argv[4] is the IPv4 Address, argv[5] is the Gateway, and argv[6] is the Netmask */
//        return wifi_utils_join( ssid, strlen(ssid), auth_type, (uint8_t*) security_key, key_length, argv[4], argv[5], argv[6]);
//    }
//    else
//    {
//        return wifi_utils_join( ssid, strlen(ssid), auth_type, (uint8_t*) security_key, key_length, NULL, NULL, NULL );
//    }
//}

//static void wlu_network_create_thread( uint32_t param )
//{
//    char **argv = NULL;
//    int argc    = 0;
//    int index   = 0;
//    thread_cmd_params_t *cmd = (thread_cmd_params_t*)param;
//    int result = deserialize_args( cmd->buf, cmd->buf_len, &argc, &argv, 0, NULL );
//
//    result = join_mfgtest( argc, argv );
//
//    if ( result != 0 )
//    {
//        goto exit;
//    }
//
//exit:
//    /* clean up all used memory */
//    if ( argv != NULL )
//    {
//        for ( index = 0 ; index < argc ; index++ )
//        {
//            free( argv[index] );
//        }
//
//        free( argv );
//    }
//    free( cmd->buf );
//    free( cmd );
//    WICED_END_OF_CURRENT_THREAD( );
//}

int wiced_wlu_ip_details( char *buf_ptr, int buf_len )
{
    int result  = 0;
    int count   = 0;
//    wiced_ip_address_t ipv4_address;
//    wiced_ip_address_t netmask;
//    wiced_ip_address_t gateway;
//
//    /* This could be extended to other interfaces if desired */
//    wiced_interface_t interface = WICED_STA_INTERFACE;
//
//    if ( wiced_network_is_ip_up( interface ) )
//    {
//        wiced_ip_get_netmask( interface, &netmask );
//        wiced_ip_get_gateway_address( interface, &gateway );
//        wiced_ip_get_ipv4_address( interface, &ipv4_address );
//        count = snprintf( buf_ptr, buf_len,
//            "   IP Addr     : %u.%u.%u.%u\n", (uint8_t)(GET_IPV4_ADDRESS(ipv4_address) >> 24),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(ipv4_address) >> 16),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(ipv4_address) >> 8),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(ipv4_address) >> 0) );
//        if ( count < 0 )
//        {
//            result = FAIL;
//            goto exit;
//        }
//
//        buf_ptr += count;
//        buf_len -= count;
//
//        count = snprintf( buf_ptr, buf_len,
//            "   Gateway     : %u.%u.%u.%u\n", (uint8_t)(GET_IPV4_ADDRESS(gateway) >> 24),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(gateway) >> 16),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(gateway) >> 8),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(gateway) >> 0) );
//        if ( count < 0 )
//        {
//            result = FAIL;
//            goto exit;
//        }
//
//        buf_ptr += count;
//        buf_len -= count;
//
//        count = snprintf( buf_ptr, buf_len,
//            "   Netmask     : %u.%u.%u.%u\n", (uint8_t)(GET_IPV4_ADDRESS(netmask) >> 24),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(netmask) >> 16),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(netmask) >> 8),
//                                                             (uint8_t)(GET_IPV4_ADDRESS(netmask) >> 0) );
//        if ( count < 0 )
//        {
//            result = FAIL;
//            goto exit;
//        }
//    }
//    else
//    {
//        snprintf( buf_ptr + count, buf_len - count, "IP not up (interface: STA)\n");
//    }
exit:
    return result;
}

//static wiced_thread_t wireless_network_start_thread;
//static wiced_bool_t   wireless_network_start_thread_running = WICED_FALSE;
//
//int wiced_wlu_wireless_network_start( char *buf_ptr, int buf_len )
//{
//    thread_cmd_params_t *cmd = malloc( sizeof( *cmd ) );
//    int result               = 0;
//
//    if ( wireless_network_start_thread_running == WICED_TRUE )
//    {
//        result = wiced_rtos_delete_thread( &wireless_network_start_thread );
//        wireless_network_start_thread_running = WICED_FALSE;
//    }
//
//    /* allocate memory to pass into the thread: network creation takes a long time */
//    if ( cmd == NULL )
//    {
//        result = FAIL;
//        goto exit;
//    }
//
//    cmd->buf = malloc( buf_len );
//
//    if ( cmd->buf == NULL )
//    {
//        result = FAIL;
//        goto exit;
//    }
//
//    memcpy( cmd->buf, buf_ptr, buf_len );
//    cmd->buf_len = buf_len;
//
//    /* take ownership of memory so the thread can access and free it later */
//    malloc_transfer_to_curr_thread( cmd->buf );
//    malloc_transfer_to_curr_thread( cmd );
//
//    /* do this on separate thread, as it is very time consuming */
//    result = wiced_rtos_create_thread(&wireless_network_start_thread, WICED_DEFAULT_NET_CREATE_PRIORITY, "wlu_create_wifi_network", wlu_network_create_thread, WLU_SERVER_STACK_SIZE, cmd );
//exit:
//    if ( result != 0 && cmd != NULL )
//    {
//        if ( cmd->buf != NULL )
//        {
//            free( cmd->buf );
//        }
//
//        free( cmd );
//    }
//    else
//    {
//        wireless_network_start_thread_running = WICED_TRUE;
//    }
//
//    return result;
//}


#if 0
static void downloading_init_func( uint32_t arg )
{
    wiced_init( );
    WICED_END_OF_CURRENT_THREAD_NO_LEAK_CHECK( );
}

void start_download( void )
{
    wiced_deinit( );
    currently_downloading = 1;

    host_rtos_init_semaphore( &downloading_semaphore );
    host_rtos_init_semaphore( &download_ready_semaphore );

    wiced_rtos_create_thread( &downloading_thread, WICED_NETWORK_WORKER_PRIORITY, "downloading_init_func", downloading_init_func, 5000, NULL);
    host_rtos_get_semaphore( &download_ready_semaphore, NEVER_TIMEOUT, WICED_FALSE );
    host_rtos_deinit_semaphore( &download_ready_semaphore );
}

void finish_download( void )
{
    host_rtos_set_semaphore( &downloading_semaphore, WICED_FALSE );
    host_rtos_deinit_semaphore( &downloading_semaphore );
    currently_downloading = 0;
}

void membytes_write( uint32_t address, uint8_t* buf, uint32_t length )
{
    wwd_bus_transfer_backplane_bytes( BUS_WRITE, address, length, buf );
}

wwd_result_t external_write_wifi_firmware_and_nvram_image( void )
{
    if ( currently_downloading == 0 )
    {
        wwd_result_t result;
        result = wwd_bus_write_wifi_firmware_image( );
        if ( result != WWD_SUCCESS )
        {
            return result;
        }
        return wwd_bus_write_wifi_nvram_image( );
    }

    host_rtos_set_semaphore( &download_ready_semaphore, WICED_FALSE );
    host_rtos_get_semaphore( &downloading_semaphore, NEVER_TIMEOUT, WICED_FALSE );
    return WWD_SUCCESS;
}

wiced_result_t wiced_wlu_server_serial_start( wiced_uart_t uart_id )
{
    wiced_assert("wlu_server already started",
            (wlu_server.started == WICED_FALSE));

    wlu_server.started = WICED_TRUE;
    wlu_server.uart_id = uart_id;

    WWD_WLAN_KEEP_AWAKE( );

    if ( uart_id != STDIO_UART )
    {
        static const platform_uart_config_t uart_config =
        {
            .baud_rate    = 115200,
            .data_width   = DATA_WIDTH_8BIT,
            .parity       = NO_PARITY,
            .stop_bits    = STOP_BITS_1,
            .flow_control = FLOW_CONTROL_DISABLED,
        };

        WICED_VERIFY( wiced_uart_init( uart_id, &uart_config, NULL ) );
    }
    /* Start wlu server thread */
    wiced_rtos_create_thread(&wlu_server.thread, WLU_SERVER_THREAD_PRIORITY, "wlu_server", wlu_server_thread, WLU_SERVER_STACK_SIZE, &wlu_server);
    return WICED_SUCCESS;
}

wiced_result_t wiced_wlu_server_serial_stop( void )
{
    if ( wiced_rtos_is_current_thread( &wlu_server.thread ) != WICED_SUCCESS )
    {
        wiced_rtos_thread_force_awake( &wlu_server.thread );
        wiced_rtos_thread_join( &wlu_server.thread );
        wiced_rtos_delete_thread( &wlu_server.thread );
    }
    WWD_WLAN_LET_SLEEP( );
    wlu_server.started = WICED_FALSE;
    return WICED_SUCCESS;
}

static void wlu_server_thread( uint32_t thread_input )
{
    wiced_wlu_server_t* wlu = (wiced_wlu_server_t*) thread_input;
    int argc = 2;
    char *argv[] = { (char*)wlu->uart_id, "" };
#ifdef  RWL_SOCKET
    char *arge[] = { (char *)wlu->eth_port, "" };
#endif


    wiced_assert( "invalid argument", wlu->uart_id < WICED_UART_MAX );

    UNUSED_PARAMETER(thread_input);

#ifdef  RWL_SOCKET
    if (wlu->eth_started ==  WICED_TRUE)
#endif
    {
#ifdef  RWL_SOCKET
        remote_server_exec( argc, (wlu->eth_started ==  WICED_TRUE)?arge:argv, NULL );
#else
        remote_server_exec(argc, argv, NULL);
#endif


    }
}

#endif //0

#ifdef  RWL_SOCKET
wiced_result_t wiced_wlu_server_eth_start( int tcp_server_port , char** argv)
{
    wiced_result_t result;
    UNUSED_PARAMETER(argv);
    wiced_assert("wlu_server already started", (wlu_server.started == WICED_FALSE));

    wlu_server.started                  = WICED_TRUE;
    wlu_server.eth_started              = WICED_TRUE;
    wlu_server.eth_port                 = tcp_server_port;



    // WWD_WLAN_KEEP_AWAKE( );

    /* Bring up the network interface */
    result = wiced_network_up( WICED_ETHERNET_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );

    if ( result != WICED_SUCCESS )
    {
        WPRINT_APP_INFO(("Failed to connect to Ethernet.\n"));
    }

    /* Create a TCP socket */
    if ( wiced_tcp_create_socket( &tcp_client_socket, WICED_ETHERNET_INTERFACE ) != WICED_SUCCESS )
    {
        WPRINT_APP_INFO( ("TCP socket creation failed\n") );
    }
    if (wiced_tcp_listen( &tcp_client_socket, tcp_server_port ) != WICED_SUCCESS)
    {
        WPRINT_APP_INFO(("TCP server socket initialization failed\n"));
        wiced_tcp_delete_socket(&tcp_client_socket);
        return WICED_ERROR;
    }

    wiced_rtos_create_thread( &wlu_server.thread, WLU_SERVER_THREAD_PRIORITY, "wlu_eth_server", wlu_server_thread, WLU_SERVER_STACK_SIZE, &wlu_server );

    return WICED_SUCCESS;
}

int
rwl_read_eth_port(void* hndle, char* read_buf, unsigned int data_size, unsigned int *numread)
{
    wiced_result_t  result = WICED_SUCCESS;
    char*           request;
    uint16_t        offset;
    uint16_t        request_length;
    uint16_t        available_data_length;
    uint16_t        copy_size;
    wiced_packet_t* temp_packet = NULL;

    *numread = 0;

    if ( data_size == 0 )
    {
        return SUCCESS;
    }

    if (wiced_tcp_receive( &tcp_client_socket, &temp_packet, WICED_WAIT_FOREVER ) == WICED_SUCCESS)
    {
        result = result;

        if (temp_packet)
        {
            /* Process the client request */
            offset = 0;
            do
            {
                wiced_packet_get_data( temp_packet, offset, (uint8_t **)&request, &request_length, &available_data_length );

                copy_size = MIN( data_size, request_length );
                memcpy(read_buf, request, copy_size);

                read_buf += copy_size;
                *numread += copy_size;
                data_size -= copy_size;

                offset += copy_size;
            } while (request_length < available_data_length);

            wiced_packet_delete( temp_packet );
        }
    }

    return SUCCESS;
}

int
rwl_write_eth_port(void* hndle, char* write_buf, unsigned long size, unsigned long *numwritten)
{
    uint16_t        available_data_length;
    wiced_packet_t* tx_packet;
    char*           tx_data;
    unsigned long temp_size = 0;
    unsigned long temp=0;

    if ( size == 0 )
     {
         return SUCCESS;
     }

    while (size > 0)
    {
    if (wiced_packet_create_tcp(&tcp_client_socket, 128, &tx_packet, (uint8_t **)&tx_data, &available_data_length) != WICED_SUCCESS)
      {
          WPRINT_APP_INFO(("TCP packet creation failed\n"));
          return WICED_ERROR;
      }
    if (size > available_data_length)
        {
        temp_size = available_data_length;
        }
    else
    {
        temp_size =size;
    }

       /*  Write the message into tx_data"*/
        memcpy(tx_data, (write_buf + temp), temp_size);

        wiced_packet_set_data_end( tx_packet, (uint8_t*)tx_data + temp_size );

        /* Send the TCP packet*/
        if ( wiced_tcp_send_packet( &tcp_client_socket, tx_packet ) != WICED_SUCCESS )
        {
            WPRINT_APP_INFO(("TCP packet send failed \n"));

             /*Delete packet, since the send failed*/
            wiced_packet_delete(tx_packet);
            return WICED_ERROR;
        }

        wiced_packet_delete(tx_packet);
        size = size - temp_size;
        temp = temp+temp_size;

     }

    return available_data_length;
}
#endif
