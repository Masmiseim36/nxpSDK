/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "mcdrv_adc_adc16.h"
#include "mcdrv_adc_adc16_hvp_ke18.h"

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
 * @brief Function set new channel assignment for next BEMF voltage sensing.
 *        Board HVP-KE18F16 specific function.
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_AssignBemfChannel_hvp_ke18(mcdrv_adc16_t *this)
{
    s_statusPass = TRUE;

    switch (this->ui16Sector) 
    {
      /* BEMF phase C sensing */
    case 0:
    case 3:
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC1)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      break;
    
      /* BEMF phase B sensing */
    case 1: 
    case 4: 
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC1)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      break;
       
      /* BEMF phase A sensing */
    case 2:
    case 5:
      if((this->bldcAdcSelCfg[this->ui16Sector]) == ADC1)
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc0SectorCfg[this->ui16Sector]));
      else
      /* Set ADC_SC1_ADCH bitfield */
      this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] = 
        (this->bldcAdcSelCfg[this->ui16Sector]->SC1[this->ui16IndexBemf] & ~(uint16_t)(ADC_SC1_ADCH(ADC_SC1_ADCH_MASK))) | 
          (ADC_SC1_ADCH(this->bldcAdc1SectorCfg[this->ui16Sector]));
      
      break;
      
    default:
      break;
    }
    
    return(s_statusPass);
}

