/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
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

#include "hall_calib.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Hall calibration routine
 *
 * @param *sHallCalibFcn input structure of type #mid_calib_hall_t for passing
 *                          all necessary parameters.
 *
 * @return None
 */
void MID_calibHall(mid_calib_hall_t* sHallCalibFcn)
{
    /* Initialization */
    if(sHallCalibFcn->bActive == FALSE)
    {
        sHallCalibFcn->bActive   = TRUE;
    }
}

