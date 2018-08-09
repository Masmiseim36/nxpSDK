/*! *********************************************************************************
* \addtogroup Link Loss Service
* @{
********************************************************************************** */
/*!
* The Clear BSD License
* Copyright (c) 2014, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
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

#ifndef _LINK_LOSS_INTERFACE_H_
#define _LINK_LOSS_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef uint8_t llsAlertLevel_t;

/*! Link Loss Service - Alert Level Values */
typedef enum
{
    gLls_NoAlert_c      = 0x00,
    gLls_MildAlert_c    = 0x01,
    gLls_HighAlert_c    = 0x02
} llsAlertLevel_tag;

/*! Link Loss Service - Configuration */
typedef struct llsConfig_tag
{
    uint16_t            serviceHandle;
    llsAlertLevel_t     initialAlertLevel;
} llsConfig_t;
/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*!**********************************************************************************
* \brief        Starts Link Loss Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Start (llsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Link Loss Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Stop (llsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Link Loss service
*
* \param[in]    clientdeviceId  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Subscribe(deviceId_t clientdeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes the GATT client from the Link Loss service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_Unsubscribe();

/*!**********************************************************************************
* \brief        Gets the alert level value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[out]   pOutAlertLevel	Pointer to store the alert level value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_GetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t *pOutAlertLevel);

/*!**********************************************************************************
* \brief        Sets the alert level value on a specified service.
*
* \param[in]    serviceHandle   Service handle.
* \param[in]    alertLevel		Alert level value.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Lls_SetAlertLevel(uint16_t serviceHandle, llsAlertLevel_t alertLevel);


#ifdef __cplusplus
}
#endif 

#endif /* _LINK_LOSS_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
