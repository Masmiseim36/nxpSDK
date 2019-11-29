/*
* Copyright 2016-2017 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef PWRM_H_
#define PWRM_H_

/*!
\file       pwrm.h
\brief      Manage the microcontrollers power and transition between
            the various power states
*/
     
/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/

#include <jendefs.h>
#if (!defined __JN518X__) && (!defined CPU_MKW41Z512VHT4)
#include <AppHardwareApi.h>
#endif
#ifdef CPU_MKW41Z512VHT4
#include "PWR_Interface.h"
#endif
/****************************************************************************/
/***        Macro Definitions                                             ***/
/****************************************************************************/

#ifndef PWRM_INLINE
#define PWRM_INLINE INLINE
#endif

#ifndef PWRM_ALWAYS_INLINE
#define PWRM_ALWAYS_INLINE ALWAYS_INLINE
#endif

#if (defined JENNIC_CHIP_FAMILY) && (JENNIC_CHIP_FAMILY == PC)
#undef PWRM_ALWAYS_INLINE /* stop inlining  for unit tests */
#define PWRM_ALWAYS_INLINE
#endif

#define PWRM_CALLBACK(name) void pwrm_##name(void)

#define PWRM_DECLARE_CALLBACK_DESCRIPTOR(name) \
    PWRM_CALLBACK(name); \
    pwrm_tsCallbackDescriptor pwrm_##name##_desc = { NULL, pwrm_##name }

#define PWRM_vRegisterWakeupCallback(name) pwrm_vRegisterWakeupCallback(&pwrm_##name##_desc)
#define PWRM_vRegisterPreSleepCallback(name) pwrm_vRegisterPreSleepCallback(&pwrm_##name##_desc)

/****************************************************************************/
/***        Type Definitions                                              ***/
/****************************************************************************/

typedef struct _pwrm_tsCallbackDescriptor {
    struct _pwrm_tsCallbackDescriptor *psNext;
    void (*prCallback)(void);
} pwrm_tsCallbackDescriptor;

typedef struct _sWakeTimerEvent {
    struct _sWakeTimerEvent  *psNext;
    uint32 u32TickDelta;
    void (*prCallbackfn)(void);
    uint8 u8Status;
} pwrm_tsWakeTimerEvent;

typedef enum {
    PWRM_E_OK,
    PWRM_E_ACTIVITY_OVERFLOW,
    PWRM_E_ACTIVITY_UNDERFLOW,
    PWRM_E_TIMER_RUNNING,
    PWRM_E_TIMER_FREE,
    PWRM_E_TIMER_INVALID
} PWRM_teStatus;
#if ((defined JENNIC_CHIP_FAMILY) && ((defined JENNIC_CHIP_FAMILY == JN516x) || (defined JENNIC_CHIP_FAMILY == JN517x)))
typedef teAHI_SleepMode PWRM_tePowerMode;
#else
typedef uint8 PWRM_tePowerMode;
#endif
/****************************************************************************/
/***        External Variables                                            ***/
/****************************************************************************/

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/

PUBLIC void PWRM_vInit(PWRM_tePowerMode ePowerMode);
#ifndef  PATCH_ENABLE
PUBLIC void PWRM_vManagePower(void);
#endif
PUBLIC PWRM_teStatus PWRM_eScheduleActivity(
        pwrm_tsWakeTimerEvent *psWake,
        uint32 u32Ticks,
        void (*prCallbackfn)(void));
PUBLIC void PWRM_vWakeInterruptCallback(void);
#if (defined JENNIC_CHIP_FAMILY) && (JENNIC_CHIP_FAMILY == JN514x)
PUBLIC void PWRM_vSetupDozeMonitor(bool_t bUseIO, bool_t bUseTimers, bool_t b32uSec);
PUBLIC void PWRM_vResetDozeTimers(void);
PUBLIC uint32 PWRM_u32GetDozeTime(void);
PUBLIC uint32 PWRM_u32GetDozeElapsedTime(void);
#endif

#if (defined JENNIC_CHIP_FAMILY)&&(JENNIC_CHIP_FAMILY == JN516x)
// lpsw3603: No timers on a '68, only IO
PUBLIC void PWRM_vSetupDozeMonitor(bool_t bUseIO);
#endif

PUBLIC void vAppMain(void);
PUBLIC void vAppRegisterPWRMCallbacks(void);
#ifdef CPU_MKW41Z512VHT4
PUBLIC void PWRM_vWakeupCallback(void);
PUBLIC void PWRM_vPreSleepCallback(void);
#endif
/****************************************************************************/
/***        Inline Functions                                              ***/
/****************************************************************************/

PWRM_INLINE PWRM_teStatus PWRM_eStartActivity(void) PWRM_ALWAYS_INLINE;
PWRM_INLINE PWRM_teStatus PWRM_eStartActivity(void) /* [I SP001262_fr 119] */
{
#ifdef CPU_MKW41Z512VHT4
	PWR_DisallowDeviceToSleep();
    return PWRM_E_OK;
#else
    /* [I SP001262_fr 120, 121, 122] */
    extern volatile uint16 s_u16ActivityCounter;
    return (0xffff != s_u16ActivityCounter) ? (s_u16ActivityCounter++, PWRM_E_OK) : PWRM_E_ACTIVITY_OVERFLOW;
#endif
}

PWRM_INLINE PWRM_teStatus PWRM_eFinishActivity(void) PWRM_ALWAYS_INLINE;
PWRM_INLINE PWRM_teStatus PWRM_eFinishActivity(void) /* [I SP001262_fr 123] */
{
#ifdef CPU_MKW41Z512VHT4
    PWR_AllowDeviceToSleep();
    return PWRM_E_OK;
#else
    /* [I SP001262_fr 124, 125, 126] */
    extern volatile uint16 s_u16ActivityCounter;
    return (0 != s_u16ActivityCounter) ? ( s_u16ActivityCounter--, PWRM_E_OK) : PWRM_E_ACTIVITY_UNDERFLOW;
#endif
}

PWRM_INLINE uint16 PWRM_u16GetActivityCount(void) PWRM_ALWAYS_INLINE;
PWRM_INLINE uint16 PWRM_u16GetActivityCount(void) /* [I SP001262_fr 166] */
{
#ifdef CPU_MKW41Z512VHT4
	extern uint8_t mLPMFlag;
    return mLPMFlag ;
#else
    extern volatile uint16 s_u16ActivityCounter;
    return s_u16ActivityCounter; /* [I SP001262_fr 167] */
#endif
}

#if (defined JENNIC_CHIP_FAMILY) && (JENNIC_CHIP_FAMILY == JN513x)
PWRM_INLINE void PWRM_vTickTimerActivated(void) PWRM_ALWAYS_INLINE;
PWRM_INLINE void PWRM_vTickTimerActivated(void)
{
    extern volatile bool_t s_bTickTimerActive;
    s_bTickTimerActive = TRUE;  /* [I SP001262_fr 153] */
}

PWRM_INLINE void PWRM_vTickTimerDeactivated(void) PWRM_ALWAYS_INLINE;
PWRM_INLINE void PWRM_vTickTimerDeactivated(void)
{
    extern volatile bool_t s_bTickTimerActive;
    s_bTickTimerActive = FALSE; /* [I SP001262_fr 155] */
}
#endif

/****************************************************************************/
/***        Private Functions                                             ***/
/****************************************************************************/

PUBLIC void pwrm_vRegisterWakeupCallback(pwrm_tsCallbackDescriptor *psCBDesc );
PUBLIC void pwrm_vRegisterPreSleepCallback(pwrm_tsCallbackDescriptor *psCBDesc );

#endif /*PWRMAN_H_*/

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
