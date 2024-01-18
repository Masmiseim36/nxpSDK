/*********************************************************************
*                     SEGGER Microcontroller GmbH                    *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2022  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************
----------------------------------------------------------------------
File        : APPW_MainTask.c
Purpose     : APPWIZARD application entry point
---------------------------END-OF-HEADER------------------------------
*/

#include "Generated/Resource.h"
#include "emwin_support.h"

/*********************************************************************
*
*       MainTask
*/
void MainTask(void) {
  //
  // Setup configuration dependent pointers
  //
  APPW_X_Setup();
  //
  // Initialize AppWizard
  //
  APPW_Init(APPW_PROJECT_PATH);
  //
  // Create all persistent screens except initial screen
  //
  APPW_CreatePersistentScreens();
  //
  // Create initial screen...
  //
  APPW_CreateRoot(APPW_INITIAL_SCREEN, WM_HBKWIN);
  //
  // ...and keep it alive
  //
  while (1) {
    while (GUI_Exec1()) {
      APPW_Exec();
    }
    APPW_Exec();
    GUI_X_Delay(5);
    BOARD_Touch_Poll();
  }
}

/*************************** End of file ****************************/
