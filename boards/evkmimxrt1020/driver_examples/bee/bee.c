/*
 * The Clear BSD License
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
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

#include "fsl_debug_console.h"
#include "fsl_bee.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define FLEXSPI_START_ADDR 0x60000000U
/* User key feature is not enabled in fuses on RT1020 */
#define DCP_USE_USER_KEY 0
#define AES_KEY_LEN 16
#define BEE_REGION_SIZE 0x10000U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
#if defined(DCP_USE_USER_KEY) && DCP_USE_USER_KEY
static const uint8_t aesKey[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10};
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    bee_region_config_t beeConfig;
    status_t status;

    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("\r\nBEE Example.\r\n");

    /* Get default configuration. */
    BEE_GetDefaultConfig(&beeConfig);

    beeConfig.mode = kBEE_AesEcbMode;
    beeConfig.regionBot = FLEXSPI_START_ADDR;
    beeConfig.regionTop = FLEXSPI_START_ADDR + BEE_REGION_SIZE;
    beeConfig.addrOffset = BEE_GetOffset(FLEXSPI_START_ADDR);
    beeConfig.regionEn = kBEE_RegionEnabled;

    /* Init BEE driver and set region0 configuration */
    BEE_Init(BEE);
    status = BEE_SetRegionConfig(BEE, kBEE_Region0, &beeConfig);
    if (status != kStatus_Success)
    {
        PRINTF("Error setting region configuration!\r\n");
    }

/* Set AES user key for region0. BEE_KEY0_SEL fuse must be set to SW-GP2 to be able to use this feature. */
#if defined(DCP_USE_USER_KEY) && DCP_USE_USER_KEY
    status = BEE_SetRegionKey(BEE, kBEE_Region0, aesKey, AES_KEY_LEN, NULL, 0);
    if (status != kStatus_Success)
    {
        PRINTF(
            "BEE key selection fuse is programmed to use OTMP key on this device. OTMP key will be used for "
            "decryption.\r\n");
    }
#endif

    /* Enable BEE decryption */
    BEE_Enable(BEE);

    PRINTF("BEE was succesfully configured for reading from 0x%x to 0x%x.\r\n", FLEXSPI_START_ADDR,
           FLEXSPI_START_ADDR + BEE_REGION_SIZE);

    /* Disable and deinit BEE */
    BEE_Deinit(BEE);
    PRINTF("Example end.\r\n");

    while (1)
    {
    }
}
