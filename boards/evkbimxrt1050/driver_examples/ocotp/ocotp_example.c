/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "board.h"
#include "fsl_ocotp.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_OCOTP_FREQ_HZ (CLOCK_GetFreq(kCLOCK_IpgClk))
/* NOTE: enable this feature will write data to the fuse map, and this operation is
 * irreversible. Please refer to the referance manual before using the read/write
 * function, and calling the write API cautiously.
 */
#define EXAMPLE_OCOTP_SHADOW_REGISTER_READ_WRITE_ENABLE 0U

#if defined(EXAMPLE_OCOTP_SHADOW_REGISTER_READ_WRITE_ENABLE) && (EXAMPLE_OCOTP_SHADOW_REGISTER_READ_WRITE_ENABLE)
/* 0x22 is address of MAC0 fuse map. */
#define EXAMPLE_OCOTP_FUSE_MAP_ADDRESS 0x22
#define EXAMPLE_OCOTP_FUSE_WRITE_VALUE 0x12345678
#endif

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief OCOTP example.
 *
 * @details Just prints version of driver by using OCOTP API.
 */

int main(void)
{
    uint32_t version;

    /* Init hardware*/
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    PRINTF("OCOTP Peripheral Driver Example\r\n\r\n");

    OCOTP_Init(OCOTP, EXAMPLE_OCOTP_FREQ_HZ);

    /* Get the OCOTP controller version. */
    version = OCOTP_GetVersion(OCOTP);
    PRINTF("OCOTP controller version: 0x%08X\r\n\r\n", version);

    /* Example code to write a MAC value the MAC0 register (0x22) and reload the shasdow register to read it back. */
#if defined(EXAMPLE_OCOTP_SHADOW_REGISTER_READ_WRITE_ENABLE) && (EXAMPLE_OCOTP_SHADOW_REGISTER_READ_WRITE_ENABLE)
    /* Read the LOCK register for checking the MAC_ADDR status. 0x00 stands for unlocking status. */
    if (0x00U == (OCOTP_LOCK_MAC_ADDR_MASK & OCOTP_ReadFuseShadowRegister(OCOTP, 0x00)))
    {
        PRINTF("The origin value of fuse address 0x%02X is 0x%08X\r\n", EXAMPLE_OCOTP_FUSE_MAP_ADDRESS,
               OCOTP_ReadFuseShadowRegister(OCOTP, EXAMPLE_OCOTP_FUSE_MAP_ADDRESS));

        if (kStatus_Success ==
            OCOTP_WriteFuseShadowRegister(OCOTP, EXAMPLE_OCOTP_FUSE_MAP_ADDRESS, EXAMPLE_OCOTP_FUSE_WRITE_VALUE))
        {
            PRINTF("OCOTP Write operation success!\r\n");
            PRINTF("The new value is 0x%08X\r\n", OCOTP_ReadFuseShadowRegister(OCOTP, EXAMPLE_OCOTP_FUSE_MAP_ADDRESS));
        }
        else
        {
            PRINTF("OCOTP write operation failed. Access deny!\r\n");
        }
    }
#endif

    while (1)
    {
    }
}
