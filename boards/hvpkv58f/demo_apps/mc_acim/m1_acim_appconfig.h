/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
/*------------------------------------------------------------------
    Automatically generated file for static configuration of the
                      ACIM FOC application

File Name: {FM_project_loc}/m1_acim_appconfig.h 

Date:  April 29, 2019, 14:56:19
------------------------------------------------------------------*/
#ifndef _M1_ACIM_APPCONFIG_H_
#define _M1_ACIM_APPCONFIG_H_

#define M1_SQRT2                         (1.414213562373F)

/* motor & application parameters        
------------------------------------------------------------------
pole-pair number                         = 2 [-]
stator resistance                        = 25.223 [Ohms]
rotor resistance                         = 23.004 [Ohms]
stator inductance                        = 0.534 [H]
rotor inductance                         = 0.534 [H]
mutual inductance                        = 0.487 [H]
moment of inertia                        = 0.000303 [kg.m2]
mechanical time constant                 = 0.843 [s]
------------------------------------------------------------------*/
#define M1_MOTOR_PP                      (2)
#define M1_U_NOM                         (230.0F)
#define M1_I_NOM                         (0.9F)
#define M1_F_NOM                         (50.0F)
#define M1_R_S                           (25.223F)
#define M1_R_R                           (23.004F)
#define M1_L_S                           (0.534F)
#define M1_L_R                           (0.534F)
#define M1_L_M                           (0.487F)
/* application scales */                 
#define M1_I_MAX                         (8.0F)
#define M1_U_MAX                         (250.0F)
#define M1_U_DCB_MAX                     (433.0F)
/* fault thresholds */                   
#define M1_U_DCB_TRIP                    (346.4F)
#define M1_U_DCB_UNDERVOLTAGE            (173.2F)
#define M1_U_DCB_OVERVOLTAGE             (368.1F)
#define M1_N_OVERSPEED                   (4950.0F)
/* DC-bus voltage filter */              
#define M1_UDCB_IIR_B0                   (0.030459027951F)
#define M1_UDCB_IIR_B1                   (0.030459027951F)
#define M1_UDCB_IIR_A1                   (0.939081944097F)
/* application counters */               
#define M1_CALIB_DURATION                (200)
#define M1_FAULT_DURATION                (5000)
#define M1_FREEWHEEL_DURATION            (2000)

/* current loop control                  
------------------------------------------------------------------
loop bandwidth                           = 150 [Hz]
loop attenuation                         = 1 [-]
loop sample time                         = 0.0001 [sec]
------------------------------------------------------------------*/
#define M1_CLOOP_LIMIT                   (0.519615484541F)
/* d-axis controller - parallel type */  
#define M1_D_KP_GAIN                     (144.165322098667F)
#define M1_D_KI_GAIN                     (7.982236624636F)
/* q-axis controller - parallel type */  
#define M1_Q_KP_GAIN                     (144.165322098667F)
#define M1_Q_KI_GAIN                     (7.982236624636F)

/* speed loop control                    
------------------------------------------------------------------
loop bandwidth                           = 2 [Hz]
loop attenuation                         = 1 [-]
loop sample time                         = 0.001 [sec]
speed ramp acceleration                  = 6000 [rpm/s]
speed ramp jerk                          = 30000 [rpm/s^2]
------------------------------------------------------------------*/
/* speed controller - parallel type */   
#define M1_SPEED_PI_KP_GAIN              (0.000578437199F)
#define M1_SPEED_PI_KI_GAIN              (0.000003760565F)
#define M1_SPEED_PI_BETA_GAIN            (1.0F)
#define M1_SPEED_LIMIT_HIGH              (0.9F)
#define M1_SPEED_LIMIT_LOW               (-0.9F)
/* required speed limitations */         
#define M1_SPEED_REQ_RMP_ACC             (6000.0F)
#define M1_SPEED_REQ_RMP_JRK             (30000.0F)
#define M1_SPEED_REQ_RMP_DUR             (0.05F)
#define M1_SPEED_REQ_MAX                 (1500.0F)
#define M1_SPEED_REQ_MIN                 (300.0F)
/* speed filter parameters */            
#define M1_SPEED_IIR_B0                  (0.003131753958F)
#define M1_SPEED_IIR_B1                  (0.003131753958F)
#define M1_SPEED_IIR_A1                  (0.993736492083F)

/* flux loop control                     
------------------------------------------------------------------
maximal d-axis current                   
MTPA minimal d-axis current              = 0.27 [A]
MTPA maximal d-axis current              = 0.9 [A]
MTPA filter bandwidth                    = 10 [Hz]
FW control bandwidth                     = 0.25 [Hz]
FW filter bandwidth                      = 10 [Hz]
------------------------------------------------------------------*/
#define M1_FLUX_ID_MIN                   (0.27F)
#define M1_FLUX_ID_MAX                   (0.9F)
#define M1_FLUX_MTPA_FILT_COEFF          (0.060898632576F)
#define M1_FLUX_ID_START                 (0.9F)
#define M1_FLUX_ID_START_MINPCT          (0.95F)
#define M1_FLUX_FW_ID_MIN                (0.27F)
#define M1_FLUX_FW_KP_GAIN               (0.074873617061F)
#define M1_FLUX_FW_KI_GAIN               (0.00322545447F)
#define M1_FLUX_IIR_B0                   (0.030459027951F)
#define M1_FLUX_IIR_B1                   (0.030459027951F)
#define M1_FLUX_IIR_A1                   (0.939081944097F)

/* rotor flux observer                   
------------------------------------------------------------------*/
#define M1_RFO_COMP_KP_GAIN              (25.0F)
#define M1_RFO_COMP_KI_GAIN              (0.01F)
#define M1_RFO_KR_INV_GAIN               (1.096509240246F)
#define M1_RFO_LS_TOT_LEAK_GAIN          (0.098535934292F)
#define M1_RFO_PSI_RA1_GAIN              (0.995710612933F)
#define M1_RFO_PSI_RB1_GAIN              (0.002088931502F)
#define M1_RFO_PSI_SA1_GAIN              (0.999372076006F)
#define M1_RFO_PSI_SA2_GAIN              (0.000099937208F)
#define M1_RFO_RS_EST                    (25.223F)
#define M1_RFO_TRQ_CNST                  (2.73595505618F)
#define M1_MRAS_KP_GAIN                  (32750.0F)
#define M1_MRAS_KI_GAIN                  (12500.0F)
#define M1_MRAS_PSI_RA1                  (0.995710612933F)
#define M1_MRAS_PSI_RB1                  (0.002088931502F)
#define M1_MRAS_TS                       (0.0001F)
#define M1_MRAS_SPDCNST                  (4.774648292757F)

/* scalar control                        
------------------------------------------------------------------*/
#define M1_SCALAR_VHZ_FACTOR_GAIN        (0.083333333333F)
#define M1_SCALAR_INTEG_GAIN             (0.000020943951F)
#define M1_SCALAR_RAMP_UP                (0.6F)
#define M1_SCALAR_RAMP_DOWN              (0.6F)
#define M1_SCALAR_U_MIN                  (25.0F)

/* motor parameter identification        
------------------------------------------------------------------*/
#define M1_P_NOM                         (250.0F)
#define M1_MID_TIME_ONESEC               (10000)
#define M1_MID_MECH_TRQ                  (1.0F)
#define M1_MID_CALC_FREQ_CLOOP           (150.0F)
#define M1_MID_CALC_FREQ_SLOOP           (2.0F)

#endif /* _M1_ACIM_APPCONFIG_H_ */
/*
*######################################################################
*                           End of File
*######################################################################
*/
