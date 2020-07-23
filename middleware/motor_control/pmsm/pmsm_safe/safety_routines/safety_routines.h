/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_ROUTINES_H_
#define _SAFETY_ROUTINES_H_

#include "safety_cfg.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*!
 * @name Safety IEC60730 Class B tests handling functions
 * @{
 */

/*!
 * @brief   Common safety initialization function and watchdog enable/disable.
 *
 * @details This function initializes the safety structure variables of
 *          following tests:
 *          -Clock test - FS_CLK_Init from IEC60730 library is called.
 *          -Flash test - Function initializes the flash test variables.
 *          -RAM test   - Initialization of the RAM test variables.
 *          -Stack test - This function calls FS_CM0_STACK_Init function from
 *                        IEC60730 library. The function fills the areas below
 *                        and above the stack with a defined pattern (see
 *                        safety_cfg.h) and initializes the remaining tests.
 *
 * @note    The function refreshes the watchdog.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsCommonInit_AR(void);

/*!
 * @brief   Handling of the safety functions that are non-interruptible must be
 *          called in the highest priority interrupt routine.
 *
 * @details This function switches between safety functions that are called in
 *          interrupt to lower the immediate workload. Following test functions
 *          are executed:
 *          -FS_fsCpuTest_FL() - Uninterruptible test of CPU registers.
 *          -FS_fsPcTest_ARFL() - Program counter test.
 *          -FS_fsRamTestSaRam_FL() - Run-time test of safety RAM.
 *          -FS_fsRamTestStack_FL() - Run-time test of Stack RAM.
 *          -FS_fsWdogRefresh_FL() - Safety watchdog runtime refresh.
 *
 * @note    Refreshes the watchdog.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsUninterruptibleTests_FL(void);


/*                            WATCHDOG TEST                                   */
/*!
 * @brief   After-reset safety watchdog test.
 *
 * @details This function is used to test the watchdog reset capability after
 *          reset. The LPTMR is used as test timer with independent source
 *          clock. Limits for WDOG timeout are calculated within function.
 *          Function operates with g_sfsTestWdog global variable.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsWatchdogTest_AR(void);



/*                            CLOCK TEST                                      */
/*!
 * @brief   Clock test status capture function, called from interrupt.
 *
 * @details This function calls clock test function from the IEC60730B library
 *          and enable the test evaluation. This function must be called in the
 *          investigated ISR (as configured in the clock test initialization
 *          function).
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsClockTestIsr_SL(void);

/*!
 * @brief   Clock test check function.
 *
 * @details This function can be called from any place of application.
 *          It calls the FS_CLK_Check function from the IEC60730 library
 *          In case of incorrect clock test result, it updates the ui32Errors
 *          variable accordingly.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsClockTestCheck_BL(void);

/*                              FLASH TEST                                    */
/*!
 * @brief   After-reset Flash test.
 *
 * @details This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested all at once. The test
 *          result is evaluated as well and safety error handler
 *          FS_ErrorHandling() is entered if necessary.
 *
 * @note    Using SW breakpoints for debugging will corrupt the test!
 * @note    The function refreshes the watchdog.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsFlashTest_AR(void);

/*!
 * @brief   Run-time Flash test.
 *
 * @details This function calls the flash test function from IEC60730 library.
 *          Safety-related part of the flash is tested in sequence, block by
 *          block. In case of incorrect flash test result, the safety error
 *          handler FS_ErrorHandling() is called.
 *
 * @note    Using SW breakpoints for debugging will corrupt the test!
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsFlashTest_BL(void);

/*                               RAM TEST                                     */
/*!
 * @brief   After-reset RAM test.
 *
 * @details This function calls the RAM test function from IEC60730 library.
 *          Safety-related part of the RAM is tested all at once. The test
 *          result is evaluated as well and safety error handler
 *          FS_ErrorHandling() is entered if necessary.
 *
 * @note    The function refreshes the watchdog.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsRamTest_AR(void);


/*                                 STACK TEST                                 */
/*!
 * @brief   After-reset underflow/overflow stack test.
 *
 * @details This function calls the FS_CM0_STACK_Test test function from
 *          IEC60730 library. Stack is tested for underflow and overflow
 *          condition. The test is interruptible and can be performed after
 *          restart and during runtime. In case of incorrect RAM test result,
 *          the safety error handler FS_ErrorHandling() is called.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsStackTest_AR(void);

/*!
 * @brief   Background loop underflow/overflow stack test.
 *
 * @details This function calls the FS_CM0_STACK_Test test function from
 *          IEC60730 library. Stack is tested for underflow and overflow
 *          condition. The test is interruptible and can be performed after
 *          restart and during runtime. In case of incorrect RAM test result,
 *          the safety error handler FS_ErrorHandling() is called.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsStackTest_BL(void);

/*                           PROGRAM COUNTER TEST                             */
/*!
 * @brief   Program counter after-reset test.
 *
 * @details This function uses two addresses: the first is the address of
 *          the iec60730b_cm0_pc_object.o  object, which is defined in linker
 *          file, and the second address comes as function argument (must be RAM
 *          address). Both addresses must be defined by the developer and
 *          suitable to test all of the possible PC bits. This test can’t be
 *          interrupted. In case of incorrect test result, the safety error
 *          handler FS_ErrorHandling() is called.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsPcTest_AR(void);

/*                          CPU REGISTRY TESTS                                */
/*!
 * @brief   After-reset CPU registers test.
 *
 * @details This function calls the CPU test functions from IEC60730 library.
 *          All the writable registers are tested at once. The errors are
 *          handled as follows:
 *          -R0-R7, R12, LR, APSR, MSP, and PSP: Stuck in the endless loop
 *           (WDOG reset is necessary):
 *          -R8-R11, PRIMASK, CONTROL: Safety error handler is entered:
 *
 * @note    The function refreshes the watchdog.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsCpuTest_AR(void);



/*!
 * @brief   Background test of CPU registers.
 *
 * @details This function calls the CPU test functions from IEC60730 library.
 *          The function can be called from the background loop. The errors are
 *          handled as follows:
 *          -R0-R7, R12, LR, and APSR: Stuck in the endless loop
 *           (WDOG reset is necessary):
 *          -R8-R11, CONTROL, PSP: Safety error handler is entered:
 *
 * @note    The interrupts are globally disabled for the duration of the CPU
 *          CONTROL and PSP register safety test.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsCpuTest_BL(void);


/*                        INTERRUPT HANDLING TEST                             */
/*!
 * @brief   Fast control loop interrupt handling test.
 *
 * @details This function check whether the fast control loop interrupt service
 *          routine is being executed too often when compared to the slow
 *          control loop ISR.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsISRTest_FL(void);

/*!
 * @brief   Slow control loop interrupt handling test.
 *
 * @details This function checks:
 *          -Whether the number of fast control loop ISR executions is in
 *           tolerance.
 *          -Whether the slow control loop ISR is not executed too often.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsISRTest_SL(void);

/*!
 * @brief   Background interrupt handling test.
 *
 * @details This function checks whether the slow control loop ISR was not
 *          executed too many times (or the background loop is not executed
 *          often enough).
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsISRTest_BL(void);

/*                          SOFTWARE FLOW TEST                                */
/*!
 * @brief   Function evaluate the after-reset software flow.
 *
 * @details This function checks the ui32Correctness member of g_sfsFlowChkAR.
 *          This structure was modified within software flow nodes via
 *          FS_FLOW_CHK_NODE(). If any node was executed out of order
 *          or not at all, the test will fail.
 *          Function also initializes BL, FL, and SL software flow checks.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsFlowCheck_AR(void);

/*!
 * @brief   Function evaluate the background loop software flow.
 *
 * @details This function checks the ui32Correctness member of g_sfsFlowChkBL.
 *          This structure was modified within software flow nodes via
 *          FS_FLOW_CHK_NODE(). If any node was executed out of order
 *          or not at all, the test will fail.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsFlowCheck_BL(void);

/*!
 * @brief   Function evaluate the fast control loop loop software flow.
 *
 * @details This function checks the ui32Correctness member of g_sfsFlowChkFL.
 *          This structure was modified within software flow nodes via
 *          FS_FLOW_CHK_NODE(). If any node was executed out of order
 *          or not at all, the test will fail.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsFlowCheck_FL(void);

/*!
 * @brief   Function evaluate the fast control loop loop software flow.
 *
 * @details This function checks the ui32Correctness member of g_sfsFlowChkSL.
 *          This structure was modified within software flow nodes via
 *          FS_FLOW_CHK_NODE(). If any node was executed out of order
 *          or not at all, the test will fail.
 *
 * @param   void
 *
 * @return  none
 */
void FS_fsFlowCheck_SL(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SAFETY_ROUTINES_H_ */
