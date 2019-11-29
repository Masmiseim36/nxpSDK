/*! *********************************************************************************
 * \defgroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* file
*
* rief 	This is a public source file for the CoAP application.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "FunctionLib.h"
#include "app_stack_config.h"
#include "ip_if_management.h"
#include "nwk_ip_manager.h"
#include "coap.h"
#include "app_coap.h"
#include "LED.h"
#include <string.h>
/*************************************************************************************
**************************************************************************************
* Private macros
**************************************************************************************
*************************************************************************************/
#define APP_LED_URI_PATH                        "/led"
#define APP_DEFAULT_DEST_ADDR                   in6addr_realmlocal_allthreadnodes
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void APP_ProcessLedCmd(uint8_t *pCommand, uint8_t dataLen);
static void APP_CoapLedCb(coapSessionStatus_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
const coapUriPath_t gAPP_LED_URI_PATH  = {SizeOfString(APP_LED_URI_PATH), (uint8_t*) APP_LED_URI_PATH};

/* CoAP instance */
uint8_t mCoapInstId = 0xFF;
/* destination address for CoAP commands */
ipAddr_t gCoapDestAddress;


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void APP_InitCoap(void)
{
    coapRegCbParams_t cbParams[] =  {{APP_CoapLedCb,  (coapUriPath_t*)&gAPP_LED_URI_PATH}};
    /* Register Services in COAP */
    sockaddrStorage_t coapParams = {0};

    NWKU_SetSockAddrInfo(&coapParams, NULL, AF_INET6, COAP_DEFAULT_PORT, 0, gIpIfBle0_c);
    mCoapInstId = COAP_CreateInstance(NULL, &coapParams, (coapRegCbParams_t *)cbParams,
                                         NumberOfElements(cbParams));
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

/*!*************************************************************************************************
\private
\fn     static void APP_CoapLedCb(bool_t sessionStatus, uint8_t *pData, coapSession_t *pSession, uint32_t dataLen)
\brief  This function is the callback function for CoAP LED message.
\brief  It performs the required operations and sends back a CoAP ACK message.

\param  [in]    sessionStatus   status for CoAP session
\param  [in]    pData           pointer to CoAP message payload
\param  [in]    pSession        pointer to CoAP session
\param  [in]    dataLen         length of CoAP payload

\return         void
***************************************************************************************************/
static void APP_CoapLedCb
(
    coapSessionStatus_t sessionStatus,
    uint8_t *pData,
    coapSession_t *pSession,
    uint32_t dataLen
)
{
    /* Process the command only if it is a POST method */
    if ((pData) && (sessionStatus == gCoapSuccess_c) && (pSession->code == gCoapPOST_c))
    {
        APP_ProcessLedCmd(pData, dataLen);
    }

    /* Send the reply if the status is Success or Duplicate */
    if ((gCoapFailure_c != sessionStatus) && (gCoapConfirmable_c == pSession->msgType))
    {
        /* Send CoAP ACK */
        COAP_Send(pSession, gCoapMsgTypeAckSuccessChanged_c, NULL, 0);
    }
}

/*!*************************************************************************************************
\private
\fn     static void APP_ProcessLedCmd(uint8_t *pCommand, uint8_t dataLen)
\brief  This function is used to process a LED command (on, off, flash, toggle, rgb, color wheel).

\param  [in]    pCommand   command data
\param  [in]    dataLen    data length

\return         void
***************************************************************************************************/
static void APP_ProcessLedCmd(uint8_t *pCommand, uint8_t dataLen)
{
    /* process command */
    if (FLib_MemCmp(pCommand, "on",2))
    {
        LED_TurnOnLed(LED_RGB);
    }
    else if (FLib_MemCmp(pCommand, "off",3))
    {
        LED_StopFlash(LED_RGB);
        LED_TurnOffLed(LED_RGB);
    }
    else if (FLib_MemCmp(pCommand, "toggle",6))
    {
        LED_ToggleLed(LED_RGB);
    }
    else if (FLib_MemCmp(pCommand, "flash",5))
    {
        LED_StartFlash(LED_RGB);
    }
#if gLedRgbEnabled_d
    else if (FLib_MemCmp(pCommand, "rgb",3))
    {
        char* p = (char *)pCommand + strlen("rgb");
        uint8_t redValue = 0, greenValue = 0, blueValue = 0;
        
        dataLen -= strlen("rgb");
        while (dataLen != 0)
        {
            if (*p == 'r')
            {
                p++;
                dataLen--;
                redValue = NWKU_atoi(p);
            }
            if (*p == 'g')
            {
                p++;
                dataLen--;
                greenValue = NWKU_atoi(p);
            }
            if (*p == 'b')
            {
                p++;
                dataLen--;
                blueValue = NWKU_atoi(p);
            }
            dataLen--;
            p++;
        }

        /* update RGB values */

        Led_RGB(LED_RGB, redValue, greenValue, blueValue);
    }
    
    else if(FLib_MemCmp(pCommand, "colorwheel",11))
    {
        LED_ColorWheel(LED_RGB, mLEDInterval_c);
    }

#endif
}
