/*
 * Amazon FreeRTOS Secure Sockets for lwIP
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://aws.amazon.com/freertos
 * http://www.FreeRTOS.org
 */

/*
 * Copyright (C) NXP 2017-2018.
 */


/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "aws_secure_sockets.h"
#include "aws_tls.h"
#include "task.h"

/* Logging includes. */
#include "aws_logging_task.h"

/* LwIP includes. */
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"


/**
* @brief Flag indicating that socket send operations are not permitted.
*
* If a WR shutdown in SOCKETS_Shutdown() is invoked, this flag is
* set in the socket's xShutdownFlags member.
*/
#define nxpsecuresocketsSOCKET_WRITE_CLOSED_FLAG    ( 1UL << 2 )

/**
* @brief Flag indicating that socket receive operations are not permitted.
*
* If a RD shutdown in SOCKETS_Shutdown() is invoked, this flag is
* set in the socket's xShutdownFlags member.
*/
#define nxpsecuresocketsSOCKET_READ_CLOSED_FLAG     ( 1UL << 1 )

/**
* @brief Delay used between network select attempts when effecting a receive timeout.
*
* Timeouts are mocked in the secure sockets layer, and this constant sets the
* sleep time between each read attempt during the receive timeout period.
*/
#define nxpsecuresocketsFIVE_MILLISECONDS    ( pdMS_TO_TICKS( 5 ) )

/**
* @brief The timeout supplied to the t_select function.
*
* Receive timeout are emulated in secure sockets layer and therefore we
* do not want the Inventek module to block. Setting to zero means
* no timeout, so one is the smallest value we can set it to.
*/
#define nxpsecuresocketsONE_MILLISECOND      ( 1 )

/* Internal context structure. */
typedef struct SSOCKETContext
{
    Socket_t xSocket;
    char * pcDestination;
    void * pvTLSContext;
    BaseType_t xRequireTLS;
    BaseType_t xSendFlags;
    BaseType_t xRecvFlags;

    BaseType_t xShutdownFlags;
    uint32_t ulSendTimeout;
    uint32_t ulRecvTimeout;

    char * pcServerCertificate;
    uint32_t ulServerCertificateLength;

    char ** ppcAlpnProtocols;
    uint32_t ulAlpnProtocolsCount;
} SSOCKETContext_t, * SSOCKETContextPtr_t;

/*
 * Helper routines.
 */

/*
 * @brief Network send callback.
 */
static BaseType_t prvNetworkSend( void * pvContext,
                                  const unsigned char * pucData,
                                  size_t xDataLength )
{
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) pvContext;

    int ret = lwip_send( ( int ) pxContext->xSocket,
                         pucData,
                         xDataLength,
                         pxContext->xSendFlags );
    return ret;
}




/*-----------------------------------------------------------*/


/*
 * @brief Network receive callback.
 */
static BaseType_t prvNetworkRecv( void * pvContext,
                                  unsigned char * pucReceiveBuffer,
                                  size_t xReceiveLength )
{
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) pvContext;

    int fd_socket = ( int ) pxContext->xSocket;
    fd_set read_set;

    FD_ZERO(&read_set);
    FD_SET(fd_socket, &read_set);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = pxContext->ulRecvTimeout * 1000;

    int retVal = 0;
    retVal = lwip_select(fd_socket+1, &read_set, NULL, NULL, &timeout);
    if(retVal > 0)
    {
        retVal = lwip_recv( ( int ) pxContext->xSocket,
                            pucReceiveBuffer,
                            xReceiveLength,
                            pxContext->xSendFlags );
    }

    return retVal;
}
/*-----------------------------------------------------------*/

/*
 * Interface routines.
 */

int32_t SOCKETS_Close( Socket_t xSocket )
{
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;
    int32_t lStatus = SOCKETS_ERROR_NONE;

    if( ( NULL != pxContext ) && ( SOCKETS_INVALID_SOCKET != pxContext ) )
    {
        if( NULL != pxContext->pcDestination )
        {
            vPortFree( pxContext->pcDestination );
        }

        if( NULL != pxContext->pcServerCertificate )
        {
            vPortFree( pxContext->pcServerCertificate );
        }

        if( pdTRUE == pxContext->xRequireTLS )
        {
            TLS_Cleanup( pxContext->pvTLSContext );
        }

        if( NULL != pxContext->ppcAlpnProtocols )
        {
            int i;
            for( i = 0; i < pxContext->ulAlpnProtocolsCount; i++ )
            {
                vPortFree( pxContext->ppcAlpnProtocols[i] );
            }
            vPortFree( pxContext->ppcAlpnProtocols );
            pxContext->ulAlpnProtocolsCount = 0;
        }

        lwip_close( ( int ) pxContext->xSocket );
        vPortFree( pxContext );
    }
    else
    {
        lStatus = SOCKETS_SOCKET_ERROR;
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Connect( Socket_t xSocket,
                         SocketsSockaddr_t * pxAddress,
                         Socklen_t xAddressLength )
{
    int32_t xStatus = SOCKETS_ERROR_NONE;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;
    TLSParams_t xTLSParams = { 0 };
    struct sockaddr_in xTempAddress = { 0 };

    if( ( SOCKETS_INVALID_SOCKET != pxContext ) && ( NULL != pxAddress ) )
    {
        /* Connect the wrapped socket. */
        xTempAddress.sin_addr.s_addr = pxAddress->ulAddress;
        xTempAddress.sin_family = pxAddress->ucSocketDomain;
        xTempAddress.sin_port = pxAddress->usPort;
        xStatus = lwip_connect( ( int ) pxContext->xSocket,
                                ( struct sockaddr * ) &xTempAddress,
                                sizeof(struct sockaddr_in) );

        /* Negotiate TLS if requested. */
        if( ( SOCKETS_ERROR_NONE == xStatus ) && ( pdTRUE == pxContext->xRequireTLS ) )
        {
            xTLSParams.ulSize = sizeof( xTLSParams );
            xTLSParams.pcDestination = pxContext->pcDestination;
            xTLSParams.pcServerCertificate = pxContext->pcServerCertificate;
            xTLSParams.ulServerCertificateLength = pxContext->ulServerCertificateLength;
            xTLSParams.pvCallerContext = pxContext;
            xTLSParams.pxNetworkRecv = prvNetworkRecv;
            xTLSParams.pxNetworkSend = prvNetworkSend;
            xTLSParams.ppcAlpnProtocols = ( const char ** ) pxContext->ppcAlpnProtocols;
            xTLSParams.ulAlpnProtocolsCount = pxContext->ulAlpnProtocolsCount;
            xStatus = TLS_Init( &pxContext->pvTLSContext, &xTLSParams );

            if( SOCKETS_ERROR_NONE == xStatus )
            {
                xStatus = TLS_Connect( pxContext->pvTLSContext );
            }
        }
    }
    else
    {
        xStatus = SOCKETS_SOCKET_ERROR;
    }

    return xStatus;
}
/*-----------------------------------------------------------*/

uint32_t SOCKETS_GetHostByName( const char * pcHostName )
{
    uint32_t addr = 0;
    int ret = netconn_gethostbyname(pcHostName, (ip_addr_t*)&addr);
    configASSERT(ret == ERR_OK);

    return addr;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Recv( Socket_t xSocket,
                      void * pvBuffer,
                      size_t xBufferLength,
                      uint32_t ulFlags )
{
    int32_t lStatus = SOCKETS_SOCKET_ERROR;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;

    pxContext->xRecvFlags = ( BaseType_t ) ulFlags;

    if( ( SOCKETS_INVALID_SOCKET != xSocket ) &&
        ( NULL != pvBuffer ) &&
        ( ( nxpsecuresocketsSOCKET_READ_CLOSED_FLAG & pxContext->xShutdownFlags ) == 0UL ) )
    {
        if( pdTRUE == pxContext->xRequireTLS )
        {
            /* Receive through TLS pipe, if negotiated. */
            lStatus = TLS_Recv( pxContext->pvTLSContext, pvBuffer, xBufferLength );
        }
        else
        {
            /* Receive unencrypted. */
            lStatus = prvNetworkRecv( pxContext, pvBuffer, xBufferLength );
        }
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Send( Socket_t xSocket,
                      const void * pvBuffer,
                      size_t xDataLength,
                      uint32_t ulFlags )
{
    int32_t lStatus = SOCKETS_SOCKET_ERROR;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;

    if( ( SOCKETS_INVALID_SOCKET != pxContext ) &&
        ( NULL != pvBuffer ) &&
        ( ( nxpsecuresocketsSOCKET_WRITE_CLOSED_FLAG & pxContext->xShutdownFlags ) == 0UL ) )
    {
        pxContext->xSendFlags = ( BaseType_t ) ulFlags;

        if( pdTRUE == pxContext->xRequireTLS )
        {
            /* Send through TLS pipe, if negotiated. */
            lStatus = TLS_Send( pxContext->pvTLSContext, pvBuffer, xDataLength );
        }
        else
        {
            /* Send unencrypted. */
            lStatus = prvNetworkSend( pxContext, pvBuffer, xDataLength );
        }
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_SetSockOpt( Socket_t xSocket,
                            int32_t lLevel,
                            int32_t lOptionName,
                            const void * pvOptionValue,
                            size_t xOptionLength )
{
    int32_t lStatus = SOCKETS_ERROR_NONE;
    TickType_t xTimeout;
    SSOCKETContextPtr_t pxContext = ( SSOCKETContextPtr_t ) xSocket;

    if( ( NULL != pxContext ) && ( SOCKETS_INVALID_SOCKET != pxContext ) )
    {
        switch( lOptionName )
        {
            case SOCKETS_SO_SERVER_NAME_INDICATION:

                /* Non-NULL destination string indicates that SNI extension should
                 * be used during TLS negotiation. */
                if( NULL == ( pxContext->pcDestination =
                                  ( char * ) pvPortMalloc( 1 + xOptionLength ) ) )
                {
                    lStatus = SOCKETS_ENOMEM;
                }
                else
                {
                    memcpy( pxContext->pcDestination, pvOptionValue, xOptionLength );
                    pxContext->pcDestination[ xOptionLength ] = '\0';
                }

                break;

            case SOCKETS_SO_TRUSTED_SERVER_CERTIFICATE:

                /* Non-NULL server certificate field indicates that the default trust
                 * list should not be used. */
                if( NULL == ( pxContext->pcServerCertificate =
                                  ( char * ) pvPortMalloc( xOptionLength ) ) )
                {
                    lStatus = SOCKETS_ENOMEM;
                }
                else
                {
                    memcpy( pxContext->pcServerCertificate, pvOptionValue, xOptionLength );
                    pxContext->ulServerCertificateLength = xOptionLength;
                }

                break;

            case SOCKETS_SO_REQUIRE_TLS:
                pxContext->xRequireTLS = pdTRUE;
                break;

            case SOCKETS_SO_NONBLOCK:
                xTimeout = 0;
                /* TODO: Investigate the NONBLOCK compile time config. */
                pxContext->ulSendTimeout = 1;
                pxContext->ulRecvTimeout = 1;

                break;

            case SOCKETS_SO_RCVTIMEO:
                xTimeout = *( ( const TickType_t * ) pvOptionValue ); /*lint !e9087 pvOptionValue passed should be of TickType_t. */

                if( xTimeout == 0U )
                {
                    pxContext->ulRecvTimeout = portMAX_DELAY;
                }
                else
                {
                    pxContext->ulRecvTimeout = xTimeout;
                }

                break;

            case SOCKETS_SO_SNDTIMEO:
                /* Comply with Berkeley standard - a 0 timeout is wait forever. */
                xTimeout = *( ( const TickType_t * ) pvOptionValue ); /*lint !e9087 pvOptionValue passed should be of TickType_t. */

                if( xTimeout == 0U )
                {
                    pxContext->ulSendTimeout = portMAX_DELAY;
                }
                else
                {
                    pxContext->ulSendTimeout = xTimeout;
                }

                break;

            case SOCKETS_SO_ALPN_PROTOCOLS:
                /* Prepare list of supported protocols, must be NULL-terminated */
                pxContext->ppcAlpnProtocols = (char **)pvPortMalloc((xOptionLength + 1 )*sizeof(char*));
                if( pxContext->ppcAlpnProtocols == NULL )
                {
                    return SOCKETS_ENOMEM;
                }

                int i;
                for( i = 0; i < xOptionLength; i++ )
                {
                    int len = strlen( ((char **)pvOptionValue)[i] );
                    pxContext->ppcAlpnProtocols[i] = (char *)pvPortMalloc(len + 1);
                    if( pxContext->ppcAlpnProtocols[i] == NULL )
                    {
                        /* Malloc failed - remove created list and set error return value */
                        while(i)
                        {
                            vPortFree( pxContext->ppcAlpnProtocols[i - 1] );
                            i--;
                        }
                        vPortFree( pxContext->ppcAlpnProtocols );
                        pxContext->ppcAlpnProtocols = NULL;
                        pxContext->ulAlpnProtocolsCount = 0;

                        return SOCKETS_ENOMEM;;
                    }
                    memcpy( pxContext->ppcAlpnProtocols[i], ((char **)pvOptionValue)[i], len );
                    pxContext->ppcAlpnProtocols[i][len] = '\0';
                }

                pxContext->ppcAlpnProtocols[xOptionLength] = NULL;
                pxContext->ulAlpnProtocolsCount = xOptionLength;

                break;

            default:
                lStatus = lwip_setsockopt( ( int ) pxContext->xSocket,
                                           lLevel,
                                           lOptionName,
                                           ( void * ) pvOptionValue,
                                           xOptionLength );
                break;
        }
    }
    else
    {
        lStatus = SOCKETS_SOCKET_ERROR;
    }

    return lStatus;
}
/*-----------------------------------------------------------*/

int32_t SOCKETS_Shutdown( Socket_t xSocket,
                          uint32_t ulHow )
{
    SSOCKETContextPtr_t pxSecureSocket = ( SSOCKETContextPtr_t) xSocket;
    int32_t lRetVal = SOCKETS_SOCKET_ERROR;

    /* Ensure that a valid socket was passed. */
    if( ( SOCKETS_INVALID_SOCKET != pxSecureSocket ) )
    {
        switch( ulHow )
        {
            case SOCKETS_SHUT_RD:
                /* Further receive calls on this socket should return error. */
                pxSecureSocket->xShutdownFlags |= nxpsecuresocketsSOCKET_READ_CLOSED_FLAG;

                /* Return success to the user. */
                lRetVal = SOCKETS_ERROR_NONE;
                break;

            case SOCKETS_SHUT_WR:
                /* Further send calls on this socket should return error. */
                pxSecureSocket->xShutdownFlags |= nxpsecuresocketsSOCKET_WRITE_CLOSED_FLAG;

                /* Return success to the user. */
                lRetVal = SOCKETS_ERROR_NONE;
                break;

            case SOCKETS_SHUT_RDWR:
                /* Further send or receive calls on this socket should return error. */
                pxSecureSocket->xShutdownFlags |= nxpsecuresocketsSOCKET_READ_CLOSED_FLAG;
                pxSecureSocket->xShutdownFlags |= nxpsecuresocketsSOCKET_WRITE_CLOSED_FLAG;

                /* Return success to the user. */
                lRetVal = SOCKETS_ERROR_NONE;
                break;

            default:
                /* An invalid value was passed for ulHow. */
                lRetVal = SOCKETS_EINVAL;
                break;
        }
    }

    return lRetVal;
}
/*-----------------------------------------------------------*/

Socket_t SOCKETS_Socket( int32_t lDomain,
                         int32_t lType,
                         int32_t lProtocol )
{
    int32_t lStatus = SOCKETS_ERROR_NONE;
    int32_t xSocket = 0;
    SSOCKETContextPtr_t pxContext = NULL;

    /* Ensure that only supported values are supplied. */
    configASSERT( lDomain == SOCKETS_AF_INET );
    configASSERT( lType == SOCKETS_SOCK_STREAM );
    configASSERT( lProtocol == SOCKETS_IPPROTO_TCP );

    /* Allocate the internal context structure. */
    pxContext = pvPortMalloc( sizeof( SSOCKETContext_t ) );

    if( pxContext != NULL )
    {
        memset( pxContext, 0, sizeof( SSOCKETContext_t ) );

        /* Create the wrapped socket. */
        xSocket = lwip_socket(lDomain, lType, lProtocol);
        if( xSocket >= 0 )
        {
            pxContext->xSocket = ( Socket_t ) xSocket;
            /* Set default timeouts. */
            pxContext->ulRecvTimeout = socketsconfigDEFAULT_RECV_TIMEOUT;
            pxContext->ulSendTimeout = socketsconfigDEFAULT_SEND_TIMEOUT;
        }
        else /* Driver could not allocate socket. */
        {
            lStatus = SOCKETS_SOCKET_ERROR;
            vPortFree( pxContext );
        }
    }
    else /* Malloc failed. */
    {
        lStatus = SOCKETS_ENOMEM;
    }

    if( lStatus != SOCKETS_ERROR_NONE )
    {
        pxContext = SOCKETS_INVALID_SOCKET;
    }

    return pxContext;
}
/*-----------------------------------------------------------*/

BaseType_t SOCKETS_Init( void )
{
    /* Empty initialization for NXP board. */
    return pdPASS;
}
