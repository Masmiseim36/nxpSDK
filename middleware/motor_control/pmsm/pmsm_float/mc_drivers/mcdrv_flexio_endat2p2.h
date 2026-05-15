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

#ifndef _MCDRV_FLEXIO_ENDAT2P2_H_
#define _MCDRV_FLEXIO_ENDAT2P2_H_

#include "fsl_common.h"
#include "mlib_types.h"
#include "mlib.h"
#include "amclib_FP.h"
#include "fsl_flexio_endat2p2.h"


/*!
 * @addtogroup bissc
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

typedef struct _mcdrv_endat2p2
{
  
    FLEXIO_ENDAT2_Type endat2Master;
    
    float_t *pfltSpdMeEst;        /* pointer to measured mechanical speed  */
    frac16_t *pf16PosElEst;       /* pointer to measured electrical position */
    acc32_t *pa32PosMeReal;       /* pointer to real position (revolution counter + mechanical position) */ 

    acc32_t a32PosErr;            /* position error to tracking observer  */
    float_t fltSpdMeEst;          /* estimated speed calculated using encoder edges */
    frac16_t f16PosMe;            /* mechanical position calculated using encoder edges */
    frac16_t f16PosMeEst;         /* estimated position calculated using tracking observer */
    
    frac16_t f16PosOffset;
    
    uint16_t ui16Pp;              /* number of motor pole pairs */
    uint16_t ui16SpeedLoopFreq;   /* speed loop frequency - used in speed computation */
    
    uint64_t ui64EndatPosition;

    int64_t i64EndatPosition;
    int64_t i64EndatPositionOld;
    int64_t i64RevCounter;
    bool_t bEndatDir;

} mcdrv_endat2p2_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /*_cplusplus*/

/*!
 * @brief Function clears parameters of object
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Endat2p2Clear(mcdrv_endat2p2_t *base);

/*!
 * @brief Function sets revolutions offset
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Endat2p2SetOffset(mcdrv_endat2p2_t *base);
   
/*!
 * @brief Function reads raw data and converts to single turn and multi turn revolutions
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_Endat2p2DataRead(mcdrv_endat2p2_t *base);

/*!
 * @brief Function processes the data (fast-loop)
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_EnDatGetPositionFoc(mcdrv_endat2p2_t * base);

/*!
 * @brief Function processes the data (slow-loop)
 *
 * @param base   Pointer to the current object
 *
 * @return none
 */
RAM_FUNC_LIB
void MCDRV_EnDatGetPositionFullAndSpeed(mcdrv_endat2p2_t * base);

#if defined(__cplusplus)
}
#endif /*_cplusplus*/
/*@}*/

#endif /* MCDRV_FLEXIO_ENDAT2P2_H_*/
