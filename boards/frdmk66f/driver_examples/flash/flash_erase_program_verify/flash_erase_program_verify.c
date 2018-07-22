/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "clock_config.h"
#include "pin_mux.h"
#include "fsl_flash.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


#define BUFFER_LEN 4

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void error_trap(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Use Standard Software Drivers (SSD) to modify flash.
 *
 * @details This function uses SSD to demonstrate flash mode:
 *            + Check flash information.
 *            + Erase a sector and verify.
 *            + Program a sector and verify.
 */
int main(void)
{
    flash_config_t flashDriver;                                            /* Flash driver Structure */
    flash_security_state_t securityStatus = kFLASH_securityStateNotSecure; /* Return protection status */
    status_t result;    /* Return code from each flash driver function */
    uint32_t destAdrss; /* Address of the target location */
    uint32_t i, failAddr, failDat;
    uint32_t buffer[BUFFER_LEN]; /* Buffer for program */

    uint32_t pflashBlockBase = 0;
    uint32_t pflashTotalSize = 0;
    uint32_t pflashSectorSize = 0;
    uint32_t dflashTotalSize = 0;
    uint32_t dflashBlockBase = 0;
    uint32_t eepromTotalSize = 0;

    /* Init hardware */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Clean up Flash driver Structure*/
    memset(&flashDriver, 0, sizeof(flash_config_t));

    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&flashDriver);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Get flash properties*/
    FLASH_GetProperty(&flashDriver, kFLASH_propertyPflashBlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&flashDriver, kFLASH_propertyPflashTotalSize, &pflashTotalSize);
    FLASH_GetProperty(&flashDriver, kFLASH_propertyPflashSectorSize, &pflashSectorSize);
    FLASH_GetProperty(&flashDriver, kFLASH_propertyDflashTotalSize, &dflashTotalSize);
    FLASH_GetProperty(&flashDriver, kFLASH_propertyDflashBlockBaseAddr, &dflashBlockBase);
    FLASH_GetProperty(&flashDriver, kFLASH_propertyEepromTotalSize, &eepromTotalSize);

    /* print welcome message */
    PRINTF("\r\n Flash Example Start \r\n");
    /* Print flash information - PFlash. */
    PRINTF("\r\n Flash Information: ");
    PRINTF("\r\n Total Program Flash Size:\t%d KB, Hex: (0x%x)", (pflashTotalSize / 1024), pflashTotalSize);
    PRINTF("\r\n Program Flash Sector Size:\t%d KB, Hex: (0x%x) ", (pflashSectorSize / 1024), pflashSectorSize);
    /* Check if DFlash exist on this device. */
    if (dflashTotalSize)
    {
        PRINTF("\r\n Data Flash Size:\t%d KB, Hex: (0x%x)", (dflashTotalSize / 1024), dflashTotalSize);
        PRINTF("\r\n Data Flash Base Address:\t0x%x", dflashBlockBase);
    }
    else
    {
        PRINTF("\r\n There is no D-Flash (FlexNVM) on this Device.");
    }
    /* Check if FlexMemory exist on this device. */
    if (eepromTotalSize)
    {
        PRINTF("\r\n Enhanced EEPROM (EEE) Block Size:\t%d KB, Hex: (0x%x)", (eepromTotalSize / 1024), eepromTotalSize);
    }
    else
    {
        PRINTF("\r\n There is no Enhanced EEPROM (EEE) on this Device.");
    }

    /* Check security status. */
    result = FLASH_GetSecurityState(&flashDriver, &securityStatus);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Print security status. */
    switch (securityStatus)
    {
        case kFLASH_securityStateNotSecure:
            PRINTF("\r\n Flash is UNSECURE!");
            break;
        case kFLASH_securityStateBackdoorEnabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is ENABLED!");
            break;
        case kFLASH_securityStateBackdoorDisabled:
            PRINTF("\r\n Flash is SECURE, BACKDOOR is DISABLED!");
            break;
        default:
            break;
    }
    PRINTF("\r\n");

    /* Debug message for user. */
    /* Erase several sectors on upper pflash block where there is no code */
    PRINTF("\r\n Erase a sector of flash");

    /* Erase a sector from destAdrss. */
    destAdrss = pflashBlockBase + (pflashTotalSize - pflashSectorSize);
    result = FLASH_Erase(&flashDriver, destAdrss, pflashSectorSize, kFLASH_apiEraseKey);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Verify sector if it's been erased. */
    result = FLASH_VerifyErase(&flashDriver, destAdrss, pflashSectorSize, kFLASH_marginValueUser);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Print message for user. */
    PRINTF("\r\n Successfully Erased Sector 0x%x -> 0x%x\r\n", destAdrss, (destAdrss + pflashSectorSize));

    /* Print message for user. */
    PRINTF("\r\n Program a buffer to a sector of flash ");
    /* Prepare buffer. */
    for (i = 0; i < BUFFER_LEN; i++)
    {
        buffer[i] = i;
    }
    destAdrss = pflashBlockBase + (pflashTotalSize - pflashSectorSize);
    result = FLASH_Program(&flashDriver, destAdrss, buffer, sizeof(buffer));
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }

    /* Program Check user margin levels */
    result = FLASH_VerifyProgram(&flashDriver, destAdrss, sizeof(buffer), buffer, kFLASH_marginValueUser, &failAddr,
                                 &failDat);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    PRINTF("\r\n Successfully Programmed and Verified Location 0x%x -> 0x%x \r\n", destAdrss,
           (destAdrss + sizeof(buffer)));

    /* Print finished message. */
    PRINTF("\r\n End of Flash Example \r\n");
    while (1)
    {
    }
}

/*
* @brief Gets called when an error occurs.
*
* @details Print error message and trap forever.
*/
void error_trap(void)
{
    PRINTF("\r\n\r\n\r\n\t---- HALTED DUE TO FLASH ERROR! ----");
    while (1)
    {
    }
}
