/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
extern void common_startup(void);
extern int32_t main(void);
extern void WatchdogEnable(void);
extern void WatchdogDisable(void);

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Start
 *
 * @param void
 *
 * @return None
 *
 * This function sets up watchdog, calls all of the needed starup routines and then
 * branches to the main process.
 */
void start(void)
{
    /* Update of watchdog configuration */
#if WATCHDOG_ENABLED
  
#if(defined(_MIMXRT1011_H_))  
      __asm("cpsid i");
    uint32_t temp = RTWDOG_CS_EN(1) | RTWDOG_CS_CLK(1) | RTWDOG_CS_UPDATE(1) | RTWDOG_CS_CMD32EN(1);//| RTWDOG_CS_PRES(1);    
    USED_WDOG->CNT = 0xD928C520; //unlock watchdog
    while ((USED_WDOG->CS & RTWDOG_CS_ULK_MASK) == 0)
    {
    }
    USED_WDOG->TOVAL = WATCHDOG_TIMEOUT_VALUE;
    USED_WDOG->CS |= temp;
    while ((USED_WDOG->CS & RTWDOG_CS_RCS_MASK) == 0)
    {
    }  
    __asm("cpsie i");
#else 
    WatchdogEnable(); 
#endif

#else
    WatchdogDisable();
#endif

    /* Copy any vector or data sections that need to be in RAM */
    common_startup();

    /* Jump to main process */
    main();

    /* No actions to perform after this so wait forever */
    while (TRUE)
        ;
}
