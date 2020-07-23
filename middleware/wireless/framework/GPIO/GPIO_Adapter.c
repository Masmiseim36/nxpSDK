/*! *********************************************************************************
* Copyright (c) 2015, Freescale Semiconductor, Inc.
* Copyright 2016-2017, 2019 NXP
* All rights reserved.
*
* \file
*
* SPDX-License-Identifier: BSD-3-Clause
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Include
*************************************************************************************
********************************************************************************** */
#include "GPIO_Adapter.h"
#include "fsl_gpio.h"
#if !(defined(FSL_FEATURE_SOC_SYSCON_COUNT) && (FSL_FEATURE_SOC_SYSCON_COUNT > 0))
#include "fsl_port.h"
#else
#include "fsl_iocon.h"
#include "fsl_inputmux.h"
#endif
#include "fsl_os_abstraction.h"
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
#include "fsl_intmux.h"
#endif

/*! *********************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
********************************************************************************** */
#define gGpio_FoundPortIsr_c  0x01
#define gGpio_FoundSimilar_c  0x02

#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT

#ifndef gGpio_IntmuxInstance_d
#define gGpio_IntmuxInstance_d 1
#endif

#endif

/*! *********************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
********************************************************************************** */


/*! *********************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
********************************************************************************** */
static void Gpio_CommonIsr(void);
static gpioStatus_t Gpio_InstallPortISR(IRQn_Type irqId, uint32_t nvicPrio);

#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
static void GpioSetInterruptType(GPIO_Type *base, uint8_t gpioPin, pinInterrupt_t int_mode);
static pinInterrupt_t GpioGetInterruptType(GPIO_Type *base, gpioPort_t gpioPort, uint8_t gpioPin);
#endif
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
static void Gpio_IntmuxInit(void);
#endif
/*! *********************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
********************************************************************************** */
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
typedef GPIO_Type PORT_Type;
#define PORT_BASE_PTRS GPIO_BASE_PTRS
#define PORT_IRQS GPIO_IRQS

static const uint8_t gpioPinsPerPort[gpioPort_Invalid_c] =
{
    32, /* gpioPort_A_c */
    3,  /* gpioPort_B_c */
    0,  /* gpioPort_C_c */
    0,  /* gpioPort_D_c */
    0,  /* gpioPort_E_c */
};

#endif

static gpioIsr_t mGpioIsr[gGpioMaxIsrEntries_c];
static uint16_t  mGpioIsrCount=0U;
static PORT_Type *const maPortBases[] = PORT_BASE_PTRS;
static GPIO_Type *const maGpioBases[] = GPIO_BASE_PTRS;
static IRQn_Type maPortIrqId[] = PORT_IRQS;
/* bit field of IO sources for the wakeup */
static uint32_t mGpioWakeIoCause = 0U;
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
static INTMUX_Type *const maIntmuxBases[] = INTMUX_BASE_PTRS;
static const IRQn_Type maIntmuxIRQNumber[][FSL_FEATURE_INTMUX_CHANNEL_COUNT] = INTMUX_IRQS;
static const uint8_t maGpioIntMuxChannel[] = {0,1,2,3,4};

#endif

/*! *********************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
********************************************************************************** */

#if !(defined(FSL_FEATURE_SOC_SYSCON_COUNT) && (FSL_FEATURE_SOC_SYSCON_COUNT > 0))
/* Section for Kinteis boards only ( exclude LPC and QN ) */

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */
uint32_t GpioGetPinPCR(gpioPort_t gpioPort, uint8_t gpioPin)
{
    PORT_Type *base = maPortBases[gpioPort];
    return (uint32_t)base->PCR[gpioPin];
}
/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */
void GpioSetPinPCR(gpioPort_t gpioPort, uint8_t gpioPin, uint32_t pcr)
{
    PORT_Type *base = maPortBases[gpioPort];
    base->PCR[gpioPin] = pcr;
}
/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */
void GpioSetPinPCR_ISF_Preserved(gpioPort_t gpioPort, uint8_t gpioPin, uint32_t pcr)
{
    PORT_Type *base = maPortBases[gpioPort];
    base->PCR[gpioPin] = pcr & (~PORT_PCR_ISF_MASK) ;
}

void GpioSetSlewRate(gpioPort_t gpioPort, uint8_t gpioPin, pinSlewRate_t slewRate)
{
    PORT_Type *base = maPortBases[gpioPort];

    if( slewRate < pinSlewRate_Invalid_c )
    {
        base->PCR[gpioPin] &= ~PORT_PCR_SRE_MASK;
        base->PCR[gpioPin] |= PORT_PCR_SRE(slewRate);
    }
}

void GpioSetOpenDrain(gpioPort_t gpioPort, uint8_t gpioPin, bool_t enable)
{
#if defined(FSL_FEATURE_PORT_HAS_OPEN_DRAIN) && FSL_FEATURE_PORT_HAS_OPEN_DRAIN
    PORT_Type *base = maPortBases[gpioPort];

    if(enable)
    {
        base->PCR[gpioPin] |= PORT_PCR_ODE_MASK;
    }
    else
    {
        base->PCR[gpioPin] &= ~PORT_PCR_ODE_MASK;
    }
#endif /* FSL_FEATURE_PORT_HAS_OPEN_DRAIN */
}

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioSetPassiveFilter(gpioPort_t gpioPort, uint8_t gpioPin, bool_t enable)
{
    PORT_Type *base = maPortBases[gpioPort];

    if(enable)
    {
        base->PCR[gpioPin] |= PORT_PCR_PFE_MASK;
    }
    else
    {
        base->PCR[gpioPin] &= ~PORT_PCR_PFE_MASK;
    }
}
#endif /* ! FSL_FEATURE_SOC_SYSCON_COUNT */

pinInterrupt_t GpioGetPinInterrupt(gpioPort_t gpioPort, uint8_t gpioPin)
{
    pinInterrupt_t result = pinInt_Disabled_c;
    PORT_Type *base = maPortBases[gpioPort];
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    result = GpioGetInterruptType( base, gpioPort, gpioPin);
#else
    result = (pinInterrupt_t)(uint32_t)((base->PCR[gpioPin] & PORT_PCR_IRQC_MASK) >> PORT_PCR_IRQC_SHIFT);
#endif
    return result;
}

void GpioSetPinInterrupt(gpioPort_t gpioPort, uint8_t gpioPin, pinInterrupt_t int_mode)
{
  if(int_mode < pinInt_Invalid_c)
  {
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    GpioSetInterruptType(maPortBases[gpioPort], gpioPin, int_mode);
    if(int_mode > pinInt_Disabled_c)
    {
      GPIO_EnableInterrupt(maPortBases[gpioPort], (1 << gpioPin));
    }
#else
    PORT_SetPinInterruptConfig(maPortBases[gpioPort], gpioPin, (port_interrupt_t)(int_mode));
#endif
  }
}

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioSetPinPullMode(gpioPort_t gpioPort, uint8_t gpioPin, pinPullSelect_t pullSelect)
{
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    IOCON_PullSet(IOCON, gpioPort, gpioPin, pullSelect);
#else
  uint32_t pcr;
  PORT_Type *base = maPortBases[gpioPort];
  pcr = base->PCR[gpioPin];
  pcr &= ~(PORT_PCR_PS_MASK | PORT_PCR_PE_MASK);
  if(pullSelect < pinPull_Disabled_c)
  {
    pcr |= (uint32_t)pullSelect | PORT_PCR_PE_MASK;
  }
  base->PCR[gpioPin] = pcr;
#endif
}
/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioSetPinMux(gpioPort_t gpioPort, uint8_t gpioPin, pinMux_t pinMux)
{
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    IOCON_FuncSet(IOCON, gpioPort, gpioPin, pinMux);
#else
  PORT_SetPinMux(maPortBases[gpioPort], (uint32_t) gpioPin, (port_mux_t)pinMux);
#endif
}

/*! *********************************************************************************
* \brief
*
* \param[in]
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioSetPinMux_ISF_Preserved(gpioPort_t gpioPort, uint8_t gpioPin, pinMux_t pinMux)
{
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    // on QN9080 ISF is not changed when setting pin mux
    GpioSetPinMux(gpioPort, gpioPin, pinMux);
#else
    PORT_Type *base = maPortBases[gpioPort];
    base->PCR[gpioPin] = (base->PCR[gpioPin] & ~(PORT_PCR_MUX_MASK | PORT_PCR_ISF_MASK)) | PORT_PCR_MUX(pinMux);
#endif
}
/*! *********************************************************************************
* \brief
*
* \param[in]  pInputConfig             pointer to a gpioInputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioClearPinIntFlag(const gpioInputPinConfig_t* pInputConfig)
{
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    GPIO_ClearPinsInterruptFlags(maPortBases[pInputConfig->gpioPort], (1 << pInputConfig->gpioPin));
#else
  PORT_ClearPinsInterruptFlags(maPortBases[pInputConfig->gpioPort], (uint32_t)1<<pInputConfig->gpioPin);
#endif
}

/*! *********************************************************************************
* \brief  Check the Gpio Is Pin Cause Of Wakeup source
*
* \param[in]  pInputConfig             pointer to a gpioInputPinConfig_t structure
*
* \return  The pin is wake up cause
*
********************************************************************************** */
bool_t GpioIsPinCauseOfWakeup(const gpioInputPinConfig_t* pInputConfig)
{
    bool_t res = FALSE;
    uint32_t pin_bit =  (uint32_t)1 << (pInputConfig->gpioPin);
    if ((mGpioWakeIoCause & pin_bit) != 0U)
    {
        mGpioWakeIoCause &= ~pin_bit;
        res = TRUE;
    }
    return res;
}
/*! *********************************************************************************
* \brief
*
* \param[in]  wkupIoSrc          wakeup io source
*
* \return  The pin is wake up cause
*
********************************************************************************** */
void GpioNotifyIoWakeupSource(uint32_t wkupIoSrc)
{
    mGpioWakeIoCause = wkupIoSrc;
}
/*! *********************************************************************************
* \brief
*
* \param[in]  pInputConfig             pointer to a gpioInputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */

uint32_t GpioIsPinIntPending(const gpioInputPinConfig_t* pInputConfig)
{
  uint32_t isfr = PORT_GetPinsInterruptFlags(maPortBases[pInputConfig->gpioPort]);
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    isfr = GPIO_GetPinsInterruptFlags(maPortBases[pInputConfig->gpioPort]);
#else
    isfr = PORT_GetPinsInterruptFlags(maPortBases[pInputConfig->gpioPort]);
#endif

  return ((isfr >> pInputConfig->gpioPin) & 0x01U);
}

/*! *********************************************************************************
* \brief  Set the logical level of the output pin
*
* \param[in]  pOutputConfig             pointer to a gpioOutputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioSetPinOutput(const gpioOutputPinConfig_t* pOutputConfig)
{
  GPIO_PortSet(maGpioBases[pOutputConfig->gpioPort], (uint32_t)1<< pOutputConfig->gpioPin);
}
/*! *********************************************************************************
* \brief  Clear the logical level of the output pin
*
* \param[in]  pOutputConfig             pointer to a gpioOutputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioClearPinOutput(const gpioOutputPinConfig_t* pOutputConfig)
{
  GPIO_PortClear(maGpioBases[pOutputConfig->gpioPort], (uint32_t)1<< pOutputConfig->gpioPin);
}
/*! *********************************************************************************
* \brief  Toggle the logical level of the output pin
*
* \param[in]  pOutputConfig             pointer to a gpioOutputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */

void GpioTogglePinOutput(const gpioOutputPinConfig_t* pOutputConfig)
{
  GPIO_PortToggle(maGpioBases[pOutputConfig->gpioPort], (uint32_t)1<< pOutputConfig->gpioPin);
}

/*! *********************************************************************************
* \brief  Reads the logical level of the output pin
*
* \param[in]  pOutputConfig             pointer to a gpioOutputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */
uint32_t GpioReadOutputPin(const gpioOutputPinConfig_t* pOutputConfig)
{
    return GPIO_PinRead(maGpioBases[pOutputConfig->gpioPort], pOutputConfig->gpioPin);
}

/*! *********************************************************************************
* \brief  Reads the logical level of the input pin
*
* \param[in]  pInputConfig             pointer to a gpioInputPinConfig_t structure
*
* \return  logical level of the input pin
*
********************************************************************************** */

uint32_t GpioReadPinInput(const gpioInputPinConfig_t* pInputConfig)
{
  return GPIO_PinRead(maGpioBases[pInputConfig->gpioPort], pInputConfig->gpioPin);
}

/*! *********************************************************************************
* \brief  Set Pin drive strength
********************************************************************************** */
void GpioSetDriveStrength(gpioPort_t gpioPort, uint8_t gpioPin, pinDriveStrength_t pinDriveStrenght)
{
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
    IOCON_DriveSet(IOCON, gpioPort, (uint32_t)gpioPin, pinDriveStrenght);
#else
  #if defined(FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH) && FSL_FEATURE_PORT_HAS_DRIVE_STRENGTH
    PORT_SetPinDriveStrength(maPortBases[gpioPort], (uint32_t)gpioPin, (uint8_t)pinDriveStrenght);
  #endif
#endif
}

/*! *********************************************************************************
* \brief  Initializes input pins
*
* \param[in]  pInputConfig             table of input pins configuration structures
* \param[in]  noOfElements             number of elements of the table
*
* \return  install status
*
********************************************************************************** */

bool_t GpioInputPinInit(const gpioInputPinConfig_t* pInputConfig, uint32_t noOfElements )
{
    gpio_pin_config_t gpioConfig;
    gpioConfig.pinDirection = kGPIO_DigitalInput; //default pin direction
    bool_t status = TRUE;

    if(pInputConfig == NULL)
    {
        status = FALSE;
    }
    else
    {
        for(; noOfElements > 0u; noOfElements--)
        {
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0))&&(defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
            if((pInputConfig->gpioPin >= gpioPinsPerPort[pInputConfig->gpioPort]) ||
#else
            if((pInputConfig->gpioPin >= NumberOfElements(((PORT_Type*)0)->PCR)) ||
#endif
               (pInputConfig->gpioPort >= (gpioPort_t)FSL_FEATURE_SOC_GPIO_COUNT) ||
               (pInputConfig->pullSelect >= pinPull_Invalid_c) ||
               (pInputConfig->interruptSelect >= pinInt_Invalid_c))
            {
                status = FALSE;
                break;
            }
            else
            {
                GpioSetPinPullMode(pInputConfig->gpioPort, pInputConfig->gpioPin, pInputConfig->pullSelect);
                GpioSetPinMux(pInputConfig->gpioPort, pInputConfig->gpioPin, pinMux_Gpio_c);
                GPIO_PinInit(maGpioBases[pInputConfig->gpioPort], pInputConfig->gpioPin, &gpioConfig); //set Pin direction = kGPIO_DigitalInput
                GpioSetPinInterrupt(pInputConfig->gpioPort, pInputConfig->gpioPin, pInputConfig->interruptSelect);

                pInputConfig++;
            }
        }
    }
    return status;
}

/*! *********************************************************************************
* \brief  Initializes output pins
*
* \param[in]  pOutputConfig   table of output pins configuration structures
* \param[in]  noOfElements    number of elements of the table
* \return  install status
*
********************************************************************************** */

bool_t GpioOutputPinInit(const gpioOutputPinConfig_t* pOutputConfig, uint32_t noOfElements )
{
  gpio_pin_config_t gpioConfig;
  gpioConfig.pinDirection = kGPIO_DigitalOutput; //pin direction = out
  bool_t status = TRUE;

  if(pOutputConfig == NULL)
  {
    status = FALSE;
  }
  else
  {
    for(; noOfElements > 0u; noOfElements--)
    {
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && ((defined(FSL_FEATURE_SOC_SYSCON_COUNT))&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
          if((pOutputConfig->gpioPin >= gpioPinsPerPort[pOutputConfig->gpioPort]) ||
#else
          if((pOutputConfig->gpioPin >= NumberOfElements(((PORT_Type*)0)->PCR)) ||
#endif
             (pOutputConfig->gpioPort >= (gpioPort_t)FSL_FEATURE_SOC_GPIO_COUNT) ||
#if !(!(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0))&&(defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0)))
             (pOutputConfig->slewRate >= pinSlewRate_Invalid_c) ||
#endif
             (pOutputConfig->driveStrength >= pinDriveStrength_Invalid_c))
      {
        status = FALSE;
        break;
      }
      else
      {
        GpioSetPinMux(pOutputConfig->gpioPort, pOutputConfig->gpioPin, pinMux_Gpio_c);
        GpioSetDriveStrength(pOutputConfig->gpioPort, pOutputConfig->gpioPin, pOutputConfig->driveStrength);
#if !(!(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && ((defined(FSL_FEATURE_SOC_SYSCON_COUNT))&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0)))
        GpioSetSlewRate(pOutputConfig->gpioPort, pOutputConfig->gpioPin, pOutputConfig->slewRate);
#endif
        gpioConfig.outputLogic = (uint8_t)pOutputConfig->outputLogic;
        GPIO_PinInit(maGpioBases[pOutputConfig->gpioPort], pOutputConfig->gpioPin, &gpioConfig);

        pOutputConfig++;
      }
    }
  }
  return status;
}

/*! *********************************************************************************
* \brief  Disable pins
*
* \param[in]  pOutputConfig   table of output pins configuration structures
* \param[in]  noOfElements    number of elements of the table
* \return  install status
*
********************************************************************************** */
void GpioPinReset(const gpioOutputPinConfig_t* pOutputConfig, uint32_t noOfElements )
{
    assert(pOutputConfig);

    for(; noOfElements > 0u; noOfElements--)
    {
        GpioSetPinMux(pOutputConfig->gpioPort, pOutputConfig->gpioPin, pinMux_PinDisabledOrAnalog_c);
        pOutputConfig++;
    }

    return;
}

/*! *********************************************************************************
* \brief  Uninstall the callback for the specified Pin Definition
*
* \param[in]  cb              The callback function to be installed
* \param[in]  priority        The priority used by the GPIO_IrqAdapter
* \param[in]  nvicPriority    The priority to be set in NVIC. Only most significant bits are used!
* \param[in]  pinDef          The KSDK pin definition
*
* \return  install status
*
********************************************************************************** */

gpioStatus_t GpioInstallIsr( pfGpioIsrCb_t cb, uint8_t priority, uint8_t nvicPriority, const gpioInputPinConfig_t* pInputConfig )
{
    uint32_t i;
    uint8_t  found   = 0;
    uint8_t  pos     = (uint8_t)mGpioIsrCount;
    gpioPort_t portId = pInputConfig->gpioPort;
    uint32_t pinMask = (uint32_t)1 << pInputConfig->gpioPin;
    gpioStatus_t status = gpio_success;
    IRQn_Type irqNo  = maPortIrqId[portId];
    #if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
    IRQn_Type intmuxIRQ ;
    if(irqNo < FSL_FEATURE_INTMUX_IRQ_START_INDEX)
    {
      intmuxIRQ = irqNo;
    }
    else
    {
      intmuxIRQ = maIntmuxIRQNumber[gGpio_IntmuxInstance_d][maGpioIntMuxChannel[portId]];
    }
    #endif

    for( i=0; i<mGpioIsrCount; i++ )
    {
        /* search for port ISR already installed */
        if( mGpioIsr[i].port == (uint8_t)portId )
        {
            found |= (uint8_t)gGpio_FoundPortIsr_c;
        }
        /* search for insert position */
        if( (pos == mGpioIsrCount) && (mGpioIsr[i].prio >= priority) )
        {
            pos = (uint8_t)i;
        }
        /* search for an entry with the same callback installed for the same port with the same priority */
        if( (mGpioIsr[i].callback == cb) && ((gpioPort_t)mGpioIsr[i].port == portId) && (mGpioIsr[i].prio == priority) )
        {
            pos = (uint8_t)i;
            found |= (uint8_t)gGpio_FoundSimilar_c;
            break;
        }
    }

    if( (found & (uint8_t)gGpio_FoundSimilar_c) != 0u )
    {
        /* found the same ISR installed for the same port, but other pins */
        mGpioIsr[pos].pinMask |= pinMask;
    }
    else
    {
      if( mGpioIsrCount >= (uint16_t)gGpioMaxIsrEntries_c )
      {
          status = gpio_outOfMemory;
      }
      else
      {
          OSA_InterruptDisable();
          if( pos != mGpioIsrCount )
          {

            /* Shift all entries to the left, to obtain a sorted list */
            for( i=mGpioIsrCount; i>pos; i-- )
            {
              mGpioIsr[i] = mGpioIsr[i-(uint32_t)1];
            }

          }
          /* install new callback */
          mGpioIsr[pos].callback = cb;
          mGpioIsr[pos].prio     = priority;
          mGpioIsr[pos].port     = (uint8_t)portId;
          mGpioIsr[pos].pinMask  = pinMask;
          #if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
          mGpioIsr[pos].irqId = intmuxIRQ;
          #else
          mGpioIsr[pos].irqId    = irqNo;
          #endif

          mGpioIsrCount++;
          OSA_InterruptEnable();
      }
    }

    if(gpio_success == status)
    {
        if( found != 0u)
        {
            /* The PORT ISR was already installed. Update NVIC priority if higher than the old one! */
            nvicPriority = nvicPriority >> (8 - __NVIC_PRIO_BITS);
            #if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
            i = NVIC_GetPriority(intmuxIRQ);
            if( i > nvicPriority )
            {
                NVIC_SetPriority(intmuxIRQ, nvicPriority);
            }
            #else
            i = NVIC_GetPriority(irqNo);
            if( i > nvicPriority )
            {
                NVIC_SetPriority(irqNo, nvicPriority);
            }
            /* install the common GPIO handler and enbale IRQ when upper layer call the function on wake up from lowpower mode */
            OSA_InstallIntHandler((uint32_t)irqNo, Gpio_CommonIsr);
            NVIC_EnableIRQ(irqNo);
            #endif
        }
        else
        {
            /* Install common PORT ISR */
            status = Gpio_InstallPortISR(irqNo, nvicPriority);
        }
    }
    return status;
}

/*! *********************************************************************************
* \brief  Uninstall the callback for the specified Pin Definition
*
* \param[in]  pinDef    The KSDK pin definition
*
* \return  uninstall status
*
********************************************************************************** */

gpioStatus_t GpioUninstallIsr( const gpioInputPinConfig_t* pInputConfig )
{
    IRQn_Type irqNo;
    uint32_t  i = 0, j;
    gpioStatus_t status;
    gpioPort_t portId = pInputConfig->gpioPort;
    uint32_t pinMask  = ((uint32_t)1 <<pInputConfig->gpioPin);
    while(true)
    {
        if( ((gpioPort_t)mGpioIsr[i].port == portId) && (bool)(mGpioIsr[i].pinMask & pinMask) )
        {
            OSA_InterruptDisable();
            /* uninstall ISR only for specified pins */
            mGpioIsr[i].pinMask &= ~pinMask;
            /* if no more pins are active, uninstall handler function */
            if( mGpioIsr[i].pinMask == 0u )
            {
                irqNo = mGpioIsr[i].irqId;
                mGpioIsr[i].callback = NULL;

                /* Shift next entries to the left */
                for( j=i; j<mGpioIsrCount-(uint32_t)1; j++ )
                {
                    mGpioIsr[j] = mGpioIsr[j+(uint32_t)1];
                }
                mGpioIsrCount--;

                /* Search for other ISR installed for the same IRQ */
                for( j=0; j<mGpioIsrCount; j++ )
                {
                    if( irqNo == mGpioIsr[j].irqId )
                    {
                        irqNo = NotAvail_IRQn;
                        break;
                    }
                }

                /* If no other ISR was installed for this IRQ, disable IRQ in NVIC */
                if( irqNo != NotAvail_IRQn )
                {
                    NVIC_DisableIRQ(irqNo);
                }
            }
            OSA_InterruptEnable();
            status = gpio_success;
            break;
        }
        else
        {
            if(++i >= mGpioIsrCount)
            {
                status = gpio_notFound;
                break;
            }
        }
    }
    return status;
}

/*! *********************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
********************************************************************************* */

/*! *********************************************************************************
* \brief  Common GPIO ISR.
*
********************************************************************************** */
static void Gpio_CommonIsr(void)
{
    uint32_t i;
    uint32_t flags;

    IRQn_Type irqId = (IRQn_Type)(uint32_t)(__get_IPSR() - (uint32_t)16);
#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
#define INSTANCE FSL_FEATURE_FLASH_CURRENT_CORE_ID
    const IRQn_Type s_intmuxIRQNumber[][FSL_FEATURE_INTMUX_CHANNEL_COUNT] = INTMUX_IRQS;
    if ((irqId >= s_intmuxIRQNumber[INSTANCE][0]) &&
        (irqId <= s_intmuxIRQNumber[INSTANCE][FSL_FEATURE_INTMUX_CHANNEL_COUNT-1]))
    {
        irqId += FSL_FEATURE_INTMUX_IRQ_START_INDEX;
    }
#endif

    /* Search for the highest priority ISR installed for the current PORT */
    for( i=0; i<mGpioIsrCount; i++ )
    {
#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
        flags = GPIO_GetPinsInterruptFlags(maPortBases[mGpioIsr[i].port]);
#else
        flags = PORT_GetPinsInterruptFlags(maPortBases[mGpioIsr[i].port]);
#endif
        if ( (mGpioIsr[i].irqId == irqId)
             && (bool)(mGpioIsr[i].pinMask & flags) )
        {
            mGpioIsr[i].callback();
        }
    }
}

/*! *********************************************************************************
* \brief  Install the Gpio_CommonIsr() ISR for the specified IRQ
*
* \param[in]  irqId    The CMSIS irq Id
* \param[in]  nvicPrio The priority to be set in NVIC
*
* \return  install status
*
********************************************************************************** */
static gpioStatus_t Gpio_InstallPortISR(IRQn_Type irqId, uint32_t nvicPrio)
{
    assert( irqId != NotAvail_IRQn );

    OSA_InstallIntHandler((uint32_t)irqId, Gpio_CommonIsr);

#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
    if(irqId>= FSL_FEATURE_INTMUX_IRQ_START_INDEX)
      {
	uint8_t portId;
	for(portId = 0; portId < gpioPort_Invalid_c; portId++)
          {
            if(maPortIrqId[portId] == irqId)
	      {
		Gpio_IntmuxInit();
		INTMUX_EnableInterrupt(maIntmuxBases[gGpio_IntmuxInstance_d], maGpioIntMuxChannel[portId], irqId);
		NVIC_SetPriority(maIntmuxIRQNumber[gGpio_IntmuxInstance_d][maGpioIntMuxChannel[portId]], nvicPrio >> (8U - __NVIC_PRIO_BITS));
		break;
	      }
          }
      }
    else
#endif
      {
	/* Enable IRQ in NVIC and set priority */
	NVIC_ClearPendingIRQ(irqId);
	NVIC_EnableIRQ(irqId);
	NVIC_SetPriority(irqId, nvicPrio >> (8 - __NVIC_PRIO_BITS));
      }
    return gpio_success;
}

#if !(defined(FSL_FEATURE_SOC_INTMUX_COUNT)&&(FSL_FEATURE_SOC_INTMUX_COUNT > 0)) && (defined(FSL_FEATURE_SOC_SYSCON_COUNT)&&(FSL_FEATURE_SOC_SYSCON_COUNT > 0))
static void GpioSetInterruptType(GPIO_Type *base,  uint8_t gpioPin, pinInterrupt_t int_mode)
{
    switch (int_mode)
    {
    case pinInt_Disabled_c:
        GPIO_DisableInterrupt(base, (uint32_t)(1 << gpioPin));
        break;

    case pinInt_RisingEdge_c:
        GPIO_SetRisingEdgeInterrupt(base, (uint32_t)(1 << gpioPin));
        break;

    case pinInt_FallingEdge_c:
        GPIO_SetFallingEdgeInterrupt(base, (uint32_t)(1 << gpioPin));
        break;

    case pinInt_LogicZero_c:
        GPIO_SetLowLevelInterrupt(base, (uint32_t)(1 << gpioPin));
        break;

    case pinInt_LogicOne_c:
        GPIO_SetHighLevelInterrupt(base, (uint32_t)(1 << gpioPin));
        break;

    default:
    	/* MISRA rule 16.4 */
        break;
    }
}

static pinInterrupt_t GpioGetInterruptType(GPIO_Type *base, gpioPort_t gpioPort, uint8_t gpioPin)
{
    pinInterrupt_t result = pinInt_Disabled_c;

    if ((base->INTENSET >> gpioPin) & 0x1)            // is interrupt enabled for gpioPin ?
    {
        if ((base->INTPOLSET >> gpioPin) & 0x1)       // polartity == 1 => high level or rising edge
        {
            result = ((base->INTTYPESET >> gpioPin) & 0x1) ? pinInt_RisingEdge_c : pinInt_LogicOne_c;
        }
        else                                          // polartity == 0 => low level or falling edge
        {
            result = ((base->INTTYPESET >> gpioPin) & 0x1) ? pinInt_FallingEdge_c : pinInt_LogicZero_c;
        }
    }
    // no else  -  result is already pinInt_Disabled_c

    return result;
}

void SetGpioWakeupLevel(GPIO_Type *base, uint32_t pin, uint8_t level)
{
    if (base == GPIOA)
    {
        // configure gpio wakeup level
        if (level == 1U)
        {
            SYSCON->PIO_WAKEUP_LVL0 = SYSCON->PIO_WAKEUP_LVL0 & ~(1U << pin);
        }
        else
        {
            SYSCON->PIO_WAKEUP_LVL0 = SYSCON->PIO_WAKEUP_LVL0 | (1U << pin);
        }
        // enable gpio wakeup pin
        SYSCON->PIO_WAKEUP_EN0 = SYSCON->PIO_WAKEUP_EN0 | (1U << pin);
    }
    else
    {
        // configure gpio wakeup level
        if (level == 1U)
        {
            SYSCON->PIO_WAKEUP_LVL1 = SYSCON->PIO_WAKEUP_LVL1 & ~(1U << pin);
        }
        else
        {
            SYSCON->PIO_WAKEUP_LVL1 = SYSCON->PIO_WAKEUP_LVL1 | (1U << pin);
        }
        // enable gpio wakeup pin
        SYSCON->PIO_WAKEUP_EN1 = SYSCON->PIO_WAKEUP_EN1 | (1U << pin);
    }
}
#endif

#if defined(FSL_FEATURE_SOC_INTMUX_COUNT) && FSL_FEATURE_SOC_INTMUX_COUNT
/*! *********************************************************************************
* \brief
*
*
********************************************************************************** */
static void Gpio_IntmuxInit(void)
{
  static uint8_t intmuxInit = 0;
  if(intmuxInit == 0)
    {
      intmuxInit = 1;
      INTMUX_Init(maIntmuxBases[gGpio_IntmuxInstance_d]);
    }
}
#endif
