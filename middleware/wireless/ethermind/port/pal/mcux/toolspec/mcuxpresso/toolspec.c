/**
 *  \file toolspec.c
 *  Toolchain specific source
 *
 *
 */

/*
 *  Copyright (C) 2013. Mindtree Limited.
 *  All rights reserved.
 */

/* --------------------------------------------- Header File Inclusion */
#include "toolspec.h"

/* --------------------------------------------- Global Definitions */

/* --------------------------------------------- Static Global Variables */

/* --------------------------------------------- External Global Variables */

/* --------------------------------------------- Exported Global Variables */

/* --------------------------------------------- Function */
#if defined(__CC_ARM) || (defined(__GNUC__))
__attribute((externally_visible))
#endif
int _write (int fd, char *ptr, int len)
{
    /*fix build warning: missing return statement at end of non-void function.*/
    return -1;
}

#if defined(__CC_ARM) || (defined(__GNUC__))
__attribute((externally_visible))
#endif
int _read (int fd, char *ptr, int len)
{
    /*fix build warning: missing return statement at end of non-void function.*/
    return -1;
}

#if defined(__CC_ARM) || (defined(__GNUC__))
__attribute((externally_visible))
#endif
int _close (int fd)
{
    return -1;
}

#if defined(__CC_ARM) || (defined(__GNUC__))
__attribute((externally_visible))
#endif
int _fstat (int fd)
{
    return -1;
}

#if defined(__CC_ARM) || (defined(__GNUC__))
__attribute((externally_visible))
#endif
int _isatty (int fd)
{
    return -1;
}

#if defined(__CC_ARM) || (defined(__GNUC__))
__attribute((externally_visible))
#endif
int _lseek (int fd)
{
    return -1;
}

void debugHardfault(uint32_t *sp)
{
#if 0
    uint32_t cfsr  = SCB->CFSR;
    uint32_t hfsr  = SCB->HFSR;
    uint32_t mmfar = SCB->MMFAR;
    uint32_t bfar  = SCB->BFAR;

    uint32_t r0  = sp[0];
    uint32_t r1  = sp[1];
    uint32_t r2  = sp[2];
    uint32_t r3  = sp[3];
    uint32_t r12 = sp[4];
    uint32_t lr  = sp[5];
    uint32_t pc  = sp[6];
    uint32_t psr = sp[7];

    static char msg[80];

    __asm volatile("BKPT #01");

    sprintf(msg, "HardFault:\n");
    sprintf(msg, "SCB->CFSR   0x%08lx\n", cfsr);
    sprintf(msg, "SCB->HFSR   0x%08lx\n", hfsr);
    sprintf(msg, "SCB->MMFAR  0x%08lx\n", mmfar);
    sprintf(msg, "SCB->BFAR   0x%08lx\n", bfar);
    sprintf(msg, "\n");

    sprintf(msg, "SP          0x%08lx\n", (uint32_t)sp);
    sprintf(msg, "R0          0x%08lx\n", r0);
    sprintf(msg, "R1          0x%08lx\n", r1);
    sprintf(msg, "R2          0x%08lx\n", r2);
    sprintf(msg, "R3          0x%08lx\n", r3);
    sprintf(msg, "R12         0x%08lx\n", r12);
    sprintf(msg, "LR          0x%08lx\n", lr);
    sprintf(msg, "PC          0x%08lx\n", pc);
    sprintf(msg, "PSR         0x%08lx\n", psr);
#endif /* 0 */

    while(1U);
}

#if 0
__attribute__( (naked) )
void HardFault_Handler(void)
{
    __asm volatile
    (
        "tst lr, #4                                    \n"
        "ite eq                                        \n"
        "mrseq r0, msp                                 \n"
        "mrsne r0, psp                                 \n"
        "ldr r1, debugHardfault_address                \n"
        "bx r1                                         \n"
        "debugHardfault_address: .word debugHardfault  \n"
    );
}
#endif /* 0 */

