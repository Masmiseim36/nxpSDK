/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_ENC_QD_H_
#define _MCDRV_ENC_QD_H_

#include "mlib.h"
#include "mlib_types.h"
#include "fsl_device_registers.h"
#include "amclib.h"
#include "pmsm_control.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MCDRV_QD (1)
     
typedef struct _mcdrv_qd_enc_t
{
    AMCLIB_TRACK_OBSRV_T_FLT        sTo;                                        /* tracking observer structure */
    ENC_Type                       *pui32QdBase;                                /* pointer to QD module base address*/  
    float_t                        *pfltSpdMeEst;                               /* pointer to measured mechanical speed  */    
    frac16_t                       *pf16PosElEst;                               /* pointer to measured electrical position */
    acc32_t                         a32PosErr;                                  /* position error to tracking observer  */    
    acc32_t                         a32PosMeGain;                               /* encoder pulses to mechanical position scale gain */ 
    float_t                         fltSpdMeEst;                                /* estimated speed calculated using tracking observer */            
    frac16_t                        f16PosMe;                                   /* mechanical position calculated using encoder edges */
    frac16_t                        f16PosMeEst;                                /* estimated position calculated using tracking observer */        
    uint16_t                        ui16Pp;                                     /* number of motor pole pairs */
    bool_t                          bDirection;                                 /* encoder direction */  
    float_t                         fltSpdEncMin;                               /* encoder minimal speed resolution */
    frac16_t                        f16PosErr;                                  /* poisition error to tracking observer  */   
    frac16_t                        f16PosMeGain;                               /* encoder pulses to mechanical position scale gain */ 
    int16_t                         i16PosMeGainSh;                             /* encoder pulses to mechanical position scale shift */ 
    acc32_t                         a32PosMeReal;                               /* real position (revolution counter + mechanical position) */
    uint32_t                        ui32RevCounter;                             /* revolution counter measured by periphery */
} mcdrv_qd_enc_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


/*!
 * @brief Function returns actual position and speed
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncGet(mcdrv_qd_enc_t *this);

/*!
 * @brief Function clears internal variables and decoder counter
 *
 * @param this   Pointer to the current object
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncClear(mcdrv_qd_enc_t *this);

/*!
 * @brief Function se mechanical position of quadrature encoder
 *
 * @param this     Pointer to the current object
 *        f16PosMe Mechanical position
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncSetPosMe(mcdrv_qd_enc_t *this, frac16_t f16PosMe);

/*!
 * @brief Function set direction of quadrature encoder
 *
 * @param this       Pointer to the current object
 *        bDirection Encoder direction
 *
 * @return boot_t true on success
 */
bool_t MCDRV_QdEncSetDirection(mcdrv_qd_enc_t *this);

#ifdef __cplusplus
}
#endif

#endif /* _MCDRV_ENC_QD_H_ */

