/*!
* The Clear BSD License
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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



