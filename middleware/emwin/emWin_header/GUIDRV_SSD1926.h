/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2019  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.50 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  NXP Semiconductors USA, Inc.  whose
registered  office  is  situated  at 411 E. Plumeria Drive, San  Jose,
CA 95134, USA  solely for  the  purposes  of  creating  libraries  for
NXPs M0, M3/M4 and  ARM7/9 processor-based  devices,  sublicensed  and
distributed under the terms and conditions of the NXP End User License
Agreement.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Microcontroller Systems LLC
Licensed to:              NXP Semiconductors, 1109 McKay Dr, M/S 76, San Jose, CA 95131, USA
Licensed SEGGER software: emWin
License number:           GUI-00186
License model:            emWin License Agreement, dated August 20th 2011 and Amendment No. 1, dated October 17th 2017 and Amendment No. 2, dated December 18th 2018
Licensed platform:        NXP's ARM 7/9, Cortex-M0, M3, M4, M7, A7, M33
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2011-08-19 - 2020-09-02
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUIDRV_SSD1926.h
Purpose     : Interface definition for GUIDRV_SSD1926 driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_SSD1926_H
#define GUIDRV_SSD1926_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Configuration structure
*/
typedef struct {
  //
  // Driver specific configuration items
  //
  int FirstSEG;
  int FirstCOM;
  int UseCache;
} CONFIG_SSD1926;

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_SSD1926_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OY_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OX_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OXY_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OS_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OSY_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OSX_8_API;
extern const GUI_DEVICE_API GUIDRV_SSD1926_OSXY_8_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_SSD1926_8       &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OY_8    &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OX_8    &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OXY_8   &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OS_8    &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OSY_8   &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OSX_8   &GUIDRV_Win_API
  #define GUIDRV_SSD1926_OSXY_8  &GUIDRV_Win_API

#else

  #define GUIDRV_SSD1926_8       &GUIDRV_SSD1926_8_API
  #define GUIDRV_SSD1926_OY_8    &GUIDRV_SSD1926_OY_8_API
  #define GUIDRV_SSD1926_OX_8    &GUIDRV_SSD1926_OX_8_API
  #define GUIDRV_SSD1926_OXY_8   &GUIDRV_SSD1926_OXY_8_API
  #define GUIDRV_SSD1926_OS_8    &GUIDRV_SSD1926_OS_8_API
  #define GUIDRV_SSD1926_OSY_8   &GUIDRV_SSD1926_OSY_8_API
  #define GUIDRV_SSD1926_OSX_8   &GUIDRV_SSD1926_OSX_8_API
  #define GUIDRV_SSD1926_OSXY_8  &GUIDRV_SSD1926_OSXY_8_API

#endif

/*********************************************************************
*
*       Public routines
*/
void GUIDRV_SSD1926_Config  (GUI_DEVICE * pDevice, CONFIG_SSD1926 * pConfig);
void GUIDRV_SSD1926_SetBus16(GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);

#if defined(__cplusplus)
}
#endif

#endif

/*************************** End of file ****************************/
