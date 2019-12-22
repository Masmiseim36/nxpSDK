/*
 * Copyright (c) 2007-2015 Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
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
 * FreeMASTER Communication Driver - protocol implementation
 */

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_protocol.h"
#include "freemaster_utils.h"

#if !(FMSTR_DISABLE)

/**************************************************************************//*!
* Local Functions
******************************************************************************/

/* The functions are local, but not declared "static" to prevent linker warnings
   when features are excluded from protocol. */
FMSTR_CHAR* _FMSTR_GetAccessPassword(FMSTR_U8 requiredAccess);
FMSTR_BPTR _FMSTR_GetBoardConfig(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR _FMSTR_AuthenticationStep1(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR _FMSTR_AuthenticationStep2(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR _FMSTR_ReadMem(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR _FMSTR_ReadMemBaseAddress(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);
FMSTR_BPTR _FMSTR_WriteMem(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus);

/**************************************************************************//*!
* Local variables
******************************************************************************/

#if FMSTR_DEBUG_TX
/* This warning is for you not to forget to disable the DEBUG_TX mode once the
 * communication line works fine. You can ignore this warning if it is okay
 * that the application will be periodically sending test frames to the world. */
#warning FMSTR_DEBUG_TX is enabled. Test frames will be transmitted periodically.
/* When communication debugging mode is requested, this global variable is used to
   turn the debugging off once a valid connection is detected */
FMSTR_BOOL fmstr_doDebugTx;
#endif

#if FMSTR_CFG_F1_RESTRICTED_ACCESS
#define SHA1_BLOCK_SIZE 20
/* Salt challenge value used between AUTH1 and AUTH2 steps of password authentication. */
static FMSTR_U8 fmstr_authSalt[FMSTR_AUTHENT_PRTCL_SHA1_SALT_LEN];
/* Currently granted access (one of FMSTR_RESTRICTED_ACCESS_xxx). */
static FMSTR_U8 fmstr_grantedAccess;
/* To save stack usage, we have the following SHA context variables static, although
 * they could be local in Auth2 function. */
static FMSTR_SHA1_CTX fmstr_sha1Ctx;
static FMSTR_U8 fmstr_accessKey[SHA1_BLOCK_SIZE];
static FMSTR_U8 fmstr_localKey[SHA1_BLOCK_SIZE];
#endif /* FMSTR_CFG_F1_RESTRICTED_ACCESS */

/**************************************************************************//*!
*
* @brief    FreeMASTER driver initialization
*
******************************************************************************/

FMSTR_BOOL FMSTR_Init(void)
{
    /* Check the transport interface validity */
    FMSTR_ASSERT_RETURN(FMSTR_TRANSPORT.Init, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_TRANSPORT.Poll, FMSTR_FALSE);
    FMSTR_ASSERT_RETURN(FMSTR_TRANSPORT.SendResponse, FMSTR_FALSE);

#if FMSTR_USE_TSA
    /* initialize TSA */
    FMSTR_InitTsa();
#endif

#if FMSTR_USE_SCOPE
    /* initialize Scope */
    FMSTR_InitScope();
#endif

#if FMSTR_USE_RECORDER
    /* initialize Recorder */
    FMSTR_InitRec();
#endif

#if FMSTR_USE_APPCMD
    /* initialize application commands */
    FMSTR_InitAppCmds();
#endif

#if FMSTR_USE_PIPES
    /* initialize PIPES interface */
    FMSTR_InitPipes();
#endif

    /* initialize communication and start listening for commands */
    if (!FMSTR_TRANSPORT.Init())
        return FMSTR_FALSE;

#if FMSTR_DEBUG_TX
    /* start in the debugging mode, we will be sending test frames periodically */
    fmstr_doDebugTx = FMSTR_TRUE;
#endif

#if FMSTR_CFG_F1_RESTRICTED_ACCESS
    /* Initialize the access protection variables */
    FMSTR_MemSet(fmstr_authSalt, 0xaa, sizeof(fmstr_authSalt));
    fmstr_grantedAccess = FMSTR_RESTRICTED_ACCESS_NO;

    /* set the access level to the highest "open" level (the one without a password) */
    for(int access=FMSTR_RESTRICTED_ACCESS_R; access<=FMSTR_RESTRICTED_ACCESS_RWF; access++)
    {
        if(_FMSTR_GetAccessPassword(access))
            break; /* this level (and all levels above) are protected by a password */
        else
            fmstr_grantedAccess = access; /* level not protected by a password is set by default */
    }
#endif

    return FMSTR_TRUE;
}

/*******************************************************************************
*
* @brief    API: Main "Polling" call from the application main loop
*
* This function either handles all the communication (polling-only mode =
* FMSTR_POLL_DRIVEN) or decodes messages received on the background by SCI interrupt
* (short-interrupt mode = FMSTR_SHORT_INTR).
*
*
*******************************************************************************/

void FMSTR_Poll(void)
{
    /* Increase entropy in each poll round */
#if FMSTR_CFG_F1_RESTRICTED_ACCESS
    static FMSTR_U8 e = 0x7;
    FMSTR_Randomize(e);
    e += 13;
#endif

    FMSTR_TRANSPORT.Poll();
}

/**************************************************************************//*!
*
* @brief    Finalize transmit buffer before transmitting
*
* @param    length - response length
*
*
* This Function takes the data already prepared in the transmit buffer
* (inlcuding the status byte). It computes the check sum and kicks on tx.
*
******************************************************************************/

void FMSTR_SendResponse(FMSTR_BPTR response, FMSTR_SIZE8 length, FMSTR_U8 statusCode)
{
    FMSTR_TRANSPORT.SendResponse(response, length, statusCode);
}

/**************************************************************************//*!
*
* @brief    Decodes the FreeMASTER protocol and calls appropriate handlers
*
* @param    msgBuffIO - message in/out buffer
*
* @return   TRUE if frame was valid and any output was generated to IO buffer
*
* This Function decodes given message and invokes proper command handler
* which fills in the response. The response transmission is initiated
* in this call as well.
*
******************************************************************************/

FMSTR_BOOL FMSTR_ProtocolDecoder(FMSTR_BPTR msgBuffIO, FMSTR_SIZE msgSize, FMSTR_U8 cmdCode)
{
    FMSTR_BPTR responseEnd = msgBuffIO;
    FMSTR_U8 statusCode = FMSTR_STS_INVALID;

    /* process command   */
    switch (cmdCode)
    {
        /* retrieve board configuration value*/
        case FMSTR_CMD_GETCONFIG:
            responseEnd = _FMSTR_GetBoardConfig(msgBuffIO, &statusCode);
            break;

#if FMSTR_USE_READMEM
        /* read a block of memory */
        case FMSTR_CMD_READMEM:
            responseEnd = _FMSTR_ReadMem(msgBuffIO, &statusCode);
            break;

#ifdef FMSTR_PLATFORM_BASE_ADDRESS
        /* read a block of memory with base address*/
        case FMSTR_CMD_READMEM_BA:
            responseEnd = _FMSTR_ReadMemBaseAddress(msgBuffIO, &statusCode);
            break;
#endif

#endif /* FMSTR_USE_READMEM */

#if FMSTR_USE_WRITEMEM

        /* write a block of memory */
        case FMSTR_CMD_WRITEMEM:
            responseEnd = _FMSTR_WriteMem(msgBuffIO, &statusCode);
            break;
#endif /* FMSTR_USE_WRITEMEM */

#if FMSTR_CFG_F1_RESTRICTED_ACCESS
        /* Initiate password authentication, request authentication challenge */
        case FMSTR_CMD_AUTH1:
            responseEnd = _FMSTR_AuthenticationStep1(msgBuffIO, &statusCode);
            break;

        /* Initiate password authentication, request authentication challenge */
        case FMSTR_CMD_AUTH2:
            responseEnd = _FMSTR_AuthenticationStep2(msgBuffIO, &statusCode);
            break;
#endif

#if FMSTR_USE_PIPES
        case FMSTR_CMD_GETPIPE:
            responseEnd = FMSTR_GetPipe(msgBuffIO, msgSize, &statusCode);
            break;
        case FMSTR_CMD_PIPE:
            responseEnd = FMSTR_PipeFrame(msgBuffIO, msgSize, &statusCode);
            break;
#endif /* FMSTR_USE_PIPES */

#if FMSTR_USE_TSA
        /* get TSA table (force EX instead of non-EX) */
        case FMSTR_CMD_GETTSAINFO:
            responseEnd = FMSTR_GetTsaInfo(msgBuffIO, &statusCode);
            break;

        case FMSTR_CMD_GETSTRLEN:
            responseEnd = FMSTR_GetStringLen(msgBuffIO, &statusCode);
            break;

        case FMSTR_CMD_URESRWI:
            responseEnd = FMSTR_UresControl(msgBuffIO, msgSize, &statusCode);
            break;
#endif /* FMSTR_USE_TSA */

#if FMSTR_USE_APPCMD
        /* accept the application command */
        case FMSTR_CMD_SENDAPPCMD:
            responseEnd = FMSTR_StoreAppCmd(msgBuffIO, msgSize, &statusCode);
            break;

        /* get the application command status */
        case FMSTR_CMD_GETAPPCMDSTS:
            responseEnd = FMSTR_GetAppCmdStatus(msgBuffIO, &statusCode);
            break;

        /* get the application command data */
        case FMSTR_CMD_GETAPPCMDDATA:
            responseEnd = FMSTR_GetAppCmdRespData(msgBuffIO, &statusCode);
            break;
#endif /* FMSTR_USE_APPCMD */

#if FMSTR_USE_RECORDER
        /* get recorder data */
        case FMSTR_CMD_GETREC:
            responseEnd = FMSTR_GetRecCmd(msgBuffIO, &statusCode);
            break;

        /* setup recorder */
        case FMSTR_CMD_SETREC:
            responseEnd = FMSTR_SetRecCmd(msgBuffIO, msgSize, &statusCode);
            break;
#endif /* FMSTR_USE_RECORDER */

#if FMSTR_USE_SCOPE
        /* prepare scope variables */
        case FMSTR_CMD_SETSCOPE:
            responseEnd = FMSTR_SetScope(msgBuffIO, msgSize, &statusCode);
            break;

        case FMSTR_CMD_READSCOPE:
            responseEnd = FMSTR_ReadScope(msgBuffIO, &statusCode, FMSTR_COMM_BUFFER_SIZE);
            break;
#endif /* FMSTR_USE_SCOPE */

        /* unknown command */
        default:
            statusCode = FMSTR_STC_INVCMD;
            break;
    }

    /* anything to send back? */
    if(statusCode != FMSTR_STS_INVALID)
    {
        /*lint -e{946,960} subtracting pointers is appropriate here */
        FMSTR_SIZE respSize = (FMSTR_SIZE)(responseEnd - msgBuffIO);
#if FMSTR_DEBUG_TX
        /* the first sane frame received from PC Host ends test frame sending */
        fmstr_doDebugTx = FMSTR_FALSE;
#endif
        FMSTR_SendResponse(msgBuffIO, respSize, statusCode);
        return FMSTR_TRUE;
    }
    else
    {
        /* nothing sent out */
        return FMSTR_FALSE;
    }
}

/**************************************************************************//*!
*
* @brief    Sending debug test frame
*
* @param    msgBuffIO - outut frame buffer
*
* @return   True if successful (always in current implementation)
*
******************************************************************************/

FMSTR_BOOL FMSTR_SendTestFrame(FMSTR_BPTR msgBuffIO)
{
    FMSTR_SendResponse(msgBuffIO, 0, FMSTR_STC_DEBUGTX_TEST);
    return FMSTR_TRUE;
}

/**************************************************************************//*!
*
* @brief    Handling GETINFO or GETINFO_BRIEF
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    retStatus - response status
*
* @return   As all command handlers, the return value should be the buffer
*           pointer where the response data payload is finished
*
******************************************************************************/

FMSTR_BPTR _FMSTR_GetBoardConfig(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    static const FMSTR_CHAR *fmstr_cfgParamNames[] =
    {
        "MTU",
        "VS",
        "NM",
        "DS",
        "BD",
        "F1",
        "BA",
        "RC",
        "SC",
        "PV",
        "PC",
    };

    FMSTR_BPTR response = msgBuffIO;
    FMSTR_U8 respCode = FMSTR_STS_INVALID;
    FMSTR_U8 *str;
    FMSTR_SIZE ix = 0;

    /* Get the IX from incomming buffer */
    msgBuffIO = FMSTR_SizeFromBuffer(&ix, msgBuffIO);

    if(ix == 0)
    {
        FMSTR_INDEX i;
        /* Get the configuration name from incomming buffer */
        msgBuffIO = FMSTR_StringFromBuffer(msgBuffIO, &str);

        /* Try to find the Index of the config item */
        for(i=0; i<FMSTR_COUNTOF(fmstr_cfgParamNames); i++)
        {
            if(!FMSTR_StrCmp(fmstr_cfgParamNames[i], str))
            {
                ix = i + 1;
                break;
            }
        }

        if(ix == 0)
        {
            respCode = FMSTR_STC_EACCESS;
            goto FMSTR_GetConfig_exit;
        }
    }

    respCode = FMSTR_STS_OK | FMSTR_STSF_VARLEN;
    response = FMSTR_StringCopyToBuffer(response, fmstr_cfgParamNames[ix - 1]);

    /* The ix must corresponded to table fmstr_cfgParamNames incremented by one */
    switch(ix)
    {
    case 1: /* MTU */
        response = FMSTR_SizeToBuffer(response, FMSTR_COMM_BUFFER_SIZE);
        break;
    case 2: /* VS */
        response = FMSTR_StringCopyToBuffer(response, FMSTR_VERSION_STR);
        break;
    case 3: /* NM */
        response = FMSTR_StringCopyToBuffer(response, FMSTR_APPLICATION_STR);
        break;
    case 4: /* DS */
        response = FMSTR_StringCopyToBuffer(response, FMSTR_DESCRIPTION_STR);
        break;
    case 5: /* BD */
        response = FMSTR_StringCopyToBuffer(response, FMSTR_BUILDTIME_STR);
        break;
    case 6: /* F1 */
        response = FMSTR_ValueToBuffer8(response, FMSTR_CFG_F1);
        break;
#ifdef FMSTR_PLATFORM_BASE_ADDRESS
    case 7: /* BA */
        response = FMSTR_SizeToBuffer(response, FMSTR_PLATFORM_BASE_ADDRESS);
        break;
#endif
    case 8: /* RC */
        response = FMSTR_ValueToBuffer8(response, FMSTR_USE_RECORDER);
        break;
    case 9: /* SC */
        response = FMSTR_ValueToBuffer8(response, FMSTR_USE_SCOPE);
        break;
    case 10: /* PV */
        response = FMSTR_ValueToBuffer8(response, FMSTR_PRCTL_VER);
        break;
    case 11: /* PC */
        response = FMSTR_ValueToBuffer8(response, FMSTR_USE_PIPES);
        break;
    default:
        respCode = FMSTR_STC_EACCESS;
        break;
    }

FMSTR_GetConfig_exit:
    *retStatus = respCode;

    return response;
}

/* Helper call to obtain password required for certain access level. */

FMSTR_CHAR* _FMSTR_GetAccessPassword(FMSTR_U8 requiredAccess)
{
    switch(requiredAccess)
    {
    case FMSTR_RESTRICTED_ACCESS_RWF:
#if defined(FMSTR_RESTRICTED_ACCESS_RWF_PASSWORD)
        return FMSTR_RESTRICTED_ACCESS_RWF_PASSWORD;
#endif
        /* fall to next case to get lower-level password... */

    case FMSTR_RESTRICTED_ACCESS_RW:
#if defined(FMSTR_RESTRICTED_ACCESS_RW_PASSWORD)
        return FMSTR_RESTRICTED_ACCESS_RW_PASSWORD;
#endif
        /* fall to next case to get lower-level password... */

    case FMSTR_RESTRICTED_ACCESS_R:
#if defined(FMSTR_RESTRICTED_ACCESS_R_PASSWORD)
        return FMSTR_RESTRICTED_ACCESS_R_PASSWORD;
#endif
        /* fall to default, no password required */

    default:
        /* no password for the requested level and any lower level */
        return NULL;
    }
}

/**************************************************************************//*!
*
* @brief    Handling FMSTR_CMD_AUTH1 command
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    retStatus - response status
*
* @return   As all command handlers, the return value should be the buffer
*           pointer where the response data payload is finished
*
******************************************************************************/

#if FMSTR_CFG_F1_RESTRICTED_ACCESS

FMSTR_BPTR _FMSTR_AuthenticationStep1(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_U8 reqAccess = 0;

    /* Get the Required access from incomming buffer */
    msgBuffIO = FMSTR_ValueFromBuffer8(&reqAccess, msgBuffIO);

    /* success */
    *retStatus = FMSTR_STS_OK | FMSTR_STSF_VARLEN;

    /* Check if password is needed for requested access. When logging-out, the client sends 0 as requested level. */
    if(reqAccess == 0 || !_FMSTR_GetAccessPassword(reqAccess))
    {
        /* no password is required, access is granted, we do not require the step 2 */
        fmstr_grantedAccess = reqAccess;
        return FMSTR_ValueToBuffer8(response, 0);
    }

    /* ULEB-encoded algorithm ID */
    response = FMSTR_SizeToBuffer(response, FMSTR_AUTHENT_PRTCL_SHA1);

    /* Generate random salt and store it to local buffer for later use with FMSTR_AuthenticationM2 handler. */
    (void) FMSTR_RandomNumbersToBuffer(fmstr_authSalt, sizeof(fmstr_authSalt));
    return FMSTR_CopyToBuffer(response, fmstr_authSalt, sizeof(fmstr_authSalt));
}

#endif /* FMSTR_CFG_F1_RESTRICTED_ACCESS */

/**************************************************************************//*!
*
* @brief    Handling FMSTR_CMD_AUTH2 command
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    retStatus - response status
*
* @return   As all command handlers, the return value should be the buffer
*           pointer where the response data payload is finished
*
******************************************************************************/

#if FMSTR_CFG_F1_RESTRICTED_ACCESS

FMSTR_BPTR _FMSTR_AuthenticationStep2(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_U8 reqAccess = 0;
    FMSTR_U8 access;
    FMSTR_CHAR* pass;

    /* Get the Required access from incomming buffer */
    msgBuffIO = FMSTR_ValueFromBuffer8(&reqAccess, msgBuffIO);

    /* Get access key provided by client */
    /* msgBuffIO = */FMSTR_CopyFromBuffer(fmstr_accessKey, msgBuffIO, sizeof(fmstr_accessKey));

    /* Evaluate all levels (from 3..1) */
    for(access=FMSTR_RESTRICTED_ACCESS_RWF; access>=FMSTR_RESTRICTED_ACCESS_R; access--)
    {
        if((pass = _FMSTR_GetAccessPassword(access)) == NULL)
            break; /* this access level and all lower levels are open (can be granted) */

#if FMSTR_USE_HASHED_PASSWORDS
        /* Password is already hashed */
        FMSTR_MemCpyFrom(fmstr_localKey, pass, sizeof(fmstr_localKey));
#else
        /* Password was provided as a string, compute SHA1 hash now */
        FMSTR_MemSet(fmstr_localKey, 0, sizeof(fmstr_localKey));
        FMSTR_Sha1Init(&fmstr_sha1Ctx);
        FMSTR_Sha1Update(&fmstr_sha1Ctx, (FMSTR_U8*)pass, FMSTR_StrLen(pass));
        FMSTR_Sha1Final(&fmstr_sha1Ctx, fmstr_localKey);
#endif

        /* compute correct key as SHA1(salt + SHA1(password) + salt) */
        FMSTR_Sha1Init(&fmstr_sha1Ctx);
        FMSTR_Sha1Update(&fmstr_sha1Ctx, fmstr_authSalt, sizeof(fmstr_authSalt));
        FMSTR_Sha1Update(&fmstr_sha1Ctx, fmstr_localKey, sizeof(fmstr_localKey));
        FMSTR_Sha1Update(&fmstr_sha1Ctx, fmstr_authSalt, sizeof(fmstr_authSalt));
        FMSTR_Sha1Final(&fmstr_sha1Ctx, fmstr_localKey);

        /* is access key valid? (i.e. is client's password valid?) */
        if(FMSTR_MemCmp(fmstr_localKey, fmstr_accessKey, sizeof(fmstr_localKey)) == 0)
            break;
    }

    /* Here, the access is the maximum level which is possible to grant with the password provided */

    /* Do not grant higher level than requested */
    fmstr_grantedAccess = access <= reqAccess ? access : reqAccess;

    /* Return success if at least some access level has been granted. Or when user was only validating the password with level=0. */
    if(access || !reqAccess)
    {
        *retStatus = FMSTR_STS_OK;
        response = FMSTR_ValueToBuffer8(response, fmstr_grantedAccess);
    }
    else
    {
        *retStatus = FMSTR_STC_EPASS;
    }

    return response;
}

#endif /* FMSTR_CFG_F1_RESTRICTED_ACCESS */

/**************************************************************************//*!
*
* @brief    Handling READMEM command
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    retStatus - response status
*
* @return   As all command handlers, the return value should be the buffer
*           pointer where the response data payload is finished
*
******************************************************************************/

FMSTR_BPTR _FMSTR_ReadMem(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_ADDR addr;
    FMSTR_SIZE size;

#if FMSTR_CFG_F1_RESTRICTED_ACCESS
    if(fmstr_grantedAccess < FMSTR_RESTRICTED_ACCESS_R)
    {
        *retStatus = FMSTR_STC_EAUTH;
        return response;
    }
#endif

    /* Get the Address from incomming buffer */
    msgBuffIO = FMSTR_AddressFromBuffer(&addr, msgBuffIO);
    /* Get the Size from incomming buffer */
    msgBuffIO = FMSTR_SizeFromBuffer(&size, msgBuffIO);

#if FMSTR_USE_TSA && FMSTR_USE_TSA_SAFETY
    if(!FMSTR_CheckTsaSpace(addr, size, FMSTR_FALSE))
    {
        *retStatus = FMSTR_STC_EACCESS;
        return response;
    }
#endif

    /* check the response will safely fit into comm buffer */
    if(size > (FMSTR_U8)FMSTR_COMM_BUFFER_SIZE)
    {
        *retStatus = FMSTR_STC_RSPBUFFOVF;
        return response;
    }

    /* success  */
    *retStatus = FMSTR_STS_OK;
    return FMSTR_CopyToBuffer(response, addr, size);
}

/**************************************************************************//*!
*
* @brief    Handling READMEM command with base address
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    retStatus - response status
*
* @return   As all command handlers, the return value should be the buffer
*           pointer where the response data payload is finished
*
******************************************************************************/

#ifdef FMSTR_PLATFORM_BASE_ADDRESS

FMSTR_BPTR _FMSTR_ReadMemBaseAddress(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR response = msgBuffIO;
    FMSTR_ADDR addr;
    FMSTR_SIZE size;
    FMSTR_INDEX index;

#if FMSTR_CFG_F1_RESTRICTED_ACCESS
    if(fmstr_grantedAccess < FMSTR_RESTRICTED_ACCESS_R)
    {
        *retStatus = FMSTR_STC_EAUTH;
        return response;
    }
#endif

    /* Get the Relative address to base from incoming buffer */
    msgBuffIO = FMSTR_IndexFromBuffer(&index, msgBuffIO);
    /* Get the Size from incomming buffer */
    msgBuffIO = FMSTR_SizeFromBuffer(&size, msgBuffIO);

    addr = (FMSTR_ADDR)(FMSTR_PLATFORM_BASE_ADDRESS + index);

#if FMSTR_USE_TSA && FMSTR_USE_TSA_SAFETY
    if(!FMSTR_CheckTsaSpace(addr, size, FMSTR_FALSE))
    {
        *retStatus = FMSTR_STC_EACCESS;
        return response;
    }
#endif

    /* check the response will safely fit into comm buffer */
    if(size > (FMSTR_U8)FMSTR_COMM_BUFFER_SIZE)
    {
        *retStatus = FMSTR_STC_RSPBUFFOVF;
        return response;
    }

    /* success  */
    *retStatus = FMSTR_STS_OK;
    return FMSTR_CopyToBuffer(response, addr, size);
}
#endif /* FMSTR_PLATFORM_BASE_ADDRESS */

/**************************************************************************//*!
*
* @brief    Handling WRITEMEM and WRITEMEM_EX commands
*
* @param    msgBuffIO - original command (in) and response buffer (out)
* @param    retStatus - response status
*
* @return   As all command handlers, the return value should be the buffer
*           pointer where the response data payload is finished
*
******************************************************************************/

FMSTR_BPTR _FMSTR_WriteMem(FMSTR_BPTR msgBuffIO, FMSTR_U8 *retStatus)
{
    FMSTR_BPTR  response = msgBuffIO;
    FMSTR_ADDR  addr;
    FMSTR_SIZE  size;
    FMSTR_U8    respCode, flags;

#if FMSTR_CFG_F1_RESTRICTED_ACCESS
    if(fmstr_grantedAccess < FMSTR_RESTRICTED_ACCESS_RW)
    {
        *retStatus = FMSTR_STC_EAUTH;
        return response;
    }
#endif

    /* Get the Flags from incomming buffer */
    msgBuffIO = FMSTR_ValueFromBuffer8(&flags, msgBuffIO);
    /* Get the Address from incomming buffer */
    msgBuffIO = FMSTR_AddressFromBuffer(&addr, msgBuffIO);
    /* Get the Size from incomming buffer */
    msgBuffIO = FMSTR_SizeFromBuffer(&size, msgBuffIO);


#if FMSTR_USE_TSA && FMSTR_USE_TSA_SAFETY
    if(!FMSTR_CheckTsaSpace(addr, size, FMSTR_TRUE))
    {
        respCode = FMSTR_STC_EACCESS;
        goto FMSTR_WriteMem_exit;
    }
#endif

    //TODO Add flash write support FLAG FMSTR_WRMFLAG_WRFLASH

    /*lint -e{534} ignoring function return value */
    if(flags & FMSTR_WRMFLAG_WITHMASK)
        FMSTR_CopyFromBufferWithMask(addr, msgBuffIO, size);
    else
        FMSTR_CopyFromBuffer(addr, msgBuffIO, size);

    respCode = FMSTR_STS_OK;

#if FMSTR_USE_TSA && FMSTR_USE_TSA_SAFETY
FMSTR_WriteMem_exit:
#endif

    *retStatus = respCode;

    return response;
}

#else /* !FMSTR_DISABLE */

/**************************************************************************//*!
*
* @brief    FreeMASTER driver initialization is disabled
*
******************************************************************************/

FMSTR_BOOL FMSTR_Init(void)
{
    return FMSTR_FALSE;
}

/*******************************************************************************
*
* @brief    API: Main "Polling" call from the application main loop
*
*******************************************************************************/

void FMSTR_Poll(void)
{
}

#endif /* !FMSTR_DISABLE */
