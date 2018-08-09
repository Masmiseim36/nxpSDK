/*
* The Clear BSD License
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

#ifndef EH_IEEE_COMMANDS_H_
#define EH_IEEE_COMMANDS_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "string.h"
#include <dbg.h>
#include "MMAC.h"
#include "EH_IEEE_802154_Switch.h"
#include "EH_Switch_Configurations.h"
#include "EH_IEEE_Features.h"


/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
/** macros for GPD security level  */

#define GPD_NO_SECURITY                                 (0x00)
#define GPD_FC_1LSB_MIC_2B_NO_ENC                       (0x01)
#define GPD_FC_4B_MIC_4B_NO_ENC                         (0x02)
#define GPD_FC_4B_MIC_4B_WITH_ENC                       (0x03)

#ifdef   GPD_SECURITY_LEVEL_FC_4B_MIC_4B_WITH_ENC
	#define GPD_SECURITY_LEVEL GPD_FC_4B_MIC_4B_WITH_ENC

#elif defined(GPD_SECURITY_LEVEL_FC_1LSB_MIC_2B_NO_ENC)
	#define GPD_SECURITY_LEVEL  GPD_FC_1LSB_MIC_2B_NO_ENC

#elif defined(GPD_SECURITY_LEVEL_FC_4B_MIC_4B_NO_ENC)
	#define GPD_SECURITY_LEVEL GPD_FC_4B_MIC_4B_NO_ENC

#else
	#define GPD_SECURITY_LEVEL GPD_NO_SECURITY
#endif


#define GPD_SOURCE_ID_SIZE                              (4)
#define SECURITY_LEVEL_5                                (0x05)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

 /**  Green Power Security Key Types */
typedef enum PACk
{
    E_GPD_ZB_NWK_KEY = 0x01,
    E_GPD_GROUP_KEY,
    E_GPD_ZB_NWK_DERIVED_GROUP_KEY,
    E_GPD_INDIVIDUAL_KEY,
    DERIVED_INDIVIDUAL_GPD_KEY = 0x07
}teGPDSecurityKeyType;

/*  Green Power Supported Command Ids */
typedef enum GPD_CommandIDs
{
    E_GPD_OFF_CMD               = 0x20,
    E_GPD_ON_CMD                = 0x21,
    E_GPD_TOGGLE_CMD            = 0x22,
    E_GPD_MOVE_UP_CMD           = 0x35,
    E_GPD_MOVE_DOWN_CMD         = 0x36,
    E_GPD_LEVEL_CONTROL_STOP    = 0x34,
    E_GPD_COMMISSIOING_CMD      = 0xE0,
    E_GPD_DECOMMISSIOINING_CMD  = 0xE1,
    E_GPD_SUCCESS_CMD           = 0xE2,
    E_GPD_CHANNEL_REQ_CMD       = 0xE3,
    E_GPD_COMM_REPLY_CMD        = 0xF0,
    E_GPD_CHANNEL_CONFIG_CMD    = 0xF3,

}teGPD_CommandIDs;

/****************************************************************************/
/***        Exported variables                                              ***/
/****************************************************************************/

/***  Global variable containing the data to be transmitted */
extern tsMacFrame sGPD_MACFrame;

/***  Global variable containing the data to be received  */
extern tsMacFrame sGPD_MACReceivedFrame;
/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/
void GPD_SendOperationalCommand(void);
void GPD_SendSpecificCommand(uint8 u8CommandId);
bool bGPD_ParseMACFrame(bool_t bIgnoreSecurity, ts_ReceivedPayload *psReceivedPayload);
void GPD_ConstructMACAndNetworkHeader(bool_t bAutoCommissioning, bool_t bSecured,bool_t bRxAfterTx);
bool bGPD_SecureFrame(uint8 u8GPDEncryptedDataIndex ,uint8     u8AESmode, uint32 u32FrameCounter, tsMacFrame *psMACFrame);
void GPD_ConstructMaintenaceFrame(void);
PUBLIC uint32 u32Reverse(uint32 u32InWord);
#endif /* EH_IEEE_COMMANDS_H_ */
