/**
 * @file app_boot.h
 * @author NXP Semiconductors
 * @version 1.0
 * @par License
 * Copyright(C) NXP Semiconductors, 2017
 * All rights reserved.
 *
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * A7-series security ICs.  This software is supplied "AS IS" without any
 * warranties of any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * Permission to use, copy and modify this software is hereby granted,
 * under NXP Semiconductors' and its licensor's relevant copyrights in
 * the software, without fee, provided that it is used in conjunction with
 * NXP Semiconductors products. This copyright, permission, and disclaimer notice
 * must appear in all copies of this code.
 *
 * @par Description
 * Implementation of the App booting time initilization functions
 */

#include "ax_api.h"

#define APP_HOST_CONNECTION_STATUS_LINK_ERR         0x02
#define APP_HOST_CONNECTION_STATUS_PORT_ADDR_ERR    0x03
#define APP_HOST_CONNECTION_STATUS_COMM_ERR         0x04
#define APP_HOST_CONNECTION_STATUS_PROTOCOL_ERR     0x05

#define APP_HOST_CONNECTION_STATUS_LINK_FAILED  0x02

#define BOOT_SKIP_EXAMPLE       (0x00)  //!< Argument relevant to ::exBoot
#define BOOT_SIMULATED_CYCLE    (0x01)  //!< Argument relevant to ::exBoot
#define BOOT_BOOTLOADER_ROLE    (0x02)  //!< Argument relevant to ::exBoot
#define BOOT_HOST_OS_RESUME     (0x04)  //!< Argument relevant to ::exBoot

#if defined(TDA8029_UART) || defined(I2C) || defined(PCSC) || defined(SPI) || defined(IPC)

#define APP_BOOT_RESUME_HANDLING(EX_BOOT_FN)                      \
    if (argc == 2)                                                \
    {                                                             \
        initMeasurement(&execTime);                               \
        if (strcmp(argv[1], "boot") == 0)                         \
        {                                                         \
            result &= EX_BOOT_FN(BOOT_BOOTLOADER_ROLE);           \
        }                                                         \
        else if (strcmp(argv[1], "resume") == 0)                  \
        {                                                         \
            result &= EX_BOOT_FN(BOOT_HOST_OS_RESUME);            \
        }                                                         \
        else if (strcmp(argv[1], "both") == 0)                    \
        {                                                         \
            result &= EX_BOOT_FN(BOOT_BOOTLOADER_ROLE);           \
            result &= EX_BOOT_FN(BOOT_HOST_OS_RESUME);            \
        }                                                         \
        else                                                      \
        {                                                         \
            printf("Unknown command option: %s.\r\n", argv[1]);   \
            printf("Usage: %s [boot|resume|both].\r\n", argv[0]); \
            result = 0;                                           \
        }                                                         \
        goto LBL_REPORT_STATUS;                                   \
    }                                                             \
    
#else   
#define APP_BOOT_RESUME_HANDLING(EX_BOOT_FN)
#endif

/**
 * Boot up of the application.  Configure clocks/pin/etc. where applicable on
 * platform. For most systems these decisions have to be made as early as
 * possible during boot.
 *
 * For many systems like linux/windows these would translate to an almost empty
 * call. For other embedded platforms this would do HW initialization.
 *
 * @return     0 on success.
 */
extern int app_boot_Init(void);

/**
 * Connect to secure element based on the pre-compiled selection of
 * Communication layer.
 *
 * @param[out] pCommState        Pointer where the communication state is updated.
 * @param[in]  pConnectionParam  Can be null where connection is I2C/etc. on the
 *                               same board.  For remote connection this would
 *                               be name of IpAddress/Server Name : Port in case
 *                               of Socket connection or COM PORT in case of
 *                               VCOM Connection.
 *
 * @return     0 on success.
 */
extern int app_boot_Connect(
    SmCommState_t* pCommState, const char* pConnectionParam);

/**
 * SET LEDs for the stauts.
 *
 * @parm[in] status 1 => Pass. 0 ==> Failed.
 */
extern void app_test_status(U8 result);

