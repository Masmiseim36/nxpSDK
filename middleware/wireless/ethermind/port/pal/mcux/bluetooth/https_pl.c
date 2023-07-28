
/**
 *  \file https_pl.c
 *
 *  This File contains the platform abstraction routines of LE HTTP Proxy
 *  Service to Transmit and Receive Data over TCP/IP protocol.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "https_pl.h"

#include "lwip/opt.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "fsl_debug_console.h"
#include <stdlib.h>
#include <stdio.h>


#include "lwip/opt.h"

#include "mbedtls/entropy.h"
#include "mbedtls/hmac_drbg.h"
#include "mbedtls/certs.h"
#include "mbedtls/x509.h"
#include "mbedtls/ssl.h"
#include "mbedtls/ssl_cache.h"
#include "mbedtls/debug.h"

#if defined(MBEDTLS_MCUX_ELS_PKC_API)
#include "els_pkc_mbedtls.h"
#else
#include "ksdk_mbedtls.h"
#endif

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* This is the value used for ssl read timeout */
#define IOT_SSL_READ_TIMEOUT 10U

#define DEBUG_LEVEL 0U

/* Maximum number of bytes to read from TCP Socket port */
/**
 * NOTE: The Buffer size is kept to 2K as the Characteristic Value
 * Size for both Headers and Entity body is only around 512 each.
 * Keep the extra as cushion, which can be trimmed if needed.
 */
#define HTTPS_BUF_SIZE_PL                     2048U

/* Global HTTPS Port Definition */
#define HTTPS_PORT                            443U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/** HTTPS Tranport Read Task */
static BT_THREAD_RETURN_TYPE https_read_task_pl (BT_THREAD_ARGS args);

extern void lwip_ping_netif_init(void);
extern void lwip_ping_dhcp(void);

typedef struct _TLSDataParams
{
    mbedtls_entropy_context entropy;
    mbedtls_hmac_drbg_context hmac_drbg;
    mbedtls_ssl_context ssl;
    mbedtls_ssl_config conf;
    uint32_t flags;
    mbedtls_x509_crt cacert;
    mbedtls_x509_crt clicert;
    mbedtls_pk_context pkey;

    /* lwIP socket file descriptor */
    int fd;

} TLSDataParams;

/*! \public
 * @brief IoT Error enum
 *
 * Enumeration of return values from the IoT_* functions within the SDK.
 * Values less than -1 are specific error codes
 * Value of -1 is a generic failure response
 * Value of 0 is a generic success response
 * Values greater than 0 are specific non-error return codes
 */
typedef enum
{
    /** Returned when the Network physical layer is connected */
    NETWORK_PHYSICAL_LAYER_CONNECTED = 6,
    /** Returned when the Network is manually disconnected */
    NETWORK_MANUALLY_DISCONNECTED = 5,
    /** Returned when the Network is disconnected and the reconnect attempt is in progress */
    NETWORK_ATTEMPTING_RECONNECT = 4,
    /** Return value of yield function to indicate auto-reconnect was successful */
    NETWORK_RECONNECTED = 3,
    /** Returned when a read attempt is made on the TLS buffer and it is empty */
    MQTT_NOTHING_TO_READ = 2,
    /** Returned when a connection request is successful and packet response is connection accepted */
    MQTT_CONNACK_CONNECTION_ACCEPTED = 1,
    /** Success return value - no error occurred */
    SUCCESS = 0,
    /** A generic error. Not enough information for a specific error code */
    FAILURE = -1,
    /** A required parameter was passed as null */
    NULL_VALUE_ERROR = -2,
    /** The TCP socket could not be established */
    TCP_CONNECTION_ERROR = -3,
    /** The TLS handshake failed */
    SSL_CONNECTION_ERROR = -4,
    /** Error associated with setting up the parameters of a Socket */
    TCP_SETUP_ERROR = -5,
    /** A timeout occurred while waiting for the TLS handshake to complete. */
    NETWORK_SSL_CONNECT_TIMEOUT_ERROR = -6,
    /** A Generic write error based on the platform used */
    NETWORK_SSL_WRITE_ERROR = -7,
    /** SSL initialization error at the TLS layer */
    NETWORK_SSL_INIT_ERROR = -8,
    /** An error occurred when loading the certificates.  The certificates could not be located or are incorrectly
       formatted. */
    NETWORK_SSL_CERT_ERROR = -9,
    /** SSL Write times out */
    NETWORK_SSL_WRITE_TIMEOUT_ERROR = -10,
    /** SSL Read times out */
    NETWORK_SSL_READ_TIMEOUT_ERROR = -11,
    /** A Generic error based on the platform used */
    NETWORK_SSL_READ_ERROR = -12,
    /** Returned when the Network is disconnected and reconnect is either disabled or physical layer is disconnected */
    NETWORK_DISCONNECTED_ERROR = -13,
    /** Returned when the Network is disconnected and the reconnect attempt has timed out */
    NETWORK_RECONNECT_TIMED_OUT_ERROR = -14,
    /** Returned when the Network is already connected and a connection attempt is made */
    NETWORK_ALREADY_CONNECTED_ERROR = -15,
    /** Network layer Error Codes */
    /** Network layer Random number generator seeding failed */
    NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED = -16,
    /** A generic error code for Network layer errors */
    NETWORK_SSL_UNKNOWN_ERROR = -17,
    /** Returned when the physical layer is disconnected */
    NETWORK_PHYSICAL_LAYER_DISCONNECTED = -18,
    /** Returned when the root certificate is invalid */
    NETWORK_X509_ROOT_CRT_PARSE_ERROR = -19,
    /** Returned when the device certificate is invalid */
    NETWORK_X509_DEVICE_CRT_PARSE_ERROR = -20,
    /** Returned when the private key failed to parse */
    NETWORK_PK_PRIVATE_KEY_PARSE_ERROR = -21,
    /** Returned when the network layer failed to open a socket */
    NETWORK_ERR_NET_SOCKET_FAILED = -22,
    /** Returned when the server is unknown */
    NETWORK_ERR_NET_UNKNOWN_HOST = -23,
    /** Returned when connect request failed */
    NETWORK_ERR_NET_CONNECT_FAILED = -24,
    /** Returned when there is nothing to read in the TLS read buffer */
    NETWORK_SSL_NOTHING_TO_READ = -25,
    /** A connection could not be established. */
    MQTT_CONNECTION_ERROR = -26,
    /** A timeout occurred while waiting for the TLS handshake to complete */
    MQTT_CONNECT_TIMEOUT_ERROR = -27,
    /** A timeout occurred while waiting for the TLS request complete */
    MQTT_REQUEST_TIMEOUT_ERROR = -28,
    /** The current client state does not match the expected value */
    MQTT_UNEXPECTED_CLIENT_STATE_ERROR = -29,
    /** The client state is not idle when request is being made */
    MQTT_CLIENT_NOT_IDLE_ERROR = -30,
    /** The MQTT RX buffer received corrupt or unexpected message  */
    MQTT_RX_MESSAGE_PACKET_TYPE_INVALID_ERROR = -31,
    /** The MQTT RX buffer received a bigger message. The message will be dropped  */
    MQTT_RX_BUFFER_TOO_SHORT_ERROR = -32,
    /** The MQTT TX buffer is too short for the outgoing message. Request will fail  */
    MQTT_TX_BUFFER_TOO_SHORT_ERROR = -33,
    /** The client is subscribed to the maximum possible number of subscriptions  */
    MQTT_MAX_SUBSCRIPTIONS_REACHED_ERROR = -34,
    /** Failed to decode the remaining packet length on incoming packet */
    MQTT_DECODE_REMAINING_LENGTH_ERROR = -35,
    /** Connect request failed with the server returning an unknown error */
    MQTT_CONNACK_UNKNOWN_ERROR = -36,
    /** Connect request failed with the server returning an unacceptable protocol version error */
    MQTT_CONNACK_UNACCEPTABLE_PROTOCOL_VERSION_ERROR = -37,
    /** Connect request failed with the server returning an identifier rejected error */
    MQTT_CONNACK_IDENTIFIER_REJECTED_ERROR = -38,
    /** Connect request failed with the server returning an unavailable error */
    MQTT_CONNACK_SERVER_UNAVAILABLE_ERROR = -39,
    /** Connect request failed with the server returning a bad userdata error */
    MQTT_CONNACK_BAD_USERDATA_ERROR = -40,
    /** Connect request failed with the server failing to authenticate the request */
    MQTT_CONNACK_NOT_AUTHORIZED_ERROR = -41,
    /** An error occurred while parsing the JSON string.  Usually malformed JSON. */
    JSON_PARSE_ERROR = -42,
    /** Shadow: The response Ack table is currently full waiting for previously published updates */
    SHADOW_WAIT_FOR_PUBLISH = -43,
    /** Any time an snprintf writes more than size value, this error will be returned */
    SHADOW_JSON_BUFFER_TRUNCATED = -44,
    /** Any time an snprintf encounters an encoding error or not enough space in the given buffer */
    SHADOW_JSON_ERROR = -45,
    /** Mutex initialization failed */
    MUTEX_INIT_ERROR = -46,
    /** Mutex lock request failed */
    MUTEX_LOCK_ERROR = -47,
    /** Mutex unlock request failed */
    MUTEX_UNLOCK_ERROR = -48,
    /** Mutex destroy failed */
    MUTEX_DESTROY_ERROR = -49,
} Error_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
TLSDataParams tlsDataParams;
//const char *HTTPS_SERVER_NAME = "os.mbed.com";
const char *HTTPS_SERVER_NAME = "httpbin.org";
const char *HTTPS_SERVER_PORT = "443";
unsigned char https_buf[1024U];

/** HPS - TCP Socket Peer Address */
static struct sockaddr_in peer_addr;

/** HPS - TCP Socket Read Task State */
static UCHAR https_transport_state_pl;

/* Socket Read Task Synchronization */
BT_DEFINE_MUTEX(https_sock_mutex)
BT_DEFINE_COND(https_sock_cond)

/* HTTPS Init/Connect Task Sync */
static volatile API_RESULT https_connect_ret;
static UCHAR * https_connect_ip_addr;
OSA_EVENT_HANDLE_DEFINE(https_event_handle);
#define HTTPS_EVENT_INIT_REQ        (1U<<0U)
#define HTTPS_EVENT_INIT_COM        (1U<<1U)
#define HTTPS_EVENT_CONNECT_REQ     (1U<<2U)
#define HTTPS_EVENT_CONNECT_COM     (1U<<3U)

/** Buffer to Read the Incoming Data */
static UCHAR https_sock_read_buf [HTTPS_BUF_SIZE_PL];

static HTTPS_TRANSPORT_CB_PL https_appl_cb;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Send function used by mbedtls ssl */
static int lwipSend(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_send((*(int *)fd), buf, len, 0U);
}

/* Send function used by mbedtls ssl */
static int lwipRecv(void *fd, unsigned char const *buf, size_t len)
{
    return lwip_recv((*(int *)fd), (void *)buf, len, 0U);
}

static int _iot_tls_verify_cert(void *data, mbedtls_x509_crt *crt, int depth, uint32_t *flags)
{
    char buf[1024U];
    ((void)data);

    HTTPS_PL_TRC("\nVerify requested for (Depth %d):\n", depth);
    (BT_IGNORE_RETURN_VALUE) mbedtls_x509_crt_info(buf, sizeof(buf) - 1U, "", crt);
    HTTPS_PL_TRC("%s", buf);

    if ((*flags) == 0U)
    {
        HTTPS_PL_TRC("  This certificate has no flags\n");
    }
    else
    {
        HTTPS_PL_TRC(buf, sizeof(buf), "  ! ", *flags);
        HTTPS_PL_TRC("%s\n", buf);
    }

    return 0U;
}

#ifdef MBEDTLS_DEBUG_C
static void my_debug(void *ctx, int level, const char *file, int line, const char *str)
{
    ((void)level);

    HTTPS_PL_TRC("\r\n%s, at line %d in file %s\n", str, line, file);
}
#endif

static int https_init(void)
{
    int ret          = 0U;
    const char *pers = "aws_iot_tls_wrapper";
    const mbedtls_md_info_t *md_info;

    /* Crypto init */
    CRYPTO_InitHardware();

    mbedtls_hmac_drbg_init(&(tlsDataParams.hmac_drbg));
    mbedtls_x509_crt_init(&(tlsDataParams.cacert));
    mbedtls_x509_crt_init(&(tlsDataParams.clicert));
    mbedtls_pk_init(&(tlsDataParams.pkey));

#if defined(MBEDTLS_DEBUG_C)
    /* Enable debug output of mbedtls */
    mbedtls_ssl_conf_dbg(&(tlsDataParams.conf), my_debug, NULL);
    mbedtls_debug_set_threshold(DEBUG_LEVEL);
#endif

    HTTPS_PL_TRC("\n  . Seeding the random number generator...");
    mbedtls_entropy_init(&(tlsDataParams.entropy));
    md_info = mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);
    if ((ret = mbedtls_hmac_drbg_seed(&(tlsDataParams.hmac_drbg), md_info, mbedtls_entropy_func,
                                      &(tlsDataParams.entropy), (const unsigned char *)pers, strlen(pers))) != 0U)
    {
        HTTPS_PL_ERR(" failed\n  ! mbedtls_hmac_drbg_seed returned -0x%x\n", -ret);
        return NETWORK_MBEDTLS_ERR_CTR_DRBG_ENTROPY_SOURCE_FAILED;
    }

    HTTPS_PL_TRC("  . Loading the CA root certificate ...");
    ret = mbedtls_x509_crt_parse(&(tlsDataParams.cacert), (const unsigned char *)mbedtls_test_ca_crt,
                                 mbedtls_test_ca_crt_len);
    if (ret < 0)
    {
        HTTPS_PL_ERR(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x while parsing root cert\n\n", -ret);
        return NETWORK_X509_ROOT_CRT_PARSE_ERROR;
    }
    HTTPS_PL_TRC(" ok (%d skipped)\n", ret);

    HTTPS_PL_TRC("  . Loading the client cert. and key...");
    ret = mbedtls_x509_crt_parse(&(tlsDataParams.clicert), (const unsigned char *)mbedtls_test_cli_crt,
                                 mbedtls_test_cli_crt_len);
    if (ret != 0U)
    {
        HTTPS_PL_ERR(" failed\n  !  mbedtls_x509_crt_parse returned -0x%x while parsing device cert\n\n", -ret);
        return NETWORK_X509_DEVICE_CRT_PARSE_ERROR;
    }

    ret = mbedtls_pk_parse_key(&(tlsDataParams.pkey), (const unsigned char *)mbedtls_test_cli_key,
                               mbedtls_test_cli_key_len, NULL, 0U);
    if (ret != 0U)
    {
        HTTPS_PL_ERR(" failed\n  !  mbedtls_pk_parse_key returned -0x%x while parsing private key\n\n", -ret);
        return NETWORK_PK_PRIVATE_KEY_PARSE_ERROR;
    }
    HTTPS_PL_TRC(" ok\n");

    return SUCCESS;
}

static int https_connect(UCHAR * ip_addr)
{
    char vrfy_buf[512U];
    bool ServerVerificationFlag = false;
    int ret          = 0U;
    INT32 hc_port;

#ifdef MBEDTLS_DEBUG_C
    unsigned char buf[MBEDTLS_SSL_MAX_CONTENT_LEN + 1U];
#endif

    struct addrinfo hints;
    (BT_IGNORE_RETURN_VALUE) memset(&hints, 0U, sizeof(struct addrinfo));
    /* MISRA C - 2012 Rule 2.2 */
#if 0
    hints.ai_family   = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE;
#endif /* 0 */

    mbedtls_ssl_init(&(tlsDataParams.ssl));
    mbedtls_ssl_config_init(&(tlsDataParams.conf));

    tlsDataParams.fd = socket(AF_INET, SOCK_STREAM, 0U);
    if (tlsDataParams.fd < 0)
    {
        return NETWORK_ERR_NET_SOCKET_FAILED;
    }

    /* Prepare Socket Address */
    hc_port = HTTPS_PORT;

    BT_mem_set (&peer_addr, 0, sizeof (peer_addr));
    peer_addr.sin_family      = AF_INET;
    peer_addr.sin_addr.s_addr = inet_addr ((const char *)ip_addr);
    peer_addr.sin_port        = htons((UINT16)hc_port);

    ret = connect
        (
            tlsDataParams.fd,
            (struct sockaddr *) &peer_addr,
            sizeof (peer_addr)
        );

    if (ret != 0U)
    {
        (BT_IGNORE_RETURN_VALUE) close(tlsDataParams.fd);
        return NETWORK_ERR_NET_CONNECT_FAILED;
    }

    HTTPS_PL_TRC("  . Setting up the SSL/TLS structure...");
    if ((ret = mbedtls_ssl_config_defaults(&(tlsDataParams.conf), MBEDTLS_SSL_IS_CLIENT, MBEDTLS_SSL_TRANSPORT_STREAM,
                                           MBEDTLS_SSL_PRESET_DEFAULT)) != 0U)
    {
        HTTPS_PL_ERR(" failed\n  ! mbedtls_ssl_config_defaults returned -0x%x\n\n", -ret);
        return SSL_CONNECTION_ERROR;
    }

    mbedtls_ssl_conf_verify(&(tlsDataParams.conf), _iot_tls_verify_cert, NULL);
#if 0
    if (ServerVerificationFlag == true)
    {
        mbedtls_ssl_conf_authmode(&(tlsDataParams.conf), MBEDTLS_SSL_VERIFY_REQUIRED);
    }
    else
    {
#endif
    mbedtls_ssl_conf_authmode(&(tlsDataParams.conf), MBEDTLS_SSL_VERIFY_OPTIONAL);
#if 0
    }
#endif
    mbedtls_ssl_conf_rng(&(tlsDataParams.conf), mbedtls_hmac_drbg_random, &(tlsDataParams.hmac_drbg));

    mbedtls_ssl_conf_ca_chain(&(tlsDataParams.conf), &(tlsDataParams.cacert), NULL);
    if ((ret = mbedtls_ssl_conf_own_cert(&(tlsDataParams.conf), &(tlsDataParams.clicert), &(tlsDataParams.pkey))) != 0U)
    {
        HTTPS_PL_ERR(" failed\n  ! mbedtls_ssl_conf_own_cert returned %d\n\n", ret);
        return SSL_CONNECTION_ERROR;
    }

    if ((ret = mbedtls_ssl_setup(&(tlsDataParams.ssl), &(tlsDataParams.conf))) != 0U)
    {
        HTTPS_PL_ERR(" failed\n  ! mbedtls_ssl_setup returned -0x%x\n\n", -ret);
        return SSL_CONNECTION_ERROR;
    }
#if 0
    if ((ret = mbedtls_ssl_set_hostname(&(tlsDataParams.ssl), server_name)) != 0)
#else
    if ((ret = mbedtls_ssl_set_hostname(&(tlsDataParams.ssl), NULL)) != 0U)
#endif
    {
        HTTPS_PL_ERR(" failed\n  ! mbedtls_ssl_set_hostname returned %d\n\n", ret);
        return SSL_CONNECTION_ERROR;
    }
    HTTPS_PL_TRC("\n\nSSL state connect : %d ", tlsDataParams.ssl.state);

    mbedtls_ssl_set_bio(&(tlsDataParams.ssl), &(tlsDataParams.fd), lwipSend, (mbedtls_ssl_recv_t *)lwipRecv, NULL);

    HTTPS_PL_TRC(" ok\n");
    HTTPS_PL_TRC("\n\nSSL state connect : %d ", tlsDataParams.ssl.state);
    HTTPS_PL_TRC("  . Performing the SSL/TLS handshake...");
    while ((ret = mbedtls_ssl_handshake(&(tlsDataParams.ssl))) != 0U)
    {
        if (ret != MBEDTLS_ERR_SSL_WANT_READ && ret != MBEDTLS_ERR_SSL_WANT_WRITE)
        {
            HTTPS_PL_ERR(" failed\n  ! mbedtls_ssl_handshake returned -0x%x\n", -ret);
            if (ret == MBEDTLS_ERR_X509_CERT_VERIFY_FAILED)
            {
                HTTPS_PL_ERR(
                    "    Unable to verify the server's certificate. "
                    "    Alternatively, you may want to use "
                    "auth_mode=optional for testing purposes.\n");
            }
            return SSL_CONNECTION_ERROR;
        }
    }

    HTTPS_PL_TRC(" ok\n    [ Protocol is %s ]\n    [ Ciphersuite is %s ]\n", mbedtls_ssl_get_version(&(tlsDataParams.ssl)),
           mbedtls_ssl_get_ciphersuite(&(tlsDataParams.ssl)));
    if ((ret = mbedtls_ssl_get_record_expansion(&(tlsDataParams.ssl))) >= 0)
    {
        HTTPS_PL_TRC("    [ Record expansion is %d ]\n", ret);
    }
    else
    {
        HTTPS_PL_ERR("    [ Record expansion is unknown (compression) ]\n");
    }

    HTTPS_PL_TRC("  . Verifying peer X.509 certificate...");

    if (ServerVerificationFlag == true)
    {
	 /**
         * Note:
         * Below block will not be executed, as ServerVerificationFlag is initialized to false.
         */
        if ((tlsDataParams.flags = mbedtls_ssl_get_verify_result(&(tlsDataParams.ssl))) != 0U)
        {
            HTTPS_PL_ERR(" failed\n");
            (BT_IGNORE_RETURN_VALUE) mbedtls_x509_crt_verify_info(vrfy_buf, sizeof(vrfy_buf), "  ! ", tlsDataParams.flags);
            HTTPS_PL_ERR("%s\n", vrfy_buf);
            /* MISRA C - 2012 Rule 2.2 */
            /* ret = SSL_CONNECTION_ERROR; */
        }
        else
        {
            HTTPS_PL_TRC(" ok\n");
            /* MISRA C - 2012 Rule 2.2 */
            /* ret = SUCCESS; */
        }
    }
    else
    {
        HTTPS_PL_TRC(" Server Verification skipped\n");
        /* MISRA C - 2012 Rule 2.2 */
        /* ret = SUCCESS; */
    }

#ifdef MBEDTLS_DEBUG_C
    if (mbedtls_ssl_get_peer_cert(&(tlsDataParams.ssl)) != NULL)
    {
        HTTPS_PL_TRC("  . Peer certificate information    ...\n");
        mbedtls_x509_crt_info((char *)buf, sizeof(buf) - 1U, "      ", mbedtls_ssl_get_peer_cert(&(tlsDataParams.ssl)));
        HTTPS_PL_TRC("%s\n", buf);
    }
#endif

    return SUCCESS;
}

static int https_disconnect(void)
{
    (BT_IGNORE_RETURN_VALUE) close(tlsDataParams.fd);
    mbedtls_ssl_free(&(tlsDataParams.ssl));
    mbedtls_ssl_config_free(&(tlsDataParams.conf));

    return SUCCESS;
}

static int https_deinit(void)
{
    mbedtls_x509_crt_free(&(tlsDataParams.clicert));
    mbedtls_x509_crt_free(&(tlsDataParams.cacert));
    mbedtls_pk_free(&(tlsDataParams.pkey));
    mbedtls_hmac_drbg_free(&(tlsDataParams.hmac_drbg));
    mbedtls_entropy_free(&(tlsDataParams.entropy));

    return SUCCESS;
}

/** HTTPS Transport Read Task */
static BT_THREAD_RETURN_TYPE https_read_task_pl (BT_THREAD_ARGS args)
{
    int                nbytes;
    UCHAR              send_data;
    DECL_STATIC UINT32 marker;
    int                to_rd_len;

    BT_IGNORE_UNUSED_PARAM(args);

    marker    = 0U;
    to_rd_len = HTTPS_BUF_SIZE_PL;

    BT_LOOP_FOREVER()
    {
        /* Lock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&https_sock_mutex);

        if (0x0U == https_transport_state_pl)
        {
            HTTPS_PL_TRC(
            "[HTTPS-PL] HPS Transport Read Task: Waiting to Service ..\n");

            (BT_IGNORE_RETURN_VALUE) BT_thread_cond_wait
            (
                &https_sock_cond,
                &https_sock_mutex
            );

            HTTPS_PL_TRC(
            "[HTTPS-PL] HPS Transport Read Task: Ready to Service ..\n");

            /* Initialize the variables when ready to read afresh */
            marker    = 0U;
            to_rd_len = HTTPS_BUF_SIZE_PL;
        }

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&https_sock_mutex);

        if (0x1U == https_transport_state_pl)
        {
            send_data = BT_TRUE;

            /* Read Data from Socket */
            nbytes = mbedtls_ssl_read
                     (
                         &(tlsDataParams.ssl),
                         &https_sock_read_buf[marker],
                         to_rd_len
                     );

            /**
             * All possible outcomes from the above recv function:
             *
             * 1. nbtes   < 0
             * 2. nbytes == 0
             * 3. nbytes > 0 but less than the needed length
             * 4. nbytes == needed length
             * 5. nbytes == Max requested Length i.e. HTTPS_BUF_SIZE_PL
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
             * case 5. [nbytes == Max requested Length i.e. HTTPS_BUF_SIZE_PL]
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
                HTTPS_PL_ERR (
                "[HTTPS-PL] Socket Read either returned 0 or Failure. nbytes = %d\n",
                nbytes);

                /**
                 * Check if the error occurred in the first read.
                 * If there is something already available, pass that
                 * to upper layer.
                 * Else, pass ZERO(same as current marker) Bytes to the upper layer.
                 */
                if (0U == marker)
                {
                    HTTPS_PL_TRC (
                    "[HTTPS-PL] Socket Failure occured during first read! nbytes = %d\n",
                    nbytes);
                }

                /* Give the callback to the application */
            }
            else if (HTTPS_BUF_SIZE_PL == nbytes)
            {
                HTTPS_PL_INF (
                "[HTTPS-PL] Received %d Bytes from socket recv\n", nbytes);

                marker = nbytes;

                HTTPS_PL_TRC (
                "[HTTPS-PL] Marking send data to UL as TRUE. Current Marker is: %d\n",
                marker);

                /* do nothing, entire buf to be given to callback */
            }
            else
            {
                /**
                 * Keep Appending the Read Data to the Buffer, and Read from Socket
                 * till a failure is received or the bytes read count reaches 0
                 */
                HTTPS_PL_INF (
                "[HTTPS-PL] Before processing: nbytes=%d; marker =%d, to_rd=%d\n",
                nbytes, marker, to_rd_len);

                marker    += nbytes;
                to_rd_len  = ((HTTPS_BUF_SIZE_PL) - marker);

                HTTPS_PL_INF (
                "[HTTPS-PL] After processing: nbytes=%d; marker =%d, to_rd=%d\n",
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
                if (NULL != https_appl_cb)
                {
                    HTTPS_PL_INF (
                    "[HTTPS-PL] Total Bytes to Send to UL is %d, Buffer is \n%s\n",
                    marker, https_sock_read_buf);

                    (BT_IGNORE_RETURN_VALUE) https_appl_cb
                    (
                        https_sock_read_buf,
                        (UINT16) marker
                    );
                }

                /* Reset Send Data Flag */
                send_data = BT_FALSE;

                /* Reset the Read Buffer */
                BT_mem_set(https_sock_read_buf, 0x0, sizeof(https_sock_read_buf));
            }
        }
    }
}

static BT_THREAD_RETURN_TYPE https_init_task (BT_THREAD_ARGS args)
{
    (void)https_init();

    /* Signal the waiting Task */
    (void)OSA_EventSet((osa_event_handle_t)https_event_handle, (osa_event_flags_t)HTTPS_EVENT_INIT_COM);

    vTaskDelete(NULL);
}

static BT_THREAD_RETURN_TYPE https_connect_task (BT_THREAD_ARGS args)
{
    BT_IGNORE_UNUSED_PARAM(args);

    BT_LOOP_FOREVER()
    {
        /* Wait for connect signal */
        (void)OSA_EventWait((osa_event_handle_t)https_event_handle, (osa_event_flags_t)HTTPS_EVENT_CONNECT_REQ, 0U, 0xffffffffU, NULL);

        /* Connect */
        if (SUCCESS != https_connect(https_connect_ip_addr))
        {
            HTTPS_PL_ERR(
            "[HTTPS-PL] FAILED to Connect to HTTP Server\n");
            https_connect_ret = API_FAILURE;
        }
        else
        {
            HTTPS_PL_INF(
            "[HTTPS-PL] Connected to HTTP Server\n");
            https_connect_ret = API_SUCCESS;
        }

        /* Signal Connection build. */
        (void)OSA_EventSet((osa_event_handle_t)https_event_handle, (osa_event_flags_t)HTTPS_EVENT_CONNECT_COM);
    }
}


API_RESULT https_transport_init_pl
           (
               HTTPS_TRANSPORT_CB_PL cb
           )
{
    BT_thread_type      tid;
    API_RESULT          retval;
    BT_thread_attr_type https_task_attr;

    static UINT8 first = 0U;

    /* Initialize */
    retval = API_FAILURE;

    BT_MUTEX_INIT (https_sock_mutex, HTTPS_PL);
    BT_COND_INIT  (https_sock_cond , HTTPS_PL);
    (void)OSA_EventCreate((osa_event_handle_t)https_event_handle, 1U);

    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&https_sock_mutex);

    /* Check for NULL */
    if (NULL != cb)
    {
        /* Store the application callback in global */
        https_appl_cb = cb;

        retval = API_SUCCESS;

        HTTPS_PL_TRC(
        "[HTTPS-PL] HPS Transport RX Callback Registration Success!\n");
    }
    else
    {
        HTTPS_PL_ERR(
        "[HTTPS-PL] HPS Transport RX Callback Registration Failed!\n");

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&https_sock_mutex);

        return retval;
    }

    /* Initialize HPS Transport Socket State */
    https_transport_state_pl = 0x0U;

    if (0U == first)
    {
        first = 1U;

        /* Platform mbedTSL Initialization */

        /* Initialize the HTTPS init Task Attributes */
        https_task_attr.thread_name       = "EtherMind HTTPS init Task";
        https_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
        https_task_attr.thread_priority   = BT_TASK_PRIORITY;

        /* MISRA C-2012 Rule 17.7 | Coverity CHECKED_RETURN */
        (void) BT_thread_create (&tid, &https_task_attr, https_init_task, NULL);

        /* Wait for init complete */
        (void)OSA_EventWait((osa_event_handle_t)https_event_handle, (osa_event_flags_t)HTTPS_EVENT_INIT_COM, 0U, 0xffffffffU, NULL);
    }

    /* Initialize the HPS Task Attributes */
    https_task_attr.thread_name       = "EtherMind HTTPS PL Task";
    https_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    https_task_attr.thread_priority   = BT_TASK_PRIORITY;

    /* Create a thread to receive data from Socket and BUFFER it */
    if (BT_thread_create (&tid, &https_task_attr, https_read_task_pl, NULL) != 0U)
    {
        HTTPS_PL_ERR(
        "[HTTPS-PL] Could NOT Create HPS Transport Read Thread\n");

        retval = API_FAILURE;

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&https_sock_mutex);

        return retval;
    }

    /* Initialize the HTTPS connect Task Attributes */
    https_task_attr.thread_name       = "EtherMind HTTPS connect PL Task";
    https_task_attr.thread_stack_size = BT_TASK_STACK_DEPTH;
    https_task_attr.thread_priority   = BT_TASK_PRIORITY;

    /* Create a thread to build https connect */
    if (BT_thread_create (&tid, &https_task_attr, https_connect_task, NULL) != 0U)
    {
        HTTPS_PL_ERR(
        "[HTTPS-PL] Could NOT Create HPS HTTPS connect Thread\n");

        retval = API_FAILURE;

        /* Unlock */
        (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&https_sock_mutex);

        return retval;
    }

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&https_sock_mutex);

    HTTPS_PL_ERR(
    "[HTTPS-PL] HPS Transport Initialization Complete, retval 0x%04X!\n",
    retval);

    return retval;
}

API_RESULT https_transport_deinit_pl
           (
               void
           )
{
    (BT_IGNORE_RETURN_VALUE) https_deinit();

    return API_SUCCESS;
}

API_RESULT https_transport_write_data_pl
           (
               UCHAR  * data,
               UINT16 data_len
           )
{
    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&https_sock_mutex);

    /* Send data to socket */
    (BT_IGNORE_RETURN_VALUE) mbedtls_ssl_write(&(tlsDataParams.ssl), data, data_len);

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&https_sock_mutex);

    return API_SUCCESS;
}


API_RESULT https_transport_open_pl (UCHAR * ip_addr)
{
    /* Initialize the Read Buffer */
    BT_mem_set(https_sock_read_buf, 0x0, sizeof(https_sock_read_buf));

    /* Create Socket connection to remote server */

    /* Signal */
    https_connect_ip_addr = ip_addr;
    (void)OSA_EventSet((osa_event_handle_t)https_event_handle, (osa_event_flags_t)HTTPS_EVENT_CONNECT_REQ);

    /* Wait for connect complete */
    (void)OSA_EventWait((osa_event_handle_t)https_event_handle, (osa_event_flags_t)HTTPS_EVENT_CONNECT_COM, 0U, 0xffffffffU, NULL);

    if (API_SUCCESS != https_connect_ret)
    {
        return API_FAILURE;
    }

    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock(&https_sock_mutex);

    /* Set the transport State */
    https_transport_state_pl = 0x1U;

    /* Signal the waiting Task */
    (BT_IGNORE_RETURN_VALUE) BT_thread_cond_signal(&https_sock_cond);

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock(&https_sock_mutex);

    HTTPS_PL_INF(
    "[HTTPS-PL] HTTP Server Communication Established\n");

    return API_SUCCESS;
}


void https_transport_close_pl (void)
{
    /* Lock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_lock (&https_sock_mutex);

    /* Reset the transport State */
    https_transport_state_pl = 0x0U;

    /* Unlock */
    (BT_IGNORE_RETURN_VALUE) BT_thread_mutex_unlock (&https_sock_mutex);

    /* Wait for HPS Transport Read Task to Exit */
    BT_usleep (100U);

    /* Close the socket connection */
    (BT_IGNORE_RETURN_VALUE) https_disconnect();

    HTTPS_PL_INF(
    "[HTTPS-PL] HTTP Server Communication Destroyed!\n");
}


