/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mcdrv_adc_hsadc.h"
#include "mcdrv_adc_hsadc_hvp_kv58.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

static bool_t s_statusPass;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initializes ADC driver to measure DC-bus current, DC-bus voltage
 *        and BEMF voltage for BLDC sensorless algorithm
 *        Board HVP-KV58F22 specific function
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_HSAdcInit_hvp_kv58(mcdrv_hsadc_t * this, mcdrv_hsadc_init_t * init)
{
    s_statusPass = TRUE;
    
    /* pointer to array with the channel numbers */
    this->pui16AdcArray = init->ui16AdcArray;

    /* default DC-bus current offset */
    this->ui16OffsetDcCurr = 0x3fff;
          
    /* check if there are one pair of assigned channels */
    if((init->ui16AdcArray[MCDRV_ADC0_BEMFA] == MCDRV_CHAN_OFF) && 
       (init->ui16AdcArray[MCDRV_ADC1_BEMFA] == MCDRV_CHAN_OFF))  
    {
        /* check if channel for phase A BEMF is assigned */
        s_statusPass = FALSE;
    }
    else if((init->ui16AdcArray[MCDRV_ADC0_BEMFB] == MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC1_BEMFB] == MCDRV_CHAN_OFF)) 
    {
        /* check if channel for phase B BEMF is assigned */
        s_statusPass = FALSE;
    }
    else if((init->ui16AdcArray[MCDRV_ADC0_BEMFC] == MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC1_BEMFC] == MCDRV_CHAN_OFF)) 
    {
        /* check if channel for phase C BEMF is assigned */
        s_statusPass = FALSE;
    }
    else if((init->ui16AdcArray[MCDRV_ADC0_UDCB]  == MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC1_UDCB]  == MCDRV_CHAN_OFF)) 
    {
        /* check if channel for DC-bus voltage is assigned */
        s_statusPass = FALSE;
    }
    else if((init->ui16AdcArray[MCDRV_ADC0_IDCB]  == MCDRV_CHAN_OFF) && 
            (init->ui16AdcArray[MCDRV_ADC1_IDCB]  == MCDRV_CHAN_OFF))
    {
        /* check if channel for DC-bus current is assigned */
        s_statusPass = FALSE;
    }
    else
    {
        /* ADC module result register assignement */
        this->ui16RsltNumBemf  = 8;
        this->ui16RsltNumAux   = 2;
        this->ui16RsltNumIdcb  = 0;
        this->ui16RsltNumUdcb  = 1;
        this->pui32Adc0Base  = init->pui32Adc0Base;
        this->pui32Adc1Base  = init->pui32Adc1Base;
       
        /*BEMF A*/
        this->bldcAdc0SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC1_BEMFA];  
        this->bldcAdc0SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC1_BEMFA];
        this->pui32BemfAAdcBase = init->pui32Adc1Base;
        
        /*BEMF B*/
        
        this->bldcAdc0SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC1_BEMFB];  
        this->bldcAdc0SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC1_BEMFB];
        this->pui32BemfBAdcBase = init->pui32Adc1Base;
        
        /*BEMF C*/    
        
        this->bldcAdc0SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];  
        this->bldcAdc0SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];  
        this->pui32BemfCAdcBase = init->pui32Adc1Base;
        this->pui32BemfCAdcBase->CLIST3 = ((this->pui32BemfCAdcBase->CLIST3 & ~(uint16_t)(HSADC_CLIST3_SAMPLE8_MASK)) |
                                           HSADC_CLIST3_SAMPLE8(init->ui16AdcArray[MCDRV_ADC1_BEMFC] + 8));
        this->ui16RsltNumBemfC  = 8;
        
        /*IDCB */
        
        this->pui32IdcbAdcBase = init->pui32Adc0Base;
        this->pui32IdcbAdcBase->CLIST1 = ((this->pui32IdcbAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE0_MASK)) |
                                           HSADC_CLIST1_SAMPLE0(init->ui16AdcArray[MCDRV_ADC0_IDCB]));
        
        /* auxiliary measurement */
        this->pui32AuxAdcBase = init->pui32Adc0Base;
        this->pui32AuxAdcBase->CLIST1 = ((this->pui32AuxAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE2_MASK)) |
                                           HSADC_CLIST1_SAMPLE2(init->ui16AdcArray[MCDRV_ADC0_AUX]));
        
        /* DC-bus voltage measurement */
        this->pui32UdcbAdcBase = init->pui32Adc0Base;
        this->pui32UdcbAdcBase->CLIST1 = ((this->pui32UdcbAdcBase->CLIST1 & ~(uint16_t)(HSADC_CLIST1_SAMPLE1_MASK)) |
                                           HSADC_CLIST1_SAMPLE1(init->ui16AdcArray[MCDRV_ADC0_UDCB]));
   }
    
    return(s_statusPass);
}

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing.
 *        Board HVP-KV58F22 specific function.
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_hvp_kv58(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector) 
    {
      /* BEMF phase C sensing */
    case 0:
    case 3:
        this->pui32BemfCAdcBase->CLIST3 = ((this->pui32BemfCAdcBase->CLIST3 & ~(uint16_t)(HSADC_CLIST3_SAMPLE8_MASK)) |
                                           HSADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFC] + 8));
      break;
    
      /* BEMF phase B sensing */
    case 1: 
    case 4: 
        this->pui32BemfBAdcBase->CLIST3 = ((this->pui32BemfBAdcBase->CLIST3 & ~(uint16_t)(HSADC_CLIST3_SAMPLE8_MASK)) |
                                           HSADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFB] + 8));
      break;
       
      /* BEMF phase A sensing */
    case 2:
    case 5:
        this->pui32BemfAAdcBase->CLIST3 = ((this->pui32BemfAAdcBase->CLIST3 & ~(uint16_t)(HSADC_CLIST3_SAMPLE8_MASK)) |
                                           HSADC_CLIST3_SAMPLE8(this->pui16AdcArray[MCDRV_ADC1_BEMFA] + 8));
      break;
      
    default:
      break;
    }
    
    return(s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual DC-bus current sample
 *
 *        Board HVP-KV58F22 specific function
 *
 *        Result register value is shifted three times to the right and stored
 *        to DC-bus current pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_BemfVoltageGet_hvp_kv58(mcdrv_hsadc_t *this)
{
    s_statusPass = TRUE;
   
    switch (this->ui16Sector) 
    {
      /* BEMF phase C sensing */
    case 0:
    case 3:
        *this->pf16BemfVoltage = (frac16_t) 
           (this->pui32Adc1Base->RSLT[this->ui16RsltNumBemf]);
      break;
    
      /* BEMF phase B sensing */
    case 1: 
    case 4: 
        *this->pf16BemfVoltage = (frac16_t) 
           (this->pui32Adc1Base->RSLT[this->ui16RsltNumBemf]);
      break;
       
      /* BEMF phase A sensing */
    case 2:
    case 5:
        *this->pf16BemfVoltage = (frac16_t) 
           (this->pui32Adc1Base->RSLT[this->ui16RsltNumBemf]); 
      break;
      
    default:
      break;
    }
    
    return(s_statusPass);
}

