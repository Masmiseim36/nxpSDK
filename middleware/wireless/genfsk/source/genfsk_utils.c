/*

Copyright 2016-2019 NXP
All rights reserved.
SPDX-License-Identifier: BSD-3-Clause

*/

#include "EmbeddedTypes.h"
#include "fsl_clock.h"
#include "genfsk_interface.h"
#include "genfsk_ll.h"
#include "genfsk_utils.h"

#include "fsl_os_abstraction.h"
#include "ModuleInfo.h"

#if defined (RADIO_IS_GEN_3P5)
#include "nxp_xcvr_gfsk_bt_0p5_h_0p5_config.h"
#include "nxp_xcvr_gfsk_bt_0p5_h_0p32_config.h"
#include "nxp_xcvr_gfsk_bt_0p5_h_0p7_config.h"
#include "nxp_xcvr_gfsk_bt_0p5_h_1p0_config.h"
#include "nxp_xcvr_msk_config.h"
#include "nxp_xcvr_coding_config.h"
#endif

#include "Flash_Adapter.h"

#include "board.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/

#if 0
/* External XTAL0 (OSC0) clock frequency. */
extern uint32_t g_xtal0Freq;
#endif

#if defined (RADIO_IS_GEN_3P5)
static          uint8_t  isHighPowerConfigured = 0x0;
GENFSK_STATIC   uint8_t  getDefaultRegisterConfig = 0x1;
GENFSK_STATIC   uint8_t  isDcdcClockEnabled = 0;
GENFSK_STATIC   uint8_t  targetValueVdd1p5 = 0xFF;
GENFSK_STATIC   uint8_t  bbLdoHfTrimTx = 0;
#endif /* RADIO_IS_GEN_3P5 */

/*!   
 * \brief The version string of Generic FSK
 */
static const char* mGenFskVersionString = (const char*)gGenFskVerString_c;

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
void GENFSK_WritePacketBuffer(uint16_t addr_offset, const uint8_t *buffer, uint16_t length)
{
    uint8_t *pTemp = NULL;
    uint32_t count = 0;

    /* Copy passed buffer to Peripheral TX_PACKET_RAM */
    pTemp = (uint8_t *)(TX_PACKET_RAM_BASE + addr_offset);
    
    for (count = 0; count < length; count++)
    {
        pTemp[count] = buffer[count];
    }
}
        
void GENFSK_ReadPacketBuffer(uint16_t addr_offset, uint8_t *buffer, uint16_t length)
{
    uint8_t *pTemp = NULL;
    uint32_t count = 0;
    
#if defined (RADIO_IS_GEN_3P5)
    uint32_t addrTemp;
    
    if ((genfskLocal[mGenfskActiveInstance].genfskRegs.packetCfg & (GENFSK_PACKET_CFG_AA_PLAYBACK_CNT_MASK | GENFSK_PACKET_CFG_LL_FETCH_AA_MASK)) == 0U)
    {
      /* No AA playback: copy it from NTW_ADR register */
      uint32_t nwAddMatched = (GENFSK->NTW_ADR_CTRL & GENFSK_NTW_ADR_CTRL_NTW_ADR_MCH_MASK) >> GENFSK_NTW_ADR_CTRL_NTW_ADR_MCH_SHIFT;
      
      switch (nwAddMatched)
      {
        case 0x8U:
          addrTemp = genfskLocal[mGenfskActiveInstance].genfskRegs.ntwAdr3;
          break;
        case 0x4U:
          addrTemp = genfskLocal[mGenfskActiveInstance].genfskRegs.ntwAdr2;
          break;
        case 0x2U:
          addrTemp = genfskLocal[mGenfskActiveInstance].genfskRegs.ntwAdr1;
          break;
        case 0x1U:
        default:
          addrTemp = genfskLocal[mGenfskActiveInstance].genfskRegs.ntwAdr0;
          break;
      }
      
      for (count = 0; count < genfskLocal[mGenfskActiveInstance].syncAddrSizeBytes; count++)
      {        
          buffer[count] = (uint8_t) ((addrTemp >> (count * 8U)) & 0xFFU);
      }
    }
#endif
    
    pTemp = (uint8_t *)(RX_PACKET_RAM_BASE + addr_offset);
    
    for ( ; count < length; count++)
    {        
        buffer[count] = *pTemp++;
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
#if !defined (RADIO_IS_GEN_3P5)
        /* read the RF clock frequency */
        uint32_t refOsc = g_xtal0Freq;
        if( refOsc == 0U )
        {
            status = gGenfskFail_c;    
        }
        else
        {
            (void)XCVR_OverrideFrequency(frequency, refOsc);
        }
#else
        (void)XCVR_OverrideFrequency(frequency);
#endif
    }
    
    return status;
}


static void GENFSK_ReceiveBadPacket(bool on)
{
#if defined (RADIO_IS_GEN_3P5)
    volatile uint32_t tmp;

    tmp = GENFSK->LENGTH_MAX;

    if (on)
    {
        tmp |= GENFSK_LENGTH_MAX_REC_BAD_PKT(1);
        GENFSK->LENGTH_MAX = tmp;
    }
    else
    {
        tmp &= ~GENFSK_LENGTH_MAX_REC_BAD_PKT_MASK;
        GENFSK->LENGTH_MAX = tmp;
    }
#endif /* RADIO_IS_GEN_3P5 */
    return;
}

void GENFSK_PromiscuousModeEnable(void)
{
    uint32_t irqSts;

    GENFSK_ReceiveBadPacket(true);

    /*
     * Set GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN in IRQ_CTRL
     * and add gGenfskPromiscuousEvent to enabledEvents
     */
    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK;
    irqSts |= GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK;
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
    genfskLocal[mGenfskActiveInstance].enabledEvents |= (uint32_t) gGenfskPromiscuousEvent;
}

void GENFSK_PromiscuousModeDisable(void)
{
    uint32_t irqSts;

    GENFSK_ReceiveBadPacket(false);

    /*
     * Clear GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN in IRQ_CTRL
     * and remove gGenfskPromiscuousEvent from enabledEvents
     */
    irqSts = GENFSK->IRQ_CTRL;
    irqSts &= ~gGENFSK_AllIrqFlags;
    irqSts |= GENFSK_IRQ_CTRL_SEQ_END_IRQ_MASK;
    irqSts &= ~GENFSK_IRQ_CTRL_SEQ_END_IRQ_EN_MASK;
    GENFSK->IRQ_CTRL = irqSts;
    genfskLocal[mGenfskActiveInstance].genfskRegs.irqCtrl = irqSts;
    genfskLocal[mGenfskActiveInstance].enabledEvents &= (uint32_t) ~gGenfskPromiscuousEvent;   
}

uint16_t GENFSK_Reverse9Bit(uint16_t input)
{    
    uint16_t output;
    
    /* Reverse 9 bits, used by software whitening for seed and poly values. */
    output = (((input & 0x1U) << 8U) | ((input & 0x100U) >> 8U));
    output |= ((input & 0x2U) << 6U) | ((input & 0x80U) >> 6U);
    output |= ((input & 0x4U) << 4U) | ((input & 0x40U) >> 4U);
    output |= ((input & 0x8U) << 2U) | ((input & 0x20U) >> 2U);
    output |= (input & 0x10U);
    return output;
}

#if !defined (RADIO_IS_GEN_3P5)
genfskStatus_t GENFSK_GetXcvrConfig(genfskRadioMode_t radioModeIn, radio_mode_t *radioMode)
{
    genfskStatus_t status = gGenfskSuccess_c;
  
    switch (radioModeIn)
    {
    case gGenfskGfskBt0p5h0p5:
        *radioMode = GFSK_BT_0p5_h_0p5;
        break;
    case gGenfskGfskBt0p5h0p32:
        *radioMode = GFSK_BT_0p5_h_0p32;
        break;
    case gGenfskGfskBt0p5h0p7:
        *radioMode = GFSK_BT_0p5_h_0p7;
        break;
    case gGenfskGfskBt0p5h1p0:
        *radioMode = GFSK_BT_0p5_h_1p0;
        break;
    case gGenfskGfskBt0p3h0p5:
        *radioMode = GFSK_BT_0p3_h_0p5;
        break;
    case gGenfskGfskBt0p7h0p5:
        *radioMode = GFSK_BT_0p7_h_0p5;
        break;
    case gGenfskFsk:
        *radioMode = GFSK_BT_0p5_h_0p5;
        break;
    case gGenfskMsk:
        *radioMode = GFSK_BT_0p5_h_0p5;
        break;
    default:
        status = gGenfskInvalidParameters_c;
        break;
    }
    
    return status;
}

#else // !defined (RADIO_IS_GEN_3P5)
/* Identify and return XCVR config corresponding to given radio mode and data rate */
genfskStatus_t GENFSK_GetXcvrConfig(genfskRadioMode_t radioModeIn, genfskDataRate_t dataRate, const xcvr_config_t **xcvrConfig)
{
    genfskStatus_t status = gGenfskSuccess_c;
  
    switch (radioModeIn)
    {
    case gGenfskGfskBt0p5h0p5:
#ifndef RADIO_IS_GEN_3P5
    case gGenfskFsk:
#endif /* RADIO_IS_GEN_3P5 */
         switch (dataRate)
         {
         case gGenfskDR1Mbps:
         case gGenfskDR2Mbps:
             /* 2Mbps specific configuration is alternate data rate under 1Mbps configuration. The selection is made in GENFSK_SelectRateConfig() API
                through DATARATE_CONFIG_SEL bit in ENH_FEATURE register: 
                bank0 => 1Mbps
                bank1 => 2Mbps
             */
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p5_1mbps_full_config;
             break;
         case gGenfskDR500Kbps:
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p5_500kbps_full_config;
             break;
         case gGenfskDR250Kbps:
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p5_250kbps_full_config;
             break;
         default:
             status = gGenfskInvalidParameters_c;
             break;
         }
         break;

    case gGenfskGfskBt0p5h0p32:
         switch (dataRate)
         {
         case gGenfskDR1Mbps:
#ifndef RADIO_IS_GEN_3P5
         case gGenfskDR2Mbps:
#endif /* RADIO_IS_GEN_3P5 */
             /* 2Mbps specific configuration is alternate data rate under 1Mbps configuration. The selection is made in GENFSK_SelectRateConfig() API
                through DATARATE_CONFIG_SEL bit in ENH_FEATURE register: 
                bank0 => 1Mbps
                bank1 => 2Mbps
             */
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p32_1mbps_full_config;
             break;
#ifndef RADIO_IS_GEN_3P5
         case gGenfskDR500Kbps:
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p32_500kbps_full_config;
             break;
         case gGenfskDR250Kbps:
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p32_250kbps_full_config;
             break;
#endif /* RADIO_IS_GEN_3P5 */
         default:
             status = gGenfskInvalidParameters_c;
             break;
         }
         break;
         
    case gGenfskGfskBt0p5h0p7:
         switch (dataRate)
         {
         case gGenfskDR1Mbps:
         case gGenfskDR2Mbps:
             /* 2Mbps specific configuration is alternate data rate under 1Mbps configuration. The selection is made in GENFSK_SelectRateConfig() API
                through DATARATE_CONFIG_SEL bit in ENH_FEATURE register: 
                bank0 => 1Mbps
                bank1 => 2Mbps
             */
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_0p7_1mbps_full_config;
             break;

         default:
             status = gGenfskInvalidParameters_c;
             break;
         }
         break;
        
    case gGenfskGfskBt0p5h1p0:
         if (gGenfskDR1Mbps == dataRate)
         {
             *xcvrConfig = &xcvr_gfsk_bt_0p5_h_1p0_1mbps_full_config;
         }
         else
         {
             status = gGenfskInvalidParameters_c;
         }
         break;

    case gGenfskMsk:
         switch (dataRate)
         {
#ifndef RADIO_IS_GEN_3P5
         case gGenfskDR1Mbps:
         case gGenfskDR2Mbps:
             /* 2Mbps specific configuration is alternate data rate under 1Mbps configuration. The selection is made in GENFSK_SelectRateConfig() API
                through DATARATE_CONFIG_SEL bit in ENH_FEATURE register: 
                bank0 => 1Mbps
                bank1 => 2Mbps
             */
             *xcvrConfig = &xcvr_msk_1mbps_full_config;
             break;
#endif /* RADIO_IS_GEN_3P5 */
         case gGenfskDR500Kbps:
             *xcvrConfig = &xcvr_msk_500kbps_full_config;
             break;
#ifndef RADIO_IS_GEN_3P5
         case gGenfskDR250Kbps:
             *xcvrConfig = &xcvr_msk_250kbps_full_config;
             break;
#endif /* RADIO_IS_GEN_3P5 */
         default:
             status = gGenfskInvalidParameters_c;
             break;
         }
         break;

    default:
        status = gGenfskInvalidParameters_c;
        break;
    }
    
    return status;
}
#endif /* !defined (RADIO_IS_GEN_3P5) */

#ifndef RADIO_IS_GEN_3P5
/* Used for MSK radio mode */
void GENFSK_MskPreProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length, uint8_t initBit)
{  
    uint8_t temp = 0x00;
    uint8_t tempByteOut = 0x00;
    uint8_t tempByteIn;
    temp = initBit;
    
    /* Used for MSK radio mode: pre encoding of given bytes */
    for (uint8_t i = 0; i < length; i++)
    {
        tempByteIn = *(pByteIn++);
        temp |= (tempByteIn << 1U);
        tempByteOut = tempByteIn ^ temp;
        tempByteOut ^= 0xAAU;
        temp = tempByteIn >> 7U;
        *(pByteOut++) = tempByteOut;
    }
}

/* Used for MSK radio mode */
void GENFSK_MskPostProcessing(uint8_t * pByteIn, uint8_t * pByteOut, uint16_t length, uint8_t initBit, uint8_t lsbToMsb)
{
    uint8_t temp = 0x00;
    uint8_t tempByteOut = 0x00;
    uint8_t byteInMask = 0x00;
    uint8_t tempByteIn = 0x00;
    
    temp = initBit;
    
    /* Used for MSK radio mode: post encoding of geiven bytes */
    for (uint16_t k = 0; k < length; k++)
    {
        tempByteIn = *(pByteIn++);
        if (lsbToMsb == 0U)
        {
            /* Reflect byte */
            tempByteIn = (tempByteIn >> 4U) | (tempByteIn << 4U);
            tempByteIn = ((tempByteIn & 0xCCU) >> 2U) | ((tempByteIn & 0x33U) << 2U);
            tempByteIn = ((tempByteIn & 0xAAU) >> 1U) | ((tempByteIn & 0x55U) << 1U);
        }
        
        byteInMask = tempByteIn ^ 0xAAU;
        
        for (uint32_t i = 0; i < 8U; i++)
        {
            tempByteOut = byteInMask ^ temp;
            temp ^= (tempByteOut << 1U);
        }
            
        temp = tempByteOut >> 7U;
        
        if (lsbToMsb == 0U)
        {
            /* Reflect byte */
            tempByteOut = (tempByteOut >> 4U) | (tempByteOut << 4U);
            tempByteOut = ((tempByteOut & 0xCCU) >> 2U) | ((tempByteOut & 0x33U) << 2U);
            tempByteOut = ((tempByteOut & 0xAAU) >> 1U) | ((tempByteOut & 0x55U) << 1U);
        }    
        *(pByteOut++) = tempByteOut;
    }
}
#endif
/* Swap bits in a series of bytes of given length */
void GENFSK_SwapBytes(uint8_t * pByteIn, uint8_t * pByteOut, uint8_t length)
{
    uint8_t tempByteOut = 0x00;
    
    for (uint32_t k=0; k<length; k++)
    {
        tempByteOut = *(pByteIn++);
        /* Reflect byte */
        tempByteOut = (tempByteOut >> 4U) | (tempByteOut << 4U);
        tempByteOut = ((tempByteOut & 0xCCU) >> 2U) | ((tempByteOut & 0x33U) << 2U);
        tempByteOut = ((tempByteOut & 0xAAU) >> 1U) | ((tempByteOut & 0x55U) << 1U);
        *(pByteOut++) = tempByteOut;
    }
}

uint32_t GENFSK_GetTxDuration(uint8_t instanceId, uint16_t nBytes)
{
    uint32_t duration = 0;
    uint32_t crcSize = 0;
    
    /* Add preamble size */
    nBytes += (uint16_t) (((genfskLocal[instanceId].genfskRegs.xcvrCfg & GENFSK_XCVR_CFG_PREAMBLE_SZ_MASK) >> GENFSK_XCVR_CFG_PREAMBLE_SZ_SHIFT) + 1U);
    
    /* Add CRC size */
    if (genfskLocal[instanceId].crcEnable != gGenfskCrcDisable)
    {
#if !defined (RADIO_IS_GEN_3P5)
        crcSize = (genfskLocal[instanceId].genfskRegs.crcCfg & GENFSK_CRC_CFG_CRC_SZ_MASK) >> GENFSK_CRC_CFG_CRC_SZ_SHIFT;
#else
        crcSize = (genfskLocal[instanceId].genfskRegs.crcCfg & RBME_CRCW_CFG3_CRC_SZ_MASK) >> RBME_CRCW_CFG3_CRC_SZ_SHIFT;
#endif
    }
    nBytes += (uint16_t)crcSize;
    
    switch (genfskLocal[instanceId].radioConfig.dataRate)
    {
    case gGenfskDR1Mbps:   /*!< GENFSK 1 MBit datarate */
        duration = (uint32_t) nBytes * 8U;
        break;
    case gGenfskDR500Kbps: /*!< GENFSK 500 KBit datarate */
        duration = (uint32_t) nBytes * 8U * 2U;
        break;
    case gGenfskDR250Kbps: /*!< GENFSK 250 KBit datarate */
        duration = (uint32_t) nBytes * 8U * 4U;
        break;
#if RADIO_IS_GEN_3P0
    case gGenfskDR2Mbps:   /*!< GENFSK 2 MBit datarate */
        duration = (uint32_t) nBytes * 8U / 2U;
        break;
#endif /* RADIO_IS_GEN_3P0 */
    default:
#if (defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 0))
        assert(0);	       /*!< should not happen */
#endif
        break;
    }
    
    return duration;
}

/* For KW36, board.c is in bluetooth repo. Need cleaning to
use in code directly BOARD_GetXtal32MhzTrim() instead of below wrapper function. */
uint32_t GENFSK_GetSavedXtalTrim(void)
{  
    uint32_t savedXtalTrim;

#if defined (RADIO_IS_GEN_3P5)    
    savedXtalTrim = (uint32_t)BOARD_GetXtal32MhzTrim(FALSE);
#else
    savedXtalTrim = gHardwareParameters.xtalTrim;
#endif /* defined (RADIO_IS_GEN_3P5) */

    return savedXtalTrim;
}

genfskStatus_t GENFSK_SetXtalTrim(uint32_t xtalTrim)
{  
    genfskStatus_t status = gGenfskInvalidParameters_c;
    
    /*If xtal trim parameter is valid set it in the trim register. */
#if defined (RADIO_IS_GEN_3P5)    
    if (xtalTrim < 0x80U)
#endif      
    {
        status = gGenfskSuccess_c;
        (void)XCVR_SetXtalTrim( (uint8_t)xtalTrim );
    }
    
    return status;
}

#if (gGenfskPreserveXcvrDacTrimValue_d == 1)
void GENFSK_SaveXcvrDcocDacTrimToFlash(xcvr_DcocDacTrim_t *xcvrDacTrim)
{
    NV_Init();
    FLib_MemCpy(&gHardwareParameters.xcvrCal, xcvrDacTrim, sizeof(xcvr_DcocDacTrim_t));
    NV_WriteHWParameters(&gHardwareParameters);
}

bool_t GENFSK_RestoreXcvrDcocDacTrimFromFlash(xcvr_DcocDacTrim_t *xcvrDacTrim)
{
    bool_t status;

    /* check if reserved memory in HW parameters is at least sizeof xcvr_DcocDacTrim_t */
    assert(sizeof(gHardwareParameters.xcvrCal) >= sizeof(xcvr_DcocDacTrim_t) );

    /* If stored value is equal to 0xFF, then it's not a valid value */
    status = FLib_MemCmpToVal(&gHardwareParameters.xcvrCal, 0xFF, sizeof(xcvr_DcocDacTrim_t));

    if (status == FALSE)
    {
        /* XCVR Cal is valid, we can restore it */
        FLib_MemCpy(xcvrDacTrim, &gHardwareParameters.xcvrCal, sizeof(xcvr_DcocDacTrim_t));
    }

    return status;
}
#endif /* gGenfskPreserveXcvrDacTrimValue_d */

#if defined (RADIO_IS_GEN_3P5)
/* API used to configure DCDC and LDOs to achieve high power/low power TX. 
The other API, GENFSK_SetTxPowerLevel() will select the power level.
Max power (low power config, TxPowerlevel=32): about 3.5 dBm
Max power (high power config, TxPowerlevel=32): about 5 dBm
Note that this function is a copy of what is done in MT LL. Idea at the end is
to create an API in framework (need to check with manager). */
genfskStatus_t GENFSK_ConfigurePower(uint8_t isHighPwrReq)
{
    genfskStatus_t  status = gGenfskSuccess_c;
    
    /*On high power request, only program required HW blocks if not already done.*/
    if (isHighPwrReq == 0x1U)
    {
        if (isHighPowerConfigured == 0x0U)
        {        
            /*TODO: This part of code will require some rework in the context of low power.*/       
            /*Save the initial config*/
            if (getDefaultRegisterConfig == 0x1U)
            {
                isDcdcClockEnabled = (uint8_t)((SIM->SCGC5 & SIM_SCGC5_DCDC_MASK) >> SIM_SCGC5_DCDC_SHIFT);
                targetValueVdd1p5 = 0xFFU;
                if (isDcdcClockEnabled == 0x1U)
                {
                    targetValueVdd1p5 = (uint8_t)((DCDC->REG3 &  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK) >> DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_SHIFT);
                }
                bbLdoHfTrimTx    = (uint8_t)((XCVR_ANALOG->BB_LDO_2 & XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK)  >> XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_SHIFT);
                getDefaultRegisterConfig = 0x0U;
            }
            
            /* The below function updates BB_LDO_2 register, make the call here to restore later on the correct value of bbLdoHfTrimTx */
            XCVR_bumpPaPowerUp();
            
            /* Configure DCDC */
            if (isDcdcClockEnabled == 0x0U)
            {
                SIM->SCGC5 |= SIM_SCGC5_DCDC_MASK;
            }
            /* At that point DCDC clock is enabled so access to DCDC registers is possible */
            DCDC->REG3 = (DCDC->REG3 & ~DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK) | DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK(14);
            
            XCVR_ANALOG->BB_LDO_2 &= ~XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK;
            XCVR_ANALOG->BB_LDO_2 |= XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX(5);

            isHighPowerConfigured = 0x1U;
        }
    }
    else
    {
        if (isHighPowerConfigured == 0x1U)
        {
            /*TODO: This part of code will require some rework in the context of low power.*/
            /*On low power request, restore back HW blocks to default configuration if high power was configured.*/
            /* if (((SIM->SCGC5 & SIM_SCGC5_DCDC_MASK) >> SIM_SCGC5_DCDC_SHIFT) == 0x1U) commented out as high power requires
            DCDC block to be on */

            if (isDcdcClockEnabled == 0x0U)
            {
                SIM->SCGC5 &= ~SIM_SCGC5_DCDC_MASK;
            }
            else
            {
                if (((DCDC->REG3 &  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK) >> DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_SHIFT) != targetValueVdd1p5)
                {
                    DCDC->REG3 = (DCDC->REG3 & ~DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK) | DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK(targetValueVdd1p5);
                }
            }
                        
            /* The below function updates BB_LDO_2 register, make the call here to not overwrite the value restored below (bbLdoHfTrimTx) */
            XCVR_releasePaPowerBump();
            
            if (((XCVR_ANALOG->BB_LDO_2 & XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK) >> XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_SHIFT) != bbLdoHfTrimTx)
            {
                XCVR_ANALOG->BB_LDO_2 &= ~XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX_MASK;
                XCVR_ANALOG->BB_LDO_2 |= XCVR_ANALOG_BB_LDO_2_BB_LDO_HF_TRIM_TX(bbLdoHfTrimTx);
            }

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
#endif
