/*
 * Copyright 2021 NXP
 *
 * License: NXP LA_OPT_NXP_Software_License
 *
 * NXP Confidential. This software is owned or controlled by NXP and may
 * only be used strictly in accordance with the applicable license terms.
 * By expressly accepting such terms or by downloading, installing,
 * activating and/or otherwise using the software, you are agreeing that
 * you have read, and that you agree to comply with and are bound by,
 * such license terms.  If you do not agree to be bound by the applicable
 * license terms, then you may not retain, install, activate or otherwise
 * use the software.  This code may only be used in a microprocessor,
 * microcontroller, sensor or digital signal processor ("NXP Product")
 * supplied directly or indirectly from NXP.  See the full NXP Software
 * License Agreement in license/LA_OPT_NXP_Software_License.pdf
 *
 * FreeMASTER Communication Driver - Network communication
 */

#include "freemaster.h"
#include "freemaster_private.h"

/* Numeric identifier to help pre-processor to identify whether our transport is used or not. */
#define FMSTR_NET_ID 1

#if (FMSTR_MK_IDSTR(FMSTR_TRANSPORT) == FMSTR_NET_ID) && FMSTR_DISABLE == 0

#include "freemaster_protocol.h"
#include "freemaster_net.h"
#include "freemaster_utils.h"

/* Offset command payload in network buffer */
#define FMSTR_NET_PAYLOAD_OFFSET 6

typedef struct FMSTR_NET_SESSION_S
{
    FMSTR_U32 lastUsed;     /* Last used session */
    FMSTR_NET_ADDR address; /* TCP/UDP address */
} FMSTR_NET_SESSION;

/***********************************
 *  local variables
 ***********************************/

/* Network sessions */
static FMSTR_NET_SESSION fmstr_pNetSessions[FMSTR_SESSION_COUNT];
static FMSTR_U32 fmstr_nSessionCounter = 0U;

/* FreeMASTER communication buffer (in/out) plus the Length, sequence number, command, payload and CRC */
static FMSTR_BCHR fmstr_pNetBuffer[FMSTR_COMM_BUFFER_SIZE + 2 + 1 + 1 + 2 + 1];

static FMSTR_SIZE fmstr_nReceived = 0U;
static FMSTR_U8 fmstr_nSeqNumber  = 1U;

/***********************************
 *  local function prototypes
 ***********************************/

/* Interface function - Initialization of serial transport */
static FMSTR_BOOL _FMSTR_NetInit(void);
/* Interface function - Poll function of serial transport */
static void _FMSTR_NetPoll(void);
/* Interface function - Send Response function of serial transport */
static void _FMSTR_NetSendResponse(FMSTR_BPTR pResponse, FMSTR_SIZE nLength, FMSTR_U8 statusCode, void *identification);

/* Session find and use or reuse */
static FMSTR_NET_SESSION *_FMSTR_FindNetSession(FMSTR_NET_ADDR *addr, FMSTR_BOOL create);
/* Close session */
static void _FMSTR_NetCloseSession(FMSTR_NET_SESSION *ses);

static FMSTR_BOOL _FMSTR_NetProcess(void);
static void _FMSTR_NetSendStatus(FMSTR_BCHR nErrCode, FMSTR_NET_SESSION *session);

#if FMSTR_NET_AUTODISCOVERY != 0
static void _FMSTR_NetSendDiscovery(const FMSTR_NET_ADDR *address);
#endif /* FMSTR_NET_AUTODISCOVERY */

/***********************************
 *  global variables
 ***********************************/

/* Interface of this serial driver */
const FMSTR_TRANSPORT_INTF FMSTR_NET = {
    FMSTR_C99_INIT(Init) _FMSTR_NetInit,
    FMSTR_C99_INIT(Poll) _FMSTR_NetPoll,
    FMSTR_C99_INIT(SendResponse) _FMSTR_NetSendResponse,
};

/******************************************************************************
 *
 * @brief    Network communication initialization
 *
 ******************************************************************************/

static FMSTR_BOOL _FMSTR_NetInit(void)
{
    /* Check the interface if it's valid */
    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.Init != NULL, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.Poll != NULL, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.Recv != NULL, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.Send != NULL, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.Close != NULL, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_NET_DRV.GetCaps != NULL, FMSTR_FALSE);

    FMSTR_MemSet(&fmstr_pNetSessions, 0, sizeof(fmstr_pNetSessions));

    /* Call initialization of network driver */
    if (FMSTR_NET_DRV.Init() == FMSTR_FALSE)
    {
        return FMSTR_FALSE;
    }

    return FMSTR_FALSE;
}

/*******************************************************************************
 *
 * @brief    API: Network Transport "Polling" call from the application main loop
 *
 * This function either handles all the NET communication (polling-only mode =
 * FMSTR_POLL_DRIVEN) or decodes messages received on the background by NET interrupt
 * (short-interrupt mode = FMSTR_SHORT_INTR).
 *
 *******************************************************************************/

static void _FMSTR_NetPoll(void)
{
    FMSTR_BOOL res = FMSTR_FALSE;

    do
    {
        /* Poll network driver */
        FMSTR_NET_DRV.Poll();

        /* Process the protocol */
        res = _FMSTR_NetProcess();

        /* Repeat until the current frame is fully processed (or communication is idle) */
    } while (res == FMSTR_FALSE);
}

/******************************************************************************
 *
 * @brief    Finalize transmit buffer before transmitting
 *
 * @param    nLength - response length (1 for status + data length)
 *
 *
 * This Function takes the data already prepared in the transmit buffer
 * (inlcuding the status byte). It computes the check sum and kicks on tx.
 *
 ******************************************************************************/

static void _FMSTR_NetSendResponse(FMSTR_BPTR pResponse, FMSTR_SIZE nLength, FMSTR_U8 statusCode, void *identification)
{
    FMSTR_U16 i;
    FMSTR_U8 c;
    FMSTR_U16 todo;
    FMSTR_U16 sent   = 0U;
    FMSTR_S32 res    = 1;
    FMSTR_BCHR chSum = 0U;
    FMSTR_BPTR pMessageIO;

    FMSTR_NET_SESSION *session = (FMSTR_NET_SESSION *)identification;
    FMSTR_ASSERT(session != NULL);

    if ((nLength > (FMSTR_SIZE)(FMSTR_COMM_BUFFER_SIZE)) || pResponse != &fmstr_pNetBuffer[FMSTR_NET_PAYLOAD_OFFSET])
    {
        /* The Network driver doesn't support bigger responses than FMSTR_COMM_BUFFER_SIZE bytes, change the response to
         * status error */
        statusCode = FMSTR_STC_RSPBUFFOVF;
        nLength    = 0U;
    }

    pMessageIO = &fmstr_pNetBuffer[0];

    /* Send the message with status, length and checksum. */
    todo = (FMSTR_U16)(nLength + 7U);

    /* Total frame length */
    pMessageIO = FMSTR_ValueToBuffer16BE(pMessageIO, todo);

    /* Sequence number */
    pMessageIO = FMSTR_ValueToBuffer8(pMessageIO, fmstr_nSeqNumber);

    /* Status code */
    pMessageIO = FMSTR_ValueToBuffer8(pMessageIO, (FMSTR_BCHR)statusCode);

    /* Response length */
    pMessageIO = FMSTR_ValueToBuffer16BE(pMessageIO, (FMSTR_U16)nLength);

    /* Initialize CRC algorithms */
    FMSTR_Crc8Init(&chSum);

    /* Checksum CRC8 */
    pResponse = &fmstr_pNetBuffer[3];
    for (i = 0; i < nLength + 3U; i++)
    {
        /* Get from FMSTR buffer */
        pResponse = FMSTR_ValueFromBuffer8(&c, pResponse);

        /* add character to checksum */
        FMSTR_Crc8AddByte(&chSum, c);
    }

    /* Store checksum after the message */
    pResponse = FMSTR_ValueToBuffer8(pResponse, chSum);

    /* Send via network */
    while (res > 0 && sent < todo)
    {
        res = FMSTR_NET_DRV.Send(&session->address, &fmstr_pNetBuffer[sent], todo - sent);

        if (res < 0)
        {
            /* Socket error condition */
            _FMSTR_NetCloseSession(session);
            return;
        }

        sent += (FMSTR_U16)res;
    }
}

/******************************************************************************
 *
 * @brief    Process network communication
 *
 * This function returns TRUE when network transmission is complete.
 * It returns FALSE when received middle of the frame (TCP) and needs to be
 * called asap after any new data are received.
 *
 ******************************************************************************/

static FMSTR_BOOL _FMSTR_NetProcess(void)
{
    FMSTR_BCHR chSum = 0U, crc;
    FMSTR_U16 i;
    FMSTR_U8 c;
    int received   = 0;
    FMSTR_U16 todo = 0;
    FMSTR_BPTR pMessageIO, pCmdPayload, pCrc;
    FMSTR_U8 cmd = 0;
    FMSTR_U16 nLength;
    FMSTR_BOOL isBroadcast = FMSTR_FALSE;

    FMSTR_NET_SESSION *session = NULL;
    FMSTR_NET_ADDR address;
    FMSTR_MemSet(&address, 0, sizeof(address));

    /* Receive data */
    received = FMSTR_NET_DRV.Recv(&fmstr_pNetBuffer[fmstr_nReceived], (FMSTR_COMM_BUFFER_SIZE + 7) - fmstr_nReceived,
                                  &address, &isBroadcast);

    if (received < 0)
    {
        /* Find session, but not create it */
        session = _FMSTR_FindNetSession(&address, FMSTR_FALSE);
        if (session != NULL)
        {
            _FMSTR_NetCloseSession(session);
        }
    }

    if (received <= 0)
    {
        return FMSTR_TRUE;
    }

    fmstr_nReceived += (FMSTR_SIZE)received;

    if (isBroadcast == FMSTR_FALSE)
    {
        /* Find session and create it */
        session = _FMSTR_FindNetSession(&address, FMSTR_TRUE);
    }

    pMessageIO = &fmstr_pNetBuffer[0];

    /* Received total length */
    if (fmstr_nReceived < 2U)
    {
        return FMSTR_FALSE;
    }

    pMessageIO = FMSTR_ValueFromBuffer16BE(&todo, pMessageIO);

    /* Sanity check of todo length */
    if (todo > ((FMSTR_U16)FMSTR_COMM_BUFFER_SIZE + 7U))
    {
        if (session != NULL)
        {
            _FMSTR_NetCloseSession(session);
        }
        return FMSTR_TRUE;
    }

    /* Not enough data in buffer */
    if (fmstr_nReceived < todo)
    {
        return FMSTR_FALSE;
    }

    /* Initialize CRC algorithms */
    FMSTR_Crc8Init(&chSum);

    /* Sequence number */
    pMessageIO = FMSTR_ValueFromBuffer8(&fmstr_nSeqNumber, pMessageIO);

    /* Pointer to start checking CRC */
    pCrc = pMessageIO;

    /* Command code */
    pMessageIO = FMSTR_ValueFromBuffer8(&cmd, pMessageIO);

    /* Command length */
    pMessageIO = FMSTR_ValueFromBuffer16BE(&nLength, pMessageIO);

    /* Bad data length */
    if (todo != (nLength + 7U))
    {
        if (session != NULL)
        {
            _FMSTR_NetCloseSession(session);
        }
        return FMSTR_TRUE;
    }

    /* Command payload */
    pCmdPayload = pMessageIO;

    /* Skip command length for get CRC */
    pMessageIO = FMSTR_SkipInBuffer(pMessageIO, nLength);

    /* CRC */
    pMessageIO = FMSTR_ValueFromBuffer8(&crc, pMessageIO);

    /* Count CRC */
    for (i = 0; i < nLength + 3U; i++)
    {
        /* Get from FMSTR buffer */
        pCrc = FMSTR_ValueFromBuffer8(&c, pCrc);

        /* add character to checksum */
        FMSTR_Crc8AddByte(&chSum, c);
    }

    /* Checksum */
    if (crc == chSum)
    {
        /* Network PING command */
        if (cmd == FMSTR_NET_PING)
        {
            if (isBroadcast == FMSTR_FALSE)
            {
                /* PING response to unicast request. */
                _FMSTR_NetSendStatus(FMSTR_STS_OK, session);
            }
        }
#if FMSTR_NET_AUTODISCOVERY != 0
        /* Network auto-discovery command */
        else if (cmd == FMSTR_NET_DISCOVERY)
        {
            /* Send discovery (also to broadcast) */
            _FMSTR_NetSendDiscovery(&address);

            /* Invalidate the session immediatelly, so it is reused next time */
            if (session != NULL)
            {
                session->lastUsed = 0U;
            }
        }
#endif
        /* Decode protocol only when not receive data from broadcast */
        else
        {
            /* Decode protocol only for unicast communiction */
            if (isBroadcast == FMSTR_FALSE)
            {
                (void)FMSTR_ProtocolDecoder(pCmdPayload, nLength, cmd, session);
            }
        }
    }
    else
    {
        /* Report error only for unicast communiction */
        if (isBroadcast == FMSTR_FALSE)
        {
            _FMSTR_NetSendStatus(FMSTR_STC_CMDCSERR, session);
        }
    }

    fmstr_nReceived = 0U;

    return FMSTR_TRUE;
}

#if FMSTR_NET_AUTODISCOVERY != 0
static void _FMSTR_NetSendDiscovery(const FMSTR_NET_ADDR *address)
{
    FMSTR_NET_IF_CAPS caps;
    FMSTR_NET_SESSION session;
    FMSTR_BPTR pMessageIO;
    FMSTR_U8 nameLen  = 0;
    FMSTR_U8 protocol = 0;

    /* Get protocol from low-level */
    FMSTR_MemSet(&caps, 0, sizeof(caps));
    FMSTR_NET_DRV.GetCaps(&caps);
    if ((caps.flags & FMSTR_NET_IF_CAPS_FLAG_UDP) != 0U)
    {
        protocol = (FMSTR_U8)FMSTR_NET_PROTOCOL_UDP;
    }
    else if ((caps.flags & FMSTR_NET_IF_CAPS_FLAG_TCP) != 0U)
    {
        protocol = (FMSTR_U8)FMSTR_NET_PROTOCOL_TCP;
    }
    else
    {
        FMSTR_ASSERT(FMSTR_FALSE);
    }

    FMSTR_MemCpy(&session.address, address, sizeof(FMSTR_NET_ADDR));

    /* Lenght of board name */
    nameLen = (FMSTR_U8)(FMSTR_StrLen(FMSTR_APPLICATION_STR) + 1U);
    if (nameLen > ((FMSTR_U8)FMSTR_COMM_BUFFER_SIZE - 3U))
    {
        nameLen = (FMSTR_U8)FMSTR_COMM_BUFFER_SIZE - 3U;
    }

    pMessageIO = &fmstr_pNetBuffer[6];

    /* Discovery command version */
    pMessageIO = FMSTR_ValueToBuffer8(pMessageIO, FMSTR_NET_DISCOVERY_VERSION);

    /* Protocol (TCP/UDP) */
    pMessageIO = FMSTR_ValueToBuffer8(pMessageIO, protocol);

    /* Length of board name */
    pMessageIO = FMSTR_ValueToBuffer8(pMessageIO, nameLen);

    /* Copy name */
    pMessageIO = FMSTR_CopyToBuffer(pMessageIO, (FMSTR_ADDR)(char *)FMSTR_APPLICATION_STR, nameLen);

    /* send response */
    _FMSTR_NetSendResponse(&fmstr_pNetBuffer[6], (FMSTR_SIZE)nameLen + 3U, FMSTR_STS_OK, &session);
}
#endif /* FMSTR_NET_AUTODISCOVERY */

static void _FMSTR_NetSendStatus(FMSTR_BCHR nErrCode, FMSTR_NET_SESSION *session)
{
    /* fill & send single-byte response */
    _FMSTR_NetSendResponse(&fmstr_pNetBuffer[6], 0U, nErrCode, session);
}

static void _FMSTR_NetCloseSession(FMSTR_NET_SESSION *ses)
{
    FMSTR_ASSERT(ses != NULL);

    /* Close socket */
    FMSTR_NET_DRV.Close(&ses->address);

    /* Free protocol session, if closed socket */
    FMSTR_FreeSession(ses);
}

static FMSTR_NET_SESSION *_FMSTR_FindNetSession(FMSTR_NET_ADDR *addr, FMSTR_BOOL create)
{
    FMSTR_NET_SESSION *ses           = NULL;
    FMSTR_NET_SESSION *freeSession   = NULL;
    FMSTR_NET_SESSION *oldestSession = NULL;
    FMSTR_INDEX i;

    FMSTR_ASSERT(addr != NULL);

    fmstr_nSessionCounter++;

    for (i = 0; i < FMSTR_SESSION_COUNT; i++)
    {
        ses = &fmstr_pNetSessions[i];

        /* Find session by address */
        if (FMSTR_MemCmp(&ses->address, addr, sizeof(FMSTR_NET_ADDR)) == 0)
        {
            /* Found session */
            ses->lastUsed = fmstr_nSessionCounter;
            return ses;
        }

        /* Find free session */
        if (freeSession == NULL && ses->lastUsed == 0U)
        {
            freeSession = ses;
        }

        /* Find oldest session */
        if (oldestSession == NULL || oldestSession->lastUsed > ses->lastUsed)
        {
            oldestSession = ses;
        }
    }

    ses = (freeSession != NULL ? freeSession : oldestSession);

    if (ses != NULL && create != FMSTR_FALSE)
    {
        /* If reusing last used session, call protocol to free this session */
        if (ses->lastUsed != 0U)
        {
            _FMSTR_NetCloseSession(ses);
        }

        FMSTR_MemCpy(&ses->address, addr, sizeof(FMSTR_NET_ADDR));
        ses->lastUsed = fmstr_nSessionCounter;
    }

    return ses;
}

#endif /* !(FMSTR_DISABLE) */
