/*
* Copyright 2021 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/


/************************************************************************************

*************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/*  Definitions                                                                     */
/*                                                                                  */
/************************************************************************************/
/*
 * Parameter choice
 *     - pick up 1 parameter setting for EAP
 */
//#define EAP_PARAM_ALL_EFFECT_OFF
//#define EAP_PARAM_VOICE_ENHANCER
//#define EAP_PARAM_MUSIC_ENHANCER_RMSLIMITER
//#define EAP_PARAM_AUTO_VOLUME_LEVELER
//#define EAP_PARAM_CONCERTSOUND
//#define EAP_PARAM_LOUDNESS_MAXIMISER
//#define EAP_PARAM_TONE_GENERATOR
//#define EAP_PARAM_CUSTOM
//#define EAP_PARAM_TEST_ALL_ON     // Parameter set used only for MIPS, memory & bit exactness test but not for audio purpose
#define EAP_PARAM_CROSSOVER_FOR_SUBWOOFER
//#define EAP_PARAM_CROSSOVER_2_WAY_SPEAKER
/*
 * This example application show some optional example which can be enable or disable
 */
// parameter update example (to be run with EAP_PARAM_LOUDNESS_MAXIMISER configuration)
//#define	EXAMPLE_PARAMETER_UPDATE
//#define	EXAMPLE_PARAMETER_UPDATE_TIME 			 2000 //example happens once at 2000ms

// volume with no smoothing update
//#define	EXAMPLE_VOLUME_UPDATE_NO_SMOOTHING
//#define	EXAMPLE_VOLUME_UPDATE_NO_SMOOTHING_TIME  3000 //example happens once at 3000ms

// AVL gain is monitored
//#define EXAMPLE_AVL_READ_GAIN

/************************************************************************************/
/*                                                                                  */
/*  Includes                                                                        */
/*                                                                                  */
/************************************************************************************/
#include "TEST_PARAM.h"

#include "EAP_Parameter_AllEffectOff.h"
#include "EAP_Parameter_VoiceEnhancer.h"
#include "EAP_Parameter_MusicEnhancerRMSLimiter.h"
#include "EAP_Parameter_LoudnessMaximiser.h"
#include "EAP_Parameter_ConcertSound.h"
#include "EAP_Parameter_AutoVolumeLeveler.h"
#include "EAP_Parameter_Custom.h"
#include "EAP_Parameter_ToneGenerator.h"
#include "EAP_Parameter_TestAllOn.h"
#include "EAP_Parameter_CrossoverForSubwoofer.h"
#include "EAP_Parameter_Crossover2waySpeakers.h"

/************************************************************************************/
/*                                                                                  */
/*  Static data                                                                     */
/*                                                                                  */
/************************************************************************************/
/* FILE HANDLE */
/* FILE HANDLE */
char	modeInputFilename[] 	= "r+b";

#ifdef EAP_PARAM_ALL_EFFECT_OFF
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllEffectOff_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllEffectOff_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllEffectOff_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_VOICE_ENHANCER
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short_VoiceEnhancer.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_VoiceEnhancer_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_VoiceEnhancer_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_VoiceEnhancer_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_MUSIC_ENHANCER_RMSLIMITER
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_MusicEnhancerRMSLimiter_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_MusicEnhancerRMSLimiter_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_MusicEnhancerRMSLimiter_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_AUTO_VOLUME_LEVELER
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short_AutoVolumeLeveler.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AutoVolumeLeveler_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AutoVolumeLeveler_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AutoVolumeLeveler_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_CONCERTSOUND
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short_ConcertSound.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_ConcertSound_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_ConcertSound_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_ConcertSound_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_LOUDNESS_MAXIMISER
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short_LoudnessMaximiser.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_LoudnessMaximiser_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_LoudnessMaximiser_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_LoudnessMaximiser_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_CUSTOM
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Custom_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Custom_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Custom_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_TONE_GENERATOR
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_ToneGenerator_OUT.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_ToneGenerator_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_ToneGenerator_AVL.pcm";
#endif
#endif

#ifdef EAP_PARAM_TEST_ALL_ON
#define TWO_OUTPUT_FILES /*In case of Crossover Filtering we will use 2 output files*/
char	inputFilename[]			= "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
/* First output file for Low band and second for High Band*/
char	outputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllTestOn_OUT_LB.pcm";
char	outputFilename2[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllTestOn_OUT_HB.pcm";
char	PSAOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllTestOn_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[]		= "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_AllTestOn_AVL.pcm";
#endif
#endif
#ifdef EAP_PARAM_CROSSOVER_FOR_SUBWOOFER
#define TWO_OUTPUT_FILES /*In case of Crossover Filtering we will use 2 output files*/
char	inputFilename[] = "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
/* First output file for Low band and second for High Band*/
char	outputFilename[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_CrossoverForSubwoofer_LB1.pcm";
char	outputFilename2[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_CrossoverForSubwoofer_HB1.pcm";
char	PSAOutputFilename[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_CrossoverForSubwoofer_PSA1.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_CrossoverForSubwoofer_AVL.pcm";
#endif
#endif
#ifdef EAP_PARAM_CROSSOVER_2_WAY_SPEAKER
#define TWO_OUTPUT_FILES /*In case of Crossover Filtering we will use 2 output files*/
char	inputFilename[] = "..\\..\\In\\EAP_demo_48000_2ch_short.pcm";
/* First output file for Low band and second for High Band*/
char	outputFilename[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Crossover2waySpeakers_LB.pcm";
char	outputFilename2[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Crossover2waySpeakers_HB.pcm";
char	PSAOutputFilename[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Crossover2waySpeakers_PSA.dat";
#ifdef EXAMPLE_AVL_READ_GAIN
char	AVLOutputFilename[] = "..\\..\\Out_VISUAL_REF\\EAP_demo_48000_2ch_short_Crossover2waySpeakers_AVL.pcm";
#endif
#endif

char	modeOutputFilename[] 	= "w+b";
char	modePSAOutputFilename[] = "w+b";
#ifdef EXAMPLE_AVL_READ_GAIN
char	modeAVLOutputFilename[] = "w+b";
#endif

/************************************************************************************/
/*                                                                                  */
/*  Application code                                                                */
/*                                                                                  */
/************************************************************************************/

int main(void)
{
    /*
    * General control variables
    */
    /* Audio IN OUT File handle */
	void                    *AUDIO_INPUT_FILE;           					 						 /* File handles for I/O */
    LVM_INT16               IOBuffer[LVM_MAX_NUM_CHANNELS * MAX_INTERNAL_BLOCKSIZE + 1] = {0};   				 /* Read buffer for the samples */
    LVM_INT16   			*pReadBuffer 									  = &IOBuffer[0];
    void                    *AUDIO_OUTPUT_FILE;           					 						 /* File handles for I/O */
#ifdef TWO_OUTPUT_FILES
	void                    *AUDIO_OUTPUT_FILE2;           					 						 /* File handles for I/O when crossover enabled */
#endif // TWO_OUTPUT_FILES
    LVM_INT16               OutBuffer[LVM_MAX_NUM_CHANNELS * MAX_INTERNAL_BLOCKSIZE + 1]  	     = {0};        		     /* Output buffer for the samples, output is always stereo */
	LVM_INT16   			*pWriteBuffer= &OutBuffer[0];											 /* Output buffer for the samples, output is always stereo */	
#ifdef TWO_OUTPUT_FILES
	LVM_INT16               OutBuffer2[LVM_MAX_NUM_CHANNELS * MAX_INTERNAL_BLOCKSIZE + 1] = { 0 };
	LVM_INT16   			*pWriteBuffer2 = &OutBuffer2[0];
#endif // TWO_OUTPUT_FILES	
    LVM_INT16               InplaceProcessing_Flag 							 = 1;					 /* 1 for in place processing then OutBuffer can be removed */
    LVM_UINT16              SampleRate 					= 0;	 	/* sample rate in Hz */
    LVM_UINT16              InputNumChannels 			= 0;
    LVM_UINT16              ReadSamplesPerChannel 		= 0;        /* Number of samples to read from the file */
    LVM_UINT16              ReadSamplesTotal 			= 0;        /* Number of samples to read from the file */
    LVM_UINT16              WriteSamplesTotal 			= 0;		/* number of sample to write to the file */
    LVM_UINT16              ProcSamplesPerChannel    	= 0;		/* number of sample to process */
    LVM_UINT16              ReadFromFileSample  		= 0;        /* number of sample read from the file*/

    // malloc memory
    LVM_INT32               MallocAlign	= 4;                    	/* 4 byte Malloc alignment */
	LVM_INT32               MallocOffset = 5;                    	/* 4+1 byte Malloc offset  */
    // scratch
    LVM_INT16               *pScratchBase = LVM_NULL;
    LVM_UINT32              ScratchSize = 0;

    //EAP Lib
    LVM_Handle_t            EAP_hInstance;                          /* Instance handle */
	//LVM_ControlParams_t     EAP_ControlParams;                      /* control Parameters */
    LVM_ControlParams_t     *pEAP_ControlParams;                    /* control Parameters */
    LVM_InstParams_t        *pEAP_InstParams;                       /* EAP_InstParams */
    LVM_MemTab_t            EAP_MemTab;                             /* Memory allocation table */
    LVM_ReturnStatus_en     LVM_Status;                         	/* Function call status */
    LVM_VersionInfo_st      EAP_VersionInfo;                        /* Version info structure */
    LVM_INT16				EAP_clearAudioBuffer_flag = LVM_TRUE;	/* call clear audio buffer (LVM_ClearAudioBuffers) function after EAP set control parameter Yes or No
    																 * LVM_ClearAudioBuffers musn't be called if only algorithm parameter is updated
    																 * LVM_ClearAudioBuffers can be called when enabling or disabling algorithm
    																 * LVM_ClearAudioBuffers need to be call when disabling AVL algorithm
    																 */

    LVM_UINT32              EAP_AudioTime = LVM_FRAME_SIZE_MS; 		/* Audio Time of the current input data in milli-seconds */
    LVM_UINT16              EAP_eventPending_flag = LVM_TRUE;      	/* Event Pending flag  to highlight new EAP configuration is present */

    // others
    LVM_INT16               mainLoop = 1;
    LVM_UINT16              i;                                  	/* loop index */

	LVM_INT32               *temp32;


    

	LVM_INT32				tempScratchSize = 0;
    LVM_INT32               frameCount = 0;					        /* display the number of processed frame */

#ifdef ALGORITHM_EQNB
    LVM_HeadroomParams_t    	*pEAP_HeadroomParams;               /* Headroom parameters */
#endif
#ifdef ALGORITHM_PSA
    void                	*PSA_OUTPUT_FILE  							 = LVM_NULL;      	/* File handles for PSA Output */
    LVM_INT16               EAP_PSA_Bands  								 = 6;             	/* PSA number bands variable*/
    LVM_UINT32 				EAP_PSA_StartOffset 						 = 200;				/* start gap in ms to let the algorithm start before get PSA */
    LVM_UINT32              EAP_PSA_AudioTime1 							 = 0;             	/* Display time increment*/
    LVM_UINT32              EAP_PSA_DisplayTimeInc 						 = 60;            	/* Display time increment in ms, when "Delta EAP_AudioTime" > EAP_PSA_DisplayTimeInc then update spectrum value */
    LVM_INT8                EAP_PSA_CurrPeaks[LVM_PSA_MAX_NUMBANDS] 	 = {0};  			/* Current spectral peaks */
    LVM_INT8                EAP_PSA_PastPeaks[LVM_PSA_MAX_NUMBANDS] 	 = {0};  			/* Past Spectral Peaks */
    LVM_UINT16              EAP_PSA_BandFreq[LVM_PSA_MAX_NUMBANDS]  	 = {0};  			/* PSA spectral band center frequencies*/
#endif   /*ALGORITHM_PSA*/
#ifdef EXAMPLE_AVL_READ_GAIN
    void                	*AVL_OUTPUT_FILE  							 = LVM_NULL;      	/* File handles for AVL Output */
    LVM_INT32       		EAP_AVL_Gain_32;											    /* AVL read gain */
    LVM_INT16       		EAP_AVL_Gain_16;												/* AVL read gain */
#endif

	LVM_INT16 *pbufferTable[2];
	pbufferTable[0] = pWriteBuffer;
	pbufferTable[1] = LVM_NULL;
	LVM_INT16				**pToWriteBuffer = pbufferTable; /* Table of pointer for process */

    /* EAP test start */
	printf("\n**** EAP Test start ****  \n\n");

    /******************************************************************************
	GET VERSION INFORMATION
    *******************************************************************************/
    LVM_Status=LVM_GetVersionInfo(&EAP_VersionInfo);
    if(LVM_Status == LVM_NULLADDRESS)
    {
        printf("Error: NULL address is given to LVM_GetVersionInfo. ");
        return -1;
    }
    printf("Working with EAP library release %s built for platform %s\n",(char*)EAP_VersionInfo.pVersionNumber, (char*)EAP_VersionInfo.pPlatform );

    /******************************************************************************
    SET THE INPUT PARAMETERS
    *******************************************************************************/
    /*
     * Select parameter configuration
     */
    EAP_clearAudioBuffer_flag = LVM_TRUE;		/* call clear audio buffer function after EAP set control parameter */

	#ifdef EAP_PARAM_ALL_EFFECT_OFF
		pEAP_ControlParams 	= &ControlParamSet_allEffectOff;               /* Control Parameters */
		pEAP_InstParams 	= &InstParams_allEffectOff;       	            /* Instance parameters */
		#ifdef ALGORITHM_EQNB
			pEAP_HeadroomParams	= &HeadroomParams_allEffectOff;   		    /* Headroom parameters */
		#endif
	#endif

    #ifdef EAP_PARAM_VOICE_ENHANCER
		pEAP_ControlParams 	= &ControlParamSet_voiceEnhancer;               /* Control Parameters */
		pEAP_InstParams 	= &InstParams_voiceEnhancer;       	            /* Instance parameters */
		#ifdef ALGORITHM_EQNB
			pEAP_HeadroomParams	= &HeadroomParams_voiceEnhancer;   		    /* Headroom parameters */
		#endif
	#endif

	#ifdef EAP_PARAM_MUSIC_ENHANCER_RMSLIMITER
		pEAP_ControlParams 	= &ControlParamSet_musicEnhancerRmsLimiter;               /* Control Parameters */
		pEAP_InstParams 	= &InstParams_musicEnhancerRmsLimiter;       	            /* Instance parameters */
		#ifdef ALGORITHM_EQNB
			pEAP_HeadroomParams	= &HeadroomParams_musicEnhancerRmsLimiter;   		    /* Headroom parameters */
		#endif
	#endif

	#ifdef EAP_PARAM_LOUDNESS_MAXIMISER
		pEAP_ControlParams 	= &ControlParamSet_loudnessMaximiser;            /* Control Parameters */
		pEAP_InstParams 	= &InstParams_loudnessMaximiser;       	         /* Instance parameters */
		#ifdef ALGORITHM_EQNB
			pEAP_HeadroomParams	= &HeadroomParams_loudnessMaximiser;         /* Headroom parameters */
		#endif
	#endif

	#ifdef EAP_PARAM_CONCERTSOUND
		pEAP_ControlParams 	= &ControlParamSet_concertSound;                /* Control Parameters */
		pEAP_InstParams 	= &InstParams_concertSound;        	            /* Instance parameters */
		#ifdef ALGORITHM_EQNB
			pEAP_HeadroomParams	= &HeadroomParams_concertSound;   		    /* Headroom parameters */
		#endif
	#endif

	#ifdef EAP_PARAM_AUTO_VOLUME_LEVELER
		pEAP_ControlParams 	= &ControlParamSet_autoVolumeLeveler;            /* Control Parameters */
		pEAP_InstParams 	= &InstParams_autoVolumeLeveler;       	         /* Instance parameters */
		#ifdef ALGORITHM_EQNB
			pEAP_HeadroomParams	= &HeadroomParams_autoVolumeLeveler;   		 /* Headroom parameters */
		#endif
	#endif

#ifdef EAP_PARAM_CUSTOM
	pEAP_ControlParams 	= &ControlParamSet_custom;               /* Control Parameters */
	pEAP_InstParams 	= &InstParams_custom;       	            /* Instance parameters */
	#ifdef ALGORITHM_EQNB
		pEAP_HeadroomParams	= &HeadroomParams_custom;   		    /* Headroom parameters */
	#endif
#endif

#ifdef EAP_PARAM_TONE_GENERATOR
	pEAP_ControlParams 	= &ControlParamSet_toneGenerator;               /* Control Parameters */
	pEAP_InstParams 	= &InstParams_toneGenerator;       	            /* Instance parameters */
	#ifdef ALGORITHM_EQNB
		pEAP_HeadroomParams	= &HeadroomParams_toneGenerator;   		    /* Headroom parameters */
	#endif
#endif

#ifdef EAP_PARAM_TEST_ALL_ON
	pEAP_ControlParams 	= &ControlParamSet_testAllOn;               /* Control Parameters */
	pEAP_InstParams 	= &InstParams_testAllOn;       	            /* Instance parameters */
	#ifdef ALGORITHM_EQNB
		pEAP_HeadroomParams	= &HeadroomParams_testAllOn;   		    /* Headroom parameters */
	#endif
#endif
#ifdef EAP_PARAM_CROSSOVER_FOR_SUBWOOFER
		pEAP_ControlParams = &ControlParamSet_CrossoverForSubwoofer;               /* Control Parameters */
		pEAP_InstParams = &InstParams_CrossoverForSubwoofer;       	            /* Instance parameters */
#ifdef ALGORITHM_EQNB
		pEAP_HeadroomParams = &HeadroomParams_CrossoverForSubwoofer;   		    /* Headroom parameters */
#endif
#endif

#ifdef EAP_PARAM_CROSSOVER_2_WAY_SPEAKER
		pEAP_ControlParams = &ControlParamSet_Crossover2WaySpeaker;               /* Control Parameters */
		pEAP_InstParams = &InstParams_Crossover2WaySpeaker;       	            /* Instance parameters */
#ifdef ALGORITHM_EQNB
		pEAP_HeadroomParams = &HeadroomParams_Crossover2WaySpeaker;   		    /* Headroom parameters */
#endif
#endif
		
    /*
    * Open the input and output files
    */
    AUDIO_INPUT_FILE = fopen(inputFilename, modeInputFilename);
    if (AUDIO_INPUT_FILE == NULL)
    {
        printf("Cannot open file %s in %s mode",&inputFilename, &modeInputFilename);
        return -1;
    }

    AUDIO_OUTPUT_FILE = fopen(outputFilename, modeOutputFilename);
    if (AUDIO_OUTPUT_FILE == NULL)
    {
        printf("Cannot open file %s in %s mode",&outputFilename, &modeOutputFilename);
        return -1;
    }

#ifdef TWO_OUTPUT_FILES
	AUDIO_OUTPUT_FILE2 = fopen(outputFilename2, modeOutputFilename);
	if (AUDIO_OUTPUT_FILE2 == NULL)
	{
		printf("Cannot open file %s in %s mode", &outputFilename2, &modeOutputFilename);
		return -1;
	}
#endif // TWO_OUTPUT_FILES

#ifdef EXAMPLE_AVL_READ_GAIN
    AVL_OUTPUT_FILE  = fopen(AVLOutputFilename, modeAVLOutputFilename);
	if (AVL_OUTPUT_FILE == NULL)
	{
		printf("Cannot open file %s in %s mode",&AVLOutputFilename, &modeAVLOutputFilename);
		return -1;
	}
#endif

	/******************************************************************************
    Allocate memory
    Force alignment by allocating extra memory
    *******************************************************************************/
    LVM_Status = LVM_GetMemoryTable(	LVM_NULL,
    									&EAP_MemTab,
    									pEAP_InstParams);
    if (LVM_Status == LVM_NULLADDRESS)
    {
        printf("Memory table parameter error - null pointer\n");
        return -1;
    }
    if (LVM_Status == LVM_OUTOFRANGE)
    {
        printf("SetControlParameter error - out of range");
        return -1;
    }

    for (i=0; i<LVM_NR_MEMORY_REGIONS; i++)
    {
        /* Log the memory size */
        printf(" Memory region %d, size %d in Bytes\n",(LVM_INT16)i,EAP_MemTab.Region[i].Size);
        if (EAP_MemTab.Region[i].Size != 0)
        {
			temp32 = malloc(EAP_MemTab.Region[i].Size + tempScratchSize + MallocAlign+ MallocOffset);
            EAP_MemTab.Region[i].pBaseAddress = (LVM_INT8 *)(temp32 + MallocAlign + tempScratchSize);
            printf(" Memory region address %p\n",EAP_MemTab.Region[i].pBaseAddress);
        }
        if (i == LVM_MEMREGION_TEMPORARY_FAST)
        {
            pScratchBase = EAP_MemTab.Region[i].pBaseAddress;
            ScratchSize = EAP_MemTab.Region[i].Size;
        }
    }

    /******************************************************************************
    Initialize
    *******************************************************************************/
    /*
    * Get an EAP Instance
    */
    EAP_hInstance = LVM_NULL;                                       	/* Initialise to NULL */
    LVM_Status = LVM_GetInstanceHandle( &EAP_hInstance,              	/* Init sets the instance handle */
    									&EAP_MemTab,
    									pEAP_InstParams);
    if (LVM_Status == LVM_NULLADDRESS)
    {
        printf("Get instance handle parameter error - null pointer\n");
        return -1;
    }
    if (LVM_Status == LVM_OUTOFRANGE)
    {
        printf("Get instance handle parameter error - out of range\n");
        return -1;
    }
	if (LVM_Status == LVM_INVALIDNXPPLATFORM)
	{
		printf("Invalid Platform\n");
		return -1;
	}

#ifdef ALGORITHM_EQNB
    {
    	// set headroom param config
        LVM_Status = LVM_SetHeadroomParams(EAP_hInstance, pEAP_HeadroomParams);
    }
#endif

	/******************************************************************************
    Main loop
	   - update the parameters setting if required
	   - Process the data
    *******************************************************************************/
    do
    {
		/*if new configuration is pending
		 *  - Update the parameters setting
		 */
        if(EAP_eventPending_flag==LVM_TRUE)
        {
            // Clear pending flag
            EAP_eventPending_flag = LVM_FALSE;

   			/*
			 * Set example application parameter
			 */
			// sampling rate
			if (pEAP_ControlParams->SampleRate == LVM_FS_8000)	{SampleRate = 8000;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_11025)	{SampleRate = 11025;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_12000)	{SampleRate = 12000;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_16000)	{SampleRate = 16000;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_22050)	{SampleRate = 22050;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_24000)	{SampleRate = 24000;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_32000)	{SampleRate = 32000;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_44100)	{SampleRate = 44100;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			if (pEAP_ControlParams->SampleRate == LVM_FS_48000)	{SampleRate = 48000;} // 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000
			// Input data format
			if (pEAP_ControlParams->SourceFormat == LVM_MONO) 		 {InputNumChannels = 1;} // LVM_MONO or LVM_MONOINSTEREO or LVM_STEREO or LVM_5DOT1 or LVM_7DOT1
			if (pEAP_ControlParams->SourceFormat == LVM_MONOINSTEREO){InputNumChannels = 2;} // LVM_MONO or LVM_MONOINSTEREO or LVM_STEREO or LVM_5DOT1 or LVM_7DOT1
			if (pEAP_ControlParams->SourceFormat == LVM_STEREO) 	 {InputNumChannels = 2;} // LVM_MONO or LVM_MONOINSTEREO or LVM_STEREO or LVM_5DOT1 or LVM_7DOT1

		    
			// Set the buffer mode
			InplaceProcessing_Flag = 0; // 0 for OUTPLACE or 2 for INPLACE

			/*
			 * Power Spectrum Analysis
			 */
#ifdef ALGORITHM_PSA
		/* PSA output file */
			PSA_OUTPUT_FILE = fopen(PSAOutputFilename, modePSAOutputFilename);
		    if (PSA_OUTPUT_FILE == NULL)
		    {
		        printf("Cannot open file %s in %s mode",&PSAOutputFilename, &modePSAOutputFilename);
		        return -1;
		    }
			EAP_PSA_Bands =(LVM_INT16)pEAP_ControlParams->PSA_NumBands;
#endif /* ALGORITHM_PSA */


			/******************************************************************************
			Call set control parameters
				- propagate the configuration to EAP
			*******************************************************************************/
			LVM_Status = LVM_SetControlParameters(	EAP_hInstance,
													pEAP_ControlParams);


			if (LVM_Status == LVM_NULLADDRESS)
			{
				printf("SetControlParameter error - null pointer\n");
				return -1;
			}
			else if ((LVM_Status > LVM_OUTOFRANGE) && (LVM_Status < LVM_RETURNSTATUS_DUMMY))
			{
				printf("SetControlParameter error - out of range (See LVM.h error number: %d for details)\n",LVM_Status);
				return -1;
			}
			else if (LVM_Status != LVM_SUCCESS)
			{
				printf("SetControlParameter error (See LVM.h error number: %d for details)\n",LVM_Status);
				return -1;
			}

			/******************************************************************************
			Call clear audio buffer
				- clear the audio buffer
				- clear the history
    		 	- LVM_ClearAudioBuffers musn't be called if only algorithm parameter is updated
				- LVM_ClearAudioBuffers can be called when enabling or disabling algorithm
				- LVM_ClearAudioBuffers need to be call when disabling AVL algorithm
			*******************************************************************************/
			if (EAP_clearAudioBuffer_flag == LVM_TRUE)
			{
				LVM_Status = LVM_ClearAudioBuffers(EAP_hInstance);

				if (LVM_Status == LVM_NULLADDRESS)
				{
					printf("ClearAudioBuffer error - null pointer\n");
				}
			} /* end if EAP_clearAudioBuffer_flag == LVM_TRUE */
        } /* end if(EAP_eventPending_flag==LVM_TRUE) if new configuration to be set */


        /*
        * Select the required buffers
        */
        if (InplaceProcessing_Flag != 0)
        {
            pWriteBuffer = pReadBuffer;
        }
        else
        {
        	pWriteBuffer = &OutBuffer[0];
			
        }
		/* If Crossover is enable, we can adress the second output buffer (High band buffer)*/
#ifdef TWO_OUTPUT_FILES
		if (pEAP_ControlParams->XO_OperatingMode == LVM_MODE_ON)
			pToWriteBuffer[1] =pWriteBuffer2;
#endif /*TWO_OUTPUT_FILES*/
		/*  If LVM Operating mode is ON
		 *		- read the sample from file
		 *		- if enough sample to build a frame are present ( end of file are not reached )
		 *			- process the audio frame
		 *			- write the output sample to a file
		 *			- get the power spectrum if required
		 *			- write the power spectrum to a file
		 *			- update audio time
		 *		- else set main flag to exit the main loop
		 */
		/*
		* Compute number of sample for this frame
		*/
		if (LVM_FRAME_SIZE_MS == 0) // random read sample size up to maximal blocksize
		{
			ReadSamplesPerChannel = (LVM_INT16)(rand() % MAX_INTERNAL_BLOCKSIZE);
		} // end LVM_FRAME_SIZE_MS == 0
		else // compute the ReadSamplesPerChannel according LVM_FRAME_SIZE_MS
		{
			ReadSamplesPerChannel = (SampleRate * LVM_FRAME_SIZE_MS) / 1000;
		} // end else LVM_FRAME_SIZE_MS == 0

		if (ReadSamplesPerChannel > MAX_INTERNAL_BLOCKSIZE)
		{
			printf("Block size too large(Max = %d): %d\n", MAX_INTERNAL_BLOCKSIZE, ReadSamplesPerChannel);
			return -1;
		}
		ReadSamplesTotal = ReadSamplesPerChannel * InputNumChannels;        		/* Number of samples to read from the file */
		WriteSamplesTotal = ReadSamplesPerChannel * InputNumChannels;				/* number of sample to write to the file, always stereo */


		/*
		 *  Read sample from file
		 */
		ReadFromFileSample = (LVM_UINT16)fread( pReadBuffer , sizeof(LVM_INT16), ReadSamplesTotal, AUDIO_INPUT_FILE );
		
		/* If not enough sample to fill one full frame then exit without call LVM_Process */
		if((LVM_INT16)(ReadFromFileSample != ReadSamplesTotal))
		{
			mainLoop = 0; // exit the loop
		}
		else // else enough sample to call LVM_Process
		{
			EAP_AudioTime += LVM_FRAME_SIZE_MS;
			LVM_Status = LVM_Process(	EAP_hInstance,               /* Instance handle */
										pReadBuffer,                 /* Input buffer */
										pToWriteBuffer,              /* Output buffer */
										ReadSamplesPerChannel,       /* Number of samples to process per channel (Ex: If stereo the total amount of sample is 2 x ReadSamplesPerChannel)*/
										EAP_AudioTime);              /* Audio Time */
			/* Check for error and stop if needed */
			if (LVM_Status == LVM_NULLADDRESS)
			{
				printf("Process parameter error - null pointer\n");
				return -1;
			}
			if (LVM_Status == LVM_INVALIDNUMSAMPLES)
			{
				printf("Invalid number of samples, check value or range \n");
				return -1;
			}
			if (LVM_Status == LVM_ALIGNMENTERROR)
			{
				printf("Alignment error, check input and/or output buffer alignment \n");
				return -1;
			}
			if(LVM_Status != LVM_SUCCESS)
			{
				printf("Error while processing: %ld\n", (LVM_INT32)LVM_Status);
				return -1;
			}

			/* Write the audio output results */
			fwrite(pToWriteBuffer[0] , sizeof(LVM_INT16), (LVM_INT32)WriteSamplesTotal, AUDIO_OUTPUT_FILE );
#ifdef TWO_OUTPUT_FILES
			if (pEAP_ControlParams->XO_OperatingMode == LVM_MODE_ON)
				fwrite(pToWriteBuffer[1], sizeof(LVM_INT16), (LVM_INT32)WriteSamplesTotal, AUDIO_OUTPUT_FILE2);
#endif/*TWO_OUTPUT_FILES*/

			/*
			* Get the PSA (power spectrum audio) output results
			*/
#ifdef ALGORITHM_PSA
			if(EAP_AudioTime > (EAP_PSA_StartOffset + EAP_PSA_AudioTime1))
			{
				LVM_GetSpectrum(	EAP_hInstance,
									EAP_PSA_CurrPeaks,
									EAP_PSA_PastPeaks,
									EAP_PSA_BandFreq,
									EAP_PSA_AudioTime1);

				fwrite( &EAP_PSA_AudioTime1, sizeof(LVM_UINT32),             1,	 PSA_OUTPUT_FILE );
				fwrite( &EAP_PSA_Bands,      sizeof(LVM_INT8), 	             1,	 PSA_OUTPUT_FILE );
				fwrite( &EAP_PSA_BandFreq,   sizeof(LVM_UINT16), EAP_PSA_Bands,  PSA_OUTPUT_FILE );
				fwrite( &EAP_PSA_CurrPeaks,  sizeof(LVM_INT8),	 EAP_PSA_Bands,  PSA_OUTPUT_FILE );
				fwrite( &EAP_PSA_PastPeaks,  sizeof(LVM_INT8),	 EAP_PSA_Bands,  PSA_OUTPUT_FILE );

				EAP_PSA_AudioTime1+=(LVM_UINT32)EAP_PSA_DisplayTimeInc;
			}
#endif

#ifdef EXAMPLE_AVL_READ_GAIN
			/******************************************************************************
	    	  OPTIONAL
	    	  Read AVL gain
	    	  As an example:
	    	  	  - read AVL gain
	    	  	  - duplicate value for enabling comparison with output audio .pcm
	    	  	  - print it in a file to be displayed
			******************************************************************************/
			LVM_Status =  LVM_GetAVLGain( EAP_hInstance,
										  &EAP_AVL_Gain_32);

			/* Check for error and stop if needed */
			if (LVM_Status == LVM_NULLADDRESS)
			{
				printf("LVM_GetAVLGain Process parameter error - null pointer\n");
				return -1;
			}
			if(LVM_Status != LVM_SUCCESS)
			{
				printf("LVM_GetAVLGain Error while processing: %ld\n", (LVM_INT32)LVM_Status);
				return -1;
			}

			EAP_AVL_Gain_16 = EAP_AVL_Gain_32 >> 1;
			for (i=0;i<ReadSamplesPerChannel;i++) // duplicate to enable comparison with output audio .pcm
			{
				fwrite( &EAP_AVL_Gain_16, sizeof(LVM_INT16),1,	 AVL_OUTPUT_FILE );
			}

#endif

		} // end else enough sample to call LVM_Process

#ifdef EXAMPLE_PARAMETER_UPDATE
		/******************************************************************************
    	  OPTIONAL
    	  Update control and tuning parameters
    	  As an example:
    	      - After 1s of normal process
    	      - Read current EAP configuration
    	      - Update current EAP configuration
    	      - The new configuration will be set at next loop with LVM_SetControlParameters & LVM_SetCustomTuning function
    	        Then the new configuration will be apply at next LVM_Process function
    	******************************************************************************/
		if (EAP_AudioTime == EXAMPLE_PARAMETER_UPDATE_TIME)
    	{
    		EAP_eventPending_flag     = LVM_TRUE;
    		EAP_clearAudioBuffer_flag = LVM_FALSE;

    		/*
    	     * Read control parameter
    	     */
    		pEAP_ControlParams = &EAP_ControlParams;
    		LVM_Status = LVM_GetControlParameters(	EAP_hInstance,
    												pEAP_ControlParams);
			if (LVM_Status == LVM_NULLADDRESS)
			{
				printf("LVM_GetControlParameters error - null pointer\n");
				return -1;
			}
			if (LVM_Status == LVM_OUTOFRANGE)
			{
				printf("LVM_GetControlParameters error - out of range\n");
				return -1;
			}
    	    /*
    	     * Update control parameter
    	     * 		instance will be updated later as EAP_eventPending_flag = LVM_TRUE
    	     */
    		pEAP_ControlParams->LM_EffectLevel 		  = LVM_LM_GENTLE; 	 				// Loudness maximizer to LVM_LM_GENTLE

    	} // end set new configuration
#endif

#ifdef EXAMPLE_VOLUME_UPDATE_NO_SMOOTHING
		/******************************************************************************
    	  OPTIONAL
    	  Update volume with no smoothing
    	  As an example:
    	      - After 2s of normal process
    	      - Read current EAP configuration
    	      - Update volume
    	      - The new configuration will be apply at next LVM_Process function
    	******************************************************************************/
		if (EAP_AudioTime == EXAMPLE_VOLUME_UPDATE_NO_SMOOTHING_TIME)
    	{
    		/*
    	     * Read control parameter
    	     */
    		pEAP_ControlParams = &EAP_ControlParams;
    		LVM_Status = LVM_GetControlParameters(	EAP_hInstance,
    												pEAP_ControlParams);
			if (LVM_Status == LVM_NULLADDRESS)
			{
				printf("LVM_GetControlParameters error - null pointer\n");
				return -1;
			}
			if (LVM_Status == LVM_OUTOFRANGE)
			{
				printf("LVM_GetControlParameters error - out of range\n");
				return -1;
			}

			/*
			 * Update the volume
			 */
			pEAP_ControlParams->VC_EffectLevel = -30;
			LVM_Status = LVM_SetVolumeNoSmoothing( 	EAP_hInstance,
													pEAP_ControlParams);

			if (LVM_Status == LVM_NULLADDRESS)
			{
				printf("LVM_SetVolumeNoSmoothing error - null pointer\n");
				return -1;
			}
			if (LVM_Status == LVM_OUTOFRANGE)
			{
				printf("LVM_SetVolumeNoSmoothing error - out of range\n");
				return -1;
			}
			if(LVM_Status != LVM_SUCCESS)
			{
				printf("LVM_SetVolumeNoSmoothing Error while processing: %ld\n", (LVM_INT32)LVM_Status);
				return -1;
			}
    	}
#endif

    } // end main loop, no more frame to be processed
    while (mainLoop!=0);

    /*
     *  Close files
     */
    fclose(AUDIO_INPUT_FILE);
    fclose(AUDIO_OUTPUT_FILE);
	/* If XO enable, we can close the second output file, else (XO disable) we can delete it */
#ifdef TWO_OUTPUT_FILES
	fclose(AUDIO_OUTPUT_FILE2);
	if (pEAP_ControlParams->XO_OperatingMode == LVM_MODE_OFF)	
	 /* CROSSOVER disable, no needd of 2 output files. We can delete the second */
		remove(outputFilename2);
#endif // TWO_OUTPUT_FILES
#ifdef ALGORITHM_PSA
    fclose(PSA_OUTPUT_FILE);
#endif

    /*
    * Free memory
    */
    LVM_Status = LVM_GetMemoryTable(EAP_hInstance,
    								&EAP_MemTab,
    								LVM_NULL);

    for (i=0; i<LVM_NR_MEMORY_REGIONS; i++)
    {
        if (EAP_MemTab.Region[i].Size != 0)
        {
			temp32 = (LVM_INT32*)EAP_MemTab.Region[i].pBaseAddress - (LVM_UINT32)MallocAlign;
            free(temp32);
        }
    }

   	printf("\n**** EAP test finished successfully ****  \n\n");
    return 0;
}
