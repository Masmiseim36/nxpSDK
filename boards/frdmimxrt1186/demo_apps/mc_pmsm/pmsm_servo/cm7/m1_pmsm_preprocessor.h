/*
    * Copyright 2025 NXP 
    *
    * SPDX-License-Identifier: BSD-3-Clause 
*/

/*
{
    "motorName": "TGT3-0065-60-48",
    "motorDescription": "Datasheet:\nKe = 3.49V/1000rpm\nKt = 0.058 Nm/A\nR2ph = 0.221Ohm\nL2ph = 0.25 mH"
}
*/

#ifndef __M1_PMSM_APPCONFIG_H 
#define __M1_PMSM_APPCONFIG_H 


// CONSTANTS
#define UmaxCoeff 		(1.73205F)
#define DiscMethodFactor 	(2.0F)
#define ParametersKk 		(1.0F)


// PARAMETERS
#define ParametersPp 						(3.0F)
#define ParametersRs 						(0.1105F)
#define ParametersLd 						(0.000116467F)
#define ParametersLq 						(0.000134492F)
#define ParametersKt 						(0.058F)
#define ParametersJ  						(0.00005F)
#define ParametersIphNom 					(11.1F)
#define ParametersUphNom 					(48.0F)
#define ParametersNnom 						(6000.0F)
#define ParametersImax 						(34.2F)
#define ParametersUdcbMax 					(77.35F)
#define ParametersUdcbTrip 					(28.0F)
#define ParametersUdcbUnder 				(12.0F)
#define ParametersUdcbOver 					(34.0F)
#define ParametersNover 					(5999.0F)
#define ParametersNmin 						(300.0F)
#define ParametersEblock 					(0.2F)
#define ParametersEblockPer 				(2000.0F)
#define ParametersNmax 						(4400.0F)
#define ParametersUdcbIIRf0 				(100.0F)
#define ParametersCalibDuration 			(0.2F)
#define ParametersFaultDuration 			(2.0F)
#define ParametersFreewheelDuration 		(1.5F)
#define ParametersScalarUqMin 				(1.0F)
#define ParametersAlignVoltage 				(1.2F)
#define ParametersAlignDuration 			(1)
#define ParametersScalarVHzRatio 			(50.0F)


// CURRENT LOOP PARAMETERS	
#define CurrentLoopSampleTime				(0.000015625F)
#define CurrentLoopF0						(400.0F)
#define CurrentLoopKsi						(1.0F)
#define CurrentLoopOutputLimit				(90.0F)
		
		
// SPEED LOOP PARAMETERS	
#define SpeedLoopSampleTime 				(0.00025F)
#define SpeedLoopF0 						(10.0F)
#define SpeedLoopKsi 						(1.0F)
#define SpeedLoopIncUp 						(4000.0F)
#define SpeedLoopIncDown 					(5000.0F)
#define SpeedLoopCutOffFreq 				(100.0F)
#define SpeedLoopUpperLimit 				(4.0F)
#define SpeedLoopLowerLimit 				(-4.0F)
#define SpeedLoopSLKp 						(0.009202212F)
#define SpeedLoopSLKi 						(0.00008095F)
					
#define SpeedLoopManualConstantTunning 		(false)
	
	
// POSITION LOOP PARAMETERS	
#define PositionLoopSampleTime 				(0.00025F)
#define PositionLoopF0 						(15.0F)
#define PositionLoopKsi 					(1.0F)
#define Servo_positionLoopUpperLimit 		(4000.0F)
#define Servo_positionLoopLowerLimit 		(-4000.0F)
#define Servo_speedLoopUpperLimit 			(8.0F)
#define Servo_speedLoopLowerLimit 			(-8.0F)
	
	
// SENSORLESS PARAMETERS	
#define SensorlessBemfObsrvF0 				(150.0F)
#define SensorlessBemfObsrvKsi 				(1.0F)
#define SensorlessTrackObsrvF0 				(35.0F)
#define SensorlessTrackObsrvKsi 			(1.0F)
#define SensorlessTrackObsrvIIRSpeedCutOff 	(400.0F)
#define SensorlessStartupRamp 				(3000.0F)
#define SensorlessStartupCurrent 			(0.6F)
#define SensorlessMergingSpeed 				(500.0F)
#define SensorlessMergingCoeff 				(100.0F)


/* PARAMETERS */
#define M1_MOTOR_PP 	(ParametersPp)
#define M1_I_PH_NOM 	(ParametersIphNom)

#define M1_N_NOM	(ParametersNnom / 60 * (ParametersPp * 2 * FLOAT_PI)) 		
#define M1_I_MAX	(ParametersImax) 											
#define M1_U_DCB_MAX	(ParametersUdcbMax)								
#define M1_U_DCB_TRIP	(ParametersUdcbTrip) 										
#define M1_U_DCB_UNDERVOLTAGE (ParametersUdcbUnder)									
#define M1_U_DCB_OVERVOLTAGE (ParametersUdcbOver)									
#define M1_N_OVERSPEED 	(ParametersNover / 60 * (ParametersPp * 2 * FLOAT_PI))		
#define M1_N_MIN 	(ParametersNmin / 60 * (ParametersPp * 2 * FLOAT_PI))		
#define M1_E_BLOCK_TRH 	(ParametersEblock)											
#define M1_E_BLOCK_PER 	(ParametersEblockPer)										
#define M1_N_MAX (ParametersNmax / 60 * (ParametersPp * 2 * FLOAT_PI))				
#define M1_CALIB_DURATION (ParametersCalibDuration / SpeedLoopSampleTime)			
#define M1_FAULT_DURATION (uint16_t)(ParametersFaultDuration / SpeedLoopSampleTime)			
#define M1_FREEWHEEL_DURATION (uint16_t)(ParametersFreewheelDuration / SpeedLoopSampleTime)	
#define M1_SCALAR_UQ_MIN (ParametersScalarUqMin)									
#define M1_ALIGN_VOLTAGE (ParametersAlignVoltage)									
#define M1_ALIGN_DURATION (uint16_t)(ParametersAlignDuration / CurrentLoopSampleTime)			
#define M1_U_MAX 		(ParametersUdcbMax / UmaxCoeff)								
#define M1_FREQ_MAX		((ParametersNmax / 60) * ParametersPp)						
#define M1_N_ANGULAR_MAX (60 / (ParametersPp * 2 * FLOAT_PI))						
#define M1_UDCB_IIR_B0 	(2 * FLOAT_PI * ParametersUdcbIIRf0 * CurrentLoopSampleTime) / (2 + (2 * FLOAT_PI * ParametersUdcbIIRf0 * CurrentLoopSampleTime)) 		
#define M1_UDCB_IIR_B1 	(2 * FLOAT_PI * ParametersUdcbIIRf0 * CurrentLoopSampleTime) / (2 + (2 * FLOAT_PI * ParametersUdcbIIRf0 * CurrentLoopSampleTime))			
#define M1_UDCB_IIR_A1 	(-(2 * FLOAT_PI * ParametersUdcbIIRf0 * CurrentLoopSampleTime - 2) / (2 + (2 * FLOAT_PI * ParametersUdcbIIRf0 * CurrentLoopSampleTime)))	
#define M1_SCALAR_VHZ_FACTOR_GAIN (ParametersUphNom*ParametersScalarVHzRatio/100/(ParametersNnom*ParametersPp/60))

#define M1_SCALAR_INTEG_GAIN ACC32(2*FLOAT_PI*ParametersPp*ParametersNmax/60*CurrentLoopSampleTime/FLOAT_PI)				
#define M1_SCALAR_RAMP_UP (SpeedLoopIncUp*CurrentLoopSampleTime/60*ParametersPp)									
#define M1_SCALAR_RAMP_DOWN (SpeedLoopIncDown*CurrentLoopSampleTime/60*ParametersPp)								


/* CURRENTLOOP */
#define M1_D_KP_GAIN   ((2 * CurrentLoopKsi * 2 * FLOAT_PI * CurrentLoopF0 * ParametersLd) - ParametersRs) 						
#define M1_D_KI_GAIN   (((2 * FLOAT_PI * CurrentLoopF0)*(2 * FLOAT_PI * CurrentLoopF0) * ParametersLd) * CurrentLoopSampleTime / DiscMethodFactor)
#define M1_Q_KP_GAIN   ((2 * CurrentLoopKsi * 2 * FLOAT_PI * CurrentLoopF0 * ParametersLq) - ParametersRs) 
#define M1_Q_KI_GAIN   (((2 * FLOAT_PI * CurrentLoopF0)*(2 * FLOAT_PI * CurrentLoopF0) * ParametersLq) * CurrentLoopSampleTime / DiscMethodFactor)

#define kp_q 	((2 * CurrentLoopKsi * 2 * FLOAT_PI * CurrentLoopF0 * ParametersLq) - ParametersRs) // in S-domain
#define ki_q_z 	(((2 * FLOAT_PI * CurrentLoopF0)*(2 * FLOAT_PI * CurrentLoopF0) * ParametersLq) * CurrentLoopSampleTime / DiscMethodFactor) // in Z-domain 
#define ki_q_s	(ki_q_z * (2/CurrentLoopSampleTime)) // in S domain
#define Ti 		(kp_q/ki_q_s)
	
#define M1_Q_IIR_ZC_B0 (CurrentLoopSampleTime/(CurrentLoopSampleTime + 2*Ti)) 
#define M1_Q_IIR_ZC_B1 (CurrentLoopSampleTime/(CurrentLoopSampleTime + 2*Ti))
#define M1_Q_IIR_ZC_A1 (((-1)*(CurrentLoopSampleTime-2*Ti)/(CurrentLoopSampleTime+2*Ti)))

#define M1_CLOOP_LIMIT (CurrentLoopOutputLimit / UmaxCoeff / 100)


/* SPEEDLOOP */
#define SensorlessWmax 		     (2 * FLOAT_PI * ParametersPp * ParametersNmax / 60)

#define M1_SPEED_RAMP_UP 		 (SpeedLoopIncUp * SpeedLoopSampleTime / (60 / (ParametersPp * 2 * FLOAT_PI)))
#define M1_SPEED_RAMP_DOWN 		 (SpeedLoopIncDown * SpeedLoopSampleTime / (60 / (ParametersPp * 2 * FLOAT_PI)))
#define M1_SPEED_LOOP_HIGH_LIMIT (SpeedLoopUpperLimit)
#define M1_SPEED_LOOP_LOW_LIMIT  (SpeedLoopLowerLimit)

#define M1_SPEED_PI_PROP_GAIN 	 ((4 * SpeedLoopKsi * FLOAT_PI * SpeedLoopF0 * ParametersJ )/ (ParametersKt * ParametersPp))
#define M1_SPEED_PI_INTEG_GAIN 	 ((((2 * FLOAT_PI * SpeedLoopF0) * (2 * FLOAT_PI * SpeedLoopF0) * ParametersJ) / (ParametersKt * ParametersPp)) * (SpeedLoopSampleTime/DiscMethodFactor))

#define M1_SPEED_IIR_B0 		 (2 * FLOAT_PI * SpeedLoopCutOffFreq * SpeedLoopSampleTime) / (2 + (2 * FLOAT_PI * SpeedLoopCutOffFreq * SpeedLoopSampleTime)) 	
#define M1_SPEED_IIR_B1 		 (2 * FLOAT_PI * SpeedLoopCutOffFreq * SpeedLoopSampleTime) / (2 + (2 * FLOAT_PI * SpeedLoopCutOffFreq * SpeedLoopSampleTime)) 	
#define M1_SPEED_IIR_A1 		 (-(2 * FLOAT_PI * SpeedLoopCutOffFreq * SpeedLoopSampleTime - 2) / (2 + (2 * FLOAT_PI * SpeedLoopCutOffFreq * SpeedLoopSampleTime)))

#define kp_w  		(M1_SPEED_PI_PROP_GAIN)	// Z and S domain has the same value
#define ki_w_z 		(M1_SPEED_PI_INTEG_GAIN) 	// in Z domain
#define ki_w_s 		(ki_w_z * (2/SpeedLoopSampleTime)) // in S domain
#define Tfilt_w  	(kp_w/ki_w_s) // Ti = kp_w/ki_w

#define M1_SPEED_IIR_ZC_B0 		 (SpeedLoopSampleTime / (SpeedLoopSampleTime + 2 * Tfilt_w )) 					
#define M1_SPEED_IIR_ZC_B1 		 (SpeedLoopSampleTime / (SpeedLoopSampleTime + 2 * Tfilt_w )) 					
#define M1_SPEED_IIR_ZC_A1 		 ((-1)*(SpeedLoopSampleTime - 2 * Tfilt_w) / (SpeedLoopSampleTime + 2 * Tfilt_w ))


/* POSITIONLOOP */
#define M1_SERVO_POSITION_P_HIGH_LIMIT 	(2 * FLOAT_PI * Servo_positionLoopUpperLimit * ParametersPp / 60.0)
#define M1_SERVO_POSITION_P_LOW_LIMIT 	(2 * FLOAT_PI * Servo_positionLoopLowerLimit * ParametersPp / 60.0)

#define k10 (1/(2*FLOAT_PI*ParametersPp)) // Output from encoder is mechanical speed, needed recomputation to angular electrical speed
#define k12 (1)
#define k_feed (1) // Feed forward gain

#define M1_SERVO_POSITION_P_PROP_GAIN 	((ParametersKk * (2 * FLOAT_PI * PositionLoopF0))/( (2 * PositionLoopKsi * ParametersKk + 1) * k10*k12))
#define M1_SERVO_FEED_FRWD_K1 			(( 2*PositionLoopKsi*M1_SERVO_POSITION_P_PROP_GAIN/(k_feed* (2 * FLOAT_PI * PositionLoopF0)) ))
#define M1_SERVO_FEED_FRWD_K2 			(M1_SERVO_POSITION_P_PROP_GAIN/(k_feed*(2*FLOAT_PI*PositionLoopF0)*(2*FLOAT_PI*PositionLoopF0)))

#define Tfilt_fi (2*PositionLoopKsi + ParametersKk)/((2*PositionLoopKsi*ParametersKk + 1)*2*FLOAT_PI*PositionLoopF0)

#define M1_SERVO_IIR_ZC_B0 				(PositionLoopSampleTime/(PositionLoopSampleTime + 2 * Tfilt_fi))  					
#define M1_SERVO_IIR_ZC_B1 				(PositionLoopSampleTime/(PositionLoopSampleTime + 2 * Tfilt_fi))  					
#define M1_SERVO_IIR_ZC_A1 				((-1)*(PositionLoopSampleTime - 2*Tfilt_fi)/(PositionLoopSampleTime + 2*Tfilt_fi))  
#define M1_SERVO_SPEED_PI_PROP_GAIN 	( ((2 * PositionLoopKsi + ParametersKk) * (2 * FLOAT_PI * PositionLoopF0) * ParametersJ) / (ParametersKt * ParametersPp) )
#define M1_SERVO_SPEED_PI_INTEG_GAIN 	( ((ParametersJ * (2 * PositionLoopKsi * ParametersKk + 1) * (2 * FLOAT_PI * PositionLoopF0) * (2 * FLOAT_PI * PositionLoopF0)) / (ParametersKt*ParametersPp) ) * (PositionLoopSampleTime / DiscMethodFactor))
#define M1_SERVO_SPEED_PI_HIGH_LIMIT 	(Servo_speedLoopUpperLimit)
#define M1_SERVO_SPEED_PI_LOW_LIMIT 	(Servo_speedLoopLowerLimit)


/* SENSORLESS */
#define M1_OL_START_RAMP_INC	(SensorlessStartupRamp * CurrentLoopSampleTime / (60 / (ParametersPp * 2 * FLOAT_PI)))
#define M1_OL_START_I 			(SensorlessStartupCurrent)
#define M1_MERG_SPEED_TRH 		(SensorlessMergingSpeed / (60 / (ParametersPp * 2 * FLOAT_PI)))
//#define M1_MERG_COEFF 			FRAC16( (Math.round((((SensorlessMergingCoeff / 100)* SensorlessMergingSpeed * ParametersPp * CurrentLoopSampleTime)/60) * 32768)) * 1/(32768) )
#define M1_MERG_COEFF 			FRAC16( (((SensorlessMergingCoeff / 100)* SensorlessMergingSpeed * ParametersPp * CurrentLoopSampleTime)/60) )
#define M1_I_SCALE 				(ParametersLd / (ParametersLd + CurrentLoopSampleTime * ParametersRs)) 
#define M1_U_SCALE 				(CurrentLoopSampleTime / (ParametersLd + CurrentLoopSampleTime * ParametersRs))
#define M1_E_SCALE 				(CurrentLoopSampleTime / (ParametersLd + CurrentLoopSampleTime * ParametersRs))
#define M1_WI_SCALE 			(ParametersLq * CurrentLoopSampleTime / (ParametersLd + CurrentLoopSampleTime * ParametersRs))
#define M1_BEMF_DQ_KP_GAIN 		(2 * SensorlessBemfObsrvKsi * 2 * FLOAT_PI * SensorlessBemfObsrvF0 * ParametersLd - ParametersRs)
#define M1_BEMF_DQ_KI_GAIN 		(ParametersLd * (2 * FLOAT_PI * SensorlessBemfObsrvF0)*(2 * FLOAT_PI * SensorlessBemfObsrvF0) * CurrentLoopSampleTime)
#define M1_TO_KP_GAIN 			(2 * SensorlessTrackObsrvKsi * 2 * FLOAT_PI * SensorlessTrackObsrvF0)
#define M1_TO_KI_GAIN 			((2 * FLOAT_PI * SensorlessTrackObsrvF0)*(2 * FLOAT_PI * SensorlessTrackObsrvF0) * CurrentLoopSampleTime)
#define M1_TO_THETA_GAIN 		(CurrentLoopSampleTime / FLOAT_PI)
#define M1_TO_SPEED_IIR_B0 		(2 * FLOAT_PI * SensorlessTrackObsrvIIRSpeedCutOff * CurrentLoopSampleTime) / (2 + (2 * FLOAT_PI * SensorlessTrackObsrvIIRSpeedCutOff * CurrentLoopSampleTime)) 
#define M1_TO_SPEED_IIR_B1 		(2 * FLOAT_PI * SensorlessTrackObsrvIIRSpeedCutOff * CurrentLoopSampleTime) / (2 + (2 * FLOAT_PI * SensorlessTrackObsrvIIRSpeedCutOff * CurrentLoopSampleTime))
#define M1_TO_SPEED_IIR_A1 		(-(2 * FLOAT_PI * SensorlessTrackObsrvIIRSpeedCutOff * CurrentLoopSampleTime - 2) / (2 + (2 * FLOAT_PI * SensorlessTrackObsrvIIRSpeedCutOff * CurrentLoopSampleTime)))


/* USER INPUT START */
/* USER INPUT END */
#endif /* __M1_PMSM_APPCONFIG_H */
