/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

#ifndef __DCDC_H__
#define __DCDC_H__

#include "EmbeddedTypes.h"


/*****************************************************************************
******************************************************************************
* Public macros
******************************************************************************
*****************************************************************************/

/*
 * NAME: gDCDC_Enabled_d
 * DESCRIPTION: Enables / Disables the DCDC platform component
 * VALID RANGE: TRUE/FALSE
 */
#ifndef gDCDC_Enabled_d
#define gDCDC_Enabled_d    0
#endif

/*
 * NAME: gDCDC_PowerSwitchPinTiedUp_d 
 * DESCRIPTION: Configure the DCDC PSWITCH pin status is Tied Up
 * VALID RANGE: TRUE/FALSE,TRUE for status tied up, FALSE for not tied up
 */
#ifndef gDCDC_PowerSwitchPinTiedUp_d 
#define gDCDC_PowerSwitchPinTiedUp_d     0
#endif

#define  gDCDC_Bypass_c             0U 
#define  gDCDC_Buck_c               1U

#ifndef gDCDC_IsrPrio_c
#define gDCDC_IsrPrio_c       (0x80U)
#endif

/*****************************************************************************
******************************************************************************
* Public type definitions
******************************************************************************
*****************************************************************************/
typedef enum
{
  gDCDC_PSwStatus_Low_c,
  gDCDC_PSwStatus_High_c
}dcdcPSwStatus_t;

typedef enum
{
  gDCDC_PSwIntEdge_Rising_c = 1,
  gDCDC_PSwIntEdge_Falling_c,
  gDCDC_PSwIntEdge_All_c
}dcdcPSwIntEdge_t;

typedef void (*pfDCDCPSwitchCallback_t)(dcdcPSwStatus_t dcdcPs);

typedef enum 
{
  gDCDC_Mode_Bypass_c = gDCDC_Bypass_c,
  gDCDC_Mode_Buck_c   = gDCDC_Buck_c,
}dcdcMode_t;

typedef enum 
{
  gDCDC_Event_NoEvent_c = 0x0U,
  gDCDC_Event_VBatOutOfRange_c = 0x1U,
  gDCDC_Event_McuV_OutputTargetChange_c = 0x2U,
  gDCDC_Event_1P8OutputTargetChange_c = 0x4U  
}dcdcEvent_t;

typedef enum 
{
  gDCDC_McuV_OutputTargetVal_1_275_c = 0U,
  gDCDC_McuV_OutputTargetVal_1_300_c,
  gDCDC_McuV_OutputTargetVal_1_325_c,
  gDCDC_McuV_OutputTargetVal_1_350_c,
  gDCDC_McuV_OutputTargetVal_1_375_c,
  gDCDC_McuV_OutputTargetVal_1_400_c,
  gDCDC_McuV_OutputTargetVal_1_425_c,
  gDCDC_McuV_OutputTargetVal_1_450_c,
  gDCDC_McuV_OutputTargetVal_1_475_c,
  gDCDC_McuV_OutputTargetVal_1_500_c,
  gDCDC_McuV_OutputTargetVal_1_525_c,
  gDCDC_McuV_OutputTargetVal_1_550_c,
  gDCDC_McuV_OutputTargetVal_1_575_c,
  gDCDC_McuV_OutputTargetVal_1_600_c,
  gDCDC_McuV_OutputTargetVal_1_625_c,
  gDCDC_McuV_OutputTargetVal_1_650_c,
  gDCDC_McuV_OutputTargetVal_1_675_c,
  gDCDC_McuV_OutputTargetVal_1_700_c,
  gDCDC_McuV_OutputTargetVal_1_725_c,
  gDCDC_McuV_OutputTargetVal_1_750_c,
  gDCDC_McuV_OutputTargetVal_1_775_c,
  gDCDC_McuV_OutputTargetVal_1_800_c
}dcdcMcuVOutputTargetVal_t;

typedef enum 
{
  gDCDC_1P8OutputTargetVal_1_650_c = 0U,
  gDCDC_1P8OutputTargetVal_1_675_c,
  gDCDC_1P8OutputTargetVal_1_700_c,
  gDCDC_1P8OutputTargetVal_1_725_c,
  gDCDC_1P8OutputTargetVal_1_750_c,
  gDCDC_1P8OutputTargetVal_1_775_c,
  gDCDC_1P8OutputTargetVal_1_800_c,
  gDCDC_1P8OutputTargetVal_1_825_c,
  gDCDC_1P8OutputTargetVal_1_850_c,
  gDCDC_1P8OutputTargetVal_1_875_c,
  gDCDC_1P8OutputTargetVal_1_900_c,
  gDCDC_1P8OutputTargetVal_1_925_c,
  gDCDC_1P8OutputTargetVal_1_950_c,
  gDCDC_1P8OutputTargetVal_1_975_c,
  gDCDC_1P8OutputTargetVal_2_000_c,
  gDCDC_1P8OutputTargetVal_2_025_c,
  gDCDC_1P8OutputTargetVal_2_050_c,
  gDCDC_1P8OutputTargetVal_2_075_c,
  gDCDC_1P8OutputTargetVal_2_100_c,
  gDCDC_1P8OutputTargetVal_2_150_c,
  gDCDC_1P8OutputTargetVal_2_200_c,
  gDCDC_1P8OutputTargetVal_2_250_c,
  gDCDC_1P8OutputTargetVal_2_300_c,
  gDCDC_1P8OutputTargetVal_2_350_c,
  gDCDC_1P8OutputTargetVal_2_400_c,
  gDCDC_1P8OutputTargetVal_2_450_c,
  gDCDC_1P8OutputTargetVal_2_500_c,
  gDCDC_1P8OutputTargetVal_2_550_c,
  gDCDC_1P8OutputTargetVal_2_600_c,
  gDCDC_1P8OutputTargetVal_2_650_c,
  gDCDC_1P8OutputTargetVal_2_700_c,
  gDCDC_1P8OutputTargetVal_2_750_c,
  gDCDC_1P8OutputTargetVal_2_800_c = 0x20U,
  gDCDC_1P8OutputTargetVal_2_825_c,
  gDCDC_1P8OutputTargetVal_2_850_c,
  gDCDC_1P8OutputTargetVal_2_875_c,
  gDCDC_1P8OutputTargetVal_2_900_c,
  gDCDC_1P8OutputTargetVal_2_925_c,
  gDCDC_1P8OutputTargetVal_2_950_c,
  gDCDC_1P8OutputTargetVal_2_975_c,
  gDCDC_1P8OutputTargetVal_3_000_c,
  gDCDC_1P8OutputTargetVal_3_025_c,
  gDCDC_1P8OutputTargetVal_3_050_c,
  gDCDC_1P8OutputTargetVal_3_075_c,
  gDCDC_1P8OutputTargetVal_3_100_c,
  gDCDC_1P8OutputTargetVal_3_125_c,
  gDCDC_1P8OutputTargetVal_3_150_c,
  gDCDC_1P8OutputTargetVal_3_175_c,
  gDCDC_1P8OutputTargetVal_3_200_c,
  gDCDC_1P8OutputTargetVal_3_225_c,
  gDCDC_1P8OutputTargetVal_3_250_c,
  gDCDC_1P8OutputTargetVal_3_275_c,
  gDCDC_1P8OutputTargetVal_3_300_c,
  gDCDC_1P8OutputTargetVal_3_325_c,
  gDCDC_1P8OutputTargetVal_3_350_c,
  gDCDC_1P8OutputTargetVal_3_375_c,
  gDCDC_1P8OutputTargetVal_3_400_c,
  gDCDC_1P8OutputTargetVal_3_425_c,
  gDCDC_1P8OutputTargetVal_3_450_c,
  gDCDC_1P8OutputTargetVal_3_475_c,
  gDCDC_1P8OutputTargetVal_3_500_c
 }dcdc1P8OutputTargetVal_t;

typedef struct dcdcCalbackParam_tag
{
  dcdcEvent_t dcdcEvent;
  dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTargetVal;
  dcdc1P8OutputTargetVal_t dcdc1P8OutputTargetVal;
  uint16_t dcdc1P8OutputMeasuredVal;
  uint16_t dcdcVbatMeasuredVal;
}dcdcCallbackParam_t;
typedef void (*pfDCDCAppCallback_t)(const dcdcCallbackParam_t* callback);

typedef struct dcdcConfig_tag
{
  uint16_t vbatMin;
  uint16_t vbatMax;
  uint8_t dcdcMode;
  uint32_t vBatMonitorIntervalMs;
  pfDCDCAppCallback_t pfDCDCAppCallback; 
  dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTargetVal;
  dcdc1P8OutputTargetVal_t  dcdc1P8OutputTargetVal;
}dcdcConfig_t;
 
#define gDCDC_McuV_OutputTargetVal_MAX_c gDCDC_McuV_OutputTargetVal_1_800_c
#define gDCDC_1P8OutputTargetVal_MAX_c   gDCDC_1P8OutputTargetVal_3_500_c
/*****************************************************************************
******************************************************************************
* Public memory declarations
******************************************************************************
*****************************************************************************/

/* none */

/*****************************************************************************
******************************************************************************
* Public prototypes
******************************************************************************
*****************************************************************************/


/*---------------------------------------------------------------------------
 * NAME: DCDC_Init
 * DESCRIPTION: initialize the DCDC module
 *---------------------------------------------------------------------------*/
extern bool_t DCDC_Init
(
    const dcdcConfig_t * pDCDCConfig
);

/*---------------------------------------------------------------------------
 * NAME: DCDC_EnterLowPower
 * DESCRIPTION: prepare to enter low power mode
 *---------------------------------------------------------------------------*/
void DCDC_EnterLowPowerCb(void);

/*---------------------------------------------------------------------------
 * NAME: DCDC_ExitLowPower
 * DESCRIPTION: prepare to exit low power mode
 *---------------------------------------------------------------------------*/
void DCDC_ExitLowPowerCb(void);

/*---------------------------------------------------------------------------
 * NAME: DCDC_ExitLowPowerPostCb
 * DESCRIPTION: used after LL restore
 *---------------------------------------------------------------------------*/
void DCDC_ExitLowPowerPostCb(void);

/*---------------------------------------------------------------------------
 * NAME: DCDC_SetOutputVoltageTargets
 * DESCRIPTION: initialize the timer module
 *---------------------------------------------------------------------------*/
bool_t DCDC_SetOutputVoltageTargets
(
dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTargetVal,
dcdc1P8OutputTargetVal_t  dcdc1P8OutputTargetVal
);

/*---------------------------------------------------------------------------
* NAME: DCDC_McuVOutputTargetTomV
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
uint16_t DCDC_McuVOutputTargetTomV(dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTarget);
/*---------------------------------------------------------------------------
* NAME: DCDC_1P8OutputTargetTomV
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
uint16_t DCDC_1P8OutputTargetTomV(dcdc1P8OutputTargetVal_t dcdc1P8OutputTarget);
/*---------------------------------------------------------------------------
* NAME: DCDC_PrepareForPulsedMode
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
bool_t DCDC_PrepareForPulsedMode(void);
/*---------------------------------------------------------------------------
* NAME: DCDC_PrepareForContinuousMode
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
bool_t DCDC_PrepareForContinuousMode(void);

/*---------------------------------------------------------------------------
* NAME: DCDC_SetUpperLimitDutyCycle
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
bool_t DCDC_SetUpperLimitDutyCycle(uint8_t upperLimitDutyCycle);

/*---------------------------------------------------------------------------
* NAME: DCDC_GetPowerSwitchStatus
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
bool_t DCDC_GetPowerSwitchStatus(dcdcPSwStatus_t* pDCDCPSwStatus);
/*---------------------------------------------------------------------------
* NAME: DCDC_ShutDown
* DESCRIPTION: 
*---------------------------------------------------------------------------*/
void DCDC_ShutDown(void);
/*---------------------------------------------------------------------------
* NAME: DCDC_InstallPSwitchCallback
* DESCRIPTION:
*---------------------------------------------------------------------------*/
bool_t DCDC_InstallPSwitchCallback(pfDCDCPSwitchCallback_t pfPSwCallback, dcdcPSwIntEdge_t pSwIntEdge);
#endif /* #ifndef __DCDC_H__ */

/*****************************************************************************
 *                               <<< EOF >>>                                 *
 *****************************************************************************/
