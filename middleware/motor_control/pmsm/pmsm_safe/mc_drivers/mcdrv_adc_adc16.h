/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _MCDRV_ADC_ADC16_H_
#define _MCDRV_ADC_ADC16_H_

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

#include "gmclib.h"

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* The output measurement data structure. */
typedef struct _mcdrv_meas
{
    GMCLIB_3COOR_T_F16 sIABC;
    frac16_t           f16UDcBus;
    frac16_t           f16AUX;
} mcdrv_meas_t;

/* Pointer to the measurement data. The data structure is updated by
   MCDRV_fsADCMeasGet() function execution. */
extern const mcdrv_meas_t* const c_psfsADC16Meas;

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief   The function reads all ADC measurements and updates the
 *          c_psfsADC16Meas output data structure.
 *
 * @details Function reads and calculates three-phase samples based on the
 *          actual SVM sector. The DC-bus voltage and auxiliary samples are
 *          read as well.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsADCMeasGet(void);

/*!
 * @brief   Set the initial channel assignment for phase currents, DC-bus
 *          voltage and the auxiliary quantity.
 *
 * @param   ui16SVMSector - The required initial SVM sector.
 *
 * @return  none
 */
void MCDRV_fsCurr3Ph2ShChanAssignInit(uint16_t ui16SVMSector);

/*!
 * @brief   Function sets new channel assignment for the next sampling based on
 *          the SVM sector.
 *
 * @param   ui16SVMSector - The required SVM sector value.
 *
 * @return  none
 */
void MCDRV_fsCurr3Ph2ShChanAssign(uint16_t ui16SVMSector);

/*!
 * @brief   Function initializes phase current channel offset measurement.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsCurr3Ph2ShCalibInit(void);

/*!
 * @brief   Function reads current samples and filter them based on the actual
 *          SVM sector.
 *
 * @param   void
 *
 * @return  Number of remaining calibration samples to be taken.
 */
uint16_t MCDRV_fsCurr3Ph2ShCalib(void);

/*!
 * @brief   Function passes measured offset values to the main ADC structure.
 *
 * @param   void
 *
 * @return  none
 */
void MCDRV_fsCurr3Ph2ShCalibSet(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MCDRV_ADC_ADC16_H_ */

