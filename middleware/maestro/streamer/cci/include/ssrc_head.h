/****************************************************************************************/
/*  Copyright (c) 2004-2018 NXP Software. All rights are reserved.                      */
/*  Reproduction in whole or in part is prohibited without the prior                    */
/*  written consent of the copyright owner.                                             */
/*                                                                                      */
/*  This software and any compilation or derivative thereof is and                      */
/*  shall remain the proprietary information of NXP Software and is                     */
/*  highly confidential in nature. Any and all use hereof is restricted                 */
/*  and is subject to the terms and conditions set forth in the                         */
/*  software license agreement concluded with NXP Software.                             */
/*                                                                                      */
/*  Under no circumstances is this software or any derivative thereof                   */
/*  to be combined with any Open Source Software in any way or                          */
/*  licensed under any Open License Terms without the express prior                     */
/*  written  permission of NXP Software.                                                */
/*                                                                                      */
/*  For the purpose of this clause, the term Open Source Software means                 */
/*  any software that is licensed under Open License Terms. Open                        */
/*  License Terms means terms in any license that require as a                          */
/*  condition of use, modification and/or distribution of a work                        */
/*                                                                                      */
/*           1. the making available of source code or other materials                  */
/*              preferred for modification, or                                          */
/*                                                                                      */
/*           2. the granting of permission for creating derivative                      */
/*              works, or                                                               */
/*                                                                                      */
/*           3. the reproduction of certain notices or license terms                    */
/*              in derivative works or accompanying documentation, or                   */
/*                                                                                      */
/*           4. the granting of a royalty-free license to any party                     */
/*              under Intellectual Property Rights                                      */
/*                                                                                      */
/*  regarding the work and/or any work that contains, is combined with,                 */
/*  requires or otherwise is based on the work.                                         */
/*                                                                                      */
/*  This software is provided for ease of recompilation only.                           */
/*  Modification and reverse engineering of this software are strictly                  */
/*  prohibited.                                                                         */
/*                                                                                      */
/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/*     $Author: nxp97292 $*/
/*     $Revision: 113138 $*/
/*     $Date: 2018-01-24 10:34:58 +0100 (Wed, 24 Jan 2018) $*/
/*                                                                                      */
/****************************************************************************************/

/*
    The input and output blocks of the SRC are by default blocks of 40 ms.  This means that
    the following default block sizes are used:

          Fs     Default Block size
        -----        ----------
         8000           320
        11025           441
        12000           480
        16000           640
        22050           882
        24000           960
        32000          1280
        44100          1764
        48000          1920

    An API is provided to change the default block size into any multiple of the minimal
    block size.

    All the sampling rates above are supported as input and as output sampling rate
*/

#ifndef SSRC_HEAD_H
#define SSRC_HEAD_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/****************************************************************************************
   INCLUDES
*****************************************************************************************/

#include "LVC_Types.h"

/****************************************************************************************
   DEFINITIONS
*****************************************************************************************/
// Enable tone protection in demo, but disable in Super wideband build
#if (defined(__DEMO_MODE) && (!defined(ALGORITHM_SUPERWIDEBAND)))

#ifndef DOUBLE_PRECISION
#define SSRC_INSTANCE_SIZE 580 /* Size of struct with one 16bit word padding */
#else                          // DOUBLE_PRECISION
#define SSRC_INSTANCE_SIZE 664 /* Size of struct for DOUBLE_PRECISION with padding */
#endif                         // DOUBLE_PRECISION

#else // __DEMO_MODE

#ifndef DOUBLE_PRECISION
#define SSRC_INSTANCE_SIZE 560 /* Size of struct with one 16bit word padding */
#else                          // DOUBLE_PRECISION
#define SSRC_INSTANCE_SIZE 652 /* Size of struct for DOUBLE_PRECISION with padding */
#endif                         // DOUBLE_PRECISION

#endif // __DEMO_MODE
#define SSRC_INSTANCE_ALIGNMENT 4
#define SSRC_SCRATCH_ALIGNMENT  4

/****************************************************************************************
   TYPE DEFINITIONS
*****************************************************************************************/

/* Status return values */
typedef enum
{
    SSRC_OK                  = 0, /* Successful return from a routine */
    SSRC_INVALID_FS          = 1, /* The input or the output sampling rate is invalid */
    SSRC_INVALID_NR_CHANNELS = 2, /* The number of channels is not equal to mono or stereo */
    SSRC_NULL_POINTER        = 3, /* One of the input pointers is NULL */
    SSRC_WRONG_NR_SAMPLES    = 4, /* Invalid number of samples */
    SSRC_ALLINGMENT_ERROR    = 5, /* The instance memory or the scratch memory is not alligned */
    SSRC_INVALID_MODE        = 6, /* A wrong value has been used for the mode parameter */
    SSRC_INVALID_VALUE       = 7, /* An invalid (out of range) value has been used for one of the parameters */
    LVXXX_RETURNSTATUS_DUMMY = LVM_MAXENUM
} SSRC_ReturnStatus_en;

/* Instance memory */
typedef struct
{
    LVM_INT32 Storage[SSRC_INSTANCE_SIZE / 4];
} SSRC_Instance_t;

/* Scratch memory */
typedef LVM_INT32 SSRC_Scratch_t;

/* Nuber of samples mode */
typedef enum
{
    SSRC_NR_SAMPLES_DEFAULT = 0,
    SSRC_NR_SAMPLES_MIN     = 1,
    SSRC_NR_SAMPLES_DUMMY   = LVM_MAXENUM
} SSRC_NR_SAMPLES_MODE_en;

/* Quality level */
typedef enum
{
    SSRC_QUALITY_HIGH      = 0,
    SSRC_QUALITY_VERY_HIGH = 1,
    SSRC_QUALITY_DUMMY     = LVM_MAXENUM
} SSRC_Quality_en;

/* Instance parameters */
typedef struct
{
    SSRC_Quality_en Quality;
    LVM_Fs_en SSRC_Fs_In;
    LVM_Fs_en SSRC_Fs_Out;
    LVM_Format_en SSRC_NrOfChannels;
    LVM_INT16 NrSamplesIn;
    LVM_INT16 NrSamplesOut;
} SSRC_Params_t;

/****************************************************************************************
   FUNCTION PROTOTYPES
*****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                SSRC_GetNrSamples                                           */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function retrieves the number of samples (or sample pairs for stereo) to be    */
/*  used as input and as output of the SSRC module.                                     */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  Mode                    There are two modes:                                        */
/*                              - SSRC_NR_SAMPELS_DEFAULT.  In this mode, the function  */
/*                                will return the number of samples for 40 ms blocks    */
/*                              - SSRC_NR_SAMPELS_MIN will return the minimal number    */
/*                                of samples that is supported for this conversion      */
/*                                ratio.  Each integer multiple of this ratio will      */
/*                                be accepted by the SSRC_Init function                 */
/*                                                                                      */
/*  pSSRC_Params            pointer to the instance parameters                          */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  SSRC_OK                 Succeeded                                                   */
/*  SSRC_INVALID_FS         When the requested input or output sampling rates           */
/*                          are invalid.                                                */
/*  SSRC_INVALID_NR_CHANNELS When the channel format is not equal to LVM_MONO           */
/*                          or LVM_STEREO                                               */
/*  SSRC_NULL_POINTER       When pSSRC_Params is a NULL pointer                         */
/*  SSRC_INVALID_MODE       When Mode is not a valid setting                            */
/*                                                                                      */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/

SSRC_ReturnStatus_en SSRC_GetNrSamples(SSRC_NR_SAMPLES_MODE_en Mode, SSRC_Params_t *pSSRC_Params);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                SSRC_GetScratchSize                                         */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function retrieves the scratch size for a given conversion ratio and           */
/*  for given buffer sizes at the input and at the output                               */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  pSSRC_Params            pointer to the instance parameters                          */
/*  pScratchSize            pointer to the scratch size.  The SSRC_GetScratchSize       */
/*                          function will fill in the correct value (in bytes).         */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  SSRC_OK                 when the function call succeeds                             */
/*  SSRC_INVALID_FS         When the requested input or output sampling rates           */
/*                          are invalid.                                                */
/*  SSRC_INVALID_NR_CHANNELS When the channel format is not equal to LVM_MONO           */
/*                          or LVM_STEREO                                               */
/*  SSRC_NULL_POINTER       When any of the input pointers is a NULL pointer            */
/*  SSRC_WRONG_NR_SAMPLES   When the number of samples on the input or on the output    */
/*                          are incorrect                                               */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/

SSRC_ReturnStatus_en SSRC_GetScratchSize(SSRC_Params_t *pSSRC_Params, LVM_INT32 *pScratchSize);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                SSRC_Init                                                   */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function is used to initialize the SSRC module instance.                       */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  pSSRC_Instance          Instance pointer                                            */
/*                                                                                      */
/*  pSSRC_Scratch           pointer to the scratch memory                               */
/*  pSSRC_Params            pointer to the instance parameters                          */
/*  pInputInScratch,        pointer to a location in the scratch memory that can be     */
/*                          used to store the input samples (e.g. to save memory)       */
/*  pOutputInScratch        pointer to a location in the scratch memory that can be     */
/*                          used to store the output samples (e.g. to save memory)      */
/*                                                                                      */
/* RETURNS:                                                                             */
/*  SSRC_OK                 Succeeded                                                   */
/*  SSRC_INVALID_FS         When the requested input or output sampling rates           */
/*                          are invalid.                                                */
/*  SSRC_INVALID_NR_CHANNELS When the channel format is not equal to LVM_MONO           */
/*                          or LVM_STEREO                                               */
/*  SSRC_WRONG_NR_SAMPLES   When the number of samples on the input or the output       */
/*                          are incorrect                                               */
/*  SSRC_NULL_POINTER       When any of the input pointers is a NULL pointer            */
/*  SSRC_ALLINGMENT_ERROR   When the instance memory or the scratch memory is not       */
/*                          4 bytes alligned                                            */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. The init function will clear the internal state                                  */
/*                                                                                      */
/****************************************************************************************/

SSRC_ReturnStatus_en SSRC_Init(SSRC_Instance_t *pSSRC_Instance,
                               SSRC_Scratch_t *pSSRC_Scratch,
                               SSRC_Params_t *pSSRC_Params,
                               LVM_INT16 **ppInputInScratch,
                               LVM_INT16 **ppOutputInScratch);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                SSRC_SetGains                                               */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  This function sets headroom gain and the post gain of the SSRC                      */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  bHeadroomGainEnabled    parameter to enable or disable the headroom gain of the     */
/*                          SSRC.  The default value is LVM_MODE_ON.  LVM_MODE_OFF      */
/*                          can be used in case it can be guaranteed that the input     */
/*                          level is below -6dB in all cases (the default headroom      */
/*                          is -6 dB)                                                   */
/*                                                                                      */
/*  bOutputGainEnabled      parameter to enable or disable the output gain.  The        */
/*                          default value is LVM_MODE_ON                                */
/*                                                                                      */
/*  OutputGain              the value of the output gain.  The output gain is a linear  */
/*                          gain value. 0x7FFF is equal to +6 dB and 0x0000 corresponds */
/*                          to -inf dB.  By default, a 3dB gain is applied, resulting   */
/*                          in an overall gain of -3dB (-6dB headroom + 3dB output gain)*/
/*                                                                                      */
/* RETURNS:                                                                             */
/*  SSRC_OK                 Succeeded                                                   */
/*  SSRC_NULL_POINTER       When pSSRC_Instance is a NULL pointer                       */
/*  SSRC_INVALID_MODE       Wrong value used for the bHeadroomGainEnabled or the        */
/*                          bOutputGainEnabled parameters.                              */
/*  SSRC_INVALID_VALUE      When OutputGain is out to the range [0;32767]               */
/*                                                                                      */
/* NOTES:                                                                               */
/*  1. The SSRC_SetGains function is an optional function that should only be used      */
/*     in rare cases.  Preferably, use the default settings.                            */
/*                                                                                      */
/****************************************************************************************/

SSRC_ReturnStatus_en SSRC_SetGains(SSRC_Instance_t *pSSRC_Instance,
                                   LVM_Mode_en bHeadroomGainEnabled,
                                   LVM_Mode_en bOutputGainEnabled,
                                   LVM_INT16 OutputGain);

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                SSRC_Process                                                */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Process function for the SSRC module.                                               */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  pSSRC_Instance          Instance pointer                                            */
/*  pSSRC_AudioIn           Pointer to the input data 16 bit                            */
/*  pSSRC_AudioOut          Pointer to the output data 16 bit                           */
/*                                                                                      */
/* RETURNS:                                                                             */
/* SSRC_OK                  Succeeded                                                   */
/* SSRC_NULL_POINTER        When one of pSSRC_Instance, pSSRC_AudioIn or pSSRC_AudioOut */
/*                          is NULL                                                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/

SSRC_ReturnStatus_en SSRC_Process(SSRC_Instance_t *pSSRC_Instance, LVM_INT16 *pSSRC_AudioIn, LVM_INT16 *pSSRC_AudioOut);

/****************************************************************************************/

/****************************************************************************************/
/*                                                                                      */
/* FUNCTION:                SSRC_Process_D32                                            */
/*                                                                                      */
/* DESCRIPTION:                                                                         */
/*  Process function for the SSRC module.                                               */
/*                                                                                      */
/* PARAMETERS:                                                                          */
/*  pSSRC_Instance          Instance pointer                                            */
/*  pSSRC_AudioIn           Pointer to the input data 32 bit                            */
/*  pSSRC_AudioOut          Pointer to the output data 32 bit                           */
/*                                                                                      */
/* RETURNS:                                                                             */
/* SSRC_OK                  Succeeded                                                   */
/* SSRC_NULL_POINTER        When one of pSSRC_Instance, pSSRC_AudioIn or pSSRC_AudioOut */
/*                          is NULL                                                     */
/*                                                                                      */
/* NOTES:                                                                               */
/*                                                                                      */
/****************************************************************************************/

SSRC_ReturnStatus_en SSRC_Process_D32(SSRC_Instance_t *pSSRC_Instance,
                                      LVM_INT32 *pSSRC_AudioIn,
                                      LVM_INT32 *pSSRC_AudioOut);

/****************************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* SSRC_HEAD_H */
