/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*!=================================================================================================
\file       shell_ip.c
\brief      This is a public source file for the shell application. It contains the implementation
            of the shell commands used in the application.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "EmbeddedTypes.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "app_stack_config.h"

#include "shell.h"
#include "ModuleInfo.h"
#include "sockets.h"
#include "ip_if_management.h"
#include "ip6.h"
#include "icmp.h"
#include "FunctionLib.h"
#include "session.h"
#include "nd.h"
#include "nd_cfg.h"
#include "coap.h"
/* LED Application */
#include "LED.h"


#if SOCK_DEMO
#include "app_sockets.h"
#endif

#if UDP_ECHO_PROTOCOL
#include "app_echo_udp.h"
#endif

#include "shell_nwk_ip.h"

#include "gatt_interface.h"
#include "gatt_server_interface.h"
#include "gatt_client_interface.h"
#include "ApplMain.h"

/*==================================================================================================
Private macros
==================================================================================================*/
#define PING_ID                     (1U)
#define PING_SEQ_NB                 (1U)
#define PING_PAYLOAD_DEFAULT_SIZE   (32U)
#define PING_PAYLOAD_START          '@'
#define PING_PAYLOAD_END            'W'
#define SHELL_CMD_MAX_ARGS          (20U)
#define PING_HEADER_SIZE            (4U)
#define DEFAULT_TIMEOUT             (2000U)
#define SHELL_PING_MIN_TIMEOUT      (2000U)
#define PING_DELAY                  (500U)

#define STRING_TRUE                 "TRUE"
#define STRING_FALSE                "FALSE"
/*==================================================================================================
Private type definitions
==================================================================================================*/
typedef void (*pfGetTable_t)(void* param);

typedef enum shellValueType_tag
{
    gString_c,
    gDecimal_c,
    gHex_c,
    gHexReversed_c,
    gArray_c,
    gTable_c,
}shellValueType_t;

/*==================================================================================================
Private prototypes
==================================================================================================*/

#if SOCK_DEMO
static int8_t SHELL_Socket(uint8_t argc, char *argv[]);
#endif
static int8_t SHELL_Ifconfig(uint8_t argc, char *argv[]);
static int8_t SHELL_Ping(uint8_t argc, char *argv[]);

#if UDP_ECHO_PROTOCOL
static int8_t SHELL_EchoUdp(uint8_t argc, char *argv[]);
#endif

static void SHELL_PrintStatus
(
    shellIpStatus_t statusCode
);

static void SHELL_Resume(void);

static void PING_EchoReplyReceiveAsync(ipPktInfo_t *pRxIpPktInfo);
static void PING_EchoReplyReceive(void *pParam);
static void PING_TimerCallback(void *param);
static void PING_HandleTimerCallback(void *param);
static void PING_RetransmitCallback(void *param);
static void PING_RetransmitHandle(void *param);
static ipPktInfo_t *PING_CreatePktInfo(ipAddr_t *pDstAddr, uint32_t payloadLen);
static int8_t SHELL_CoapSend(uint8_t argc, char *argv []);
static void SHELL_CoapAckReceive(coapSessionStatus_t sessionStatus, void *pData, coapSession_t *pSession, uint32_t dataLen);
static void SHELL_PrintPingStatistics();

static int8_t SHELL_MulticastGroups(uint8_t argc, char *argv[]);

static void float_division(char *intPart, char *fractPart, uint32_t sent, uint32_t lost);
static const char* SHELL_InterfaceUidToName(ipIfUniqueId_t uniqueId);

/*==================================================================================================
Private global variables declarations
==================================================================================================*/

const cmd_tbl_t aShellCommands[] =
{
    {
        "                   ", SHELL_CMD_MAX_ARGS, 0, NULL
#if SHELL_USE_HELP
        ,"",
        ""
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "ifconfig", SHELL_CMD_MAX_ARGS, 0, SHELL_Ifconfig
#if SHELL_USE_HELP
        ,"IP Stack interfaces configuration",
        "IP Stack interfaces configuration\r\n"
        "   ifconfig - displays all interfaces and addresses configured on the device\r\n"
#if ND_ENABLED
        "   ifconfig ncache - displays the ND neighbors\r\n"
#endif
#endif /* SHELL_USE_HELP */
        "   ifconfig <interface ID> ip <IP address>", NULL
    },
    
    {
        "mcastgroup", SHELL_CMD_MAX_ARGS, 0, SHELL_MulticastGroups
#if SHELL_USE_HELP
        ,"Multicast groups management",
        "Multicast groups management\r\n"
        "   mcastgroup show - displays all joined multicast groups\r\n"
        "   mcastgroup add <IP group address> <interface id>- joins to a new multicast group\r\n"
        "   mcastgroup leave <IP group address> <interface id>- leaves a multicast group\r\n"
        "Warning! Do not remove addresses that may affect stack's behaviour!"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    
    {
        "ping", SHELL_CMD_MAX_ARGS, 0, SHELL_Ping
#if SHELL_USE_HELP
        ,"IP Stack ping tool",
        "IP Stack ping IPv4/IPv6 addresses\r\n"
        "   ping <ip address> -i <timeout> -c <count> -s <size> -t <continuous ping> -S <source IP address>\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    {
        "coap", SHELL_CMD_MAX_ARGS, 0, SHELL_CoapSend
#if SHELL_USE_HELP
        ,"Send CoAP message",
        "Send CoAP message\r\n"
        "   coap <reqtype: CON/NON> <reqcode (GET/POST/PUT/DELETE)> <IP addr dest> <URI path> <payload ASCII>\r\n"
        "Example: coap CON POST 2001::1 /led on \r\n"
        "         coap CON POST 2001::1 /led off \r\n"
        "         coap CON POST 2001::1 /led toggle \r\n"
        "         coap CON POST 2001::1 /led flash\r\n"
        "         coap CON POST 2001::1 /led rgb r255 g255 b255\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
    
    
#if SOCK_DEMO
    {
        "socket", SHELL_CMD_MAX_ARGS, 0, SHELL_Socket
#if SHELL_USE_HELP
        ,"IP Stack BSD Sockets commands",
        "IP Stack BSD Sockets commands\r\n"
        "   socket open <protocol> <remote ip addr> <remote port>\r\n"
        "   socket send <socket id> <payload>\r\n"
        "   socket close <socket id>\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */
    },
#endif
    
#if UDP_ECHO_PROTOCOL
    {
        "echoudp", SHELL_CMD_MAX_ARGS, 0, SHELL_EchoUdp
#if SHELL_USE_HELP
        ,"Echo udp client",
        "Echo udp client\r\n"
        "   echoudp -s<size> -S<source address> -t<continuous request> -i<timeout> <target ip address>\r\n"
#endif /* SHELL_USE_HELP */
#if SHELL_USE_AUTO_COMPLETE
        ,NULL
#endif /* SHELL_USE_AUTO_COMPLETE */

    },
#endif /* UDP_ECHO_PROTOCOL */

};

const icmpProtMsgTypeHandler_t gaShellProtMsgTypeHandlerTbl6[] =
{
    {PING_EchoReplyReceiveAsync, 129U, ICMP_CODE_DEFAULT}
};

const uint8_t gShellProtMsgTypeHandlerTbl6Size = sizeof(gaShellProtMsgTypeHandlerTbl6)/sizeof(icmpProtMsgTypeHandler_t);


#if ECHO_PROTOCOL
    extern uint16_t mEchoUdpCounter;
#endif
/* Ping variables */
uint64_t                pingTimeStamp = 0;
tmrTimerID_t            pingTimerID = gTmrInvalidTimerID_c;
static bool_t           mContinuousPing = FALSE;
static uint32_t         mPingTimeoutMs;
static uint16_t         defaultSeqNb = PING_SEQ_NB;
static uint16_t         pingSize = 0;
static uint16_t         pingCounter = 0;
static ipAddr_t         mDstIpAddr;
static uint32_t         mPingMacSec = 5;

static tmrTimerID_t     mDelayTimerID = gTmrInvalidTimerID_c;
static char             addrStr[INET6_ADDRSTRLEN];
static tmrTimerID_t     timerIDSelect = gTmrInvalidTimerID_c;

extern moduleInfo_t IP_version;

static bool_t           shellCommandsEnabled = FALSE; /*!< Avoid initializing the module multiple times */
ipAddr_t                *pSrcIpAddr = NULL; /* Used for keeping ping source address */


uint32_t instanceID = 0;

/* CoAP instance */
static uint8_t mCoapInstId = 0xFF;

/*==================================================================================================
Public global variables declarations
==================================================================================================*/

/*==================================================================================================
Public functions
==================================================================================================*/
/*!*************************************************************************************************
\fn     void SHELLComm_Init(void)
\brief  This function is used to initialize the SHELL commands module.

\param  [in]    pMsgQueue   pointer to the message queue

\return         void
***************************************************************************************************/
void SHELLComm_Init
(
    void
)
{
    if(!shellCommandsEnabled)
    {
        /* Register functions */
        shell_register_function_array((cmd_tbl_t *)&aShellCommands[0], NumberOfElements(aShellCommands));
        shellCommandsEnabled = TRUE;
    }
    
    TMR_TimeStampInit();
}

/*!*************************************************************************************************
\fn     void SHELL_PrintIpAddr(ipAddrTypes_t addrType)
\brief  This function is used for printing in shell terminal the IP addresses of a certain type.

\param  [in]    ipAddrTypes_t    the type of the IP addresses to be printed

\return         void
***************************************************************************************************/
void SHELL_PrintIpAddr
(
    ipAddrTypes_t addrType
)
{
    uint32_t iIfIdx, iIpIdx;
    char addrStr[INET6_ADDRSTRLEN];
    ifHandle_t ifHandle;
    ip6IfAddrData_t *pIp6AddrData;
#if IP_IP4_ENABLE
    ip4IfAddrData_t *pIp4AddrData;
#endif

    /* IP Interfaces */
    for(iIfIdx = 0; iIfIdx < IP_IF_NB; iIfIdx++)
    {
        /* Get interface by index */
        ifHandle = IP_IF_GetIfByIndex(iIfIdx);

        if(ifHandle)
        {
            shell_printf("\n\rInterface %d: %s", iIfIdx, SHELL_InterfaceUidToName(ifHandle->ifUniqueId));

            /* Get IPv6 addresses for an interface */
            for(iIpIdx = 0; iIpIdx < IP_IF_IP6_ADDR_NB; iIpIdx++)
            {
                /* get IP address by index */
                pIp6AddrData = IP_IF_GetAddrByIf6(ifHandle->ifUniqueId, iIpIdx, FALSE);

                if (pIp6AddrData)
                {
                    ntop(AF_INET6, &pIp6AddrData->ip6Addr, addrStr, INET6_ADDRSTRLEN);

                    if (((addrType == gAllIpAddr_c))
                        && (IP6_IsUniqueLocalAddr(&pIp6AddrData->ip6Addr)))
                    {
                        shell_printf("\n\r\tUnique local address: %s", addrStr);
                    }
                    if (((addrType == gGlobalAddr_c) ||(addrType == gAllIpAddr_c) )
                        && (IP6_IsGlobalAddr(&pIp6AddrData->ip6Addr)))
                    {
                        shell_printf("\n\r\tGlobal address: %s", addrStr);
                    }
                    if (((addrType == gLinkLocalAddr_c) ||(addrType == gAllIpAddr_c) )
                        && (IP6_IsLinkLocalAddr(&pIp6AddrData->ip6Addr)))
                    {
                        uint8_t addrTypeIndex = 16;
                        if(IP6_IsAddrEui64(&pIp6AddrData->ip6Addr))
                        {
                            addrTypeIndex = 64;
                        }
                        shell_printf("\n\r\tLink local address (LL%d): %s", addrTypeIndex, addrStr);
                    }
                }
            }

            /* Link-local all-Thread-nodes, Realm-Local All Thread Nodes */
            if(addrType == gAllIpAddr_c)
            {
                /* Link-local all-Thread-nodes */
                if(IP_IF_IsMyMulticastGroupAddr6(ifHandle->ifUniqueId,(ipAddr_t*)&in6addr_linklocal_allthreadnodes))
                {
                    ntop(AF_INET6, &in6addr_linklocal_allthreadnodes, addrStr, INET6_ADDRSTRLEN);
                    shell_printf("\n\r\tLink local all Thread Nodes(MCast):  %s", addrStr);
                }
                /* Realm-Local All Thread Nodes */
                if(IP_IF_IsMyMulticastGroupAddr6(ifHandle->ifUniqueId,(ipAddr_t*)&in6addr_realmlocal_allthreadnodes))
                {
                    ntop(AF_INET6, &in6addr_realmlocal_allthreadnodes, addrStr, INET6_ADDRSTRLEN);
                    shell_printf("\n\r\tRealm local all Thread Nodes(MCast): %s", addrStr);
                }
            }
        }
    }
}

/*==================================================================================================
Private functions
==================================================================================================*/
/*!*************************************************************************************************
\private
\fn     void SHELL_Resume(void)
\brief  This function is used to resume SHELL execution after a pause was issued.

\return void
***************************************************************************************************/
static void SHELL_Resume
(
    void
)
{
#if SOCK_DEMO
    App_ShellSocketResume();
#endif

    /* Stop Ping timer */
    if(pingTimerID != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(pingTimerID);
        pingTimerID = gTmrInvalidTimerID_c;
    }

    mContinuousPing = FALSE;
    if(timerIDSelect != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(timerIDSelect);
        timerIDSelect = gTmrInvalidTimerID_c;
    }
    if(mDelayTimerID != gTmrInvalidTimerID_c)
    {
        TMR_FreeTimer(mDelayTimerID);
        mDelayTimerID = gTmrInvalidTimerID_c;
    }


    if(pSrcIpAddr)
    {
        MEM_BufferFree(pSrcIpAddr);
        pSrcIpAddr = NULL;
    }

    shell_refresh();
}

/*!*************************************************************************************************
\private
\fn     int8_t  SHELL_Ping(uint8_t argc, char *argv[])
\brief  This function is used for "ping" shell command.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return         void
***************************************************************************************************/
static int8_t SHELL_Ping(uint8_t argc, char *argv[])
{
    ipPktInfo_t *pIpPktInfo;
    command_ret_t ret = CMD_RET_SUCCESS;
    uint16_t count;
    char *pValue;
    uint8_t ap = AF_INET6;

    /* Stop infinite ping */
    if(argc == 0)
    {
        if(mContinuousPing)
        {
            SHELL_Resume();
        }
    }
    /* Check number of arguments according to the shellComm table */
    else
    {
        /* Reset the size of the ping */
        pingSize = PING_PAYLOAD_DEFAULT_SIZE;
        mPingTimeoutMs = DEFAULT_TIMEOUT;
        count = 0; /* infinite ping */

        /* Get option value for -s */
        pValue = shell_get_opt(argc, argv, "-s");
        if(pValue)
        {
            pingSize = (uint16_t)atoi(pValue);
        }

        /* Get option value for -i */
        pValue = shell_get_opt(argc, argv, "-i");
        if(pValue)
        {
            mPingTimeoutMs = (uint16_t)atoi(pValue);
            if (mPingTimeoutMs < SHELL_PING_MIN_TIMEOUT)
            {
                mPingTimeoutMs = SHELL_PING_MIN_TIMEOUT;
            }
        }

        /* Get option value for -c */
        pValue = shell_get_opt(argc, argv, "-c");
        if(pValue)
        {
            count = (uint16_t)atoi(pValue);
        }

        /* Get option value for -src */
        pValue = shell_get_opt(argc, argv, "-S");
        if(pValue)
        {
            /* Use MEM_BufferAllocForever() in case the ping takes more than 2 minutes */
            pSrcIpAddr = MEM_BufferAllocForever(sizeof(ipAddr_t), 0);
            pton(AF_INET6, pValue, pSrcIpAddr);
        }

        /* Find "-t" option */
        for (uint32_t i = 0; i < argc; i++)
        {
            if (FLib_MemCmp(argv[i],"-t",2))
            {
                 count = 0xFFFF;
                 break;
            }
        }

        /* Verify IP address (v4 or v6) */
        uint8_t *pText = (uint8_t *)argv[1];
        uint8_t strLen = strlen((const char*)argv[1]);
        while(strLen)
        {
            if(*pText == '.')
            {
                ap = AF_INET;
                break;
            }
            pText++;
            strLen--;
        }


        /* Check if the destination IP address is valid */
        if (1 != pton(ap,argv[1],&mDstIpAddr))
        {
            shell_write("Invalid destination IP address");
            return ret;
        }

        /* Pause SHELL command parsing */
        ret = CMD_RET_ASYNC;
        /* Check number of counts */
        mContinuousPing = TRUE;
        pingCounter = 0xFFFFU;
        if(count == 0)
        {
            pingCounter = 3;
        }
        else if(count == 1)
        {
            mContinuousPing = FALSE;

        }
        else if (count != 0xFFFF)
        {
            pingCounter = count - 1;
        }
        /* Create Ping packet */
        pIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, pingSize);

        if(NULL != pIpPktInfo)
        {
            /* If we have a specified source address: set it */
            if(pSrcIpAddr)
            {
                IP_AddrCopy(pIpPktInfo->pIpSrcAddr, pSrcIpAddr);
            }

            /* Send packet to ICMP for transmission */
            if(ap == AF_INET6)
            {
                ICMP_Send(pIpPktInfo, gIcmp6TypeEchoRequest_c, ICMP_CODE_DEFAULT);
            }
            else
            {
                ICMP_Send(pIpPktInfo, gIcmp4TypeEchoRequest_c, ICMP_CODE_DEFAULT);
            }

            /* Get timestamp */
            pingTimeStamp = TMR_GetTimestamp();

            if (gTmrInvalidTimerID_c == pingTimerID)
            {
                /* Allocate ping timer */
                pingTimerID = TMR_AllocateTimer();

                if (pingTimerID != gTmrInvalidTimerID_c)
                {
                    /* Start timer */
                    TMR_StartSingleShotTimer(pingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);

                    shell_printf("Pinging %s with %u bytes of data:\r\n", argv[1], pingSize);
                }
                else
                {
                    shell_write("Timer cannot be allocated!");
                    ret = CMD_RET_SUCCESS;
                }
            }
            else
            {
                shell_write("Timer already allocated!");
                ret = CMD_RET_SUCCESS;
            }
        }
        else
        {
            shell_write("No memory for creating the ping packet");
            ret = CMD_RET_SUCCESS;
        }
    } /* Correct number of arguments */

    return ret;
}

#if SOCK_DEMO
/*!*************************************************************************************************
\private
\fn     int8_t SHELL_Socket(uint8_t argc, char *argv[])
\brief  This function is used for "open" shell command.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return         void
***************************************************************************************************/
static int8_t SHELL_Socket(uint8_t argc, char *argv[])
{

    command_ret_t ret = CMD_RET_ASYNC;
    appSockCmdParams_t* pAppSockCmdParams = MEM_BufferAlloc(sizeof(appSockCmdParams_t));

    /* Stop any socket pending commands */
    if((argc == 0) || (pAppSockCmdParams == NULL))
    {
        SHELL_Resume();
        ret = CMD_RET_SUCCESS;
        return ret;
    }

    FLib_MemSet(pAppSockCmdParams, 0, sizeof(appSockCmdParams_t));
    /* socket open */
    if(!strcmp(argv[1], "open"))
    {
        /* Check number of arguments according to the shellComm table */

        pAppSockCmdParams->appSocketsCmd = gSockOpenUdp;
        /* socket open udp */
        if(!strcmp(argv[2], "udp"))
        {

            uint32_t result = -1;

            pAppSockCmdParams->appSocketsTrans = gSockUdp;
            /* Set local information */
            pAppSockCmdParams->ipVersion = AF_INET6;
            /* Set remote information for easier send */
            if(pAppSockCmdParams->ipVersion == AF_INET6)
            {
               pAppSockCmdParams->sin6_port = atoi((char const*)argv[4]);
               result = pton(AF_INET6, argv[3], &pAppSockCmdParams->sin6_addr);
            }
            else
            {
               pAppSockCmdParams->sin_port = atoi((char const*)argv[4]);
               result = pton(AF_INET, argv[3], &pAppSockCmdParams->sin_addr);
            }
            if(result == -1)
            {
              shell_write("IP address has a wrong format");
              SHELL_NEWLINE();
              ret = CMD_RET_FAILURE;
            }
        }
        else
        {
          ret = CMD_RET_FAILURE;
          shell_write("Wrong number of parameters");
        }

    }
    /* socket close */
    else if(!strcmp(argv[1], "close"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockClose;
        pAppSockCmdParams->sock32 = atoi((const char*)argv[2]);
    }
    /* socket send */
    else if(!strcmp(argv[1], "send"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockSend;

        /* Check number of arguments according to the shellComm table */
        if(argc == 4)
        {
            /* Get socket id */
            pAppSockCmdParams->sock32 = atoi((const char *)argv[2]);
            pAppSockCmdParams->dataLen = strlen((char const*)argv[3]) + 1; //null terminated

            pAppSockCmdParams->pData = MEM_BufferAlloc(pAppSockCmdParams->dataLen);
            if(NULL != pAppSockCmdParams->pData)
            {
              FLib_MemCpy(pAppSockCmdParams->pData,argv[3],pAppSockCmdParams->dataLen);
            }
            else
            {
              ret = CMD_RET_FAILURE;
            }
        }
        else
        {
           ret = CMD_RET_FAILURE;
           shell_write("Wrong number of parameters");
        }
    }
    /* socket select */
    else if(!strcmp(argv[1], "select"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockSelect;

        /* Check number of arguments according to the shellComm table */
        if(argc == 3)
        {
            /* Get socket ID from command */
            pAppSockCmdParams->sock32 = atoi((char const*)argv[2]);
        }
        else
        {
           ret = CMD_RET_FAILURE;
           shell_write("Wrong number of parameters");
        }
    }
    /* socket post */
    else if(!strcmp(argv[1], "post"))
    {
        pAppSockCmdParams->appSocketsCmd = gSockPost;

        /* Check number of arguments according to the shellComm table */
        if(argc == 4 || argc == 3 || argc == 5)
        {

            pAppSockCmdParams->bSelectedFlag = FALSE;
            /* socket poll <socket id> <timeout> */
            if(NWKU_IsNUmber(argv[2]))
            {
                pAppSockCmdParams->sock32 = atoi((char const*)argv[2]);
                if(argc == 4)
                {
                    /* Get time from command */
                    pAppSockCmdParams->timeMs = atoi((char const*)argv[3]);
                }
                else
                {
                    pAppSockCmdParams->timeMs = DEFAULT_TIMEOUT;
                }
            }
            /* socket poll selected */
            else if(!strcmp((const char*)argv[2], "selected"))
            {
                pAppSockCmdParams->bSelectedFlag = TRUE;

                /* Get time from command */
                pAppSockCmdParams->timeMs = atoi((char const*)argv[3]);
            }
        }
        else
        {
            shell_write("Wrong number of parameters");
            ret = CMD_RET_FAILURE;
        }
    }
    else
    {
        shell_write("Wrong command syntax");
        ret = CMD_RET_FAILURE;
    }

    if(ret == CMD_RET_ASYNC)
    {
        if( FALSE == App_SocketSendAsync(pAppSockCmdParams))
        {
            ret = CMD_RET_FAILURE;
            shell_write("No memory for creating the command");
            MEM_BufferFree(pAppSockCmdParams);
        }
    }
    else
    {
        MEM_BufferFree(pAppSockCmdParams);
    }
    return ret;
}

#endif /* SOCK_DEMO */

#if UDP_ECHO_PROTOCOL
/*!*************************************************************************************************
\private
\fn     int8_t SHELL_EchoUdp(uint8_t argc, char *argv[])
\brief  This function is used to send packets with echo protocol.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return         void
***************************************************************************************************/
static int8_t SHELL_EchoUdp(uint8_t argc, char *argv[])
{
    return ECHO_ShellUdp(argc, argv);
}
#endif

/*!*************************************************************************************************
\private
\fn     void SHELL_CoapSend(uint8_t argc, char *argv[])
\brief  This function is used for "coap" command.

\param  [in]

\return         void
***************************************************************************************************/
static int8_t SHELL_CoapSend(uint8_t argc, char *argv[])
{
    /*coap <reqtype: CON/NON> <reqcode (GET/POST/PUT/DELETE)> <IP addr dest> <URI path> <payload ASCII>*/
    char *pValue;
    command_ret_t ret = CMD_RET_ASYNC;
    coapMessageTypes_t requestType = gCoapConfirmable_c;
    coapReqRespCodes_t requestCode = gCoapPOST_c;
    coapSession_t* pCoapSession = NULL;
    uint8_t* pCoapPayload = NULL;
    uint32_t coapPayloadSize = 0;
    ipAddr_t destAddr;
    coapStartUnsecParams_t coapParams = {COAP_DEFAULT_PORT, AF_INET6};

    if (argc >= 4)
    {       
        if (!strcmp(argv[1], "CON"))
        {
            requestType = gCoapConfirmable_c;
        }
        else if (!strcmp(argv[1], "NON"))
        {
            requestType = gCoapNonConfirmable_c;
            ret = CMD_RET_SUCCESS;
        }

        if (!strcmp(argv[2], "GET"))
        {
            requestCode = gCoapGET_c;
        }
        else if (!strcmp(argv[2], "POST"))
        {
            requestCode = gCoapPOST_c;
        }
        else if (!strcmp(argv[2], "PUT"))
        {
            requestCode = gCoapPUT_c;
        }
        else if (!strcmp(argv[2], "DELETE"))
        {
            requestCode = gCoapDELETE_c;
        }
        else
        {   
            SHELL_PrintStatus(gShellIp_Failure_c);
            return CMD_RET_SUCCESS;
        }

        if (0xFF == mCoapInstId)
        {
            mCoapInstId = COAP_CreateInstance(NULL, &coapParams, gIpIfBle0_c, NULL, 0);
        }

        pCoapSession = COAP_OpenSession(mCoapInstId);

        if (NULL != pCoapSession)
        {

            /* Get destination address */
            pton(AF_INET6,argv[3], &destAddr);

            /* Get URI path */
            if (argc >= 5)
            {
                pValue = argv[4];
                if (pValue)
                {   
                    char* pStartUri = pValue;
                    char* pOption = '\0';
                    uint8_t size = 0;                  
                    while (*pStartUri != '\0')
                    {   
                        if (*pStartUri == '/')
                        {   
                            if (size != 0)
                            {
                                COAP_AddOptionToList(pCoapSession,COAP_URI_PATH_OPTION, (uint8_t*)pOption,size);
                                /* Reset counters */
                                size = 0;                                
                            }
                            pOption = pStartUri+1;
                            
                        }
                        else
                        {
                            size++;
                        }
                        pStartUri++;
                    }
                    if (size != 0)
                    {
                        COAP_AddOptionToList(pCoapSession,COAP_URI_PATH_OPTION, (uint8_t*)pOption,size);
                    }
                }
            }

            /* Get payload */
            if (argc >= 6)
            {
                pValue = argv[5];
                if (pValue)
                {
                    coapPayloadSize = strlen(pValue);
                    if (!strcmp(argv[5], "rgb"))
                    {
                        coapPayloadSize += strlen(argv[6]) + strlen(argv[7]) + strlen(argv[8]) + 4;

                    }
                    pCoapPayload = MEM_BufferAlloc(coapPayloadSize);

                    if (pCoapPayload)
                    {
                        FLib_MemSet(pCoapPayload,0,coapPayloadSize);
                        FLib_MemCpy(pCoapPayload,pValue,coapPayloadSize);
                    }
                }
            }
            /* Send CoAP message */
            FLib_MemCpy(&pCoapSession->remoteAddr, &destAddr,sizeof(ipAddr_t));
            pCoapSession->code  = requestCode;
            pCoapSession->msgType = requestType;
            pCoapSession->pCallback = SHELL_CoapAckReceive;
            COAP_SendMsg(pCoapSession, pCoapPayload, coapPayloadSize);

            MEM_BufferFree(pCoapPayload);

        }
    }
    else
    {
        shell_write("Invalid number of parameters!\n\r");
        ret = CMD_RET_SUCCESS;
    }

    return ret;
}

/*!*************************************************************************************************
\private
\fn     void SHELL_CoapAckReceive(coapSessionStatus_t sessionStatus, void *pData,
                                  coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for the COAP ACK message.

\param  [in]

\return         void
***************************************************************************************************/
static void SHELL_CoapAckReceive
(
    coapSessionStatus_t sessionStatus,
    void *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    char remoteAddrStr[INET6_ADDRSTRLEN];
    uint8_t temp[10];

    if (gCoapSuccess_c == sessionStatus)
    {
        ntop(AF_INET6, &pSession->remoteAddr, remoteAddrStr, INET6_ADDRSTRLEN);
        /* coap rsp from <IP addr>: <ACK> <rspcode: X.XX> <payload ASCII> */
        shell_printf("coap rsp from ");
        shell_printf(remoteAddrStr);
        if (gCoapAcknowledgement_c == pSession->msgType)
        {
            shell_printf(" ACK ");

        }

        if (0 != dataLen)
        {
            temp[dataLen]='\0';
            FLib_MemCpy(temp,pData,dataLen);
            shell_printf((char*)temp);
        }
    }
    else
    {
        shell_printf("No response received!");
    }
    SHELL_Resume();
}

/*!*************************************************************************************************
\private
\fn     int8_t SHELL_Ifconfig(uint8_t argc, char *argv[])
\brief  This function is used for ifconfig.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return         void
***************************************************************************************************/
static int8_t SHELL_Ifconfig(uint8_t argc, char *argv[])
{
#if IP_IP6_ENABLE
    int32_t interfaceId;
    ipAddr_t inIpAddr;
    ifHandle_t ifHandle;
    uint32_t bindStatus = gIpOk_c;
#if ND_ENABLED
    uint32_t iIf;
    ndNeighborEntry_t *pNdNeighbor = NULL;
    char addrStr[INET6_ADDRSTRLEN];
#endif

    /* ifconfig all */
    if(argc == 1)
    {
        SHELL_PrintIpAddr(gAllIpAddr_c);
    }
#if ND_ENABLED
    /* ifconfig ncache */
    else if((argc == 2) && !strcmp(argv[1], "ncache"))
    {
        interfaceId = atoi(argv[1]);
        shell_write("ND Neighbor Cache:\n\r");
        /* Get interfaces by index */
        iIf = 0;
        ifHandle = IP_IF_GetIfByIndex(iIf);
        while(ifHandle)
        {
            shell_printf("\n\r\tInterface %d: %s\n\r", iIf, SHELL_InterfaceUidToName(ifHandle->ifUniqueId));

            for (uint32_t iCount = 0U; iCount < ND_NEIGHBOR_CACHE_SIZE; iCount++)
            {
                pNdNeighbor = ND_NeighborCacheGetByIdx(ifHandle, iCount);
                if (pNdNeighbor)
                {
                    ntop(AF_INET6, &pNdNeighbor->ipAddr, addrStr, INET6_ADDRSTRLEN);
                    shell_printf("\t%s\n\r", addrStr);
                }
            }
            /* Go to the next interface */
            iIf++;
            ifHandle = IP_IF_GetIfByIndex(iIf);
        }
    }
#endif /* ND_ENABLED */

    /* ifconfig <interface ID> ip <IP address> */
    else if((argc == 4) && !strcmp(argv[2], "ip"))
    {
        interfaceId = atoi(argv[1]);

        /* Check IP address */
        if(pton(AF_INET6, argv[3], &inIpAddr) != 1)
        {
            shell_write("Malformed IP address");
        }
        else
        {
            ifHandle = IP_IF_GetIfByIndex(interfaceId);
            bindStatus = IP_IF_BindAddr6(ifHandle->ifUniqueId, &inIpAddr,ip6AddrTypeManual_c,
                IP6_ADDRESS_LIFETIME_INFINITE, 64U);
            switch (bindStatus)
            {
                case gIpOk_c:
                    shell_write("interface configured");
                    break;

                case gIpNoAddressSpaceError_c:
                    shell_write("address limit reached");
                    break;

                default:
                    shell_write("error");
                    break;
            }
        }
    }
    else
    {
        shell_write("Unknown command\n\r");
    }
#endif

    return CMD_RET_SUCCESS;
}

/*!*************************************************************************************************
\fn     void SHELL_MulticastGroups(shellIpStatus_t statusCode)
\brief  This function is used for checking the joined multicast groups or to join a new one.

\param  [in]
\return         void
***************************************************************************************************/
static int8_t SHELL_MulticastGroups(uint8_t argc, char *argv[])
{
    char groupAddrStr[INET6_ADDRSTRLEN];
    ipAddr_t groupAddr = {{0}};
    command_ret_t ret = CMD_RET_SUCCESS;
    shellIpStatus_t retStatus = gShellIp_Failure_c;
    extern ipIfStruct_t *aInterfaceTable[];
    extern uint32_t interfaceTableSize;
    extern ip6MulticastAddrData_t* aMulticastAddrTable[];
    extern uint32_t multicastAddrTableSize;

    if (!strcmp(argv[1], "show"))
    {
        for (uint32_t ifCount = 0; ifCount < interfaceTableSize; ifCount++)
        {
            if (aInterfaceTable[ifCount] != NULL)
            {
                shell_printf("\r\nInterface %d:\r\n", ifCount);
                for (uint32_t iCount = 0; iCount < multicastAddrTableSize; iCount++)
                {
                    if ((NULL != aMulticastAddrTable[iCount]) &&
                    (aMulticastAddrTable[iCount]->ifUniqueId == aInterfaceTable[ifCount]->ifUniqueId))
                    {
                        ntop(AF_INET6, &aMulticastAddrTable[iCount]->mCastAddr, groupAddrStr, INET6_ADDRSTRLEN);
                        shell_printf("Multicast Group: ");
                        shell_printf(groupAddrStr);
                        shell_printf("\r\n");
                    }
                }
            }
        }
    }
    if ((!strcmp(argv[1], "add")) || (!strcmp(argv[1], "leave")))
    {
        if (argv[2] != NULL)
        {
            if (1 != pton(AF_INET6, argv[2], &groupAddr))
            {
                shell_write("Invalid multicast group address");
            }
            else
            {
                ifHandle_t ifHandle = IP_IF_GetIfByIndex(atoi(argv[3]));

                if (((!strcmp(argv[1], "add")) && (gIpOk_c == IP_IF_AddMulticastGroup6(ifHandle->ifUniqueId, &groupAddr)))
                 || ((!strcmp(argv[1], "leave")) && (gIpOk_c == IP_IF_LeaveMulticastGroup6(ifHandle->ifUniqueId, &groupAddr))))
                {
                    retStatus = gShellIp_Success_c;
                }
                SHELL_PrintStatus(retStatus);
            }
        }
        else
        {
            shell_write("Type a multicast group address");
        }
    }
    return ret;
}

/*!*************************************************************************************************
\fn     void SHELL_PrintStatus(shellIpStatus_t statusCode)
\brief  This function is used for printing in shell terminal the status of the input operation.

\param  [in]    statusCode    The code of the status to be printed

\return         void
***************************************************************************************************/
static void SHELL_PrintStatus
(
    shellIpStatus_t statusCode
)
{
    if (gShellIp_Success_c != statusCode)
    {
        shell_write(" An error has occured!");
    }
    else
    {
        shell_write(" Success!");
    }
}

/*!*************************************************************************************************
\private
\fn         void SHELL_PrintPingStatistics()
\brief      This function prints ping statistics.

\param [in]   void

\return       void
***************************************************************************************************/
static void SHELL_PrintPingStatistics()
{
  uint32_t sent=0;
  uint32_t received=0;
  uint32_t lost;
  char aLoss[5];
  char intPart[3];
  char fractPart[2];

  if(IP_IsAddrIPv6(&mDstIpAddr))
  {
#if IP_IP6_ENABLE
    SHELL_NEWLINE();

    sent = ICMP_GetStatistics(gIcmpStatsProtIcmp6_c, gIcmpStatsDirTransmit_c, gIcmpStatsTypeEchoRequest6_c);
    received = ICMP_GetStatistics(gIcmpStatsProtIcmp6_c, gIcmpStatsDirReceive_c, gIcmpStatsTypeEchoReply6_c);

    if (sent != 0)
    {
      shell_printf("Ping statistics for %s:", ntop(AF_INET6, &mDstIpAddr, addrStr, INET6_ADDRSTRLEN));
    }
#endif /* IP_IP6_ENABLE */
  }
  else
  {
#if IP_IP4_ENABLE
    SHELL_NEWLINE();

    sent = ICMP_GetStatistics(gIcmpStatsProtIcmp4_c, gIcmpStatsDirTransmit_c, gIcmpStatsTypeEchoRequest4_c);
    received = ICMP_GetStatistics(gIcmpStatsProtIcmp4_c, gIcmpStatsDirReceive_c, gIcmpStatsTypeEchoReply4_c);

    if (sent != 0)
    {
      shell_printf("Ping statistics for %s:", ntop(AF_INET, &mDstIpAddr, addrStr, INET_ADDRSTRLEN));
    }
#endif /* IP_IP4_ENABLE */
  }

  if ((IP_IP4_ENABLE && !IP_IsAddrIPv6(&mDstIpAddr)) || (IP_IP6_ENABLE && IP_IsAddrIPv6(&mDstIpAddr)))
  {
    SHELL_NEWLINE();

    lost = sent - received;

    if (sent != 0)
    {
      if (sent == lost)
      {
        sprintf(aLoss, "%s", "100");
      }
      else if (lost == 0)
      {
        sprintf(aLoss, "%s", "0");
      }
      else
      {
        float_division(intPart, fractPart, sent, lost);

        if (strcmp(fractPart, "00") == 0)
        {
          sprintf(aLoss, "%s", "0");
        }
        else
        {
          sprintf(aLoss, "%s.%s", intPart, fractPart);
        }
      }

      shell_printf("Packets: Sent = %d, Received = %d, Lost = %d (%s%% loss)", sent, received, lost, aLoss);
    }

    ICMP_ResetStatistics();
  }
}

/*!*************************************************************************************************
\fn    void SHELL_Process(void* param)
\brief

\param [in]   param    mCmdBuf
***************************************************************************************************/
extern void SHELL_Process(void* param)
{
    extern int8_t (*mpfShellBreak)(uint8_t argc, char * argv[]);
    uint8_t argc;
    char * argv[SHELL_MAX_ARGS+1];    /* NULL terminated  */
    cmd_tbl_t * cmdtp;
    int16_t ret = CMD_RET_FAILURE;

    // Split command into arguments
    argc = make_argv((char*)param, SHELL_MAX_ARGS+1, argv);
    if( argc >= SHELL_MAX_ARGS )
    {
        shell_write("** Too many args (max. ");
        shell_writeDec(SHELL_MAX_ARGS);
        shell_write (") **\r\n");
    }
    // Search for the appropriate command
    cmdtp = shell_find_command(argv[0]);
    if ((cmdtp != NULL) && (cmdtp->cmd != NULL))
    {
        if (argc > cmdtp->maxargs)
        {
            ret = CMD_RET_USAGE;
        }
        else
        {

            ret = (cmdtp->cmd)(argc, argv);
        }
    }
    else
    {
        shell_write("Unknown command '");
        shell_write(argv[0]);
#if SHELL_USE_HELP
        shell_write("' - try 'help'\r\n");
#else
        shell_write("' ");
#endif
        ret = CMD_RET_FAILURE;
    }
#if SHELL_USE_HELP
    if( ret == CMD_RET_USAGE )
    {
        if( cmdtp->usage != NULL )
        {
            shell_write(cmdtp->name);
            shell_writeN(" - ", 3);
            shell_write(cmdtp->usage);
            SHELL_NEWLINE();
        }
        if( cmdtp->help != NULL )
        {
            shell_write("Usage:\r\n");
            shell_write(cmdtp->name);
            shell_writeN(" ", 1);
            shell_write(cmdtp->help);
            SHELL_NEWLINE();
        }
        else
        {
            shell_write ("- No additional help available.\r\n");
        }
    }
#endif
    if( ret == CMD_RET_ASYNC )
    {
        mpfShellBreak = cmdtp->cmd;
    }
    else
    {
        mpfShellBreak = NULL;
        shell_refresh();
    }
    MEM_BufferFree(param);
}

/*!*************************************************************************************************
\private
\fn     ipPktInfo_t *PING_CreatePktInfo(uint32_t payloadLen)
\brief  This function is used to create the packet info structure needed by ICMP ping.

\param  [in]    pDstAddr    pointer to the destination IP address
\param  [in]    payloadLen  the size of the payload

\return         ipPktInfo_t pointer to a packet info structure
***************************************************************************************************/
static ipPktInfo_t *PING_CreatePktInfo(ipAddr_t *pDstAddr, uint32_t payloadLen)
{
    uint16_t echoId = PING_ID;
    ipPktInfo_t *pIpPktInfo = NULL;
    uint16_t idx;
    uint16_t iPayload;
    uint8_t *pPayload;

    pIpPktInfo = NWKU_CreateIpPktInfo();
    if(NULL != pIpPktInfo)
    {
        pIpPktInfo->pIpDstAddr = &pIpPktInfo->ipDstAddr;
        pIpPktInfo->pIpSrcAddr = &pIpPktInfo->ipSrcAddr;

        /* Allocate and populate pIpPktInfo->pNwkBuff using Echo request payload and
           echo request identifier and sequence number */
        pIpPktInfo->pNwkBuff = NWKU_CreateNwkBuffer(payloadLen + sizeof(echoId) + sizeof(defaultSeqNb));

        if(NULL != pIpPktInfo->pNwkBuff)
        {
            /* Allocate and populate pIpPktInfo->pIpDstAddr */
            IP_AddrCopy(pIpPktInfo->pIpDstAddr, pDstAddr);

            /* Determine IP source address based on IP destination address */
            if(IP_IsAddrIPv6(pIpPktInfo->pIpDstAddr))
            {
#if IP_IP6_ENABLE

              ipAddr_t*  pSrcAddr = IP_IF_SelSrcAddr6(gIpIfUndef_c, pIpPktInfo->pIpDstAddr);
              IP_AddrCopy(pIpPktInfo->pIpSrcAddr, pSrcAddr);
#endif /* IP_IP6_ENABLE */
            }
            else
            {
#if IP_IP4_ENABLE
              NWKU_ConvertIp4Addr(IP_IF_SelSrcAddr4(pIpPktInfo->pIpDstAddr, pIpPktInfo->ipPktOptions.ifHandle),
                                  pIpPktInfo->pIpSrcAddr);
#endif /* IP_IP4_ENABLE */
            }

            /* Populate pIpPktInfo->pIpPktOptions */
            // TODO: call function to determine interface hop limit
            pIpPktInfo->ipPktOptions.hopLimit = ICMP_DEFAULT_HOP_LIMIT;
            pIpPktInfo->ipPktOptions.security = mPingMacSec;
            // TODO: pIpPktInfo->ipPktOptions.lqi
            // TODO: pIpPktInfo->ipPktOptions.qos

            /* Populate first the echo request identifier */
            htonas(pIpPktInfo->pNwkBuff->pData, echoId);

            /* Populate the echo request sequence number with a default value */
            htonas(pIpPktInfo->pNwkBuff->pData + sizeof(echoId), defaultSeqNb++);

            /* Set ping payload: 0x61..0x77(a..w) */
            pPayload = pIpPktInfo->pNwkBuff->pData + sizeof(echoId) + sizeof(defaultSeqNb);
            iPayload = 0;
            for(idx=0;idx<payloadLen;idx++,iPayload++)
            {
                if(iPayload > (PING_PAYLOAD_END - PING_PAYLOAD_START))
                {
                    iPayload = 0;
                }
                pPayload[idx] = iPayload + PING_PAYLOAD_START;
            }
        }
        else
        {
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }

    return pIpPktInfo;
}
/*!*************************************************************************************************
\private
\fn     void PING_EchoReplyReceiveAsync(ipPktInfo_t ipPktInfo)
\brief  Interface function for the user app. It handles a received Ping Echo Reply message.

\param  [in]    pIpPktInfo      Pointer to the packet information structure.
***************************************************************************************************/
static void PING_EchoReplyReceiveAsync
(
    ipPktInfo_t *pIpPktInfo
)
{
    App_PostCallbackMessage(PING_EchoReplyReceive, (void *)pIpPktInfo);
}

/*!*************************************************************************************************
\private
\fn     void PING_EchoReplyReceive(ipPktInfo_t ipPktInfo)
\brief  Interface function for the user app. It handles a received Ping Echo Reply message.

\param  [in]    pIpPktInfo      Pointer to the packet information structure.
***************************************************************************************************/
static void PING_EchoReplyReceive
(
    void *pParam
)
{
    uint16_t echoId;
    uint16_t seqNb;
    uint32_t payloadLen;
    uint64_t tempTimestamp;
    ipPktInfo_t *pIpPktInfo = (ipPktInfo_t *)pParam;

    /* Reply from desired IP address */
    if(IP_IsAddrEqual(&mDstIpAddr, pIpPktInfo->pIpSrcAddr) || IP6_IsMulticastAddr(&mDstIpAddr))
    {
        /* Get first the echo request identifier */
        //htonas(pIpPktInfo->pNwkBuff->pData, echoId);
        echoId = ntohas(pIpPktInfo->pNextProt);
        if(echoId == PING_ID)
        {
            /* Get the echo request sequence number */
            //htonas(pIpPktInfo->pNwkBuff->pData + sizeof(echoId), defaultSeqNb);
            seqNb = ntohas(pIpPktInfo->pNextProt + sizeof(echoId));
            if(seqNb == defaultSeqNb-1)
            {
                /* Get payload length from the ICMP packet.
                 * The ping payload is with an offset of 4 */
                payloadLen = pIpPktInfo->prot.nextProtLen - PING_HEADER_SIZE;

                /* Compare payload */
                /*if(FLib_MemCmp(pIpPktInfo->pNextProt + sizeof(echoId) + sizeof(seqNb),
                            (void*)PING_PAYLOAD, payloadLen - (sizeof(echoId) + sizeof(seqNb))))
            {*/
                if(IP_IsAddrIPv6(pIpPktInfo->pIpSrcAddr))
                {
                    ntop(AF_INET6, pIpPktInfo->pIpSrcAddr, addrStr, INET6_ADDRSTRLEN);
                }
                else
                {
                    ntop(AF_INET, pIpPktInfo->pIpSrcAddr, addrStr, INET_ADDRSTRLEN);
                }

                /* Free the input pIpPktInfo  */
                NWKU_FreeIpPktInfo(&pIpPktInfo);

                shell_write("Reply from ");
                shell_write(addrStr);
                shell_write(": bytes=");
                shell_writeDec(payloadLen);
                shell_write(" time=");
                tempTimestamp = TMR_GetTimestamp();
                tempTimestamp -= pingTimeStamp;
                tempTimestamp /= 1000;
                shell_writeDec(tempTimestamp);
                shell_write("ms");
                SHELL_NEWLINE();

                /* Stop timer */
                if(pingTimerID != gTmrInvalidTimerID_c)
                {
                    TMR_StopTimer(pingTimerID);
                }

                /* Continuous ping: restart */
                if(mContinuousPing)
                {

                    /* Start timer */
                    if(mDelayTimerID == gTmrInvalidTimerID_c)
                    {
                        mDelayTimerID = TMR_AllocateTimer();
                    }
                    TMR_StartSingleShotTimer(mDelayTimerID, PING_DELAY, PING_RetransmitCallback, NULL);
                }
                else
                {
                    SHELL_PrintPingStatistics();
                    SHELL_Resume();
                }
                /*else
            {
                shell_write("Reply payload not matching\n\r");
            }*/
            }
            else
            {
                shell_write("Reply sequence number not matching\n\r");
                /* Free the input pIpPktInfo  */
                NWKU_FreeIpPktInfo(&pIpPktInfo);
            }
        }
        else
        {
            shell_write("Reply PING ID not matching\n\r");
            /* Free the input pIpPktInfo  */
            NWKU_FreeIpPktInfo(&pIpPktInfo);
        }
    }
    else
    {
        shell_write("Reply IP source address not matching\n\r");
        /* Free the input pIpPktInfo  */
        NWKU_FreeIpPktInfo(&pIpPktInfo);
    }

}

/*!*************************************************************************************************
\private
\fn     void PING_TimerCallback(void *param)
\brief  This function sets the timeout expire value.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void PING_TimerCallback(void *param)
{
    App_PostCallbackMessage(PING_HandleTimerCallback, NULL);
}

/*!*************************************************************************************************
\private
\fn     void PING_HandleTimerCallback(void *param)
\brief  This function sets the timeout expire value.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void PING_HandleTimerCallback(void *param)
{
    /* Ping reply was not received */
    shell_write("Request timed out.");
    SHELL_NEWLINE();

    if(mContinuousPing)
    {
        ipPktInfo_t *pPingIpPktInfo;

        /* Create Ping packet */
        pPingIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, pingSize);

        if(NULL != pPingIpPktInfo)
        {
            if(pSrcIpAddr)
            {
                IP_AddrCopy(pPingIpPktInfo->pIpSrcAddr, pSrcIpAddr);
            }

            /* Send packet to ICMP for transmission */
            ICMP_Send(pPingIpPktInfo, gIcmp6TypeEchoRequest_c, ICMP_CODE_DEFAULT);

            if(pingCounter > 0 && pingCounter != 0xFFFFU)
            {
                pingCounter--;
            }

            /* Counter have reached 0: stop pinging */
            if(pingCounter == 0)
            {
                mContinuousPing = FALSE;
            }

            /* Get timestamp */
            pingTimeStamp = TMR_GetTimestamp();

            if (pingTimerID != gTmrInvalidTimerID_c)
            {
                /* Start timer */
                TMR_StartSingleShotTimer(pingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);
            }
            else
            {
                shell_write("Invalid Timer ID!");
            }
        }
        else
        {
            shell_write("No memory for creating the ping packet");
            SHELL_Resume();
        }
    }
    else
    {
        SHELL_PrintPingStatistics();
        SHELL_Resume();
    }
}

/*!*************************************************************************************************
\private
\fn     void PING_RetransmitCallback(void *param)
\brief  This function is called when 500ms ping timer expires. This timer is used to send another
        Ping.Request after a Ping.Reply packet was received.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void PING_RetransmitCallback
(
    void *param
)
{
    App_PostCallbackMessage(PING_RetransmitHandle, NULL);  
}

/*!*************************************************************************************************
\private
\fn     void PING_RetransmitHandle(void *param)
\brief  This function is handles the 500ms timeout. This timer is used to send another Ping.Request
        after a Ping.Reply packet was received.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void PING_RetransmitHandle
(
    void *param
)
{
    ipPktInfo_t *pPingIpPktInfo;

    /* Create Ping packet */
    pPingIpPktInfo = PING_CreatePktInfo(&mDstIpAddr, pingSize);

    if(NULL != pPingIpPktInfo)
    {
        /* If we have a specified source address: set it */
        if(pSrcIpAddr)
        {
            IP_AddrCopy(pPingIpPktInfo->pIpSrcAddr, pSrcIpAddr);
        }

        /* Send packet to ICMP for transmission */
        if(IP_IsAddrIPv6(&mDstIpAddr))
        {
            ICMP_Send(pPingIpPktInfo, gIcmp6TypeEchoRequest_c, ICMP_CODE_DEFAULT);
        }
        else
        {
            ICMP_Send(pPingIpPktInfo, gIcmp4TypeEchoRequest_c, ICMP_CODE_DEFAULT);
        }

        /* Get timestamp */
        pingTimeStamp = TMR_GetTimestamp();

        /* Start timer */
        TMR_StartSingleShotTimer(pingTimerID, mPingTimeoutMs, PING_TimerCallback, NULL);

        if(pingCounter > 0 && pingCounter != 0xFFFFU)
        {
            pingCounter--;
        }

        /* Counter have reached 0: stop pinging */
        if(pingCounter == 0)
        {
            mContinuousPing = FALSE;
            MEM_BufferFree(pSrcIpAddr);
            pSrcIpAddr = NULL;
        }

        TMR_FreeTimer(mDelayTimerID);
        mDelayTimerID = gTmrInvalidTimerID_c;
    }
    else
    {
        mContinuousPing = FALSE;
        MEM_BufferFree(pSrcIpAddr);
        pSrcIpAddr = NULL;

        TMR_FreeTimer(mDelayTimerID);
        mDelayTimerID = gTmrInvalidTimerID_c;

        TMR_FreeTimer(pingTimerID);
        pingTimerID = gTmrInvalidTimerID_c;

        shell_write("No memory for creating the ping packet");
        SHELL_Resume();
    }
}

/*!*************************************************************************************************
\private
\fn         const char* SHELL_InterfaceUidToName(ipIfUniqueId_t uniqueId)
\brief      This function returns the name of the 6loBle interface or unknown otherwise

\param [in]   uniqueId - interface unique identifier

\return       pointer to interface name
***************************************************************************************************/
static const char* SHELL_InterfaceUidToName(ipIfUniqueId_t uniqueId)
{
    extern const char* mIfName;
    
    if(uniqueId == gIpIfBle0_c || uniqueId == gIpIfBle1_c)
    {
        return mIfName;
    }
    
    return "Unknown";
    
}
/*!*************************************************************************************************
\private
\fn         void float_division(char *intPart, char *fractPart, int sent, int lost)
\brief      This function determines integer and fractionary part of lost / sent division.

\param [in]   intPart - Retain integer part of division
              fractPart - Retain fractionary part of division
              sent - Number of packets sent
              lost - Number of packets lost

\return       void
***************************************************************************************************/
static void float_division(char *intPart, char *fractPart, uint32_t sent, uint32_t lost)
{
    uint32_t multiplier = 1;
    uint32_t mulFactor;

    uint16_t loss;
    char aLoss[5];

    int8_t cIntPartDigits;
    uint8_t cLostDigits = 0;
    uint8_t cSentDigits = 0;

    uint32_t lostCopy = lost;
    uint32_t sentCopy = sent;

    while (lostCopy != 0)
    {
      cLostDigits++;
      lostCopy = lostCopy / 10;
    }
    while (sentCopy != 0)
    {
      cSentDigits++;
      sentCopy = sentCopy / 10;
    }

    mulFactor = 3 + cLostDigits;
    cIntPartDigits = 2 + (cLostDigits - cSentDigits);

    while (mulFactor > 0)
    {
        multiplier *= 10;
        mulFactor--;
    }

    loss = lost * multiplier / sent;
    sprintf(aLoss, "%d", loss);
    aLoss[4] = '\0';

    switch (cIntPartDigits)
    {
        case 2:
            intPart[0] = aLoss[0];
            intPart[1] = aLoss[1];
            intPart[2] = '\0';

            fractPart[0] = aLoss[2];
            fractPart[1] = aLoss[3];
            fractPart[2] = '\0';
            break;

        case 1:
            intPart[0] = aLoss[0];
            intPart[1] = '\0';

            fractPart[0] = aLoss[1];
            fractPart[1] = aLoss[2];
            fractPart[2] = '\0';
            break;

        case 0:
            intPart[0] = '0';
            intPart[1] = '\0';

            fractPart[0] = aLoss[0];
            fractPart[1] = aLoss[1];
            fractPart[2] = '\0';
            break;

        case -1:
            intPart[0] = '0';
            intPart[1] = '\0';

            fractPart[0] = '0';
            fractPart[1] = aLoss[0];
            fractPart[2] = '\0';
            break;

        default:
            sprintf(intPart, "%s", "0");
            sprintf(fractPart, "%s", "00");
    }
}

/*==================================================================================================
Private debug functions
==================================================================================================*/
