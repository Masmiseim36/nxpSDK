/*! *********************************************************************************
* \addtogroup BLE PROFILES
* @{
********************************************************************************** */
/*!
* The Clear BSD License
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

#ifndef _NEXT_DST_CHANGE_INTERFACE_H_
#define _NEXT_DST_CHANGE_INTERFACE_H_

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "gatt_interface.h"
#include "current_time_interface.h"

/***********************************************************************************
*************************************************************************************
* Public constants & macros
*************************************************************************************
************************************************************************************/

/***********************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum
{
    gNdcsStandardTime           = 0U,
    gNdcsHalfDaylightTime       = 2U,
    gNdcsDaylightTime           = 4U,
    gNdcsDoubleDaylightTime     = 8U,
    gNdcsDstUnknown             = 255U
} dstOffset_tag;

typedef uint8_t dstOffset_t;

typedef PACKED_STRUCT ndcsTimeWithDst_tag
{
    ctsDateTime_t  date;
    dstOffset_t offset;
} ndcsTimeWithDst_t;

/*! Next DST Change Service - Configuration */
typedef struct ndcsConfig_tag
{
    uint16_t            serviceHandle;
    ndcsTimeWithDst_t   timeWithDst;
} ndcsConfig_t;

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
* \brief        Starts Next DST Change Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Start(ndcsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Stops Next DST Change Service functionality
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Stop(ndcsConfig_t *pServiceConfig);

/*!**********************************************************************************
* \brief        Subscribes a GATT client to the Next DST Change Service
*
* \param[in]    pClient  Client Id in Device DB.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Subscribe(deviceId_t clientDeviceId);

/*!**********************************************************************************
* \brief        Unsubscribes a GATT client from the Next DST Change Service
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_Unsubscribe();

/*!**********************************************************************************
* \brief        Records DST Change on a specified service handle.
*
* \param[in]    pServiceConfig  Pointer to structure that contains server 
*                               configuration information.
*
* \return       gBleSuccess_c or error.
************************************************************************************/
bleResult_t Ndcs_RecordDSTChange (ndcsConfig_t *pServiceConfig);

#ifdef __cplusplus
}
#endif 

#endif /* _NEXT_DST_CHANGE_INTERFACE_H_ */

/*! *********************************************************************************
* @}
********************************************************************************** */
