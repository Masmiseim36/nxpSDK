/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "low_power_node_menus.h"

char * const cu8Logo[]={  
  "\f\r\n",
  "\n\n\r\n",
  " ####         ######      ##### ##########\n\r",
  " ######      # ######    ##### #############\n\r", 
  " #######     ## ######  ##### ###############\n\r",  
  " ########    ### ########### ####       #####\n\r",  
  " #### #####  #### ######### #####       #####\n\r",   
  " ####  ##### #### ######### #################\n\r",  
  " ####   ######## ########### ###############\n\r", 
  " ####     ##### ######  ##### ############\n\r",
  " ####      ### ######    ##### ##\n\r",
  " ####       # ######      ##### #\n\r\n\r",
  "\r             Low Power Demo\n\n"           ,
  "\r\n -Press enter to start",
  NULL
};

char * const cu8MainMenu[]={  
  "\f\r Low Power Demo Interface short cuts    \n",
  "\r------------------------------------------\n",
  "\r -Press [q] for channel up\n",
  "\r -Press [w] for channel down\n",
  "\r -Press [a] for Power up\n",
  "\r -Press [s] for Power down\n",
  "\r -Press [r] for RX Mode\n",
  "\r -Press [t] for TX Mode\n",
  "\r Use these keys in the main menu before running any scenario \n",
  "\r  ______________________________________\n",
  "\r |                                      |\n",
  "\r |     Select Low Power Demo Option     |\n",
  "\r |______________________________________|\n",
  "\r -Press [1] Manual Power Modes Configuration\n",
  "\r -Press [2] Low Power Preamble Search Scenario  \n",
  "\r -Press [!] Reset MCU\n\r\n",
  NULL
};


char * const cu8ShortCutsBar[]={ 
  "\f\r\n",
  "\r------------------------------------------\n",
  "\r  [q] Ch+     [a] Pw+     [r] RX Mode     \n",  
  "\r  [w] Ch-     [s] Pw-     [t] TX Mode     \n",  
  "\r------------------------------------------\n\r",
  NULL
};


char * const cu8LPCSelectMCUPowerModeMenuString[]={ 
  "\r\n", 
  "\rSelect MCU Power mode: \n",
  "\r-Press [0] Wait mode \n",
  "\r-Press [1] Stop mode \n",
  "\r-Press [2] VLPR (Very Low Power Run) mode  \n",
  "\r-Press [3] VLPW (Very Low Power Wait) mode \n",
  "\r-Press [4] VLPS (Very Low Power Stop) mode  \n",
  "\r-Press [5] LLS  (Low Leakage Stop) mode  \n",
  "\r-Press [6] VLLS3 (Very Low Leakage Stop 3) mode  \n",
  "\r-Press [7] VLLS2 (Very Low Leakage Stop 2) mode  \n",
  "\r-Press [8] VLLS1 (Very Low Leakage Stop 1) mode \n",
  "\r-Press [9] VLLS0 (Very Low Leakage Stop 0) mode  \n",
  "\r-Press [p] Return to previous menu  \r\n",
  NULL
};

char * const cu8WakeUpNonLLWU[]={ 
  "\r\n", 
  "\rSelect Wake Up Source: \n",
  "\r-Press [1] UART (any key) \n",
  "\r-Press [2] GPIO (any)   \n",                                             
  "\r-Press [3] LPTMR (5s)  \r\n",                                                 
  NULL
};

char * const cu8WakeUpLLWU[]={ 
  "\r\n", 
  "\rSelect Wake Up Source: \n",
  "\r-Press [1] GPIO (any) \n",                                             
  "\r-Press [2] LPTMR (5s)  \r\n",                                                 
  NULL
};

char * const cu8LPPSMenu[]={ 
  "\f\r\n",
  "\r------------------------------------------------------\n",
  "\r  *LPPS: TX device goes in Stop for 2s and wakes up   \n",
  "\r  to send wake up packet. RX device goes in Stop      \n"
  "\r  and xcvr in reception with lpps on.                 \n",  
  "\r  * TX exits after 5 transmissions.                   \n",
  "\r  * Exit RX (LPPS) by pressing any key                \n", 
  "\r  or by receiving a wake up packet from the TX device.\n",
  "\r------------------------------------------------------\n\r",
  NULL
};
/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/***********************************************************************************
*
* PrintMenu
*
************************************************************************************/
void PrintMenu(char * const pu8Menu[], uint8_t port)
{
  uint8_t u8Index = 0;
  (void)port;
  while(pu8Menu[u8Index]){
    Serial_Print(port, pu8Menu[u8Index], gAllowToBlock_d);
    u8Index++;
  }
}
/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/



