/*
* Copyright 2025 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */

#include "mcdrv_sinc.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Codes
 ******************************************************************************/

/*!
 * @brief Function reads SINC data and converts to measured phase currentsand DC-bus voltage
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_SincReadPhCurrDcBusVolt(mcdrv_sinc_t *this)
{
  float_t fltTmp;
  uint32_t ui32ChannelId;
  int32_t i32Sinc1ResultsArray;
  float_t fltSinc1Results[4];

  /* Read channels 0-3 */
  for( ui32ChannelId = 0U; ui32ChannelId  < 4; ++ui32ChannelId )
  {
      const uint32_t u32CurrentCocMask = (uint32_t)(1U << (ui32ChannelId + SINC_NIS_CHF0_SHIFT));

      this->pui32SincBaseAddress->NIS = u32CurrentCocMask;	// Clear the CHF flag

      /* Read FIFO to discard useless data in FIFO, do not use this sample */
      i32Sinc1ResultsArray = (int32_t)this->pui32SincBaseAddress->CHANNEL[ui32ChannelId].CRDATA;
       
      // ENOB is 13 bits, so right bitshift of 19
      i32Sinc1ResultsArray = (int32_t)(this->pui32SincBaseAddress->CHANNEL[ui32ChannelId].CRDATA) >> 19U;
      fltTmp = ((float_t)i32Sinc1ResultsArray) / ((float_t)0x800);        // get result within range <-1;1>
      fltSinc1Results[ui32ChannelId] = fltTmp;
  }
  
  /* Use the newest SINC measured phase currents */
  this->psIABC->fltA = (-1.0F) * fltSinc1Results[0] * this->fltCurrentScale;
  this->psIABC->fltB = (-1.0F) * fltSinc1Results[1] * this->fltCurrentScale;
  this->psIABC->fltC = (-1.0F) * fltSinc1Results[2] * this->fltCurrentScale;
  *this->pfltUDcBus = fltSinc1Results[3] * this->fltDCBvoltageScale;
}

