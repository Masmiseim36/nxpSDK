/*
 * Copyright 2019 NXP
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "exception_handling.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Firstly check the current LR value, All a valid LR EXC_RETURN values have bits[31:5] set to one.
 * If the bits[31:5] of LR is one, then use check the LR to get faulted SP.
 * If bits[31:5] is not all one, then LR have been pushed to current MSP, will loop check MSP to get
 * valid LR value, and use the LR value to get which SP happen faulted.
 * Then check bit[2] of LR to get which SP faulted happen, if bit[2] of LR is one, then fault happen
 * with PSP, if bit[2] of LR is zero, then fault happen with MSP. */
#if defined(__IAR_SYSTEMS_ICC__) || defined(__GNUC__)
#define COPY_TO_STACK()                                  \
    __asm volatile("push {r0-r1}");                      \
    __asm volatile("push {r0-r5}");                      \
    __asm volatile("mrs  r1, msp");                      \
    __asm volatile("adds r1, r1, #32");                  \
    __asm volatile("mov  r5, r1");                       \
    __asm volatile("and  r2, lr,#0xffffffe0");           \
    __asm volatile("cmp  r2, #0xffffffe0");              \
    __asm volatile(                                      \
        "beq  _lr_case \n"                               \
        "_loop_check: \n"                                \
        "mov r4, #4\n"                                   \
        "adds r1, r1, #4\n"                              \
        "ldr  r3,[r1]\n"                                 \
        "and  r2, r3,#0xffffffe0 \n"                     \
        "cmp  r2, #0xffffffe0 \n"                        \
        "bne  _loop_check \n"                            \
        "b _handle_lr\n"                                 \
        "_lr_case: \n"                                   \
        "mov r4, #0\n"                                   \
        "mov r3, lr\n"                                   \
        "_handle_lr: ");                                 \
    __asm volatile("adds  r1, r1, r4");                  \
    __asm volatile("msr   msp, r1");                     \
    __asm volatile("tst  r3,#4");                        \
    __asm volatile(                                      \
        "ite    eq \n"                                   \
        "mrseq r0, msp \n"                               \
        "mrsne r0, psp");                                \
    __asm volatile("push  {r0}");                        \
    __asm volatile("mrs   r0, ipsr");                    \
    __asm volatile("push  {r0}");                        \
    __asm volatile("ldr   r0, =g_exceptionStackStruct"); \
    __asm volatile("str   sp, [r0]");                    \
    __asm volatile("sub   r5, r5, #32");                 \
    __asm volatile("msr   msp, r5");                     \
    __asm volatile("pop   {r0-r5}");

#elif defined(__ARMCC_VERSION)
__ASM void COPY_TO_STACK(void)
{
    PRESERVE8
    EXTERN g_exceptionStackStruct

    push {r0-r1}
    push {r0-r5}
    mrs r1, msp
    adds r1, r1, #32
    mov r5, r1
    and r2, lr, #0xffffffe0
    cmp r2, #0xffffffe0
    beq _lr_case
_loop_check
    mov r4, #4
    adds r1, r1, #4
    ldr r3, [r1] 
    and r2, r3, #0xffffffe0
    cmp r2, #0xffffffe0
    bne _loop_check
    b _handle_lr
_lr_case
    mov r4, #0
    mov r3, lr 
_handle_lr
    adds r1, r1, r4
    msr msp, r1 
    tst r3, #4
    ite eq
        mrseq r0, msp
        mrsne r0, psp
    push{r0}
    mrs r0, ipsr
    push{r0}
    ldr r0, = g_exceptionStackStruct
    str sp,[r0]
    sub r5, r5, #32
    msr msp, r5
    pop {r0-r5}
    bx lr

    ALIGN
}
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/

exception_stack_data_t *g_exceptionStackStruct = NULL;

const scb_data_text_t scb_data_text[] = {{32, 0x000, "CPUID   - CPUID Base Register"},
                                         {32, 0x004, "ICSR    - Interrupt Control and State Register"},
                                         {32, 0x008, "VTOR    - Vector Table Offset Register"},
                                         {32, 0x00C, "AIRCR   - Application Interrupt and Reset Control Register"},
                                         {32, 0x010, "SCR     - System Control Register"},
                                         {32, 0x014, "CCR     - Configuration Control Register"},
                                         {8, 0x018, "SHPR[0] - System Handlers Priority Registers 0"},
                                         {8, 0x019, "SHPR[1] - System Handlers Priority Registers 1"},
                                         {8, 0x01a, "SHPR[2] - System Handlers Priority Registers 2"},
                                         {8, 0x01b, "SHPR[3] - System Handlers Priority Registers 3"},
                                         {8, 0x01C, "SHPR[4] - System Handlers Priority Registers 4"},
                                         {8, 0x01d, "SHPR[5] - System Handlers Priority Registers 5"},
                                         {8, 0x01e, "SHPR[6] - System Handlers Priority Registers 6"},
                                         {8, 0x01f, "SHPR[7] - System Handlers Priority Registers 7"},
                                         {8, 0x020, "SHPR[8] - System Handlers Priority Registers 8"},
                                         {8, 0x021, "SHPR[9] - System Handlers Priority Registers 9"},
                                         {8, 0x022, "SHPR[10] - System Handlers Priority Registers 10"},
                                         {8, 0x023, "SHPR[11] - System Handlers Priority Registers 11"},
                                         {32, 0x024, "SHCSR    - System Handler Control and State Register"},
                                         {32, 0x028, "CFSR     - Configurable Fault Status Register"},
                                         {32, 0x02C, "HFSR     - HardFault Status Register"},
                                         {32, 0x030, "DFSR     - Debug Fault Status Register"},
                                         {32, 0x034, "MMFAR    - MemManage Fault Address Register"},
                                         {32, 0x038, "BFAR     - BusFault Address Register"},
                                         {32, 0x03C, "AFSR     - Auxiliary Fault Status Register"},
                                         {32, 0x040, "ID_PFR[0]  - Processor Feature Register"},
                                         {32, 0x044, "ID_PFR[1]  - Processor Feature Register"},
                                         {32, 0x048, "ID_DFR     - Debug Feature Register"},
                                         {32, 0x04C, "ID_AFR     - Auxiliary Feature Register"},
                                         {32, 0x050, "ID_MFR[0]  - Memory Model Feature Register"},
                                         {32, 0x054, "ID_MFR[1]  - Memory Model Feature Register"},
                                         {32, 0x058, "ID_MFR[2]  - Memory Model Feature Register"},
                                         {32, 0x05C, "ID_MFR[3]  - Memory Model Feature Register"},
                                         {32, 0x060, "ID_ISAR[0] - Instruction Set Attributes Register"},
                                         {32, 0x064, "ID_ISAR[1] - Instruction Set Attributes Register"},
                                         {32, 0x068, "ID_ISAR[2] - Instruction Set Attributes Register"},
                                         {32, 0x06C, "ID_ISAR[3] - Instruction Set Attributes Register"},
                                         {32, 0x070, "ID_ISAR[4] - Instruction Set Attributes Register"},
                                         {32, 0x078, "CLIDR  - Cache Level ID register"},
                                         {32, 0x07C, "CTR    - Cache Type register"},
                                         {32, 0x080, "CCSIDR - Cache Size ID Register"},
                                         {32, 0x084, "CSSELR - Cache Size Selection Register"},
                                         {32, 0x088, "CPACR  - Coprocessor Access Control Register"},
                                         {32, 0x240, "MVFR0  - Media and VFP Feature Register 0"},
                                         {32, 0x244, "MVFR1  - Media and VFP Feature Register 1"},
                                         {32, 0x248, "MVFR2  - Media and VFP Feature Register 2"},
                                         {32, 0x290, "ITCMCR - Instruction Tightly-Coupled Memory Control Register"},
                                         {32, 0x294, "DTCMCR - Data Tightly-Coupled Memory Control Registers"},
                                         {32, 0x298, "AHBPCR - AHBP Control Register"},
                                         {32, 0x29C, "CACR   - L1 Cache Control Register"},
                                         {32, 0x2A0, "AHBSCR - AHB Slave Control Register"},
                                         {32, 0x2A8, "ABFSR  - Auxiliary Bus Fault Status Register"}};

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
static void EXCEPTION_ConfigurableFaultStatusRegisterPrint(void)
{
    uint32_t cfsr = SCB->CFSR;

    /* Configurable Fault Status Register */
    EXCEPTION_PRINTF("0x%08X - CFSR     - Configurable Fault Status Register\n\r", cfsr);
    /* MemManage Fault Status Register (part of SCB Configurable Fault Status Register) */
    if (cfsr & SCB_CFSR_IACCVIOL_Msk) /* SCB CFSR (MMFSR): IACCVIOL Mask */
    {
        EXCEPTION_PRINTF(
            " > The processor attempted an instruction fetch from a location that does not permit execution\n\r");
    }
    if (cfsr & SCB_CFSR_DACCVIOL_Msk) /* SCB CFSR (MMFSR): DACCVIOL Mask */
    {
        EXCEPTION_PRINTF(
            " > The processor attempted a load or store at a location that does not permit the operation\n\r");
    }
    if (cfsr & SCB_CFSR_MSTKERR_Msk) /* SCB CFSR (MMFSR): MSTKERR Mask */
    {
        EXCEPTION_PRINTF(" > Unstack for an exception return has caused one or more access violations\n\r");
    }
    if (cfsr & SCB_CFSR_MUNSTKERR_Msk) /* SCB CFSR (MMFSR): MUNSTKERR Mask */
    {
        EXCEPTION_PRINTF(" > Stacking for an exception entry has caused one or more access violations\n\r");
    }
    if (cfsr & SCB_CFSR_MLSPERR_Msk) /* SCB CFSR (MMFSR): MLSPERR Mask */
    {
        EXCEPTION_PRINTF(" > A MemManage fault occurred during floating-point lazy state preservation\n\r");
    }
    if (cfsr & SCB_CFSR_MMARVALID_Msk) /* SCB CFSR (MMFSR): MMARVALID Mask */
    {
        EXCEPTION_PRINTF(" > MMAR holds a valid fault address: ");
        EXCEPTION_PRINTF("0x%08X - MMFAR    - MemManage Fault Address Register\n\r", SCB->MMFAR);
    }

    /* BusFault Status Register (part of SCB Configurable Fault Status Register) */
    if (cfsr & SCB_CFSR_IBUSERR_Msk) /* SCB CFSR (BFSR): IBUSERR Mask */
    {
        EXCEPTION_PRINTF(" > Instruction bus error\n\r");
    }
    if (cfsr & SCB_CFSR_PRECISERR_Msk) /* SCB CFSR (BFSR): PRECISERR Mask */
    {
        EXCEPTION_PRINTF(
            " > A data bus error has occurred, PC value stacked for the exception return points to the instruction "
            "that caused the fault\n\r");
    }
    if (cfsr & SCB_CFSR_IMPRECISERR_Msk) /* SCB CFSR (BFSR): IMPRECISERR Mask */
    {
        EXCEPTION_PRINTF(
            " > A data bus error has occurred, return address in the stack frame is not related to the instruction "
            "that caused the error\n\r");
    }
    if (cfsr & SCB_CFSR_UNSTKERR_Msk) /* SCB CFSR (BFSR): UNSTKERR Mask */
    {
        EXCEPTION_PRINTF(" > Unstack for an exception return has caused one or more BusFaults\n\r");
    }
    if (cfsr & SCB_CFSR_STKERR_Msk) /* SCB CFSR (BFSR): STKERR Mask */
    {
        EXCEPTION_PRINTF(" > Stacking for an exception entry has caused one or more BusFaults\n\r");
    }
    if (cfsr & SCB_CFSR_LSPERR_Msk) /* SCB CFSR (BFSR): LSPERR Mask */
    {
        EXCEPTION_PRINTF(" > A bus fault occurred during floating-point lazy state preservation\n\r");
    }
    if (cfsr & SCB_CFSR_BFARVALID_Msk) /* SCB CFSR (BFSR): BFARVALID Mask */
    {
        EXCEPTION_PRINTF(" > BFAR holds a valid fault address: ");
        EXCEPTION_PRINTF("0x%08X - BFAR     - BusFault Address Register\n\r", SCB->BFAR);
    }

    /* UsageFault Status Register (part of SCB Configurable Fault Status Register) */
    if (cfsr & SCB_CFSR_UNDEFINSTR_Msk) /* SCB CFSR (UFSR): UNDEFINSTR Mask */
    {
        EXCEPTION_PRINTF(" > The processor has attempted to execute an undefined instruction\n\r");
    }
    if (cfsr & SCB_CFSR_INVSTATE_Msk) /* SCB CFSR (UFSR): INVSTATE Mask */
    {
        EXCEPTION_PRINTF(
            " > The processor has attempted to execute an instruction that makes illegal use of the EPSR\n\r");
    }
    if (cfsr & SCB_CFSR_INVPC_Msk) /* SCB CFSR (UFSR): INVPC Mask */
    {
        EXCEPTION_PRINTF(" > The processor has attempted an illegal load of EXC_RETURN to the PC\n\r");
    }
    if (cfsr & SCB_CFSR_NOCP_Msk) /* SCB CFSR (UFSR): NOCP Mask */
    {
        EXCEPTION_PRINTF(" > The processor has attempted to access a coprocessor\n\r");
    }
    if (cfsr & SCB_CFSR_UNALIGNED_Msk) /* SCB CFSR (UFSR): UNALIGNED Mask */
    {
        EXCEPTION_PRINTF(" > The processor has made an unaligned memory access\n\r");
    }
    if (cfsr & SCB_CFSR_DIVBYZERO_Msk) /* SCB CFSR (UFSR): DIVBYZERO Mask */
    {
        EXCEPTION_PRINTF(" > The processor has executed an SDIV or UDIV instruction with a divisor of 0\n\r");
    }
}

static void EXCEPTION_HardFaultStatusRegisterPrint(void)
{
    uint32_t hfsr = SCB->HFSR;
    /* HardFault Status Register */
    EXCEPTION_PRINTF("0x%08X - HFSR     - HardFault Status Register\n\r", hfsr);

    /* SCB Hard Fault Status Register Definitions (part of SCB Configurable Fault Status Register) */
    if (hfsr & SCB_HFSR_VECTTBL_Msk) /* SCB HFSR: VECTTBL Mask */
    {
        EXCEPTION_PRINTF(" > BusFault on vector table read\n\r");
    }
    if (hfsr & SCB_HFSR_FORCED_Msk) /* SCB HFSR: FORCED Mask */
    {
        EXCEPTION_PRINTF(" > Forced HardFault\n\r");
    }
    if (hfsr & SCB_HFSR_DEBUGEVT_Msk) /* SCB HFSR: DEBUGEVT Mask */
    {
        EXCEPTION_PRINTF(
            " > Reserved for Debug use. When writing to the register you must write 1 to this bit, otherwise behavior "
            "is unpredictable\n\r");
    }
}

static void EXCEPTION_HardFaultRegisterPrint(void)
{
    (void)EXCEPTION_ConfigurableFaultStatusRegisterPrint();
    (void)EXCEPTION_HardFaultStatusRegisterPrint();
    for (uint32_t i = 0; i < ARRAY_SIZE(scb_data_text); i++)
    {
        if (32 == scb_data_text[i].type)
        {
            EXCEPTION_PRINTF("0x%08X - %s\n\r", *(uint32_t *)(SCB_BASE + scb_data_text[i].offset),
                             scb_data_text[i].str);
        }
        else if (8 == scb_data_text[i].type)
        {
            EXCEPTION_PRINTF("0x%08X - %s\n\r", *(uint8_t *)(SCB_BASE + scb_data_text[i].offset), scb_data_text[i].str);
        }
    }
}
#endif

static void EXCEPTION_StackFramePrint(void)
{
#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
    EXCEPTION_PRINTF("\n\r--- Stack frame -------------------------------------------------------\n\r");
    EXCEPTION_PRINTF(" IPSR= 0x%08X >>>>> ", g_exceptionStackStruct->IPSR);
    switch (g_exceptionStackStruct->IPSR & 0xFF)
    {
        case 0:
            EXCEPTION_PRINTF("Thread mode");
            break;
        case 1:
            EXCEPTION_PRINTF("Reserved 1");
            break;
        case 2:
            EXCEPTION_PRINTF("NMI");
            break;
        case 3:
            EXCEPTION_PRINTF("HardFault");
            break;
        case 4:
            EXCEPTION_PRINTF("MemManage");
            break;
        case 5:
            EXCEPTION_PRINTF("BusFault");
            break;
        case 6:
            EXCEPTION_PRINTF("UsageFault");
            break;
        case 7:
            EXCEPTION_PRINTF("Reserved 7");
            break;
        case 8:
            EXCEPTION_PRINTF("Reserved 8");
            break;
        case 9:
            EXCEPTION_PRINTF("Reserved 9");
            break;
        case 10:
            EXCEPTION_PRINTF("Reserved 10");
            break;
        case 11:
            EXCEPTION_PRINTF("SVCall");
            break;
        case 12:
            EXCEPTION_PRINTF("Reserved for Debug");
            break;
        case 13:
            EXCEPTION_PRINTF("Reserved 13");
            break;
        case 14:
            EXCEPTION_PRINTF("PendSV");
            break;
        case 15:
            EXCEPTION_PRINTF("SysTick");
            break;
        default:
            EXCEPTION_PRINTF("IRQ%d", (g_exceptionStackStruct->IPSR & 0xFF) - 16);
            break;
    }
    EXCEPTION_PRINTF(" <<<<<\n\r");

    EXCEPTION_PRINTF(" R0  = 0x%08X   ", g_exceptionStackStruct->R0);
    EXCEPTION_PRINTF(" R1  = 0x%08X   ", g_exceptionStackStruct->R1);
    EXCEPTION_PRINTF(" R2  = 0x%08X   ", g_exceptionStackStruct->R2);
    EXCEPTION_PRINTF(" R3  = 0x%08X\n\r", g_exceptionStackStruct->R3);
    EXCEPTION_PRINTF(" R12 = 0x%08X   ", g_exceptionStackStruct->R12);
    EXCEPTION_PRINTF(" LR  = 0x%08X   ", g_exceptionStackStruct->LR);
    EXCEPTION_PRINTF(" PC  = 0x%08X   ", g_exceptionStackStruct->PC);
    EXCEPTION_PRINTF(" PSR = 0x%08X\n\r", g_exceptionStackStruct->xPSR);
    EXCEPTION_PRINTF(" SP (when faulted)  = 0x%08X\n\r", g_exceptionStackStruct->SP);
    EXCEPTION_PRINTF("\n--- System Control Block (SCB) ----------------------------------------\n\r");
    (void)EXCEPTION_HardFaultRegisterPrint();
    while (g_exceptionStackStruct->IPSR & 0xFF)
    {
    }

#endif
}

void NMI_Handler(void)
#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
{
    EXCEPTION_DataPrint();
}
#else
{
    while (1)
    {
    }
}
#endif

#if (defined(__MCUXPRESSO) && defined(__SEMIHOST_HARDFAULT_DISABLE)) || (!defined(__MCUXPRESSO))
void HardFault_Handler(void)
#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
{
    EXCEPTION_DataPrint();
}
#else
{
    while (1)
    {
    }
}
#endif
#endif

void MemManage_Handler(void)
#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
{
    EXCEPTION_DataPrint();
}
#else
{
    while (1)
    {
    }
}
#endif

void BusFault_Handler(void)
#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
{
    EXCEPTION_DataPrint();
}
#else
{
    while (1)
    {
    }
}
#endif

void UsageFault_Handler(void)
#if (defined(EXCEPTION_HANDLING_LOG_ENABLE) && (EXCEPTION_HANDLING_LOG_ENABLE == 1U))
{
    EXCEPTION_DataPrint();
}
#else
{
    while (1)
    {
    }
}
#endif

void EXCEPTION_CopyToStack(void)
{
    /* Push SP (when faulted) and IPSR to MSP and save MSP contain the exception Stack Struct to
     * g_exceptionStackStruct*/
    COPY_TO_STACK();
} 

/*!
 * @brief Exception data print function.
 *
 * This function should be called at first place in the ISR where the R0-R3, R12, LR, PC and xPSR
 * have been saved in the stack by ARM,
 * so that the SP and IPSR could be saved right after them. By default, exception handling module will
 * overwrite the following ISR as the reference:
 * NMI_Handler, HardFault_Handler, MemManage_Handler, BusFault_Handler, UsageFault_Handler.
 * The users can refer to these ISRs to have their owner ISRs to output the stack frame information.
 *
 * At last, all the stack frame information would be output from UART instance initialized in the debug
 * console if EXCEPTION_HANDLING_LOG_ENABLE is set to 1.
 * USB CDC could not be supported.
 * SWO is not supported yet.
 * If no UART instance is initialized because debug console is not enabled or there is no HW UART instance available,
 * the users need to route EXCEPTION_PRINTF to an available IO function to output the stack frame information.
 * This function should NOT be called in the task context.
 *
 * @note For MCUXpresso IDE, make sure unselect Include semihost Hardfault handler feature on SDK import wizard,
 * otherwise HardFault_Handler function of semihost_hardfault.c project will be used on MCUXpresso project.
 * Make sure __SEMIHOST_HARDFAULT_DISABLE Macro is defined in if want to use  HardFault_Handler function
 * in exception_handling.c.
 *
 * @retval No return vaule.
 */

void EXCEPTION_DataPrint(void)
{
    /* Here set g_exceptionStackStruct to 0x1 to avoid compiler optimization on some compiler */
    g_exceptionStackStruct = (exception_stack_data_t *)0x1;
    (void)EXCEPTION_CopyToStack();
    (void)EXCEPTION_StackFramePrint();
}
