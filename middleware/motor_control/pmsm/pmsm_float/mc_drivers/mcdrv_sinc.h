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

#ifndef _MCDRV_SINC_H_
#define _MCDRV_SINC_H_

#include "fsl_common.h"
#include "mlib_types.h"
#include "mlib_FP.h"
#include "gmclib_FP.h"

/*!
 * @addtogroup bissc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef struct _mcdrv_sinc
{
  SINC_Type *pui32SincBaseAddress;      /* Pointer to SINC base */
  GMCLIB_3COOR_T_FLT *psIABC;           /* Pointer to 3-phase currents. */
  float_t *pfltUDcBus;                  /* Pointer to DC Bus voltage variable */
  uint16_t *pui16SVMSector; /* pointer to the SVM sector */
  
  const float_t fltDCBvoltageScale;
  const float_t fltCurrentScale;
  const uint32_t ui32ENOB;
  const uint32_t ui32OSR;
  const uint32_t ui32ORD;
} mcdrv_sinc_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*!
 * @brief Function reads SINC data and converts to measured phase currentsand DC-bus voltage
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_SincReadPhCurrDcBusVolt(mcdrv_sinc_t *this);


#if defined(__cplusplus)
}
#endif /*_cplusplus*/
/*@}*/

#endif /* MCDRV_SINC_H_*/
