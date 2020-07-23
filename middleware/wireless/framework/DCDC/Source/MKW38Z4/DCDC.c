/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017,2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */
#include "EmbeddedTypes.h"
#include "DCDC.h"
#include "TimersManager.h"
#include "fsl_os_abstraction.h"
#include "board.h"
/*****************************************************************************
******************************************************************************
* Private type definitions
******************************************************************************
*****************************************************************************/

typedef struct dcdcInputs_tag
{
  dcdcMcuVOutputTargetVal_t outputTarget_McuV;
  dcdc1P8OutputTargetVal_t  outputTarget_1P8;
  uint16_t vbatVal_mV;
}dcdcInputs_t;

/*****************************************************************************
******************************************************************************
* Private macros
******************************************************************************
*****************************************************************************/
#ifdef DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK_MASK  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK_SHIFT  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_SHIFT
#else
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK  DCDC_REG3_DCDC_VDD1P45CTRL_TRG_BUCK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK_MASK  DCDC_REG3_DCDC_VDD1P45CTRL_TRG_BUCK_MASK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK_SHIFT  DCDC_REG3_DCDC_VDD1P45CTRL_TRG_BUCK_SHIFT
#endif

#ifdef DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BOOST_MASK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BOOST  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BOOST
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BOOST_MASK  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BOOST_MASK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BOOST_SHIFT  DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BOOST_SHIFT
#else
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BOOST  DCDC_REG3_DCDC_VDD1P45CTRL_TRG_BOOST
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BOOST_MASK  DCDC_REG3_DCDC_VDD1P45CTRL_TRG_BOOST_MASK
#define DCDC_REG3_VDD_MCU_CTRL_TRG_BOOST_SHIFT  DCDC_REG3_DCDC_VDD1P45CTRL_TRG_BOOST_SHIFT
#endif
#ifdef DCDC_REG3_DCDC_VDD1P5CTRL_DISABLE_STEP_MASK
#define DCDC_REG3_DCDC_VDD_MCU_CTRL_DISABLE_STEP_MASK  DCDC_REG3_DCDC_VDD1P5CTRL_DISABLE_STEP_MASK
#else
#define DCDC_REG3_DCDC_VDD_MCU_CTRL_DISABLE_STEP_MASK  DCDC_REG3_DCDC_VDD1P45CTRL_DISABLE_STEP_MASK
#endif


#define REG_READ_FIELD(reg, fieldName)                (((reg) & (fieldName##_MASK)) >> (fieldName##_SHIFT))

#define REG_READ_MODIFY_FIELD(reg, fieldName, value)  (((reg) & (~fieldName##_MASK)) | (((uint32_t)(((uint32_t)(value)) << fieldName##_SHIFT)) & fieldName##_MASK))

#define REG_WRITE_FIELD(reg, fieldName, value) {reg = (((reg) & (~fieldName##_MASK)) | fieldName(value));}


#define mDCDC_VBatMinBuck_c      1800U
#define mDCDC_VBatMaxBuck_c      4200U
#define mDCDC_VBatMinBoost_c     900U
#define mDCDC_VBatMaxBoost_c     1800U
#define mDCDC_McuVTrgMaxBuck_c   gDCDC_McuV_OutputTargetVal_1_650_c
#define mDCDC_McuVTrgMaxBoost_c  gDCDC_McuV_OutputTargetVal_1_800_c
#define mDCDC_BoostVOutToVBatMin_c   50U
#define mDCDC_BuckVBatToVOutMin_c    0U
#define mDCDC_PosLimitBoostIn_c      0x12U
#define mDCDC_BGAPVal_mV_c           999U
#define mDCDC_DutyCycleMax_c         127U
//#define mDCDC_BGAPVal_mV_c         1037U
//#define mDCDC_BGAPVal_mV_c         1038U
#ifndef gADC16_ReferenceVoltageSource_d
#define gADC16_ReferenceVoltageSource_d kADC16_ReferenceVoltageSourceVref
#endif
/*****************************************************************************
 *****************************************************************************
 * Private prototypes
 *****************************************************************************
 *****************************************************************************/

/*****************************************************************************
 *****************************************************************************
 * Private memory definitions
 *****************************************************************************
 *****************************************************************************/

#if gDCDC_Enabled_d
static tmrTimerID_t mVBatMonitorTimerId;
static const dcdcConfig_t * mpDCDCConfig;
static dcdcCallbackParam_t mDCDCCallbackParam;
static dcdcInputs_t mDCDCInputs;
#if gDCDC_PowerSwitchPinTiedUp_d == 0
static pfDCDCPSwitchCallback_t mpfDCDCPSwitchCallback;
#endif /*gDCDC_PowerSwitchPinTiedUp_d*/
#endif /*gDCDC_Enabled_d*/

/*****************************************************************************
******************************************************************************
* Private functions
******************************************************************************
*****************************************************************************/

#if gDCDC_Enabled_d
/*---------------------------------------------------------------------------
* NAME: DCDC_AdjustVbatDiv
* DESCRIPTION:
*---------------------------------------------------------------------------*/
static void DCDC_AdjustVbatDiv(void)
{
  uint16_t batVal;
  uint8_t vBatDiv = 3U;
  do
  {
    DCDC->REG0 = REG_READ_MODIFY_FIELD(DCDC->REG0, DCDC_REG0_DCDC_VBAT_DIV_CTRL, vBatDiv);
    batVal = BOARD_GetAdcBatLvl();
    if(batVal > 0x7FFU)
    {
      break;
    }
  }
  while(vBatDiv-- != 2U);
}
/*---------------------------------------------------------------------------
* NAME: DCDC_Get1P8VOutputTargetAndVBatInmV
* DESCRIPTION:
*---------------------------------------------------------------------------*/
static void DCDC_Get1P8VOutputTargetAndVBatInmV(uint16_t* p1P8VOutputInMv, uint16_t* pVBatInMv )
{
  uint16_t batVal;
  uint16_t bgapVal;
  uint32_t mvVal;
  uint8_t vBatDiv;
  batVal = BOARD_GetAdcBatLvl();
  bgapVal = BOARD_GetAdcBgLvl();
  mvVal = 4095U;
  mvVal *= mDCDC_BGAPVal_mV_c;
  mvVal += ((uint32_t)bgapVal>>1U);
  mvVal /= bgapVal;
  *p1P8VOutputInMv = (uint16_t)(mvVal&0xffffU);
  vBatDiv = (uint8_t)(REG_READ_FIELD(DCDC->REG0, DCDC_REG0_DCDC_VBAT_DIV_CTRL)&0xffU);
  vBatDiv--;
  mvVal = ((uint32_t)batVal<<vBatDiv);
  mvVal *= mDCDC_BGAPVal_mV_c;
  mvVal += ((uint32_t)bgapVal>>1U);
  mvVal /= bgapVal;
  *pVBatInMv = (uint16_t)(mvVal&0xffffU);
}
/*---------------------------------------------------------------------------
* NAME: DCDC_mVTo1P8OutputTargetBuck
* DESCRIPTION:
*---------------------------------------------------------------------------*/
static dcdc1P8OutputTargetVal_t DCDC_mVTo1P8OutputTargetBuck(uint16_t mV)
{
    dcdc1P8OutputTargetVal_t status;
    if(mV < 1650U)
    {
        status = gDCDC_1P8OutputTargetVal_1_650_c;
    }
    else if(mV <= 2100U)
    {
        mV -= 1650U;
        mV /= 25U;
        status = (dcdc1P8OutputTargetVal_t)mV;
    }
    else if(mV <= 2800U)
    {
        mV -= 2100U;
        mV /= 50U;
        mV += (uint16_t)gDCDC_1P8OutputTargetVal_2_100_c;
        status = (dcdc1P8OutputTargetVal_t)mV;
    }
    else
    {
        mV -= 2800U;
        mV /= 25U;
        mV += (uint16_t)gDCDC_1P8OutputTargetVal_2_800_c;
        if(mV > (uint16_t)gDCDC_1P8OutputTargetVal_MAX_c)
        {
            mV = (uint16_t)gDCDC_1P8OutputTargetVal_MAX_c;
        }
        status = (dcdc1P8OutputTargetVal_t)mV;
    }
    return status;
}

/*---------------------------------------------------------------------------
 * NAME: DCDC_GetOutputTargetsBuck
 * DESCRIPTION:
 *---------------------------------------------------------------------------*/
static void DCDC_GetOutputTargetsBuck(dcdcMcuVOutputTargetVal_t* pMcuVOutputTarget, dcdc1P8OutputTargetVal_t* p1P8OutputTarget )
{
  dcdcMcuVOutputTargetVal_t dcdcMcuVWanted;
  dcdc1P8OutputTargetVal_t dcdc1P8MaxOutputTarget,dcdc1P8OutputTarget,dcdc1P8Wanted;
  uint16_t vBATmV;
  OSA_InterruptDisable();
  {
    dcdcMcuVWanted = mDCDCInputs.outputTarget_McuV;
    dcdc1P8Wanted = mDCDCInputs.outputTarget_1P8;
    vBATmV = mDCDCInputs.vbatVal_mV;
  }
  OSA_InterruptEnable();
  dcdc1P8MaxOutputTarget = DCDC_mVTo1P8OutputTargetBuck(vBATmV - mDCDC_BuckVBatToVOutMin_c);
  if(dcdc1P8Wanted <= dcdc1P8MaxOutputTarget)
  {
    dcdc1P8OutputTarget = dcdc1P8Wanted;
  }
  else
  {
    dcdc1P8OutputTarget = dcdc1P8MaxOutputTarget;
  }
  *pMcuVOutputTarget = dcdcMcuVWanted;
  *p1P8OutputTarget = dcdc1P8OutputTarget;
}
/*---------------------------------------------------------------------------
* NAME: DCDC_VBatMonitorBuck
* DESCRIPTION:
*---------------------------------------------------------------------------*/
static void DCDC_VBatMonitorBuck(void* param)
{
    //BOARD_DBGLPIOSET(5,0);
    uint16_t mV_VBat;
    uint16_t mV_1P8V;
    uint8_t dcdcMcuVOutput;
    uint8_t dcdc1P8Output;
    uint8_t dcdcEvent = 0x0U;
    dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTarget;
    dcdc1P8OutputTargetVal_t dcdc1P8OutputTarget;
    mDCDCCallbackParam.dcdcEvent = gDCDC_Event_NoEvent_c;

    /* This function shouldn't be used in bypass mode */
    assert(mpDCDCConfig->dcdcMode == gDCDC_Buck_c);

    DCDC_AdjustVbatDiv();
    DCDC_Get1P8VOutputTargetAndVBatInmV(&mV_1P8V ,&mV_VBat);
    OSA_InterruptDisable();
    mDCDCInputs.vbatVal_mV  = mV_VBat;
    OSA_InterruptEnable();

    DCDC->REG2 &= ~DCDC_REG2_DCDC_BATTMONITOR_EN_BATADJ_MASK;
    DCDC->REG2 = REG_READ_MODIFY_FIELD(DCDC->REG2, DCDC_REG2_DCDC_BATTMONITOR_BATT_VAL, (uint32_t)((uint32_t)mV_VBat >> 3U));
    DCDC->REG2 |= DCDC_REG2_DCDC_BATTMONITOR_EN_BATADJ_MASK;
    mDCDCCallbackParam.dcdcVbatMeasuredVal = mV_VBat;
    mDCDCCallbackParam.dcdc1P8OutputMeasuredVal = mV_1P8V;
    DCDC_GetOutputTargetsBuck(&dcdcMcuVOutputTarget, &dcdc1P8OutputTarget );

    OSA_InterruptDisable();
    DCDC->REG3 = REG_READ_MODIFY_FIELD(DCDC->REG3, DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK, dcdcMcuVOutputTarget);
    DCDC->REG3 = REG_READ_MODIFY_FIELD(DCDC->REG3, DCDC_REG3_DCDC_VDD1P8CTRL_TRG, dcdc1P8OutputTarget);
    OSA_InterruptEnable();

    if(param == NULL)
    {
        if(mpDCDCConfig->pfDCDCAppCallback !=NULL )
        {
            if((mDCDCInputs.vbatVal_mV < mpDCDCConfig->vbatMin) || (mDCDCInputs.vbatVal_mV > mpDCDCConfig->vbatMax))
            {
                dcdcEvent |= (uint8_t)gDCDC_Event_VBatOutOfRange_c;
            }
            OSA_InterruptDisable();
            dcdcMcuVOutput = (uint8_t)(REG_READ_FIELD(DCDC->REG3, DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK)&0xffU);
            if(mDCDCCallbackParam.dcdcMcuVOutputTargetVal != (dcdcMcuVOutputTargetVal_t)dcdcMcuVOutput)
            {
                mDCDCCallbackParam.dcdcMcuVOutputTargetVal = (dcdcMcuVOutputTargetVal_t)dcdcMcuVOutput;
                dcdcEvent |= (uint8_t)gDCDC_Event_McuV_OutputTargetChange_c;
            }
            dcdc1P8Output = (uint8_t)(REG_READ_FIELD(DCDC->REG3, DCDC_REG3_DCDC_VDD1P8CTRL_TRG)&0xffU);
            if(mDCDCCallbackParam.dcdc1P8OutputTargetVal != (dcdc1P8OutputTargetVal_t)dcdc1P8Output)
            {
                mDCDCCallbackParam.dcdc1P8OutputTargetVal = (dcdc1P8OutputTargetVal_t)dcdc1P8Output;
                dcdcEvent |= (uint8_t)gDCDC_Event_1P8OutputTargetChange_c;
            }
            OSA_InterruptEnable();
            mDCDCCallbackParam.dcdcEvent = (dcdcEvent_t)dcdcEvent;
            mpDCDCConfig->pfDCDCAppCallback(&mDCDCCallbackParam);
        }
    }
    //BOARD_DBGLPIOSET(5,1);
}

/*---------------------------------------------------------------------------
 * NAME: DCDC_PSwitchIsr
 * DESCRIPTION:
 *---------------------------------------------------------------------------*/
#if gDCDC_PowerSwitchPinTiedUp_d == 0
static void DCDC_PSwitchIsr(void)
{
  dcdcPSwStatus_t dcdcPSwStatus;
  DCDC->REG6 |= DCDC_REG6_PSWITCH_INT_CLEAR_MASK;
  DCDC->REG6 &= ~DCDC_REG6_PSWITCH_INT_CLEAR_MASK;
  /* Check pswitch status */
  if( REG_READ_FIELD(DCDC->REG0, DCDC_REG0_PSWITCH_STATUS) == 0U)
  {
    dcdcPSwStatus = gDCDC_PSwStatus_Low_c;
  }
  else
  {
    dcdcPSwStatus = gDCDC_PSwStatus_High_c;
  }
  /* Call callback fun */
  mpfDCDCPSwitchCallback(dcdcPSwStatus);
}
#endif /*gDCDC_PowerSwitchPinTiedUp_d*/

#endif /*gDCDC_Enabled_d*/

/*****************************************************************************
******************************************************************************
* Public functions
******************************************************************************
*****************************************************************************/

/*---------------------------------------------------------------------------
 * NAME: DCDC_Init
 * DESCRIPTION: initialize
 *---------------------------------------------------------------------------*/
bool_t DCDC_Init(const dcdcConfig_t * pDCDCConfig)
{
#if gDCDC_Enabled_d == 0
    (void) pDCDCConfig;
    return TRUE;
#else /*gDCDC_Enabled_d == 0*/
    bool_t ret = TRUE;

    if(pDCDCConfig == NULL)
    {
        ret = FALSE;
    }
    else
    {
        CLOCK_EnableClock(kCLOCK_Dcdc0);
        if(pDCDCConfig->dcdcMode == gDCDC_Bypass_c)
        {
            mpDCDCConfig = pDCDCConfig;
        }
        else
        {
            if(pDCDCConfig->vbatMin > pDCDCConfig->vbatMax)
            {
                ret = FALSE;
            }
            else
            {
                if(pDCDCConfig->dcdcMode == gDCDC_Buck_c)
                {
                    if((pDCDCConfig->vbatMin < mDCDC_VBatMinBuck_c)||(pDCDCConfig->vbatMax > mDCDC_VBatMaxBuck_c))
                    {
                        ret = FALSE;
                    }
                    else
                    {
                        if(gDCDC_McuV_OutputTargetVal_MAX_c < pDCDCConfig->dcdcMcuVOutputTargetVal)
                        {
                            ret = FALSE;
                        }
                    }
                }
                else
                {
                    ret = FALSE;
                }
                if(TRUE == ret)
                {
                    if(gDCDC_1P8OutputTargetVal_MAX_c < pDCDCConfig->dcdc1P8OutputTargetVal)
                    {
                        ret = FALSE;
                    }
                    else
                    {
                        TMR_Init();
                        mVBatMonitorTimerId = TMR_AllocateTimer();
                        if(gTmrInvalidTimerID_c == mVBatMonitorTimerId)
                        {
                            ret = FALSE;
                        }
                        else
                        {
                            BOARD_InitAdc();
                            mDCDCInputs.outputTarget_McuV = pDCDCConfig->dcdcMcuVOutputTargetVal;
                            mDCDCInputs.outputTarget_1P8 = pDCDCConfig->dcdc1P8OutputTargetVal;
                            mpDCDCConfig = pDCDCConfig;
                            DCDC->REG1 |= DCDC_REG1_DCDC_LOOPCTRL_EN_DF_HYST_MASK | DCDC_REG1_DCDC_LOOPCTRL_EN_CM_HYST_MASK;
                            DCDC->REG2 |= DCDC_REG2_DCDC_LOOPCTRL_HYST_SIGN_MASK;
                            DCDC->REG3 &= ~(DCDC_REG3_DCDC_VDD_MCU_CTRL_DISABLE_STEP_MASK | DCDC_REG3_DCDC_VDD1P8CTRL_DISABLE_STEP_MASK );
                            PMC->REGSC |=PMC_REGSC_BGBE_MASK;
                            mDCDCCallbackParam.dcdc1P8OutputTargetVal = gDCDC_1P8OutputTargetVal_1_800_c;
                            #ifdef DCDC_REG3_DCDC_VDD1P5CTRL_TRG_BUCK_MASK
                            mDCDCCallbackParam.dcdcMcuVOutputTargetVal = gDCDC_McuV_OutputTargetVal_1_500_c;
                            #else
                            mDCDCCallbackParam.dcdcMcuVOutputTargetVal = gDCDC_McuV_OutputTargetVal_1_450_c;
                            #endif
                            DCDC_AdjustVbatDiv();
                            DCDC_VBatMonitorBuck((uint8_t*)1UL);
                            (void)TMR_StartLowPowerTimer(mVBatMonitorTimerId, gTmrIntervalTimer_c ,pDCDCConfig->vBatMonitorIntervalMs, DCDC_VBatMonitorBuck, NULL);
                        }
                    }
                }
            }
        }
    }
    return ret;
#endif /*gDCDC_Enabled_d == 0*/
}
/*---------------------------------------------------------------------------
 * NAME: DCDC_EnterLowPowerCb
 * DESCRIPTION: enter low power mode
 *---------------------------------------------------------------------------*/
void DCDC_EnterLowPowerCb(void)
{
#if gDCDC_Enabled_d

    (void)DCDC_PrepareForPulsedMode();
    /* stop DCDC timer to avoid wake up during no activity */
    (void)TMR_StopTimer(mVBatMonitorTimerId);
#endif
}
/*---------------------------------------------------------------------------
 * NAME: DCDC_ExitLowPowerCb
 * DESCRIPTION: exit low power mode
 *---------------------------------------------------------------------------*/
void DCDC_ExitLowPowerCb(void)
{
#if gDCDC_Enabled_d
    (void)DCDC_PrepareForContinuousMode();
#endif
}
/*---------------------------------------------------------------------------
 * NAME: DCDC_ExitLowPowerPostCb
 * DESCRIPTION: used after LL restore
 *---------------------------------------------------------------------------*/
void DCDC_ExitLowPowerPostCb(void)
{
#if (gDCDC_Enabled_d) && (APP_DCDC_MODE == gDCDC_Buck_c)
    /* Perform Vbat monitor */
    DCDC_VBatMonitorBuck(NULL);
    /* Restart VBat monitor timer in case the device stays active for a long time */
    (void)TMR_StartLowPowerTimer(mVBatMonitorTimerId, gTmrIntervalTimer_c , mpDCDCConfig->vBatMonitorIntervalMs, DCDC_VBatMonitorBuck, NULL);
#endif
}
/*---------------------------------------------------------------------------
 * NAME: DCDC_SetOutputVoltageTargets
 * DESCRIPTION: initialize the timer module
 *---------------------------------------------------------------------------*/
bool_t DCDC_SetOutputVoltageTargets(dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTargetVal, dcdc1P8OutputTargetVal_t  dcdc1P8OutputTargetVal)
{
#if gDCDC_Enabled_d == 0
  (void)dcdcMcuVOutputTargetVal;
  (void)dcdc1P8OutputTargetVal;
  return TRUE;
#else /*gDCDC_Enabled_d == 0*/
  dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTarget;
  dcdc1P8OutputTargetVal_t dcdc1P8OutputTarget;
  bool_t ret = FALSE;

  if(NULL != mpDCDCConfig) /*if mpDCDCConfig == NULL, DCDC_Init has to be called prior to this function*/
  {
    if(mpDCDCConfig->dcdcMode == gDCDC_Bypass_c)
    {
      ret = TRUE;
    }
    else
    {
      if(gDCDC_McuV_OutputTargetVal_MAX_c >= dcdcMcuVOutputTargetVal)
      {
        if(gDCDC_1P8OutputTargetVal_MAX_c >= dcdc1P8OutputTargetVal)
        {
          OSA_InterruptDisable();
          mDCDCInputs.outputTarget_McuV = dcdcMcuVOutputTargetVal;
          mDCDCInputs.outputTarget_1P8 = dcdc1P8OutputTargetVal;
          OSA_InterruptEnable();
          DCDC_GetOutputTargetsBuck(&dcdcMcuVOutputTarget, &dcdc1P8OutputTarget );

          OSA_InterruptDisable();
          {
            DCDC->REG3 = REG_READ_MODIFY_FIELD(DCDC->REG3, DCDC_REG3_VDD_MCU_CTRL_TRG_BUCK, dcdcMcuVOutputTarget);
            DCDC->REG3 = REG_READ_MODIFY_FIELD(DCDC->REG3, DCDC_REG3_DCDC_VDD1P8CTRL_TRG, dcdc1P8OutputTarget);
          }
          OSA_InterruptEnable();
          ret = TRUE;
        }
      }
    }
  }
  return ret;
#endif /*gDCDC_Enabled_d == 0*/
}
/*---------------------------------------------------------------------------
* NAME: DCDC_McuVOutputTargetTomV
* DESCRIPTION:
*---------------------------------------------------------------------------*/
uint16_t DCDC_McuVOutputTargetTomV(dcdcMcuVOutputTargetVal_t dcdcMcuVOutputTarget)
{
  uint16_t mV;
  mV = (uint16_t)((uint16_t)dcdcMcuVOutputTarget-(uint16_t)gDCDC_McuV_OutputTargetVal_1_275_c) * 25U + 1275U;
  return mV;
}
/*---------------------------------------------------------------------------
* NAME: DCDC_1P8OutputTargetTomV
* DESCRIPTION:
*---------------------------------------------------------------------------*/
uint16_t DCDC_1P8OutputTargetTomV(dcdc1P8OutputTargetVal_t dcdc1P8OutputTarget)
{
  uint16_t mV;
  if( dcdc1P8OutputTarget < gDCDC_1P8OutputTargetVal_2_100_c)
  {
    mV = (uint16_t)((uint16_t)dcdc1P8OutputTarget - (uint16_t)gDCDC_1P8OutputTargetVal_1_650_c)*25U + 1650U;
  }
  else if(dcdc1P8OutputTarget < gDCDC_1P8OutputTargetVal_2_800_c )
  {
    mV = (uint16_t)((uint16_t)dcdc1P8OutputTarget - (uint16_t)gDCDC_1P8OutputTargetVal_2_100_c)*50U + 2100U;
  }
  else
  {
    mV = (uint16_t)((uint16_t)dcdc1P8OutputTarget - (uint16_t)gDCDC_1P8OutputTargetVal_2_800_c)*25U + 2800U;
  }

  return mV;
}
/*---------------------------------------------------------------------------
* NAME: DCDC_PrepareForPulsedMode
* DESCRIPTION:
*---------------------------------------------------------------------------*/
bool_t DCDC_PrepareForPulsedMode(void)
{
#if gDCDC_Enabled_d == 0
    return TRUE;
#else
  bool_t ret = FALSE;
  if(NULL != mpDCDCConfig ) /*if mpDCDCConfig == NULL, DCDC_Init has to be called prior to this function*/
  {
    //DCDC_BWR_REG0_DCDC_VBAT_DIV_CTRL(DCDC_BASE_PTR, 0);
    DCDC->REG0 &=  ~DCDC_REG0_DCDC_VBAT_DIV_CTRL_MASK ;

    if(gDCDC_Bypass_c != mpDCDCConfig->dcdcMode)
    {
      DCDC->REG3 |= DCDC_REG3_DCDC_VDD_MCU_CTRL_DISABLE_STEP_MASK | DCDC_REG3_DCDC_VDD1P8CTRL_DISABLE_STEP_MASK ;
      DCDC->REG1 |= DCDC_REG1_DCDC_LOOPCTRL_EN_DF_HYST_MASK | DCDC_REG1_DCDC_LOOPCTRL_EN_CM_HYST_MASK;
      DCDC->REG2 |= DCDC_REG2_DCDC_LOOPCTRL_HYST_SIGN_MASK;
      DCDC->REG0 |= DCDC_REG0_DCDC_LP_DF_CMP_ENABLE_MASK;
    }
    ret = TRUE;
  }
  return ret;
#endif
}

/*---------------------------------------------------------------------------
* NAME: DCDC_PrepareForContinuousMode
* DESCRIPTION:
*---------------------------------------------------------------------------*/
bool_t DCDC_PrepareForContinuousMode(void)
{
#if gDCDC_Enabled_d == 0
    return TRUE;
#else
    bool_t ret = FALSE;
    if(NULL != mpDCDCConfig) /*if NULL == mpDCDCConfig, DCDC_Init has to be called prior to this function*/
    {
        if( gDCDC_Bypass_c != mpDCDCConfig->dcdcMode )
        {
            DCDC->REG3 &= ~(DCDC_REG3_DCDC_VDD_MCU_CTRL_DISABLE_STEP_MASK | DCDC_REG3_DCDC_VDD1P8CTRL_DISABLE_STEP_MASK );
            DCDC->REG1 |= DCDC_REG1_DCDC_LOOPCTRL_EN_DF_HYST_MASK | DCDC_REG1_DCDC_LOOPCTRL_EN_CM_HYST_MASK;
            DCDC->REG2 |= DCDC_REG2_DCDC_LOOPCTRL_HYST_SIGN_MASK;
            DCDC->REG0 &= ~DCDC_REG0_DCDC_LP_DF_CMP_ENABLE_MASK;
        }
        ret = TRUE;
    }
    return ret;
#endif
}

/*---------------------------------------------------------------------------
* NAME: DCDC_SetUpperLimitDutyCycle
* DESCRIPTION:
*---------------------------------------------------------------------------*/
bool_t DCDC_SetUpperLimitDutyCycle(uint8_t upperLimitDutyCycle)
{
#if gDCDC_Enabled_d == 0
    return TRUE;
#else
    bool_t ret = FALSE;
    if(NULL != mpDCDCConfig) /*if mpDCDCConfig == NULL, DCDC_Init has to be called prior to this function*/
    {
        if(gDCDC_Bypass_c == mpDCDCConfig->dcdcMode )
        {
            ret = TRUE;
        }
        else
        {
            if(mDCDC_DutyCycleMax_c >= upperLimitDutyCycle)
            {
                DCDC->REG1 = REG_READ_MODIFY_FIELD(DCDC->REG1, DCDC_REG1_POSLIMIT_BUCK_IN,upperLimitDutyCycle);
                ret = TRUE;
            }
        }
    }
    return ret;
#endif
}

/*---------------------------------------------------------------------------
* NAME: DCDC_GetPowerSwitchStatus
* DESCRIPTION:
*---------------------------------------------------------------------------*/
bool_t DCDC_GetPowerSwitchStatus(dcdcPSwStatus_t* pDCDCPSwStatus)
{
#if gDCDC_Enabled_d == 0
  *pDCDCPSwStatus = gDCDC_PSwStatus_High_c;
  return TRUE;
#else /*gDCDC_Enabled_d == 0*/
  bool_t status = TRUE;
  if(mpDCDCConfig == NULL)
  {
    /* DCDC_Init has to be called prior to this function*/
    *pDCDCPSwStatus = gDCDC_PSwStatus_High_c;
    status = FALSE;
  }
  else
  {
     *pDCDCPSwStatus = (dcdcPSwStatus_t)(uint8_t)(REG_READ_FIELD(DCDC->REG0, DCDC_REG0_PSWITCH_STATUS)&0xffU);
  }
  return status;
#endif /*gDCDC_Enabled_d == 0*/
}

/*---------------------------------------------------------------------------
* NAME: DCDC_ShutDown
* DESCRIPTION:
*---------------------------------------------------------------------------*/
void DCDC_ShutDown(void)
{
#if gDCDC_Enabled_d == 0
    return ;
#else /*gDCDC_Enabled_d == 0*/

    assert(NULL != mpDCDCConfig);   /*if mpDCDCConfig is NULL, DCDC_Init has to be called prior to this function*/
    assert(mpDCDCConfig->dcdcMode == gDCDC_Buck_c);  /* dcdcMode must be buck */

#if gDCDC_PowerSwitchPinTiedUp_d == 0
    /*  if PSWITCH is pressed manually, its status can be 1 for some ms
        so we wait for the status to be 0.
        An infinite loop is not an issue here because when we call DCDC_ShutDown
        we assume the software should not go further or we may face an issue */
    while(TRUE)
    {
        if( REG_READ_FIELD(DCDC->REG0, DCDC_REG0_PSWITCH_STATUS) == 0U)
        {
            DCDC->REG4 = (uint32_t)0x3E770001U;
            DCDC->REG4 = (uint32_t)0x3E770001U;
        }
    }
#endif  /* gDCDC_PowerSwitchPinTiedUp_d */

#endif /*gDCDC_Enabled_d == 0*/
}
/*---------------------------------------------------------------------------
* NAME: DCDC_InstallPSwitchCallback
* DESCRIPTION:
*---------------------------------------------------------------------------*/
bool_t DCDC_InstallPSwitchCallback(pfDCDCPSwitchCallback_t pfPSwCallback, dcdcPSwIntEdge_t pSwIntEdge)
{
    bool_t status = FALSE;

#if gDCDC_Enabled_d == 0
    return status;
#else
    if(mpDCDCConfig != NULL)
    {
        (void)NVIC_DisableIRQ(LVD_LVW_DCDC_IRQn);
#if gDCDC_PowerSwitchPinTiedUp_d == 0
        mpfDCDCPSwitchCallback = pfPSwCallback;
#endif  /* gDCDC_PowerSwitchPinTiedUp_d */
        if(pfPSwCallback != NULL)
        {
            uint32_t reg6;
            /* Clear ISR status for DCDC */
            reg6 = DCDC->REG6 & ~(DCDC_REG6_PSWITCH_INT_RISE_EN_MASK | DCDC_REG6_PSWITCH_INT_FALL_EN_MASK | DCDC_REG6_PSWITCH_INT_MUTE_MASK);
            if(((uint8_t)gDCDC_PSwIntEdge_Rising_c & (uint8_t)pSwIntEdge) != 0U)
            {
                reg6 |= DCDC_REG6_PSWITCH_INT_RISE_EN_MASK;
            }
            if(((uint8_t)gDCDC_PSwIntEdge_Falling_c & (uint8_t)pSwIntEdge) != 0U)
            {
                reg6 |= DCDC_REG6_PSWITCH_INT_FALL_EN_MASK;
            }
            DCDC->REG6 = reg6;
#if gDCDC_PowerSwitchPinTiedUp_d == 0
        /* install ISR for DCDC */
        (void)NVIC_SetPriority(LVD_LVW_DCDC_IRQn, (uint32_t)((uint32_t)gDCDC_IsrPrio_c >> (8U - (uint8_t)__NVIC_PRIO_BITS)));
        OSA_InstallIntHandler((uint32_t)LVD_LVW_DCDC_IRQn, DCDC_PSwitchIsr);
        (void)NVIC_EnableIRQ(LVD_LVW_DCDC_IRQn);
#endif  /* gDCDC_PowerSwitchPinTiedUp_d */
        }
        else
        {
            /* Clear ISR status for DCDC */
            DCDC->REG6 &= ~(DCDC_REG6_PSWITCH_INT_RISE_EN_MASK | DCDC_REG6_PSWITCH_INT_FALL_EN_MASK);
        }
        status = TRUE;

    }
    else
    {
        /* Return false if mpDCDCConfig is NULL */
        status = FALSE;
    }
  return status;
#endif
}
