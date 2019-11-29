/*
* Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


/*!=================================================================================================
\file       app_led.c
\brief      This is a public source file for the application led configuration
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/
#include "app_stack_config.h"
#include "app_led.h"

#include "board.h"
#include "fsl_gpio.h"

/*==================================================================================================
Private macros
==================================================================================================*/
/* LED configuration */
#define LedCfg_Off(ledId)                                       ledId, 0, 0, 0, 0, 0
#define LedCfg_On(ledId)                                        ledId, (gLedSetup_On_c), 0, 0, 0, 0
#define LedCfg_Toggle(ledId, state)                             ledId, ((state)?0:gLedSetup_On_c), 0, 0, 0, 0
#define LedCfg_Chasing(ledId)                                   ledId, (gLedSetup_Chasing_c), 0, 0, 0, 0
#define LedCfg_PeriodChasing(ledId, flashPeriod)                ledId, (gLedSetup_Chasing_c|gLedSetup_FlashPeriod_c),  0, 0, 0, flashPeriod
#define LedCfg_Flashing(ledId)                                  ledId, (gLedSetup_On_c|gLedSetup_Flashing_c),0, 0, 0, 0
#define LedCfg_PeriodFlashing(ledId, flashPeriod)               ledId, (gLedSetup_Flashing_c|gLedSetup_FlashPeriod_c), 0, 0, 0, flashPeriod
#define LedCfg_NotUsed(ledId)                                   gLedId_Undefined_c, 0, 0, 0, 0, 0


#if gLedRgbEnabled_d
#define LedCfg_Red(ledId, rgb, rValue)                          ledId, (gLedSetup_Red_c)|(gLedSetup_Rgb_c & rgb),  rValue, 0, 0, 0
#define LedCfg_Green(ledId, rgb, gValue)                        ledId, (gLedSetup_Green_c)|(gLedSetup_Rgb_c & rgb),0, gValue, 0, 0
#define LedCfg_Blue(ledId, rgb, bValue)                         ledId, (gLedSetup_Blue_c)|(gLedSetup_Rgb_c & rgb), 0, 0, bValue, 0
#define LedCfg_RGB(ledId, rgb, rValue, gValue, bValue)          ledId, (gLedSetup_Red_c)|(gLedSetup_Green_c)|(gLedSetup_Blue_c)| (gLedSetup_Rgb_c & rgb), rValue, gValue, bValue, 0
#define LedCfg_RedFlashing(ledId, rgb, rValue)                  ledId, (gLedSetup_Flashing_c)|(gLedSetup_Red_c)|(gLedSetup_Rgb_c & rgb), rValue, 0, 0, 0
#define LedCfg_GreenFlashing(ledId, rgb, gValue)                ledId, (gLedSetup_Flashing_c)|(gLedSetup_Green_c)|(gLedSetup_Rgb_c & rgb), 0, gValue, 0, 0
#define LedCfg_BlueFlashing(ledId, rgb, bValue)                 ledId, (gLedSetup_Flashing_c)|(gLedSetup_Blue_c)| (gLedSetup_Rgb_c & rgb), 0, 0, bValue, 0
#define LedCfg_RgbFlashing(ledId, rgb, rValue, gValue, bValue)  ledId, (gLedSetup_Flashing_c)|(gLedSetup_Red_c)|(gLedSetup_Green_c)|(gLedSetup_Blue_c)| (gLedSetup_Rgb_c & rgb), rValue, gValue, bValue, 0
#define LedCfg_RedChasing(ledId, rgb, rValue)                   ledId, (gLedSetup_Chasing_c)|(gLedSetup_Red_c)| (gLedSetup_Rgb_c & rgb), rValue, 0, 0, 0
#define LedCfg_GreenChasing(ledId, rgb, gValue)                 ledId, (gLedSetup_Chasing_c)|(gLedSetup_Green_c)|(gLedSetup_Rgb_c & rgb), 0, gValue, 0, 0
#define LedCfg_BlueChasing(ledId, rgb, bValue)                  ledId, (gLedSetup_Chasing_c)|(gLedSetup_Blue_c)|(gLedSetup_Rgb_c & rgb),  0, 0, bValue, 0
#define LedCfg_ColorWheel(ledId, flashPeriod)                   ledId, (gLedSetupColorWheel_c|gLedSetup_FlashPeriod_c), 0, 0, 0, flashPeriod
#else /* gLedRgbEnabled_d */
#define LedCfg_Red(ledId, rgb, rValue)                          ledId, 0, 0, 0, 0, 0
#define LedCfg_Green(ledId, rgb, gValue)                        ledId, 0, 0, 0, 0, 0
#define LedCfg_Blue(ledId, rgb, bValue)                         ledId, 0, 0, 0, 0, 0
#define LedCfg_RGB(ledId, rgb, rValue, gValue, bValue)          ledId, 0, 0, 0, 0, 0
#define LedCfg_RedFlashing(ledId, rgb, rValue)                  ledId, 0, 0, 0, 0, 0
#define LedCfg_GreenFlashing(ledId, rgb, gValue)                ledId, 0, 0, 0, 0, 0
#define LedCfg_BlueFlashing(ledId, rgb, bValue)                 ledId, 0, 0, 0, 0, 0
#define LedCfg_RgbFlashing(ledId, rgb, rValue, gValue, bValue)  ledId, 0, 0, 0, 0, 0
#define LedCfg_RedChasing(ledId, rgb, rValue)                   ledId, 0, 0, 0, 0, 0
#define LedCfg_GreenChasing(ledId, rgb, gValue)                 ledId, 0, 0, 0, 0, 0
#define LedCfg_BlueChasing(ledId, rgb, bValue)                  ledId, 0, 0, 0, 0, 0
#define LedCfg_ColorWheel(ledId, flashPeriod)                   ledId, 0, 0, 0, 0, 0
#endif /* gLedRgbEnabled_d */

/*==================================================================================================
Private type definitions
==================================================================================================*/
/* application led configuration */
typedef struct appLedConfig_tag{
  uint8_t ledId;                        /* led identifier*/
  uint8_t ledSetup;                     /* bitmap */
  uint8_t redValue;                     /* red value*/
  uint8_t greenValue;                   /* green value*/
  uint8_t blueValue;                    /* blue value */
  uint16_t flashPeriod;                 /* flashing period */
}appLedConfig_t;

/* application leds configuration*/
typedef struct appLedsConfig_tag{
  appLedConfig_t ledConfig[gConfigLedsOnTargetBoard_c];
}appLedsConfig_t;


/* app led state */
typedef struct appLedState_tag{
  appDeviceState_t stateId;
  appLedsConfig_t *pLedsConfig;
}appLedState_t;


/*==================================================================================================
Private global variables declarations
==================================================================================================*/
#ifdef FRDM_KW24
/* FRDM_KW24 configuration mode - leds/state*/
const appLedsConfig_t gLedCfgFactoryDefault =       {LedCfg_BlueFlashing(LED_RGB, gLedSetup_Rgb_c, LED_MAX_RGB_VALUE_c), LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgJoiningOrAttaching =   {LedCfg_ColorWheel(LED_RGB, 50),                                     LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgNwkConnected =         {LedCfg_Off(LED_RGB),                                                LedCfg_Off(LED4)};
const appLedsConfig_t gLedCfgNwkOperationPending =  {LedCfg_Off(LED_RGB),                                                LedCfg_Flashing(LED4)};
#if !THREAD_ED_CONFIG
const appLedsConfig_t gLedCfgIsLeader =             {LedCfg_Green(LED_RGB, gLedSetup_Rgb_c, LED_MAX_RGB_VALUE_c),        LedCfg_On(LED4)};
const appLedsConfig_t gLedCfgIsActiveRouter =       {LedCfg_Off(LED_RGB),                                                LedCfg_On(LED4)};
#endif
/* FRDM_KW24 application mode - leds/state*/
const appLedsConfig_t gLedCfgAppLedOn =            {LedCfg_On(LED_RGB),                                                 LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedOff =           {LedCfg_Off(LED_RGB),                                                LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedToggle =        {LedCfg_Toggle(LED_RGB, gLedState_Undefined_c),                      LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedFlash =         {LedCfg_Flashing(LED_RGB),                                           LedCfg_NotUsed(LED4)};
/* app led RGB configuration -- should be in RAM - rValue, gValue, bValue could be changed by application */
appLedsConfig_t gLedCfgAppLedRgb =                 {LedCfg_RGB(LED_RGB, gLedSetup_Rgb_c, 0, 0, LED_MAX_RGB_VALUE_c),    LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedColorWheel =    {LedCfg_ColorWheel(LED_RGB, 200),         LedCfg_NotUsed(LED4)};

#elif defined(FRDM_KW41Z)

/* FRDM_KW41Z configuration mode - leds/state*/
const appLedsConfig_t gLedCfgFactoryDefault =       {LedCfg_BlueFlashing(LED_RGB, gLedSetup_Rgb_c, LED_MAX_RGB_VALUE_c), LedCfg_Flashing(LED1)};
const appLedsConfig_t gLedCfgJoiningOrAttaching =   {LedCfg_ColorWheel(LED_RGB, 50),                                     LedCfg_Flashing(LED1)};
const appLedsConfig_t gLedCfgNwkConnected =         {LedCfg_Off(LED_RGB),                                                LedCfg_Off(LED1)};
const appLedsConfig_t gLedCfgNwkOperationPending =  {LedCfg_Off(LED_RGB),                                                LedCfg_Flashing(LED1)};
#if !THREAD_ED_CONFIG
const appLedsConfig_t gLedCfgIsLeader =             {LedCfg_Green(LED_RGB, gLedSetup_Rgb_c, LED_MAX_RGB_VALUE_c),        LedCfg_On(LED1)};
const appLedsConfig_t gLedCfgIsActiveRouter =       {LedCfg_Off(LED_RGB),                                                LedCfg_On(LED1)};
#endif
/* FRDM_KW41Z application mode - leds/state*/
const appLedsConfig_t gLedCfgAppLedOn =            {LedCfg_On(LED_RGB),                                                 LedCfg_NotUsed(LED1)};
const appLedsConfig_t gLedCfgAppLedOff =           {LedCfg_Off(LED_RGB),                                                LedCfg_NotUsed(LED1)};
const appLedsConfig_t gLedCfgAppLedToggle =        {LedCfg_Toggle(LED_RGB, gLedState_Undefined_c),                      LedCfg_NotUsed(LED1)};
const appLedsConfig_t gLedCfgAppLedFlash =         {LedCfg_Flashing(LED_RGB),                                           LedCfg_NotUsed(LED1)};
/* app led RGB configuration -- should be in RAM - rValue, gValue, bValue could be changed by application */
appLedsConfig_t gLedCfgAppLedRgb =                 {LedCfg_RGB(LED_RGB, gLedSetup_Rgb_c, 0, 0, LED_MAX_RGB_VALUE_c),    LedCfg_NotUsed(LED1)};
const appLedsConfig_t gLedCfgAppLedColorWheel =    {LedCfg_ColorWheel(LED_RGB, 200),         LedCfg_NotUsed(LED1)};

#elif defined(FRDM_K64F)
/* FRDM_K64F configuration mode - leds/state*/
const appLedsConfig_t gLedCfgFactoryDefault =        {LedCfg_BlueFlashing(LED_RGB, 0, 0),                         LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgJoiningOrAttaching =    {LedCfg_ColorWheel(LED_RGB, gLedSetup_DefaultFlashPeriod_c), LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgNwkConnected =          {LedCfg_Off(LED_RGB),                                        LedCfg_Off(LED4)};
const appLedsConfig_t gLedCfgNwkOperationPending =   {LedCfg_Off(LED_RGB),                                        LedCfg_Flashing(LED4)};
#if !THREAD_ED_CONFIG
const appLedsConfig_t gLedCfgIsLeader =              {LedCfg_Green(LED_RGB, 0, 0),                                LedCfg_On(LED4)};
const appLedsConfig_t gLedCfgIsActiveRouter =        {LedCfg_NotUsed(LED_RGB),                                    LedCfg_On(LED4)};
#endif
/* FRDM_K64F application mode - leds/state*/
const appLedsConfig_t gLedCfgAppLedOn =            {LedCfg_On(LED_RGB),                                         LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedOff =           {LedCfg_Off(LED_RGB),                                        LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedToggle =        {LedCfg_Toggle(LED_RGB, gLedState_Undefined_c),              LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedFlash =         {LedCfg_Flashing(LED_RGB),                                   LedCfg_NotUsed(LED4)};
/* app led RGB configuration -- should be in RAM - rValue, gValue, bValue could be changed by application */
appLedsConfig_t gLedCfgAppLedRgb =                 {LedCfg_RGB(LED_RGB, gLedSetup_Rgb_c, 0, 0, LED_MAX_RGB_VALUE_c), LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedColorWheel =    {LedCfg_ColorWheel(LED_RGB, 200),                            LedCfg_NotUsed(LED4)};

#elif defined(FRDM_KL46Z)
/* FRDM_KL46Z configuration mode - leds/state*/
const appLedsConfig_t gLedCfgFactoryDefault =        {LedCfg_BlueFlashing(LED_RGB, 0, 0),                          LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgJoiningOrAttaching =    {LedCfg_ColorWheel(LED_RGB, 50),                              LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgNwkConnected =          {LedCfg_Off(LED_RGB),                                         LedCfg_Off(LED4)};
const appLedsConfig_t gLedCfgNwkOperationPending =   {LedCfg_Off(LED_RGB),                                         LedCfg_Flashing(LED4)};
#if !THREAD_ED_CONFIG
const appLedsConfig_t gLedCfgIsLeader =              {LedCfg_Green(LED_RGB, 0, 0),                                 LedCfg_On(LED4)};
const appLedsConfig_t gLedCfgIsActiveRouter =        {LedCfg_NotUsed(LED_RGB),                                     LedCfg_On(LED4)};
#endif
/* FRDM_KL46Z application mode - leds/state*/
const appLedsConfig_t gLedCfgAppLedOn =            {LedCfg_On(LED_RGB),                                          LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedOff =           {LedCfg_Off(LED_RGB),                                         LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedToggle =        {LedCfg_Toggle(LED_RGB, gLedState_Undefined_c),               LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedFlash =         {LedCfg_Flashing(LED_RGB),                                    LedCfg_NotUsed(LED4)};
/* app led RGB configuration -- should be in RAM - rValue, gValue, bValue could be changed by application */
appLedsConfig_t gLedCfgAppLedRgb =                 {LedCfg_RGB(LED_RGB, gLedSetup_Rgb_c, 0, 0, LED_MAX_RGB_VALUE_c),LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedColorWheel =    {LedCfg_ColorWheel(LED_RGB, 200),                             LedCfg_NotUsed(LED4)};

#elif defined(USB_KW24) || defined(USB_KW41)
/* USB-KW24D512Dongle configuration mode - leds/state*/
const appLedsConfig_t gLedCfgFactoryDefault =        {LedCfg_Flashing(LED1),              LedCfg_Flashing(LED2)};
const appLedsConfig_t gLedCfgJoiningOrAttaching =    {LedCfg_Chasing(LED1),               LedCfg_Chasing(LED2)};
const appLedsConfig_t gLedCfgNwkConnected =          {LedCfg_Off(LED1),                   LedCfg_Off(LED2)};
const appLedsConfig_t gLedCfgNwkOperationPending =   {LedCfg_Flashing(LED1),              LedCfg_Off(LED2)};
#if !THREAD_ED_CONFIG
const appLedsConfig_t gLedCfgIsLeader =              {LedCfg_On(LED1),                    LedCfg_Off(LED2)};
const appLedsConfig_t gLedCfgIsActiveRouter =        {LedCfg_On(LED1),                    LedCfg_Off(LED2)};
#endif
/* USB-KW24D512Dongle application mode - leds/state*/
const appLedsConfig_t gLedCfgAppLedOn =            {LedCfg_NotUsed(LED1),               LedCfg_On(LED2)};
const appLedsConfig_t gLedCfgAppLedOff =           {LedCfg_NotUsed(LED1),               LedCfg_Off(LED2)};
const appLedsConfig_t gLedCfgAppLedToggle =        {LedCfg_NotUsed(LED1),               LedCfg_Toggle(LED2, gLedState_Undefined_c)};
const appLedsConfig_t gLedCfgAppLedFlash =         {LedCfg_NotUsed(LED1),               LedCfg_Flashing(LED2)};
#else
/* TWR-KW24D512, TWR-KW21D256 */
/* configuration mode - leds/state*/
const appLedsConfig_t gLedCfgFactoryDefault =        {LedCfg_Flashing(LED1),        LedCfg_Flashing(LED2),            LedCfg_Flashing(LED3),            LedCfg_Flashing(LED4)};
const appLedsConfig_t gLedCfgJoiningOrAttaching =    {LedCfg_Chasing(LED1),         LedCfg_Chasing(LED2),             LedCfg_Chasing(LED3),             LedCfg_Chasing(LED4)};
const appLedsConfig_t gLedCfgNwkConnected =          {LedCfg_Off(LED1),             LedCfg_Off(LED2),                 LedCfg_Off(LED3),                 LedCfg_Off(LED4)};
const appLedsConfig_t gLedCfgNwkOperationPending =   {LedCfg_Off(LED1),             LedCfg_Off(LED2),                 LedCfg_Off(LED3),                 LedCfg_Flashing(LED4)};
#if !THREAD_ED_CONFIG
const appLedsConfig_t gLedCfgIsLeader =              {LedCfg_On(LED1),              LedCfg_NotUsed(LED2),             LedCfg_NotUsed(LED3),             LedCfg_On(LED4)};
const appLedsConfig_t gLedCfgIsActiveRouter =        {LedCfg_Off(LED1),             LedCfg_Off(LED2),                 LedCfg_Off(LED3),                 LedCfg_On(LED4)};
#endif
/* application mode - leds/state*/
const appLedsConfig_t gLedCfgAppLedOn =            {LedCfg_NotUsed(LED1),         LedCfg_NotUsed(LED2),             LedCfg_On(LED3),                  LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedOff =           {LedCfg_NotUsed(LED1),         LedCfg_NotUsed(LED2),             LedCfg_Off(LED3),                 LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedToggle =        {LedCfg_NotUsed(LED1),         LedCfg_NotUsed(LED2),             LedCfg_Toggle(LED3, gLedState_Undefined_c),LedCfg_NotUsed(LED4)};
const appLedsConfig_t gLedCfgAppLedFlash =         {LedCfg_NotUsed(LED1),         LedCfg_NotUsed(LED2),             LedCfg_Flashing(LED3),            LedCfg_NotUsed(LED4)};

#endif /* FRDM_K64F */


const appLedState_t gLedConfigState[] = {
  {gDeviceState_FactoryDefault_c,        (appLedsConfig_t *)&gLedCfgFactoryDefault},
  {gDeviceState_JoiningOrAttaching_c,    (appLedsConfig_t *)&gLedCfgJoiningOrAttaching},
  {gDeviceState_NwkOperationPending_c,   (appLedsConfig_t *)&gLedCfgNwkOperationPending},
#if !THREAD_ED_CONFIG
  {gDeviceState_Leader_c,                (appLedsConfig_t *)&gLedCfgIsLeader},
  {gDeviceState_ActiveRouter_c,          (appLedsConfig_t *)&gLedCfgIsActiveRouter},
#endif
  {gDeviceState_NwkConnected_c,          (appLedsConfig_t *)&gLedCfgNwkConnected},
  {gDeviceState_NwkFailure_c,            (appLedsConfig_t *)&gLedCfgNwkOperationPending}
};

const appLedState_t gLedAppState[] = {
  {gDeviceState_AppLedOn_c,              (appLedsConfig_t *)&gLedCfgAppLedOn},
  {gDeviceState_AppLedOff_c,             (appLedsConfig_t *)&gLedCfgAppLedOff},
  {gDeviceState_AppLedToggle_c,          (appLedsConfig_t *)&gLedCfgAppLedToggle},
  {gDeviceState_AppLedFlash_c,           (appLedsConfig_t *)&gLedCfgAppLedFlash},
#if gLedRgbEnabled_d
  {gDeviceState_AppLedRgb_c,             (appLedsConfig_t *)&gLedCfgAppLedRgb},
  {gDeviceState_AppLedColorWheel_c,      (appLedsConfig_t *)&gLedCfgAppLedColorWheel},
#endif
#if !THREAD_ED_CONFIG
  {gDeviceState_Leader_c,                (appLedsConfig_t *)&gLedCfgIsLeader},
  {gDeviceState_ActiveRouter_c,          (appLedsConfig_t *)&gLedCfgIsActiveRouter},
#endif
  {gDeviceState_JoiningOrAttaching_c,    (appLedsConfig_t *)&gLedCfgJoiningOrAttaching}
};

/*==================================================================================================
Public global variables declarations
==================================================================================================*/
#if gLEDSupported_d
/* 802.15.4 - LED tx activity variables:*/
bool_t gEnable802154TxLed = FALSE;
bool_t gEnableTxLed = FALSE;
bool_t routerLedState = FALSE;
#endif

/*==================================================================================================
Private function prototypes
==================================================================================================*/
#if gLEDSupported_d
static appLedState_t* LedState_GetEntry(appDeviceMode_t devModeId, appDeviceState_t devStateId);
static void LedConfig_SetState(appLedState_t *pStateLedCfg);
#endif
/*==================================================================================================
Public functions
==================================================================================================*/
#if gLEDSupported_d
/*!*************************************************************************************************
\public
\fn  void Led_SetState(appDeviceMode_t devModeId, appDeviceState_t devStateId)
\brief set LEDs based on device modeId/stateId

\param [in]   devModeId                 device mode id
\param [in]   devStateId                device state id
 ***************************************************************************************************/
void Led_SetState(appDeviceMode_t devModeId, appDeviceState_t devStateId)
{
    /* configure leds */
    LedConfig_SetState(LedState_GetEntry(devModeId, devStateId));
}
#endif /* gLEDSupported_d */

#if gLedRgbEnabled_d
/*!*************************************************************************************************
\public
\fn  void Led_UpdateRgbState(uint8_t redValue, uint8_t greenValue, uint8_t blueValue)
\brief set RGB LEDs based on input values for red, green and blue

\param [in]    redValue      Red value
\param [in]    greenValue    Green value
\param [in]    blueValue     Blue value
 ***************************************************************************************************/
void Led_UpdateRgbState
(
    uint8_t redValue,
    uint8_t greenValue,
    uint8_t blueValue
)
{
    /* update rgb values */
    for(uint8_t i=0; i< gConfigLedsOnTargetBoard_c; i++)
    {
        if(gLedCfgAppLedRgb.ledConfig[i].ledId == LED_RGB)
        {
            gLedCfgAppLedRgb.ledConfig[i].redValue = redValue;
            gLedCfgAppLedRgb.ledConfig[i].greenValue = greenValue;
            gLedCfgAppLedRgb.ledConfig[i].blueValue = blueValue;
        }
    }
}
#endif

#if gLEDSupported_d
/*!*************************************************************************************************
\fn     void Led_MacTxOn(void)
\brief  Function used to set LED On (80215.4 tx activity)
***************************************************************************************************/
void Led_MacTxOn
(
    void
)
{
    OSA_InterruptDisable();
    if(gEnable802154TxLed && (!gEnableTxLed))
    {
        gEnableTxLed = TRUE;
        if(routerLedState)
        {
            AppTxLedActivityOff();
        }
        else
        {
            AppTxLedActivityOn();
        }
    }
    OSA_InterruptEnable();
}

/*!*************************************************************************************************
\fn     void Led_MacTxOff(void)
\brief  Function used to set LED Off (80215.4 tx activity)
***************************************************************************************************/
void Led_MacTxOff
(
    void
)
{
    OSA_InterruptDisable();
    if(gEnable802154TxLed || gEnableTxLed)
    {
        if(routerLedState)
        {
            AppTxLedActivityOn();
        }
        else
        {
            AppTxLedActivityOff();
        }
        gEnableTxLed = FALSE;
    }
    OSA_InterruptEnable();
}
#endif /* gLEDSupported_d */

/*!*************************************************************************************************
\fn     void Led_Identify(bool_t state)
\brief  This function is used to start the board identification by flashing the TX LED.

\param  [in]    state      Variable used to toggle the identification process
***************************************************************************************************/
void Led_Identify
(
    bool_t state
)
{
#if gLEDSupported_d
    if(state == TRUE)
    {
        Led_PeriodFlash(LED_TX_ACTIVITY, 500);
    }
    else
    {
        Led_TurnOff(LED_TX_ACTIVITY);
    }
#endif
}

#if gLEDSupported_d
/*==================================================================================================
Private functions
==================================================================================================*/

/*!*************************************************************************************************
\private
\fn static appLedState_t* LedState_GetEntry(appDeviceState_t devStateId)
\brief Get led configuration entry

\param [in]   devModeId                 device mode identifier
       [in]   devStateId                device state identifier

\return       appLedState_t             pointer to led state structure
 ***************************************************************************************************/
static appLedState_t* LedState_GetEntry
(
    appDeviceMode_t devModeId,
    appDeviceState_t devStateId
)
{
    appLedState_t *pResult = NULL;
    appLedState_t *pLedState = (appLedState_t *)&gLedConfigState[0];
    uint8_t noElements = NumberOfElements(gLedConfigState);

    if(devModeId == gDeviceMode_Application_c)
    {
        noElements = NumberOfElements(gLedAppState);
        pLedState = (appLedState_t *)&gLedAppState[0];
    }

    for(uint8_t i = 0; i< noElements; i++)
    {
        if(devStateId == pLedState[i].stateId)
        {
            pResult = (appLedState_t *)&pLedState[i];
            break;
        }
    }

    return pResult;
}

/*!*************************************************************************************************
\private
\fn static void LedConfig_SetState(appLedState_t *pStateLedCfg)
\brief Set led configuration

\param [in]   pStateLedCfg     pointer to led configuration
 ***************************************************************************************************/
static void LedConfig_SetState
(
    appLedState_t *pStateLedCfg
)
{
    uint8_t i;
    uint8_t startLedChasing = gLedId_Undefined_c;
    uint16_t flashInterval = gLedSetup_DefaultFlashPeriod_c;
#if gLedRgbEnabled_d
    uint8_t redValue = 0, greenValue = 0, blueValue = 0;
    bool_t bSetRgb = FALSE;
#endif
    if(!pStateLedCfg)
    {
        return;
    }

    for(i=0; i< gConfigLedsOnTargetBoard_c; i++)
    {
        /* validate LedId */
        if(pStateLedCfg->pLedsConfig->ledConfig[i].ledId != gLedId_Undefined_c)
        {
            /* application toggle state */
            if(pStateLedCfg->stateId == gDeviceState_AppLedToggle_c)
            {
                /* toggle the led */
                Led_Toggle(pStateLedCfg->pLedsConfig->ledConfig[i].ledId);
            }
            else
            {
                /* on/off led */
                if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_On_c)
                {
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledId == LED_TX_ACTIVITY)
                    {
                        routerLedState = TRUE;
                    }
                    /* turn On the led */
                    Led_TurnOn(pStateLedCfg->pLedsConfig->ledConfig[i].ledId);
                }
                else
                {
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledId == LED_TX_ACTIVITY)
                    {
                        routerLedState = FALSE;
                    }
                    /* turn off the led */
                    Led_TurnOff(pStateLedCfg->pLedsConfig->ledConfig[i].ledId);
                }
 #if gLedRgbEnabled_d
                /* should be Red? */
                if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Red_c)
                {
                    redValue = LED_MAX_RGB_VALUE_c;
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Rgb_c)
                    {
                        redValue = pStateLedCfg->pLedsConfig->ledConfig[i].redValue;
                    }
                    bSetRgb = TRUE;
                }
                 /* should be Green? */
                if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Green_c)
                {
                    greenValue = LED_MAX_RGB_VALUE_c;
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Rgb_c)
                    {
                        greenValue = pStateLedCfg->pLedsConfig->ledConfig[i].greenValue;
                    }
                    bSetRgb = TRUE;
                }
                /* should be Blue? */
                if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Blue_c)
                {
                    blueValue = LED_MAX_RGB_VALUE_c;
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Rgb_c)
                    {
                        blueValue = pStateLedCfg->pLedsConfig->ledConfig[i].blueValue;
                    }
                    bSetRgb = TRUE;
                }
                /* set rgb led */
                if(bSetRgb)
                {
                     Led_RGB(pStateLedCfg->pLedsConfig->ledConfig[i].ledId, redValue, greenValue, blueValue);
                     bSetRgb = FALSE;
                }

 #endif
 #if gLedRgbEnabled_d
                /* color wheel? */
                if((pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetupColorWheel_c) ==
                   gLedSetupColorWheel_c)
                {
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_FlashPeriod_c)
                    {
                        flashInterval = pStateLedCfg->pLedsConfig->ledConfig[i].flashPeriod;
                    }
                    LED_ColorWheel(pStateLedCfg->pLedsConfig->ledConfig[i].ledId, flashInterval);
                }
                else
#endif
                {
                    /* flashing? */
                    if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Flashing_c)
                    {
                        if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_FlashPeriod_c)
                        {
                            flashInterval = pStateLedCfg->pLedsConfig->ledConfig[i].flashPeriod;
                        }
                        Led_PeriodFlash(pStateLedCfg->pLedsConfig->ledConfig[i].ledId, flashInterval);
                    }
                    /* chasing? */
                    else if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_Chasing_c)
                    {
                        if(startLedChasing > pStateLedCfg->pLedsConfig->ledConfig[i].ledId )
                        {
                            startLedChasing = pStateLedCfg->pLedsConfig->ledConfig[i].ledId;
                            if(pStateLedCfg->pLedsConfig->ledConfig[i].ledSetup & gLedSetup_FlashPeriod_c)
                            {
                                flashInterval = pStateLedCfg->pLedsConfig->ledConfig[i].flashPeriod;
                            }
                        }
                    }
                }
            }
        }
    }

    if(startLedChasing != gLedId_Undefined_c)
    {
        Led_Chasing(startLedChasing, flashInterval);
    }
}
#endif /* gLEDSupported_d */
/*==================================================================================================
Private debug functions
==================================================================================================*/
