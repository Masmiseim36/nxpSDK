/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*${header:start}*/
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "app.h"
/*${header:end}*/

/*${variable:start}*/

/*${variable:end}*/

/*${function:start}*/
void BOARD_InitHardware(void)
{
    BOARD_ConfigMPU();  
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    
    BOARD_InitPsRamPins_Xspi1();
    BOARD_Init16bitsPsRam(XSPI1);
    
    if (DEMO_MMU_INSTANCE == MMU1)
    {
        GlikeyWriteEnable(GLIKEY0, 14U); /* Enable write access to MMU1 */
    }
    else
    {
      GlikeyWriteEnable(GLIKEY0, 15U); /* Enable write access to MMU0 */
    }
}
/*${function:end}*/
