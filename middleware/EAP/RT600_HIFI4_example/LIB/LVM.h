/*
* Copyright 2020 NXP
*
* NXP Confidential. This software is owned or controlled by NXP and may only
* be used strictly in accordance with the applicable license terms found in
* file LICENSE.txt
*/


/****************************************************************************************/
/*                                                                                      */
/*  Header file for the application layer interface of Concert Sound, Bass Enhancement  */
/*  and volume management bundle.                                                       */
/*                                                                                      */
/*  This files includes all definitions, types, structures and function                 */
/*  prototypes required by the calling layer. All other types, structures and           */
/*  functions are private.                                                              */
/*                                                                                      */
/****************************************************************************************/
/*                                                                                      */
/*  Note: 1                                                                             */
/*  =======                                                                             */
/*  The algorithm can execute either with separate input and output buffers or with     */
/*  a common buffer, i.e. the data is processed in-place.                               */
/*                                                                                      */
/****************************************************************************************/
/*                                                                                      */
/*  Note: 2                                                                             */
/*  =======                                                                             */
/*  Three data formats are support Stereo,Mono-In-Stereo and Mono. The data is          */
/*  interleaved as follows:                                                             */
/*                                                                                      */
/*  Byte Offset         Stereo Input         Mono-In-Stereo Input       Mono Input      */
/*  ===========         ============         ====================     ==============    */
/*      0               Left Sample #1          Mono Sample #1        Mono Sample #1    */
/*      2               Right Sample #1         Mono Sample #1        Mono Sample #2    */
/*      4               Left Sample #2          Mono Sample #2        Mono Sample #3    */
/*      6               Right Sample #2         Mono Sample #2        Mono Sample #4    */
/*      .                      .                     .                     .            */
/*      .                      .                     .                     .            */
/*                                                                                      */
/****************************************************************************************/

#ifndef __LVM_H__
#define __LVM_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#ifdef  ALGORITHM_CS
#define ALGORITHM_VIRTUALIZER
#else
#ifdef  ALGORITHM_CI
#define ALGORITHM_VIRTUALIZER
#endif  /* ALGORITHM_CI */
#endif  /* ALGORITHM_CS */

#ifdef ALGORITHM_DBE
#define ALGORITHM_BASS
#else
#ifdef ALGORITHM_PB
#define ALGORITHM_BASS
#endif   /* ALGORITHM_PB */
#endif   /* ALGORITHM_DBE */


/****************************************************************************************/
/*                                                                                      */
/*  Includes                                                                            */
/*                                                                                      */
/****************************************************************************************/

#include "LVC_Types.h"


/****************************************************************************************/
/*                                                                                      */
/*  Definitions                                                                         */
/*                                                                                      */
/****************************************************************************************/

/* Memory table*/
#define LVM_NR_MEMORY_REGIONS                 4     /* Number of memory regions */

#ifdef ALGORITHM_VIRTUALIZER
/* Concert Sound effect level presets */
#define LVM_CS_EFFECT_NONE                    0     /* 0% effect, minimum value */
#define LVM_CS_EFFECT_LOW                 16384     /* 50% effect */
#define LVM_CS_EFFECT_MED                 24576     /* 75% effect */
#define LVM_CS_EFFECT_HIGH                32767     /* 100% effect, maximum value */

#endif /* ALGORITHM_VIRTUALIZER */
#ifdef ALGORITHM_TE
#define LVM_TE_LOW_MIPS                   32767     /* Treble enhancement 6dB Mips saving mode */

#endif /* ALGORITHM_TE */
#ifdef ALGORITHM_BASS
/* Bass enhancement effect level presets */
#define LVM_BE_0DB                            0     /* 0dB boost, no effect */
#define LVM_BE_3DB                            3     /* +3dB boost */
#define LVM_BE_6DB                            6     /* +6dB boost */
#define LVM_BE_9DB                            9     /* +9dB boost */
#define LVM_BE_12DB                          12     /* +12dB boost */
#define LVM_BE_15DB                          15     /* +15dB boost */

#endif /* ALGORITHM_BASS */
#define LVM_HEADROOM_MAX_NBANDS               5     /* Headroom management */

// EAP maximum value
#define LVM_MAX_BLK_SIZE				    480     /* maximum number of sample */
#define LVM_MAX_CHANNELS					  2     /* maximum number of channel */
#define MAX_EQNB_BANDS      				 16     /* EQ maximum number of band */
#define LVM_MAX_PSA_BANDS					 64     /* PSA maximum number of band */
#define LVM_FRAME_SIZE_MS					 10     /* size frame in ms, default is 10 */


/****************************************************************************************/
/*                                                                                      */
/*  Types                                                                               */
/*                                                                                      */
/****************************************************************************************/

/* Instance handle */
typedef void *LVM_Handle_t;


/* Status return values */
typedef enum
{
    LVM_SUCCESS            = 0,                     /* Successful return from a routine */
    LVM_ALIGNMENTERROR     = 1,                     /* Memory alignment error */
    LVM_NULLADDRESS        = 2,                     /* NULL allocation address */
    LVM_OUTOFRANGE         = 3,                     /* Out of range control parameter */
    LVM_INVALIDNUMSAMPLES  = 4,                     /* Invalid number of samples */
    LVM_WRONGAUDIOTIME     = 5,                     /* Wrong time value for audio time*/
    LVM_ALGORITHMDISABLED  = 6,                     /* Algorithm is disabled*/
    LVM_NOT_INITIALIZED    = 7,                   /* Process function was called for a non-initialized module */
    LVM_RETURNSTATUS_DUMMY = LVM_MAXENUM
} LVM_ReturnStatus_en;


/* Buffer Management mode */
typedef enum
{
    LVM_MANAGED_BUFFERS   = 0,
    LVM_UNMANAGED_BUFFERS = 1,
    LVM_BUFFERS_DUMMY     = LVM_MAXENUM
} LVM_BufferMode_en;



/* Output device type */
typedef enum
{
    LVM_HEADPHONES             = 0,
    LVM_MOBILE_SPEAKERS_SMALL  = 2,
    LVM_MOBILE_SPEAKERS_MEDIUM = 3,
    LVM_MOBILE_SPEAKERS_LARGE  = 4,
    LVM_SPEAKERTYPE_MAX        = LVM_MAXENUM
} LVM_OutputDeviceType_en;

#ifdef ALGORITHM_VIRTUALIZER
/* Virtualizer mode selection*/
typedef enum
{
    LVM_CONCERTSOUND       = 0,
#ifndef CINEMASOUND_STEREO_ONLY
    LVM_CINEMASOUND_MOVIE  = 1,
    LVM_CINEMASOUND_MUSIC  = 2,
#endif /* CINEMASOUND_STEREO_ONLY */
    LVM_CINEMASOUND_STEREO = 3,
    LVM_VIRTUALIZERTYPE_DUMMY   = LVM_MAXENUM
} LVM_VirtualizerType_en;
#endif  /* ALGORITHM_VIRTUALIZER */


#ifdef ALGORITHM_EQNB
/* N-Band Equaliser operating mode */
typedef enum
{
    LVM_EQNB_OFF   = 0,
    LVM_EQNB_ON    = 1,
    LVM_EQNB_DUMMY = LVM_MAXENUM
} LVM_EQNB_Mode_en;


#endif /* ALGORITHM_EQNB */
/* Filter mode control */
typedef enum
{
    LVM_EQNB_FILTER_OFF = 0,
    LVM_EQNB_FILTER_ON  = 1,
    LVM_EQNB_FILTER_DUMMY = LVM_MAXENUM
} LVM_EQNB_FilterMode_en;


#ifdef ALGORITHM_BASS
/* Bass Enhancement operating mode */
typedef enum
{
    LVM_BE_OFF   = 0,
    LVM_BE_ON    = 1,
    LVM_BE_DUMMY = LVM_MAXENUM
} LVM_BE_Mode_en;


/* Bass Enhancement centre frequency selection control */
typedef enum
{
    LVM_BE_CENTRE_55Hz  = 0,
    LVM_BE_CENTRE_66Hz  = 1,
    LVM_BE_CENTRE_78Hz  = 2,
    LVM_BE_CENTRE_90Hz  = 3,
    LVM_BE_CENTRE_DUMMY = LVM_MAXENUM
} LVM_BE_CentreFreq_en;


/* Bass Enhancement HPF selection control */
typedef enum
{
    LVM_BE_HPF_OFF   = 0,
    LVM_BE_HPF_ON    = 1,
    LVM_BE_HPF_DUMMY = LVM_MAXENUM
} LVM_BE_FilterSelect_en;


#endif /* ALGORITHM_BASS */


/* Volume Control operating mode */
typedef enum
{
    LVM_VC_OFF   = 0,
    LVM_VC_ON    = 1,
    LVM_VC_DUMMY = LVM_MAXENUM
} LVM_VC_Mode_en;


#ifdef ALGORITHM_TE
/* Treble Enhancement operating mode */
typedef enum
{
    LVM_TE_OFF   = 0,
    LVM_TE_ON    = 1,
    LVM_TE_DUMMY = LVM_MAXENUM
} LVM_TE_Mode_en;


#endif /* ALGORITHM_TE */
#ifdef ALGORITHM_LM
/* Loudness Maximiser operating mode */
typedef enum
{
    LVM_LM_OFF   = 0,
    LVM_LM_ON    = 1,
    LVM_LM_DUMMY = LVM_MAXENUM
}LVM_LM_Mode_en;


/* Loudness Maximiser effect setting */
typedef enum
{
    LVM_LM_GENTLE       = 0,
    LVM_LM_MEDIUM       = 1,
    LVM_LM_EXTREME      = 2,
    LVM_LM_EFFECT_DUMMY = LVM_MAXENUM
}LVM_LM_Effect_en;


#endif /* ALGORITHM_LM */
#ifdef ALGORITHM_GM
/* Gentle Mix operating mode */
typedef enum
{
    LVM_GM_OFF   = 0,
    LVM_GM_ON    = 1,
    LVM_GM_DUMMY = LVM_MAXENUM
} LVM_GM_Mode_en;


/* Gentle Mix effect setting */
typedef enum
{
    LVM_GM_FULL         = 0,
    LVM_GM_REDUCED      = 1,
    LVM_GM_EFFECT_DUMMY = LVM_MAXENUM
} LVM_GM_Effect_en;


#endif /* ALGORITHM_GM */
#ifdef ALGORITHM_RS
/* RichSound operating mode */
typedef enum
{
    LVM_RS_OFF   = 0,
    LVM_RS_ON    = 1,
    LVM_RS_DUMMY = LVM_MAXENUM
} LVM_RS_Mode_en;


/* RichSound configuration */
typedef enum
{
    LVM_RS_CONFIG_AVL_AEQ = 0,
    LVM_RS_CONFIG_AVL_FEQ = 1,
    LVM_RS_CONFIG_AVL     = 2,
    LVM_RS_CONFIG_FEQ     = 3,
    LVM_RS_CONFIG_DUMMY   = LVM_MAXENUM
}   LVM_RS_Config_en;


/* RichSound Auto Volume effect */
typedef enum
{
    LVM_RS_AVL_LOW    = 0,
    LVM_RS_AVL_MEDIUM = 1,
    LVM_RS_AVL_HIGH   = 2,
    LVM_RS_AVL_DUMMY  = LVM_MAXENUM
}   LVM_RS_AVLEffect_en;


#endif /* ALGORITHM_RS */
#ifdef ALGORITHM_AVL
/* AVL operating mode */
typedef enum
{
    LVM_AVL_OFF   = 0,
    LVM_AVL_ON    = 1,
    LVM_AVL_DUMMY = LVM_MAXENUM
} LVM_AVL_Mode_en;


#endif /* ALGORITHM_AVL */
/* Headroom management operating mode */
typedef enum
{
    LVM_HEADROOM_OFF   = 0,
    LVM_HEADROOM_ON    = 1,
    LVM_Headroom_DUMMY = LVM_MAXENUM
} LVM_Headroom_Mode_en;


#ifdef ALGORITHM_TG
/* Tone Generator operating mode */
typedef enum
{
    LVM_TG_OFF        = 0,
    LVM_TG_CONTINUOUS = 1,
    LVM_TG_ONESHOT    = 2,
    LVM_TG_DUMMY      = LVM_MAXENUM
} LVM_TG_Mode_en;


/* Tone Generator sweep mode */
typedef enum
{
    LVM_TG_SWEEPLIN    = 0,
    LVM_TG_SWEEPLOG    = 1,
    LVM_TG_SWEEP_DUMMY = LVM_MAXENUM
} LVM_TG_SweepMode_en;


#endif /* ALGORITHM_TG */
#ifdef ALGORITHM_PSA
typedef enum
{
    LVM_PSA_SPEED_SLOW,                                  /* Peak decaying at slow speed */
    LVM_PSA_SPEED_MEDIUM,                                /* Peak decaying at medium speed */
    LVM_PSA_SPEED_FAST,                                  /* Peak decaying at fast speed */
    LVM_PSA_SPEED_DUMMY = LVM_MAXENUM
} LVM_PSA_DecaySpeed_en;

typedef enum
{
    LVM_PSA_OFF   = 0,
    LVM_PSA_ON    = 1,
    LVM_PSA_DUMMY = LVM_MAXENUM
} LVM_PSA_Mode_en;

#endif /* ALGORITHM_PSA */


/****************************************************************************************/
/*                                                                                      */
/*  Structures                                                                          */
/*                                                                                      */
/****************************************************************************************/

/* Version information */
typedef struct
{
    LVM_CHAR                    *pVersionNumber;        /* Pointer to the version number in the format X.YY.ZZ */
    LVM_CHAR                    *pPlatform;             /* Pointer to the library platform type */
} LVM_VersionInfo_st;

/* Memory table containing the region definitions */
typedef struct
{
    LVM_MemoryRegion_st         Region[LVM_NR_MEMORY_REGIONS];  /* One definition for each region */
} LVM_MemTab_t;


/* N-Band equaliser band definition */
typedef struct
{
    LVM_INT16                   Gain;                   /* Band gain in dB */
    LVM_UINT16                  Frequency;              /* Band centre frequency in Hz */
    LVM_UINT16                  QFactor;                /* Band quality factor (x100) */
} LVM_EQNB_BandDef_t;


/* Headroom band definition */
typedef struct
{
    LVM_UINT16                  Limit_Low;              /* Low frequency limit of the band in Hertz */
    LVM_UINT16                  Limit_High;             /* High frequency limit of the band in Hertz */
    LVM_INT16                   Headroom_Offset;        /* Headroom = biggest band gain - Headroom_Offset */
} LVM_HeadroomBandDef_t;


/* Custom Tuning Parameters*/
typedef struct
{
    LVM_INT16                   MidGain;            /* MidChannelGain */
    LVM_UINT16                  MidCornerFreq;      /* Shelving Filter Corner Frequency */
    LVM_UINT16                  SideHighPassCutoff; /* SideBoost HighPassFilter Corner Frequency */
    LVM_UINT16                  SideLowPassCutoff;  /* SideBoost LowPassFilter Corner Frequency */
    LVM_INT16                   SideGain;           /* Side Channel Gain */

    LVM_INT16                   Elevation;          /* Virtual speaker elevation in degrees */
    LVM_INT16                   FrontAngle;         /* Front speaker angle */
    LVM_INT16                   SurroundAngle;      /* Surround speaker angle */

    LVM_EQNB_FilterMode_en      Tuning_HPF_Mode;    /* High pass filter */
    LVM_INT16                   Tuning_HPF_CornerFreq;
    LVM_INT16                   NumTuningBands;     /* Number of tuning equalization bands */
    LVM_EQNB_BandDef_t          *pTuningBandDefs;   /* Tuning equalization filter */
} LVM_Custom_TuningParams_st;


/* Control Parameter structure */
typedef struct
{
    /* General parameters */
    LVM_Mode_en                 OperatingMode;          /* Bundle operating mode On/Bypass */
    LVM_Fs_en                   SampleRate;             /* Sample rate */
    LVM_Format_en               SourceFormat;           /* Input data format */
    LVM_OutputDeviceType_en     SpeakerType;            /* Output device type */
    LVM_SpeakerType_en          SpeakerTypeInternal;    /* Device speaker type, mono or stereo */

#ifdef ALGORITHM_CS
    /* Concert Sound Virtualizer parameters*/
    LVM_Mode_en                 VirtualizerOperatingMode; /* Virtualizer operating mode On/Off */
    LVM_VirtualizerType_en      VirtualizerType;          /* Virtualizer type: ConcertSound, CinemaSound Music or CinemaSound Movie */
    LVM_UINT16                  VirtualizerReverbLevel;   /* Virtualizer reverb level in % */
    LVM_INT16                   CS_EffectLevel;           /* Concert Sound effect level */
#endif /* ALGORITHM_CS */

#ifdef ALGORITHM_CI
    /* Cinema Sound Virtualizer parameters*/
    LVM_Mode_en                 VirtualizerOperatingMode; /* Virtualizer operating mode On/Off */
    LVM_VirtualizerType_en      VirtualizerType;          /* Virtualizer type: ConcertSound, CinemaSound Music or CinemaSound Movie */
    LVM_UINT16                  VirtualizerReverbLevel;   /* Virtualizer reverb level in % */
    LVM_INT16                   CS_EffectLevel;           /* Concert Sound effect level */
#endif /* ALGORITHM_CI */

#ifdef ALGORITHM_EQNB
    /* N-Band Equaliser parameters */
    LVM_EQNB_Mode_en            EQNB_OperatingMode;     /* N-Band Equaliser operating mode */
    LVM_EQNB_FilterMode_en      EQNB_LPF_Mode;          /* Low pass filter */
    LVM_INT16                   EQNB_LPF_CornerFreq;
    LVM_EQNB_FilterMode_en      EQNB_HPF_Mode;          /* High pass filter */
    LVM_INT16                   EQNB_HPF_CornerFreq;
    LVM_UINT16                  EQNB_NBands;            /* Number of bands */
    LVM_EQNB_BandDef_t          *pEQNB_BandDefinition;  /* Pointer to equaliser definitions */

#endif /* ALGORITHM_EQNB */
#ifdef ALGORITHM_DBE
    /* Bass Enhancement parameters */
    LVM_BE_Mode_en              BE_OperatingMode;       /* Bass Enhancement operating mode */
    LVM_INT16                   BE_EffectLevel;         /* Bass Enhancement effect level */
    LVM_BE_CentreFreq_en        BE_CentreFreq;          /* Bass Enhancement centre frequency */
    LVM_BE_FilterSelect_en      BE_HPF;                 /* Bass Enhancement high pass filter selector */

#endif /* ALGORITHM_DBE */
#ifdef ALGORITHM_PB
    /* Bass Enhancement parameters */
    LVM_BE_Mode_en              BE_OperatingMode;       /* Bass Enhancement operating mode */
    LVM_INT16                   BE_EffectLevel;         /* Bass Enhancement effect level */
    LVM_BE_CentreFreq_en        BE_CentreFreq;          /* Bass Enhancement centre frequency */
    LVM_BE_FilterSelect_en      BE_HPF;                 /* Bass Enhancement high pass filter selector */

#endif /* ALGORITHM_PB */
    /* Volume Control parameters */
    LVM_INT16                   VC_EffectLevel;         /* Volume Control setting in dBs */
    LVM_INT16                   VC_Balance;             /* Left Right Balance control in dB (-96 to 96 dB), -ve values reduce */

#ifdef ALGORITHM_TE
    /* Treble Enhancement parameters */
    LVM_TE_Mode_en              TE_OperatingMode;       /* Treble Enhancement On/Off */
    LVM_INT16                   TE_EffectLevel;         /* Treble Enhancement gain dBs */

#endif /* ALGORITHM_TE */
#ifdef ALGORITHM_LM
    /* Loudness Maximiser parameters */
    LVM_LM_Mode_en              LM_OperatingMode;       /* Loudness Maximiser operating mode */
    LVM_LM_Effect_en            LM_EffectLevel;         /* Loudness Maximiser effect level */
    LVM_UINT16                  LM_Attenuation;         /* Loudness Maximiser output attenuation */
    LVM_UINT16                  LM_CompressorGain;      /* Loudness Maximiser output compressor gain */
    LVM_UINT16                  LM_SpeakerCutOff;       /* Loudness Maximiser speaker cut off frequency */

#endif /* ALGORITHM_LM */
#ifdef ALGORITHM_GM
    /* Gentle Mix parameters */
    LVM_GM_Mode_en              GM_OperatingMode;       /* Gentle mix operating mode */
    LVM_GM_Effect_en            GM_EffectLevel;         /* Gentle mix effect level */

#endif /* ALGORITHM_GM */
#ifdef ALGORITHM_RS
    /* Richsound parameters */
    LVM_RS_Mode_en              RS_OperatingMode;       /* Richsound operating mode */
    LVM_RS_Config_en            RS_Config;              /* Richsound configuration */
    LVM_RS_AVLEffect_en         RS_AVLEffect;           /* Richsound auto volume effect */
    LVM_INT16                   RS_EQBandLow;           /* Richsound low frequency band setting */
    LVM_INT16                   RS_EQBandMid;           /* Richsound middle frequency band setting */
    LVM_INT16                   RS_EQBandHigh;          /* Richsound high frequency band setting */

#endif /* ALGORITHM_RS */
#ifdef ALGORITHM_AVL
    /* AVL parameters */
    LVM_AVL_Mode_en             AVL_OperatingMode;      /* AVL operating mode */

#endif /* ALGORITHM_AVL */
#ifdef ALGORITHM_TG
    /* Tone Generator parameters */
    LVM_TG_Mode_en              TG_OperatingMode;       /* Tone generator mode */
    LVM_TG_SweepMode_en         TG_SweepMode;           /* Log or linear sweep */
    LVM_UINT16                  TG_StartFrequency;      /* Sweep start frequency in Hz */
    LVM_INT16                   TG_StartAmplitude;      /* Sweep start amplitude in dBr */
    LVM_UINT16                  TG_StopFrequency;       /* Sweep stop frequency in Hz */
    LVM_INT16                   TG_StopAmplitude;       /* Sweep stop amplitude in dBr */
    LVM_UINT16                  TG_SweepDuration;       /* Sweep duration in seconds, 0 for infinite duration tone */
    LVM_Callback                pTG_CallBack;           /* End of sweep callback */
    LVM_INT16                   TG_CallBackID;          /* Callback ID*/
    void                        *pTGAppMemSpace;        /* Application instance handle or memory area */
#endif /* ALGORITHM_TG */

#ifdef ALGORITHM_PSA
    /* General Control */
    LVM_PSA_Mode_en             PSA_Enable;

    /* Spectrum Analyzer parameters */
    LVM_PSA_DecaySpeed_en       PSA_PeakDecayRate;      /* Peak value decay rate*/
    LVM_UINT16                  PSA_NumBands;           /* Number of Bands*/

#endif /* ALGORITHM_PSA */

} LVM_ControlParams_t;


/* Instance Parameter structure */
typedef struct
{
    /* General */
    LVM_BufferMode_en           BufferMode;             /* Buffer management mode */
    LVM_UINT16                  MaxBlockSize;           /* Maximum processing block size */

    /* N-Band Equaliser */
    LVM_UINT16                  EQNB_NumBands;          /* Maximum number of equaliser bands */

#ifdef ALGORITHM_PSA
    /* PSA */
    LVM_UINT16                  PSA_HistorySize;         /* PSA History size in ms: 200 to 5000 */
    LVM_UINT16                  PSA_MaxBands;            /* Maximum number of bands: 6 to 64 */
    LVM_UINT16                  PSA_SpectrumUpdateRate;  /* Spectrum update rate : 10 to 25*/
    LVM_PSA_Mode_en             PSA_Included;            /* Controls the instance memory allocation for PSA: ON/OFF */
#endif /* ALGORITHM_PSA */
} LVM_InstParams_t;


/* Headroom management parameter structure */
typedef struct
{
    LVM_Headroom_Mode_en        Headroom_OperatingMode; /* Headroom Control On/Off */
    LVM_HeadroomBandDef_t       *pHeadroomDefinition;   /* Pointer to headroom bands definition */
    LVM_UINT16                  NHeadroomBands;         /* Number of headroom bands */
} LVM_HeadroomParams_t;


/****************************************************************************************/
/*                                                                                      */
/*  Function Prototypes                                                                 */
/*                                                                                      */
/****************************************************************************************/


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetVersionInfo                                          */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to retrieve information about the library's version.          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  pVersion                Pointer to an empty version info structure                  */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         when pVersion is NULL                                       */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetVersionInfo(LVM_VersionInfo_st  *pVersion);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetMemoryTable                                          */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used for memory allocation and free. It can be called in           */
/*  two ways:                                                                           */
/*                                                                                      */
/*      hInstance = NULL                Returns the memory requirements                 */
/*      hInstance = Instance handle     Returns the memory requirements and             */
/*                                      allocated base addresses for the instance       */
/*                                                                                      */
/*  When this function is called for memory allocation (hInstance=NULL) the memory      */
/*  base address pointers are NULL on return.                                           */
/*                                                                                      */
/*  When the function is called for free (hInstance = Instance Handle) the memory       */
/*  table returns the allocated memory and base addresses used during initialisation.   */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pMemoryTable            Pointer to an empty memory definition table                 */
/*  pInstParams             Pointer to the instance parameters                          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When one of pMemoryTable or pInstParams is NULL             */
/*  LVM_OUTOFRANGE          When any of the Instance parameters are out of range        */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetMemoryTable(LVM_Handle_t         hInstance,
                                       LVM_MemTab_t         *pMemoryTable,
                                       LVM_InstParams_t     *pInstParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetInstanceHandle                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to create a bundle instance. It returns the created instance  */
/*  handle through phInstance. All parameters are set to their default, inactive state. */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  phInstance              pointer to the instance handle                              */
/*  pMemoryTable            Pointer to the memory definition table                      */
/*  pInstParams             Pointer to the instance parameters                          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Initialisation succeeded                                    */
/*  LVM_ALIGNMENTERROR      Instance or scratch memory on incorrect alignment           */
/*  LVM_NULLADDRESS         Instance or scratch memory has a NULL pointer               */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. This function must not be interrupted by the LVM_Process function                */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetInstanceHandle(LVM_Handle_t        *phInstance,
                                          LVM_MemTab_t        *pMemoryTable,
                                          LVM_InstParams_t    *pInstParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_ClearAudioBuffers                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to clear the internal audio buffers of the bundle.            */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Buffers Cleared                                             */
/*  LVM_NULLADDRESS         Instance is NULL                                            */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. This function may be interrupted by the LVM_Process function                */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_ClearAudioBuffers(LVM_Handle_t  hInstance);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                 LVM_GetControlParameters                                   */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Request the LifeVibes module parameters. The current parameter set is returned      */
/*  via the parameter pointer.                                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance                Instance handle                                            */
/*  pParams                  Pointer to an empty parameter structure                    */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS          Succeeded                                                      */
/*  LVM_NULLADDRESS      when any of hInstance or pParams is NULL                       */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetControlParameters(LVM_Handle_t           hInstance,
                                             LVM_ControlParams_t    *pParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_SetControlParameters                                    */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Sets or changes the LifeVibes module parameters.                                    */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pParams                 Pointer to a parameter structure                            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When hInstance, pParams or any control pointers are NULL    */
/*  LVM_OUTOFRANGE          When any of the control parameters are out of range         */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_SetControlParameters(LVM_Handle_t           hInstance,
                                             LVM_ControlParams_t    *pParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_Process                                                 */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Process function for the LifeVibes module.                                          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance handle                                             */
/*  pInData                 Pointer to the input data                                   */
/*  pOutData                Pointer to the output data                                  */
/*  NumSamples              Number of samples to process per channel                    */
/*                         (Ex: If stereo the total amount of sample is 2 x NumSamples) */
/*  AudioTime               Audio Time of the current input data in milli-seconds       */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS            Succeeded                                                    */
/*  LVM_INVALIDNUMSAMPLES  When the NumSamples is not a valied multiple in unmanaged    */
/*                         buffer mode                                                  */
/*  LVM_ALIGNMENTERROR     When either the input our output buffers are not 32-bit      */
/*                         aligned in unmanaged mode                                    */
/*  LVM_NULLADDRESS        When one of hInstance, pInData or pOutData is NULL           */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. The input and output buffers must be 32-bit aligned                              */
/*  2. Number of samples is defined as follows:                                         */
/*      MONO                the number of samples in the block                          */
/*      MONOINSTEREO        the number of sample pairs in the block                     */
/*      STEREO              the number of sample pairs in the block                     */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_Process(LVM_Handle_t                hInstance,
                                const LVM_INT16             *pInData,
                                LVM_INT16                   *pOutData,
                                LVM_UINT16                  NumSamples,
                                LVM_UINT32                  AudioTime);


#ifdef ALGORITHM_RS
/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetRSGains                                              */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/* This function is used to retrieve the Richsound last generated gains,                */
/* in Q16.15 linear values.                                                             */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pRS_Gains               Pointer to an array of seven gains                          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When hInstance or pRS_Gains are null addresses              */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. This function may be interrupted by the LVM_Process function                     */
/*  2. Seven gains are available.                                                       */
/*  Those are (in order of appearance in the array pointed by pRS_Gains parameter)      */
/*                Total gain on low band,                                               */
/*                Total gain on mid band,                                               */
/*                Total gain on high band,                                              */
/*                AEQ output gain on low band,                                          */
/*                AEQ output gain on mid band,                                          */
/*                AEQ output gain on high band,                                         */
/*                AVL output gain.                                                      */
/* Total gain is the mix of the AEQ output gain on one band and the AVL output gain.    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetRSGains( LVM_Handle_t            hInstance,
                                    LVM_INT32               *pRS_Gains);


#endif /* ALGORITHM_RS */
#ifdef ALGORITHM_AVL
/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetAVLGain                                              */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to retrieve the AVL last generated gain in Q16.15             */
/*  linear values.                                                                      */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pAVL_Gain               Pointer to the gain                                         */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When hInstance or pAVL_Gain are null addresses              */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetAVLGain( LVM_Handle_t    hInstance,
                                    LVM_INT32       *pAVL_Gain);


#endif /* ALGORITHM_AVL */
#ifdef ALGORITHM_EQNB
/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_SetHeadroomParams                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to set the automatic headroom management parameters.         */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pHeadroomParams         Pointer to headroom parameter structure                     */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_SetHeadroomParams(  LVM_Handle_t            hInstance,
                                            LVM_HeadroomParams_t    *pHeadroomParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetHeadroomParams                                       */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to get the automatic headroom management parameters.          */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pHeadroomParams         Pointer to headroom parameter structure (output)            */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When hInstance or pHeadroomParams are NULL                  */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1.  This function may be interrupted by the LVM_Process function                    */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetHeadroomParams(  LVM_Handle_t            hInstance,
                                            LVM_HeadroomParams_t    *pHeadroomParams);


#endif /* ALGORITHM_EQNB */

#ifdef ALGORITHM_PSA
/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_GetSpectrum                                             */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/* This function is used to retrieve Spectral information at a given Audio time         */
/* for display usage                                                                    */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pCurrentPeaks           Pointer to location where currents peaks are to be saved    */
/*  pPastPeaks              Pointer to location where past peaks are to be saved        */
/*  pCentreFreqs            Pointer to location where centre frequency of each band is  */
/*                          to be saved                                                 */
/*  AudioTime               Audio time at which the spectral information is needed      */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         If any of input addresses are NULL                          */
/*  LVM_WRONGAUDIOTIME      Failure due to audio time error                             */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. This function may be interrupted by the LVM_Process function                     */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetSpectrum( LVM_Handle_t            hInstance,
                                     LVM_INT8                *pCurrentPeaks,
                                     LVM_INT8                *pPastPeaks,
                                     LVM_UINT16              *pCentreFreqs,
                                     LVM_UINT32              AudioTime);


#endif /* ALGORITHM_PSA */


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                LVM_SetVolumeNoSmoothing                                    */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/* This function is used to set output volume without any smoothing                     */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance               Instance Handle                                             */
/*  pParams                 Control Parameters, only volume value is used here          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         If any of input addresses are NULL                          */
/*  LVM_OUTOFRANGE          When any of the control parameters are out of range         */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. This function may be interrupted by the LVM_Process function                     */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_SetVolumeNoSmoothing( LVM_Handle_t           hInstance,
                                              LVM_ControlParams_t    *pParams);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                 LVM_SetCustomTuning                                        */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to set custom tuning parameters for the sound enhancements    */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance                Instance Handle                                            */
/*  pCustom_Tuning           Pointer to the custom tuning parameters structure          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When hInstance or pCustom_Tuning are null addresses         */
/*  LVM_OUTOFRANGE          When one or more parameter is out of range                  */
/*                                                                                      */
/* NOTES:                                                                               */
/*    1.    This function may be interrupted by the LVM_Process function                */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_SetCustomTuning(LVM_Handle_t                   hInstance,
                                        LVM_Custom_TuningParams_st    *pCustom_Tuning);


/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                 LVM_GetCustomTuning                                        */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to retrieve custom tuning parameters from the sound           */
/*  enhancements.                                                                       */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  hInstance                Instance Handle                                            */
/*  pCustom_Tuning           Pointer to the custom tuning parameters structure          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  LVM_SUCCESS             Succeeded                                                   */
/*  LVM_NULLADDRESS         When hInstance or pCustom_Tuning are null addresses         */
/*                                                                                      */
/* NOTES:                                                                               */
/*    1.    This function may be interrupted by the LVM_Process function                */
/*                                                                                      */
/****************************************************************************************/
#ifdef __DLL_EXPORT
__declspec(dllexport)
#endif /* __DLL_EXPORT */
LVM_ReturnStatus_en LVM_GetCustomTuning(LVM_Handle_t                   hInstance,
                                        LVM_Custom_TuningParams_st    *pCustom_Tuning);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif      /* __LVM_H__ */

