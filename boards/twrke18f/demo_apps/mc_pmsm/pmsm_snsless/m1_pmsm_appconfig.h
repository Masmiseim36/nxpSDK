/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************/
// File Name: {FM_project_loc}/pmsm_appconfig.h 
//
// Date:  April 26, 2019, 15:33:8
//
// Automatically generated file for static configuration of the PMSM FOC application
/**********************************************************************/

#ifndef __M1_PMSM_APPCONFIG_H
#define __M1_PMSM_APPCONFIG_H


//Motor Parameters                      
//----------------------------------------------------------------------
//Pole-pair number                      = 2 [-]
//Stator resistance                     = 0.47 [Ohms]
//Direct axis inductance                = 0.000385 [H]
//Quadrature axis inductance            = 0.00043 [H]
//Back-EMF constant                     = 0.0152 [V.sec/rad]
//Drive inertia                         = 0.000016 [kg.m2]
//Nominal current                       = 2 [A]

#define M1_MOTOR_PP                        (2)
//----------------------------------------------------------------------

//Application scales                    
//----------------------------------------------------------------------
#define M1_I_MAX                           (8.0F)
#define M1_U_DCB_MAX                       (36.3F)
#define M1_U_MAX                           (21.0F)
#define M1_N_MAX                           (922.0F)
#define M1_FREQ_MAX                        (147.0F)
#define M1_U_DCB_TRIP                      (28.0F)
#define M1_U_DCB_UNDERVOLTAGE              (16.0F)
#define M1_U_DCB_OVERVOLTAGE               (30.0F)
#define M1_N_OVERSPEED                     (900.6F)
#define M1_N_MIN                           (104.7F)
#define M1_N_NOM                           (837.8F)
#define M1_I_PH_NOM                        (2.0F)
//DCB Voltage Filter                    
#define M1_UDCB_IIR_B0                     (0.030459027951F)
#define M1_UDCB_IIR_B1                     (0.030459027951F)
#define M1_UDCB_IIR_A1                     (0.939081944097F)
//Mechanical Alignment                  
#define M1_ALIGN_VOLTAGE                   (1.2F)
#define M1_ALIGN_DURATION                  (4000)
//Application counters                  
#define M1_CALIB_DURATION                  (200)
#define M1_FAULT_DURATION                  (6000)
#define M1_FREEWHEEL_DURATION              (1500)
//Miscellaneous                         
#define M1_E_BLOCK_TRH                     (1.4F)
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
#define M1_D_KP_GAIN                       (0.884654752228F)
#define M1_D_KI_GAIN                       (0.119161655697F)
//Q-axis Controller - Parallel type     
#define M1_Q_KP_GAIN                       (1.042991021969F)
#define M1_Q_KI_GAIN                       (0.133089641428F)

//Speed Loop Control                    
//----------------------------------------------------------------------
//Loop bandwidth                        = 20 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.001 [sec]
//----------------------------------------------------------------------
//Speed Controller - Parallel type      
#define M1_SPEED_PI_PROP_GAIN              (0.005F)
#define M1_SPEED_PI_INTEG_GAIN             (0.00005F)
#define M1_SPEED_LOOP_HIGH_LIMIT           (3.0F)
#define M1_SPEED_LOOP_LOW_LIMIT            (-3.0F)

#define M1_SPEED_RAMP_UP                   (2.094395102393F)
#define M1_SPEED_RAMP_DOWN                 (2.094395102393F)

#define M1_SPEED_IIR_B0                    (0.030459027951F)
#define M1_SPEED_IIR_B1                    (0.030459027951F)
#define M1_SPEED_IIR_A1                    (0.939081944097F)

//Sensorless BEMF DQ and Tracking Observer
//----------------------------------------------------------------------
//Loop bandwidth                        = 300 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.0001 [sec]
//----------------------------------------------------------------------
//Bemf DQ Observer                      
#define M1_I_SCALE                         (0.891203703704F)
#define M1_U_SCALE                         (0.231481481482F)
#define M1_E_SCALE                         (0.231481481482F)
#define M1_WI_SCALE                        (0.000099537038F)
#define M1_BEMF_DQ_KP_GAIN                 (0.981415805959F)
#define M1_BEMF_DQ_KI_GAIN                 (0.136792717F)

//Bemf DQ Observer                      
#define M1_TO_KP_GAIN                      (251.327412287184F)
#define M1_TO_KI_GAIN                      (1.579136704175F)
#define M1_TO_THETA_GAIN                   (0.000031830989F)
//Observer speed output filter          
#define M1_TO_SPEED_IIR_B0                 (0.111635211705F)
#define M1_TO_SPEED_IIR_B1                 (0.111635211705F)
#define M1_TO_SPEED_IIR_A1                 (0.776729576591F)
//Open loop start-up                    
#define M1_OL_START_RAMP_INC               (0.062831853072F)
#define M1_OL_START_I                      (0.65F)
#define M1_MERG_SPEED_TRH                  (104.71975511966F)
#define M1_MERG_COEFF                      FRAC16(0.009155273438)

//Control Structure Module - Scalar Control
//----------------------------------------------------------------------
#define M1_SCALAR_VHZ_FACTOR_GAIN          (0.11249999999999999F)
#define M1_SCALAR_INTEG_GAIN               ACC32(0.029333333333)
#define M1_SCALAR_RAMP_UP                  (2.094395102393F)
#define M1_SCALAR_RAMP_DOWN                (2.094395102393F)

#endif

//End of generated file                 
/**********************************************************************/
