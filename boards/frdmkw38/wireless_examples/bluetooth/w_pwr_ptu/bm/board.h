/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _BOARD_H_
#define _BOARD_H_

#include "fsl_common.h"

#include "EmbeddedTypes.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*! @brief The board name */
#define BOARD_NAME "FRDM-KW38"
#define MANUFACTURER_NAME               "NXP"

/*! @brief The UART to use for debug messages. */
#define BOARD_USE_LPUART
#define BOARD_DEBUG_UART_TYPE kSerialPort_Uart
#define BOARD_DEBUG_UART_BASEADDR (uint32_t) LPUART1
#define BOARD_DEBUG_UART_INSTANCE 1U
#define BOARD_DEBUG_UART_CLKSRC kCLOCK_Osc0ErClk
#define BOARD_DEBUG_UART_CLK_FREQ CLOCK_GetOsc0ErClkFreq()
#define BOARD_UART_IRQ LPUART0_LPUART1_IRQn
/*TBD*/
#define BOARD_UART_IRQ_HANDLER LPUART0_LPUART1_IRQHandler

#ifndef BOARD_DEBUG_UART_BAUDRATE
#define BOARD_DEBUG_UART_BAUDRATE 115200
#endif /* BOARD_DEBUG_UART_BAUDRATE */

/*! @brief The i2c instance used for i2c connection by default */
#define BOARD_I2C_BASEADDR I2C0

#define BOARD_ACCEL_I2C_BASEADDR I2C0
#define BOARD_ACCEL_I2C_CLOCK_FREQ CLOCK_GetFreq(I2C0_CLK_SRC)

/*! @brief The CMP instance/channel used for board. */
#define BOARD_CMP_BASEADDR CMP0
#define BOARD_CMP_CHANNEL 1U

/* Board accelerometer driver */
#define BOARD_ACCEL_FXOS

/*! @brief The rtc instance used for board. */
#define BOARD_RTC_FUNC_BASEADDR RTC

/*! DCDC input pin is not tied Up */
//#define gDCDC_PowerSwitchPinTiedUp_d           0

/*! @brief Define the port interrupt number for the board switches */
#ifndef BOARD_SW2_GPIO
#define BOARD_SW2_GPIO GPIOB
#endif
#ifndef BOARD_SW2_PORT
#define BOARD_SW2_PORT PORTB
#endif
#ifndef BOARD_SW2_GPIO_PIN
#define BOARD_SW2_GPIO_PIN 0U
#endif
#define BOARD_SW2_IRQ PORTB_PORTC_IRQn
#define BOARD_SW2_IRQ_HANDLER PORTB_PORTC_IRQHandler
#define BOARD_SW2_NAME "SW2"
#ifndef BOARD_SW3_GPIO
#define BOARD_SW3_GPIO GPIOC
#endif
#ifndef BOARD_SW3_PORT
#define BOARD_SW3_PORT PORTC
#endif
#ifndef BOARD_SW3_GPIO_PIN
#define BOARD_SW3_GPIO_PIN 2U
#endif
#define BOARD_SW3_IRQ PORTB_PORTC_IRQn
#define BOARD_SW3_IRQ_HANDLER PORTB_PORTC_IRQHandler
#define BOARD_SW3_NAME "SW3"
#define BOARD_SW2_SW3_SHARED_IRQ_HANDLER 1

/* Set cPWR_Sw3Wakeup to enable Keyboard SW3 as wakeup source
   Now replaced by PWR_SetWakeupPins() called by application at run time
   Note : Does not support serial peripheral wakeup */
#if (defined(cPWR_Sw3Wakeup) && (cPWR_Sw3Wakeup == 1))
#define BOARD_LLWU_PIN_ENABLE_BITMAP    0x0400
#endif

/* Mapping device pins to registers - Do not change */
#define  BOARD_WkupPin_PTC16_c_RisingDetect    (0x1U)
#define  BOARD_WkupPin_PTC16_c_FallingDetect   (BOARD_WkupPin_PTC16_c_RisingDetect <<1)
#define  BOARD_WkupPin_PTC16_c_BothDetect      (BOARD_WkupPin_PTC16_c_RisingDetect | BOARD_WkupPin_PTC16_c_FallingDetect)
#define  BOARD_WkupPin_PTC17_c_RisingDetect    (BOARD_WkupPin_PTC16_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC17_c_FallingDetect   (BOARD_WkupPin_PTC16_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC17_c_BothDetect      (BOARD_WkupPin_PTC16_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC18_c_RisingDetect    (BOARD_WkupPin_PTC17_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC18_c_FallingDetect   (BOARD_WkupPin_PTC17_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC18_c_BothDetect      (BOARD_WkupPin_PTC17_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC19_c_RisingDetect    (BOARD_WkupPin_PTC18_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC19_c_FallingDetect   (BOARD_WkupPin_PTC18_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC19_c_BothDetect      (BOARD_WkupPin_PTC18_c_BothDetect    << 2)

#define  BOARD_WkupPin_PTA16_c_RisingDetect    (BOARD_WkupPin_PTC19_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTA16_c_FallingDetect   (BOARD_WkupPin_PTC19_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTA16_c_BothDetect      (BOARD_WkupPin_PTC19_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTA17_c_RisingDetect    (BOARD_WkupPin_PTA16_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTA17_c_FallingDetect   (BOARD_WkupPin_PTA16_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTA17_c_BothDetect      (BOARD_WkupPin_PTA16_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTA18_c_RisingDetect    (BOARD_WkupPin_PTA17_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTA18_c_FallingDetect   (BOARD_WkupPin_PTA17_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTA18_c_BothDetect      (BOARD_WkupPin_PTA17_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTA19_c_RisingDetect    (BOARD_WkupPin_PTA18_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTA19_c_FallingDetect   (BOARD_WkupPin_PTA18_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTA19_c_BothDetect      (BOARD_WkupPin_PTA18_c_BothDetect    << 2)

#define  BOARD_WkupPin_PTB0_c_RisingDetect     (BOARD_WkupPin_PTA19_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTB0_c_FallingDetect    (BOARD_WkupPin_PTA19_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTB0_c_BothDetect       (BOARD_WkupPin_PTA19_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTB2_c_RisingDetect     (BOARD_WkupPin_PTB0_c_RisingDetect   << 2)
#define  BOARD_WkupPin_PTB2_c_FallingDetect    (BOARD_WkupPin_PTB0_c_FallingDetect  << 2)
#define  BOARD_WkupPin_PTB2_c_BothDetect       (BOARD_WkupPin_PTB0_c_BothDetect     << 2)

#define  BOARD_WkupPin_PTC2_c_RisingDetect     (BOARD_WkupPin_PTB2_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC2_c_FallingDetect    (BOARD_WkupPin_PTB2_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC2_c_BothDetect       (BOARD_WkupPin_PTB2_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC3_c_RisingDetect     (BOARD_WkupPin_PTC2_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC3_c_FallingDetect    (BOARD_WkupPin_PTC2_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC3_c_BothDetect       (BOARD_WkupPin_PTC2_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC4_c_RisingDetect     (BOARD_WkupPin_PTC3_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC4_c_FallingDetect    (BOARD_WkupPin_PTC3_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC4_c_BothDetect       (BOARD_WkupPin_PTC3_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC5_c_RisingDetect     (BOARD_WkupPin_PTC4_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC5_c_FallingDetect    (BOARD_WkupPin_PTC4_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC5_c_BothDetect       (BOARD_WkupPin_PTC4_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC6_c_RisingDetect     (BOARD_WkupPin_PTC5_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC6_c_FallingDetect    (BOARD_WkupPin_PTC5_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC6_c_BothDetect       (BOARD_WkupPin_PTC5_c_BothDetect    << 2)
#define  BOARD_WkupPin_PTC7_c_RisingDetect     (BOARD_WkupPin_PTC6_c_RisingDetect  << 2)
#define  BOARD_WkupPin_PTC7_c_FallingDetect    (BOARD_WkupPin_PTC6_c_FallingDetect << 2)
#define  BOARD_WkupPin_PTC7_c_BothDetect       (BOARD_WkupPin_PTC6_c_BothDetect    << 2)

/* Mapping the serial and Keyboard wakeup pin to the correct device pins
   Use the Lowpower module API called from application to enable/disable these wakeup pins if needed :
     PWR_SetWakeupPins() and PWR_UnSetWakeupPins() */
#define BOARD_WAKEUP_PIN_KEYBOARD_BITMAP        BOARD_WkupPin_PTC2_c_FallingDetect
#define BOARD_WAKEUP_PIN_SERIAL_BITMAP          BOARD_WkupPin_PTC6_c_BothDetect

/* Board led color mapping */
#define LOGIC_LED_ON 1U
#define LOGIC_LED_OFF 0U
#ifndef BOARD_LED_RED_GPIO
#define BOARD_LED_RED_GPIO GPIOB
#endif
#define BOARD_LED_RED_GPIO_PORT PORTC
#ifndef BOARD_LED_RED_GPIO_PIN
#define BOARD_LED_RED_GPIO_PIN 1U
#endif
#ifndef BOARD_LED_GREEN_GPIO
#define BOARD_LED_GREEN_GPIO GPIOA
#endif
#define BOARD_LED_GREEN_GPIO_PORT PORTA
#ifndef BOARD_LED_GREEN_GPIO_PIN
#define BOARD_LED_GREEN_GPIO_PIN 16U
#endif
#ifndef BOARD_LED_BLUE_GPIO
#define BOARD_LED_BLUE_GPIO GPIOB
#endif
#define BOARD_LED_BLUE_GPIO_PORT PORTB
#ifndef BOARD_LED_BLUE_GPIO_PIN
#define BOARD_LED_BLUE_GPIO_PIN 2U
#endif

#define LED_RED_INIT(output)                                           \
    GPIO_PinWrite(BOARD_LED_RED_GPIO, BOARD_LED_RED_GPIO_PIN, output); \
    BOARD_LED_RED_GPIO->PDDR |= (1U << BOARD_LED_RED_GPIO_PIN) /*!< Enable target LED_RED */
#define LED_RED_ON()                                                                                                   \
    GPIO_PortSet(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN)                     /*!< Turn on target LED_RED \ \ \
                                                                                        */
#define LED_RED_OFF() GPIO_PortClear(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Turn off target LED_RED */
#define LED_RED_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1U << BOARD_LED_RED_GPIO_PIN) /*!< Toggle on target LED_RED */

#define LED_GREEN_INIT(output)                                             \
    GPIO_PinWrite(BOARD_LED_GREEN_GPIO, BOARD_LED_GREEN_GPIO_PIN, output); \
    BOARD_LED_GREEN_GPIO->PDDR |= (1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Enable target LED_GREEN */
#define LED_GREEN_ON() \
    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn on target LED_GREEN */
#define LED_GREEN_OFF() \
    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Turn off target LED_GREEN */
#define LED_GREEN_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_GREEN_GPIO, 1U << BOARD_LED_GREEN_GPIO_PIN) /*!< Toggle on target LED_GREEN */

#define LED_BLUE_INIT(output)                                            \
    GPIO_PinWrite(BOARD_LED_BLUE_GPIO, BOARD_LED_BLUE_GPIO_PIN, output); \
    BOARD_LED_BLUE_GPIO->PDDR |= (1U << BOARD_LED_BLUE_GPIO_PIN)                       /*!< Enable target LED_BLUE */
#define LED_BLUE_ON() GPIO_PortSet(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn on target LED_BLUE */
#define LED_BLUE_OFF() \
    GPIO_PortClear(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Turn off target LED_BLUE */
#define LED_BLUE_TOGGLE() \
    GPIO_PortToggle(BOARD_LED_BLUE_GPIO, 1U << BOARD_LED_BLUE_GPIO_PIN) /*!< Toggle on target LED_BLUE */

#if defined(LOGIC_LED_ON)
#if (LOGIC_LED_ON)
#define TPM_LED_ON_LEVEL kTPM_HighTrue
#else
#define TPM_LED_ON_LEVEL kTPM_LowTrue
#endif
#else
#define TPM_LED_ON_LEVEL kTPM_LowTrue
#endif

/* ERPC DSPI configuration */
#define ERPC_BOARD_DSPI_BASEADDR SPI0
#define ERPC_BOARD_DSPI_BAUDRATE 500000U
#define ERPC_BOARD_DSPI_CLKSRC DSPI0_CLK_SRC
#define ERPC_BOARD_DSPI_CLK_FREQ CLOCK_GetFreq(DSPI0_CLK_SRC)
#define ERPC_BOARD_DSPI_INT_GPIO GPIOC
#define ERPC_BOARD_DSPI_INT_PORT PORTC
#define ERPC_BOARD_DSPI_INT_PIN 5U
#define ERPC_BOARD_DSPI_INT_PIN_IRQ PORTB_PORTC_IRQn
#define ERPC_BOARD_DSPI_INT_PIN_IRQ_HANDLER PORTB_PORTC_IRQHandler

#define gADC16_ReferenceVoltageSource_d kADC16_ReferenceVoltageSourceValt

/* DCDC driver default mode */
#ifndef APP_DCDC_MODE
#define APP_DCDC_MODE    (gDCDC_Mode_Buck_c)
#endif

/* Vbat monitor Interval in Ms */
#ifndef APP_DCDC_VBAT_MONITOR_INTERVAL
#define APP_DCDC_VBAT_MONITOR_INTERVAL    (50000)
#endif

/* Connectivity */
#ifndef APP_SERIAL_INTERFACE_TYPE
#define APP_SERIAL_INTERFACE_TYPE (gSerialMgrLpuart_c)
#endif

/* EVAL use LPUART0 for HCI */
#ifndef APP_SERIAL_INTERFACE_INSTANCE
#define APP_SERIAL_INTERFACE_INSTANCE (0)
#endif

#ifndef APP_SERIAL_INTERFACE_SPEED
#define APP_SERIAL_INTERFACE_SPEED (115200)
#endif

#ifndef gStackTimerInstance_c
#define gStackTimerInstance_c           1
#endif
#ifndef gRedLedPwmTimerInstance_c
#define gRedLedPwmTimerInstance_c       0
#endif
#ifndef gGreenLedPwmTimerInstance_c
#define gGreenLedPwmTimerInstance_c     2
#endif
#ifndef gBlueLedPwmTimerInstance_c
#define gBlueLedPwmTimerInstance_c      2
#endif

#ifndef gRedLedPwmTimerChannel_c
#define gRedLedPwmTimerChannel_c        2
#endif
#ifndef gGreenLedPwmTimerChannel_c
#define gGreenLedPwmTimerChannel_c      1
#endif
#ifndef gBlueLedPwmTimerChannel_c
#define gBlueLedPwmTimerChannel_c       0
#endif
/* Index into the ledPins[] array */
#ifndef gRedLedIdx_c
#define gRedLedIdx_c                    0
#endif


#ifndef BOARD_DBGINITSET
#define BOARD_DBGINITSET(__x, __y)
#endif

#ifndef BOARD_DBGINITDBGIO
#define BOARD_DBGINITDBGIO()
#endif

#ifndef BOARD_DBGAPPIOSET
#define BOARD_DBGAPPIOSET(__x, __y)
#endif

#ifndef BOARD_DBGTOGGLEDBGIO
#define BOARD_DBGTOGGLEDBGIO()
#endif

#ifndef BOARD_DBGCONFIGINIT
#define BOARD_DBGCONFIGINIT(__x)
#endif

#ifndef DBG_LOG_DUMP
#define DBG_LOG_DUMP(__x)
#endif



#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/
void hardware_init(void);

/* Functions used to determine the frequency of a module's input clock. */
uint32_t BOARD_GetLpuartClock(uint32_t instance);
uint32_t BOARD_GetTpmClock(uint32_t instance);
uint32_t BOARD_GetSpiClock(uint32_t instance);
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);
void BOARD_GetMACAddr(uint8_t* aOutMacAddr5B);
void BOARD_DCDCInit(void);
void BOARD_InitAdc(void);
void BOARD_SerialClkInit(void);
bool_t BOARD_WasRtcShutDown(void);
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
void BOARD_InitSerialManager(void);
uint8_t BOARD_GetSerialManagerInterface(void);
void BOARD_SetSerialManagerInterface(uint8_t interface);
#endif
uint8_t BOARD_GetBatteryLevel(void);
uint16_t BOARD_GetPotentiometerLevel(void);
int32_t BOARD_GetTemperature(void);
uint16_t BOARD_GetAdcBatLvl(void);
uint16_t BOARD_GetAdcBgLvl(void);

void BOARD_RTC_Init(void);
void BOARD_RTC_Deinit(void);


/* Increase Core clock frequency to 47972352Hz */
extern void BOARD_SetCoreClock48Mhz(void);

/* Reset core clock to default (20-25MHz) */
extern void BOARD_ResetCoreClock(void);

/* Function called by low power module prior to entering LLS or VLLS2/3 lowpower modes
   It should be used for setting appropriately the IOs to avoid leakage and
     configure the DCDC for lowpower mode  */
extern void BOARD_EnterLowPowerCb(void);

/* Function called by low power module after exiting LLS or VLLS2/3 lowpower modes
   It should be used to restore the IOs configuration,
     and reconfigure the DCDC setting for active mode            */
extern void BOARD_ExitLowPowerCb(void);

/* Function to restore the SOC peripherals before Radio Init
   It is not called from wakeup from LLS mode.
   32MHz external clock is not yet ready at this stage
   The excecution time for this BOARD_ExitLowPowerVllsCb() function added with
     BOARD_EnterLowPowerCb() execution time shall not exceed 279s (TBC) (currently t last 112us TBC)
     so the Radio and Link layer HW are restored on time for the LL event.
     In case longer processing is required on wakeup from VLLS2/3
     then BOARD_ExitLowPowerVllsPostCb() shall be used */
extern void BOARD_ExitLowPowerVllsCb(void);

/* Any processing on wakeup from VLLS2/3 shall be implemented in this function so the
     Link layer hardware restoration timming is not impaired.
   Long processing in this function will reduce the time the device can remain in
     STOP mode.  Even if there is no impact on the Link layer event, adding too much code
     here will increase the active time power consumption        */
extern void BOARD_ExitLowPowerVllsPostCb(void);

/* Deprecated : Lowpower module calls directly the functions BOARD_EnterLowPowerCb(),
    BOARD_ExitLowPowerCb(), BOARD_ExitLowPowerVllsCb(), BOARD_ExitLowPowerVllsPostCb()
   was: Function to install callbacks for actions before and after entering low power. */
extern void BOARD_InstallLowPowerCallbacks(void);

/* Deprecated : All essential device restore from VLLS is now performed directly in the
     lowpower module
   was: Function to install callback for actions after exiting VLLS low power mode. */
extern void BOARD_InstallWarmbootCallback(void);

extern void BOARD_InitDebugConsole(void);
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_I2C_Init(I2C_Type *base, uint32_t clkSrc_Hz);
status_t BOARD_I2C_Send(I2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize);
status_t BOARD_I2C_Receive(I2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize);
void BOARD_Accel_I2C_Init(void);
status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff);
status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize);
#endif /* SDK_I2C_BASED_COMPONENT_USED */

extern uint8_t BOARD_GetXtal32MhzTrim(bool_t regRead);
extern void BOARD_SetXtal32MHzTrim(uint8_t trimValue, bool_t saveToHwParams);
extern uint8_t BOARD_GetRssiAdjustment(bool_t regRead);
extern void BOARD_SetRssiAdjustment(uint8_t newRssiAdjustmentValue, bool_t saveToHwParams);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* _BOARD_H_ */
