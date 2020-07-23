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
#include "clock_config.h"
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
#include "fsl_debug_console.h"
#include "FunctionLib.h"
#include "RNG_Interface.h"
#if cPWR_UsePowerDownMode
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

#if gDCDC_Enabled_d
#include "DCDC.h"
#endif

#include "Keyboard.h"
#include "SerialManager.h"
#include "Flash_Adapter.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Checks on compile switches  */
#if defined(gApp_EnableDCDCShutdown_d) && (gApp_EnableDCDCShutdown_d == 1)
#if (!defined(gDCDC_Enabled_d) || (gDCDC_Enabled_d == 0)) || (!defined(gDCDC_PowerSwitchPinTiedUp_d) || (gDCDC_PowerSwitchPinTiedUp_d == 1))
#warning gApp_EnableDCDCShutdown_d requires gDCDC_Enabled_d to be set to 1 and gDCDC_PowerSwitchPinTiedUp_d to be set to 0
#endif
#endif

/************************************************************************************
* Private type definitions and macros
************************************************************************************/

/*! @brief Default trimming value for 32MHz crystal on KW38 */
#define BOARD_32MHZ_XTAL_TRIM_DEFAULT         0x4B

/************************************************************************************
* Private memory declarations
************************************************************************************/
#if gDCDC_Enabled_d == 1
static const dcdcConfig_t mDcdcDefaultConfig =
{
#if APP_DCDC_MODE == gDCDC_Buck_c
  .vbatMin = 1800,
  .vbatMax = 4200,
#elif APP_DCDC_MODE == gDCDC_Boost_c
  .vbatMin = 900,
  .vbatMax = 1800,
#endif
  .dcdcMode = (dcdcMode_t)APP_DCDC_MODE,
  .vBatMonitorIntervalMs = APP_DCDC_VBAT_MONITOR_INTERVAL,
  .pfDCDCAppCallback = NULL, /* .pfDCDCAppCallback = DCDCCallback, */
  .dcdcMcuVOutputTargetVal = gDCDC_McuV_OutputTargetVal_1_500_c,
  .dcdc1P8OutputTargetVal = gDCDC_1P8OutputTargetVal_1_800_c
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

typedef struct gpioPinId_tag
{
    gpioPort_t   gpioPort;
    uint8_t      gpioPin;
}gpioPinId_t;


/************************************************************************************
*************************************************************************************
* Private type definitions and macros
*************************************************************************************
************************************************************************************/
#define ADCDAC_LDO_OVERRIDE           (1)
#define ADC16_INSTANCE                (0)   /* ADC instance */
#define ADC16_CHN_GROUP               (0)   /* ADC group configuration selection */

#define ADC16_BATLVL_CHN              (23) /* Potentiometer channel */
#define ADC16_BL_LOWER_LIMIT          (0) /* min percentage of battery charge */
#define ADC16_BL_UPPER_LIMIT          (100) /* max percentage of battery charge */
#define ADC16_BL_DYNAMIC_RANGE        (ADC16_BL_UPPER_LIMIT - ADC16_BL_LOWER_LIMIT) /* Range = [ADC16_HB_LOWER_LIMIT .. ADC16_HB_LOWER_LIMIT + ADC16_HB_DYNAMIC_RANGE] */

#define ADC16_TEMP_SENSOR_CHN         (26) /* ADC channel of the Temperature Sensor */
#define ADC16_BANDGAP_CHN             (27) /* ADC channel of BANDGAP Voltage reference */

#define MIN_VOLT_BUCK                 (180)
#define MAX_VOLT_BUCK                 (310)
#define FULL_BAT                      (100)
#define EMPTY_BAT                     (0)

#define ADCR_VDD                      (4095U)     /* Maximum value when use 12b resolution */
#define V_BG                          (1000U)     /* BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25                      (716U)      /* Typical converted value at 25 oC in mV */
#define M                             (1740U)     /* Typical slope:uV/oC */
#define STANDARD_TEMP                 (25)
#define mNoOfPinsDisabledInLowPower_c (16)

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
uint32_t offsetVdd = 0;
static uint32_t adcValue = 0; /* ADC value */
static adc16_config_t adcUserConfig; /* structure for user config */

const gpioPinId_t maPinsDisabledInLowPower[mNoOfPinsDisabledInLowPower_c]={
{gpioPort_A_c, 16},
{gpioPort_A_c, 17},
{gpioPort_B_c, 1},
{gpioPort_B_c, 2},
{gpioPort_B_c, 3},
{gpioPort_C_c, 0},
{gpioPort_C_c, 1},
{gpioPort_C_c, 3},
{gpioPort_C_c, 6},
{gpioPort_C_c, 7},
{gpioPort_C_c, 16},
{gpioPort_C_c, 17},
{gpioPort_C_c, 18},
{gpioPort_C_c, 19},
{gpioPort_A_c, 0},
{gpioPort_A_c, 1}
};
uint32_t maPCRSave[mNoOfPinsDisabledInLowPower_c];

/* 32MHz Xtal trim */
static uint8_t Xtal32MhzTrim = BOARD_32MHZ_XTAL_TRIM_DEFAULT;

#if cPWR_UsePowerDownMode
#if cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5
static uint32_t scb_vtor;
static uint32_t sim_scgc5;
static uint32_t sim_sopt2;
static uint32_t nvic_iser;
static uint32_t nvic_ip[8];
#endif
#endif

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/
static inline uint32_t ADC16_BatLvl(void);
static inline uint32_t ADC16_BgLvl(void);
static uint16_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode);
static void DCDC_AdjustVbatDiv4();
static void ADC16_CalibrateParams(void);

void BOARD_InstallLowPowerCallbacks(void);
void BOARD_EnterLowPowerCb(void);
void BOARD_ExitLowPowerCb(void);
void BOARD_InstallWarmbootCallback(void);
void BOARD_VLLS_EnterCb(void);
void BOARD_WarmbootCb(void);
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);
void BOARD_GetMACAddr(uint8_t* aOutMacAddr5B);
void BOARD_ReinitButtons(void);
void BOARD_SetPinsLowPower(bool_t isLowPower);

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#if defined(gDCDC_Enabled_d) && (gDCDC_Enabled_d == 1)
#if defined(gApp_EnableDCDCShutdown_d) && (gApp_EnableDCDCShutdown_d == 1)
static void BOARD_DCDCpswitchCallback(dcdcPSwStatus_t dcdcPSwStatus)
{
    /* TODO : use an other IO to shutdown the DCDC                              */
    int j = 0;
    while(j<5000000)
    {
        j++;
    }
    DCDC_ShutDown();
}
#endif
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void hardware_init(void)
{
    static uint8_t initialized = 0;

    BOARD_DBGINITSET(0,1);
    BOARD_DBGINITSET(0,0);

    if( !initialized )
    {
        BOARD_InitHardware();

        NV_ReadHWParameters(&gHardwareParameters);

        if(0xFFFFFFFF != gHardwareParameters.xtalTrim)
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
    uint32_t clock;

    clock = BOARD_DEBUG_UART_CLK_FREQ;
    return clock;
}

/* Initialize debug console. */
void BOARD_InitDebugConsole(void)
{
#if defined(gDebugConsoleEnable_d) && (gDebugConsoleEnable_d == 1)
    uint32_t uartClkSrcFreq;

    uartClkSrcFreq = BOARD_DEBUG_UART_CLK_FREQ;

    DbgConsole_Init(BOARD_DEBUG_UART_INSTANCE, BOARD_DEBUG_UART_BAUDRATE, BOARD_DEBUG_UART_TYPE, uartClkSrcFreq);
#endif
}

uint32_t BOARD_GetTpmClock(uint32_t instance)
{
    uint32_t clock;
    uint32_t clockSource = (SIM->SOPT2 & SIM_SOPT2_TPMSRC_MASK) >> SIM_SOPT2_TPMSRC_SHIFT;

    instance = instance; /* Remove compiler warnings */

    switch(clockSource)
    {
    case 1: /* MCGFLLCLK */
        clock = CLOCK_GetFllFreq();
        break;
    case 2: /* OSCERCLK */
        clock = CLOCK_GetOsc0ErClkFreq();
        break;
    case 3: /* MCGIRCLK */
        clock = CLOCK_GetInternalRefClkFreq();
        break;
    default: /* Clock disabled */

        clock  = 0;
        break;
    }
    return clock;
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


void BOARD_SetNMIPinMux(port_mux_t mux)
{
    CLOCK_EnableClock(kCLOCK_PortB);

    PORT_SetPinMux(PORTB, 18, mux);
}

void NMI_Handler(void)
{
    BOARD_SetNMIPinMux(kPORT_MuxAsGpio);
}

void BOARD_InitAdc(void)
{
    CLOCK_EnableClock(kCLOCK_Adc0);
    CLOCK_EnableClock(kCLOCK_Dcdc0);
    ADC16_GetDefaultConfig(&adcUserConfig);
    adcUserConfig.resolution = kADC16_ResolutionSE12Bit;
    adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
    ADC16_Init(ADC0, &adcUserConfig);
    ADC16_CalibrateParams();
}

uint8_t BOARD_GetBatteryLevel(void)
{
    uint16_t batVal, bandgapValue, batLvl, batVolt, bgVolt = 100; /*cV*/

    bandgapValue = ADC16_BgLvl();
    DCDC_AdjustVbatDiv4(); /* Bat voltage  divided by 4 */
    batVal = ADC16_BatLvl() * 4; /* Need to multiply the value by 4 because the measured voltage is divided by 4*/

    batVolt = bgVolt * batVal / bandgapValue;

    batLvl = (batVolt - MIN_VOLT_BUCK) * (FULL_BAT - EMPTY_BAT) / (MAX_VOLT_BUCK - MIN_VOLT_BUCK);
    return ((batLvl <= 100) ? batLvl:100);
}


/* Initialize DCDC. */
void BOARD_DCDCInit(void)
{
#if defined(gDCDC_Enabled_d) && (gDCDC_Enabled_d == 1)
    /* Initialize DCDC module */
    DCDC_Init(&mDcdcDefaultConfig);
#if defined(gApp_EnableDCDCShutdown_d) && (gApp_EnableDCDCShutdown_d == 1)
    DCDC_InstallPSwitchCallback(BOARD_DCDCpswitchCallback, gDCDC_PSwIntEdge_Rising_c);
#endif
#endif
}


/* get 4 words of information that uniquely identifies the MCU */
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen)
{
    uint32_t uid[4] = {0};

    uid[0] = SIM->SDID;
    uid[1] = SIM->UIDMH;
    uid[2] = SIM->UIDML;
    uid[3] = SIM->UIDL;

    FLib_MemCpy(aOutUid16B, (uint8_t*)uid, sizeof(uid));
    *pOutLen = sizeof(uid);

    return;
}

/* get the device MAC ADDRESS of 40 bits */
void BOARD_GetMACAddr(uint8_t* aOutMacAddr5B)
{
    aOutMacAddr5B[4] = (RSIM->MAC_MSB    )&0xFF;
    aOutMacAddr5B[3] = (RSIM->MAC_LSB>>24)&0xFF;
    aOutMacAddr5B[2] = (RSIM->MAC_LSB>>16)&0xFF;
    aOutMacAddr5B[1] = (RSIM->MAC_LSB>>8 )&0xFF;
    aOutMacAddr5B[0] = (RSIM->MAC_LSB    )&0xFF;

    return;
}

void BOARD_InstallLowPowerCallbacks()
{
#if cPWR_UsePowerDownMode
  PWR_RegisterLowPowerEnterCallback((pfPWRCallBack_t)BOARD_EnterLowPowerCb);
  PWR_RegisterLowPowerExitCallback((pfPWRCallBack_t)BOARD_ExitLowPowerCb);
#endif
}

void BOARD_InstallWarmbootCallback()
{
#if cPWR_UsePowerDownMode
#if cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5
  PWR_RegisterVLLSEnterCallback((pfPWRCallBack_t)BOARD_VLLS_EnterCb);
  PWR_RegisterWarmbootCallback((pfPWRCallBack_t)BOARD_WarmbootCb);
#endif
#endif
}

void BOARD_SetPinsLowPower(bool_t isLowPower)
{
  uint32_t i,j;

  if(isLowPower)
  {

    for(i=0; i<mNoOfPinsDisabledInLowPower_c; i++)
    {
      maPCRSave[i] = GpioGetPinPCR(maPinsDisabledInLowPower[i].gpioPort, maPinsDisabledInLowPower[i].gpioPin);
      GpioSetPinMux_ISF_Preserved(maPinsDisabledInLowPower[i].gpioPort, maPinsDisabledInLowPower[i].gpioPin, pinMux_PinDisabledOrAnalog_c);
    }
  }
  else
  {
    for(i=0,j=(mNoOfPinsDisabledInLowPower_c-1) ; i<mNoOfPinsDisabledInLowPower_c; i++,j--)
    {
      GpioSetPinPCR_ISF_Preserved(maPinsDisabledInLowPower[j].gpioPort, maPinsDisabledInLowPower[j].gpioPin, maPCRSave[j]);
    }
  }
}

void BOARD_EnterLowPowerCb()
{
#if gKeyBoardSupported_d
    KBD_PrepareEnterLowPower();
#endif

    BOARD_SetPinsLowPower(TRUE);

#if gDCDC_Enabled_d
    //DCDC_BWR_REG0_DCDC_VBAT_DIV_CTRL(DCDC_BASE_PTR, 0);
    DCDC->REG0 &=  ~DCDC_REG0_DCDC_VBAT_DIV_CTRL_MASK ;
    DCDC_PrepareForPulsedMode();
#endif
}

void BOARD_ExitLowPowerCb()
{
#if gDCDC_Enabled_d
    DCDC_PrepareForContinuousMode();
#endif

#if gKeyBoardSupported_d
    KBD_PrepareExitLowPower();
#endif

    BOARD_SetPinsLowPower(FALSE);
 }

#if cPWR_UsePowerDownMode
#if cPWR_EnableDeepSleepMode_8 || cPWR_EnableDeepSleepMode_5
void BOARD_VLLS_EnterCb(void)
{
    uint32_t i;
    scb_vtor  = SCB->VTOR;
    sim_scgc5 = SIM->SCGC5;
    sim_sopt2 = SIM->SOPT2;
    nvic_iser = NVIC->ISER[0U];

    for(i=0;i<8;i++)
    {
        nvic_ip[i]=NVIC->IP[i];
    }
}

void BOARD_WarmbootCb()
{
    uint32_t i;

    /* Restore the VTOR */
    SCB->VTOR = scb_vtor;
    /* Restore peripheral clocks */
    SIM->SCGC5 = sim_scgc5;
    /* Restore peripheral clock source option */
    SIM->SOPT2 = sim_sopt2;

    for(i=0;i<8;i++)
    {
        NVIC->IP[i]=nvic_ip[i];
    }
    NVIC->ICPR[0U] = nvic_iser;
    NVIC->ISER[0U] = nvic_iser;

    if((PMC->REGSC & PMC_REGSC_ACKISO_MASK) != 0x00U)
    {
        PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* Release hold with ACKISO:  Only has an effect if recovering from VLLSx.*/
        /*clear power management registers after recovery from vlls*/
        SMC->STOPCTRL &= ~SMC_STOPCTRL_LLSM_MASK;
        SMC->PMCTRL   &= ~(SMC_PMCTRL_STOPM_MASK | SMC_PMCTRL_RUNM_MASK);
    }

    #if (gTMR_Enabled_d)
    /* Initialize the timer */
    StackTimer_Init(NULL);
    #endif

    #if gDCDC_Enabled_d
    /* Restore DCDC */
    DCDC_ExitLowPower();
    #endif

    /* Init RF */
    {
        uint8_t restore_status = 0x0U;
        pfd_rf_gen_interface(0x01, &restore_status, NULL);
    }

    /* Re-establish System clocks config */
    BOARD_BootClockRUN();

    BOARD_ReinitButtons();
    SerialInterface_Reinit(0);
    Serial_SetBaudRate(0, APP_SERIAL_INTERFACE_SPEED);

}
#endif
#endif

void BOARD_BLPEtoBLPI(void)
{
}

void BOARD_BLPItoBLPE(void)
{
    while((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) == 0 )
    {}
}

int32_t BOARD_GetTemperature(void)
{
    uint16_t tempVal, bandgapValue, tempVolt, bgVolt = 100; /*cV*/
    uint32_t vdd, adcrTemp25, adcr100m;

    bandgapValue = ADC16_BgLvl();
    tempVal = ADC16_ReadValue(ADC16_TEMP_SENSOR_CHN, false);
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
    adcr100m = ADCR_VDD*100/ vdd;

    /* Multiplied by 1000 because M in uM/oC */
    /* Temperature = 25 - (ADCR_T - ADCR_TEMP25) * 100*1000 / ADCR_100M*M */
    return(int32_t)(STANDARD_TEMP - ((int32_t)adcValue - (int32_t)adcrTemp25) * 100000 /(int32_t)(adcr100m*M));
}

uint16_t BOARD_GetPotentiometerLevel(void)
{
    uint32_t random = 0;
    RNG_GetRandomNo(&random);

    return (random & 0x0FFF);
}

void BOARD_ReinitButtons(void)
{
#if gKeyBoardSupported_d && (gKBD_KeysCount_c > 0)
    uint8_t idx = 0;
    BOARD_InitButtons();
    for(idx = 0; idx < gKBD_KeysCount_c; idx++)
    {
        if(kbdSwButtons[idx].swType == gKBDTypeGpio_c)
        {
            (void)GpioInputPinInit(kbdSwButtons[idx].config_struct.pSwGpio, 1);
        }
    }
#endif
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
        .enableInLowPowerMode = false,              /*!< Enable bandgap buffer in low power mode. */
#endif      /* FSL_FEATURE_PMC_HAS_BGEN */
#if (defined(FSL_FEATURE_PMC_HAS_BGBDS) && FSL_FEATURE_PMC_HAS_BGBDS)
        .drive = kPmcBandgapBufferDriveLow,         /*!< Bandgap buffer drive select.             */
#endif      /* FSL_FEATURE_PMC_HAS_BGBDS */
    } ;

    // Enable BANDGAP reference voltage
    PMC_ConfigureBandgapBuffer(PMC, &pmcBandgapConfig);
}

/*!
 * @brief Gets the current voltage of the battery
 *
 * This function measure the ADC channel corresponding to the battery
 */

static inline uint32_t ADC16_BatLvl(void)
{
      adcValue = ADC16_ReadValue(ADC16_BATLVL_CHN, false);
      return adcValue;
}

/*!
 * @brief Gets the current bandgap voltage
 *
 * This function measure the ADC channel corresponding to the bandgap
 */

static inline uint32_t ADC16_BgLvl(void)
{
    adcValue = ADC16_ReadValue(ADC16_BANDGAP_CHN, false);
    return adcValue;
}

/*!
 * @brief Reads the ADC value from the channel given as input
 *
 * This function measure the ADC channel given as input
 */
static uint16_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode)
{
  adc16_channel_config_t chnConfig;

    /* Configure the conversion channel */
    chnConfig.channelNumber     = chnIdx;
#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    chnConfig.enableDifferentialConversion = diffMode;
#endif
    chnConfig.enableInterruptOnConversionCompleted  = false;

    /* Software trigger the conversion */
    ADC16_SetChannelConfig(ADC0, ADC16_CHN_GROUP, &chnConfig);
    /* Wait for the conversion to be done */
    while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, ADC16_CHN_GROUP)));

    /* Fetch the conversion value */
    adcValue =  ADC16_GetChannelConversionValue(ADC0, ADC16_CHN_GROUP);
    /* Calculates adcValue in 16bit resolution from 12bit resolution
    in order to convert to reading */
#if (FSL_FEATURE_ADC16_MAX_RESOLUTION < 16)
    adcValue = adcValue << 4;
#endif
    /* Pause the conversion */

    return adcValue;
}

static void DCDC_AdjustVbatDiv4()
{
  const uint8_t vBatDiv = 3;
  DCDC->REG0 = (DCDC->REG0 & ~DCDC_REG0_DCDC_VBAT_DIV_CTRL_MASK )| DCDC_REG0_DCDC_VBAT_DIV_CTRL(vBatDiv);
}

void BOARD_DeinitLPUART(void)
{
}

void BOARD_DeinitSPI(void)
{
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
