/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
void MID_calibHall(mid_calib_hall_t *sHallCalibFcn)
{
    /* Initialization */
    if (sHallCalibFcn->ui16Active == 0)
    {
        sHallCalibFcn->ui16Active = TRUE;
    }
}
