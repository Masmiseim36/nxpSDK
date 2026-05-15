/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MC_PERIPH_INIT_H_
#define _MC_PERIPH_INIT_H_


#include "fsl_common.h"
#include "fsl_xbar.h"
#include "fsl_clock.h"

#include "board.h"

#include "mcdrv_pwm3ph_pwma.h"
#include "mcdrv_flexio_endat2p2.h"
#include "mcdrv_sinc2.h"
#include "fsl_flexio_endat2p2.h"
#include "m1_pmsm_appconfig.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Structure used during clocks and modulo calculations */
typedef struct _clock_setup
{
    uint32_t ui32FastPeripheralClock;
    uint32_t ui32CpuFrequency;
    uint32_t ui32BusClock;
    uint32_t ui32SysPllClock;
    uint16_t ui16M1SpeedLoopFreq;   
    uint16_t ui16M1SpeedLoopModulo;
    uint16_t ui16M1PwmFreq;
    uint16_t ui16M1PwmModulo; 
    uint16_t ui16M1PwmDeadTime;  
} clock_setup_t;

/******************************************************************************
 * Clock & PWM definition for motor 1
 ******************************************************************************/
#define M1_PWM_FREQ (32000)         /* PWM frequency - 16kHz */
#define M1_FOC_FREQ_VS_PWM_FREQ (2) /* FOC calculation is called every n-th PWM reload */
#define M1_SPEED_LOOP_FREQ (4000)   /* Speed loop frequency */
#define M1_PWM_DEADTIME (500)       /* Output PWM deadtime value in nanoseconds */

/* Over Current Fault detection */
#define M1_FAULT_NUM (0)
#define M1_FAULT_OV_NUM (1)

/* Braking resistor macros */
#define M1_BRAKE_SET()
#define M1_BRAKE_CLEAR()
/* DC bus braking threshold hysteresis */
#define M1_U_DCB_HYSTERESIS (0.05F)
   
/******************************************************************************
 * Define common phase currents and DC bus measurement functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_CURR_3PH_VOLT_DCB_GET(par)    /* Need to be empty - SINC data are read and processed in SINC IRQ. */

#define M1_MCDRV_CURR_3PH_CHAN_ASSIGN(par)      /* Keep this macro to avoid project build error (macro is used in MID). */
#define M1_MCDRV_CURR_3PH_CALIB_INIT(par) 
#define M1_MCDRV_CURR_3PH_CALIB(par)
#define M1_MCDRV_CURR_3PH_CALIB_SET(par)

/******************************************************************************
 * Define motor 1 slow control loop timer
 ******************************************************************************/
#define M1_MCDRV_TMR_SLOWLOOP_INIT() InitTMR1()

/******************************************************************************
 * Define 3-ph PWM control functions for motor 1
 ******************************************************************************/
#define M1_MCDRV_PWM_PERIPH_INIT() (M1_InitPWM())
#define M1_MCDRV_PWM3PH_SET(par) (MCDRV_eFlexPwm3PhSet(par))
#define M1_MCDRV_PWM3PH_EN(par) (MCDRV_eFlexPwm3PhOutEn(par))
#define M1_MCDRV_PWM3PH_DIS(par) (MCDRV_eFlexPwm3PhOutDis(par))
#define M1_MCDRV_PWM3PH_FLT_GET(par) (MCDRV_eFlexPwm3PhFltGet(par))
#define M1_MCDRV_PWM3PH_FLT_OV_GET(par) (MCDRV_eFlexPwm3PhFlt2Get(par))
        
/******************************************************************************
 * Define SINC filter
 ******************************************************************************/

#define PWM_CLK_MC                      (132000000U)
#define SINC_CLK_MC                     (16500000U)

#define SINC3_ORD		        (3U)	// Third-order filter
#define SINC3_OSR			(63U)   // 64x oversampling (PFOSR + 1)
#define SINC3_IRQ_PRIORITY		(0U)
#define SINC3_NUM_CHANNELS_USED		(4U)
#define SINC3_PRETRIGGER_VAL            ((SINC3_ORD * (SINC3_OSR + 1U)) + ((SINC3_OSR + 1U) / 2U))      // 3*OSR + OSR/2 ... first sample (OSR*ORD)/MCLK, next samples OSR/MCLK
#define SINC3_PWM_CLK_RATION            (PWM_CLK_MC / SINC_CLK_MC)
#define M1_MCDRV_SINC_INIT()            (M1_InitSinc())
#define M1_MCDRV_SINC_GET(par)          (MCDRV_SincReadPhCurrDcBusVolt(par))

/* Init sensors pointers */
#define M1_SET_PTR_U_DC_BUS(par)       (g_sM1Curr3phDcBus.pfltUDcBus = &(par))
#define M1_SET_PTR_I_ABC(par)          (g_sM1Curr3phDcBus.psIABC = &(par))
#define M1_SET_PTR_SECTOR(par)         (g_sM1Curr3phDcBus.pui16SVMSector = &(par))
#define M1_SET_PTR_AUX_CHAN(par)

/******************************************************************************
 * Define position and speed sensor for motor 1 - EnDat2.2
 ******************************************************************************/

/* Example specific position/speed sensor defines */
#define M1_MCDRV_FLEXIO_ENDAT2P2_INIT()         (M1_InitFlexIOEndat2p2())
#define M1_MCDRV_FLEXIO_ENDAT2P2_GET(par)       (MCDRV_Endat2p2DataRead(par))

#define M1_FLEXIO_INSTANCE                      FLEXIO1
#define M1_FLEXIO_CLOCK_ROOT	                kCLOCK_Root_Flexio1
#define M1_FLEXIO_CLOCK_ROOT_MUX                kCLOCK_FLEXIO1_ClockRoot_MuxSysPll3Div2 // tuto ma wenbin chybu
#define M1_FLEXIO_IRQHandler                    FLEXIO1_IRQHandler
#define M1_FLEXIO_IRQn                          FLEXIO1_IRQn

/* ENCODER_ECN_1325 */
#define M1_FLEXIO_ENDAT2P2_BIT_RATE             (8000000U)
#define M1_FLEXIO_ENDAT2P2_MT_LEN               (0U)
#define M1_FLEXIO_ENDAT2P2_ST_LEN               (25U)
#define M1_FLEXIO_ENDAT2P2_RECOVERY_TIME_III    (800U)  // 900ns

#define M1_FLEXIO_ENDAT2P2_RXD_CHANNEL          (0U) // M1_ENDATA_DATA_IN_ENC_A
#define M1_FLEXIO_ENDAT2P2_TXD_CHANNEL          (3U) // M1_ENDAT_DATA_OUT_ENC_HOME
#define M1_FLEXIO_ENDAT2P2_CLK_CHANNEL          (2U) // M1_ENDAT_CLK_ENC_I
#define M1_FLEXIO_ENDAT2P2_DIR_CHANNEL          (1U) // M1_ENDAT_DIR_ENC_B

/* Common position/speed sensor defines */
#define M1_MCDRV_ENC_GET_DATA_FAST(par)         (MCDRV_EnDatGetPositionFoc(par))
#define M1_MCDRV_ENC_GET_DATA_SLOW(par)         (MCDRV_EnDatGetPositionFullAndSpeed(par))
#define M1_MCDRV_ENC_CLEAR(par)                 (MCDRV_Endat2p2Clear(par))
#define M1_MCDRV_ENC_SET_OFFSET(par)            (MCDRV_Endat2p2SetOffset(par)) 
#define M1_MCDRV_ENC_SET_DIRECTION(par)
#define M1_MCDRV_ENC_SET_PULSES(par)
#define M1_MCDRV_ENC_GET_POSITION(par)
#define M1_MCDRV_ENC_GET_SPEED(par)
    
#define USER_J3_16_OFF()     RGPIO_PortClear(BOARD_INITPINS_TP_J3_16_GPIO, 1U << BOARD_INITPINS_TP_J3_16_GPIO_PIN)      /*!< Turn off target USER_LED */
#define USER_J3_16_ON()      RGPIO_PortSet(BOARD_INITPINS_TP_J3_16_GPIO, 1U << BOARD_INITPINS_TP_J3_16_GPIO_PIN)        /*!<Turn on target USER_LED*/
#define USER_J3_16_TOGGLE()  RGPIO_PortToggle(BOARD_INITPINS_TP_J3_16_GPIO, 1u << BOARD_INITPINS_TP_J3_16_GPIO_PIN)     /*!< Toggle target USER_LED */
/******************************************************************************
 * Global variable definitions
 ******************************************************************************/

extern mcdrv_pwm3ph_pwma_t g_sM1Pwm3ph;
extern clock_setup_t g_sClockSetup;
extern mcdrv_sinc_t g_sM1Curr3phDcBus;
extern mcdrv_endat2p2_t g_sM1Enc;

/*******************************************************************************
 * API
 ******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

void MCDRV_Init(void);
void InitClock(void);
void InitTMR1(void);
void M1_InitPWM(void);
void M1_InitFlexIOEndat2p2(void);
void M1_InitSinc(void);

#ifdef __cplusplus
}
#endif

#endif /* _MC_PERIPH_INIT_H_  */
