/*! *********************************************************************************
* Copyright 2013-2015, Freescale Semiconductor, Inc.
* Copyright 2016-2018 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */

/************************************************************************************
*************************************************************************************
* Include
*************************************************************************************
************************************************************************************/
#include "EmbeddedTypes.h"
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_clock.h"
#include "fsl_smc.h"
#include "fsl_debug_console.h"
#include "fsl_pmc.h"
#include "fsl_port.h"
#include "fsl_adc16.h"
#include "board.h"
#include "pin_mux.h"
#include "GPIO_Adapter.h"
#include "TMR_Adapter.h"
#include "RNG_Interface.h"
#include "Flash_Adapter.h"
#include "clock_config.h"

#if cPWR_UsePowerDownMode
#include "PWR_Interface.h"
#include "PWR_Configuration.h"
#endif

#if gDCDC_Enabled_d
#include "DCDC.h"
#endif

#include "FunctionLib.h"
#include "Keyboard.h"
#include "fsl_os_abstraction.h"
#include "Panic.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
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

#define MIN_VOLT_BUCK                 (180)
#define MAX_VOLT_BUCK                 (310)
#define FULL_BAT                      (100)
#define EMPTY_BAT                     (0)

#define ADCR_VDD                      (4095U)     /* Maximum value when use 12b resolution */
#define V_BG                          (1000U)     /* BANDGAP voltage in mV (trim to 1.0V) */
#define V_TEMP25                      (716U)      /* Typical converted value at 25 oC in mV */
#define M                             (1620U)     /* Typical slope:uV/oC */
#define STANDARD_TEMP                 (25)
#define mNoOfPinsDisabledInLowPower_c (16)

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
uint32_t offsetVdd = 0;
#if gBoard_ADC_uses_mutex_d              
static osaMutexId_t mADCMutexId;
#endif
static const adc16_config_t adcConfig = {
  .referenceVoltageSource = gADC16_ReferenceVoltageSource_d,
  .clockSource = kADC16_ClockSourceAlt0, // bus clock
  .enableAsynchronousClock = false,
  .clockDivider = kADC16_ClockDivider8,
  .resolution = kADC16_ResolutionSE12Bit,
  .longSampleMode = kADC16_LongSampleDisabled,
  .enableHighSpeed = false,
  .enableLowPower = false,
  .enableContinuousConversion = false
};

#if gDCDC_Enabled_d == 1
const dcdcConfig_t mDcdcDefaultConfig =
{
#if APP_DCDC_MODE == gDCDC_Mode_Buck_c
  .vbatMin = 1800,
  .vbatMax = 4200,
#elif APP_DCDC_MODE == gDCDC_Mode_Boost_c
  .vbatMin = 900,
  .vbatMax = 1800,
#endif
  .dcdcMode = APP_DCDC_MODE,
  .vBatMonitorIntervalMs = APP_DCDC_VBAT_MONITOR_INTERVAL,
  .pfDCDCAppCallback = NULL, /* .pfDCDCAppCallback = DCDCCallback, */
  .dcdcMcuVOutputTargetVal = gDCDC_McuV_OutputTargetVal_1_500_c,
  .dcdc1P8OutputTargetVal = gDCDC_1P8OutputTargetVal_1_800_c
};
#endif
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

/* FRDM-KW36 Rev X2 xtal trim */
static const uint8_t mXtalTrimDefault = 0x36;
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
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void ADC16_CalibrateParams(void);
static void ADC_Hardware_Init(void);
static uint32_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode);
static void DCDC_AdjustVbatDiv4();
void BOARD_SetPinsLowPower(bool_t isLowPower);

/************************************************************************************
*************************************************************************************
* Public functions prototypes
*************************************************************************************
************************************************************************************/
void BOARD_InstallLowPowerCallbacks(void);
void BOARD_EnterLowPowerCb(void);
void BOARD_ExitLowPowerCb(void);
void BOARD_InstallWarmbootCallback(void);
void BOARD_VLLS_EnterCb(void);
void BOARD_WarmbootCb(void);
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);
void BOARD_GetMACAddr(uint8_t* aOutMacAddr5B);
void BOARD_ReinitButtons(void);
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void hardware_init(void)
{
    static uint8_t initialized = 0;

    if( !initialized )
    {
        if(SMC->PMPROT == 0x00)
        {
          SMC->PMPROT = SYSTEM_SMC_PMPROT_VALUE;
        }

        if((PMC->REGSC & PMC_REGSC_ACKISO_MASK) != 0x00U)
        {
            PMC->REGSC |= PMC_REGSC_ACKISO_MASK; /* Release hold with ACKISO:  Only has an effect if recovering from VLLSx.*/
            /*clear power management registers after recovery from vlls*/
            SMC->STOPCTRL &= ~SMC_STOPCTRL_LLSM_MASK;
            SMC->PMCTRL   &= ~(SMC_PMCTRL_STOPM_MASK | SMC_PMCTRL_RUNM_MASK);
        }

        /* enable clock for PORTs */
        CLOCK_EnableClock(kCLOCK_PortA);
        CLOCK_EnableClock(kCLOCK_PortB);
        CLOCK_EnableClock(kCLOCK_PortC);

        SIM->SCGC6 |= (SIM_SCGC6_DMAMUX_MASK); /* Enable clock to DMA_MUX (SIM module) */
        SIM->SCGC7 |= (SIM_SCGC7_DMA_MASK);
        /* Enable PHYDIG clock for revId > 0 */
        SIM->SCGC5 |= SIM_SCGC5_PHYDIG_MASK;

        /* Init board clock */
        BOARD_BootClockRUN();

        /* Init DCDC module */
        BOARD_DCDCInit();

        #if gMWS_UseCoexistence_d
        /* reset TSM timing registers used by CoEx */
        XCVR_TSM->TIMING43 = 0xFFFFFFFF;
        XCVR_TSM->TIMING49 = 0xFFFFFFFF;
        XCVR_TSM->TIMING50 = 0xFFFFFFFF;
        /* clear TSM_IRQ0 flag, if set */
        if(XCVR_MISC->XCVR_STATUS & XCVR_CTRL_XCVR_STATUS_TSM_IRQ0_MASK)
        {
          XCVR_MISC->XCVR_STATUS |= XCVR_CTRL_XCVR_STATUS_TSM_IRQ0_MASK;
        }
        #endif

        /* Install callbacks to handle enter and exit low power */
        #if cPWR_UsePowerDownMode
        BOARD_InstallLowPowerCallbacks();
        BOARD_InstallWarmbootCallback();
        #endif

        NV_ReadHWParameters(&gHardwareParameters);
        if(0xFFFFFFFF == gHardwareParameters.xtalTrim)
        {
            gHardwareParameters.xtalTrim = mXtalTrimDefault;
        }
        initialized = 1;
    }
}

void NMI_Handler(void)
{
    /* Disable NMI pin */
    CLOCK_EnableClock(kCLOCK_PortB);
    GpioSetPinMux(gpioPort_B_c, 18u, pinMux_Alt2_c);
}

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
        ADC_Hardware_Init();    
        
    }
}
uint8_t BOARD_GetBatteryLevel(void)
{
    uint32_t batVal, bandgapValue, batLvl, batVolt, bgVolt = 100; /*cV*/

    bandgapValue = BOARD_GetAdcBgLvl();
    DCDC_AdjustVbatDiv4(); /* Bat voltage  divided by 4 */
    batVal = BOARD_GetAdcBatLvl() * 4; /* Need to multiply the value by 4 because the measured voltage is divided by 4*/

    batVolt = bgVolt * batVal / bandgapValue;

    batLvl = (batVolt - MIN_VOLT_BUCK) * (FULL_BAT - EMPTY_BAT) / (MAX_VOLT_BUCK - MIN_VOLT_BUCK);
    return ((batLvl <= 100) ? batLvl:100);
}

uint16_t BOARD_GetPotentiometerLevel(void)
{
    uint32_t random = 0;
    RNG_GetRandomNo(&random);

    return (random & 0x0FFF);
}

int32_t BOARD_GetTemperature(void)
{
    uint32_t tempVal, bandgapValue, tempVolt, bgVolt = 100; /*cV*/
    uint32_t vdd, adcrTemp25, adcr100m;

    bandgapValue = BOARD_GetAdcBgLvl();
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
    return (int32_t)(STANDARD_TEMP - ((int32_t)tempVal - (int32_t)adcrTemp25) * 100000 /(int32_t)(adcr100m*M));
}

/* Initialize DCDC. */
void BOARD_DCDCInit(void)
{
#if gDCDC_Enabled_d == 1
    /* Initialize DCDC module */
    DCDC_Init(&mDcdcDefaultConfig);
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

 #if FSL_FEATURE_ADC16_HAS_CALIBRATION
      ADC16_DoAutoCalibration(ADC0);
 #endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
    pmc_bandgap_buffer_config_t pmcBandgapConfig =
{
#if (defined(FSL_FEATURE_PMC_HAS_BGBE) && FSL_FEATURE_PMC_HAS_BGBE)
       .enable = true, /*!< Enable bandgap buffer.                   */
#endif
#if (defined(FSL_FEATURE_PMC_HAS_BGEN) && FSL_FEATURE_PMC_HAS_BGEN)
    .enableInLowPowerMode = false, /*!< Enable bandgap buffer in low power mode. */
#endif                         /* FSL_FEATURE_PMC_HAS_BGEN */
#if (defined(FSL_FEATURE_PMC_HAS_BGBDS) && FSL_FEATURE_PMC_HAS_BGBDS)
    .drive = kPmcBandgapBufferDriveLow, /*!< Bandgap buffer drive select.             */
#endif                                       /* FSL_FEATURE_PMC_HAS_BGBDS */
} ;

    // Enable BANDGAP reference voltage
    PMC_ConfigureBandgapBuffer(PMC, &pmcBandgapConfig);
}
/*!
 * @brief Gets the current voltage of the battery
 *
 * This function measure the ADC channel corresponding to the battery
 */

uint32_t BOARD_GetAdcBatLvl(void)
{
      return ADC16_ReadValue(ADC16_BATLVL_CHN , false);
}

/*!
 * @brief Gets the current bandgap voltage
 *
 * This function measure the ADC channel corresponding to the bandgap
 */
uint32_t BOARD_GetAdcBgLvl(void)
{
    return ADC16_ReadValue(ADC16_BANDGAP_CHN, false);
}

/*!
 * @brief Reads the ADC value from the channel given as input
 *
 * This function measure the ADC channel given as input
 */
static uint32_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode)
{
    adc16_channel_config_t chnConfig;
    uint32_t adcValue; 
    
    /* Configure the conversion channel */
    chnConfig.channelNumber     = chnIdx;
#if FSL_FEATURE_ADC16_HAS_DIFF_MODE
    chnConfig.enableDifferentialConversion = diffMode;
#endif
    chnConfig.enableInterruptOnConversionCompleted  = false;
#if gBoard_ADC_uses_mutex_d    
    (void)OSA_MutexLock(mADCMutexId, osaWaitForever_c);
#endif
    /* Software trigger the conversion */
    ADC16_SetChannelConfig(ADC0, ADC16_CHN_GROUP, &chnConfig);
    /* Wait for the conversion to be done */
    while (0U == (kADC16_ChannelConversionDoneFlag & ADC16_GetChannelStatusFlags(ADC0, ADC16_CHN_GROUP)));
    
    /* Fetch the conversion value */
    adcValue =  ADC16_GetChannelConversionValue(ADC0, ADC16_CHN_GROUP);
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

static void ADC_Hardware_Init()
{
    CLOCK_EnableClock(kCLOCK_Dcdc0);
    CLOCK_EnableClock(kCLOCK_Adc0);
    ADC16_Init(ADC0, &adcConfig);  
    ADC16_EnableHardwareTrigger(ADC0, false); 
    ADC16_CalibrateParams();
    ADC16_SetChannelMuxMode(ADC0, kADC16_ChannelMuxA);
#if ADC16_USES_HARDWARE_AVERAGE
    ADC16_SetHardwareAverage(ADC0 , kADC16_HardwareAverageCount16);        
#else
    ADC16_SetHardwareAverage(ADC0 , kADC16_HardwareAverageDisabled);
#endif        
    ADC16_SetHardwareCompareConfig(ADC0, (const adc16_hardware_compare_config_t*)NULL);
}
static void DCDC_AdjustVbatDiv4()
{
    const uint8_t vBatDiv = 3;
    DCDC->REG0 = (DCDC->REG0 & ~DCDC_REG0_DCDC_VBAT_DIV_CTRL_MASK )| DCDC_REG0_DCDC_VBAT_DIV_CTRL(vBatDiv);
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
    scb_vtor = SCB->VTOR;
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
    /* Restore DCDC */
    ADC_Hardware_Init();
    BOARD_ReinitButtons();
}
#endif
#endif
uint32_t BOARD_GetLpuartClock(uint32_t instance)
{
    uint32_t clock;
    uint32_t clockSource = (SIM->SOPT2 & SIM_SOPT2_LPUART0SRC_MASK) >> SIM_SOPT2_LPUART0SRC_SHIFT;

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
        clock = 0;
        break;
    }

    return clock;
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
        clock = 0;
        break;
    }

    return clock;
}

uint32_t BOARD_GetSpiClock(uint32_t instance)
{
    instance = instance; /* Remove compiler warnings */
    return CLOCK_GetFreq(kCLOCK_BusClk);
}

uint32_t BOARD_GetI2cClock(uint32_t instance)
{
    uint32_t clock;

    switch(instance)
    {
    case 1:
        clock = CLOCK_GetFreq(kCLOCK_CoreSysClk);
        break;

    default:
        clock = CLOCK_GetFreq(kCLOCK_BusClk);
        break;
    }

    return clock;
}

void BOARD_BLPEtoBLPI(void)
{
}

void BOARD_BLPItoBLPE(void)
{
    while((RSIM->CONTROL & RSIM_CONTROL_RF_OSC_READY_MASK) == 0 )
    {}
}

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_ReinitButtons
 * Description   : Reinitializes buttons after VLLS.
 *
 *END**************************************************************************/
void BOARD_ReinitButtons(void)
{
    uint8_t idx = 0;
    BOARD_InitButtons();
    for(idx = 0; idx < gKBD_KeysCount_c; idx++)
    {
        if(kbdSwButtons[idx].swType == gKBDTypeGpio_c)
        {
            (void)GpioInputPinInit(kbdSwButtons[idx].config_struct.pSwGpio, 1);
        }
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/

