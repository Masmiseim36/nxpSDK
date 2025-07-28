/*
 * Copyright 2024 NXP
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
 * Include
 ************************************************************************************/

#include <stdbool.h>
#include "fsl_common.h"
#include "fsl_device_registers.h"
#include "EmbeddedTypes.h"
#include "fwk_debug.h"
#include "fwk_platform_definitions.h"

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/

/* From NBU core, Set this flag so the coredump data is stored in SW LL RAM log */
//#define gDBG_LogInLinkLayerDebugStructEnabled_d 1

#if defined(gDBG_LogInLinkLayerDebugStructEnabled_d) && (gDBG_LogInLinkLayerDebugStructEnabled_d == 1)
#include "ll_types.h"
#include "ble_debug_struct.h"
/* No debug console on NBU core on KW45 */
#undef PRINTF
#define PRINTF(...)

static DEBUG_STRUCT *debug_info = (DEBUG_STRUCT *)(0xB0000000);
#else
/* Definition for PRINTF */
#include "fsl_debug_console.h"
#endif /* gDBG_LogInLinkLayerDebugStructEnabled_d */

int sys_debug_panic_triggered = 0;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/**
 * HardFaultHandler:
 * This is called from the HardFault_HandlerAsm with a pointer the Fault stack
 * as the parameter. We can then read the values from the stack and place them
 * into local variables for ease of reading.
 * We then read the various Fault Status and Address Registers to help decode
 * cause of the fault.
 * The function ends with a BKPT instruction to force control back into the debugger
 */
void HardFaultHandler(unsigned long *hardfault_args)
{
    volatile unsigned int stacked_r0;
    volatile unsigned int stacked_r1;
    volatile unsigned int stacked_r2;
    volatile unsigned int stacked_r3;
    volatile unsigned int stacked_r4;
    volatile unsigned int stacked_r5;
    volatile unsigned int stacked_r6;
    volatile unsigned int stacked_r7;
    volatile unsigned int stacked_r8;
    volatile unsigned int stacked_r9;
    volatile unsigned int stacked_r10;
    volatile unsigned int stacked_r11;
    volatile unsigned int stacked_r12;
    volatile unsigned int stacked_lr;
    volatile unsigned int stacked_pc;
    volatile unsigned int stacked_psp;
    volatile unsigned int stacked_psr;
    volatile unsigned int EXEC_RETURN;

    stacked_r0  = ((unsigned int)hardfault_args[0]);
    stacked_r1  = ((unsigned int)hardfault_args[1]);
    stacked_r2  = ((unsigned int)hardfault_args[2]);
    stacked_r3  = ((unsigned int)hardfault_args[3]);
    stacked_r12 = ((unsigned int)hardfault_args[4]);
    stacked_lr  = ((unsigned int)hardfault_args[5]);
    stacked_pc  = ((unsigned int)hardfault_args[6]);
    stacked_psr = ((unsigned int)hardfault_args[7]);

    __asm volatile(" mov %0, r4" : "=r"(stacked_r4));
    __asm volatile(" mov %0, r5" : "=r"(stacked_r5));
    __asm volatile(" mov %0, r6" : "=r"(stacked_r6));
    __asm volatile(" mov %0, r7" : "=r"(stacked_r7));
    __asm volatile(" mov %0, r8" : "=r"(stacked_r8));
    __asm volatile(" mov %0, r9" : "=r"(stacked_r9));
    __asm volatile(" mov %0, r10" : "=r"(stacked_r10));
    __asm volatile(" mov %0, r11" : "=r"(stacked_r11));

    __asm volatile(" mov %0, lr" : "=r"(EXEC_RETURN));

    /* need to remove 8 stacked words from sp to get the initial SP value when the afault occurred */
    stacked_psp = (unsigned int)hardfault_args + (8 * 4);

    /* Dump the cortex register first in case of issues in the logging dump */
    PRINTF("\r\n============\r\n");
    PRINTF("HardFault from pc    = 0x%08x (%d)\r\n", stacked_pc, stacked_pc);

    PRINTF("r0    = 0x%08x\r\n", stacked_r0);
    PRINTF("r1    = 0x%08x\r\n", stacked_r1);
    PRINTF("r2    = 0x%08x\r\n", stacked_r2);
    PRINTF("r3    = 0x%08x\r\n", stacked_r3);

    PRINTF("r4    = 0x%08x\r\n", stacked_r4);
    PRINTF("r5    = 0x%08x\r\n", stacked_r5);
    PRINTF("r6    = 0x%08x\r\n", stacked_r6);
    PRINTF("r7    = 0x%08x\r\n", stacked_r7);
    PRINTF("r8    = 0x%08x\r\n", stacked_r8);
    PRINTF("r9    = 0x%08x\r\n", stacked_r9);
    PRINTF("r10   = 0x%08x\r\n", stacked_r10);
    PRINTF("r11   = 0x%08x\r\n", stacked_r11);

    PRINTF("r12   = 0x%08x\r\n", stacked_r12);
    PRINTF("lr    = 0x%08x (%d)\r\n", stacked_lr, stacked_lr);
    PRINTF("psp   = 0x%08x\r\n", stacked_psp);
    PRINTF("psr   = 0x%08x\r\n\n", stacked_psr);

#if defined(gDBG_LogInLinkLayerDebugStructEnabled_d) && (gDBG_LogInLinkLayerDebugStructEnabled_d == 1)
    debug_info->dbg_info.cust.u.dbg_exception.pc  = stacked_pc;
    debug_info->dbg_info.cust.u.dbg_exception.lr  = stacked_lr;
    debug_info->dbg_info.cust.u.dbg_exception.psp = stacked_psp;
    debug_info->dbg_info.cust.u.dbg_exception.psr = stacked_psr;
    debug_info->dbg_info.cust.u.dbg_exception.r0  = stacked_r0;
    debug_info->dbg_info.cust.u.dbg_exception.r1  = stacked_r1;
    debug_info->dbg_info.cust.u.dbg_exception.r2  = stacked_r2;
    debug_info->dbg_info.cust.u.dbg_exception.r3  = stacked_r3;
    debug_info->dbg_info.cust.u.dbg_exception.r4  = stacked_r4;
    debug_info->dbg_info.cust.u.dbg_exception.r5  = stacked_r5;
    debug_info->dbg_info.cust.u.dbg_exception.r6  = stacked_r6;
    debug_info->dbg_info.cust.u.dbg_exception.r7  = stacked_r7;
    debug_info->dbg_info.cust.u.dbg_exception.r8  = stacked_r8;
    debug_info->dbg_info.cust.u.dbg_exception.r9  = stacked_r9;
    debug_info->dbg_info.cust.u.dbg_exception.r10 = stacked_r10;
    debug_info->dbg_info.cust.u.dbg_exception.r11 = stacked_r11;
    debug_info->dbg_info.cust.u.dbg_exception.r12 = stacked_r12;
#endif /* gDBG_LogInLinkLayerDebugStructEnabled_d */

#if (__CORTEX_M == 33) || (__CORTEX_M == 3)

#if defined(gDBG_LogInLinkLayerDebugStructEnabled_d) && (gDBG_LogInLinkLayerDebugStructEnabled_d == 1)
    debug_info->dbg_info.cust.exception_id         = __get_IPSR();
    debug_info->dbg_info.cust.u.dbg_exception.cfsr = SCB->CFSR;
#endif

    PRINTF("_CFSR = 0x%08x (Configurable Fault Status Register: UFSR|BFSR|MMSR)\r\n", SCB->CFSR);
    PRINTF("_HFSR = 0x%08x (Hard Fault Status Register)\r\n", SCB->HFSR);
    PRINTF("_DFSR = 0x%08x (Debug Fault Status Register)\r\n", SCB->DFSR);
    PRINTF("_AFSR = 0x%08x (Auxiliary Fault Status Register)\r\n\n", SCB->AFSR);
    PRINTF("_SHCSR= 0x%08x (System Handler Control and State Register)\r\n", (unsigned int)SCB->SHCSR);

    if ((SCB->CFSR & SCB_CFSR_MMARVALID_Msk) != 0U)
    {
        PRINTF("_MMAR = 0x%08x (MemManage Fault Address Register)\r\n", SCB->MMFAR);
#if defined(gDBG_LogInLinkLayerDebugStructEnabled_d) && (gDBG_LogInLinkLayerDebugStructEnabled_d == 1)
        debug_info->dbg_info.cust.u.dbg_exception.u.mmfar = SCB->MMFAR;
#endif
    }
    if ((SCB->CFSR & SCB_CFSR_BFARVALID_Msk) != 0U)
    {
        PRINTF("_BFAR = 0x%08x (Bus Fault Address Register)\r\n", SCB->BFAR);
#if defined(gDBG_LogInLinkLayerDebugStructEnabled_d) && (gDBG_LogInLinkLayerDebugStructEnabled_d == 1)
        debug_info->dbg_info.cust.u.dbg_exception.u.bfar = SCB->BFAR;
#endif
    }
    if ((SCB->CFSR & SCB_CFSR_DIVBYZERO_Msk) != 0U)
    {
        PRINTF("Division by zero\r\n");
    }
    if ((SCB->CFSR & SCB_CFSR_UNALIGNED_Msk) != 0U)
    {
        PRINTF("Unaligned access\r\n");
    }

    PRINTF("Exception_id = 0x%08x \r\n", __get_IPSR());
#endif

    PRINTF("EXEC_RETURN = 0x%08x (%d current lr)\r\n", EXEC_RETURN, EXEC_RETURN);
    PRINTF("comming from %s\r\n", (EXEC_RETURN & 0x4) ? "Thread mode (Process stack)" : "Handler mode (Main Stack)");

    /* If gLoggingActive_d is set, Dump the log now */
    DBG_LOG_DUMP();

    /* Avoid recursive panic/fault */
    if (sys_debug_panic_triggered < 1)
    {
        sys_debug_panic_triggered++;
        /* Print diagnostics */
        sys_get_interrupt_status();
        sys_dump_current_callstack((unsigned int *)stacked_psp);
        sys_get_task_stats();
        sys_callstack();
        //sys_memory_status(MEM_STAT_ALL);
        PRINTF("\n");
    }

    while (true)
    {
    }
}

#if ((__CORTEX_M == 33) || (__CORTEX_M == 3))
void HardFault_Handler(void)
{
    __asm volatile(
        "movs r0, #4\t\n"
        "mov  r1, lr\t\n"
        "tst  r0, r1\t\n" /* Check EXC_RETURN[2] */
        "beq 1f\t\n"
        "mrs r0, psp\t\n"
        "ldr r1,=HardFaultHandler\t\n"
        "bx r1\t\n"
        "1:mrs r0,msp\t\n"
        "ldr r1,=HardFaultHandler\t\n"
        "bx r1\t\n");
}

void NMI_Handler(void)
{
    __asm volatile("  b HardFault_Handler \n");
}

void MemManage_Handler(void)
{
    __asm volatile("  b HardFault_Handler \n");
}

void BusFault_Handler(void)
{
    __asm volatile(" b HardFault_Handler \n");
}

void UsageFault_Handler(void)
{
    __asm volatile(" b HardFault_Handler \n");
}
#endif

/*============================================================================
                           LOCAL CONSTANTS
=============================================================================*/

#define SYS_CALLSTACK_DEPTH         300
#define SYS_DEBUG_MAX_TASKS_NB      15
#define SYS_DEBUG_CALLSTACK_KEYWORD ">>>"

/*============================================================================
                            LOCAL TYPEDEFS
=============================================================================*/

/*============================================================================
                              LOCAL MACROS
=============================================================================*/

/*============================================================================
                         LOCAL FUNCTION PROTOTYPES
=============================================================================*/

/*============================================================================
                             LOCAL VARIABLES
=============================================================================*/

#if defined(SDK_OS_FREE_RTOS)
#include "fsl_os_abstraction.h"
#include "FreeRTOS.h"
#include "task.h"
uint8_t TaskStatusList[SYS_DEBUG_MAX_TASKS_NB * sizeof(TaskStatus_t)];
#endif
//extern unsigned int __text_start__;
//extern unsigned int __text_end__;
extern unsigned int __StackTop;

#define _STACK_TOP  __StackTop
#define _TEXT_START gPlatformFlashStartAddress_c
#define _TEXT_END   gPlatformFlashEndAddress_c

/*============================================================================
                            GLOBAL VARIABLES
=============================================================================*/

/*============================================================================
                             LOCAL FUNCTIONS
=============================================================================*/

/* Parses a stack and dump addresses that are possibly part of the call stack
 * (each address in the text range)
 */
static void sys_dump_callstack(unsigned int stack_ptr, unsigned int stack_top)
{
    unsigned int *addr;

    PRINTF(SYS_DEBUG_CALLSTACK_KEYWORD);

    /* Search for possible backtrace addresses in the stack */
    for (addr = (unsigned int *)stack_ptr; addr < (unsigned int *)stack_top; addr++)
    {
        unsigned int val = *addr;
        if ((val > (unsigned int)_TEXT_START) && (val < (unsigned int)_TEXT_END))
        {
            PRINTF("0x%x ", val);
        }
    }
    PRINTF("\n");
}

static void sys_dump_callstack_ext(unsigned int stack_pointer, unsigned int *task_hdl)
{
    /* During task creation, the stack is allocated before the task context in FreeRTOS,
     * The task context does not contain the top of the stack neither the stack size.
     * to get the top of the stack, Assumption is that the task context is at the top of the stack.
     */
    unsigned int topOfStack =
        (((stack_pointer + SYS_CALLSTACK_DEPTH) > (unsigned int)task_hdl) && (stack_pointer < (unsigned int)task_hdl)) ?
            (unsigned int)task_hdl :
            (stack_pointer + SYS_CALLSTACK_DEPTH);

    /* Ensure that computed top of stack is not outside data memory (case for exception context) */
    if (topOfStack > (unsigned int)&_STACK_TOP)
    {
        //topOfStack = (unsigned int)&_STACK_TOP;
    }

    PRINTF("CallStack from sp=0x%x to top=0x%x (approximate):\n", stack_pointer, topOfStack);
    sys_dump_callstack(stack_pointer, topOfStack);
    PRINTF("\n");
}

static void sys_dump_exception_callstack(void)
{
    volatile unsigned int exception_sp;
    __asm volatile(" mrs r0, msp");
    __asm volatile(" mov %0, r0" : "=r"(exception_sp));

    PRINTF("Exception Callstack from 0x%x to 0x%x\n", exception_sp, (unsigned int)&_STACK_TOP);
    sys_dump_callstack(exception_sp, (unsigned int)&_STACK_TOP);
    PRINTF("\n");
}

/*============================================================================
                            GLOBAL FUNCTIONS
=============================================================================*/

/* Dump an arbitrary number of calls in the stack */
void sys_dump_current_callstack(unsigned int *stack_pointer)
{
    unsigned int *task_hdl = (unsigned int *)OSA_TaskGetCurrentHandle();

    PRINTF("\nActive thread : ");
    sys_dump_callstack_ext((unsigned int)stack_pointer, task_hdl);
}

#define IRQN_TYPE_MAX DSP_IRQn

void sys_get_interrupt_status(void)
{
    unsigned int IRQn;
    bool_t       irq_pending_running = FALSE;

    PRINTF("\n==========INTERRUPT STATUS==========\n");

    for (IRQn = 0; IRQn < IRQN_TYPE_MAX; IRQn++)
    {
        if (NVIC_GetPendingIRQ(IRQn))
        {
            PRINTF("IRQ pending: %d\n", IRQn);
            irq_pending_running = TRUE;
        }
        if (NVIC_GetActive(IRQn))
        {
            PRINTF("IRQ active : %d\n", IRQn);
            irq_pending_running = TRUE;
        }
    }

    if (!irq_pending_running)
    {
        PRINTF("No IRQ pending or running\n");
    }
}

/* TODO : this function shall be moved to Freertos port */
/* external definition for a private FreeRTOS function implemented in task.h */
extern TaskHandle_t xTaskGetHandle(const char *pcNameToQuery);

/* function created from void vTaskGetRunTimeStats( char *pcWriteBuffer, size_t bufSize ) */
void sys_get_task_stats(void)
{
    static const char task_state_char[] = {
        'r', /* Running, never returned by uxTaskGetSystemState */
        'R', /* Ready */
        'B', /* Blocked */
        'S', /* Suspended */
        'D', /* Deleted */
        'I', /* Invalid */
    };

    TaskStatus_t        *pxTaskStatusArray = (TaskStatus_t *)&TaskStatusList;
    volatile UBaseType_t uxArraySize, x;
    uint32_t             ulTotalRunTime;

    PRINTF("==========TASK STATUS==========\n");

    {
        /* Generate raw status information about each task. */
        uxArraySize = uxTaskGetSystemState(pxTaskStatusArray, SYS_DEBUG_MAX_TASKS_NB, &ulTotalRunTime);
        if (uxArraySize == SYS_DEBUG_MAX_TASKS_NB)
        {
            PRINTF("WARNING: SYS_DEBUG_MAX_TASKS_NB too small - task list incomplete !!\n");
        }
        /* For percentage calculations.*/
        ulTotalRunTime /= 100UL;

        /* For each populated position in the pxTaskStatusArray array,
           format the raw data as human readable ASCII data */
        PRINTF("%s\t%s  %s  %s     %s   %s      %s\n", "Task Name", "State", "PRI", "Base_PRI", "Time", "Watermark",
               "SP");
        for (x = 0; x < uxArraySize; x++)
        {
            unsigned int *task_hdl;
            int           task_state;

            task_state = pxTaskStatusArray[x].eCurrentState;
            if (task_state > eInvalid)
            {
                task_state = eInvalid;
            }

            task_hdl = (unsigned int *)xTaskGetHandle((const char *)pxTaskStatusArray[x].pcTaskName);

            /* If the percentage is zero here then the task has
               consumed less than 1% of the total run time. */
            PRINTF(
                "%s%s\t%c\t%d\t%d\t%d\t%d\t%d\n", pxTaskStatusArray[x].pcTaskName,
                (strlen(pxTaskStatusArray[x].pcTaskName) < 8) ? "\t" : "", /* Adjust tabulation */
                task_state_char[task_state], pxTaskStatusArray[x].uxCurrentPriority,
                pxTaskStatusArray[x].uxBasePriority, pxTaskStatusArray[x].ulRunTimeCounter,
                pxTaskStatusArray[x].usStackHighWaterMark,
                /* by construction, first word in taskHandle is the last stack pointer saved in last context switch */
                task_hdl[0]);
        }

        PRINTF("\n==========Thread Callstacks========== (code start=0x%x code end=0x%x)\n", (unsigned int)_TEXT_START,
               (unsigned int)_TEXT_END);
        for (x = 0; x < uxArraySize; x++)
        {
            unsigned int *task_hdl = (void *)xTaskGetHandle((const char *)pxTaskStatusArray[x].pcTaskName);

            /* Current stack pointer of the thread is given by the first word in task handle */
            unsigned int CurrentSP = (unsigned int)task_hdl[0];

            PRINTF("%s : ", pxTaskStatusArray[x].pcTaskName);
            sys_dump_callstack_ext(CurrentSP, task_hdl);
        }

        sys_dump_exception_callstack();

        PRINTF(
            "Command for parsing a stack:\n>arm-none-eabi-addr2line.exe -e <your_project>.elf "
            "<paste_stack_line_above>\nadd option -f to show functions\n\n");
    }
}

void sys_callstack(void)
{
    volatile unsigned int stacked_r13; /* R13 = Stack pointer */
    __asm volatile(" mov %0, r13" : "=r"(stacked_r13));

    /* Print diagnostics */
    sys_dump_current_callstack((unsigned int *)stacked_r13);
}