/*
 * Copyright 2021, 2024 NXP
 *
 * License: NXP LA_OPT_Online Code Hosting NXP_Software_License
 *
 * NXP Proprietary. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.
 *
 * FreeMASTER Communication Driver - Network LWIP TCP driver
 */

#include "freemaster.h"
#include "freemaster_private.h"

/* Compile this code only if the NET driver is selected in freemaster_cfg.h. */
#if defined(FMSTR_NET_DRV) && (FMSTR_MK_IDSTR(FMSTR_NET_DRV) == FMSTR_NET_LWIP_TCP_ID)

#include "freemaster_net_lwip_tcp.h"

#if FMSTR_DISABLE == 0

#include "freemaster_protocol.h"
#include "freemaster_net.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/def.h"
#include "lwip/sockets.h"

/******************************************************************************
 * Adapter configuration
 ******************************************************************************/
#if (defined(FMSTR_SHORT_INTR) && FMSTR_SHORT_INTR) || (defined(FMSTR_LONG_INTR) && FMSTR_LONG_INTR)
#error The FreeMASTER network TCP lwip driver does not support interrupt mode.
#endif

/* santiy check of lwip options */
#if FMSTR_SESSION_COUNT > (MEMP_NUM_NETCONN - 1)
#error The MEMP_NUM_NETCONN (lwipopts.h) must be (FMSTR_SESSION_COUNT+1) or higher!
#endif

#if FMSTR_SESSION_COUNT > (MEMP_NUM_TCP_PCB_LISTEN - 1)
#error The MEMP_NUM_TCP_PCB_LISTEN (lwipopts.h) must be (FMSTR_SESSION_COUNT+1) or higher!
#endif

#if FMSTR_SESSION_COUNT > (MEMP_NUM_TCP_PCB - 1)
#error The MEMP_NUM_TCP_PCB (lwipopts.h) must be (FMSTR_SESSION_COUNT+1) or higher!
#endif

#if FMSTR_NET_AUTODISCOVERY != 0
#define FMSTR_TCP_SESSION_COUNT FMSTR_SESSION_COUNT + 1
#else
#define FMSTR_TCP_SESSION_COUNT FMSTR_SESSION_COUNT
#endif
/******************************************************************************
 * Local types
 ******************************************************************************/

typedef struct FMSTR_TCP_SESSION_S
{
    int sock;
    FMSTR_BOOL receivePending;
    FMSTR_NET_ADDR address;
} FMSTR_TCP_SESSION;

/******************************************************************************
 * Local functions
 ******************************************************************************/

static FMSTR_BOOL _FMSTR_NetLwipTcpInit(void);
static void _FMSTR_NetLwipTcpPoll(void);
static FMSTR_S32 _FMSTR_NetLwipTcpRecv(FMSTR_BPTR msgBuff,
                                       FMSTR_SIZE msgMaxSize,
                                       FMSTR_NET_ADDR *recvAddr,
                                       FMSTR_BOOL *isBroadcast);
static FMSTR_S32 _FMSTR_NetLwipTcpSend(FMSTR_NET_ADDR *sendAddr, FMSTR_BPTR msgBuff, FMSTR_SIZE msgSize);
static void _FMSTR_NetLwipTcpClose(FMSTR_NET_ADDR *addr);
static void _FMSTR_NetLwipTcpGetCaps(FMSTR_NET_IF_CAPS *caps);
static void _FMSTR_NetAddrToFmstr(struct sockaddr *remoteAddr, FMSTR_NET_ADDR *fmstrAddr);

/******************************************************************************
 * Local variables
 ******************************************************************************/

/* TCP sessions */
static FMSTR_TCP_SESSION fmstrTcpSessions[FMSTR_TCP_SESSION_COUNT];
/* TCP listen socket */
static int fmstrTcpListenSock = 0;

#if FMSTR_NET_AUTODISCOVERY != 0
/* UDP Broadcast socket */
static int fmstrUdpBroadcastSock = 0;
#endif /* FMSTR_NET_AUTODISCOVERY */

/******************************************************************************
 * Driver interface
 ******************************************************************************/
/* Interface of this network TCP driver */
const FMSTR_NET_DRV_INTF FMSTR_NET_LWIP_TCP = {
    .Init    = _FMSTR_NetLwipTcpInit,
    .Poll    = _FMSTR_NetLwipTcpPoll,
    .Recv    = _FMSTR_NetLwipTcpRecv,
    .Send    = _FMSTR_NetLwipTcpSend,
    .Close   = _FMSTR_NetLwipTcpClose,
    .GetCaps = _FMSTR_NetLwipTcpGetCaps,
};

/******************************************************************************
 * Implementation
 ******************************************************************************/

static FMSTR_BOOL _FMSTR_NetLwipTcpInit(void)
{
    struct sockaddr_in destAddr4;
#if FMSTR_NET_AUTODISCOVERY != 0
    struct sockaddr_in bindAddr;
#endif
    FMSTR_INDEX i;
    int err;

    FMSTR_MemSet(&fmstrTcpSessions, 0, sizeof(fmstrTcpSessions));
    FMSTR_MemSet(&destAddr4, 0, sizeof(destAddr4));
    destAddr4.sin_family = AF_INET; // TODO: prepare address for IPv4 or IPv6 ?

    /* Prepare sockets */
    for (i = 0; i < FMSTR_TCP_SESSION_COUNT; i++)
    {
        fmstrTcpSessions[i].sock = -1;
    }

    /* TCP listen port */
    destAddr4.sin_port = htons(FMSTR_NET_PORT);

    /* Create new listen socket */
    fmstrTcpListenSock = lwip_socket(AF_INET, SOCK_STREAM, IPPROTO_IP); // TODO: IPv6?
    if (fmstrTcpListenSock < 0)
    {
        return FMSTR_FALSE;
    }

#if FMSTR_NET_BLOCKING_TIMEOUT == 0
    {
        /* Set non-blocking socket */
        int flags = fcntl(fmstrTcpListenSock, F_GETFL, 0);
        err       = fcntl(fmstrTcpListenSock, F_SETFL, flags | O_NONBLOCK);
        if (err < 0)
        {
            return FMSTR_FALSE;
        }
    }
#endif

    /* Socket bind */
    err = lwip_bind(fmstrTcpListenSock, (struct sockaddr *)&destAddr4, sizeof(destAddr4));
    if (err < 0)
    {
        return FMSTR_FALSE;
    }

    /* Listen */
    err = lwip_listen(fmstrTcpListenSock, 0);
    if (err < 0)
    {
        return FMSTR_FALSE;
    }

#if FMSTR_NET_AUTODISCOVERY != 0

    bindAddr.sin_family      = AF_INET;
    bindAddr.sin_len         = (u8_t)sizeof(bindAddr);
    bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bindAddr.sin_port        = htons(FMSTR_NET_PORT);

    /* Create new UDP listen socket */
    fmstrUdpBroadcastSock = lwip_socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (fmstrUdpBroadcastSock < 0)
    {
        return FMSTR_FALSE;
    }

    /* Socket bind */
    err = lwip_bind(fmstrUdpBroadcastSock, (struct sockaddr *)&bindAddr, sizeof(bindAddr));
    if (err < 0)
    {
        return FMSTR_FALSE;
    }

#if FMSTR_NET_BLOCKING_TIMEOUT == 0
    {
        /* Set non-blocking socket */
        int flags = fcntl(fmstrUdpBroadcastSock, F_GETFL, 0);
        err       = fcntl(fmstrUdpBroadcastSock, F_SETFL, flags | O_NONBLOCK);
        if (err < 0)
        {
            return FMSTR_FALSE;
        }
    }
#endif

    fmstrTcpSessions[0].sock = fmstrUdpBroadcastSock;

#endif /* FMSTR_NET_AUTODISCOVERY */

    return FMSTR_TRUE;
}

static FMSTR_TCP_SESSION *_FMSTR_NetLwipTcpSessionPending(void)
{
    FMSTR_INDEX i;

    for (i = 0; i < FMSTR_TCP_SESSION_COUNT; i++)
    {
        /* Find pending session */
        if (fmstrTcpSessions[i].sock >= 0 && fmstrTcpSessions[i].receivePending != FMSTR_FALSE)
        {
            return &fmstrTcpSessions[i];
        }
    }

    return NULL;
}

static FMSTR_TCP_SESSION *_FMSTR_NetLwipTcpSessionFind(FMSTR_NET_ADDR *sendAddr)
{
    FMSTR_INDEX i;

    for (i = 0; i < FMSTR_TCP_SESSION_COUNT; i++)
    {
        /* Find free session */
        if (sendAddr == NULL)
        {
            if (fmstrTcpSessions[i].sock < 0)
            {
                return &fmstrTcpSessions[i];
            }
        }
        /* Find session by address */
        else
        {
            if (FMSTR_MemCmp(&fmstrTcpSessions[i].address, sendAddr, sizeof(FMSTR_NET_ADDR)) == 0)
            {
                return &fmstrTcpSessions[i];
            }
        }
    }

    return NULL;
}

static void _FMSTR_NetLwipTcpAccept(void)
{
    struct sockaddr remote_addr;
    socklen_t length;
    int newSock = 0;

    FMSTR_MemSet(&remote_addr, 0, sizeof(remote_addr));
    length = sizeof(remote_addr);

    /* Accept socket */
    newSock = lwip_accept(fmstrTcpListenSock, &remote_addr, &length);
    if (newSock >= 0)
    {
        FMSTR_TCP_SESSION *newSes;

#if FMSTR_NET_BLOCKING_TIMEOUT == 0
        /* Set non-blocking socket */
        int flags = fcntl(newSock, F_GETFL, 0);
        fcntl(newSock, F_SETFL, flags | O_NONBLOCK);
#endif

        newSes = _FMSTR_NetLwipTcpSessionFind(NULL);
        if (newSes != NULL)
        {
            newSes->sock = newSock;
            _FMSTR_NetAddrToFmstr(&remote_addr, &newSes->address);
        }
    }
}

static void _FMSTR_NetLwipTcpPoll(void)
{
    FMSTR_TCP_SESSION *freeSes;
    FMSTR_INDEX i;

    /* Any session is still pending to read */
    if (_FMSTR_NetLwipTcpSessionPending() != NULL)
    {
        return;
    }

    /* Find if free session available */
    freeSes = _FMSTR_NetLwipTcpSessionFind(NULL);

#if FMSTR_NET_BLOCKING_TIMEOUT == 0
    if (freeSes != NULL)
    {
        _FMSTR_NetLwipTcpAccept();
    }

    /* Set all active sessions to read pending */
    for (i = 0; i < FMSTR_TCP_SESSION_COUNT; i++)
    {
        if (fmstrTcpSessions[i].sock >= 0)
        {
            fmstrTcpSessions[i].receivePending = FMSTR_TRUE;
        }
    }
#else
    {
        int maxFd = 0;
        fd_set readset;
        struct timeval tv;

        FD_ZERO(&readset);
        if (freeSes != NULL)
        {
            /* Listen socket */
            FD_SET(fmstrTcpListenSock, &readset);
            maxFd = fmstrTcpListenSock;
        }

        /* set timeout */
        tv.tv_sec  = FMSTR_NET_BLOCKING_TIMEOUT / 1000;
        tv.tv_usec = (FMSTR_NET_BLOCKING_TIMEOUT % 1000) * 1000;

        /* Prepare active sockets for read */
        for (i = 0; i < FMSTR_TCP_SESSION_COUNT; i++)
        {
            if (fmstrTcpSessions[i].sock >= 0)
            {
                FD_SET(fmstrTcpSessions[i].sock, &readset);

                if (maxFd < fmstrTcpSessions[i].sock)
                {
                    maxFd = fmstrTcpSessions[i].sock;
                }
            }
        }

        if (lwip_select(maxFd + 1, (fd_set *)&readset, NULL, NULL, &tv) >= 1)
        {
            /* Pending accept */
            if (FD_ISSET(fmstrTcpListenSock, &readset))
            {
                _FMSTR_NetLwipTcpAccept();
            }

            /* Pending receive */
            for (i = 0; i < FMSTR_TCP_SESSION_COUNT; i++)
            {
                if (fmstrTcpSessions[i].sock >= 0 && FD_ISSET(fmstrTcpSessions[i].sock, &readset))
                {
                    fmstrTcpSessions[i].receivePending = FMSTR_TRUE;
                }
            }
        }
    }
#endif
}

static FMSTR_S32 _FMSTR_NetLwipTcpRecv(FMSTR_BPTR msgBuff,
                                       FMSTR_SIZE msgMaxSize,
                                       FMSTR_NET_ADDR *recvAddr,
                                       FMSTR_BOOL *isBroadcast)
{
    int res                = 0;
    FMSTR_TCP_SESSION *ses = NULL;

    FMSTR_ASSERT(msgBuff != NULL);
    FMSTR_ASSERT(recvAddr != NULL);
    FMSTR_ASSERT(isBroadcast != NULL);

    *isBroadcast = FMSTR_FALSE;

    if (fmstrTcpListenSock < 0)
    {
        return 0;
    }

    /* Any receive pending? */
    ses = _FMSTR_NetLwipTcpSessionPending();
    if (ses == NULL)
    {
        return 0;
    }

#if FMSTR_NET_AUTODISCOVERY != 0
    /* Receive UDP broadcast */
    if (ses->sock == fmstrUdpBroadcastSock)
    {
        struct sockaddr remote_addr;
        socklen_t length = sizeof(remote_addr);

        *isBroadcast = FMSTR_TRUE;

        res = lwip_recvfrom(ses->sock, msgBuff, msgMaxSize, 0, &remote_addr, &length);

        /* Copy address */
        _FMSTR_NetAddrToFmstr(&remote_addr, &ses->address);
        /* Copy address */
        FMSTR_MemCpy(recvAddr, &ses->address, sizeof(FMSTR_NET_ADDR));
    }
    else
#endif /* FMSTR_NET_AUTODISCOVERY */
    {
        res = lwip_recv(ses->sock, msgBuff, msgMaxSize, 0);

        /* Copy address */
        FMSTR_MemCpy(recvAddr, &ses->address, sizeof(FMSTR_NET_ADDR));
    }
#if FMSTR_NET_BLOCKING_TIMEOUT != 0
    if (res == 0)
    {
        res = -1;
    }
#else
    if (res < 0 && errno == EWOULDBLOCK)
    {
        res = 0;
    }
    if (res == 0)
    {
        ses->receivePending = FMSTR_FALSE;
    }
#endif
    if (res < 0 && errno == EWOULDBLOCK)
    {
        res = 0;
    }
    if (res == 0)
    {
        ses->receivePending = FMSTR_FALSE;
    }

    return res;
}

static FMSTR_S32 _FMSTR_NetLwipTcpSend(FMSTR_NET_ADDR *sendAddr, FMSTR_BPTR msgBuff, FMSTR_SIZE msgSize)
{
    FMSTR_TCP_SESSION *ses = NULL;
    int res                = 0;

    FMSTR_ASSERT(msgBuff != NULL);
    FMSTR_ASSERT(sendAddr != NULL);

    // TODO: what should do, when async discovery want to send (not found session)

    /* Find session by address */
    ses = _FMSTR_NetLwipTcpSessionFind(sendAddr);
    if (ses == NULL)
    {
        /* Same as socket error */
        return -1;
    }

    /* This session is not pending now */
    ses->receivePending = FMSTR_FALSE;

#if FMSTR_NET_AUTODISCOVERY != 0
    /* Receive UDP broadcast */
    if (ses->sock == fmstrUdpBroadcastSock)
    {
        struct sockaddr_in destAddr4;
        FMSTR_MemSet(&destAddr4, 0, sizeof(destAddr4));
        destAddr4.sin_len    = (u8_t)sizeof(destAddr4);
        destAddr4.sin_family = AF_INET;
        destAddr4.sin_port   = htons(sendAddr->port);
        FMSTR_MemCpy(&destAddr4.sin_addr.s_addr, sendAddr->addr.v4, 4);

        /* Send data */
        res = lwip_sendto(ses->sock, msgBuff, msgSize, 0, (struct sockaddr *)&destAddr4, sizeof(destAddr4));
    }
    else
#endif
    {
        /* Send data */
        res = lwip_send(ses->sock, msgBuff, msgSize, 0);
    }

    return res;
}

static void _FMSTR_NetLwipTcpClose(FMSTR_NET_ADDR *addr)
{
    FMSTR_TCP_SESSION *ses = NULL;

    /* Find session by address */
    ses = _FMSTR_NetLwipTcpSessionFind(addr);
    if (ses == NULL)
    {
        /* Session not found */
        return;
    }

#if FMSTR_NET_AUTODISCOVERY != 0
    if (ses->sock == fmstrUdpBroadcastSock)
    {
        /* Broadcast session cannot be closed */
        return;
    }
#endif

    /* Close socket */
    (void)lwip_close(ses->sock);

    FMSTR_MemSet(ses, 0, sizeof(FMSTR_TCP_SESSION));
    ses->sock = -1;
}

static void _FMSTR_NetLwipTcpGetCaps(FMSTR_NET_IF_CAPS *caps)
{
    FMSTR_ASSERT(caps != NULL);

    caps->flags |= FMSTR_NET_IF_CAPS_FLAG_TCP;
}

static void _FMSTR_NetAddrToFmstr(struct sockaddr *remoteAddr, FMSTR_NET_ADDR *fmstrAddr)
{
    FMSTR_ASSERT(remoteAddr != NULL);
    FMSTR_ASSERT(fmstrAddr != NULL);

    if ((((struct sockaddr *)remoteAddr)->sa_family & AF_INET) != 0U)
    {
        struct sockaddr_in *in = (struct sockaddr_in *)remoteAddr;
        fmstrAddr->type        = FMSTR_NET_ADDR_TYPE_V4;
        FMSTR_MemCpy(fmstrAddr->addr.v4, &in->sin_addr.s_addr, sizeof(fmstrAddr->addr.v4));
        fmstrAddr->port = htons(in->sin_port);
    }
#if LWIP_IPV6
    else if ((((struct sockaddr *)remoteAddr)->sa_family & AF_INET6) != 0U)
    {
        struct sockaddr_in6 *in6 = (struct sockaddr_in6 *)remoteAddr;

        fmstrAddr->type = FMSTR_NET_ADDR_TYPE_V6;
        FMSTR_MemCpy(fmstrAddr->addr.v6, &(in6->sin6_addr.s6_addr), sizeof(fmstrAddr->addr.v6));
        fmstrAddr->port = htons(in6->sin6_port);
    }
#endif
}

#endif /* (!(FMSTR_DISABLE)) */
#endif /* defined(FMSTR_NET_DRV) && (FMSTR_MK_IDSTR(FMSTR_NET_DRV) == FMSTR_NET_MCUX_LWIP_TCP_ID) */
