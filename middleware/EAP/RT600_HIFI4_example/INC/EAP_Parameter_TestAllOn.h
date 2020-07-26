/*
* Copyright 2020 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/

#ifndef EAP_PARAMETER_TEST_ALL_ON
#define EAP_PARAMETER_TEST_ALL_ON
/*******************************************************************/
/* Include */
/*******************************************************************/
#include "LVM.h" // EAP library

/***********************************************************************/
/* 							EQUALISER CONFIG 						   */
/***********************************************************************/
/* User EQ SET1:
 *
 */
LVM_EQNB_BandDef_t EQNB_BandDefs_UserEq1_testAllOn[MAX_EQNB_BANDS] 	= {
																  6,50,96,   	// gain(dB), freq(Hz) , Qfactor *100
																  3,205,96,
																  2,837,96,
																  2,3427,96,
																  0,1400,96,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0,
																  0,0,0 };

/* Speaker EQ SET1:
 *
 */
LVM_EQNB_BandDef_t EQNB_BandDefs_SpeakerEq1_testAllOn[MAX_EQNB_BANDS] = {
																	15,7000,1000,   		// gain(dB), freq(Hz) , Qfactor *100
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0,
																	0,0,0 };


/***********************************************************************/
/* 						INSTANCE PARAM CONFIG 						   */
/***********************************************************************/
/* INSTANCE PARAM CONFIG SET1:
 *
 */
LVM_InstParams_t InstParams_testAllOn = {
	/* General */
	.BufferMode		= LVM_MANAGED_BUFFERS,            /* Buffer management mode */
	.MaxBlockSize 	= 480,      	    			  /* Maximum processing block size max = 480 */
	.EQNB_NumBands 	= MAX_EQNB_BANDS,          		  /* Maximum number of band for the equalizer */
#ifdef ALGORITHM_PSA
	/* PSA */
	.PSA_HistorySize 			= 1000,         	  /* PSA History size in ms: 200 to 5000 */
	.PSA_MaxBands 				= LVM_MAX_PSA_BANDS,  /* Maximum number of bands: 6 to 64 */
	.PSA_SpectrumUpdateRate 	= 25,  				  /* Spectrum update rate : 10 to 25*/
	.PSA_Included 				= LVM_PSA_ON,         /* Controls the instance memory allocation for PSA: ON/OFF */
#endif /* ALGORITHM_PSA */
};


/***********************************************************************/
/* 						HEADROOM PARAM CONFIG 						   */
/***********************************************************************/
/* HEADROOM PARAM CONFIG SET1:
 *
 */
#ifdef ALGORITHM_EQNB
LVM_HeadroomBandDef_t HeadroomBandDef_testAllOn[LVM_HEADROOM_MAX_NBANDS]  = { 20,4999,3,    		/* Limit_Low, Limit_High, Headroom_Offset */
																		 5000,24000,4,
																		 0,0,0,
																		 0,0,0,
																		 0,0,0 };

LVM_HeadroomParams_t HeadroomParams_testAllOn = {
		.Headroom_OperatingMode	= LVM_HEADROOM_ON,  		/* Headroom Control On/Off */
		.pHeadroomDefinition	= &HeadroomBandDef_testAllOn[0], /* Headroom band definition */
		.NHeadroomBands			= 2,     					/* Number of headroom bands */
};
#endif /* ALGORITHM_EQNB */


/***********************************************************************/
/* 						CONTROL PARAM CONFIG 						   */
/***********************************************************************/

/* CONTROL PARAM CONFIG SET1:
 *
 */
LVM_ControlParams_t	ControlParamSet_testAllOn = {
	/* General parameters */
	.OperatingMode	 	 = LVM_MODE_ON, 		// LVM_MODE_ON or LVM_MODE_OFF / Bundle operating mode On/Bypass
	.SampleRate 		 = LVM_FS_48000, 	    // LVM_FS_8000, LVM_FS_11025, LVM_FS_12000, LVM_FS_16000, LVM_FS_22050, LVM_FS_24000, LVM_FS_32000, LVM_FS_44100, LVM_FS_48000
	.SourceFormat		 = LVM_STEREO, 		    // LVM_MONO or LVM_MONOINSTEREO or LVM_STEREO or LVM_5DOT1 or LVM_7DOT1
	.SpeakerType 		 = LVM_MOBILE_SPEAKERS_SMALL,  // LVM_HEADPHONES or LVM_MOBILE_SPEAKERS_SMALL or LVM_MOBILE_SPEAKERS_MEDIUM or LVM_MOBILE_SPEAKERS_LARGE
	.SpeakerTypeInternal = LVM_SPEAKER_STEREO,  // Device speaker type, LVM_SPEAKER_MONO or LVM_SPEAKER_STEREO
#ifdef ALGORITHM_3DWIDENING
    /* Concert Sound Virtualizer parameters*/
	.VirtualizerOperatingMode 	= LVM_MODE_ON,   		// LVM_MODE_ON or LVM_MODE_OFF;
	.VirtualizerType 			= LVM_CONCERTSOUND,     // LVM_CONCERTSOUND or LVM_CINEMASOUND_MUSIC or LVM_CINEMASOUND_MOVIE or LVM_CINEMASOUND_STEREO;
	.VirtualizerReverbLevel 	= (LVM_UINT16)75,   	// Virtualizer reverb level min = 0% to max = 100%
	.CS_EffectLevel 			= (LVM_INT16)30000,     // Concert Sound effect level min = 0 to max = 32767
#endif /* ALGORITHM_CS */

#ifdef ALGORITHM_EQNB
		/* N-Band Equalizer parameters */
	.EQNB_OperatingMode 			= LVM_EQNB_ON, 			  		 // LVM_EQNB_ON or LVM_EQNB_OFF;
	.EQNB_LPF_Mode 					= LVM_EQNB_FILTER_OFF, 	  		 // LVM_EQNB_FILTER_ON or LVM_EQNB_FILTER_OFF;
	.EQNB_LPF_CornerFreq 			= (LVM_INT16)20000,		  		 // EQNB LowPass Corner Frequency;
	.EQNB_HPF_Mode 					= LVM_EQNB_FILTER_OFF,     		 // LVM_EQNB_FILTER_ON or LVM_EQNB_FILTER_OFF;
	.EQNB_HPF_CornerFreq			= (LVM_INT16)20,		  		 // EQNB HighPass Corner Frequency;
	.EQNB_NBands					= (LVM_UINT16)4,          		 // Number of bands 0 to MAX_EQNB_BANDS
	.pEQNB_BandDefinition			= &EQNB_BandDefs_UserEq1_testAllOn[0],// EQ band configuration

#endif /* ALGORITHM_EQNB */

#ifdef ALGORITHM_BASS
    /* Bass Enhancement parameters */
	.BE_OperatingMode 		= LVM_BE_ON, 				// LVM_BE_ON or LVM_BE_OFF;
	.BE_EffectLevel 		= (LVM_INT16)LVM_BE_6DB, 	// LVM_BE_0DB or LVM_BE_3DB or LVM_BE_6DB or LVM_BE_9DB or LVM_BE_12DB or LVM_BE_15DB;
	.BE_CentreFreq 			= LVM_BE_CENTRE_55Hz,		// LVM_BE_CENTRE_55Hz or LVM_BE_CENTRE_66Hz or LVM_BE_CENTRE_78Hz or LVM_BE_CENTRE_90Hz;
	.BE_HPF 				= LVM_BE_HPF_ON, 			// LVM_BE_HPF_ON or LVM_BE_HPF_OFF;
#endif /* ALGORITHM_DBE */

    /* Volume Control parameters */
	.VC_EffectLevel 		= (LVM_INT16)-3, 			// Volume Control setting in dBs -96 to 0 dB
	.VC_Balance     		= (LVM_INT16)0,   			// Left Right Balance control in dB (-96 to 96 dB)

#ifdef ALGORITHM_TE
    /* Treble Enhancement parameters */
	.TE_OperatingMode 		= LVM_TE_ON,  		// LVM_TE_ON or LVM_TE_OFF
	.TE_EffectLevel 		= (LVM_INT16)9, 	// Treble Enhancement gain 0dB to 15dB or LVM_TE_LOW_MIPS for saving MIPS

#endif /* ALGORITHM_TE */
#ifdef ALGORITHM_LM
    /* Loudness Maximizer parameters */
	.LM_OperatingMode 		= LVM_LM_ON, 		// LVM_LM_ON or LVM_LM_OFF
	.LM_EffectLevel 		= LVM_LM_MEDIUM, 	// LVM_LM_GENTLE or LVM_LM_MEDIUM or LVM_LM_EXTREME
	.LM_Attenuation 		= (LVM_UINT16)0, 	// 0 to 6 ; Loudness Maximizer output attenuation
	.LM_CompressorGain		= (LVM_UINT16)2, 	// 0 to 6 ; Loudness Maximizer output compressor gain
	.LM_SpeakerCutOff 		= (LVM_UINT16)0, 	// 150 to 1100; Loudness Maximizer speaker cut off frequency

#endif /* ALGORITHM_LM */
#ifdef ALGORITHM_GM
    /* Gentle Mix parameters */
	.GM_OperatingMode 		= LVM_GM_OFF,      	// Gentle mix operating mode LVM_GM_ON or LVM_GM_OFF
	.GM_EffectLevel 		= LVM_GM_FULL,      // Gentle mix effect level LVM_GM_REDUCED or LVM_GM_FULL

#endif /* ALGORITHM_GM */
#ifdef ALGORITHM_RS
    /* Richsound parameters */
	.RS_OperatingMode		= LVM_RS_OFF, 				// LVM_RS_ON or LVM_RS_OFF
	.RS_Config 				= LVM_RS_CONFIG_FEQ, 	// LVM_RS_CONFIG_AVL_AEQ or LVM_RS_CONFIG_AVL_FEQ or LVM_RS_CONFIG_AVL or LVM_RS_CONFIG_FEQ
	.RS_AVLEffect 			= LVM_RS_AVL_LOW, 			// LVM_RS_AVL_LOW or LVM_RS_AVL_MEDIUM or LVM_RS_AVL_HIGH;  Richsound auto volume effect
	.RS_EQBandLow   		= (LVM_INT16)0,  			// Rich Sound Low Band gain Set
	.RS_EQBandMid   		= (LVM_INT16)0,  			// Rich Sound Mid Band gain Set
	.RS_EQBandHigh  		= (LVM_INT16)0,  			// Rich Sound High Band gain Set
#endif /* ALGORITHM_RS */
#ifdef ALGORITHM_AVL
    /* AVL parameters */
	.AVL_OperatingMode 		= LVM_AVL_ON, 				// LVM_AVL_ON or LVM_AVL_OFF
#endif /* ALGORITHM_AVL */

#ifdef ALGORITHM_TG
    /* Tone Generator parameters */
	.TG_OperatingMode		= LVM_TG_ONESHOT, 		// LVM_TG_OFF or LVM_TG_CONTINUOUS or LVM_TG_ONESHOT
	.TG_SweepMode			= LVM_TG_SWEEPLOG, 		// LVM_TG_SWEEPLIN or LVM_TG_SWEEPLOG
	.TG_StartFrequency 		= (LVM_UINT16)20, 		// Tone Generator Sweep Start Frequency 20 to 24000 Hz
	.TG_StartAmplitude		= (LVM_INT16)-48,		// Tone Generator Sweep Start Amplitude -96 to 0 dB
	.TG_StopFrequency  		= (LVM_UINT16)22000,    // Tone Generator Sweep Stop Frequency 20 to 24000 Hz
	.TG_StopAmplitude  		= (LVM_INT16)-45,		// Tone Generator Sweep Stop Amplitude -96 to 0 dB
	.TG_SweepDuration  		= (LVM_UINT16)1,		// Tone Generator Sweep Duration; Sweep duration in seconds, 0 for infinite duration tone
	.pTG_CallBack  			= LVM_NULL,    			// WARNING Callback not supported; End of sweep callback
	.TG_CallBackID 			= 0x100,          		// Callback ID
	.pTGAppMemSpace 		= (void *)LVM_NULL,     // Application instance handle or memory area
#endif /* ALGORITHM_TG */

#ifdef ALGORITHM_PSA
    /* General Control */
	.PSA_Enable				= LVM_PSA_ON, 			  // LVM_PSA_ON or LVM_PSA_OFF;
    /* Spectrum Analyzer parameters */
	.PSA_PeakDecayRate 		= LVM_PSA_SPEED_SLOW,     // LVM_PSA_SPEED_SLOW or LVM_PSA_SPEED_MEDIUM or LVM_PSA_SPEED_FAST; Peak value decay rate
	.PSA_NumBands 			= (LVM_UINT16)32, 		  // 6 to 64; Number of Bands
#endif /* ALGORITHM_PSA */
};

/***********************************************************************/
/* 						CUSTOM TUNING CONFIG 						   */
/***********************************************************************/

/* CUSTOM PARAM CONFIG SET1:
 *
 */
#ifdef EXAMPLE_CUSTOM_TUNING
LVM_Custom_TuningParams_st CustomTuningParams_testAllOn = {
	.MidGain 			 = 0,            			 /* MidChannelGain */
	.MidCornerFreq 		 = 500,      				 /* Shelving Filter Corner Frequency */
    .SideHighPassCutoff  = 600, 					 /* SideBoost HighPassFilter Corner Frequency */
    .SideLowPassCutoff 	 = 1544,  					 /* SideBoost LowPassFilter Corner Frequency */
    .SideGain 			 = 15,           			 /* Side Channel Gain */

    .Elevation 			 = 10,          			 /* Virtual speaker elevation in degrees */
    .FrontAngle 		 = 45,         				 /* Front speaker angle */
    .SurroundAngle 		 = 90,      				 /* Surround speaker angle */

    .Tuning_HPF_Mode 	    = LVM_EQNB_FILTER_OFF,    /* High pass filter */
    .Tuning_HPF_CornerFreq  = 100,
    .NumTuningBands 		= 1,     				 /* Number of tuning equalization bands */
    .pTuningBandDefs        = &EQNB_BandDefs_SpeakerEq1_testAllOn[0], /* EQ band configuration */
};
#endif /* EXAMPLE_CUSTOM_TUNING */

#endif // end    EAP_PARAMETER_TEST_ALL_ON
