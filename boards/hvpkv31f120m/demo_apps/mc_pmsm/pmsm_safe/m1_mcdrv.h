/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _M1_MCDRV_H
#define _M1_MCDRV_H

#include "mcdrv_pwm3ph_ftm.h"
#include "mcdrv_adc_adc16.h"
#include "mcdrv_gpio.h"

/*
 * Define motor 1 ADC control functions
 */
#define M1_MCDRV_ADC_GET_ALL()             (MCDRV_fsADCMeasGet())
#define M1_MCDRV_ADC_MEAS_INIT(par)        (MCDRV_fsCurr3Ph2ShChanAssignInit(par))
#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par) (MCDRV_fsCurr3Ph2ShChanAssign(par))
#define M1_MCDRV_CURR_3PH_CALIB_INIT()     (MCDRV_fsCurr3Ph2ShCalibInit())
#define M1_MCDRV_CURR_3PH_CALIB()          (MCDRV_fsCurr3Ph2ShCalib())
#define M1_MCDRV_CURR_3PH_CALIB_SET()      (MCDRV_fsCurr3Ph2ShCalibSet())
#define M1_MCDRV_MEAS                      (c_psfsADC16Meas)

/*
 * Define motor 1 three-phase PWM control functions
 */
#define M1_MCDRV_PWM3PH_SET(par)           (MCDRV_fsFtmPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN()               (MCDRV_fsFtmPwm3PhOutEn())
#define M1_MCDRV_PWM3PH_DIS()              (MCDRV_fsFtmPwm3PhOutDis())
#define M1_MCDRV_PWM3PH_FLT_GET()          (MCDRV_fsFtmPwm3PhFltGet())

/*
 * GPIO control driver
 */
#define M1_MCDRV_BRAKE_ON()                (MCDRV_fsBrakeOn())
#define M1_MCDRV_BRAKE_OFF()               (MCDRV_fsBrakeOff())

/* The measured data structure. */
extern const mcdrv_meas_t* const c_psfsADC16Meas;


#endif /* _M1_MCDRV_H_  */
