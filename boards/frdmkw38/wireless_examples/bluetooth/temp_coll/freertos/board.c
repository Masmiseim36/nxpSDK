/*
 * Copyright 2017-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************************
* Include
************************************************************************************/

#include <stdint.h>
#include "fsl_common.h"
#include "fsl_gpio.h"
#include "board.h"
#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
#include "fsl_i2c.h"
#endif /* SDK_I2C_BASED_COMPONENT_USED */
#include "app.h"
#include "fsl_port.h"
#include "pin_mux.h"
#include "fsl_pmc.h"
#include "fsl_adc16.h"
#include "GPIO_Adapter.h"
#include "TMR_Adapter.h"
#include "RNG_Interface.h"
#include "FunctionLib.h"
#include "RNG_Interface.h"
#if cPWR_UsePowerDownMode
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

#if (defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1))
#include "fsl_debug_console.h"
#endif

#if gDCDC_Enabled_d
#include "DCDC.h"
#endif

#include "Keyboard.h"
#include "SerialManager.h"
#include "Flash_Adapter.h"
#include "fsl_rtc.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Checks on compile switches  */
#if defined(gPWR_UsePswitchMode_d) && (gPWR_UsePswitchMode_d == 1)
#if (!defined(gDCDC_Enabled_d) || (gDCDC_Enabled_d == 0)) || (!defined(gDCDC_PowerSwitchPinTiedUp_d) || (gDCDC_PowerSwitchPinTiedUp_d == 1))
#warning gPWR_UsePswitchMode_d requires gDCDC_Enabled_d to be set to 1 and gDCDC_PowerSwitchPinTiedUp_d to be set to 0
#endif
#if (APP_DCDC_MODE != gDCDC_Buck_c)
#error Pswitch mode can only be used when DCDC configured in Buck mode
#endif
#endif

/************************************************************************************
* Private type definitions and macros
************************************************************************************/

/*! @brief Default trimming value for 32MHz crystal on KW38 */
#define BOARD_32MHZ_XTAL_TRIM_DEFAULT         0x3AU

/*! @brief Default Cload configuration for 32KHz crystal on KW38
    Cload can be adjusted from 0 to 30pF with 4 capacitor in RTC_CR:
        SC2P: enable/disable 2pF load
        SC4P: enable/disable 4pF load
        SC8P: enable/disable 8pF load
        SC16P: enable/disable 16pF load
    Note: refer to RTC_CR_SC2P_MASK RTC_CR_SC4P_MASK RTC_CR_SC8P_MASK RTC_CR_SC16P_MASK*/
#ifndef BOARD_32KHZ_CLOAD_DEFAULT
#define BOARD_32KHZ_CLOAD_DEFAULT (uint16_t)(0U)
#endif

/************************************************************************************
* Private memory declarations
************************************************************************************/
#if ((defined(gBoard_ADC_uses_mutex_d)) && (gBoard_ADC_uses_mutex_d > 0U))
static osaMutexId_t mADCMutexId;
#endif
static const adc16_config_t adcConfig = {
  .referenceVoltageSource = gADC16_ReferenceVoltageSource_d,
  .clockSource = kADC16_ClockSourceAlt0, // bus clock
  .enableAsynchronousClock = FALSE,
  .clockDivider = kADC16_ClockDivider8,
  .resolution = kADC16_ResolutionSE12Bit,
  .longSampleMode = kADC16_LongSampleDisabled,
  .enableHighSpeed = FALSE,
  .enableLowPower = FALSE,
  .enableContinuousConversion = FALSE
};

#if ((defined(gDCDC_Enabled_d)) && (gDCDC_Enabled_d == 1U))
static const dcdcConfig_t mDcdcDefaultConfig =
{
#if APP_DCDC_MODE == gDCDC_Buck_c
  .vbatMin = 1800U,
  .vbatMax = 4200U,
#endif
  .dcdcMode = (uint8_t)APP_DCDC_MODE,
  .vBatMonitorIntervalMs = APP_DCDC_VBAT_MONITOR_INTERVAL,
  .pfDCDCAppCallback = NULL, /* .pfDCDCAppCallback = DCDCCallback, */
  .dcdcMcuVOutputTargetVal = gDCDC_McuV_OutputTargetVal_1_500_c,
#if defined(gDCDC_3p3V_Output_d) && (gDCDC_3p3V_Output_d == 1U)
  .dcdc1P8OutputTargetVal = gDCDC_1P8OutputTargetVal_3_300_c
#else
  .dcdc1P8OutputTargetVal = gDCDC_1P8OutputTargetVal_1_800_c
#endif
};

#endif

/*! @brief Clock configuration structure. */
typedef struct _clock_config
{
    mcg_config_t mcgConfig;       /*!< MCG configuration.      */
    sim_clock_config_t simConfig; /*!< SIM configuration.      */
    osc_config_t oscConfig;       /*!< OSC configuration.      */
    uint32_t coreClock;           /*!< core clock frequency.   */
} clock_config_t;

/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
#define ADCDAC_LDO_OVERRIDE           (1)
#define ADC16_INSTANCE                (0)   /* ADC instance */
#define ADC16_CHN_GROUP               (0)   /* ADC group configuration selection */
#define ADC16_USES_HARDWARE_AVERAGE   (0)
#ifndef gBoard_ADC_uses_mutex_d
#define gBoard_ADC_uses_mutex_d       (0)
#endif
#define ADC16_BATLVL_CHN              (23) /* Potentiometer channel */
#define ADC16_BL_LOWER_LIMIT          (0) /* min percentage of battery charge */
#define ADC16_BL_UPPER_LIMIT          (100) /* max percentage of battery charge */
#define ADC16_BL_DYNAMIC_RANGE        (ADC16_BL_UPPER_LIMIT - ADC16_BL_LOWER_LIMIT) /* Range = [ADC16_HB_LOWER_LIMIT .. ADC16_HB_LOWER_LIMIT + ADC16_HB_DYNAMIC_RANGE] */

#define ADC16_TEMP_SENSOR_CHN         (26) /* ADC channel of the Temperature Sensor */
#define ADC16_BANDGAP_CHN             (27) /* ADC channel of BANDGAP Voltage reference */

#define MIN_VOLT_BUCK                 (180U)
#define MAX_VOLT_BUCK                 (310U)

#if defined(GCOV_DO_COVERAGE) && (GCOV_DO_COVERAGE == 1)
extern uint16_t gcov_FULL_BAT;
#define FULL_BAT (gcov_FULL_BAT)
#endif

#ifndef FULL_BAT
#define FULL_BAT                      (100U)
#endif

#define EMPTY_BAT                     (0U)

#define ADCR_VDD                      (4095U)     /* Maximum value when use 12b resolution */
#define V_BG                          (1000U)     /* BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25                      (716U)      /* Typical converted value at 25 oC in mV */
#define M                             (1740U)     /* Typical slope:uV/oC */
#define STANDARD_TEMP                 (25)


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if defined(gBoard_ManageSwdPinsInLowPower_d) && (gBoard_ManageSwdPinsInLowPower_d > 0)
/* Variables to store the PCR register value for SWD_DIO and SWD_CLK pins */
static uint32_t mSWDIO_PCR_Save;
static uint32_t mSWDCLK_PCR_Save;
#endif

/* 32MHz Xtal trim */
static uint8_t Xtal32MhzTrim = BOARD_32MHZ_XTAL_TRIM_DEFAULT;

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
#define BOARD_SERIAL_MGR_IF_INVALID           0xFFu
static uint8_t gSerMgrIf     = BOARD_SERIAL_MGR_IF_INVALID;
#endif

static bool_t mRTC_WasShutDown = FALSE;

/************************************************************************************
*************************************************************************************
* Private functions Declarations
*************************************************************************************
************************************************************************************/
extern void NMI_Handler(void);
/* ADC routines */
static uint16_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode);
static void DCDC_AdjustVbatDiv4(void);
static void ADC16_CalibrateParams(void);
static void ADC_Hardware_Init(void);

#if defined(gBoard_ManageSwdPinsInLowPower_d) && (gBoard_ManageSwdPinsInLowPower_d > 0)
static void BOARD_SetSWDPinsLowPower(bool_t isLowPower);
#endif

static void BOARD_CheckRTCStatus(void);

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
/* Initialize the serial clock */
void BOARD_SerialClkInit(void)
{
    /* Set LPUART0 clock source. */
    CLOCK_SetLpuart0Clock(2U);
}

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
/* Initialize the serial manager */
void BOARD_InitSerialManager(void)
{
    serialStatus_t ret;

    /* Init Serial Manager */
    Serial_InitManager();

    /* Register Serial Manager interface */
    ret = Serial_InitInterface(&gSerMgrIf, APP_SERIAL_INTERFACE_TYPE, APP_SERIAL_INTERFACE_INSTANCE);
    assert(ret == gSerial_Success_c );
    NOT_USED(ret);

    /* Set serial baud rate */
    ret = Serial_SetBaudRate(gSerMgrIf, APP_SERIAL_INTERFACE_SPEED);
    assert(ret == gSerial_Success_c);
    NOT_USED(ret);
}

uint8_t BOARD_GetSerialManagerInterface(void)
{
    uint8_t serialInterface;

    /* Get the serial interface*/
    assert( gSerMgrIf != BOARD_SERIAL_MGR_IF_INVALID );
    serialInterface = gSerMgrIf;
    return serialInterface;
}

void BOARD_SetSerialManagerInterface(uint8_t interface)
{
    /* raise assert error if serial interface already initialized */
    assert(gSerMgrIf == BOARD_SERIAL_MGR_IF_INVALID);
    assert(interface != BOARD_SERIAL_MGR_IF_INVALID);
    /* Set Serial interface to manage */
    gSerMgrIf = interface;
}
#endif

bool_t BOARD_WasRtcShutDown(void)
{
    /* Tell if the RTC was shutdown before going to lowpower*/
    return mRTC_WasShutDown;
}

void hardware_init(void)
{
    static uint8_t initialized = 0;

    BOARD_DBGINITSET(0,1);
    BOARD_DBGINITSET(0,0);

    if( initialized == 0U )
    {
        /* We check if the 32kHz oscillator was shut down before
           going VLLS0/1, so LowPower module can know if waiting for 32k osc restart is needed or not
           (see PWR_WaitFor32kOscRestart) */
        BOARD_CheckRTCStatus();

        /* Initialize the hardware */
        BOARD_InitHardware();

        (void)NV_ReadHWParameters(&gHardwareParameters); /* Load the HW parameters from Flash to RAM */

        if(0xFFFFFFFFU != gHardwareParameters.xtalTrim)
        {
            Xtal32MhzTrim = (uint8_t)gHardwareParameters.xtalTrim;
        }
    }

    BOARD_DBGINITSET(0,1);

    BOARD_DBGCONFIGINIT(!initialized);

    initialized = 1;
 }

uint32_t BOARD_GetLpuartClock(uint32_t instance)
{
    uint32_t lpuartClock;

    /* Get lpuart clock frequency */
    lpuartClock = BOARD_DEBUG_UART_CLK_FREQ;
    return lpuartClock;
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    uint32_t uartClkSrcFreq;

    /* Set the pins for the Debug Console using UART1 */
    BOARD_InitPins();

    /* Set LPUART1 clock source to OSCERCLK clock */
    CLOCK_SetLpuart1Clock(2U);

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
#endif
}

/* Get tpm clock frequency */
uint32_t BOARD_GetTpmClock(uint32_t instance)
{
    uint32_t tpmClock;
    uint32_t clockSource = (SIM->SOPT2 & SIM_SOPT2_TPMSRC_MASK) >> SIM_SOPT2_TPMSRC_SHIFT;

    instance = instance; /* Remove compiler warnings */

    switch(clockSource)
    {
    case 1: /* MCGFLLCLK */
        tpmClock = CLOCK_GetFllFreq();
        break;
    case 2: /* OSCERCLK */
        tpmClock = CLOCK_GetOsc0ErClkFreq();
        break;
    case 3: /* MCGIRCLK */
        tpmClock = CLOCK_GetInternalRefClkFreq();
        break;
    default: /* Clock disabled */

        tpmClock  = 0;
        break;
    }
    return tpmClock;
}

#if defined(SDK_I2C_BASED_COMPONENT_USED) && SDK_I2C_BASED_COMPONENT_USED
void BOARD_I2C_Init(I2C_Type *base, uint32_t clkSrc_Hz)
{
    i2c_master_config_t i2cConfig = {0};

    I2C_MasterGetDefaultConfig(&i2cConfig);
    I2C_MasterInit(base, &i2cConfig, clkSrc_Hz);
}

status_t BOARD_I2C_Send(I2C_Type *base,
                        uint8_t deviceAddress,
                        uint32_t subAddress,
                        uint8_t subaddressSize,
                        uint8_t *txBuff,
                        uint8_t txBuffSize)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.direction      = kI2C_Write;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = txBuff;
    masterXfer.dataSize       = txBuffSize;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(base, &masterXfer);
}

status_t BOARD_I2C_Receive(I2C_Type *base,
                           uint8_t deviceAddress,
                           uint32_t subAddress,
                           uint8_t subaddressSize,
                           uint8_t *rxBuff,
                           uint8_t rxBuffSize)
{
    i2c_master_transfer_t masterXfer;

    /* Prepare transfer structure. */
    masterXfer.slaveAddress   = deviceAddress;
    masterXfer.subaddress     = subAddress;
    masterXfer.subaddressSize = subaddressSize;
    masterXfer.data           = rxBuff;
    masterXfer.dataSize       = rxBuffSize;
    masterXfer.direction      = kI2C_Read;
    masterXfer.flags          = kI2C_TransferDefaultFlag;

    return I2C_MasterTransferBlocking(base, &masterXfer);
}

void BOARD_Accel_I2C_Init(void)
{
    BOARD_I2C_Init(BOARD_ACCEL_I2C_BASEADDR, BOARD_ACCEL_I2C_CLOCK_FREQ);
}

status_t BOARD_Accel_I2C_Send(uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint32_t txBuff)
{
    uint8_t data = (uint8_t)txBuff;

    return BOARD_I2C_Send(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, &data, 1);
}

status_t BOARD_Accel_I2C_Receive(
    uint8_t deviceAddress, uint32_t subAddress, uint8_t subaddressSize, uint8_t *rxBuff, uint8_t rxBuffSize)
{
    return BOARD_I2C_Receive(BOARD_ACCEL_I2C_BASEADDR, deviceAddress, subAddress, subaddressSize, rxBuff, rxBuffSize);
}
#endif /* SDK_I2C_BASED_COMPONENT_USED */

uint32_t BOARD_GetSpiClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

static void BOARD_SetNMIPinMux(port_mux_t mux)
{
    /* Enable port clock */
    CLOCK_EnableClock(kCLOCK_PortB);

    PORT_SetPinMux(PORTB, 18, mux);
}

void NMI_Handler(void)
{
    /* Set NMI pin mux */
    BOARD_SetNMIPinMux(kPORT_MuxAsGpio);
}

/* Initialize ADC module*/
void BOARD_InitAdc(void)
{
    static bool_t initialized = FALSE;
    if (!initialized)
    {
        initialized = TRUE;
#if gBoard_ADC_uses_mutex_d
        mADCMutexId = OSA_MutexCreate();
        if( mADCMutexId == NULL )
        {
            panic(0,0,0,0);
        }
#endif
        /* Initialize ADC hardware */
        ADC_Hardware_Init();

    }
    ADC16_CalibrateParams();
}

/*Get the board battery level*/
uint8_t BOARD_GetBatteryLevel(void)
{
    uint16_t batVal, bandgapValue, batLvl, batVolt, bgVolt = 100U; /*cV*/

    bandgapValue = BOARD_GetAdcBgLvl();
    DCDC_AdjustVbatDiv4(); /* Bat voltage  divided by 4 */
    batVal = BOARD_GetAdcBatLvl() * 4U; /* Need to multiply the value by 4 because the measured voltage is divided by 4*/

    batVolt = bgVolt * batVal / bandgapValue;

    batLvl = (batVolt - MIN_VOLT_BUCK) * (FULL_BAT - EMPTY_BAT) / (MAX_VOLT_BUCK - MIN_VOLT_BUCK);
    return ((batLvl <= 100U) ? (uint8_t)batLvl : 100U);
}


/* Initialize DCDC. */
void BOARD_DCDCInit(void)
{
#if defined(gDCDC_Enabled_d) && (gDCDC_Enabled_d == 1)
    /* Initialize DCDC module */
    (void)DCDC_Init(&mDcdcDefaultConfig);
#endif
}


/* get 4 words of information that uniquely identifies the MCU */
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen)
{
    uint32_t uid[4] = {0};

    /* Get the MCU uid */
    uid[0] = SIM->SDID;
    uid[1] = SIM->UIDMH;
    uid[2] = SIM->UIDML;
    uid[3] = SIM->UIDL;

    FLib_MemCpy(aOutUid16B, (uint8_t*)uid, sizeof(uid));
    /* Get the uid length */
    *pOutLen = (uint8_t)sizeof(uid);

    return;
}

/* Get the device MAC ADDRESS of 40 bits */
void BOARD_GetMACAddr(uint8_t* aOutMacAddr5B)
{
    /* Get the MAC address from RSIM->MAC_MSB */
    aOutMacAddr5B[4] = (uint8_t)(RSIM->MAC_MSB    )&0xFFU;
    /* Get the MAC address from RSIM->MAC_LSB */
    aOutMacAddr5B[3] = (uint8_t)(RSIM->MAC_LSB>>24)&0xFFU;
    aOutMacAddr5B[2] = (uint8_t)(RSIM->MAC_LSB>>16)&0xFFU;
    aOutMacAddr5B[1] = (uint8_t)(RSIM->MAC_LSB>>8 )&0xFFU;
    aOutMacAddr5B[0] = (uint8_t)(RSIM->MAC_LSB    )&0xFFU;

    return;
}

void BOARD_InstallLowPowerCallbacks(void)
{
#if cPWR_UsePowerDownMode
  PWR_RegisterLowPowerEnterCallback((pfPWRCallBack_t)BOARD_EnterLowPowerCb); /* Register LowPower Enter Callback*/
  PWR_RegisterLowPowerExitCallback((pfPWRCallBack_t)BOARD_ExitLowPowerCb); /* Register LowPower Exit Callback */
#endif
}

/* Keep for backward compatibility with KW36 - deprecated on KW38
   Now merged with BOARD_InstallLowPowerCallbacks() */
void BOARD_InstallWarmbootCallback(void)
{
}
#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
static void BOARD_LPUARTPrepareEnterLowPower(void)
{
  /* set LPUART0 TX pin to low power */
  /* keep RX pin , it is used as wakeup source */
  GpioSetPinMux_ISF_Preserved(gpioPort_C_c, 7, pinMux_PinDisabledOrAnalog_c);
}
#endif

#if defined(gBoard_ManageSwdPinsInLowPower_d) && (gBoard_ManageSwdPinsInLowPower_d > 0)
static void BOARD_SetSWDPinsLowPower(bool_t isLowPower)
{
    bool_t clock_portA = FALSE;
    uint32_t sim_scgc5 = SIM->SCGC5 ;

    /* Activate PORTA clock if disabled */
    if((sim_scgc5 & SIM_SCGC5_PORTA_MASK)==0)
    {
        clock_portA = TRUE;
        SIM->SCGC5 = sim_scgc5 | SIM_SCGC5_PORTA_MASK;
    }

    if(isLowPower)
    {
        /* Store SWDIO PCR value */
        mSWDIO_PCR_Save = GpioGetPinPCR(gpioPort_A_c, 0);
        /* Store SWDCLK PCR value */
        mSWDCLK_PCR_Save = GpioGetPinPCR(gpioPort_A_c, 1);
        /* Disable SWDIO pin */
        GpioSetPinMux_ISF_Preserved(gpioPort_A_c, 0, pinMux_PinDisabledOrAnalog_c);
        /* Disable SWDCLK pin */
        GpioSetPinMux_ISF_Preserved(gpioPort_A_c, 1, pinMux_PinDisabledOrAnalog_c);
    }
    else
    {
        /* Enable SWDIO pin */
        GpioSetPinPCR_ISF_Preserved(gpioPort_A_c, 0, mSWDIO_PCR_Save);
        /* Disable SWDIO pin */
        GpioSetPinPCR_ISF_Preserved(gpioPort_A_c, 1, mSWDCLK_PCR_Save);
    }

    /* Restore PORTA clock settings */
    if(clock_portA)
    {
        SIM->SCGC5 = sim_scgc5;
    }
}
#endif

void BOARD_RTC_Init(void)
{
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

    /* clean Cload configuration */
    RTC->CR &= (uint16_t)~(RTC_CR_SC2P_MASK | RTC_CR_SC4P_MASK | RTC_CR_SC8P_MASK | RTC_CR_SC16P_MASK);

    if ((RTC->CR & RTC_CR_OSCE_MASK) == 0u)
    {
        /* RTC_CR: OSCE=1 and set optimized Cload configuration to avoid high clock drift */
        RTC->CR |= RTC_CR_OSCE_MASK | BOARD_32KHZ_CLOAD_DEFAULT;
    }
    else
    {
        /* set optimized Cload configuration to avoid high clock drift */
        RTC->CR |= BOARD_32KHZ_CLOAD_DEFAULT;
    }

    /* We don t use RTC module for now, just the OSC32K. Make sure Interrupts are disabled */
    RTC_DisableInterrupts( RTC, (uint32_t)kRTC_TimeInvalidInterruptEnable
                | (uint32_t)kRTC_TimeOverflowInterruptEnable
                | (uint32_t)kRTC_AlarmInterruptEnable
                | (uint32_t)kRTC_SecondsInterruptEnable    );
}


void BOARD_RTC_Deinit(void)
{
    if((SIM->SCGC6 & (uint32_t)SIM_SCGC6_RTC_MASK) != 0U)
    {
        /* switch off 32kHz oscillator */
        RTC->CR &= ~RTC_CR_OSCE_MASK;
    }

    RTC_DisableInterrupts( RTC, (uint32_t)kRTC_TimeInvalidInterruptEnable
                | (uint32_t)kRTC_TimeOverflowInterruptEnable
                | (uint32_t)kRTC_AlarmInterruptEnable
                | (uint32_t)kRTC_SecondsInterruptEnable    );
}

void BOARD_SetCoreClock48Mhz(void)
{
    /* Set core clock to 48Mhz */
    MCG->C4 |= MCG_C4_DRST_DRS(1) | MCG_C4_DMX32(1);
}

void BOARD_ResetCoreClock(void)
{
    /* Set core clock to default clock (20-25MHz) */
    MCG->C4 &= (uint8_t)(~(MCG_C4_DRST_DRS(1) | MCG_C4_DMX32(1)));
}

void BOARD_EnterLowPowerCb(void)
{
#if gKeyBoardSupported_d
#if gKBD_TsiElectdCount_c
   /* keyboard enter low power */
    KBD_PrepareEnterLowPower();
#endif
#endif

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    /* lpuart enter low power */
    BOARD_LPUARTPrepareEnterLowPower();
#endif

#if defined(gBoard_ManageSwdPinsInLowPower_d) && (gBoard_ManageSwdPinsInLowPower_d > 0)
    BOARD_SetSWDPinsLowPower(TRUE);
#endif

#if gDCDC_Enabled_d
    DCDC_EnterLowPowerCb();
#endif
}

void BOARD_ExitLowPowerCb(void)
{
    BOARD_DBGINITSET(2,0);

#if defined(gBoard_ManageSwdPinsInLowPower_d) && (gBoard_ManageSwdPinsInLowPower_d > 0)
    BOARD_SetSWDPinsLowPower(FALSE);
#endif

#if gDCDC_Enabled_d
    DCDC_ExitLowPowerCb();
#endif

#if defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0)
    /* Set up Serial interface clock */
    BOARD_SerialClkInit();

    /* Set up LPUART0 pin for serial */
    BOARD_InitLPUART();
#endif

    BOARD_DBGINITSET(2,1);
}

#if (cPWR_EnableDeepSleepMode_8) || (cPWR_EnableDeepSleepMode_5) || (cPWR_EnableDeepSleepMode_9)
void BOARD_ExitLowPowerVllsCb(void)
{
    BOARD_DBGINITSET(2,0);

    /* Reinit button pins
        Note : Keyboard pin reinit is done directly in KBD_PrepareExitLowPower */
    BOARD_InitButtons();

#if (gKeyBoardSupported_d && (!defined(gDbgUseDtest) || (gDbgUseDtest == 0)))
    /* Reinit GPIO for Keyboard module */
    KBD_PrepareExitLowPower();
#endif

    /* Restore 32KHz oscillator if disabled */
    BOARD_RTC_Init();

    /* Re-Init peripherals required for temperature measurement */
    ADC_Hardware_Init();
    BOARD_InitAdc();

    BOARD_DBGINITSET(2,1);
}


void BOARD_ExitLowPowerVllsPostCb(void)
{
    BOARD_DBGINITSET(2,0);
    BOARD_DBGCONFIGINIT(FALSE);

#if (gKeyBoardSupported_d && (defined(gDbgUseDtest) && (gDbgUseDtest == 1)))
    /* If dtest is enabled, keyboard reinit can only be done after BOARD_DBGCONFIGINIT()       */
    KBD_PrepareExitLowPower();
#endif

    BOARD_DBGINITSET(2,1);
    BOARD_DBGINITSET(2,0);

#if (defined(gSerialManagerMaxInterfaces_c) && (gSerialManagerMaxInterfaces_c > 0))
    assert( gSerMgrIf != BOARD_SERIAL_MGR_IF_INVALID );
    SerialInterface_Reinit(gSerMgrIf);
#endif

    BOARD_DBGINITSET(2,1);
    BOARD_DBGINITSET(2,0);

#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    /* restore Debug Console - takes roughly 350us to complete */
    BOARD_InitDebugConsole();
#endif

    BOARD_DBGINITSET(2,1);
}
#endif


int32_t BOARD_GetTemperature(void)
{
    uint16_t tempVal, bandgapValue, tempVolt, bgVolt = 100U; /*cV*/
    uint32_t vdd, adcrTemp25, adcr100m;

    bandgapValue = BOARD_GetAdcBgLvl();
    tempVal = ADC16_ReadValue(ADC16_TEMP_SENSOR_CHN, 0);
    tempVolt = bgVolt * tempVal / bandgapValue;
    (void)tempVolt;

    /* Get VDD value measured in mV */
    /* VDD = (ADCR_VDD x V_BG) / ADCR_BG  */
    vdd = ADCR_VDD * V_BG / bandgapValue;
    /* Calibrate ADCR_TEMP25  */
    /* ADCR_TEMP25 = ADCR_VDD x V_TEMP25 / VDD  */
    adcrTemp25 = ADCR_VDD * V_TEMP25 / vdd;
    /* Calculate conversion value of 100mV. */
    /* ADCR_100M = ADCR_VDD x 100 / VDD */
    adcr100m = ADCR_VDD*100U/ vdd;

    /* Multiplied by 1000 because M in uM/oC */
    /* Temperature = 25 - (ADCR_T - ADCR_TEMP25) * 100*1000 / ADCR_100M*M */
    return(((int32_t)STANDARD_TEMP - ((int32_t)tempVal - (int32_t)adcrTemp25) * 100000 /((int32_t)adcr100m*(int32_t)M)));
}

uint16_t BOARD_GetPotentiometerLevel(void)
{
    uint32_t randomNum = 0;
    RNG_GetRandomNo(&randomNum); /* Get random number */

    return (uint16_t)((uint16_t)randomNum & 0x0FFFU);
}

#if defined(gTrapDiv0) && (gTrapDiv0 > 0 )
void __aeabi_idiv0(void)
{
    /* Save the Link Register */
    volatile uint32_t savedLR = 0;
    __asm("push {r2}  ");
    __asm("push {LR} ");
    __asm("pop  {r2} ");
    __asm("str  r2, [SP, #4]");
    __asm("pop {r2}");

    __disable_irq();
    /* infinite loop just to ensure this routine never returns */
    for(;;)
    {
        __asm("NOP");
    }
}
#endif /* gTrapDiv0 */

uint8_t BOARD_GetXtal32MhzTrim(bool_t regRead)
{
    uint8_t retVal;

    if (TRUE == regRead)
    {
      /* get the XTAL trim value from XCVR reg */
      retVal = (uint8_t)((RSIM->ANA_TRIM & RSIM_ANA_TRIM_BB_XTAL_TRIM_MASK)>>RSIM_ANA_TRIM_BB_XTAL_TRIM_SHIFT);
    }
    else
    {
        /* get the XTAL trim value from HW params */
        retVal = Xtal32MhzTrim;
    }

    return retVal;
}

void BOARD_SetXtal32MHzTrim(uint8_t trimValue, bool_t saveToHwParams)
{
    uint32_t temp;

    assert((trimValue & 0x80U) == 0U); /* High bit must not be set */
    /* Apply a trim value to the crystal oscillator */
    temp = RSIM->ANA_TRIM;
    temp &= ~(RSIM_ANA_TRIM_BB_XTAL_TRIM_MASK);
    RSIM->ANA_TRIM = temp | RSIM_ANA_TRIM_BB_XTAL_TRIM(trimValue);

    if ((TRUE == saveToHwParams))
    {
        hardwareParameters_t hwParams;

        /* write new XTAL trim value into hardware params structure */
        (void)NV_ReadHWParameters(&hwParams);
        hwParams.xtalTrim = (uint32_t)trimValue;
        (void)NV_WriteHWParameters(&hwParams);

        /* update the local variable that holds the XTAL trim value */
        Xtal32MhzTrim = trimValue;
    }
}

uint8_t BOARD_GetRssiAdjustment(bool_t regRead)
{
    uint8_t retVal;

    if (TRUE == regRead)
    {
        /* get the RSSI adjustment value from XCVR reg */
        retVal = ((uint8_t)((XCVR_RX_DIG->RSSI_CTRL_0 & XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_MASK)
                              >> XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_SHIFT) & 0xFFu);
    }
    else
    {
        /* get the RSSI adjustment value from HW params */
        hardwareParameters_t hwParams;
        (void)NV_ReadHWParameters(&hwParams);
        retVal = hwParams.rssiAdjustment;
    }

    return retVal;
}

void BOARD_SetRssiAdjustment(uint8_t newRssiAdjustmentValue, bool_t saveToHwParams)
{
    /* write the new RSSI adjustment value into XCVR */
    XCVR_RX_DIG->RSSI_CTRL_0 &= ~XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ_MASK;
    XCVR_RX_DIG->RSSI_CTRL_0 |= XCVR_RX_DIG_RSSI_CTRL_0_RSSI_ADJ(newRssiAdjustmentValue);

    if (TRUE == saveToHwParams)
    {
        hardwareParameters_t hwParams;

        /* write new RSSI adjustment value into hardware params structure */
        (void)NV_ReadHWParameters(&hwParams);
        hwParams.rssiAdjustment = newRssiAdjustmentValue;
        (void)NV_WriteHWParameters(&hwParams);
    }
}



/*!
 * @brief Gets the current voltage of the battery
 *
 * This function measure the ADC channel corresponding to the battery
 */

uint16_t BOARD_GetAdcBatLvl(void)
{
    return ADC16_ReadValue(ADC16_BATLVL_CHN, 0);
}

/*!
 * @brief Gets the current bandgap voltage
 *
 * This function measure the ADC channel corresponding to the bandgap
 */

uint16_t BOARD_GetAdcBgLvl(void)
{
    return ADC16_ReadValue(ADC16_BANDGAP_CHN, 0);
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
/*!
 * @brief Parameters calibration: VDD and ADCR_TEMP25
 *
 * This function used BANDGAP as reference voltage to measure vdd and
 * calibrate V_TEMP25 with that vdd value.
 */
static void ADC16_CalibrateParams(void)
{
#if gDCDC_Enabled_d == 0
#if FSL_FEATURE_ADC16_HAS_CALIBRATION
    ADC16_DoAutoCalibration(ADC0);
#endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
    ADC16_SetHardwareAverage(ADC0 , kADC16_HardwareAverageCount16);
#endif

    const pmc_bandgap_buffer_config_t pmcBandgapConfig =
    {
#if (defined(FSL_FEATURE_PMC_HAS_BGBE) && FSL_FEATURE_PMC_HAS_BGBE)
        .enable = true,                             /*!< Enable bandgap buffer.                   */
#endif
#if (defined(FSL_FEATURE_PMC_HAS_BGEN) && FSL_FEATURE_PMC_HAS_BGEN)
        .enableInLowPowerMode = FALSE,              /*!< Enable bandgap buffer in low power mode. */
#endif      /* FSL_FEATURE_PMC_HAS_BGEN */
#if (defined(FSL_FEATURE_PMC_HAS_BGBDS) && FSL_FEATURE_PMC_HAS_BGBDS)
        .drive = kPmcBandgapBufferDriveLow,         /*!< Bandgap buffer drive select.             */
#endif      /* FSL_FEATURE_PMC_HAS_BGBDS */
    } ;

    /* Enable BANDGAP reference voltage */
    PMC_ConfigureBandgapBuffer(PMC, &pmcBandgapConfig);
}


/*!
 * @brief Reads the ADC value from the channel given as input
 *
 * This function measure the ADC channel given as input
 */
static uint16_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode)
{
    adc16_channel_config_t chnConfig;
    uint16_t adcValue;
    /* Configure the conversion channel */
    chnConfig.channelNumber     = chnIdx;
#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    chnConfig.enableDifferentialConversion = (bool)diffMode;
#endif
    chnConfig.enableInterruptOnConversionCompleted  = FALSE;
#if gBoard_ADC_uses_mutex_d
    (void)OSA_MutexLock(mADCMutexId, osaWaitForever_c);
#endif
    /* Software trigger the conversion */
    ADC16_SetChannelConfig(ADC0, ADC16_CHN_GROUP, &chnConfig);
    /* Wait for the conversion to be done */
    while (0U == ((uint8_t)kADC16_ChannelConversionDoneFlag & (uint8_t)ADC16_GetChannelStatusFlags(ADC0, ADC16_CHN_GROUP)))
    {
    }

    /* Fetch the conversion value */
    adcValue =  (uint16_t)ADC16_GetChannelConversionValue(ADC0, ADC16_CHN_GROUP);
#if gBoard_ADC_uses_mutex_d
    (void)OSA_MutexUnlock(mADCMutexId);
#endif
    /* Calculates adcValue in 16bit resolution from 12bit resolution
    in order to convert to reading */
#if (FSL_FEATURE_ADC16_MAX_RESOLUTION < 16)
    adcValue = adcValue << 4;
#endif
    /* Pause the conversion */

    return adcValue;
}

static void ADC_Hardware_Init(void)
{
    CLOCK_EnableClock(kCLOCK_Dcdc0);
    /* Enable Adc clock*/
    CLOCK_EnableClock(kCLOCK_Adc0);
    /* Initializes the ADC16 module */
    ADC16_Init(ADC0, &adcConfig);
    ADC16_EnableHardwareTrigger(ADC0, FALSE);
    ADC16_CalibrateParams();
    ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxA);

#if ADC16_USES_HARDWARE_AVERAGE
    ADC16_SetHardwareAverage(ADC0 , kADC16_HardwareAverageCount16);
#else
    ADC16_SetHardwareAverage(ADC0 , kADC16_HardwareAverageDisabled);
#endif

    ADC16_SetHardwareCompareConfig(ADC0, (const adc16_hardware_compare_config_t*)NULL);
}

static void DCDC_AdjustVbatDiv4(void)
{
  /* DCDC Adjust VbatDiv */
  const uint8_t vBatDiv = 3;
  DCDC->REG0 = (DCDC->REG0 & ~DCDC_REG0_DCDC_VBAT_DIV_CTRL_MASK )| DCDC_REG0_DCDC_VBAT_DIV_CTRL(vBatDiv);
}

static void BOARD_CheckRTCStatus(void)
{
    /* enable register access clock */
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;
    if ((RTC->CR & RTC_CR_OSCE_MASK) == 0U)
    {
        /* RTC is not running */
        mRTC_WasShutDown = TRUE;
    }
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
