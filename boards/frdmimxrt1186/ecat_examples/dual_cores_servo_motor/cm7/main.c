/*
 * Copyright 2025 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_common.h"
#include "mc_periph_init.h"
#include "pin_mux.h"
#include "peripherals.h"
#include "fsl_rgpio.h"
#include "fsl_lpuart.h"
#include "m1_sm_servo.h"
#include "m2_sm_servo.h"
#include "board.h"
#include "mu_ipc_shm_client.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Version info */
#define MCRSP_VER "2.0.0" /* motor control package version */

/* Example's feature set in form of bits inside ui16featureSet.
   This feature set is expected to be growing over time.
   ... | FEATURE_S_RAMP | FEATURE_FIELD_WEAKENING | FEATURE_ENC
*/
#define FEATURE_ENC (1)               /* Encoder feature flag */
#define FEATURE_FIELD_WEAKENING (0)   /* Field weakening feature flag */
#define FEATURE_S_RAMP (0)            /* S-ramp feature flag */

#define FEATURE_SET (FEATURE_ENC << (0) | \
                     FEATURE_FIELD_WEAKENING << (1) | \
                     FEATURE_S_RAMP << (2))

/*! @brief The UART to use for FreeMASTER communication */
#define BOARD_FMSTR_UART_PORT LPUART1
#define BOARD_FMSTR_UART_BAUDRATE 115200U

#define BOARD_USER_BUTTON_PRIORITY 4

/* CPU load measurement SysTick START / STOP macros */
#define SYSTICK_START_COUNT() (SysTick->VAL = SysTick->LOAD)
#define SYSTICK_STOP_COUNT(par1)   \
    uint32_t val  = SysTick->VAL;  \
    uint32_t load = SysTick->LOAD; \
    par1          = load - val

/* Three instruction added after interrupt flag clearing as required */
#define M1_END_OF_ISR \
    {                 \
        __DSB();      \
        __ISB();      \
    }


/*******************************************************************************
 * Variables
 ******************************************************************************/
static int64_t i64EthercatPosTargetM1 = 0x0;
static int64_t i64EthercatPosTargetM2 = 0x0;
static int32_t i32EthercatM1PosCurrMT = 0x0;
static int32_t i32EthercatM2PosCurrMT = 0x0;
static int32_t i32EthercatM1PosOldST = -1;
static int32_t i32EthercatM2PosOldST = -1;
static int32_t i32InitializedM1 = 0;
static int32_t i32InitializedM2 = 0;
extern struct param_t *g_param;
uint64_t ui64EndatData;

/* Structure used in FM to get required ID's */
app_ver_t g_sAppIdFM = {
    "../../../examples/_boards/frdmimxrt1186/ecat_examples/dual_cores_servo_motor/remote/cm7",        /* User Path 1- the highest priority */
    "",       /* User Path 2 */
    "frdmimxrt1186", /* board id */
    "pmsm_servo_dual",     /* example id */
    MCRSP_VER,      /* sw version */
    FEATURE_SET,    /* example's feature-set */
};


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//static void BOARD_InitUART(uint32_t u32BaudRate);

/* EnDat2.2 interrupt */
RAM_FUNC_LIB
void M2_FLEXIO_IRQHandler(void);
RAM_FUNC_LIB
void M1_FLEXIO_IRQHandler(void);

static void BOARD_InitSysTick(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

int32_t GetM1PositionActualValue(void)
{
    return (g_sM1Enc.endat2Master.st | (i32EthercatM1PosCurrMT << 25))& 0xFFFFFFFF;
}

int32_t GetM2PositionActualValue(void)
{
   return (g_sM2Enc.endat2Master.st | (i32EthercatM2PosCurrMT << 25))& 0xFFFFFFFF;
}

acc32_t GetM1PositionCmdValue(int32_t targetPos)
{
    int32_t i32MultTurn = 0;
    uint16_t u16SingleTurn = 0;
    int32_t delta;
    delta = ((int32_t)targetPos - (int32_t)(i64EthercatPosTargetM1 & 0xFFFFFFFF));
    i64EthercatPosTargetM1 += delta;
    i32MultTurn = (i64EthercatPosTargetM1 >> 25) & 0x1FFFFFFFF ;
    u16SingleTurn = (i64EthercatPosTargetM1 >> 10) & 0x7FFF;
    return (acc32_t)((i32MultTurn << 15 ) + u16SingleTurn);
}

acc32_t GetM2PositionCmdValue(int32_t targetPos)
{
    int32_t i32MultTurn = 0;
    uint16_t u16SingleTurn = 0;
    int32_t delta;
    delta = ((int32_t)targetPos - (int32_t)(i64EthercatPosTargetM2 & 0xFFFFFFFF));
    i64EthercatPosTargetM2 += delta;
    i32MultTurn = (i64EthercatPosTargetM2 >> 25) & 0x1FFFFFFFF ;
    u16SingleTurn = (i64EthercatPosTargetM2 >> 10) & 0x7FFF;
    return (acc32_t)((i32MultTurn << 15 ) + u16SingleTurn);
}

int Cia402_status_machine_trans(uint8_t axis, uint8_t trans_id, struct param_t *g_param)
{
   int ret = 0;
   if (axis == 0) {
       switch (trans_id) {
           case 0: break;
           case 1: break;
           case 2: break;
           case 3:
               M1_OpenPWM();
               break;
           case 4:
               M1_SetAppSwitch(1);
               i64EthercatPosTargetM1 = g_sM1Enc.i64RevCounter * (33554432) + g_sM1Enc.ui64EndatPosition;
               if (g_sM1Ctrl.eState != kSM_AppRun || g_eM1StateRun != kRunState_Spin)
               {
                   ret = -1;
               }
               break;
           case 5:
           case 6:
               M1_SetAppSwitch(0);
               M1_ClosePWM();
               break;
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
               M1_ClosePWM();
               break;
       }
   } else if (axis == 1){
       switch (trans_id) {
           case 0: break;
           case 1: break;
           case 2: break;
           case 3: M2_OpenPWM(); break;
           case 4:
               M2_SetAppSwitch(1);
               i64EthercatPosTargetM2 = g_sM2Enc.i64RevCounter * (33554432) + g_sM2Enc.ui64EndatPosition;
               if (g_sM2Ctrl.eState != kSM_AppRun || g_eM2StateRun != kRunState_Spin) {
                   ret = -1;
               }
               break;
           case 5:
           case 6:
               M2_SetAppSwitch(0);
               M2_ClosePWM();
               break;
           case 7:
           case 8:
           case 9:
           case 10:
           case 11:
           case 12:
           case 13:
           case 14:
           case 15:
               M2_ClosePWM();
               break;
       }
   }
   return ret;
}

int obj_write_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    uint8_t *p = pData;
    if (Index == 0x6060 && Subindex == 0x0) {
        if (p[0] == 0x08) {
            g_sM1Drive.eControl = kControlMode_PositionFOC;
        }
    } else if (Index == 0x6860 && Subindex == 0x0) {
        if (p[0] == 0x08) {
            g_sM2Drive.eControl = kControlMode_PositionFOC;
        }
    } 
    return 0;
}

int obj_read_callback(uint16_t Index, uint8_t Subindex, uint8_t size, void *pData)
{
    uint32_t *p = pData;
    if (Index == 0x6064 && Subindex == 0x0) {
        *p = GetM1PositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x606c && Subindex == 0x0) {
        *p = (uint32_t)g_sM1Drive.sSpeed.fltSpeedFilt;
        return sizeof(*p);
    }  else  if (Index == 0x6864 && Subindex == 0x0) {
        *p = GetM2PositionActualValue();
        return sizeof(*p);
    } else if (Index == 0x686c && Subindex == 0x0) {
        *p = (uint32_t)g_sM2Drive.sSpeed.fltSpeedFilt;
        return sizeof(*p);
    }
    return 0;
}

/*!
 * @brief Main function
 */
int main(void)
{
    /*Accessing ID structure to prevent optimization*/
    g_sAppIdFM.ui16FeatureSet = FEATURE_SET;

    uint32_t ui32PrimaskReg;

    /* Disable all interrupts before peripherals are initialized */
    ui32PrimaskReg = DisableGlobalIRQ();

    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    PRINTF("cm7_Core running\r\n");
	/* Waiting until MU_ipc_shm_master_init() is completed on CM33 core */
    MU_ipc_shm_client_init();
    
    /* SysTick initialization for CPU load measurement */
    BOARD_InitSysTick();
    /* Init peripheral motor control driver for motors M1 and M2 */
    MCDRV_Init();
    /* Turn off application */
    M1_SetAppSwitch(FALSE);
    M2_SetAppSwitch(FALSE);

    /* Enable interrupts */
    EnableGlobalIRQ(ui32PrimaskReg);

    /* Enable PWM clock */
    g_sM1Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);    /* Enable PWM for Motor 1 */
    g_sM2Pwm3ph.pui32PwmBaseAddress->MCTRL |= PWM_MCTRL_RUN(0xF);    /* Enable PWM for Motor 2 */
    /* Infinite loop */
    while (1)
    {
        if (!i32InitializedM1) {
            if (g_param->axis[0].axis_is_active) {
                M1_MCDRV_SINC_INIT();
                M1_MCDRV_FLEXIO_ENDAT2P2_INIT();
                i32InitializedM1 = 1;
            }
        }

        if (!i32InitializedM2) {
            if (g_param->axis[1].axis_is_active) {
                M2_MCDRV_SINC_INIT();
                M2_MCDRV_FLEXIO_ENDAT2P2_INIT();
                i32InitializedM2 = 1;
            }
        }
    }
}

/* Endat2.2 IRQ handler for M1 */
RAM_FUNC_LIB
void M1_FLEXIO_IRQHandler(void)
{
    int32_t detal = 0;
    M1_MCDRV_SINC_GET(&g_sM1Curr3phDcBus);
    /* get position from EnDat2.2 */
    M1_MCDRV_FLEXIO_ENDAT2P2_GET(&g_sM1Enc);
    FLEXIO_ENDAT2_ClearRxStatusFlags(&g_sM1Enc.endat2Master);

    if (!(i32EthercatM1PosOldST < 0)) {
        detal = (int32_t)g_sM1Enc.endat2Master.st - i32EthercatM1PosOldST;
        if (detal < -16777216) {
            i32EthercatM1PosCurrMT++;
        }
        else if (detal > 16777216) {
            i32EthercatM1PosCurrMT--;
        }
    }
    i32EthercatM1PosOldST = g_sM1Enc.endat2Master.st;

    /* M2 State machine */
    SM_StateMachineFast(&g_sM1Ctrl);

    SDK_ISR_EXIT_BARRIER;
}

RAM_FUNC_LIB
void M2_FLEXIO_IRQHandler(void)
{
    int32_t detal = 0;
    M2_MCDRV_SINC_GET(&g_sM2Curr3phDcBus);
    /* get position from EnDat2.2 */
    M2_MCDRV_FLEXIO_ENDAT2P2_GET(&g_sM2Enc);
    FLEXIO_ENDAT2_ClearRxStatusFlags(&g_sM2Enc.endat2Master);

    if (!(i32EthercatM2PosOldST < 0)) {
        detal = (int32_t)g_sM2Enc.endat2Master.st - i32EthercatM2PosOldST;
        if (detal < -16777216) {
            i32EthercatM2PosCurrMT++;
        }
        else if (detal > 16777216) {
            i32EthercatM2PosCurrMT--;
        }
    }
    i32EthercatM2PosOldST = g_sM2Enc.endat2Master.st;

    /* M2 State machine */
    SM_StateMachineFast(&g_sM2Ctrl);

    SDK_ISR_EXIT_BARRIER;
}

/*!
 * @brief   motor_slow_task called with EtherCAT DC period and processes following functions:
 *           - motor slow application machine function
 */
RAM_FUNC_LIB
int motor_slow_task(int axis, struct pdo_to_motor_t *pdo_m, struct pdo_to_esc_t *pdo_e, struct param_t *param)
{
    if (axis == 0) {
        M1_SetPosition(GetM1PositionCmdValue(pdo_m->Target_pos));
        SM_StateMachineSlow(&g_sM1Ctrl);
        pdo_e->Actual_pos = GetM1PositionActualValue();
        pdo_e->Actual_vel = (uint32_t)(g_sM1Drive.sSpeed.fltSpeedFilt);

    } else if (axis == 1) {
        M2_SetPosition(GetM2PositionCmdValue(pdo_m->Target_pos));
        SM_StateMachineSlow(&g_sM2Ctrl);
        pdo_e->Actual_pos = GetM2PositionActualValue();
        pdo_e->Actual_vel = (uint32_t)(g_sM2Drive.sSpeed.fltSpeedFilt);
    }
    return 0;
}

/*!
 *@brief      SysTick initialization for CPU cycle measurement
 *
 *@param      none
 *
 *@return     none
 */
static void BOARD_InitSysTick(void)
{
    /* Initialize SysTick core timer to run free */
    /* Set period to maximum value 2^24*/
    SysTick->LOAD = 0xFFFFFF;

    /*Clock source - System Clock*/
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;

    /*Start Sys Timer*/
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}
