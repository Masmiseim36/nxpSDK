/*! *********************************************************************************
* Copyright (c) 2013-2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017 NXP
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
#include "clock_config.h"
#include "GPIO_Adapter.h"
#include "RNG_Interface.h"
#include "Flash_Adapter.h"

#if cPWR_UsePowerDownMode
#include "PWR_Interface.h"
#endif

#if gDCDC_Enabled_d
#include "DCDC.h"
#endif

#include "FunctionLib.h"
#include "Keyboard.h"

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
#define M                             (1620U)     /* Typical slope:uV/oC */
#define STANDARD_TEMP                 (25)
#define mNoOfPinsDisabledInLowPower_c (19)

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
uint32_t offsetVdd = 0;
static uint32_t adcValue = 0; /* ADC value */
static adc16_config_t adcUserConfig; /* structure for user config */

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
{gpioPort_B_c, 16},
{gpioPort_B_c, 17},
{gpioPort_C_c, 0},
{gpioPort_C_c, 1},
{gpioPort_C_c, 2},
{gpioPort_C_c, 3},
{gpioPort_C_c, 6},
{gpioPort_C_c, 7},
{gpioPort_C_c, 16},
{gpioPort_C_c, 17},
{gpioPort_C_c, 18},
{gpioPort_C_c, 19},
{gpioPort_A_c, 0},
{gpioPort_A_c, 1},
};
uint32_t maPCRSave[mNoOfPinsDisabledInLowPower_c];

/* FRDM-KW41Z Rev A2 xtal trim */
static const uint8_t mXtalTrimDefault = 0x30;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static void ADC16_CalibrateParams(void);
static inline uint32_t ADC16_BatLvl(void);
static inline uint32_t ADC16_BgLvl(void);
static uint16_t ADC16_ReadValue(uint32_t chnIdx, uint8_t diffMode);
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
void BOARD_GetMCUUid(uint8_t* aOutUid16B, uint8_t* pOutLen);

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

        /* Init board clock */
        BOARD_BootClockRUN();
        
        /* Init DCDC module */
        BOARD_DCDCInit();
        
        /* Install callbacks to handle enter and exit low power */
        #if cPWR_UsePowerDownMode
        BOARD_InstallLowPowerCallbacks();
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
#if (gDCDC_Enabled_d == 0) || ((gDCDC_Enabled_d == 1) && (APP_DCDC_MODE == gDCDC_Mode_Bypass_c))
        CLOCK_EnableClock(kCLOCK_Dcdc0);
        CLOCK_EnableClock(kCLOCK_Adc0);
        ADC16_GetDefaultConfig(&adcUserConfig);
        adcUserConfig.resolution = kADC16_ResolutionSE12Bit;
        adcUserConfig.referenceVoltageSource = kADC16_ReferenceVoltageSourceValt;
        ADC16_Init(ADC0, &adcUserConfig);  
        ADC16_CalibrateParams();
#endif     
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

uint16_t BOARD_GetPotentiometerLevel(void)
{
    uint32_t random = 0;
    RNG_GetRandomNo(&random);
    
    return (random & 0x0FFF);
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
    return (int32_t)(STANDARD_TEMP - ((int32_t)adcValue - (int32_t)adcrTemp25) * 100000 /(int32_t)(adcr100m*M));
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
void ADC16_CalibrateParams(void)
{
#if gDCDC_Enabled_d == 0    
 #if FSL_FEATURE_ADC16_HAS_CALIBRATION
      ADC16_DoAutoCalibration(ADC0);
 #endif /* FSL_FEATURE_ADC16_HAS_CALIBRATION */
      ADC16_SetHardwareAverage(ADC0 , kADC16_HardwareAverageCount16);
#endif
  
    pmc_bandgap_buffer_config_t pmcBandgapConfig = 
{
#if (defined(FSL_FEATURE_PMC_HAS_BGBE) && FSL_FEATURE_PMC_HAS_BGBE)
       .enable = true, /*!< Enable bandgap buffer.                   */
#endif
#if (defined(FSL_FEATURE_PMC_HAS_BGEN) && FSL_FEATURE_PMC_HAS_BGEN)
    .enableInLowPower = false, /*!< Enable bandgap buffer in low power mode. */
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

void BOARD_InstallLowPowerCallbacks()
{
#if cPWR_UsePowerDownMode
  PWR_RegisterLowPowerEnterCallback((pfPWRCallBack_t)BOARD_EnterLowPowerCb);
  PWR_RegisterLowPowerExitCallback((pfPWRCallBack_t)BOARD_ExitLowPowerCb); 
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

/*******************************************************************************
 * EOF
 ******************************************************************************/

