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

#ifndef EH_IEEE_FEATURES_H_
#define EH_IEEE_FEATURES_H_
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include "jendefs.h"

#include "SecLib.h"
#include "string.h"
#include <dbg.h>
#include "EH_IEEE_802154_Switch.h"
#include "EH_Switch_Configurations.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

/* Bits used for GPD State machine. Read only */

#define COMM_COMP_SET_BIT                   (0x01)
#define CHANNEL_SET_BIT                     (0x02)
#define COMM_REPLY_RCVD_BIT                 (0x04)
#define SEC_MAT_SET_BIT                     (0x08)
#define GPDF_SUCCESS_SET_BIT                (0x10)
#define GPD_PREVIOUS_ON_CMD_SET_BIT         (0x20)
#define GPD_DE_COMM_MODE_SET_BIT            (0x40)
/** EEPROM start sector for persistence storage */
#define EEPROM_START_SECTOR                 (0)
/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

/** GP device  persistent storage Data */

typedef struct GPD_PersistentData
{
    /** This variable is a bit map and used for storing status
     * information during commissioning process
        Bit 0-: This bit stores whether commissioning is completed
            0: commissioning is not completed
            1: commissioning is completed
        Bit 1: This bit stores whether channel is set
            0: channel is not set
            1: channel set
        Bit 2: This bit stores whether commissioning reply is received
            0: commissioning reply not received
            1: commissioning reply  received
        Bit 3: This bit stores whether security keys are received
            0: security keys not received
            1: security keys received
        Bit 4: This bit stores whether success GPDF command is send or not
            0: success GPDF command not send
            1: success GPDF command is send
        Bit 5: This bit stores whether ON or OFF command to be sent next
            0: send ON command
            1: Send OFF command
        Bit 6: This bit stores whether device in de commissioning mode
            0: not in de commissioning mode
            1: de commissioning mode

    */
    bmap8 b8CommissioiningStatusBits;
    /** This variable stores operating channel */
    uint8  u8Channel;

    /** Stores the PAN id of the network. */
    uint16 U16Panid;

    /** The MAC sequential number */
    uint8 U8MACSeqNumber;

    /** The number of packets to be transmitted in a channel */
    uint8 u8NoOfTransPerChannel;

    /** The channel index in the channel list  */
    uint8 u8ChannelIndex;
#ifdef GPD_WITH_SECURITY
    /** Security key type */
    uint8 U8GPDSecKeyType;

    /** Security key */
    tsReg128 sGPDSecKey;

    /** Frame Counter */
    uint32 U32FrameCounter;
#endif
    bmap8 b8CommissioiningStatusBitsPrevious;
    /** Variable indicating whether PDM contains valid data */
    uint8 u8PDMValid;


}tGPD_PersistantData;

/** Structure holding GPD non persistent data  */
typedef struct
{
    /** MAC sequence number used in the last command. */
    uint8 u8MACSeqNo;

    /** Number of times a button is pressed */
    uint8 u8ButtonCount;
}ts_GPDData;

typedef struct
{
    uint8           u8CmdId;
    uint8           u8CmdPayloadLen;
    uint8           *pu8CmdPayload;
    uint32          u32ReceivedFrameCounter;
}ts_ReceivedPayload;
/****************************************************************************/
/***        Exported Variables                                           ***/
/****************************************************************************/
/** GP Feature Global variables */

/** Variable Holding GPD persistent data */
extern tGPD_PersistantData sGPDPersistentData;

/** Variable holding GPD non persistent data */
extern ts_GPDData sGPDData;



/****************************************************************************/
/***        Public Functions                                              ***/
/****************************************************************************/

void HandleGPDStateMachine( uint8 u8CmdId);
#ifndef LITTLE_ENDIAN_PROCESSOR
void vMemRevCpy(uint8 *pu8Dst, uint8 *pu8Src, uint8 u8Len);
#endif
void vSendDecommissioingCmd(void);
void vClearPersistentData(bool bIsWriteToEEPROM);
void vSetModeToDeComm(void );
void vHandleRxMacFrame(void);
void vLoadPersistentData(void);

#endif /* EH_IEEE_FEATURES_H_ */
