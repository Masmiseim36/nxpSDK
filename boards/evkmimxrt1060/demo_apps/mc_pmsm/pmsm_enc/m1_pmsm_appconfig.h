/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/**********************************************************************/
// File Name: {FM_project_loc}/../src/projects/evkimxrt1060/pmsm_appconfig.h 
//
// Date:  November 29, 2018, 12:34:18
//
// Automatically generated file for static configuration of the PMSM FOC application
/**********************************************************************/

#ifndef __M1_PMSM_APPCONFIG_H
#define __M1_PMSM_APPCONFIG_H


//Motor Parameters                      
//----------------------------------------------------------------------
//Pole-pair number                      = 4 [-]
//Stator resistance                     = 0.72 [Ohms]
//Direct axis inductance                = 0.000326 [H]
//Quadrature axis inductance            = 0.000294 [H]
//Back-EMF constant                     = 0.0393 [V.sec/rad]
//Drive inertia                         = 0.000017 [kg.m2]
//Nominal current                       = 2 [A]

#define M1_MOTOR_PP                        (4)
//----------------------------------------------------------------------

//Application scales                    
//----------------------------------------------------------------------
#define M1_I_MAX                           (8.25F)
#define M1_U_DCB_MAX                       (60.8F)
#define M1_U_MAX                           (35.1F)
#define M1_N_MAX                           (1843.0F)
#define M1_FREQ_MAX                        (293.0F)
#define M1_U_DCB_TRIP                      (28.0F)
#define M1_U_DCB_UNDERVOLTAGE              (16.0F)
#define M1_U_DCB_OVERVOLTAGE               (30.0F)
#define M1_N_OVERSPEED                     (1842.6F)
#define M1_N_MIN                           (125.7F)
#define M1_N_NOM                           (1675.5F)
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
//Loop bandwidth                        = 288 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.0001 [sec]
//----------------------------------------------------------------------
//Current Controller Output Limit       
#define M1_CLOOP_LIMIT                     (0.519615484541F)
//D-axis Controller - Parallel type     
#define M1_D_KP_GAIN                       (0.459831404241F)
#define M1_D_KI_GAIN                       (0.106748630555F)
//Q-axis Controller - Parallel type     
#define M1_Q_KP_GAIN                       (0.344019732659F)
#define M1_Q_KI_GAIN                       (0.096270237371F)

//Speed Loop Control                    
//----------------------------------------------------------------------
//Loop bandwidth                        = 28 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.001 [sec]
//----------------------------------------------------------------------
//Speed Controller - Parallel type      
#define M1_SPEED_PI_PROP_GAIN              (0.009202212481F)
#define M1_SPEED_PI_INTEG_GAIN             (0.000080946889F)
#define M1_SPEED_LOOP_HIGH_LIMIT           (2.0F)
#define M1_SPEED_LOOP_LOW_LIMIT            (-2.0F)

#define M1_SPEED_RAMP_UP                   (4.188790204786F)
#define M1_SPEED_RAMP_DOWN                 (4.188790204786F)

#define M1_SPEED_IIR_B0                    (0.030459027951F)
#define M1_SPEED_IIR_B1                    (0.030459027951F)
#define M1_SPEED_IIR_A1                    (0.939081944097F)

#define M1_POS_P_PROP_GAIN                 FRAC16(0.12)

//Position & Speed Sensors Module       
//----------------------------------------------------------------------
//Loop Bandwidth                        = 100 [Hz]
//Loop Attenuation                      = 1 [-]
//Loop sample time                      = 0.0001 [sec]
//----------------------------------------------------------------------
#define M1_POSPE_KP_GAIN                   (1256.637061435917F)
#define M1_POSPE_KI_GAIN                   (39.47841760435744F)
#define M1_POSPE_INTEG_GAIN                (0.00003183098861837907F)
#define M1_POSPE_ENC_PULSES                (1000)
#define M1_POSPE_ENC_DIRECTION             (1)
#define M1_POSPE_ENC_N_MIN                 (0.0F)
#define M1_POSPE_MECH_POS_GAIN             ACC32(16.384)

//Sensorless BEMF DQ and Tracking Observer
//----------------------------------------------------------------------
//Loop bandwidth                        = 300 [Hz]
//Loop attenuation                      = 1 [-]
//Loop sample time                      = 0.0001 [sec]
//----------------------------------------------------------------------
//Bemf DQ Observer                      
#define M1_I_SCALE                         (0.819095477387F)
#define M1_U_SCALE                         (0.251256281408F)
#define M1_E_SCALE                         (0.251256281408F)
#define M1_WI_SCALE                        (0.000073869347F)
#define M1_BEMF_DQ_KP_GAIN                 (0.508991046085F)
#define M1_BEMF_DQ_KI_GAIN                 (0.115829677252F)

//Bemf DQ Observer                      
#define M1_TO_KP_GAIN                      (879.645943005142F)
#define M1_TO_KI_GAIN                      (19.344424626136F)
#define M1_TO_THETA_GAIN                   (0.000031830989F)
//Observer speed output filter          
#define M1_TO_SPEED_IIR_B0                 (0.111635211705F)
#define M1_TO_SPEED_IIR_B1                 (0.111635211705F)
#define M1_TO_SPEED_IIR_A1                 (0.776729576591F)
//Open loop start-up                    
#define M1_OL_START_RAMP_INC               (0.125663706144F)
#define M1_OL_START_I                      (0.65F)
#define M1_MERG_SPEED_TRH                  (209.43951023932F)
#define M1_MERG_COEFF                      FRAC16(0.004577636719)

//Control Structure Module - Scalar Control
//----------------------------------------------------------------------
#define M1_SCALAR_VHZ_FACTOR_GAIN          (0.056249999999999994F)
#define M1_SCALAR_INTEG_GAIN               ACC32(0.058666666667)
#define M1_SCALAR_RAMP_UP                  (4.188790204786F)
#define M1_SCALAR_RAMP_DOWN                (4.188790204786F)

#endif

//End of generated file                 
/**********************************************************************/
