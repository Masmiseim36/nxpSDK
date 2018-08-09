/*!
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

#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"
#include "ModuleInfo.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern GENFSK_LocalStruct_t genfskLocal[];
extern uint8_t mGenfskActiveInstance;
extern uint32_t SystemCoreClock;

/*!   
 * \brief The version string of Generic FSK
 */
char* mGenFskVersionString = (char*)gGenFskVerString_c;

/*!   
 * \brief Register Generic FSK module information
 */
RegisterModuleInfo(GEN_FSK, /* DO NOT MODIFY */
                   mGenFskVersionString, /* DO NOT MODIFY */
                   gGenFskModuleId_c, /* DO NOT MODIFY, EDIT in genfsk_utils.h */
                   gGenFskVerMajor_c, /* DO NOT MODIFY, EDIT in genfsk_utils.h */
                   gGenFskVerMinor_c, /* DO NOT MODIFY, EDIT in genfsk_utils.h */
                   gGenFskVerPatch_c, /* DO NOT MODIFY, EDIT in genfsk_utils.h */
                   gGenFskBuildNo_c); /* DO NOT MODIFY, EDIT in genfsk_utils.h */

/*******************************************************************************
 * Code
 ******************************************************************************/
void GENFSK_WritePacketBuffer(uint16_t addr_offset, uint8_t *buffer, uint16_t length)
{
    uint8_t *pTemp = NULL;
    uint16_t count = 0;
    
    pTemp = (uint8_t *)(PACKET_BUFFER_BASE_ADDR + addr_offset);
    
    for (count = 0; count < length; count++)
    {
        pTemp[count] = buffer[count];
    }
}
        
void GENFSK_ReadPacketBuffer(uint16_t addr_offset, uint8_t *buffer, uint16_t length)
{
    uint8_t *pTemp = NULL;
    uint16_t count = 0;
    
    pTemp = (uint8_t *)(PACKET_BUFFER_BASE_ADDR + addr_offset);
    
    for (count = 0; count < length; count++)
    {        
        buffer[count] = pTemp[count];
    }
}

genfskStatus_t GENFSK_OverrideFrequency(uint32_t frequency)
{
    genfskStatus_t status = gGenfskSuccess_c;
    
    if ((frequency < 2360000000UL) || (frequency > 2483500000UL))
    {
        status = gGenfskInvalidParameters_c;        
    }
    
    XCVR_OverrideFrequency(frequency, SystemCoreClock);
    
    return status;
}

void GENFSK_PromiscuousModeEnable(void)
{
    uint32_t irqSts;    
    
    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK;
    irqSts |= GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK;
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
    genfskLocal[mGenfskActiveInstance].enabledEvents |= gGENFSK_PromiscuousEventMask_c;
}

void GENFSK_PromiscuousModeDisable(void)
{
    uint32_t irqSts;
    
    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK;
    irqSts &= ~GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK;
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
    genfskLocal[mGenfskActiveInstance].enabledEvents &= ~gGENFSK_PromiscuousEventMask_c;   
}
    
uint16_t GENFSK_Reverse9Bit(uint16_t input)
{    
    uint16_t output;
    
    output = (((input & 0x1) << 8) | ((input & 0x100) >> 8));
    output |= ((input & 0x2) << 6) | ((input & 0x80) >> 6);
    output |= ((input & 0x4) << 4) | ((input & 0x40) >> 4);
    output |= ((input & 0x8) << 2) | ((input & 0x20) >> 2);
    output |= (input & 0x10);
    return output;
}

void GENFSK_SwitchToInstance(uint8_t instanceId)
{
    radio_mode_t radioMode = NUM_RADIO_MODES;
    uint32_t irqSts;
    
    OSA_InterruptDisable();    

    irqSts = GENFSK->IRQ_CTRL & gGENFSK_AllIrqFlags;
    irqSts |= genfskLocal[instanceId].genfskRegs.irqCtrl;    
    GENFSK->IRQ_CTRL |= irqSts;
    
    GENFSK->XCVR_CFG &= ~(GENFSK_XCVR_CFG_RX_DEWHITEN_DIS_MASK |
                          GENFSK_XCVR_CFG_TX_WHITEN_DIS_MASK |
                          GENFSK_XCVR_CFG_SW_CRC_EN_MASK |
                          GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK);
    GENFSK->XCVR_CFG = genfskLocal[instanceId].genfskRegs.xcvrCfg;
    GENFSK->CHANNEL_NUM = genfskLocal[instanceId].genfskRegs.channelNum;
    GENFSK->TX_POWER = genfskLocal[instanceId].genfskRegs.txPower;
    GENFSK->NTW_ADR_CTRL = genfskLocal[instanceId].genfskRegs.ntwAdrCtrl;
    GENFSK->NTW_ADR_0 = genfskLocal[instanceId].genfskRegs.ntwAdr0;
    GENFSK->NTW_ADR_1 = genfskLocal[instanceId].genfskRegs.ntwAdr1;
    GENFSK->NTW_ADR_2 = genfskLocal[instanceId].genfskRegs.ntwAdr2;
    GENFSK->NTW_ADR_3 = genfskLocal[instanceId].genfskRegs.ntwAdr3;
    GENFSK->PACKET_CFG = genfskLocal[instanceId].genfskRegs.packetCfg;
    GENFSK->H0_CFG = genfskLocal[instanceId].genfskRegs.h0Cfg;
    GENFSK->H1_CFG = genfskLocal[instanceId].genfskRegs.h1Cfg;
    GENFSK->CRC_CFG = genfskLocal[instanceId].genfskRegs.crcCfg;
    GENFSK->CRC_INIT = genfskLocal[instanceId].genfskRegs.crcInit;
    GENFSK->CRC_POLY = genfskLocal[instanceId].genfskRegs.crcPoly;
    GENFSK->CRC_XOR_OUT = genfskLocal[instanceId].genfskRegs.crcXorOut;
    GENFSK->WHITEN_CFG = genfskLocal[instanceId].genfskRegs.whitenCfg;
    GENFSK->WHITEN_POLY = genfskLocal[instanceId].genfskRegs.whitenPoly;
    GENFSK->WHITEN_SZ_THR = genfskLocal[instanceId].genfskRegs.whitenSzThr;
    GENFSK->BITRATE = genfskLocal[instanceId].genfskRegs.bitRate;
    
    switch (genfskLocal[instanceId].radioConfig.radioMode)
    {
    case gGenfskGfskBt0p5h0p5:
        radioMode = GFSK_BT_0p5_h_0p5;
        break;
    case gGenfskGfskBt0p5h0p32:
        radioMode = GFSK_BT_0p5_h_0p32;
        break;
    case gGenfskGfskBt0p5h0p7:
        radioMode = GFSK_BT_0p5_h_0p7;
        break;
    case gGenfskGfskBt0p5h1p0:
        radioMode = GFSK_BT_0p5_h_1p0;
        break;
    case gGenfskGfskBt0p3h0p5:
        radioMode = GFSK_BT_0p3_h_0p5;
        break;
    case gGenfskGfskBt0p7h0p5:
        radioMode = GFSK_BT_0p7_h_0p5;
        break;
    case gGenfskFsk:
        radioMode = GFSK_BT_0p5_h_0p5;
        break;
    default:
        radioMode = GFSK_BT_0p5_h_0p5;
        break;
    }
        
    XCVR_ChangeMode(radioMode, (data_rate_t)genfskLocal[instanceId].radioConfig.dataRate);
    
    if ((genfskLocal[instanceId].radioConfig.radioMode == gGenfskFsk) || 
        (genfskLocal[instanceId].radioConfig.radioMode == gGenfskMsk))
    {
        XCVR_TX_DIG->GFSK_COEFF1 = ((0U)<<0 | (511U)<<7 | (0U)<<16 | (511U)<<23);
        XCVR_TX_DIG->GFSK_COEFF2 = 0x00000000U;
        XCVR_TX_DIG->GFSK_CTRL |= XCVR_TX_DIG_GFSK_CTRL_GFSK_FLD_MASK;
    }
    
    OSA_InterruptEnable();
}

void GENFSK_MskPreProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length, uint8_t initBit)
{  
    uint8_t temp = 0x00;
    uint8_t tempByteOut = 0x00;
    uint8_t tempByteIn;
    temp = initBit;
    
    for (uint8_t i = 0; i < length; i++)
    {
        tempByteIn = *(pByteIn++);
        temp |= (tempByteIn << 1);
        tempByteOut = tempByteIn ^ temp;
        tempByteOut ^= 0xAA;
        temp = tempByteIn >> 7;
        *(pByteOut++) = tempByteOut;
    }
}

void GENFSK_MskPostProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length, uint8_t initBit, uint8_t lsbToMsb)
{
    uint8_t temp = 0x00;
    uint8_t tempByteOut = 0x00;
    uint8_t byteInMask = 0x00;
    uint8_t tempByteIn = 0x00;
    
    temp = initBit;
    
    for (uint8_t k = 0; k < length; k++)
    {
        tempByteIn = *(pByteIn++);
        if (!lsbToMsb)
        {
            /* Reflect byte */
            tempByteIn = (tempByteIn >> 4) | (tempByteIn << 4);
            tempByteIn = ((tempByteIn & 0xCC) >> 2) | ((tempByteIn & 0x33) << 2);
            tempByteIn = ((tempByteIn & 0xAA) >> 1) | ((tempByteIn & 0x55) << 1);
        }
        
        byteInMask = tempByteIn ^ 0xAA;
        
        for (uint32_t i = 0; i < 8; i++)
        {
            tempByteOut = byteInMask ^ temp;
            temp ^= (tempByteOut << 1);
        }
            
        temp = tempByteOut >> 7;
        
        if (!lsbToMsb)
        {
            /* Reflect byte */
            tempByteOut = (tempByteOut >> 4) | (tempByteOut << 4);
            tempByteOut = ((tempByteOut & 0xCC) >> 2) | ((tempByteOut & 0x33) << 2);
            tempByteOut = ((tempByteOut & 0xAA) >> 1) | ((tempByteOut & 0x55) << 1);
        }    
        *(pByteOut++) = tempByteOut;
    }
}

void GENFSK_SwapBytes(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length)
{
    uint8_t tempByteOut = 0x00;
    
    for (uint32_t k=0; k<length; k++)
    {
        tempByteOut = *(pByteIn++);
        /* Reflect byte */
        tempByteOut = (tempByteOut >> 4) | (tempByteOut << 4);
        tempByteOut = ((tempByteOut & 0xCC) >> 2) | ((tempByteOut & 0x33) << 2);
        tempByteOut = ((tempByteOut & 0xAA) >> 1) | ((tempByteOut & 0x55) << 1);
        *(pByteOut++) = tempByteOut;
    }
}
