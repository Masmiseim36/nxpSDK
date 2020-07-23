/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_FLOW_CHECK_H_
#define _SAFETY_FLOW_CHECK_H_

#include <stdint.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/* The flow-check data type. */
typedef struct
{
    uint32_t ui32Signature;   /* The run-time signature. */
    uint32_t ui32Correctness; /* The run-time correctness register. */
} fs_flow_check_t;


/*
 * Macro for power-on-reset initialization of software flow check.
 * -sVar: The flow-check static variable.
 */
#define FS_FLOW_CHK_INIT(sVar, node_start)\
    (sVar).ui32Signature = (node_start);\
    (sVar).ui32Correctness = 0UL

/*
 * Macro for addition of software flow node.
 * -sVar: The flow-check static variable.
 * -node_new: The new node name.
 */
#define FS_FLOW_CHK_NODE(sVar, node_new)\
     (sVar).ui32Signature ^= ((node_new) ^ (node_new##_PREV));\
     (sVar).ui32Correctness |= ((sVar).ui32Signature ^ (node_new))


/*                         AFTER-RESET FLOW CHECK                             */
/* Default initial node value. */
#define FS_FLCK_AR_NODE_START               (0xD932A000UL)
/* Watchdog periphery initialization node. */
#define FS_FLCK_AR_NODE_WDOG_INIT           (0xC81CA001UL)
#define FS_FLCK_AR_NODE_WDOG_INIT_PREV      (FS_FLCK_AR_NODE_START)
/* Common startup node. */
#define FS_FLCK_AR_NODE_STARTUP             (0xF098A002UL)
#define FS_FLCK_AR_NODE_STARTUP_PREV        (FS_FLCK_AR_NODE_WDOG_INIT)
/* Safety-related peripheral init - clock init node. */
#define FS_FLCK_AR_NODE_FSPER_INIT_CLK      (0x3417A003UL)
#define FS_FLCK_AR_NODE_FSPER_INIT_CLK_PREV (FS_FLCK_AR_NODE_STARTUP)
/* Safety-related peripheral init - MCU pin init node. */
#define FS_FLCK_AR_NODE_FSPER_INIT_PIN      (0x472AA004UL)
#define FS_FLCK_AR_NODE_FSPER_INIT_PIN_PREV (FS_FLCK_AR_NODE_FSPER_INIT_CLK)
/* Safety-related peripheral init - reference backup timer. */
#define FS_FLCK_AR_NODE_FSPER_INIT_TMR      (0x5E73A005UL)
#define FS_FLCK_AR_NODE_FSPER_INIT_TMR_PREV (FS_FLCK_AR_NODE_FSPER_INIT_PIN)
/* Common safety routine init node. */
#define FS_FLCK_AR_NODE_FSTEST_INIT         (0x3C36A006UL)
#define FS_FLCK_AR_NODE_FSTEST_INIT_PREV    (FS_FLCK_AR_NODE_FSPER_INIT_TMR)
/* Watchdog after-reset test node. */
#define FS_FLCK_AR_NODE_FSTEST_WDOG         (0xDB7AA007UL)
#define FS_FLCK_AR_NODE_FSTEST_WDOG_PREV    (FS_FLCK_AR_NODE_FSTEST_INIT)
/* FLASH after-reset test node. */
#define FS_FLCK_AR_NODE_FSTEST_FLASH        (0x1A26A008UL)
#define FS_FLCK_AR_NODE_FSTEST_FLASH_PREV   (FS_FLCK_AR_NODE_FSTEST_WDOG)
/* RAM after-reset test node. */
#define FS_FLCK_AR_NODE_FSTEST_RAM          (0xEDBDA009UL)
#define FS_FLCK_AR_NODE_FSTEST_RAM_PREV     (FS_FLCK_AR_NODE_FSTEST_FLASH)
/* PC register after-reset test node. */
#define FS_FLCK_AR_NODE_FSTEST_PC           (0x7649A00AUL)
#define FS_FLCK_AR_NODE_FSTEST_PC_PREV      (FS_FLCK_AR_NODE_FSTEST_RAM)
/* CPU registers after-reset test node. */
#define FS_FLCK_AR_NODE_FSTEST_CPU          (0xF391A00BUL)
#define FS_FLCK_AR_NODE_FSTEST_CPU_PREV     (FS_FLCK_AR_NODE_FSTEST_PC)
/* STACK after-reset test node. */
#define FS_FLCK_AR_NODE_FSTEST_STACK        (0x23F8A00CUL)
#define FS_FLCK_AR_NODE_FSTEST_STACK_PREV   (FS_FLCK_AR_NODE_FSTEST_CPU)
/* Motor-control ADC periphery initialization node. */
#define FS_FLCK_AR_NODE_MCPER_INIT_ADC      (0x143CA00DUL)
#define FS_FLCK_AR_NODE_MCPER_INIT_ADC_PREV (FS_FLCK_AR_NODE_FSTEST_STACK)
/* Motor-control PDB periphery initialization node. */
#define FS_FLCK_AR_NODE_MCPER_INIT_PDB      (0xE0C7A00EUL)
#define FS_FLCK_AR_NODE_MCPER_INIT_PDB_PREV (FS_FLCK_AR_NODE_MCPER_INIT_ADC)
/* Motor-control slow-loop timer periphery initialization node. */
#define FS_FLCK_AR_NODE_MCPER_INIT_TMR      (0xBC98A00FUL)
#define FS_FLCK_AR_NODE_MCPER_INIT_TMR_PREV (FS_FLCK_AR_NODE_MCPER_INIT_PDB)
/* Motor-control PWM periphery initialization node. */
#define FS_FLCK_AR_NODE_MCPER_INIT_PWM      (0xB9F0A010UL)
#define FS_FLCK_AR_NODE_MCPER_INIT_PWM_PREV (FS_FLCK_AR_NODE_MCPER_INIT_TMR)
/* ADC peripheral driver initialization node. */
#define FS_FLCK_AR_NODE_MCDRV_INIT_ADC      (0x5590A011UL)
#define FS_FLCK_AR_NODE_MCDRV_INIT_ADC_PREV (FS_FLCK_AR_NODE_MCPER_INIT_PWM)
/* Motor-control state-machine initialization node. */
#define FS_FLCK_AR_NODE_MCSM_INIT           (0xA5DCA012UL)
#define FS_FLCK_AR_NODE_MCSM_INIT_PREV      (FS_FLCK_AR_NODE_MCDRV_INIT_ADC)
/* After-reset sequence complete node. */
#define FS_FLCK_AR_NODE_LAST                (FS_FLCK_AR_NODE_MCSM_INIT)


/*                      BACKGROUND LOOP FLOW CHECK                            */
/* Default initial background loop node. */
#define FS_FLCK_BL_NODE_START               (0x4D3AB000UL)
/* Background CPU registry safety test node. */
#define FS_FLCK_BL_FSTEST_CPU               (0xFCF9B001UL)
#define FS_FLCK_BL_FSTEST_CPU_PREV          (FS_FLCK_BL_NODE_START)
/* Background safety FLASH runtime test node. */
#define FS_FLCK_BL_FSTEST_FLASH             (0x9487B002UL)
#define FS_FLCK_BL_FSTEST_FLASH_PREV        (FS_FLCK_BL_FSTEST_CPU)
/* Background safety clock test evaluation node. */
#define FS_FLCK_BL_FSTEST_CLOCK             (0xF7ABB003UL)
#define FS_FLCK_BL_FSTEST_CLOCK_PREV        (FS_FLCK_BL_FSTEST_FLASH)
/* Background stack overflow/underflow runtime test node. */
#define FS_FLCK_BL_FSTEST_STACK             (0x4923B004UL)
#define FS_FLCK_BL_FSTEST_STACK_PREV        (FS_FLCK_BL_FSTEST_CLOCK)
/* Background interrupt handling test evaluation node. */
#define FS_FLCK_BL_FSTEST_ISR               (0x3943B005UL)
#define FS_FLCK_BL_FSTEST_ISR_PREV          (FS_FLCK_BL_FSTEST_STACK)
/* Background loop complete node. */
#define FS_FLCK_BL_NODE_LAST                (FS_FLCK_BL_FSTEST_ISR)


/*                      FAST CONTROL LOOP FLOW CHECK                          */
/* The common node signatures (flow junctions). */

/* Default initial fast control loop node. */
#define FS_FLCK_FL_NODE_START               (0xA957C000UL)

/* The MC.SM state-specific part completed node (common state exit value). */
#define FS_FLCK_FL_MCSM_ST_DONE             (0xB329C010UL)
/* The MC.SM state-machine done node (common MC.SM exit value). */
#define FS_FLCK_FL_MCSM_ALL_DONE            (0x63C1C020UL)

/* The MC.SM ADC driver measurement acquisition node. */
#define FS_FLCK_FL_MCDRV_ADC_MEAS           (0xA485C001UL)
#define FS_FLCK_FL_MCDRV_ADC_MEAS_PREV      (FS_FLCK_FL_NODE_START)
/* The MC.SM common fault detection node. */
#define FS_FLCK_FL_MCSM_FAULT_DIAG          (0x4235C002UL)
#define FS_FLCK_FL_MCSM_FAULT_DIAG_PREV     (FS_FLCK_FL_MCDRV_ADC_MEAS)
/* The MC.SM state-specific fault diagnostics node. */
#define FS_FLCK_FL_MCSM_ST_DIAG             (0xE1E0C003UL)
#define FS_FLCK_FL_MCSM_ST_DIAG_PREV        (FS_FLCK_FL_MCSM_FAULT_DIAG)
/* The MC.SM state transition branch node. */
#define FS_FLCK_FL_MCSM_ST_TRANS            (FS_FLCK_FL_MCSM_ST_DONE)
#define FS_FLCK_FL_MCSM_ST_TRANS_PREV       (FS_FLCK_FL_MCSM_ST_DIAG)
/* MC.SM state control node. */
#define FS_FLCK_FL_MCSM_ST_CTRL             (FS_FLCK_FL_MCSM_ST_DONE)
#define FS_FLCK_FL_MCSM_ST_CTRL_PREV        (FS_FLCK_FL_MCSM_ST_DIAG)
/* The MC.SM ADC driver channel assignment node. */
#define FS_FLCK_FL_MCDRV_ADC_ASSIGN         (0xA849C005UL)
#define FS_FLCK_FL_MCDRV_ADC_ASSIGN_PREV    (FS_FLCK_FL_MCSM_ST_DONE)
/* The MC.SM PWM driver update node. */
#define FS_FLCK_FL_MCDRV_PWM_UPDATE         (FS_FLCK_FL_MCSM_ALL_DONE)
#define FS_FLCK_FL_MCDRV_PWM_UPDATE_PREV    (FS_FLCK_FL_MCDRV_ADC_ASSIGN)

/* The safety uninterruptible tests node. */
#define FS_FLCK_FL_FSTST_UNINTERR           (0xCF3CC006UL)
#define FS_FLCK_FL_FSTST_UNINTERR_PREV      (FS_FLCK_FL_MCSM_ALL_DONE)
/* The safety interrupt handling tests node. */
#define FS_FLCK_FL_FSTST_ISR                (0x6A7CC006UL)
#define FS_FLCK_FL_FSTST_ISR_PREV           (FS_FLCK_FL_FSTST_UNINTERR)

/* Background loop complete node. */
#define FS_FLCK_FL_NODE_LAST                (FS_FLCK_FL_FSTST_ISR)

/*                      SLOW CONTROL LOOP FLOW CHECK                          */
/* Default initial slow control loop node. */
#define FS_FLCK_SL_NODE_START               (0x6090D000UL)
/* Slow control loop safety clock test node. */
#define FS_FLCK_SL_FSTEST_CLOCK             (0x7356D001UL)
#define FS_FLCK_SL_FSTEST_CLOCK_PREV        (FS_FLCK_SL_NODE_START)
/* Slow control loop safety interrupt handling test node. */
#define FS_FLCK_SL_FSTEST_ISR               (0x2460D002UL)
#define FS_FLCK_SL_FSTEST_ISR_PREV          (FS_FLCK_SL_FSTEST_CLOCK)
/* Slow control loop MC.SM state-machine execution. */
#define FS_FLCK_SL_MCSM_CTRL                (0x4A33D003UL)
#define FS_FLCK_SL_MCSM_CTRL_PREV           (FS_FLCK_SL_FSTEST_ISR)
/* Slow control loop complete node. */
#define FS_FLCK_SL_NODE_LAST                (FS_FLCK_SL_MCSM_CTRL)



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* List the valid software modules, in which the after-reset flow check occurs. */
#if defined(_MAIN_H_)               ||\
    defined(_SAFETY_PERIPH_INIT_H_) ||\
    defined(_STARTUP_)              ||\
    defined(_SAFETY_ROUTINES_H_)    ||\
    defined(_MCDRV_PERIPH_INIT_H_)  ||\
    defined(_M1_STATE_MACHINE_H_)

/* The after-reset software flow check. */
/*! @note The variable is defined in safety_routine.c. */
extern fs_flow_check_t g_sfsFlowChkAR;

#endif /* defined("valid module header blockers")  */

/* List the valid software modules, in which the background loop flow check
   occurs. */
#if defined(_SAFETY_ROUTINES_H_)

/* The background loop software flow check. */
/*! @note The variable is defined in safety_routine.c. */
extern fs_flow_check_t g_sfsFlowChkBL;

#endif /* defined("valid module header blockers")  */


/* List the valid software modules, in which the fast-loop flow check occurs. */
#if defined(_SAFETY_ROUTINES_H_)  ||\
    defined(_M1_STATE_MACHINE_H_) ||\
    defined(_MCDRV_ADC_ADC16_H_)  ||\
    defined(_MCDRV_PWM3PH_FTM_H_)

/* The fast-control loop software flow check. */
/*! @note The variable is defined in safety_routine.c. */
extern fs_flow_check_t g_sfsFlowChkFL;

#endif /* defined("valid module header blockers")  */


/* List the valid software modules, in which the slow-loop flow check occurs. */
#if defined(_SAFETY_ROUTINES_H_)  ||\
    defined(_M1_STATE_MACHINE_H_)

/* The slow control loop software flow check. */
/*! @note The variable is defined in safety_routine.c. */
extern fs_flow_check_t g_sfsFlowChkSL;

#endif /* defined("valid module header blockers")  */


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SAFETY_FLOW_CHECK_H_ */
