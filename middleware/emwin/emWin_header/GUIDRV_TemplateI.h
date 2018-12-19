/*********************************************************************
*                SEGGER Microcontroller GmbH                         *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2018  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.48 - Graphical user interface for embedded applications **
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
License model:            emWin License Agreement, dated August 20th 2011 and Amendment, dated October 19th 2017
Licensed platform:        NXP's ARM 7/9, Cortex-M0, M3, M4, M7, A7
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2011-08-19 - 2018-09-02
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUIDRV_TemplateI.h
Purpose     : Interface definition for GUIDRV_TemplateI driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_TEMPLATE_I_H
#define GUIDRV_TEMPLATE_I_H

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
  int Dummy;
} CONFIG_TEMPLATE_I;

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OY_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OX_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OXY_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OS_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OSY_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OSX_16_API;
extern const GUI_DEVICE_API GUIDRV_TEMPLATE_I_OSXY_16_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_TEMPLATE_I_16       &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OY_16    &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OX_16    &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OXY_16   &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OS_16    &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OSY_16   &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OSX_16   &GUIDRV_Win_API
  #define GUIDRV_TEMPLATE_I_OSXY_16  &GUIDRV_Win_API

#else

  #define GUIDRV_TEMPLATE_I_16       &GUIDRV_TEMPLATE_I_16_API
  #define GUIDRV_TEMPLATE_I_OY_16    &GUIDRV_TEMPLATE_I_OY_16_API
  #define GUIDRV_TEMPLATE_I_OX_16    &GUIDRV_TEMPLATE_I_OX_16_API
  #define GUIDRV_TEMPLATE_I_OXY_16   &GUIDRV_TEMPLATE_I_OXY_16_API
  #define GUIDRV_TEMPLATE_I_OS_16    &GUIDRV_TEMPLATE_I_OS_16_API
  #define GUIDRV_TEMPLATE_I_OSY_16   &GUIDRV_TEMPLATE_I_OSY_16_API
  #define GUIDRV_TEMPLATE_I_OSX_16   &GUIDRV_TEMPLATE_I_OSX_16_API
  #define GUIDRV_TEMPLATE_I_OSXY_16  &GUIDRV_TEMPLATE_I_OSXY_16_API

#endif

/*********************************************************************
*
*       Public routines
*/
void GUIDRV_TemplateI_Config    (GUI_DEVICE * pDevice, CONFIG_TEMPLATE_I * pConfig);
void GUIDRV_TemplateI_SetBus_XXX(GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);
void GUIDRV_TemplateI_SetFuncXXX(GUI_DEVICE * pDevice);

#if defined(__cplusplus)
}
#endif

#endif

/*************************** End of file ****************************/
