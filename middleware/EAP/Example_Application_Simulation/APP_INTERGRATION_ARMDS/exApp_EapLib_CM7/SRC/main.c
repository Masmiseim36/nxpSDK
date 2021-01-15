/*
** Copyright (c) 2006-2017 Arm Limited (or its affiliates). All rights reserved.
** Use, modification and redistribution of this file is subject to your possession of a
** valid End User License Agreement for the Arm Product of which these examples are part of 
** and your compliance with all applicable terms and conditions of such licence agreement.
*/

/* This file contains the main() program that sets the vector table location, displays a welcome message,
initializes the MPU, enables the caches, starts the SysTick timer, initializes the Process Stack Pointer, 
changes Thread mode to Unprivileged and to use the Process Stack, then runs the main application (sorts) */


#include "scs.h"
#include "TEST_PARAM.h"

#include <stdio.h>
#include <math.h>
#include <arm_acle.h>

/* Linker symbols from scatter-file */
#define VectorTableOffsetRegister 0xE000ED08
#define ICIALLU 0xE000EF50
extern unsigned int Image$$VECTORS$$Base;
extern unsigned int Image$$PROCESS_STACK$$ZI$$Limit;

/* Enable the FPU if required */
#ifdef __ARM_FP
extern void $Super$$__rt_lib_init(void);

void $Sub$$__rt_lib_init(void)
{
    /* Enable the FPU in both privileged and user modes by setting bits 20-23 to enable CP10 and CP11 */
    SCS.CPACR = SCS.CPACR | (0xF << 20);
    $Super$$__rt_lib_init();
}
#endif

__attribute__((noreturn)) int main(int argc, char *argv[])
{
	/* Processor starts-up in Privileged Thread Mode using Main Stack */

    /* Tell the processor the location of the vector table, obtained from the scatter file */
    *(volatile unsigned int *)(VectorTableOffsetRegister) = (unsigned int) &Image$$VECTORS$$Base;

    /* Display a welcome message via semihosting */
    printf("Cortex-M7 bare-metal EAP integration example\n");

    /* Invalidate the caches */
    *(volatile unsigned int *)(ICIALLU) = 0;

    /* Enable both I & D caches */
    SCS.CCR = SCS.CCR | 0x30000;

    /* Initialize Process Stack Pointer */
    __arm_wsr("PSP", (unsigned int) &Image$$PROCESS_STACK$$ZI$$Limit);

    /* Change Thread mode to Unprivileged and to use the Process Stack */
    unsigned int read_ctrl = __arm_rsr("CONTROL");
    __arm_wsr("CONTROL", read_ctrl | 3);

    /* Flush and refill pipeline with unprivileged permissions */
    __isb(0xf);

    /* EAP test start */
	printf("\n**** EAP Test start ****  \n\n");

    int returnTest = eapTest();
    if (returnTest == 0)
    {
    	printf("\n**** EAP test finished successfully ****  \n\n");
    }
    else
    {
    	printf("\n**** ERROR: EAP test finished with error code %d \n\n",returnTest);
    }
}

