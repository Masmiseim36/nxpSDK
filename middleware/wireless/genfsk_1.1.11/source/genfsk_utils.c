/*! *********************************************************************************
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#include "fsl_clock.h"
#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"
#include "fsl_xcvr.h"
#include "fsl_os_abstraction.h"
#include "ModuleInfo.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void GENFSK_EnableInterrupts(uint32_t mask);
extern void GENFSK_DisableInterrupts(uint32_t mask);

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern GENFSK_LocalStruct_t genfskLocal[];
extern uint8_t mGenfskActiveInstance;

/* External XTAL0 (OSC0) clock frequency. */
extern uint32_t g_xtal0Freq;
static uint8_t  isHighPowerConfigured = 0x0;
/*!   
 * \brief The version string of Generic FSK
 */
const char* mGenFskVersionString = (const char*)gGenFskVerString_c;

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
    else
    {
        /* read the RF clock frequency */
        uint32_t refOsc = g_xtal0Freq;
        if( refOsc == 0 )
        {
            status = gGenfskFail_c;    
        }
        else
        {
            XCVR_OverrideFrequency(frequency, refOsc);
        }
    }
    
    return status;
}

void GENFSK_PromiscuousModeEnable(void)
{
    GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK);
    genfskLocal[mGenfskActiveInstance].enabledEvents |= gGENFSK_PromiscuousEventMask_c;
}

void GENFSK_PromiscuousModeDisable(void)
{
    GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK);
    genfskLocal[mGenfskActiveInstance].enabledEvents &= ~gGENFSK_PromiscuousEventMask_c;   
}
    

void GENFSK_GapModeEnable(void)
{
    GENFSK_EnableInterrupts(GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK);
    genfskLocal[mGenfskActiveInstance].enabledEvents |= gGenfskGapEvent;
}

void GENFSK_GapModeDisable(void)
{
    if(!(genfskLocal[mGenfskActiveInstance].enabledEvents & gGenfskNwkAddressMatch))
    {
        GENFSK_DisableInterrupts(GENFSK_IRQ_CTRL_NTW_ADR_IRQ_EN_MASK);
    }
    genfskLocal[mGenfskActiveInstance].enabledEvents &= ~gGenfskGapEvent;
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
    GENFSK->BITRATE = genfskLocal[instanceId].radioConfig.dataRate;
    genfskLocal[instanceId].genfskRegs.bitRate = genfskLocal[instanceId].radioConfig.dataRate;

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

uint32_t GENFSK_GetTxDuration(uint8_t instanceId, uint16_t nBytes)
{
    uint32_t duration;
    
    /* Add preamble size */
    nBytes += ((genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK) >> GENFSK_XCVR_CFG_PREAMBLE_SZ_SHIFT) + 1;
    
    /* Add CRC size */
    if (genfskLocal[instanceId].crcEnable)
    {
        nBytes += (genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_SZ_MASK) >> GENFSK_CRC_CFG_CRC_SZ_SHIFT;
    }
    
    switch (genfskLocal[instanceId].radioConfig.dataRate)
    {
    case gGenfskDR1Mbps:   /*!< GENFSK 1 MBit datarate */
        duration = nBytes * 8;
        break;
    case gGenfskDR500Kbps: /*!< GENFSK 500 KBit datarate */
        duration = nBytes * 8 * 2;
        break;
    case gGenfskDR250Kbps: /*!< GENFSK 250 KBit datarate */
        duration = nBytes * 8 * 4;
        break;
#if RADIO_IS_GEN_3P0
    case gGenfskDR2Mbps:   /*!< GENFSK 2 MBit datarate */
        duration = nBytes * 8 / 2;
        break;
#endif /* RADIO_IS_GEN_3P0 */
    default:
        duration = 0;
        break;
    }
    
    return duration;
}

/* This function can be customized and overidden to perform whatever matching of received 
* packet that matched NW ADDR
*/ 
WEAK bool_t GENFSK_CheckPacketMatch()
{
    return FALSE;
}

/* API used to configure DCDC and LDOs to achieve high power/low power TX. 
The other API, GENFSK_SetTxPowerLevel() will select the power level.
Max power (low power config, TxPowerlevel=32): about 3.5 dBm
Max power (high power config, TxPowerlevel=32): about 5 dBm
Note that this function is a copy of what is done in MT LL. Idea at the end is
to create an API in framework (need to check with manager). */
genfskStatus_t GENFSK_ConfigurePower(uint8_t isHighPwrReq)
{
    static uint8_t  getDefaultRegisterConfig = 0x1;
    static uint8_t  isDcdcClockEnabled = 0;
    static uint8_t  targetValueVdd1p5 = 0;
    static uint8_t  bbLdoHfTrimTx = 0;
    genfskStatus_t  status = gGenfskSuccess_c;
    
    /*On high power request, only program required HW blocks if not already done.*/
    if (isHighPwrReq == 0x1U)
    {
        if (isHighPowerConfigured == 0x0U)
        {
            XCVR_bumpPaPowerUp();
        
            /*TODO: This part of code will require some rework in the context of low power.*/       
            /*Save the initial config*/
            if (getDefaultRegisterConfig == 0x1U)
            {
                isDcdcClockEnabled = (uint8_t)((SIM->SCGC5 & SIM_SCGC5_DCDC_MASK) >> SIM_SCGC5_DCDC_SHIFT);
                if (isDcdcClockEnabled == 0x1U)
                {
                    targetValueVdd1p5 = (uint8_t)((DCDC->REG3 &  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK) >> DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_SHIFT);
                }
                bbLdoHfTrimTx    = (uint8_t)(( XCVR_ANALOG->BB_LDO_1 & XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_MASK)  >> XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_SHIFT);
                getDefaultRegisterConfig = 0x0U;
            }
            /* Configure DCDC */
            if (isDcdcClockEnabled == 0x0U)
            {
                SIM->SCGC5 |= SIM_SCGC5_DCDC_MASK;

                DCDC->REG3 |= DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK(14);
            }
            XCVR_ANALOG->BB_LDO_1 &= XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_MASK;
            XCVR_ANALOG->BB_LDO_1 |= XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM(5);

            isHighPowerConfigured = 0x1U;
        }
    }
    else
    {
        if (isHighPowerConfigured == 0x1U)
        {
            /*TODO: This part of code will require some rework in the context of low power.*/
            /*On low power request, restore back HW blocks to default configuration if high power was configured.*/
            if (((SIM->SCGC5 & SIM_SCGC5_DCDC_MASK) >> SIM_SCGC5_DCDC_SHIFT) == 0x1U)
            {
                if (isDcdcClockEnabled == 0x0U)
                {
                    SIM->SCGC5 &= ~SIM_SCGC5_DCDC_MASK;
                }
                else
                {
                    if (((DCDC->REG3 &  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK) >> DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_SHIFT) != targetValueVdd1p5)
                    {
                        DCDC->REG3 &= ~DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK;
                        DCDC->REG3 |= DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK(targetValueVdd1p5);
                    }
                }
            }
            if (((XCVR_ANALOG->BB_LDO_1 & XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_MASK) >> XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_SHIFT) != bbLdoHfTrimTx)
            {
                XCVR_ANALOG->BB_LDO_2 &= XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM_MASK;
                XCVR_ANALOG->BB_LDO_2 |= XCVR_ANALOG_BB_LDO_1_BB_LDO_HF_TRIM(bbLdoHfTrimTx);
            }

            XCVR_releasePaPowerBump();
            isHighPowerConfigured = 0x0U;
        }
    }
    return status;
}

/* Reports if high power configuration (DCDC, LDO, ...) has been set. */
uint8_t GENFSK_IsHighPowerConfigured(void)
{
    return isHighPowerConfigured;
}
