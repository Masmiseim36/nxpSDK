/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/*!=================================================================================================
\file       app_echo_udp.c
\brief      This is a public source file for the echo protocol application.

==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include <string.h>


/* FSL Framework */
#include "MemManager.h"
#include "FunctionLib.h"

/* Application */
#include "sockets.h"
#include "app_thread_config.h"
#include "session.h"
#include "app_echo_udp.h"

#if THREAD_USE_SHELL
    #include "shell.h"
#endif

#if THREAD_USE_THCI
    #include "thci.h"
    #include "FsciInterface.h"
#endif

/*==================================================================================================
Private macros
==================================================================================================*/
#if UDP_ECHO_PROTOCOL

#define ECHO_SERVER_PORT    (7U)
#define ECHO_PAYLOAD        {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47}
#define ECHO_PAYLOAD_SIZE   (8U)


#define ECHO_DELAY          (1000U)

typedef enum echoUdpStatus_tag
{
    mEcho_Ok                 = 0x00U,
    mEcho_Timeout            = 0x01U,
    mEcho_Fail               = 0x02U
}echoUdpStatus_t;

/*==================================================================================================
Public functions
==================================================================================================*/
static int8_t ECHO_CreatePacket(void* pData);
static void ECHO_TimerCallback(void *param);
static void ECHO_RetransmitCallback(void *param);


taskMsgQueue_t * pmEchoUdpMsgQueue = NULL;   /*!< Pointer to main thread message queue */

static int32_t mEchoUdpSrvSockFd = -1;// mEchoTcpSrvSockFd;
tmrTimerID_t resumeEchoUdpTimer = gTmrInvalidTimerID_c;
tmrTimerID_t mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
uint64_t mEchoUdpTimestamp;
static int32_t mSocketFd = -1;
ipAddr_t mEchoUdpSrcIpAddr = {0};
ipAddr_t mEchoUdpDstIpAddr = {0};
static uint32_t mEchoUdpTimeoutMs = 2000U;
uint16_t mEchoUdpCounter = 0;
static int32_t mEchoUdpPktSize = 32;

/*!*************************************************************************************************
\fn     void ECHO_ProtocolInit(void)
\brief  This function is used to initialize the server for echo protocol.

\param  [in]    pMsgQueue     task message queue
\return         void
***************************************************************************************************/
void ECHO_ProtocolInit
(
    taskMsgQueue_t * pMsgQueue
)
{
    sockaddrStorage_t portAddr;

    if (pmEchoUdpMsgQueue == NULL)
    {
        pmEchoUdpMsgQueue = pMsgQueue;
        /* Check if the socket has already been created */
        if (mEchoUdpSrvSockFd == -1)
        {
            /* Set local address and local port */
            FLib_MemSet(&portAddr,0,sizeof(sockaddrStorage_t));
            ((sockaddrIn6_t*)&portAddr)->sin6_family = AF_INET6;
            ((sockaddrIn6_t*)&portAddr)->sin6_port = ECHO_SERVER_PORT;
            IP_AddrCopy(&((sockaddrIn6_t*)&portAddr)->sin6_addr, &in6addr_any);
            ((sockaddrIn6_t *)&portAddr)->sin6_scope_id = gIpIfSlp0_c;

            /* Open UDP socket */
            mEchoUdpSrvSockFd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
            if(mEchoUdpSrvSockFd != -1)
            {
                (void)bind(mEchoUdpSrvSockFd,&portAddr,sizeof(sockaddrStorage_t));
                Session_RegisterCb(mEchoUdpSrvSockFd,ECHO_UdpServerService,pMsgQueue, mSessCbData_c);
            }

            /* Open TCP socket */
            /*mEchoTcpSrvSockFd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
            if(mEchoTcpSrvSockFd != -1)
            {
                bind(mEchoTcpSrvSockFd,pPortAddr,sizeof(sockaddrStorage_t));
                Session_RegisterCb(mEchoTcpSrvSockFd,ECHO_TcpServerService, mSessCbData_c);
            }*/
        }
        if (mSocketFd == -1)
        {
            FLib_MemSet(&portAddr, 0, sizeof(sockaddrStorage_t));
            mSocketFd =  socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
            (void)bind(mSocketFd, &portAddr, sizeof(sockaddrStorage_t));
            Session_RegisterCb(mSocketFd, ECHO_service, NULL, mSessCbData_c);
        }
    }
}
/*
void ECHO_TcpServerService
(
    void *pInData
)
{

}
*/
/*!*************************************************************************************************
\fn     void ECHO_UdpServerService(uint8_t *pInData)
\brief  This function is used to initialize the server for echo protocol.

\param  [in]    pInData     pointer to the received packet
***************************************************************************************************/
void ECHO_UdpServerService
(
    uint8_t *pInData
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)pInData;

    if(pSessionPacket->packetOpt.security && (pSessionPacket->localAddr.ss_family == AF_INET6))
    {
        ((sockaddrIn6_t*)&pSessionPacket->remAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1,5);
    }
    (void)sendto(mEchoUdpSrvSockFd,(uint8_t*)pSessionPacket->pData, pSessionPacket->dataLen, MSG_SEND_WITH_MEMBUFF,
                 &pSessionPacket->remAddr, sizeof(pSessionPacket->remAddr));

    //MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);
}

/*!*************************************************************************************************
\fn     void ECHO_service(uint8_t* pInData)
\brief  This function is used to receive message from the server and resume shell.

\param  [in]    pInData    socket used for receiving

\return    void
***************************************************************************************************/
void ECHO_service
(
    uint8_t *pInData
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)pInData;
    uint64_t tempTimestamp;
 #if THREAD_USE_SHELL   
    static char ipAddr[INET6_ADDRSTRLEN];
#endif   

    tempTimestamp = TMR_GetTimestamp();
    tempTimestamp -= mEchoUdpTimestamp;
    tempTimestamp /= 1000;
#if THREAD_USE_SHELL
    ntop(AF_INET6, pSessionPacket->remAddr.ss_addr, ipAddr, INET6_ADDRSTRLEN);
    shell_printf("Message received from %s: bytes=%d, time=", ipAddr, pSessionPacket->dataLen);
    shell_writeDec(tempTimestamp);
    shell_write("ms");
    shell_refresh();
#endif
#if THREAD_USE_THCI
    THCI_EchoUdpRcv(mEcho_Ok);
#endif
    MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);

    TMR_FreeTimer(resumeEchoUdpTimer);
    resumeEchoUdpTimer = gTmrInvalidTimerID_c;

    TMR_FreeTimer(mEchoUdpDelayTimerID);
    mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
    
    if (mEchoUdpCounter != 0)
    {   
        /* Start timer */
        if(mEchoUdpDelayTimerID == gTmrInvalidTimerID_c)
        {
            mEchoUdpDelayTimerID = TMR_AllocateTimer();
            TMR_StartSingleShotTimer(mEchoUdpDelayTimerID, ECHO_DELAY,
                                    ECHO_RetransmitCallback, NULL);
        }
        
    }    
}


#if THREAD_USE_SHELL
/*!*************************************************************************************************
\fn     int8_t SHELL_EchoUdp(uint8_t argc, char *argv[])
\brief  This function is used for "echoudp" shell command.

\param  [in]    argc    number of arguments the command was called with
\param  [in]    argv    pointer to a list of pointers to the arguments

\return    void
***************************************************************************************************/
int8_t ECHO_ShellUdp
(
    uint8_t argc,
    char *argv[]
)
{ 
    sockaddrStorage_t sourceAddr = {0};
    int32_t validIpAddr = 0;    
    int8_t result = CMD_RET_SUCCESS;
    char* pValue = NULL;
    mEchoUdpPktSize = 32;
    mEchoUdpTimeoutMs = 2000;
    mEchoUdpCounter = 1;
    char ipAddr[INET6_ADDRSTRLEN];
    
    if (argc == 0)
    {   
        if (gTmrInvalidTimerID_c != resumeEchoUdpTimer)
        {
            TMR_FreeTimer(resumeEchoUdpTimer);
            resumeEchoUdpTimer = gTmrInvalidTimerID_c;
        }

        if (gTmrInvalidTimerID_c != mEchoUdpDelayTimerID)
        {
    
            TMR_FreeTimer(mEchoUdpDelayTimerID);
            mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
        }
        return result;
    }
    if (gTmrInvalidTimerID_c != resumeEchoUdpTimer)
    {
        /* timer is already allocated */
        result = CMD_RET_FAILURE;
        return result;
    }
    /* Get option value for -s */
    pValue = shell_get_opt(argc, argv, "-s");
    if(pValue)
    {
        mEchoUdpPktSize = NWKU_atoi(pValue);
    }
    /* Get option value for -S */
    pValue = shell_get_opt(argc, argv, "-S");
    if(pValue)
    {
        pton(AF_INET6, pValue, &mEchoUdpSrcIpAddr);
        IP_AddrCopy((ipAddr_t*)sourceAddr.ss_addr, &mEchoUdpSrcIpAddr);
        ((sockaddrIn6_t *)&sourceAddr)->sin6_scope_id = gIpIfSlp0_c;
        (void)bind(mSocketFd, &sourceAddr, sizeof(sockaddrStorage_t));
    }
    /* Get option value for -i */
    pValue = shell_get_opt(argc, argv, "-i");
    if(pValue)
    {
        mEchoUdpTimeoutMs = (uint32_t)NWKU_atoi(pValue);        
    }

    /* Get option value for -c */
    pValue = shell_get_opt(argc, argv, "-c");
    if(pValue)
    {
        mEchoUdpCounter = (uint16_t)NWKU_atoi(pValue);
    }
    
    /* Find "-t" option */
    for (uint32_t i = 0; i < argc; i++)
    {
        if (FLib_MemCmp(argv[i],"-t",2))
        {
             mEchoUdpCounter = THR_ALL_FFs16;
             break;
        }
    }

    /* last parameter should be the target IP address */
    if(strchr(argv[argc - 1], ':'))
    {
        validIpAddr = pton(AF_INET6, argv[argc - 1], &mEchoUdpDstIpAddr);
    }
    if(validIpAddr)
    {
        result = ECHO_CreatePacket(NULL);
        if (result == mEcho_Ok)
        {
            shell_printf("Message sent to %s with %u bytes of data:\n\r", 
                ntop(AF_INET6, &mEchoUdpDstIpAddr, ipAddr, sizeof(ipAddr_t)), mEchoUdpPktSize);
            result = CMD_RET_ASYNC;
        }
        else
        {
            shell_write("Transmission Failed!");
            result = CMD_RET_FAILURE;
        }
    }
    
    
    return result;
}
#endif

/*!*************************************************************************************************
\fn     void ECHO_HandleTimerCallback(void* pInData)
\brief  This function is used to resume shell if no reply received after echo udp message is sent.

\param  [in]    pInData    socket used for receiving

\return    void
***************************************************************************************************/
static void ECHO_HandleTimerCallback
(
    void* pInData
)
{
#if THREAD_USE_THCI
        THCI_EchoUdpRcv(mEcho_Timeout);
#endif

#if THREAD_USE_SHELL
    shell_write("Request timed out\r\n");
#endif

    TMR_FreeTimer(resumeEchoUdpTimer);
    resumeEchoUdpTimer = gTmrInvalidTimerID_c;

    TMR_FreeTimer(mEchoUdpDelayTimerID);
    mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
        
    if (mEchoUdpCounter != 0)
    {
        /* Start timer */
        if(mEchoUdpDelayTimerID == gTmrInvalidTimerID_c)
        {
            mEchoUdpDelayTimerID = TMR_AllocateTimer();
        }
        TMR_StartSingleShotTimer(mEchoUdpDelayTimerID, ECHO_DELAY, ECHO_RetransmitCallback, NULL);
    }
#if THREAD_USE_SHELL    
    else
    {       
        shell_refresh();
    } 
#endif    
}


#if THREAD_USE_THCI
/*!*************************************************************************************************
\fn     void ECHO_ThciUdp(uint32_t payloadSize, uint16_t timeout, uint8_t count, ipAddr_t* pDestIpAddr,
                          ipAddr_t* pSourceIpAddr)
\brief  This function is used to send an echo UDP message.

\param  [in]    payloadSize    size of message payload
\param  [in]    timeout        timeout
\param  [in]    security       if the message is secured at MAC layer
\param  [in]    pDestIpAddr    pointer to the destination ip address
\param  [in]    pSourceIpAddr  pointer to the source ip address

\return    void
***************************************************************************************************/
void ECHO_ThciUdp
(
    uint32_t payloadSize,
    uint16_t timeout,
    uint8_t count,
    ipAddr_t* pDestIpAddr,
    ipAddr_t* pSourceIpAddr
)
{

    mEchoUdpCounter = count;
    FLib_MemCpy(&mEchoUdpDstIpAddr, pDestIpAddr, sizeof(ipAddr_t));
    FLib_MemCpy(&mEchoUdpSrcIpAddr, pSourceIpAddr, sizeof(ipAddr_t));
    mEchoUdpPktSize = payloadSize;
    mEchoUdpTimeoutMs = timeout;
    
    if (resumeEchoUdpTimer != gTmrInvalidTimerID_c)
    {
        return;
    }
    
    ECHO_CreatePacket(NULL);
    
}
#endif
/*!*************************************************************************************************
\private
\fn     int8_t ECHO_CreatePacket(void *param)
\brief  This function is used to create and send an echo UDP pachet.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static int8_t ECHO_CreatePacket(void* pData)
{   
    sockaddrStorage_t portAddr = {0};
    uint8_t *pPacket = NULL;
    uint8_t payload[8] = ECHO_PAYLOAD;    
    int8_t result = mEcho_Ok;
    
    FLib_MemSet(&portAddr,0,sizeof(sockaddrStorage_t));

    pPacket = NWKU_MEM_BufferAlloc(mEchoUdpPktSize);
    if ( NULL != pPacket )
    {
        int32_t offset = 0;
        while(offset <= (mEchoUdpPktSize - 9))
        {
            FLib_MemCpy((void*)(pPacket+offset), payload, ECHO_PAYLOAD_SIZE);
            offset+=ECHO_PAYLOAD_SIZE;
        }
        if(offset != mEchoUdpPktSize -1)
        {
            FLib_MemCpy((void*)(pPacket + offset), payload, mEchoUdpPktSize - offset);
        }
        
        IP_AddrCopy(&((sockaddrIn6_t*)&portAddr)->sin6_addr, &mEchoUdpDstIpAddr);
        ((sockaddrIn6_t*)&portAddr)->sin6_family = AF_INET6;
        ((sockaddrIn6_t*)&portAddr)->sin6_port = ECHO_SERVER_PORT;
        resumeEchoUdpTimer = TMR_AllocateTimer();
        
        if (resumeEchoUdpTimer != gTmrInvalidTimerID_c)
        {                        
            mEchoUdpTimestamp = TMR_GetTimestamp();
            ((sockaddrIn6_t*)&portAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1, 5);
            if (-1 != sendto(mSocketFd,(uint8_t*)pPacket, mEchoUdpPktSize, MSG_SEND_WITH_MEMBUFF, &portAddr, 
                   sizeof(sockaddrStorage_t)))
            {   
                mEchoUdpCounter--;
                TMR_StartSingleShotTimer(resumeEchoUdpTimer, mEchoUdpTimeoutMs, ECHO_TimerCallback,(void*)mSocketFd);                
                result = mEcho_Ok;
            }
            else
            {
                MEM_BufferFree(pPacket);
                result = mEcho_Fail;
            }
        }
        else
        {   
            MEM_BufferFree(pPacket);            
            result = mEcho_Fail;
        }
    }
    else
    {
       result = mEcho_Fail;
    }

    return result;
}

/*!*************************************************************************************************
\private
\fn     void ECHO_TimerCallback(void *param)
\brief  This function sets the timeout expire value.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void ECHO_TimerCallback(void *param)
{
    (void)NWKU_SendMsg(ECHO_HandleTimerCallback, NULL, pmEchoUdpMsgQueue);
}

/*!*************************************************************************************************
\private
\fn     void ECHO_RetransmitCallback(void *param)
\brief  This function is the callback for sending a new echo UDP message.

\param  [in]    param   unused

\return         void
***************************************************************************************************/
static void ECHO_RetransmitCallback
(
    void *param
)
{
    (void)NWKU_SendMsg((nwkMsgHandler)ECHO_CreatePacket, NULL, pmEchoUdpMsgQueue);

    TMR_FreeTimer(mEchoUdpDelayTimerID);
    mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
}

#endif


