/*
************************************************************************************************************************
*                                                      uC/OS-III
*                                                 The Real-Time Kernel
*
*                                  (c) Copyright 2009-2015; Micrium, Inc.; Weston, FL
*                           All rights reserved.  Protected by international copyright laws.
*
*                                                   TASK MANAGEMENT
*
* File    : OS_TASK.C
* By      : JJL
* Version : V3.05.01
*
* LICENSING TERMS:
* ---------------
*           uC/OS-III is provided in source form for FREE short-term evaluation, for educational use or
*           for peaceful research.  If you plan or intend to use uC/OS-III in a commercial application/
*           product then, you need to contact Micrium to properly license uC/OS-III for its use in your
*           application/product.   We provide ALL the source code for your convenience and to help you
*           experience uC/OS-III.  The fact that the source is provided does NOT mean that you can use
*           it commercially without paying a licensing fee.
*
*           Knowledge of the source code may NOT be used to develop a similar product.
*
*           Please help us continue to provide the embedded community with the finest software available.
*           Your honesty is greatly appreciated.
*
*           You can find our product's user manual, API reference, release notes and
*           more information at https://doc.micrium.com.
*           You can contact us at www.micrium.com.
************************************************************************************************************************
*/

#define  MICRIUM_SOURCE
#include "os.h"


#ifdef VSC_INCLUDE_SOURCE_FILE_NAMES
const  CPU_CHAR  *os_task__c = "$Id: $";
#endif

/*
************************************************************************************************************************
*                                                CHANGE PRIORITY OF A TASK
*
* Description: This function allows you to change the priority of a task dynamically.  Note that the new
*              priority MUST be available.
*
* Arguments  : p_tcb      is the TCB of the tack to change the priority for
*
*              prio_new   is the new priority
*
*              p_err      is a pointer to an error code returned by this function:
*
*                             OS_ERR_NONE                    Is the call was successful
*                             OS_ERR_OS_NOT_RUNNING          If uC/OS-III is not running yet
*                             OS_ERR_PRIO_INVALID            If the priority you specify is higher that the maximum allowed
*                                                              (i.e. >= (OS_CFG_PRIO_MAX-1)) or already in use by a kernel
*                                                              task
*                             OS_ERR_STATE_INVALID           If the task is in an invalid state
*                             OS_ERR_TASK_CHANGE_PRIO_ISR    If you tried to change the task's priority from an ISR
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_CHANGE_PRIO_EN == DEF_ENABLED)
void  OSTaskChangePrio (OS_TCB   *p_tcb,
                        OS_PRIO   prio_new,
                        OS_ERR   *p_err)
{
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    OS_PRIO  prio_high;
#endif
    CPU_SR_ALLOC();


#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((p_tcb != DEF_NULL) && (p_tcb->TaskState == OS_TASK_STATE_DEL)) {
       *p_err = OS_ERR_STATE_INVALID;
        return;
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Not allowed to call from an ISR                      */
       *p_err = OS_ERR_TASK_CHANGE_PRIO_ISR;
        return;
    }
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (prio_new == 0u) {                                        /* Cannot set to IntQueue Task priority                 */
       *p_err = OS_ERR_PRIO_INVALID;
        return;
    }
#endif

    if (prio_new >= (OS_CFG_PRIO_MAX - 1u)) {                   /* Cannot set to Idle Task priority                     */
       *p_err = OS_ERR_PRIO_INVALID;
        return;
    }

    OS_CRITICAL_ENTER();

    if (p_tcb == DEF_NULL) {                                    /* Are we changing the priority of 'self'?              */
        if (OSRunning != OS_STATE_OS_RUNNING) {
            OS_CRITICAL_EXIT();
           *p_err = OS_ERR_OS_NOT_RUNNING;
            return;
        }
        p_tcb = OSTCBCurPtr;
    }

#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    p_tcb->BasePrio = prio_new;                                 /* Update base priority                                 */

    if (p_tcb->MutexGrpHeadPtr != DEF_NULL) {                   /* Owning a mutex?                                      */
        if (prio_new > p_tcb->Prio) {
            prio_high = OS_MutexGrpPrioFindHighest(p_tcb);
            if (prio_new > prio_high) {
                prio_new = prio_high;
            }
        }
    }
#endif

    OS_TaskChangePrio(p_tcb, prio_new);

    OS_CRITICAL_EXIT_NO_SCHED();

    if (OSRunning == OS_STATE_OS_RUNNING) {
        OSSched();                                              /* Run highest priority task ready                      */
    }

   *p_err = OS_ERR_NONE;
}
#endif


/*
************************************************************************************************************************
*                                                    CREATE A TASK
*
* Description: This function is used to have uC/OS-III manage the execution of a task.  Tasks can either be created
*              prior to the start of multitasking or by a running task.  A task cannot be created by an ISR.
*
* Arguments  : p_tcb          is a pointer to the task's TCB
*
*              p_name         is a pointer to an ASCII string to provide a name to the task.
*
*              p_task         is a pointer to the task's code
*
*              p_arg          is a pointer to an optional data area which can be used to pass parameters to
*                             the task when the task first executes.  Where the task is concerned it thinks
*                             it was invoked and passed the argument 'p_arg' as follows:
*
*                                 void Task (void *p_arg)
*                                 {
*                                     for (;;) {
*                                         Task code;
*                                     }
*                                 }
*
*              prio           is the task's priority.  A unique priority MUST be assigned to each task and the
*                             lower the number, the higher the priority.
*
*              p_stk_base     is a pointer to the base address of the stack (i.e. low address).
*
*              stk_limit      is the number of stack elements to set as 'watermark' limit for the stack.  This value
*                             represents the number of CPU_STK entries left before the stack is full.  For example,
*                             specifying 10% of the 'stk_size' value indicates that the stack limit will be reached
*                             when the stack reaches 90% full.
*
*              stk_size       is the size of the stack in number of elements.  If CPU_STK is set to CPU_INT08U,
*                             'stk_size' corresponds to the number of bytes available.  If CPU_STK is set to
*                             CPU_INT16U, 'stk_size' contains the number of 16-bit entries available.  Finally, if
*                             CPU_STK is set to CPU_INT32U, 'stk_size' contains the number of 32-bit entries
*                             available on the stack.
*
*              q_size         is the maximum number of messages that can be sent to the task
*
*              time_quanta    amount of time (in ticks) for time slice when round-robin between tasks.  Specify 0 to use
*                             the default.
*
*              p_ext          is a pointer to a user supplied memory location which is used as a TCB extension.
*                             For example, this user memory can hold the contents of floating-point registers
*                             during a context switch, the time each task takes to execute, the number of times
*                             the task has been switched-in, etc.
*
*              opt            contains additional information (or options) about the behavior of the task.
*                             See OS_OPT_TASK_xxx in OS.H.  Current choices are:
*
*                                 OS_OPT_TASK_NONE            No option selected
*                                 OS_OPT_TASK_STK_CHK         Stack checking to be allowed for the task
*                                 OS_OPT_TASK_STK_CLR         Clear the stack when the task is created
*                                 OS_OPT_TASK_SAVE_FP         If the CPU has floating-point registers, save them
*                                                             during a context switch.
*                                 OS_OPT_TASK_NO_TLS          If the caller doesn't want or need TLS (Thread Local
*                                                             Storage) support for the task.  If you do not include this
*                                                             option, TLS will be supported by default.
*
*              p_err          is a pointer to an error code that will be set during this call.  The value pointer
*                             to by 'p_err' can be:
*
*                                 OS_ERR_NONE                    If the function was successful
*                                 OS_ERR_ILLEGAL_CREATE_RUN_TIME If you are trying to create the task after you called
*                                                                   OSStart()
*                                 OS_ERR_PRIO_INVALID            If the priority you specify is higher that the maximum
*                                                                   allowed (i.e. >= OS_CFG_PRIO_MAX-1) or,
*                                                                If OS_CFG_ISR_POST_DEFERRED_EN is set to 1 and you tried
*                                                                   to use priority 0 which is reserved
*                                 OS_ERR_STAT_STK_SIZE_INVALID   If the stack was overflowed during stack init
*                                 OS_ERR_STK_INVALID             If you specified a NULL pointer for 'p_stk_base'
*                                 OS_ERR_STK_SIZE_INVALID        If you specified zero for the 'stk_size'
*                                 OS_ERR_STK_LIMIT_INVALID       If you specified a 'stk_limit' greater than or equal
*                                                                   to 'stk_size'
*                                 OS_ERR_TASK_CREATE_ISR         If you tried to create a task from an ISR
*                                 OS_ERR_TASK_INVALID            If you specified a NULL pointer for 'p_task'
*                                 OS_ERR_TCB_INVALID             If you specified a NULL pointer for 'p_tcb'
*
* Returns    : none
*
* Note(s)    : 1) OSTaskCreate() will return with the error OS_ERR_STAT_STK_SIZE_INVALID when a stack overflow is detected
*                 during stack initialization. In that specific case some memory may have been corrupted. It is
*                 therefore recommended to treat OS_ERR_STAT_STK_SIZE_INVALID as a fatal error.
************************************************************************************************************************
*/

void  OSTaskCreate (OS_TCB        *p_tcb,
                    CPU_CHAR      *p_name,
                    OS_TASK_PTR    p_task,
                    void          *p_arg,
                    OS_PRIO        prio,
                    CPU_STK       *p_stk_base,
                    CPU_STK_SIZE   stk_limit,
                    CPU_STK_SIZE   stk_size,
                    OS_MSG_QTY     q_size,
                    OS_TICK        time_quanta,
                    void          *p_ext,
                    OS_OPT         opt,
                    OS_ERR        *p_err)
{
    CPU_STK_SIZE   i;
#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    OS_REG_ID      reg_nbr;
#endif
#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_ID      id;
#endif

    CPU_STK       *p_sp;
    CPU_STK       *p_stk_limit;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#ifdef OS_SAFETY_CRITICAL_IEC61508
    if (OSSafetyCriticalStartFlag == DEF_TRUE) {
       *p_err = OS_ERR_ILLEGAL_CREATE_RUN_TIME;
        return;
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* --------- CANNOT CREATE A TASK FROM AN ISR --------- */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_TASK_CREATE_ISR;
        return;
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)                          /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_tcb == DEF_NULL) {                                    /* User must supply a valid OS_TCB                      */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_TCB_INVALID;
        return;
    }
    if (p_task == 0u) {                                         /* User must supply a valid task                        */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_TASK_INVALID;
        return;
    }
    if (p_stk_base == DEF_NULL) {                               /* User must supply a valid stack base address          */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_STK_INVALID;
        return;
    }
    if (stk_size < OSCfg_StkSizeMin) {                          /* User must supply a valid minimum stack size          */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_STK_SIZE_INVALID;
        return;
    }
    if (stk_limit >= stk_size) {                                /* User must supply a valid stack limit                 */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_STK_LIMIT_INVALID;
        return;
    }
    if (prio >= OS_CFG_PRIO_MAX) {                              /* Priority must be within 0 and OS_CFG_PRIO_MAX-1      */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_PRIO_INVALID;
        return;
    }
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (prio == 0u) {
        if (p_tcb != &OSIntQTaskTCB) {
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
            TRACE_OS_TASK_CREATE_FAILED(p_tcb);                 /* Record the event.                                    */
#endif
           *p_err = OS_ERR_PRIO_INVALID;                        /* Not allowed to use priority 0                        */
            return;
        }
    }
#endif

    if (prio == (OS_CFG_PRIO_MAX - 1u)) {
#if (OS_CFG_TASK_IDLE_EN == DEF_ENABLED)
        if (p_tcb != &OSIdleTaskTCB) {
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
            TRACE_OS_TASK_CREATE_FAILED(p_tcb);                 /* Record the event.                                    */
#endif
           *p_err = OS_ERR_PRIO_INVALID;                        /* Not allowed to use same priority as idle task        */
            return;
        }
#else
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_CREATE_FAILED(p_tcb);                     /* Record the event.                                    */
#endif
       *p_err = OS_ERR_PRIO_INVALID;                            /* Not allowed to use same priority as idle task        */
        return;
#endif
    }

    OS_TaskInitTCB(p_tcb);                                      /* Initialize the TCB to default values                 */

   *p_err = OS_ERR_NONE;
                                                                /* -------------- CLEAR THE TASK'S STACK -------------- */
    if (((opt & OS_OPT_TASK_STK_CHK) != 0u) ||                  /* See if stack checking has been enabled               */
        ((opt & OS_OPT_TASK_STK_CLR) != 0u)) {                  /* See if stack needs to be cleared                     */
        if ((opt & OS_OPT_TASK_STK_CLR) != 0u) {
            p_sp = p_stk_base;
            for (i = 0u; i < stk_size; i++) {                   /* Stack grows from HIGH to LOW memory                  */
               *p_sp = 0u;                                      /* Clear from bottom of stack and up!                   */
                p_sp++;
            }
        }
    }
                                                                /* ------ INITIALIZE THE STACK FRAME OF THE TASK ------ */
#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)
    p_stk_limit = p_stk_base + stk_limit;
#else
    p_stk_limit = p_stk_base + (stk_size - 1u) - stk_limit;
#endif

    p_sp = OSTaskStkInit(p_task,
                         p_arg,
                         p_stk_base,
                         p_stk_limit,
                         stk_size,
                         opt);

#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)             /* Check if we overflown the stack during init          */
    if (p_sp < p_stk_base) {
       *p_err = OS_ERR_STAT_STK_SIZE_INVALID;
        return;
    }
#else
    if (p_sp > p_stk_base + stk_size) {
       *p_err = OS_ERR_STAT_STK_SIZE_INVALID;
        return;
    }
#endif

#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)                 /* Initialize Redzoned stack                            */
    OS_TaskStkRedzoneInit(p_stk_base, stk_size);
#endif

                                                                /* ------------ INITIALIZE THE TCB FIELDS ------------- */
#if (OS_CFG_DBG_EN == DEF_ENABLED)
    p_tcb->TaskEntryAddr = p_task;                              /* Save task entry point address                        */
    p_tcb->TaskEntryArg  = p_arg;                               /* Save task entry argument                             */
#endif

#if (OS_CFG_DBG_EN == DEF_ENABLED)
    p_tcb->NamePtr       = p_name;                              /* Save task name                                       */
#else
    (void)&p_name;
#endif

    p_tcb->Prio          = prio;                                /* Save the task's priority                             */

#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    p_tcb->BasePrio      = prio;                                /* Set the base priority                                */
#endif

    p_tcb->StkPtr        = p_sp;                                /* Save the new top-of-stack pointer                    */
#if ((OS_CFG_DBG_EN == DEF_ENABLED) || (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED))
    p_tcb->StkLimitPtr   = p_stk_limit;                         /* Save the stack limit pointer                         */
#endif

#if (OS_CFG_SCHED_ROUND_ROBIN_EN == DEF_ENABLED)
    p_tcb->TimeQuanta    = time_quanta;                         /* Save the #ticks for time slice (0 means not sliced)  */
    if (time_quanta == 0u) {
        p_tcb->TimeQuantaCtr = OSSchedRoundRobinDfltTimeQuanta;
    } else {
        p_tcb->TimeQuantaCtr = time_quanta;
    }
#else
    (void)&time_quanta;
#endif

    p_tcb->ExtPtr        = p_ext;                               /* Save pointer to TCB extension                        */
#if ((OS_CFG_DBG_EN == DEF_ENABLED) || (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED) || (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED))
    p_tcb->StkBasePtr    = p_stk_base;                          /* Save pointer to the base address of the stack        */
    p_tcb->StkSize       = stk_size;                            /* Save the stack size (in number of CPU_STK elements)  */
#endif
    p_tcb->Opt           = opt;                                 /* Save task options                                    */

#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    for (reg_nbr = 0u; reg_nbr < OS_CFG_TASK_REG_TBL_SIZE; reg_nbr++) {
        p_tcb->RegTbl[reg_nbr] = 0u;
    }
#endif

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
    OS_MsgQInit(&p_tcb->MsgQ,                                   /* Initialize the task's message queue                  */
                q_size);
#else
    (void)&q_size;
#endif

    OSTaskCreateHook(p_tcb);                                    /* Call user defined hook                               */

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_CREATE(p_tcb);                                /* Record the event.                                    */
    TRACE_OS_TASK_SEM_CREATE(p_tcb, p_name);                    /* Record the event.                                    */
#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
    TRACE_OS_TASK_MSG_Q_CREATE(&p_tcb->MsgQ, p_name);           /* Record the event.                                    */
#endif
#endif

#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    for (id = 0u; id < OS_CFG_TLS_TBL_SIZE; id++) {
        p_tcb->TLS_Tbl[id] = 0u;
    }
    OS_TLS_TaskCreate(p_tcb);                                   /* Call TLS hook                                        */
#endif
                                                                /* -------------- ADD TASK TO READY LIST -------------- */
    OS_CRITICAL_ENTER();
    OS_PrioInsert(p_tcb->Prio);
    OS_RdyListInsertTail(p_tcb);

#if (OS_CFG_DBG_EN == DEF_ENABLED)
    OS_TaskDbgListAdd(p_tcb);
#endif

    OSTaskQty++;                                                /* Increment the #tasks counter                         */

    if (OSRunning != OS_STATE_OS_RUNNING) {                     /* Return if multitasking has not started               */
        OS_CRITICAL_EXIT();
        return;
    }

    OS_CRITICAL_EXIT_NO_SCHED();

    OSSched();
}


/*
************************************************************************************************************************
*                                                     DELETE A TASK
*
* Description: This function allows you to delete a task.  The calling task can delete itself by specifying a NULL
*              pointer for 'p_tcb'.  The deleted task is returned to the dormant state and can be re-activated by
*              creating the deleted task again.
*
* Arguments  : p_tcb      is the TCB of the tack to delete
*
*              p_err      is a pointer to an error code returned by this function:
*
*                             OS_ERR_NONE                    If the call is successful
*                             OS_ERR_ILLEGAL_DEL_RUN_TIME    If you are trying to delete the task after you called
*                                                              OSStart()
*                             OS_ERR_OS_NOT_RUNNING          If uC/OS-III is not running yet
*                             OS_ERR_STATE_INVALID           If the state of the task is invalid
*                             OS_ERR_TASK_DEL_IDLE           If you attempted to delete uC/OS-III's idle task
*                             OS_ERR_TASK_DEL_INVALID        If you attempted to delete uC/OS-III's ISR handler task
*                             OS_ERR_TASK_DEL_ISR            If you tried to delete a task from an ISR
*
* Returns    : none
*
* Note(s)    : 1) 'p_err' gets set to OS_ERR_NONE before OSSched() to allow the returned err or code to be monitored even
*                 for a task that is deleting itself. In this case, 'p_err' MUST point to a global variable that can be
*                 accessed by another task.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_DEL_EN == DEF_ENABLED)
void  OSTaskDel (OS_TCB  *p_tcb,
                 OS_ERR  *p_err)
{
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    OS_TCB  *p_tcb_owner;
    OS_PRIO  prio_new;
#endif
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#ifdef OS_SAFETY_CRITICAL_IEC61508
    if (OSSafetyCriticalStartFlag == DEF_TRUE) {
       *p_err = OS_ERR_ILLEGAL_DEL_RUN_TIME;
        return;
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* See if trying to delete from ISR                     */
       *p_err = OS_ERR_TASK_DEL_ISR;
        return;
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return;
    }
#endif

#if (OS_CFG_TASK_IDLE_EN == DEF_ENABLED)
    if (p_tcb == &OSIdleTaskTCB) {                              /* Not allowed to delete the idle task                  */
       *p_err = OS_ERR_TASK_DEL_IDLE;
        return;
    }
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (p_tcb == &OSIntQTaskTCB) {                              /* Cannot delete the ISR handler task                   */
       *p_err = OS_ERR_TASK_DEL_INVALID;
        return;
    }
#endif

    if (p_tcb == DEF_NULL) {                                    /* Delete 'Self'?                                       */
        CPU_CRITICAL_ENTER();
        p_tcb  = OSTCBCurPtr;                                   /* Yes.                                                 */
        CPU_CRITICAL_EXIT();
    }

    OS_CRITICAL_ENTER();
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
             OS_RdyListRemove(p_tcb);
             break;

        case OS_TASK_STATE_SUSPENDED:
             break;

        case OS_TASK_STATE_DLY:                                 /* Task is only delayed, not on any wait list           */
        case OS_TASK_STATE_DLY_SUSPENDED:
#if (OS_CFG_TASK_TICK_EN == DEF_ENABLED)
             OS_TickListRemove(p_tcb);
#endif
             break;

        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
             switch (p_tcb->PendOn) {                           /* See what we are pending on                           */
                 case OS_TASK_PEND_ON_NOTHING:
                 case OS_TASK_PEND_ON_TASK_Q:                   /* There is no wait list for these two                  */
                 case OS_TASK_PEND_ON_TASK_SEM:
                      break;

                 case OS_TASK_PEND_ON_FLAG:                     /* Remove from wait list                                */
#if (OS_CFG_PEND_MULTI_EN == DEF_ENABLED)
                 case OS_TASK_PEND_ON_MULTI:
#endif
                 case OS_TASK_PEND_ON_Q:
                 case OS_TASK_PEND_ON_SEM:
                      OS_PendListRemove(p_tcb);
                      break;

#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
                 case OS_TASK_PEND_ON_MUTEX:
                      p_tcb_owner = ((OS_MUTEX *)p_tcb->PendDataTblPtr->PendObjPtr)->OwnerTCBPtr;
                      prio_new = p_tcb_owner->Prio;
                      OS_PendListRemove(p_tcb);
                      if ((p_tcb_owner->Prio != p_tcb_owner->BasePrio) &&
                          (p_tcb_owner->Prio == p_tcb->Prio)) { /* Has the owner inherited a priority?                  */
                          prio_new = OS_MutexGrpPrioFindHighest(p_tcb_owner);
                          prio_new = prio_new > p_tcb_owner->BasePrio ? p_tcb_owner->BasePrio : prio_new;
                      }
                      p_tcb->PendOn = OS_TASK_PEND_ON_NOTHING;

                      if (prio_new != p_tcb_owner->Prio) {
                          OS_TaskChangePrio(p_tcb_owner, prio_new);
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
                          TRACE_OS_MUTEX_TASK_PRIO_DISINHERIT(p_tcb_owner, p_tcb_owner->Prio);
#endif
                      }
                      break;
#endif

                 default:
                      break;
             }
#if (OS_CFG_TASK_TICK_EN == DEF_ENABLED)
             if ((p_tcb->TaskState == OS_TASK_STATE_PEND_TIMEOUT) ||
                 (p_tcb->TaskState == OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED)) {
                 OS_TickListRemove(p_tcb);
             }
#endif
             break;

        default:
            OS_CRITICAL_EXIT();
           *p_err = OS_ERR_STATE_INVALID;
            return;
    }

#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    if(p_tcb->MutexGrpHeadPtr != DEF_NULL) {
        OS_MutexGrpPostAll(p_tcb);
    }
#endif

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
    (void)OS_MsgQFreeAll(&p_tcb->MsgQ);                         /* Free task's message queue messages                   */
#endif

    OSTaskDelHook(p_tcb);                                       /* Call user defined hook                               */

#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_TaskDel(p_tcb);                                      /* Call TLS hook                                        */
#endif

#if (OS_CFG_DBG_EN == DEF_ENABLED)
    OS_TaskDbgListRemove(p_tcb);
#endif

    OSTaskQty--;                                                /* One less task being managed                          */

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_DEL(p_tcb);                                   /* Record the event.                                    */
#endif
#if (OS_CFG_TASK_STK_REDZONE_EN != DEF_ENABLED)                 /* Don't clear the TCB before checking the red-zone     */
    OS_TaskInitTCB(p_tcb);                                      /* Initialize the TCB to default values                 */
#endif
    p_tcb->TaskState = (OS_STATE)OS_TASK_STATE_DEL;             /* Indicate that the task was deleted                   */

    OS_CRITICAL_EXIT_NO_SCHED();

   *p_err = OS_ERR_NONE;                                        /* See Note #1.                                         */

    OSSched();                                                  /* Find new highest priority task                       */
}
#endif


/*
************************************************************************************************************************
*                                                    FLUSH TASK's QUEUE
*
* Description: This function is used to flush the task's internal message queue.
*
* Arguments  : p_tcb       is a pointer to the task's OS_TCB.  Specifying a NULL pointer indicates that you wish to
*                          flush the message queue of the calling task.
*
*              p_err       is a pointer to a variable that will contain an error code returned by this function.
*
*                              OS_ERR_NONE              Upon success
*                              OS_ERR_FLUSH_ISR         If you called this function from an ISR
*                              OS_ERR_OS_NOT_RUNNING    If uC/OS-III is not running yet
*
* Returns     : The number of entries freed from the queue
*
* Note(s)     : 1) You should use this function with great care because, when to flush the queue, you LOOSE the
*                  references to what the queue entries are pointing to and thus, you could cause 'memory leaks'.  In
*                  other words, the data you are pointing to that's being referenced by the queue entries should, most
*                  likely, need to be de-allocated (i.e. freed).
************************************************************************************************************************
*/

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
OS_MSG_QTY  OSTaskQFlush (OS_TCB  *p_tcb,
                          OS_ERR  *p_err)
{
    OS_MSG_QTY  entries;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (0u);
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Can't flush a message queue from an ISR              */
       *p_err = OS_ERR_FLUSH_ISR;
        return (0u);
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return (0u);
    }
#endif

    if (p_tcb == DEF_NULL) {                                    /* Flush message queue of calling task?                 */
        CPU_CRITICAL_ENTER();
        p_tcb = OSTCBCurPtr;
        CPU_CRITICAL_EXIT();
    }

    OS_CRITICAL_ENTER();
    entries = OS_MsgQFreeAll(&p_tcb->MsgQ);                     /* Return all OS_MSGs to the OS_MSG pool                */
    OS_CRITICAL_EXIT();
   *p_err   = OS_ERR_NONE;
    return (entries);
}
#endif


/*
************************************************************************************************************************
*                                                  WAIT FOR A MESSAGE
*
* Description: This function causes the current task to wait for a message to be posted to it.
*
* Arguments  : timeout       is an optional timeout period (in clock ticks).  If non-zero, your task will wait for a
*                            message to arrive up to the amount of time specified by this argument.
*                            If you specify 0, however, your task will wait forever or, until a message arrives.
*
*              opt           determines whether the user wants to block if the task's queue is empty or not:
*
*                                OS_OPT_PEND_BLOCKING
*                                OS_OPT_PEND_NON_BLOCKING
*
*              p_msg_size    is a pointer to a variable that will receive the size of the message
*
*              p_ts          is a pointer to a variable that will receive the timestamp of when the message was
*                            received.  If you pass a NULL pointer (i.e. (CPU_TS *)0) then you will not get the
*                            timestamp.  In other words, passing a NULL pointer is valid and indicates that you don't
*                            need the timestamp.
*
*              p_err         is a pointer to where an error message will be deposited.  Possible error
*                            messages are:
*
*                                OS_ERR_NONE               The call was successful and your task received a message.
*                                OS_ERR_OPT_INVALID        If you specified an invalid option
*                                OS_ERR_OS_NOT_RUNNING     If uC/OS-III is not running yet
*                                OS_ERR_PEND_ABORT         If the pend was aborted
*                                OS_ERR_PEND_ISR           If you called this function from an ISR and the result
*                                OS_ERR_PEND_WOULD_BLOCK   If you specified non-blocking but the queue was not empty
*                                OS_ERR_PTR_INVALID        If 'p_msg_size' is NULL
*                                OS_ERR_SCHED_LOCKED       If the scheduler is locked
*                                OS_ERR_TIMEOUT            A message was not received within the specified timeout
*                                                          would lead to a suspension
*
* Returns    : A pointer to the message received or a NULL pointer upon error.
*
* Note(s)    : 1) It is possible to receive NULL pointers when there are no errors.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
void  *OSTaskQPend (OS_TICK       timeout,
                    OS_OPT        opt,
                    OS_MSG_SIZE  *p_msg_size,
                    CPU_TS       *p_ts,
                    OS_ERR       *p_err)
{
    OS_MSG_Q     *p_msg_q;
    void         *p_void;
    CPU_SR_ALLOC();


#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (DEF_NULL);
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Can't Pend from an ISR                               */
       *p_err = OS_ERR_PEND_ISR;
        return (DEF_NULL);
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return (DEF_NULL);
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)                          /* ---------------- VALIDATE ARGUMENTS ---------------- */
    if (p_msg_size == DEF_NULL) {                               /* User must supply a valid destination for msg size    */
       *p_err = OS_ERR_PTR_INVALID;
        return (DEF_NULL);
    }
    switch (opt) {                                              /* User must supply a valid option                      */
        case OS_OPT_PEND_BLOCKING:
        case OS_OPT_PEND_NON_BLOCKING:
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;
             return (DEF_NULL);
    }
#endif

    if (p_ts != DEF_NULL) {
       *p_ts = 0u;                                              /* Initialize the returned timestamp                    */
    }

    CPU_CRITICAL_ENTER();
    p_msg_q = &OSTCBCurPtr->MsgQ;                               /* Any message waiting in the message queue?            */
    p_void  = OS_MsgQGet(p_msg_q,
                         p_msg_size,
                         p_ts,
                         p_err);
    if (*p_err == OS_ERR_NONE) {
#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
#if (OS_CFG_TS_EN == DEF_ENABLED)
        if (p_ts != DEF_NULL) {
            OSTCBCurPtr->MsgQPendTime = OS_TS_GET() - *p_ts;
            if (OSTCBCurPtr->MsgQPendTimeMax < OSTCBCurPtr->MsgQPendTime) {
                OSTCBCurPtr->MsgQPendTimeMax = OSTCBCurPtr->MsgQPendTime;
            }
        }
#endif
#endif
        CPU_CRITICAL_EXIT();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_MSG_Q_PEND(p_msg_q);                      /* Record the event.                                    */
#endif
        return (p_void);                                        /* Yes, Return oldest message received                  */
    }

    if ((opt & OS_OPT_PEND_NON_BLOCKING) != 0u) {               /* Caller wants to block if not available?              */
       *p_err = OS_ERR_PEND_WOULD_BLOCK;                        /* No                                                   */
        CPU_CRITICAL_EXIT();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_MSG_Q_PEND_FAILED(p_msg_q);               /* Record the event.                                    */
#endif
        return (DEF_NULL);
    } else {                                                    /* Yes                                                  */
        if (OSSchedLockNestingCtr > 0u) {                       /* Can't block when the scheduler is locked             */
            CPU_CRITICAL_EXIT();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
            TRACE_OS_TASK_MSG_Q_PEND_FAILED(p_msg_q);           /* Record the event.                                    */
#endif
           *p_err = OS_ERR_SCHED_LOCKED;
            return (DEF_NULL);
        }
    }
                                                                /* Lock the scheduler/re-enable interrupts              */
    OS_CRITICAL_ENTER_CPU_EXIT();
    OS_Pend(DEF_NULL,                                           /* Block task pending on Message                        */
            DEF_NULL,
            OS_TASK_PEND_ON_TASK_Q,
            timeout);
    OS_CRITICAL_EXIT_NO_SCHED();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_MSG_Q_PEND_BLOCK(p_msg_q);                    /* Record the event.                                    */
#endif
    OSSched();                                                  /* Find the next highest priority task ready to run     */

    CPU_CRITICAL_ENTER();
    switch (OSTCBCurPtr->PendStatus) {
        case OS_STATUS_PEND_OK:                                 /* Extract message from TCB (Put there by Post)         */
             p_void      = OSTCBCurPtr->MsgPtr;
            *p_msg_size  = OSTCBCurPtr->MsgSize;
#if (OS_CFG_TS_EN == DEF_ENABLED)
             if (p_ts != DEF_NULL) {
                *p_ts = OSTCBCurPtr->TS;
#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
                OSTCBCurPtr->MsgQPendTime = OS_TS_GET() - OSTCBCurPtr->TS;
                if (OSTCBCurPtr->MsgQPendTimeMax < OSTCBCurPtr->MsgQPendTime) {
                    OSTCBCurPtr->MsgQPendTimeMax = OSTCBCurPtr->MsgQPendTime;
                }
#endif
             }
#endif
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_MSG_Q_PEND(p_msg_q);                 /* Record the event.                                    */
#endif
            *p_err = OS_ERR_NONE;
             break;

        case OS_STATUS_PEND_ABORT:                              /* Indicate that we aborted                             */
             p_void     = DEF_NULL;
            *p_msg_size = 0u;
             if (p_ts != DEF_NULL) {
                *p_ts = 0u;
             }
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_MSG_Q_PEND_FAILED(p_msg_q);          /* Record the event.                                    */
#endif
            *p_err      =  OS_ERR_PEND_ABORT;
             break;

        case OS_STATUS_PEND_TIMEOUT:                            /* Indicate that we didn't get event within TO          */
        default:
             p_void     = DEF_NULL;
            *p_msg_size = 0u;
#if (OS_CFG_TS_EN == DEF_ENABLED)
             if (p_ts  != DEF_NULL) {
                *p_ts = OSTCBCurPtr->TS;
             }
#endif
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_MSG_Q_PEND_FAILED(p_msg_q);          /* Record the event.                                    */
#endif
            *p_err      =  OS_ERR_TIMEOUT;
             break;
    }
    CPU_CRITICAL_EXIT();
    return (p_void);                                            /* Return received message                              */
}
#endif


/*
************************************************************************************************************************
*                                              ABORT WAITING FOR A MESSAGE
*
* Description: This function aborts & readies the task specified.  This function should be used to fault-abort the wait
*              for a message, rather than to normally post the message to the task via OSTaskQPost().
*
* Arguments  : p_tcb     is a pointer to the task to pend abort
*
*              opt       provides options for this function:
*
*                            OS_OPT_POST_NONE         No option specified
*                            OS_OPT_POST_NO_SCHED     Indicates that the scheduler will not be called.
*
*              p_err     is a pointer to a variable that will contain an error code returned by this function.
*
*                            OS_ERR_NONE               If the task was readied and informed of the aborted wait
*                            OS_ERR_OPT_INVALID        If you specified an invalid option
*                            OS_ERR_OS_NOT_RUNNING     If uC/OS-III is not running yet
*                            OS_ERR_PEND_ABORT_ISR     If you called this function from an ISR
*                            OS_ERR_PEND_ABORT_NONE    If task was not pending on a message and thus there is nothing to
*                                                      abort
*                            OS_ERR_PEND_ABORT_SELF    If you passed a NULL pointer for 'p_tcb'
*
* Returns    : == DEF_FALSE   if task was not waiting for a message, or upon error.
*              == DEF_TRUE    if task was waiting for a message and was readied and informed.
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED) && (OS_CFG_TASK_Q_PEND_ABORT_EN == DEF_ENABLED)
CPU_BOOLEAN  OSTaskQPendAbort (OS_TCB  *p_tcb,
                               OS_OPT   opt,
                               OS_ERR  *p_err)
{
    CPU_TS         ts;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (DEF_FALSE);
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* See if called from ISR ...                           */
       *p_err = OS_ERR_PEND_ABORT_ISR;                          /* ... can't Pend Abort from an ISR                     */
        return (DEF_FALSE);
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return (DEF_FALSE);
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)                          /* ---------------- VALIDATE ARGUMENTS ---------------- */
    switch (opt) {                                              /* User must supply a valid option                      */
        case OS_OPT_POST_NONE:
        case OS_OPT_POST_NO_SCHED:
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;
             return (DEF_FALSE);
    }
#endif

    CPU_CRITICAL_ENTER();
#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((p_tcb == DEF_NULL) ||                                  /* Pend abort self?                                     */
        (p_tcb == OSTCBCurPtr)) {
        CPU_CRITICAL_EXIT();
       *p_err = OS_ERR_PEND_ABORT_SELF;                         /* ... doesn't make sense                               */
        return (DEF_FALSE);
    }
#endif

    if (p_tcb->PendOn != OS_TASK_PEND_ON_TASK_Q) {              /* Is task waiting for a message?                       */
        CPU_CRITICAL_EXIT();                                    /* No                                                   */
       *p_err = OS_ERR_PEND_ABORT_NONE;
        return (DEF_FALSE);
    }

    OS_CRITICAL_ENTER_CPU_EXIT();
#if (OS_CFG_TS_EN == DEF_ENABLED)
    ts = OS_TS_GET();                                           /* Get timestamp of when the abort occurred             */
#else
    ts = 0u;
#endif
    OS_PendAbort(DEF_NULL,                                      /* Abort the pend                                       */
                 p_tcb,
                 ts);
    OS_CRITICAL_EXIT_NO_SCHED();
    if ((opt & OS_OPT_POST_NO_SCHED) == 0u) {
        OSSched();                                              /* Run the scheduler                                    */
    }
   *p_err = OS_ERR_NONE;
    return (DEF_TRUE);
}
#endif


/*
************************************************************************************************************************
*                                               POST MESSAGE TO A TASK
*
* Description: This function sends a message to a task.
*
* Arguments  : p_tcb      is a pointer to the TCB of the task receiving a message.  If you specify a NULL pointer then
*                         the message will be posted to the task's queue of the calling task.  In other words, you'd be
*                         posting a message to yourself.
*
*              p_void     is a pointer to the message to send.
*
*              msg_size   is the size of the message sent (in bytes)
*
*              opt        specifies whether the post will be FIFO or LIFO:
*
*                             OS_OPT_POST_FIFO       Post at the end   of the queue
*                             OS_OPT_POST_LIFO       Post at the front of the queue
*
*                             OS_OPT_POST_NO_SCHED   Do not run the scheduler after the post
*
*                          Note(s): 1) OS_OPT_POST_NO_SCHED can be added with one of the other options.
*
*
*              p_err      is a pointer to a variable that will hold the error code associated
*                         with the outcome of this call.  Errors can be:
*
*                             OS_ERR_NONE              The call was successful and the message was sent
*                             OS_ERR_INT_Q_FULL        If the deferred interrupt post queue is full
*                             OS_ERR_MSG_POOL_EMPTY    If there are no more OS_MSGs available from the pool
*                             OS_ERR_OPT_INVALID       If you specified an invalid option
*                             OS_ERR_OS_NOT_RUNNING    If uC/OS-III is not running yet
*                             OS_ERR_Q_MAX             If the queue is full
*                             OS_ERR_STATE_INVALID     If the task is in an invalid state.  This should never happen
*                                                      and if it does, would be considered a system failure
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
void  OSTaskQPost (OS_TCB       *p_tcb,
                   void         *p_void,
                   OS_MSG_SIZE   msg_size,
                   OS_OPT        opt,
                   OS_ERR       *p_err)
{
    CPU_TS   ts;



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return;
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)                          /* ---------------- VALIDATE ARGUMENTS ---------------- */
    switch (opt) {                                              /* User must supply a valid option                      */
        case OS_OPT_POST_FIFO:
        case OS_OPT_POST_LIFO:
        case OS_OPT_POST_FIFO | OS_OPT_POST_NO_SCHED:
        case OS_OPT_POST_LIFO | OS_OPT_POST_NO_SCHED:
             break;

        default:
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_MSG_Q_POST_FAILED(&p_tcb->MsgQ);     /* Record the event.                                    */
#endif
            *p_err = OS_ERR_OPT_INVALID;
             return;
    }
#endif

#if (OS_CFG_TS_EN == DEF_ENABLED)
    ts = OS_TS_GET();                                           /* Get timestamp                                        */
#else
    ts = 0u;
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {
        OS_IntQPost(OS_OBJ_TYPE_TASK_MSG,                       /* Post to ISR queue                                    */
                    p_tcb,
                    p_void,
                    msg_size,
                    0u,
                    opt,
                    ts,
                    p_err);
        return;
    }
#endif

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_MSG_Q_POST(&p_tcb->MsgQ);                     /* Record the event.                                    */
#endif

    OS_TaskQPost(p_tcb,
                 p_void,
                 msg_size,
                 opt,
                 ts,
                 p_err);
}
#endif


/*
************************************************************************************************************************
*                                       GET THE CURRENT VALUE OF A TASK REGISTER
*
* Description: This function is called to obtain the current value of a task register.  Task registers are application
*              specific and can be used to store task specific values such as 'error numbers' (i.e. errno), statistics,
*              etc.
*
* Arguments  : p_tcb     is a pointer to the OS_TCB of the task you want to read the register from.  If 'p_tcb' is a
*                        NULL pointer then you will get the register of the current task.
*
*              id        is the 'id' of the desired task variable.  Note that the 'id' must be less than
*                        OS_CFG_TASK_REG_TBL_SIZE
*
*              p_err     is a pointer to a variable that will hold an error code related to this call.
*
*                            OS_ERR_NONE               If the call was successful
*                            OS_ERR_REG_ID_INVALID     If the 'id' is not between 0 and OS_CFG_TASK_REG_TBL_SIZE-1
*
* Returns    : The current value of the task's register or 0 if an error is detected.
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
OS_REG  OSTaskRegGet (OS_TCB     *p_tcb,
                      OS_REG_ID   id,
                      OS_ERR     *p_err)
{
    OS_REG     value;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (0u);
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (id >= OS_CFG_TASK_REG_TBL_SIZE) {
       *p_err = OS_ERR_REG_ID_INVALID;
        return (0u);
    }
#endif

    CPU_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {
        p_tcb = OSTCBCurPtr;
    }
    value = p_tcb->RegTbl[id];
    CPU_CRITICAL_EXIT();
   *p_err = OS_ERR_NONE;
    return (value);
}
#endif


/*
************************************************************************************************************************
*                                    ALLOCATE THE NEXT AVAILABLE TASK REGISTER ID
*
* Description: This function is called to obtain a task register ID.  This function thus allows task registers IDs to be
*              allocated dynamically instead of statically.
*
* Arguments  : p_err       is a pointer to a variable that will hold an error code related to this call.
*
*                            OS_ERR_NONE               If the call was successful
*                            OS_ERR_NO_MORE_ID_AVAIL   If you are attempting to assign more task register IDs than you
*                                                          have available through OS_CFG_TASK_REG_TBL_SIZE
*
* Returns    : The next available task register 'id' or OS_CFG_TASK_REG_TBL_SIZE if an error is detected.
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
OS_REG_ID  OSTaskRegGetID (OS_ERR  *p_err)
{
    OS_REG_ID  id;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return ((OS_REG_ID)OS_CFG_TASK_REG_TBL_SIZE);
    }
#endif

    CPU_CRITICAL_ENTER();
    if (OSTaskRegNextAvailID >= OS_CFG_TASK_REG_TBL_SIZE) {     /* See if we exceeded the number of IDs available       */
       *p_err = OS_ERR_NO_MORE_ID_AVAIL;                        /* Yes, cannot allocate more task register IDs          */
        CPU_CRITICAL_EXIT();
        return (OS_CFG_TASK_REG_TBL_SIZE);
    }

    id    = OSTaskRegNextAvailID;                               /* Assign the next available ID                         */
    OSTaskRegNextAvailID++;                                     /* Increment available ID for next request              */
    CPU_CRITICAL_EXIT();
   *p_err = OS_ERR_NONE;
    return (id);
}
#endif


/*
************************************************************************************************************************
*                                       SET THE CURRENT VALUE OF A TASK REGISTER
*
* Description: This function is called to change the current value of a task register.  Task registers are application
*              specific and can be used to store task specific values such as 'error numbers' (i.e. errno), statistics,
*              etc.
*
* Arguments  : p_tcb     is a pointer to the OS_TCB of the task you want to set the register for.  If 'p_tcb' is a NULL
*                        pointer then you will change the register of the current task.
*
*              id        is the 'id' of the desired task register.  Note that the 'id' must be less than
*                        OS_CFG_TASK_REG_TBL_SIZE
*
*              value     is the desired value for the task register.
*
*              p_err     is a pointer to a variable that will hold an error code related to this call.
*
*                            OS_ERR_NONE               If the call was successful
*                            OS_ERR_REG_ID_INVALID     If the 'id' is not between 0 and OS_CFG_TASK_REG_TBL_SIZE-1
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
void  OSTaskRegSet (OS_TCB     *p_tcb,
                    OS_REG_ID   id,
                    OS_REG      value,
                    OS_ERR     *p_err)
{
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (id >= OS_CFG_TASK_REG_TBL_SIZE) {
       *p_err = OS_ERR_REG_ID_INVALID;
        return;
    }
#endif

    CPU_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {
        p_tcb = OSTCBCurPtr;
    }
    p_tcb->RegTbl[id] = value;
    CPU_CRITICAL_EXIT();
   *p_err             = OS_ERR_NONE;
}
#endif


/*
************************************************************************************************************************
*                                               RESUME A SUSPENDED TASK
*
* Description: This function is called to resume a previously suspended task.  This is the only call that will remove an
*              explicit task suspension.
*
* Arguments  : p_tcb      Is a pointer to the task's OS_TCB to resume
*
*              p_err      Is a pointer to a variable that will contain an error code returned by this function
*
*                             OS_ERR_NONE                  If the requested task is resumed
*                             OS_ERR_INT_Q_FULL            If the deferred interrupt post queue is full
*                             OS_ERR_OS_NOT_RUNNING        If uC/OS-III is not running yet
*                             OS_ERR_STATE_INVALID         If the task is in an invalid state
*                             OS_ERR_TASK_NOT_SUSPENDED    If the task to resume has not been suspended
*                             OS_ERR_TASK_RESUME_ISR       If you called this function from an ISR
*                             OS_ERR_TASK_RESUME_SELF      You cannot resume 'self'
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_SUSPEND_EN == DEF_ENABLED)
void  OSTaskResume (OS_TCB  *p_tcb,
                    OS_ERR  *p_err)
{
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN   == DEF_DISABLED) && \
    (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Not allowed to call from an ISR                      */
       *p_err = OS_ERR_TASK_RESUME_ISR;
        return;
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return;
    }
#endif


    CPU_CRITICAL_ENTER();
#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    if ((p_tcb == DEF_NULL) ||                                  /* We cannot resume 'self'                              */
        (p_tcb == OSTCBCurPtr)) {
        CPU_CRITICAL_EXIT();
       *p_err  = OS_ERR_TASK_RESUME_SELF;
        return;
    }
#endif
    CPU_CRITICAL_EXIT();

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* See if called from an ISR                            */
        OS_IntQPost(OS_OBJ_TYPE_TASK_RESUME,                    /* Post to ISR queue                                    */
                    (void *)p_tcb,
                    DEF_NULL,
                    0u,
                    0u,
                    0u,
                    0u,
                    p_err);
        return;
    }
#endif

    OS_TaskResume(p_tcb, p_err);

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_RESUME(p_tcb);                                /* Record the event.                                    */
#endif
}
#endif


/*
************************************************************************************************************************
*                                              WAIT FOR A TASK SEMAPHORE
*
* Description: This function is called to block the current task until a signal is sent by another task or ISR.
*
* Arguments  : timeout       is the amount of time you are will to wait for the signal
*
*              opt           determines whether the user wants to block if a semaphore post was not received:
*
*                                OS_OPT_PEND_BLOCKING
*                                OS_OPT_PEND_NON_BLOCKING
*
*              p_ts          is a pointer to a variable that will receive the timestamp of when the semaphore was posted
*                            or pend aborted.  If you pass a NULL pointer (i.e. (CPU_TS *)0) then you will not get the
*                            timestamp.  In other words, passing a NULL pointer is valid and indicates that you don't
*                            need the timestamp.
*
*              p_err         is a pointer to an error code that will be set by this function
*
*                                OS_ERR_NONE                The call was successful and your task received a message
*                                OS_ERR_OPT_INVALID         You specified an invalid option
*                                OS_ERR_OS_NOT_RUNNING      If uC/OS-III is not running yet
*                                OS_ERR_PEND_ABORT          If the pend was aborted
*                                OS_ERR_PEND_ISR            If you called this function from an ISR
*                                OS_ERR_PEND_WOULD_BLOCK    If you specified non-blocking but no signal was received
*                                OS_ERR_SCHED_LOCKED        If the scheduler is locked
*                                OS_ERR_STATUS_INVALID      If the pend status is invalid
*                                OS_ERR_TIMEOUT             A message was not received within the specified timeout
*
* Returns    : The current count of signals the task received, 0 if none.
*
* Note(s)    : none
************************************************************************************************************************
*/

OS_SEM_CTR  OSTaskSemPend (OS_TICK   timeout,
                           OS_OPT    opt,
                           CPU_TS   *p_ts,
                           OS_ERR   *p_err)
{
    OS_SEM_CTR    ctr;
    CPU_SR_ALLOC();


#if (OS_CFG_TS_EN == DEF_DISABLED)
    (void)&p_ts;                                                /* Prevent compiler warning for not using 'ts'          */
#endif

#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (0u);
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Not allowed to call from an ISR                      */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);             /* Record the event.                                    */
#endif
       *p_err = OS_ERR_PEND_ISR;
        return (0u);
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return (0u);
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    switch (opt) {                                              /* Validate 'opt'                                       */
        case OS_OPT_PEND_BLOCKING:
        case OS_OPT_PEND_NON_BLOCKING:
             break;

        default:
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);        /* Record the event.                                    */
#endif
            *p_err = OS_ERR_OPT_INVALID;
             return (0u);
    }
#endif

    CPU_CRITICAL_ENTER();
    if (OSTCBCurPtr->SemCtr > 0u) {                             /* See if task already been signaled                    */
        OSTCBCurPtr->SemCtr--;
        ctr = OSTCBCurPtr->SemCtr;
#if (OS_CFG_TS_EN == DEF_ENABLED)
        if (p_ts != DEF_NULL) {
           *p_ts  = OSTCBCurPtr->TS;
        }
#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
#if (OS_CFG_TS_EN == DEF_ENABLED)
        OSTCBCurPtr->SemPendTime = OS_TS_GET() - OSTCBCurPtr->TS;
        if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) {
            OSTCBCurPtr->SemPendTimeMax = OSTCBCurPtr->SemPendTime;
        }
#endif
#endif
#endif
        CPU_CRITICAL_EXIT();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_SEM_PEND(OSTCBCurPtr);                    /* Record the event.                                    */
#endif
       *p_err = OS_ERR_NONE;
        return (ctr);
    }

    if ((opt & OS_OPT_PEND_NON_BLOCKING) != 0u) {               /* Caller wants to block if not available?              */
        CPU_CRITICAL_EXIT();
#if (OS_CFG_TS_EN == DEF_ENABLED)
        if (p_ts != DEF_NULL) {
            *p_ts  = 0u;
        }
#endif
       *p_err = OS_ERR_PEND_WOULD_BLOCK;                        /* No                                                   */
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
        TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);             /* Record the event.                                    */
#endif
        return (0u);
    } else {                                                    /* Yes                                                  */
        if (OSSchedLockNestingCtr > 0u) {                       /* Can't pend when the scheduler is locked              */
#if (OS_CFG_TS_EN == DEF_ENABLED)
            if (p_ts != DEF_NULL) {
               *p_ts  = 0u;
            }
#endif
            CPU_CRITICAL_EXIT();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
            TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);         /* Record the event.                                    */
#endif
           *p_err = OS_ERR_SCHED_LOCKED;
            return (0u);
        }
    }
                                                                /* Lock the scheduler/re-enable interrupts              */
    OS_CRITICAL_ENTER_CPU_EXIT();
    OS_Pend(DEF_NULL,                                           /* Block task pending on Signal                         */
            DEF_NULL,
            OS_TASK_PEND_ON_TASK_SEM,
            timeout);
    OS_CRITICAL_EXIT_NO_SCHED();
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_SEM_PEND_BLOCK(OSTCBCurPtr);                  /* Record the event.                                    */
#endif
    OSSched();                                                  /* Find next highest priority task ready to run         */

    CPU_CRITICAL_ENTER();
    switch (OSTCBCurPtr->PendStatus) {                          /* See if we timed-out or aborted                       */
        case OS_STATUS_PEND_OK:
#if (OS_CFG_TS_EN == DEF_ENABLED)
             if (p_ts != DEF_NULL) {
                *p_ts                    =  OSTCBCurPtr->TS;
#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
#if (OS_CFG_TS_EN == DEF_ENABLED)
                OSTCBCurPtr->SemPendTime = OS_TS_GET() - OSTCBCurPtr->TS;
                if (OSTCBCurPtr->SemPendTimeMax < OSTCBCurPtr->SemPendTime) {
                    OSTCBCurPtr->SemPendTimeMax = OSTCBCurPtr->SemPendTime;
                }
#endif
#endif
             }
#endif
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_SEM_PEND(OSTCBCurPtr);               /* Record the event.                                    */
#endif
            *p_err = OS_ERR_NONE;
             break;

        case OS_STATUS_PEND_ABORT:
#if (OS_CFG_TS_EN == DEF_ENABLED)
             if (p_ts != DEF_NULL) {
                *p_ts = OSTCBCurPtr->TS;
             }
#endif
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);        /* Record the event.                                    */
#endif
            *p_err = OS_ERR_PEND_ABORT;                         /* Indicate that we aborted                             */
             break;

        case OS_STATUS_PEND_TIMEOUT:
#if (OS_CFG_TS_EN == DEF_ENABLED)
             if (p_ts != DEF_NULL) {
                *p_ts = 0u;
             }
#endif
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);        /* Record the event.                                    */
#endif
            *p_err = OS_ERR_TIMEOUT;                            /* Indicate that we didn't get event within TO          */
             break;

        default:
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_SEM_PEND_FAILED(OSTCBCurPtr);        /* Record the event.                                    */
#endif
            *p_err = OS_ERR_STATUS_INVALID;
             break;
    }
    ctr = OSTCBCurPtr->SemCtr;
    CPU_CRITICAL_EXIT();
    return (ctr);
}


/*
************************************************************************************************************************
*                                               ABORT WAITING FOR A SIGNAL
*
* Description: This function aborts & readies the task specified.  This function should be used to fault-abort the wait
*              for a signal, rather than to normally post the signal to the task via OSTaskSemPost().
*
* Arguments  : p_tcb     is a pointer to the task to pend abort
*
*              opt       provides options for this function:
*
*                            OS_OPT_POST_NONE         No option selected
*                            OS_OPT_POST_NO_SCHED     Indicates that the scheduler will not be called.
*
*              p_err     is a pointer to a variable that will contain an error code returned by this function.
*
*                            OS_ERR_NONE               If the task was readied and informed of the aborted wait
*                            OS_ERR_OPT_INVALID        You specified an invalid option
*                            OS_ERR_OS_NOT_RUNNING     If uC/OS-III is not running yet
*                            OS_ERR_PEND_ABORT_ISR     If you tried calling this function from an ISR
*                            OS_ERR_PEND_ABORT_NONE    If the task was not waiting for a signal
*                            OS_ERR_PEND_ABORT_SELF    If you attempted to pend abort the calling task.  This is not
*                                                      possible since the calling task cannot be pending because it's
*                                                      running
*
* Returns    : == DEF_FALSE   if task was not waiting for a message, or upon error.
*              == DEF_TRUE    if task was waiting for a message and was readied and informed.
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_TASK_SEM_PEND_ABORT_EN == DEF_ENABLED)
CPU_BOOLEAN  OSTaskSemPendAbort (OS_TCB  *p_tcb,
                                 OS_OPT   opt,
                                 OS_ERR  *p_err)
{
    CPU_TS  ts;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (DEF_FALSE);
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* See if called from ISR ...                           */
       *p_err = OS_ERR_PEND_ABORT_ISR;                          /* ... can't Pend Abort from an ISR                     */
        return (DEF_FALSE);
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return (DEF_FALSE);
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    switch (opt) {                                              /* Validate 'opt'                                       */
        case OS_OPT_POST_NONE:
        case OS_OPT_POST_NO_SCHED:
             break;

        default:
            *p_err = OS_ERR_OPT_INVALID;
             return (DEF_FALSE);
    }
#endif

    CPU_CRITICAL_ENTER();
    if ((p_tcb == DEF_NULL) ||                                  /* Pend abort self?                                     */
        (p_tcb == OSTCBCurPtr)) {
        CPU_CRITICAL_EXIT();                                    /* ... doesn't make sense!                              */
       *p_err = OS_ERR_PEND_ABORT_SELF;
        return (DEF_FALSE);
    }

    if (p_tcb->PendOn != OS_TASK_PEND_ON_TASK_SEM) {            /* Is task waiting for a signal?                        */
        CPU_CRITICAL_EXIT();
       *p_err = OS_ERR_PEND_ABORT_NONE;
        return (DEF_FALSE);
    }
    CPU_CRITICAL_EXIT();

    OS_CRITICAL_ENTER();
#if (OS_CFG_TS_EN == DEF_ENABLED)
    ts = OS_TS_GET();
#else
    ts = 0u;
#endif
    OS_PendAbort(DEF_NULL,
                 p_tcb,
                 ts);
    OS_CRITICAL_EXIT_NO_SCHED();
    if ((opt & OS_OPT_POST_NO_SCHED) == 0u) {
        OSSched();                                              /* Run the scheduler                                    */
    }
   *p_err = OS_ERR_NONE;
    return (DEF_TRUE);
}
#endif


/*
************************************************************************************************************************
*                                                    SIGNAL A TASK
*
* Description: This function is called to signal a task waiting for a signal.
*
* Arguments  : p_tcb     is the pointer to the TCB of the task to signal.  A NULL pointer indicates that you are sending
*                        a signal to yourself.
*
*              opt       determines the type of POST performed:
*
*                             OS_OPT_POST_NONE         No option
*                             OS_OPT_POST_NO_SCHED     Do not call the scheduler
*
*              p_err     is a pointer to an error code returned by this function:
*
*                            OS_ERR_NONE              If the requested task is signaled
*                            OS_ERR_INT_Q_FULL        If the deferred interrupt post queue is full
*                            OS_ERR_OPT_INVALID       If you specified an invalid option
*                            OS_ERR_OS_NOT_RUNNING    If uC/OS-III is not running yet
*                            OS_ERR_SEM_OVF           If the post would cause the semaphore count to overflow
*                            OS_ERR_STATE_INVALID     If the task is in an invalid state.  This should never happen
*                                                     and if it does, would be considered a system failure
*
* Returns    : The current value of the task's signal counter or 0 if called from an ISR
*
* Note(s)    : none
************************************************************************************************************************
*/

OS_SEM_CTR  OSTaskSemPost (OS_TCB  *p_tcb,
                           OS_OPT   opt,
                           OS_ERR  *p_err)
{
    OS_SEM_CTR  ctr;
    CPU_TS      ts;



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (0u);
    }
#endif

#if (OS_CFG_INVALID_OS_CALLS_CHK_EN == DEF_ENABLED)             /* Is the kernel running?                               */
    if (OSRunning != OS_STATE_OS_RUNNING) {
       *p_err = OS_ERR_OS_NOT_RUNNING;
        return (0u);
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    switch (opt) {                                              /* Validate 'opt'                                       */
        case OS_OPT_POST_NONE:
        case OS_OPT_POST_NO_SCHED:
             break;

        default:
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
             TRACE_OS_TASK_SEM_POST_FAILED(p_tcb);              /* Record the event.                                    */
#endif
            *p_err =  OS_ERR_OPT_INVALID;
             return (0u);
    }
#endif

#if (OS_CFG_TS_EN == DEF_ENABLED)
    ts = OS_TS_GET();                                           /* Get timestamp                                        */
#else
    ts = 0u;
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* See if called from an ISR                            */
        OS_IntQPost(OS_OBJ_TYPE_TASK_SIGNAL,                    /* Post to ISR queue                                    */
                    (void      *)p_tcb,
                    DEF_NULL,
                    0u,
                    0u,
                    0u,
                    ts,
                    p_err);
        return (0u);
    }
#endif

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
    TRACE_OS_TASK_SEM_POST(p_tcb);                              /* Record the event.                                    */
#endif

    ctr = OS_TaskSemPost(p_tcb,
                         opt,
                         ts,
                         p_err);

    return (ctr);
}


/*
************************************************************************************************************************
*                                            SET THE SIGNAL COUNTER OF A TASK
*
* Description: This function is called to clear the signal counter
*
* Arguments  : p_tcb      is the pointer to the TCB of the task to clear the counter.  If you specify a NULL pointer
*                         then the signal counter of the current task will be cleared.
*
*              cnt        is the desired value of the semaphore counter
*
*              p_err      is a pointer to an error code returned by this function
*
*                             OS_ERR_NONE             If the signal counter of the requested task is set
*                             OS_ERR_SET_ISR          If the function was called from an ISR
*                             OS_ERR_TASK_WAITING     One or more tasks were waiting on the semaphore
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

OS_SEM_CTR  OSTaskSemSet (OS_TCB      *p_tcb,
                          OS_SEM_CTR   cnt,
                          OS_ERR      *p_err)
{
    OS_SEM_CTR  ctr;
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return (0u);
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Not allowed to call from an ISR                      */
       *p_err = OS_ERR_SET_ISR;
        return (0u);
    }
#endif

    CPU_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {
        p_tcb = OSTCBCurPtr;
    }

    if (((p_tcb->TaskState   & OS_TASK_STATE_PEND) != 0u) &&    /* Not allowed when a task is waiting.                  */
         (p_tcb->PendOn == OS_TASK_PEND_ON_TASK_SEM)) {
        CPU_CRITICAL_EXIT();
       *p_err = OS_ERR_TASK_WAITING;
        return (0u);
    }

    ctr           = p_tcb->SemCtr;
    p_tcb->SemCtr = (OS_SEM_CTR)cnt;
    CPU_CRITICAL_EXIT();
   *p_err         =  OS_ERR_NONE;
    return (ctr);
}


/*
************************************************************************************************************************
*                                                    STACK CHECKING
*
* Description: This function is called to calculate the amount of free memory left on the specified task's stack.
*
* Arguments  : p_tcb       is a pointer to the TCB of the task to check.  If you specify a NULL pointer then
*                          you are specifying that you want to check the stack of the current task.
*
*              p_free      is a pointer to a variable that will receive the number of free 'entries' on the task's stack.
*
*              p_used      is a pointer to a variable that will receive the number of used 'entries' on the task's stack.
*
*              p_err       is a pointer to a variable that will contain an error code.
*
*                              OS_ERR_NONE               Upon success
*                              OS_ERR_PTR_INVALID        If either 'p_free' or 'p_used' are NULL pointers
*                              OS_ERR_TASK_NOT_EXIST     If the stack pointer of the task is a NULL pointer
*                              OS_ERR_TASK_OPT           If you did NOT specified OS_OPT_TASK_STK_CHK when the task
*                                                        was created
*                              OS_ERR_TASK_STK_CHK_ISR   You called this function from an ISR
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED)
void  OSTaskStkChk (OS_TCB        *p_tcb,
                    CPU_STK_SIZE  *p_free,
                    CPU_STK_SIZE  *p_used,
                    OS_ERR        *p_err)
{
    CPU_STK_SIZE  free_stk;
    CPU_STK      *p_stk;
    CPU_SR_ALLOC();


#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* See if trying to check stack from ISR                */
       *p_err = OS_ERR_TASK_STK_CHK_ISR;
        return;
    }
#endif

#if (OS_CFG_ARG_CHK_EN == DEF_ENABLED)
    if (p_free == DEF_NULL) {                                   /* User must specify valid destinations for the sizes   */
       *p_err = OS_ERR_PTR_INVALID;
        return;
    }

    if (p_used == DEF_NULL) {
       *p_err = OS_ERR_PTR_INVALID;
        return;
    }
#endif

    CPU_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {                                    /* Check the stack of the current task?                 */
        p_tcb = OSTCBCurPtr;                                    /* Yes                                                  */
    }

    if (p_tcb->StkPtr == DEF_NULL) {                            /* Make sure task exist                                 */
        CPU_CRITICAL_EXIT();
       *p_free = 0u;
       *p_used = 0u;
       *p_err  = OS_ERR_TASK_NOT_EXIST;
        return;
    }

    if ((p_tcb->Opt & OS_OPT_TASK_STK_CHK) == 0u) {             /* Make sure stack checking option is set               */
        CPU_CRITICAL_EXIT();
       *p_free = 0u;
       *p_used = 0u;
       *p_err  = OS_ERR_TASK_OPT;
        return;
    }
    CPU_CRITICAL_EXIT();

    free_stk = 0u;
#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)
    p_stk = p_tcb->StkBasePtr;                                  /* Start at the lowest memory and go up                 */
#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)
    p_stk += OS_CFG_TASK_STK_REDZONE_DEPTH;
#endif
    while (*p_stk == 0u) {                                      /* Compute the number of zero entries on the stk        */
        p_stk++;
        free_stk++;
    }
#else
    p_stk = p_tcb->StkBasePtr + p_tcb->StkSize - 1u;            /* Start at the highest memory and go down              */
#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)
    p_stk -= OS_CFG_TASK_STK_REDZONE_DEPTH;
#endif
    while (*p_stk == 0u) {
        free_stk++;
        p_stk--;
    }
#endif
#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)
    free_stk -= OS_CFG_TASK_STK_REDZONE_DEPTH;                  /* Compensate for stack elements used by Redzone.       */
#endif
   *p_free = free_stk;
   *p_used = (p_tcb->StkSize - free_stk);                       /* Compute number of entries used on the stack          */
   *p_err  = OS_ERR_NONE;
}
#endif


/*
************************************************************************************************************************
*                                            CHECK THE STACK REDZONE OF A TASK
*
* Description: Verify a task's stack redzone.
*
* Arguments  : p_tcb     is a pointer to the TCB of the task to check or null for the current task.
*
* Returns    : If the stack is corrupted (DEF_FAIL) or not (DEF_OK).
*
* Note(s)    : These functions are INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)
CPU_BOOLEAN  OSTaskStkRedzoneChk (OS_TCB  *p_tcb)
{
    CPU_BOOLEAN  stk_status;


    if (p_tcb == DEF_NULL) {
        p_tcb = OSTCBCurPtr;
    }
                                                                /* Check if SP is valid:                                */
                                                                /*   StkBase <= SP < (StkBase + StkSize)                */
    if ((p_tcb->StkPtr <   p_tcb->StkBasePtr) ||
        (p_tcb->StkPtr >= (p_tcb->StkBasePtr + p_tcb->StkSize))) {
        return (DEF_FAIL);
    }

    stk_status = OS_TaskStkRedzoneChk(p_tcb->StkBasePtr, p_tcb->StkSize);

    return (stk_status);
}
#endif


/*
************************************************************************************************************************
*                                                   SUSPEND A TASK
*
* Description: This function is called to suspend a task.  The task can be the calling task if 'p_tcb' is a NULL pointer
*              or the pointer to the TCB of the calling task.
*
* Arguments  : p_tcb    is a pointer to the TCB to suspend.
*                       If p_tcb is a NULL pointer then, suspend the current task.
*
*              p_err    is a pointer to a variable that will receive an error code from this function.
*
*                           OS_ERR_NONE                        If the requested task is suspended
*                           OS_ERR_INT_Q_FULL                  If the deferred interrupt post queue is full
*                           OS_ERR_OS_NOT_RUNNING              If uC/OS-III is not running yet
*                           OS_ERR_SCHED_LOCKED                You can't suspend the current task is the scheduler is
*                                                                  locked
*                           OS_ERR_STATE_INVALID               If the task is in an invalid state
*                           OS_ERR_TASK_SUSPEND_CTR_OVF        If the nesting counter overflowed.
*                           OS_ERR_TASK_SUSPEND_ISR            If you called this function from an ISR
*                           OS_ERR_TASK_SUSPEND_IDLE           If you attempted to suspend the idle task which is not
*                                                                  allowed
*                           OS_ERR_TASK_SUSPEND_INT_HANDLER    If you attempted to suspend the idle task which is not
*                                                                  allowed
*
* Returns    : none
*
* Note(s)    : 1) You should use this function with great care.  If you suspend a task that is waiting for an event
*                 (i.e. a message, a semaphore, a queue ...) you will prevent this task from running when the event
*                 arrives.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_SUSPEND_EN == DEF_ENABLED)
void   OSTaskSuspend (OS_TCB  *p_tcb,
                      OS_ERR  *p_err)
{
#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN   == DEF_DISABLED) && \
    (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Not allowed to call from an ISR                      */
       *p_err = OS_ERR_TASK_SUSPEND_ISR;
        return;
    }
#endif

#if (OS_CFG_TASK_IDLE_EN == DEF_ENABLED)
    if (p_tcb == &OSIdleTaskTCB) {                              /* Make sure not suspending the idle task               */
       *p_err = OS_ERR_TASK_SUSPEND_IDLE;
        return;
    }
#endif

#if (OS_CFG_ISR_POST_DEFERRED_EN == DEF_ENABLED)
    if (p_tcb == &OSIntQTaskTCB) {                              /* Not allowed to suspend the ISR handler task          */
       *p_err = OS_ERR_TASK_SUSPEND_INT_HANDLER;
        return;
    }

    if (OSIntNestingCtr > 0u) {                                 /* See if called from an ISR                            */
        OS_IntQPost(OS_OBJ_TYPE_TASK_SUSPEND,                   /* Post to ISR queue                                    */
                    (void *)p_tcb,
                    DEF_NULL,
                    0u,
                    0u,
                    0u,
                    0u,
                    p_err);
        return;
    }
#endif

    OS_TaskSuspend(p_tcb, p_err);
}
#endif


/*
************************************************************************************************************************
*                                                CHANGE A TASK'S TIME SLICE
*
* Description: This function is called to change the value of the task's specific time slice.
*
* Arguments  : p_tcb        is the pointer to the TCB of the task to change. If you specify an NULL pointer, the current
*                           task is assumed.
*
*              time_quanta  is the number of ticks before the CPU is taken away when round-robin scheduling is enabled.
*
*              p_err        is a pointer to an error code returned by this function:
*
*                               OS_ERR_NONE       Upon success
*                               OS_ERR_SET_ISR    If you called this function from an ISR
*
* Returns    : none
*
* Note(s)    : none
************************************************************************************************************************
*/

#if (OS_CFG_SCHED_ROUND_ROBIN_EN == DEF_ENABLED)
void  OSTaskTimeQuantaSet (OS_TCB   *p_tcb,
                           OS_TICK   time_quanta,
                           OS_ERR   *p_err)
{
    CPU_SR_ALLOC();



#ifdef OS_SAFETY_CRITICAL
    if (p_err == DEF_NULL) {
        OS_SAFETY_CRITICAL_EXCEPTION();
        return;
    }
#endif

#if (OS_CFG_CALLED_FROM_ISR_CHK_EN == DEF_ENABLED)
    if (OSIntNestingCtr > 0u) {                                 /* Can't call this function from an ISR                 */
       *p_err = OS_ERR_SET_ISR;
        return;
    }
#endif

    CPU_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {
        p_tcb = OSTCBCurPtr;
    }

    if (time_quanta == 0u) {
        p_tcb->TimeQuanta    = OSSchedRoundRobinDfltTimeQuanta;
    } else {
        p_tcb->TimeQuanta    = time_quanta;
    }
    if (p_tcb->TimeQuanta > p_tcb->TimeQuantaCtr) {
        p_tcb->TimeQuantaCtr = p_tcb->TimeQuanta;
    }
    CPU_CRITICAL_EXIT();
   *p_err = OS_ERR_NONE;
}
#endif


/*
************************************************************************************************************************
*                                            ADD/REMOVE TASK TO/FROM DEBUG LIST
*
* Description: These functions are called by uC/OS-III to add or remove an OS_TCB from the debug list.
*
* Arguments  : p_tcb     is a pointer to the OS_TCB to add/remove
*
* Returns    : none
*
* Note(s)    : These functions are INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

#if (OS_CFG_DBG_EN == DEF_ENABLED)
void  OS_TaskDbgListAdd (OS_TCB  *p_tcb)
{
    p_tcb->DbgPrevPtr                = DEF_NULL;
    if (OSTaskDbgListPtr == DEF_NULL) {
        p_tcb->DbgNextPtr            = DEF_NULL;
    } else {
        p_tcb->DbgNextPtr            =  OSTaskDbgListPtr;
        OSTaskDbgListPtr->DbgPrevPtr =  p_tcb;
    }
    OSTaskDbgListPtr                 =  p_tcb;
}



void  OS_TaskDbgListRemove (OS_TCB  *p_tcb)
{
    OS_TCB  *p_tcb_next;
    OS_TCB  *p_tcb_prev;


    p_tcb_prev = p_tcb->DbgPrevPtr;
    p_tcb_next = p_tcb->DbgNextPtr;

    if (p_tcb_prev == DEF_NULL) {
        OSTaskDbgListPtr = p_tcb_next;
        if (p_tcb_next != DEF_NULL) {
            p_tcb_next->DbgPrevPtr = DEF_NULL;
        }
        p_tcb->DbgNextPtr = DEF_NULL;

    } else if (p_tcb_next == DEF_NULL) {
        p_tcb_prev->DbgNextPtr = DEF_NULL;
        p_tcb->DbgPrevPtr      = DEF_NULL;

    } else {
        p_tcb_prev->DbgNextPtr =  p_tcb_next;
        p_tcb_next->DbgPrevPtr =  p_tcb_prev;
        p_tcb->DbgNextPtr      = DEF_NULL;
        p_tcb->DbgPrevPtr      = DEF_NULL;
    }
}
#endif


/*
************************************************************************************************************************
*                                             TASK MANAGER INITIALIZATION
*
* Description: This function is called by OSInit() to initialize the task management.
*

* Argument(s): p_err        is a pointer to a variable that will contain an error code returned by this function.
*
*                                OS_ERR_NONE     the call was successful
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TaskInit (OS_ERR  *p_err)
{
#if (OS_CFG_DBG_EN == DEF_ENABLED)
    OSTaskDbgListPtr = DEF_NULL;
#endif

    OSTaskQty        = 0u;                                      /* Clear the number of tasks                            */

#if ((OS_CFG_TASK_PROFILE_EN == DEF_ENABLED) || (OS_CFG_DBG_EN == DEF_ENABLED))
    OSTaskCtxSwCtr   = 0u;                                      /* Clear the context switch counter                     */
#endif

   *p_err            = OS_ERR_NONE;
}


/*
************************************************************************************************************************
*                                               INITIALIZE TCB FIELDS
*
* Description: This function is called to initialize a TCB to default values
*
* Arguments  : p_tcb    is a pointer to the TCB to initialize
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TaskInitTCB (OS_TCB  *p_tcb)
{
#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    OS_REG_ID   reg_id;
#endif
#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    OS_TLS_ID   id;
#endif


    p_tcb->StkPtr             = DEF_NULL;
#if ((OS_CFG_DBG_EN == DEF_ENABLED) || (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED))
    p_tcb->StkLimitPtr        = DEF_NULL;
#endif

    p_tcb->ExtPtr             = DEF_NULL;

    p_tcb->NextPtr            = DEF_NULL;
    p_tcb->PrevPtr            = DEF_NULL;

#if (OS_CFG_TASK_TICK_EN == DEF_ENABLED)
    p_tcb->TickNextPtr        = DEF_NULL;
    p_tcb->TickPrevPtr        = DEF_NULL;
    p_tcb->TickListPtr        = DEF_NULL;
#endif

#if (OS_CFG_DBG_EN == DEF_ENABLED)
    p_tcb->NamePtr            = (CPU_CHAR *)((void *)"?Task");
#endif

#if ((OS_CFG_DBG_EN == DEF_ENABLED) || (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED))
    p_tcb->StkBasePtr         = DEF_NULL;
#endif

#if (OS_CFG_DBG_EN == DEF_ENABLED)
    p_tcb->TaskEntryAddr      = 0u;
    p_tcb->TaskEntryArg       = DEF_NULL;
#endif

#if (OS_CFG_PEND_MULTI_EN == DEF_ENABLED)
    p_tcb->PendDataTblPtr     = DEF_NULL;
    p_tcb->PendDataTblEntries = 0u;
#endif

#if (OS_CFG_TS_EN == DEF_ENABLED)
    p_tcb->TS                 = 0u;
#endif

#if (OS_MSG_EN == DEF_ENABLED)
    p_tcb->MsgPtr             = DEF_NULL;
    p_tcb->MsgSize            = 0u;
#endif

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
    OS_MsgQInit(&p_tcb->MsgQ,
                 0u);
#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
    p_tcb->MsgQPendTime       = 0u;
    p_tcb->MsgQPendTimeMax    = 0u;
#endif
#endif

#if (OS_CFG_FLAG_EN == DEF_ENABLED)
    p_tcb->FlagsPend          = 0u;
    p_tcb->FlagsOpt           = 0u;
    p_tcb->FlagsRdy           = 0u;
#endif

#if (OS_CFG_TASK_REG_TBL_SIZE > 0u)
    for (reg_id = 0u; reg_id < OS_CFG_TASK_REG_TBL_SIZE; reg_id++) {
        p_tcb->RegTbl[reg_id] = 0u;
    }
#endif

#if defined(OS_CFG_TLS_TBL_SIZE) && (OS_CFG_TLS_TBL_SIZE > 0u)
    for (id = 0u; id < OS_CFG_TLS_TBL_SIZE; id++) {
        p_tcb->TLS_Tbl[id]    = 0u;
    }
#endif

    p_tcb->SemCtr             = 0u;
#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
    p_tcb->SemPendTime        = 0u;
    p_tcb->SemPendTimeMax     = 0u;
#endif

#if ((OS_CFG_DBG_EN == DEF_ENABLED) || (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED))
    p_tcb->StkSize            = 0u;
#endif


#if (OS_CFG_TASK_SUSPEND_EN == DEF_ENABLED)
    p_tcb->SuspendCtr         = 0u;
#endif

#if (OS_CFG_STAT_TASK_STK_CHK_EN == DEF_ENABLED)
    p_tcb->StkFree            = 0u;
    p_tcb->StkUsed            = 0u;
#endif

    p_tcb->Opt                = 0u;

#if (OS_CFG_TASK_TICK_EN == DEF_ENABLED)
    p_tcb->TickRemain         = 0u;
    p_tcb->TickCtrPrev        = 0u;
#endif

#if (OS_CFG_SCHED_ROUND_ROBIN_EN == DEF_ENABLED)
    p_tcb->TimeQuanta         = 0u;
    p_tcb->TimeQuantaCtr      = 0u;
#endif

#if (OS_CFG_TASK_PROFILE_EN == DEF_ENABLED)
    p_tcb->CPUUsage           = 0u;
    p_tcb->CPUUsageMax        = 0u;
    p_tcb->CtxSwCtr           = 0u;
    p_tcb->CyclesDelta        = 0u;
#if (OS_CFG_TS_EN == DEF_ENABLED)
    p_tcb->CyclesStart        = OS_TS_GET();                    /* Read the current timestamp and save                    */
#else
    p_tcb->CyclesStart        = 0u;
#endif
    p_tcb->CyclesTotal        = 0u;
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    p_tcb->IntDisTimeMax      = 0u;
#endif
#if (OS_CFG_SCHED_LOCK_TIME_MEAS_EN == DEF_ENABLED)
    p_tcb->SchedLockTimeMax   = 0u;
#endif

    p_tcb->PendOn             = OS_TASK_PEND_ON_NOTHING;
    p_tcb->PendStatus         = OS_STATUS_PEND_OK;
    p_tcb->TaskState          = OS_TASK_STATE_RDY;

    p_tcb->Prio               = OS_PRIO_INIT;
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    p_tcb->BasePrio           = OS_PRIO_INIT;
    p_tcb->MutexGrpHeadPtr    = DEF_NULL;
#endif

#if (OS_CFG_DBG_EN == DEF_ENABLED)
    p_tcb->DbgPrevPtr         = DEF_NULL;
    p_tcb->DbgNextPtr         = DEF_NULL;
    p_tcb->DbgNamePtr         = (CPU_CHAR *)((void *)" ");
#endif
}


/*
************************************************************************************************************************
*                                               POST MESSAGE TO A TASK
*
* Description: This function sends a message to a task
*
* Arguments  : p_tcb      is a pointer to the TCB of the task receiving a message.  If you specify a NULL pointer then
*                         the message will be posted to the task's queue of the calling task.  In other words, you'd be
*                         posting a message to yourself.
*
*              p_void     is a pointer to the message to send.
*
*              msg_size   is the size of the message sent (in #bytes)
*
*              opt        specifies whether the post will be FIFO or LIFO:
*
*                             OS_OPT_POST_FIFO       Post at the end   of the queue
*                             OS_OPT_POST_LIFO       Post at the front of the queue
*
*                             OS_OPT_POST_NO_SCHED   Do not run the scheduler after the post
*
*                          Note(s): 1) OS_OPT_POST_NO_SCHED can be added with one of the other options.
*
*
*              ts         is a timestamp indicating when the post occurred.
*
*              p_err      is a pointer to a variable that will hold the error code associated
*                         with the outcome of this call.  Errors can be:
*
*                             OS_ERR_NONE            The call was successful and the message was sent
*                             OS_ERR_MSG_POOL_EMPTY  If there are no more OS_MSGs available from the pool
*                             OS_ERR_Q_MAX           If the queue is full
*                             OS_ERR_STATE_INVALID   If the task is in an invalid state.  This should never happen
*                                                    and if it does, would be considered a system failure.
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_Q_EN == DEF_ENABLED)
void  OS_TaskQPost (OS_TCB       *p_tcb,
                    void         *p_void,
                    OS_MSG_SIZE   msg_size,
                    OS_OPT        opt,
                    CPU_TS        ts,
                    OS_ERR       *p_err)
{
    CPU_SR_ALLOC();



   *p_err = OS_ERR_NONE;                                        /* Assume we won't have any errors                      */
    OS_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {                                    /* Post msg to 'self'?                                  */
        p_tcb = OSTCBCurPtr;
    }
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
        case OS_TASK_STATE_DLY:
        case OS_TASK_STATE_SUSPENDED:
        case OS_TASK_STATE_DLY_SUSPENDED:
             OS_MsgQPut(&p_tcb->MsgQ,                           /* Deposit the message in the queue                     */
                        p_void,
                        msg_size,
                        opt,
                        ts,
                        p_err);
             OS_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_TIMEOUT:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
             if (p_tcb->PendOn == OS_TASK_PEND_ON_TASK_Q) {     /* Is task waiting for a message to be sent to it?      */
                 OS_Post(DEF_NULL,
                         p_tcb,
                         p_void,
                         msg_size,
                         ts);
                 OS_CRITICAL_EXIT_NO_SCHED();
                 if ((opt & OS_OPT_POST_NO_SCHED) == 0u) {
                     OSSched();                                 /* Run the scheduler                                    */
                 }
             } else {
                 OS_MsgQPut(&p_tcb->MsgQ,                       /* No,  Task is pending on something else ...           */
                            p_void,                             /* ... Deposit the message in the task's queue          */
                            msg_size,
                            opt,
                            ts,
                            p_err);
                 OS_CRITICAL_EXIT();
             }
             break;

        default:
             OS_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
             break;
    }
}
#endif


/*
************************************************************************************************************************
*                                               RESUME A SUSPENDED TASK
*
* Description: This function is called to resume a previously suspended task.  This is the only call that will remove an
*              explicit task suspension.
*
* Arguments  : p_tcb      Is a pointer to the task's OS_TCB to resume
*
*              p_err      Is a pointer to a variable that will contain an error code returned by this function
*
*                             OS_ERR_NONE                  if the requested task is resumed
*                             OS_ERR_STATE_INVALID         if the task is in an invalid state
*                             OS_ERR_TASK_NOT_SUSPENDED    if the task to resume has not been suspended
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_SUSPEND_EN == DEF_ENABLED)
void  OS_TaskResume (OS_TCB  *p_tcb,
                     OS_ERR  *p_err)
{
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
   *p_err  = OS_ERR_NONE;
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
        case OS_TASK_STATE_DLY:
        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_TIMEOUT:
             CPU_CRITICAL_EXIT();
            *p_err = OS_ERR_TASK_NOT_SUSPENDED;
             break;

        case OS_TASK_STATE_SUSPENDED:
             OS_CRITICAL_ENTER_CPU_EXIT();
             p_tcb->SuspendCtr--;
             if (p_tcb->SuspendCtr == 0u) {
                 p_tcb->TaskState = OS_TASK_STATE_RDY;
                 OS_RdyListInsert(p_tcb);                       /* Insert the task in the ready list                    */
             }
             OS_CRITICAL_EXIT_NO_SCHED();
             break;

        case OS_TASK_STATE_DLY_SUSPENDED:
             p_tcb->SuspendCtr--;
             if (p_tcb->SuspendCtr == 0u) {
                 p_tcb->TaskState = OS_TASK_STATE_DLY;
             }
             CPU_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_PEND_SUSPENDED:
             p_tcb->SuspendCtr--;
             if (p_tcb->SuspendCtr == 0u) {
                 p_tcb->TaskState = OS_TASK_STATE_PEND;
             }
             CPU_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
             p_tcb->SuspendCtr--;
             if (p_tcb->SuspendCtr == 0u) {
                 p_tcb->TaskState = OS_TASK_STATE_PEND_TIMEOUT;
             }
             CPU_CRITICAL_EXIT();
             break;

        default:
             CPU_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
             return;
    }

    OSSched();
}
#endif


/*
************************************************************************************************************************
*                                              CATCH ACCIDENTAL TASK RETURN
*
* Description: This function is called if a task accidentally returns without deleting itself.  In other words, a task
*              should either be an infinite loop or delete itself if it's done.
*
* Arguments  : none
*
* Returns    : none
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

void  OS_TaskReturn (void)
{
    OS_ERR  err;



    OSTaskReturnHook(OSTCBCurPtr);                              /* Call hook to let user decide on what to do           */
#if (OS_CFG_TASK_DEL_EN == DEF_ENABLED)
    OSTaskDel( DEF_NULL,                                        /* Delete task if it accidentally returns!              */
              &err);
#else
    while (DEF_TRUE) {
        OSTimeDly(OSCfg_TickRate_Hz,
                  OS_OPT_TIME_DLY,
                  &err);
        (void)err;
    }
#endif
}


/*
************************************************************************************************************************
*                                                    SIGNAL A TASK
*
* Description: This function is called to signal a task waiting for a signal.
*
* Arguments  : p_tcb     is the pointer to the TCB of the task to signal.  A NULL pointer indicates that you are sending
*                        a signal to yourself.
*
*              opt       determines the type of POST performed:
*
*                             OS_OPT_POST_NONE         No option
*
*                             OS_OPT_POST_NO_SCHED     Do not call the scheduler
*
*              ts        is a timestamp indicating when the post occurred.
*
*              p_err     is a pointer to an error code returned by this function:
*
*                            OS_ERR_NONE           If the requested task is signaled
*                            OS_ERR_SEM_OVF        If the post would cause the semaphore count to overflow.
*                            OS_ERR_STATE_INVALID  If the task is in an invalid state.  This should never happen
*                                                  and if it does, would be considered a system failure.
*
* Returns    : The current value of the task's signal counter or 0 if called from an ISR
*
* Note(s)    : This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

OS_SEM_CTR  OS_TaskSemPost (OS_TCB  *p_tcb,
                            OS_OPT   opt,
                            CPU_TS   ts,
                            OS_ERR  *p_err)
{
    OS_SEM_CTR  ctr;
    CPU_SR_ALLOC();


#if (OS_CFG_TS_EN == DEF_DISABLED)
    (void)ts;                                                   /* Prevent compiler warning for not using 'ts'          */
#endif

    OS_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {                                    /* Post signal to 'self'?                               */
        p_tcb = OSTCBCurPtr;
    }
#if (OS_CFG_TS_EN == DEF_ENABLED)
    p_tcb->TS = ts;
#endif
   *p_err     = OS_ERR_NONE;                                    /* Assume we won't have any errors                      */
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
        case OS_TASK_STATE_DLY:
        case OS_TASK_STATE_SUSPENDED:
        case OS_TASK_STATE_DLY_SUSPENDED:
             switch (sizeof(OS_SEM_CTR)) {
                 case 1u:
                      if (p_tcb->SemCtr == DEF_INT_08U_MAX_VAL) {
                          OS_CRITICAL_EXIT();
                         *p_err = OS_ERR_SEM_OVF;
                          return (0u);
                      }
                      break;

                 case 2u:
                      if (p_tcb->SemCtr == DEF_INT_16U_MAX_VAL) {
                          OS_CRITICAL_EXIT();
                         *p_err = OS_ERR_SEM_OVF;
                          return (0u);
                      }
                      break;

                 case 4u:
                      if (p_tcb->SemCtr == DEF_INT_32U_MAX_VAL) {
                          OS_CRITICAL_EXIT();
                         *p_err = OS_ERR_SEM_OVF;
                          return (0u);
                      }
                      break;

                 default:
                      break;
             }
             p_tcb->SemCtr++;                                   /* Task signaled is not pending on anything             */
             ctr = p_tcb->SemCtr;
             OS_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_PEND:
        case OS_TASK_STATE_PEND_TIMEOUT:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
             if (p_tcb->PendOn == OS_TASK_PEND_ON_TASK_SEM) {   /* Is task signaled waiting for a signal?               */
                 OS_Post(DEF_NULL,                              /* Task is pending on signal                            */
                         p_tcb,
                         DEF_NULL,
                         0u,
                         ts);
                 ctr = p_tcb->SemCtr;
                 OS_CRITICAL_EXIT_NO_SCHED();
                 if ((opt & OS_OPT_POST_NO_SCHED) == 0u) {
                     OSSched();                                 /* Run the scheduler                                    */
                 }
             } else {
                 switch (sizeof(OS_SEM_CTR)) {
                     case 1u:
                          if (p_tcb->SemCtr == DEF_INT_08U_MAX_VAL) {
                              OS_CRITICAL_EXIT();
                             *p_err = OS_ERR_SEM_OVF;
                              return (0u);
                          }
                          break;

                     case 2u:
                          if (p_tcb->SemCtr == DEF_INT_16U_MAX_VAL) {
                              OS_CRITICAL_EXIT();
                             *p_err = OS_ERR_SEM_OVF;
                              return (0u);
                          }
                          break;

                     case 4u:
                          if (p_tcb->SemCtr == DEF_INT_32U_MAX_VAL) {
                              OS_CRITICAL_EXIT();
                             *p_err = OS_ERR_SEM_OVF;
                              return (0u);
                          }
                          break;

                     default:
                          break;
                 }
                 p_tcb->SemCtr++;                               /* No,  Task signaled is NOT pending on semaphore ...   */
                 ctr = p_tcb->SemCtr;                           /* ... it must be waiting on something else             */
                 OS_CRITICAL_EXIT();
             }
             break;

        default:
             OS_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
             ctr   = 0u;
             break;
    }
    return (ctr);
}


/*
************************************************************************************************************************
*                                          CHECK THE STACK REDZONE OF A TASK
*
* Description: Verify a task's stack redzone.
*
* Arguments  : p_tcb        is a pointer to the base of the stack.
*
*              stk_size     is the size of the stack.
*
* Returns    : none
*
* Caller(s)  : OSTaskStkRedzoneChk() and OSIntExit().
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)
CPU_BOOLEAN   OS_TaskStkRedzoneChk (CPU_STK       *p_base,
                                    CPU_STK_SIZE   stk_size)
{
    CPU_INT32U  i;


#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)
    for (i = 0u; i < OS_CFG_TASK_STK_REDZONE_DEPTH; i++) {
        if (*p_base != (CPU_DATA)OS_STACK_CHECK_VAL) {
            return (DEF_FAIL);
        }
        p_base++;
    }
#else
    p_base = p_base + stk_size - 1u;
    for (i = 0u; i < OS_CFG_TASK_STK_REDZONE_DEPTH; i++) {
        if (*p_base != (CPU_DATA)OS_STACK_CHECK_VAL) {
            return (DEF_FAIL);
        }
        p_base--;
    }
#endif

    return (DEF_OK);
}
#endif


/*
************************************************************************************************************************
*                                          INITIALIZE A REDZONE ENABLED STACK
*
* Description: This functions is used to initialize a stack with Redzone checking.
*
* Arguments  : p_tcb        is a pointer to the base of the stack.
*
*              stk_size     is the size of the stack.
*
* Returns    : If the stack is corrupted (DEF_FAIL) or not (DEF_OK).
*
* Caller(s)  : OSTaskCreate() and OSInit().
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application should not call it.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_STK_REDZONE_EN == DEF_ENABLED)
void  OS_TaskStkRedzoneInit (CPU_STK       *p_base,
                             CPU_STK_SIZE   stk_size)
{
    CPU_STK_SIZE   i;


#if (CPU_CFG_STK_GROWTH == CPU_STK_GROWTH_HI_TO_LO)
    for (i = 0u; i < OS_CFG_TASK_STK_REDZONE_DEPTH; i++) {
        *(p_base + i) = (CPU_DATA)OS_STACK_CHECK_VAL;
    }
#else
    for (i = 0u; i < OS_CFG_TASK_STK_REDZONE_DEPTH; i++) {
        *(p_base + stk_size - 1u - i) = (CPU_DATA)OS_STACK_CHECK_VAL;
    }
#endif
}
#endif


/*
************************************************************************************************************************
*                                                   SUSPEND A TASK
*
* Description: This function is called to suspend a task.  The task can be the calling task if 'p_tcb' is a NULL pointer
*              or the pointer to the TCB of the calling task.
*
* Arguments  : p_tcb    is a pointer to the TCB to suspend.
*                       If p_tcb is a NULL pointer then, suspend the current task.
*
*              p_err    is a pointer to a variable that will receive an error code from this function.
*
*                           OS_ERR_NONE                        If the requested task is suspended
*                           OS_ERR_OS_NOT_RUNNING              If uC/OS-III is not running yet
*                           OS_ERR_SCHED_LOCKED                You can't suspend the current task is the scheduler is
*                                                                locked
*                           OS_ERR_STATE_INVALID               If the task is in an invalid state
*                           OS_ERR_TASK_SUSPEND_CTR_OVF        If the nesting counter overflowed.
*
* Returns    : none
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application should not call it.
*
*              2) You should use this function with great care.  If you suspend a task that is waiting for an event
*                 (i.e. a message, a semaphore, a queue ...) you will prevent this task from running when the event
*                 arrives.
************************************************************************************************************************
*/

#if (OS_CFG_TASK_SUSPEND_EN == DEF_ENABLED)
void  OS_TaskSuspend (OS_TCB  *p_tcb,
                      OS_ERR  *p_err)
{
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    if (p_tcb == DEF_NULL) {                                    /* See if specified to suspend self                     */
        if (OSRunning != OS_STATE_OS_RUNNING) {                 /* Can't suspend self when the kernel isn't running     */
            CPU_CRITICAL_EXIT();
           *p_err = OS_ERR_OS_NOT_RUNNING;
            return;
        }
        p_tcb = OSTCBCurPtr;
    }

    if (p_tcb == OSTCBCurPtr) {
        if (OSSchedLockNestingCtr > 0u) {                       /* Can't suspend when the scheduler is locked           */
            CPU_CRITICAL_EXIT();
           *p_err = OS_ERR_SCHED_LOCKED;
            return;
        }
    }

   *p_err = OS_ERR_NONE;
    switch (p_tcb->TaskState) {
        case OS_TASK_STATE_RDY:
             OS_CRITICAL_ENTER_CPU_EXIT();
             p_tcb->TaskState  =  OS_TASK_STATE_SUSPENDED;
             p_tcb->SuspendCtr = 1u;
             OS_RdyListRemove(p_tcb);
             OS_CRITICAL_EXIT_NO_SCHED();
             break;

        case OS_TASK_STATE_DLY:
             p_tcb->TaskState  = OS_TASK_STATE_DLY_SUSPENDED;
             p_tcb->SuspendCtr = 1u;
             CPU_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_PEND:
             p_tcb->TaskState  = OS_TASK_STATE_PEND_SUSPENDED;
             p_tcb->SuspendCtr = 1u;
             CPU_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_PEND_TIMEOUT:
             p_tcb->TaskState  = OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED;
             p_tcb->SuspendCtr = 1u;
             CPU_CRITICAL_EXIT();
             break;

        case OS_TASK_STATE_SUSPENDED:
        case OS_TASK_STATE_DLY_SUSPENDED:
        case OS_TASK_STATE_PEND_SUSPENDED:
        case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
			if (p_tcb->SuspendCtr == (OS_NESTING_CTR)-1) {
                 CPU_CRITICAL_EXIT();
                *p_err = OS_ERR_TASK_SUSPEND_CTR_OVF;
                 return;
             }
             p_tcb->SuspendCtr++;
             CPU_CRITICAL_EXIT();
             break;

        default:
             CPU_CRITICAL_EXIT();
            *p_err = OS_ERR_STATE_INVALID;
             return;
    }

    if (OSRunning == OS_STATE_OS_RUNNING) {                     /* Only schedule when the kernel is running             */
        OSSched();
    }
}
#endif


/*
************************************************************************************************************************
*                                               CHANGE PRIORITY OF A TASK
*
* Description: This function is called by the kernel to perform the actual operation of changing a task's priority.
*              Priority inheritance is updated if necessary.
*
*
*
* Argument(s): p_tcb        is a pointer to the tcb of the task to change the priority.
*
*              prio_new     is the new priority to give to the task.
*
*
* Returns    : none.
*
* Note(s)    : 1) This function is INTERNAL to uC/OS-III and your application MUST NOT call it.
************************************************************************************************************************
*/

void  OS_TaskChangePrio(OS_TCB  *p_tcb,
                        OS_PRIO  prio_new)
{
    OS_TCB  *p_tcb_owner;
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
    OS_PRIO  prio_cur;
#endif


    do {
        p_tcb_owner = DEF_NULL;
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
        prio_cur    = p_tcb->Prio;
#endif
        switch (p_tcb->TaskState) {
            case OS_TASK_STATE_RDY:
                 OS_RdyListRemove(p_tcb);                       /* Remove from current priority                         */
                 p_tcb->Prio = prio_new;                        /* Set new task priority                                */
                 OS_PrioInsert(p_tcb->Prio);
                 if (p_tcb == OSTCBCurPtr) {
                     OS_RdyListInsertHead(p_tcb);
                 } else {
                     OS_RdyListInsertTail(p_tcb);
                 }
                 break;

            case OS_TASK_STATE_DLY:                             /* Nothing to do except change the priority in the OS_TCB*/
            case OS_TASK_STATE_SUSPENDED:
            case OS_TASK_STATE_DLY_SUSPENDED:
                 p_tcb->Prio = prio_new;                        /* Set new task priority                                */
                 break;

            case OS_TASK_STATE_PEND:
            case OS_TASK_STATE_PEND_TIMEOUT:
            case OS_TASK_STATE_PEND_SUSPENDED:
            case OS_TASK_STATE_PEND_TIMEOUT_SUSPENDED:
                 p_tcb->Prio = prio_new;                        /* Set new task priority                                */
                 switch (p_tcb->PendOn) {                       /* What to do depends on what we are pending on         */
                     case OS_TASK_PEND_ON_FLAG:
#if (OS_CFG_PEND_MULTI_EN == DEF_ENABLED)
                     case OS_TASK_PEND_ON_MULTI:
#endif
                     case OS_TASK_PEND_ON_Q:
                     case OS_TASK_PEND_ON_SEM:
                          OS_PendListChangePrio(p_tcb);
                          break;

                     case OS_TASK_PEND_ON_MUTEX:
#if (OS_CFG_MUTEX_EN == DEF_ENABLED)
                          OS_PendListChangePrio(p_tcb);
                          p_tcb_owner = ((OS_MUTEX *)p_tcb->PendDataTblPtr->PendObjPtr)->OwnerTCBPtr;
                          if (prio_cur > prio_new) {            /* Are we increasing the priority?                      */
                              if (p_tcb_owner->Prio <= prio_new) { /* Yes, do we need to give this prio to the owner?   */
                                  p_tcb_owner = DEF_NULL;
                              } else {
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
                                 TRACE_OS_MUTEX_TASK_PRIO_INHERIT(p_tcb_owner, prio_new);
#endif
                              }
                          } else {
                              if (p_tcb_owner->Prio == prio_cur) { /* No, is it required to check for a lower prio?     */
                                  prio_new = OS_MutexGrpPrioFindHighest(p_tcb_owner);
                                  prio_new = prio_new > p_tcb_owner->BasePrio ? p_tcb_owner->BasePrio : prio_new;
                                  if (prio_new == p_tcb_owner->Prio) {
                                      p_tcb_owner = DEF_NULL;
                                  } else {
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN == DEF_ENABLED))
                                     TRACE_OS_MUTEX_TASK_PRIO_DISINHERIT(p_tcb_owner, prio_new);
#endif
                                  }
                              }
                          }
#endif
                         break;

                     case OS_TASK_PEND_ON_TASK_Q:
                     case OS_TASK_PEND_ON_TASK_SEM:
                     default:
                          break;
                 }
                 break;

            default:
                 return;
        }
        p_tcb = p_tcb_owner;
    } while (p_tcb != DEF_NULL);
}
