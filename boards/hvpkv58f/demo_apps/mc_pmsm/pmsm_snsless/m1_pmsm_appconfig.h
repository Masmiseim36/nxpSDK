/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************/
// File Name: {FM_project_loc}/pmsm_appconfig.h 
//
// Date:  April 26, 2019, 12:26:40
//
// Automatically generated file for static configuration of the PMSM FOC application
/**********************************************************************/

#ifndef __M1_PMSM_APPCONFIG_H
#define __M1_PMSM_APPCONFIG_H


//Motor Parameters                      
//----------------------------------------------------------------------
//Pole-pair number                      = 4 [-]
//Stator resistance                     = 1.45 [Ohms]
//Direct axis inductance                = 0.00606 [H]
//Quadrature axis inductance            = 0.005762 [H]
//Back-EMF constant                     = 0.1135 [V.sec/rad]
//Drive inertia                         = 0.0000016 [kg.m2]
//Nominal current                       = 2 [A]

#define M1_MOTOR_PP                        (4)
//----------------------------------------------------------------------

//Application scales                    
//----------------------------------------------------------------------
#define M1_I_MAX                           (8.0F)
#define M1_U_DCB_MAX                       (433.0F)
#define M1_U_MAX                           (250.0F)
#define M1_N_MAX                           (1843.0F)
#define M1_FREQ_MAX                        (293.0F)
#define M1_U_DCB_TRIP                      (346.4F)
#define M1_U_DCB_UNDERVOLTAGE              (30.0F)
#define M1_U_DCB_OVERVOLTAGE               (346.4F)
#define M1_N_OVERSPEED                     (1750.9F)
#define M1_N_MIN                           (167.6F)
#define M1_N_NOM                           (1675.5F)
#define M1_I_PH_NOM                        (2.0F)
//DCB Voltage Filter                    
#define M1_UDCB_IIR_B0                     (0.030459027951F)
#define M1_UDCB_IIR_B1                     (0.030459027951F)
#define M1_UDCB_IIR_A1                     (0.939081944097F)
//Mechanical Alignment                  
#define M1_ALIGN_VOLTAGE                   (12.7F)
#define M1_ALIGN_DURATION                  (8000)
//Application counters                  
#define M1_CALIB_DURATION                  (200)
#define M1_FAULT_DURATION                  (3000)
#define M1_FREEWHEEL_DURATION              (1000)
//Miscellaneous                         
#define M1_E_BLOCK_TRH                     (7.0F)
#define M1_E_BLOCK_PER                     (2000)

//Current Loop Control                  
//----------------------------------------------------------------------
//Loop bandwidth                        = 280 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.0001 [sec]
//----------------------------------------------------------------------
//Current Controller Output Limit       
#define M1_CLOOP_LIMIT                     (0.519615484541F)
//D-axis Controller - Parallel type     
#define M1_D_KP_GAIN                       (19.872617658445F)
#define M1_D_KI_GAIN                       (1.87563541175F)
//Q-axis Controller - Parallel type     
#define M1_Q_KP_GAIN                       (18.824079694383F)
#define M1_Q_KI_GAIN                       (1.783401195133F)

//Speed Loop Control                    
//----------------------------------------------------------------------
//Loop bandwidth                        = 10 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.001 [sec]
//----------------------------------------------------------------------
//Speed Controller - Parallel type      
#define M1_SPEED_PI_PROP_GAIN              (0.000107103084F)
#define M1_SPEED_PI_INTEG_GAIN             (3.36475e-7F)
#define M1_SPEED_LOOP_HIGH_LIMIT           (2.0F)
#define M1_SPEED_LOOP_LOW_LIMIT            (-2.0F)

#define M1_SPEED_RAMP_UP                   (4.188790204786F)
#define M1_SPEED_RAMP_DOWN                 (4.188790204786F)

#define M1_SPEED_IIR_B0                    (0.030459027951F)
#define M1_SPEED_IIR_B1                    (0.030459027951F)
#define M1_SPEED_IIR_A1                    (0.939081944097F)

//Sensorless BEMF DQ and Tracking Observer
//----------------------------------------------------------------------
//Loop bandwidth                        = 280 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.0001 [sec]
//----------------------------------------------------------------------
//Bemf DQ Observer                      
#define M1_I_SCALE                         (0.97663174859F)
#define M1_U_SCALE                         (0.016116035456F)
#define M1_E_SCALE                         (0.016116035456F)
#define M1_WI_SCALE                        (0.000092860597F)
#define M1_BEMF_DQ_KP_GAIN                 (19.872617658445F)
#define M1_BEMF_DQ_KI_GAIN                 (1.875635411751F)

//Bemf DQ Observer                      
#define M1_TO_KP_GAIN                      (314.15926535898F)
#define M1_TO_KI_GAIN                      (2.467401100273F)
#define M1_TO_THETA_GAIN                   (0.000031830989F)
//Observer speed output filter          
#define M1_TO_SPEED_IIR_B0                 (0.111635211705F)
#define M1_TO_SPEED_IIR_B1                 (0.111635211705F)
#define M1_TO_SPEED_IIR_A1                 (0.776729576591F)
//Open loop start-up                    
#define M1_OL_START_RAMP_INC               (0.062831853072F)
#define M1_OL_START_I                      (0.2F)
#define M1_MERG_SPEED_TRH                  (209.43951023932F)
#define M1_MERG_COEFF                      FRAC16(0.004577636719)

//Control Structure Module - Scalar Control
//----------------------------------------------------------------------
#define M1_SCALAR_VHZ_FACTOR_GAIN          (0.75F)
#define M1_SCALAR_INTEG_GAIN               ACC32(0.058666666667)
#define M1_SCALAR_RAMP_UP                  (4.188790204786F)
#define M1_SCALAR_RAMP_DOWN                (4.188790204786F)

#endif

//End of generated file                 
/**********************************************************************/
