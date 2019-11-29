/*!
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "wireless_messenger_menus.h"

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
  "\r          Wireless Messenger Demo\n\n"           ,
  "\r\n -Press enter to start",
  NULL
};

char * const cu8MainMenu[]={  
  "\f\r  Wireless Messenger Interface short cuts\n",
  "\r------------------------------------------\n",
  "\r -Press [q] for channel up\n",
  "\r -Press [w] for channel down\n",
  "\r -Press [a] for Power up\n",
  "\r -Press [s] for Power down\n",
  "\r -Press [z] for CCA Fail Retry Up\n",
  "\r -Press [x] for CCA Fail Retry down\n",
  "\r -Press [r] for ACK Fail Retry up\n",
  "\r -Press [t] for ACK Fail Retry down\n",
  "\r These keys can be used all over the application to change \n",
  "\r the test parameters\n",
  "\r  ______________________________________\n",
  "\r |                                      |\n",
  "\r |   Select Wireless Messenger Option   |\n",
  "\r |______________________________________|\n",
  "\r -Press [1] Configure Tx Params\n",
  "\r -Press [2] Wireless Messenger Console  \n",
#if gSmacUseSecurity_c
  "\r -Press [3] Security IV and Key Config  \n",
#endif
  "\r -Press [!] Reset MCU\n\r\n",
  NULL
};


char * const cu8ConfigMenu[]={  
  "\f\r  Wireless Messenger Configuration Menu\n",
  "\r------------------------------------------\n",
  "\r  ________________________________\n",
  "\r |                                |\n",
  "\r |  Select Configuration Option   |\n",
  "\r |________________________________|\n",
  "\r -Press [1] Configure Short Pan ID\n",
  "\r -Press [2] Configure Short Src ID\n",
  "\r -Press [3] Configure Short Dst ID\n",
  "\r -Press [p] Back to main menu\n\r\n",
  NULL
};

char * const cu8ShortCutsBar[]={ 
  "\f\r\n",
  "\r------------------------------------------------------------------------------\n",
  "\r  [q] Ch+     [a] Pw+     [z] CCAFailRetryCount+     [r] ACKFailRetryCount+   \n",  
  "\r  [w] Ch-     [s] Pw-     [x] CCAFailRetryCount-     [t] ACKFailRetryCount-   \n",  
  "\r------------------------------------------------------------------------------\n\r",
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



