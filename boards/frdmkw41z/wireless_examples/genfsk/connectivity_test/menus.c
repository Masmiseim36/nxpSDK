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

#include "menus.h"

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

char * const cu8Logo[] =
{  
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
  "\r    Generic FSK Connectivity Test Demo\n\n",
  "\r\n -Press enter to start",
  NULL
};

/*@CMA, Conn Test. New string*/
char * const cu8MainMenu[] =
{  
  "\f\r  Connectivity Test Interface short cuts\n",
  "\r------------------------------------------\n",
  "\r -Press [t] for Tx operation\n",
  "\r -Press [r] for Rx operation\n",
  "\r -Press [q] for channel up\n",
  "\r -Press [w] for channel down\n",
  "\r -Press [a] for Power up\n",
  "\r -Press [s] for Power down\n",
  "\r -Press [n] to increase the Payload\n",
  "\r -Press [m] to decrease the Payload\n",
  "\r These keys can be used all over the application to change \n",
  "\r the test parameters\n",
  "\r  ________________________________\n",
  "\r |                                |\n",
  "\r |   Select the Test to perform   |\n",
  "\r |________________________________|\n",
  "\r -Press [1] Continuous tests\n",
  "\r -Press [2] Packet Error Rate test\n",
  "\r -Press [3] Range test\n",
  "\r -Press [!] Reset MCU\n\r\n",
  NULL
};

/*@CMA, Conn Test. New string*/
char * const cu8ShortCutsBar[] =
{ 
  "\f\r\n",
  "\r------------------------------------------\n",
  "\r   [t] Tx   [q] Ch+  [a] Pw+  [n] Pyld+\n",  
  "\r   [r] Rx   [w] Ch-  [s] Pw-  [m] Pyld-\n",  
  "\r------------------------------------------\r",
  NULL
};

char * const cu8ContinuousTestMenu[] =
{ 
  "\r __________________________ \n",
  "\r|                          |\n",
  "\r|   Continuous Test Menu   |\n",
  "\r|__________________________|\n\r\n",
  "\r-Press [1] Idle\n",
  "\r-Press [2] Burst Transmission using random-payload packets\n",
  "\r-Press [3] Continuous Modulated Transmission 1's\n",
  "\r-Press [4] Continuous Modulated Transmission 0's\n",
  "\r-Press [5] Continuous Modulated Transmission pseudo-random\n",
  "\r-Press [6] Continuous Unmodulated Transmission\n",
  "\r-Press [7] Continuous Packet Mode Reception\n",
  "\r-Press [8] Continuous Energy Detect\n",
  "\r-Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8PerTxTestMenu[] =
{ 
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  ______________________________________ \n",
  "\r |                                      |\n",
  "\r |      PER Tx Low Power Test Menu      |\n",
  "\r |______________________________________|\n\r\n",
#else
  "\r  ____________________________ \n",
  "\r |                            |\n",
  "\r |      PER Tx Test Menu      |\n",
  "\r |____________________________|\n\r\n",
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r  Choose the amount of packets to send:\n",
  "\r [0] - 1     Packet     [1] - 25    Packets\n",
  "\r [2] - 100   Packets    [3] - 500   Packets\n",
  "\r [4] - 1000  Packets    [5] - 2000  Packets\n",
  "\r [6] - 5000  Packets    [7] - 10000 Packets\n",
  "\r [8] - 65535 Packets\n\r\n",
  "\r-Press any push button to stop Transmiting Packets\n",
  "\r-Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8PerRxTestMenu[] =
{ 
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  ________________________________ \n",
  "\r |                                |\n",
  "\r |   PER Rx Low Power Test Menu   |\n",
  "\r |________________________________|\n\r\n",
#else
  "\r  ______________________ \n",
  "\r |                      |\n",
  "\r |   PER Rx Test Menu   |\n",
  "\r |______________________|\n\r\n",
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r -Press [space bar] to start/stop Receiving Packets\n",
  "\r -Press any push button to stop Receiving Packets\n",
  "\r -Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8RangeTxTestMenu[] =
{ 
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  __________________________________ \n",
  "\r |                                  |\n",
  "\r |   Range Tx Low Power Test Menu   |\n",
  "\r |__________________________________|\n\r\n",
#else
  "\r  ________________________ \n",
  "\r |                        |\n",
  "\r |   Range Tx Test Menu   |\n",
  "\r |________________________|\n\r\n",  
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r -Press [space bar] to start/stop Transmiting Packets\n",
  "\r -Press any push button to stop Transmiting Packets\n",
  "\r -Press [p] Previous Menu\n\r\n",
  NULL
};

char * const cu8RangeRxTestMenu[] =
{ 
#if ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable))
  "\r  __________________________________ \n",
  "\r |                                  |\n",
  "\r |   Range Rx Low Power Test Menu   |\n",
  "\r |__________________________________|\n\r\n",
#else
  "\r  ________________________ \n",
  "\r |                        |\n",
  "\r |   Range Rx Test Menu   |\n",
  "\r |________________________|\n\r\n",  
#endif /* ((cPWR_UsePowerDownMode) && (cPWR_GENFSK_LL_Enable)) */
  "\r -Press [space bar] to start/stop Receiving Packets\n",
  "\r -Press any push button to stop Receiving Packets\n",
  "\r -Press [p] Previous Menu\n\r\n",
  NULL
};

char* const cu8PerRxMessages[] =
{
  "\f\n\rPER Test Rx Running\r\n\r\n",
  "\n\rPER Test Rx failed. Press [ENTER] to continue\r\n\r\n",
  "\n\rPER Test Rx Stopped\r\n\r\n",
  "\n\rPER Test Finished\r\n\r\n",
  "\r\n Press [ENTER] to go back to the Per Rx test menu"
};

char* const cu8PerTxMessages[] =
{
  "\r\n\r\n Please type TX interval in miliseconds ( > 6 ms ) "
  "and press [ENTER]\r\n ",
  "\r\n\tError: TX Interval too small\r\n",
  "\r\n\r\n-TX failed. Press [ENTER] to continue\r\n\r\n",
  "\f\r\n Running PER Tx, Number of packets: ",
  "\r\n PER Tx DONE \r\n""\r\n\r\n Press [ENTER] to go "
  "back to the PER Tx test menu ",
  "\r\n\r\n-TX failed. Press [ENTER] to continue\r\n\r\n",
  "\r\n\r\n-Test interrupted by user. Press [ENTER] to continue\r\n\r\n"
};

char* const cu8RangeRxMessages[] =
{
  "\f\r\nRange Test Rx Running\r\n",
  "\f\r\nRange Test Rx failed\r\n"
  "\r\n\r\n Press [ENTER] to go back to the Range Rx test menu",
  "\n\r\n\rRange Test Rx Stopped\r\n\r\n",
  "\r\n\r\n Press [ENTER] to go back to the Range Rx test menu",
};

char* const cu8RangeTxMessages[] =
{
  "\f\r\nRange Test Tx Running\r\n",
  "\f\r\nRange Test Tx failed\r\n"
  "\r\n\r\n Press [ENTER] to go back to the Range Tx test menu",
  "\r\nPacket Dropped",
  "\n\r\n\rRange Test Tx Stopped\r\n\r\n",
  "\r\n\r\n Press [ENTER] to go back to the Range Tx test menu"
};

char * const cu8ContMessages[] =
{
    "\r\n\r\n Packet Mode PRBS started. Press [SPACE] to stop test\r\n",
    "\r\n\r\n Packet Mode PRBS failed. Press [ENTER] to continue\r\n",
    "\r\n\r\n Packet Mode PRBS stopped. Press [ENTER] to continue\r\n",
    "\r\n\r\n Packet Mode Rx started. Press [SPACE] to stop test\r\n",
    "\r\n\r\n Packet Mode Rx failed. Press [ENTER] to continue\r\n",
    "\r\n\r\n Packet Mode Rx stopped. Press [ENTER] to continue\r\n",
    "\r\n New Packet: 0x",
    " Idle",
    " Mod 1's",
    " Mod 0's",
    " Mod PN",
    " Unmod",
    "\r\n\r\n Continuous ED stopped. Press [ENTER] to continue\r\n",
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
/*! *********************************************************************************
* \brief  Prints test paramteres
********************************************************************************** */
void PrintTestParams(ct_config_params_t* params, bool_t bEraseLine, uint8_t port)
{   
    uint8_t u8lineLen = 65;
    uint8_t u8Index;
    
    if(bEraseLine)
    {
        Serial_Print(port, "\r", gAllowToBlock_d);
        for(u8Index = 0;u8Index<u8lineLen;u8Index++)
        {
            Serial_Print(port, " ", gAllowToBlock_d);
        }
        Serial_Print(port ,"\r", gAllowToBlock_d);
    }
    u8Index = 0;
    while(params[u8Index].paramType != gParamTypeMaxType_c)
    {
       Serial_Print(port, (char*)params[u8Index].paramName, gAllowToBlock_d);
       Serial_Print(port, " ", gAllowToBlock_d);
       if(params[u8Index].paramType == gParamTypeString_c)
       {
           Serial_Print(port, 
                        (char*)params[u8Index].paramValue.stringValue, 
                        gAllowToBlock_d);
       }
       else if(params[u8Index].paramType == gParamTypeNumber_c)
       {
           Serial_PrintDec(port, params[u8Index].paramValue.decValue);
       }
       else if(params[u8Index].paramType == gParamTypeBool_c)
       {
           if(params[u8Index].paramValue.boolValue)
           {
               Serial_Print(port, "TRUE", gAllowToBlock_d);
           }
           else
           {
               Serial_Print(port, "FALSE", gAllowToBlock_d);
           }
       }
       Serial_Print(port, ", ", gAllowToBlock_d);
       ++u8Index;
    }
    Serial_Print(port, "\b\b>", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints PER RX related information
********************************************************************************** */
void PrintPerRxInfo(uint16_t u16PacketsIndex, 
                    uint16_t u16ReceivedPackets, 
                    ct_rx_indication_t* pIndicationInfo, 
                    uint8_t port)
{
    int8_t i8TempRssiValue = 0;
    i8TempRssiValue = (int8_t)(pIndicationInfo->rssi);
    
    Serial_Print(port, "Packet ", gAllowToBlock_d);
    Serial_PrintDec(port,(uint32_t)u16ReceivedPackets);
    Serial_Print(port, ". Packet index: ",gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16PacketsIndex);
    Serial_Print(port, ". Rssi: ", gAllowToBlock_d);
    if(i8TempRssiValue < 0)
    {
        i8TempRssiValue *= -1;
        Serial_Print(port, "-", gAllowToBlock_d);
    }
    Serial_PrintDec(port, (uint32_t)i8TempRssiValue);
    Serial_Print(port, ". Timestamp: ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)pIndicationInfo->timestamp);
    Serial_Print(port, "\r\n", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints PER RX final statistics
********************************************************************************** */
void PrintPerRxFinal(uint16_t u16Received,
                     uint16_t u16TotalPackets,
                     int8_t i8AvgRssi,
                     uint8_t port)
{
    Serial_Print(port,"\r\nAverage Rssi during PER: ",gAllowToBlock_d);
    
    if(i8AvgRssi< 0)
    {
        i8AvgRssi *= -1;
        
        Serial_Print(port, "-",gAllowToBlock_d);    
    }
    Serial_PrintDec(port, (uint32_t)i8AvgRssi);
    Serial_Print(port, " dBm\r\n",gAllowToBlock_d);
    Serial_Print(port, "Received ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16Received);
    Serial_Print(port, " of ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16TotalPackets);
    Serial_Print(port, " packets transmitted \r\n", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints PER TX related information
********************************************************************************** */
void PrintPerTxPackets(uint16_t u16SentPackets,
                       bool_t bIsValid,
                       uint8_t port)
{
    if(bIsValid)
    {
        Serial_Print(port,"\r\n Packet ",gAllowToBlock_d);
        Serial_PrintDec(port,(uint32_t)u16SentPackets);
    }
    else
    {
        Serial_Print(port,"\r\n (Failed) Packet ",gAllowToBlock_d);
        Serial_PrintDec(port,(uint32_t)u16SentPackets);
    }
}

/*! *********************************************************************************
* \brief  Prints Range TX dropped packets information
********************************************************************************** */
void PrintRangeTxDroppedPackets(uint16_t u16PacketsDropped, uint8_t port)
{
    Serial_Print(port, "\r\nPackets dropped ", gAllowToBlock_d);
    Serial_PrintDec(port, (uint32_t)u16PacketsDropped); 
}

/*! *********************************************************************************
* \brief  Prints Average RSSI information for PER and Range tests
********************************************************************************** */
void PrintAvgRssi(int8_t i8AverageRssi, uint8_t port)
{
    Serial_Print(port, "Average RSSI     ", gAllowToBlock_d);
    if(i8AverageRssi < 0)
    {
        Serial_Print(port, "-", gAllowToBlock_d);
        i8AverageRssi *= -1;
    }
    Serial_PrintDec(port, (uint32_t) i8AverageRssi);
    Serial_Print(port," dBm", gAllowToBlock_d);
}

/*! *********************************************************************************
* \brief  Prints packet RSSI for PER and Range tests
********************************************************************************** */
void PrintCrtRssi(int8_t i8CrtRssi, uint8_t port)
{
    Serial_Print(port, "\r\n RSSI = ", gAllowToBlock_d);
    if(i8CrtRssi < 0)
    {
        Serial_Print(port, "-", gAllowToBlock_d);
        i8CrtRssi *= -1;
    }
    Serial_PrintDec(port, (uint32_t)i8CrtRssi);
    Serial_Print(port," dBm", gAllowToBlock_d);
}