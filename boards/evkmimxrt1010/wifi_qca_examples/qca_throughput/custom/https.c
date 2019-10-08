// Copyright (c) Qualcomm Atheros, Inc.
// All rights reserved.
// Redistribution and use in source and binary forms, with or without modification, are permitted (subject to
// the limitations in the disclaimer below) provided that the following conditions are met:
//
// � Redistributions of source code must retain the above copyright notice, this list of conditions and the
//   following disclaimer.
// � Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
//   following disclaimer in the documentation and/or other materials provided with the distribution.
// � Neither the name of nor the names of its contributors may be used to endorse or promote products derived
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

#include <main.h>
#include <throughput.h>
#include <stdio.h>

#if ENABLE_STACK_OFFLOAD
#if ENABLE_SSL
#include <https.h>

#define DEFAULT_PACKET_SIZE 1400
#define CLIENT_WAIT_TIME 1000

#define HTTPS_DEFAULT_PORT 443

#define HTTPD_RESP_200 "200 OK"
#define HTTPD_RESP_404 "404 Not found"
#define HTTPD_RESP_PROTOCOL "HTTP/1.0"

#define HTTPS_PRINTF(args...) // no printing...
//#define HTTPS_PRINTF(args...) PRINTF(args)

/* Include certificate used by HTTPS servert*/
//#include "cert-kingfisher-inc.h"
extern const uint8_t sharkSslRSACertKingfisher[1016];

typedef struct
{
    SSL_CTX *sslCtx;
    SSL *ssl;
    uint32_t serverSocket;
    uint32_t clientSocket;
} HTTPS_SERVER_DATA_T;

static HTTPS_SERVER_DATA_T HttpsServerData;

#if ENABLE_HTTPS_SERVER
static const char *notfound_html =
    "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\r\n"
    "<html><head><title>404 Not Found</title></head>\r\n"
    "<body>\r\n"
    "<h1>Not Found</h1>\r\n"
    "</body></html>\r\n";

static const char *index_html =
    "<html>\r\n"
    "<head><title>It Works</title></head>\r\n"
    "<body><h1>It Works!</h1>\r\n"
    "<p>This is the index.html page.</p>\r\n"
    "</body>\r\n"
    "</html>\r\n";
#endif

#if ENABLE_HTTPS_CLIENT
static int32_t send_http_get_request(SSL *ssl, char *host, char *path)
{
    int n = 0, l = CFG_PACKET_SIZE_MAX_TX;
    char *pBuf = CUSTOM_ALLOC(l);

    if (pBuf == NULL)
    {
        return A_ERROR;
    }

    n += snprintf(&pBuf[n], l - n, "GET %s HTTP/1.0\r\n", path);
    n += snprintf(&pBuf[n], l - n, "Host: %s\r\n", host);
    n += snprintf(&pBuf[n], l - n, "User-Agent: IOE Client\r\n");
    n += snprintf(&pBuf[n], l - n, "Accept: text/html\r\n");
    n += snprintf(&pBuf[n], l - n, "Content-length: 0\r\n\r\n");
    uint32_t res = SSL_write(ssl, pBuf, n);

    HTTPS_PRINTF("%s", pBuf);
    HTTPS_PRINTF("SSL_write(%d) %d" NL, n, res);

#if !NON_BLOCKING_TX
    // Free the buffer only if NON_BLOCKING_TX is not enabled
    CUSTOM_FREE(pBuf);
#endif

    return res;
}

static int32_t read_http_response(SSL *ssl, uint32_t socketHandle)
{
    int32_t res;
    int32_t received;
    int32_t total = 0;
    char *pBuf;

    do
    {
        res = t_select((void *)handle, socketHandle, CLIENT_WAIT_TIME);
        HTTPS_PRINTF("t_select() %d" NL, res);
        if (res == A_OK)
        {
// HTTP response packet is available
#if ZERO_COPY
            received = SSL_read(ssl, (void **)&pBuf, CFG_PACKET_SIZE_MAX_RX);
            HTTPS_PRINTF("SSL_read() %d" NL, received);
            if (received > 0)
            {
                int32_t i;
                for (i = 0; i < received; i++)
                {
                    PRINTF("%c", pBuf[i]);
                }
                zero_copy_free(pBuf);
                total += received;
            }
#else
            pBuf = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT);
            if (pBuf == NULL)
            {
                PRINTF("ERROR: Out of memory error" NL);
                res = A_ERROR;
            }
            else
            {
                do
                {
                    received = SSL_read(ssl, pBuf, CFG_PACKET_SIZE_MAX_RX);
                    if (received > 0)
                    {
                        int32_t i;
                        for (i = 0; i < received; i++)
                        {
                            PRINTF("%c", pBuf[i]);
                        }
                        total += received;
                    }
                } while (received > 0);
                A_FREE(pBuf, MALLOC_ID_CONTEXT);
            }
#endif
        }
    } while (res == A_OK);

    return total;
}

static int32_t https_client_get(char *host, char *path, uint32_t port, uint32_t inputBufSize, uint32_t outputBufSize)
{
    int32_t res           = A_ERROR;
    SSL_CTX *sslCtx       = NULL;
    SSL *ssl              = NULL;
    uint32_t socketHandle = 0;
    DNC_CFG_CMD dnsCfg;
    DNC_RESP_INFO dnsRespInfo;
    SOCKADDR_T hostAddr;
    uint8_t *caList    = ssl_cert_data_buf;
    uint16_t caListLen = ssl_cert_data_buf_len;

    if (port == 0)
    {
        port = HTTPS_DEFAULT_PORT;
    }
    if (inputBufSize == 0)
    {
        inputBufSize = SSL_INBUF_SIZE;
    }
    if (outputBufSize == 0)
    {
        outputBufSize = SSL_OUTBUF_SIZE;
    }

    do
    {
        // resolve the IP address of the host
        uint32_t tmp_ipaddrs_list = 0; // packed issue
        if (0 == ath_inet_aton(host, &tmp_ipaddrs_list))
        {
            dnsRespInfo.ipaddrs_list[0] = tmp_ipaddrs_list;
            if (strlen(host) >= sizeof(dnsCfg.ahostname))
            {
                PRINTF("GetERROR: host name too long" NL);
                break;
            }
            strcpy((char *)dnsCfg.ahostname, host);
            dnsCfg.domain = ATH_AF_INET;
            dnsCfg.mode   = RESOLVEHOSTNAME;
            if (A_OK != custom_ip_resolve_hostname(handle, &dnsCfg, &dnsRespInfo))
            {
                PRINTF("GetERROR: Unable to resolve host name" NL);
                break;
            }
        }

        // create SSL context
        sslCtx = SSL_ctx_new(SSL_CLIENT, inputBufSize, outputBufSize, 0);
        HTTPS_PRINTF("SSL_ctx_new(SSL_CLIENT,%d,%d,%d) %x" NL, inputBufSize, outputBufSize, 0, sslCtx);
        if (sslCtx == NULL)
        {
            PRINTF("GetERROR: Unable to create SSL context" NL);
            break;
        }

        // Load certificate
        if (caListLen != 0)
        {
            res = SSL_setCaList(sslCtx, caList, caListLen);
            HTTPS_PRINTF("SSL_setCaList() %d" NL, res);
            if (res < A_OK)
            {
                PRINTF("GetERROR: Unable to load CA list" NL);
                break;
            }
        }

        // Create socket
        socketHandle = t_socket((void *)handle, ATH_AF_INET, SOCK_STREAM_TYPE, 0);
        if (socketHandle == (uint32_t)A_ERROR)
        {
            PRINTF("GetERROR: Unable to create socket" NL);
            res = A_ERROR;
            break;
        }

        // Connect to the HTTPS server
        memset(&hostAddr, 0, sizeof(hostAddr));
        hostAddr.sin_addr.s_addr = dnsRespInfo.ipaddrs_list[0];
        hostAddr.sin_port        = port;
        hostAddr.sin_family      = ATH_AF_INET;
        res                      = t_connect((void *)handle, socketHandle, (&hostAddr), sizeof(hostAddr));
        if (res != A_OK)
        {
            PRINTF("GetERROR: Connection failed." NL);
            break;
        }

        // Create SSL connection inst
        ssl = SSL_new(sslCtx);
        HTTPS_PRINTF("SSL_new() %x" NL, ssl);
        if (ssl == NULL)
        {
            PRINTF("GetERROR: Unable to create SSL context" NL);
            res = A_ERROR;
            break;
        }

        // Add socket handle to SSL connection
        res = SSL_set_fd(ssl, socketHandle);
        HTTPS_PRINTF("SSL_set_fd() %d" NL, res);
        if (res < 0)
        {
            PRINTF("GetERROR: Unable to add socket handle to SSL" NL);
            break;
        }

        // SSL handshake with server
        res = SSL_connect(ssl);
        HTTPS_PRINTF("SSL_connect() %d" NL, res);
        if (res < 0)
        {
            PRINTF("GetERROR: SSL connect failed" NL);
            break;
        }

        // Send HTTP GET request
        res = send_http_get_request(ssl, host, path);
        if (res < 0)
        {
            PRINTF("GetERROR: Failed to send HTTP GET request" NL);
            break;
        }

        // Receive and print the HTTP response
        res = read_http_response(ssl, socketHandle);
        if (res <= 0)
        {
            PRINTF("GetERROR: No HTTP response message received" NL);
            break;
        }
        PRINTF("GetOK" NL);
    } while (0);

    // Clean up
    if (ssl != NULL)
    {
        SSL_shutdown(ssl);
    }
    if (sslCtx != NULL)
    {
        SSL_ctx_free(sslCtx);
    }
    if (socketHandle > 0)
    {
        t_shutdown(handle, socketHandle);
    }

    if (res > A_OK) // some of the SSL functions return 1 for success!
    {
        res = A_OK;
    }
    return res;
}

int32_t https_client_handler(int32_t argc, char *argv[])
{
    if (IS_DRIVER_READY != A_OK)
    {
        PRINTF("Driver not loaded!" NL);
        return A_ERROR;
    }

    if (argc < 4)
    {
        if (argc > 1)
        {
            PRINTF("Incomplete params" NL);
        }
        PRINTF("Usage: httpsc get <host> <path> [options]  - get a page from a HTTPS server" NL);
        PRINTF("  where <host>  = Host name or IPaddress of HTTPS server" NL);
        PRINTF("        <path>  = Page to retrieve" NL);
        PRINTF("  options:" NL);
        PRINTF("        -p<num> = Port number of HTTPS server (optional)" NL);
        PRINTF("        -i<num> = SharkSSL input buffer size (optional)" NL);
        PRINTF("        -o<num> = SharkSSL output buffer size (optional)" NL);
        return A_ERROR;
    }

    if (ATH_STRCMP(argv[1], "get") == 0)
    {
        char *host             = argv[2];
        char *path             = argv[3];
        uint32_t port          = 0;
        uint32_t inputBufSize  = 0;
        uint32_t outputBufSize = 0;
        int argn               = 4;

        // Handle optional arguments
        while (argn < argc)
        {
            if ((argv[argn][0] == '-') && (strlen(argv[argn]) > 2))
            {
                switch (argv[argn][1])
                {
                    case 'p':
                        port = atol(&argv[argn][2]);
                        break;
                    case 'i':
                        inputBufSize = atol(&argv[argn][2]);
                        break;
                    case 'o':
                        outputBufSize = atol(&argv[argn][2]);
                        break;
                }
            }
            argn++;
        }
        return https_client_get(host, path, port, inputBufSize, outputBufSize);
    }
    else
    {
        PRINTF("Unknown command %s, supported command: get" NL, argv[1]);
        return (A_ERROR);
    }
}
#endif // ENABLE_HTTPS_CLIENT

#if ENABLE_HTTPS_SERVER
static uint32_t send_http_response(const char *respCode, const char *content, int contentLen)
{
    int n = 0, l = CFG_PACKET_SIZE_MAX_TX;
    char *pBuf = CUSTOM_ALLOC(l);

    if (pBuf == NULL)
    {
        HTTPS_PRINTF("NO TX buffer!!!!" NL);
        return 0;
    }

    // NOTE: This code does NOT split the HTTPS response message in multiple
    // buffers if it is too big to fit in one TX buffer!
    n += snprintf(pBuf, l - n, "%s %s\r\n", HTTPD_RESP_PROTOCOL, respCode);
    n += snprintf(&pBuf[n], l - n, "Content-Type: text/html\r\n");
    n += snprintf(&pBuf[n], l - n, "Content-Length: %d\r\n", contentLen);
    n += snprintf(&pBuf[n], l - n, "\r\n");
    memcpy(&pBuf[n], content, contentLen);
    n += contentLen;
    n += snprintf(&pBuf[n], l - n, "\r\n");
    int i;
    for (i = 0; i < n; i++)
    {
        HTTPS_PRINTF("%c", pBuf[i]);
    }

    uint32_t res = SSL_write(HttpsServerData.ssl, pBuf, n);
    HTTPS_PRINTF("SSL_write(%d) %d" NL, n, res);

#if !NON_BLOCKING_TX
    // Free the buffer only if NON_BLOCKING_TX is not enabled
    CUSTOM_FREE(pBuf);
#endif
    return res;
}

static int32_t process_http_request(char *buf, int size)
{
    const char *content;
    int contentLen;
    const char *respCode = HTTPD_RESP_200;
    int len              = 0;

    if (strncmp(buf, "GET ", 4) == 0)
    {
        len = 4;
    }
    // else if (strncmp(buf, "POST ", 5) == 0)
    //    len = 5;
    else
    {
        HTTPS_PRINTF("Unsupported HTTP request" NL);
        return -2;
    }

    // Parsing "GET /some/path" or "GET /some/path HTTP/1.X"
    buf += len;
    char *q = strchr(buf, ' ');
    if (q == NULL)
    {
        q = strchr(buf, '\r');
    }

    // Terminate file name
    *q = '\0';

    // find file
    if (strcmp(buf, "/") == 0 || strcmp(buf, "/index.html") == 0)
    {
        content = index_html;
    }
    else
    {
        respCode = HTTPD_RESP_404;
        content  = notfound_html;
    }
    contentLen = strlen(content);

    // Send HTTP response
    return send_http_response(respCode, content, contentLen);
}

static void handle_client(void)
{
    int32_t res;
    int32_t received;
    char *pBuf;

    while (HttpsServerData.clientSocket > 0)
    {
        res = t_select(handle, HttpsServerData.clientSocket, CLIENT_WAIT_TIME);
        if (res == A_SOCK_INVALID)
        {
            HTTPS_PRINTF("t_select() %d" NL, res);
            t_shutdown(handle, HttpsServerData.clientSocket);
            HttpsServerData.clientSocket = 0;
            return;
        }

        if (res == A_OK)
        {
// HTTP response packet is available
#if ZERO_COPY
            do
            {
                received = SSL_read(HttpsServerData.ssl, (void **)&pBuf, CFG_PACKET_SIZE_MAX_RX);
                HTTPS_PRINTF("SSL_read() %d" NL, received);
                if (received > 0)
                {
                    // Simple solution that requres the HTTP request to be sent in one packet!
                    if (pBuf[received - 1] == '\n' && (pBuf[received - 3] == '\n' || pBuf[received - 2] == '\n'))
                    {
                        pBuf[received - 1] = '\0';
                        HTTPS_PRINTF("%s", pBuf);
                        process_http_request(pBuf, received);
                        t_shutdown(handle, HttpsServerData.clientSocket);
                        HttpsServerData.clientSocket = 0;
                    }
                    zero_copy_free(pBuf);
                }
            } while (received > 0 && HttpsServerData.clientSocket > 0);
#else
            pBuf = A_MALLOC(CFG_PACKET_SIZE_MAX_RX, MALLOC_ID_CONTEXT);
            if (pBuf == NULL)
            {
                PRINTF("ERROR: Out of memory error" NL);
                received = -1;
                res      = A_ERROR;
            }
            else
            {
                do
                {
                    received = SSL_read(HttpsServerData.ssl, pBuf, CFG_PACKET_SIZE_MAX_RX);
                    if (received > 0)
                    {
                        // Simple solution that requres the HTTP request to be sent in one packet!
                        if (pBuf[received - 1] == '\n' && (pBuf[received - 3] == '\n' || pBuf[received - 2] == '\n'))
                        {
                            pBuf[received - 1] = '\0';
                            HTTPS_PRINTF("%s", pBuf);
                            process_http_request(pBuf, received);
                            t_shutdown(handle, HttpsServerData.clientSocket);
                            HttpsServerData.clientSocket = 0;
                        }
                    }
                } while (received > 0);
                A_FREE(pBuf, MALLOC_ID_CONTEXT);
            }
#endif
        }
    }
}

static void https_server_close_all(void)
{
    // Make sure sockets and SLL contexts are free'ed
    if (HttpsServerData.ssl != NULL)
    {
        SSL_shutdown(HttpsServerData.ssl);
        HttpsServerData.ssl = NULL;
    }
    if (HttpsServerData.sslCtx != NULL)
    {
        SSL_ctx_free(HttpsServerData.sslCtx);
        HttpsServerData.sslCtx = NULL;
    }
    if (HttpsServerData.clientSocket != 0)
    {
        if (HttpsServerData.clientSocket > 0)
        {
            t_shutdown(handle, HttpsServerData.clientSocket);
        }
        HttpsServerData.clientSocket = 0;
    }
    if (HttpsServerData.serverSocket != 0)
    {
        uint32_t socket              = HttpsServerData.serverSocket;
        HttpsServerData.serverSocket = 0;
        if (socket > 0)
        {
            t_shutdown(handle, socket);
        }
    }
}

static int32_t https_server_start(uint16_t Port, uint32_t InputBufSize, uint32_t OutputBufSize)
{
    int32_t res = A_ERROR;
    SOCKADDR_T addr;
    uint8_t *cert     = ssl_cert_data_buf;
    uint16_t cert_len = ssl_cert_data_buf_len;

    if (HttpsServerData.serverSocket)
    {
        PRINTF("ERROR: server started");
        return A_ERROR;
    }

    if (Port == 0)
    {
        Port = HTTPS_DEFAULT_PORT;
    }
    if (InputBufSize == 0)
    {
        InputBufSize = SSL_INBUF_SIZE;
    }
    if (OutputBufSize == 0)
    {
        OutputBufSize = SSL_OUTBUF_SIZE;
    }

    while (1)
    {
        // Create server socket
        HttpsServerData.serverSocket = t_socket(handle, ATH_AF_INET, SOCK_STREAM_TYPE, 0);
        if (HttpsServerData.serverSocket == (uint32_t)A_ERROR)
        {
            PRINTF("ERROR: Unable to create socket" NL);
            res = A_ERROR;
            break;
        }

        // Bind
        memset(&addr, 0, sizeof(addr));
        addr.sin_port   = Port;
        addr.sin_family = ATH_AF_INET;
        res             = t_bind(handle, HttpsServerData.serverSocket, &addr, sizeof(addr));
        if (res != A_OK)
        {
            PRINTF("ERROR: Socket bind error" NL);
            break;
        }

        // Create SSL context
        HttpsServerData.sslCtx = SSL_ctx_new(SSL_SERVER, InputBufSize, OutputBufSize, 0);
        if (HttpsServerData.sslCtx == NULL)
        {
            PRINTF("ERROR: Unable to create SSL context" NL);
            break;
        }

        // Load certificate
        if (cert_len == 0)
        {
            // Load the default certificate
            cert     = (uint8_t *)sharkSslRSACertKingfisher;
            cert_len = sizeof(sharkSslRSACertKingfisher);
            PRINTF("Using the default  certificate" NL);
        }
        if (SSL_addCert(HttpsServerData.sslCtx, cert, cert_len) < A_OK)
        {
            PRINTF("ERROR: Unable to load certificate" NL);
            break;
        }

        PRINTF("HTTPS server started - waiting for a client to connect on port %d" NL, Port);

        // Listen
        res = t_listen(handle, HttpsServerData.serverSocket, 1);
        if (res != A_OK)
        {
            PRINTF("ERROR: Socket listen error" NL);
            break;
        }

        // block until a client connects
        do
        {
            res = t_select(handle, HttpsServerData.serverSocket, 50);
        } while (res == A_ERROR && HttpsServerData.serverSocket != 0);
        if (HttpsServerData.serverSocket == 0)
        {
            break; // socket no longer valid
        }

        // Accept incoming connection
        HttpsServerData.clientSocket = t_accept(handle, HttpsServerData.serverSocket, &addr, sizeof(addr));
        if (HttpsServerData.clientSocket != (uint32_t)A_ERROR)
        {
            do
            {
                // Create SSL connection inst
                HttpsServerData.ssl = SSL_new(HttpsServerData.sslCtx);
                if (HttpsServerData.ssl == NULL)
                {
                    PRINTF("ERROR: Unable to create SSL context" NL);
                    res = A_ERROR;
                    break;
                }

                // Add socket handle to SSL connection
                res = SSL_set_fd(HttpsServerData.ssl, HttpsServerData.clientSocket);
                if (res < 0)
                {
                    PRINTF("ERROR: Unable to add socket handle to SSL" NL);
                    break;
                }

                // SSL handshake with server
                res = SSL_accept(HttpsServerData.ssl);
                if (res < 0)
                {
                    PRINTF("ERROR: SSL accept failed" NL);
                    break;
                }

                // Handle HTTP requests from client
                handle_client();
            } while (0);
        }
        else
        {
            HttpsServerData.clientSocket = 0;
        }

        // clean up
        https_server_close_all();
    };

    // clean up
    https_server_close_all();
    PRINTF("HTTPS server stopped" NL);
    return A_OK;
}

static int32_t https_server_stop(void)
{
    if (HttpsServerData.serverSocket == 0)
    {
        PRINTF("ERROR: server not started" NL);
        return A_ERROR;
    }

    https_server_close_all();
    return A_OK;
}

int32_t https_server_handler(int32_t argc, char *argv[])
{
    /*Check if driver is loaded*/
    if (IS_DRIVER_READY != A_OK)
    {
        return A_ERROR;
    }

    if (argc < 2)
    {
        PRINTF("Usage: httpss start|stop> [options] = Start/Stop the HTTPS server" NL);
        PRINTF("  options: (start only)" NL);
        PRINTF("    -p<num> = Port number of HTTPS server (optional)" NL);
        PRINTF("    -i<num> = SharkSSL input buffer size (optional)" NL);
        PRINTF("    -o<num> = SharkSSL output buffer size (optional)" NL);
        return A_ERROR;
    }

    if (ATH_STRCMP(argv[1], "start") == 0)
    {
        uint32_t port          = 0;
        uint32_t inputBufSize  = 0;
        uint32_t outputBufSize = 0;
        int argn               = 2;

        // Handle optional arguments
        while (argn < argc)
        {
            if ((argv[argn][0] == '-') && (strlen(argv[argn]) > 2))
            {
                switch (argv[argn][1])
                {
                    case 'p':
                        port = atol(&argv[argn][2]);
                        break;
                    case 'i':
                        inputBufSize = atol(&argv[argn][2]);
                        break;
                    case 'o':
                        outputBufSize = atol(&argv[argn][2]);
                        break;
                    default:
                        PRINTF("Invalid option: %s" NL, argv[argn]);
                        return (A_ERROR);
                }
            }
            argn++;
        }
        return https_server_start(port, inputBufSize, outputBufSize);
    }

    if (ATH_STRCMP(argv[1], "stop") == 0)
    {
        return https_server_stop();
    }

    PRINTF("Unknown command: %s, supported: start, stop" NL, argv[1]);
    return (A_ERROR);
}
#endif
#endif // ENABLE_SSL
#endif // ENABLE_STACK_OFFLOAD
