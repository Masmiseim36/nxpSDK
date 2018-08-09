/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "mcdrv_adc_adc16.h"
#include "mcdrv_adc_adc16_hvp_kv31.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
* Variables
******************************************************************************/

static bool_t s_statusPass;
static volatile bool_t s_bkey = TRUE;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Initializes ADC driver to measure DC-bus current, DC-bus voltage
 *        and BEMF voltage for BLDC sensorless algorithm.
 *        Board HVP-KV31F120M specific function.
 *
 * @param this   Pointer to the current object
 * @param init   Pointer to initialization structure
 *
 * @return boot_t true on success
 */
bool_t MCDRV_Adc16Init_hvp_kv31(mcdrv_adc16_t * this, mcdrv_adc16_init_t * init)
{
    s_statusPass = TRUE;
    
    /* offset filter window */
    this->ui16OffsetFiltWindow = 3;
    
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
        /* 0 -> result register 0 for ADC0 or ADC1 */
        /* 1 -> result register 1 for ADC0 or ADC1 */
        this->ui16IndexBemf  = 0;
        this->ui16IndexAux   = 0;
        this->ui16IndexUdcb  = 1;
        this->ui16IndexIdcb  = 1;
        
        /* BEMF phase A measurement */
        if(init->ui16AdcArray[MCDRV_ADC0_BEMFA] == MCDRV_CHAN_OFF)
        {
            this->pui32BemfAAdcBase = init->pui32Adc1Base ;
            this->bldcAdc1SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC1_BEMFA];   
            this->bldcAdcSelCfg[2] = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC1_BEMFA];  
            this->bldcAdcSelCfg[5] = init->pui32Adc1Base;
        }
        else
        {
            this->pui32BemfAAdcBase = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[2] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];  
            this->bldcAdcSelCfg[2] = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[5] = init->ui16AdcArray[MCDRV_ADC0_BEMFA];  
            this->bldcAdcSelCfg[5] = init->pui32Adc0Base;
        }
        
        /* BEMF phase B measurement */
        if(init->ui16AdcArray[MCDRV_ADC0_BEMFB] == MCDRV_CHAN_OFF)
        {
            this->pui32BemfBAdcBase = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC1_BEMFB];  
            this->bldcAdcSelCfg[1] = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC1_BEMFB];  
            this->bldcAdcSelCfg[4] = init->pui32Adc1Base;
        }
        else
        {
            this->pui32BemfBAdcBase = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[1] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];  
            this->bldcAdcSelCfg[1] = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[4] = init->ui16AdcArray[MCDRV_ADC0_BEMFB];  
            this->bldcAdcSelCfg[4] = init->pui32Adc0Base;
        }
        
        /* BEMF phase C measurement */
        if(init->ui16AdcArray[MCDRV_ADC0_BEMFC] == MCDRV_CHAN_OFF)
        {
            this->pui32BemfCAdcBase = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];  
            this->bldcAdcSelCfg[0] = init->pui32Adc1Base;
            this->bldcAdc1SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC1_BEMFC];  
            this->bldcAdcSelCfg[3] = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] = 
              (this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_BEMFC]));
        }
        else
        {
            this->pui32BemfCAdcBase = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[0] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];  
            this->bldcAdcSelCfg[0] = init->pui32Adc0Base;
            this->bldcAdc0SectorCfg[3] = init->ui16AdcArray[MCDRV_ADC0_BEMFC];  
            this->bldcAdcSelCfg[3] = init->pui32Adc0Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] = 
              (this->pui32BemfCAdcBase->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_BEMFC]));
         }
        
        /* DC-bus current measurement */
        if(init->ui16AdcArray[MCDRV_ADC0_IDCB] == MCDRV_CHAN_OFF)
        {
            this->pui32IdcbAdcBase = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] = 
              (this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_IDCB]));
            
        }
        else
        {
            this->pui32IdcbAdcBase = init->pui32Adc0Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] = 
              (this->pui32IdcbAdcBase->SC1[this->ui16IndexIdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_IDCB]));
            
        }

        /* auxiliary measurement */
        this->pui32AuxAdcBase = init->pui32Adc0Base;
        /* Set ADC_SC1_ADCH bitfield */
        this->pui32AuxAdcBase->SC1[this->ui16IndexAux] = 
          (this->pui32AuxAdcBase->SC1[this->ui16IndexAux] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
            (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_AUX]));
            
        
        /* DC-bus voltage measurement */
        if(init->ui16AdcArray[MCDRV_ADC0_UDCB] == MCDRV_CHAN_OFF)
        {
            this->pui32UdcbAdcBase = init->pui32Adc1Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] = 
              (this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC1_UDCB]));
        
        }
        else
        {
            this->pui32UdcbAdcBase = init->pui32Adc0Base;
            /* Set ADC_SC1_ADCH bitfield */
            this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] = 
              (this->pui32UdcbAdcBase->SC1[this->ui16IndexUdcb] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
                (ADC_SC1_ADCH(init->ui16AdcArray[MCDRV_ADC0_UDCB]));
        
        }

        /* trigger */
        ADC0->SC1[0] |= ADC_SC1_AIEN_MASK;
        
    }
    
    return(s_statusPass);
}

/*!
 * @brief Function set new channel assignment for next BEMF voltage sensing.
 *        Board HVP-KV31F120M specific function. 
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_hvp_kv31(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector) 
    {
    /* BEMF phase C sensing */
    case 0:
    case 3:
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
      {
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
            
      
      }
      else
      {
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      ADC0->SC1[this->ui16IndexAux] = (ADC0->SC1[this->ui16IndexAux] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | (ADC_SC1_ADCH(MCDRV_TEMP));
      }
      break;
    
    /* BEMF phase B sensing */
    case 1: 
    case 4: 
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      
      else
      {
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      ADC0->SC1[this->ui16IndexAux] = (ADC0->SC1[this->ui16IndexAux] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | (ADC_SC1_ADCH(MCDRV_TEMP));
      }
      break;
       
    /* BEMF phase A sensing */
    case 2:
    case 5:
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC0)
      {
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
        
      ADC1->SC1[this->ui16IndexAux] = (ADC1->SC1[this->ui16IndexAux] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | (ADC_SC1_ADCH(MCDRV_TEMP));
      }      
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      break;
      
    default:
      
      break;
    }
    
    return(s_statusPass);
}

/*!
 * @brief Function reads ADC result register containing actual auxiliary sample
 *        Board HVP-KV31F120M specific function. 
 *
 *        Result register value is shifted 3 times right and stored to
 *        auxiliary pointer
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AuxValGet_hvp_kv31(mcdrv_adc16_t * this)
{
    s_statusPass = TRUE;
    
    switch (this->ui16Sector) 
    {
    /* when BEMF phase C is sensing */
    case 0:
    case 3: 
      *this->pui16AuxChan = (frac16_t) (MLIB_ShLSat_F16((ADC0->R[this->ui16IndexAux] ), 3));
      break;
    
    /* when BEMF phase B is sensing */
    case 1: 
    case 4: 
       *this->pui16AuxChan = (frac16_t) (MLIB_ShLSat_F16((ADC0->R[this->ui16IndexAux] ), 3));
       break;
       
    /* when BEMF phase A is sensing */
    case 2:
    case 5:
      *this->pui16AuxChan = (frac16_t) (MLIB_ShLSat_F16((ADC1->R[this->ui16IndexAux] ), 3));
      break;
      
    /* default */
    default:
       *this->pui16AuxChan = (frac16_t) (MLIB_ShLSat_F16((ADC0->R[this->ui16IndexAux] ), 3));
       break;
    }
    return(s_statusPass);
}

