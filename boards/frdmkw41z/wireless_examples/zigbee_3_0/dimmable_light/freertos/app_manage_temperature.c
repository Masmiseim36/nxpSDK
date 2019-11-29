/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       app_manage_temperature.c
\brief      Set of functions/task for temperature related behaviour
==============================================================================*/

/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <jendefs.h>


#include "app_manage_temperature.h"
#include "DriverBulb.h"
#include "dbg.h"

#include "ZTimer.h"
#include "app_main.h"

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#define TEMP_XTAL_HALF_PULL             95  /*  95C */
#define TEMP_XTAL_HALF_PUSH             93  /*  93C */
#define TEMP_XTAL_FULL_PULL            110  /* 110C */
#define TEMP_XTAL_FULL_PUSH            108  /* 108C */

#define ADC_REG                        0x02001f04


#define TEMPERATURE_DECIMATION_FACTOR  10

#ifdef DEBUG_TEMPERATURE
#define TRACE_APP_TEMPERATURE               TRUE
#else
#define TRACE_APP_TEMPERATURE                FALSE
#endif

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef enum
{
    E_STATE_XTAL_UNPULLED = 0,
    E_STATE_XTAL_SEMIPULLED = 1,
    E_STATE_XTAL_PULLED = 3
} teXtalPullingStates;


/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/
#if 0
PRIVATE void vPullXtal(int32 i32Temperature);
PRIVATE int16 i16GetChipTemp(uint16 u16AdcValue);
#endif
/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/
/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void APP_vManageTemperatureInit(void)
{
    /* set up the ADCS on platforms that dont already use them */
#if (defined DR1175) || (defined DR1173) || (defined DR1190)
	// todo
    /* Set up the analogue peripherals ready to handle the conversions */
    vAHI_ApConfigure(E_AHI_AP_REGULATOR_ENABLE,
    E_AHI_AP_INT_DISABLE,
    E_AHI_AP_SAMPLE_8,
    E_AHI_AP_CLOCKDIV_500KHZ,
    E_AHI_AP_INTREF);

    while (!bAHI_APRegulatorEnabled());   /* spin on reg not enabled */
#endif

    DBG_vPrintf(TRACE_APP_TEMPERATURE, "\nAPP: InitManageTemperature");
}

/****************************************************************************
*
* NAME: APP_cbTimerRadioRecal
*
* DESCRIPTION:
* Calibrates the radio every minute or defer for 10 seconds if MAC busy
*
****************************************************************************/
PUBLIC void APP_cbTimerRadioRecal(void *pvParam)
{
    DBG_vPrintf(TRACE_APP_TEMPERATURE,"\nAPP: Recal Radio ");
#if 0
    if (eAHI_AttemptCalibration() == E_CAL_SUCCESS)
    {
        ZTIMER_eStart(u8TimerRadioRecal, ZTIMER_TIME_SEC(60));
        DBG_vPrintf(TRACE_APP_TEMPERATURE,"OK");
    }
    else
    {
        ZTIMER_eStart(u8TimerRadioRecal, ZTIMER_TIME_MSEC(10));
        DBG_vPrintf(TRACE_APP_TEMPERATURE,"deferred (MAC busy)");
    }
#endif
}

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/
#if 0
/****************************************************************************/
/* NAME: vPullXtal                                                          */
/*                                                                          */
/* DESCRIPTION:                                                             */
/* Oscillator pulling State machine                                         */
/****************************************************************************/
PRIVATE void vPullXtal(int32 i32Temperature)
{

    static teXtalPullingStates eXtalPullingState = E_STATE_XTAL_UNPULLED;

    DBG_vPrintf(TRACE_APP_TEMPERATURE, "\nAPP: T =%d C",i32Temperature);

    switch (eXtalPullingState)
    {
        case  E_STATE_XTAL_UNPULLED :
            if (i32Temperature >= TEMP_XTAL_HALF_PULL)
            {
                DBG_vPrintf(TRACE_APP_TEMPERATURE, "\nAPP: Xtal 1/2 pulled");
                eXtalPullingState = E_STATE_XTAL_SEMIPULLED;
                vAHI_ClockXtalPull(eXtalPullingState);
            }
            break;

        case  E_STATE_XTAL_SEMIPULLED :
            if (i32Temperature >= TEMP_XTAL_FULL_PULL)
            {
                DBG_vPrintf(TRACE_APP_TEMPERATURE, "\nAPP: Xtal full pulled");
                eXtalPullingState = E_STATE_XTAL_PULLED;
                vAHI_ClockXtalPull(eXtalPullingState);
            }
            else if (i32Temperature < TEMP_XTAL_HALF_PUSH)
            {
                DBG_vPrintf(TRACE_APP_TEMPERATURE, "\nAPP: Xtal not pulled");
                eXtalPullingState = E_STATE_XTAL_UNPULLED;
                vAHI_ClockXtalPull(eXtalPullingState);
            }
            break;

        case  E_STATE_XTAL_PULLED :
            if (i32Temperature < TEMP_XTAL_FULL_PUSH)
            {
                DBG_vPrintf(TRACE_APP_TEMPERATURE, "\nAPP: Xtal 1/2 pulled");
                eXtalPullingState = E_STATE_XTAL_SEMIPULLED;
                vAHI_ClockXtalPull(eXtalPullingState);
            }
            break;

        default :
        break;
    }

}
#endif
/****************************************************************************/
/* NAME: APP_vManageTemperatureTick                                         */
/*                                                                          */
/* DESCRIPTION:                                                             */
/* uses the tick count to schedule the adc conversions                      */
/****************************************************************************/

PUBLIC void APP_vManageTemperatureTick(uint32 u32Ticks)
{
#if 0
    static uint32 u32DecimationCounter = TEMPERATURE_DECIMATION_FACTOR;

    uint16 u16AdcTempSensor;
    int32 i32ChipTemperature;
    /* every second interleave a temperature adc conversion start */
    if (u32Ticks == 90)
    {
        vAHI_AdcEnable(E_AHI_ADC_SINGLE_SHOT,E_AHI_AP_INPUT_RANGE_2,E_AHI_ADC_SRC_TEMP);
        vAHI_AdcStartSample();
    }

    /*...and read it 5 ms later */
    if (u32Ticks == 95)
    {
        /* only take every reading every 10 seconds */
        if (--u32DecimationCounter == 0)
        {
            u32DecimationCounter = TEMPERATURE_DECIMATION_FACTOR;

            if (DriverBulb_u16GetAdcValue)
            {
                u16AdcTempSensor = DriverBulb_u16GetAdcValue(4);
            }
            else
            {
                u16AdcTempSensor = u16AHI_AdcRead();
            }

            i32ChipTemperature = i16GetChipTemp(u16AdcTempSensor);
            vPullXtal(i32ChipTemperature);
        }
    }
#endif
}

/****************************************************************************/
/* NAME: i16GetChipTemp                                                     */
/*                                                                          */
/* DESCRIPTION:                                                             */
/* Helper Function to convert 10bit ADC reading to degrees C                */
/* Formula: DegC = Typical DegC - ((Reading12 - Typ12) * ScaleFactor)       */
/* Where C = 25 and temps sensor output 730mv at 25C (from datasheet)       */
/* As we use 2Vref and 10bit adc this gives (730/2400)*4096  [=Typ12 =1210] */
/* Scale factor is half the 0.706 data-sheet resolution DegC/LSB (2Vref)    */
/*                                                                          */
/* RETURNS: Chip Temperature in DegC                                        */
/****************************************************************************/
#if 0
PRIVATE int16 i16GetChipTemp(uint16 u16AdcValue)
{
    int16 i16Centigrade;

    i16Centigrade = (int16) ((int32) 25 - ((((int32) (u16AdcValue*4) - (int32) 1210) * (int32) 353) / (int32) 1000));

    return (i16Centigrade);
}
#endif
/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
