/*
* The Clear BSD License
* Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
* All rights reserved.
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

#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_smc.h"
#include "fsl_debug_console.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "board.h"
#include "pin_mux.h"
#include "GPIO_Adapter.h"
#include "RNG_Interface.h"
#include "FunctionLib.h"
#include "clock_config.h"
#include "Flash_Adapter.h"

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
void hardware_init(void)
{
    static uint8_t initialized = 0;
    
    if( !initialized )
    {
        if((PMC->REGSC & PMC_REGSC_ACKISO_MASK) != 0x00U)
        {
            PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* Release hold with ACKISO:  Only has an effect if recovering from VLLSx.*/
        }
            
        /* Enable USB Master Flash Read Access */
        /* FMC_BWR_PFAPR_M4AP(FMC, 1); */
        FMC->PFAPR |= FMC_PFAPR_M4AP_MASK;
        /* Init board clock */
        BOARD_BootClockRUN();
        CLOCK_EnableClock(kCLOCK_PortB); 
        NV_ReadHWParameters(&gHardwareParameters);
        initialized = 1;
    }
}

void BOARD_InitAdc(void)
{
     
}

uint8_t BOARD_GetBatteryLevel(void)
{
    return 100;    
}

uint16_t BOARD_GetPotentiometerLevel(void)
{
    uint32_t random = 0;
    RNG_GetRandomNo(&random);
    
    return (random & 0xFF);
}

int32_t BOARD_GetTemperature(void)
{
    return 25;
}

uint32_t BOARD_GetLpuartClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t BOARD_GetUartClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_CoreSysClk);
}

uint32_t BOARD_GetFtmClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t BOARD_GetSpiClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_BusClk);    
}

uint32_t BOARD_GetI2cClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_BusClk);    
}


/* get 4 words of information that uniquely identifies the MCU */
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen)
{   
    uint32_t uid[4] = {0};

    uid[0] = SIM->SDID;
    uid[1] = SIM->UIDMH;
    uid[2] = SIM->UIDML;
    uid[3] = SIM->UIDL;
    
    FLib_MemCpy(aOutUid16B, (uint8_t*)uid, sizeof(uid));
    *pOutLen = sizeof(uid);
    
    return;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

