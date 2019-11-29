/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*!=============================================================================
\file       DriverBulb_KW41z.c
\brief      Driver Bulb for KW41z
==============================================================================*/

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/

/* SDK includes */
#include <jendefs.h>

#include "fsl_debug_console.h"
#include "LED.h"

#if !gLedRgbEnabled_d
#warning "This driver requires the RGB LED to be enabled"
#endif

/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/
#define gKelvinMin_c        1000
#define gKelvinMax_c        40000
#define gDBG_TRACE_DRIVER_BULB_d    0


/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/
typedef struct
{
  uint8_t red;
  uint8_t green;
  uint8_t blue;
} tsRGBVal;

typedef struct
{
  tsRGBVal color;
  uint32_t level;
  uint32_t temperature;
} tsLEDControl;

typedef struct
{
  uint32 tmpKelvin;
  tsRGBVal RGBVal;
} tsKelvinTable;

/****************************************************************************/
/***        Local Function Prototypes                                     ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       		bGetRgbFromKelvin
 *
 * DESCRIPTION:		Returns the RGB color code for a given color temperature
 *
 ****************************************************************************/
#if defined(ColorTemperatureLight) || defined(ExtendedColorLight)
static bool_t bGetRgbFromKelvin(uint32_t tmpKelvin, tsRGBVal* rgbValue);
#endif

/****************************************************************************
 *
 * NAME:       		vUpdateBulbDriver
 *
 * DESCRIPTION:		Updates the RGB LED values
 *
 ****************************************************************************/
static void vUpdateBulbDriver(void);

/****************************************************************************/
/***        Exported Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Global Variables                                              ***/
/****************************************************************************/

/****************************************************************************/
/***        Local Variables                                               ***/
/****************************************************************************/
static bool_t bBulbOn = FALSE;
static bool_t bBulbInitialized = FALSE;
static volatile tsLEDControl mLedControl;

#if defined(ColorTemperatureLight) || defined(ExtendedColorLight)
static const tsKelvinTable mKelvinTable[] =
{
    {40000, {155, 188, 255}},
    {39500, {155, 188, 255}},
    {39000, {155, 188, 255}},
    {38500, {155, 188, 255}},
    {38000, {156, 188, 255}},
    {37500, {156, 188, 255}},
    {37000, {156, 189, 255}},
    {36500, {156, 189, 255}},
    {36000, {156, 189, 255}},
    {35500, {157, 189, 255}},
    {35000, {157, 189, 255}},
    {34500, {157, 189, 255}},
    {34000, {157, 189, 255}},
    {33500, {157, 189, 255}},
    {33000, {158, 190, 255}},
    {32500, {158, 190, 255}},
    {32000, {158, 190, 255}},
    {31500, {158, 190, 255}},
    {31000, {159, 190, 255}},
    {30500, {159, 190, 255}},
    {30000, {159, 191, 255}},
    {29500, {159, 191, 255}},
    {29000, {160, 191, 255}},
    {28500, {160, 191, 255}},
    {28000, {160, 191, 255}},
    {27500, {161, 192, 255}},
    {27000, {161, 192, 255}},
    {26500, {161, 192, 255}},
    {26000, {162, 192, 255}},
    {25500, {162, 193, 255}},
    {25000, {163, 193, 255}},
    {24500, {163, 193, 255}},
    {24000, {163, 194, 255}},
    {23500, {164, 194, 255}},
    {23000, {164, 194, 255}},
    {22500, {165, 195, 255}},
    {22000, {166, 195, 255}},
    {21500, {166, 195, 255}},
    {21000, {167, 196, 255}},
    {20500, {168, 196, 255}},
    {20000, {168, 197, 255}},
    {19500, {169, 197, 255}},
    {19000, {170, 198, 255}},
    {18500, {171, 198, 255}},
    {18000, {172, 199, 255}},
    {17500, {173, 200, 255}},
    {17000, {174, 200, 255}},
    {16500, {175, 201, 255}},
    {16000, {176, 202, 255}},
    {15500, {177, 203, 255}},
    {15000, {179, 204, 255}},
    {14500, {180, 205, 255}},
    {14000, {182, 206, 255}},
    {13500, {184, 207, 255}},
    {13000, {186, 208, 255}},
    {12500, {188, 210, 255}},
    {12000, {191, 211, 255}},
    {11500, {193, 213, 255}},
    {11000, {196, 215, 255}},
    {10500, {200, 217, 255}},
    {10000, {204, 219, 255}},
    { 9500, {208, 222, 255}},
    { 9000, {214, 225, 255}},
    { 8500, {220, 229, 255}},
    { 8000, {227, 233, 255}},
    { 7500, {235, 238, 255}},
    { 7000, {245, 243, 255}},
    { 6500, {255, 249, 253}},
    { 6000, {255, 243, 239}},
    { 5500, {255, 236, 224}},
    { 5000, {255, 228, 206}},
    { 4500, {255, 219, 186}},
    { 4000, {255, 207, 161}},
    { 3500, {255, 196, 137}},
    { 3000, {255, 180, 107}},
    { 2500, {255, 161,  72}},
    { 2000, {255, 137,  18}},
    { 1500, {255, 109,   0}},
    { 1000, {255,  51,   0}}
};
#endif

/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       		bGetRgbFromKelvin
 *
 * DESCRIPTION:		Returns the RGB color code for a given color temperature
 *
 ****************************************************************************/
#if defined(ColorTemperatureLight) || defined(ExtendedColorLight)
static bool_t bGetRgbFromKelvin(uint32_t tmpKelvin, tsRGBVal* rgbValue)
{
  uint8_t loopCnt;

  if((tmpKelvin < gKelvinMin_c) || (tmpKelvin > gKelvinMax_c))
  {
    return FALSE;
  }

  for(loopCnt = 0; loopCnt < sizeof(mKelvinTable)/sizeof(mKelvinTable[0]); loopCnt++)
  {
    if(tmpKelvin >= mKelvinTable[loopCnt].tmpKelvin)
    {
      rgbValue->red = mKelvinTable[loopCnt].RGBVal.red;
      rgbValue->green = mKelvinTable[loopCnt].RGBVal.green;
      rgbValue->blue = mKelvinTable[loopCnt].RGBVal.blue;
      break;
    }
  }
  return TRUE;
}
#endif

/****************************************************************************
 *
 * NAME:       		vUpdateBulbDriver
 *
 * DESCRIPTION:		Updates the RGB LED values
 *
 ****************************************************************************/
static void vUpdateBulbDriver(void)
{
  
#if gDBG_TRACE_DRIVER_BULB_d
  PRINTF("vUpdateBulbDriver\n");        
#endif
        
  uint8_t u8Red;
  uint8_t u8Green;
  uint8_t u8Blue;

  if(bBulbOn)
  {
    /* Scale colour for brightness level */
    u8Red   = (uint8)(((uint32)mLedControl.color.red   * (uint32)mLedControl.level) / (uint32)(LED_MAX_RGB_VALUE_c));
    u8Green = (uint8)(((uint32)mLedControl.color.green * (uint32)mLedControl.level) / (uint32)(LED_MAX_RGB_VALUE_c));
    u8Blue  = (uint8)(((uint32)mLedControl.color.blue  * (uint32)mLedControl.level) / (uint32)(LED_MAX_RGB_VALUE_c));

        /* Don't allow fully OFF */
        if ((u8Red   <= 2) && (u8Green <= 2) && (u8Blue  <= 2))
        {
            u8Red   = 3;
            u8Green = 3;
            u8Blue  = 3;
        }
  }
  else /* Turn OFF */
  {
    u8Red   = 0;
    u8Green = 0;
    u8Blue  = 0;
  }

  /* Set the new RGB values */
  LED_SetRgbLed(LED_RGB, u8Red, u8Green, u8Blue);
}

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

/****************************************************************************
 *
 * NAME:       		DriverBulb_vInit
 *
 * DESCRIPTION:		Initialize the Driver Bulb
 *
 ****************************************************************************/
 void DriverBulb_vInit(void)
{
  if(!bBulbInitialized)
  {
#if gDBG_TRACE_DRIVER_BULB_d
    PRINTF("DriverBulb_vInit\n");
#endif        
    /* Initialize the level and color temperature variables */
    mLedControl.level = mLedControl.temperature = LED_MAX_RGB_VALUE_c;
    /* Initialize the RGB colors */
    mLedControl.color.red = mLedControl.color.blue = mLedControl.color.green = LED_MAX_RGB_VALUE_c;
    /* Set bulb driver output */
    vUpdateBulbDriver();
    /* Bulb state is now ON */
    bBulbOn = TRUE;        
    /* Initialization completed */
    bBulbInitialized = TRUE;        
  }
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_vSetOnOff
 *
 * DESCRIPTION:		Set the ON/OFF state of the Driver Bulb
 *
 ****************************************************************************/
 void DriverBulb_vSetOnOff(bool_t bOn)
{
#if gDBG_TRACE_DRIVER_BULB_d
  PRINTF("DriverBulb_vSetOnOff: %d\n", bOn);
#endif
  bBulbOn = bOn;
  vUpdateBulbDriver();
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_vSetLevel
 *
 * DESCRIPTION:		Set the level value of the Driver Bulb
 *
 ****************************************************************************/
 void DriverBulb_vSetLevel(uint32 u32Level)
{
#if gDBG_TRACE_DRIVER_BULB_d
  PRINTF("DriverBulb_vSetLevel u32Level:%d\n", u32Level);
#endif

  /* return if same level */
  if(mLedControl.level == u32Level)
  {
    return;
  }

  /* copy the new level */
  mLedControl.level = (uint8) MAX(1, u32Level);

  /* is the bulb on? Setting the level should not switch on a bulb that is off */
  if(bBulbOn)
  {
    /* Update bulb driver output */
    vUpdateBulbDriver();
  }
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_vSetTunableWhiteColourTemperature
 *
 * DESCRIPTION:		Set the color temperature value of the Driver Bulb
 *
 ****************************************************************************/
#if defined(ColorTemperatureLight) || defined(ExtendedColorLight)
 void DriverBulb_vSetTunableWhiteColourTemperature(uint32 u32ColTemp)
{
#if gDBG_TRACE_DRIVER_BULB_d
  PRINTF("DriverBulb_vSetTunableWhiteColourTemperature u32ColTemp:%d\n", u32ColTemp);
#endif

  /* return if same colour temperature */
  if(mLedControl.temperature == u32ColTemp)
  {
    return;
  }

  /* is the bulb on? Setting the colour should not work on a bulb that is off */
  if(bBulbOn)
  {
    mLedControl.temperature = u32ColTemp;
    if(bGetRgbFromKelvin((mLedControl.temperature), (tsRGBVal*)&mLedControl.color))
    {
      /* Update bulb driver output */
      vUpdateBulbDriver();
    }
  }
}
#endif

/****************************************************************************
 *
 * NAME:       		DriverBulb_vSetColour
 *
 * DESCRIPTION:		Set the colour value of the Driver Bulb
 *
 ****************************************************************************/
#if defined(ExtendedColorLight)
void DriverBulb_vSetColour(uint32 u32Red, uint32 u32Green, uint32 u32Blue)
{
#if gDBG_TRACE_DRIVER_BULB_d
  PRINTF("DriverBulb_vSetColour: %d %d %d\n", u32Red, u32Green, u32Blue);
#endif
  
  /* return if same colour */
  if((mLedControl.color.red == (uint8)u32Red) && (mLedControl.color.green == (uint8)u32Green) && (mLedControl.color.blue == (uint8)u32Blue))
  {
    return;
  }

  /* is the bulb on? Setting the colour should not work on a bulb that is off */
  if(bBulbOn)
  {
    /* copy the new color */
    mLedControl.color.red= u32Red;
    mLedControl.color.green= u32Green;
    mLedControl.color.blue= u32Blue;
    
    /* Update bulb driver output */
    vUpdateBulbDriver();
  }
}
#endif

/****************************************************************************
 *
 * NAME:       		DriverBulb_bOn
 *
 * DESCRIPTION:		Return the bulb ON/OFF state
 *
 ****************************************************************************/
bool_t DriverBulb_bOn(void)
{
  return (bBulbOn);
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_bReady
 *
 * DESCRIPTION:		Check if the bulb is ready to operate
 *
 ****************************************************************************/
bool_t DriverBulb_bReady(void)
{
  return (bBulbInitialized);
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_bFailed
 *
 * DESCRIPTION:		Check if the bulb has failures (unimplemented)
 *
 ****************************************************************************/
bool_t DriverBulb_bFailed(void)
{
  return (FALSE);
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_vTick
 *
 * DESCRIPTION:		N/A
 *
 ****************************************************************************/
void DriverBulb_vTick(void)
{
  /* unimplemented */
}

/****************************************************************************
 *
 * NAME:       		DriverBulb_i16Analogue
 *
 * DESCRIPTION:		N/A
 *
 ****************************************************************************/
int16 DriverBulb_i16Analogue(uint8 u8Adc, uint16 u16AdcRead)
{
	return 0;
}

/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
