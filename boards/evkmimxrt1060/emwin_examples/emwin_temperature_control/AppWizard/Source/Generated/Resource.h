/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2020  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : Resource.h
Purpose     : Generated file do NOT edit!
---------------------------END-OF-HEADER------------------------------
*/

#ifndef RESOURCE_H
#define RESOURCE_H

#include "AppWizard.h"

/*********************************************************************
*
*       Text
*/
#define ID_TEXT_0 0
#define ID_TEXT_1 1
#define ID_TEXT_2 2
#define ID_TEXT_On 3
#define ID_TEXT_Off 4
#define ID_TEXT_Auto 5
#define ID_TEXT_3 6
#define ID_TEXT_4 7
#define ID_TEXT_5 8
#define ID_TEXT_6 9
#define ID_TEXT_7 10
#define ID_TEXT_8 11
#define ID_TEXT_9 12
#define ID_TEXT_10 13
#define ID_TEXT_11 14

#define APPW_MANAGE_TEXT APPW_MANAGE_TEXT_EXT

extern const unsigned char acAPPW_Language_0[];

/*********************************************************************
*
*       Fonts
*/
extern const unsigned char acNettoOT_24_Normal_EXT_AA4[];
extern const unsigned char acRoboto_24_Normal_EXT_AA4[];
extern const unsigned char acRoboto_16_Normal_EXT_AA4[];

/*********************************************************************
*
*       Images
*/
extern const unsigned char acTemperatureControl_3_480x272[];
extern const unsigned char acKNOB_106x106[];
extern const unsigned char acGreenGlowMarker_16x9[];
extern const unsigned char acButtonRound_48x21[];
extern const unsigned char acButtonRoundPressed_48x21[];
extern const unsigned char acSeggerLogo_80x40[];

/*********************************************************************
*
*       Screens
*/
#define ID_SCREEN_TEMP_CTRL (GUI_ID_USER + 4096)

extern APPW_ROOT_INFO ID_SCREEN_TEMP_CTRL_RootInfo;

#define APPW_INITIAL_SCREEN &ID_SCREEN_TEMP_CTRL_RootInfo

/*********************************************************************
*
*       Project path
*/
#define APPW_PROJECT_PATH ""

#endif  // RESOURCE_H

/*************************** End of file ****************************/
