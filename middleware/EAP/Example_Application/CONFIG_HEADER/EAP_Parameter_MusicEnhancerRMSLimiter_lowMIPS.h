/*
* Copyright 2021 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/

// ** DO NOT MODIFY THIS SECTION **
// EAP API version: 3.0

#ifndef EAP_PARAMETER_MUSIC_ENHANCER_LOWMIPS
#define EAP_PARAMETER_MUSIC_ENHANCER_LOWMIPS
/*******************************************************************/
/* Include */
/*******************************************************************/


/***********************************************************************/
/* 							EQUALISER CONFIG 						   */
/***********************************************************************/
/* User EQ SET1:
 *
 */
LVM_EQNB_BandDef_t EQNB_BandDefs_UserEq1_musicEnhancerRmsLimiter_lowMIPS[LVM_EQNB_MAX_BANDS_NBR] 	= {
																  {6,50,96},   	// gain(dB), freq(Hz) , Qfactor *100
																  {3,205,96},
																  {1,837,96},
																  {0,0,0},
																  {0,0,0},
																  {0,0,0},
																  {0,0,0},
																  {0,0,0},
																  {0,0,0},
																  {0,0,0} };

/* Speaker EQ SET1:
 *
 */
LVM_EQNB_BandDef_t EQNB_BandDefs_ProductEq1_musicEnhancerRmsLimiter_lowMIPS[LVM_EQNB_MAX_BANDS_NBR] = {
																	{0,0,0},   		// gain(dB), freq(Hz) , Qfactor *100
																	{0,0,0},
																	{0,0,0},
																	{0,0,0},
																	{0,0,0},
																	{0,0,0},
																	{0,0,0},
																	{0,0,0},
																	{0,0,0},
																	{0,0,0} };


/***********************************************************************/
/* 						INSTANCE PARAM CONFIG 						   */
/***********************************************************************/
/* INSTANCE PARAM CONFIG SET1:
 *
 */
LVM_InstParams_t InstParams_musicEnhancerRmsLimiter_lowMIPS = {
	/* General */
	.BufferMode		= LVM_MANAGED_BUFFERS,					/* Buffer management mode */
	.MaxBlockSize 	= 480,      	    					/* Maximum processing block size max = 480 */
	.EQNB_NumBands 	= LVM_EQNB_MAX_BANDS_NBR,				/* Maximum number of band for the equalizer */
	.PR_EQNB_NumBands = LVM_EQNB_MAX_BANDS_NBR,   			/* Maximum number of band for the equalizer */
	.Platform = LVM_IMXRT1050,								/* Choose platform between : LVM_IMXRT1050,LVM_IMXRT1060, LVM_IMXRT1064, LVM_IMXRT1170, LVM_LPC55, LVM_IMXRT500, LVM_IMXRT600*/
#ifdef ALGORITHM_PSA
	/* PSA */
	.PSA_HistorySize 			= 1000,         			/* PSA History size in ms: 200 to 5000 */
	.PSA_MaxBands 				= LVM_PSA_MAX_NUMBANDS,		/* Maximum number of bands: 6 to 64 */
	.PSA_SpectrumUpdateRate 	= 25,  						/* Spectrum update rate : 10 to 25*/
	.PSA_Included 				= LVM_PSA_ON,				/* Controls the instance memory allocation for PSA: ON/OFF */
#endif /* ALGORITHM_PSA */
};


/***********************************************************************/
/* 						HEADROOM PARAM CONFIG 						   */
/***********************************************************************/
/* HEADROOM PARAM CONFIG SET1:
 *
 */
#ifdef ALGORITHM_EQNB
LVM_HeadroomBandDef_t HeadroomBandDef_musicEnhancerRmsLimiter_lowMIPS[LVM_HEADROOM_MAX_NBANDS]  = { {20,4999,3},    		/* Limit_Low, Limit_High, Headroom_Offset */
																						    {5000,24000,5},
																						    {0,0,0},
																						    {0,0,0},
																						    {0,0,0} };

LVM_HeadroomParams_t HeadroomParams_musicEnhancerRmsLimiter_lowMIPS = {
		.Headroom_OperatingMode	= LVM_HEADROOM_ON,  							/* Headroom Control On/Off */
		.pHeadroomDefinition	= &HeadroomBandDef_musicEnhancerRmsLimiter_lowMIPS[0],	/* Headroom band definition */
		.NHeadroomBands			= 2,     										/* Number of headroom bands */
};
#endif /* ALGORITHM_EQNB */


/***********************************************************************/
/* 						CONTROL PARAM CONFIG 						   */
/***********************************************************************/

/* CONTROL PARAM CONFIG SET1:
 *
 */
LVM_ControlParams_t	ControlParamSet_musicEnhancerRmsLimiter_lowMIPS = {
	/* General parameters */
	.OperatingMode	 	 = LVM_MODE_ON, 							// LVM_MODE_ON or LVM_MODE_OFF / Bundle operating mode On/Bypass
	.SampleRate 		 = LVM_FS_48000, 							// LVM_FS_8000, LVM_FS_11025, LVM_FS_12000, LVM_FS_16000, LVM_FS_22050, LVM_FS_24000, LVM_FS_32000, LVM_FS_44100, LVM_FS_48000,LVM_FS_96000
	.SourceFormat		 = LVM_STEREO, 								// LVM_MONO or LVM_MONOINSTEREO or LVM_STEREO or LVM_5DOT1 or LVM_7DOT1
	.SpeakerType 		 = LVM_HEADPHONES, 							// LVM_HEADPHONES or LVM_MOBILE_SPEAKERS_SMALL or LVM_MOBILE_SPEAKERS_MEDIUM or LVM_MOBILE_SPEAKERS_LARGE
	.SpeakerTypeInternal = LVM_SPEAKER_STEREO,						// Device speaker type, LVM_SPEAKER_MONO or LVM_SPEAKER_STEREO
#ifdef ALGORITHM_3DWIDENING
    /* Concert Sound Virtualizer parameters*/
	.VirtualizerOperatingMode 	= LVM_MODE_OFF, 					// LVM_MODE_ON or LVM_MODE_OFF;
	.VirtualizerType 			= LVM_CONCERTSOUND,					// LVM_CONCERTSOUND;
	.VirtualizerReverbLevel 	= (LVM_UINT16)100,  				// Virtualizer reverb level min = 0% to max = 100%
	.CS_EffectLevel 			= (LVM_INT16)32767,					// Concert Sound effect level min = 0 to max = 32767
#endif /* ALGORITHM_CS */

#ifdef ALGORITHM_EQNB
		/* N-Band Equalizer parameters */
	.EQNB_OperatingMode 			= LVM_EQNB_OFF, 			  		 // LVM_EQNB_ON or LVM_EQNB_OFF;
	.EQNB_LPF_Mode 					= LVM_EQNB_FILTER_OFF, 	  		 // LVM_EQNB_FILTER_ON or LVM_EQNB_FILTER_OFF;
	.EQNB_LPF_CornerFreq 			= (LVM_INT16)2000,		  		 // EQNB LowPass Corner Frequency;
	.EQNB_HPF_Mode 					= LVM_EQNB_FILTER_OFF,     		 // LVM_EQNB_FILTER_ON or LVM_EQNB_FILTER_OFF;
	.EQNB_HPF_CornerFreq			= (LVM_INT16)20,		  		 // EQNB HighPass Corner Frequency;
	.EQNB_NBands					= (LVM_UINT16)3,          		 // Number of bands 0 to LVM_EQNB_MAX_BANDS_NBR
	.pEQNB_BandDefinition			= &EQNB_BandDefs_UserEq1_musicEnhancerRmsLimiter_lowMIPS[0],// EQ band configuration
#endif /* ALGORITHM_EQNB */

#ifdef ALGORITHM_PR_EQNB
	/* N-Band Equalizer parameters */
	.PR_EQNB_OperatingMode 			= LVM_EQNB_OFF,			  		 // LVM_EQNB_ON or LVM_EQNB_OFF;
	.PR_EQNB_LPF_Mode 				= LVM_EQNB_FILTER_OFF, 	  		 // LVM_EQNB_FILTER_ON or LVM_EQNB_FILTER_OFF;
	.PR_EQNB_LPF_CornerFreq 		= (LVM_INT16)2000,		  		 // EQNB LowPass Corner Frequency;
	.PR_EQNB_HPF_Mode 				= LVM_EQNB_FILTER_OFF,     		 // LVM_EQNB_FILTER_ON or LVM_EQNB_FILTER_OFF;
	.PR_EQNB_HPF_CornerFreq			= (LVM_INT16)20,		  		 // EQNB HighPass Corner Frequency;
	.PR_EQNB_NBands					= (LVM_UINT16)0,          		 // Number of bands 0 to LVM_EQNB_MAX_BANDS_NBR
	.pPR_EQNB_BandDefinition		= &EQNB_BandDefs_ProductEq1_musicEnhancerRmsLimiter_lowMIPS[0],// EQ band configuration
#endif /* ALGORITHM_PR_EQNB */

#ifdef ALGORITHM_BASS
    /* Bass Enhancement parameters */
	.BE_OperatingMode 		= LVM_BE_ON, 							// LVM_BE_ON or LVM_BE_OFF;
	.BE_EffectLevel 		= (LVM_INT16)LVM_BE_9DB, 				// LVM_BE_0DB or LVM_BE_3DB or LVM_BE_6DB or LVM_BE_9DB or LVM_BE_12DB or LVM_BE_15DB;
	.BE_CentreFreq 			= LVM_BE_CENTRE_90Hz,					// LVM_BE_CENTRE_55Hz or LVM_BE_CENTRE_66Hz or LVM_BE_CENTRE_78Hz or LVM_BE_CENTRE_90Hz;
	.BE_HPF 				= LVM_BE_HPF_OFF, 						// LVM_BE_HPF_ON or LVM_BE_HPF_OFF;
#endif /* ALGORITHM_DBE */

    /* Volume Control parameters */
	.VC_EffectLevel 		= (LVM_INT16)0, 						// Volume Control setting in dBs -96 to 0 dB
	.VC_Balance     		= (LVM_INT16)0,   						// Left Right Balance control in dB (-96 to 96 dB)

#ifdef ALGORITHM_TE
    /* Treble Enhancement parameters */
	.TE_OperatingMode 		= LVM_TE_ON,  							// LVM_TE_ON or LVM_TE_OFF
	.TE_EffectLevel 		= (LVM_INT16)4, 						// Treble Enhancement gain 0dB to 15dB or LVM_TE_LOW_MIPS for saving MIPS

#endif /* ALGORITHM_TE */
#ifdef ALGORITHM_LM
    /* Loudness Maximizer parameters */
	.LM_OperatingMode 		= LVM_LM_OFF, 							// LVM_LM_ON or LVM_LM_OFF
	.LM_EffectLevel 		= LVM_LM_GENTLE, 						// LVM_LM_GENTLE or LVM_LM_MEDIUM or LVM_LM_EXTREME
	.LM_Attenuation 		= (LVM_UINT16)0, 						// 0 to 6 ; Loudness Maximizer output attenuation
	.LM_CompressorGain		= (LVM_UINT16)2, 						// 0 to 6 ; Loudness Maximizer output compressor gain
	.LM_SpeakerCutOff 		= (LVM_UINT16)0, 						// 150Hz to 1100Hz & 0 to disable; Loudness Maximizer speaker cut off frequency

#endif /* ALGORITHM_LM */

#ifdef ALGORITHM_AVL
    /* AVL parameters */
	.AVL_OperatingMode 		= LVM_AVL_OFF, 							// LVM_AVL_ON or LVM_AVL_OFF
#endif /* ALGORITHM_AVL */

#ifdef ALGORITHM_TG
    /* Tone Generator parameters */
	.TG_OperatingMode		= LVM_TG_OFF, 							// LVM_TG_OFF or LVM_TG_CONTINUOUS or LVM_TG_ONESHOT
	.TG_SweepMode			= LVM_TG_SWEEPLOG, 						// LVM_TG_SWEEPLIN or LVM_TG_SWEEPLOG
	.TG_StartFrequency 		= (LVM_UINT16)20, 						// Tone Generator Sweep Start Frequency 20 to 24000 Hz
	.TG_StartAmplitude		= (LVM_INT16)0,							// Tone Generator Sweep Start Amplitude -96 to 0 dB
	.TG_StopFrequency  		= (LVM_UINT16)2200,						// Tone Generator Sweep Stop Frequency 20 to 24000 Hz
	.TG_StopAmplitude  		= (LVM_INT16)0,							// Tone Generator Sweep Stop Amplitude -96 to 0 dB
	.TG_SweepDuration  		= (LVM_UINT16)60,						// Tone Generator Sweep Duration; Sweep duration in seconds, 0 for infinite duration tone
	.pTG_CallBack  			= LVM_NULL,    							// WARNING Callback not supported; End of sweep callback
	.TG_CallBackID 			= 0x100,          						// Callback ID
	.pTGAppMemSpace 		= (void *)LVM_NULL,						// Application instance handle or memory area
#endif /* ALGORITHM_TG */

#ifdef ALGORITHM_PSA
    /* General Control */
	.PSA_Enable				= LVM_PSA_ON, 							// LVM_PSA_ON or LVM_PSA_OFF;
    /* Spectrum Analyzer parameters */
	.PSA_PeakDecayRate 		= LVM_PSA_SPEED_SLOW,					// LVM_PSA_SPEED_SLOW or LVM_PSA_SPEED_MEDIUM or LVM_PSA_SPEED_FAST; Peak value decay rate
	.PSA_NumBands 			= (LVM_UINT16)32, 						// 6 to 64; Number of Bands
#endif /* ALGORITHM_PSA */

#ifdef ALGORITHM_LIMP
	.LIMP_OperatingMode = LVM_LIMP_OFF,								// LIMP operating mode: LVM_LIMP_ON or LVM_LIMP_OFF
	.LIMP_Threshold		= -3,    									// LIMP threshold in dB: -24dB to 0dB
#endif /* ALGORITHM_LIMP */

#ifdef ALGORITHM_LIMR
	.LIMR_OperatingMode = LVM_LIMR_ON,								// LVM_LIMR_ON or LVM_LIMR_OFF
	.LIMR_Reference		= LVM_LIMR_REF_INPUT,						// LIMR reference input: LVM_LIMR_REF_INPUT or LVM_LIMR_REF_0DBFS
	.LIMR_Threshold		= 0,										// LIMR threshold in dB: -24dB to 0dB
#endif /* ALGORITHM_LIMR */

	 /* advanced parameter */
	// Concert sound advanced parameter
#ifdef ALGORITHM_CS
    .CS_AP_Mode               = LVM_AP_DEFAULT,						// concert sound advanced parameter mode: LVM_AP_DEFAULT or LVM_AP_MANUAL
	.CS_AP_MidGain            = 0,            						// MidChannelGain: -10 to 10 dB
    .CS_AP_MidCornerFreq      = 500,      							// Shelving Filter Corner Frequency: 20 to 24000 Hz
    .CS_AP_SideHighPassCutoff = 600, 								// SideBoost HighPassFilter Corner Frequency: 20 to 24000 Hz
    .CS_AP_SideLowPassCutoff  = 1544,  								// SideBoost LowPassFilter Corner Frequency: 20 to 24000 Hz
    .CS_AP_SideGain           = 10,           						// Side Channel Gain: 0 to 15 dB
#endif
/*Crossover Parameters*/
#ifdef ALGORITHM_XO
		.XO_OperatingMode = LVM_MODE_OFF,							// LVM_MODE_ON or LVM_MODE_OFF
		.XO_cutoffFrequency = 100,									//Cutoff frequency in Hz (range  = [60 Hz - 6 000 Hz])
#endif // ALGORITHM_XO
};

#endif // end    EAP_PARAMETER_MUSIC_ENHANCER
