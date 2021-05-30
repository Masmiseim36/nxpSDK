/*
** ###################################################################
**     Processors:          MKM34Z256VLL7
**                          MKM34Z256VLQ7
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          IAR ANSI C/C++ Compiler for ARM
**                          MCUXpresso Compiler
**
**     Reference manual:    KM34P144M75SF0RM, Rev.1, Jan 2015
**     Version:             rev. 1.2, 2015-03-06
**     Build:               b170224
**
**     Abstract:
**         CMSIS Peripheral Access Layer for MKM34Z7
**
**     Copyright 1997-2016 Freescale Semiconductor, Inc.
**     Copyright 2016-2017 NXP
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of the copyright holder nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.nxp.com
**     mail:                 support@nxp.com
**
**     Revisions:
**     - rev. 1.0 (2014-10-17)
**         Initial version.
**     - rev. 1.1 (2015-01-27)
**         Update according to reference manual rev. 1, RC.
**     - rev. 1.2 (2015-03-06)
**         Update according to reference manual rev. 1.
**
** ###################################################################
*/

/*!
 * @file MKM34Z7.h
 * @version 1.2
 * @date 2015-03-06
 * @brief CMSIS Peripheral Access Layer for MKM34Z7
 *
 * CMSIS Peripheral Access Layer for MKM34Z7
 */

#ifndef _MKM34Z7_H_
#define _MKM34Z7_H_                              /**< Symbol preventing repeated inclusion */

/** Memory map major version (memory maps with equal major version number are
 * compatible) */
#define MCU_MEM_MAP_VERSION 0x0100U
/** Memory map minor version */
#define MCU_MEM_MAP_VERSION_MINOR 0x0002U


/* ----------------------------------------------------------------------------
   -- Interrupt vector numbers
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Interrupt_vector_numbers Interrupt vector numbers
 * @{
 */

/** Interrupt Number Definitions */
#define NUMBER_OF_INT_VECTORS 48                 /**< Number of interrupts in the Vector table */

typedef enum IRQn {
  /* Auxiliary constants */
  NotAvail_IRQn                = -128,             /**< Not available device specific interrupt */

  /* Core interrupts */
  NonMaskableInt_IRQn          = -14,              /**< Non Maskable Interrupt */
  HardFault_IRQn               = -13,              /**< Cortex-M0 SV Hard Fault Interrupt */
  SVCall_IRQn                  = -5,               /**< Cortex-M0 SV Call Interrupt */
  PendSV_IRQn                  = -2,               /**< Cortex-M0 Pend SV Interrupt */
  SysTick_IRQn                 = -1,               /**< Cortex-M0 System Tick Interrupt */

  /* Device specific interrupts */
  DMA0_IRQn                    = 0,                /**< DMA channel 0 transfer complete */
  DMA1_IRQn                    = 1,                /**< DMA channel 1 transfer complete */
  DMA2_IRQn                    = 2,                /**< DMA channel 2 transfer complete */
  DMA3_IRQn                    = 3,                /**< DMA channel 3 transfer complete */
  SPI0_SPI1_IRQn               = 4,                /**< SPI0/SPI1 ORed interrupt */
  PDB0_IRQn                    = 5,                /**< PDB0 ORed interrupt */
  PMC_IRQn                     = 6,                /**< Low-voltage detect, low-voltage warning */
  TMR0_IRQn                    = 7,                /**< Quad Timer Channel 0 */
  TMR1_IRQn                    = 8,                /**< Quad Timer Channel 1 */
  TMR2_IRQn                    = 9,                /**< Quad Timer Channel 2 */
  TMR3_IRQn                    = 10,               /**< Quad Timer Channel 3 */
  PIT0_PIT1_IRQn               = 11,               /**< PIT0/PIT1 ORed interrupt */
  LLWU_IRQn                    = 12,               /**< Low Leakage Wakeup */
  FTFA_IRQn                    = 13,               /**< Command complete and read collision */
  CMP0_CMP1_CMP2_IRQn          = 14,               /**< CMP0/CMP1/CMP2 ORed interrupt */
  LCD_IRQn                     = 15,               /**< LCD interrupt */
  ADC0_IRQn                    = 16,               /**< ADC0 interrupt */
  PTx_IRQn                     = 17,               /**< Single interrupt vector for GPIOA,GPIOB,GPIOC,GPIOD,GPIOE,GPIOF,GPIOG,GPIOH,GPIOI,GPIOJ,GPIOK,GPIOL,GPIOM */
  RNGA_IRQn                    = 18,               /**< RNGA interrupt */
  UART0_UART1_UART2_UART3_IRQn = 19,               /**< UART0/UART1/UART2/UART3 ORed interrupt */
  MMAU_IRQn                    = 20,               /**< Memory Mapped Arithmetic Unit interrupt */
  AFE_CH0_IRQn                 = 21,               /**< AFE Channel 0 */
  AFE_CH1_IRQn                 = 22,               /**< AFE Channel 1 */
  AFE_CH2_IRQn                 = 23,               /**< AFE Channel 2 */
  AFE_CH3_IRQn                 = 24,               /**< AFE Channel 3 */
  RTC_IRQn                     = 25,               /**< IRTC interrupt */
  I2C0_I2C1_IRQn               = 26,               /**< I2C0/I2C1 ORed interrupt */
  LPUART0_IRQn                 = 27,               /**< LPUART0 status and error interrupt */
  MCG_IRQn                     = 28,               /**< MCG interrupt */
  WDOG_EWM_IRQn                = 29,               /**< WDOG/EWM ORed interrupt */
  LPTMR0_IRQn                  = 30,               /**< LPTMR0 interrupt */
  XBAR_IRQn                    = 31                /**< XBAR interrupt */
} IRQn_Type;

/*!
 * @}
 */ /* end of group Interrupt_vector_numbers */


/* ----------------------------------------------------------------------------
   -- Cortex M0 Core Configuration
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Cortex_Core_Configuration Cortex M0 Core Configuration
 * @{
 */

#define __CM0PLUS_REV                  0x0000    /**< Core revision r0p0 */
#define __MPU_PRESENT                  0         /**< Defines if an MPU is present or not */
#define __VTOR_PRESENT                 1         /**< Defines if VTOR is present or not */
#define __NVIC_PRIO_BITS               2         /**< Number of priority bits implemented in the NVIC */
#define __Vendor_SysTickConfig         0         /**< Vendor specific implementation of SysTickConfig is defined */

#include "core_cm0plus.h"              /* Core Peripheral Access Layer */
#include "system_MKM34Z7.h"            /* Device specific configuration file */

/*!
 * @}
 */ /* end of group Cortex_Core_Configuration */


/* ----------------------------------------------------------------------------
   -- Mapping Information
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Mapping_Information Mapping Information
 * @{
 */

/** Mapping Information */
/*!
 * @addtogroup edma_request
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*!
 * @brief Structure for the DMA hardware request
 *
 * Defines the structure for the DMA hardware request collections. The user can configure the
 * hardware request into DMAMUX to trigger the DMA transfer accordingly. The index
 * of the hardware request varies according  to the to SoC.
 */
typedef enum _dma_request_source
{
    kDmaRequestMux0Disable          = 0|0x100U,    /**< Disable */
    kDmaRequestMux0Reserved1        = 1|0x100U,    /**< Reserved1 */
    kDmaRequestMux0UART0Rx          = 2|0x100U,    /**< UART0 Receive */
    kDmaRequestMux0UART0Tx          = 3|0x100U,    /**< UART0 Transmit */
    kDmaRequestMux0UART1Rx          = 4|0x100U,    /**< UART1 Receive */
    kDmaRequestMux0UART1Tx          = 5|0x100U,    /**< UART1 Transmit */
    kDmaRequestMux0UART2Rx          = 6|0x100U,    /**< UART2 Receive */
    kDmaRequestMux0UART2Tx          = 7|0x100U,    /**< UART2 Transmit */
    kDmaRequestMux0UART3Rx          = 8|0x100U,    /**< UART3 Receive */
    kDmaRequestMux0UART3Tx          = 9|0x100U,    /**< UART3 Transmit */
    kDmaRequestMux0Reserved10       = 10|0x100U,   /**< Reserved10 */
    kDmaRequestMux0Reserved11       = 11|0x100U,   /**< Reserved11 */
    kDmaRequestMux0Reserved12       = 12|0x100U,   /**< Reserved12 */
    kDmaRequestMux0Reserved13       = 13|0x100U,   /**< Reserved13 */
    kDmaRequestMux0Reserved14       = 14|0x100U,   /**< Reserved14 */
    kDmaRequestMux0Reserved15       = 15|0x100U,   /**< Reserved15 */
    kDmaRequestMux0SPI0Rx           = 16|0x100U,   /**< SPI0 Receive */
    kDmaRequestMux0SPI0Tx           = 17|0x100U,   /**< SPI0 Transmit */
    kDmaRequestMux0SPI1Rx           = 18|0x100U,   /**< SPI1 Receive */
    kDmaRequestMux0SPI1Tx           = 19|0x100U,   /**< SPI1 Transmit */
    kDmaRequestMux0Reserved20       = 20|0x100U,   /**< Reserved20 */
    kDmaRequestMux0Reserved21       = 21|0x100U,   /**< Reserved21 */
    kDmaRequestMux0I2C0             = 22|0x100U,   /**< I2C0 */
    kDmaRequestMux0I2C1             = 23|0x100U,   /**< I2C1 */
    kDmaRequestMux0TMR0Channel0     = 24|0x100U,   /**< TPMCH0 */
    kDmaRequestMux0TMR0Channel1     = 25|0x100U,   /**< TPMCH1 */
    kDmaRequestMux0TMR0Channel2     = 26|0x100U,   /**< TPMCH2 */
    kDmaRequestMux0TMR0Channel3     = 27|0x100U,   /**< TPMCH3 */
    kDmaRequestMux0XBARRequest0     = 28|0x100U,   /**< XBARCH0 */
    kDmaRequestMux0XBARRequest1     = 29|0x100U,   /**< XBARCH1 */
    kDmaRequestMux0XBARRequest2     = 30|0x100U,   /**< XBARCH2 */
    kDmaRequestMux0XBARRequest3     = 31|0x100U,   /**< XBARCH3 */
    kDmaRequestMux0AFEChannel0      = 32|0x100U,   /**< AFECH0 */
    kDmaRequestMux0AFEChannel1      = 33|0x100U,   /**< AFECH1 */
    kDmaRequestMux0AFEChannel2      = 34|0x100U,   /**< AFECH2 */
    kDmaRequestMux0AFEChannel3      = 35|0x100U,   /**< AFECH3 */
    kDmaRequestMux0PortJ            = 36|0x100U,   /**< GPIO Port J */
    kDmaRequestMux0PortK            = 37|0x100U,   /**< GPIO Port K */
    kDmaRequestMux0PortL            = 38|0x100U,   /**< GPIO Port L */
    kDmaRequestMux0PortM            = 39|0x100U,   /**< GPIO Port M */
    kDmaRequestMux0SarADC           = 40|0x100U,   /**< ADC */
    kDmaRequestMux0Reserved41       = 41|0x100U,   /**< Reserved41 */
    kDmaRequestMux0CMP0             = 42|0x100U,   /**< CMP0 */
    kDmaRequestMux0CMP1             = 43|0x100U,   /**< CMP1 */
    kDmaRequestMux0CMP2             = 44|0x100U,   /**< CMP2 */
    kDmaRequestMux0Reserved45       = 45|0x100U,   /**< Reserved45 */
    kDmaRequestMux0Reserved46       = 46|0x100U,   /**< Reserved46 */
    kDmaRequestMux0MMAU             = 47|0x100U,   /**< MMAU */
    kDmaRequestMux0PDB0             = 48|0x100U,   /**< PDB0 */
    kDmaRequestMux0PORTA            = 49|0x100U,   /**< GPIO Port A */
    kDmaRequestMux0PORTB            = 50|0x100U,   /**< GPIO Port B */
    kDmaRequestMux0PORTC            = 51|0x100U,   /**< GPIO Port C */
    kDmaRequestMux0PORTD            = 52|0x100U,   /**< GPIO Port D */
    kDmaRequestMux0PORTE            = 53|0x100U,   /**< GPIO Port E */
    kDmaRequestMux0PORTF            = 54|0x100U,   /**< GPIO Port F */
    kDmaRequestMux0PORTG            = 55|0x100U,   /**< GPIO Port G */
    kDmaRequestMux0PORTH            = 56|0x100U,   /**< GPIO Port H */
    kDmaRequestMux0PORTI            = 57|0x100U,   /**< GPIO Port I */
    kDmaRequestMux0LPUART0Rx        = 58|0x100U,   /**< LPUART0 Receive */
    kDmaRequestMux0LPUART0Tx        = 59|0x100U,   /**< LPUART0 Transmit */
    kDmaRequestMux0AlwaysOn60       = 60|0x100U,   /**< Always enabled. */
    kDmaRequestMux0AlwaysOn61       = 61|0x100U,   /**< Always enabled. */
    kDmaRequestMux0AlwaysOn62       = 62|0x100U,   /**< Always enabled. */
    kDmaRequestMux0AlwaysOn63       = 63|0x100U,   /**< Always enabled. */
} dma_request_source_t;

/* @} */

typedef enum _xbar_input_signal
{
    kXBAR_InputVdd                  = 0|0x100U,    /**< Logic 1 (VDD) output assigned to XBAR_IN0 input. */
    kXBAR_InputVss                  = 1|0x100U,    /**< Logic 0 (VSS) output assigned to XBAR_IN1 input. */
    kXBAR_InputAfeClock             = 2|0x100U,    /**< AFE modulator clock output output assigned to XBAR_IN2 input. */
    kXBAR_InputAfeMod0DataOutput    = 3|0x100U,    /**< AFE modulator 0 data output output assigned to XBAR_IN3 input. */
    kXBAR_InputLptmr0Output         = 4|0x100U,    /**< LPTimer Output output assigned to XBAR_IN4 input. */
    kXBAR_InputClockOutput          = 5|0x100U,    /**< Clock Output output assigned to XBAR_IN5 input. */
    kXBAR_InputTmrCh0Output         = 6|0x100U,    /**< Quad Timer channel 0 output output assigned to XBAR_IN6 input. */
    kXBAR_InputTmrCh1Output         = 7|0x100U,    /**< Quad Timer channel 1 output output assigned to XBAR_IN7 input. */
    kXBAR_InputTmrCh2Output         = 8|0x100U,    /**< Quad Timer channel 2 output output assigned to XBAR_IN8 input. */
    kXBAR_InputTmrCh3Output         = 9|0x100U,    /**< Quad Timer channel 3 output output assigned to XBAR_IN9 input. */
    kXBAR_InputRtcClockOutput       = 10|0x100U,   /**< iRTC Clock Output output assigned to XBAR_IN10 input. */
    kXBAR_InputCmp0Output           = 11|0x100U,   /**< CMP0 Output output assigned to XBAR_IN11 input. */
    kXBAR_InputCmp1Output           = 12|0x100U,   /**< CMP1 Output output assigned to XBAR_IN12 input. */
    kXBAR_InputRtcAlarmOutput       = 13|0x100U,   /**< iRTC Alarm Output output assigned to XBAR_IN13 input. */
    kXBAR_InputUartTxOutput         = 14|0x100U,   /**< UART TX Output (after modulation) output assigned to XBAR_IN14 input. */
    kXBAR_InputEwmOutput            = 15|0x100U,   /**< EWM Output (EWM_OUT) output assigned to XBAR_IN15 input. */
    kXBAR_InputPit0Tif0             = 16|0x100U,   /**< PIT 0 Timer Interrupt Flag 0 (Timeout 0) output assigned to XBAR_IN16 input. */
    kXBAR_InputXbarIn0              = 17|0x100U,   /**< XBAR Input pin 0 output assigned to XBAR_IN17 input. */
    kXBAR_InputXbarIn1              = 18|0x100U,   /**< XBAR Input pin 1 output assigned to XBAR_IN18 input. */
    kXBAR_InputXbarIn2              = 19|0x100U,   /**< XBAR Input pin 2 output assigned to XBAR_IN19 input. */
    kXBAR_InputXbarIn3              = 20|0x100U,   /**< XBAR Input pin 3 output assigned to XBAR_IN20 input. */
    kXBAR_InputXbarIn4              = 21|0x100U,   /**< XBAR Input pin 4 output assigned to XBAR_IN21 input. */
    kXBAR_InputXbarIn5              = 22|0x100U,   /**< XBAR Input pin 5 output assigned to XBAR_IN22 input. */
    kXBAR_InputXbarIn6              = 23|0x100U,   /**< XBAR Input pin 6 output assigned to XBAR_IN23 input. */
    kXBAR_InputXbarIn7              = 24|0x100U,   /**< XBAR Input pin 7 output assigned to XBAR_IN24 input. */
    kXBAR_InputXbarIn8              = 25|0x100U,   /**< XBAR Input pin 8 output assigned to XBAR_IN25 input. */
    kXBAR_InputAdcCocoFlag          = 26|0x100U,   /**< ORed conversion complete flag for all SAR ADC channels output assigned to XBAR_IN26 input. */
    kXBAR_InputAfeAllCocFlag        = 27|0x100U,   /**< ORed conversion complete flag for all AFE channels output assigned to XBAR_IN27 input. */
    kXBAR_InputAfeCh0CocFlag        = 28|0x100U,   /**< AFE Channel 0 conversion complete output assigned to XBAR_IN28 input. */
    kXBAR_InputAfeCh1CocFlag        = 29|0x100U,   /**< AFE Channel 1 conversion complete output assigned to XBAR_IN29 input. */
    kXBAR_InputAfeCh2CocFlag        = 30|0x100U,   /**< AFE Channel 2 conversion complete output assigned to XBAR_IN30 input. */
    kXBAR_InputAfeCh3CocFlag        = 31|0x100U,   /**< AFE Channel 3 conversion complete output assigned to XBAR_IN31 input. */
    kXBAR_InputDmaDoneSignal        = 32|0x100U,   /**< DMA Done Signal output assigned to XBAR_IN32 input. */
    kXBAR_InputXbarIn9              = 33|0x100U,   /**< XBAR Input pin 9 output assigned to XBAR_IN33 input. */
    kXBAR_InputXbarIn10             = 34|0x100U,   /**< XBAR Input pin 10 output assigned to XBAR_IN34 input. */
    kXBAR_InputCmp2Output           = 35|0x100U,   /**< CMP2 Output output assigned to XBAR_IN35 input. */
    kXBAR_InputPit0Tif1             = 36|0x100U,   /**< PIT 0 Timer Interrupt Flag 1 (Timeout 1) output assigned to XBAR_IN36 input. */
    kXBAR_InputPit1Tif0             = 37|0x100U,   /**< PIT 1 Timer Interrupt Flag 0 (Timeout 0) output assigned to XBAR_IN37 input. */
    kXBAR_InputPit1Tif1             = 38|0x100U,   /**< PIT 1 Timer Interrupt Flag 1 (Timeout 1) output assigned to XBAR_IN38 input. */
    kXBAR_InputAfeMod1DataOutput    = 39|0x100U,   /**< AFE modulator 1 data output output assigned to XBAR_IN39 input. */
    kXBAR_InputAfeMod2DataOutput    = 40|0x100U,   /**< AFE modulator 2 data output output assigned to XBAR_IN40 input. */
    kXBAR_InputAfeMod3DataOutput    = 41|0x100U,   /**< AFE modulator 3 data output output assigned to XBAR_IN41 input. */
    kXBAR_InputAdcCocoA             = 42|0x100U,   /**< SAR ADC conversion complete A output assigned to XBAR_IN42 input. */
    kXBAR_InputAdcCocoB             = 43|0x100U,   /**< SAR ADC conversion complete B output assigned to XBAR_IN43 input. */
    kXBAR_InputAdcCocoC             = 44|0x100U,   /**< SAR ADC conversion complete C output assigned to XBAR_IN44 input. */
    kXBAR_InputAdcCocoD             = 45|0x100U,   /**< SAR ADC conversion complete D output assigned to XBAR_IN45 input. */
    kXBAR_InputPdb0Ch0Pretrigger0   = 46|0x100U,   /**< PDB0 CH0 Pre-trigger 0 output assigned to XBAR_IN46 input. */
    kXBAR_InputPdb0Ch0Pretrigger1   = 47|0x100U,   /**< PDB0 CH0 Pre-trigger 1 output assigned to XBAR_IN47 input. */
    kXBAR_InputPdb0Ch0Pretrigger2   = 48|0x100U,   /**< PDB0 CH0 Pre-trigger 2 output assigned to XBAR_IN48 input. */
    kXBAR_InputPdb0Ch0Pretrigger3   = 49|0x100U,   /**< PDB0 CH0 Pre-trigger 3 output assigned to XBAR_IN49 input. */
    kXBAR_InputPdb0Ch0Trigger       = 50|0x100U,   /**< PDB0 CH0 Trigger output assigned to XBAR_IN50 input. */
    kXBAR_InputPdb0PulseOut0        = 51|0x100U,   /**< PDB0 Pulse-Out 0 output assigned to XBAR_IN51 input. */
} xbar_input_signal_t;

typedef enum _xbar_output_signal
{
    kXBAR_OutputXbDmaInt0           = 0|0x100U,    /**< XBAR_OUT0 output assigned to XBAR DMA request or Interrupt 0 */
    kXBAR_OutputXbDmaInt1           = 1|0x100U,    /**< XBAR_OUT1 output assigned to XBAR DMA request or Interrupt 1 */
    kXBAR_OutputXbDmaInt2           = 2|0x100U,    /**< XBAR_OUT2 output assigned to XBAR DMA request or Interrupt 2 */
    kXBAR_OutputXbDmaInt3           = 3|0x100U,    /**< XBAR_OUT3 output assigned to XBAR DMA request or Interrupt 3 */
    kXBAR_OutputCmp2SampleWinInput  = 4|0x100U,    /**< XBAR_OUT4 output assigned to CMP2 Sample Window input */
    kXBAR_OutputTmrCh0SecInput      = 5|0x100U,    /**< XBAR_OUT5 output assigned to Quad Timer channel 0 secondary input */
    kXBAR_OutputTmrCh1SecInput      = 6|0x100U,    /**< XBAR_OUT6 output assigned to Quad Timer channel 1 secondary input */
    kXBAR_OutputTmrCh2SecInput      = 7|0x100U,    /**< XBAR_OUT7 output assigned to Quad Timer channel 2 secondary input */
    kXBAR_OutputTmrCh3SecInput      = 8|0x100U,    /**< XBAR_OUT8 output assigned to Quad Timer channel 3 secondary input */
    kXBAR_OutputTmrClockInput1      = 9|0x100U,    /**< XBAR_OUT9 output assigned to Quad Timer primary clock input 1 */
    kXBAR_OutputTmrClockInput2      = 10|0x100U,   /**< XBAR_OUT10 output assigned to Quad Timer primary clock input 2 */
    kXBAR_OutputCmp0SampleWinInput  = 11|0x100U,   /**< XBAR_OUT11 output assigned to CMP0 Sample Window input */
    kXBAR_OutputCmp1SampleWinInput  = 12|0x100U,   /**< XBAR_OUT12 output assigned to CMP1 Sample Window input */
    kXBAR_OutputUartRxInput         = 13|0x100U,   /**< XBAR_OUT13 output assigned to UART Rx IrDA Input */
    kXBAR_OutputUartTxModCarrier    = 14|0x100U,   /**< XBAR_OUT14 output assigned to SIM UART Tx IrDA Modulator Carrier Input */
    kXBAR_OutputAdcTrgA             = 15|0x100U,   /**< XBAR_OUT15 output assigned to SAR ADC trigger select A pulse */
    kXBAR_OutputAdcTrgB             = 16|0x100U,   /**< XBAR_OUT16 output assigned to SAR ADC trigger select B pulse */
    kXBAR_OutputXbOut0              = 17|0x100U,   /**< XBAR_OUT17 output assigned to XBAR Output pin 0 */
    kXBAR_OutputXbOut1              = 18|0x100U,   /**< XBAR_OUT18 output assigned to XBAR Output pin 1 */
    kXBAR_OutputXbOut2              = 19|0x100U,   /**< XBAR_OUT19 output assigned to XBAR Output pin 2 */
    kXBAR_OutputXbOut3              = 20|0x100U,   /**< XBAR_OUT20 output assigned to XBAR Output pin 3 */
    kXBAR_OutputXbOut4              = 21|0x100U,   /**< XBAR_OUT21 output assigned to XBAR Output pin 4 */
    kXBAR_OutputXbOut5              = 22|0x100U,   /**< XBAR_OUT22 output assigned to XBAR Output pin 5 */
    kXBAR_OutputXbOut6              = 23|0x100U,   /**< XBAR_OUT23 output assigned to XBAR Output pin 6 */
    kXBAR_OutputXbOut7              = 24|0x100U,   /**< XBAR_OUT24 output assigned to XBAR Output pin 7 */
    kXBAR_OutputXbOut8              = 25|0x100U,   /**< XBAR_OUT25 output assigned to XBAR Output pin 8 */
    kXBAR_OutputAdcTrgC             = 26|0x100U,   /**< XBAR_OUT26 output assigned to SAR ADC trigger select C pulse */
    kXBAR_OutputAdcTrgD             = 27|0x100U,   /**< XBAR_OUT27 output assigned to SAR ADC trigger select D pulse */
    kXBAR_OutputAfeCh0Trg           = 28|0x100U,   /**< XBAR_OUT28 output assigned to AFE Channel 0 Trigger */
    kXBAR_OutputAfeCh1Trg           = 29|0x100U,   /**< XBAR_OUT29 output assigned to AFE Channel 1 Trigger */
    kXBAR_OutputAfeCh2Trg           = 30|0x100U,   /**< XBAR_OUT30 output assigned to AFE Channel 2 Trigger */
    kXBAR_OutputAfeCh3Trg           = 31|0x100U,   /**< XBAR_OUT31 output assigned to AFE Channel 3 Trigger */
    kXBAR_OutputEwmIn               = 32|0x100U,   /**< XBAR_OUT32 output assigned to EWM input (EWM_IN) */
    kXBAR_OutputXbOut9              = 33|0x100U,   /**< XBAR_OUT33 output assigned to XBAR Output pin 9 */
    kXBAR_OutputXbOut10             = 34|0x100U,   /**< XBAR_OUT34 output assigned to XBAR Output pin 10 */
    kXBAR_OutputPdb0PreTrigger0Ack  = 35|0x100U,   /**< XBAR_OUT35 output assigned to PDB0 Pre-trigger0 B2B Ack */
    kXBAR_OutputPdb0PreTrigger1Ack  = 36|0x100U,   /**< XBAR_OUT36 output assigned to PDB0 Pre-trigger1 B2B Ack */
    kXBAR_OutputPdb0PreTrigger2Ack  = 37|0x100U,   /**< XBAR_OUT37 output assigned to PDB0 Pre-trigger2 B2B Ack */
    kXBAR_OutputPdb0PreTrigger3Ack  = 38|0x100U,   /**< XBAR_OUT38 output assigned to PDB0 Pre-trigger3 B2B Ack */
    kXBAR_OutputPdb0TriggerInput    = 39|0x100U,   /**< XBAR_OUT39 output assigned to PDB0 Trigger Input */
    kXBAR_OutputAfeCh0ExmodData     = 40|0x100U,   /**< XBAR_OUT40 output assigned to External modulator data input for AFE Channel 0 */
    kXBAR_OutputAfeCh1ExmodData     = 41|0x100U,   /**< XBAR_OUT41 output assigned to External modulator data input for AFE Channel 1 */
    kXBAR_OutputAfeCh2ExmodData     = 42|0x100U,   /**< XBAR_OUT42 output assigned to External modulator data input for AFE Channel 2 */
    kXBAR_OutputAfeCh3ExmodData     = 43|0x100U,   /**< XBAR_OUT43 output assigned to External modulator data input for AFE Channel 3 */
} xbar_output_signal_t;


/*!
 * @}
 */ /* end of group Mapping_Information */


/* ----------------------------------------------------------------------------
   -- Device Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Peripheral_access_layer Device Peripheral Access Layer
 * @{
 */


/*
** Start of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma push
  #pragma anon_unions
#elif defined(__CWCC__)
  #pragma push
  #pragma cpp_extensions on
#elif defined(__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=extended
#else
  #error Not supported compiler type
#endif

/* ----------------------------------------------------------------------------
   -- ADC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Peripheral_Access_Layer ADC Peripheral Access Layer
 * @{
 */

/** ADC - Register Layout Typedef */
typedef struct {
  __IO uint32_t SC1[4];                            /**< ADC Status and Control Registers 1, array offset: 0x0, array step: 0x4 */
  __IO uint32_t CFG1;                              /**< ADC Configuration Register 1, offset: 0x10 */
  __IO uint32_t CFG2;                              /**< ADC Configuration Register 2, offset: 0x14 */
  __I  uint32_t R[4];                              /**< ADC Data Result Register, array offset: 0x18, array step: 0x4 */
  __IO uint32_t CV1;                               /**< Compare Value Registers, offset: 0x28 */
  __IO uint32_t CV2;                               /**< Compare Value Registers, offset: 0x2C */
  __IO uint32_t SC2;                               /**< Status and Control Register 2, offset: 0x30 */
  __IO uint32_t SC3;                               /**< Status and Control Register 3, offset: 0x34 */
  __IO uint32_t OFS;                               /**< ADC Offset Correction Register, offset: 0x38 */
  __IO uint32_t PG;                                /**< ADC Plus-Side Gain Register, offset: 0x3C */
       uint8_t RESERVED_0[4];
  __IO uint32_t CLPD;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x44 */
  __IO uint32_t CLPS;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x48 */
  __IO uint32_t CLP4;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x4C */
  __IO uint32_t CLP3;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x50 */
  __IO uint32_t CLP2;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x54 */
  __IO uint32_t CLP1;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x58 */
  __IO uint32_t CLP0;                              /**< ADC Plus-Side General Calibration Value Register, offset: 0x5C */
} ADC_Type;

/* ----------------------------------------------------------------------------
   -- ADC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ADC_Register_Masks ADC Register Masks
 * @{
 */

/*! @name SC1 - ADC Status and Control Registers 1 */
#define ADC_SC1_ADCH_MASK                        (0x1FU)
#define ADC_SC1_ADCH_SHIFT                       (0U)
#define ADC_SC1_ADCH(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC1_ADCH_SHIFT)) & ADC_SC1_ADCH_MASK)
#define ADC_SC1_AIEN_MASK                        (0x40U)
#define ADC_SC1_AIEN_SHIFT                       (6U)
#define ADC_SC1_AIEN(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC1_AIEN_SHIFT)) & ADC_SC1_AIEN_MASK)
#define ADC_SC1_COCO_MASK                        (0x80U)
#define ADC_SC1_COCO_SHIFT                       (7U)
#define ADC_SC1_COCO(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC1_COCO_SHIFT)) & ADC_SC1_COCO_MASK)

/* The count of ADC_SC1 */
#define ADC_SC1_COUNT                            (4U)

/*! @name CFG1 - ADC Configuration Register 1 */
#define ADC_CFG1_ADICLK_MASK                     (0x3U)
#define ADC_CFG1_ADICLK_SHIFT                    (0U)
#define ADC_CFG1_ADICLK(x)                       (((uint32_t)(((uint32_t)(x)) << ADC_CFG1_ADICLK_SHIFT)) & ADC_CFG1_ADICLK_MASK)
#define ADC_CFG1_MODE_MASK                       (0xCU)
#define ADC_CFG1_MODE_SHIFT                      (2U)
#define ADC_CFG1_MODE(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CFG1_MODE_SHIFT)) & ADC_CFG1_MODE_MASK)
#define ADC_CFG1_ADLSMP_MASK                     (0x10U)
#define ADC_CFG1_ADLSMP_SHIFT                    (4U)
#define ADC_CFG1_ADLSMP(x)                       (((uint32_t)(((uint32_t)(x)) << ADC_CFG1_ADLSMP_SHIFT)) & ADC_CFG1_ADLSMP_MASK)
#define ADC_CFG1_ADIV_MASK                       (0x60U)
#define ADC_CFG1_ADIV_SHIFT                      (5U)
#define ADC_CFG1_ADIV(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CFG1_ADIV_SHIFT)) & ADC_CFG1_ADIV_MASK)
#define ADC_CFG1_ADLPC_MASK                      (0x80U)
#define ADC_CFG1_ADLPC_SHIFT                     (7U)
#define ADC_CFG1_ADLPC(x)                        (((uint32_t)(((uint32_t)(x)) << ADC_CFG1_ADLPC_SHIFT)) & ADC_CFG1_ADLPC_MASK)

/*! @name CFG2 - ADC Configuration Register 2 */
#define ADC_CFG2_ADLSTS_MASK                     (0x3U)
#define ADC_CFG2_ADLSTS_SHIFT                    (0U)
#define ADC_CFG2_ADLSTS(x)                       (((uint32_t)(((uint32_t)(x)) << ADC_CFG2_ADLSTS_SHIFT)) & ADC_CFG2_ADLSTS_MASK)
#define ADC_CFG2_ADHSC_MASK                      (0x4U)
#define ADC_CFG2_ADHSC_SHIFT                     (2U)
#define ADC_CFG2_ADHSC(x)                        (((uint32_t)(((uint32_t)(x)) << ADC_CFG2_ADHSC_SHIFT)) & ADC_CFG2_ADHSC_MASK)
#define ADC_CFG2_ADACKEN_MASK                    (0x8U)
#define ADC_CFG2_ADACKEN_SHIFT                   (3U)
#define ADC_CFG2_ADACKEN(x)                      (((uint32_t)(((uint32_t)(x)) << ADC_CFG2_ADACKEN_SHIFT)) & ADC_CFG2_ADACKEN_MASK)
#define ADC_CFG2_MUXSEL_MASK                     (0x10U)
#define ADC_CFG2_MUXSEL_SHIFT                    (4U)
#define ADC_CFG2_MUXSEL(x)                       (((uint32_t)(((uint32_t)(x)) << ADC_CFG2_MUXSEL_SHIFT)) & ADC_CFG2_MUXSEL_MASK)

/*! @name R - ADC Data Result Register */
#define ADC_R_D_MASK                             (0xFFFFU)
#define ADC_R_D_SHIFT                            (0U)
#define ADC_R_D(x)                               (((uint32_t)(((uint32_t)(x)) << ADC_R_D_SHIFT)) & ADC_R_D_MASK)

/* The count of ADC_R */
#define ADC_R_COUNT                              (4U)

/*! @name CV1 - Compare Value Registers */
#define ADC_CV1_CV_MASK                          (0xFFFFU)
#define ADC_CV1_CV_SHIFT                         (0U)
#define ADC_CV1_CV(x)                            (((uint32_t)(((uint32_t)(x)) << ADC_CV1_CV_SHIFT)) & ADC_CV1_CV_MASK)

/*! @name CV2 - Compare Value Registers */
#define ADC_CV2_CV_MASK                          (0xFFFFU)
#define ADC_CV2_CV_SHIFT                         (0U)
#define ADC_CV2_CV(x)                            (((uint32_t)(((uint32_t)(x)) << ADC_CV2_CV_SHIFT)) & ADC_CV2_CV_MASK)

/*! @name SC2 - Status and Control Register 2 */
#define ADC_SC2_REFSEL_MASK                      (0x3U)
#define ADC_SC2_REFSEL_SHIFT                     (0U)
#define ADC_SC2_REFSEL(x)                        (((uint32_t)(((uint32_t)(x)) << ADC_SC2_REFSEL_SHIFT)) & ADC_SC2_REFSEL_MASK)
#define ADC_SC2_DMAEN_MASK                       (0x4U)
#define ADC_SC2_DMAEN_SHIFT                      (2U)
#define ADC_SC2_DMAEN(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_SC2_DMAEN_SHIFT)) & ADC_SC2_DMAEN_MASK)
#define ADC_SC2_ACREN_MASK                       (0x8U)
#define ADC_SC2_ACREN_SHIFT                      (3U)
#define ADC_SC2_ACREN(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_SC2_ACREN_SHIFT)) & ADC_SC2_ACREN_MASK)
#define ADC_SC2_ACFGT_MASK                       (0x10U)
#define ADC_SC2_ACFGT_SHIFT                      (4U)
#define ADC_SC2_ACFGT(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_SC2_ACFGT_SHIFT)) & ADC_SC2_ACFGT_MASK)
#define ADC_SC2_ACFE_MASK                        (0x20U)
#define ADC_SC2_ACFE_SHIFT                       (5U)
#define ADC_SC2_ACFE(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC2_ACFE_SHIFT)) & ADC_SC2_ACFE_MASK)
#define ADC_SC2_ADTRG_MASK                       (0x40U)
#define ADC_SC2_ADTRG_SHIFT                      (6U)
#define ADC_SC2_ADTRG(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_SC2_ADTRG_SHIFT)) & ADC_SC2_ADTRG_MASK)
#define ADC_SC2_ADACT_MASK                       (0x80U)
#define ADC_SC2_ADACT_SHIFT                      (7U)
#define ADC_SC2_ADACT(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_SC2_ADACT_SHIFT)) & ADC_SC2_ADACT_MASK)

/*! @name SC3 - Status and Control Register 3 */
#define ADC_SC3_AVGS_MASK                        (0x3U)
#define ADC_SC3_AVGS_SHIFT                       (0U)
#define ADC_SC3_AVGS(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC3_AVGS_SHIFT)) & ADC_SC3_AVGS_MASK)
#define ADC_SC3_AVGE_MASK                        (0x4U)
#define ADC_SC3_AVGE_SHIFT                       (2U)
#define ADC_SC3_AVGE(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC3_AVGE_SHIFT)) & ADC_SC3_AVGE_MASK)
#define ADC_SC3_ADCO_MASK                        (0x8U)
#define ADC_SC3_ADCO_SHIFT                       (3U)
#define ADC_SC3_ADCO(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC3_ADCO_SHIFT)) & ADC_SC3_ADCO_MASK)
#define ADC_SC3_CALF_MASK                        (0x40U)
#define ADC_SC3_CALF_SHIFT                       (6U)
#define ADC_SC3_CALF(x)                          (((uint32_t)(((uint32_t)(x)) << ADC_SC3_CALF_SHIFT)) & ADC_SC3_CALF_MASK)
#define ADC_SC3_CAL_MASK                         (0x80U)
#define ADC_SC3_CAL_SHIFT                        (7U)
#define ADC_SC3_CAL(x)                           (((uint32_t)(((uint32_t)(x)) << ADC_SC3_CAL_SHIFT)) & ADC_SC3_CAL_MASK)

/*! @name OFS - ADC Offset Correction Register */
#define ADC_OFS_OFS_MASK                         (0xFFFFU)
#define ADC_OFS_OFS_SHIFT                        (0U)
#define ADC_OFS_OFS(x)                           (((uint32_t)(((uint32_t)(x)) << ADC_OFS_OFS_SHIFT)) & ADC_OFS_OFS_MASK)

/*! @name PG - ADC Plus-Side Gain Register */
#define ADC_PG_PG_MASK                           (0xFFFFU)
#define ADC_PG_PG_SHIFT                          (0U)
#define ADC_PG_PG(x)                             (((uint32_t)(((uint32_t)(x)) << ADC_PG_PG_SHIFT)) & ADC_PG_PG_MASK)

/*! @name CLPD - ADC Plus-Side General Calibration Value Register */
#define ADC_CLPD_CLPD_MASK                       (0x3FU)
#define ADC_CLPD_CLPD_SHIFT                      (0U)
#define ADC_CLPD_CLPD(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLPD_CLPD_SHIFT)) & ADC_CLPD_CLPD_MASK)

/*! @name CLPS - ADC Plus-Side General Calibration Value Register */
#define ADC_CLPS_CLPS_MASK                       (0x3FU)
#define ADC_CLPS_CLPS_SHIFT                      (0U)
#define ADC_CLPS_CLPS(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLPS_CLPS_SHIFT)) & ADC_CLPS_CLPS_MASK)

/*! @name CLP4 - ADC Plus-Side General Calibration Value Register */
#define ADC_CLP4_CLP4_MASK                       (0x3FFU)
#define ADC_CLP4_CLP4_SHIFT                      (0U)
#define ADC_CLP4_CLP4(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLP4_CLP4_SHIFT)) & ADC_CLP4_CLP4_MASK)

/*! @name CLP3 - ADC Plus-Side General Calibration Value Register */
#define ADC_CLP3_CLP3_MASK                       (0x1FFU)
#define ADC_CLP3_CLP3_SHIFT                      (0U)
#define ADC_CLP3_CLP3(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLP3_CLP3_SHIFT)) & ADC_CLP3_CLP3_MASK)

/*! @name CLP2 - ADC Plus-Side General Calibration Value Register */
#define ADC_CLP2_CLP2_MASK                       (0xFFU)
#define ADC_CLP2_CLP2_SHIFT                      (0U)
#define ADC_CLP2_CLP2(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLP2_CLP2_SHIFT)) & ADC_CLP2_CLP2_MASK)

/*! @name CLP1 - ADC Plus-Side General Calibration Value Register */
#define ADC_CLP1_CLP1_MASK                       (0x7FU)
#define ADC_CLP1_CLP1_SHIFT                      (0U)
#define ADC_CLP1_CLP1(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLP1_CLP1_SHIFT)) & ADC_CLP1_CLP1_MASK)

/*! @name CLP0 - ADC Plus-Side General Calibration Value Register */
#define ADC_CLP0_CLP0_MASK                       (0x3FU)
#define ADC_CLP0_CLP0_SHIFT                      (0U)
#define ADC_CLP0_CLP0(x)                         (((uint32_t)(((uint32_t)(x)) << ADC_CLP0_CLP0_SHIFT)) & ADC_CLP0_CLP0_MASK)


/*!
 * @}
 */ /* end of group ADC_Register_Masks */


/* ADC - Peripheral instance base addresses */
/** Peripheral ADC0 base address */
#define ADC0_BASE                                (0x4002B000u)
/** Peripheral ADC0 base pointer */
#define ADC0                                     ((ADC_Type *)ADC0_BASE)
/** Array initializer of ADC peripheral base addresses */
#define ADC_BASE_ADDRS                           { ADC0_BASE }
/** Array initializer of ADC peripheral base pointers */
#define ADC_BASE_PTRS                            { ADC0 }
/** Interrupt vectors for the ADC peripheral type */
#define ADC_IRQS                                 { ADC0_IRQn }

/*!
 * @}
 */ /* end of group ADC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- AFE Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AFE_Peripheral_Access_Layer AFE Peripheral Access Layer
 * @{
 */

/** AFE - Register Layout Typedef */
typedef struct {
  __IO uint32_t CFR[4];                            /**< Channel0 Configuration Register..Channel3 Configuration Register, array offset: 0x0, array step: 0x4 */
       uint8_t RESERVED_0[8];
  __IO uint32_t CR;                                /**< Control Register, offset: 0x18 */
  __IO uint32_t CKR;                               /**< Clock Configuration Register, offset: 0x1C */
  __IO uint32_t DI;                                /**< DMA and Interrupt Register, offset: 0x20 */
       uint8_t RESERVED_1[8];
  __IO uint32_t DR[4];                             /**< Channel0 Delay Register..Channel3 Delay Register, array offset: 0x2C, array step: 0x4 */
       uint8_t RESERVED_2[8];
  __I  uint32_t RR[4];                             /**< Channel0 Result Register..Channel3 Result Register, array offset: 0x44, array step: 0x4 */
       uint8_t RESERVED_3[8];
  __I  uint32_t SR;                                /**< Status Register, offset: 0x5C */
} AFE_Type;

/* ----------------------------------------------------------------------------
   -- AFE Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AFE_Register_Masks AFE Register Masks
 * @{
 */

/*! @name CFR - Channel0 Configuration Register..Channel3 Configuration Register */
#define AFE_CFR_HW_TRG_MASK                      (0x200U)
#define AFE_CFR_HW_TRG_SHIFT                     (9U)
#define AFE_CFR_HW_TRG(x)                        (((uint32_t)(((uint32_t)(x)) << AFE_CFR_HW_TRG_SHIFT)) & AFE_CFR_HW_TRG_MASK)
#define AFE_CFR_DEC_CLK_INP_SEL_MASK             (0x400U)
#define AFE_CFR_DEC_CLK_INP_SEL_SHIFT            (10U)
#define AFE_CFR_DEC_CLK_INP_SEL(x)               (((uint32_t)(((uint32_t)(x)) << AFE_CFR_DEC_CLK_INP_SEL_SHIFT)) & AFE_CFR_DEC_CLK_INP_SEL_MASK)
#define AFE_CFR_DEC_CLK_EDGE_SEL_MASK            (0x800U)
#define AFE_CFR_DEC_CLK_EDGE_SEL_SHIFT           (11U)
#define AFE_CFR_DEC_CLK_EDGE_SEL(x)              (((uint32_t)(((uint32_t)(x)) << AFE_CFR_DEC_CLK_EDGE_SEL_SHIFT)) & AFE_CFR_DEC_CLK_EDGE_SEL_MASK)
#define AFE_CFR_CC_MASK                          (0x1000U)
#define AFE_CFR_CC_SHIFT                         (12U)
#define AFE_CFR_CC(x)                            (((uint32_t)(((uint32_t)(x)) << AFE_CFR_CC_SHIFT)) & AFE_CFR_CC_MASK)
#define AFE_CFR_DEC_EN_MASK                      (0x2000U)
#define AFE_CFR_DEC_EN_SHIFT                     (13U)
#define AFE_CFR_DEC_EN(x)                        (((uint32_t)(((uint32_t)(x)) << AFE_CFR_DEC_EN_SHIFT)) & AFE_CFR_DEC_EN_MASK)
#define AFE_CFR_SD_MOD_EN_MASK                   (0x4000U)
#define AFE_CFR_SD_MOD_EN_SHIFT                  (14U)
#define AFE_CFR_SD_MOD_EN(x)                     (((uint32_t)(((uint32_t)(x)) << AFE_CFR_SD_MOD_EN_SHIFT)) & AFE_CFR_SD_MOD_EN_MASK)
#define AFE_CFR_BYP_MODE_MASK                    (0x20000U)
#define AFE_CFR_BYP_MODE_SHIFT                   (17U)
#define AFE_CFR_BYP_MODE(x)                      (((uint32_t)(((uint32_t)(x)) << AFE_CFR_BYP_MODE_SHIFT)) & AFE_CFR_BYP_MODE_MASK)
#define AFE_CFR_PGA_GAIN_SEL_MASK                (0x380000U)
#define AFE_CFR_PGA_GAIN_SEL_SHIFT               (19U)
#define AFE_CFR_PGA_GAIN_SEL(x)                  (((uint32_t)(((uint32_t)(x)) << AFE_CFR_PGA_GAIN_SEL_SHIFT)) & AFE_CFR_PGA_GAIN_SEL_MASK)
#define AFE_CFR_PGA_EN_MASK                      (0x1000000U)
#define AFE_CFR_PGA_EN_SHIFT                     (24U)
#define AFE_CFR_PGA_EN(x)                        (((uint32_t)(((uint32_t)(x)) << AFE_CFR_PGA_EN_SHIFT)) & AFE_CFR_PGA_EN_MASK)
#define AFE_CFR_DEC_OSR_MASK                     (0xE0000000U)
#define AFE_CFR_DEC_OSR_SHIFT                    (29U)
#define AFE_CFR_DEC_OSR(x)                       (((uint32_t)(((uint32_t)(x)) << AFE_CFR_DEC_OSR_SHIFT)) & AFE_CFR_DEC_OSR_MASK)

/* The count of AFE_CFR */
#define AFE_CFR_COUNT                            (4U)

/*! @name CR - Control Register */
#define AFE_CR_STRTUP_CNT_MASK                   (0xFE00U)
#define AFE_CR_STRTUP_CNT_SHIFT                  (9U)
#define AFE_CR_STRTUP_CNT(x)                     (((uint32_t)(((uint32_t)(x)) << AFE_CR_STRTUP_CNT_SHIFT)) & AFE_CR_STRTUP_CNT_MASK)
#define AFE_CR_RESULT_FORMAT_MASK                (0x40000U)
#define AFE_CR_RESULT_FORMAT_SHIFT               (18U)
#define AFE_CR_RESULT_FORMAT(x)                  (((uint32_t)(((uint32_t)(x)) << AFE_CR_RESULT_FORMAT_SHIFT)) & AFE_CR_RESULT_FORMAT_MASK)
#define AFE_CR_DLY_OK_MASK                       (0x200000U)
#define AFE_CR_DLY_OK_SHIFT                      (21U)
#define AFE_CR_DLY_OK(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_CR_DLY_OK_SHIFT)) & AFE_CR_DLY_OK_MASK)
#define AFE_CR_RST_B_MASK                        (0x400000U)
#define AFE_CR_RST_B_SHIFT                       (22U)
#define AFE_CR_RST_B(x)                          (((uint32_t)(((uint32_t)(x)) << AFE_CR_RST_B_SHIFT)) & AFE_CR_RST_B_MASK)
#define AFE_CR_LPM_EN_MASK                       (0x2000000U)
#define AFE_CR_LPM_EN_SHIFT                      (25U)
#define AFE_CR_LPM_EN(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_CR_LPM_EN_SHIFT)) & AFE_CR_LPM_EN_MASK)
#define AFE_CR_SOFT_TRG3_MASK                    (0x8000000U)
#define AFE_CR_SOFT_TRG3_SHIFT                   (27U)
#define AFE_CR_SOFT_TRG3(x)                      (((uint32_t)(((uint32_t)(x)) << AFE_CR_SOFT_TRG3_SHIFT)) & AFE_CR_SOFT_TRG3_MASK)
#define AFE_CR_SOFT_TRG2_MASK                    (0x10000000U)
#define AFE_CR_SOFT_TRG2_SHIFT                   (28U)
#define AFE_CR_SOFT_TRG2(x)                      (((uint32_t)(((uint32_t)(x)) << AFE_CR_SOFT_TRG2_SHIFT)) & AFE_CR_SOFT_TRG2_MASK)
#define AFE_CR_SOFT_TRG1_MASK                    (0x20000000U)
#define AFE_CR_SOFT_TRG1_SHIFT                   (29U)
#define AFE_CR_SOFT_TRG1(x)                      (((uint32_t)(((uint32_t)(x)) << AFE_CR_SOFT_TRG1_SHIFT)) & AFE_CR_SOFT_TRG1_MASK)
#define AFE_CR_SOFT_TRG0_MASK                    (0x40000000U)
#define AFE_CR_SOFT_TRG0_SHIFT                   (30U)
#define AFE_CR_SOFT_TRG0(x)                      (((uint32_t)(((uint32_t)(x)) << AFE_CR_SOFT_TRG0_SHIFT)) & AFE_CR_SOFT_TRG0_MASK)
#define AFE_CR_MSTR_EN_MASK                      (0x80000000U)
#define AFE_CR_MSTR_EN_SHIFT                     (31U)
#define AFE_CR_MSTR_EN(x)                        (((uint32_t)(((uint32_t)(x)) << AFE_CR_MSTR_EN_SHIFT)) & AFE_CR_MSTR_EN_MASK)

/*! @name CKR - Clock Configuration Register */
#define AFE_CKR_CLS_MASK                         (0x600000U)
#define AFE_CKR_CLS_SHIFT                        (21U)
#define AFE_CKR_CLS(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_CKR_CLS_SHIFT)) & AFE_CKR_CLS_MASK)
#define AFE_CKR_DIV_MASK                         (0xF0000000U)
#define AFE_CKR_DIV_SHIFT                        (28U)
#define AFE_CKR_DIV(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_CKR_DIV_SHIFT)) & AFE_CKR_DIV_MASK)

/*! @name DI - DMA and Interrupt Register */
#define AFE_DI_INTEN3_MASK                       (0x800000U)
#define AFE_DI_INTEN3_SHIFT                      (23U)
#define AFE_DI_INTEN3(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_INTEN3_SHIFT)) & AFE_DI_INTEN3_MASK)
#define AFE_DI_INTEN2_MASK                       (0x1000000U)
#define AFE_DI_INTEN2_SHIFT                      (24U)
#define AFE_DI_INTEN2(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_INTEN2_SHIFT)) & AFE_DI_INTEN2_MASK)
#define AFE_DI_INTEN1_MASK                       (0x2000000U)
#define AFE_DI_INTEN1_SHIFT                      (25U)
#define AFE_DI_INTEN1(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_INTEN1_SHIFT)) & AFE_DI_INTEN1_MASK)
#define AFE_DI_INTEN0_MASK                       (0x4000000U)
#define AFE_DI_INTEN0_SHIFT                      (26U)
#define AFE_DI_INTEN0(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_INTEN0_SHIFT)) & AFE_DI_INTEN0_MASK)
#define AFE_DI_DMAEN3_MASK                       (0x10000000U)
#define AFE_DI_DMAEN3_SHIFT                      (28U)
#define AFE_DI_DMAEN3(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_DMAEN3_SHIFT)) & AFE_DI_DMAEN3_MASK)
#define AFE_DI_DMAEN2_MASK                       (0x20000000U)
#define AFE_DI_DMAEN2_SHIFT                      (29U)
#define AFE_DI_DMAEN2(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_DMAEN2_SHIFT)) & AFE_DI_DMAEN2_MASK)
#define AFE_DI_DMAEN1_MASK                       (0x40000000U)
#define AFE_DI_DMAEN1_SHIFT                      (30U)
#define AFE_DI_DMAEN1(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_DMAEN1_SHIFT)) & AFE_DI_DMAEN1_MASK)
#define AFE_DI_DMAEN0_MASK                       (0x80000000U)
#define AFE_DI_DMAEN0_SHIFT                      (31U)
#define AFE_DI_DMAEN0(x)                         (((uint32_t)(((uint32_t)(x)) << AFE_DI_DMAEN0_SHIFT)) & AFE_DI_DMAEN0_MASK)

/*! @name DR - Channel0 Delay Register..Channel3 Delay Register */
#define AFE_DR_DLY_MASK                          (0x7FFU)
#define AFE_DR_DLY_SHIFT                         (0U)
#define AFE_DR_DLY(x)                            (((uint32_t)(((uint32_t)(x)) << AFE_DR_DLY_SHIFT)) & AFE_DR_DLY_MASK)

/* The count of AFE_DR */
#define AFE_DR_COUNT                             (4U)

/*! @name RR - Channel0 Result Register..Channel3 Result Register */
#define AFE_RR_SDR_MASK                          (0x7FFFFFU)
#define AFE_RR_SDR_SHIFT                         (0U)
#define AFE_RR_SDR(x)                            (((uint32_t)(((uint32_t)(x)) << AFE_RR_SDR_SHIFT)) & AFE_RR_SDR_MASK)
#define AFE_RR_SIGN_BITS_MASK                    (0xFF800000U)
#define AFE_RR_SIGN_BITS_SHIFT                   (23U)
#define AFE_RR_SIGN_BITS(x)                      (((uint32_t)(((uint32_t)(x)) << AFE_RR_SIGN_BITS_SHIFT)) & AFE_RR_SIGN_BITS_MASK)

/* The count of AFE_RR */
#define AFE_RR_COUNT                             (4U)

/*! @name SR - Status Register */
#define AFE_SR_RDY3_MASK                         (0x10000U)
#define AFE_SR_RDY3_SHIFT                        (16U)
#define AFE_SR_RDY3(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_RDY3_SHIFT)) & AFE_SR_RDY3_MASK)
#define AFE_SR_RDY2_MASK                         (0x20000U)
#define AFE_SR_RDY2_SHIFT                        (17U)
#define AFE_SR_RDY2(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_RDY2_SHIFT)) & AFE_SR_RDY2_MASK)
#define AFE_SR_RDY1_MASK                         (0x40000U)
#define AFE_SR_RDY1_SHIFT                        (18U)
#define AFE_SR_RDY1(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_RDY1_SHIFT)) & AFE_SR_RDY1_MASK)
#define AFE_SR_RDY0_MASK                         (0x80000U)
#define AFE_SR_RDY0_SHIFT                        (19U)
#define AFE_SR_RDY0(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_RDY0_SHIFT)) & AFE_SR_RDY0_MASK)
#define AFE_SR_OVR3_MASK                         (0x200000U)
#define AFE_SR_OVR3_SHIFT                        (21U)
#define AFE_SR_OVR3(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_OVR3_SHIFT)) & AFE_SR_OVR3_MASK)
#define AFE_SR_OVR2_MASK                         (0x400000U)
#define AFE_SR_OVR2_SHIFT                        (22U)
#define AFE_SR_OVR2(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_OVR2_SHIFT)) & AFE_SR_OVR2_MASK)
#define AFE_SR_OVR1_MASK                         (0x800000U)
#define AFE_SR_OVR1_SHIFT                        (23U)
#define AFE_SR_OVR1(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_OVR1_SHIFT)) & AFE_SR_OVR1_MASK)
#define AFE_SR_OVR0_MASK                         (0x1000000U)
#define AFE_SR_OVR0_SHIFT                        (24U)
#define AFE_SR_OVR0(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_OVR0_SHIFT)) & AFE_SR_OVR0_MASK)
#define AFE_SR_COC3_MASK                         (0x10000000U)
#define AFE_SR_COC3_SHIFT                        (28U)
#define AFE_SR_COC3(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_COC3_SHIFT)) & AFE_SR_COC3_MASK)
#define AFE_SR_COC2_MASK                         (0x20000000U)
#define AFE_SR_COC2_SHIFT                        (29U)
#define AFE_SR_COC2(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_COC2_SHIFT)) & AFE_SR_COC2_MASK)
#define AFE_SR_COC1_MASK                         (0x40000000U)
#define AFE_SR_COC1_SHIFT                        (30U)
#define AFE_SR_COC1(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_COC1_SHIFT)) & AFE_SR_COC1_MASK)
#define AFE_SR_COC0_MASK                         (0x80000000U)
#define AFE_SR_COC0_SHIFT                        (31U)
#define AFE_SR_COC0(x)                           (((uint32_t)(((uint32_t)(x)) << AFE_SR_COC0_SHIFT)) & AFE_SR_COC0_MASK)


/*!
 * @}
 */ /* end of group AFE_Register_Masks */


/* AFE - Peripheral instance base addresses */
/** Peripheral AFE base address */
#define AFE_BASE                                 (0x40030000u)
/** Peripheral AFE base pointer */
#define AFE                                      ((AFE_Type *)AFE_BASE)
/** Array initializer of AFE peripheral base addresses */
#define AFE_BASE_ADDRS                           { AFE_BASE }
/** Array initializer of AFE peripheral base pointers */
#define AFE_BASE_PTRS                            { AFE }
/** Interrupt vectors for the AFE peripheral type */
#define AFE_IRQS                                 { { AFE_CH0_IRQn, AFE_CH1_IRQn, AFE_CH2_IRQn, AFE_CH3_IRQn } }

/*!
 * @}
 */ /* end of group AFE_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- AIPS Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AIPS_Peripheral_Access_Layer AIPS Peripheral Access Layer
 * @{
 */

/** AIPS - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[32];
  __IO uint32_t PACRA;                             /**< Peripheral Access Control Register, offset: 0x20 */
  __IO uint32_t PACRB;                             /**< Peripheral Access Control Register, offset: 0x24 */
       uint8_t RESERVED_1[24];
  __IO uint32_t PACRE;                             /**< Peripheral Access Control Register, offset: 0x40 */
  __IO uint32_t PACRF;                             /**< Peripheral Access Control Register, offset: 0x44 */
  __IO uint32_t PACRG;                             /**< Peripheral Access Control Register, offset: 0x48 */
  __IO uint32_t PACRH;                             /**< Peripheral Access Control Register, offset: 0x4C */
  __IO uint32_t PACRI;                             /**< Peripheral Access Control Register, offset: 0x50 */
  __IO uint32_t PACRJ;                             /**< Peripheral Access Control Register, offset: 0x54 */
  __IO uint32_t PACRK;                             /**< Peripheral Access Control Register, offset: 0x58 */
  __IO uint32_t PACRL;                             /**< Peripheral Access Control Register, offset: 0x5C */
  __IO uint32_t PACRM;                             /**< Peripheral Access Control Register, offset: 0x60 */
  __IO uint32_t PACRN;                             /**< Peripheral Access Control Register, offset: 0x64 */
  __IO uint32_t PACRO;                             /**< Peripheral Access Control Register, offset: 0x68 */
  __IO uint32_t PACRP;                             /**< Peripheral Access Control Register, offset: 0x6C */
} AIPS_Type;

/* ----------------------------------------------------------------------------
   -- AIPS Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup AIPS_Register_Masks AIPS Register Masks
 * @{
 */

/*! @name PACRA - Peripheral Access Control Register */
#define AIPS_PACRA_AC7_MASK                      (0x7U)
#define AIPS_PACRA_AC7_SHIFT                     (0U)
#define AIPS_PACRA_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC7_SHIFT)) & AIPS_PACRA_AC7_MASK)
#define AIPS_PACRA_RO7_MASK                      (0x8U)
#define AIPS_PACRA_RO7_SHIFT                     (3U)
#define AIPS_PACRA_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO7_SHIFT)) & AIPS_PACRA_RO7_MASK)
#define AIPS_PACRA_AC6_MASK                      (0x70U)
#define AIPS_PACRA_AC6_SHIFT                     (4U)
#define AIPS_PACRA_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC6_SHIFT)) & AIPS_PACRA_AC6_MASK)
#define AIPS_PACRA_RO6_MASK                      (0x80U)
#define AIPS_PACRA_RO6_SHIFT                     (7U)
#define AIPS_PACRA_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO6_SHIFT)) & AIPS_PACRA_RO6_MASK)
#define AIPS_PACRA_AC5_MASK                      (0x700U)
#define AIPS_PACRA_AC5_SHIFT                     (8U)
#define AIPS_PACRA_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC5_SHIFT)) & AIPS_PACRA_AC5_MASK)
#define AIPS_PACRA_RO5_MASK                      (0x800U)
#define AIPS_PACRA_RO5_SHIFT                     (11U)
#define AIPS_PACRA_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO5_SHIFT)) & AIPS_PACRA_RO5_MASK)
#define AIPS_PACRA_AC4_MASK                      (0x7000U)
#define AIPS_PACRA_AC4_SHIFT                     (12U)
#define AIPS_PACRA_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC4_SHIFT)) & AIPS_PACRA_AC4_MASK)
#define AIPS_PACRA_RO4_MASK                      (0x8000U)
#define AIPS_PACRA_RO4_SHIFT                     (15U)
#define AIPS_PACRA_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO4_SHIFT)) & AIPS_PACRA_RO4_MASK)
#define AIPS_PACRA_AC3_MASK                      (0x70000U)
#define AIPS_PACRA_AC3_SHIFT                     (16U)
#define AIPS_PACRA_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC3_SHIFT)) & AIPS_PACRA_AC3_MASK)
#define AIPS_PACRA_RO3_MASK                      (0x80000U)
#define AIPS_PACRA_RO3_SHIFT                     (19U)
#define AIPS_PACRA_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO3_SHIFT)) & AIPS_PACRA_RO3_MASK)
#define AIPS_PACRA_AC2_MASK                      (0x700000U)
#define AIPS_PACRA_AC2_SHIFT                     (20U)
#define AIPS_PACRA_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC2_SHIFT)) & AIPS_PACRA_AC2_MASK)
#define AIPS_PACRA_RO2_MASK                      (0x800000U)
#define AIPS_PACRA_RO2_SHIFT                     (23U)
#define AIPS_PACRA_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO2_SHIFT)) & AIPS_PACRA_RO2_MASK)
#define AIPS_PACRA_AC1_MASK                      (0x7000000U)
#define AIPS_PACRA_AC1_SHIFT                     (24U)
#define AIPS_PACRA_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC1_SHIFT)) & AIPS_PACRA_AC1_MASK)
#define AIPS_PACRA_RO1_MASK                      (0x8000000U)
#define AIPS_PACRA_RO1_SHIFT                     (27U)
#define AIPS_PACRA_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO1_SHIFT)) & AIPS_PACRA_RO1_MASK)
#define AIPS_PACRA_AC0_MASK                      (0x70000000U)
#define AIPS_PACRA_AC0_SHIFT                     (28U)
#define AIPS_PACRA_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_AC0_SHIFT)) & AIPS_PACRA_AC0_MASK)
#define AIPS_PACRA_RO0_MASK                      (0x80000000U)
#define AIPS_PACRA_RO0_SHIFT                     (31U)
#define AIPS_PACRA_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRA_RO0_SHIFT)) & AIPS_PACRA_RO0_MASK)

/*! @name PACRB - Peripheral Access Control Register */
#define AIPS_PACRB_AC7_MASK                      (0x7U)
#define AIPS_PACRB_AC7_SHIFT                     (0U)
#define AIPS_PACRB_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC7_SHIFT)) & AIPS_PACRB_AC7_MASK)
#define AIPS_PACRB_RO7_MASK                      (0x8U)
#define AIPS_PACRB_RO7_SHIFT                     (3U)
#define AIPS_PACRB_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO7_SHIFT)) & AIPS_PACRB_RO7_MASK)
#define AIPS_PACRB_AC6_MASK                      (0x70U)
#define AIPS_PACRB_AC6_SHIFT                     (4U)
#define AIPS_PACRB_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC6_SHIFT)) & AIPS_PACRB_AC6_MASK)
#define AIPS_PACRB_RO6_MASK                      (0x80U)
#define AIPS_PACRB_RO6_SHIFT                     (7U)
#define AIPS_PACRB_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO6_SHIFT)) & AIPS_PACRB_RO6_MASK)
#define AIPS_PACRB_AC5_MASK                      (0x700U)
#define AIPS_PACRB_AC5_SHIFT                     (8U)
#define AIPS_PACRB_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC5_SHIFT)) & AIPS_PACRB_AC5_MASK)
#define AIPS_PACRB_RO5_MASK                      (0x800U)
#define AIPS_PACRB_RO5_SHIFT                     (11U)
#define AIPS_PACRB_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO5_SHIFT)) & AIPS_PACRB_RO5_MASK)
#define AIPS_PACRB_AC4_MASK                      (0x7000U)
#define AIPS_PACRB_AC4_SHIFT                     (12U)
#define AIPS_PACRB_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC4_SHIFT)) & AIPS_PACRB_AC4_MASK)
#define AIPS_PACRB_RO4_MASK                      (0x8000U)
#define AIPS_PACRB_RO4_SHIFT                     (15U)
#define AIPS_PACRB_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO4_SHIFT)) & AIPS_PACRB_RO4_MASK)
#define AIPS_PACRB_AC3_MASK                      (0x70000U)
#define AIPS_PACRB_AC3_SHIFT                     (16U)
#define AIPS_PACRB_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC3_SHIFT)) & AIPS_PACRB_AC3_MASK)
#define AIPS_PACRB_RO3_MASK                      (0x80000U)
#define AIPS_PACRB_RO3_SHIFT                     (19U)
#define AIPS_PACRB_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO3_SHIFT)) & AIPS_PACRB_RO3_MASK)
#define AIPS_PACRB_AC2_MASK                      (0x700000U)
#define AIPS_PACRB_AC2_SHIFT                     (20U)
#define AIPS_PACRB_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC2_SHIFT)) & AIPS_PACRB_AC2_MASK)
#define AIPS_PACRB_RO2_MASK                      (0x800000U)
#define AIPS_PACRB_RO2_SHIFT                     (23U)
#define AIPS_PACRB_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO2_SHIFT)) & AIPS_PACRB_RO2_MASK)
#define AIPS_PACRB_AC1_MASK                      (0x7000000U)
#define AIPS_PACRB_AC1_SHIFT                     (24U)
#define AIPS_PACRB_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC1_SHIFT)) & AIPS_PACRB_AC1_MASK)
#define AIPS_PACRB_RO1_MASK                      (0x8000000U)
#define AIPS_PACRB_RO1_SHIFT                     (27U)
#define AIPS_PACRB_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO1_SHIFT)) & AIPS_PACRB_RO1_MASK)
#define AIPS_PACRB_AC0_MASK                      (0x70000000U)
#define AIPS_PACRB_AC0_SHIFT                     (28U)
#define AIPS_PACRB_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_AC0_SHIFT)) & AIPS_PACRB_AC0_MASK)
#define AIPS_PACRB_RO0_MASK                      (0x80000000U)
#define AIPS_PACRB_RO0_SHIFT                     (31U)
#define AIPS_PACRB_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRB_RO0_SHIFT)) & AIPS_PACRB_RO0_MASK)

/*! @name PACRE - Peripheral Access Control Register */
#define AIPS_PACRE_AC7_MASK                      (0x7U)
#define AIPS_PACRE_AC7_SHIFT                     (0U)
#define AIPS_PACRE_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC7_SHIFT)) & AIPS_PACRE_AC7_MASK)
#define AIPS_PACRE_RO7_MASK                      (0x8U)
#define AIPS_PACRE_RO7_SHIFT                     (3U)
#define AIPS_PACRE_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO7_SHIFT)) & AIPS_PACRE_RO7_MASK)
#define AIPS_PACRE_AC6_MASK                      (0x70U)
#define AIPS_PACRE_AC6_SHIFT                     (4U)
#define AIPS_PACRE_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC6_SHIFT)) & AIPS_PACRE_AC6_MASK)
#define AIPS_PACRE_RO6_MASK                      (0x80U)
#define AIPS_PACRE_RO6_SHIFT                     (7U)
#define AIPS_PACRE_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO6_SHIFT)) & AIPS_PACRE_RO6_MASK)
#define AIPS_PACRE_AC5_MASK                      (0x700U)
#define AIPS_PACRE_AC5_SHIFT                     (8U)
#define AIPS_PACRE_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC5_SHIFT)) & AIPS_PACRE_AC5_MASK)
#define AIPS_PACRE_RO5_MASK                      (0x800U)
#define AIPS_PACRE_RO5_SHIFT                     (11U)
#define AIPS_PACRE_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO5_SHIFT)) & AIPS_PACRE_RO5_MASK)
#define AIPS_PACRE_AC4_MASK                      (0x7000U)
#define AIPS_PACRE_AC4_SHIFT                     (12U)
#define AIPS_PACRE_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC4_SHIFT)) & AIPS_PACRE_AC4_MASK)
#define AIPS_PACRE_RO4_MASK                      (0x8000U)
#define AIPS_PACRE_RO4_SHIFT                     (15U)
#define AIPS_PACRE_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO4_SHIFT)) & AIPS_PACRE_RO4_MASK)
#define AIPS_PACRE_AC3_MASK                      (0x70000U)
#define AIPS_PACRE_AC3_SHIFT                     (16U)
#define AIPS_PACRE_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC3_SHIFT)) & AIPS_PACRE_AC3_MASK)
#define AIPS_PACRE_RO3_MASK                      (0x80000U)
#define AIPS_PACRE_RO3_SHIFT                     (19U)
#define AIPS_PACRE_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO3_SHIFT)) & AIPS_PACRE_RO3_MASK)
#define AIPS_PACRE_AC2_MASK                      (0x700000U)
#define AIPS_PACRE_AC2_SHIFT                     (20U)
#define AIPS_PACRE_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC2_SHIFT)) & AIPS_PACRE_AC2_MASK)
#define AIPS_PACRE_RO2_MASK                      (0x800000U)
#define AIPS_PACRE_RO2_SHIFT                     (23U)
#define AIPS_PACRE_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO2_SHIFT)) & AIPS_PACRE_RO2_MASK)
#define AIPS_PACRE_AC1_MASK                      (0x7000000U)
#define AIPS_PACRE_AC1_SHIFT                     (24U)
#define AIPS_PACRE_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC1_SHIFT)) & AIPS_PACRE_AC1_MASK)
#define AIPS_PACRE_RO1_MASK                      (0x8000000U)
#define AIPS_PACRE_RO1_SHIFT                     (27U)
#define AIPS_PACRE_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO1_SHIFT)) & AIPS_PACRE_RO1_MASK)
#define AIPS_PACRE_AC0_MASK                      (0x70000000U)
#define AIPS_PACRE_AC0_SHIFT                     (28U)
#define AIPS_PACRE_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_AC0_SHIFT)) & AIPS_PACRE_AC0_MASK)
#define AIPS_PACRE_RO0_MASK                      (0x80000000U)
#define AIPS_PACRE_RO0_SHIFT                     (31U)
#define AIPS_PACRE_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRE_RO0_SHIFT)) & AIPS_PACRE_RO0_MASK)

/*! @name PACRF - Peripheral Access Control Register */
#define AIPS_PACRF_AC7_MASK                      (0x7U)
#define AIPS_PACRF_AC7_SHIFT                     (0U)
#define AIPS_PACRF_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC7_SHIFT)) & AIPS_PACRF_AC7_MASK)
#define AIPS_PACRF_RO7_MASK                      (0x8U)
#define AIPS_PACRF_RO7_SHIFT                     (3U)
#define AIPS_PACRF_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO7_SHIFT)) & AIPS_PACRF_RO7_MASK)
#define AIPS_PACRF_AC6_MASK                      (0x70U)
#define AIPS_PACRF_AC6_SHIFT                     (4U)
#define AIPS_PACRF_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC6_SHIFT)) & AIPS_PACRF_AC6_MASK)
#define AIPS_PACRF_RO6_MASK                      (0x80U)
#define AIPS_PACRF_RO6_SHIFT                     (7U)
#define AIPS_PACRF_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO6_SHIFT)) & AIPS_PACRF_RO6_MASK)
#define AIPS_PACRF_AC5_MASK                      (0x700U)
#define AIPS_PACRF_AC5_SHIFT                     (8U)
#define AIPS_PACRF_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC5_SHIFT)) & AIPS_PACRF_AC5_MASK)
#define AIPS_PACRF_RO5_MASK                      (0x800U)
#define AIPS_PACRF_RO5_SHIFT                     (11U)
#define AIPS_PACRF_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO5_SHIFT)) & AIPS_PACRF_RO5_MASK)
#define AIPS_PACRF_AC4_MASK                      (0x7000U)
#define AIPS_PACRF_AC4_SHIFT                     (12U)
#define AIPS_PACRF_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC4_SHIFT)) & AIPS_PACRF_AC4_MASK)
#define AIPS_PACRF_RO4_MASK                      (0x8000U)
#define AIPS_PACRF_RO4_SHIFT                     (15U)
#define AIPS_PACRF_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO4_SHIFT)) & AIPS_PACRF_RO4_MASK)
#define AIPS_PACRF_AC3_MASK                      (0x70000U)
#define AIPS_PACRF_AC3_SHIFT                     (16U)
#define AIPS_PACRF_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC3_SHIFT)) & AIPS_PACRF_AC3_MASK)
#define AIPS_PACRF_RO3_MASK                      (0x80000U)
#define AIPS_PACRF_RO3_SHIFT                     (19U)
#define AIPS_PACRF_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO3_SHIFT)) & AIPS_PACRF_RO3_MASK)
#define AIPS_PACRF_AC2_MASK                      (0x700000U)
#define AIPS_PACRF_AC2_SHIFT                     (20U)
#define AIPS_PACRF_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC2_SHIFT)) & AIPS_PACRF_AC2_MASK)
#define AIPS_PACRF_RO2_MASK                      (0x800000U)
#define AIPS_PACRF_RO2_SHIFT                     (23U)
#define AIPS_PACRF_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO2_SHIFT)) & AIPS_PACRF_RO2_MASK)
#define AIPS_PACRF_AC1_MASK                      (0x7000000U)
#define AIPS_PACRF_AC1_SHIFT                     (24U)
#define AIPS_PACRF_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC1_SHIFT)) & AIPS_PACRF_AC1_MASK)
#define AIPS_PACRF_RO1_MASK                      (0x8000000U)
#define AIPS_PACRF_RO1_SHIFT                     (27U)
#define AIPS_PACRF_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO1_SHIFT)) & AIPS_PACRF_RO1_MASK)
#define AIPS_PACRF_AC0_MASK                      (0x70000000U)
#define AIPS_PACRF_AC0_SHIFT                     (28U)
#define AIPS_PACRF_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_AC0_SHIFT)) & AIPS_PACRF_AC0_MASK)
#define AIPS_PACRF_RO0_MASK                      (0x80000000U)
#define AIPS_PACRF_RO0_SHIFT                     (31U)
#define AIPS_PACRF_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRF_RO0_SHIFT)) & AIPS_PACRF_RO0_MASK)

/*! @name PACRG - Peripheral Access Control Register */
#define AIPS_PACRG_AC7_MASK                      (0x7U)
#define AIPS_PACRG_AC7_SHIFT                     (0U)
#define AIPS_PACRG_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC7_SHIFT)) & AIPS_PACRG_AC7_MASK)
#define AIPS_PACRG_RO7_MASK                      (0x8U)
#define AIPS_PACRG_RO7_SHIFT                     (3U)
#define AIPS_PACRG_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO7_SHIFT)) & AIPS_PACRG_RO7_MASK)
#define AIPS_PACRG_AC6_MASK                      (0x70U)
#define AIPS_PACRG_AC6_SHIFT                     (4U)
#define AIPS_PACRG_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC6_SHIFT)) & AIPS_PACRG_AC6_MASK)
#define AIPS_PACRG_RO6_MASK                      (0x80U)
#define AIPS_PACRG_RO6_SHIFT                     (7U)
#define AIPS_PACRG_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO6_SHIFT)) & AIPS_PACRG_RO6_MASK)
#define AIPS_PACRG_AC5_MASK                      (0x700U)
#define AIPS_PACRG_AC5_SHIFT                     (8U)
#define AIPS_PACRG_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC5_SHIFT)) & AIPS_PACRG_AC5_MASK)
#define AIPS_PACRG_RO5_MASK                      (0x800U)
#define AIPS_PACRG_RO5_SHIFT                     (11U)
#define AIPS_PACRG_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO5_SHIFT)) & AIPS_PACRG_RO5_MASK)
#define AIPS_PACRG_AC4_MASK                      (0x7000U)
#define AIPS_PACRG_AC4_SHIFT                     (12U)
#define AIPS_PACRG_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC4_SHIFT)) & AIPS_PACRG_AC4_MASK)
#define AIPS_PACRG_RO4_MASK                      (0x8000U)
#define AIPS_PACRG_RO4_SHIFT                     (15U)
#define AIPS_PACRG_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO4_SHIFT)) & AIPS_PACRG_RO4_MASK)
#define AIPS_PACRG_AC3_MASK                      (0x70000U)
#define AIPS_PACRG_AC3_SHIFT                     (16U)
#define AIPS_PACRG_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC3_SHIFT)) & AIPS_PACRG_AC3_MASK)
#define AIPS_PACRG_RO3_MASK                      (0x80000U)
#define AIPS_PACRG_RO3_SHIFT                     (19U)
#define AIPS_PACRG_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO3_SHIFT)) & AIPS_PACRG_RO3_MASK)
#define AIPS_PACRG_AC2_MASK                      (0x700000U)
#define AIPS_PACRG_AC2_SHIFT                     (20U)
#define AIPS_PACRG_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC2_SHIFT)) & AIPS_PACRG_AC2_MASK)
#define AIPS_PACRG_RO2_MASK                      (0x800000U)
#define AIPS_PACRG_RO2_SHIFT                     (23U)
#define AIPS_PACRG_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO2_SHIFT)) & AIPS_PACRG_RO2_MASK)
#define AIPS_PACRG_AC1_MASK                      (0x7000000U)
#define AIPS_PACRG_AC1_SHIFT                     (24U)
#define AIPS_PACRG_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC1_SHIFT)) & AIPS_PACRG_AC1_MASK)
#define AIPS_PACRG_RO1_MASK                      (0x8000000U)
#define AIPS_PACRG_RO1_SHIFT                     (27U)
#define AIPS_PACRG_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO1_SHIFT)) & AIPS_PACRG_RO1_MASK)
#define AIPS_PACRG_AC0_MASK                      (0x70000000U)
#define AIPS_PACRG_AC0_SHIFT                     (28U)
#define AIPS_PACRG_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_AC0_SHIFT)) & AIPS_PACRG_AC0_MASK)
#define AIPS_PACRG_RO0_MASK                      (0x80000000U)
#define AIPS_PACRG_RO0_SHIFT                     (31U)
#define AIPS_PACRG_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRG_RO0_SHIFT)) & AIPS_PACRG_RO0_MASK)

/*! @name PACRH - Peripheral Access Control Register */
#define AIPS_PACRH_AC7_MASK                      (0x7U)
#define AIPS_PACRH_AC7_SHIFT                     (0U)
#define AIPS_PACRH_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC7_SHIFT)) & AIPS_PACRH_AC7_MASK)
#define AIPS_PACRH_RO7_MASK                      (0x8U)
#define AIPS_PACRH_RO7_SHIFT                     (3U)
#define AIPS_PACRH_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO7_SHIFT)) & AIPS_PACRH_RO7_MASK)
#define AIPS_PACRH_AC6_MASK                      (0x70U)
#define AIPS_PACRH_AC6_SHIFT                     (4U)
#define AIPS_PACRH_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC6_SHIFT)) & AIPS_PACRH_AC6_MASK)
#define AIPS_PACRH_RO6_MASK                      (0x80U)
#define AIPS_PACRH_RO6_SHIFT                     (7U)
#define AIPS_PACRH_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO6_SHIFT)) & AIPS_PACRH_RO6_MASK)
#define AIPS_PACRH_AC5_MASK                      (0x700U)
#define AIPS_PACRH_AC5_SHIFT                     (8U)
#define AIPS_PACRH_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC5_SHIFT)) & AIPS_PACRH_AC5_MASK)
#define AIPS_PACRH_RO5_MASK                      (0x800U)
#define AIPS_PACRH_RO5_SHIFT                     (11U)
#define AIPS_PACRH_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO5_SHIFT)) & AIPS_PACRH_RO5_MASK)
#define AIPS_PACRH_AC4_MASK                      (0x7000U)
#define AIPS_PACRH_AC4_SHIFT                     (12U)
#define AIPS_PACRH_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC4_SHIFT)) & AIPS_PACRH_AC4_MASK)
#define AIPS_PACRH_RO4_MASK                      (0x8000U)
#define AIPS_PACRH_RO4_SHIFT                     (15U)
#define AIPS_PACRH_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO4_SHIFT)) & AIPS_PACRH_RO4_MASK)
#define AIPS_PACRH_AC3_MASK                      (0x70000U)
#define AIPS_PACRH_AC3_SHIFT                     (16U)
#define AIPS_PACRH_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC3_SHIFT)) & AIPS_PACRH_AC3_MASK)
#define AIPS_PACRH_RO3_MASK                      (0x80000U)
#define AIPS_PACRH_RO3_SHIFT                     (19U)
#define AIPS_PACRH_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO3_SHIFT)) & AIPS_PACRH_RO3_MASK)
#define AIPS_PACRH_AC2_MASK                      (0x700000U)
#define AIPS_PACRH_AC2_SHIFT                     (20U)
#define AIPS_PACRH_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC2_SHIFT)) & AIPS_PACRH_AC2_MASK)
#define AIPS_PACRH_RO2_MASK                      (0x800000U)
#define AIPS_PACRH_RO2_SHIFT                     (23U)
#define AIPS_PACRH_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO2_SHIFT)) & AIPS_PACRH_RO2_MASK)
#define AIPS_PACRH_AC1_MASK                      (0x7000000U)
#define AIPS_PACRH_AC1_SHIFT                     (24U)
#define AIPS_PACRH_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC1_SHIFT)) & AIPS_PACRH_AC1_MASK)
#define AIPS_PACRH_RO1_MASK                      (0x8000000U)
#define AIPS_PACRH_RO1_SHIFT                     (27U)
#define AIPS_PACRH_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO1_SHIFT)) & AIPS_PACRH_RO1_MASK)
#define AIPS_PACRH_AC0_MASK                      (0x70000000U)
#define AIPS_PACRH_AC0_SHIFT                     (28U)
#define AIPS_PACRH_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_AC0_SHIFT)) & AIPS_PACRH_AC0_MASK)
#define AIPS_PACRH_RO0_MASK                      (0x80000000U)
#define AIPS_PACRH_RO0_SHIFT                     (31U)
#define AIPS_PACRH_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRH_RO0_SHIFT)) & AIPS_PACRH_RO0_MASK)

/*! @name PACRI - Peripheral Access Control Register */
#define AIPS_PACRI_AC7_MASK                      (0x7U)
#define AIPS_PACRI_AC7_SHIFT                     (0U)
#define AIPS_PACRI_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC7_SHIFT)) & AIPS_PACRI_AC7_MASK)
#define AIPS_PACRI_RO7_MASK                      (0x8U)
#define AIPS_PACRI_RO7_SHIFT                     (3U)
#define AIPS_PACRI_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO7_SHIFT)) & AIPS_PACRI_RO7_MASK)
#define AIPS_PACRI_AC6_MASK                      (0x70U)
#define AIPS_PACRI_AC6_SHIFT                     (4U)
#define AIPS_PACRI_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC6_SHIFT)) & AIPS_PACRI_AC6_MASK)
#define AIPS_PACRI_RO6_MASK                      (0x80U)
#define AIPS_PACRI_RO6_SHIFT                     (7U)
#define AIPS_PACRI_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO6_SHIFT)) & AIPS_PACRI_RO6_MASK)
#define AIPS_PACRI_AC5_MASK                      (0x700U)
#define AIPS_PACRI_AC5_SHIFT                     (8U)
#define AIPS_PACRI_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC5_SHIFT)) & AIPS_PACRI_AC5_MASK)
#define AIPS_PACRI_RO5_MASK                      (0x800U)
#define AIPS_PACRI_RO5_SHIFT                     (11U)
#define AIPS_PACRI_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO5_SHIFT)) & AIPS_PACRI_RO5_MASK)
#define AIPS_PACRI_AC4_MASK                      (0x7000U)
#define AIPS_PACRI_AC4_SHIFT                     (12U)
#define AIPS_PACRI_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC4_SHIFT)) & AIPS_PACRI_AC4_MASK)
#define AIPS_PACRI_RO4_MASK                      (0x8000U)
#define AIPS_PACRI_RO4_SHIFT                     (15U)
#define AIPS_PACRI_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO4_SHIFT)) & AIPS_PACRI_RO4_MASK)
#define AIPS_PACRI_AC3_MASK                      (0x70000U)
#define AIPS_PACRI_AC3_SHIFT                     (16U)
#define AIPS_PACRI_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC3_SHIFT)) & AIPS_PACRI_AC3_MASK)
#define AIPS_PACRI_RO3_MASK                      (0x80000U)
#define AIPS_PACRI_RO3_SHIFT                     (19U)
#define AIPS_PACRI_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO3_SHIFT)) & AIPS_PACRI_RO3_MASK)
#define AIPS_PACRI_AC2_MASK                      (0x700000U)
#define AIPS_PACRI_AC2_SHIFT                     (20U)
#define AIPS_PACRI_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC2_SHIFT)) & AIPS_PACRI_AC2_MASK)
#define AIPS_PACRI_RO2_MASK                      (0x800000U)
#define AIPS_PACRI_RO2_SHIFT                     (23U)
#define AIPS_PACRI_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO2_SHIFT)) & AIPS_PACRI_RO2_MASK)
#define AIPS_PACRI_AC1_MASK                      (0x7000000U)
#define AIPS_PACRI_AC1_SHIFT                     (24U)
#define AIPS_PACRI_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC1_SHIFT)) & AIPS_PACRI_AC1_MASK)
#define AIPS_PACRI_RO1_MASK                      (0x8000000U)
#define AIPS_PACRI_RO1_SHIFT                     (27U)
#define AIPS_PACRI_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO1_SHIFT)) & AIPS_PACRI_RO1_MASK)
#define AIPS_PACRI_AC0_MASK                      (0x70000000U)
#define AIPS_PACRI_AC0_SHIFT                     (28U)
#define AIPS_PACRI_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_AC0_SHIFT)) & AIPS_PACRI_AC0_MASK)
#define AIPS_PACRI_RO0_MASK                      (0x80000000U)
#define AIPS_PACRI_RO0_SHIFT                     (31U)
#define AIPS_PACRI_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRI_RO0_SHIFT)) & AIPS_PACRI_RO0_MASK)

/*! @name PACRJ - Peripheral Access Control Register */
#define AIPS_PACRJ_AC7_MASK                      (0x7U)
#define AIPS_PACRJ_AC7_SHIFT                     (0U)
#define AIPS_PACRJ_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC7_SHIFT)) & AIPS_PACRJ_AC7_MASK)
#define AIPS_PACRJ_RO7_MASK                      (0x8U)
#define AIPS_PACRJ_RO7_SHIFT                     (3U)
#define AIPS_PACRJ_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO7_SHIFT)) & AIPS_PACRJ_RO7_MASK)
#define AIPS_PACRJ_AC6_MASK                      (0x70U)
#define AIPS_PACRJ_AC6_SHIFT                     (4U)
#define AIPS_PACRJ_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC6_SHIFT)) & AIPS_PACRJ_AC6_MASK)
#define AIPS_PACRJ_RO6_MASK                      (0x80U)
#define AIPS_PACRJ_RO6_SHIFT                     (7U)
#define AIPS_PACRJ_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO6_SHIFT)) & AIPS_PACRJ_RO6_MASK)
#define AIPS_PACRJ_AC5_MASK                      (0x700U)
#define AIPS_PACRJ_AC5_SHIFT                     (8U)
#define AIPS_PACRJ_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC5_SHIFT)) & AIPS_PACRJ_AC5_MASK)
#define AIPS_PACRJ_RO5_MASK                      (0x800U)
#define AIPS_PACRJ_RO5_SHIFT                     (11U)
#define AIPS_PACRJ_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO5_SHIFT)) & AIPS_PACRJ_RO5_MASK)
#define AIPS_PACRJ_AC4_MASK                      (0x7000U)
#define AIPS_PACRJ_AC4_SHIFT                     (12U)
#define AIPS_PACRJ_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC4_SHIFT)) & AIPS_PACRJ_AC4_MASK)
#define AIPS_PACRJ_RO4_MASK                      (0x8000U)
#define AIPS_PACRJ_RO4_SHIFT                     (15U)
#define AIPS_PACRJ_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO4_SHIFT)) & AIPS_PACRJ_RO4_MASK)
#define AIPS_PACRJ_AC3_MASK                      (0x70000U)
#define AIPS_PACRJ_AC3_SHIFT                     (16U)
#define AIPS_PACRJ_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC3_SHIFT)) & AIPS_PACRJ_AC3_MASK)
#define AIPS_PACRJ_RO3_MASK                      (0x80000U)
#define AIPS_PACRJ_RO3_SHIFT                     (19U)
#define AIPS_PACRJ_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO3_SHIFT)) & AIPS_PACRJ_RO3_MASK)
#define AIPS_PACRJ_AC2_MASK                      (0x700000U)
#define AIPS_PACRJ_AC2_SHIFT                     (20U)
#define AIPS_PACRJ_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC2_SHIFT)) & AIPS_PACRJ_AC2_MASK)
#define AIPS_PACRJ_RO2_MASK                      (0x800000U)
#define AIPS_PACRJ_RO2_SHIFT                     (23U)
#define AIPS_PACRJ_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO2_SHIFT)) & AIPS_PACRJ_RO2_MASK)
#define AIPS_PACRJ_AC1_MASK                      (0x7000000U)
#define AIPS_PACRJ_AC1_SHIFT                     (24U)
#define AIPS_PACRJ_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC1_SHIFT)) & AIPS_PACRJ_AC1_MASK)
#define AIPS_PACRJ_RO1_MASK                      (0x8000000U)
#define AIPS_PACRJ_RO1_SHIFT                     (27U)
#define AIPS_PACRJ_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO1_SHIFT)) & AIPS_PACRJ_RO1_MASK)
#define AIPS_PACRJ_AC0_MASK                      (0x70000000U)
#define AIPS_PACRJ_AC0_SHIFT                     (28U)
#define AIPS_PACRJ_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_AC0_SHIFT)) & AIPS_PACRJ_AC0_MASK)
#define AIPS_PACRJ_RO0_MASK                      (0x80000000U)
#define AIPS_PACRJ_RO0_SHIFT                     (31U)
#define AIPS_PACRJ_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRJ_RO0_SHIFT)) & AIPS_PACRJ_RO0_MASK)

/*! @name PACRK - Peripheral Access Control Register */
#define AIPS_PACRK_AC7_MASK                      (0x7U)
#define AIPS_PACRK_AC7_SHIFT                     (0U)
#define AIPS_PACRK_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC7_SHIFT)) & AIPS_PACRK_AC7_MASK)
#define AIPS_PACRK_RO7_MASK                      (0x8U)
#define AIPS_PACRK_RO7_SHIFT                     (3U)
#define AIPS_PACRK_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO7_SHIFT)) & AIPS_PACRK_RO7_MASK)
#define AIPS_PACRK_AC6_MASK                      (0x70U)
#define AIPS_PACRK_AC6_SHIFT                     (4U)
#define AIPS_PACRK_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC6_SHIFT)) & AIPS_PACRK_AC6_MASK)
#define AIPS_PACRK_RO6_MASK                      (0x80U)
#define AIPS_PACRK_RO6_SHIFT                     (7U)
#define AIPS_PACRK_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO6_SHIFT)) & AIPS_PACRK_RO6_MASK)
#define AIPS_PACRK_AC5_MASK                      (0x700U)
#define AIPS_PACRK_AC5_SHIFT                     (8U)
#define AIPS_PACRK_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC5_SHIFT)) & AIPS_PACRK_AC5_MASK)
#define AIPS_PACRK_RO5_MASK                      (0x800U)
#define AIPS_PACRK_RO5_SHIFT                     (11U)
#define AIPS_PACRK_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO5_SHIFT)) & AIPS_PACRK_RO5_MASK)
#define AIPS_PACRK_AC4_MASK                      (0x7000U)
#define AIPS_PACRK_AC4_SHIFT                     (12U)
#define AIPS_PACRK_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC4_SHIFT)) & AIPS_PACRK_AC4_MASK)
#define AIPS_PACRK_RO4_MASK                      (0x8000U)
#define AIPS_PACRK_RO4_SHIFT                     (15U)
#define AIPS_PACRK_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO4_SHIFT)) & AIPS_PACRK_RO4_MASK)
#define AIPS_PACRK_AC3_MASK                      (0x70000U)
#define AIPS_PACRK_AC3_SHIFT                     (16U)
#define AIPS_PACRK_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC3_SHIFT)) & AIPS_PACRK_AC3_MASK)
#define AIPS_PACRK_RO3_MASK                      (0x80000U)
#define AIPS_PACRK_RO3_SHIFT                     (19U)
#define AIPS_PACRK_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO3_SHIFT)) & AIPS_PACRK_RO3_MASK)
#define AIPS_PACRK_AC2_MASK                      (0x700000U)
#define AIPS_PACRK_AC2_SHIFT                     (20U)
#define AIPS_PACRK_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC2_SHIFT)) & AIPS_PACRK_AC2_MASK)
#define AIPS_PACRK_RO2_MASK                      (0x800000U)
#define AIPS_PACRK_RO2_SHIFT                     (23U)
#define AIPS_PACRK_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO2_SHIFT)) & AIPS_PACRK_RO2_MASK)
#define AIPS_PACRK_AC1_MASK                      (0x7000000U)
#define AIPS_PACRK_AC1_SHIFT                     (24U)
#define AIPS_PACRK_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC1_SHIFT)) & AIPS_PACRK_AC1_MASK)
#define AIPS_PACRK_RO1_MASK                      (0x8000000U)
#define AIPS_PACRK_RO1_SHIFT                     (27U)
#define AIPS_PACRK_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO1_SHIFT)) & AIPS_PACRK_RO1_MASK)
#define AIPS_PACRK_AC0_MASK                      (0x70000000U)
#define AIPS_PACRK_AC0_SHIFT                     (28U)
#define AIPS_PACRK_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_AC0_SHIFT)) & AIPS_PACRK_AC0_MASK)
#define AIPS_PACRK_RO0_MASK                      (0x80000000U)
#define AIPS_PACRK_RO0_SHIFT                     (31U)
#define AIPS_PACRK_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRK_RO0_SHIFT)) & AIPS_PACRK_RO0_MASK)

/*! @name PACRL - Peripheral Access Control Register */
#define AIPS_PACRL_AC7_MASK                      (0x7U)
#define AIPS_PACRL_AC7_SHIFT                     (0U)
#define AIPS_PACRL_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC7_SHIFT)) & AIPS_PACRL_AC7_MASK)
#define AIPS_PACRL_RO7_MASK                      (0x8U)
#define AIPS_PACRL_RO7_SHIFT                     (3U)
#define AIPS_PACRL_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO7_SHIFT)) & AIPS_PACRL_RO7_MASK)
#define AIPS_PACRL_AC6_MASK                      (0x70U)
#define AIPS_PACRL_AC6_SHIFT                     (4U)
#define AIPS_PACRL_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC6_SHIFT)) & AIPS_PACRL_AC6_MASK)
#define AIPS_PACRL_RO6_MASK                      (0x80U)
#define AIPS_PACRL_RO6_SHIFT                     (7U)
#define AIPS_PACRL_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO6_SHIFT)) & AIPS_PACRL_RO6_MASK)
#define AIPS_PACRL_AC5_MASK                      (0x700U)
#define AIPS_PACRL_AC5_SHIFT                     (8U)
#define AIPS_PACRL_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC5_SHIFT)) & AIPS_PACRL_AC5_MASK)
#define AIPS_PACRL_RO5_MASK                      (0x800U)
#define AIPS_PACRL_RO5_SHIFT                     (11U)
#define AIPS_PACRL_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO5_SHIFT)) & AIPS_PACRL_RO5_MASK)
#define AIPS_PACRL_AC4_MASK                      (0x7000U)
#define AIPS_PACRL_AC4_SHIFT                     (12U)
#define AIPS_PACRL_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC4_SHIFT)) & AIPS_PACRL_AC4_MASK)
#define AIPS_PACRL_RO4_MASK                      (0x8000U)
#define AIPS_PACRL_RO4_SHIFT                     (15U)
#define AIPS_PACRL_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO4_SHIFT)) & AIPS_PACRL_RO4_MASK)
#define AIPS_PACRL_AC3_MASK                      (0x70000U)
#define AIPS_PACRL_AC3_SHIFT                     (16U)
#define AIPS_PACRL_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC3_SHIFT)) & AIPS_PACRL_AC3_MASK)
#define AIPS_PACRL_RO3_MASK                      (0x80000U)
#define AIPS_PACRL_RO3_SHIFT                     (19U)
#define AIPS_PACRL_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO3_SHIFT)) & AIPS_PACRL_RO3_MASK)
#define AIPS_PACRL_AC2_MASK                      (0x700000U)
#define AIPS_PACRL_AC2_SHIFT                     (20U)
#define AIPS_PACRL_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC2_SHIFT)) & AIPS_PACRL_AC2_MASK)
#define AIPS_PACRL_RO2_MASK                      (0x800000U)
#define AIPS_PACRL_RO2_SHIFT                     (23U)
#define AIPS_PACRL_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO2_SHIFT)) & AIPS_PACRL_RO2_MASK)
#define AIPS_PACRL_AC1_MASK                      (0x7000000U)
#define AIPS_PACRL_AC1_SHIFT                     (24U)
#define AIPS_PACRL_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC1_SHIFT)) & AIPS_PACRL_AC1_MASK)
#define AIPS_PACRL_RO1_MASK                      (0x8000000U)
#define AIPS_PACRL_RO1_SHIFT                     (27U)
#define AIPS_PACRL_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO1_SHIFT)) & AIPS_PACRL_RO1_MASK)
#define AIPS_PACRL_AC0_MASK                      (0x70000000U)
#define AIPS_PACRL_AC0_SHIFT                     (28U)
#define AIPS_PACRL_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_AC0_SHIFT)) & AIPS_PACRL_AC0_MASK)
#define AIPS_PACRL_RO0_MASK                      (0x80000000U)
#define AIPS_PACRL_RO0_SHIFT                     (31U)
#define AIPS_PACRL_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRL_RO0_SHIFT)) & AIPS_PACRL_RO0_MASK)

/*! @name PACRM - Peripheral Access Control Register */
#define AIPS_PACRM_AC7_MASK                      (0x7U)
#define AIPS_PACRM_AC7_SHIFT                     (0U)
#define AIPS_PACRM_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC7_SHIFT)) & AIPS_PACRM_AC7_MASK)
#define AIPS_PACRM_RO7_MASK                      (0x8U)
#define AIPS_PACRM_RO7_SHIFT                     (3U)
#define AIPS_PACRM_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO7_SHIFT)) & AIPS_PACRM_RO7_MASK)
#define AIPS_PACRM_AC6_MASK                      (0x70U)
#define AIPS_PACRM_AC6_SHIFT                     (4U)
#define AIPS_PACRM_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC6_SHIFT)) & AIPS_PACRM_AC6_MASK)
#define AIPS_PACRM_RO6_MASK                      (0x80U)
#define AIPS_PACRM_RO6_SHIFT                     (7U)
#define AIPS_PACRM_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO6_SHIFT)) & AIPS_PACRM_RO6_MASK)
#define AIPS_PACRM_AC5_MASK                      (0x700U)
#define AIPS_PACRM_AC5_SHIFT                     (8U)
#define AIPS_PACRM_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC5_SHIFT)) & AIPS_PACRM_AC5_MASK)
#define AIPS_PACRM_RO5_MASK                      (0x800U)
#define AIPS_PACRM_RO5_SHIFT                     (11U)
#define AIPS_PACRM_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO5_SHIFT)) & AIPS_PACRM_RO5_MASK)
#define AIPS_PACRM_AC4_MASK                      (0x7000U)
#define AIPS_PACRM_AC4_SHIFT                     (12U)
#define AIPS_PACRM_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC4_SHIFT)) & AIPS_PACRM_AC4_MASK)
#define AIPS_PACRM_RO4_MASK                      (0x8000U)
#define AIPS_PACRM_RO4_SHIFT                     (15U)
#define AIPS_PACRM_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO4_SHIFT)) & AIPS_PACRM_RO4_MASK)
#define AIPS_PACRM_AC3_MASK                      (0x70000U)
#define AIPS_PACRM_AC3_SHIFT                     (16U)
#define AIPS_PACRM_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC3_SHIFT)) & AIPS_PACRM_AC3_MASK)
#define AIPS_PACRM_RO3_MASK                      (0x80000U)
#define AIPS_PACRM_RO3_SHIFT                     (19U)
#define AIPS_PACRM_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO3_SHIFT)) & AIPS_PACRM_RO3_MASK)
#define AIPS_PACRM_AC2_MASK                      (0x700000U)
#define AIPS_PACRM_AC2_SHIFT                     (20U)
#define AIPS_PACRM_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC2_SHIFT)) & AIPS_PACRM_AC2_MASK)
#define AIPS_PACRM_RO2_MASK                      (0x800000U)
#define AIPS_PACRM_RO2_SHIFT                     (23U)
#define AIPS_PACRM_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO2_SHIFT)) & AIPS_PACRM_RO2_MASK)
#define AIPS_PACRM_AC1_MASK                      (0x7000000U)
#define AIPS_PACRM_AC1_SHIFT                     (24U)
#define AIPS_PACRM_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC1_SHIFT)) & AIPS_PACRM_AC1_MASK)
#define AIPS_PACRM_RO1_MASK                      (0x8000000U)
#define AIPS_PACRM_RO1_SHIFT                     (27U)
#define AIPS_PACRM_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO1_SHIFT)) & AIPS_PACRM_RO1_MASK)
#define AIPS_PACRM_AC0_MASK                      (0x70000000U)
#define AIPS_PACRM_AC0_SHIFT                     (28U)
#define AIPS_PACRM_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_AC0_SHIFT)) & AIPS_PACRM_AC0_MASK)
#define AIPS_PACRM_RO0_MASK                      (0x80000000U)
#define AIPS_PACRM_RO0_SHIFT                     (31U)
#define AIPS_PACRM_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRM_RO0_SHIFT)) & AIPS_PACRM_RO0_MASK)

/*! @name PACRN - Peripheral Access Control Register */
#define AIPS_PACRN_AC7_MASK                      (0x7U)
#define AIPS_PACRN_AC7_SHIFT                     (0U)
#define AIPS_PACRN_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC7_SHIFT)) & AIPS_PACRN_AC7_MASK)
#define AIPS_PACRN_RO7_MASK                      (0x8U)
#define AIPS_PACRN_RO7_SHIFT                     (3U)
#define AIPS_PACRN_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO7_SHIFT)) & AIPS_PACRN_RO7_MASK)
#define AIPS_PACRN_AC6_MASK                      (0x70U)
#define AIPS_PACRN_AC6_SHIFT                     (4U)
#define AIPS_PACRN_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC6_SHIFT)) & AIPS_PACRN_AC6_MASK)
#define AIPS_PACRN_RO6_MASK                      (0x80U)
#define AIPS_PACRN_RO6_SHIFT                     (7U)
#define AIPS_PACRN_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO6_SHIFT)) & AIPS_PACRN_RO6_MASK)
#define AIPS_PACRN_AC5_MASK                      (0x700U)
#define AIPS_PACRN_AC5_SHIFT                     (8U)
#define AIPS_PACRN_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC5_SHIFT)) & AIPS_PACRN_AC5_MASK)
#define AIPS_PACRN_RO5_MASK                      (0x800U)
#define AIPS_PACRN_RO5_SHIFT                     (11U)
#define AIPS_PACRN_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO5_SHIFT)) & AIPS_PACRN_RO5_MASK)
#define AIPS_PACRN_AC4_MASK                      (0x7000U)
#define AIPS_PACRN_AC4_SHIFT                     (12U)
#define AIPS_PACRN_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC4_SHIFT)) & AIPS_PACRN_AC4_MASK)
#define AIPS_PACRN_RO4_MASK                      (0x8000U)
#define AIPS_PACRN_RO4_SHIFT                     (15U)
#define AIPS_PACRN_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO4_SHIFT)) & AIPS_PACRN_RO4_MASK)
#define AIPS_PACRN_AC3_MASK                      (0x70000U)
#define AIPS_PACRN_AC3_SHIFT                     (16U)
#define AIPS_PACRN_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC3_SHIFT)) & AIPS_PACRN_AC3_MASK)
#define AIPS_PACRN_RO3_MASK                      (0x80000U)
#define AIPS_PACRN_RO3_SHIFT                     (19U)
#define AIPS_PACRN_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO3_SHIFT)) & AIPS_PACRN_RO3_MASK)
#define AIPS_PACRN_AC2_MASK                      (0x700000U)
#define AIPS_PACRN_AC2_SHIFT                     (20U)
#define AIPS_PACRN_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC2_SHIFT)) & AIPS_PACRN_AC2_MASK)
#define AIPS_PACRN_RO2_MASK                      (0x800000U)
#define AIPS_PACRN_RO2_SHIFT                     (23U)
#define AIPS_PACRN_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO2_SHIFT)) & AIPS_PACRN_RO2_MASK)
#define AIPS_PACRN_AC1_MASK                      (0x7000000U)
#define AIPS_PACRN_AC1_SHIFT                     (24U)
#define AIPS_PACRN_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC1_SHIFT)) & AIPS_PACRN_AC1_MASK)
#define AIPS_PACRN_RO1_MASK                      (0x8000000U)
#define AIPS_PACRN_RO1_SHIFT                     (27U)
#define AIPS_PACRN_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO1_SHIFT)) & AIPS_PACRN_RO1_MASK)
#define AIPS_PACRN_AC0_MASK                      (0x70000000U)
#define AIPS_PACRN_AC0_SHIFT                     (28U)
#define AIPS_PACRN_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_AC0_SHIFT)) & AIPS_PACRN_AC0_MASK)
#define AIPS_PACRN_RO0_MASK                      (0x80000000U)
#define AIPS_PACRN_RO0_SHIFT                     (31U)
#define AIPS_PACRN_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRN_RO0_SHIFT)) & AIPS_PACRN_RO0_MASK)

/*! @name PACRO - Peripheral Access Control Register */
#define AIPS_PACRO_AC7_MASK                      (0x7U)
#define AIPS_PACRO_AC7_SHIFT                     (0U)
#define AIPS_PACRO_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC7_SHIFT)) & AIPS_PACRO_AC7_MASK)
#define AIPS_PACRO_RO7_MASK                      (0x8U)
#define AIPS_PACRO_RO7_SHIFT                     (3U)
#define AIPS_PACRO_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO7_SHIFT)) & AIPS_PACRO_RO7_MASK)
#define AIPS_PACRO_AC6_MASK                      (0x70U)
#define AIPS_PACRO_AC6_SHIFT                     (4U)
#define AIPS_PACRO_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC6_SHIFT)) & AIPS_PACRO_AC6_MASK)
#define AIPS_PACRO_RO6_MASK                      (0x80U)
#define AIPS_PACRO_RO6_SHIFT                     (7U)
#define AIPS_PACRO_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO6_SHIFT)) & AIPS_PACRO_RO6_MASK)
#define AIPS_PACRO_AC5_MASK                      (0x700U)
#define AIPS_PACRO_AC5_SHIFT                     (8U)
#define AIPS_PACRO_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC5_SHIFT)) & AIPS_PACRO_AC5_MASK)
#define AIPS_PACRO_RO5_MASK                      (0x800U)
#define AIPS_PACRO_RO5_SHIFT                     (11U)
#define AIPS_PACRO_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO5_SHIFT)) & AIPS_PACRO_RO5_MASK)
#define AIPS_PACRO_AC4_MASK                      (0x7000U)
#define AIPS_PACRO_AC4_SHIFT                     (12U)
#define AIPS_PACRO_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC4_SHIFT)) & AIPS_PACRO_AC4_MASK)
#define AIPS_PACRO_RO4_MASK                      (0x8000U)
#define AIPS_PACRO_RO4_SHIFT                     (15U)
#define AIPS_PACRO_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO4_SHIFT)) & AIPS_PACRO_RO4_MASK)
#define AIPS_PACRO_AC3_MASK                      (0x70000U)
#define AIPS_PACRO_AC3_SHIFT                     (16U)
#define AIPS_PACRO_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC3_SHIFT)) & AIPS_PACRO_AC3_MASK)
#define AIPS_PACRO_RO3_MASK                      (0x80000U)
#define AIPS_PACRO_RO3_SHIFT                     (19U)
#define AIPS_PACRO_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO3_SHIFT)) & AIPS_PACRO_RO3_MASK)
#define AIPS_PACRO_AC2_MASK                      (0x700000U)
#define AIPS_PACRO_AC2_SHIFT                     (20U)
#define AIPS_PACRO_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC2_SHIFT)) & AIPS_PACRO_AC2_MASK)
#define AIPS_PACRO_RO2_MASK                      (0x800000U)
#define AIPS_PACRO_RO2_SHIFT                     (23U)
#define AIPS_PACRO_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO2_SHIFT)) & AIPS_PACRO_RO2_MASK)
#define AIPS_PACRO_AC1_MASK                      (0x7000000U)
#define AIPS_PACRO_AC1_SHIFT                     (24U)
#define AIPS_PACRO_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC1_SHIFT)) & AIPS_PACRO_AC1_MASK)
#define AIPS_PACRO_RO1_MASK                      (0x8000000U)
#define AIPS_PACRO_RO1_SHIFT                     (27U)
#define AIPS_PACRO_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO1_SHIFT)) & AIPS_PACRO_RO1_MASK)
#define AIPS_PACRO_AC0_MASK                      (0x70000000U)
#define AIPS_PACRO_AC0_SHIFT                     (28U)
#define AIPS_PACRO_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_AC0_SHIFT)) & AIPS_PACRO_AC0_MASK)
#define AIPS_PACRO_RO0_MASK                      (0x80000000U)
#define AIPS_PACRO_RO0_SHIFT                     (31U)
#define AIPS_PACRO_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRO_RO0_SHIFT)) & AIPS_PACRO_RO0_MASK)

/*! @name PACRP - Peripheral Access Control Register */
#define AIPS_PACRP_AC7_MASK                      (0x7U)
#define AIPS_PACRP_AC7_SHIFT                     (0U)
#define AIPS_PACRP_AC7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC7_SHIFT)) & AIPS_PACRP_AC7_MASK)
#define AIPS_PACRP_RO7_MASK                      (0x8U)
#define AIPS_PACRP_RO7_SHIFT                     (3U)
#define AIPS_PACRP_RO7(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO7_SHIFT)) & AIPS_PACRP_RO7_MASK)
#define AIPS_PACRP_AC6_MASK                      (0x70U)
#define AIPS_PACRP_AC6_SHIFT                     (4U)
#define AIPS_PACRP_AC6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC6_SHIFT)) & AIPS_PACRP_AC6_MASK)
#define AIPS_PACRP_RO6_MASK                      (0x80U)
#define AIPS_PACRP_RO6_SHIFT                     (7U)
#define AIPS_PACRP_RO6(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO6_SHIFT)) & AIPS_PACRP_RO6_MASK)
#define AIPS_PACRP_AC5_MASK                      (0x700U)
#define AIPS_PACRP_AC5_SHIFT                     (8U)
#define AIPS_PACRP_AC5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC5_SHIFT)) & AIPS_PACRP_AC5_MASK)
#define AIPS_PACRP_RO5_MASK                      (0x800U)
#define AIPS_PACRP_RO5_SHIFT                     (11U)
#define AIPS_PACRP_RO5(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO5_SHIFT)) & AIPS_PACRP_RO5_MASK)
#define AIPS_PACRP_AC4_MASK                      (0x7000U)
#define AIPS_PACRP_AC4_SHIFT                     (12U)
#define AIPS_PACRP_AC4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC4_SHIFT)) & AIPS_PACRP_AC4_MASK)
#define AIPS_PACRP_RO4_MASK                      (0x8000U)
#define AIPS_PACRP_RO4_SHIFT                     (15U)
#define AIPS_PACRP_RO4(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO4_SHIFT)) & AIPS_PACRP_RO4_MASK)
#define AIPS_PACRP_AC3_MASK                      (0x70000U)
#define AIPS_PACRP_AC3_SHIFT                     (16U)
#define AIPS_PACRP_AC3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC3_SHIFT)) & AIPS_PACRP_AC3_MASK)
#define AIPS_PACRP_RO3_MASK                      (0x80000U)
#define AIPS_PACRP_RO3_SHIFT                     (19U)
#define AIPS_PACRP_RO3(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO3_SHIFT)) & AIPS_PACRP_RO3_MASK)
#define AIPS_PACRP_AC2_MASK                      (0x700000U)
#define AIPS_PACRP_AC2_SHIFT                     (20U)
#define AIPS_PACRP_AC2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC2_SHIFT)) & AIPS_PACRP_AC2_MASK)
#define AIPS_PACRP_RO2_MASK                      (0x800000U)
#define AIPS_PACRP_RO2_SHIFT                     (23U)
#define AIPS_PACRP_RO2(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO2_SHIFT)) & AIPS_PACRP_RO2_MASK)
#define AIPS_PACRP_AC1_MASK                      (0x7000000U)
#define AIPS_PACRP_AC1_SHIFT                     (24U)
#define AIPS_PACRP_AC1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC1_SHIFT)) & AIPS_PACRP_AC1_MASK)
#define AIPS_PACRP_RO1_MASK                      (0x8000000U)
#define AIPS_PACRP_RO1_SHIFT                     (27U)
#define AIPS_PACRP_RO1(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO1_SHIFT)) & AIPS_PACRP_RO1_MASK)
#define AIPS_PACRP_AC0_MASK                      (0x70000000U)
#define AIPS_PACRP_AC0_SHIFT                     (28U)
#define AIPS_PACRP_AC0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_AC0_SHIFT)) & AIPS_PACRP_AC0_MASK)
#define AIPS_PACRP_RO0_MASK                      (0x80000000U)
#define AIPS_PACRP_RO0_SHIFT                     (31U)
#define AIPS_PACRP_RO0(x)                        (((uint32_t)(((uint32_t)(x)) << AIPS_PACRP_RO0_SHIFT)) & AIPS_PACRP_RO0_MASK)


/*!
 * @}
 */ /* end of group AIPS_Register_Masks */


/* AIPS - Peripheral instance base addresses */
/** Peripheral AIPS base address */
#define AIPS_BASE                                (0x40000000u)
/** Peripheral AIPS base pointer */
#define AIPS                                     ((AIPS_Type *)AIPS_BASE)
/** Array initializer of AIPS peripheral base addresses */
#define AIPS_BASE_ADDRS                          { AIPS_BASE }
/** Array initializer of AIPS peripheral base pointers */
#define AIPS_BASE_PTRS                           { AIPS }

/*!
 * @}
 */ /* end of group AIPS_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- CAU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CAU_Peripheral_Access_Layer CAU Peripheral Access Layer
 * @{
 */

/** CAU - Register Layout Typedef */
typedef struct {
  __O  uint32_t DIRECT[16];                        /**< Direct access register 0..Direct access register 15, array offset: 0x0, array step: 0x4 */
       uint8_t RESERVED_0[2048];
  __O  uint32_t LDR_CASR;                          /**< Status register - Load Register command, offset: 0x840 */
  __O  uint32_t LDR_CAA;                           /**< Accumulator register - Load Register command, offset: 0x844 */
  __O  uint32_t LDR_CA[9];                         /**< General Purpose Register 0 - Load Register command..General Purpose Register 8 - Load Register command, array offset: 0x848, array step: 0x4 */
       uint8_t RESERVED_1[20];
  __I  uint32_t STR_CASR;                          /**< Status register - Store Register command, offset: 0x880 */
  __I  uint32_t STR_CAA;                           /**< Accumulator register - Store Register command, offset: 0x884 */
  __I  uint32_t STR_CA[9];                         /**< General Purpose Register 0 - Store Register command..General Purpose Register 8 - Store Register command, array offset: 0x888, array step: 0x4 */
       uint8_t RESERVED_2[20];
  __O  uint32_t ADR_CASR;                          /**< Status register - Add Register command, offset: 0x8C0 */
  __O  uint32_t ADR_CAA;                           /**< Accumulator register - Add to register command, offset: 0x8C4 */
  __O  uint32_t ADR_CA[9];                         /**< General Purpose Register 0 - Add to register command..General Purpose Register 8 - Add to register command, array offset: 0x8C8, array step: 0x4 */
       uint8_t RESERVED_3[20];
  __O  uint32_t RADR_CASR;                         /**< Status register - Reverse and Add to Register command, offset: 0x900 */
  __O  uint32_t RADR_CAA;                          /**< Accumulator register - Reverse and Add to Register command, offset: 0x904 */
  __O  uint32_t RADR_CA[9];                        /**< General Purpose Register 0 - Reverse and Add to Register command..General Purpose Register 8 - Reverse and Add to Register command, array offset: 0x908, array step: 0x4 */
       uint8_t RESERVED_4[84];
  __O  uint32_t XOR_CASR;                          /**< Status register - Exclusive Or command, offset: 0x980 */
  __O  uint32_t XOR_CAA;                           /**< Accumulator register - Exclusive Or command, offset: 0x984 */
  __O  uint32_t XOR_CA[9];                         /**< General Purpose Register 0 - Exclusive Or command..General Purpose Register 8 - Exclusive Or command, array offset: 0x988, array step: 0x4 */
       uint8_t RESERVED_5[20];
  __O  uint32_t ROTL_CASR;                         /**< Status register - Rotate Left command, offset: 0x9C0 */
  __O  uint32_t ROTL_CAA;                          /**< Accumulator register - Rotate Left command, offset: 0x9C4 */
  __O  uint32_t ROTL_CA[9];                        /**< General Purpose Register 0 - Rotate Left command..General Purpose Register 8 - Rotate Left command, array offset: 0x9C8, array step: 0x4 */
       uint8_t RESERVED_6[276];
  __O  uint32_t AESC_CASR;                         /**< Status register - AES Column Operation command, offset: 0xB00 */
  __O  uint32_t AESC_CAA;                          /**< Accumulator register - AES Column Operation command, offset: 0xB04 */
  __O  uint32_t AESC_CA[9];                        /**< General Purpose Register 0 - AES Column Operation command..General Purpose Register 8 - AES Column Operation command, array offset: 0xB08, array step: 0x4 */
       uint8_t RESERVED_7[20];
  __O  uint32_t AESIC_CASR;                        /**< Status register - AES Inverse Column Operation command, offset: 0xB40 */
  __O  uint32_t AESIC_CAA;                         /**< Accumulator register - AES Inverse Column Operation command, offset: 0xB44 */
  __O  uint32_t AESIC_CA[9];                       /**< General Purpose Register 0 - AES Inverse Column Operation command..General Purpose Register 8 - AES Inverse Column Operation command, array offset: 0xB48, array step: 0x4 */
} CAU_Type;

/* ----------------------------------------------------------------------------
   -- CAU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CAU_Register_Masks CAU Register Masks
 * @{
 */

/*! @name DIRECT - Direct access register 0..Direct access register 15 */
#define CAU_DIRECT_CAU_DIRECT0_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT0_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT0(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT0_SHIFT)) & CAU_DIRECT_CAU_DIRECT0_MASK)
#define CAU_DIRECT_CAU_DIRECT1_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT1_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT1(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT1_SHIFT)) & CAU_DIRECT_CAU_DIRECT1_MASK)
#define CAU_DIRECT_CAU_DIRECT2_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT2_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT2(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT2_SHIFT)) & CAU_DIRECT_CAU_DIRECT2_MASK)
#define CAU_DIRECT_CAU_DIRECT3_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT3_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT3(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT3_SHIFT)) & CAU_DIRECT_CAU_DIRECT3_MASK)
#define CAU_DIRECT_CAU_DIRECT4_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT4_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT4(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT4_SHIFT)) & CAU_DIRECT_CAU_DIRECT4_MASK)
#define CAU_DIRECT_CAU_DIRECT5_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT5_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT5(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT5_SHIFT)) & CAU_DIRECT_CAU_DIRECT5_MASK)
#define CAU_DIRECT_CAU_DIRECT6_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT6_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT6(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT6_SHIFT)) & CAU_DIRECT_CAU_DIRECT6_MASK)
#define CAU_DIRECT_CAU_DIRECT7_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT7_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT7(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT7_SHIFT)) & CAU_DIRECT_CAU_DIRECT7_MASK)
#define CAU_DIRECT_CAU_DIRECT8_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT8_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT8(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT8_SHIFT)) & CAU_DIRECT_CAU_DIRECT8_MASK)
#define CAU_DIRECT_CAU_DIRECT9_MASK              (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT9_SHIFT             (0U)
#define CAU_DIRECT_CAU_DIRECT9(x)                (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT9_SHIFT)) & CAU_DIRECT_CAU_DIRECT9_MASK)
#define CAU_DIRECT_CAU_DIRECT10_MASK             (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT10_SHIFT            (0U)
#define CAU_DIRECT_CAU_DIRECT10(x)               (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT10_SHIFT)) & CAU_DIRECT_CAU_DIRECT10_MASK)
#define CAU_DIRECT_CAU_DIRECT11_MASK             (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT11_SHIFT            (0U)
#define CAU_DIRECT_CAU_DIRECT11(x)               (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT11_SHIFT)) & CAU_DIRECT_CAU_DIRECT11_MASK)
#define CAU_DIRECT_CAU_DIRECT12_MASK             (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT12_SHIFT            (0U)
#define CAU_DIRECT_CAU_DIRECT12(x)               (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT12_SHIFT)) & CAU_DIRECT_CAU_DIRECT12_MASK)
#define CAU_DIRECT_CAU_DIRECT13_MASK             (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT13_SHIFT            (0U)
#define CAU_DIRECT_CAU_DIRECT13(x)               (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT13_SHIFT)) & CAU_DIRECT_CAU_DIRECT13_MASK)
#define CAU_DIRECT_CAU_DIRECT14_MASK             (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT14_SHIFT            (0U)
#define CAU_DIRECT_CAU_DIRECT14(x)               (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT14_SHIFT)) & CAU_DIRECT_CAU_DIRECT14_MASK)
#define CAU_DIRECT_CAU_DIRECT15_MASK             (0xFFFFFFFFU)
#define CAU_DIRECT_CAU_DIRECT15_SHIFT            (0U)
#define CAU_DIRECT_CAU_DIRECT15(x)               (((uint32_t)(((uint32_t)(x)) << CAU_DIRECT_CAU_DIRECT15_SHIFT)) & CAU_DIRECT_CAU_DIRECT15_MASK)

/* The count of CAU_DIRECT */
#define CAU_DIRECT_COUNT                         (16U)

/*! @name LDR_CASR - Status register - Load Register command */
#define CAU_LDR_CASR_IC_MASK                     (0x1U)
#define CAU_LDR_CASR_IC_SHIFT                    (0U)
#define CAU_LDR_CASR_IC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CASR_IC_SHIFT)) & CAU_LDR_CASR_IC_MASK)
#define CAU_LDR_CASR_DPE_MASK                    (0x2U)
#define CAU_LDR_CASR_DPE_SHIFT                   (1U)
#define CAU_LDR_CASR_DPE(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CASR_DPE_SHIFT)) & CAU_LDR_CASR_DPE_MASK)
#define CAU_LDR_CASR_VER_MASK                    (0xF0000000U)
#define CAU_LDR_CASR_VER_SHIFT                   (28U)
#define CAU_LDR_CASR_VER(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CASR_VER_SHIFT)) & CAU_LDR_CASR_VER_MASK)

/*! @name LDR_CAA - Accumulator register - Load Register command */
#define CAU_LDR_CAA_ACC_MASK                     (0xFFFFFFFFU)
#define CAU_LDR_CAA_ACC_SHIFT                    (0U)
#define CAU_LDR_CAA_ACC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CAA_ACC_SHIFT)) & CAU_LDR_CAA_ACC_MASK)

/*! @name LDR_CA - General Purpose Register 0 - Load Register command..General Purpose Register 8 - Load Register command */
#define CAU_LDR_CA_CA0_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA0_SHIFT                     (0U)
#define CAU_LDR_CA_CA0(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA0_SHIFT)) & CAU_LDR_CA_CA0_MASK)
#define CAU_LDR_CA_CA1_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA1_SHIFT                     (0U)
#define CAU_LDR_CA_CA1(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA1_SHIFT)) & CAU_LDR_CA_CA1_MASK)
#define CAU_LDR_CA_CA2_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA2_SHIFT                     (0U)
#define CAU_LDR_CA_CA2(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA2_SHIFT)) & CAU_LDR_CA_CA2_MASK)
#define CAU_LDR_CA_CA3_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA3_SHIFT                     (0U)
#define CAU_LDR_CA_CA3(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA3_SHIFT)) & CAU_LDR_CA_CA3_MASK)
#define CAU_LDR_CA_CA4_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA4_SHIFT                     (0U)
#define CAU_LDR_CA_CA4(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA4_SHIFT)) & CAU_LDR_CA_CA4_MASK)
#define CAU_LDR_CA_CA5_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA5_SHIFT                     (0U)
#define CAU_LDR_CA_CA5(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA5_SHIFT)) & CAU_LDR_CA_CA5_MASK)
#define CAU_LDR_CA_CA6_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA6_SHIFT                     (0U)
#define CAU_LDR_CA_CA6(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA6_SHIFT)) & CAU_LDR_CA_CA6_MASK)
#define CAU_LDR_CA_CA7_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA7_SHIFT                     (0U)
#define CAU_LDR_CA_CA7(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA7_SHIFT)) & CAU_LDR_CA_CA7_MASK)
#define CAU_LDR_CA_CA8_MASK                      (0xFFFFFFFFU)
#define CAU_LDR_CA_CA8_SHIFT                     (0U)
#define CAU_LDR_CA_CA8(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_LDR_CA_CA8_SHIFT)) & CAU_LDR_CA_CA8_MASK)

/* The count of CAU_LDR_CA */
#define CAU_LDR_CA_COUNT                         (9U)

/*! @name STR_CASR - Status register - Store Register command */
#define CAU_STR_CASR_IC_MASK                     (0x1U)
#define CAU_STR_CASR_IC_SHIFT                    (0U)
#define CAU_STR_CASR_IC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_STR_CASR_IC_SHIFT)) & CAU_STR_CASR_IC_MASK)
#define CAU_STR_CASR_DPE_MASK                    (0x2U)
#define CAU_STR_CASR_DPE_SHIFT                   (1U)
#define CAU_STR_CASR_DPE(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_STR_CASR_DPE_SHIFT)) & CAU_STR_CASR_DPE_MASK)
#define CAU_STR_CASR_VER_MASK                    (0xF0000000U)
#define CAU_STR_CASR_VER_SHIFT                   (28U)
#define CAU_STR_CASR_VER(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_STR_CASR_VER_SHIFT)) & CAU_STR_CASR_VER_MASK)

/*! @name STR_CAA - Accumulator register - Store Register command */
#define CAU_STR_CAA_ACC_MASK                     (0xFFFFFFFFU)
#define CAU_STR_CAA_ACC_SHIFT                    (0U)
#define CAU_STR_CAA_ACC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_STR_CAA_ACC_SHIFT)) & CAU_STR_CAA_ACC_MASK)

/*! @name STR_CA - General Purpose Register 0 - Store Register command..General Purpose Register 8 - Store Register command */
#define CAU_STR_CA_CA0_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA0_SHIFT                     (0U)
#define CAU_STR_CA_CA0(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA0_SHIFT)) & CAU_STR_CA_CA0_MASK)
#define CAU_STR_CA_CA1_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA1_SHIFT                     (0U)
#define CAU_STR_CA_CA1(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA1_SHIFT)) & CAU_STR_CA_CA1_MASK)
#define CAU_STR_CA_CA2_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA2_SHIFT                     (0U)
#define CAU_STR_CA_CA2(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA2_SHIFT)) & CAU_STR_CA_CA2_MASK)
#define CAU_STR_CA_CA3_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA3_SHIFT                     (0U)
#define CAU_STR_CA_CA3(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA3_SHIFT)) & CAU_STR_CA_CA3_MASK)
#define CAU_STR_CA_CA4_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA4_SHIFT                     (0U)
#define CAU_STR_CA_CA4(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA4_SHIFT)) & CAU_STR_CA_CA4_MASK)
#define CAU_STR_CA_CA5_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA5_SHIFT                     (0U)
#define CAU_STR_CA_CA5(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA5_SHIFT)) & CAU_STR_CA_CA5_MASK)
#define CAU_STR_CA_CA6_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA6_SHIFT                     (0U)
#define CAU_STR_CA_CA6(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA6_SHIFT)) & CAU_STR_CA_CA6_MASK)
#define CAU_STR_CA_CA7_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA7_SHIFT                     (0U)
#define CAU_STR_CA_CA7(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA7_SHIFT)) & CAU_STR_CA_CA7_MASK)
#define CAU_STR_CA_CA8_MASK                      (0xFFFFFFFFU)
#define CAU_STR_CA_CA8_SHIFT                     (0U)
#define CAU_STR_CA_CA8(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_STR_CA_CA8_SHIFT)) & CAU_STR_CA_CA8_MASK)

/* The count of CAU_STR_CA */
#define CAU_STR_CA_COUNT                         (9U)

/*! @name ADR_CASR - Status register - Add Register command */
#define CAU_ADR_CASR_IC_MASK                     (0x1U)
#define CAU_ADR_CASR_IC_SHIFT                    (0U)
#define CAU_ADR_CASR_IC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CASR_IC_SHIFT)) & CAU_ADR_CASR_IC_MASK)
#define CAU_ADR_CASR_DPE_MASK                    (0x2U)
#define CAU_ADR_CASR_DPE_SHIFT                   (1U)
#define CAU_ADR_CASR_DPE(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CASR_DPE_SHIFT)) & CAU_ADR_CASR_DPE_MASK)
#define CAU_ADR_CASR_VER_MASK                    (0xF0000000U)
#define CAU_ADR_CASR_VER_SHIFT                   (28U)
#define CAU_ADR_CASR_VER(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CASR_VER_SHIFT)) & CAU_ADR_CASR_VER_MASK)

/*! @name ADR_CAA - Accumulator register - Add to register command */
#define CAU_ADR_CAA_ACC_MASK                     (0xFFFFFFFFU)
#define CAU_ADR_CAA_ACC_SHIFT                    (0U)
#define CAU_ADR_CAA_ACC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CAA_ACC_SHIFT)) & CAU_ADR_CAA_ACC_MASK)

/*! @name ADR_CA - General Purpose Register 0 - Add to register command..General Purpose Register 8 - Add to register command */
#define CAU_ADR_CA_CA0_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA0_SHIFT                     (0U)
#define CAU_ADR_CA_CA0(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA0_SHIFT)) & CAU_ADR_CA_CA0_MASK)
#define CAU_ADR_CA_CA1_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA1_SHIFT                     (0U)
#define CAU_ADR_CA_CA1(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA1_SHIFT)) & CAU_ADR_CA_CA1_MASK)
#define CAU_ADR_CA_CA2_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA2_SHIFT                     (0U)
#define CAU_ADR_CA_CA2(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA2_SHIFT)) & CAU_ADR_CA_CA2_MASK)
#define CAU_ADR_CA_CA3_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA3_SHIFT                     (0U)
#define CAU_ADR_CA_CA3(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA3_SHIFT)) & CAU_ADR_CA_CA3_MASK)
#define CAU_ADR_CA_CA4_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA4_SHIFT                     (0U)
#define CAU_ADR_CA_CA4(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA4_SHIFT)) & CAU_ADR_CA_CA4_MASK)
#define CAU_ADR_CA_CA5_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA5_SHIFT                     (0U)
#define CAU_ADR_CA_CA5(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA5_SHIFT)) & CAU_ADR_CA_CA5_MASK)
#define CAU_ADR_CA_CA6_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA6_SHIFT                     (0U)
#define CAU_ADR_CA_CA6(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA6_SHIFT)) & CAU_ADR_CA_CA6_MASK)
#define CAU_ADR_CA_CA7_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA7_SHIFT                     (0U)
#define CAU_ADR_CA_CA7(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA7_SHIFT)) & CAU_ADR_CA_CA7_MASK)
#define CAU_ADR_CA_CA8_MASK                      (0xFFFFFFFFU)
#define CAU_ADR_CA_CA8_SHIFT                     (0U)
#define CAU_ADR_CA_CA8(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_ADR_CA_CA8_SHIFT)) & CAU_ADR_CA_CA8_MASK)

/* The count of CAU_ADR_CA */
#define CAU_ADR_CA_COUNT                         (9U)

/*! @name RADR_CASR - Status register - Reverse and Add to Register command */
#define CAU_RADR_CASR_IC_MASK                    (0x1U)
#define CAU_RADR_CASR_IC_SHIFT                   (0U)
#define CAU_RADR_CASR_IC(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CASR_IC_SHIFT)) & CAU_RADR_CASR_IC_MASK)
#define CAU_RADR_CASR_DPE_MASK                   (0x2U)
#define CAU_RADR_CASR_DPE_SHIFT                  (1U)
#define CAU_RADR_CASR_DPE(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CASR_DPE_SHIFT)) & CAU_RADR_CASR_DPE_MASK)
#define CAU_RADR_CASR_VER_MASK                   (0xF0000000U)
#define CAU_RADR_CASR_VER_SHIFT                  (28U)
#define CAU_RADR_CASR_VER(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CASR_VER_SHIFT)) & CAU_RADR_CASR_VER_MASK)

/*! @name RADR_CAA - Accumulator register - Reverse and Add to Register command */
#define CAU_RADR_CAA_ACC_MASK                    (0xFFFFFFFFU)
#define CAU_RADR_CAA_ACC_SHIFT                   (0U)
#define CAU_RADR_CAA_ACC(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CAA_ACC_SHIFT)) & CAU_RADR_CAA_ACC_MASK)

/*! @name RADR_CA - General Purpose Register 0 - Reverse and Add to Register command..General Purpose Register 8 - Reverse and Add to Register command */
#define CAU_RADR_CA_CA0_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA0_SHIFT                    (0U)
#define CAU_RADR_CA_CA0(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA0_SHIFT)) & CAU_RADR_CA_CA0_MASK)
#define CAU_RADR_CA_CA1_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA1_SHIFT                    (0U)
#define CAU_RADR_CA_CA1(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA1_SHIFT)) & CAU_RADR_CA_CA1_MASK)
#define CAU_RADR_CA_CA2_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA2_SHIFT                    (0U)
#define CAU_RADR_CA_CA2(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA2_SHIFT)) & CAU_RADR_CA_CA2_MASK)
#define CAU_RADR_CA_CA3_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA3_SHIFT                    (0U)
#define CAU_RADR_CA_CA3(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA3_SHIFT)) & CAU_RADR_CA_CA3_MASK)
#define CAU_RADR_CA_CA4_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA4_SHIFT                    (0U)
#define CAU_RADR_CA_CA4(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA4_SHIFT)) & CAU_RADR_CA_CA4_MASK)
#define CAU_RADR_CA_CA5_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA5_SHIFT                    (0U)
#define CAU_RADR_CA_CA5(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA5_SHIFT)) & CAU_RADR_CA_CA5_MASK)
#define CAU_RADR_CA_CA6_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA6_SHIFT                    (0U)
#define CAU_RADR_CA_CA6(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA6_SHIFT)) & CAU_RADR_CA_CA6_MASK)
#define CAU_RADR_CA_CA7_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA7_SHIFT                    (0U)
#define CAU_RADR_CA_CA7(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA7_SHIFT)) & CAU_RADR_CA_CA7_MASK)
#define CAU_RADR_CA_CA8_MASK                     (0xFFFFFFFFU)
#define CAU_RADR_CA_CA8_SHIFT                    (0U)
#define CAU_RADR_CA_CA8(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_RADR_CA_CA8_SHIFT)) & CAU_RADR_CA_CA8_MASK)

/* The count of CAU_RADR_CA */
#define CAU_RADR_CA_COUNT                        (9U)

/*! @name XOR_CASR - Status register - Exclusive Or command */
#define CAU_XOR_CASR_IC_MASK                     (0x1U)
#define CAU_XOR_CASR_IC_SHIFT                    (0U)
#define CAU_XOR_CASR_IC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CASR_IC_SHIFT)) & CAU_XOR_CASR_IC_MASK)
#define CAU_XOR_CASR_DPE_MASK                    (0x2U)
#define CAU_XOR_CASR_DPE_SHIFT                   (1U)
#define CAU_XOR_CASR_DPE(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CASR_DPE_SHIFT)) & CAU_XOR_CASR_DPE_MASK)
#define CAU_XOR_CASR_VER_MASK                    (0xF0000000U)
#define CAU_XOR_CASR_VER_SHIFT                   (28U)
#define CAU_XOR_CASR_VER(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CASR_VER_SHIFT)) & CAU_XOR_CASR_VER_MASK)

/*! @name XOR_CAA - Accumulator register - Exclusive Or command */
#define CAU_XOR_CAA_ACC_MASK                     (0xFFFFFFFFU)
#define CAU_XOR_CAA_ACC_SHIFT                    (0U)
#define CAU_XOR_CAA_ACC(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CAA_ACC_SHIFT)) & CAU_XOR_CAA_ACC_MASK)

/*! @name XOR_CA - General Purpose Register 0 - Exclusive Or command..General Purpose Register 8 - Exclusive Or command */
#define CAU_XOR_CA_CA0_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA0_SHIFT                     (0U)
#define CAU_XOR_CA_CA0(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA0_SHIFT)) & CAU_XOR_CA_CA0_MASK)
#define CAU_XOR_CA_CA1_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA1_SHIFT                     (0U)
#define CAU_XOR_CA_CA1(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA1_SHIFT)) & CAU_XOR_CA_CA1_MASK)
#define CAU_XOR_CA_CA2_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA2_SHIFT                     (0U)
#define CAU_XOR_CA_CA2(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA2_SHIFT)) & CAU_XOR_CA_CA2_MASK)
#define CAU_XOR_CA_CA3_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA3_SHIFT                     (0U)
#define CAU_XOR_CA_CA3(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA3_SHIFT)) & CAU_XOR_CA_CA3_MASK)
#define CAU_XOR_CA_CA4_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA4_SHIFT                     (0U)
#define CAU_XOR_CA_CA4(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA4_SHIFT)) & CAU_XOR_CA_CA4_MASK)
#define CAU_XOR_CA_CA5_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA5_SHIFT                     (0U)
#define CAU_XOR_CA_CA5(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA5_SHIFT)) & CAU_XOR_CA_CA5_MASK)
#define CAU_XOR_CA_CA6_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA6_SHIFT                     (0U)
#define CAU_XOR_CA_CA6(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA6_SHIFT)) & CAU_XOR_CA_CA6_MASK)
#define CAU_XOR_CA_CA7_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA7_SHIFT                     (0U)
#define CAU_XOR_CA_CA7(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA7_SHIFT)) & CAU_XOR_CA_CA7_MASK)
#define CAU_XOR_CA_CA8_MASK                      (0xFFFFFFFFU)
#define CAU_XOR_CA_CA8_SHIFT                     (0U)
#define CAU_XOR_CA_CA8(x)                        (((uint32_t)(((uint32_t)(x)) << CAU_XOR_CA_CA8_SHIFT)) & CAU_XOR_CA_CA8_MASK)

/* The count of CAU_XOR_CA */
#define CAU_XOR_CA_COUNT                         (9U)

/*! @name ROTL_CASR - Status register - Rotate Left command */
#define CAU_ROTL_CASR_IC_MASK                    (0x1U)
#define CAU_ROTL_CASR_IC_SHIFT                   (0U)
#define CAU_ROTL_CASR_IC(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CASR_IC_SHIFT)) & CAU_ROTL_CASR_IC_MASK)
#define CAU_ROTL_CASR_DPE_MASK                   (0x2U)
#define CAU_ROTL_CASR_DPE_SHIFT                  (1U)
#define CAU_ROTL_CASR_DPE(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CASR_DPE_SHIFT)) & CAU_ROTL_CASR_DPE_MASK)
#define CAU_ROTL_CASR_VER_MASK                   (0xF0000000U)
#define CAU_ROTL_CASR_VER_SHIFT                  (28U)
#define CAU_ROTL_CASR_VER(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CASR_VER_SHIFT)) & CAU_ROTL_CASR_VER_MASK)

/*! @name ROTL_CAA - Accumulator register - Rotate Left command */
#define CAU_ROTL_CAA_ACC_MASK                    (0xFFFFFFFFU)
#define CAU_ROTL_CAA_ACC_SHIFT                   (0U)
#define CAU_ROTL_CAA_ACC(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CAA_ACC_SHIFT)) & CAU_ROTL_CAA_ACC_MASK)

/*! @name ROTL_CA - General Purpose Register 0 - Rotate Left command..General Purpose Register 8 - Rotate Left command */
#define CAU_ROTL_CA_CA0_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA0_SHIFT                    (0U)
#define CAU_ROTL_CA_CA0(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA0_SHIFT)) & CAU_ROTL_CA_CA0_MASK)
#define CAU_ROTL_CA_CA1_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA1_SHIFT                    (0U)
#define CAU_ROTL_CA_CA1(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA1_SHIFT)) & CAU_ROTL_CA_CA1_MASK)
#define CAU_ROTL_CA_CA2_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA2_SHIFT                    (0U)
#define CAU_ROTL_CA_CA2(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA2_SHIFT)) & CAU_ROTL_CA_CA2_MASK)
#define CAU_ROTL_CA_CA3_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA3_SHIFT                    (0U)
#define CAU_ROTL_CA_CA3(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA3_SHIFT)) & CAU_ROTL_CA_CA3_MASK)
#define CAU_ROTL_CA_CA4_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA4_SHIFT                    (0U)
#define CAU_ROTL_CA_CA4(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA4_SHIFT)) & CAU_ROTL_CA_CA4_MASK)
#define CAU_ROTL_CA_CA5_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA5_SHIFT                    (0U)
#define CAU_ROTL_CA_CA5(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA5_SHIFT)) & CAU_ROTL_CA_CA5_MASK)
#define CAU_ROTL_CA_CA6_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA6_SHIFT                    (0U)
#define CAU_ROTL_CA_CA6(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA6_SHIFT)) & CAU_ROTL_CA_CA6_MASK)
#define CAU_ROTL_CA_CA7_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA7_SHIFT                    (0U)
#define CAU_ROTL_CA_CA7(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA7_SHIFT)) & CAU_ROTL_CA_CA7_MASK)
#define CAU_ROTL_CA_CA8_MASK                     (0xFFFFFFFFU)
#define CAU_ROTL_CA_CA8_SHIFT                    (0U)
#define CAU_ROTL_CA_CA8(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_ROTL_CA_CA8_SHIFT)) & CAU_ROTL_CA_CA8_MASK)

/* The count of CAU_ROTL_CA */
#define CAU_ROTL_CA_COUNT                        (9U)

/*! @name AESC_CASR - Status register - AES Column Operation command */
#define CAU_AESC_CASR_IC_MASK                    (0x1U)
#define CAU_AESC_CASR_IC_SHIFT                   (0U)
#define CAU_AESC_CASR_IC(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CASR_IC_SHIFT)) & CAU_AESC_CASR_IC_MASK)
#define CAU_AESC_CASR_DPE_MASK                   (0x2U)
#define CAU_AESC_CASR_DPE_SHIFT                  (1U)
#define CAU_AESC_CASR_DPE(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CASR_DPE_SHIFT)) & CAU_AESC_CASR_DPE_MASK)
#define CAU_AESC_CASR_VER_MASK                   (0xF0000000U)
#define CAU_AESC_CASR_VER_SHIFT                  (28U)
#define CAU_AESC_CASR_VER(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CASR_VER_SHIFT)) & CAU_AESC_CASR_VER_MASK)

/*! @name AESC_CAA - Accumulator register - AES Column Operation command */
#define CAU_AESC_CAA_ACC_MASK                    (0xFFFFFFFFU)
#define CAU_AESC_CAA_ACC_SHIFT                   (0U)
#define CAU_AESC_CAA_ACC(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CAA_ACC_SHIFT)) & CAU_AESC_CAA_ACC_MASK)

/*! @name AESC_CA - General Purpose Register 0 - AES Column Operation command..General Purpose Register 8 - AES Column Operation command */
#define CAU_AESC_CA_CA0_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA0_SHIFT                    (0U)
#define CAU_AESC_CA_CA0(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA0_SHIFT)) & CAU_AESC_CA_CA0_MASK)
#define CAU_AESC_CA_CA1_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA1_SHIFT                    (0U)
#define CAU_AESC_CA_CA1(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA1_SHIFT)) & CAU_AESC_CA_CA1_MASK)
#define CAU_AESC_CA_CA2_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA2_SHIFT                    (0U)
#define CAU_AESC_CA_CA2(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA2_SHIFT)) & CAU_AESC_CA_CA2_MASK)
#define CAU_AESC_CA_CA3_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA3_SHIFT                    (0U)
#define CAU_AESC_CA_CA3(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA3_SHIFT)) & CAU_AESC_CA_CA3_MASK)
#define CAU_AESC_CA_CA4_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA4_SHIFT                    (0U)
#define CAU_AESC_CA_CA4(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA4_SHIFT)) & CAU_AESC_CA_CA4_MASK)
#define CAU_AESC_CA_CA5_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA5_SHIFT                    (0U)
#define CAU_AESC_CA_CA5(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA5_SHIFT)) & CAU_AESC_CA_CA5_MASK)
#define CAU_AESC_CA_CA6_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA6_SHIFT                    (0U)
#define CAU_AESC_CA_CA6(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA6_SHIFT)) & CAU_AESC_CA_CA6_MASK)
#define CAU_AESC_CA_CA7_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA7_SHIFT                    (0U)
#define CAU_AESC_CA_CA7(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA7_SHIFT)) & CAU_AESC_CA_CA7_MASK)
#define CAU_AESC_CA_CA8_MASK                     (0xFFFFFFFFU)
#define CAU_AESC_CA_CA8_SHIFT                    (0U)
#define CAU_AESC_CA_CA8(x)                       (((uint32_t)(((uint32_t)(x)) << CAU_AESC_CA_CA8_SHIFT)) & CAU_AESC_CA_CA8_MASK)

/* The count of CAU_AESC_CA */
#define CAU_AESC_CA_COUNT                        (9U)

/*! @name AESIC_CASR - Status register - AES Inverse Column Operation command */
#define CAU_AESIC_CASR_IC_MASK                   (0x1U)
#define CAU_AESIC_CASR_IC_SHIFT                  (0U)
#define CAU_AESIC_CASR_IC(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CASR_IC_SHIFT)) & CAU_AESIC_CASR_IC_MASK)
#define CAU_AESIC_CASR_DPE_MASK                  (0x2U)
#define CAU_AESIC_CASR_DPE_SHIFT                 (1U)
#define CAU_AESIC_CASR_DPE(x)                    (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CASR_DPE_SHIFT)) & CAU_AESIC_CASR_DPE_MASK)
#define CAU_AESIC_CASR_VER_MASK                  (0xF0000000U)
#define CAU_AESIC_CASR_VER_SHIFT                 (28U)
#define CAU_AESIC_CASR_VER(x)                    (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CASR_VER_SHIFT)) & CAU_AESIC_CASR_VER_MASK)

/*! @name AESIC_CAA - Accumulator register - AES Inverse Column Operation command */
#define CAU_AESIC_CAA_ACC_MASK                   (0xFFFFFFFFU)
#define CAU_AESIC_CAA_ACC_SHIFT                  (0U)
#define CAU_AESIC_CAA_ACC(x)                     (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CAA_ACC_SHIFT)) & CAU_AESIC_CAA_ACC_MASK)

/*! @name AESIC_CA - General Purpose Register 0 - AES Inverse Column Operation command..General Purpose Register 8 - AES Inverse Column Operation command */
#define CAU_AESIC_CA_CA0_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA0_SHIFT                   (0U)
#define CAU_AESIC_CA_CA0(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA0_SHIFT)) & CAU_AESIC_CA_CA0_MASK)
#define CAU_AESIC_CA_CA1_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA1_SHIFT                   (0U)
#define CAU_AESIC_CA_CA1(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA1_SHIFT)) & CAU_AESIC_CA_CA1_MASK)
#define CAU_AESIC_CA_CA2_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA2_SHIFT                   (0U)
#define CAU_AESIC_CA_CA2(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA2_SHIFT)) & CAU_AESIC_CA_CA2_MASK)
#define CAU_AESIC_CA_CA3_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA3_SHIFT                   (0U)
#define CAU_AESIC_CA_CA3(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA3_SHIFT)) & CAU_AESIC_CA_CA3_MASK)
#define CAU_AESIC_CA_CA4_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA4_SHIFT                   (0U)
#define CAU_AESIC_CA_CA4(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA4_SHIFT)) & CAU_AESIC_CA_CA4_MASK)
#define CAU_AESIC_CA_CA5_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA5_SHIFT                   (0U)
#define CAU_AESIC_CA_CA5(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA5_SHIFT)) & CAU_AESIC_CA_CA5_MASK)
#define CAU_AESIC_CA_CA6_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA6_SHIFT                   (0U)
#define CAU_AESIC_CA_CA6(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA6_SHIFT)) & CAU_AESIC_CA_CA6_MASK)
#define CAU_AESIC_CA_CA7_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA7_SHIFT                   (0U)
#define CAU_AESIC_CA_CA7(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA7_SHIFT)) & CAU_AESIC_CA_CA7_MASK)
#define CAU_AESIC_CA_CA8_MASK                    (0xFFFFFFFFU)
#define CAU_AESIC_CA_CA8_SHIFT                   (0U)
#define CAU_AESIC_CA_CA8(x)                      (((uint32_t)(((uint32_t)(x)) << CAU_AESIC_CA_CA8_SHIFT)) & CAU_AESIC_CA_CA8_MASK)

/* The count of CAU_AESIC_CA */
#define CAU_AESIC_CA_COUNT                       (9U)


/*!
 * @}
 */ /* end of group CAU_Register_Masks */


/* CAU - Peripheral instance base addresses */
/** Peripheral CAU base address */
#define CAU_BASE                                 (0xF0005000u)
/** Peripheral CAU base pointer */
#define CAU                                      ((CAU_Type *)CAU_BASE)
/** Array initializer of CAU peripheral base addresses */
#define CAU_BASE_ADDRS                           { CAU_BASE }
/** Array initializer of CAU peripheral base pointers */
#define CAU_BASE_PTRS                            { CAU }

/*!
 * @}
 */ /* end of group CAU_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- CMP Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Peripheral_Access_Layer CMP Peripheral Access Layer
 * @{
 */

/** CMP - Register Layout Typedef */
typedef struct {
  __IO uint8_t CR0;                                /**< CMP Control Register 0, offset: 0x0 */
  __IO uint8_t CR1;                                /**< CMP Control Register 1, offset: 0x1 */
  __IO uint8_t FPR;                                /**< CMP Filter Period Register, offset: 0x2 */
  __IO uint8_t SCR;                                /**< CMP Status and Control Register, offset: 0x3 */
  __IO uint8_t DACCR;                              /**< DAC Control Register, offset: 0x4 */
  __IO uint8_t MUXCR;                              /**< MUX Control Register, offset: 0x5 */
} CMP_Type;

/* ----------------------------------------------------------------------------
   -- CMP Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CMP_Register_Masks CMP Register Masks
 * @{
 */

/*! @name CR0 - CMP Control Register 0 */
#define CMP_CR0_HYSTCTR_MASK                     (0x3U)
#define CMP_CR0_HYSTCTR_SHIFT                    (0U)
#define CMP_CR0_HYSTCTR(x)                       (((uint8_t)(((uint8_t)(x)) << CMP_CR0_HYSTCTR_SHIFT)) & CMP_CR0_HYSTCTR_MASK)
#define CMP_CR0_FILTER_CNT_MASK                  (0x70U)
#define CMP_CR0_FILTER_CNT_SHIFT                 (4U)
#define CMP_CR0_FILTER_CNT(x)                    (((uint8_t)(((uint8_t)(x)) << CMP_CR0_FILTER_CNT_SHIFT)) & CMP_CR0_FILTER_CNT_MASK)

/*! @name CR1 - CMP Control Register 1 */
#define CMP_CR1_EN_MASK                          (0x1U)
#define CMP_CR1_EN_SHIFT                         (0U)
#define CMP_CR1_EN(x)                            (((uint8_t)(((uint8_t)(x)) << CMP_CR1_EN_SHIFT)) & CMP_CR1_EN_MASK)
#define CMP_CR1_OPE_MASK                         (0x2U)
#define CMP_CR1_OPE_SHIFT                        (1U)
#define CMP_CR1_OPE(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_CR1_OPE_SHIFT)) & CMP_CR1_OPE_MASK)
#define CMP_CR1_COS_MASK                         (0x4U)
#define CMP_CR1_COS_SHIFT                        (2U)
#define CMP_CR1_COS(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_CR1_COS_SHIFT)) & CMP_CR1_COS_MASK)
#define CMP_CR1_INV_MASK                         (0x8U)
#define CMP_CR1_INV_SHIFT                        (3U)
#define CMP_CR1_INV(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_CR1_INV_SHIFT)) & CMP_CR1_INV_MASK)
#define CMP_CR1_PMODE_MASK                       (0x10U)
#define CMP_CR1_PMODE_SHIFT                      (4U)
#define CMP_CR1_PMODE(x)                         (((uint8_t)(((uint8_t)(x)) << CMP_CR1_PMODE_SHIFT)) & CMP_CR1_PMODE_MASK)
#define CMP_CR1_TRIGM_MASK                       (0x20U)
#define CMP_CR1_TRIGM_SHIFT                      (5U)
#define CMP_CR1_TRIGM(x)                         (((uint8_t)(((uint8_t)(x)) << CMP_CR1_TRIGM_SHIFT)) & CMP_CR1_TRIGM_MASK)
#define CMP_CR1_WE_MASK                          (0x40U)
#define CMP_CR1_WE_SHIFT                         (6U)
#define CMP_CR1_WE(x)                            (((uint8_t)(((uint8_t)(x)) << CMP_CR1_WE_SHIFT)) & CMP_CR1_WE_MASK)
#define CMP_CR1_SE_MASK                          (0x80U)
#define CMP_CR1_SE_SHIFT                         (7U)
#define CMP_CR1_SE(x)                            (((uint8_t)(((uint8_t)(x)) << CMP_CR1_SE_SHIFT)) & CMP_CR1_SE_MASK)

/*! @name FPR - CMP Filter Period Register */
#define CMP_FPR_FILT_PER_MASK                    (0xFFU)
#define CMP_FPR_FILT_PER_SHIFT                   (0U)
#define CMP_FPR_FILT_PER(x)                      (((uint8_t)(((uint8_t)(x)) << CMP_FPR_FILT_PER_SHIFT)) & CMP_FPR_FILT_PER_MASK)

/*! @name SCR - CMP Status and Control Register */
#define CMP_SCR_COUT_MASK                        (0x1U)
#define CMP_SCR_COUT_SHIFT                       (0U)
#define CMP_SCR_COUT(x)                          (((uint8_t)(((uint8_t)(x)) << CMP_SCR_COUT_SHIFT)) & CMP_SCR_COUT_MASK)
#define CMP_SCR_CFF_MASK                         (0x2U)
#define CMP_SCR_CFF_SHIFT                        (1U)
#define CMP_SCR_CFF(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_SCR_CFF_SHIFT)) & CMP_SCR_CFF_MASK)
#define CMP_SCR_CFR_MASK                         (0x4U)
#define CMP_SCR_CFR_SHIFT                        (2U)
#define CMP_SCR_CFR(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_SCR_CFR_SHIFT)) & CMP_SCR_CFR_MASK)
#define CMP_SCR_IEF_MASK                         (0x8U)
#define CMP_SCR_IEF_SHIFT                        (3U)
#define CMP_SCR_IEF(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_SCR_IEF_SHIFT)) & CMP_SCR_IEF_MASK)
#define CMP_SCR_IER_MASK                         (0x10U)
#define CMP_SCR_IER_SHIFT                        (4U)
#define CMP_SCR_IER(x)                           (((uint8_t)(((uint8_t)(x)) << CMP_SCR_IER_SHIFT)) & CMP_SCR_IER_MASK)
#define CMP_SCR_DMAEN_MASK                       (0x40U)
#define CMP_SCR_DMAEN_SHIFT                      (6U)
#define CMP_SCR_DMAEN(x)                         (((uint8_t)(((uint8_t)(x)) << CMP_SCR_DMAEN_SHIFT)) & CMP_SCR_DMAEN_MASK)

/*! @name DACCR - DAC Control Register */
#define CMP_DACCR_VOSEL_MASK                     (0x3FU)
#define CMP_DACCR_VOSEL_SHIFT                    (0U)
#define CMP_DACCR_VOSEL(x)                       (((uint8_t)(((uint8_t)(x)) << CMP_DACCR_VOSEL_SHIFT)) & CMP_DACCR_VOSEL_MASK)
#define CMP_DACCR_VRSEL_MASK                     (0x40U)
#define CMP_DACCR_VRSEL_SHIFT                    (6U)
#define CMP_DACCR_VRSEL(x)                       (((uint8_t)(((uint8_t)(x)) << CMP_DACCR_VRSEL_SHIFT)) & CMP_DACCR_VRSEL_MASK)
#define CMP_DACCR_DACEN_MASK                     (0x80U)
#define CMP_DACCR_DACEN_SHIFT                    (7U)
#define CMP_DACCR_DACEN(x)                       (((uint8_t)(((uint8_t)(x)) << CMP_DACCR_DACEN_SHIFT)) & CMP_DACCR_DACEN_MASK)

/*! @name MUXCR - MUX Control Register */
#define CMP_MUXCR_MSEL_MASK                      (0x7U)
#define CMP_MUXCR_MSEL_SHIFT                     (0U)
#define CMP_MUXCR_MSEL(x)                        (((uint8_t)(((uint8_t)(x)) << CMP_MUXCR_MSEL_SHIFT)) & CMP_MUXCR_MSEL_MASK)
#define CMP_MUXCR_PSEL_MASK                      (0x38U)
#define CMP_MUXCR_PSEL_SHIFT                     (3U)
#define CMP_MUXCR_PSEL(x)                        (((uint8_t)(((uint8_t)(x)) << CMP_MUXCR_PSEL_SHIFT)) & CMP_MUXCR_PSEL_MASK)


/*!
 * @}
 */ /* end of group CMP_Register_Masks */


/* CMP - Peripheral instance base addresses */
/** Peripheral CMP0 base address */
#define CMP0_BASE                                (0x40072000u)
/** Peripheral CMP0 base pointer */
#define CMP0                                     ((CMP_Type *)CMP0_BASE)
/** Peripheral CMP1 base address */
#define CMP1_BASE                                (0x40072008u)
/** Peripheral CMP1 base pointer */
#define CMP1                                     ((CMP_Type *)CMP1_BASE)
/** Peripheral CMP2 base address */
#define CMP2_BASE                                (0x40072010u)
/** Peripheral CMP2 base pointer */
#define CMP2                                     ((CMP_Type *)CMP2_BASE)
/** Array initializer of CMP peripheral base addresses */
#define CMP_BASE_ADDRS                           { CMP0_BASE, CMP1_BASE, CMP2_BASE }
/** Array initializer of CMP peripheral base pointers */
#define CMP_BASE_PTRS                            { CMP0, CMP1, CMP2 }
/** Interrupt vectors for the CMP peripheral type */
#define CMP_IRQS                                 { CMP0_CMP1_CMP2_IRQn, CMP0_CMP1_CMP2_IRQn, CMP0_CMP1_CMP2_IRQn }

/*!
 * @}
 */ /* end of group CMP_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- CRC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Peripheral_Access_Layer CRC Peripheral Access Layer
 * @{
 */

/** CRC - Register Layout Typedef */
typedef struct {
  union {                                          /* offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      __IO uint16_t DATAL;                             /**< CRC_DATAL register., offset: 0x0 */
      __IO uint16_t DATAH;                             /**< CRC_DATAH register., offset: 0x2 */
    } ACCESS16BIT;
    __IO uint32_t DATA;                              /**< CRC Data register, offset: 0x0 */
    struct {                                         /* offset: 0x0 */
      __IO uint8_t DATALL;                             /**< CRC_DATALL register., offset: 0x0 */
      __IO uint8_t DATALU;                             /**< CRC_DATALU register., offset: 0x1 */
      __IO uint8_t DATAHL;                             /**< CRC_DATAHL register., offset: 0x2 */
      __IO uint8_t DATAHU;                             /**< CRC_DATAHU register., offset: 0x3 */
    } ACCESS8BIT;
  };
  union {                                          /* offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      __IO uint16_t GPOLYL;                            /**< CRC_GPOLYL register., offset: 0x4 */
      __IO uint16_t GPOLYH;                            /**< CRC_GPOLYH register., offset: 0x6 */
    } GPOLY_ACCESS16BIT;
    __IO uint32_t GPOLY;                             /**< CRC Polynomial register, offset: 0x4 */
    struct {                                         /* offset: 0x4 */
      __IO uint8_t GPOLYLL;                            /**< CRC_GPOLYLL register., offset: 0x4 */
      __IO uint8_t GPOLYLU;                            /**< CRC_GPOLYLU register., offset: 0x5 */
      __IO uint8_t GPOLYHL;                            /**< CRC_GPOLYHL register., offset: 0x6 */
      __IO uint8_t GPOLYHU;                            /**< CRC_GPOLYHU register., offset: 0x7 */
    } GPOLY_ACCESS8BIT;
  };
  union {                                          /* offset: 0x8 */
    __IO uint32_t CTRL;                              /**< CRC Control register, offset: 0x8 */
    struct {                                         /* offset: 0x8 */
           uint8_t RESERVED_0[3];
      __IO uint8_t CTRLHU;                             /**< CRC_CTRLHU register., offset: 0xB */
    } CTRL_ACCESS8BIT;
  };
} CRC_Type;

/* ----------------------------------------------------------------------------
   -- CRC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup CRC_Register_Masks CRC Register Masks
 * @{
 */

/*! @name DATAL - CRC_DATAL register. */
#define CRC_DATAL_DATAL_MASK                     (0xFFFFU)
#define CRC_DATAL_DATAL_SHIFT                    (0U)
#define CRC_DATAL_DATAL(x)                       (((uint16_t)(((uint16_t)(x)) << CRC_DATAL_DATAL_SHIFT)) & CRC_DATAL_DATAL_MASK)

/*! @name DATAH - CRC_DATAH register. */
#define CRC_DATAH_DATAH_MASK                     (0xFFFFU)
#define CRC_DATAH_DATAH_SHIFT                    (0U)
#define CRC_DATAH_DATAH(x)                       (((uint16_t)(((uint16_t)(x)) << CRC_DATAH_DATAH_SHIFT)) & CRC_DATAH_DATAH_MASK)

/*! @name DATA - CRC Data register */
#define CRC_DATA_LL_MASK                         (0xFFU)
#define CRC_DATA_LL_SHIFT                        (0U)
#define CRC_DATA_LL(x)                           (((uint32_t)(((uint32_t)(x)) << CRC_DATA_LL_SHIFT)) & CRC_DATA_LL_MASK)
#define CRC_DATA_LU_MASK                         (0xFF00U)
#define CRC_DATA_LU_SHIFT                        (8U)
#define CRC_DATA_LU(x)                           (((uint32_t)(((uint32_t)(x)) << CRC_DATA_LU_SHIFT)) & CRC_DATA_LU_MASK)
#define CRC_DATA_HL_MASK                         (0xFF0000U)
#define CRC_DATA_HL_SHIFT                        (16U)
#define CRC_DATA_HL(x)                           (((uint32_t)(((uint32_t)(x)) << CRC_DATA_HL_SHIFT)) & CRC_DATA_HL_MASK)
#define CRC_DATA_HU_MASK                         (0xFF000000U)
#define CRC_DATA_HU_SHIFT                        (24U)
#define CRC_DATA_HU(x)                           (((uint32_t)(((uint32_t)(x)) << CRC_DATA_HU_SHIFT)) & CRC_DATA_HU_MASK)

/*! @name DATALL - CRC_DATALL register. */
#define CRC_DATALL_DATALL_MASK                   (0xFFU)
#define CRC_DATALL_DATALL_SHIFT                  (0U)
#define CRC_DATALL_DATALL(x)                     (((uint8_t)(((uint8_t)(x)) << CRC_DATALL_DATALL_SHIFT)) & CRC_DATALL_DATALL_MASK)

/*! @name DATALU - CRC_DATALU register. */
#define CRC_DATALU_DATALU_MASK                   (0xFFU)
#define CRC_DATALU_DATALU_SHIFT                  (0U)
#define CRC_DATALU_DATALU(x)                     (((uint8_t)(((uint8_t)(x)) << CRC_DATALU_DATALU_SHIFT)) & CRC_DATALU_DATALU_MASK)

/*! @name DATAHL - CRC_DATAHL register. */
#define CRC_DATAHL_DATAHL_MASK                   (0xFFU)
#define CRC_DATAHL_DATAHL_SHIFT                  (0U)
#define CRC_DATAHL_DATAHL(x)                     (((uint8_t)(((uint8_t)(x)) << CRC_DATAHL_DATAHL_SHIFT)) & CRC_DATAHL_DATAHL_MASK)

/*! @name DATAHU - CRC_DATAHU register. */
#define CRC_DATAHU_DATAHU_MASK                   (0xFFU)
#define CRC_DATAHU_DATAHU_SHIFT                  (0U)
#define CRC_DATAHU_DATAHU(x)                     (((uint8_t)(((uint8_t)(x)) << CRC_DATAHU_DATAHU_SHIFT)) & CRC_DATAHU_DATAHU_MASK)

/*! @name GPOLYL - CRC_GPOLYL register. */
#define CRC_GPOLYL_GPOLYL_MASK                   (0xFFFFU)
#define CRC_GPOLYL_GPOLYL_SHIFT                  (0U)
#define CRC_GPOLYL_GPOLYL(x)                     (((uint16_t)(((uint16_t)(x)) << CRC_GPOLYL_GPOLYL_SHIFT)) & CRC_GPOLYL_GPOLYL_MASK)

/*! @name GPOLYH - CRC_GPOLYH register. */
#define CRC_GPOLYH_GPOLYH_MASK                   (0xFFFFU)
#define CRC_GPOLYH_GPOLYH_SHIFT                  (0U)
#define CRC_GPOLYH_GPOLYH(x)                     (((uint16_t)(((uint16_t)(x)) << CRC_GPOLYH_GPOLYH_SHIFT)) & CRC_GPOLYH_GPOLYH_MASK)

/*! @name GPOLY - CRC Polynomial register */
#define CRC_GPOLY_LOW_MASK                       (0xFFFFU)
#define CRC_GPOLY_LOW_SHIFT                      (0U)
#define CRC_GPOLY_LOW(x)                         (((uint32_t)(((uint32_t)(x)) << CRC_GPOLY_LOW_SHIFT)) & CRC_GPOLY_LOW_MASK)
#define CRC_GPOLY_HIGH_MASK                      (0xFFFF0000U)
#define CRC_GPOLY_HIGH_SHIFT                     (16U)
#define CRC_GPOLY_HIGH(x)                        (((uint32_t)(((uint32_t)(x)) << CRC_GPOLY_HIGH_SHIFT)) & CRC_GPOLY_HIGH_MASK)

/*! @name GPOLYLL - CRC_GPOLYLL register. */
#define CRC_GPOLYLL_GPOLYLL_MASK                 (0xFFU)
#define CRC_GPOLYLL_GPOLYLL_SHIFT                (0U)
#define CRC_GPOLYLL_GPOLYLL(x)                   (((uint8_t)(((uint8_t)(x)) << CRC_GPOLYLL_GPOLYLL_SHIFT)) & CRC_GPOLYLL_GPOLYLL_MASK)

/*! @name GPOLYLU - CRC_GPOLYLU register. */
#define CRC_GPOLYLU_GPOLYLU_MASK                 (0xFFU)
#define CRC_GPOLYLU_GPOLYLU_SHIFT                (0U)
#define CRC_GPOLYLU_GPOLYLU(x)                   (((uint8_t)(((uint8_t)(x)) << CRC_GPOLYLU_GPOLYLU_SHIFT)) & CRC_GPOLYLU_GPOLYLU_MASK)

/*! @name GPOLYHL - CRC_GPOLYHL register. */
#define CRC_GPOLYHL_GPOLYHL_MASK                 (0xFFU)
#define CRC_GPOLYHL_GPOLYHL_SHIFT                (0U)
#define CRC_GPOLYHL_GPOLYHL(x)                   (((uint8_t)(((uint8_t)(x)) << CRC_GPOLYHL_GPOLYHL_SHIFT)) & CRC_GPOLYHL_GPOLYHL_MASK)

/*! @name GPOLYHU - CRC_GPOLYHU register. */
#define CRC_GPOLYHU_GPOLYHU_MASK                 (0xFFU)
#define CRC_GPOLYHU_GPOLYHU_SHIFT                (0U)
#define CRC_GPOLYHU_GPOLYHU(x)                   (((uint8_t)(((uint8_t)(x)) << CRC_GPOLYHU_GPOLYHU_SHIFT)) & CRC_GPOLYHU_GPOLYHU_MASK)

/*! @name CTRL - CRC Control register */
#define CRC_CTRL_TCRC_MASK                       (0x1000000U)
#define CRC_CTRL_TCRC_SHIFT                      (24U)
#define CRC_CTRL_TCRC(x)                         (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_TCRC_SHIFT)) & CRC_CTRL_TCRC_MASK)
#define CRC_CTRL_WAS_MASK                        (0x2000000U)
#define CRC_CTRL_WAS_SHIFT                       (25U)
#define CRC_CTRL_WAS(x)                          (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_WAS_SHIFT)) & CRC_CTRL_WAS_MASK)
#define CRC_CTRL_FXOR_MASK                       (0x4000000U)
#define CRC_CTRL_FXOR_SHIFT                      (26U)
#define CRC_CTRL_FXOR(x)                         (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_FXOR_SHIFT)) & CRC_CTRL_FXOR_MASK)
#define CRC_CTRL_TOTR_MASK                       (0x30000000U)
#define CRC_CTRL_TOTR_SHIFT                      (28U)
#define CRC_CTRL_TOTR(x)                         (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_TOTR_SHIFT)) & CRC_CTRL_TOTR_MASK)
#define CRC_CTRL_TOT_MASK                        (0xC0000000U)
#define CRC_CTRL_TOT_SHIFT                       (30U)
#define CRC_CTRL_TOT(x)                          (((uint32_t)(((uint32_t)(x)) << CRC_CTRL_TOT_SHIFT)) & CRC_CTRL_TOT_MASK)

/*! @name CTRLHU - CRC_CTRLHU register. */
#define CRC_CTRLHU_TCRC_MASK                     (0x1U)
#define CRC_CTRLHU_TCRC_SHIFT                    (0U)
#define CRC_CTRLHU_TCRC(x)                       (((uint8_t)(((uint8_t)(x)) << CRC_CTRLHU_TCRC_SHIFT)) & CRC_CTRLHU_TCRC_MASK)
#define CRC_CTRLHU_WAS_MASK                      (0x2U)
#define CRC_CTRLHU_WAS_SHIFT                     (1U)
#define CRC_CTRLHU_WAS(x)                        (((uint8_t)(((uint8_t)(x)) << CRC_CTRLHU_WAS_SHIFT)) & CRC_CTRLHU_WAS_MASK)
#define CRC_CTRLHU_FXOR_MASK                     (0x4U)
#define CRC_CTRLHU_FXOR_SHIFT                    (2U)
#define CRC_CTRLHU_FXOR(x)                       (((uint8_t)(((uint8_t)(x)) << CRC_CTRLHU_FXOR_SHIFT)) & CRC_CTRLHU_FXOR_MASK)
#define CRC_CTRLHU_TOTR_MASK                     (0x30U)
#define CRC_CTRLHU_TOTR_SHIFT                    (4U)
#define CRC_CTRLHU_TOTR(x)                       (((uint8_t)(((uint8_t)(x)) << CRC_CTRLHU_TOTR_SHIFT)) & CRC_CTRLHU_TOTR_MASK)
#define CRC_CTRLHU_TOT_MASK                      (0xC0U)
#define CRC_CTRLHU_TOT_SHIFT                     (6U)
#define CRC_CTRLHU_TOT(x)                        (((uint8_t)(((uint8_t)(x)) << CRC_CTRLHU_TOT_SHIFT)) & CRC_CTRLHU_TOT_MASK)


/*!
 * @}
 */ /* end of group CRC_Register_Masks */


/* CRC - Peripheral instance base addresses */
/** Peripheral CRC base address */
#define CRC_BASE                                 (0x40034000u)
/** Peripheral CRC base pointer */
#define CRC0                                     ((CRC_Type *)CRC_BASE)
/** Array initializer of CRC peripheral base addresses */
#define CRC_BASE_ADDRS                           { CRC_BASE }
/** Array initializer of CRC peripheral base pointers */
#define CRC_BASE_PTRS                            { CRC0 }

/*!
 * @}
 */ /* end of group CRC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- DMA Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMA_Peripheral_Access_Layer DMA Peripheral Access Layer
 * @{
 */

/** DMA - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[256];
  struct {                                         /* offset: 0x100, array step: 0x10 */
    __IO uint32_t SAR;                               /**< Source Address Register, array offset: 0x100, array step: 0x10 */
    __IO uint32_t DAR;                               /**< Destination Address Register, array offset: 0x104, array step: 0x10 */
    union {                                          /* offset: 0x108, array step: 0x10 */
      __IO uint32_t DSR_BCR;                           /**< DMA Status Register / Byte Count Register, array offset: 0x108, array step: 0x10 */
      struct {                                         /* offset: 0x108, array step: 0x10 */
             uint8_t RESERVED_0[3];
        __IO uint8_t DSR;                                /**< DMA_DSR0 register...DMA_DSR3 register., array offset: 0x10B, array step: 0x10 */
      } DMA_DSR_ACCESS8BIT;
    };
    __IO uint32_t DCR;                               /**< DMA Control Register, array offset: 0x10C, array step: 0x10 */
  } DMA[4];
} DMA_Type;

/* ----------------------------------------------------------------------------
   -- DMA Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMA_Register_Masks DMA Register Masks
 * @{
 */

/*! @name SAR - Source Address Register */
#define DMA_SAR_SAR_MASK                         (0xFFFFFFFFU)
#define DMA_SAR_SAR_SHIFT                        (0U)
#define DMA_SAR_SAR(x)                           (((uint32_t)(((uint32_t)(x)) << DMA_SAR_SAR_SHIFT)) & DMA_SAR_SAR_MASK)

/* The count of DMA_SAR */
#define DMA_SAR_COUNT                            (4U)

/*! @name DAR - Destination Address Register */
#define DMA_DAR_DAR_MASK                         (0xFFFFFFFFU)
#define DMA_DAR_DAR_SHIFT                        (0U)
#define DMA_DAR_DAR(x)                           (((uint32_t)(((uint32_t)(x)) << DMA_DAR_DAR_SHIFT)) & DMA_DAR_DAR_MASK)

/* The count of DMA_DAR */
#define DMA_DAR_COUNT                            (4U)

/*! @name DSR_BCR - DMA Status Register / Byte Count Register */
#define DMA_DSR_BCR_BCR_MASK                     (0xFFFFFFU)
#define DMA_DSR_BCR_BCR_SHIFT                    (0U)
#define DMA_DSR_BCR_BCR(x)                       (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_BCR_SHIFT)) & DMA_DSR_BCR_BCR_MASK)
#define DMA_DSR_BCR_DONE_MASK                    (0x1000000U)
#define DMA_DSR_BCR_DONE_SHIFT                   (24U)
#define DMA_DSR_BCR_DONE(x)                      (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_DONE_SHIFT)) & DMA_DSR_BCR_DONE_MASK)
#define DMA_DSR_BCR_BSY_MASK                     (0x2000000U)
#define DMA_DSR_BCR_BSY_SHIFT                    (25U)
#define DMA_DSR_BCR_BSY(x)                       (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_BSY_SHIFT)) & DMA_DSR_BCR_BSY_MASK)
#define DMA_DSR_BCR_REQ_MASK                     (0x4000000U)
#define DMA_DSR_BCR_REQ_SHIFT                    (26U)
#define DMA_DSR_BCR_REQ(x)                       (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_REQ_SHIFT)) & DMA_DSR_BCR_REQ_MASK)
#define DMA_DSR_BCR_BED_MASK                     (0x10000000U)
#define DMA_DSR_BCR_BED_SHIFT                    (28U)
#define DMA_DSR_BCR_BED(x)                       (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_BED_SHIFT)) & DMA_DSR_BCR_BED_MASK)
#define DMA_DSR_BCR_BES_MASK                     (0x20000000U)
#define DMA_DSR_BCR_BES_SHIFT                    (29U)
#define DMA_DSR_BCR_BES(x)                       (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_BES_SHIFT)) & DMA_DSR_BCR_BES_MASK)
#define DMA_DSR_BCR_CE_MASK                      (0x40000000U)
#define DMA_DSR_BCR_CE_SHIFT                     (30U)
#define DMA_DSR_BCR_CE(x)                        (((uint32_t)(((uint32_t)(x)) << DMA_DSR_BCR_CE_SHIFT)) & DMA_DSR_BCR_CE_MASK)

/* The count of DMA_DSR_BCR */
#define DMA_DSR_BCR_COUNT                        (4U)

/* The count of DMA_DSR */
#define DMA_DSR_COUNT                            (4U)

/*! @name DCR - DMA Control Register */
#define DMA_DCR_LCH2_MASK                        (0x3U)
#define DMA_DCR_LCH2_SHIFT                       (0U)
#define DMA_DCR_LCH2(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_LCH2_SHIFT)) & DMA_DCR_LCH2_MASK)
#define DMA_DCR_LCH1_MASK                        (0xCU)
#define DMA_DCR_LCH1_SHIFT                       (2U)
#define DMA_DCR_LCH1(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_LCH1_SHIFT)) & DMA_DCR_LCH1_MASK)
#define DMA_DCR_LINKCC_MASK                      (0x30U)
#define DMA_DCR_LINKCC_SHIFT                     (4U)
#define DMA_DCR_LINKCC(x)                        (((uint32_t)(((uint32_t)(x)) << DMA_DCR_LINKCC_SHIFT)) & DMA_DCR_LINKCC_MASK)
#define DMA_DCR_D_REQ_MASK                       (0x80U)
#define DMA_DCR_D_REQ_SHIFT                      (7U)
#define DMA_DCR_D_REQ(x)                         (((uint32_t)(((uint32_t)(x)) << DMA_DCR_D_REQ_SHIFT)) & DMA_DCR_D_REQ_MASK)
#define DMA_DCR_DMOD_MASK                        (0xF00U)
#define DMA_DCR_DMOD_SHIFT                       (8U)
#define DMA_DCR_DMOD(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_DMOD_SHIFT)) & DMA_DCR_DMOD_MASK)
#define DMA_DCR_SMOD_MASK                        (0xF000U)
#define DMA_DCR_SMOD_SHIFT                       (12U)
#define DMA_DCR_SMOD(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_SMOD_SHIFT)) & DMA_DCR_SMOD_MASK)
#define DMA_DCR_START_MASK                       (0x10000U)
#define DMA_DCR_START_SHIFT                      (16U)
#define DMA_DCR_START(x)                         (((uint32_t)(((uint32_t)(x)) << DMA_DCR_START_SHIFT)) & DMA_DCR_START_MASK)
#define DMA_DCR_DSIZE_MASK                       (0x60000U)
#define DMA_DCR_DSIZE_SHIFT                      (17U)
#define DMA_DCR_DSIZE(x)                         (((uint32_t)(((uint32_t)(x)) << DMA_DCR_DSIZE_SHIFT)) & DMA_DCR_DSIZE_MASK)
#define DMA_DCR_DINC_MASK                        (0x80000U)
#define DMA_DCR_DINC_SHIFT                       (19U)
#define DMA_DCR_DINC(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_DINC_SHIFT)) & DMA_DCR_DINC_MASK)
#define DMA_DCR_SSIZE_MASK                       (0x300000U)
#define DMA_DCR_SSIZE_SHIFT                      (20U)
#define DMA_DCR_SSIZE(x)                         (((uint32_t)(((uint32_t)(x)) << DMA_DCR_SSIZE_SHIFT)) & DMA_DCR_SSIZE_MASK)
#define DMA_DCR_SINC_MASK                        (0x400000U)
#define DMA_DCR_SINC_SHIFT                       (22U)
#define DMA_DCR_SINC(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_SINC_SHIFT)) & DMA_DCR_SINC_MASK)
#define DMA_DCR_EADREQ_MASK                      (0x800000U)
#define DMA_DCR_EADREQ_SHIFT                     (23U)
#define DMA_DCR_EADREQ(x)                        (((uint32_t)(((uint32_t)(x)) << DMA_DCR_EADREQ_SHIFT)) & DMA_DCR_EADREQ_MASK)
#define DMA_DCR_UMNSM_MASK                       (0x3000000U)
#define DMA_DCR_UMNSM_SHIFT                      (24U)
#define DMA_DCR_UMNSM(x)                         (((uint32_t)(((uint32_t)(x)) << DMA_DCR_UMNSM_SHIFT)) & DMA_DCR_UMNSM_MASK)
#define DMA_DCR_CHACR_MASK                       (0xC000000U)
#define DMA_DCR_CHACR_SHIFT                      (26U)
#define DMA_DCR_CHACR(x)                         (((uint32_t)(((uint32_t)(x)) << DMA_DCR_CHACR_SHIFT)) & DMA_DCR_CHACR_MASK)
#define DMA_DCR_AA_MASK                          (0x10000000U)
#define DMA_DCR_AA_SHIFT                         (28U)
#define DMA_DCR_AA(x)                            (((uint32_t)(((uint32_t)(x)) << DMA_DCR_AA_SHIFT)) & DMA_DCR_AA_MASK)
#define DMA_DCR_CS_MASK                          (0x20000000U)
#define DMA_DCR_CS_SHIFT                         (29U)
#define DMA_DCR_CS(x)                            (((uint32_t)(((uint32_t)(x)) << DMA_DCR_CS_SHIFT)) & DMA_DCR_CS_MASK)
#define DMA_DCR_ERQ_MASK                         (0x40000000U)
#define DMA_DCR_ERQ_SHIFT                        (30U)
#define DMA_DCR_ERQ(x)                           (((uint32_t)(((uint32_t)(x)) << DMA_DCR_ERQ_SHIFT)) & DMA_DCR_ERQ_MASK)
#define DMA_DCR_EINT_MASK                        (0x80000000U)
#define DMA_DCR_EINT_SHIFT                       (31U)
#define DMA_DCR_EINT(x)                          (((uint32_t)(((uint32_t)(x)) << DMA_DCR_EINT_SHIFT)) & DMA_DCR_EINT_MASK)

/* The count of DMA_DCR */
#define DMA_DCR_COUNT                            (4U)


/*!
 * @}
 */ /* end of group DMA_Register_Masks */


/* DMA - Peripheral instance base addresses */
/** Peripheral DMA base address */
#define DMA_BASE                                 (0x40008000u)
/** Peripheral DMA base pointer */
#define DMA0                                     ((DMA_Type *)DMA_BASE)
/** Array initializer of DMA peripheral base addresses */
#define DMA_BASE_ADDRS                           { DMA_BASE }
/** Array initializer of DMA peripheral base pointers */
#define DMA_BASE_PTRS                            { DMA0 }
/** Interrupt vectors for the DMA peripheral type */
#define DMA_CHN_IRQS                             { { DMA0_IRQn, DMA1_IRQn, DMA2_IRQn, DMA3_IRQn } }

/*!
 * @}
 */ /* end of group DMA_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- DMAMUX Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMAMUX_Peripheral_Access_Layer DMAMUX Peripheral Access Layer
 * @{
 */

/** DMAMUX - Register Layout Typedef */
typedef struct {
  __IO uint8_t CHCFG[4];                           /**< Channel Configuration register, array offset: 0x0, array step: 0x1 */
} DMAMUX_Type;

/* ----------------------------------------------------------------------------
   -- DMAMUX Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup DMAMUX_Register_Masks DMAMUX Register Masks
 * @{
 */

/*! @name CHCFG - Channel Configuration register */
#define DMAMUX_CHCFG_SOURCE_MASK                 (0x3FU)
#define DMAMUX_CHCFG_SOURCE_SHIFT                (0U)
#define DMAMUX_CHCFG_SOURCE(x)                   (((uint8_t)(((uint8_t)(x)) << DMAMUX_CHCFG_SOURCE_SHIFT)) & DMAMUX_CHCFG_SOURCE_MASK)
#define DMAMUX_CHCFG_TRIG_MASK                   (0x40U)
#define DMAMUX_CHCFG_TRIG_SHIFT                  (6U)
#define DMAMUX_CHCFG_TRIG(x)                     (((uint8_t)(((uint8_t)(x)) << DMAMUX_CHCFG_TRIG_SHIFT)) & DMAMUX_CHCFG_TRIG_MASK)
#define DMAMUX_CHCFG_ENBL_MASK                   (0x80U)
#define DMAMUX_CHCFG_ENBL_SHIFT                  (7U)
#define DMAMUX_CHCFG_ENBL(x)                     (((uint8_t)(((uint8_t)(x)) << DMAMUX_CHCFG_ENBL_SHIFT)) & DMAMUX_CHCFG_ENBL_MASK)

/* The count of DMAMUX_CHCFG */
#define DMAMUX_CHCFG_COUNT                       (4U)


/*!
 * @}
 */ /* end of group DMAMUX_Register_Masks */


/* DMAMUX - Peripheral instance base addresses */
/** Peripheral DMAMUX0 base address */
#define DMAMUX0_BASE                             (0x40021000u)
/** Peripheral DMAMUX0 base pointer */
#define DMAMUX0                                  ((DMAMUX_Type *)DMAMUX0_BASE)
/** Array initializer of DMAMUX peripheral base addresses */
#define DMAMUX_BASE_ADDRS                        { DMAMUX0_BASE }
/** Array initializer of DMAMUX peripheral base pointers */
#define DMAMUX_BASE_PTRS                         { DMAMUX0 }

/*!
 * @}
 */ /* end of group DMAMUX_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- EWM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EWM_Peripheral_Access_Layer EWM Peripheral Access Layer
 * @{
 */

/** EWM - Register Layout Typedef */
typedef struct {
  __IO uint8_t CTRL;                               /**< Control Register, offset: 0x0 */
  __O  uint8_t SERV;                               /**< Service Register, offset: 0x1 */
  __IO uint8_t CMPL;                               /**< Compare Low Register, offset: 0x2 */
  __IO uint8_t CMPH;                               /**< Compare High Register, offset: 0x3 */
  __IO uint8_t CLKCTRL;                            /**< Clock Control Register, offset: 0x4 */
  __IO uint8_t CLKPRESCALER;                       /**< Clock Prescaler Register, offset: 0x5 */
} EWM_Type;

/* ----------------------------------------------------------------------------
   -- EWM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup EWM_Register_Masks EWM Register Masks
 * @{
 */

/*! @name CTRL - Control Register */
#define EWM_CTRL_EWMEN_MASK                      (0x1U)
#define EWM_CTRL_EWMEN_SHIFT                     (0U)
#define EWM_CTRL_EWMEN(x)                        (((uint8_t)(((uint8_t)(x)) << EWM_CTRL_EWMEN_SHIFT)) & EWM_CTRL_EWMEN_MASK)
#define EWM_CTRL_ASSIN_MASK                      (0x2U)
#define EWM_CTRL_ASSIN_SHIFT                     (1U)
#define EWM_CTRL_ASSIN(x)                        (((uint8_t)(((uint8_t)(x)) << EWM_CTRL_ASSIN_SHIFT)) & EWM_CTRL_ASSIN_MASK)
#define EWM_CTRL_INEN_MASK                       (0x4U)
#define EWM_CTRL_INEN_SHIFT                      (2U)
#define EWM_CTRL_INEN(x)                         (((uint8_t)(((uint8_t)(x)) << EWM_CTRL_INEN_SHIFT)) & EWM_CTRL_INEN_MASK)
#define EWM_CTRL_INTEN_MASK                      (0x8U)
#define EWM_CTRL_INTEN_SHIFT                     (3U)
#define EWM_CTRL_INTEN(x)                        (((uint8_t)(((uint8_t)(x)) << EWM_CTRL_INTEN_SHIFT)) & EWM_CTRL_INTEN_MASK)

/*! @name SERV - Service Register */
#define EWM_SERV_SERVICE_MASK                    (0xFFU)
#define EWM_SERV_SERVICE_SHIFT                   (0U)
#define EWM_SERV_SERVICE(x)                      (((uint8_t)(((uint8_t)(x)) << EWM_SERV_SERVICE_SHIFT)) & EWM_SERV_SERVICE_MASK)

/*! @name CMPL - Compare Low Register */
#define EWM_CMPL_COMPAREL_MASK                   (0xFFU)
#define EWM_CMPL_COMPAREL_SHIFT                  (0U)
#define EWM_CMPL_COMPAREL(x)                     (((uint8_t)(((uint8_t)(x)) << EWM_CMPL_COMPAREL_SHIFT)) & EWM_CMPL_COMPAREL_MASK)

/*! @name CMPH - Compare High Register */
#define EWM_CMPH_COMPAREH_MASK                   (0xFFU)
#define EWM_CMPH_COMPAREH_SHIFT                  (0U)
#define EWM_CMPH_COMPAREH(x)                     (((uint8_t)(((uint8_t)(x)) << EWM_CMPH_COMPAREH_SHIFT)) & EWM_CMPH_COMPAREH_MASK)

/*! @name CLKCTRL - Clock Control Register */
#define EWM_CLKCTRL_CLKSEL_MASK                  (0x3U)
#define EWM_CLKCTRL_CLKSEL_SHIFT                 (0U)
#define EWM_CLKCTRL_CLKSEL(x)                    (((uint8_t)(((uint8_t)(x)) << EWM_CLKCTRL_CLKSEL_SHIFT)) & EWM_CLKCTRL_CLKSEL_MASK)

/*! @name CLKPRESCALER - Clock Prescaler Register */
#define EWM_CLKPRESCALER_CLK_DIV_MASK            (0xFFU)
#define EWM_CLKPRESCALER_CLK_DIV_SHIFT           (0U)
#define EWM_CLKPRESCALER_CLK_DIV(x)              (((uint8_t)(((uint8_t)(x)) << EWM_CLKPRESCALER_CLK_DIV_SHIFT)) & EWM_CLKPRESCALER_CLK_DIV_MASK)


/*!
 * @}
 */ /* end of group EWM_Register_Masks */


/* EWM - Peripheral instance base addresses */
/** Peripheral EWM base address */
#define EWM_BASE                                 (0x40061000u)
/** Peripheral EWM base pointer */
#define EWM                                      ((EWM_Type *)EWM_BASE)
/** Array initializer of EWM peripheral base addresses */
#define EWM_BASE_ADDRS                           { EWM_BASE }
/** Array initializer of EWM peripheral base pointers */
#define EWM_BASE_PTRS                            { EWM }
/** Interrupt vectors for the EWM peripheral type */
#define EWM_IRQS                                 { WDOG_EWM_IRQn }

/*!
 * @}
 */ /* end of group EWM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- FGPIO Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Peripheral_Access_Layer FGPIO Peripheral Access Layer
 * @{
 */

/** FGPIO - Register Layout Typedef */
typedef struct {
  __IO uint8_t PDOR;                               /**< Port Data Output Register, offset: 0x0 */
       uint8_t RESERVED_0[3];
  __O  uint8_t PSOR;                               /**< Port Set Output Register, offset: 0x4 */
       uint8_t RESERVED_1[3];
  __O  uint8_t PCOR;                               /**< Port Clear Output Register, offset: 0x8 */
       uint8_t RESERVED_2[3];
  __O  uint8_t PTOR;                               /**< Port Toggle Output Register, offset: 0xC */
       uint8_t RESERVED_3[3];
  __I  uint8_t PDIR;                               /**< Port Data Input Register, offset: 0x10 */
       uint8_t RESERVED_4[3];
  __IO uint8_t PDDR;                               /**< Port Data Direction Register, offset: 0x14 */
       uint8_t RESERVED_5[7];
  __IO uint8_t GACR;                               /**< GPIO Attribute Checker Register, offset: 0x1C */
} FGPIO_Type;

/* ----------------------------------------------------------------------------
   -- FGPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FGPIO_Register_Masks FGPIO Register Masks
 * @{
 */

/*! @name PDOR - Port Data Output Register */
#define FGPIO_PDOR_PDO_MASK                      (0xFFU)
#define FGPIO_PDOR_PDO_SHIFT                     (0U)
#define FGPIO_PDOR_PDO(x)                        (((uint8_t)(((uint8_t)(x)) << FGPIO_PDOR_PDO_SHIFT)) & FGPIO_PDOR_PDO_MASK)

/*! @name PSOR - Port Set Output Register */
#define FGPIO_PSOR_PTSO_MASK                     (0xFFU)
#define FGPIO_PSOR_PTSO_SHIFT                    (0U)
#define FGPIO_PSOR_PTSO(x)                       (((uint8_t)(((uint8_t)(x)) << FGPIO_PSOR_PTSO_SHIFT)) & FGPIO_PSOR_PTSO_MASK)

/*! @name PCOR - Port Clear Output Register */
#define FGPIO_PCOR_PTCO_MASK                     (0xFFU)
#define FGPIO_PCOR_PTCO_SHIFT                    (0U)
#define FGPIO_PCOR_PTCO(x)                       (((uint8_t)(((uint8_t)(x)) << FGPIO_PCOR_PTCO_SHIFT)) & FGPIO_PCOR_PTCO_MASK)

/*! @name PTOR - Port Toggle Output Register */
#define FGPIO_PTOR_PTTO_MASK                     (0xFFU)
#define FGPIO_PTOR_PTTO_SHIFT                    (0U)
#define FGPIO_PTOR_PTTO(x)                       (((uint8_t)(((uint8_t)(x)) << FGPIO_PTOR_PTTO_SHIFT)) & FGPIO_PTOR_PTTO_MASK)

/*! @name PDIR - Port Data Input Register */
#define FGPIO_PDIR_PDI_MASK                      (0xFFU)
#define FGPIO_PDIR_PDI_SHIFT                     (0U)
#define FGPIO_PDIR_PDI(x)                        (((uint8_t)(((uint8_t)(x)) << FGPIO_PDIR_PDI_SHIFT)) & FGPIO_PDIR_PDI_MASK)

/*! @name PDDR - Port Data Direction Register */
#define FGPIO_PDDR_PDD_MASK                      (0xFFU)
#define FGPIO_PDDR_PDD_SHIFT                     (0U)
#define FGPIO_PDDR_PDD(x)                        (((uint8_t)(((uint8_t)(x)) << FGPIO_PDDR_PDD_SHIFT)) & FGPIO_PDDR_PDD_MASK)

/*! @name GACR - GPIO Attribute Checker Register */
#define FGPIO_GACR_ACB_MASK                      (0x7U)
#define FGPIO_GACR_ACB_SHIFT                     (0U)
#define FGPIO_GACR_ACB(x)                        (((uint8_t)(((uint8_t)(x)) << FGPIO_GACR_ACB_SHIFT)) & FGPIO_GACR_ACB_MASK)
#define FGPIO_GACR_ROB_MASK                      (0x80U)
#define FGPIO_GACR_ROB_SHIFT                     (7U)
#define FGPIO_GACR_ROB(x)                        (((uint8_t)(((uint8_t)(x)) << FGPIO_GACR_ROB_SHIFT)) & FGPIO_GACR_ROB_MASK)


/*!
 * @}
 */ /* end of group FGPIO_Register_Masks */


/* FGPIO - Peripheral instance base addresses */
/** Peripheral FGPIOA base address */
#define FGPIOA_BASE                              (0xF8000000u)
/** Peripheral FGPIOA base pointer */
#define FGPIOA                                   ((FGPIO_Type *)FGPIOA_BASE)
/** Peripheral FGPIOB base address */
#define FGPIOB_BASE                              (0xF8000001u)
/** Peripheral FGPIOB base pointer */
#define FGPIOB                                   ((FGPIO_Type *)FGPIOB_BASE)
/** Peripheral FGPIOC base address */
#define FGPIOC_BASE                              (0xF8000002u)
/** Peripheral FGPIOC base pointer */
#define FGPIOC                                   ((FGPIO_Type *)FGPIOC_BASE)
/** Peripheral FGPIOD base address */
#define FGPIOD_BASE                              (0xF8000003u)
/** Peripheral FGPIOD base pointer */
#define FGPIOD                                   ((FGPIO_Type *)FGPIOD_BASE)
/** Peripheral FGPIOE base address */
#define FGPIOE_BASE                              (0xF8000040u)
/** Peripheral FGPIOE base pointer */
#define FGPIOE                                   ((FGPIO_Type *)FGPIOE_BASE)
/** Peripheral FGPIOF base address */
#define FGPIOF_BASE                              (0xF8000041u)
/** Peripheral FGPIOF base pointer */
#define FGPIOF                                   ((FGPIO_Type *)FGPIOF_BASE)
/** Peripheral FGPIOG base address */
#define FGPIOG_BASE                              (0xF8000042u)
/** Peripheral FGPIOG base pointer */
#define FGPIOG                                   ((FGPIO_Type *)FGPIOG_BASE)
/** Peripheral FGPIOH base address */
#define FGPIOH_BASE                              (0xF8000043u)
/** Peripheral FGPIOH base pointer */
#define FGPIOH                                   ((FGPIO_Type *)FGPIOH_BASE)
/** Peripheral FGPIOI base address */
#define FGPIOI_BASE                              (0xF8000080u)
/** Peripheral FGPIOI base pointer */
#define FGPIOI                                   ((FGPIO_Type *)FGPIOI_BASE)
/** Peripheral FGPIOJ base address */
#define FGPIOJ_BASE                              (0xF8000081u)
/** Peripheral FGPIOJ base pointer */
#define FGPIOJ                                   ((FGPIO_Type *)FGPIOJ_BASE)
/** Peripheral FGPIOK base address */
#define FGPIOK_BASE                              (0xF8000082u)
/** Peripheral FGPIOK base pointer */
#define FGPIOK                                   ((FGPIO_Type *)FGPIOK_BASE)
/** Peripheral FGPIOL base address */
#define FGPIOL_BASE                              (0xF8000083u)
/** Peripheral FGPIOL base pointer */
#define FGPIOL                                   ((FGPIO_Type *)FGPIOL_BASE)
/** Peripheral FGPIOM base address */
#define FGPIOM_BASE                              (0xF80000C0u)
/** Peripheral FGPIOM base pointer */
#define FGPIOM                                   ((FGPIO_Type *)FGPIOM_BASE)
/** Array initializer of FGPIO peripheral base addresses */
#define FGPIO_BASE_ADDRS                         { FGPIOA_BASE, FGPIOB_BASE, FGPIOC_BASE, FGPIOD_BASE, FGPIOE_BASE, FGPIOF_BASE, FGPIOG_BASE, FGPIOH_BASE, FGPIOI_BASE, FGPIOJ_BASE, FGPIOK_BASE, FGPIOL_BASE, FGPIOM_BASE }
/** Array initializer of FGPIO peripheral base pointers */
#define FGPIO_BASE_PTRS                          { FGPIOA, FGPIOB, FGPIOC, FGPIOD, FGPIOE, FGPIOF, FGPIOG, FGPIOH, FGPIOI, FGPIOJ, FGPIOK, FGPIOL, FGPIOM }

/*!
 * @}
 */ /* end of group FGPIO_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- FTFA Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Peripheral_Access_Layer FTFA Peripheral Access Layer
 * @{
 */

/** FTFA - Register Layout Typedef */
typedef struct {
  __IO uint8_t FSTAT;                              /**< Flash Status Register, offset: 0x0 */
  __IO uint8_t FCNFG;                              /**< Flash Configuration Register, offset: 0x1 */
  __I  uint8_t FSEC;                               /**< Flash Security Register, offset: 0x2 */
  __I  uint8_t FOPT;                               /**< Flash Option Register, offset: 0x3 */
  __IO uint8_t FCCOB3;                             /**< Flash Common Command Object Registers, offset: 0x4 */
  __IO uint8_t FCCOB2;                             /**< Flash Common Command Object Registers, offset: 0x5 */
  __IO uint8_t FCCOB1;                             /**< Flash Common Command Object Registers, offset: 0x6 */
  __IO uint8_t FCCOB0;                             /**< Flash Common Command Object Registers, offset: 0x7 */
  __IO uint8_t FCCOB7;                             /**< Flash Common Command Object Registers, offset: 0x8 */
  __IO uint8_t FCCOB6;                             /**< Flash Common Command Object Registers, offset: 0x9 */
  __IO uint8_t FCCOB5;                             /**< Flash Common Command Object Registers, offset: 0xA */
  __IO uint8_t FCCOB4;                             /**< Flash Common Command Object Registers, offset: 0xB */
  __IO uint8_t FCCOBB;                             /**< Flash Common Command Object Registers, offset: 0xC */
  __IO uint8_t FCCOBA;                             /**< Flash Common Command Object Registers, offset: 0xD */
  __IO uint8_t FCCOB9;                             /**< Flash Common Command Object Registers, offset: 0xE */
  __IO uint8_t FCCOB8;                             /**< Flash Common Command Object Registers, offset: 0xF */
  __IO uint8_t FPROT3;                             /**< Program Flash Protection Registers, offset: 0x10 */
  __IO uint8_t FPROT2;                             /**< Program Flash Protection Registers, offset: 0x11 */
  __IO uint8_t FPROT1;                             /**< Program Flash Protection Registers, offset: 0x12 */
  __IO uint8_t FPROT0;                             /**< Program Flash Protection Registers, offset: 0x13 */
} FTFA_Type;

/* ----------------------------------------------------------------------------
   -- FTFA Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup FTFA_Register_Masks FTFA Register Masks
 * @{
 */

/*! @name FSTAT - Flash Status Register */
#define FTFA_FSTAT_MGSTAT0_MASK                  (0x1U)
#define FTFA_FSTAT_MGSTAT0_SHIFT                 (0U)
#define FTFA_FSTAT_MGSTAT0(x)                    (((uint8_t)(((uint8_t)(x)) << FTFA_FSTAT_MGSTAT0_SHIFT)) & FTFA_FSTAT_MGSTAT0_MASK)
#define FTFA_FSTAT_FPVIOL_MASK                   (0x10U)
#define FTFA_FSTAT_FPVIOL_SHIFT                  (4U)
#define FTFA_FSTAT_FPVIOL(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FSTAT_FPVIOL_SHIFT)) & FTFA_FSTAT_FPVIOL_MASK)
#define FTFA_FSTAT_ACCERR_MASK                   (0x20U)
#define FTFA_FSTAT_ACCERR_SHIFT                  (5U)
#define FTFA_FSTAT_ACCERR(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FSTAT_ACCERR_SHIFT)) & FTFA_FSTAT_ACCERR_MASK)
#define FTFA_FSTAT_RDCOLERR_MASK                 (0x40U)
#define FTFA_FSTAT_RDCOLERR_SHIFT                (6U)
#define FTFA_FSTAT_RDCOLERR(x)                   (((uint8_t)(((uint8_t)(x)) << FTFA_FSTAT_RDCOLERR_SHIFT)) & FTFA_FSTAT_RDCOLERR_MASK)
#define FTFA_FSTAT_CCIF_MASK                     (0x80U)
#define FTFA_FSTAT_CCIF_SHIFT                    (7U)
#define FTFA_FSTAT_CCIF(x)                       (((uint8_t)(((uint8_t)(x)) << FTFA_FSTAT_CCIF_SHIFT)) & FTFA_FSTAT_CCIF_MASK)

/*! @name FCNFG - Flash Configuration Register */
#define FTFA_FCNFG_ERSSUSP_MASK                  (0x10U)
#define FTFA_FCNFG_ERSSUSP_SHIFT                 (4U)
#define FTFA_FCNFG_ERSSUSP(x)                    (((uint8_t)(((uint8_t)(x)) << FTFA_FCNFG_ERSSUSP_SHIFT)) & FTFA_FCNFG_ERSSUSP_MASK)
#define FTFA_FCNFG_ERSAREQ_MASK                  (0x20U)
#define FTFA_FCNFG_ERSAREQ_SHIFT                 (5U)
#define FTFA_FCNFG_ERSAREQ(x)                    (((uint8_t)(((uint8_t)(x)) << FTFA_FCNFG_ERSAREQ_SHIFT)) & FTFA_FCNFG_ERSAREQ_MASK)
#define FTFA_FCNFG_RDCOLLIE_MASK                 (0x40U)
#define FTFA_FCNFG_RDCOLLIE_SHIFT                (6U)
#define FTFA_FCNFG_RDCOLLIE(x)                   (((uint8_t)(((uint8_t)(x)) << FTFA_FCNFG_RDCOLLIE_SHIFT)) & FTFA_FCNFG_RDCOLLIE_MASK)
#define FTFA_FCNFG_CCIE_MASK                     (0x80U)
#define FTFA_FCNFG_CCIE_SHIFT                    (7U)
#define FTFA_FCNFG_CCIE(x)                       (((uint8_t)(((uint8_t)(x)) << FTFA_FCNFG_CCIE_SHIFT)) & FTFA_FCNFG_CCIE_MASK)

/*! @name FSEC - Flash Security Register */
#define FTFA_FSEC_SEC_MASK                       (0x3U)
#define FTFA_FSEC_SEC_SHIFT                      (0U)
#define FTFA_FSEC_SEC(x)                         (((uint8_t)(((uint8_t)(x)) << FTFA_FSEC_SEC_SHIFT)) & FTFA_FSEC_SEC_MASK)
#define FTFA_FSEC_FSLACC_MASK                    (0xCU)
#define FTFA_FSEC_FSLACC_SHIFT                   (2U)
#define FTFA_FSEC_FSLACC(x)                      (((uint8_t)(((uint8_t)(x)) << FTFA_FSEC_FSLACC_SHIFT)) & FTFA_FSEC_FSLACC_MASK)
#define FTFA_FSEC_MEEN_MASK                      (0x30U)
#define FTFA_FSEC_MEEN_SHIFT                     (4U)
#define FTFA_FSEC_MEEN(x)                        (((uint8_t)(((uint8_t)(x)) << FTFA_FSEC_MEEN_SHIFT)) & FTFA_FSEC_MEEN_MASK)
#define FTFA_FSEC_KEYEN_MASK                     (0xC0U)
#define FTFA_FSEC_KEYEN_SHIFT                    (6U)
#define FTFA_FSEC_KEYEN(x)                       (((uint8_t)(((uint8_t)(x)) << FTFA_FSEC_KEYEN_SHIFT)) & FTFA_FSEC_KEYEN_MASK)

/*! @name FOPT - Flash Option Register */
#define FTFA_FOPT_OPT_MASK                       (0xFFU)
#define FTFA_FOPT_OPT_SHIFT                      (0U)
#define FTFA_FOPT_OPT(x)                         (((uint8_t)(((uint8_t)(x)) << FTFA_FOPT_OPT_SHIFT)) & FTFA_FOPT_OPT_MASK)

/*! @name FCCOB3 - Flash Common Command Object Registers */
#define FTFA_FCCOB3_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB3_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB3_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB3_CCOBn_SHIFT)) & FTFA_FCCOB3_CCOBn_MASK)

/*! @name FCCOB2 - Flash Common Command Object Registers */
#define FTFA_FCCOB2_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB2_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB2_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB2_CCOBn_SHIFT)) & FTFA_FCCOB2_CCOBn_MASK)

/*! @name FCCOB1 - Flash Common Command Object Registers */
#define FTFA_FCCOB1_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB1_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB1_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB1_CCOBn_SHIFT)) & FTFA_FCCOB1_CCOBn_MASK)

/*! @name FCCOB0 - Flash Common Command Object Registers */
#define FTFA_FCCOB0_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB0_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB0_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB0_CCOBn_SHIFT)) & FTFA_FCCOB0_CCOBn_MASK)

/*! @name FCCOB7 - Flash Common Command Object Registers */
#define FTFA_FCCOB7_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB7_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB7_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB7_CCOBn_SHIFT)) & FTFA_FCCOB7_CCOBn_MASK)

/*! @name FCCOB6 - Flash Common Command Object Registers */
#define FTFA_FCCOB6_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB6_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB6_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB6_CCOBn_SHIFT)) & FTFA_FCCOB6_CCOBn_MASK)

/*! @name FCCOB5 - Flash Common Command Object Registers */
#define FTFA_FCCOB5_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB5_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB5_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB5_CCOBn_SHIFT)) & FTFA_FCCOB5_CCOBn_MASK)

/*! @name FCCOB4 - Flash Common Command Object Registers */
#define FTFA_FCCOB4_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB4_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB4_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB4_CCOBn_SHIFT)) & FTFA_FCCOB4_CCOBn_MASK)

/*! @name FCCOBB - Flash Common Command Object Registers */
#define FTFA_FCCOBB_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOBB_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOBB_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOBB_CCOBn_SHIFT)) & FTFA_FCCOBB_CCOBn_MASK)

/*! @name FCCOBA - Flash Common Command Object Registers */
#define FTFA_FCCOBA_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOBA_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOBA_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOBA_CCOBn_SHIFT)) & FTFA_FCCOBA_CCOBn_MASK)

/*! @name FCCOB9 - Flash Common Command Object Registers */
#define FTFA_FCCOB9_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB9_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB9_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB9_CCOBn_SHIFT)) & FTFA_FCCOB9_CCOBn_MASK)

/*! @name FCCOB8 - Flash Common Command Object Registers */
#define FTFA_FCCOB8_CCOBn_MASK                   (0xFFU)
#define FTFA_FCCOB8_CCOBn_SHIFT                  (0U)
#define FTFA_FCCOB8_CCOBn(x)                     (((uint8_t)(((uint8_t)(x)) << FTFA_FCCOB8_CCOBn_SHIFT)) & FTFA_FCCOB8_CCOBn_MASK)

/*! @name FPROT3 - Program Flash Protection Registers */
#define FTFA_FPROT3_PROT_MASK                    (0xFFU)
#define FTFA_FPROT3_PROT_SHIFT                   (0U)
#define FTFA_FPROT3_PROT(x)                      (((uint8_t)(((uint8_t)(x)) << FTFA_FPROT3_PROT_SHIFT)) & FTFA_FPROT3_PROT_MASK)

/*! @name FPROT2 - Program Flash Protection Registers */
#define FTFA_FPROT2_PROT_MASK                    (0xFFU)
#define FTFA_FPROT2_PROT_SHIFT                   (0U)
#define FTFA_FPROT2_PROT(x)                      (((uint8_t)(((uint8_t)(x)) << FTFA_FPROT2_PROT_SHIFT)) & FTFA_FPROT2_PROT_MASK)

/*! @name FPROT1 - Program Flash Protection Registers */
#define FTFA_FPROT1_PROT_MASK                    (0xFFU)
#define FTFA_FPROT1_PROT_SHIFT                   (0U)
#define FTFA_FPROT1_PROT(x)                      (((uint8_t)(((uint8_t)(x)) << FTFA_FPROT1_PROT_SHIFT)) & FTFA_FPROT1_PROT_MASK)

/*! @name FPROT0 - Program Flash Protection Registers */
#define FTFA_FPROT0_PROT_MASK                    (0xFFU)
#define FTFA_FPROT0_PROT_SHIFT                   (0U)
#define FTFA_FPROT0_PROT(x)                      (((uint8_t)(((uint8_t)(x)) << FTFA_FPROT0_PROT_SHIFT)) & FTFA_FPROT0_PROT_MASK)


/*!
 * @}
 */ /* end of group FTFA_Register_Masks */


/* FTFA - Peripheral instance base addresses */
/** Peripheral FTFA base address */
#define FTFA_BASE                                (0x40020000u)
/** Peripheral FTFA base pointer */
#define FTFA                                     ((FTFA_Type *)FTFA_BASE)
/** Array initializer of FTFA peripheral base addresses */
#define FTFA_BASE_ADDRS                          { FTFA_BASE }
/** Array initializer of FTFA peripheral base pointers */
#define FTFA_BASE_PTRS                           { FTFA }
/** Interrupt vectors for the FTFA peripheral type */
#define FTFA_COMMAND_COMPLETE_IRQS               { FTFA_IRQn }

/*!
 * @}
 */ /* end of group FTFA_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- GPIO Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Peripheral_Access_Layer GPIO Peripheral Access Layer
 * @{
 */

/** GPIO - Register Layout Typedef */
typedef struct {
  __IO uint8_t PDOR;                               /**< Port Data Output Register, offset: 0x0 */
       uint8_t RESERVED_0[3];
  __O  uint8_t PSOR;                               /**< Port Set Output Register, offset: 0x4 */
       uint8_t RESERVED_1[3];
  __O  uint8_t PCOR;                               /**< Port Clear Output Register, offset: 0x8 */
       uint8_t RESERVED_2[3];
  __O  uint8_t PTOR;                               /**< Port Toggle Output Register, offset: 0xC */
       uint8_t RESERVED_3[3];
  __I  uint8_t PDIR;                               /**< Port Data Input Register, offset: 0x10 */
       uint8_t RESERVED_4[3];
  __IO uint8_t PDDR;                               /**< Port Data Direction Register, offset: 0x14 */
       uint8_t RESERVED_5[7];
  __IO uint8_t GACR;                               /**< GPIO Attribute Checker Register, offset: 0x1C */
} GPIO_Type;

/* ----------------------------------------------------------------------------
   -- GPIO Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup GPIO_Register_Masks GPIO Register Masks
 * @{
 */

/*! @name PDOR - Port Data Output Register */
#define GPIO_PDOR_PDO_MASK                       (0xFFU)
#define GPIO_PDOR_PDO_SHIFT                      (0U)
#define GPIO_PDOR_PDO(x)                         (((uint8_t)(((uint8_t)(x)) << GPIO_PDOR_PDO_SHIFT)) & GPIO_PDOR_PDO_MASK)

/*! @name PSOR - Port Set Output Register */
#define GPIO_PSOR_PTSO_MASK                      (0xFFU)
#define GPIO_PSOR_PTSO_SHIFT                     (0U)
#define GPIO_PSOR_PTSO(x)                        (((uint8_t)(((uint8_t)(x)) << GPIO_PSOR_PTSO_SHIFT)) & GPIO_PSOR_PTSO_MASK)

/*! @name PCOR - Port Clear Output Register */
#define GPIO_PCOR_PTCO_MASK                      (0xFFU)
#define GPIO_PCOR_PTCO_SHIFT                     (0U)
#define GPIO_PCOR_PTCO(x)                        (((uint8_t)(((uint8_t)(x)) << GPIO_PCOR_PTCO_SHIFT)) & GPIO_PCOR_PTCO_MASK)

/*! @name PTOR - Port Toggle Output Register */
#define GPIO_PTOR_PTTO_MASK                      (0xFFU)
#define GPIO_PTOR_PTTO_SHIFT                     (0U)
#define GPIO_PTOR_PTTO(x)                        (((uint8_t)(((uint8_t)(x)) << GPIO_PTOR_PTTO_SHIFT)) & GPIO_PTOR_PTTO_MASK)

/*! @name PDIR - Port Data Input Register */
#define GPIO_PDIR_PDI_MASK                       (0xFFU)
#define GPIO_PDIR_PDI_SHIFT                      (0U)
#define GPIO_PDIR_PDI(x)                         (((uint8_t)(((uint8_t)(x)) << GPIO_PDIR_PDI_SHIFT)) & GPIO_PDIR_PDI_MASK)

/*! @name PDDR - Port Data Direction Register */
#define GPIO_PDDR_PDD_MASK                       (0xFFU)
#define GPIO_PDDR_PDD_SHIFT                      (0U)
#define GPIO_PDDR_PDD(x)                         (((uint8_t)(((uint8_t)(x)) << GPIO_PDDR_PDD_SHIFT)) & GPIO_PDDR_PDD_MASK)

/*! @name GACR - GPIO Attribute Checker Register */
#define GPIO_GACR_ACB_MASK                       (0x7U)
#define GPIO_GACR_ACB_SHIFT                      (0U)
#define GPIO_GACR_ACB(x)                         (((uint8_t)(((uint8_t)(x)) << GPIO_GACR_ACB_SHIFT)) & GPIO_GACR_ACB_MASK)
#define GPIO_GACR_ROB_MASK                       (0x80U)
#define GPIO_GACR_ROB_SHIFT                      (7U)
#define GPIO_GACR_ROB(x)                         (((uint8_t)(((uint8_t)(x)) << GPIO_GACR_ROB_SHIFT)) & GPIO_GACR_ROB_MASK)


/*!
 * @}
 */ /* end of group GPIO_Register_Masks */


/* GPIO - Peripheral instance base addresses */
/** Peripheral GPIOA base address */
#define GPIOA_BASE                               (0x400FF000u)
/** Peripheral GPIOA base pointer */
#define GPIOA                                    ((GPIO_Type *)GPIOA_BASE)
/** Peripheral GPIOB base address */
#define GPIOB_BASE                               (0x400FF001u)
/** Peripheral GPIOB base pointer */
#define GPIOB                                    ((GPIO_Type *)GPIOB_BASE)
/** Peripheral GPIOC base address */
#define GPIOC_BASE                               (0x400FF002u)
/** Peripheral GPIOC base pointer */
#define GPIOC                                    ((GPIO_Type *)GPIOC_BASE)
/** Peripheral GPIOD base address */
#define GPIOD_BASE                               (0x400FF003u)
/** Peripheral GPIOD base pointer */
#define GPIOD                                    ((GPIO_Type *)GPIOD_BASE)
/** Peripheral GPIOE base address */
#define GPIOE_BASE                               (0x400FF040u)
/** Peripheral GPIOE base pointer */
#define GPIOE                                    ((GPIO_Type *)GPIOE_BASE)
/** Peripheral GPIOF base address */
#define GPIOF_BASE                               (0x400FF041u)
/** Peripheral GPIOF base pointer */
#define GPIOF                                    ((GPIO_Type *)GPIOF_BASE)
/** Peripheral GPIOG base address */
#define GPIOG_BASE                               (0x400FF042u)
/** Peripheral GPIOG base pointer */
#define GPIOG                                    ((GPIO_Type *)GPIOG_BASE)
/** Peripheral GPIOH base address */
#define GPIOH_BASE                               (0x400FF043u)
/** Peripheral GPIOH base pointer */
#define GPIOH                                    ((GPIO_Type *)GPIOH_BASE)
/** Peripheral GPIOI base address */
#define GPIOI_BASE                               (0x400FF080u)
/** Peripheral GPIOI base pointer */
#define GPIOI                                    ((GPIO_Type *)GPIOI_BASE)
/** Peripheral GPIOJ base address */
#define GPIOJ_BASE                               (0x400FF081u)
/** Peripheral GPIOJ base pointer */
#define GPIOJ                                    ((GPIO_Type *)GPIOJ_BASE)
/** Peripheral GPIOK base address */
#define GPIOK_BASE                               (0x400FF082u)
/** Peripheral GPIOK base pointer */
#define GPIOK                                    ((GPIO_Type *)GPIOK_BASE)
/** Peripheral GPIOL base address */
#define GPIOL_BASE                               (0x400FF083u)
/** Peripheral GPIOL base pointer */
#define GPIOL                                    ((GPIO_Type *)GPIOL_BASE)
/** Peripheral GPIOM base address */
#define GPIOM_BASE                               (0x400FF0C0u)
/** Peripheral GPIOM base pointer */
#define GPIOM                                    ((GPIO_Type *)GPIOM_BASE)
/** Array initializer of GPIO peripheral base addresses */
#define GPIO_BASE_ADDRS                          { GPIOA_BASE, GPIOB_BASE, GPIOC_BASE, GPIOD_BASE, GPIOE_BASE, GPIOF_BASE, GPIOG_BASE, GPIOH_BASE, GPIOI_BASE, GPIOJ_BASE, GPIOK_BASE, GPIOL_BASE, GPIOM_BASE }
/** Array initializer of GPIO peripheral base pointers */
#define GPIO_BASE_PTRS                           { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG, GPIOH, GPIOI, GPIOJ, GPIOK, GPIOL, GPIOM }

/*!
 * @}
 */ /* end of group GPIO_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- I2C Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Peripheral_Access_Layer I2C Peripheral Access Layer
 * @{
 */

/** I2C - Register Layout Typedef */
typedef struct {
  __IO uint8_t A1;                                 /**< I2C Address Register 1, offset: 0x0 */
  __IO uint8_t F;                                  /**< I2C Frequency Divider register, offset: 0x1 */
  __IO uint8_t C1;                                 /**< I2C Control Register 1, offset: 0x2 */
  __IO uint8_t S;                                  /**< I2C Status register, offset: 0x3 */
  __IO uint8_t D;                                  /**< I2C Data I/O register, offset: 0x4 */
  __IO uint8_t C2;                                 /**< I2C Control Register 2, offset: 0x5 */
  __IO uint8_t FLT;                                /**< I2C Programmable Input Glitch Filter Register, offset: 0x6 */
  __IO uint8_t RA;                                 /**< I2C Range Address register, offset: 0x7 */
  __IO uint8_t SMB;                                /**< I2C SMBus Control and Status register, offset: 0x8 */
  __IO uint8_t A2;                                 /**< I2C Address Register 2, offset: 0x9 */
  __IO uint8_t SLTH;                               /**< I2C SCL Low Timeout Register High, offset: 0xA */
  __IO uint8_t SLTL;                               /**< I2C SCL Low Timeout Register Low, offset: 0xB */
  __IO uint8_t S2;                                 /**< I2C Status register 2, offset: 0xC */
} I2C_Type;

/* ----------------------------------------------------------------------------
   -- I2C Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup I2C_Register_Masks I2C Register Masks
 * @{
 */

/*! @name A1 - I2C Address Register 1 */
#define I2C_A1_AD_MASK                           (0xFEU)
#define I2C_A1_AD_SHIFT                          (1U)
#define I2C_A1_AD(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_A1_AD_SHIFT)) & I2C_A1_AD_MASK)

/*! @name F - I2C Frequency Divider register */
#define I2C_F_ICR_MASK                           (0x3FU)
#define I2C_F_ICR_SHIFT                          (0U)
#define I2C_F_ICR(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_F_ICR_SHIFT)) & I2C_F_ICR_MASK)
#define I2C_F_MULT_MASK                          (0xC0U)
#define I2C_F_MULT_SHIFT                         (6U)
#define I2C_F_MULT(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_F_MULT_SHIFT)) & I2C_F_MULT_MASK)

/*! @name C1 - I2C Control Register 1 */
#define I2C_C1_DMAEN_MASK                        (0x1U)
#define I2C_C1_DMAEN_SHIFT                       (0U)
#define I2C_C1_DMAEN(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_C1_DMAEN_SHIFT)) & I2C_C1_DMAEN_MASK)
#define I2C_C1_WUEN_MASK                         (0x2U)
#define I2C_C1_WUEN_SHIFT                        (1U)
#define I2C_C1_WUEN(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_C1_WUEN_SHIFT)) & I2C_C1_WUEN_MASK)
#define I2C_C1_RSTA_MASK                         (0x4U)
#define I2C_C1_RSTA_SHIFT                        (2U)
#define I2C_C1_RSTA(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_C1_RSTA_SHIFT)) & I2C_C1_RSTA_MASK)
#define I2C_C1_TXAK_MASK                         (0x8U)
#define I2C_C1_TXAK_SHIFT                        (3U)
#define I2C_C1_TXAK(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_C1_TXAK_SHIFT)) & I2C_C1_TXAK_MASK)
#define I2C_C1_TX_MASK                           (0x10U)
#define I2C_C1_TX_SHIFT                          (4U)
#define I2C_C1_TX(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_C1_TX_SHIFT)) & I2C_C1_TX_MASK)
#define I2C_C1_MST_MASK                          (0x20U)
#define I2C_C1_MST_SHIFT                         (5U)
#define I2C_C1_MST(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_C1_MST_SHIFT)) & I2C_C1_MST_MASK)
#define I2C_C1_IICIE_MASK                        (0x40U)
#define I2C_C1_IICIE_SHIFT                       (6U)
#define I2C_C1_IICIE(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_C1_IICIE_SHIFT)) & I2C_C1_IICIE_MASK)
#define I2C_C1_IICEN_MASK                        (0x80U)
#define I2C_C1_IICEN_SHIFT                       (7U)
#define I2C_C1_IICEN(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_C1_IICEN_SHIFT)) & I2C_C1_IICEN_MASK)

/*! @name S - I2C Status register */
#define I2C_S_RXAK_MASK                          (0x1U)
#define I2C_S_RXAK_SHIFT                         (0U)
#define I2C_S_RXAK(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_S_RXAK_SHIFT)) & I2C_S_RXAK_MASK)
#define I2C_S_IICIF_MASK                         (0x2U)
#define I2C_S_IICIF_SHIFT                        (1U)
#define I2C_S_IICIF(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_S_IICIF_SHIFT)) & I2C_S_IICIF_MASK)
#define I2C_S_SRW_MASK                           (0x4U)
#define I2C_S_SRW_SHIFT                          (2U)
#define I2C_S_SRW(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_S_SRW_SHIFT)) & I2C_S_SRW_MASK)
#define I2C_S_RAM_MASK                           (0x8U)
#define I2C_S_RAM_SHIFT                          (3U)
#define I2C_S_RAM(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_S_RAM_SHIFT)) & I2C_S_RAM_MASK)
#define I2C_S_ARBL_MASK                          (0x10U)
#define I2C_S_ARBL_SHIFT                         (4U)
#define I2C_S_ARBL(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_S_ARBL_SHIFT)) & I2C_S_ARBL_MASK)
#define I2C_S_BUSY_MASK                          (0x20U)
#define I2C_S_BUSY_SHIFT                         (5U)
#define I2C_S_BUSY(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_S_BUSY_SHIFT)) & I2C_S_BUSY_MASK)
#define I2C_S_IAAS_MASK                          (0x40U)
#define I2C_S_IAAS_SHIFT                         (6U)
#define I2C_S_IAAS(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_S_IAAS_SHIFT)) & I2C_S_IAAS_MASK)
#define I2C_S_TCF_MASK                           (0x80U)
#define I2C_S_TCF_SHIFT                          (7U)
#define I2C_S_TCF(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_S_TCF_SHIFT)) & I2C_S_TCF_MASK)

/*! @name D - I2C Data I/O register */
#define I2C_D_DATA_MASK                          (0xFFU)
#define I2C_D_DATA_SHIFT                         (0U)
#define I2C_D_DATA(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_D_DATA_SHIFT)) & I2C_D_DATA_MASK)

/*! @name C2 - I2C Control Register 2 */
#define I2C_C2_AD_MASK                           (0x7U)
#define I2C_C2_AD_SHIFT                          (0U)
#define I2C_C2_AD(x)                             (((uint8_t)(((uint8_t)(x)) << I2C_C2_AD_SHIFT)) & I2C_C2_AD_MASK)
#define I2C_C2_RMEN_MASK                         (0x8U)
#define I2C_C2_RMEN_SHIFT                        (3U)
#define I2C_C2_RMEN(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_C2_RMEN_SHIFT)) & I2C_C2_RMEN_MASK)
#define I2C_C2_SBRC_MASK                         (0x10U)
#define I2C_C2_SBRC_SHIFT                        (4U)
#define I2C_C2_SBRC(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_C2_SBRC_SHIFT)) & I2C_C2_SBRC_MASK)
#define I2C_C2_HDRS_MASK                         (0x20U)
#define I2C_C2_HDRS_SHIFT                        (5U)
#define I2C_C2_HDRS(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_C2_HDRS_SHIFT)) & I2C_C2_HDRS_MASK)
#define I2C_C2_ADEXT_MASK                        (0x40U)
#define I2C_C2_ADEXT_SHIFT                       (6U)
#define I2C_C2_ADEXT(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_C2_ADEXT_SHIFT)) & I2C_C2_ADEXT_MASK)
#define I2C_C2_GCAEN_MASK                        (0x80U)
#define I2C_C2_GCAEN_SHIFT                       (7U)
#define I2C_C2_GCAEN(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_C2_GCAEN_SHIFT)) & I2C_C2_GCAEN_MASK)

/*! @name FLT - I2C Programmable Input Glitch Filter Register */
#define I2C_FLT_FLT_MASK                         (0xFU)
#define I2C_FLT_FLT_SHIFT                        (0U)
#define I2C_FLT_FLT(x)                           (((uint8_t)(((uint8_t)(x)) << I2C_FLT_FLT_SHIFT)) & I2C_FLT_FLT_MASK)
#define I2C_FLT_STARTF_MASK                      (0x10U)
#define I2C_FLT_STARTF_SHIFT                     (4U)
#define I2C_FLT_STARTF(x)                        (((uint8_t)(((uint8_t)(x)) << I2C_FLT_STARTF_SHIFT)) & I2C_FLT_STARTF_MASK)
#define I2C_FLT_SSIE_MASK                        (0x20U)
#define I2C_FLT_SSIE_SHIFT                       (5U)
#define I2C_FLT_SSIE(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_FLT_SSIE_SHIFT)) & I2C_FLT_SSIE_MASK)
#define I2C_FLT_STOPF_MASK                       (0x40U)
#define I2C_FLT_STOPF_SHIFT                      (6U)
#define I2C_FLT_STOPF(x)                         (((uint8_t)(((uint8_t)(x)) << I2C_FLT_STOPF_SHIFT)) & I2C_FLT_STOPF_MASK)
#define I2C_FLT_SHEN_MASK                        (0x80U)
#define I2C_FLT_SHEN_SHIFT                       (7U)
#define I2C_FLT_SHEN(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_FLT_SHEN_SHIFT)) & I2C_FLT_SHEN_MASK)

/*! @name RA - I2C Range Address register */
#define I2C_RA_RAD_MASK                          (0xFEU)
#define I2C_RA_RAD_SHIFT                         (1U)
#define I2C_RA_RAD(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_RA_RAD_SHIFT)) & I2C_RA_RAD_MASK)

/*! @name SMB - I2C SMBus Control and Status register */
#define I2C_SMB_SHTF2IE_MASK                     (0x1U)
#define I2C_SMB_SHTF2IE_SHIFT                    (0U)
#define I2C_SMB_SHTF2IE(x)                       (((uint8_t)(((uint8_t)(x)) << I2C_SMB_SHTF2IE_SHIFT)) & I2C_SMB_SHTF2IE_MASK)
#define I2C_SMB_SHTF2_MASK                       (0x2U)
#define I2C_SMB_SHTF2_SHIFT                      (1U)
#define I2C_SMB_SHTF2(x)                         (((uint8_t)(((uint8_t)(x)) << I2C_SMB_SHTF2_SHIFT)) & I2C_SMB_SHTF2_MASK)
#define I2C_SMB_SHTF1_MASK                       (0x4U)
#define I2C_SMB_SHTF1_SHIFT                      (2U)
#define I2C_SMB_SHTF1(x)                         (((uint8_t)(((uint8_t)(x)) << I2C_SMB_SHTF1_SHIFT)) & I2C_SMB_SHTF1_MASK)
#define I2C_SMB_SLTF_MASK                        (0x8U)
#define I2C_SMB_SLTF_SHIFT                       (3U)
#define I2C_SMB_SLTF(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_SMB_SLTF_SHIFT)) & I2C_SMB_SLTF_MASK)
#define I2C_SMB_TCKSEL_MASK                      (0x10U)
#define I2C_SMB_TCKSEL_SHIFT                     (4U)
#define I2C_SMB_TCKSEL(x)                        (((uint8_t)(((uint8_t)(x)) << I2C_SMB_TCKSEL_SHIFT)) & I2C_SMB_TCKSEL_MASK)
#define I2C_SMB_SIICAEN_MASK                     (0x20U)
#define I2C_SMB_SIICAEN_SHIFT                    (5U)
#define I2C_SMB_SIICAEN(x)                       (((uint8_t)(((uint8_t)(x)) << I2C_SMB_SIICAEN_SHIFT)) & I2C_SMB_SIICAEN_MASK)
#define I2C_SMB_ALERTEN_MASK                     (0x40U)
#define I2C_SMB_ALERTEN_SHIFT                    (6U)
#define I2C_SMB_ALERTEN(x)                       (((uint8_t)(((uint8_t)(x)) << I2C_SMB_ALERTEN_SHIFT)) & I2C_SMB_ALERTEN_MASK)
#define I2C_SMB_FACK_MASK                        (0x80U)
#define I2C_SMB_FACK_SHIFT                       (7U)
#define I2C_SMB_FACK(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_SMB_FACK_SHIFT)) & I2C_SMB_FACK_MASK)

/*! @name A2 - I2C Address Register 2 */
#define I2C_A2_SAD_MASK                          (0xFEU)
#define I2C_A2_SAD_SHIFT                         (1U)
#define I2C_A2_SAD(x)                            (((uint8_t)(((uint8_t)(x)) << I2C_A2_SAD_SHIFT)) & I2C_A2_SAD_MASK)

/*! @name SLTH - I2C SCL Low Timeout Register High */
#define I2C_SLTH_SSLT_MASK                       (0xFFU)
#define I2C_SLTH_SSLT_SHIFT                      (0U)
#define I2C_SLTH_SSLT(x)                         (((uint8_t)(((uint8_t)(x)) << I2C_SLTH_SSLT_SHIFT)) & I2C_SLTH_SSLT_MASK)

/*! @name SLTL - I2C SCL Low Timeout Register Low */
#define I2C_SLTL_SSLT_MASK                       (0xFFU)
#define I2C_SLTL_SSLT_SHIFT                      (0U)
#define I2C_SLTL_SSLT(x)                         (((uint8_t)(((uint8_t)(x)) << I2C_SLTL_SSLT_SHIFT)) & I2C_SLTL_SSLT_MASK)

/*! @name S2 - I2C Status register 2 */
#define I2C_S2_EMPTY_MASK                        (0x1U)
#define I2C_S2_EMPTY_SHIFT                       (0U)
#define I2C_S2_EMPTY(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_S2_EMPTY_SHIFT)) & I2C_S2_EMPTY_MASK)
#define I2C_S2_ERROR_MASK                        (0x2U)
#define I2C_S2_ERROR_SHIFT                       (1U)
#define I2C_S2_ERROR(x)                          (((uint8_t)(((uint8_t)(x)) << I2C_S2_ERROR_SHIFT)) & I2C_S2_ERROR_MASK)


/*!
 * @}
 */ /* end of group I2C_Register_Masks */


/* I2C - Peripheral instance base addresses */
/** Peripheral I2C0 base address */
#define I2C0_BASE                                (0x40067000u)
/** Peripheral I2C0 base pointer */
#define I2C0                                     ((I2C_Type *)I2C0_BASE)
/** Peripheral I2C1 base address */
#define I2C1_BASE                                (0x40068000u)
/** Peripheral I2C1 base pointer */
#define I2C1                                     ((I2C_Type *)I2C1_BASE)
/** Array initializer of I2C peripheral base addresses */
#define I2C_BASE_ADDRS                           { I2C0_BASE, I2C1_BASE }
/** Array initializer of I2C peripheral base pointers */
#define I2C_BASE_PTRS                            { I2C0, I2C1 }
/** Interrupt vectors for the I2C peripheral type */
#define I2C_IRQS                                 { I2C0_I2C1_IRQn, I2C0_I2C1_IRQn }

/*!
 * @}
 */ /* end of group I2C_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- LCD Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCD_Peripheral_Access_Layer LCD Peripheral Access Layer
 * @{
 */

/** LCD - Register Layout Typedef */
typedef struct {
  __IO uint32_t GCR;                               /**< LCD General Control Register, offset: 0x0 */
  __IO uint32_t AR;                                /**< LCD Auxiliary Register, offset: 0x4 */
  __IO uint32_t FDCR;                              /**< LCD Fault Detect Control Register, offset: 0x8 */
  __IO uint32_t FDSR;                              /**< LCD Fault Detect Status Register, offset: 0xC */
  __IO uint32_t PEN[2];                            /**< LCD Pin Enable register, array offset: 0x10, array step: 0x4 */
  __IO uint32_t BPEN[2];                           /**< LCD Back Plane Enable register, array offset: 0x18, array step: 0x4 */
  union {                                          /* offset: 0x20 */
    __IO uint32_t WF[16];                            /**< LCD Waveform register, array offset: 0x20, array step: 0x4 */
    __IO uint8_t WF8B[64];                           /**< LCD Waveform Register 0...LCD Waveform Register 63., array offset: 0x20, array step: 0x1 */
  };
} LCD_Type;

/* ----------------------------------------------------------------------------
   -- LCD Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LCD_Register_Masks LCD Register Masks
 * @{
 */

/*! @name GCR - LCD General Control Register */
#define LCD_GCR_DUTY_MASK                        (0x7U)
#define LCD_GCR_DUTY_SHIFT                       (0U)
#define LCD_GCR_DUTY(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_GCR_DUTY_SHIFT)) & LCD_GCR_DUTY_MASK)
#define LCD_GCR_LCLK_MASK                        (0x38U)
#define LCD_GCR_LCLK_SHIFT                       (3U)
#define LCD_GCR_LCLK(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_GCR_LCLK_SHIFT)) & LCD_GCR_LCLK_MASK)
#define LCD_GCR_SOURCE_MASK                      (0x40U)
#define LCD_GCR_SOURCE_SHIFT                     (6U)
#define LCD_GCR_SOURCE(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_GCR_SOURCE_SHIFT)) & LCD_GCR_SOURCE_MASK)
#define LCD_GCR_LCDEN_MASK                       (0x80U)
#define LCD_GCR_LCDEN_SHIFT                      (7U)
#define LCD_GCR_LCDEN(x)                         (((uint32_t)(((uint32_t)(x)) << LCD_GCR_LCDEN_SHIFT)) & LCD_GCR_LCDEN_MASK)
#define LCD_GCR_LCDSTP_MASK                      (0x100U)
#define LCD_GCR_LCDSTP_SHIFT                     (8U)
#define LCD_GCR_LCDSTP(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_GCR_LCDSTP_SHIFT)) & LCD_GCR_LCDSTP_MASK)
#define LCD_GCR_LCDDOZE_MASK                     (0x200U)
#define LCD_GCR_LCDDOZE_SHIFT                    (9U)
#define LCD_GCR_LCDDOZE(x)                       (((uint32_t)(((uint32_t)(x)) << LCD_GCR_LCDDOZE_SHIFT)) & LCD_GCR_LCDDOZE_MASK)
#define LCD_GCR_ALTDIV_MASK                      (0x3000U)
#define LCD_GCR_ALTDIV_SHIFT                     (12U)
#define LCD_GCR_ALTDIV(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_GCR_ALTDIV_SHIFT)) & LCD_GCR_ALTDIV_MASK)
#define LCD_GCR_FDCIEN_MASK                      (0x4000U)
#define LCD_GCR_FDCIEN_SHIFT                     (14U)
#define LCD_GCR_FDCIEN(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_GCR_FDCIEN_SHIFT)) & LCD_GCR_FDCIEN_MASK)
#define LCD_GCR_VSUPPLY_MASK                     (0x20000U)
#define LCD_GCR_VSUPPLY_SHIFT                    (17U)
#define LCD_GCR_VSUPPLY(x)                       (((uint32_t)(((uint32_t)(x)) << LCD_GCR_VSUPPLY_SHIFT)) & LCD_GCR_VSUPPLY_MASK)
#define LCD_GCR_LADJ_MASK                        (0x300000U)
#define LCD_GCR_LADJ_SHIFT                       (20U)
#define LCD_GCR_LADJ(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_GCR_LADJ_SHIFT)) & LCD_GCR_LADJ_MASK)
#define LCD_GCR_CPSEL_MASK                       (0x800000U)
#define LCD_GCR_CPSEL_SHIFT                      (23U)
#define LCD_GCR_CPSEL(x)                         (((uint32_t)(((uint32_t)(x)) << LCD_GCR_CPSEL_SHIFT)) & LCD_GCR_CPSEL_MASK)
#define LCD_GCR_RVTRIM_MASK                      (0xF000000U)
#define LCD_GCR_RVTRIM_SHIFT                     (24U)
#define LCD_GCR_RVTRIM(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_GCR_RVTRIM_SHIFT)) & LCD_GCR_RVTRIM_MASK)
#define LCD_GCR_RVEN_MASK                        (0x80000000U)
#define LCD_GCR_RVEN_SHIFT                       (31U)
#define LCD_GCR_RVEN(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_GCR_RVEN_SHIFT)) & LCD_GCR_RVEN_MASK)

/*! @name AR - LCD Auxiliary Register */
#define LCD_AR_BRATE_MASK                        (0x7U)
#define LCD_AR_BRATE_SHIFT                       (0U)
#define LCD_AR_BRATE(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_AR_BRATE_SHIFT)) & LCD_AR_BRATE_MASK)
#define LCD_AR_BMODE_MASK                        (0x8U)
#define LCD_AR_BMODE_SHIFT                       (3U)
#define LCD_AR_BMODE(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_AR_BMODE_SHIFT)) & LCD_AR_BMODE_MASK)
#define LCD_AR_BLANK_MASK                        (0x20U)
#define LCD_AR_BLANK_SHIFT                       (5U)
#define LCD_AR_BLANK(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_AR_BLANK_SHIFT)) & LCD_AR_BLANK_MASK)
#define LCD_AR_ALT_MASK                          (0x40U)
#define LCD_AR_ALT_SHIFT                         (6U)
#define LCD_AR_ALT(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_AR_ALT_SHIFT)) & LCD_AR_ALT_MASK)
#define LCD_AR_BLINK_MASK                        (0x80U)
#define LCD_AR_BLINK_SHIFT                       (7U)
#define LCD_AR_BLINK(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_AR_BLINK_SHIFT)) & LCD_AR_BLINK_MASK)
#define LCD_AR_LCDIF_MASK                        (0x8000U)
#define LCD_AR_LCDIF_SHIFT                       (15U)
#define LCD_AR_LCDIF(x)                          (((uint32_t)(((uint32_t)(x)) << LCD_AR_LCDIF_SHIFT)) & LCD_AR_LCDIF_MASK)

/*! @name FDCR - LCD Fault Detect Control Register */
#define LCD_FDCR_FDPINID_MASK                    (0x3FU)
#define LCD_FDCR_FDPINID_SHIFT                   (0U)
#define LCD_FDCR_FDPINID(x)                      (((uint32_t)(((uint32_t)(x)) << LCD_FDCR_FDPINID_SHIFT)) & LCD_FDCR_FDPINID_MASK)
#define LCD_FDCR_FDBPEN_MASK                     (0x40U)
#define LCD_FDCR_FDBPEN_SHIFT                    (6U)
#define LCD_FDCR_FDBPEN(x)                       (((uint32_t)(((uint32_t)(x)) << LCD_FDCR_FDBPEN_SHIFT)) & LCD_FDCR_FDBPEN_MASK)
#define LCD_FDCR_FDEN_MASK                       (0x80U)
#define LCD_FDCR_FDEN_SHIFT                      (7U)
#define LCD_FDCR_FDEN(x)                         (((uint32_t)(((uint32_t)(x)) << LCD_FDCR_FDEN_SHIFT)) & LCD_FDCR_FDEN_MASK)
#define LCD_FDCR_FDSWW_MASK                      (0xE00U)
#define LCD_FDCR_FDSWW_SHIFT                     (9U)
#define LCD_FDCR_FDSWW(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_FDCR_FDSWW_SHIFT)) & LCD_FDCR_FDSWW_MASK)
#define LCD_FDCR_FDPRS_MASK                      (0x7000U)
#define LCD_FDCR_FDPRS_SHIFT                     (12U)
#define LCD_FDCR_FDPRS(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_FDCR_FDPRS_SHIFT)) & LCD_FDCR_FDPRS_MASK)

/*! @name FDSR - LCD Fault Detect Status Register */
#define LCD_FDSR_FDCNT_MASK                      (0xFFU)
#define LCD_FDSR_FDCNT_SHIFT                     (0U)
#define LCD_FDSR_FDCNT(x)                        (((uint32_t)(((uint32_t)(x)) << LCD_FDSR_FDCNT_SHIFT)) & LCD_FDSR_FDCNT_MASK)
#define LCD_FDSR_FDCF_MASK                       (0x8000U)
#define LCD_FDSR_FDCF_SHIFT                      (15U)
#define LCD_FDSR_FDCF(x)                         (((uint32_t)(((uint32_t)(x)) << LCD_FDSR_FDCF_SHIFT)) & LCD_FDSR_FDCF_MASK)

/*! @name PEN - LCD Pin Enable register */
#define LCD_PEN_PEN_MASK                         (0xFFFFFFFFU)
#define LCD_PEN_PEN_SHIFT                        (0U)
#define LCD_PEN_PEN(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_PEN_PEN_SHIFT)) & LCD_PEN_PEN_MASK)

/* The count of LCD_PEN */
#define LCD_PEN_COUNT                            (2U)

/*! @name BPEN - LCD Back Plane Enable register */
#define LCD_BPEN_BPEN_MASK                       (0xFFFFFFFFU)
#define LCD_BPEN_BPEN_SHIFT                      (0U)
#define LCD_BPEN_BPEN(x)                         (((uint32_t)(((uint32_t)(x)) << LCD_BPEN_BPEN_SHIFT)) & LCD_BPEN_BPEN_MASK)

/* The count of LCD_BPEN */
#define LCD_BPEN_COUNT                           (2U)

/*! @name WF - LCD Waveform register */
#define LCD_WF_WF0_MASK                          (0xFFU)
#define LCD_WF_WF0_SHIFT                         (0U)
#define LCD_WF_WF0(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF0_SHIFT)) & LCD_WF_WF0_MASK)
#define LCD_WF_WF60_MASK                         (0xFFU)
#define LCD_WF_WF60_SHIFT                        (0U)
#define LCD_WF_WF60(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF60_SHIFT)) & LCD_WF_WF60_MASK)
#define LCD_WF_WF56_MASK                         (0xFFU)
#define LCD_WF_WF56_SHIFT                        (0U)
#define LCD_WF_WF56(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF56_SHIFT)) & LCD_WF_WF56_MASK)
#define LCD_WF_WF52_MASK                         (0xFFU)
#define LCD_WF_WF52_SHIFT                        (0U)
#define LCD_WF_WF52(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF52_SHIFT)) & LCD_WF_WF52_MASK)
#define LCD_WF_WF4_MASK                          (0xFFU)
#define LCD_WF_WF4_SHIFT                         (0U)
#define LCD_WF_WF4(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF4_SHIFT)) & LCD_WF_WF4_MASK)
#define LCD_WF_WF48_MASK                         (0xFFU)
#define LCD_WF_WF48_SHIFT                        (0U)
#define LCD_WF_WF48(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF48_SHIFT)) & LCD_WF_WF48_MASK)
#define LCD_WF_WF44_MASK                         (0xFFU)
#define LCD_WF_WF44_SHIFT                        (0U)
#define LCD_WF_WF44(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF44_SHIFT)) & LCD_WF_WF44_MASK)
#define LCD_WF_WF40_MASK                         (0xFFU)
#define LCD_WF_WF40_SHIFT                        (0U)
#define LCD_WF_WF40(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF40_SHIFT)) & LCD_WF_WF40_MASK)
#define LCD_WF_WF8_MASK                          (0xFFU)
#define LCD_WF_WF8_SHIFT                         (0U)
#define LCD_WF_WF8(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF8_SHIFT)) & LCD_WF_WF8_MASK)
#define LCD_WF_WF36_MASK                         (0xFFU)
#define LCD_WF_WF36_SHIFT                        (0U)
#define LCD_WF_WF36(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF36_SHIFT)) & LCD_WF_WF36_MASK)
#define LCD_WF_WF32_MASK                         (0xFFU)
#define LCD_WF_WF32_SHIFT                        (0U)
#define LCD_WF_WF32(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF32_SHIFT)) & LCD_WF_WF32_MASK)
#define LCD_WF_WF28_MASK                         (0xFFU)
#define LCD_WF_WF28_SHIFT                        (0U)
#define LCD_WF_WF28(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF28_SHIFT)) & LCD_WF_WF28_MASK)
#define LCD_WF_WF12_MASK                         (0xFFU)
#define LCD_WF_WF12_SHIFT                        (0U)
#define LCD_WF_WF12(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF12_SHIFT)) & LCD_WF_WF12_MASK)
#define LCD_WF_WF24_MASK                         (0xFFU)
#define LCD_WF_WF24_SHIFT                        (0U)
#define LCD_WF_WF24(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF24_SHIFT)) & LCD_WF_WF24_MASK)
#define LCD_WF_WF20_MASK                         (0xFFU)
#define LCD_WF_WF20_SHIFT                        (0U)
#define LCD_WF_WF20(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF20_SHIFT)) & LCD_WF_WF20_MASK)
#define LCD_WF_WF16_MASK                         (0xFFU)
#define LCD_WF_WF16_SHIFT                        (0U)
#define LCD_WF_WF16(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF16_SHIFT)) & LCD_WF_WF16_MASK)
#define LCD_WF_WF5_MASK                          (0xFF00U)
#define LCD_WF_WF5_SHIFT                         (8U)
#define LCD_WF_WF5(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF5_SHIFT)) & LCD_WF_WF5_MASK)
#define LCD_WF_WF49_MASK                         (0xFF00U)
#define LCD_WF_WF49_SHIFT                        (8U)
#define LCD_WF_WF49(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF49_SHIFT)) & LCD_WF_WF49_MASK)
#define LCD_WF_WF45_MASK                         (0xFF00U)
#define LCD_WF_WF45_SHIFT                        (8U)
#define LCD_WF_WF45(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF45_SHIFT)) & LCD_WF_WF45_MASK)
#define LCD_WF_WF61_MASK                         (0xFF00U)
#define LCD_WF_WF61_SHIFT                        (8U)
#define LCD_WF_WF61(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF61_SHIFT)) & LCD_WF_WF61_MASK)
#define LCD_WF_WF25_MASK                         (0xFF00U)
#define LCD_WF_WF25_SHIFT                        (8U)
#define LCD_WF_WF25(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF25_SHIFT)) & LCD_WF_WF25_MASK)
#define LCD_WF_WF17_MASK                         (0xFF00U)
#define LCD_WF_WF17_SHIFT                        (8U)
#define LCD_WF_WF17(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF17_SHIFT)) & LCD_WF_WF17_MASK)
#define LCD_WF_WF41_MASK                         (0xFF00U)
#define LCD_WF_WF41_SHIFT                        (8U)
#define LCD_WF_WF41(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF41_SHIFT)) & LCD_WF_WF41_MASK)
#define LCD_WF_WF13_MASK                         (0xFF00U)
#define LCD_WF_WF13_SHIFT                        (8U)
#define LCD_WF_WF13(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF13_SHIFT)) & LCD_WF_WF13_MASK)
#define LCD_WF_WF57_MASK                         (0xFF00U)
#define LCD_WF_WF57_SHIFT                        (8U)
#define LCD_WF_WF57(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF57_SHIFT)) & LCD_WF_WF57_MASK)
#define LCD_WF_WF53_MASK                         (0xFF00U)
#define LCD_WF_WF53_SHIFT                        (8U)
#define LCD_WF_WF53(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF53_SHIFT)) & LCD_WF_WF53_MASK)
#define LCD_WF_WF37_MASK                         (0xFF00U)
#define LCD_WF_WF37_SHIFT                        (8U)
#define LCD_WF_WF37(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF37_SHIFT)) & LCD_WF_WF37_MASK)
#define LCD_WF_WF9_MASK                          (0xFF00U)
#define LCD_WF_WF9_SHIFT                         (8U)
#define LCD_WF_WF9(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF9_SHIFT)) & LCD_WF_WF9_MASK)
#define LCD_WF_WF1_MASK                          (0xFF00U)
#define LCD_WF_WF1_SHIFT                         (8U)
#define LCD_WF_WF1(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF1_SHIFT)) & LCD_WF_WF1_MASK)
#define LCD_WF_WF29_MASK                         (0xFF00U)
#define LCD_WF_WF29_SHIFT                        (8U)
#define LCD_WF_WF29(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF29_SHIFT)) & LCD_WF_WF29_MASK)
#define LCD_WF_WF33_MASK                         (0xFF00U)
#define LCD_WF_WF33_SHIFT                        (8U)
#define LCD_WF_WF33(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF33_SHIFT)) & LCD_WF_WF33_MASK)
#define LCD_WF_WF21_MASK                         (0xFF00U)
#define LCD_WF_WF21_SHIFT                        (8U)
#define LCD_WF_WF21(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF21_SHIFT)) & LCD_WF_WF21_MASK)
#define LCD_WF_WF26_MASK                         (0xFF0000U)
#define LCD_WF_WF26_SHIFT                        (16U)
#define LCD_WF_WF26(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF26_SHIFT)) & LCD_WF_WF26_MASK)
#define LCD_WF_WF46_MASK                         (0xFF0000U)
#define LCD_WF_WF46_SHIFT                        (16U)
#define LCD_WF_WF46(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF46_SHIFT)) & LCD_WF_WF46_MASK)
#define LCD_WF_WF6_MASK                          (0xFF0000U)
#define LCD_WF_WF6_SHIFT                         (16U)
#define LCD_WF_WF6(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF6_SHIFT)) & LCD_WF_WF6_MASK)
#define LCD_WF_WF42_MASK                         (0xFF0000U)
#define LCD_WF_WF42_SHIFT                        (16U)
#define LCD_WF_WF42(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF42_SHIFT)) & LCD_WF_WF42_MASK)
#define LCD_WF_WF18_MASK                         (0xFF0000U)
#define LCD_WF_WF18_SHIFT                        (16U)
#define LCD_WF_WF18(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF18_SHIFT)) & LCD_WF_WF18_MASK)
#define LCD_WF_WF38_MASK                         (0xFF0000U)
#define LCD_WF_WF38_SHIFT                        (16U)
#define LCD_WF_WF38(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF38_SHIFT)) & LCD_WF_WF38_MASK)
#define LCD_WF_WF22_MASK                         (0xFF0000U)
#define LCD_WF_WF22_SHIFT                        (16U)
#define LCD_WF_WF22(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF22_SHIFT)) & LCD_WF_WF22_MASK)
#define LCD_WF_WF34_MASK                         (0xFF0000U)
#define LCD_WF_WF34_SHIFT                        (16U)
#define LCD_WF_WF34(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF34_SHIFT)) & LCD_WF_WF34_MASK)
#define LCD_WF_WF50_MASK                         (0xFF0000U)
#define LCD_WF_WF50_SHIFT                        (16U)
#define LCD_WF_WF50(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF50_SHIFT)) & LCD_WF_WF50_MASK)
#define LCD_WF_WF14_MASK                         (0xFF0000U)
#define LCD_WF_WF14_SHIFT                        (16U)
#define LCD_WF_WF14(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF14_SHIFT)) & LCD_WF_WF14_MASK)
#define LCD_WF_WF54_MASK                         (0xFF0000U)
#define LCD_WF_WF54_SHIFT                        (16U)
#define LCD_WF_WF54(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF54_SHIFT)) & LCD_WF_WF54_MASK)
#define LCD_WF_WF2_MASK                          (0xFF0000U)
#define LCD_WF_WF2_SHIFT                         (16U)
#define LCD_WF_WF2(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF2_SHIFT)) & LCD_WF_WF2_MASK)
#define LCD_WF_WF58_MASK                         (0xFF0000U)
#define LCD_WF_WF58_SHIFT                        (16U)
#define LCD_WF_WF58(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF58_SHIFT)) & LCD_WF_WF58_MASK)
#define LCD_WF_WF30_MASK                         (0xFF0000U)
#define LCD_WF_WF30_SHIFT                        (16U)
#define LCD_WF_WF30(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF30_SHIFT)) & LCD_WF_WF30_MASK)
#define LCD_WF_WF62_MASK                         (0xFF0000U)
#define LCD_WF_WF62_SHIFT                        (16U)
#define LCD_WF_WF62(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF62_SHIFT)) & LCD_WF_WF62_MASK)
#define LCD_WF_WF10_MASK                         (0xFF0000U)
#define LCD_WF_WF10_SHIFT                        (16U)
#define LCD_WF_WF10(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF10_SHIFT)) & LCD_WF_WF10_MASK)
#define LCD_WF_WF63_MASK                         (0xFF000000U)
#define LCD_WF_WF63_SHIFT                        (24U)
#define LCD_WF_WF63(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF63_SHIFT)) & LCD_WF_WF63_MASK)
#define LCD_WF_WF59_MASK                         (0xFF000000U)
#define LCD_WF_WF59_SHIFT                        (24U)
#define LCD_WF_WF59(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF59_SHIFT)) & LCD_WF_WF59_MASK)
#define LCD_WF_WF55_MASK                         (0xFF000000U)
#define LCD_WF_WF55_SHIFT                        (24U)
#define LCD_WF_WF55(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF55_SHIFT)) & LCD_WF_WF55_MASK)
#define LCD_WF_WF3_MASK                          (0xFF000000U)
#define LCD_WF_WF3_SHIFT                         (24U)
#define LCD_WF_WF3(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF3_SHIFT)) & LCD_WF_WF3_MASK)
#define LCD_WF_WF51_MASK                         (0xFF000000U)
#define LCD_WF_WF51_SHIFT                        (24U)
#define LCD_WF_WF51(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF51_SHIFT)) & LCD_WF_WF51_MASK)
#define LCD_WF_WF47_MASK                         (0xFF000000U)
#define LCD_WF_WF47_SHIFT                        (24U)
#define LCD_WF_WF47(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF47_SHIFT)) & LCD_WF_WF47_MASK)
#define LCD_WF_WF43_MASK                         (0xFF000000U)
#define LCD_WF_WF43_SHIFT                        (24U)
#define LCD_WF_WF43(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF43_SHIFT)) & LCD_WF_WF43_MASK)
#define LCD_WF_WF7_MASK                          (0xFF000000U)
#define LCD_WF_WF7_SHIFT                         (24U)
#define LCD_WF_WF7(x)                            (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF7_SHIFT)) & LCD_WF_WF7_MASK)
#define LCD_WF_WF39_MASK                         (0xFF000000U)
#define LCD_WF_WF39_SHIFT                        (24U)
#define LCD_WF_WF39(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF39_SHIFT)) & LCD_WF_WF39_MASK)
#define LCD_WF_WF35_MASK                         (0xFF000000U)
#define LCD_WF_WF35_SHIFT                        (24U)
#define LCD_WF_WF35(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF35_SHIFT)) & LCD_WF_WF35_MASK)
#define LCD_WF_WF31_MASK                         (0xFF000000U)
#define LCD_WF_WF31_SHIFT                        (24U)
#define LCD_WF_WF31(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF31_SHIFT)) & LCD_WF_WF31_MASK)
#define LCD_WF_WF11_MASK                         (0xFF000000U)
#define LCD_WF_WF11_SHIFT                        (24U)
#define LCD_WF_WF11(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF11_SHIFT)) & LCD_WF_WF11_MASK)
#define LCD_WF_WF27_MASK                         (0xFF000000U)
#define LCD_WF_WF27_SHIFT                        (24U)
#define LCD_WF_WF27(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF27_SHIFT)) & LCD_WF_WF27_MASK)
#define LCD_WF_WF23_MASK                         (0xFF000000U)
#define LCD_WF_WF23_SHIFT                        (24U)
#define LCD_WF_WF23(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF23_SHIFT)) & LCD_WF_WF23_MASK)
#define LCD_WF_WF19_MASK                         (0xFF000000U)
#define LCD_WF_WF19_SHIFT                        (24U)
#define LCD_WF_WF19(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF19_SHIFT)) & LCD_WF_WF19_MASK)
#define LCD_WF_WF15_MASK                         (0xFF000000U)
#define LCD_WF_WF15_SHIFT                        (24U)
#define LCD_WF_WF15(x)                           (((uint32_t)(((uint32_t)(x)) << LCD_WF_WF15_SHIFT)) & LCD_WF_WF15_MASK)

/* The count of LCD_WF */
#define LCD_WF_COUNT                             (16U)

/*! @name WF8B - LCD Waveform Register 0...LCD Waveform Register 63. */
#define LCD_WF8B_BPALCD0_MASK                    (0x1U)
#define LCD_WF8B_BPALCD0_SHIFT                   (0U)
#define LCD_WF8B_BPALCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD0_SHIFT)) & LCD_WF8B_BPALCD0_MASK)
#define LCD_WF8B_BPALCD63_MASK                   (0x1U)
#define LCD_WF8B_BPALCD63_SHIFT                  (0U)
#define LCD_WF8B_BPALCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD63_SHIFT)) & LCD_WF8B_BPALCD63_MASK)
#define LCD_WF8B_BPALCD62_MASK                   (0x1U)
#define LCD_WF8B_BPALCD62_SHIFT                  (0U)
#define LCD_WF8B_BPALCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD62_SHIFT)) & LCD_WF8B_BPALCD62_MASK)
#define LCD_WF8B_BPALCD61_MASK                   (0x1U)
#define LCD_WF8B_BPALCD61_SHIFT                  (0U)
#define LCD_WF8B_BPALCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD61_SHIFT)) & LCD_WF8B_BPALCD61_MASK)
#define LCD_WF8B_BPALCD60_MASK                   (0x1U)
#define LCD_WF8B_BPALCD60_SHIFT                  (0U)
#define LCD_WF8B_BPALCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD60_SHIFT)) & LCD_WF8B_BPALCD60_MASK)
#define LCD_WF8B_BPALCD59_MASK                   (0x1U)
#define LCD_WF8B_BPALCD59_SHIFT                  (0U)
#define LCD_WF8B_BPALCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD59_SHIFT)) & LCD_WF8B_BPALCD59_MASK)
#define LCD_WF8B_BPALCD58_MASK                   (0x1U)
#define LCD_WF8B_BPALCD58_SHIFT                  (0U)
#define LCD_WF8B_BPALCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD58_SHIFT)) & LCD_WF8B_BPALCD58_MASK)
#define LCD_WF8B_BPALCD57_MASK                   (0x1U)
#define LCD_WF8B_BPALCD57_SHIFT                  (0U)
#define LCD_WF8B_BPALCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD57_SHIFT)) & LCD_WF8B_BPALCD57_MASK)
#define LCD_WF8B_BPALCD1_MASK                    (0x1U)
#define LCD_WF8B_BPALCD1_SHIFT                   (0U)
#define LCD_WF8B_BPALCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD1_SHIFT)) & LCD_WF8B_BPALCD1_MASK)
#define LCD_WF8B_BPALCD56_MASK                   (0x1U)
#define LCD_WF8B_BPALCD56_SHIFT                  (0U)
#define LCD_WF8B_BPALCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD56_SHIFT)) & LCD_WF8B_BPALCD56_MASK)
#define LCD_WF8B_BPALCD55_MASK                   (0x1U)
#define LCD_WF8B_BPALCD55_SHIFT                  (0U)
#define LCD_WF8B_BPALCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD55_SHIFT)) & LCD_WF8B_BPALCD55_MASK)
#define LCD_WF8B_BPALCD54_MASK                   (0x1U)
#define LCD_WF8B_BPALCD54_SHIFT                  (0U)
#define LCD_WF8B_BPALCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD54_SHIFT)) & LCD_WF8B_BPALCD54_MASK)
#define LCD_WF8B_BPALCD53_MASK                   (0x1U)
#define LCD_WF8B_BPALCD53_SHIFT                  (0U)
#define LCD_WF8B_BPALCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD53_SHIFT)) & LCD_WF8B_BPALCD53_MASK)
#define LCD_WF8B_BPALCD52_MASK                   (0x1U)
#define LCD_WF8B_BPALCD52_SHIFT                  (0U)
#define LCD_WF8B_BPALCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD52_SHIFT)) & LCD_WF8B_BPALCD52_MASK)
#define LCD_WF8B_BPALCD51_MASK                   (0x1U)
#define LCD_WF8B_BPALCD51_SHIFT                  (0U)
#define LCD_WF8B_BPALCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD51_SHIFT)) & LCD_WF8B_BPALCD51_MASK)
#define LCD_WF8B_BPALCD50_MASK                   (0x1U)
#define LCD_WF8B_BPALCD50_SHIFT                  (0U)
#define LCD_WF8B_BPALCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD50_SHIFT)) & LCD_WF8B_BPALCD50_MASK)
#define LCD_WF8B_BPALCD2_MASK                    (0x1U)
#define LCD_WF8B_BPALCD2_SHIFT                   (0U)
#define LCD_WF8B_BPALCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD2_SHIFT)) & LCD_WF8B_BPALCD2_MASK)
#define LCD_WF8B_BPALCD49_MASK                   (0x1U)
#define LCD_WF8B_BPALCD49_SHIFT                  (0U)
#define LCD_WF8B_BPALCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD49_SHIFT)) & LCD_WF8B_BPALCD49_MASK)
#define LCD_WF8B_BPALCD48_MASK                   (0x1U)
#define LCD_WF8B_BPALCD48_SHIFT                  (0U)
#define LCD_WF8B_BPALCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD48_SHIFT)) & LCD_WF8B_BPALCD48_MASK)
#define LCD_WF8B_BPALCD47_MASK                   (0x1U)
#define LCD_WF8B_BPALCD47_SHIFT                  (0U)
#define LCD_WF8B_BPALCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD47_SHIFT)) & LCD_WF8B_BPALCD47_MASK)
#define LCD_WF8B_BPALCD46_MASK                   (0x1U)
#define LCD_WF8B_BPALCD46_SHIFT                  (0U)
#define LCD_WF8B_BPALCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD46_SHIFT)) & LCD_WF8B_BPALCD46_MASK)
#define LCD_WF8B_BPALCD45_MASK                   (0x1U)
#define LCD_WF8B_BPALCD45_SHIFT                  (0U)
#define LCD_WF8B_BPALCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD45_SHIFT)) & LCD_WF8B_BPALCD45_MASK)
#define LCD_WF8B_BPALCD44_MASK                   (0x1U)
#define LCD_WF8B_BPALCD44_SHIFT                  (0U)
#define LCD_WF8B_BPALCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD44_SHIFT)) & LCD_WF8B_BPALCD44_MASK)
#define LCD_WF8B_BPALCD43_MASK                   (0x1U)
#define LCD_WF8B_BPALCD43_SHIFT                  (0U)
#define LCD_WF8B_BPALCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD43_SHIFT)) & LCD_WF8B_BPALCD43_MASK)
#define LCD_WF8B_BPALCD3_MASK                    (0x1U)
#define LCD_WF8B_BPALCD3_SHIFT                   (0U)
#define LCD_WF8B_BPALCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD3_SHIFT)) & LCD_WF8B_BPALCD3_MASK)
#define LCD_WF8B_BPALCD42_MASK                   (0x1U)
#define LCD_WF8B_BPALCD42_SHIFT                  (0U)
#define LCD_WF8B_BPALCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD42_SHIFT)) & LCD_WF8B_BPALCD42_MASK)
#define LCD_WF8B_BPALCD41_MASK                   (0x1U)
#define LCD_WF8B_BPALCD41_SHIFT                  (0U)
#define LCD_WF8B_BPALCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD41_SHIFT)) & LCD_WF8B_BPALCD41_MASK)
#define LCD_WF8B_BPALCD40_MASK                   (0x1U)
#define LCD_WF8B_BPALCD40_SHIFT                  (0U)
#define LCD_WF8B_BPALCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD40_SHIFT)) & LCD_WF8B_BPALCD40_MASK)
#define LCD_WF8B_BPALCD39_MASK                   (0x1U)
#define LCD_WF8B_BPALCD39_SHIFT                  (0U)
#define LCD_WF8B_BPALCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD39_SHIFT)) & LCD_WF8B_BPALCD39_MASK)
#define LCD_WF8B_BPALCD38_MASK                   (0x1U)
#define LCD_WF8B_BPALCD38_SHIFT                  (0U)
#define LCD_WF8B_BPALCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD38_SHIFT)) & LCD_WF8B_BPALCD38_MASK)
#define LCD_WF8B_BPALCD37_MASK                   (0x1U)
#define LCD_WF8B_BPALCD37_SHIFT                  (0U)
#define LCD_WF8B_BPALCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD37_SHIFT)) & LCD_WF8B_BPALCD37_MASK)
#define LCD_WF8B_BPALCD36_MASK                   (0x1U)
#define LCD_WF8B_BPALCD36_SHIFT                  (0U)
#define LCD_WF8B_BPALCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD36_SHIFT)) & LCD_WF8B_BPALCD36_MASK)
#define LCD_WF8B_BPALCD4_MASK                    (0x1U)
#define LCD_WF8B_BPALCD4_SHIFT                   (0U)
#define LCD_WF8B_BPALCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD4_SHIFT)) & LCD_WF8B_BPALCD4_MASK)
#define LCD_WF8B_BPALCD35_MASK                   (0x1U)
#define LCD_WF8B_BPALCD35_SHIFT                  (0U)
#define LCD_WF8B_BPALCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD35_SHIFT)) & LCD_WF8B_BPALCD35_MASK)
#define LCD_WF8B_BPALCD34_MASK                   (0x1U)
#define LCD_WF8B_BPALCD34_SHIFT                  (0U)
#define LCD_WF8B_BPALCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD34_SHIFT)) & LCD_WF8B_BPALCD34_MASK)
#define LCD_WF8B_BPALCD33_MASK                   (0x1U)
#define LCD_WF8B_BPALCD33_SHIFT                  (0U)
#define LCD_WF8B_BPALCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD33_SHIFT)) & LCD_WF8B_BPALCD33_MASK)
#define LCD_WF8B_BPALCD32_MASK                   (0x1U)
#define LCD_WF8B_BPALCD32_SHIFT                  (0U)
#define LCD_WF8B_BPALCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD32_SHIFT)) & LCD_WF8B_BPALCD32_MASK)
#define LCD_WF8B_BPALCD31_MASK                   (0x1U)
#define LCD_WF8B_BPALCD31_SHIFT                  (0U)
#define LCD_WF8B_BPALCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD31_SHIFT)) & LCD_WF8B_BPALCD31_MASK)
#define LCD_WF8B_BPALCD30_MASK                   (0x1U)
#define LCD_WF8B_BPALCD30_SHIFT                  (0U)
#define LCD_WF8B_BPALCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD30_SHIFT)) & LCD_WF8B_BPALCD30_MASK)
#define LCD_WF8B_BPALCD29_MASK                   (0x1U)
#define LCD_WF8B_BPALCD29_SHIFT                  (0U)
#define LCD_WF8B_BPALCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD29_SHIFT)) & LCD_WF8B_BPALCD29_MASK)
#define LCD_WF8B_BPALCD5_MASK                    (0x1U)
#define LCD_WF8B_BPALCD5_SHIFT                   (0U)
#define LCD_WF8B_BPALCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD5_SHIFT)) & LCD_WF8B_BPALCD5_MASK)
#define LCD_WF8B_BPALCD28_MASK                   (0x1U)
#define LCD_WF8B_BPALCD28_SHIFT                  (0U)
#define LCD_WF8B_BPALCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD28_SHIFT)) & LCD_WF8B_BPALCD28_MASK)
#define LCD_WF8B_BPALCD27_MASK                   (0x1U)
#define LCD_WF8B_BPALCD27_SHIFT                  (0U)
#define LCD_WF8B_BPALCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD27_SHIFT)) & LCD_WF8B_BPALCD27_MASK)
#define LCD_WF8B_BPALCD26_MASK                   (0x1U)
#define LCD_WF8B_BPALCD26_SHIFT                  (0U)
#define LCD_WF8B_BPALCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD26_SHIFT)) & LCD_WF8B_BPALCD26_MASK)
#define LCD_WF8B_BPALCD25_MASK                   (0x1U)
#define LCD_WF8B_BPALCD25_SHIFT                  (0U)
#define LCD_WF8B_BPALCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD25_SHIFT)) & LCD_WF8B_BPALCD25_MASK)
#define LCD_WF8B_BPALCD24_MASK                   (0x1U)
#define LCD_WF8B_BPALCD24_SHIFT                  (0U)
#define LCD_WF8B_BPALCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD24_SHIFT)) & LCD_WF8B_BPALCD24_MASK)
#define LCD_WF8B_BPALCD23_MASK                   (0x1U)
#define LCD_WF8B_BPALCD23_SHIFT                  (0U)
#define LCD_WF8B_BPALCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD23_SHIFT)) & LCD_WF8B_BPALCD23_MASK)
#define LCD_WF8B_BPALCD22_MASK                   (0x1U)
#define LCD_WF8B_BPALCD22_SHIFT                  (0U)
#define LCD_WF8B_BPALCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD22_SHIFT)) & LCD_WF8B_BPALCD22_MASK)
#define LCD_WF8B_BPALCD6_MASK                    (0x1U)
#define LCD_WF8B_BPALCD6_SHIFT                   (0U)
#define LCD_WF8B_BPALCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD6_SHIFT)) & LCD_WF8B_BPALCD6_MASK)
#define LCD_WF8B_BPALCD21_MASK                   (0x1U)
#define LCD_WF8B_BPALCD21_SHIFT                  (0U)
#define LCD_WF8B_BPALCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD21_SHIFT)) & LCD_WF8B_BPALCD21_MASK)
#define LCD_WF8B_BPALCD20_MASK                   (0x1U)
#define LCD_WF8B_BPALCD20_SHIFT                  (0U)
#define LCD_WF8B_BPALCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD20_SHIFT)) & LCD_WF8B_BPALCD20_MASK)
#define LCD_WF8B_BPALCD19_MASK                   (0x1U)
#define LCD_WF8B_BPALCD19_SHIFT                  (0U)
#define LCD_WF8B_BPALCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD19_SHIFT)) & LCD_WF8B_BPALCD19_MASK)
#define LCD_WF8B_BPALCD18_MASK                   (0x1U)
#define LCD_WF8B_BPALCD18_SHIFT                  (0U)
#define LCD_WF8B_BPALCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD18_SHIFT)) & LCD_WF8B_BPALCD18_MASK)
#define LCD_WF8B_BPALCD17_MASK                   (0x1U)
#define LCD_WF8B_BPALCD17_SHIFT                  (0U)
#define LCD_WF8B_BPALCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD17_SHIFT)) & LCD_WF8B_BPALCD17_MASK)
#define LCD_WF8B_BPALCD16_MASK                   (0x1U)
#define LCD_WF8B_BPALCD16_SHIFT                  (0U)
#define LCD_WF8B_BPALCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD16_SHIFT)) & LCD_WF8B_BPALCD16_MASK)
#define LCD_WF8B_BPALCD15_MASK                   (0x1U)
#define LCD_WF8B_BPALCD15_SHIFT                  (0U)
#define LCD_WF8B_BPALCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD15_SHIFT)) & LCD_WF8B_BPALCD15_MASK)
#define LCD_WF8B_BPALCD7_MASK                    (0x1U)
#define LCD_WF8B_BPALCD7_SHIFT                   (0U)
#define LCD_WF8B_BPALCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD7_SHIFT)) & LCD_WF8B_BPALCD7_MASK)
#define LCD_WF8B_BPALCD14_MASK                   (0x1U)
#define LCD_WF8B_BPALCD14_SHIFT                  (0U)
#define LCD_WF8B_BPALCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD14_SHIFT)) & LCD_WF8B_BPALCD14_MASK)
#define LCD_WF8B_BPALCD13_MASK                   (0x1U)
#define LCD_WF8B_BPALCD13_SHIFT                  (0U)
#define LCD_WF8B_BPALCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD13_SHIFT)) & LCD_WF8B_BPALCD13_MASK)
#define LCD_WF8B_BPALCD12_MASK                   (0x1U)
#define LCD_WF8B_BPALCD12_SHIFT                  (0U)
#define LCD_WF8B_BPALCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD12_SHIFT)) & LCD_WF8B_BPALCD12_MASK)
#define LCD_WF8B_BPALCD11_MASK                   (0x1U)
#define LCD_WF8B_BPALCD11_SHIFT                  (0U)
#define LCD_WF8B_BPALCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD11_SHIFT)) & LCD_WF8B_BPALCD11_MASK)
#define LCD_WF8B_BPALCD10_MASK                   (0x1U)
#define LCD_WF8B_BPALCD10_SHIFT                  (0U)
#define LCD_WF8B_BPALCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD10_SHIFT)) & LCD_WF8B_BPALCD10_MASK)
#define LCD_WF8B_BPALCD9_MASK                    (0x1U)
#define LCD_WF8B_BPALCD9_SHIFT                   (0U)
#define LCD_WF8B_BPALCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD9_SHIFT)) & LCD_WF8B_BPALCD9_MASK)
#define LCD_WF8B_BPALCD8_MASK                    (0x1U)
#define LCD_WF8B_BPALCD8_SHIFT                   (0U)
#define LCD_WF8B_BPALCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPALCD8_SHIFT)) & LCD_WF8B_BPALCD8_MASK)
#define LCD_WF8B_BPBLCD1_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD1_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD1_SHIFT)) & LCD_WF8B_BPBLCD1_MASK)
#define LCD_WF8B_BPBLCD32_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD32_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD32_SHIFT)) & LCD_WF8B_BPBLCD32_MASK)
#define LCD_WF8B_BPBLCD30_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD30_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD30_SHIFT)) & LCD_WF8B_BPBLCD30_MASK)
#define LCD_WF8B_BPBLCD60_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD60_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD60_SHIFT)) & LCD_WF8B_BPBLCD60_MASK)
#define LCD_WF8B_BPBLCD24_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD24_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD24_SHIFT)) & LCD_WF8B_BPBLCD24_MASK)
#define LCD_WF8B_BPBLCD28_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD28_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD28_SHIFT)) & LCD_WF8B_BPBLCD28_MASK)
#define LCD_WF8B_BPBLCD23_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD23_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD23_SHIFT)) & LCD_WF8B_BPBLCD23_MASK)
#define LCD_WF8B_BPBLCD48_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD48_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD48_SHIFT)) & LCD_WF8B_BPBLCD48_MASK)
#define LCD_WF8B_BPBLCD10_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD10_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD10_SHIFT)) & LCD_WF8B_BPBLCD10_MASK)
#define LCD_WF8B_BPBLCD15_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD15_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD15_SHIFT)) & LCD_WF8B_BPBLCD15_MASK)
#define LCD_WF8B_BPBLCD36_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD36_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD36_SHIFT)) & LCD_WF8B_BPBLCD36_MASK)
#define LCD_WF8B_BPBLCD44_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD44_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD44_SHIFT)) & LCD_WF8B_BPBLCD44_MASK)
#define LCD_WF8B_BPBLCD62_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD62_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD62_SHIFT)) & LCD_WF8B_BPBLCD62_MASK)
#define LCD_WF8B_BPBLCD53_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD53_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD53_SHIFT)) & LCD_WF8B_BPBLCD53_MASK)
#define LCD_WF8B_BPBLCD22_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD22_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD22_SHIFT)) & LCD_WF8B_BPBLCD22_MASK)
#define LCD_WF8B_BPBLCD47_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD47_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD47_SHIFT)) & LCD_WF8B_BPBLCD47_MASK)
#define LCD_WF8B_BPBLCD33_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD33_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD33_SHIFT)) & LCD_WF8B_BPBLCD33_MASK)
#define LCD_WF8B_BPBLCD2_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD2_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD2_SHIFT)) & LCD_WF8B_BPBLCD2_MASK)
#define LCD_WF8B_BPBLCD49_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD49_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD49_SHIFT)) & LCD_WF8B_BPBLCD49_MASK)
#define LCD_WF8B_BPBLCD0_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD0_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD0_SHIFT)) & LCD_WF8B_BPBLCD0_MASK)
#define LCD_WF8B_BPBLCD55_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD55_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD55_SHIFT)) & LCD_WF8B_BPBLCD55_MASK)
#define LCD_WF8B_BPBLCD56_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD56_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD56_SHIFT)) & LCD_WF8B_BPBLCD56_MASK)
#define LCD_WF8B_BPBLCD21_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD21_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD21_SHIFT)) & LCD_WF8B_BPBLCD21_MASK)
#define LCD_WF8B_BPBLCD6_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD6_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD6_SHIFT)) & LCD_WF8B_BPBLCD6_MASK)
#define LCD_WF8B_BPBLCD29_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD29_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD29_SHIFT)) & LCD_WF8B_BPBLCD29_MASK)
#define LCD_WF8B_BPBLCD25_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD25_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD25_SHIFT)) & LCD_WF8B_BPBLCD25_MASK)
#define LCD_WF8B_BPBLCD8_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD8_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD8_SHIFT)) & LCD_WF8B_BPBLCD8_MASK)
#define LCD_WF8B_BPBLCD54_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD54_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD54_SHIFT)) & LCD_WF8B_BPBLCD54_MASK)
#define LCD_WF8B_BPBLCD38_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD38_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD38_SHIFT)) & LCD_WF8B_BPBLCD38_MASK)
#define LCD_WF8B_BPBLCD43_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD43_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD43_SHIFT)) & LCD_WF8B_BPBLCD43_MASK)
#define LCD_WF8B_BPBLCD20_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD20_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD20_SHIFT)) & LCD_WF8B_BPBLCD20_MASK)
#define LCD_WF8B_BPBLCD9_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD9_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD9_SHIFT)) & LCD_WF8B_BPBLCD9_MASK)
#define LCD_WF8B_BPBLCD7_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD7_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD7_SHIFT)) & LCD_WF8B_BPBLCD7_MASK)
#define LCD_WF8B_BPBLCD50_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD50_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD50_SHIFT)) & LCD_WF8B_BPBLCD50_MASK)
#define LCD_WF8B_BPBLCD40_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD40_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD40_SHIFT)) & LCD_WF8B_BPBLCD40_MASK)
#define LCD_WF8B_BPBLCD63_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD63_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD63_SHIFT)) & LCD_WF8B_BPBLCD63_MASK)
#define LCD_WF8B_BPBLCD26_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD26_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD26_SHIFT)) & LCD_WF8B_BPBLCD26_MASK)
#define LCD_WF8B_BPBLCD12_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD12_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD12_SHIFT)) & LCD_WF8B_BPBLCD12_MASK)
#define LCD_WF8B_BPBLCD19_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD19_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD19_SHIFT)) & LCD_WF8B_BPBLCD19_MASK)
#define LCD_WF8B_BPBLCD34_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD34_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD34_SHIFT)) & LCD_WF8B_BPBLCD34_MASK)
#define LCD_WF8B_BPBLCD39_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD39_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD39_SHIFT)) & LCD_WF8B_BPBLCD39_MASK)
#define LCD_WF8B_BPBLCD59_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD59_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD59_SHIFT)) & LCD_WF8B_BPBLCD59_MASK)
#define LCD_WF8B_BPBLCD61_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD61_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD61_SHIFT)) & LCD_WF8B_BPBLCD61_MASK)
#define LCD_WF8B_BPBLCD37_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD37_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD37_SHIFT)) & LCD_WF8B_BPBLCD37_MASK)
#define LCD_WF8B_BPBLCD31_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD31_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD31_SHIFT)) & LCD_WF8B_BPBLCD31_MASK)
#define LCD_WF8B_BPBLCD58_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD58_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD58_SHIFT)) & LCD_WF8B_BPBLCD58_MASK)
#define LCD_WF8B_BPBLCD18_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD18_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD18_SHIFT)) & LCD_WF8B_BPBLCD18_MASK)
#define LCD_WF8B_BPBLCD45_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD45_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD45_SHIFT)) & LCD_WF8B_BPBLCD45_MASK)
#define LCD_WF8B_BPBLCD27_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD27_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD27_SHIFT)) & LCD_WF8B_BPBLCD27_MASK)
#define LCD_WF8B_BPBLCD14_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD14_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD14_SHIFT)) & LCD_WF8B_BPBLCD14_MASK)
#define LCD_WF8B_BPBLCD51_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD51_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD51_SHIFT)) & LCD_WF8B_BPBLCD51_MASK)
#define LCD_WF8B_BPBLCD52_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD52_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD52_SHIFT)) & LCD_WF8B_BPBLCD52_MASK)
#define LCD_WF8B_BPBLCD4_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD4_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD4_SHIFT)) & LCD_WF8B_BPBLCD4_MASK)
#define LCD_WF8B_BPBLCD35_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD35_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD35_SHIFT)) & LCD_WF8B_BPBLCD35_MASK)
#define LCD_WF8B_BPBLCD17_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD17_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD17_SHIFT)) & LCD_WF8B_BPBLCD17_MASK)
#define LCD_WF8B_BPBLCD41_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD41_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD41_SHIFT)) & LCD_WF8B_BPBLCD41_MASK)
#define LCD_WF8B_BPBLCD11_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD11_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD11_SHIFT)) & LCD_WF8B_BPBLCD11_MASK)
#define LCD_WF8B_BPBLCD46_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD46_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD46_SHIFT)) & LCD_WF8B_BPBLCD46_MASK)
#define LCD_WF8B_BPBLCD57_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD57_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD57_SHIFT)) & LCD_WF8B_BPBLCD57_MASK)
#define LCD_WF8B_BPBLCD42_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD42_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD42_SHIFT)) & LCD_WF8B_BPBLCD42_MASK)
#define LCD_WF8B_BPBLCD5_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD5_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD5_SHIFT)) & LCD_WF8B_BPBLCD5_MASK)
#define LCD_WF8B_BPBLCD3_MASK                    (0x2U)
#define LCD_WF8B_BPBLCD3_SHIFT                   (1U)
#define LCD_WF8B_BPBLCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD3_SHIFT)) & LCD_WF8B_BPBLCD3_MASK)
#define LCD_WF8B_BPBLCD16_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD16_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD16_SHIFT)) & LCD_WF8B_BPBLCD16_MASK)
#define LCD_WF8B_BPBLCD13_MASK                   (0x2U)
#define LCD_WF8B_BPBLCD13_SHIFT                  (1U)
#define LCD_WF8B_BPBLCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPBLCD13_SHIFT)) & LCD_WF8B_BPBLCD13_MASK)
#define LCD_WF8B_BPCLCD10_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD10_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD10_SHIFT)) & LCD_WF8B_BPCLCD10_MASK)
#define LCD_WF8B_BPCLCD55_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD55_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD55_SHIFT)) & LCD_WF8B_BPCLCD55_MASK)
#define LCD_WF8B_BPCLCD2_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD2_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD2_SHIFT)) & LCD_WF8B_BPCLCD2_MASK)
#define LCD_WF8B_BPCLCD23_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD23_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD23_SHIFT)) & LCD_WF8B_BPCLCD23_MASK)
#define LCD_WF8B_BPCLCD48_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD48_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD48_SHIFT)) & LCD_WF8B_BPCLCD48_MASK)
#define LCD_WF8B_BPCLCD24_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD24_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD24_SHIFT)) & LCD_WF8B_BPCLCD24_MASK)
#define LCD_WF8B_BPCLCD60_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD60_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD60_SHIFT)) & LCD_WF8B_BPCLCD60_MASK)
#define LCD_WF8B_BPCLCD47_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD47_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD47_SHIFT)) & LCD_WF8B_BPCLCD47_MASK)
#define LCD_WF8B_BPCLCD22_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD22_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD22_SHIFT)) & LCD_WF8B_BPCLCD22_MASK)
#define LCD_WF8B_BPCLCD8_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD8_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD8_SHIFT)) & LCD_WF8B_BPCLCD8_MASK)
#define LCD_WF8B_BPCLCD21_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD21_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD21_SHIFT)) & LCD_WF8B_BPCLCD21_MASK)
#define LCD_WF8B_BPCLCD49_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD49_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD49_SHIFT)) & LCD_WF8B_BPCLCD49_MASK)
#define LCD_WF8B_BPCLCD25_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD25_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD25_SHIFT)) & LCD_WF8B_BPCLCD25_MASK)
#define LCD_WF8B_BPCLCD1_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD1_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD1_SHIFT)) & LCD_WF8B_BPCLCD1_MASK)
#define LCD_WF8B_BPCLCD20_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD20_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD20_SHIFT)) & LCD_WF8B_BPCLCD20_MASK)
#define LCD_WF8B_BPCLCD50_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD50_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD50_SHIFT)) & LCD_WF8B_BPCLCD50_MASK)
#define LCD_WF8B_BPCLCD19_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD19_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD19_SHIFT)) & LCD_WF8B_BPCLCD19_MASK)
#define LCD_WF8B_BPCLCD26_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD26_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD26_SHIFT)) & LCD_WF8B_BPCLCD26_MASK)
#define LCD_WF8B_BPCLCD59_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD59_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD59_SHIFT)) & LCD_WF8B_BPCLCD59_MASK)
#define LCD_WF8B_BPCLCD61_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD61_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD61_SHIFT)) & LCD_WF8B_BPCLCD61_MASK)
#define LCD_WF8B_BPCLCD46_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD46_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD46_SHIFT)) & LCD_WF8B_BPCLCD46_MASK)
#define LCD_WF8B_BPCLCD18_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD18_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD18_SHIFT)) & LCD_WF8B_BPCLCD18_MASK)
#define LCD_WF8B_BPCLCD5_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD5_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD5_SHIFT)) & LCD_WF8B_BPCLCD5_MASK)
#define LCD_WF8B_BPCLCD63_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD63_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD63_SHIFT)) & LCD_WF8B_BPCLCD63_MASK)
#define LCD_WF8B_BPCLCD27_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD27_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD27_SHIFT)) & LCD_WF8B_BPCLCD27_MASK)
#define LCD_WF8B_BPCLCD17_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD17_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD17_SHIFT)) & LCD_WF8B_BPCLCD17_MASK)
#define LCD_WF8B_BPCLCD51_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD51_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD51_SHIFT)) & LCD_WF8B_BPCLCD51_MASK)
#define LCD_WF8B_BPCLCD9_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD9_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD9_SHIFT)) & LCD_WF8B_BPCLCD9_MASK)
#define LCD_WF8B_BPCLCD54_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD54_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD54_SHIFT)) & LCD_WF8B_BPCLCD54_MASK)
#define LCD_WF8B_BPCLCD15_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD15_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD15_SHIFT)) & LCD_WF8B_BPCLCD15_MASK)
#define LCD_WF8B_BPCLCD16_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD16_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD16_SHIFT)) & LCD_WF8B_BPCLCD16_MASK)
#define LCD_WF8B_BPCLCD14_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD14_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD14_SHIFT)) & LCD_WF8B_BPCLCD14_MASK)
#define LCD_WF8B_BPCLCD32_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD32_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD32_SHIFT)) & LCD_WF8B_BPCLCD32_MASK)
#define LCD_WF8B_BPCLCD28_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD28_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD28_SHIFT)) & LCD_WF8B_BPCLCD28_MASK)
#define LCD_WF8B_BPCLCD53_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD53_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD53_SHIFT)) & LCD_WF8B_BPCLCD53_MASK)
#define LCD_WF8B_BPCLCD33_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD33_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD33_SHIFT)) & LCD_WF8B_BPCLCD33_MASK)
#define LCD_WF8B_BPCLCD0_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD0_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD0_SHIFT)) & LCD_WF8B_BPCLCD0_MASK)
#define LCD_WF8B_BPCLCD43_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD43_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD43_SHIFT)) & LCD_WF8B_BPCLCD43_MASK)
#define LCD_WF8B_BPCLCD7_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD7_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD7_SHIFT)) & LCD_WF8B_BPCLCD7_MASK)
#define LCD_WF8B_BPCLCD4_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD4_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD4_SHIFT)) & LCD_WF8B_BPCLCD4_MASK)
#define LCD_WF8B_BPCLCD34_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD34_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD34_SHIFT)) & LCD_WF8B_BPCLCD34_MASK)
#define LCD_WF8B_BPCLCD29_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD29_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD29_SHIFT)) & LCD_WF8B_BPCLCD29_MASK)
#define LCD_WF8B_BPCLCD45_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD45_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD45_SHIFT)) & LCD_WF8B_BPCLCD45_MASK)
#define LCD_WF8B_BPCLCD57_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD57_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD57_SHIFT)) & LCD_WF8B_BPCLCD57_MASK)
#define LCD_WF8B_BPCLCD42_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD42_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD42_SHIFT)) & LCD_WF8B_BPCLCD42_MASK)
#define LCD_WF8B_BPCLCD35_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD35_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD35_SHIFT)) & LCD_WF8B_BPCLCD35_MASK)
#define LCD_WF8B_BPCLCD13_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD13_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD13_SHIFT)) & LCD_WF8B_BPCLCD13_MASK)
#define LCD_WF8B_BPCLCD36_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD36_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD36_SHIFT)) & LCD_WF8B_BPCLCD36_MASK)
#define LCD_WF8B_BPCLCD30_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD30_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD30_SHIFT)) & LCD_WF8B_BPCLCD30_MASK)
#define LCD_WF8B_BPCLCD52_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD52_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD52_SHIFT)) & LCD_WF8B_BPCLCD52_MASK)
#define LCD_WF8B_BPCLCD58_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD58_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD58_SHIFT)) & LCD_WF8B_BPCLCD58_MASK)
#define LCD_WF8B_BPCLCD41_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD41_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD41_SHIFT)) & LCD_WF8B_BPCLCD41_MASK)
#define LCD_WF8B_BPCLCD37_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD37_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD37_SHIFT)) & LCD_WF8B_BPCLCD37_MASK)
#define LCD_WF8B_BPCLCD3_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD3_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD3_SHIFT)) & LCD_WF8B_BPCLCD3_MASK)
#define LCD_WF8B_BPCLCD12_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD12_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD12_SHIFT)) & LCD_WF8B_BPCLCD12_MASK)
#define LCD_WF8B_BPCLCD11_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD11_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD11_SHIFT)) & LCD_WF8B_BPCLCD11_MASK)
#define LCD_WF8B_BPCLCD38_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD38_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD38_SHIFT)) & LCD_WF8B_BPCLCD38_MASK)
#define LCD_WF8B_BPCLCD44_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD44_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD44_SHIFT)) & LCD_WF8B_BPCLCD44_MASK)
#define LCD_WF8B_BPCLCD31_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD31_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD31_SHIFT)) & LCD_WF8B_BPCLCD31_MASK)
#define LCD_WF8B_BPCLCD40_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD40_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD40_SHIFT)) & LCD_WF8B_BPCLCD40_MASK)
#define LCD_WF8B_BPCLCD62_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD62_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD62_SHIFT)) & LCD_WF8B_BPCLCD62_MASK)
#define LCD_WF8B_BPCLCD56_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD56_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD56_SHIFT)) & LCD_WF8B_BPCLCD56_MASK)
#define LCD_WF8B_BPCLCD39_MASK                   (0x4U)
#define LCD_WF8B_BPCLCD39_SHIFT                  (2U)
#define LCD_WF8B_BPCLCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD39_SHIFT)) & LCD_WF8B_BPCLCD39_MASK)
#define LCD_WF8B_BPCLCD6_MASK                    (0x4U)
#define LCD_WF8B_BPCLCD6_SHIFT                   (2U)
#define LCD_WF8B_BPCLCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPCLCD6_SHIFT)) & LCD_WF8B_BPCLCD6_MASK)
#define LCD_WF8B_BPDLCD47_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD47_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD47_SHIFT)) & LCD_WF8B_BPDLCD47_MASK)
#define LCD_WF8B_BPDLCD23_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD23_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD23_SHIFT)) & LCD_WF8B_BPDLCD23_MASK)
#define LCD_WF8B_BPDLCD48_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD48_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD48_SHIFT)) & LCD_WF8B_BPDLCD48_MASK)
#define LCD_WF8B_BPDLCD24_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD24_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD24_SHIFT)) & LCD_WF8B_BPDLCD24_MASK)
#define LCD_WF8B_BPDLCD15_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD15_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD15_SHIFT)) & LCD_WF8B_BPDLCD15_MASK)
#define LCD_WF8B_BPDLCD22_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD22_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD22_SHIFT)) & LCD_WF8B_BPDLCD22_MASK)
#define LCD_WF8B_BPDLCD60_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD60_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD60_SHIFT)) & LCD_WF8B_BPDLCD60_MASK)
#define LCD_WF8B_BPDLCD10_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD10_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD10_SHIFT)) & LCD_WF8B_BPDLCD10_MASK)
#define LCD_WF8B_BPDLCD21_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD21_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD21_SHIFT)) & LCD_WF8B_BPDLCD21_MASK)
#define LCD_WF8B_BPDLCD49_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD49_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD49_SHIFT)) & LCD_WF8B_BPDLCD49_MASK)
#define LCD_WF8B_BPDLCD1_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD1_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD1_SHIFT)) & LCD_WF8B_BPDLCD1_MASK)
#define LCD_WF8B_BPDLCD25_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD25_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD25_SHIFT)) & LCD_WF8B_BPDLCD25_MASK)
#define LCD_WF8B_BPDLCD20_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD20_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD20_SHIFT)) & LCD_WF8B_BPDLCD20_MASK)
#define LCD_WF8B_BPDLCD2_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD2_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD2_SHIFT)) & LCD_WF8B_BPDLCD2_MASK)
#define LCD_WF8B_BPDLCD55_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD55_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD55_SHIFT)) & LCD_WF8B_BPDLCD55_MASK)
#define LCD_WF8B_BPDLCD59_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD59_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD59_SHIFT)) & LCD_WF8B_BPDLCD59_MASK)
#define LCD_WF8B_BPDLCD5_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD5_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD5_SHIFT)) & LCD_WF8B_BPDLCD5_MASK)
#define LCD_WF8B_BPDLCD19_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD19_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD19_SHIFT)) & LCD_WF8B_BPDLCD19_MASK)
#define LCD_WF8B_BPDLCD6_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD6_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD6_SHIFT)) & LCD_WF8B_BPDLCD6_MASK)
#define LCD_WF8B_BPDLCD26_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD26_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD26_SHIFT)) & LCD_WF8B_BPDLCD26_MASK)
#define LCD_WF8B_BPDLCD0_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD0_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD0_SHIFT)) & LCD_WF8B_BPDLCD0_MASK)
#define LCD_WF8B_BPDLCD50_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD50_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD50_SHIFT)) & LCD_WF8B_BPDLCD50_MASK)
#define LCD_WF8B_BPDLCD46_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD46_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD46_SHIFT)) & LCD_WF8B_BPDLCD46_MASK)
#define LCD_WF8B_BPDLCD18_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD18_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD18_SHIFT)) & LCD_WF8B_BPDLCD18_MASK)
#define LCD_WF8B_BPDLCD61_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD61_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD61_SHIFT)) & LCD_WF8B_BPDLCD61_MASK)
#define LCD_WF8B_BPDLCD9_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD9_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD9_SHIFT)) & LCD_WF8B_BPDLCD9_MASK)
#define LCD_WF8B_BPDLCD17_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD17_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD17_SHIFT)) & LCD_WF8B_BPDLCD17_MASK)
#define LCD_WF8B_BPDLCD27_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD27_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD27_SHIFT)) & LCD_WF8B_BPDLCD27_MASK)
#define LCD_WF8B_BPDLCD53_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD53_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD53_SHIFT)) & LCD_WF8B_BPDLCD53_MASK)
#define LCD_WF8B_BPDLCD51_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD51_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD51_SHIFT)) & LCD_WF8B_BPDLCD51_MASK)
#define LCD_WF8B_BPDLCD54_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD54_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD54_SHIFT)) & LCD_WF8B_BPDLCD54_MASK)
#define LCD_WF8B_BPDLCD13_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD13_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD13_SHIFT)) & LCD_WF8B_BPDLCD13_MASK)
#define LCD_WF8B_BPDLCD16_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD16_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD16_SHIFT)) & LCD_WF8B_BPDLCD16_MASK)
#define LCD_WF8B_BPDLCD32_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD32_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD32_SHIFT)) & LCD_WF8B_BPDLCD32_MASK)
#define LCD_WF8B_BPDLCD14_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD14_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD14_SHIFT)) & LCD_WF8B_BPDLCD14_MASK)
#define LCD_WF8B_BPDLCD28_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD28_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD28_SHIFT)) & LCD_WF8B_BPDLCD28_MASK)
#define LCD_WF8B_BPDLCD43_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD43_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD43_SHIFT)) & LCD_WF8B_BPDLCD43_MASK)
#define LCD_WF8B_BPDLCD4_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD4_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD4_SHIFT)) & LCD_WF8B_BPDLCD4_MASK)
#define LCD_WF8B_BPDLCD45_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD45_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD45_SHIFT)) & LCD_WF8B_BPDLCD45_MASK)
#define LCD_WF8B_BPDLCD8_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD8_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD8_SHIFT)) & LCD_WF8B_BPDLCD8_MASK)
#define LCD_WF8B_BPDLCD62_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD62_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD62_SHIFT)) & LCD_WF8B_BPDLCD62_MASK)
#define LCD_WF8B_BPDLCD33_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD33_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD33_SHIFT)) & LCD_WF8B_BPDLCD33_MASK)
#define LCD_WF8B_BPDLCD34_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD34_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD34_SHIFT)) & LCD_WF8B_BPDLCD34_MASK)
#define LCD_WF8B_BPDLCD29_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD29_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD29_SHIFT)) & LCD_WF8B_BPDLCD29_MASK)
#define LCD_WF8B_BPDLCD58_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD58_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD58_SHIFT)) & LCD_WF8B_BPDLCD58_MASK)
#define LCD_WF8B_BPDLCD57_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD57_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD57_SHIFT)) & LCD_WF8B_BPDLCD57_MASK)
#define LCD_WF8B_BPDLCD42_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD42_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD42_SHIFT)) & LCD_WF8B_BPDLCD42_MASK)
#define LCD_WF8B_BPDLCD35_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD35_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD35_SHIFT)) & LCD_WF8B_BPDLCD35_MASK)
#define LCD_WF8B_BPDLCD52_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD52_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD52_SHIFT)) & LCD_WF8B_BPDLCD52_MASK)
#define LCD_WF8B_BPDLCD7_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD7_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD7_SHIFT)) & LCD_WF8B_BPDLCD7_MASK)
#define LCD_WF8B_BPDLCD36_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD36_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD36_SHIFT)) & LCD_WF8B_BPDLCD36_MASK)
#define LCD_WF8B_BPDLCD30_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD30_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD30_SHIFT)) & LCD_WF8B_BPDLCD30_MASK)
#define LCD_WF8B_BPDLCD41_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD41_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD41_SHIFT)) & LCD_WF8B_BPDLCD41_MASK)
#define LCD_WF8B_BPDLCD37_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD37_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD37_SHIFT)) & LCD_WF8B_BPDLCD37_MASK)
#define LCD_WF8B_BPDLCD44_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD44_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD44_SHIFT)) & LCD_WF8B_BPDLCD44_MASK)
#define LCD_WF8B_BPDLCD63_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD63_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD63_SHIFT)) & LCD_WF8B_BPDLCD63_MASK)
#define LCD_WF8B_BPDLCD38_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD38_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD38_SHIFT)) & LCD_WF8B_BPDLCD38_MASK)
#define LCD_WF8B_BPDLCD56_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD56_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD56_SHIFT)) & LCD_WF8B_BPDLCD56_MASK)
#define LCD_WF8B_BPDLCD40_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD40_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD40_SHIFT)) & LCD_WF8B_BPDLCD40_MASK)
#define LCD_WF8B_BPDLCD31_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD31_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD31_SHIFT)) & LCD_WF8B_BPDLCD31_MASK)
#define LCD_WF8B_BPDLCD12_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD12_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD12_SHIFT)) & LCD_WF8B_BPDLCD12_MASK)
#define LCD_WF8B_BPDLCD39_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD39_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD39_SHIFT)) & LCD_WF8B_BPDLCD39_MASK)
#define LCD_WF8B_BPDLCD3_MASK                    (0x8U)
#define LCD_WF8B_BPDLCD3_SHIFT                   (3U)
#define LCD_WF8B_BPDLCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD3_SHIFT)) & LCD_WF8B_BPDLCD3_MASK)
#define LCD_WF8B_BPDLCD11_MASK                   (0x8U)
#define LCD_WF8B_BPDLCD11_SHIFT                  (3U)
#define LCD_WF8B_BPDLCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPDLCD11_SHIFT)) & LCD_WF8B_BPDLCD11_MASK)
#define LCD_WF8B_BPELCD12_MASK                   (0x10U)
#define LCD_WF8B_BPELCD12_SHIFT                  (4U)
#define LCD_WF8B_BPELCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD12_SHIFT)) & LCD_WF8B_BPELCD12_MASK)
#define LCD_WF8B_BPELCD39_MASK                   (0x10U)
#define LCD_WF8B_BPELCD39_SHIFT                  (4U)
#define LCD_WF8B_BPELCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD39_SHIFT)) & LCD_WF8B_BPELCD39_MASK)
#define LCD_WF8B_BPELCD3_MASK                    (0x10U)
#define LCD_WF8B_BPELCD3_SHIFT                   (4U)
#define LCD_WF8B_BPELCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD3_SHIFT)) & LCD_WF8B_BPELCD3_MASK)
#define LCD_WF8B_BPELCD38_MASK                   (0x10U)
#define LCD_WF8B_BPELCD38_SHIFT                  (4U)
#define LCD_WF8B_BPELCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD38_SHIFT)) & LCD_WF8B_BPELCD38_MASK)
#define LCD_WF8B_BPELCD40_MASK                   (0x10U)
#define LCD_WF8B_BPELCD40_SHIFT                  (4U)
#define LCD_WF8B_BPELCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD40_SHIFT)) & LCD_WF8B_BPELCD40_MASK)
#define LCD_WF8B_BPELCD37_MASK                   (0x10U)
#define LCD_WF8B_BPELCD37_SHIFT                  (4U)
#define LCD_WF8B_BPELCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD37_SHIFT)) & LCD_WF8B_BPELCD37_MASK)
#define LCD_WF8B_BPELCD41_MASK                   (0x10U)
#define LCD_WF8B_BPELCD41_SHIFT                  (4U)
#define LCD_WF8B_BPELCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD41_SHIFT)) & LCD_WF8B_BPELCD41_MASK)
#define LCD_WF8B_BPELCD36_MASK                   (0x10U)
#define LCD_WF8B_BPELCD36_SHIFT                  (4U)
#define LCD_WF8B_BPELCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD36_SHIFT)) & LCD_WF8B_BPELCD36_MASK)
#define LCD_WF8B_BPELCD8_MASK                    (0x10U)
#define LCD_WF8B_BPELCD8_SHIFT                   (4U)
#define LCD_WF8B_BPELCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD8_SHIFT)) & LCD_WF8B_BPELCD8_MASK)
#define LCD_WF8B_BPELCD35_MASK                   (0x10U)
#define LCD_WF8B_BPELCD35_SHIFT                  (4U)
#define LCD_WF8B_BPELCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD35_SHIFT)) & LCD_WF8B_BPELCD35_MASK)
#define LCD_WF8B_BPELCD42_MASK                   (0x10U)
#define LCD_WF8B_BPELCD42_SHIFT                  (4U)
#define LCD_WF8B_BPELCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD42_SHIFT)) & LCD_WF8B_BPELCD42_MASK)
#define LCD_WF8B_BPELCD34_MASK                   (0x10U)
#define LCD_WF8B_BPELCD34_SHIFT                  (4U)
#define LCD_WF8B_BPELCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD34_SHIFT)) & LCD_WF8B_BPELCD34_MASK)
#define LCD_WF8B_BPELCD33_MASK                   (0x10U)
#define LCD_WF8B_BPELCD33_SHIFT                  (4U)
#define LCD_WF8B_BPELCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD33_SHIFT)) & LCD_WF8B_BPELCD33_MASK)
#define LCD_WF8B_BPELCD11_MASK                   (0x10U)
#define LCD_WF8B_BPELCD11_SHIFT                  (4U)
#define LCD_WF8B_BPELCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD11_SHIFT)) & LCD_WF8B_BPELCD11_MASK)
#define LCD_WF8B_BPELCD43_MASK                   (0x10U)
#define LCD_WF8B_BPELCD43_SHIFT                  (4U)
#define LCD_WF8B_BPELCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD43_SHIFT)) & LCD_WF8B_BPELCD43_MASK)
#define LCD_WF8B_BPELCD32_MASK                   (0x10U)
#define LCD_WF8B_BPELCD32_SHIFT                  (4U)
#define LCD_WF8B_BPELCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD32_SHIFT)) & LCD_WF8B_BPELCD32_MASK)
#define LCD_WF8B_BPELCD31_MASK                   (0x10U)
#define LCD_WF8B_BPELCD31_SHIFT                  (4U)
#define LCD_WF8B_BPELCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD31_SHIFT)) & LCD_WF8B_BPELCD31_MASK)
#define LCD_WF8B_BPELCD44_MASK                   (0x10U)
#define LCD_WF8B_BPELCD44_SHIFT                  (4U)
#define LCD_WF8B_BPELCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD44_SHIFT)) & LCD_WF8B_BPELCD44_MASK)
#define LCD_WF8B_BPELCD30_MASK                   (0x10U)
#define LCD_WF8B_BPELCD30_SHIFT                  (4U)
#define LCD_WF8B_BPELCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD30_SHIFT)) & LCD_WF8B_BPELCD30_MASK)
#define LCD_WF8B_BPELCD29_MASK                   (0x10U)
#define LCD_WF8B_BPELCD29_SHIFT                  (4U)
#define LCD_WF8B_BPELCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD29_SHIFT)) & LCD_WF8B_BPELCD29_MASK)
#define LCD_WF8B_BPELCD7_MASK                    (0x10U)
#define LCD_WF8B_BPELCD7_SHIFT                   (4U)
#define LCD_WF8B_BPELCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD7_SHIFT)) & LCD_WF8B_BPELCD7_MASK)
#define LCD_WF8B_BPELCD45_MASK                   (0x10U)
#define LCD_WF8B_BPELCD45_SHIFT                  (4U)
#define LCD_WF8B_BPELCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD45_SHIFT)) & LCD_WF8B_BPELCD45_MASK)
#define LCD_WF8B_BPELCD28_MASK                   (0x10U)
#define LCD_WF8B_BPELCD28_SHIFT                  (4U)
#define LCD_WF8B_BPELCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD28_SHIFT)) & LCD_WF8B_BPELCD28_MASK)
#define LCD_WF8B_BPELCD2_MASK                    (0x10U)
#define LCD_WF8B_BPELCD2_SHIFT                   (4U)
#define LCD_WF8B_BPELCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD2_SHIFT)) & LCD_WF8B_BPELCD2_MASK)
#define LCD_WF8B_BPELCD27_MASK                   (0x10U)
#define LCD_WF8B_BPELCD27_SHIFT                  (4U)
#define LCD_WF8B_BPELCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD27_SHIFT)) & LCD_WF8B_BPELCD27_MASK)
#define LCD_WF8B_BPELCD46_MASK                   (0x10U)
#define LCD_WF8B_BPELCD46_SHIFT                  (4U)
#define LCD_WF8B_BPELCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD46_SHIFT)) & LCD_WF8B_BPELCD46_MASK)
#define LCD_WF8B_BPELCD26_MASK                   (0x10U)
#define LCD_WF8B_BPELCD26_SHIFT                  (4U)
#define LCD_WF8B_BPELCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD26_SHIFT)) & LCD_WF8B_BPELCD26_MASK)
#define LCD_WF8B_BPELCD10_MASK                   (0x10U)
#define LCD_WF8B_BPELCD10_SHIFT                  (4U)
#define LCD_WF8B_BPELCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD10_SHIFT)) & LCD_WF8B_BPELCD10_MASK)
#define LCD_WF8B_BPELCD13_MASK                   (0x10U)
#define LCD_WF8B_BPELCD13_SHIFT                  (4U)
#define LCD_WF8B_BPELCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD13_SHIFT)) & LCD_WF8B_BPELCD13_MASK)
#define LCD_WF8B_BPELCD25_MASK                   (0x10U)
#define LCD_WF8B_BPELCD25_SHIFT                  (4U)
#define LCD_WF8B_BPELCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD25_SHIFT)) & LCD_WF8B_BPELCD25_MASK)
#define LCD_WF8B_BPELCD5_MASK                    (0x10U)
#define LCD_WF8B_BPELCD5_SHIFT                   (4U)
#define LCD_WF8B_BPELCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD5_SHIFT)) & LCD_WF8B_BPELCD5_MASK)
#define LCD_WF8B_BPELCD24_MASK                   (0x10U)
#define LCD_WF8B_BPELCD24_SHIFT                  (4U)
#define LCD_WF8B_BPELCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD24_SHIFT)) & LCD_WF8B_BPELCD24_MASK)
#define LCD_WF8B_BPELCD47_MASK                   (0x10U)
#define LCD_WF8B_BPELCD47_SHIFT                  (4U)
#define LCD_WF8B_BPELCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD47_SHIFT)) & LCD_WF8B_BPELCD47_MASK)
#define LCD_WF8B_BPELCD23_MASK                   (0x10U)
#define LCD_WF8B_BPELCD23_SHIFT                  (4U)
#define LCD_WF8B_BPELCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD23_SHIFT)) & LCD_WF8B_BPELCD23_MASK)
#define LCD_WF8B_BPELCD22_MASK                   (0x10U)
#define LCD_WF8B_BPELCD22_SHIFT                  (4U)
#define LCD_WF8B_BPELCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD22_SHIFT)) & LCD_WF8B_BPELCD22_MASK)
#define LCD_WF8B_BPELCD48_MASK                   (0x10U)
#define LCD_WF8B_BPELCD48_SHIFT                  (4U)
#define LCD_WF8B_BPELCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD48_SHIFT)) & LCD_WF8B_BPELCD48_MASK)
#define LCD_WF8B_BPELCD21_MASK                   (0x10U)
#define LCD_WF8B_BPELCD21_SHIFT                  (4U)
#define LCD_WF8B_BPELCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD21_SHIFT)) & LCD_WF8B_BPELCD21_MASK)
#define LCD_WF8B_BPELCD49_MASK                   (0x10U)
#define LCD_WF8B_BPELCD49_SHIFT                  (4U)
#define LCD_WF8B_BPELCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD49_SHIFT)) & LCD_WF8B_BPELCD49_MASK)
#define LCD_WF8B_BPELCD20_MASK                   (0x10U)
#define LCD_WF8B_BPELCD20_SHIFT                  (4U)
#define LCD_WF8B_BPELCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD20_SHIFT)) & LCD_WF8B_BPELCD20_MASK)
#define LCD_WF8B_BPELCD19_MASK                   (0x10U)
#define LCD_WF8B_BPELCD19_SHIFT                  (4U)
#define LCD_WF8B_BPELCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD19_SHIFT)) & LCD_WF8B_BPELCD19_MASK)
#define LCD_WF8B_BPELCD9_MASK                    (0x10U)
#define LCD_WF8B_BPELCD9_SHIFT                   (4U)
#define LCD_WF8B_BPELCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD9_SHIFT)) & LCD_WF8B_BPELCD9_MASK)
#define LCD_WF8B_BPELCD50_MASK                   (0x10U)
#define LCD_WF8B_BPELCD50_SHIFT                  (4U)
#define LCD_WF8B_BPELCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD50_SHIFT)) & LCD_WF8B_BPELCD50_MASK)
#define LCD_WF8B_BPELCD18_MASK                   (0x10U)
#define LCD_WF8B_BPELCD18_SHIFT                  (4U)
#define LCD_WF8B_BPELCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD18_SHIFT)) & LCD_WF8B_BPELCD18_MASK)
#define LCD_WF8B_BPELCD6_MASK                    (0x10U)
#define LCD_WF8B_BPELCD6_SHIFT                   (4U)
#define LCD_WF8B_BPELCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD6_SHIFT)) & LCD_WF8B_BPELCD6_MASK)
#define LCD_WF8B_BPELCD17_MASK                   (0x10U)
#define LCD_WF8B_BPELCD17_SHIFT                  (4U)
#define LCD_WF8B_BPELCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD17_SHIFT)) & LCD_WF8B_BPELCD17_MASK)
#define LCD_WF8B_BPELCD51_MASK                   (0x10U)
#define LCD_WF8B_BPELCD51_SHIFT                  (4U)
#define LCD_WF8B_BPELCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD51_SHIFT)) & LCD_WF8B_BPELCD51_MASK)
#define LCD_WF8B_BPELCD16_MASK                   (0x10U)
#define LCD_WF8B_BPELCD16_SHIFT                  (4U)
#define LCD_WF8B_BPELCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD16_SHIFT)) & LCD_WF8B_BPELCD16_MASK)
#define LCD_WF8B_BPELCD56_MASK                   (0x10U)
#define LCD_WF8B_BPELCD56_SHIFT                  (4U)
#define LCD_WF8B_BPELCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD56_SHIFT)) & LCD_WF8B_BPELCD56_MASK)
#define LCD_WF8B_BPELCD57_MASK                   (0x10U)
#define LCD_WF8B_BPELCD57_SHIFT                  (4U)
#define LCD_WF8B_BPELCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD57_SHIFT)) & LCD_WF8B_BPELCD57_MASK)
#define LCD_WF8B_BPELCD52_MASK                   (0x10U)
#define LCD_WF8B_BPELCD52_SHIFT                  (4U)
#define LCD_WF8B_BPELCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD52_SHIFT)) & LCD_WF8B_BPELCD52_MASK)
#define LCD_WF8B_BPELCD1_MASK                    (0x10U)
#define LCD_WF8B_BPELCD1_SHIFT                   (4U)
#define LCD_WF8B_BPELCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD1_SHIFT)) & LCD_WF8B_BPELCD1_MASK)
#define LCD_WF8B_BPELCD58_MASK                   (0x10U)
#define LCD_WF8B_BPELCD58_SHIFT                  (4U)
#define LCD_WF8B_BPELCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD58_SHIFT)) & LCD_WF8B_BPELCD58_MASK)
#define LCD_WF8B_BPELCD59_MASK                   (0x10U)
#define LCD_WF8B_BPELCD59_SHIFT                  (4U)
#define LCD_WF8B_BPELCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD59_SHIFT)) & LCD_WF8B_BPELCD59_MASK)
#define LCD_WF8B_BPELCD53_MASK                   (0x10U)
#define LCD_WF8B_BPELCD53_SHIFT                  (4U)
#define LCD_WF8B_BPELCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD53_SHIFT)) & LCD_WF8B_BPELCD53_MASK)
#define LCD_WF8B_BPELCD14_MASK                   (0x10U)
#define LCD_WF8B_BPELCD14_SHIFT                  (4U)
#define LCD_WF8B_BPELCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD14_SHIFT)) & LCD_WF8B_BPELCD14_MASK)
#define LCD_WF8B_BPELCD0_MASK                    (0x10U)
#define LCD_WF8B_BPELCD0_SHIFT                   (4U)
#define LCD_WF8B_BPELCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD0_SHIFT)) & LCD_WF8B_BPELCD0_MASK)
#define LCD_WF8B_BPELCD60_MASK                   (0x10U)
#define LCD_WF8B_BPELCD60_SHIFT                  (4U)
#define LCD_WF8B_BPELCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD60_SHIFT)) & LCD_WF8B_BPELCD60_MASK)
#define LCD_WF8B_BPELCD15_MASK                   (0x10U)
#define LCD_WF8B_BPELCD15_SHIFT                  (4U)
#define LCD_WF8B_BPELCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD15_SHIFT)) & LCD_WF8B_BPELCD15_MASK)
#define LCD_WF8B_BPELCD61_MASK                   (0x10U)
#define LCD_WF8B_BPELCD61_SHIFT                  (4U)
#define LCD_WF8B_BPELCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD61_SHIFT)) & LCD_WF8B_BPELCD61_MASK)
#define LCD_WF8B_BPELCD54_MASK                   (0x10U)
#define LCD_WF8B_BPELCD54_SHIFT                  (4U)
#define LCD_WF8B_BPELCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD54_SHIFT)) & LCD_WF8B_BPELCD54_MASK)
#define LCD_WF8B_BPELCD62_MASK                   (0x10U)
#define LCD_WF8B_BPELCD62_SHIFT                  (4U)
#define LCD_WF8B_BPELCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD62_SHIFT)) & LCD_WF8B_BPELCD62_MASK)
#define LCD_WF8B_BPELCD63_MASK                   (0x10U)
#define LCD_WF8B_BPELCD63_SHIFT                  (4U)
#define LCD_WF8B_BPELCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD63_SHIFT)) & LCD_WF8B_BPELCD63_MASK)
#define LCD_WF8B_BPELCD55_MASK                   (0x10U)
#define LCD_WF8B_BPELCD55_SHIFT                  (4U)
#define LCD_WF8B_BPELCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD55_SHIFT)) & LCD_WF8B_BPELCD55_MASK)
#define LCD_WF8B_BPELCD4_MASK                    (0x10U)
#define LCD_WF8B_BPELCD4_SHIFT                   (4U)
#define LCD_WF8B_BPELCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPELCD4_SHIFT)) & LCD_WF8B_BPELCD4_MASK)
#define LCD_WF8B_BPFLCD13_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD13_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD13_SHIFT)) & LCD_WF8B_BPFLCD13_MASK)
#define LCD_WF8B_BPFLCD39_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD39_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD39_SHIFT)) & LCD_WF8B_BPFLCD39_MASK)
#define LCD_WF8B_BPFLCD55_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD55_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD55_SHIFT)) & LCD_WF8B_BPFLCD55_MASK)
#define LCD_WF8B_BPFLCD47_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD47_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD47_SHIFT)) & LCD_WF8B_BPFLCD47_MASK)
#define LCD_WF8B_BPFLCD63_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD63_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD63_SHIFT)) & LCD_WF8B_BPFLCD63_MASK)
#define LCD_WF8B_BPFLCD43_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD43_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD43_SHIFT)) & LCD_WF8B_BPFLCD43_MASK)
#define LCD_WF8B_BPFLCD5_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD5_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD5_SHIFT)) & LCD_WF8B_BPFLCD5_MASK)
#define LCD_WF8B_BPFLCD62_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD62_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD62_SHIFT)) & LCD_WF8B_BPFLCD62_MASK)
#define LCD_WF8B_BPFLCD14_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD14_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD14_SHIFT)) & LCD_WF8B_BPFLCD14_MASK)
#define LCD_WF8B_BPFLCD24_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD24_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD24_SHIFT)) & LCD_WF8B_BPFLCD24_MASK)
#define LCD_WF8B_BPFLCD54_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD54_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD54_SHIFT)) & LCD_WF8B_BPFLCD54_MASK)
#define LCD_WF8B_BPFLCD15_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD15_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD15_SHIFT)) & LCD_WF8B_BPFLCD15_MASK)
#define LCD_WF8B_BPFLCD32_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD32_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD32_SHIFT)) & LCD_WF8B_BPFLCD32_MASK)
#define LCD_WF8B_BPFLCD61_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD61_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD61_SHIFT)) & LCD_WF8B_BPFLCD61_MASK)
#define LCD_WF8B_BPFLCD25_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD25_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD25_SHIFT)) & LCD_WF8B_BPFLCD25_MASK)
#define LCD_WF8B_BPFLCD60_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD60_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD60_SHIFT)) & LCD_WF8B_BPFLCD60_MASK)
#define LCD_WF8B_BPFLCD41_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD41_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD41_SHIFT)) & LCD_WF8B_BPFLCD41_MASK)
#define LCD_WF8B_BPFLCD33_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD33_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD33_SHIFT)) & LCD_WF8B_BPFLCD33_MASK)
#define LCD_WF8B_BPFLCD53_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD53_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD53_SHIFT)) & LCD_WF8B_BPFLCD53_MASK)
#define LCD_WF8B_BPFLCD59_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD59_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD59_SHIFT)) & LCD_WF8B_BPFLCD59_MASK)
#define LCD_WF8B_BPFLCD0_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD0_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD0_SHIFT)) & LCD_WF8B_BPFLCD0_MASK)
#define LCD_WF8B_BPFLCD46_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD46_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD46_SHIFT)) & LCD_WF8B_BPFLCD46_MASK)
#define LCD_WF8B_BPFLCD58_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD58_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD58_SHIFT)) & LCD_WF8B_BPFLCD58_MASK)
#define LCD_WF8B_BPFLCD26_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD26_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD26_SHIFT)) & LCD_WF8B_BPFLCD26_MASK)
#define LCD_WF8B_BPFLCD36_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD36_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD36_SHIFT)) & LCD_WF8B_BPFLCD36_MASK)
#define LCD_WF8B_BPFLCD10_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD10_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD10_SHIFT)) & LCD_WF8B_BPFLCD10_MASK)
#define LCD_WF8B_BPFLCD52_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD52_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD52_SHIFT)) & LCD_WF8B_BPFLCD52_MASK)
#define LCD_WF8B_BPFLCD57_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD57_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD57_SHIFT)) & LCD_WF8B_BPFLCD57_MASK)
#define LCD_WF8B_BPFLCD27_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD27_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD27_SHIFT)) & LCD_WF8B_BPFLCD27_MASK)
#define LCD_WF8B_BPFLCD11_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD11_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD11_SHIFT)) & LCD_WF8B_BPFLCD11_MASK)
#define LCD_WF8B_BPFLCD56_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD56_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD56_SHIFT)) & LCD_WF8B_BPFLCD56_MASK)
#define LCD_WF8B_BPFLCD1_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD1_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD1_SHIFT)) & LCD_WF8B_BPFLCD1_MASK)
#define LCD_WF8B_BPFLCD8_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD8_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD8_SHIFT)) & LCD_WF8B_BPFLCD8_MASK)
#define LCD_WF8B_BPFLCD40_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD40_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD40_SHIFT)) & LCD_WF8B_BPFLCD40_MASK)
#define LCD_WF8B_BPFLCD51_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD51_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD51_SHIFT)) & LCD_WF8B_BPFLCD51_MASK)
#define LCD_WF8B_BPFLCD16_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD16_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD16_SHIFT)) & LCD_WF8B_BPFLCD16_MASK)
#define LCD_WF8B_BPFLCD45_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD45_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD45_SHIFT)) & LCD_WF8B_BPFLCD45_MASK)
#define LCD_WF8B_BPFLCD6_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD6_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD6_SHIFT)) & LCD_WF8B_BPFLCD6_MASK)
#define LCD_WF8B_BPFLCD17_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD17_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD17_SHIFT)) & LCD_WF8B_BPFLCD17_MASK)
#define LCD_WF8B_BPFLCD28_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD28_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD28_SHIFT)) & LCD_WF8B_BPFLCD28_MASK)
#define LCD_WF8B_BPFLCD42_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD42_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD42_SHIFT)) & LCD_WF8B_BPFLCD42_MASK)
#define LCD_WF8B_BPFLCD29_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD29_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD29_SHIFT)) & LCD_WF8B_BPFLCD29_MASK)
#define LCD_WF8B_BPFLCD50_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD50_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD50_SHIFT)) & LCD_WF8B_BPFLCD50_MASK)
#define LCD_WF8B_BPFLCD18_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD18_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD18_SHIFT)) & LCD_WF8B_BPFLCD18_MASK)
#define LCD_WF8B_BPFLCD34_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD34_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD34_SHIFT)) & LCD_WF8B_BPFLCD34_MASK)
#define LCD_WF8B_BPFLCD19_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD19_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD19_SHIFT)) & LCD_WF8B_BPFLCD19_MASK)
#define LCD_WF8B_BPFLCD2_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD2_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD2_SHIFT)) & LCD_WF8B_BPFLCD2_MASK)
#define LCD_WF8B_BPFLCD9_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD9_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD9_SHIFT)) & LCD_WF8B_BPFLCD9_MASK)
#define LCD_WF8B_BPFLCD3_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD3_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD3_SHIFT)) & LCD_WF8B_BPFLCD3_MASK)
#define LCD_WF8B_BPFLCD37_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD37_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD37_SHIFT)) & LCD_WF8B_BPFLCD37_MASK)
#define LCD_WF8B_BPFLCD49_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD49_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD49_SHIFT)) & LCD_WF8B_BPFLCD49_MASK)
#define LCD_WF8B_BPFLCD20_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD20_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD20_SHIFT)) & LCD_WF8B_BPFLCD20_MASK)
#define LCD_WF8B_BPFLCD44_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD44_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD44_SHIFT)) & LCD_WF8B_BPFLCD44_MASK)
#define LCD_WF8B_BPFLCD30_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD30_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD30_SHIFT)) & LCD_WF8B_BPFLCD30_MASK)
#define LCD_WF8B_BPFLCD21_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD21_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD21_SHIFT)) & LCD_WF8B_BPFLCD21_MASK)
#define LCD_WF8B_BPFLCD35_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD35_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD35_SHIFT)) & LCD_WF8B_BPFLCD35_MASK)
#define LCD_WF8B_BPFLCD4_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD4_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD4_SHIFT)) & LCD_WF8B_BPFLCD4_MASK)
#define LCD_WF8B_BPFLCD31_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD31_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD31_SHIFT)) & LCD_WF8B_BPFLCD31_MASK)
#define LCD_WF8B_BPFLCD48_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD48_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD48_SHIFT)) & LCD_WF8B_BPFLCD48_MASK)
#define LCD_WF8B_BPFLCD7_MASK                    (0x20U)
#define LCD_WF8B_BPFLCD7_SHIFT                   (5U)
#define LCD_WF8B_BPFLCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD7_SHIFT)) & LCD_WF8B_BPFLCD7_MASK)
#define LCD_WF8B_BPFLCD22_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD22_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD22_SHIFT)) & LCD_WF8B_BPFLCD22_MASK)
#define LCD_WF8B_BPFLCD38_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD38_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD38_SHIFT)) & LCD_WF8B_BPFLCD38_MASK)
#define LCD_WF8B_BPFLCD12_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD12_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD12_SHIFT)) & LCD_WF8B_BPFLCD12_MASK)
#define LCD_WF8B_BPFLCD23_MASK                   (0x20U)
#define LCD_WF8B_BPFLCD23_SHIFT                  (5U)
#define LCD_WF8B_BPFLCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPFLCD23_SHIFT)) & LCD_WF8B_BPFLCD23_MASK)
#define LCD_WF8B_BPGLCD14_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD14_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD14_SHIFT)) & LCD_WF8B_BPGLCD14_MASK)
#define LCD_WF8B_BPGLCD55_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD55_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD55_SHIFT)) & LCD_WF8B_BPGLCD55_MASK)
#define LCD_WF8B_BPGLCD63_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD63_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD63_SHIFT)) & LCD_WF8B_BPGLCD63_MASK)
#define LCD_WF8B_BPGLCD15_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD15_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD15_SHIFT)) & LCD_WF8B_BPGLCD15_MASK)
#define LCD_WF8B_BPGLCD62_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD62_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD62_SHIFT)) & LCD_WF8B_BPGLCD62_MASK)
#define LCD_WF8B_BPGLCD54_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD54_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD54_SHIFT)) & LCD_WF8B_BPGLCD54_MASK)
#define LCD_WF8B_BPGLCD61_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD61_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD61_SHIFT)) & LCD_WF8B_BPGLCD61_MASK)
#define LCD_WF8B_BPGLCD60_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD60_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD60_SHIFT)) & LCD_WF8B_BPGLCD60_MASK)
#define LCD_WF8B_BPGLCD59_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD59_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD59_SHIFT)) & LCD_WF8B_BPGLCD59_MASK)
#define LCD_WF8B_BPGLCD53_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD53_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD53_SHIFT)) & LCD_WF8B_BPGLCD53_MASK)
#define LCD_WF8B_BPGLCD58_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD58_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD58_SHIFT)) & LCD_WF8B_BPGLCD58_MASK)
#define LCD_WF8B_BPGLCD0_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD0_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD0_SHIFT)) & LCD_WF8B_BPGLCD0_MASK)
#define LCD_WF8B_BPGLCD57_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD57_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD57_SHIFT)) & LCD_WF8B_BPGLCD57_MASK)
#define LCD_WF8B_BPGLCD52_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD52_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD52_SHIFT)) & LCD_WF8B_BPGLCD52_MASK)
#define LCD_WF8B_BPGLCD7_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD7_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD7_SHIFT)) & LCD_WF8B_BPGLCD7_MASK)
#define LCD_WF8B_BPGLCD56_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD56_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD56_SHIFT)) & LCD_WF8B_BPGLCD56_MASK)
#define LCD_WF8B_BPGLCD6_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD6_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD6_SHIFT)) & LCD_WF8B_BPGLCD6_MASK)
#define LCD_WF8B_BPGLCD51_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD51_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD51_SHIFT)) & LCD_WF8B_BPGLCD51_MASK)
#define LCD_WF8B_BPGLCD16_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD16_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD16_SHIFT)) & LCD_WF8B_BPGLCD16_MASK)
#define LCD_WF8B_BPGLCD1_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD1_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD1_SHIFT)) & LCD_WF8B_BPGLCD1_MASK)
#define LCD_WF8B_BPGLCD17_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD17_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD17_SHIFT)) & LCD_WF8B_BPGLCD17_MASK)
#define LCD_WF8B_BPGLCD50_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD50_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD50_SHIFT)) & LCD_WF8B_BPGLCD50_MASK)
#define LCD_WF8B_BPGLCD18_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD18_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD18_SHIFT)) & LCD_WF8B_BPGLCD18_MASK)
#define LCD_WF8B_BPGLCD19_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD19_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD19_SHIFT)) & LCD_WF8B_BPGLCD19_MASK)
#define LCD_WF8B_BPGLCD8_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD8_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD8_SHIFT)) & LCD_WF8B_BPGLCD8_MASK)
#define LCD_WF8B_BPGLCD49_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD49_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD49_SHIFT)) & LCD_WF8B_BPGLCD49_MASK)
#define LCD_WF8B_BPGLCD20_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD20_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD20_SHIFT)) & LCD_WF8B_BPGLCD20_MASK)
#define LCD_WF8B_BPGLCD9_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD9_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD9_SHIFT)) & LCD_WF8B_BPGLCD9_MASK)
#define LCD_WF8B_BPGLCD21_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD21_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD21_SHIFT)) & LCD_WF8B_BPGLCD21_MASK)
#define LCD_WF8B_BPGLCD13_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD13_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD13_SHIFT)) & LCD_WF8B_BPGLCD13_MASK)
#define LCD_WF8B_BPGLCD48_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD48_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD48_SHIFT)) & LCD_WF8B_BPGLCD48_MASK)
#define LCD_WF8B_BPGLCD22_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD22_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD22_SHIFT)) & LCD_WF8B_BPGLCD22_MASK)
#define LCD_WF8B_BPGLCD5_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD5_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD5_SHIFT)) & LCD_WF8B_BPGLCD5_MASK)
#define LCD_WF8B_BPGLCD47_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD47_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD47_SHIFT)) & LCD_WF8B_BPGLCD47_MASK)
#define LCD_WF8B_BPGLCD23_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD23_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD23_SHIFT)) & LCD_WF8B_BPGLCD23_MASK)
#define LCD_WF8B_BPGLCD24_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD24_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD24_SHIFT)) & LCD_WF8B_BPGLCD24_MASK)
#define LCD_WF8B_BPGLCD25_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD25_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD25_SHIFT)) & LCD_WF8B_BPGLCD25_MASK)
#define LCD_WF8B_BPGLCD46_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD46_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD46_SHIFT)) & LCD_WF8B_BPGLCD46_MASK)
#define LCD_WF8B_BPGLCD26_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD26_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD26_SHIFT)) & LCD_WF8B_BPGLCD26_MASK)
#define LCD_WF8B_BPGLCD27_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD27_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD27_SHIFT)) & LCD_WF8B_BPGLCD27_MASK)
#define LCD_WF8B_BPGLCD10_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD10_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD10_SHIFT)) & LCD_WF8B_BPGLCD10_MASK)
#define LCD_WF8B_BPGLCD45_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD45_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD45_SHIFT)) & LCD_WF8B_BPGLCD45_MASK)
#define LCD_WF8B_BPGLCD28_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD28_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD28_SHIFT)) & LCD_WF8B_BPGLCD28_MASK)
#define LCD_WF8B_BPGLCD29_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD29_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD29_SHIFT)) & LCD_WF8B_BPGLCD29_MASK)
#define LCD_WF8B_BPGLCD4_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD4_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD4_SHIFT)) & LCD_WF8B_BPGLCD4_MASK)
#define LCD_WF8B_BPGLCD44_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD44_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD44_SHIFT)) & LCD_WF8B_BPGLCD44_MASK)
#define LCD_WF8B_BPGLCD30_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD30_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD30_SHIFT)) & LCD_WF8B_BPGLCD30_MASK)
#define LCD_WF8B_BPGLCD2_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD2_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD2_SHIFT)) & LCD_WF8B_BPGLCD2_MASK)
#define LCD_WF8B_BPGLCD31_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD31_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD31_SHIFT)) & LCD_WF8B_BPGLCD31_MASK)
#define LCD_WF8B_BPGLCD43_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD43_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD43_SHIFT)) & LCD_WF8B_BPGLCD43_MASK)
#define LCD_WF8B_BPGLCD32_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD32_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD32_SHIFT)) & LCD_WF8B_BPGLCD32_MASK)
#define LCD_WF8B_BPGLCD33_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD33_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD33_SHIFT)) & LCD_WF8B_BPGLCD33_MASK)
#define LCD_WF8B_BPGLCD42_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD42_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD42_SHIFT)) & LCD_WF8B_BPGLCD42_MASK)
#define LCD_WF8B_BPGLCD34_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD34_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD34_SHIFT)) & LCD_WF8B_BPGLCD34_MASK)
#define LCD_WF8B_BPGLCD11_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD11_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD11_SHIFT)) & LCD_WF8B_BPGLCD11_MASK)
#define LCD_WF8B_BPGLCD35_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD35_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD35_SHIFT)) & LCD_WF8B_BPGLCD35_MASK)
#define LCD_WF8B_BPGLCD12_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD12_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD12_SHIFT)) & LCD_WF8B_BPGLCD12_MASK)
#define LCD_WF8B_BPGLCD41_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD41_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD41_SHIFT)) & LCD_WF8B_BPGLCD41_MASK)
#define LCD_WF8B_BPGLCD36_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD36_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD36_SHIFT)) & LCD_WF8B_BPGLCD36_MASK)
#define LCD_WF8B_BPGLCD3_MASK                    (0x40U)
#define LCD_WF8B_BPGLCD3_SHIFT                   (6U)
#define LCD_WF8B_BPGLCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD3_SHIFT)) & LCD_WF8B_BPGLCD3_MASK)
#define LCD_WF8B_BPGLCD37_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD37_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD37_SHIFT)) & LCD_WF8B_BPGLCD37_MASK)
#define LCD_WF8B_BPGLCD40_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD40_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD40_SHIFT)) & LCD_WF8B_BPGLCD40_MASK)
#define LCD_WF8B_BPGLCD38_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD38_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD38_SHIFT)) & LCD_WF8B_BPGLCD38_MASK)
#define LCD_WF8B_BPGLCD39_MASK                   (0x40U)
#define LCD_WF8B_BPGLCD39_SHIFT                  (6U)
#define LCD_WF8B_BPGLCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPGLCD39_SHIFT)) & LCD_WF8B_BPGLCD39_MASK)
#define LCD_WF8B_BPHLCD63_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD63_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD63(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD63_SHIFT)) & LCD_WF8B_BPHLCD63_MASK)
#define LCD_WF8B_BPHLCD62_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD62_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD62(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD62_SHIFT)) & LCD_WF8B_BPHLCD62_MASK)
#define LCD_WF8B_BPHLCD61_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD61_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD61(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD61_SHIFT)) & LCD_WF8B_BPHLCD61_MASK)
#define LCD_WF8B_BPHLCD60_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD60_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD60(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD60_SHIFT)) & LCD_WF8B_BPHLCD60_MASK)
#define LCD_WF8B_BPHLCD59_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD59_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD59(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD59_SHIFT)) & LCD_WF8B_BPHLCD59_MASK)
#define LCD_WF8B_BPHLCD58_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD58_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD58(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD58_SHIFT)) & LCD_WF8B_BPHLCD58_MASK)
#define LCD_WF8B_BPHLCD57_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD57_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD57(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD57_SHIFT)) & LCD_WF8B_BPHLCD57_MASK)
#define LCD_WF8B_BPHLCD0_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD0_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD0(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD0_SHIFT)) & LCD_WF8B_BPHLCD0_MASK)
#define LCD_WF8B_BPHLCD56_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD56_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD56(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD56_SHIFT)) & LCD_WF8B_BPHLCD56_MASK)
#define LCD_WF8B_BPHLCD55_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD55_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD55(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD55_SHIFT)) & LCD_WF8B_BPHLCD55_MASK)
#define LCD_WF8B_BPHLCD54_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD54_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD54(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD54_SHIFT)) & LCD_WF8B_BPHLCD54_MASK)
#define LCD_WF8B_BPHLCD53_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD53_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD53(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD53_SHIFT)) & LCD_WF8B_BPHLCD53_MASK)
#define LCD_WF8B_BPHLCD52_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD52_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD52(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD52_SHIFT)) & LCD_WF8B_BPHLCD52_MASK)
#define LCD_WF8B_BPHLCD51_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD51_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD51(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD51_SHIFT)) & LCD_WF8B_BPHLCD51_MASK)
#define LCD_WF8B_BPHLCD50_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD50_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD50(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD50_SHIFT)) & LCD_WF8B_BPHLCD50_MASK)
#define LCD_WF8B_BPHLCD1_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD1_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD1(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD1_SHIFT)) & LCD_WF8B_BPHLCD1_MASK)
#define LCD_WF8B_BPHLCD49_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD49_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD49(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD49_SHIFT)) & LCD_WF8B_BPHLCD49_MASK)
#define LCD_WF8B_BPHLCD48_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD48_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD48(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD48_SHIFT)) & LCD_WF8B_BPHLCD48_MASK)
#define LCD_WF8B_BPHLCD47_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD47_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD47(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD47_SHIFT)) & LCD_WF8B_BPHLCD47_MASK)
#define LCD_WF8B_BPHLCD46_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD46_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD46(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD46_SHIFT)) & LCD_WF8B_BPHLCD46_MASK)
#define LCD_WF8B_BPHLCD45_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD45_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD45(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD45_SHIFT)) & LCD_WF8B_BPHLCD45_MASK)
#define LCD_WF8B_BPHLCD44_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD44_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD44(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD44_SHIFT)) & LCD_WF8B_BPHLCD44_MASK)
#define LCD_WF8B_BPHLCD43_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD43_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD43(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD43_SHIFT)) & LCD_WF8B_BPHLCD43_MASK)
#define LCD_WF8B_BPHLCD2_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD2_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD2(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD2_SHIFT)) & LCD_WF8B_BPHLCD2_MASK)
#define LCD_WF8B_BPHLCD42_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD42_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD42(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD42_SHIFT)) & LCD_WF8B_BPHLCD42_MASK)
#define LCD_WF8B_BPHLCD41_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD41_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD41(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD41_SHIFT)) & LCD_WF8B_BPHLCD41_MASK)
#define LCD_WF8B_BPHLCD40_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD40_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD40(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD40_SHIFT)) & LCD_WF8B_BPHLCD40_MASK)
#define LCD_WF8B_BPHLCD39_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD39_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD39(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD39_SHIFT)) & LCD_WF8B_BPHLCD39_MASK)
#define LCD_WF8B_BPHLCD38_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD38_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD38(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD38_SHIFT)) & LCD_WF8B_BPHLCD38_MASK)
#define LCD_WF8B_BPHLCD37_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD37_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD37(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD37_SHIFT)) & LCD_WF8B_BPHLCD37_MASK)
#define LCD_WF8B_BPHLCD36_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD36_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD36(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD36_SHIFT)) & LCD_WF8B_BPHLCD36_MASK)
#define LCD_WF8B_BPHLCD3_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD3_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD3(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD3_SHIFT)) & LCD_WF8B_BPHLCD3_MASK)
#define LCD_WF8B_BPHLCD35_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD35_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD35(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD35_SHIFT)) & LCD_WF8B_BPHLCD35_MASK)
#define LCD_WF8B_BPHLCD34_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD34_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD34(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD34_SHIFT)) & LCD_WF8B_BPHLCD34_MASK)
#define LCD_WF8B_BPHLCD33_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD33_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD33(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD33_SHIFT)) & LCD_WF8B_BPHLCD33_MASK)
#define LCD_WF8B_BPHLCD32_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD32_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD32(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD32_SHIFT)) & LCD_WF8B_BPHLCD32_MASK)
#define LCD_WF8B_BPHLCD31_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD31_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD31(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD31_SHIFT)) & LCD_WF8B_BPHLCD31_MASK)
#define LCD_WF8B_BPHLCD30_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD30_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD30(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD30_SHIFT)) & LCD_WF8B_BPHLCD30_MASK)
#define LCD_WF8B_BPHLCD29_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD29_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD29(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD29_SHIFT)) & LCD_WF8B_BPHLCD29_MASK)
#define LCD_WF8B_BPHLCD4_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD4_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD4(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD4_SHIFT)) & LCD_WF8B_BPHLCD4_MASK)
#define LCD_WF8B_BPHLCD28_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD28_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD28(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD28_SHIFT)) & LCD_WF8B_BPHLCD28_MASK)
#define LCD_WF8B_BPHLCD27_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD27_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD27(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD27_SHIFT)) & LCD_WF8B_BPHLCD27_MASK)
#define LCD_WF8B_BPHLCD26_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD26_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD26(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD26_SHIFT)) & LCD_WF8B_BPHLCD26_MASK)
#define LCD_WF8B_BPHLCD25_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD25_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD25(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD25_SHIFT)) & LCD_WF8B_BPHLCD25_MASK)
#define LCD_WF8B_BPHLCD24_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD24_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD24(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD24_SHIFT)) & LCD_WF8B_BPHLCD24_MASK)
#define LCD_WF8B_BPHLCD23_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD23_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD23(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD23_SHIFT)) & LCD_WF8B_BPHLCD23_MASK)
#define LCD_WF8B_BPHLCD22_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD22_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD22(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD22_SHIFT)) & LCD_WF8B_BPHLCD22_MASK)
#define LCD_WF8B_BPHLCD5_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD5_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD5(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD5_SHIFT)) & LCD_WF8B_BPHLCD5_MASK)
#define LCD_WF8B_BPHLCD21_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD21_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD21(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD21_SHIFT)) & LCD_WF8B_BPHLCD21_MASK)
#define LCD_WF8B_BPHLCD20_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD20_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD20(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD20_SHIFT)) & LCD_WF8B_BPHLCD20_MASK)
#define LCD_WF8B_BPHLCD19_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD19_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD19(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD19_SHIFT)) & LCD_WF8B_BPHLCD19_MASK)
#define LCD_WF8B_BPHLCD18_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD18_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD18(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD18_SHIFT)) & LCD_WF8B_BPHLCD18_MASK)
#define LCD_WF8B_BPHLCD17_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD17_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD17(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD17_SHIFT)) & LCD_WF8B_BPHLCD17_MASK)
#define LCD_WF8B_BPHLCD16_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD16_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD16(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD16_SHIFT)) & LCD_WF8B_BPHLCD16_MASK)
#define LCD_WF8B_BPHLCD15_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD15_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD15(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD15_SHIFT)) & LCD_WF8B_BPHLCD15_MASK)
#define LCD_WF8B_BPHLCD6_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD6_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD6(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD6_SHIFT)) & LCD_WF8B_BPHLCD6_MASK)
#define LCD_WF8B_BPHLCD14_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD14_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD14(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD14_SHIFT)) & LCD_WF8B_BPHLCD14_MASK)
#define LCD_WF8B_BPHLCD13_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD13_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD13(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD13_SHIFT)) & LCD_WF8B_BPHLCD13_MASK)
#define LCD_WF8B_BPHLCD12_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD12_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD12(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD12_SHIFT)) & LCD_WF8B_BPHLCD12_MASK)
#define LCD_WF8B_BPHLCD11_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD11_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD11(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD11_SHIFT)) & LCD_WF8B_BPHLCD11_MASK)
#define LCD_WF8B_BPHLCD10_MASK                   (0x80U)
#define LCD_WF8B_BPHLCD10_SHIFT                  (7U)
#define LCD_WF8B_BPHLCD10(x)                     (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD10_SHIFT)) & LCD_WF8B_BPHLCD10_MASK)
#define LCD_WF8B_BPHLCD9_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD9_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD9(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD9_SHIFT)) & LCD_WF8B_BPHLCD9_MASK)
#define LCD_WF8B_BPHLCD8_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD8_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD8(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD8_SHIFT)) & LCD_WF8B_BPHLCD8_MASK)
#define LCD_WF8B_BPHLCD7_MASK                    (0x80U)
#define LCD_WF8B_BPHLCD7_SHIFT                   (7U)
#define LCD_WF8B_BPHLCD7(x)                      (((uint8_t)(((uint8_t)(x)) << LCD_WF8B_BPHLCD7_SHIFT)) & LCD_WF8B_BPHLCD7_MASK)

/* The count of LCD_WF8B */
#define LCD_WF8B_COUNT                           (64U)


/*!
 * @}
 */ /* end of group LCD_Register_Masks */


/* LCD - Peripheral instance base addresses */
/** Peripheral LCD base address */
#define LCD_BASE                                 (0x40043000u)
/** Peripheral LCD base pointer */
#define LCD                                      ((LCD_Type *)LCD_BASE)
/** Array initializer of LCD peripheral base addresses */
#define LCD_BASE_ADDRS                           { LCD_BASE }
/** Array initializer of LCD peripheral base pointers */
#define LCD_BASE_PTRS                            { LCD }
/** Interrupt vectors for the LCD peripheral type */
#define LCD_LCD_IRQS                             { LCD_IRQn }

/*!
 * @}
 */ /* end of group LCD_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- LLWU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LLWU_Peripheral_Access_Layer LLWU Peripheral Access Layer
 * @{
 */

/** LLWU - Register Layout Typedef */
typedef struct {
  __IO uint8_t PE1;                                /**< LLWU Pin Enable 1 register, offset: 0x0 */
  __IO uint8_t PE2;                                /**< LLWU Pin Enable 2 register, offset: 0x1 */
  __IO uint8_t PE3;                                /**< LLWU Pin Enable 3 register, offset: 0x2 */
  __IO uint8_t PE4;                                /**< LLWU Pin Enable 4 register, offset: 0x3 */
  __IO uint8_t PE5;                                /**< LLWU Pin Enable 5 register, offset: 0x4 */
  __IO uint8_t PE6;                                /**< LLWU Pin Enable 6 register, offset: 0x5 */
       uint8_t RESERVED_0[2];
  __IO uint8_t ME;                                 /**< LLWU Module Enable register, offset: 0x8 */
  __IO uint8_t PF1;                                /**< LLWU Pin Flag 1 register, offset: 0x9 */
  __IO uint8_t PF2;                                /**< LLWU Pin Flag 2 register, offset: 0xA */
  __IO uint8_t PF3;                                /**< LLWU Pin Flag 3 register, offset: 0xB */
       uint8_t RESERVED_1[1];
  __I  uint8_t MF5;                                /**< LLWU Module Flag 5 register, offset: 0xD */
  __IO uint8_t FILT1;                              /**< LLWU Pin Filter 1 register, offset: 0xE */
  __IO uint8_t FILT2;                              /**< LLWU Pin Filter 2 register, offset: 0xF */
} LLWU_Type;

/* ----------------------------------------------------------------------------
   -- LLWU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LLWU_Register_Masks LLWU Register Masks
 * @{
 */

/*! @name PE1 - LLWU Pin Enable 1 register */
#define LLWU_PE1_WUPE0_MASK                      (0x3U)
#define LLWU_PE1_WUPE0_SHIFT                     (0U)
#define LLWU_PE1_WUPE0(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE1_WUPE0_SHIFT)) & LLWU_PE1_WUPE0_MASK)
#define LLWU_PE1_WUPE1_MASK                      (0xCU)
#define LLWU_PE1_WUPE1_SHIFT                     (2U)
#define LLWU_PE1_WUPE1(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE1_WUPE1_SHIFT)) & LLWU_PE1_WUPE1_MASK)
#define LLWU_PE1_WUPE2_MASK                      (0x30U)
#define LLWU_PE1_WUPE2_SHIFT                     (4U)
#define LLWU_PE1_WUPE2(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE1_WUPE2_SHIFT)) & LLWU_PE1_WUPE2_MASK)
#define LLWU_PE1_WUPE3_MASK                      (0xC0U)
#define LLWU_PE1_WUPE3_SHIFT                     (6U)
#define LLWU_PE1_WUPE3(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE1_WUPE3_SHIFT)) & LLWU_PE1_WUPE3_MASK)

/*! @name PE2 - LLWU Pin Enable 2 register */
#define LLWU_PE2_WUPE4_MASK                      (0x3U)
#define LLWU_PE2_WUPE4_SHIFT                     (0U)
#define LLWU_PE2_WUPE4(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE2_WUPE4_SHIFT)) & LLWU_PE2_WUPE4_MASK)
#define LLWU_PE2_WUPE5_MASK                      (0xCU)
#define LLWU_PE2_WUPE5_SHIFT                     (2U)
#define LLWU_PE2_WUPE5(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE2_WUPE5_SHIFT)) & LLWU_PE2_WUPE5_MASK)
#define LLWU_PE2_WUPE6_MASK                      (0x30U)
#define LLWU_PE2_WUPE6_SHIFT                     (4U)
#define LLWU_PE2_WUPE6(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE2_WUPE6_SHIFT)) & LLWU_PE2_WUPE6_MASK)
#define LLWU_PE2_WUPE7_MASK                      (0xC0U)
#define LLWU_PE2_WUPE7_SHIFT                     (6U)
#define LLWU_PE2_WUPE7(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE2_WUPE7_SHIFT)) & LLWU_PE2_WUPE7_MASK)

/*! @name PE3 - LLWU Pin Enable 3 register */
#define LLWU_PE3_WUPE8_MASK                      (0x3U)
#define LLWU_PE3_WUPE8_SHIFT                     (0U)
#define LLWU_PE3_WUPE8(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE3_WUPE8_SHIFT)) & LLWU_PE3_WUPE8_MASK)
#define LLWU_PE3_WUPE9_MASK                      (0xCU)
#define LLWU_PE3_WUPE9_SHIFT                     (2U)
#define LLWU_PE3_WUPE9(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PE3_WUPE9_SHIFT)) & LLWU_PE3_WUPE9_MASK)
#define LLWU_PE3_WUPE10_MASK                     (0x30U)
#define LLWU_PE3_WUPE10_SHIFT                    (4U)
#define LLWU_PE3_WUPE10(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE3_WUPE10_SHIFT)) & LLWU_PE3_WUPE10_MASK)
#define LLWU_PE3_WUPE11_MASK                     (0xC0U)
#define LLWU_PE3_WUPE11_SHIFT                    (6U)
#define LLWU_PE3_WUPE11(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE3_WUPE11_SHIFT)) & LLWU_PE3_WUPE11_MASK)

/*! @name PE4 - LLWU Pin Enable 4 register */
#define LLWU_PE4_WUPE12_MASK                     (0x3U)
#define LLWU_PE4_WUPE12_SHIFT                    (0U)
#define LLWU_PE4_WUPE12(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE4_WUPE12_SHIFT)) & LLWU_PE4_WUPE12_MASK)
#define LLWU_PE4_WUPE13_MASK                     (0xCU)
#define LLWU_PE4_WUPE13_SHIFT                    (2U)
#define LLWU_PE4_WUPE13(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE4_WUPE13_SHIFT)) & LLWU_PE4_WUPE13_MASK)
#define LLWU_PE4_WUPE14_MASK                     (0x30U)
#define LLWU_PE4_WUPE14_SHIFT                    (4U)
#define LLWU_PE4_WUPE14(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE4_WUPE14_SHIFT)) & LLWU_PE4_WUPE14_MASK)
#define LLWU_PE4_WUPE15_MASK                     (0xC0U)
#define LLWU_PE4_WUPE15_SHIFT                    (6U)
#define LLWU_PE4_WUPE15(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE4_WUPE15_SHIFT)) & LLWU_PE4_WUPE15_MASK)

/*! @name PE5 - LLWU Pin Enable 5 register */
#define LLWU_PE5_WUPE16_MASK                     (0x3U)
#define LLWU_PE5_WUPE16_SHIFT                    (0U)
#define LLWU_PE5_WUPE16(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE5_WUPE16_SHIFT)) & LLWU_PE5_WUPE16_MASK)
#define LLWU_PE5_WUPE17_MASK                     (0xCU)
#define LLWU_PE5_WUPE17_SHIFT                    (2U)
#define LLWU_PE5_WUPE17(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE5_WUPE17_SHIFT)) & LLWU_PE5_WUPE17_MASK)
#define LLWU_PE5_WUPE18_MASK                     (0x30U)
#define LLWU_PE5_WUPE18_SHIFT                    (4U)
#define LLWU_PE5_WUPE18(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE5_WUPE18_SHIFT)) & LLWU_PE5_WUPE18_MASK)
#define LLWU_PE5_WUPE19_MASK                     (0xC0U)
#define LLWU_PE5_WUPE19_SHIFT                    (6U)
#define LLWU_PE5_WUPE19(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE5_WUPE19_SHIFT)) & LLWU_PE5_WUPE19_MASK)

/*! @name PE6 - LLWU Pin Enable 6 register */
#define LLWU_PE6_WUPE20_MASK                     (0x3U)
#define LLWU_PE6_WUPE20_SHIFT                    (0U)
#define LLWU_PE6_WUPE20(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE6_WUPE20_SHIFT)) & LLWU_PE6_WUPE20_MASK)
#define LLWU_PE6_WUPE21_MASK                     (0xCU)
#define LLWU_PE6_WUPE21_SHIFT                    (2U)
#define LLWU_PE6_WUPE21(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE6_WUPE21_SHIFT)) & LLWU_PE6_WUPE21_MASK)
#define LLWU_PE6_WUPE22_MASK                     (0x30U)
#define LLWU_PE6_WUPE22_SHIFT                    (4U)
#define LLWU_PE6_WUPE22(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE6_WUPE22_SHIFT)) & LLWU_PE6_WUPE22_MASK)
#define LLWU_PE6_WUPE23_MASK                     (0xC0U)
#define LLWU_PE6_WUPE23_SHIFT                    (6U)
#define LLWU_PE6_WUPE23(x)                       (((uint8_t)(((uint8_t)(x)) << LLWU_PE6_WUPE23_SHIFT)) & LLWU_PE6_WUPE23_MASK)

/*! @name ME - LLWU Module Enable register */
#define LLWU_ME_WUME0_MASK                       (0x1U)
#define LLWU_ME_WUME0_SHIFT                      (0U)
#define LLWU_ME_WUME0(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME0_SHIFT)) & LLWU_ME_WUME0_MASK)
#define LLWU_ME_WUME1_MASK                       (0x2U)
#define LLWU_ME_WUME1_SHIFT                      (1U)
#define LLWU_ME_WUME1(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME1_SHIFT)) & LLWU_ME_WUME1_MASK)
#define LLWU_ME_WUME2_MASK                       (0x4U)
#define LLWU_ME_WUME2_SHIFT                      (2U)
#define LLWU_ME_WUME2(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME2_SHIFT)) & LLWU_ME_WUME2_MASK)
#define LLWU_ME_WUME3_MASK                       (0x8U)
#define LLWU_ME_WUME3_SHIFT                      (3U)
#define LLWU_ME_WUME3(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME3_SHIFT)) & LLWU_ME_WUME3_MASK)
#define LLWU_ME_WUME4_MASK                       (0x10U)
#define LLWU_ME_WUME4_SHIFT                      (4U)
#define LLWU_ME_WUME4(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME4_SHIFT)) & LLWU_ME_WUME4_MASK)
#define LLWU_ME_WUME5_MASK                       (0x20U)
#define LLWU_ME_WUME5_SHIFT                      (5U)
#define LLWU_ME_WUME5(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME5_SHIFT)) & LLWU_ME_WUME5_MASK)
#define LLWU_ME_WUME6_MASK                       (0x40U)
#define LLWU_ME_WUME6_SHIFT                      (6U)
#define LLWU_ME_WUME6(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME6_SHIFT)) & LLWU_ME_WUME6_MASK)
#define LLWU_ME_WUME7_MASK                       (0x80U)
#define LLWU_ME_WUME7_SHIFT                      (7U)
#define LLWU_ME_WUME7(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_ME_WUME7_SHIFT)) & LLWU_ME_WUME7_MASK)

/*! @name PF1 - LLWU Pin Flag 1 register */
#define LLWU_PF1_WUF0_MASK                       (0x1U)
#define LLWU_PF1_WUF0_SHIFT                      (0U)
#define LLWU_PF1_WUF0(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF0_SHIFT)) & LLWU_PF1_WUF0_MASK)
#define LLWU_PF1_WUF1_MASK                       (0x2U)
#define LLWU_PF1_WUF1_SHIFT                      (1U)
#define LLWU_PF1_WUF1(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF1_SHIFT)) & LLWU_PF1_WUF1_MASK)
#define LLWU_PF1_WUF2_MASK                       (0x4U)
#define LLWU_PF1_WUF2_SHIFT                      (2U)
#define LLWU_PF1_WUF2(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF2_SHIFT)) & LLWU_PF1_WUF2_MASK)
#define LLWU_PF1_WUF3_MASK                       (0x8U)
#define LLWU_PF1_WUF3_SHIFT                      (3U)
#define LLWU_PF1_WUF3(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF3_SHIFT)) & LLWU_PF1_WUF3_MASK)
#define LLWU_PF1_WUF4_MASK                       (0x10U)
#define LLWU_PF1_WUF4_SHIFT                      (4U)
#define LLWU_PF1_WUF4(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF4_SHIFT)) & LLWU_PF1_WUF4_MASK)
#define LLWU_PF1_WUF5_MASK                       (0x20U)
#define LLWU_PF1_WUF5_SHIFT                      (5U)
#define LLWU_PF1_WUF5(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF5_SHIFT)) & LLWU_PF1_WUF5_MASK)
#define LLWU_PF1_WUF6_MASK                       (0x40U)
#define LLWU_PF1_WUF6_SHIFT                      (6U)
#define LLWU_PF1_WUF6(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF6_SHIFT)) & LLWU_PF1_WUF6_MASK)
#define LLWU_PF1_WUF7_MASK                       (0x80U)
#define LLWU_PF1_WUF7_SHIFT                      (7U)
#define LLWU_PF1_WUF7(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF1_WUF7_SHIFT)) & LLWU_PF1_WUF7_MASK)

/*! @name PF2 - LLWU Pin Flag 2 register */
#define LLWU_PF2_WUF8_MASK                       (0x1U)
#define LLWU_PF2_WUF8_SHIFT                      (0U)
#define LLWU_PF2_WUF8(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF8_SHIFT)) & LLWU_PF2_WUF8_MASK)
#define LLWU_PF2_WUF9_MASK                       (0x2U)
#define LLWU_PF2_WUF9_SHIFT                      (1U)
#define LLWU_PF2_WUF9(x)                         (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF9_SHIFT)) & LLWU_PF2_WUF9_MASK)
#define LLWU_PF2_WUF10_MASK                      (0x4U)
#define LLWU_PF2_WUF10_SHIFT                     (2U)
#define LLWU_PF2_WUF10(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF10_SHIFT)) & LLWU_PF2_WUF10_MASK)
#define LLWU_PF2_WUF11_MASK                      (0x8U)
#define LLWU_PF2_WUF11_SHIFT                     (3U)
#define LLWU_PF2_WUF11(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF11_SHIFT)) & LLWU_PF2_WUF11_MASK)
#define LLWU_PF2_WUF12_MASK                      (0x10U)
#define LLWU_PF2_WUF12_SHIFT                     (4U)
#define LLWU_PF2_WUF12(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF12_SHIFT)) & LLWU_PF2_WUF12_MASK)
#define LLWU_PF2_WUF13_MASK                      (0x20U)
#define LLWU_PF2_WUF13_SHIFT                     (5U)
#define LLWU_PF2_WUF13(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF13_SHIFT)) & LLWU_PF2_WUF13_MASK)
#define LLWU_PF2_WUF14_MASK                      (0x40U)
#define LLWU_PF2_WUF14_SHIFT                     (6U)
#define LLWU_PF2_WUF14(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF14_SHIFT)) & LLWU_PF2_WUF14_MASK)
#define LLWU_PF2_WUF15_MASK                      (0x80U)
#define LLWU_PF2_WUF15_SHIFT                     (7U)
#define LLWU_PF2_WUF15(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF2_WUF15_SHIFT)) & LLWU_PF2_WUF15_MASK)

/*! @name PF3 - LLWU Pin Flag 3 register */
#define LLWU_PF3_WUF16_MASK                      (0x1U)
#define LLWU_PF3_WUF16_SHIFT                     (0U)
#define LLWU_PF3_WUF16(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF16_SHIFT)) & LLWU_PF3_WUF16_MASK)
#define LLWU_PF3_WUF17_MASK                      (0x2U)
#define LLWU_PF3_WUF17_SHIFT                     (1U)
#define LLWU_PF3_WUF17(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF17_SHIFT)) & LLWU_PF3_WUF17_MASK)
#define LLWU_PF3_WUF18_MASK                      (0x4U)
#define LLWU_PF3_WUF18_SHIFT                     (2U)
#define LLWU_PF3_WUF18(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF18_SHIFT)) & LLWU_PF3_WUF18_MASK)
#define LLWU_PF3_WUF19_MASK                      (0x8U)
#define LLWU_PF3_WUF19_SHIFT                     (3U)
#define LLWU_PF3_WUF19(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF19_SHIFT)) & LLWU_PF3_WUF19_MASK)
#define LLWU_PF3_WUF20_MASK                      (0x10U)
#define LLWU_PF3_WUF20_SHIFT                     (4U)
#define LLWU_PF3_WUF20(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF20_SHIFT)) & LLWU_PF3_WUF20_MASK)
#define LLWU_PF3_WUF21_MASK                      (0x20U)
#define LLWU_PF3_WUF21_SHIFT                     (5U)
#define LLWU_PF3_WUF21(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF21_SHIFT)) & LLWU_PF3_WUF21_MASK)
#define LLWU_PF3_WUF22_MASK                      (0x40U)
#define LLWU_PF3_WUF22_SHIFT                     (6U)
#define LLWU_PF3_WUF22(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF22_SHIFT)) & LLWU_PF3_WUF22_MASK)
#define LLWU_PF3_WUF23_MASK                      (0x80U)
#define LLWU_PF3_WUF23_SHIFT                     (7U)
#define LLWU_PF3_WUF23(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_PF3_WUF23_SHIFT)) & LLWU_PF3_WUF23_MASK)

/*! @name MF5 - LLWU Module Flag 5 register */
#define LLWU_MF5_MWUF0_MASK                      (0x1U)
#define LLWU_MF5_MWUF0_SHIFT                     (0U)
#define LLWU_MF5_MWUF0(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF0_SHIFT)) & LLWU_MF5_MWUF0_MASK)
#define LLWU_MF5_MWUF1_MASK                      (0x2U)
#define LLWU_MF5_MWUF1_SHIFT                     (1U)
#define LLWU_MF5_MWUF1(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF1_SHIFT)) & LLWU_MF5_MWUF1_MASK)
#define LLWU_MF5_MWUF2_MASK                      (0x4U)
#define LLWU_MF5_MWUF2_SHIFT                     (2U)
#define LLWU_MF5_MWUF2(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF2_SHIFT)) & LLWU_MF5_MWUF2_MASK)
#define LLWU_MF5_MWUF3_MASK                      (0x8U)
#define LLWU_MF5_MWUF3_SHIFT                     (3U)
#define LLWU_MF5_MWUF3(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF3_SHIFT)) & LLWU_MF5_MWUF3_MASK)
#define LLWU_MF5_MWUF4_MASK                      (0x10U)
#define LLWU_MF5_MWUF4_SHIFT                     (4U)
#define LLWU_MF5_MWUF4(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF4_SHIFT)) & LLWU_MF5_MWUF4_MASK)
#define LLWU_MF5_MWUF5_MASK                      (0x20U)
#define LLWU_MF5_MWUF5_SHIFT                     (5U)
#define LLWU_MF5_MWUF5(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF5_SHIFT)) & LLWU_MF5_MWUF5_MASK)
#define LLWU_MF5_MWUF6_MASK                      (0x40U)
#define LLWU_MF5_MWUF6_SHIFT                     (6U)
#define LLWU_MF5_MWUF6(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF6_SHIFT)) & LLWU_MF5_MWUF6_MASK)
#define LLWU_MF5_MWUF7_MASK                      (0x80U)
#define LLWU_MF5_MWUF7_SHIFT                     (7U)
#define LLWU_MF5_MWUF7(x)                        (((uint8_t)(((uint8_t)(x)) << LLWU_MF5_MWUF7_SHIFT)) & LLWU_MF5_MWUF7_MASK)

/*! @name FILT1 - LLWU Pin Filter 1 register */
#define LLWU_FILT1_FILTSEL_MASK                  (0x1FU)
#define LLWU_FILT1_FILTSEL_SHIFT                 (0U)
#define LLWU_FILT1_FILTSEL(x)                    (((uint8_t)(((uint8_t)(x)) << LLWU_FILT1_FILTSEL_SHIFT)) & LLWU_FILT1_FILTSEL_MASK)
#define LLWU_FILT1_FILTE_MASK                    (0x60U)
#define LLWU_FILT1_FILTE_SHIFT                   (5U)
#define LLWU_FILT1_FILTE(x)                      (((uint8_t)(((uint8_t)(x)) << LLWU_FILT1_FILTE_SHIFT)) & LLWU_FILT1_FILTE_MASK)
#define LLWU_FILT1_FILTF_MASK                    (0x80U)
#define LLWU_FILT1_FILTF_SHIFT                   (7U)
#define LLWU_FILT1_FILTF(x)                      (((uint8_t)(((uint8_t)(x)) << LLWU_FILT1_FILTF_SHIFT)) & LLWU_FILT1_FILTF_MASK)

/*! @name FILT2 - LLWU Pin Filter 2 register */
#define LLWU_FILT2_FILTSEL_MASK                  (0x1FU)
#define LLWU_FILT2_FILTSEL_SHIFT                 (0U)
#define LLWU_FILT2_FILTSEL(x)                    (((uint8_t)(((uint8_t)(x)) << LLWU_FILT2_FILTSEL_SHIFT)) & LLWU_FILT2_FILTSEL_MASK)
#define LLWU_FILT2_FILTE_MASK                    (0x60U)
#define LLWU_FILT2_FILTE_SHIFT                   (5U)
#define LLWU_FILT2_FILTE(x)                      (((uint8_t)(((uint8_t)(x)) << LLWU_FILT2_FILTE_SHIFT)) & LLWU_FILT2_FILTE_MASK)
#define LLWU_FILT2_FILTF_MASK                    (0x80U)
#define LLWU_FILT2_FILTF_SHIFT                   (7U)
#define LLWU_FILT2_FILTF(x)                      (((uint8_t)(((uint8_t)(x)) << LLWU_FILT2_FILTF_SHIFT)) & LLWU_FILT2_FILTF_MASK)


/*!
 * @}
 */ /* end of group LLWU_Register_Masks */


/* LLWU - Peripheral instance base addresses */
/** Peripheral LLWU base address */
#define LLWU_BASE                                (0x4007C000u)
/** Peripheral LLWU base pointer */
#define LLWU                                     ((LLWU_Type *)LLWU_BASE)
/** Array initializer of LLWU peripheral base addresses */
#define LLWU_BASE_ADDRS                          { LLWU_BASE }
/** Array initializer of LLWU peripheral base pointers */
#define LLWU_BASE_PTRS                           { LLWU }
/** Interrupt vectors for the LLWU peripheral type */
#define LLWU_IRQS                                { LLWU_IRQn }

/*!
 * @}
 */ /* end of group LLWU_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- LPTMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Peripheral_Access_Layer LPTMR Peripheral Access Layer
 * @{
 */

/** LPTMR - Register Layout Typedef */
typedef struct {
  __IO uint32_t CSR;                               /**< Low Power Timer Control Status Register, offset: 0x0 */
  __IO uint32_t PSR;                               /**< Low Power Timer Prescale Register, offset: 0x4 */
  __IO uint32_t CMR;                               /**< Low Power Timer Compare Register, offset: 0x8 */
  __IO uint32_t CNR;                               /**< Low Power Timer Counter Register, offset: 0xC */
} LPTMR_Type;

/* ----------------------------------------------------------------------------
   -- LPTMR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPTMR_Register_Masks LPTMR Register Masks
 * @{
 */

/*! @name CSR - Low Power Timer Control Status Register */
#define LPTMR_CSR_TEN_MASK                       (0x1U)
#define LPTMR_CSR_TEN_SHIFT                      (0U)
#define LPTMR_CSR_TEN(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TEN_SHIFT)) & LPTMR_CSR_TEN_MASK)
#define LPTMR_CSR_TMS_MASK                       (0x2U)
#define LPTMR_CSR_TMS_SHIFT                      (1U)
#define LPTMR_CSR_TMS(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TMS_SHIFT)) & LPTMR_CSR_TMS_MASK)
#define LPTMR_CSR_TFC_MASK                       (0x4U)
#define LPTMR_CSR_TFC_SHIFT                      (2U)
#define LPTMR_CSR_TFC(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TFC_SHIFT)) & LPTMR_CSR_TFC_MASK)
#define LPTMR_CSR_TPP_MASK                       (0x8U)
#define LPTMR_CSR_TPP_SHIFT                      (3U)
#define LPTMR_CSR_TPP(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TPP_SHIFT)) & LPTMR_CSR_TPP_MASK)
#define LPTMR_CSR_TPS_MASK                       (0x30U)
#define LPTMR_CSR_TPS_SHIFT                      (4U)
#define LPTMR_CSR_TPS(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TPS_SHIFT)) & LPTMR_CSR_TPS_MASK)
#define LPTMR_CSR_TIE_MASK                       (0x40U)
#define LPTMR_CSR_TIE_SHIFT                      (6U)
#define LPTMR_CSR_TIE(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TIE_SHIFT)) & LPTMR_CSR_TIE_MASK)
#define LPTMR_CSR_TCF_MASK                       (0x80U)
#define LPTMR_CSR_TCF_SHIFT                      (7U)
#define LPTMR_CSR_TCF(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_CSR_TCF_SHIFT)) & LPTMR_CSR_TCF_MASK)

/*! @name PSR - Low Power Timer Prescale Register */
#define LPTMR_PSR_PCS_MASK                       (0x3U)
#define LPTMR_PSR_PCS_SHIFT                      (0U)
#define LPTMR_PSR_PCS(x)                         (((uint32_t)(((uint32_t)(x)) << LPTMR_PSR_PCS_SHIFT)) & LPTMR_PSR_PCS_MASK)
#define LPTMR_PSR_PBYP_MASK                      (0x4U)
#define LPTMR_PSR_PBYP_SHIFT                     (2U)
#define LPTMR_PSR_PBYP(x)                        (((uint32_t)(((uint32_t)(x)) << LPTMR_PSR_PBYP_SHIFT)) & LPTMR_PSR_PBYP_MASK)
#define LPTMR_PSR_PRESCALE_MASK                  (0x78U)
#define LPTMR_PSR_PRESCALE_SHIFT                 (3U)
#define LPTMR_PSR_PRESCALE(x)                    (((uint32_t)(((uint32_t)(x)) << LPTMR_PSR_PRESCALE_SHIFT)) & LPTMR_PSR_PRESCALE_MASK)

/*! @name CMR - Low Power Timer Compare Register */
#define LPTMR_CMR_COMPARE_MASK                   (0xFFFFU)
#define LPTMR_CMR_COMPARE_SHIFT                  (0U)
#define LPTMR_CMR_COMPARE(x)                     (((uint32_t)(((uint32_t)(x)) << LPTMR_CMR_COMPARE_SHIFT)) & LPTMR_CMR_COMPARE_MASK)

/*! @name CNR - Low Power Timer Counter Register */
#define LPTMR_CNR_COUNTER_MASK                   (0xFFFFU)
#define LPTMR_CNR_COUNTER_SHIFT                  (0U)
#define LPTMR_CNR_COUNTER(x)                     (((uint32_t)(((uint32_t)(x)) << LPTMR_CNR_COUNTER_SHIFT)) & LPTMR_CNR_COUNTER_MASK)


/*!
 * @}
 */ /* end of group LPTMR_Register_Masks */


/* LPTMR - Peripheral instance base addresses */
/** Peripheral LPTMR0 base address */
#define LPTMR0_BASE                              (0x4003C000u)
/** Peripheral LPTMR0 base pointer */
#define LPTMR0                                   ((LPTMR_Type *)LPTMR0_BASE)
/** Array initializer of LPTMR peripheral base addresses */
#define LPTMR_BASE_ADDRS                         { LPTMR0_BASE }
/** Array initializer of LPTMR peripheral base pointers */
#define LPTMR_BASE_PTRS                          { LPTMR0 }
/** Interrupt vectors for the LPTMR peripheral type */
#define LPTMR_IRQS                               { LPTMR0_IRQn }

/*!
 * @}
 */ /* end of group LPTMR_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- LPUART Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPUART_Peripheral_Access_Layer LPUART Peripheral Access Layer
 * @{
 */

/** LPUART - Register Layout Typedef */
typedef struct {
  __IO uint32_t BAUD;                              /**< LPUART Baud Rate Register, offset: 0x0 */
  __IO uint32_t STAT;                              /**< LPUART Status Register, offset: 0x4 */
  __IO uint32_t CTRL;                              /**< LPUART Control Register, offset: 0x8 */
  __IO uint32_t DATA;                              /**< LPUART Data Register, offset: 0xC */
  __IO uint32_t MATCH;                             /**< LPUART Match Address Register, offset: 0x10 */
  __IO uint32_t MODIR;                             /**< LPUART Modem IrDA Register, offset: 0x14 */
} LPUART_Type;

/* ----------------------------------------------------------------------------
   -- LPUART Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup LPUART_Register_Masks LPUART Register Masks
 * @{
 */

/*! @name BAUD - LPUART Baud Rate Register */
#define LPUART_BAUD_SBR_MASK                     (0x1FFFU)
#define LPUART_BAUD_SBR_SHIFT                    (0U)
#define LPUART_BAUD_SBR(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_SBR_SHIFT)) & LPUART_BAUD_SBR_MASK)
#define LPUART_BAUD_SBNS_MASK                    (0x2000U)
#define LPUART_BAUD_SBNS_SHIFT                   (13U)
#define LPUART_BAUD_SBNS(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_SBNS_SHIFT)) & LPUART_BAUD_SBNS_MASK)
#define LPUART_BAUD_RXEDGIE_MASK                 (0x4000U)
#define LPUART_BAUD_RXEDGIE_SHIFT                (14U)
#define LPUART_BAUD_RXEDGIE(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_RXEDGIE_SHIFT)) & LPUART_BAUD_RXEDGIE_MASK)
#define LPUART_BAUD_LBKDIE_MASK                  (0x8000U)
#define LPUART_BAUD_LBKDIE_SHIFT                 (15U)
#define LPUART_BAUD_LBKDIE(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_LBKDIE_SHIFT)) & LPUART_BAUD_LBKDIE_MASK)
#define LPUART_BAUD_RESYNCDIS_MASK               (0x10000U)
#define LPUART_BAUD_RESYNCDIS_SHIFT              (16U)
#define LPUART_BAUD_RESYNCDIS(x)                 (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_RESYNCDIS_SHIFT)) & LPUART_BAUD_RESYNCDIS_MASK)
#define LPUART_BAUD_BOTHEDGE_MASK                (0x20000U)
#define LPUART_BAUD_BOTHEDGE_SHIFT               (17U)
#define LPUART_BAUD_BOTHEDGE(x)                  (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_BOTHEDGE_SHIFT)) & LPUART_BAUD_BOTHEDGE_MASK)
#define LPUART_BAUD_MATCFG_MASK                  (0xC0000U)
#define LPUART_BAUD_MATCFG_SHIFT                 (18U)
#define LPUART_BAUD_MATCFG(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_MATCFG_SHIFT)) & LPUART_BAUD_MATCFG_MASK)
#define LPUART_BAUD_RDMAE_MASK                   (0x200000U)
#define LPUART_BAUD_RDMAE_SHIFT                  (21U)
#define LPUART_BAUD_RDMAE(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_RDMAE_SHIFT)) & LPUART_BAUD_RDMAE_MASK)
#define LPUART_BAUD_TDMAE_MASK                   (0x800000U)
#define LPUART_BAUD_TDMAE_SHIFT                  (23U)
#define LPUART_BAUD_TDMAE(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_TDMAE_SHIFT)) & LPUART_BAUD_TDMAE_MASK)
#define LPUART_BAUD_OSR_MASK                     (0x1F000000U)
#define LPUART_BAUD_OSR_SHIFT                    (24U)
#define LPUART_BAUD_OSR(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_OSR_SHIFT)) & LPUART_BAUD_OSR_MASK)
#define LPUART_BAUD_M10_MASK                     (0x20000000U)
#define LPUART_BAUD_M10_SHIFT                    (29U)
#define LPUART_BAUD_M10(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_M10_SHIFT)) & LPUART_BAUD_M10_MASK)
#define LPUART_BAUD_MAEN2_MASK                   (0x40000000U)
#define LPUART_BAUD_MAEN2_SHIFT                  (30U)
#define LPUART_BAUD_MAEN2(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_MAEN2_SHIFT)) & LPUART_BAUD_MAEN2_MASK)
#define LPUART_BAUD_MAEN1_MASK                   (0x80000000U)
#define LPUART_BAUD_MAEN1_SHIFT                  (31U)
#define LPUART_BAUD_MAEN1(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_BAUD_MAEN1_SHIFT)) & LPUART_BAUD_MAEN1_MASK)

/*! @name STAT - LPUART Status Register */
#define LPUART_STAT_MA2F_MASK                    (0x4000U)
#define LPUART_STAT_MA2F_SHIFT                   (14U)
#define LPUART_STAT_MA2F(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_MA2F_SHIFT)) & LPUART_STAT_MA2F_MASK)
#define LPUART_STAT_MA1F_MASK                    (0x8000U)
#define LPUART_STAT_MA1F_SHIFT                   (15U)
#define LPUART_STAT_MA1F(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_MA1F_SHIFT)) & LPUART_STAT_MA1F_MASK)
#define LPUART_STAT_PF_MASK                      (0x10000U)
#define LPUART_STAT_PF_SHIFT                     (16U)
#define LPUART_STAT_PF(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_PF_SHIFT)) & LPUART_STAT_PF_MASK)
#define LPUART_STAT_FE_MASK                      (0x20000U)
#define LPUART_STAT_FE_SHIFT                     (17U)
#define LPUART_STAT_FE(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_FE_SHIFT)) & LPUART_STAT_FE_MASK)
#define LPUART_STAT_NF_MASK                      (0x40000U)
#define LPUART_STAT_NF_SHIFT                     (18U)
#define LPUART_STAT_NF(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_NF_SHIFT)) & LPUART_STAT_NF_MASK)
#define LPUART_STAT_OR_MASK                      (0x80000U)
#define LPUART_STAT_OR_SHIFT                     (19U)
#define LPUART_STAT_OR(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_OR_SHIFT)) & LPUART_STAT_OR_MASK)
#define LPUART_STAT_IDLE_MASK                    (0x100000U)
#define LPUART_STAT_IDLE_SHIFT                   (20U)
#define LPUART_STAT_IDLE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_IDLE_SHIFT)) & LPUART_STAT_IDLE_MASK)
#define LPUART_STAT_RDRF_MASK                    (0x200000U)
#define LPUART_STAT_RDRF_SHIFT                   (21U)
#define LPUART_STAT_RDRF(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RDRF_SHIFT)) & LPUART_STAT_RDRF_MASK)
#define LPUART_STAT_TC_MASK                      (0x400000U)
#define LPUART_STAT_TC_SHIFT                     (22U)
#define LPUART_STAT_TC(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_TC_SHIFT)) & LPUART_STAT_TC_MASK)
#define LPUART_STAT_TDRE_MASK                    (0x800000U)
#define LPUART_STAT_TDRE_SHIFT                   (23U)
#define LPUART_STAT_TDRE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_TDRE_SHIFT)) & LPUART_STAT_TDRE_MASK)
#define LPUART_STAT_RAF_MASK                     (0x1000000U)
#define LPUART_STAT_RAF_SHIFT                    (24U)
#define LPUART_STAT_RAF(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RAF_SHIFT)) & LPUART_STAT_RAF_MASK)
#define LPUART_STAT_LBKDE_MASK                   (0x2000000U)
#define LPUART_STAT_LBKDE_SHIFT                  (25U)
#define LPUART_STAT_LBKDE(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_LBKDE_SHIFT)) & LPUART_STAT_LBKDE_MASK)
#define LPUART_STAT_BRK13_MASK                   (0x4000000U)
#define LPUART_STAT_BRK13_SHIFT                  (26U)
#define LPUART_STAT_BRK13(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_BRK13_SHIFT)) & LPUART_STAT_BRK13_MASK)
#define LPUART_STAT_RWUID_MASK                   (0x8000000U)
#define LPUART_STAT_RWUID_SHIFT                  (27U)
#define LPUART_STAT_RWUID(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RWUID_SHIFT)) & LPUART_STAT_RWUID_MASK)
#define LPUART_STAT_RXINV_MASK                   (0x10000000U)
#define LPUART_STAT_RXINV_SHIFT                  (28U)
#define LPUART_STAT_RXINV(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RXINV_SHIFT)) & LPUART_STAT_RXINV_MASK)
#define LPUART_STAT_MSBF_MASK                    (0x20000000U)
#define LPUART_STAT_MSBF_SHIFT                   (29U)
#define LPUART_STAT_MSBF(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_MSBF_SHIFT)) & LPUART_STAT_MSBF_MASK)
#define LPUART_STAT_RXEDGIF_MASK                 (0x40000000U)
#define LPUART_STAT_RXEDGIF_SHIFT                (30U)
#define LPUART_STAT_RXEDGIF(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_RXEDGIF_SHIFT)) & LPUART_STAT_RXEDGIF_MASK)
#define LPUART_STAT_LBKDIF_MASK                  (0x80000000U)
#define LPUART_STAT_LBKDIF_SHIFT                 (31U)
#define LPUART_STAT_LBKDIF(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_STAT_LBKDIF_SHIFT)) & LPUART_STAT_LBKDIF_MASK)

/*! @name CTRL - LPUART Control Register */
#define LPUART_CTRL_PT_MASK                      (0x1U)
#define LPUART_CTRL_PT_SHIFT                     (0U)
#define LPUART_CTRL_PT(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_PT_SHIFT)) & LPUART_CTRL_PT_MASK)
#define LPUART_CTRL_PE_MASK                      (0x2U)
#define LPUART_CTRL_PE_SHIFT                     (1U)
#define LPUART_CTRL_PE(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_PE_SHIFT)) & LPUART_CTRL_PE_MASK)
#define LPUART_CTRL_ILT_MASK                     (0x4U)
#define LPUART_CTRL_ILT_SHIFT                    (2U)
#define LPUART_CTRL_ILT(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_ILT_SHIFT)) & LPUART_CTRL_ILT_MASK)
#define LPUART_CTRL_WAKE_MASK                    (0x8U)
#define LPUART_CTRL_WAKE_SHIFT                   (3U)
#define LPUART_CTRL_WAKE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_WAKE_SHIFT)) & LPUART_CTRL_WAKE_MASK)
#define LPUART_CTRL_M_MASK                       (0x10U)
#define LPUART_CTRL_M_SHIFT                      (4U)
#define LPUART_CTRL_M(x)                         (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_M_SHIFT)) & LPUART_CTRL_M_MASK)
#define LPUART_CTRL_RSRC_MASK                    (0x20U)
#define LPUART_CTRL_RSRC_SHIFT                   (5U)
#define LPUART_CTRL_RSRC(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_RSRC_SHIFT)) & LPUART_CTRL_RSRC_MASK)
#define LPUART_CTRL_DOZEEN_MASK                  (0x40U)
#define LPUART_CTRL_DOZEEN_SHIFT                 (6U)
#define LPUART_CTRL_DOZEEN(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_DOZEEN_SHIFT)) & LPUART_CTRL_DOZEEN_MASK)
#define LPUART_CTRL_LOOPS_MASK                   (0x80U)
#define LPUART_CTRL_LOOPS_SHIFT                  (7U)
#define LPUART_CTRL_LOOPS(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_LOOPS_SHIFT)) & LPUART_CTRL_LOOPS_MASK)
#define LPUART_CTRL_IDLECFG_MASK                 (0x700U)
#define LPUART_CTRL_IDLECFG_SHIFT                (8U)
#define LPUART_CTRL_IDLECFG(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_IDLECFG_SHIFT)) & LPUART_CTRL_IDLECFG_MASK)
#define LPUART_CTRL_MA2IE_MASK                   (0x4000U)
#define LPUART_CTRL_MA2IE_SHIFT                  (14U)
#define LPUART_CTRL_MA2IE(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_MA2IE_SHIFT)) & LPUART_CTRL_MA2IE_MASK)
#define LPUART_CTRL_MA1IE_MASK                   (0x8000U)
#define LPUART_CTRL_MA1IE_SHIFT                  (15U)
#define LPUART_CTRL_MA1IE(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_MA1IE_SHIFT)) & LPUART_CTRL_MA1IE_MASK)
#define LPUART_CTRL_SBK_MASK                     (0x10000U)
#define LPUART_CTRL_SBK_SHIFT                    (16U)
#define LPUART_CTRL_SBK(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_SBK_SHIFT)) & LPUART_CTRL_SBK_MASK)
#define LPUART_CTRL_RWU_MASK                     (0x20000U)
#define LPUART_CTRL_RWU_SHIFT                    (17U)
#define LPUART_CTRL_RWU(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_RWU_SHIFT)) & LPUART_CTRL_RWU_MASK)
#define LPUART_CTRL_RE_MASK                      (0x40000U)
#define LPUART_CTRL_RE_SHIFT                     (18U)
#define LPUART_CTRL_RE(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_RE_SHIFT)) & LPUART_CTRL_RE_MASK)
#define LPUART_CTRL_TE_MASK                      (0x80000U)
#define LPUART_CTRL_TE_SHIFT                     (19U)
#define LPUART_CTRL_TE(x)                        (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TE_SHIFT)) & LPUART_CTRL_TE_MASK)
#define LPUART_CTRL_ILIE_MASK                    (0x100000U)
#define LPUART_CTRL_ILIE_SHIFT                   (20U)
#define LPUART_CTRL_ILIE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_ILIE_SHIFT)) & LPUART_CTRL_ILIE_MASK)
#define LPUART_CTRL_RIE_MASK                     (0x200000U)
#define LPUART_CTRL_RIE_SHIFT                    (21U)
#define LPUART_CTRL_RIE(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_RIE_SHIFT)) & LPUART_CTRL_RIE_MASK)
#define LPUART_CTRL_TCIE_MASK                    (0x400000U)
#define LPUART_CTRL_TCIE_SHIFT                   (22U)
#define LPUART_CTRL_TCIE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TCIE_SHIFT)) & LPUART_CTRL_TCIE_MASK)
#define LPUART_CTRL_TIE_MASK                     (0x800000U)
#define LPUART_CTRL_TIE_SHIFT                    (23U)
#define LPUART_CTRL_TIE(x)                       (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TIE_SHIFT)) & LPUART_CTRL_TIE_MASK)
#define LPUART_CTRL_PEIE_MASK                    (0x1000000U)
#define LPUART_CTRL_PEIE_SHIFT                   (24U)
#define LPUART_CTRL_PEIE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_PEIE_SHIFT)) & LPUART_CTRL_PEIE_MASK)
#define LPUART_CTRL_FEIE_MASK                    (0x2000000U)
#define LPUART_CTRL_FEIE_SHIFT                   (25U)
#define LPUART_CTRL_FEIE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_FEIE_SHIFT)) & LPUART_CTRL_FEIE_MASK)
#define LPUART_CTRL_NEIE_MASK                    (0x4000000U)
#define LPUART_CTRL_NEIE_SHIFT                   (26U)
#define LPUART_CTRL_NEIE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_NEIE_SHIFT)) & LPUART_CTRL_NEIE_MASK)
#define LPUART_CTRL_ORIE_MASK                    (0x8000000U)
#define LPUART_CTRL_ORIE_SHIFT                   (27U)
#define LPUART_CTRL_ORIE(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_ORIE_SHIFT)) & LPUART_CTRL_ORIE_MASK)
#define LPUART_CTRL_TXINV_MASK                   (0x10000000U)
#define LPUART_CTRL_TXINV_SHIFT                  (28U)
#define LPUART_CTRL_TXINV(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TXINV_SHIFT)) & LPUART_CTRL_TXINV_MASK)
#define LPUART_CTRL_TXDIR_MASK                   (0x20000000U)
#define LPUART_CTRL_TXDIR_SHIFT                  (29U)
#define LPUART_CTRL_TXDIR(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_TXDIR_SHIFT)) & LPUART_CTRL_TXDIR_MASK)
#define LPUART_CTRL_R9T8_MASK                    (0x40000000U)
#define LPUART_CTRL_R9T8_SHIFT                   (30U)
#define LPUART_CTRL_R9T8(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_R9T8_SHIFT)) & LPUART_CTRL_R9T8_MASK)
#define LPUART_CTRL_R8T9_MASK                    (0x80000000U)
#define LPUART_CTRL_R8T9_SHIFT                   (31U)
#define LPUART_CTRL_R8T9(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_CTRL_R8T9_SHIFT)) & LPUART_CTRL_R8T9_MASK)

/*! @name DATA - LPUART Data Register */
#define LPUART_DATA_R0T0_MASK                    (0x1U)
#define LPUART_DATA_R0T0_SHIFT                   (0U)
#define LPUART_DATA_R0T0(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R0T0_SHIFT)) & LPUART_DATA_R0T0_MASK)
#define LPUART_DATA_R1T1_MASK                    (0x2U)
#define LPUART_DATA_R1T1_SHIFT                   (1U)
#define LPUART_DATA_R1T1(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R1T1_SHIFT)) & LPUART_DATA_R1T1_MASK)
#define LPUART_DATA_R2T2_MASK                    (0x4U)
#define LPUART_DATA_R2T2_SHIFT                   (2U)
#define LPUART_DATA_R2T2(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R2T2_SHIFT)) & LPUART_DATA_R2T2_MASK)
#define LPUART_DATA_R3T3_MASK                    (0x8U)
#define LPUART_DATA_R3T3_SHIFT                   (3U)
#define LPUART_DATA_R3T3(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R3T3_SHIFT)) & LPUART_DATA_R3T3_MASK)
#define LPUART_DATA_R4T4_MASK                    (0x10U)
#define LPUART_DATA_R4T4_SHIFT                   (4U)
#define LPUART_DATA_R4T4(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R4T4_SHIFT)) & LPUART_DATA_R4T4_MASK)
#define LPUART_DATA_R5T5_MASK                    (0x20U)
#define LPUART_DATA_R5T5_SHIFT                   (5U)
#define LPUART_DATA_R5T5(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R5T5_SHIFT)) & LPUART_DATA_R5T5_MASK)
#define LPUART_DATA_R6T6_MASK                    (0x40U)
#define LPUART_DATA_R6T6_SHIFT                   (6U)
#define LPUART_DATA_R6T6(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R6T6_SHIFT)) & LPUART_DATA_R6T6_MASK)
#define LPUART_DATA_R7T7_MASK                    (0x80U)
#define LPUART_DATA_R7T7_SHIFT                   (7U)
#define LPUART_DATA_R7T7(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R7T7_SHIFT)) & LPUART_DATA_R7T7_MASK)
#define LPUART_DATA_R8T8_MASK                    (0x100U)
#define LPUART_DATA_R8T8_SHIFT                   (8U)
#define LPUART_DATA_R8T8(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R8T8_SHIFT)) & LPUART_DATA_R8T8_MASK)
#define LPUART_DATA_R9T9_MASK                    (0x200U)
#define LPUART_DATA_R9T9_SHIFT                   (9U)
#define LPUART_DATA_R9T9(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_R9T9_SHIFT)) & LPUART_DATA_R9T9_MASK)
#define LPUART_DATA_IDLINE_MASK                  (0x800U)
#define LPUART_DATA_IDLINE_SHIFT                 (11U)
#define LPUART_DATA_IDLINE(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_IDLINE_SHIFT)) & LPUART_DATA_IDLINE_MASK)
#define LPUART_DATA_RXEMPT_MASK                  (0x1000U)
#define LPUART_DATA_RXEMPT_SHIFT                 (12U)
#define LPUART_DATA_RXEMPT(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_RXEMPT_SHIFT)) & LPUART_DATA_RXEMPT_MASK)
#define LPUART_DATA_FRETSC_MASK                  (0x2000U)
#define LPUART_DATA_FRETSC_SHIFT                 (13U)
#define LPUART_DATA_FRETSC(x)                    (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_FRETSC_SHIFT)) & LPUART_DATA_FRETSC_MASK)
#define LPUART_DATA_PARITYE_MASK                 (0x4000U)
#define LPUART_DATA_PARITYE_SHIFT                (14U)
#define LPUART_DATA_PARITYE(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_PARITYE_SHIFT)) & LPUART_DATA_PARITYE_MASK)
#define LPUART_DATA_NOISY_MASK                   (0x8000U)
#define LPUART_DATA_NOISY_SHIFT                  (15U)
#define LPUART_DATA_NOISY(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_DATA_NOISY_SHIFT)) & LPUART_DATA_NOISY_MASK)

/*! @name MATCH - LPUART Match Address Register */
#define LPUART_MATCH_MA1_MASK                    (0x3FFU)
#define LPUART_MATCH_MA1_SHIFT                   (0U)
#define LPUART_MATCH_MA1(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_MATCH_MA1_SHIFT)) & LPUART_MATCH_MA1_MASK)
#define LPUART_MATCH_MA2_MASK                    (0x3FF0000U)
#define LPUART_MATCH_MA2_SHIFT                   (16U)
#define LPUART_MATCH_MA2(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_MATCH_MA2_SHIFT)) & LPUART_MATCH_MA2_MASK)

/*! @name MODIR - LPUART Modem IrDA Register */
#define LPUART_MODIR_TXCTSE_MASK                 (0x1U)
#define LPUART_MODIR_TXCTSE_SHIFT                (0U)
#define LPUART_MODIR_TXCTSE(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_TXCTSE_SHIFT)) & LPUART_MODIR_TXCTSE_MASK)
#define LPUART_MODIR_TXRTSE_MASK                 (0x2U)
#define LPUART_MODIR_TXRTSE_SHIFT                (1U)
#define LPUART_MODIR_TXRTSE(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_TXRTSE_SHIFT)) & LPUART_MODIR_TXRTSE_MASK)
#define LPUART_MODIR_TXRTSPOL_MASK               (0x4U)
#define LPUART_MODIR_TXRTSPOL_SHIFT              (2U)
#define LPUART_MODIR_TXRTSPOL(x)                 (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_TXRTSPOL_SHIFT)) & LPUART_MODIR_TXRTSPOL_MASK)
#define LPUART_MODIR_RXRTSE_MASK                 (0x8U)
#define LPUART_MODIR_RXRTSE_SHIFT                (3U)
#define LPUART_MODIR_RXRTSE(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_RXRTSE_SHIFT)) & LPUART_MODIR_RXRTSE_MASK)
#define LPUART_MODIR_TXCTSC_MASK                 (0x10U)
#define LPUART_MODIR_TXCTSC_SHIFT                (4U)
#define LPUART_MODIR_TXCTSC(x)                   (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_TXCTSC_SHIFT)) & LPUART_MODIR_TXCTSC_MASK)
#define LPUART_MODIR_TXCTSSRC_MASK               (0x20U)
#define LPUART_MODIR_TXCTSSRC_SHIFT              (5U)
#define LPUART_MODIR_TXCTSSRC(x)                 (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_TXCTSSRC_SHIFT)) & LPUART_MODIR_TXCTSSRC_MASK)
#define LPUART_MODIR_TNP_MASK                    (0x30000U)
#define LPUART_MODIR_TNP_SHIFT                   (16U)
#define LPUART_MODIR_TNP(x)                      (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_TNP_SHIFT)) & LPUART_MODIR_TNP_MASK)
#define LPUART_MODIR_IREN_MASK                   (0x40000U)
#define LPUART_MODIR_IREN_SHIFT                  (18U)
#define LPUART_MODIR_IREN(x)                     (((uint32_t)(((uint32_t)(x)) << LPUART_MODIR_IREN_SHIFT)) & LPUART_MODIR_IREN_MASK)


/*!
 * @}
 */ /* end of group LPUART_Register_Masks */


/* LPUART - Peripheral instance base addresses */
/** Peripheral LPUART0 base address */
#define LPUART0_BASE                             (0x4002A000u)
/** Peripheral LPUART0 base pointer */
#define LPUART0                                  ((LPUART_Type *)LPUART0_BASE)
/** Array initializer of LPUART peripheral base addresses */
#define LPUART_BASE_ADDRS                        { LPUART0_BASE }
/** Array initializer of LPUART peripheral base pointers */
#define LPUART_BASE_PTRS                         { LPUART0 }
/** Interrupt vectors for the LPUART peripheral type */
#define LPUART_RX_TX_IRQS                        { LPUART0_IRQn }
#define LPUART_ERR_IRQS                          { LPUART0_IRQn }

/*!
 * @}
 */ /* end of group LPUART_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MCG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Peripheral_Access_Layer MCG Peripheral Access Layer
 * @{
 */

/** MCG - Register Layout Typedef */
typedef struct {
  __IO uint8_t C1;                                 /**< MCG Control 1 Register, offset: 0x0 */
  __IO uint8_t C2;                                 /**< MCG Control 2 Register, offset: 0x1 */
  __IO uint8_t C3;                                 /**< MCG Control 3 Register, offset: 0x2 */
  __IO uint8_t C4;                                 /**< MCG Control 4 Register, offset: 0x3 */
  __IO uint8_t C5;                                 /**< MCG Control 5 Register, offset: 0x4 */
  __IO uint8_t C6;                                 /**< MCG Control 6 Register, offset: 0x5 */
  __IO uint8_t S;                                  /**< MCG Status Register, offset: 0x6 */
       uint8_t RESERVED_0[1];
  __IO uint8_t SC;                                 /**< MCG Status and Control Register, offset: 0x8 */
       uint8_t RESERVED_1[1];
  __IO uint8_t ATCVH;                              /**< MCG Auto Trim Compare Value High Register, offset: 0xA */
  __IO uint8_t ATCVL;                              /**< MCG Auto Trim Compare Value Low Register, offset: 0xB */
  __IO uint8_t C7;                                 /**< MCG Control 7 Register, offset: 0xC */
  __IO uint8_t C8;                                 /**< MCG Control 8 Register, offset: 0xD */
  __I  uint8_t C9;                                 /**< MCG Control 9 Register, offset: 0xE */
} MCG_Type;

/* ----------------------------------------------------------------------------
   -- MCG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCG_Register_Masks MCG Register Masks
 * @{
 */

/*! @name C1 - MCG Control 1 Register */
#define MCG_C1_IREFSTEN_MASK                     (0x1U)
#define MCG_C1_IREFSTEN_SHIFT                    (0U)
#define MCG_C1_IREFSTEN(x)                       (((uint8_t)(((uint8_t)(x)) << MCG_C1_IREFSTEN_SHIFT)) & MCG_C1_IREFSTEN_MASK)
#define MCG_C1_IRCLKEN_MASK                      (0x2U)
#define MCG_C1_IRCLKEN_SHIFT                     (1U)
#define MCG_C1_IRCLKEN(x)                        (((uint8_t)(((uint8_t)(x)) << MCG_C1_IRCLKEN_SHIFT)) & MCG_C1_IRCLKEN_MASK)
#define MCG_C1_IREFS_MASK                        (0x4U)
#define MCG_C1_IREFS_SHIFT                       (2U)
#define MCG_C1_IREFS(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_C1_IREFS_SHIFT)) & MCG_C1_IREFS_MASK)
#define MCG_C1_FRDIV_MASK                        (0x38U)
#define MCG_C1_FRDIV_SHIFT                       (3U)
#define MCG_C1_FRDIV(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_C1_FRDIV_SHIFT)) & MCG_C1_FRDIV_MASK)
#define MCG_C1_CLKS_MASK                         (0xC0U)
#define MCG_C1_CLKS_SHIFT                        (6U)
#define MCG_C1_CLKS(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_C1_CLKS_SHIFT)) & MCG_C1_CLKS_MASK)

/*! @name C2 - MCG Control 2 Register */
#define MCG_C2_IRCS_MASK                         (0x1U)
#define MCG_C2_IRCS_SHIFT                        (0U)
#define MCG_C2_IRCS(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_C2_IRCS_SHIFT)) & MCG_C2_IRCS_MASK)
#define MCG_C2_LP_MASK                           (0x2U)
#define MCG_C2_LP_SHIFT                          (1U)
#define MCG_C2_LP(x)                             (((uint8_t)(((uint8_t)(x)) << MCG_C2_LP_SHIFT)) & MCG_C2_LP_MASK)
#define MCG_C2_EREFS0_MASK                       (0x4U)
#define MCG_C2_EREFS0_SHIFT                      (2U)
#define MCG_C2_EREFS0(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C2_EREFS0_SHIFT)) & MCG_C2_EREFS0_MASK)
#define MCG_C2_HGO0_MASK                         (0x8U)
#define MCG_C2_HGO0_SHIFT                        (3U)
#define MCG_C2_HGO0(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_C2_HGO0_SHIFT)) & MCG_C2_HGO0_MASK)
#define MCG_C2_RANGE0_MASK                       (0x30U)
#define MCG_C2_RANGE0_SHIFT                      (4U)
#define MCG_C2_RANGE0(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C2_RANGE0_SHIFT)) & MCG_C2_RANGE0_MASK)
#define MCG_C2_LOCRE0_MASK                       (0x80U)
#define MCG_C2_LOCRE0_SHIFT                      (7U)
#define MCG_C2_LOCRE0(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C2_LOCRE0_SHIFT)) & MCG_C2_LOCRE0_MASK)

/*! @name C3 - MCG Control 3 Register */
#define MCG_C3_SCTRIM_MASK                       (0xFFU)
#define MCG_C3_SCTRIM_SHIFT                      (0U)
#define MCG_C3_SCTRIM(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C3_SCTRIM_SHIFT)) & MCG_C3_SCTRIM_MASK)

/*! @name C4 - MCG Control 4 Register */
#define MCG_C4_SCFTRIM_MASK                      (0x1U)
#define MCG_C4_SCFTRIM_SHIFT                     (0U)
#define MCG_C4_SCFTRIM(x)                        (((uint8_t)(((uint8_t)(x)) << MCG_C4_SCFTRIM_SHIFT)) & MCG_C4_SCFTRIM_MASK)
#define MCG_C4_FCTRIM_MASK                       (0x1EU)
#define MCG_C4_FCTRIM_SHIFT                      (1U)
#define MCG_C4_FCTRIM(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C4_FCTRIM_SHIFT)) & MCG_C4_FCTRIM_MASK)
#define MCG_C4_DRST_DRS_MASK                     (0x60U)
#define MCG_C4_DRST_DRS_SHIFT                    (5U)
#define MCG_C4_DRST_DRS(x)                       (((uint8_t)(((uint8_t)(x)) << MCG_C4_DRST_DRS_SHIFT)) & MCG_C4_DRST_DRS_MASK)
#define MCG_C4_DMX32_MASK                        (0x80U)
#define MCG_C4_DMX32_SHIFT                       (7U)
#define MCG_C4_DMX32(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_C4_DMX32_SHIFT)) & MCG_C4_DMX32_MASK)

/*! @name C5 - MCG Control 5 Register */
#define MCG_C5_PLLSTEN0_MASK                     (0x20U)
#define MCG_C5_PLLSTEN0_SHIFT                    (5U)
#define MCG_C5_PLLSTEN0(x)                       (((uint8_t)(((uint8_t)(x)) << MCG_C5_PLLSTEN0_SHIFT)) & MCG_C5_PLLSTEN0_MASK)
#define MCG_C5_PLLCLKEN0_MASK                    (0x40U)
#define MCG_C5_PLLCLKEN0_SHIFT                   (6U)
#define MCG_C5_PLLCLKEN0(x)                      (((uint8_t)(((uint8_t)(x)) << MCG_C5_PLLCLKEN0_SHIFT)) & MCG_C5_PLLCLKEN0_MASK)

/*! @name C6 - MCG Control 6 Register */
#define MCG_C6_CHGPMP_BIAS_MASK                  (0x1FU)
#define MCG_C6_CHGPMP_BIAS_SHIFT                 (0U)
#define MCG_C6_CHGPMP_BIAS(x)                    (((uint8_t)(((uint8_t)(x)) << MCG_C6_CHGPMP_BIAS_SHIFT)) & MCG_C6_CHGPMP_BIAS_MASK)
#define MCG_C6_CME0_MASK                         (0x20U)
#define MCG_C6_CME0_SHIFT                        (5U)
#define MCG_C6_CME0(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_C6_CME0_SHIFT)) & MCG_C6_CME0_MASK)
#define MCG_C6_PLLS_MASK                         (0x40U)
#define MCG_C6_PLLS_SHIFT                        (6U)
#define MCG_C6_PLLS(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_C6_PLLS_SHIFT)) & MCG_C6_PLLS_MASK)
#define MCG_C6_LOLIE0_MASK                       (0x80U)
#define MCG_C6_LOLIE0_SHIFT                      (7U)
#define MCG_C6_LOLIE0(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C6_LOLIE0_SHIFT)) & MCG_C6_LOLIE0_MASK)

/*! @name S - MCG Status Register */
#define MCG_S_IRCST_MASK                         (0x1U)
#define MCG_S_IRCST_SHIFT                        (0U)
#define MCG_S_IRCST(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_S_IRCST_SHIFT)) & MCG_S_IRCST_MASK)
#define MCG_S_OSCINIT0_MASK                      (0x2U)
#define MCG_S_OSCINIT0_SHIFT                     (1U)
#define MCG_S_OSCINIT0(x)                        (((uint8_t)(((uint8_t)(x)) << MCG_S_OSCINIT0_SHIFT)) & MCG_S_OSCINIT0_MASK)
#define MCG_S_CLKST_MASK                         (0xCU)
#define MCG_S_CLKST_SHIFT                        (2U)
#define MCG_S_CLKST(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_S_CLKST_SHIFT)) & MCG_S_CLKST_MASK)
#define MCG_S_IREFST_MASK                        (0x10U)
#define MCG_S_IREFST_SHIFT                       (4U)
#define MCG_S_IREFST(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_S_IREFST_SHIFT)) & MCG_S_IREFST_MASK)
#define MCG_S_PLLST_MASK                         (0x20U)
#define MCG_S_PLLST_SHIFT                        (5U)
#define MCG_S_PLLST(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_S_PLLST_SHIFT)) & MCG_S_PLLST_MASK)
#define MCG_S_LOCK0_MASK                         (0x40U)
#define MCG_S_LOCK0_SHIFT                        (6U)
#define MCG_S_LOCK0(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_S_LOCK0_SHIFT)) & MCG_S_LOCK0_MASK)
#define MCG_S_LOLS0_MASK                         (0x80U)
#define MCG_S_LOLS0_SHIFT                        (7U)
#define MCG_S_LOLS0(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_S_LOLS0_SHIFT)) & MCG_S_LOLS0_MASK)

/*! @name SC - MCG Status and Control Register */
#define MCG_SC_LOCS0_MASK                        (0x1U)
#define MCG_SC_LOCS0_SHIFT                       (0U)
#define MCG_SC_LOCS0(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_SC_LOCS0_SHIFT)) & MCG_SC_LOCS0_MASK)
#define MCG_SC_FCRDIV_MASK                       (0xEU)
#define MCG_SC_FCRDIV_SHIFT                      (1U)
#define MCG_SC_FCRDIV(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_SC_FCRDIV_SHIFT)) & MCG_SC_FCRDIV_MASK)
#define MCG_SC_FLTPRSRV_MASK                     (0x10U)
#define MCG_SC_FLTPRSRV_SHIFT                    (4U)
#define MCG_SC_FLTPRSRV(x)                       (((uint8_t)(((uint8_t)(x)) << MCG_SC_FLTPRSRV_SHIFT)) & MCG_SC_FLTPRSRV_MASK)
#define MCG_SC_ATMF_MASK                         (0x20U)
#define MCG_SC_ATMF_SHIFT                        (5U)
#define MCG_SC_ATMF(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_SC_ATMF_SHIFT)) & MCG_SC_ATMF_MASK)
#define MCG_SC_ATMS_MASK                         (0x40U)
#define MCG_SC_ATMS_SHIFT                        (6U)
#define MCG_SC_ATMS(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_SC_ATMS_SHIFT)) & MCG_SC_ATMS_MASK)
#define MCG_SC_ATME_MASK                         (0x80U)
#define MCG_SC_ATME_SHIFT                        (7U)
#define MCG_SC_ATME(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_SC_ATME_SHIFT)) & MCG_SC_ATME_MASK)

/*! @name ATCVH - MCG Auto Trim Compare Value High Register */
#define MCG_ATCVH_ATCVH_MASK                     (0xFFU)
#define MCG_ATCVH_ATCVH_SHIFT                    (0U)
#define MCG_ATCVH_ATCVH(x)                       (((uint8_t)(((uint8_t)(x)) << MCG_ATCVH_ATCVH_SHIFT)) & MCG_ATCVH_ATCVH_MASK)

/*! @name ATCVL - MCG Auto Trim Compare Value Low Register */
#define MCG_ATCVL_ATCVL_MASK                     (0xFFU)
#define MCG_ATCVL_ATCVL_SHIFT                    (0U)
#define MCG_ATCVL_ATCVL(x)                       (((uint8_t)(((uint8_t)(x)) << MCG_ATCVL_ATCVL_SHIFT)) & MCG_ATCVL_ATCVL_MASK)

/*! @name C7 - MCG Control 7 Register */
#define MCG_C7_OSCSEL_MASK                       (0x1U)
#define MCG_C7_OSCSEL_SHIFT                      (0U)
#define MCG_C7_OSCSEL(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C7_OSCSEL_SHIFT)) & MCG_C7_OSCSEL_MASK)
#define MCG_C7_PLL32KREFSEL_MASK                 (0xC0U)
#define MCG_C7_PLL32KREFSEL_SHIFT                (6U)
#define MCG_C7_PLL32KREFSEL(x)                   (((uint8_t)(((uint8_t)(x)) << MCG_C7_PLL32KREFSEL_SHIFT)) & MCG_C7_PLL32KREFSEL_MASK)

/*! @name C8 - MCG Control 8 Register */
#define MCG_C8_LOCS1_MASK                        (0x1U)
#define MCG_C8_LOCS1_SHIFT                       (0U)
#define MCG_C8_LOCS1(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_C8_LOCS1_SHIFT)) & MCG_C8_LOCS1_MASK)
#define MCG_C8_COARSE_LOLIE_MASK                 (0x10U)
#define MCG_C8_COARSE_LOLIE_SHIFT                (4U)
#define MCG_C8_COARSE_LOLIE(x)                   (((uint8_t)(((uint8_t)(x)) << MCG_C8_COARSE_LOLIE_SHIFT)) & MCG_C8_COARSE_LOLIE_MASK)
#define MCG_C8_CME1_MASK                         (0x20U)
#define MCG_C8_CME1_SHIFT                        (5U)
#define MCG_C8_CME1(x)                           (((uint8_t)(((uint8_t)(x)) << MCG_C8_CME1_SHIFT)) & MCG_C8_CME1_MASK)
#define MCG_C8_LOLRE_MASK                        (0x40U)
#define MCG_C8_LOLRE_SHIFT                       (6U)
#define MCG_C8_LOLRE(x)                          (((uint8_t)(((uint8_t)(x)) << MCG_C8_LOLRE_SHIFT)) & MCG_C8_LOLRE_MASK)
#define MCG_C8_LOCRE1_MASK                       (0x80U)
#define MCG_C8_LOCRE1_SHIFT                      (7U)
#define MCG_C8_LOCRE1(x)                         (((uint8_t)(((uint8_t)(x)) << MCG_C8_LOCRE1_SHIFT)) & MCG_C8_LOCRE1_MASK)

/*! @name C9 - MCG Control 9 Register */
#define MCG_C9_COARSE_LOCK_MASK                  (0x40U)
#define MCG_C9_COARSE_LOCK_SHIFT                 (6U)
#define MCG_C9_COARSE_LOCK(x)                    (((uint8_t)(((uint8_t)(x)) << MCG_C9_COARSE_LOCK_SHIFT)) & MCG_C9_COARSE_LOCK_MASK)
#define MCG_C9_COARSE_LOLS_MASK                  (0x80U)
#define MCG_C9_COARSE_LOLS_SHIFT                 (7U)
#define MCG_C9_COARSE_LOLS(x)                    (((uint8_t)(((uint8_t)(x)) << MCG_C9_COARSE_LOLS_SHIFT)) & MCG_C9_COARSE_LOLS_MASK)


/*!
 * @}
 */ /* end of group MCG_Register_Masks */


/* MCG - Peripheral instance base addresses */
/** Peripheral MCG base address */
#define MCG_BASE                                 (0x40064000u)
/** Peripheral MCG base pointer */
#define MCG                                      ((MCG_Type *)MCG_BASE)
/** Array initializer of MCG peripheral base addresses */
#define MCG_BASE_ADDRS                           { MCG_BASE }
/** Array initializer of MCG peripheral base pointers */
#define MCG_BASE_PTRS                            { MCG }
/** Interrupt vectors for the MCG peripheral type */
#define MCG_IRQS                                 { MCG_IRQn }
/* MCG C2[EREFS] backward compatibility */
#define MCG_C2_EREFS_MASK         (MCG_C2_EREFS0_MASK)
#define MCG_C2_EREFS_SHIFT        (MCG_C2_EREFS0_SHIFT)
#define MCG_C2_EREFS_WIDTH        (MCG_C2_EREFS0_WIDTH)
#define MCG_C2_EREFS(x)           (MCG_C2_EREFS0(x))

/* MCG C2[HGO] backward compatibility */
#define MCG_C2_HGO_MASK         (MCG_C2_HGO0_MASK)
#define MCG_C2_HGO_SHIFT        (MCG_C2_HGO0_SHIFT)
#define MCG_C2_HGO_WIDTH        (MCG_C2_HGO0_WIDTH)
#define MCG_C2_HGO(x)           (MCG_C2_HGO0(x))

/* MCG C2[RANGE] backward compatibility */
#define MCG_C2_RANGE_MASK         (MCG_C2_RANGE0_MASK)
#define MCG_C2_RANGE_SHIFT        (MCG_C2_RANGE0_SHIFT)
#define MCG_C2_RANGE_WIDTH        (MCG_C2_RANGE0_WIDTH)
#define MCG_C2_RANGE(x)           (MCG_C2_RANGE0(x))


/*!
 * @}
 */ /* end of group MCG_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MCM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Peripheral_Access_Layer MCM Peripheral Access Layer
 * @{
 */

/** MCM - Register Layout Typedef */
typedef struct {
       uint8_t RESERVED_0[8];
  __I  uint16_t PLASC;                             /**< Crossbar Switch (AXBS) Slave Configuration, offset: 0x8 */
  __I  uint16_t PLAMC;                             /**< Crossbar Switch (AXBS) Master Configuration, offset: 0xA */
  __IO uint32_t PLACR;                             /**< Platform Control Register, offset: 0xC */
       uint8_t RESERVED_1[32];
  __IO uint32_t PID;                               /**< Process ID register, offset: 0x30 */
       uint8_t RESERVED_2[12];
  __IO uint32_t CPO;                               /**< Compute Operation Control Register, offset: 0x40 */
       uint8_t RESERVED_3[60];
  __IO uint32_t MATCR[1];                          /**< Master Attribute Configuration Register, array offset: 0x80, array step: 0x4 */
} MCM_Type;

/* ----------------------------------------------------------------------------
   -- MCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MCM_Register_Masks MCM Register Masks
 * @{
 */

/*! @name PLASC - Crossbar Switch (AXBS) Slave Configuration */
#define MCM_PLASC_ASC_MASK                       (0xFFU)
#define MCM_PLASC_ASC_SHIFT                      (0U)
#define MCM_PLASC_ASC(x)                         (((uint16_t)(((uint16_t)(x)) << MCM_PLASC_ASC_SHIFT)) & MCM_PLASC_ASC_MASK)

/*! @name PLAMC - Crossbar Switch (AXBS) Master Configuration */
#define MCM_PLAMC_AMC_MASK                       (0xFFU)
#define MCM_PLAMC_AMC_SHIFT                      (0U)
#define MCM_PLAMC_AMC(x)                         (((uint16_t)(((uint16_t)(x)) << MCM_PLAMC_AMC_SHIFT)) & MCM_PLAMC_AMC_MASK)

/*! @name PLACR - Platform Control Register */
#define MCM_PLACR_ARB_MASK                       (0x200U)
#define MCM_PLACR_ARB_SHIFT                      (9U)
#define MCM_PLACR_ARB(x)                         (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_ARB_SHIFT)) & MCM_PLACR_ARB_MASK)
#define MCM_PLACR_CFCC_MASK                      (0x400U)
#define MCM_PLACR_CFCC_SHIFT                     (10U)
#define MCM_PLACR_CFCC(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_CFCC_SHIFT)) & MCM_PLACR_CFCC_MASK)
#define MCM_PLACR_DFCDA_MASK                     (0x800U)
#define MCM_PLACR_DFCDA_SHIFT                    (11U)
#define MCM_PLACR_DFCDA(x)                       (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_DFCDA_SHIFT)) & MCM_PLACR_DFCDA_MASK)
#define MCM_PLACR_DFCIC_MASK                     (0x1000U)
#define MCM_PLACR_DFCIC_SHIFT                    (12U)
#define MCM_PLACR_DFCIC(x)                       (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_DFCIC_SHIFT)) & MCM_PLACR_DFCIC_MASK)
#define MCM_PLACR_DFCC_MASK                      (0x2000U)
#define MCM_PLACR_DFCC_SHIFT                     (13U)
#define MCM_PLACR_DFCC(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_DFCC_SHIFT)) & MCM_PLACR_DFCC_MASK)
#define MCM_PLACR_EFDS_MASK                      (0x4000U)
#define MCM_PLACR_EFDS_SHIFT                     (14U)
#define MCM_PLACR_EFDS(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_EFDS_SHIFT)) & MCM_PLACR_EFDS_MASK)
#define MCM_PLACR_DFCS_MASK                      (0x8000U)
#define MCM_PLACR_DFCS_SHIFT                     (15U)
#define MCM_PLACR_DFCS(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_DFCS_SHIFT)) & MCM_PLACR_DFCS_MASK)
#define MCM_PLACR_ESFC_MASK                      (0x10000U)
#define MCM_PLACR_ESFC_SHIFT                     (16U)
#define MCM_PLACR_ESFC(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_PLACR_ESFC_SHIFT)) & MCM_PLACR_ESFC_MASK)

/*! @name PID - Process ID register */
#define MCM_PID_PID_MASK                         (0xFFU)
#define MCM_PID_PID_SHIFT                        (0U)
#define MCM_PID_PID(x)                           (((uint32_t)(((uint32_t)(x)) << MCM_PID_PID_SHIFT)) & MCM_PID_PID_MASK)

/*! @name CPO - Compute Operation Control Register */
#define MCM_CPO_CPOREQ_MASK                      (0x1U)
#define MCM_CPO_CPOREQ_SHIFT                     (0U)
#define MCM_CPO_CPOREQ(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_CPO_CPOREQ_SHIFT)) & MCM_CPO_CPOREQ_MASK)
#define MCM_CPO_CPOACK_MASK                      (0x2U)
#define MCM_CPO_CPOACK_SHIFT                     (1U)
#define MCM_CPO_CPOACK(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_CPO_CPOACK_SHIFT)) & MCM_CPO_CPOACK_MASK)
#define MCM_CPO_CPOWOI_MASK                      (0x4U)
#define MCM_CPO_CPOWOI_SHIFT                     (2U)
#define MCM_CPO_CPOWOI(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_CPO_CPOWOI_SHIFT)) & MCM_CPO_CPOWOI_MASK)

/*! @name MATCR - Master Attribute Configuration Register */
#define MCM_MATCR_ATC0_MASK                      (0x7U)
#define MCM_MATCR_ATC0_SHIFT                     (0U)
#define MCM_MATCR_ATC0(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_MATCR_ATC0_SHIFT)) & MCM_MATCR_ATC0_MASK)
#define MCM_MATCR_RO0_MASK                       (0x80U)
#define MCM_MATCR_RO0_SHIFT                      (7U)
#define MCM_MATCR_RO0(x)                         (((uint32_t)(((uint32_t)(x)) << MCM_MATCR_RO0_SHIFT)) & MCM_MATCR_RO0_MASK)
#define MCM_MATCR_ATC2_MASK                      (0x70000U)
#define MCM_MATCR_ATC2_SHIFT                     (16U)
#define MCM_MATCR_ATC2(x)                        (((uint32_t)(((uint32_t)(x)) << MCM_MATCR_ATC2_SHIFT)) & MCM_MATCR_ATC2_MASK)
#define MCM_MATCR_RO2_MASK                       (0x800000U)
#define MCM_MATCR_RO2_SHIFT                      (23U)
#define MCM_MATCR_RO2(x)                         (((uint32_t)(((uint32_t)(x)) << MCM_MATCR_RO2_SHIFT)) & MCM_MATCR_RO2_MASK)

/* The count of MCM_MATCR */
#define MCM_MATCR_COUNT                          (1U)


/*!
 * @}
 */ /* end of group MCM_Register_Masks */


/* MCM - Peripheral instance base addresses */
/** Peripheral MCM base address */
#define MCM_BASE                                 (0xF0003000u)
/** Peripheral MCM base pointer */
#define MCM                                      ((MCM_Type *)MCM_BASE)
/** Array initializer of MCM peripheral base addresses */
#define MCM_BASE_ADDRS                           { MCM_BASE }
/** Array initializer of MCM peripheral base pointers */
#define MCM_BASE_PTRS                            { MCM }

/*!
 * @}
 */ /* end of group MCM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MMAU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MMAU_Peripheral_Access_Layer MMAU Peripheral Access Layer
 * @{
 */

/** MMAU - Register Layout Typedef */
typedef struct {
  __IO uint32_t X0;                                /**< Operand Register X0, offset: 0x0 */
  __IO uint32_t X1;                                /**< Operand Register X1, offset: 0x4 */
  __IO uint32_t X2;                                /**< Operand Register X2, offset: 0x8 */
  __IO uint32_t X3;                                /**< Operand Register X3, offset: 0xC */
  __IO uint32_t A0;                                /**< Accumulator Register A0, offset: 0x10 */
  __IO uint32_t A1;                                /**< Accumulator Register A1, offset: 0x14 */
  __IO uint32_t CSR;                               /**< Control/Status Register, offset: 0x18 */
  __IO uint32_t CSR_IF_CLR;                        /**< CSR Interrupt Flags Clearance Register, offset: 0x1C */
} MMAU_Type;

/* ----------------------------------------------------------------------------
   -- MMAU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MMAU_Register_Masks MMAU Register Masks
 * @{
 */

/*! @name X0 - Operand Register X0 */
#define MMAU_X0_X0_MASK                          (0xFFFFFFFFU)
#define MMAU_X0_X0_SHIFT                         (0U)
#define MMAU_X0_X0(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_X0_X0_SHIFT)) & MMAU_X0_X0_MASK)

/*! @name X1 - Operand Register X1 */
#define MMAU_X1_X1_MASK                          (0xFFFFFFFFU)
#define MMAU_X1_X1_SHIFT                         (0U)
#define MMAU_X1_X1(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_X1_X1_SHIFT)) & MMAU_X1_X1_MASK)

/*! @name X2 - Operand Register X2 */
#define MMAU_X2_X2_MASK                          (0xFFFFFFFFU)
#define MMAU_X2_X2_SHIFT                         (0U)
#define MMAU_X2_X2(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_X2_X2_SHIFT)) & MMAU_X2_X2_MASK)

/*! @name X3 - Operand Register X3 */
#define MMAU_X3_X3_MASK                          (0xFFFFFFFFU)
#define MMAU_X3_X3_SHIFT                         (0U)
#define MMAU_X3_X3(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_X3_X3_SHIFT)) & MMAU_X3_X3_MASK)

/*! @name A0 - Accumulator Register A0 */
#define MMAU_A0_A0_MASK                          (0xFFFFFFFFU)
#define MMAU_A0_A0_SHIFT                         (0U)
#define MMAU_A0_A0(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_A0_A0_SHIFT)) & MMAU_A0_A0_MASK)

/*! @name A1 - Accumulator Register A1 */
#define MMAU_A1_A1_MASK                          (0xFFFFFFFFU)
#define MMAU_A1_A1_SHIFT                         (0U)
#define MMAU_A1_A1(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_A1_A1_SHIFT)) & MMAU_A1_A1_MASK)

/*! @name CSR - Control/Status Register */
#define MMAU_CSR_Q_MASK                          (0x1U)
#define MMAU_CSR_Q_SHIFT                         (0U)
#define MMAU_CSR_Q(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_Q_SHIFT)) & MMAU_CSR_Q_MASK)
#define MMAU_CSR_V_MASK                          (0x2U)
#define MMAU_CSR_V_SHIFT                         (1U)
#define MMAU_CSR_V(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_V_SHIFT)) & MMAU_CSR_V_MASK)
#define MMAU_CSR_DZ_MASK                         (0x4U)
#define MMAU_CSR_DZ_SHIFT                        (2U)
#define MMAU_CSR_DZ(x)                           (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_DZ_SHIFT)) & MMAU_CSR_DZ_MASK)
#define MMAU_CSR_N_MASK                          (0x8U)
#define MMAU_CSR_N_SHIFT                         (3U)
#define MMAU_CSR_N(x)                            (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_N_SHIFT)) & MMAU_CSR_N_MASK)
#define MMAU_CSR_QIF_MASK                        (0x10U)
#define MMAU_CSR_QIF_SHIFT                       (4U)
#define MMAU_CSR_QIF(x)                          (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_QIF_SHIFT)) & MMAU_CSR_QIF_MASK)
#define MMAU_CSR_VIF_MASK                        (0x20U)
#define MMAU_CSR_VIF_SHIFT                       (5U)
#define MMAU_CSR_VIF(x)                          (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_VIF_SHIFT)) & MMAU_CSR_VIF_MASK)
#define MMAU_CSR_DZIF_MASK                       (0x40U)
#define MMAU_CSR_DZIF_SHIFT                      (6U)
#define MMAU_CSR_DZIF(x)                         (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_DZIF_SHIFT)) & MMAU_CSR_DZIF_MASK)
#define MMAU_CSR_QIE_MASK                        (0x1000U)
#define MMAU_CSR_QIE_SHIFT                       (12U)
#define MMAU_CSR_QIE(x)                          (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_QIE_SHIFT)) & MMAU_CSR_QIE_MASK)
#define MMAU_CSR_VIE_MASK                        (0x2000U)
#define MMAU_CSR_VIE_SHIFT                       (13U)
#define MMAU_CSR_VIE(x)                          (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_VIE_SHIFT)) & MMAU_CSR_VIE_MASK)
#define MMAU_CSR_DZIE_MASK                       (0x4000U)
#define MMAU_CSR_DZIE_SHIFT                      (14U)
#define MMAU_CSR_DZIE(x)                         (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_DZIE_SHIFT)) & MMAU_CSR_DZIE_MASK)
#define MMAU_CSR_DRE_MASK                        (0x10000U)
#define MMAU_CSR_DRE_SHIFT                       (16U)
#define MMAU_CSR_DRE(x)                          (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_DRE_SHIFT)) & MMAU_CSR_DRE_MASK)
#define MMAU_CSR_SO_MASK                         (0x20000U)
#define MMAU_CSR_SO_SHIFT                        (17U)
#define MMAU_CSR_SO(x)                           (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_SO_SHIFT)) & MMAU_CSR_SO_MASK)
#define MMAU_CSR_HDR_MASK                        (0xF00000U)
#define MMAU_CSR_HDR_SHIFT                       (20U)
#define MMAU_CSR_HDR(x)                          (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_HDR_SHIFT)) & MMAU_CSR_HDR_MASK)
#define MMAU_CSR_BUSY_MASK                       (0x80000000U)
#define MMAU_CSR_BUSY_SHIFT                      (31U)
#define MMAU_CSR_BUSY(x)                         (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_BUSY_SHIFT)) & MMAU_CSR_BUSY_MASK)

/*! @name CSR_IF_CLR - CSR Interrupt Flags Clearance Register */
#define MMAU_CSR_IF_CLR_Q_MASK                   (0x1U)
#define MMAU_CSR_IF_CLR_Q_SHIFT                  (0U)
#define MMAU_CSR_IF_CLR_Q(x)                     (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_Q_SHIFT)) & MMAU_CSR_IF_CLR_Q_MASK)
#define MMAU_CSR_IF_CLR_V_MASK                   (0x2U)
#define MMAU_CSR_IF_CLR_V_SHIFT                  (1U)
#define MMAU_CSR_IF_CLR_V(x)                     (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_V_SHIFT)) & MMAU_CSR_IF_CLR_V_MASK)
#define MMAU_CSR_IF_CLR_DZ_MASK                  (0x4U)
#define MMAU_CSR_IF_CLR_DZ_SHIFT                 (2U)
#define MMAU_CSR_IF_CLR_DZ(x)                    (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_DZ_SHIFT)) & MMAU_CSR_IF_CLR_DZ_MASK)
#define MMAU_CSR_IF_CLR_N_MASK                   (0x8U)
#define MMAU_CSR_IF_CLR_N_SHIFT                  (3U)
#define MMAU_CSR_IF_CLR_N(x)                     (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_N_SHIFT)) & MMAU_CSR_IF_CLR_N_MASK)
#define MMAU_CSR_IF_CLR_QIF_MASK                 (0x10U)
#define MMAU_CSR_IF_CLR_QIF_SHIFT                (4U)
#define MMAU_CSR_IF_CLR_QIF(x)                   (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_QIF_SHIFT)) & MMAU_CSR_IF_CLR_QIF_MASK)
#define MMAU_CSR_IF_CLR_VIF_MASK                 (0x20U)
#define MMAU_CSR_IF_CLR_VIF_SHIFT                (5U)
#define MMAU_CSR_IF_CLR_VIF(x)                   (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_VIF_SHIFT)) & MMAU_CSR_IF_CLR_VIF_MASK)
#define MMAU_CSR_IF_CLR_DZIF_MASK                (0x40U)
#define MMAU_CSR_IF_CLR_DZIF_SHIFT               (6U)
#define MMAU_CSR_IF_CLR_DZIF(x)                  (((uint32_t)(((uint32_t)(x)) << MMAU_CSR_IF_CLR_DZIF_SHIFT)) & MMAU_CSR_IF_CLR_DZIF_MASK)


/*!
 * @}
 */ /* end of group MMAU_Register_Masks */


/* MMAU - Peripheral instance base addresses */
/** Peripheral MMAU base address */
#define MMAU_BASE                                (0xF0004000u)
/** Peripheral MMAU base pointer */
#define MMAU                                     ((MMAU_Type *)MMAU_BASE)
/** Array initializer of MMAU peripheral base addresses */
#define MMAU_BASE_ADDRS                          { MMAU_BASE }
/** Array initializer of MMAU peripheral base pointers */
#define MMAU_BASE_PTRS                           { MMAU }
/** Interrupt vectors for the MMAU peripheral type */
#define MMAU_IRQS                                { MMAU_IRQn }

/*!
 * @}
 */ /* end of group MMAU_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MTB Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Peripheral_Access_Layer MTB Peripheral Access Layer
 * @{
 */

/** MTB - Register Layout Typedef */
typedef struct {
  __IO uint32_t POSITION;                          /**< MTB Position Register, offset: 0x0 */
  __IO uint32_t MASTER;                            /**< MTB Master Register, offset: 0x4 */
  __IO uint32_t FLOW;                              /**< MTB Flow Register, offset: 0x8 */
  __I  uint32_t BASE;                              /**< MTB Base Register, offset: 0xC */
       uint8_t RESERVED_0[3824];
  __I  uint32_t MODECTRL;                          /**< Integration Mode Control Register, offset: 0xF00 */
       uint8_t RESERVED_1[156];
  __I  uint32_t TAGSET;                            /**< Claim TAG Set Register, offset: 0xFA0 */
  __I  uint32_t TAGCLEAR;                          /**< Claim TAG Clear Register, offset: 0xFA4 */
       uint8_t RESERVED_2[8];
  __I  uint32_t LOCKACCESS;                        /**< Lock Access Register, offset: 0xFB0 */
  __I  uint32_t LOCKSTAT;                          /**< Lock Status Register, offset: 0xFB4 */
  __I  uint32_t AUTHSTAT;                          /**< Authentication Status Register, offset: 0xFB8 */
  __I  uint32_t DEVICEARCH;                        /**< Device Architecture Register, offset: 0xFBC */
       uint8_t RESERVED_3[8];
  __I  uint32_t DEVICECFG;                         /**< Device Configuration Register, offset: 0xFC8 */
  __I  uint32_t DEVICETYPID;                       /**< Device Type Identifier Register, offset: 0xFCC */
  __I  uint32_t PERIPHID4;                         /**< Peripheral ID Register, offset: 0xFD0 */
  __I  uint32_t PERIPHID5;                         /**< Peripheral ID Register, offset: 0xFD4 */
  __I  uint32_t PERIPHID6;                         /**< Peripheral ID Register, offset: 0xFD8 */
  __I  uint32_t PERIPHID7;                         /**< Peripheral ID Register, offset: 0xFDC */
  __I  uint32_t PERIPHID0;                         /**< Peripheral ID Register, offset: 0xFE0 */
  __I  uint32_t PERIPHID1;                         /**< Peripheral ID Register, offset: 0xFE4 */
  __I  uint32_t PERIPHID2;                         /**< Peripheral ID Register, offset: 0xFE8 */
  __I  uint32_t PERIPHID3;                         /**< Peripheral ID Register, offset: 0xFEC */
  __I  uint32_t COMPID[4];                         /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} MTB_Type;

/* ----------------------------------------------------------------------------
   -- MTB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTB_Register_Masks MTB Register Masks
 * @{
 */

/*! @name POSITION - MTB Position Register */
#define MTB_POSITION_WRAP_MASK                   (0x4U)
#define MTB_POSITION_WRAP_SHIFT                  (2U)
#define MTB_POSITION_WRAP(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_POSITION_WRAP_SHIFT)) & MTB_POSITION_WRAP_MASK)
#define MTB_POSITION_POINTER_MASK                (0xFFFFFFF8U)
#define MTB_POSITION_POINTER_SHIFT               (3U)
#define MTB_POSITION_POINTER(x)                  (((uint32_t)(((uint32_t)(x)) << MTB_POSITION_POINTER_SHIFT)) & MTB_POSITION_POINTER_MASK)

/*! @name MASTER - MTB Master Register */
#define MTB_MASTER_MASK_MASK                     (0x1FU)
#define MTB_MASTER_MASK_SHIFT                    (0U)
#define MTB_MASTER_MASK(x)                       (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_MASK_SHIFT)) & MTB_MASTER_MASK_MASK)
#define MTB_MASTER_TSTARTEN_MASK                 (0x20U)
#define MTB_MASTER_TSTARTEN_SHIFT                (5U)
#define MTB_MASTER_TSTARTEN(x)                   (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_TSTARTEN_SHIFT)) & MTB_MASTER_TSTARTEN_MASK)
#define MTB_MASTER_TSTOPEN_MASK                  (0x40U)
#define MTB_MASTER_TSTOPEN_SHIFT                 (6U)
#define MTB_MASTER_TSTOPEN(x)                    (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_TSTOPEN_SHIFT)) & MTB_MASTER_TSTOPEN_MASK)
#define MTB_MASTER_SFRWPRIV_MASK                 (0x80U)
#define MTB_MASTER_SFRWPRIV_SHIFT                (7U)
#define MTB_MASTER_SFRWPRIV(x)                   (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_SFRWPRIV_SHIFT)) & MTB_MASTER_SFRWPRIV_MASK)
#define MTB_MASTER_RAMPRIV_MASK                  (0x100U)
#define MTB_MASTER_RAMPRIV_SHIFT                 (8U)
#define MTB_MASTER_RAMPRIV(x)                    (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_RAMPRIV_SHIFT)) & MTB_MASTER_RAMPRIV_MASK)
#define MTB_MASTER_HALTREQ_MASK                  (0x200U)
#define MTB_MASTER_HALTREQ_SHIFT                 (9U)
#define MTB_MASTER_HALTREQ(x)                    (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_HALTREQ_SHIFT)) & MTB_MASTER_HALTREQ_MASK)
#define MTB_MASTER_EN_MASK                       (0x80000000U)
#define MTB_MASTER_EN_SHIFT                      (31U)
#define MTB_MASTER_EN(x)                         (((uint32_t)(((uint32_t)(x)) << MTB_MASTER_EN_SHIFT)) & MTB_MASTER_EN_MASK)

/*! @name FLOW - MTB Flow Register */
#define MTB_FLOW_AUTOSTOP_MASK                   (0x1U)
#define MTB_FLOW_AUTOSTOP_SHIFT                  (0U)
#define MTB_FLOW_AUTOSTOP(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_FLOW_AUTOSTOP_SHIFT)) & MTB_FLOW_AUTOSTOP_MASK)
#define MTB_FLOW_AUTOHALT_MASK                   (0x2U)
#define MTB_FLOW_AUTOHALT_SHIFT                  (1U)
#define MTB_FLOW_AUTOHALT(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_FLOW_AUTOHALT_SHIFT)) & MTB_FLOW_AUTOHALT_MASK)
#define MTB_FLOW_WATERMARK_MASK                  (0xFFFFFFF8U)
#define MTB_FLOW_WATERMARK_SHIFT                 (3U)
#define MTB_FLOW_WATERMARK(x)                    (((uint32_t)(((uint32_t)(x)) << MTB_FLOW_WATERMARK_SHIFT)) & MTB_FLOW_WATERMARK_MASK)

/*! @name BASE - MTB Base Register */
#define MTB_BASE_BASEADDR_MASK                   (0xFFFFFFFFU)
#define MTB_BASE_BASEADDR_SHIFT                  (0U)
#define MTB_BASE_BASEADDR(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_BASE_BASEADDR_SHIFT)) & MTB_BASE_BASEADDR_MASK)

/*! @name MODECTRL - Integration Mode Control Register */
#define MTB_MODECTRL_MODECTRL_MASK               (0xFFFFFFFFU)
#define MTB_MODECTRL_MODECTRL_SHIFT              (0U)
#define MTB_MODECTRL_MODECTRL(x)                 (((uint32_t)(((uint32_t)(x)) << MTB_MODECTRL_MODECTRL_SHIFT)) & MTB_MODECTRL_MODECTRL_MASK)

/*! @name TAGSET - Claim TAG Set Register */
#define MTB_TAGSET_TAGSET_MASK                   (0xFFFFFFFFU)
#define MTB_TAGSET_TAGSET_SHIFT                  (0U)
#define MTB_TAGSET_TAGSET(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_TAGSET_TAGSET_SHIFT)) & MTB_TAGSET_TAGSET_MASK)

/*! @name TAGCLEAR - Claim TAG Clear Register */
#define MTB_TAGCLEAR_TAGCLEAR_MASK               (0xFFFFFFFFU)
#define MTB_TAGCLEAR_TAGCLEAR_SHIFT              (0U)
#define MTB_TAGCLEAR_TAGCLEAR(x)                 (((uint32_t)(((uint32_t)(x)) << MTB_TAGCLEAR_TAGCLEAR_SHIFT)) & MTB_TAGCLEAR_TAGCLEAR_MASK)

/*! @name LOCKACCESS - Lock Access Register */
#define MTB_LOCKACCESS_LOCKACCESS_MASK           (0xFFFFFFFFU)
#define MTB_LOCKACCESS_LOCKACCESS_SHIFT          (0U)
#define MTB_LOCKACCESS_LOCKACCESS(x)             (((uint32_t)(((uint32_t)(x)) << MTB_LOCKACCESS_LOCKACCESS_SHIFT)) & MTB_LOCKACCESS_LOCKACCESS_MASK)

/*! @name LOCKSTAT - Lock Status Register */
#define MTB_LOCKSTAT_LOCKSTAT_MASK               (0xFFFFFFFFU)
#define MTB_LOCKSTAT_LOCKSTAT_SHIFT              (0U)
#define MTB_LOCKSTAT_LOCKSTAT(x)                 (((uint32_t)(((uint32_t)(x)) << MTB_LOCKSTAT_LOCKSTAT_SHIFT)) & MTB_LOCKSTAT_LOCKSTAT_MASK)

/*! @name AUTHSTAT - Authentication Status Register */
#define MTB_AUTHSTAT_BIT0_MASK                   (0x1U)
#define MTB_AUTHSTAT_BIT0_SHIFT                  (0U)
#define MTB_AUTHSTAT_BIT0(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_AUTHSTAT_BIT0_SHIFT)) & MTB_AUTHSTAT_BIT0_MASK)
#define MTB_AUTHSTAT_BIT1_MASK                   (0x2U)
#define MTB_AUTHSTAT_BIT1_SHIFT                  (1U)
#define MTB_AUTHSTAT_BIT1(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_AUTHSTAT_BIT1_SHIFT)) & MTB_AUTHSTAT_BIT1_MASK)
#define MTB_AUTHSTAT_BIT2_MASK                   (0x4U)
#define MTB_AUTHSTAT_BIT2_SHIFT                  (2U)
#define MTB_AUTHSTAT_BIT2(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_AUTHSTAT_BIT2_SHIFT)) & MTB_AUTHSTAT_BIT2_MASK)
#define MTB_AUTHSTAT_BIT3_MASK                   (0x8U)
#define MTB_AUTHSTAT_BIT3_SHIFT                  (3U)
#define MTB_AUTHSTAT_BIT3(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_AUTHSTAT_BIT3_SHIFT)) & MTB_AUTHSTAT_BIT3_MASK)

/*! @name DEVICEARCH - Device Architecture Register */
#define MTB_DEVICEARCH_DEVICEARCH_MASK           (0xFFFFFFFFU)
#define MTB_DEVICEARCH_DEVICEARCH_SHIFT          (0U)
#define MTB_DEVICEARCH_DEVICEARCH(x)             (((uint32_t)(((uint32_t)(x)) << MTB_DEVICEARCH_DEVICEARCH_SHIFT)) & MTB_DEVICEARCH_DEVICEARCH_MASK)

/*! @name DEVICECFG - Device Configuration Register */
#define MTB_DEVICECFG_DEVICECFG_MASK             (0xFFFFFFFFU)
#define MTB_DEVICECFG_DEVICECFG_SHIFT            (0U)
#define MTB_DEVICECFG_DEVICECFG(x)               (((uint32_t)(((uint32_t)(x)) << MTB_DEVICECFG_DEVICECFG_SHIFT)) & MTB_DEVICECFG_DEVICECFG_MASK)

/*! @name DEVICETYPID - Device Type Identifier Register */
#define MTB_DEVICETYPID_DEVICETYPID_MASK         (0xFFFFFFFFU)
#define MTB_DEVICETYPID_DEVICETYPID_SHIFT        (0U)
#define MTB_DEVICETYPID_DEVICETYPID(x)           (((uint32_t)(((uint32_t)(x)) << MTB_DEVICETYPID_DEVICETYPID_SHIFT)) & MTB_DEVICETYPID_DEVICETYPID_MASK)

/*! @name PERIPHID4 - Peripheral ID Register */
#define MTB_PERIPHID4_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID4_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID4_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID4_PERIPHID_SHIFT)) & MTB_PERIPHID4_PERIPHID_MASK)

/*! @name PERIPHID5 - Peripheral ID Register */
#define MTB_PERIPHID5_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID5_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID5_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID5_PERIPHID_SHIFT)) & MTB_PERIPHID5_PERIPHID_MASK)

/*! @name PERIPHID6 - Peripheral ID Register */
#define MTB_PERIPHID6_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID6_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID6_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID6_PERIPHID_SHIFT)) & MTB_PERIPHID6_PERIPHID_MASK)

/*! @name PERIPHID7 - Peripheral ID Register */
#define MTB_PERIPHID7_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID7_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID7_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID7_PERIPHID_SHIFT)) & MTB_PERIPHID7_PERIPHID_MASK)

/*! @name PERIPHID0 - Peripheral ID Register */
#define MTB_PERIPHID0_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID0_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID0_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID0_PERIPHID_SHIFT)) & MTB_PERIPHID0_PERIPHID_MASK)

/*! @name PERIPHID1 - Peripheral ID Register */
#define MTB_PERIPHID1_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID1_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID1_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID1_PERIPHID_SHIFT)) & MTB_PERIPHID1_PERIPHID_MASK)

/*! @name PERIPHID2 - Peripheral ID Register */
#define MTB_PERIPHID2_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID2_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID2_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID2_PERIPHID_SHIFT)) & MTB_PERIPHID2_PERIPHID_MASK)

/*! @name PERIPHID3 - Peripheral ID Register */
#define MTB_PERIPHID3_PERIPHID_MASK              (0xFFFFFFFFU)
#define MTB_PERIPHID3_PERIPHID_SHIFT             (0U)
#define MTB_PERIPHID3_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << MTB_PERIPHID3_PERIPHID_SHIFT)) & MTB_PERIPHID3_PERIPHID_MASK)

/*! @name COMPID - Component ID Register */
#define MTB_COMPID_COMPID_MASK                   (0xFFFFFFFFU)
#define MTB_COMPID_COMPID_SHIFT                  (0U)
#define MTB_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x)) << MTB_COMPID_COMPID_SHIFT)) & MTB_COMPID_COMPID_MASK)

/* The count of MTB_COMPID */
#define MTB_COMPID_COUNT                         (4U)


/*!
 * @}
 */ /* end of group MTB_Register_Masks */


/* MTB - Peripheral instance base addresses */
/** Peripheral MTB base address */
#define MTB_BASE                                 (0xF0000000u)
/** Peripheral MTB base pointer */
#define MTB                                      ((MTB_Type *)MTB_BASE)
/** Array initializer of MTB peripheral base addresses */
#define MTB_BASE_ADDRS                           { MTB_BASE }
/** Array initializer of MTB peripheral base pointers */
#define MTB_BASE_PTRS                            { MTB }

/*!
 * @}
 */ /* end of group MTB_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- MTBDWT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Peripheral_Access_Layer MTBDWT Peripheral Access Layer
 * @{
 */

/** MTBDWT - Register Layout Typedef */
typedef struct {
  __I  uint32_t CTRL;                              /**< MTB DWT Control Register, offset: 0x0 */
       uint8_t RESERVED_0[28];
  struct {                                         /* offset: 0x20, array step: 0x10 */
    __IO uint32_t COMP;                              /**< MTB_DWT Comparator Register, array offset: 0x20, array step: 0x10 */
    __IO uint32_t MASK;                              /**< MTB_DWT Comparator Mask Register, array offset: 0x24, array step: 0x10 */
    __IO uint32_t FCT;                               /**< MTB_DWT Comparator Function Register 0..MTB_DWT Comparator Function Register 1, array offset: 0x28, array step: 0x10 */
         uint8_t RESERVED_0[4];
  } COMPARATOR[2];
       uint8_t RESERVED_1[448];
  __IO uint32_t TBCTRL;                            /**< MTB_DWT Trace Buffer Control Register, offset: 0x200 */
       uint8_t RESERVED_2[3524];
  __I  uint32_t DEVICECFG;                         /**< Device Configuration Register, offset: 0xFC8 */
  __I  uint32_t DEVICETYPID;                       /**< Device Type Identifier Register, offset: 0xFCC */
  __I  uint32_t PERIPHID4;                         /**< Peripheral ID Register, offset: 0xFD0 */
  __I  uint32_t PERIPHID5;                         /**< Peripheral ID Register, offset: 0xFD4 */
  __I  uint32_t PERIPHID6;                         /**< Peripheral ID Register, offset: 0xFD8 */
  __I  uint32_t PERIPHID7;                         /**< Peripheral ID Register, offset: 0xFDC */
  __I  uint32_t PERIPHID0;                         /**< Peripheral ID Register, offset: 0xFE0 */
  __I  uint32_t PERIPHID1;                         /**< Peripheral ID Register, offset: 0xFE4 */
  __I  uint32_t PERIPHID2;                         /**< Peripheral ID Register, offset: 0xFE8 */
  __I  uint32_t PERIPHID3;                         /**< Peripheral ID Register, offset: 0xFEC */
  __I  uint32_t COMPID[4];                         /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} MTBDWT_Type;

/* ----------------------------------------------------------------------------
   -- MTBDWT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup MTBDWT_Register_Masks MTBDWT Register Masks
 * @{
 */

/*! @name CTRL - MTB DWT Control Register */
#define MTBDWT_CTRL_DWTCFGCTRL_MASK              (0xFFFFFFFU)
#define MTBDWT_CTRL_DWTCFGCTRL_SHIFT             (0U)
#define MTBDWT_CTRL_DWTCFGCTRL(x)                (((uint32_t)(((uint32_t)(x)) << MTBDWT_CTRL_DWTCFGCTRL_SHIFT)) & MTBDWT_CTRL_DWTCFGCTRL_MASK)
#define MTBDWT_CTRL_NUMCMP_MASK                  (0xF0000000U)
#define MTBDWT_CTRL_NUMCMP_SHIFT                 (28U)
#define MTBDWT_CTRL_NUMCMP(x)                    (((uint32_t)(((uint32_t)(x)) << MTBDWT_CTRL_NUMCMP_SHIFT)) & MTBDWT_CTRL_NUMCMP_MASK)

/*! @name COMP - MTB_DWT Comparator Register */
#define MTBDWT_COMP_COMP_MASK                    (0xFFFFFFFFU)
#define MTBDWT_COMP_COMP_SHIFT                   (0U)
#define MTBDWT_COMP_COMP(x)                      (((uint32_t)(((uint32_t)(x)) << MTBDWT_COMP_COMP_SHIFT)) & MTBDWT_COMP_COMP_MASK)

/* The count of MTBDWT_COMP */
#define MTBDWT_COMP_COUNT                        (2U)

/*! @name MASK - MTB_DWT Comparator Mask Register */
#define MTBDWT_MASK_MASK_MASK                    (0x1FU)
#define MTBDWT_MASK_MASK_SHIFT                   (0U)
#define MTBDWT_MASK_MASK(x)                      (((uint32_t)(((uint32_t)(x)) << MTBDWT_MASK_MASK_SHIFT)) & MTBDWT_MASK_MASK_MASK)

/* The count of MTBDWT_MASK */
#define MTBDWT_MASK_COUNT                        (2U)

/*! @name FCT - MTB_DWT Comparator Function Register 0..MTB_DWT Comparator Function Register 1 */
#define MTBDWT_FCT_FUNCTION_MASK                 (0xFU)
#define MTBDWT_FCT_FUNCTION_SHIFT                (0U)
#define MTBDWT_FCT_FUNCTION(x)                   (((uint32_t)(((uint32_t)(x)) << MTBDWT_FCT_FUNCTION_SHIFT)) & MTBDWT_FCT_FUNCTION_MASK)
#define MTBDWT_FCT_DATAVMATCH_MASK               (0x100U)
#define MTBDWT_FCT_DATAVMATCH_SHIFT              (8U)
#define MTBDWT_FCT_DATAVMATCH(x)                 (((uint32_t)(((uint32_t)(x)) << MTBDWT_FCT_DATAVMATCH_SHIFT)) & MTBDWT_FCT_DATAVMATCH_MASK)
#define MTBDWT_FCT_DATAVSIZE_MASK                (0xC00U)
#define MTBDWT_FCT_DATAVSIZE_SHIFT               (10U)
#define MTBDWT_FCT_DATAVSIZE(x)                  (((uint32_t)(((uint32_t)(x)) << MTBDWT_FCT_DATAVSIZE_SHIFT)) & MTBDWT_FCT_DATAVSIZE_MASK)
#define MTBDWT_FCT_DATAVADDR0_MASK               (0xF000U)
#define MTBDWT_FCT_DATAVADDR0_SHIFT              (12U)
#define MTBDWT_FCT_DATAVADDR0(x)                 (((uint32_t)(((uint32_t)(x)) << MTBDWT_FCT_DATAVADDR0_SHIFT)) & MTBDWT_FCT_DATAVADDR0_MASK)
#define MTBDWT_FCT_MATCHED_MASK                  (0x1000000U)
#define MTBDWT_FCT_MATCHED_SHIFT                 (24U)
#define MTBDWT_FCT_MATCHED(x)                    (((uint32_t)(((uint32_t)(x)) << MTBDWT_FCT_MATCHED_SHIFT)) & MTBDWT_FCT_MATCHED_MASK)

/* The count of MTBDWT_FCT */
#define MTBDWT_FCT_COUNT                         (2U)

/*! @name TBCTRL - MTB_DWT Trace Buffer Control Register */
#define MTBDWT_TBCTRL_ACOMP0_MASK                (0x1U)
#define MTBDWT_TBCTRL_ACOMP0_SHIFT               (0U)
#define MTBDWT_TBCTRL_ACOMP0(x)                  (((uint32_t)(((uint32_t)(x)) << MTBDWT_TBCTRL_ACOMP0_SHIFT)) & MTBDWT_TBCTRL_ACOMP0_MASK)
#define MTBDWT_TBCTRL_ACOMP1_MASK                (0x2U)
#define MTBDWT_TBCTRL_ACOMP1_SHIFT               (1U)
#define MTBDWT_TBCTRL_ACOMP1(x)                  (((uint32_t)(((uint32_t)(x)) << MTBDWT_TBCTRL_ACOMP1_SHIFT)) & MTBDWT_TBCTRL_ACOMP1_MASK)
#define MTBDWT_TBCTRL_NUMCOMP_MASK               (0xF0000000U)
#define MTBDWT_TBCTRL_NUMCOMP_SHIFT              (28U)
#define MTBDWT_TBCTRL_NUMCOMP(x)                 (((uint32_t)(((uint32_t)(x)) << MTBDWT_TBCTRL_NUMCOMP_SHIFT)) & MTBDWT_TBCTRL_NUMCOMP_MASK)

/*! @name DEVICECFG - Device Configuration Register */
#define MTBDWT_DEVICECFG_DEVICECFG_MASK          (0xFFFFFFFFU)
#define MTBDWT_DEVICECFG_DEVICECFG_SHIFT         (0U)
#define MTBDWT_DEVICECFG_DEVICECFG(x)            (((uint32_t)(((uint32_t)(x)) << MTBDWT_DEVICECFG_DEVICECFG_SHIFT)) & MTBDWT_DEVICECFG_DEVICECFG_MASK)

/*! @name DEVICETYPID - Device Type Identifier Register */
#define MTBDWT_DEVICETYPID_DEVICETYPID_MASK      (0xFFFFFFFFU)
#define MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT     (0U)
#define MTBDWT_DEVICETYPID_DEVICETYPID(x)        (((uint32_t)(((uint32_t)(x)) << MTBDWT_DEVICETYPID_DEVICETYPID_SHIFT)) & MTBDWT_DEVICETYPID_DEVICETYPID_MASK)

/*! @name PERIPHID4 - Peripheral ID Register */
#define MTBDWT_PERIPHID4_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID4_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID4_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID4_PERIPHID_SHIFT)) & MTBDWT_PERIPHID4_PERIPHID_MASK)

/*! @name PERIPHID5 - Peripheral ID Register */
#define MTBDWT_PERIPHID5_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID5_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID5_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID5_PERIPHID_SHIFT)) & MTBDWT_PERIPHID5_PERIPHID_MASK)

/*! @name PERIPHID6 - Peripheral ID Register */
#define MTBDWT_PERIPHID6_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID6_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID6_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID6_PERIPHID_SHIFT)) & MTBDWT_PERIPHID6_PERIPHID_MASK)

/*! @name PERIPHID7 - Peripheral ID Register */
#define MTBDWT_PERIPHID7_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID7_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID7_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID7_PERIPHID_SHIFT)) & MTBDWT_PERIPHID7_PERIPHID_MASK)

/*! @name PERIPHID0 - Peripheral ID Register */
#define MTBDWT_PERIPHID0_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID0_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID0_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID0_PERIPHID_SHIFT)) & MTBDWT_PERIPHID0_PERIPHID_MASK)

/*! @name PERIPHID1 - Peripheral ID Register */
#define MTBDWT_PERIPHID1_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID1_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID1_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID1_PERIPHID_SHIFT)) & MTBDWT_PERIPHID1_PERIPHID_MASK)

/*! @name PERIPHID2 - Peripheral ID Register */
#define MTBDWT_PERIPHID2_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID2_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID2_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID2_PERIPHID_SHIFT)) & MTBDWT_PERIPHID2_PERIPHID_MASK)

/*! @name PERIPHID3 - Peripheral ID Register */
#define MTBDWT_PERIPHID3_PERIPHID_MASK           (0xFFFFFFFFU)
#define MTBDWT_PERIPHID3_PERIPHID_SHIFT          (0U)
#define MTBDWT_PERIPHID3_PERIPHID(x)             (((uint32_t)(((uint32_t)(x)) << MTBDWT_PERIPHID3_PERIPHID_SHIFT)) & MTBDWT_PERIPHID3_PERIPHID_MASK)

/*! @name COMPID - Component ID Register */
#define MTBDWT_COMPID_COMPID_MASK                (0xFFFFFFFFU)
#define MTBDWT_COMPID_COMPID_SHIFT               (0U)
#define MTBDWT_COMPID_COMPID(x)                  (((uint32_t)(((uint32_t)(x)) << MTBDWT_COMPID_COMPID_SHIFT)) & MTBDWT_COMPID_COMPID_MASK)

/* The count of MTBDWT_COMPID */
#define MTBDWT_COMPID_COUNT                      (4U)


/*!
 * @}
 */ /* end of group MTBDWT_Register_Masks */


/* MTBDWT - Peripheral instance base addresses */
/** Peripheral MTBDWT base address */
#define MTBDWT_BASE                              (0xF0001000u)
/** Peripheral MTBDWT base pointer */
#define MTBDWT                                   ((MTBDWT_Type *)MTBDWT_BASE)
/** Array initializer of MTBDWT peripheral base addresses */
#define MTBDWT_BASE_ADDRS                        { MTBDWT_BASE }
/** Array initializer of MTBDWT peripheral base pointers */
#define MTBDWT_BASE_PTRS                         { MTBDWT }

/*!
 * @}
 */ /* end of group MTBDWT_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- NV Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Peripheral_Access_Layer NV Peripheral Access Layer
 * @{
 */

/** NV - Register Layout Typedef */
typedef struct {
  __I  uint8_t BACKKEY3;                           /**< Backdoor Comparison Key 3., offset: 0x0 */
  __I  uint8_t BACKKEY2;                           /**< Backdoor Comparison Key 2., offset: 0x1 */
  __I  uint8_t BACKKEY1;                           /**< Backdoor Comparison Key 1., offset: 0x2 */
  __I  uint8_t BACKKEY0;                           /**< Backdoor Comparison Key 0., offset: 0x3 */
  __I  uint8_t BACKKEY7;                           /**< Backdoor Comparison Key 7., offset: 0x4 */
  __I  uint8_t BACKKEY6;                           /**< Backdoor Comparison Key 6., offset: 0x5 */
  __I  uint8_t BACKKEY5;                           /**< Backdoor Comparison Key 5., offset: 0x6 */
  __I  uint8_t BACKKEY4;                           /**< Backdoor Comparison Key 4., offset: 0x7 */
  __I  uint8_t FPROT3;                             /**< Non-volatile P-Flash Protection 1 - Low Register, offset: 0x8 */
  __I  uint8_t FPROT2;                             /**< Non-volatile P-Flash Protection 1 - High Register, offset: 0x9 */
  __I  uint8_t FPROT1;                             /**< Non-volatile P-Flash Protection 0 - Low Register, offset: 0xA */
  __I  uint8_t FPROT0;                             /**< Non-volatile P-Flash Protection 0 - High Register, offset: 0xB */
  __I  uint8_t FSEC;                               /**< Non-volatile Flash Security Register, offset: 0xC */
  __I  uint8_t FOPT;                               /**< Non-volatile Flash Option Register, offset: 0xD */
} NV_Type;

/* ----------------------------------------------------------------------------
   -- NV Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup NV_Register_Masks NV Register Masks
 * @{
 */

/*! @name BACKKEY3 - Backdoor Comparison Key 3. */
#define NV_BACKKEY3_KEY_MASK                     (0xFFU)
#define NV_BACKKEY3_KEY_SHIFT                    (0U)
#define NV_BACKKEY3_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY3_KEY_SHIFT)) & NV_BACKKEY3_KEY_MASK)

/*! @name BACKKEY2 - Backdoor Comparison Key 2. */
#define NV_BACKKEY2_KEY_MASK                     (0xFFU)
#define NV_BACKKEY2_KEY_SHIFT                    (0U)
#define NV_BACKKEY2_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY2_KEY_SHIFT)) & NV_BACKKEY2_KEY_MASK)

/*! @name BACKKEY1 - Backdoor Comparison Key 1. */
#define NV_BACKKEY1_KEY_MASK                     (0xFFU)
#define NV_BACKKEY1_KEY_SHIFT                    (0U)
#define NV_BACKKEY1_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY1_KEY_SHIFT)) & NV_BACKKEY1_KEY_MASK)

/*! @name BACKKEY0 - Backdoor Comparison Key 0. */
#define NV_BACKKEY0_KEY_MASK                     (0xFFU)
#define NV_BACKKEY0_KEY_SHIFT                    (0U)
#define NV_BACKKEY0_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY0_KEY_SHIFT)) & NV_BACKKEY0_KEY_MASK)

/*! @name BACKKEY7 - Backdoor Comparison Key 7. */
#define NV_BACKKEY7_KEY_MASK                     (0xFFU)
#define NV_BACKKEY7_KEY_SHIFT                    (0U)
#define NV_BACKKEY7_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY7_KEY_SHIFT)) & NV_BACKKEY7_KEY_MASK)

/*! @name BACKKEY6 - Backdoor Comparison Key 6. */
#define NV_BACKKEY6_KEY_MASK                     (0xFFU)
#define NV_BACKKEY6_KEY_SHIFT                    (0U)
#define NV_BACKKEY6_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY6_KEY_SHIFT)) & NV_BACKKEY6_KEY_MASK)

/*! @name BACKKEY5 - Backdoor Comparison Key 5. */
#define NV_BACKKEY5_KEY_MASK                     (0xFFU)
#define NV_BACKKEY5_KEY_SHIFT                    (0U)
#define NV_BACKKEY5_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY5_KEY_SHIFT)) & NV_BACKKEY5_KEY_MASK)

/*! @name BACKKEY4 - Backdoor Comparison Key 4. */
#define NV_BACKKEY4_KEY_MASK                     (0xFFU)
#define NV_BACKKEY4_KEY_SHIFT                    (0U)
#define NV_BACKKEY4_KEY(x)                       (((uint8_t)(((uint8_t)(x)) << NV_BACKKEY4_KEY_SHIFT)) & NV_BACKKEY4_KEY_MASK)

/*! @name FPROT3 - Non-volatile P-Flash Protection 1 - Low Register */
#define NV_FPROT3_PROT_MASK                      (0xFFU)
#define NV_FPROT3_PROT_SHIFT                     (0U)
#define NV_FPROT3_PROT(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FPROT3_PROT_SHIFT)) & NV_FPROT3_PROT_MASK)

/*! @name FPROT2 - Non-volatile P-Flash Protection 1 - High Register */
#define NV_FPROT2_PROT_MASK                      (0xFFU)
#define NV_FPROT2_PROT_SHIFT                     (0U)
#define NV_FPROT2_PROT(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FPROT2_PROT_SHIFT)) & NV_FPROT2_PROT_MASK)

/*! @name FPROT1 - Non-volatile P-Flash Protection 0 - Low Register */
#define NV_FPROT1_PROT_MASK                      (0xFFU)
#define NV_FPROT1_PROT_SHIFT                     (0U)
#define NV_FPROT1_PROT(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FPROT1_PROT_SHIFT)) & NV_FPROT1_PROT_MASK)

/*! @name FPROT0 - Non-volatile P-Flash Protection 0 - High Register */
#define NV_FPROT0_PROT_MASK                      (0xFFU)
#define NV_FPROT0_PROT_SHIFT                     (0U)
#define NV_FPROT0_PROT(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FPROT0_PROT_SHIFT)) & NV_FPROT0_PROT_MASK)

/*! @name FSEC - Non-volatile Flash Security Register */
#define NV_FSEC_SEC_MASK                         (0x3U)
#define NV_FSEC_SEC_SHIFT                        (0U)
#define NV_FSEC_SEC(x)                           (((uint8_t)(((uint8_t)(x)) << NV_FSEC_SEC_SHIFT)) & NV_FSEC_SEC_MASK)
#define NV_FSEC_FSLACC_MASK                      (0xCU)
#define NV_FSEC_FSLACC_SHIFT                     (2U)
#define NV_FSEC_FSLACC(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FSEC_FSLACC_SHIFT)) & NV_FSEC_FSLACC_MASK)
#define NV_FSEC_MEEN_MASK                        (0x30U)
#define NV_FSEC_MEEN_SHIFT                       (4U)
#define NV_FSEC_MEEN(x)                          (((uint8_t)(((uint8_t)(x)) << NV_FSEC_MEEN_SHIFT)) & NV_FSEC_MEEN_MASK)
#define NV_FSEC_KEYEN_MASK                       (0xC0U)
#define NV_FSEC_KEYEN_SHIFT                      (6U)
#define NV_FSEC_KEYEN(x)                         (((uint8_t)(((uint8_t)(x)) << NV_FSEC_KEYEN_SHIFT)) & NV_FSEC_KEYEN_MASK)

/*! @name FOPT - Non-volatile Flash Option Register */
#define NV_FOPT_LPBOOT_MASK                      (0x1U)
#define NV_FOPT_LPBOOT_SHIFT                     (0U)
#define NV_FOPT_LPBOOT(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FOPT_LPBOOT_SHIFT)) & NV_FOPT_LPBOOT_MASK)
#define NV_FOPT_NMI_EN_MASK                      (0x4U)
#define NV_FOPT_NMI_EN_SHIFT                     (2U)
#define NV_FOPT_NMI_EN(x)                        (((uint8_t)(((uint8_t)(x)) << NV_FOPT_NMI_EN_SHIFT)) & NV_FOPT_NMI_EN_MASK)
#define NV_FOPT_EXE_MODE_MASK                    (0x8U)
#define NV_FOPT_EXE_MODE_SHIFT                   (3U)
#define NV_FOPT_EXE_MODE(x)                      (((uint8_t)(((uint8_t)(x)) << NV_FOPT_EXE_MODE_SHIFT)) & NV_FOPT_EXE_MODE_MASK)
#define NV_FOPT_CLK_SRC_MASK                     (0x20U)
#define NV_FOPT_CLK_SRC_SHIFT                    (5U)
#define NV_FOPT_CLK_SRC(x)                       (((uint8_t)(((uint8_t)(x)) << NV_FOPT_CLK_SRC_SHIFT)) & NV_FOPT_CLK_SRC_MASK)


/*!
 * @}
 */ /* end of group NV_Register_Masks */


/* NV - Peripheral instance base addresses */
/** Peripheral FTFA_FlashConfig base address */
#define FTFA_FlashConfig_BASE                    (0x400u)
/** Peripheral FTFA_FlashConfig base pointer */
#define FTFA_FlashConfig                         ((NV_Type *)FTFA_FlashConfig_BASE)
/** Array initializer of NV peripheral base addresses */
#define NV_BASE_ADDRS                            { FTFA_FlashConfig_BASE }
/** Array initializer of NV peripheral base pointers */
#define NV_BASE_PTRS                             { FTFA_FlashConfig }

/*!
 * @}
 */ /* end of group NV_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- OSC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Peripheral_Access_Layer OSC Peripheral Access Layer
 * @{
 */

/** OSC - Register Layout Typedef */
typedef struct {
  __IO uint8_t CR;                                 /**< OSC Control Register, offset: 0x0 */
} OSC_Type;

/* ----------------------------------------------------------------------------
   -- OSC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup OSC_Register_Masks OSC Register Masks
 * @{
 */

/*! @name CR - OSC Control Register */
#define OSC_CR_SC16P_MASK                        (0x1U)
#define OSC_CR_SC16P_SHIFT                       (0U)
#define OSC_CR_SC16P(x)                          (((uint8_t)(((uint8_t)(x)) << OSC_CR_SC16P_SHIFT)) & OSC_CR_SC16P_MASK)
#define OSC_CR_SC8P_MASK                         (0x2U)
#define OSC_CR_SC8P_SHIFT                        (1U)
#define OSC_CR_SC8P(x)                           (((uint8_t)(((uint8_t)(x)) << OSC_CR_SC8P_SHIFT)) & OSC_CR_SC8P_MASK)
#define OSC_CR_SC4P_MASK                         (0x4U)
#define OSC_CR_SC4P_SHIFT                        (2U)
#define OSC_CR_SC4P(x)                           (((uint8_t)(((uint8_t)(x)) << OSC_CR_SC4P_SHIFT)) & OSC_CR_SC4P_MASK)
#define OSC_CR_SC2P_MASK                         (0x8U)
#define OSC_CR_SC2P_SHIFT                        (3U)
#define OSC_CR_SC2P(x)                           (((uint8_t)(((uint8_t)(x)) << OSC_CR_SC2P_SHIFT)) & OSC_CR_SC2P_MASK)
#define OSC_CR_EREFSTEN_MASK                     (0x20U)
#define OSC_CR_EREFSTEN_SHIFT                    (5U)
#define OSC_CR_EREFSTEN(x)                       (((uint8_t)(((uint8_t)(x)) << OSC_CR_EREFSTEN_SHIFT)) & OSC_CR_EREFSTEN_MASK)
#define OSC_CR_ERCLKEN_MASK                      (0x80U)
#define OSC_CR_ERCLKEN_SHIFT                     (7U)
#define OSC_CR_ERCLKEN(x)                        (((uint8_t)(((uint8_t)(x)) << OSC_CR_ERCLKEN_SHIFT)) & OSC_CR_ERCLKEN_MASK)


/*!
 * @}
 */ /* end of group OSC_Register_Masks */


/* OSC - Peripheral instance base addresses */
/** Peripheral OSC base address */
#define OSC_BASE                                 (0x40066000u)
/** Peripheral OSC base pointer */
#define OSC                                      ((OSC_Type *)OSC_BASE)
/** Array initializer of OSC peripheral base addresses */
#define OSC_BASE_ADDRS                           { OSC_BASE }
/** Array initializer of OSC peripheral base pointers */
#define OSC_BASE_PTRS                            { OSC }

/*!
 * @}
 */ /* end of group OSC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PDB Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PDB_Peripheral_Access_Layer PDB Peripheral Access Layer
 * @{
 */

/** PDB - Register Layout Typedef */
typedef struct {
  __IO uint32_t SC;                                /**< Status and Control register, offset: 0x0 */
  __IO uint32_t MOD;                               /**< Modulus register, offset: 0x4 */
  __I  uint32_t CNT;                               /**< Counter register, offset: 0x8 */
  __IO uint32_t IDLY;                              /**< Interrupt Delay register, offset: 0xC */
  struct {                                         /* offset: 0x10, array step: 0x18 */
    __IO uint32_t C1;                                /**< Channel n Control register 1, array offset: 0x10, array step: 0x18 */
    __IO uint32_t S;                                 /**< Channel n Status register, array offset: 0x14, array step: 0x18 */
    __IO uint32_t DLY[4];                            /**< Channel n Delay 0 register..Channel n Delay 3 register, array offset: 0x18, array step: index*0x18, index2*0x4 */
  } CH[1];
       uint8_t RESERVED_0[360];
  __IO uint32_t POEN;                              /**< Pulse-Out n Enable register, offset: 0x190 */
  __IO uint32_t PODLY[1];                          /**< Pulse-Out n Delay register, array offset: 0x194, array step: 0x4 */
} PDB_Type;

/* ----------------------------------------------------------------------------
   -- PDB Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PDB_Register_Masks PDB Register Masks
 * @{
 */

/*! @name SC - Status and Control register */
#define PDB_SC_LDOK_MASK                         (0x1U)
#define PDB_SC_LDOK_SHIFT                        (0U)
#define PDB_SC_LDOK(x)                           (((uint32_t)(((uint32_t)(x)) << PDB_SC_LDOK_SHIFT)) & PDB_SC_LDOK_MASK)
#define PDB_SC_CONT_MASK                         (0x2U)
#define PDB_SC_CONT_SHIFT                        (1U)
#define PDB_SC_CONT(x)                           (((uint32_t)(((uint32_t)(x)) << PDB_SC_CONT_SHIFT)) & PDB_SC_CONT_MASK)
#define PDB_SC_MULT_MASK                         (0xCU)
#define PDB_SC_MULT_SHIFT                        (2U)
#define PDB_SC_MULT(x)                           (((uint32_t)(((uint32_t)(x)) << PDB_SC_MULT_SHIFT)) & PDB_SC_MULT_MASK)
#define PDB_SC_PDBIE_MASK                        (0x20U)
#define PDB_SC_PDBIE_SHIFT                       (5U)
#define PDB_SC_PDBIE(x)                          (((uint32_t)(((uint32_t)(x)) << PDB_SC_PDBIE_SHIFT)) & PDB_SC_PDBIE_MASK)
#define PDB_SC_PDBIF_MASK                        (0x40U)
#define PDB_SC_PDBIF_SHIFT                       (6U)
#define PDB_SC_PDBIF(x)                          (((uint32_t)(((uint32_t)(x)) << PDB_SC_PDBIF_SHIFT)) & PDB_SC_PDBIF_MASK)
#define PDB_SC_PDBEN_MASK                        (0x80U)
#define PDB_SC_PDBEN_SHIFT                       (7U)
#define PDB_SC_PDBEN(x)                          (((uint32_t)(((uint32_t)(x)) << PDB_SC_PDBEN_SHIFT)) & PDB_SC_PDBEN_MASK)
#define PDB_SC_TRGSEL_MASK                       (0xF00U)
#define PDB_SC_TRGSEL_SHIFT                      (8U)
#define PDB_SC_TRGSEL(x)                         (((uint32_t)(((uint32_t)(x)) << PDB_SC_TRGSEL_SHIFT)) & PDB_SC_TRGSEL_MASK)
#define PDB_SC_PRESCALER_MASK                    (0x7000U)
#define PDB_SC_PRESCALER_SHIFT                   (12U)
#define PDB_SC_PRESCALER(x)                      (((uint32_t)(((uint32_t)(x)) << PDB_SC_PRESCALER_SHIFT)) & PDB_SC_PRESCALER_MASK)
#define PDB_SC_DMAEN_MASK                        (0x8000U)
#define PDB_SC_DMAEN_SHIFT                       (15U)
#define PDB_SC_DMAEN(x)                          (((uint32_t)(((uint32_t)(x)) << PDB_SC_DMAEN_SHIFT)) & PDB_SC_DMAEN_MASK)
#define PDB_SC_SWTRIG_MASK                       (0x10000U)
#define PDB_SC_SWTRIG_SHIFT                      (16U)
#define PDB_SC_SWTRIG(x)                         (((uint32_t)(((uint32_t)(x)) << PDB_SC_SWTRIG_SHIFT)) & PDB_SC_SWTRIG_MASK)
#define PDB_SC_PDBEIE_MASK                       (0x20000U)
#define PDB_SC_PDBEIE_SHIFT                      (17U)
#define PDB_SC_PDBEIE(x)                         (((uint32_t)(((uint32_t)(x)) << PDB_SC_PDBEIE_SHIFT)) & PDB_SC_PDBEIE_MASK)
#define PDB_SC_LDMOD_MASK                        (0xC0000U)
#define PDB_SC_LDMOD_SHIFT                       (18U)
#define PDB_SC_LDMOD(x)                          (((uint32_t)(((uint32_t)(x)) << PDB_SC_LDMOD_SHIFT)) & PDB_SC_LDMOD_MASK)

/*! @name MOD - Modulus register */
#define PDB_MOD_MOD_MASK                         (0xFFFFU)
#define PDB_MOD_MOD_SHIFT                        (0U)
#define PDB_MOD_MOD(x)                           (((uint32_t)(((uint32_t)(x)) << PDB_MOD_MOD_SHIFT)) & PDB_MOD_MOD_MASK)

/*! @name CNT - Counter register */
#define PDB_CNT_CNT_MASK                         (0xFFFFU)
#define PDB_CNT_CNT_SHIFT                        (0U)
#define PDB_CNT_CNT(x)                           (((uint32_t)(((uint32_t)(x)) << PDB_CNT_CNT_SHIFT)) & PDB_CNT_CNT_MASK)

/*! @name IDLY - Interrupt Delay register */
#define PDB_IDLY_IDLY_MASK                       (0xFFFFU)
#define PDB_IDLY_IDLY_SHIFT                      (0U)
#define PDB_IDLY_IDLY(x)                         (((uint32_t)(((uint32_t)(x)) << PDB_IDLY_IDLY_SHIFT)) & PDB_IDLY_IDLY_MASK)

/*! @name C1 - Channel n Control register 1 */
#define PDB_C1_EN_MASK                           (0xFFU)
#define PDB_C1_EN_SHIFT                          (0U)
#define PDB_C1_EN(x)                             (((uint32_t)(((uint32_t)(x)) << PDB_C1_EN_SHIFT)) & PDB_C1_EN_MASK)
#define PDB_C1_TOS_MASK                          (0xFF00U)
#define PDB_C1_TOS_SHIFT                         (8U)
#define PDB_C1_TOS(x)                            (((uint32_t)(((uint32_t)(x)) << PDB_C1_TOS_SHIFT)) & PDB_C1_TOS_MASK)
#define PDB_C1_BB_MASK                           (0xFF0000U)
#define PDB_C1_BB_SHIFT                          (16U)
#define PDB_C1_BB(x)                             (((uint32_t)(((uint32_t)(x)) << PDB_C1_BB_SHIFT)) & PDB_C1_BB_MASK)

/* The count of PDB_C1 */
#define PDB_C1_COUNT                             (1U)

/*! @name S - Channel n Status register */
#define PDB_S_ERR_MASK                           (0xFFU)
#define PDB_S_ERR_SHIFT                          (0U)
#define PDB_S_ERR(x)                             (((uint32_t)(((uint32_t)(x)) << PDB_S_ERR_SHIFT)) & PDB_S_ERR_MASK)
#define PDB_S_CF_MASK                            (0xFF0000U)
#define PDB_S_CF_SHIFT                           (16U)
#define PDB_S_CF(x)                              (((uint32_t)(((uint32_t)(x)) << PDB_S_CF_SHIFT)) & PDB_S_CF_MASK)

/* The count of PDB_S */
#define PDB_S_COUNT                              (1U)

/*! @name DLY - Channel n Delay 0 register..Channel n Delay 3 register */
#define PDB_DLY_DLY_MASK                         (0xFFFFU)
#define PDB_DLY_DLY_SHIFT                        (0U)
#define PDB_DLY_DLY(x)                           (((uint32_t)(((uint32_t)(x)) << PDB_DLY_DLY_SHIFT)) & PDB_DLY_DLY_MASK)

/* The count of PDB_DLY */
#define PDB_DLY_COUNT                            (1U)

/* The count of PDB_DLY */
#define PDB_DLY_COUNT2                           (4U)

/*! @name POEN - Pulse-Out n Enable register */
#define PDB_POEN_POEN_MASK                       (0xFFU)
#define PDB_POEN_POEN_SHIFT                      (0U)
#define PDB_POEN_POEN(x)                         (((uint32_t)(((uint32_t)(x)) << PDB_POEN_POEN_SHIFT)) & PDB_POEN_POEN_MASK)

/*! @name PODLY - Pulse-Out n Delay register */
#define PDB_PODLY_DLY2_MASK                      (0xFFFFU)
#define PDB_PODLY_DLY2_SHIFT                     (0U)
#define PDB_PODLY_DLY2(x)                        (((uint32_t)(((uint32_t)(x)) << PDB_PODLY_DLY2_SHIFT)) & PDB_PODLY_DLY2_MASK)
#define PDB_PODLY_DLY1_MASK                      (0xFFFF0000U)
#define PDB_PODLY_DLY1_SHIFT                     (16U)
#define PDB_PODLY_DLY1(x)                        (((uint32_t)(((uint32_t)(x)) << PDB_PODLY_DLY1_SHIFT)) & PDB_PODLY_DLY1_MASK)

/* The count of PDB_PODLY */
#define PDB_PODLY_COUNT                          (1U)


/*!
 * @}
 */ /* end of group PDB_Register_Masks */


/* PDB - Peripheral instance base addresses */
/** Peripheral PDB0 base address */
#define PDB0_BASE                                (0x40036000u)
/** Peripheral PDB0 base pointer */
#define PDB0                                     ((PDB_Type *)PDB0_BASE)
/** Array initializer of PDB peripheral base addresses */
#define PDB_BASE_ADDRS                           { PDB0_BASE }
/** Array initializer of PDB peripheral base pointers */
#define PDB_BASE_PTRS                            { PDB0 }
/** Interrupt vectors for the PDB peripheral type */
#define PDB_IRQS                                 { PDB0_IRQn }

/*!
 * @}
 */ /* end of group PDB_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PIT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Peripheral_Access_Layer PIT Peripheral Access Layer
 * @{
 */

/** PIT - Register Layout Typedef */
typedef struct {
  __IO uint32_t MCR;                               /**< PIT Module Control Register, offset: 0x0 */
       uint8_t RESERVED_0[252];
  struct {                                         /* offset: 0x100, array step: 0x10 */
    __IO uint32_t LDVAL;                             /**< Timer Load Value Register, array offset: 0x100, array step: 0x10 */
    __I  uint32_t CVAL;                              /**< Current Timer Value Register, array offset: 0x104, array step: 0x10 */
    __IO uint32_t TCTRL;                             /**< Timer Control Register, array offset: 0x108, array step: 0x10 */
    __IO uint32_t TFLG;                              /**< Timer Flag Register, array offset: 0x10C, array step: 0x10 */
  } CHANNEL[2];
} PIT_Type;

/* ----------------------------------------------------------------------------
   -- PIT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PIT_Register_Masks PIT Register Masks
 * @{
 */

/*! @name MCR - PIT Module Control Register */
#define PIT_MCR_FRZ_MASK                         (0x1U)
#define PIT_MCR_FRZ_SHIFT                        (0U)
#define PIT_MCR_FRZ(x)                           (((uint32_t)(((uint32_t)(x)) << PIT_MCR_FRZ_SHIFT)) & PIT_MCR_FRZ_MASK)
#define PIT_MCR_MDIS_MASK                        (0x2U)
#define PIT_MCR_MDIS_SHIFT                       (1U)
#define PIT_MCR_MDIS(x)                          (((uint32_t)(((uint32_t)(x)) << PIT_MCR_MDIS_SHIFT)) & PIT_MCR_MDIS_MASK)

/*! @name LDVAL - Timer Load Value Register */
#define PIT_LDVAL_TSV_MASK                       (0xFFFFFFFFU)
#define PIT_LDVAL_TSV_SHIFT                      (0U)
#define PIT_LDVAL_TSV(x)                         (((uint32_t)(((uint32_t)(x)) << PIT_LDVAL_TSV_SHIFT)) & PIT_LDVAL_TSV_MASK)

/* The count of PIT_LDVAL */
#define PIT_LDVAL_COUNT                          (2U)

/*! @name CVAL - Current Timer Value Register */
#define PIT_CVAL_TVL_MASK                        (0xFFFFFFFFU)
#define PIT_CVAL_TVL_SHIFT                       (0U)
#define PIT_CVAL_TVL(x)                          (((uint32_t)(((uint32_t)(x)) << PIT_CVAL_TVL_SHIFT)) & PIT_CVAL_TVL_MASK)

/* The count of PIT_CVAL */
#define PIT_CVAL_COUNT                           (2U)

/*! @name TCTRL - Timer Control Register */
#define PIT_TCTRL_TEN_MASK                       (0x1U)
#define PIT_TCTRL_TEN_SHIFT                      (0U)
#define PIT_TCTRL_TEN(x)                         (((uint32_t)(((uint32_t)(x)) << PIT_TCTRL_TEN_SHIFT)) & PIT_TCTRL_TEN_MASK)
#define PIT_TCTRL_TIE_MASK                       (0x2U)
#define PIT_TCTRL_TIE_SHIFT                      (1U)
#define PIT_TCTRL_TIE(x)                         (((uint32_t)(((uint32_t)(x)) << PIT_TCTRL_TIE_SHIFT)) & PIT_TCTRL_TIE_MASK)
#define PIT_TCTRL_CHN_MASK                       (0x4U)
#define PIT_TCTRL_CHN_SHIFT                      (2U)
#define PIT_TCTRL_CHN(x)                         (((uint32_t)(((uint32_t)(x)) << PIT_TCTRL_CHN_SHIFT)) & PIT_TCTRL_CHN_MASK)

/* The count of PIT_TCTRL */
#define PIT_TCTRL_COUNT                          (2U)

/*! @name TFLG - Timer Flag Register */
#define PIT_TFLG_TIF_MASK                        (0x1U)
#define PIT_TFLG_TIF_SHIFT                       (0U)
#define PIT_TFLG_TIF(x)                          (((uint32_t)(((uint32_t)(x)) << PIT_TFLG_TIF_SHIFT)) & PIT_TFLG_TIF_MASK)

/* The count of PIT_TFLG */
#define PIT_TFLG_COUNT                           (2U)


/*!
 * @}
 */ /* end of group PIT_Register_Masks */


/* PIT - Peripheral instance base addresses */
/** Peripheral PIT0 base address */
#define PIT0_BASE                                (0x4002D000u)
/** Peripheral PIT0 base pointer */
#define PIT0                                     ((PIT_Type *)PIT0_BASE)
/** Peripheral PIT1 base address */
#define PIT1_BASE                                (0x4002E000u)
/** Peripheral PIT1 base pointer */
#define PIT1                                     ((PIT_Type *)PIT1_BASE)
/** Array initializer of PIT peripheral base addresses */
#define PIT_BASE_ADDRS                           { PIT0_BASE, PIT1_BASE }
/** Array initializer of PIT peripheral base pointers */
#define PIT_BASE_PTRS                            { PIT0, PIT1 }
/** Interrupt vectors for the PIT peripheral type */
#define PIT_IRQS                                 { { PIT0_PIT1_IRQn, PIT0_PIT1_IRQn }, { PIT0_PIT1_IRQn, PIT0_PIT1_IRQn } }

/*!
 * @}
 */ /* end of group PIT_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PMC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Peripheral_Access_Layer PMC Peripheral Access Layer
 * @{
 */

/** PMC - Register Layout Typedef */
typedef struct {
  __IO uint8_t LVDSC1;                             /**< Low Voltage Detect Status And Control 1 register, offset: 0x0 */
  __IO uint8_t LVDSC2;                             /**< Low Voltage Detect Status And Control 2 register, offset: 0x1 */
  __IO uint8_t REGSC;                              /**< Regulator Status And Control register, offset: 0x2 */
} PMC_Type;

/* ----------------------------------------------------------------------------
   -- PMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PMC_Register_Masks PMC Register Masks
 * @{
 */

/*! @name LVDSC1 - Low Voltage Detect Status And Control 1 register */
#define PMC_LVDSC1_LVDV_MASK                     (0x3U)
#define PMC_LVDSC1_LVDV_SHIFT                    (0U)
#define PMC_LVDSC1_LVDV(x)                       (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC1_LVDV_SHIFT)) & PMC_LVDSC1_LVDV_MASK)
#define PMC_LVDSC1_LVDRE_MASK                    (0x10U)
#define PMC_LVDSC1_LVDRE_SHIFT                   (4U)
#define PMC_LVDSC1_LVDRE(x)                      (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC1_LVDRE_SHIFT)) & PMC_LVDSC1_LVDRE_MASK)
#define PMC_LVDSC1_LVDIE_MASK                    (0x20U)
#define PMC_LVDSC1_LVDIE_SHIFT                   (5U)
#define PMC_LVDSC1_LVDIE(x)                      (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC1_LVDIE_SHIFT)) & PMC_LVDSC1_LVDIE_MASK)
#define PMC_LVDSC1_LVDACK_MASK                   (0x40U)
#define PMC_LVDSC1_LVDACK_SHIFT                  (6U)
#define PMC_LVDSC1_LVDACK(x)                     (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC1_LVDACK_SHIFT)) & PMC_LVDSC1_LVDACK_MASK)
#define PMC_LVDSC1_LVDF_MASK                     (0x80U)
#define PMC_LVDSC1_LVDF_SHIFT                    (7U)
#define PMC_LVDSC1_LVDF(x)                       (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC1_LVDF_SHIFT)) & PMC_LVDSC1_LVDF_MASK)

/*! @name LVDSC2 - Low Voltage Detect Status And Control 2 register */
#define PMC_LVDSC2_LVWV_MASK                     (0x3U)
#define PMC_LVDSC2_LVWV_SHIFT                    (0U)
#define PMC_LVDSC2_LVWV(x)                       (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC2_LVWV_SHIFT)) & PMC_LVDSC2_LVWV_MASK)
#define PMC_LVDSC2_LVWIE_MASK                    (0x20U)
#define PMC_LVDSC2_LVWIE_SHIFT                   (5U)
#define PMC_LVDSC2_LVWIE(x)                      (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC2_LVWIE_SHIFT)) & PMC_LVDSC2_LVWIE_MASK)
#define PMC_LVDSC2_LVWACK_MASK                   (0x40U)
#define PMC_LVDSC2_LVWACK_SHIFT                  (6U)
#define PMC_LVDSC2_LVWACK(x)                     (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC2_LVWACK_SHIFT)) & PMC_LVDSC2_LVWACK_MASK)
#define PMC_LVDSC2_LVWF_MASK                     (0x80U)
#define PMC_LVDSC2_LVWF_SHIFT                    (7U)
#define PMC_LVDSC2_LVWF(x)                       (((uint8_t)(((uint8_t)(x)) << PMC_LVDSC2_LVWF_SHIFT)) & PMC_LVDSC2_LVWF_MASK)

/*! @name REGSC - Regulator Status And Control register */
#define PMC_REGSC_BGBE_MASK                      (0x1U)
#define PMC_REGSC_BGBE_SHIFT                     (0U)
#define PMC_REGSC_BGBE(x)                        (((uint8_t)(((uint8_t)(x)) << PMC_REGSC_BGBE_SHIFT)) & PMC_REGSC_BGBE_MASK)
#define PMC_REGSC_REGONS_MASK                    (0x4U)
#define PMC_REGSC_REGONS_SHIFT                   (2U)
#define PMC_REGSC_REGONS(x)                      (((uint8_t)(((uint8_t)(x)) << PMC_REGSC_REGONS_SHIFT)) & PMC_REGSC_REGONS_MASK)
#define PMC_REGSC_ACKISO_MASK                    (0x8U)
#define PMC_REGSC_ACKISO_SHIFT                   (3U)
#define PMC_REGSC_ACKISO(x)                      (((uint8_t)(((uint8_t)(x)) << PMC_REGSC_ACKISO_SHIFT)) & PMC_REGSC_ACKISO_MASK)
#define PMC_REGSC_BGEN_MASK                      (0x10U)
#define PMC_REGSC_BGEN_SHIFT                     (4U)
#define PMC_REGSC_BGEN(x)                        (((uint8_t)(((uint8_t)(x)) << PMC_REGSC_BGEN_SHIFT)) & PMC_REGSC_BGEN_MASK)


/*!
 * @}
 */ /* end of group PMC_Register_Masks */


/* PMC - Peripheral instance base addresses */
/** Peripheral PMC base address */
#define PMC_BASE                                 (0x4007D000u)
/** Peripheral PMC base pointer */
#define PMC                                      ((PMC_Type *)PMC_BASE)
/** Array initializer of PMC peripheral base addresses */
#define PMC_BASE_ADDRS                           { PMC_BASE }
/** Array initializer of PMC peripheral base pointers */
#define PMC_BASE_PTRS                            { PMC }
/** Interrupt vectors for the PMC peripheral type */
#define PMC_IRQS                                 { PMC_IRQn }

/*!
 * @}
 */ /* end of group PMC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- PORT Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Peripheral_Access_Layer PORT Peripheral Access Layer
 * @{
 */

/** PORT - Register Layout Typedef */
typedef struct {
  __IO uint32_t PCR[8];                            /**< Pin Control Register n, array offset: 0x0, array step: 0x4 */
       uint8_t RESERVED_0[96];
  __O  uint32_t GPCLR;                             /**< Global Pin Control Low Register, offset: 0x80 */
  __O  uint32_t GPCHR;                             /**< Global Pin Control High Register, offset: 0x84 */
       uint8_t RESERVED_1[24];
  __IO uint32_t ISFR;                              /**< Interrupt Status Flag Register, offset: 0xA0 */
       uint8_t RESERVED_2[28];
  __IO uint32_t DFER;                              /**< Digital Filter Enable Register, offset: 0xC0 */
  __IO uint32_t DFCR;                              /**< Digital Filter Clock Register, offset: 0xC4 */
  __IO uint32_t DFWR;                              /**< Digital Filter Width Register, offset: 0xC8 */
} PORT_Type;

/* ----------------------------------------------------------------------------
   -- PORT Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup PORT_Register_Masks PORT Register Masks
 * @{
 */

/*! @name PCR - Pin Control Register n */
#define PORT_PCR_PS_MASK                         (0x1U)
#define PORT_PCR_PS_SHIFT                        (0U)
#define PORT_PCR_PS(x)                           (((uint32_t)(((uint32_t)(x)) << PORT_PCR_PS_SHIFT)) & PORT_PCR_PS_MASK)
#define PORT_PCR_PE_MASK                         (0x2U)
#define PORT_PCR_PE_SHIFT                        (1U)
#define PORT_PCR_PE(x)                           (((uint32_t)(((uint32_t)(x)) << PORT_PCR_PE_SHIFT)) & PORT_PCR_PE_MASK)
#define PORT_PCR_SRE_MASK                        (0x4U)
#define PORT_PCR_SRE_SHIFT                       (2U)
#define PORT_PCR_SRE(x)                          (((uint32_t)(((uint32_t)(x)) << PORT_PCR_SRE_SHIFT)) & PORT_PCR_SRE_MASK)
#define PORT_PCR_ODE_MASK                        (0x20U)
#define PORT_PCR_ODE_SHIFT                       (5U)
#define PORT_PCR_ODE(x)                          (((uint32_t)(((uint32_t)(x)) << PORT_PCR_ODE_SHIFT)) & PORT_PCR_ODE_MASK)
#define PORT_PCR_MUX_MASK                        (0x700U)
#define PORT_PCR_MUX_SHIFT                       (8U)
#define PORT_PCR_MUX(x)                          (((uint32_t)(((uint32_t)(x)) << PORT_PCR_MUX_SHIFT)) & PORT_PCR_MUX_MASK)
#define PORT_PCR_LK_MASK                         (0x8000U)
#define PORT_PCR_LK_SHIFT                        (15U)
#define PORT_PCR_LK(x)                           (((uint32_t)(((uint32_t)(x)) << PORT_PCR_LK_SHIFT)) & PORT_PCR_LK_MASK)
#define PORT_PCR_IRQC_MASK                       (0xF0000U)
#define PORT_PCR_IRQC_SHIFT                      (16U)
#define PORT_PCR_IRQC(x)                         (((uint32_t)(((uint32_t)(x)) << PORT_PCR_IRQC_SHIFT)) & PORT_PCR_IRQC_MASK)
#define PORT_PCR_ISF_MASK                        (0x1000000U)
#define PORT_PCR_ISF_SHIFT                       (24U)
#define PORT_PCR_ISF(x)                          (((uint32_t)(((uint32_t)(x)) << PORT_PCR_ISF_SHIFT)) & PORT_PCR_ISF_MASK)

/* The count of PORT_PCR */
#define PORT_PCR_COUNT                           (8U)

/*! @name GPCLR - Global Pin Control Low Register */
#define PORT_GPCLR_GPWD_MASK                     (0xFFFFU)
#define PORT_GPCLR_GPWD_SHIFT                    (0U)
#define PORT_GPCLR_GPWD(x)                       (((uint32_t)(((uint32_t)(x)) << PORT_GPCLR_GPWD_SHIFT)) & PORT_GPCLR_GPWD_MASK)
#define PORT_GPCLR_GPWE_MASK                     (0xFFFF0000U)
#define PORT_GPCLR_GPWE_SHIFT                    (16U)
#define PORT_GPCLR_GPWE(x)                       (((uint32_t)(((uint32_t)(x)) << PORT_GPCLR_GPWE_SHIFT)) & PORT_GPCLR_GPWE_MASK)

/*! @name GPCHR - Global Pin Control High Register */
#define PORT_GPCHR_GPWD_MASK                     (0xFFFFU)
#define PORT_GPCHR_GPWD_SHIFT                    (0U)
#define PORT_GPCHR_GPWD(x)                       (((uint32_t)(((uint32_t)(x)) << PORT_GPCHR_GPWD_SHIFT)) & PORT_GPCHR_GPWD_MASK)
#define PORT_GPCHR_GPWE_MASK                     (0xFFFF0000U)
#define PORT_GPCHR_GPWE_SHIFT                    (16U)
#define PORT_GPCHR_GPWE(x)                       (((uint32_t)(((uint32_t)(x)) << PORT_GPCHR_GPWE_SHIFT)) & PORT_GPCHR_GPWE_MASK)

/*! @name ISFR - Interrupt Status Flag Register */
#define PORT_ISFR_ISF_MASK                       (0xFFFFFFFFU)
#define PORT_ISFR_ISF_SHIFT                      (0U)
#define PORT_ISFR_ISF(x)                         (((uint32_t)(((uint32_t)(x)) << PORT_ISFR_ISF_SHIFT)) & PORT_ISFR_ISF_MASK)

/*! @name DFER - Digital Filter Enable Register */
#define PORT_DFER_DFE_MASK                       (0xFFFFFFFFU)
#define PORT_DFER_DFE_SHIFT                      (0U)
#define PORT_DFER_DFE(x)                         (((uint32_t)(((uint32_t)(x)) << PORT_DFER_DFE_SHIFT)) & PORT_DFER_DFE_MASK)

/*! @name DFCR - Digital Filter Clock Register */
#define PORT_DFCR_CS_MASK                        (0x1U)
#define PORT_DFCR_CS_SHIFT                       (0U)
#define PORT_DFCR_CS(x)                          (((uint32_t)(((uint32_t)(x)) << PORT_DFCR_CS_SHIFT)) & PORT_DFCR_CS_MASK)

/*! @name DFWR - Digital Filter Width Register */
#define PORT_DFWR_FILT_MASK                      (0x1FU)
#define PORT_DFWR_FILT_SHIFT                     (0U)
#define PORT_DFWR_FILT(x)                        (((uint32_t)(((uint32_t)(x)) << PORT_DFWR_FILT_SHIFT)) & PORT_DFWR_FILT_MASK)


/*!
 * @}
 */ /* end of group PORT_Register_Masks */


/* PORT - Peripheral instance base addresses */
/** Peripheral PORTA base address */
#define PORTA_BASE                               (0x40046000u)
/** Peripheral PORTA base pointer */
#define PORTA                                    ((PORT_Type *)PORTA_BASE)
/** Peripheral PORTB base address */
#define PORTB_BASE                               (0x40047000u)
/** Peripheral PORTB base pointer */
#define PORTB                                    ((PORT_Type *)PORTB_BASE)
/** Peripheral PORTC base address */
#define PORTC_BASE                               (0x40048000u)
/** Peripheral PORTC base pointer */
#define PORTC                                    ((PORT_Type *)PORTC_BASE)
/** Peripheral PORTD base address */
#define PORTD_BASE                               (0x40049000u)
/** Peripheral PORTD base pointer */
#define PORTD                                    ((PORT_Type *)PORTD_BASE)
/** Peripheral PORTE base address */
#define PORTE_BASE                               (0x4004A000u)
/** Peripheral PORTE base pointer */
#define PORTE                                    ((PORT_Type *)PORTE_BASE)
/** Peripheral PORTF base address */
#define PORTF_BASE                               (0x4004B000u)
/** Peripheral PORTF base pointer */
#define PORTF                                    ((PORT_Type *)PORTF_BASE)
/** Peripheral PORTG base address */
#define PORTG_BASE                               (0x4004C000u)
/** Peripheral PORTG base pointer */
#define PORTG                                    ((PORT_Type *)PORTG_BASE)
/** Peripheral PORTH base address */
#define PORTH_BASE                               (0x4004D000u)
/** Peripheral PORTH base pointer */
#define PORTH                                    ((PORT_Type *)PORTH_BASE)
/** Peripheral PORTI base address */
#define PORTI_BASE                               (0x4004E000u)
/** Peripheral PORTI base pointer */
#define PORTI                                    ((PORT_Type *)PORTI_BASE)
/** Peripheral PORTJ base address */
#define PORTJ_BASE                               (0x40037000u)
/** Peripheral PORTJ base pointer */
#define PORTJ                                    ((PORT_Type *)PORTJ_BASE)
/** Peripheral PORTK base address */
#define PORTK_BASE                               (0x40038000u)
/** Peripheral PORTK base pointer */
#define PORTK                                    ((PORT_Type *)PORTK_BASE)
/** Peripheral PORTL base address */
#define PORTL_BASE                               (0x40039000u)
/** Peripheral PORTL base pointer */
#define PORTL                                    ((PORT_Type *)PORTL_BASE)
/** Peripheral PORTM base address */
#define PORTM_BASE                               (0x4003A000u)
/** Peripheral PORTM base pointer */
#define PORTM                                    ((PORT_Type *)PORTM_BASE)
/** Array initializer of PORT peripheral base addresses */
#define PORT_BASE_ADDRS                          { PORTA_BASE, PORTB_BASE, PORTC_BASE, PORTD_BASE, PORTE_BASE, PORTF_BASE, PORTG_BASE, PORTH_BASE, PORTI_BASE, PORTJ_BASE, PORTK_BASE, PORTL_BASE, PORTM_BASE }
/** Array initializer of PORT peripheral base pointers */
#define PORT_BASE_PTRS                           { PORTA, PORTB, PORTC, PORTD, PORTE, PORTF, PORTG, PORTH, PORTI, PORTJ, PORTK, PORTL, PORTM }
/** Interrupt vectors for the PORT peripheral type */
#define PORT_IRQS                                { PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn, PTx_IRQn }

/*!
 * @}
 */ /* end of group PORT_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- RCM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Peripheral_Access_Layer RCM Peripheral Access Layer
 * @{
 */

/** RCM - Register Layout Typedef */
typedef struct {
  __I  uint8_t SRS0;                               /**< System Reset Status Register 0, offset: 0x0 */
  __I  uint8_t SRS1;                               /**< System Reset Status Register 1, offset: 0x1 */
       uint8_t RESERVED_0[2];
  __IO uint8_t RPFC;                               /**< Reset Pin Filter Control register, offset: 0x4 */
  __IO uint8_t RPFW;                               /**< Reset Pin Filter Width register, offset: 0x5 */
       uint8_t RESERVED_1[2];
  __IO uint8_t SSRS0;                              /**< Sticky System Reset Status Register 0, offset: 0x8 */
  __IO uint8_t SSRS1;                              /**< Sticky System Reset Status Register 1, offset: 0x9 */
} RCM_Type;

/* ----------------------------------------------------------------------------
   -- RCM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RCM_Register_Masks RCM Register Masks
 * @{
 */

/*! @name SRS0 - System Reset Status Register 0 */
#define RCM_SRS0_WAKEUP_MASK                     (0x1U)
#define RCM_SRS0_WAKEUP_SHIFT                    (0U)
#define RCM_SRS0_WAKEUP(x)                       (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_WAKEUP_SHIFT)) & RCM_SRS0_WAKEUP_MASK)
#define RCM_SRS0_LVD_MASK                        (0x2U)
#define RCM_SRS0_LVD_SHIFT                       (1U)
#define RCM_SRS0_LVD(x)                          (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_LVD_SHIFT)) & RCM_SRS0_LVD_MASK)
#define RCM_SRS0_LOC_MASK                        (0x4U)
#define RCM_SRS0_LOC_SHIFT                       (2U)
#define RCM_SRS0_LOC(x)                          (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_LOC_SHIFT)) & RCM_SRS0_LOC_MASK)
#define RCM_SRS0_LOL_MASK                        (0x8U)
#define RCM_SRS0_LOL_SHIFT                       (3U)
#define RCM_SRS0_LOL(x)                          (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_LOL_SHIFT)) & RCM_SRS0_LOL_MASK)
#define RCM_SRS0_WDOG_MASK                       (0x20U)
#define RCM_SRS0_WDOG_SHIFT                      (5U)
#define RCM_SRS0_WDOG(x)                         (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_WDOG_SHIFT)) & RCM_SRS0_WDOG_MASK)
#define RCM_SRS0_PIN_MASK                        (0x40U)
#define RCM_SRS0_PIN_SHIFT                       (6U)
#define RCM_SRS0_PIN(x)                          (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_PIN_SHIFT)) & RCM_SRS0_PIN_MASK)
#define RCM_SRS0_POR_MASK                        (0x80U)
#define RCM_SRS0_POR_SHIFT                       (7U)
#define RCM_SRS0_POR(x)                          (((uint8_t)(((uint8_t)(x)) << RCM_SRS0_POR_SHIFT)) & RCM_SRS0_POR_MASK)

/*! @name SRS1 - System Reset Status Register 1 */
#define RCM_SRS1_LOCKUP_MASK                     (0x2U)
#define RCM_SRS1_LOCKUP_SHIFT                    (1U)
#define RCM_SRS1_LOCKUP(x)                       (((uint8_t)(((uint8_t)(x)) << RCM_SRS1_LOCKUP_SHIFT)) & RCM_SRS1_LOCKUP_MASK)
#define RCM_SRS1_SW_MASK                         (0x4U)
#define RCM_SRS1_SW_SHIFT                        (2U)
#define RCM_SRS1_SW(x)                           (((uint8_t)(((uint8_t)(x)) << RCM_SRS1_SW_SHIFT)) & RCM_SRS1_SW_MASK)
#define RCM_SRS1_MDM_AP_MASK                     (0x8U)
#define RCM_SRS1_MDM_AP_SHIFT                    (3U)
#define RCM_SRS1_MDM_AP(x)                       (((uint8_t)(((uint8_t)(x)) << RCM_SRS1_MDM_AP_SHIFT)) & RCM_SRS1_MDM_AP_MASK)
#define RCM_SRS1_SACKERR_MASK                    (0x20U)
#define RCM_SRS1_SACKERR_SHIFT                   (5U)
#define RCM_SRS1_SACKERR(x)                      (((uint8_t)(((uint8_t)(x)) << RCM_SRS1_SACKERR_SHIFT)) & RCM_SRS1_SACKERR_MASK)

/*! @name RPFC - Reset Pin Filter Control register */
#define RCM_RPFC_RSTFLTSRW_MASK                  (0x3U)
#define RCM_RPFC_RSTFLTSRW_SHIFT                 (0U)
#define RCM_RPFC_RSTFLTSRW(x)                    (((uint8_t)(((uint8_t)(x)) << RCM_RPFC_RSTFLTSRW_SHIFT)) & RCM_RPFC_RSTFLTSRW_MASK)
#define RCM_RPFC_RSTFLTSS_MASK                   (0x4U)
#define RCM_RPFC_RSTFLTSS_SHIFT                  (2U)
#define RCM_RPFC_RSTFLTSS(x)                     (((uint8_t)(((uint8_t)(x)) << RCM_RPFC_RSTFLTSS_SHIFT)) & RCM_RPFC_RSTFLTSS_MASK)

/*! @name RPFW - Reset Pin Filter Width register */
#define RCM_RPFW_RSTFLTSEL_MASK                  (0x1FU)
#define RCM_RPFW_RSTFLTSEL_SHIFT                 (0U)
#define RCM_RPFW_RSTFLTSEL(x)                    (((uint8_t)(((uint8_t)(x)) << RCM_RPFW_RSTFLTSEL_SHIFT)) & RCM_RPFW_RSTFLTSEL_MASK)

/*! @name SSRS0 - Sticky System Reset Status Register 0 */
#define RCM_SSRS0_SWAKEUP_MASK                   (0x1U)
#define RCM_SSRS0_SWAKEUP_SHIFT                  (0U)
#define RCM_SSRS0_SWAKEUP(x)                     (((uint8_t)(((uint8_t)(x)) << RCM_SSRS0_SWAKEUP_SHIFT)) & RCM_SSRS0_SWAKEUP_MASK)
#define RCM_SSRS0_SLVD_MASK                      (0x2U)
#define RCM_SSRS0_SLVD_SHIFT                     (1U)
#define RCM_SSRS0_SLVD(x)                        (((uint8_t)(((uint8_t)(x)) << RCM_SSRS0_SLVD_SHIFT)) & RCM_SSRS0_SLVD_MASK)
#define RCM_SSRS0_SLOL_MASK                      (0x8U)
#define RCM_SSRS0_SLOL_SHIFT                     (3U)
#define RCM_SSRS0_SLOL(x)                        (((uint8_t)(((uint8_t)(x)) << RCM_SSRS0_SLOL_SHIFT)) & RCM_SSRS0_SLOL_MASK)
#define RCM_SSRS0_SWDOG_MASK                     (0x20U)
#define RCM_SSRS0_SWDOG_SHIFT                    (5U)
#define RCM_SSRS0_SWDOG(x)                       (((uint8_t)(((uint8_t)(x)) << RCM_SSRS0_SWDOG_SHIFT)) & RCM_SSRS0_SWDOG_MASK)
#define RCM_SSRS0_SPIN_MASK                      (0x40U)
#define RCM_SSRS0_SPIN_SHIFT                     (6U)
#define RCM_SSRS0_SPIN(x)                        (((uint8_t)(((uint8_t)(x)) << RCM_SSRS0_SPIN_SHIFT)) & RCM_SSRS0_SPIN_MASK)
#define RCM_SSRS0_SPOR_MASK                      (0x80U)
#define RCM_SSRS0_SPOR_SHIFT                     (7U)
#define RCM_SSRS0_SPOR(x)                        (((uint8_t)(((uint8_t)(x)) << RCM_SSRS0_SPOR_SHIFT)) & RCM_SSRS0_SPOR_MASK)

/*! @name SSRS1 - Sticky System Reset Status Register 1 */
#define RCM_SSRS1_SLOCKUP_MASK                   (0x2U)
#define RCM_SSRS1_SLOCKUP_SHIFT                  (1U)
#define RCM_SSRS1_SLOCKUP(x)                     (((uint8_t)(((uint8_t)(x)) << RCM_SSRS1_SLOCKUP_SHIFT)) & RCM_SSRS1_SLOCKUP_MASK)
#define RCM_SSRS1_SSW_MASK                       (0x4U)
#define RCM_SSRS1_SSW_SHIFT                      (2U)
#define RCM_SSRS1_SSW(x)                         (((uint8_t)(((uint8_t)(x)) << RCM_SSRS1_SSW_SHIFT)) & RCM_SSRS1_SSW_MASK)
#define RCM_SSRS1_SMDM_AP_MASK                   (0x8U)
#define RCM_SSRS1_SMDM_AP_SHIFT                  (3U)
#define RCM_SSRS1_SMDM_AP(x)                     (((uint8_t)(((uint8_t)(x)) << RCM_SSRS1_SMDM_AP_SHIFT)) & RCM_SSRS1_SMDM_AP_MASK)
#define RCM_SSRS1_SSACKERR_MASK                  (0x20U)
#define RCM_SSRS1_SSACKERR_SHIFT                 (5U)
#define RCM_SSRS1_SSACKERR(x)                    (((uint8_t)(((uint8_t)(x)) << RCM_SSRS1_SSACKERR_SHIFT)) & RCM_SSRS1_SSACKERR_MASK)


/*!
 * @}
 */ /* end of group RCM_Register_Masks */


/* RCM - Peripheral instance base addresses */
/** Peripheral RCM base address */
#define RCM_BASE                                 (0x4007B000u)
/** Peripheral RCM base pointer */
#define RCM                                      ((RCM_Type *)RCM_BASE)
/** Array initializer of RCM peripheral base addresses */
#define RCM_BASE_ADDRS                           { RCM_BASE }
/** Array initializer of RCM peripheral base pointers */
#define RCM_BASE_PTRS                            { RCM }

/*!
 * @}
 */ /* end of group RCM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- RNG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RNG_Peripheral_Access_Layer RNG Peripheral Access Layer
 * @{
 */

/** RNG - Register Layout Typedef */
typedef struct {
  __IO uint32_t CR;                                /**< RNGA Control Register, offset: 0x0 */
  __I  uint32_t SR;                                /**< RNGA Status Register, offset: 0x4 */
  __O  uint32_t ER;                                /**< RNGA Entropy Register, offset: 0x8 */
  __I  uint32_t OR;                                /**< RNGA Output Register, offset: 0xC */
} RNG_Type;

/* ----------------------------------------------------------------------------
   -- RNG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RNG_Register_Masks RNG Register Masks
 * @{
 */

/*! @name CR - RNGA Control Register */
#define RNG_CR_GO_MASK                           (0x1U)
#define RNG_CR_GO_SHIFT                          (0U)
#define RNG_CR_GO(x)                             (((uint32_t)(((uint32_t)(x)) << RNG_CR_GO_SHIFT)) & RNG_CR_GO_MASK)
#define RNG_CR_HA_MASK                           (0x2U)
#define RNG_CR_HA_SHIFT                          (1U)
#define RNG_CR_HA(x)                             (((uint32_t)(((uint32_t)(x)) << RNG_CR_HA_SHIFT)) & RNG_CR_HA_MASK)
#define RNG_CR_INTM_MASK                         (0x4U)
#define RNG_CR_INTM_SHIFT                        (2U)
#define RNG_CR_INTM(x)                           (((uint32_t)(((uint32_t)(x)) << RNG_CR_INTM_SHIFT)) & RNG_CR_INTM_MASK)
#define RNG_CR_CLRI_MASK                         (0x8U)
#define RNG_CR_CLRI_SHIFT                        (3U)
#define RNG_CR_CLRI(x)                           (((uint32_t)(((uint32_t)(x)) << RNG_CR_CLRI_SHIFT)) & RNG_CR_CLRI_MASK)
#define RNG_CR_SLP_MASK                          (0x10U)
#define RNG_CR_SLP_SHIFT                         (4U)
#define RNG_CR_SLP(x)                            (((uint32_t)(((uint32_t)(x)) << RNG_CR_SLP_SHIFT)) & RNG_CR_SLP_MASK)

/*! @name SR - RNGA Status Register */
#define RNG_SR_SECV_MASK                         (0x1U)
#define RNG_SR_SECV_SHIFT                        (0U)
#define RNG_SR_SECV(x)                           (((uint32_t)(((uint32_t)(x)) << RNG_SR_SECV_SHIFT)) & RNG_SR_SECV_MASK)
#define RNG_SR_LRS_MASK                          (0x2U)
#define RNG_SR_LRS_SHIFT                         (1U)
#define RNG_SR_LRS(x)                            (((uint32_t)(((uint32_t)(x)) << RNG_SR_LRS_SHIFT)) & RNG_SR_LRS_MASK)
#define RNG_SR_ORU_MASK                          (0x4U)
#define RNG_SR_ORU_SHIFT                         (2U)
#define RNG_SR_ORU(x)                            (((uint32_t)(((uint32_t)(x)) << RNG_SR_ORU_SHIFT)) & RNG_SR_ORU_MASK)
#define RNG_SR_ERRI_MASK                         (0x8U)
#define RNG_SR_ERRI_SHIFT                        (3U)
#define RNG_SR_ERRI(x)                           (((uint32_t)(((uint32_t)(x)) << RNG_SR_ERRI_SHIFT)) & RNG_SR_ERRI_MASK)
#define RNG_SR_SLP_MASK                          (0x10U)
#define RNG_SR_SLP_SHIFT                         (4U)
#define RNG_SR_SLP(x)                            (((uint32_t)(((uint32_t)(x)) << RNG_SR_SLP_SHIFT)) & RNG_SR_SLP_MASK)
#define RNG_SR_OREG_LVL_MASK                     (0xFF00U)
#define RNG_SR_OREG_LVL_SHIFT                    (8U)
#define RNG_SR_OREG_LVL(x)                       (((uint32_t)(((uint32_t)(x)) << RNG_SR_OREG_LVL_SHIFT)) & RNG_SR_OREG_LVL_MASK)
#define RNG_SR_OREG_SIZE_MASK                    (0xFF0000U)
#define RNG_SR_OREG_SIZE_SHIFT                   (16U)
#define RNG_SR_OREG_SIZE(x)                      (((uint32_t)(((uint32_t)(x)) << RNG_SR_OREG_SIZE_SHIFT)) & RNG_SR_OREG_SIZE_MASK)

/*! @name ER - RNGA Entropy Register */
#define RNG_ER_EXT_ENT_MASK                      (0xFFFFFFFFU)
#define RNG_ER_EXT_ENT_SHIFT                     (0U)
#define RNG_ER_EXT_ENT(x)                        (((uint32_t)(((uint32_t)(x)) << RNG_ER_EXT_ENT_SHIFT)) & RNG_ER_EXT_ENT_MASK)

/*! @name OR - RNGA Output Register */
#define RNG_OR_RANDOUT_MASK                      (0xFFFFFFFFU)
#define RNG_OR_RANDOUT_SHIFT                     (0U)
#define RNG_OR_RANDOUT(x)                        (((uint32_t)(((uint32_t)(x)) << RNG_OR_RANDOUT_SHIFT)) & RNG_OR_RANDOUT_MASK)


/*!
 * @}
 */ /* end of group RNG_Register_Masks */


/* RNG - Peripheral instance base addresses */
/** Peripheral RNG base address */
#define RNG_BASE                                 (0x40029000u)
/** Peripheral RNG base pointer */
#define RNG                                      ((RNG_Type *)RNG_BASE)
/** Array initializer of RNG peripheral base addresses */
#define RNG_BASE_ADDRS                           { RNG_BASE }
/** Array initializer of RNG peripheral base pointers */
#define RNG_BASE_PTRS                            { RNG }
/** Interrupt vectors for the RNG peripheral type */
#define RNG_IRQS                                 { RNGA_IRQn }

/*!
 * @}
 */ /* end of group RNG_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- ROM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Peripheral_Access_Layer ROM Peripheral Access Layer
 * @{
 */

/** ROM - Register Layout Typedef */
typedef struct {
  __I  uint32_t ENTRY[3];                          /**< Entry, array offset: 0x0, array step: 0x4 */
  __I  uint32_t TABLEMARK;                         /**< End of Table Marker Register, offset: 0xC */
       uint8_t RESERVED_0[4028];
  __I  uint32_t SYSACCESS;                         /**< System Access Register, offset: 0xFCC */
  __I  uint32_t PERIPHID4;                         /**< Peripheral ID Register, offset: 0xFD0 */
  __I  uint32_t PERIPHID5;                         /**< Peripheral ID Register, offset: 0xFD4 */
  __I  uint32_t PERIPHID6;                         /**< Peripheral ID Register, offset: 0xFD8 */
  __I  uint32_t PERIPHID7;                         /**< Peripheral ID Register, offset: 0xFDC */
  __I  uint32_t PERIPHID0;                         /**< Peripheral ID Register, offset: 0xFE0 */
  __I  uint32_t PERIPHID1;                         /**< Peripheral ID Register, offset: 0xFE4 */
  __I  uint32_t PERIPHID2;                         /**< Peripheral ID Register, offset: 0xFE8 */
  __I  uint32_t PERIPHID3;                         /**< Peripheral ID Register, offset: 0xFEC */
  __I  uint32_t COMPID[4];                         /**< Component ID Register, array offset: 0xFF0, array step: 0x4 */
} ROM_Type;

/* ----------------------------------------------------------------------------
   -- ROM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup ROM_Register_Masks ROM Register Masks
 * @{
 */

/*! @name ENTRY - Entry */
#define ROM_ENTRY_ENTRY_MASK                     (0xFFFFFFFFU)
#define ROM_ENTRY_ENTRY_SHIFT                    (0U)
#define ROM_ENTRY_ENTRY(x)                       (((uint32_t)(((uint32_t)(x)) << ROM_ENTRY_ENTRY_SHIFT)) & ROM_ENTRY_ENTRY_MASK)

/* The count of ROM_ENTRY */
#define ROM_ENTRY_COUNT                          (3U)

/*! @name TABLEMARK - End of Table Marker Register */
#define ROM_TABLEMARK_MARK_MASK                  (0xFFFFFFFFU)
#define ROM_TABLEMARK_MARK_SHIFT                 (0U)
#define ROM_TABLEMARK_MARK(x)                    (((uint32_t)(((uint32_t)(x)) << ROM_TABLEMARK_MARK_SHIFT)) & ROM_TABLEMARK_MARK_MASK)

/*! @name SYSACCESS - System Access Register */
#define ROM_SYSACCESS_SYSACCESS_MASK             (0xFFFFFFFFU)
#define ROM_SYSACCESS_SYSACCESS_SHIFT            (0U)
#define ROM_SYSACCESS_SYSACCESS(x)               (((uint32_t)(((uint32_t)(x)) << ROM_SYSACCESS_SYSACCESS_SHIFT)) & ROM_SYSACCESS_SYSACCESS_MASK)

/*! @name PERIPHID4 - Peripheral ID Register */
#define ROM_PERIPHID4_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID4_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID4_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID4_PERIPHID_SHIFT)) & ROM_PERIPHID4_PERIPHID_MASK)

/*! @name PERIPHID5 - Peripheral ID Register */
#define ROM_PERIPHID5_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID5_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID5_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID5_PERIPHID_SHIFT)) & ROM_PERIPHID5_PERIPHID_MASK)

/*! @name PERIPHID6 - Peripheral ID Register */
#define ROM_PERIPHID6_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID6_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID6_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID6_PERIPHID_SHIFT)) & ROM_PERIPHID6_PERIPHID_MASK)

/*! @name PERIPHID7 - Peripheral ID Register */
#define ROM_PERIPHID7_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID7_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID7_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID7_PERIPHID_SHIFT)) & ROM_PERIPHID7_PERIPHID_MASK)

/*! @name PERIPHID0 - Peripheral ID Register */
#define ROM_PERIPHID0_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID0_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID0_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID0_PERIPHID_SHIFT)) & ROM_PERIPHID0_PERIPHID_MASK)

/*! @name PERIPHID1 - Peripheral ID Register */
#define ROM_PERIPHID1_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID1_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID1_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID1_PERIPHID_SHIFT)) & ROM_PERIPHID1_PERIPHID_MASK)

/*! @name PERIPHID2 - Peripheral ID Register */
#define ROM_PERIPHID2_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID2_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID2_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID2_PERIPHID_SHIFT)) & ROM_PERIPHID2_PERIPHID_MASK)

/*! @name PERIPHID3 - Peripheral ID Register */
#define ROM_PERIPHID3_PERIPHID_MASK              (0xFFFFFFFFU)
#define ROM_PERIPHID3_PERIPHID_SHIFT             (0U)
#define ROM_PERIPHID3_PERIPHID(x)                (((uint32_t)(((uint32_t)(x)) << ROM_PERIPHID3_PERIPHID_SHIFT)) & ROM_PERIPHID3_PERIPHID_MASK)

/*! @name COMPID - Component ID Register */
#define ROM_COMPID_COMPID_MASK                   (0xFFFFFFFFU)
#define ROM_COMPID_COMPID_SHIFT                  (0U)
#define ROM_COMPID_COMPID(x)                     (((uint32_t)(((uint32_t)(x)) << ROM_COMPID_COMPID_SHIFT)) & ROM_COMPID_COMPID_MASK)

/* The count of ROM_COMPID */
#define ROM_COMPID_COUNT                         (4U)


/*!
 * @}
 */ /* end of group ROM_Register_Masks */


/* ROM - Peripheral instance base addresses */
/** Peripheral ROM base address */
#define ROM_BASE                                 (0xF0002000u)
/** Peripheral ROM base pointer */
#define ROM                                      ((ROM_Type *)ROM_BASE)
/** Array initializer of ROM peripheral base addresses */
#define ROM_BASE_ADDRS                           { ROM_BASE }
/** Array initializer of ROM peripheral base pointers */
#define ROM_BASE_PTRS                            { ROM }

/*!
 * @}
 */ /* end of group ROM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- RTC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Peripheral_Access_Layer RTC Peripheral Access Layer
 * @{
 */

/** RTC - Register Layout Typedef */
typedef struct {
  __IO uint16_t YEARMON;                           /**< RTC Year and Month Counters Register, offset: 0x0 */
  __IO uint16_t DAYS;                              /**< RTC Days and Day-of-Week Counters Register, offset: 0x2 */
  __IO uint16_t HOURMIN;                           /**< RTC Hours and Minutes Counters Register, offset: 0x4 */
  __IO uint16_t SECONDS;                           /**< RTC Seconds Counters Register, offset: 0x6 */
  __IO uint16_t ALM_YEARMON;                       /**< RTC Year and Months Alarm Register, offset: 0x8 */
  __IO uint16_t ALM_DAYS;                          /**< RTC Days Alarm Register, offset: 0xA */
  __IO uint16_t ALM_HOURMIN;                       /**< RTC Hours and Minutes Alarm Register, offset: 0xC */
  __IO uint16_t ALM_SECONDS;                       /**< RTC Seconds Alarm Register, offset: 0xE */
  __IO uint16_t CTRL;                              /**< RTC Control Register, offset: 0x10 */
  __IO uint16_t STATUS;                            /**< RTC Status Register, offset: 0x12 */
  __IO uint16_t ISR;                               /**< RTC Interrupt Status Register, offset: 0x14 */
  __IO uint16_t IER;                               /**< RTC Interrupt Enable Register, offset: 0x16 */
       uint8_t RESERVED_0[8];
  __IO uint16_t GP_DATA_REG;                       /**< RTC General Purpose Data Register, offset: 0x20 */
  __IO uint16_t DST_HOUR;                          /**< RTC Daylight Saving Hour Register, offset: 0x22 */
  __IO uint16_t DST_MONTH;                         /**< RTC Daylight Saving Month Register, offset: 0x24 */
  __IO uint16_t DST_DAY;                           /**< RTC Daylight Saving Day Register, offset: 0x26 */
  __IO uint16_t COMPEN;                            /**< RTC Compensation Register, offset: 0x28 */
       uint8_t RESERVED_1[8];
  __IO uint16_t TAMPER_SCR;                        /**< RTC Tamper Status and Control Register, offset: 0x32 */
  __IO uint16_t FILTER01_CFG;                      /**< RTC Tamper 0 1 Filter Configuration Register, offset: 0x34 */
  __IO uint16_t FILTER2_CFG;                       /**< RTC Tamper 2 Filter Configuration Register, offset: 0x36 */
       uint8_t RESERVED_2[10];
  __IO uint16_t CTRL2;                             /**< RTC Control 2 Register, offset: 0x42 */
} RTC_Type;

/* ----------------------------------------------------------------------------
   -- RTC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup RTC_Register_Masks RTC Register Masks
 * @{
 */

/*! @name YEARMON - RTC Year and Month Counters Register */
#define RTC_YEARMON_MON_CNT_MASK                 (0xFU)
#define RTC_YEARMON_MON_CNT_SHIFT                (0U)
#define RTC_YEARMON_MON_CNT(x)                   (((uint16_t)(((uint16_t)(x)) << RTC_YEARMON_MON_CNT_SHIFT)) & RTC_YEARMON_MON_CNT_MASK)
#define RTC_YEARMON_YROFST_MASK                  (0xFF00U)
#define RTC_YEARMON_YROFST_SHIFT                 (8U)
#define RTC_YEARMON_YROFST(x)                    (((uint16_t)(((uint16_t)(x)) << RTC_YEARMON_YROFST_SHIFT)) & RTC_YEARMON_YROFST_MASK)

/*! @name DAYS - RTC Days and Day-of-Week Counters Register */
#define RTC_DAYS_DAY_CNT_MASK                    (0x1FU)
#define RTC_DAYS_DAY_CNT_SHIFT                   (0U)
#define RTC_DAYS_DAY_CNT(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_DAYS_DAY_CNT_SHIFT)) & RTC_DAYS_DAY_CNT_MASK)
#define RTC_DAYS_DOW_MASK                        (0x700U)
#define RTC_DAYS_DOW_SHIFT                       (8U)
#define RTC_DAYS_DOW(x)                          (((uint16_t)(((uint16_t)(x)) << RTC_DAYS_DOW_SHIFT)) & RTC_DAYS_DOW_MASK)

/*! @name HOURMIN - RTC Hours and Minutes Counters Register */
#define RTC_HOURMIN_MIN_CNT_MASK                 (0x3FU)
#define RTC_HOURMIN_MIN_CNT_SHIFT                (0U)
#define RTC_HOURMIN_MIN_CNT(x)                   (((uint16_t)(((uint16_t)(x)) << RTC_HOURMIN_MIN_CNT_SHIFT)) & RTC_HOURMIN_MIN_CNT_MASK)
#define RTC_HOURMIN_HOUR_CNT_MASK                (0x1F00U)
#define RTC_HOURMIN_HOUR_CNT_SHIFT               (8U)
#define RTC_HOURMIN_HOUR_CNT(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_HOURMIN_HOUR_CNT_SHIFT)) & RTC_HOURMIN_HOUR_CNT_MASK)

/*! @name SECONDS - RTC Seconds Counters Register */
#define RTC_SECONDS_SEC_CNT_MASK                 (0x3FU)
#define RTC_SECONDS_SEC_CNT_SHIFT                (0U)
#define RTC_SECONDS_SEC_CNT(x)                   (((uint16_t)(((uint16_t)(x)) << RTC_SECONDS_SEC_CNT_SHIFT)) & RTC_SECONDS_SEC_CNT_MASK)

/*! @name ALM_YEARMON - RTC Year and Months Alarm Register */
#define RTC_ALM_YEARMON_ALM_MON_MASK             (0xFU)
#define RTC_ALM_YEARMON_ALM_MON_SHIFT            (0U)
#define RTC_ALM_YEARMON_ALM_MON(x)               (((uint16_t)(((uint16_t)(x)) << RTC_ALM_YEARMON_ALM_MON_SHIFT)) & RTC_ALM_YEARMON_ALM_MON_MASK)
#define RTC_ALM_YEARMON_ALM_YEAR_MASK            (0xFF00U)
#define RTC_ALM_YEARMON_ALM_YEAR_SHIFT           (8U)
#define RTC_ALM_YEARMON_ALM_YEAR(x)              (((uint16_t)(((uint16_t)(x)) << RTC_ALM_YEARMON_ALM_YEAR_SHIFT)) & RTC_ALM_YEARMON_ALM_YEAR_MASK)

/*! @name ALM_DAYS - RTC Days Alarm Register */
#define RTC_ALM_DAYS_ALM_DAY_MASK                (0x1FU)
#define RTC_ALM_DAYS_ALM_DAY_SHIFT               (0U)
#define RTC_ALM_DAYS_ALM_DAY(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_ALM_DAYS_ALM_DAY_SHIFT)) & RTC_ALM_DAYS_ALM_DAY_MASK)

/*! @name ALM_HOURMIN - RTC Hours and Minutes Alarm Register */
#define RTC_ALM_HOURMIN_ALM_MIN_MASK             (0x3FU)
#define RTC_ALM_HOURMIN_ALM_MIN_SHIFT            (0U)
#define RTC_ALM_HOURMIN_ALM_MIN(x)               (((uint16_t)(((uint16_t)(x)) << RTC_ALM_HOURMIN_ALM_MIN_SHIFT)) & RTC_ALM_HOURMIN_ALM_MIN_MASK)
#define RTC_ALM_HOURMIN_ALM_HOUR_MASK            (0x1F00U)
#define RTC_ALM_HOURMIN_ALM_HOUR_SHIFT           (8U)
#define RTC_ALM_HOURMIN_ALM_HOUR(x)              (((uint16_t)(((uint16_t)(x)) << RTC_ALM_HOURMIN_ALM_HOUR_SHIFT)) & RTC_ALM_HOURMIN_ALM_HOUR_MASK)

/*! @name ALM_SECONDS - RTC Seconds Alarm Register */
#define RTC_ALM_SECONDS_ALM_SEC_MASK             (0x3FU)
#define RTC_ALM_SECONDS_ALM_SEC_SHIFT            (0U)
#define RTC_ALM_SECONDS_ALM_SEC(x)               (((uint16_t)(((uint16_t)(x)) << RTC_ALM_SECONDS_ALM_SEC_SHIFT)) & RTC_ALM_SECONDS_ALM_SEC_MASK)
#define RTC_ALM_SECONDS_DEC_SEC_MASK             (0x100U)
#define RTC_ALM_SECONDS_DEC_SEC_SHIFT            (8U)
#define RTC_ALM_SECONDS_DEC_SEC(x)               (((uint16_t)(((uint16_t)(x)) << RTC_ALM_SECONDS_DEC_SEC_SHIFT)) & RTC_ALM_SECONDS_DEC_SEC_MASK)
#define RTC_ALM_SECONDS_INC_SEC_MASK             (0x200U)
#define RTC_ALM_SECONDS_INC_SEC_SHIFT            (9U)
#define RTC_ALM_SECONDS_INC_SEC(x)               (((uint16_t)(((uint16_t)(x)) << RTC_ALM_SECONDS_INC_SEC_SHIFT)) & RTC_ALM_SECONDS_INC_SEC_MASK)

/*! @name CTRL - RTC Control Register */
#define RTC_CTRL_FINEEN_MASK                     (0x1U)
#define RTC_CTRL_FINEEN_SHIFT                    (0U)
#define RTC_CTRL_FINEEN(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_FINEEN_SHIFT)) & RTC_CTRL_FINEEN_MASK)
#define RTC_CTRL_COMP_EN_MASK                    (0x2U)
#define RTC_CTRL_COMP_EN_SHIFT                   (1U)
#define RTC_CTRL_COMP_EN(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_COMP_EN_SHIFT)) & RTC_CTRL_COMP_EN_MASK)
#define RTC_CTRL_ALM_MATCH_MASK                  (0xCU)
#define RTC_CTRL_ALM_MATCH_SHIFT                 (2U)
#define RTC_CTRL_ALM_MATCH(x)                    (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_ALM_MATCH_SHIFT)) & RTC_CTRL_ALM_MATCH_MASK)
#define RTC_CTRL_TIMER_STB_MASK_MASK             (0x10U)
#define RTC_CTRL_TIMER_STB_MASK_SHIFT            (4U)
#define RTC_CTRL_TIMER_STB_MASK(x)               (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_TIMER_STB_MASK_SHIFT)) & RTC_CTRL_TIMER_STB_MASK_MASK)
#define RTC_CTRL_DST_EN_MASK                     (0x40U)
#define RTC_CTRL_DST_EN_SHIFT                    (6U)
#define RTC_CTRL_DST_EN(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_DST_EN_SHIFT)) & RTC_CTRL_DST_EN_MASK)
#define RTC_CTRL_SWR_MASK                        (0x100U)
#define RTC_CTRL_SWR_SHIFT                       (8U)
#define RTC_CTRL_SWR(x)                          (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_SWR_SHIFT)) & RTC_CTRL_SWR_MASK)
#define RTC_CTRL_CLKOUT_MASK                     (0x6000U)
#define RTC_CTRL_CLKOUT_SHIFT                    (13U)
#define RTC_CTRL_CLKOUT(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_CTRL_CLKOUT_SHIFT)) & RTC_CTRL_CLKOUT_MASK)

/*! @name STATUS - RTC Status Register */
#define RTC_STATUS_INVAL_BIT_MASK                (0x1U)
#define RTC_STATUS_INVAL_BIT_SHIFT               (0U)
#define RTC_STATUS_INVAL_BIT(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_INVAL_BIT_SHIFT)) & RTC_STATUS_INVAL_BIT_MASK)
#define RTC_STATUS_WRITE_PROT_EN_MASK            (0x2U)
#define RTC_STATUS_WRITE_PROT_EN_SHIFT           (1U)
#define RTC_STATUS_WRITE_PROT_EN(x)              (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_WRITE_PROT_EN_SHIFT)) & RTC_STATUS_WRITE_PROT_EN_MASK)
#define RTC_STATUS_CPU_LOW_VOLT_MASK             (0x4U)
#define RTC_STATUS_CPU_LOW_VOLT_SHIFT            (2U)
#define RTC_STATUS_CPU_LOW_VOLT(x)               (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_CPU_LOW_VOLT_SHIFT)) & RTC_STATUS_CPU_LOW_VOLT_MASK)
#define RTC_STATUS_RST_SRC_MASK                  (0x8U)
#define RTC_STATUS_RST_SRC_SHIFT                 (3U)
#define RTC_STATUS_RST_SRC(x)                    (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_RST_SRC_SHIFT)) & RTC_STATUS_RST_SRC_MASK)
#define RTC_STATUS_CMP_INT_MASK                  (0x20U)
#define RTC_STATUS_CMP_INT_SHIFT                 (5U)
#define RTC_STATUS_CMP_INT(x)                    (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_CMP_INT_SHIFT)) & RTC_STATUS_CMP_INT_MASK)
#define RTC_STATUS_WE_MASK                       (0xC0U)
#define RTC_STATUS_WE_SHIFT                      (6U)
#define RTC_STATUS_WE(x)                         (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_WE_SHIFT)) & RTC_STATUS_WE_MASK)
#define RTC_STATUS_BUS_ERR_MASK                  (0x100U)
#define RTC_STATUS_BUS_ERR_SHIFT                 (8U)
#define RTC_STATUS_BUS_ERR(x)                    (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_BUS_ERR_SHIFT)) & RTC_STATUS_BUS_ERR_MASK)
#define RTC_STATUS_CMP_DONE_MASK                 (0x800U)
#define RTC_STATUS_CMP_DONE_SHIFT                (11U)
#define RTC_STATUS_CMP_DONE(x)                   (((uint16_t)(((uint16_t)(x)) << RTC_STATUS_CMP_DONE_SHIFT)) & RTC_STATUS_CMP_DONE_MASK)

/*! @name ISR - RTC Interrupt Status Register */
#define RTC_ISR_TAMPER_IS_MASK                   (0x1U)
#define RTC_ISR_TAMPER_IS_SHIFT                  (0U)
#define RTC_ISR_TAMPER_IS(x)                     (((uint16_t)(((uint16_t)(x)) << RTC_ISR_TAMPER_IS_SHIFT)) & RTC_ISR_TAMPER_IS_MASK)
#define RTC_ISR_ALM_IS_MASK                      (0x4U)
#define RTC_ISR_ALM_IS_SHIFT                     (2U)
#define RTC_ISR_ALM_IS(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_ALM_IS_SHIFT)) & RTC_ISR_ALM_IS_MASK)
#define RTC_ISR_DAY_IS_MASK                      (0x8U)
#define RTC_ISR_DAY_IS_SHIFT                     (3U)
#define RTC_ISR_DAY_IS(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_DAY_IS_SHIFT)) & RTC_ISR_DAY_IS_MASK)
#define RTC_ISR_HOUR_IS_MASK                     (0x10U)
#define RTC_ISR_HOUR_IS_SHIFT                    (4U)
#define RTC_ISR_HOUR_IS(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_ISR_HOUR_IS_SHIFT)) & RTC_ISR_HOUR_IS_MASK)
#define RTC_ISR_MIN_IS_MASK                      (0x20U)
#define RTC_ISR_MIN_IS_SHIFT                     (5U)
#define RTC_ISR_MIN_IS(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_MIN_IS_SHIFT)) & RTC_ISR_MIN_IS_MASK)
#define RTC_ISR_IS_1HZ_MASK                      (0x40U)
#define RTC_ISR_IS_1HZ_SHIFT                     (6U)
#define RTC_ISR_IS_1HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_1HZ_SHIFT)) & RTC_ISR_IS_1HZ_MASK)
#define RTC_ISR_IS_2HZ_MASK                      (0x80U)
#define RTC_ISR_IS_2HZ_SHIFT                     (7U)
#define RTC_ISR_IS_2HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_2HZ_SHIFT)) & RTC_ISR_IS_2HZ_MASK)
#define RTC_ISR_IS_4HZ_MASK                      (0x100U)
#define RTC_ISR_IS_4HZ_SHIFT                     (8U)
#define RTC_ISR_IS_4HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_4HZ_SHIFT)) & RTC_ISR_IS_4HZ_MASK)
#define RTC_ISR_IS_8HZ_MASK                      (0x200U)
#define RTC_ISR_IS_8HZ_SHIFT                     (9U)
#define RTC_ISR_IS_8HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_8HZ_SHIFT)) & RTC_ISR_IS_8HZ_MASK)
#define RTC_ISR_IS_16HZ_MASK                     (0x400U)
#define RTC_ISR_IS_16HZ_SHIFT                    (10U)
#define RTC_ISR_IS_16HZ(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_16HZ_SHIFT)) & RTC_ISR_IS_16HZ_MASK)
#define RTC_ISR_IS_32HZ_MASK                     (0x800U)
#define RTC_ISR_IS_32HZ_SHIFT                    (11U)
#define RTC_ISR_IS_32HZ(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_32HZ_SHIFT)) & RTC_ISR_IS_32HZ_MASK)
#define RTC_ISR_IS_64HZ_MASK                     (0x1000U)
#define RTC_ISR_IS_64HZ_SHIFT                    (12U)
#define RTC_ISR_IS_64HZ(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_64HZ_SHIFT)) & RTC_ISR_IS_64HZ_MASK)
#define RTC_ISR_IS_128HZ_MASK                    (0x2000U)
#define RTC_ISR_IS_128HZ_SHIFT                   (13U)
#define RTC_ISR_IS_128HZ(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_128HZ_SHIFT)) & RTC_ISR_IS_128HZ_MASK)
#define RTC_ISR_IS_256HZ_MASK                    (0x4000U)
#define RTC_ISR_IS_256HZ_SHIFT                   (14U)
#define RTC_ISR_IS_256HZ(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_256HZ_SHIFT)) & RTC_ISR_IS_256HZ_MASK)
#define RTC_ISR_IS_512HZ_MASK                    (0x8000U)
#define RTC_ISR_IS_512HZ_SHIFT                   (15U)
#define RTC_ISR_IS_512HZ(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_ISR_IS_512HZ_SHIFT)) & RTC_ISR_IS_512HZ_MASK)

/*! @name IER - RTC Interrupt Enable Register */
#define RTC_IER_TAMPER_IE_MASK                   (0x1U)
#define RTC_IER_TAMPER_IE_SHIFT                  (0U)
#define RTC_IER_TAMPER_IE(x)                     (((uint16_t)(((uint16_t)(x)) << RTC_IER_TAMPER_IE_SHIFT)) & RTC_IER_TAMPER_IE_MASK)
#define RTC_IER_ALM_IE_MASK                      (0x4U)
#define RTC_IER_ALM_IE_SHIFT                     (2U)
#define RTC_IER_ALM_IE(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_ALM_IE_SHIFT)) & RTC_IER_ALM_IE_MASK)
#define RTC_IER_DAY_IE_MASK                      (0x8U)
#define RTC_IER_DAY_IE_SHIFT                     (3U)
#define RTC_IER_DAY_IE(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_DAY_IE_SHIFT)) & RTC_IER_DAY_IE_MASK)
#define RTC_IER_HOUR_IE_MASK                     (0x10U)
#define RTC_IER_HOUR_IE_SHIFT                    (4U)
#define RTC_IER_HOUR_IE(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_IER_HOUR_IE_SHIFT)) & RTC_IER_HOUR_IE_MASK)
#define RTC_IER_MIN_IE_MASK                      (0x20U)
#define RTC_IER_MIN_IE_SHIFT                     (5U)
#define RTC_IER_MIN_IE(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_MIN_IE_SHIFT)) & RTC_IER_MIN_IE_MASK)
#define RTC_IER_IE_1HZ_MASK                      (0x40U)
#define RTC_IER_IE_1HZ_SHIFT                     (6U)
#define RTC_IER_IE_1HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_1HZ_SHIFT)) & RTC_IER_IE_1HZ_MASK)
#define RTC_IER_IE_2HZ_MASK                      (0x80U)
#define RTC_IER_IE_2HZ_SHIFT                     (7U)
#define RTC_IER_IE_2HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_2HZ_SHIFT)) & RTC_IER_IE_2HZ_MASK)
#define RTC_IER_IE_4HZ_MASK                      (0x100U)
#define RTC_IER_IE_4HZ_SHIFT                     (8U)
#define RTC_IER_IE_4HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_4HZ_SHIFT)) & RTC_IER_IE_4HZ_MASK)
#define RTC_IER_IE_8HZ_MASK                      (0x200U)
#define RTC_IER_IE_8HZ_SHIFT                     (9U)
#define RTC_IER_IE_8HZ(x)                        (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_8HZ_SHIFT)) & RTC_IER_IE_8HZ_MASK)
#define RTC_IER_IE_16HZ_MASK                     (0x400U)
#define RTC_IER_IE_16HZ_SHIFT                    (10U)
#define RTC_IER_IE_16HZ(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_16HZ_SHIFT)) & RTC_IER_IE_16HZ_MASK)
#define RTC_IER_IE_32HZ_MASK                     (0x800U)
#define RTC_IER_IE_32HZ_SHIFT                    (11U)
#define RTC_IER_IE_32HZ(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_32HZ_SHIFT)) & RTC_IER_IE_32HZ_MASK)
#define RTC_IER_IE_64HZ_MASK                     (0x1000U)
#define RTC_IER_IE_64HZ_SHIFT                    (12U)
#define RTC_IER_IE_64HZ(x)                       (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_64HZ_SHIFT)) & RTC_IER_IE_64HZ_MASK)
#define RTC_IER_IE_128HZ_MASK                    (0x2000U)
#define RTC_IER_IE_128HZ_SHIFT                   (13U)
#define RTC_IER_IE_128HZ(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_128HZ_SHIFT)) & RTC_IER_IE_128HZ_MASK)
#define RTC_IER_IE_256HZ_MASK                    (0x4000U)
#define RTC_IER_IE_256HZ_SHIFT                   (14U)
#define RTC_IER_IE_256HZ(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_256HZ_SHIFT)) & RTC_IER_IE_256HZ_MASK)
#define RTC_IER_IE_512HZ_MASK                    (0x8000U)
#define RTC_IER_IE_512HZ_SHIFT                   (15U)
#define RTC_IER_IE_512HZ(x)                      (((uint16_t)(((uint16_t)(x)) << RTC_IER_IE_512HZ_SHIFT)) & RTC_IER_IE_512HZ_MASK)

/*! @name GP_DATA_REG - RTC General Purpose Data Register */
#define RTC_GP_DATA_REG_CFG0_MASK                (0x1U)
#define RTC_GP_DATA_REG_CFG0_SHIFT               (0U)
#define RTC_GP_DATA_REG_CFG0(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG0_SHIFT)) & RTC_GP_DATA_REG_CFG0_MASK)
#define RTC_GP_DATA_REG_CFG1_MASK                (0x2U)
#define RTC_GP_DATA_REG_CFG1_SHIFT               (1U)
#define RTC_GP_DATA_REG_CFG1(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG1_SHIFT)) & RTC_GP_DATA_REG_CFG1_MASK)
#define RTC_GP_DATA_REG_CFG2_MASK                (0x4U)
#define RTC_GP_DATA_REG_CFG2_SHIFT               (2U)
#define RTC_GP_DATA_REG_CFG2(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG2_SHIFT)) & RTC_GP_DATA_REG_CFG2_MASK)
#define RTC_GP_DATA_REG_CFG3_MASK                (0x8U)
#define RTC_GP_DATA_REG_CFG3_SHIFT               (3U)
#define RTC_GP_DATA_REG_CFG3(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG3_SHIFT)) & RTC_GP_DATA_REG_CFG3_MASK)
#define RTC_GP_DATA_REG_CFG4_MASK                (0x10U)
#define RTC_GP_DATA_REG_CFG4_SHIFT               (4U)
#define RTC_GP_DATA_REG_CFG4(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG4_SHIFT)) & RTC_GP_DATA_REG_CFG4_MASK)
#define RTC_GP_DATA_REG_CFG5_MASK                (0x20U)
#define RTC_GP_DATA_REG_CFG5_SHIFT               (5U)
#define RTC_GP_DATA_REG_CFG5(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG5_SHIFT)) & RTC_GP_DATA_REG_CFG5_MASK)
#define RTC_GP_DATA_REG_CFG6_MASK                (0x40U)
#define RTC_GP_DATA_REG_CFG6_SHIFT               (6U)
#define RTC_GP_DATA_REG_CFG6(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG6_SHIFT)) & RTC_GP_DATA_REG_CFG6_MASK)
#define RTC_GP_DATA_REG_CFG7_MASK                (0x80U)
#define RTC_GP_DATA_REG_CFG7_SHIFT               (7U)
#define RTC_GP_DATA_REG_CFG7(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG7_SHIFT)) & RTC_GP_DATA_REG_CFG7_MASK)
#define RTC_GP_DATA_REG_CFG8_MASK                (0x100U)
#define RTC_GP_DATA_REG_CFG8_SHIFT               (8U)
#define RTC_GP_DATA_REG_CFG8(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG8_SHIFT)) & RTC_GP_DATA_REG_CFG8_MASK)
#define RTC_GP_DATA_REG_CFG9_MASK                (0x200U)
#define RTC_GP_DATA_REG_CFG9_SHIFT               (9U)
#define RTC_GP_DATA_REG_CFG9(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG9_SHIFT)) & RTC_GP_DATA_REG_CFG9_MASK)
#define RTC_GP_DATA_REG_CFG10_MASK               (0x400U)
#define RTC_GP_DATA_REG_CFG10_SHIFT              (10U)
#define RTC_GP_DATA_REG_CFG10(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG10_SHIFT)) & RTC_GP_DATA_REG_CFG10_MASK)
#define RTC_GP_DATA_REG_CFG11_MASK               (0x800U)
#define RTC_GP_DATA_REG_CFG11_SHIFT              (11U)
#define RTC_GP_DATA_REG_CFG11(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG11_SHIFT)) & RTC_GP_DATA_REG_CFG11_MASK)
#define RTC_GP_DATA_REG_CFG12_MASK               (0x1000U)
#define RTC_GP_DATA_REG_CFG12_SHIFT              (12U)
#define RTC_GP_DATA_REG_CFG12(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG12_SHIFT)) & RTC_GP_DATA_REG_CFG12_MASK)
#define RTC_GP_DATA_REG_CFG13_MASK               (0x2000U)
#define RTC_GP_DATA_REG_CFG13_SHIFT              (13U)
#define RTC_GP_DATA_REG_CFG13(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG13_SHIFT)) & RTC_GP_DATA_REG_CFG13_MASK)
#define RTC_GP_DATA_REG_CFG14_MASK               (0x4000U)
#define RTC_GP_DATA_REG_CFG14_SHIFT              (14U)
#define RTC_GP_DATA_REG_CFG14(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG14_SHIFT)) & RTC_GP_DATA_REG_CFG14_MASK)
#define RTC_GP_DATA_REG_CFG15_MASK               (0x8000U)
#define RTC_GP_DATA_REG_CFG15_SHIFT              (15U)
#define RTC_GP_DATA_REG_CFG15(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_GP_DATA_REG_CFG15_SHIFT)) & RTC_GP_DATA_REG_CFG15_MASK)

/*! @name DST_HOUR - RTC Daylight Saving Hour Register */
#define RTC_DST_HOUR_DST_END_HOUR_MASK           (0x1FU)
#define RTC_DST_HOUR_DST_END_HOUR_SHIFT          (0U)
#define RTC_DST_HOUR_DST_END_HOUR(x)             (((uint16_t)(((uint16_t)(x)) << RTC_DST_HOUR_DST_END_HOUR_SHIFT)) & RTC_DST_HOUR_DST_END_HOUR_MASK)
#define RTC_DST_HOUR_DST_START_HOUR_MASK         (0x1F00U)
#define RTC_DST_HOUR_DST_START_HOUR_SHIFT        (8U)
#define RTC_DST_HOUR_DST_START_HOUR(x)           (((uint16_t)(((uint16_t)(x)) << RTC_DST_HOUR_DST_START_HOUR_SHIFT)) & RTC_DST_HOUR_DST_START_HOUR_MASK)

/*! @name DST_MONTH - RTC Daylight Saving Month Register */
#define RTC_DST_MONTH_DST_END_MONTH_MASK         (0xFU)
#define RTC_DST_MONTH_DST_END_MONTH_SHIFT        (0U)
#define RTC_DST_MONTH_DST_END_MONTH(x)           (((uint16_t)(((uint16_t)(x)) << RTC_DST_MONTH_DST_END_MONTH_SHIFT)) & RTC_DST_MONTH_DST_END_MONTH_MASK)
#define RTC_DST_MONTH_DST_START_MONTH_MASK       (0xF00U)
#define RTC_DST_MONTH_DST_START_MONTH_SHIFT      (8U)
#define RTC_DST_MONTH_DST_START_MONTH(x)         (((uint16_t)(((uint16_t)(x)) << RTC_DST_MONTH_DST_START_MONTH_SHIFT)) & RTC_DST_MONTH_DST_START_MONTH_MASK)

/*! @name DST_DAY - RTC Daylight Saving Day Register */
#define RTC_DST_DAY_DST_END_DAY_MASK             (0x1FU)
#define RTC_DST_DAY_DST_END_DAY_SHIFT            (0U)
#define RTC_DST_DAY_DST_END_DAY(x)               (((uint16_t)(((uint16_t)(x)) << RTC_DST_DAY_DST_END_DAY_SHIFT)) & RTC_DST_DAY_DST_END_DAY_MASK)
#define RTC_DST_DAY_DST_START_DAY_MASK           (0x1F00U)
#define RTC_DST_DAY_DST_START_DAY_SHIFT          (8U)
#define RTC_DST_DAY_DST_START_DAY(x)             (((uint16_t)(((uint16_t)(x)) << RTC_DST_DAY_DST_START_DAY_SHIFT)) & RTC_DST_DAY_DST_START_DAY_MASK)

/*! @name COMPEN - RTC Compensation Register */
#define RTC_COMPEN_COMPEN_VAL_MASK               (0xFFFFU)
#define RTC_COMPEN_COMPEN_VAL_SHIFT              (0U)
#define RTC_COMPEN_COMPEN_VAL(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_COMPEN_COMPEN_VAL_SHIFT)) & RTC_COMPEN_COMPEN_VAL_MASK)

/*! @name TAMPER_SCR - RTC Tamper Status and Control Register */
#define RTC_TAMPER_SCR_TMPR_EN_MASK              (0xFU)
#define RTC_TAMPER_SCR_TMPR_EN_SHIFT             (0U)
#define RTC_TAMPER_SCR_TMPR_EN(x)                (((uint16_t)(((uint16_t)(x)) << RTC_TAMPER_SCR_TMPR_EN_SHIFT)) & RTC_TAMPER_SCR_TMPR_EN_MASK)
#define RTC_TAMPER_SCR_TMPR_STS_MASK             (0xF00U)
#define RTC_TAMPER_SCR_TMPR_STS_SHIFT            (8U)
#define RTC_TAMPER_SCR_TMPR_STS(x)               (((uint16_t)(((uint16_t)(x)) << RTC_TAMPER_SCR_TMPR_STS_SHIFT)) & RTC_TAMPER_SCR_TMPR_STS_MASK)

/*! @name FILTER01_CFG - RTC Tamper 0 1 Filter Configuration Register */
#define RTC_FILTER01_CFG_FIL_DUR1_MASK           (0xFU)
#define RTC_FILTER01_CFG_FIL_DUR1_SHIFT          (0U)
#define RTC_FILTER01_CFG_FIL_DUR1(x)             (((uint16_t)(((uint16_t)(x)) << RTC_FILTER01_CFG_FIL_DUR1_SHIFT)) & RTC_FILTER01_CFG_FIL_DUR1_MASK)
#define RTC_FILTER01_CFG_CLK_SEL1_MASK           (0x70U)
#define RTC_FILTER01_CFG_CLK_SEL1_SHIFT          (4U)
#define RTC_FILTER01_CFG_CLK_SEL1(x)             (((uint16_t)(((uint16_t)(x)) << RTC_FILTER01_CFG_CLK_SEL1_SHIFT)) & RTC_FILTER01_CFG_CLK_SEL1_MASK)
#define RTC_FILTER01_CFG_POL1_MASK               (0x80U)
#define RTC_FILTER01_CFG_POL1_SHIFT              (7U)
#define RTC_FILTER01_CFG_POL1(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_FILTER01_CFG_POL1_SHIFT)) & RTC_FILTER01_CFG_POL1_MASK)
#define RTC_FILTER01_CFG_FIL_DUR0_MASK           (0xF00U)
#define RTC_FILTER01_CFG_FIL_DUR0_SHIFT          (8U)
#define RTC_FILTER01_CFG_FIL_DUR0(x)             (((uint16_t)(((uint16_t)(x)) << RTC_FILTER01_CFG_FIL_DUR0_SHIFT)) & RTC_FILTER01_CFG_FIL_DUR0_MASK)
#define RTC_FILTER01_CFG_CLK_SEL0_MASK           (0x7000U)
#define RTC_FILTER01_CFG_CLK_SEL0_SHIFT          (12U)
#define RTC_FILTER01_CFG_CLK_SEL0(x)             (((uint16_t)(((uint16_t)(x)) << RTC_FILTER01_CFG_CLK_SEL0_SHIFT)) & RTC_FILTER01_CFG_CLK_SEL0_MASK)
#define RTC_FILTER01_CFG_POL0_MASK               (0x8000U)
#define RTC_FILTER01_CFG_POL0_SHIFT              (15U)
#define RTC_FILTER01_CFG_POL0(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_FILTER01_CFG_POL0_SHIFT)) & RTC_FILTER01_CFG_POL0_MASK)

/*! @name FILTER2_CFG - RTC Tamper 2 Filter Configuration Register */
#define RTC_FILTER2_CFG_FIL_DUR2_MASK            (0xF00U)
#define RTC_FILTER2_CFG_FIL_DUR2_SHIFT           (8U)
#define RTC_FILTER2_CFG_FIL_DUR2(x)              (((uint16_t)(((uint16_t)(x)) << RTC_FILTER2_CFG_FIL_DUR2_SHIFT)) & RTC_FILTER2_CFG_FIL_DUR2_MASK)
#define RTC_FILTER2_CFG_CLK_SEL2_MASK            (0x7000U)
#define RTC_FILTER2_CFG_CLK_SEL2_SHIFT           (12U)
#define RTC_FILTER2_CFG_CLK_SEL2(x)              (((uint16_t)(((uint16_t)(x)) << RTC_FILTER2_CFG_CLK_SEL2_SHIFT)) & RTC_FILTER2_CFG_CLK_SEL2_MASK)
#define RTC_FILTER2_CFG_POL2_MASK                (0x8000U)
#define RTC_FILTER2_CFG_POL2_SHIFT               (15U)
#define RTC_FILTER2_CFG_POL2(x)                  (((uint16_t)(((uint16_t)(x)) << RTC_FILTER2_CFG_POL2_SHIFT)) & RTC_FILTER2_CFG_POL2_MASK)

/*! @name CTRL2 - RTC Control 2 Register */
#define RTC_CTRL2_TAMP_CFG_OVER_MASK             (0x1U)
#define RTC_CTRL2_TAMP_CFG_OVER_SHIFT            (0U)
#define RTC_CTRL2_TAMP_CFG_OVER(x)               (((uint16_t)(((uint16_t)(x)) << RTC_CTRL2_TAMP_CFG_OVER_SHIFT)) & RTC_CTRL2_TAMP_CFG_OVER_MASK)
#define RTC_CTRL2_WAKEUP_STATUS_MASK             (0x60U)
#define RTC_CTRL2_WAKEUP_STATUS_SHIFT            (5U)
#define RTC_CTRL2_WAKEUP_STATUS(x)               (((uint16_t)(((uint16_t)(x)) << RTC_CTRL2_WAKEUP_STATUS_SHIFT)) & RTC_CTRL2_WAKEUP_STATUS_MASK)
#define RTC_CTRL2_WAKEUP_MODE_MASK               (0x80U)
#define RTC_CTRL2_WAKEUP_MODE_SHIFT              (7U)
#define RTC_CTRL2_WAKEUP_MODE(x)                 (((uint16_t)(((uint16_t)(x)) << RTC_CTRL2_WAKEUP_MODE_SHIFT)) & RTC_CTRL2_WAKEUP_MODE_MASK)


/*!
 * @}
 */ /* end of group RTC_Register_Masks */


/* RTC - Peripheral instance base addresses */
/** Peripheral RTC base address */
#define RTC_BASE                                 (0x40050000u)
/** Peripheral RTC base pointer */
#define RTC                                      ((RTC_Type *)RTC_BASE)
/** Array initializer of RTC peripheral base addresses */
#define RTC_BASE_ADDRS                           { RTC_BASE }
/** Array initializer of RTC peripheral base pointers */
#define RTC_BASE_PTRS                            { RTC }
/** Interrupt vectors for the RTC peripheral type */
#define RTC_IRQS                                 { RTC_IRQn }

/*!
 * @}
 */ /* end of group RTC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- SIM Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Peripheral_Access_Layer SIM Peripheral Access Layer
 * @{
 */

/** SIM - Register Layout Typedef */
typedef struct {
  __IO uint32_t SOPT1;                             /**< System Options Register 1, offset: 0x0 */
  __IO uint32_t SOPT1_CFG;                         /**< SOPT1 Configuration Register, offset: 0x4 */
       uint8_t RESERVED_0[4092];
  __IO uint32_t CTRL_REG;                          /**< System Control Register, offset: 0x1004 */
       uint8_t RESERVED_1[28];
  __I  uint32_t SDID;                              /**< System Device Identification Register, offset: 0x1024 */
       uint8_t RESERVED_2[12];
  __IO uint32_t SCGC4;                             /**< System Clock Gating Control Register 4, offset: 0x1034 */
  __IO uint32_t SCGC5;                             /**< System Clock Gating Control Register 5, offset: 0x1038 */
  __IO uint32_t SCGC6;                             /**< System Clock Gating Control Register 6, offset: 0x103C */
  __IO uint32_t SCGC7;                             /**< System Clock Gating Control Register 7, offset: 0x1040 */
  __IO uint32_t CLKDIV1;                           /**< System Clock Divider Register 1, offset: 0x1044 */
       uint8_t RESERVED_3[4];
  __IO uint32_t FCFG1;                             /**< Flash Configuration Register 1, offset: 0x104C */
  __I  uint32_t FCFG2;                             /**< Flash Configuration Register 2, offset: 0x1050 */
  __I  uint32_t UIDH;                              /**< Unique Identification Register High, offset: 0x1054 */
  __I  uint32_t UIDMH;                             /**< Unique Identification Register Mid-High, offset: 0x1058 */
  __I  uint32_t UIDML;                             /**< Unique Identification Register Mid-Low, offset: 0x105C */
  __I  uint32_t UIDL;                              /**< Unique Identification Register Low, offset: 0x1060 */
       uint8_t RESERVED_4[8];
  __IO uint32_t MISC_CTL;                          /**< Miscellaneous Control Register, offset: 0x106C */
       uint8_t RESERVED_5[88];
  __I  uint32_t ADC_COMP0;                         /**< ADC Compensation Register 0, offset: 0x10C8 */
  __I  uint32_t ADC_COMP1;                         /**< ADC Compensation Register 1, offset: 0x10CC */
} SIM_Type;

/* ----------------------------------------------------------------------------
   -- SIM Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SIM_Register_Masks SIM Register Masks
 * @{
 */

/*! @name SOPT1 - System Options Register 1 */
#define SIM_SOPT1_SRAMSIZE_MASK                  (0xF000U)
#define SIM_SOPT1_SRAMSIZE_SHIFT                 (12U)
#define SIM_SOPT1_SRAMSIZE(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_SRAMSIZE_SHIFT)) & SIM_SOPT1_SRAMSIZE_MASK)
#define SIM_SOPT1_OSC32KSEL_MASK                 (0xC0000U)
#define SIM_SOPT1_OSC32KSEL_SHIFT                (18U)
#define SIM_SOPT1_OSC32KSEL(x)                   (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_OSC32KSEL_SHIFT)) & SIM_SOPT1_OSC32KSEL_MASK)

/*! @name SOPT1_CFG - SOPT1 Configuration Register */
#define SIM_SOPT1_CFG_LPTMR0SEL_MASK             (0x3U)
#define SIM_SOPT1_CFG_LPTMR0SEL_SHIFT            (0U)
#define SIM_SOPT1_CFG_LPTMR0SEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_CFG_LPTMR0SEL_SHIFT)) & SIM_SOPT1_CFG_LPTMR0SEL_MASK)
#define SIM_SOPT1_CFG_LPTMR1SEL_MASK             (0xCU)
#define SIM_SOPT1_CFG_LPTMR1SEL_SHIFT            (2U)
#define SIM_SOPT1_CFG_LPTMR1SEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_CFG_LPTMR1SEL_SHIFT)) & SIM_SOPT1_CFG_LPTMR1SEL_MASK)
#define SIM_SOPT1_CFG_LPTMR2SEL_MASK             (0x30U)
#define SIM_SOPT1_CFG_LPTMR2SEL_SHIFT            (4U)
#define SIM_SOPT1_CFG_LPTMR2SEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_CFG_LPTMR2SEL_SHIFT)) & SIM_SOPT1_CFG_LPTMR2SEL_MASK)
#define SIM_SOPT1_CFG_LPTMR3SEL_MASK             (0xC0U)
#define SIM_SOPT1_CFG_LPTMR3SEL_SHIFT            (6U)
#define SIM_SOPT1_CFG_LPTMR3SEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_CFG_LPTMR3SEL_SHIFT)) & SIM_SOPT1_CFG_LPTMR3SEL_MASK)
#define SIM_SOPT1_CFG_RAMSBDIS_MASK              (0x100U)
#define SIM_SOPT1_CFG_RAMSBDIS_SHIFT             (8U)
#define SIM_SOPT1_CFG_RAMSBDIS(x)                (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_CFG_RAMSBDIS_SHIFT)) & SIM_SOPT1_CFG_RAMSBDIS_MASK)
#define SIM_SOPT1_CFG_RAMBPEN_MASK               (0x200U)
#define SIM_SOPT1_CFG_RAMBPEN_SHIFT              (9U)
#define SIM_SOPT1_CFG_RAMBPEN(x)                 (((uint32_t)(((uint32_t)(x)) << SIM_SOPT1_CFG_RAMBPEN_SHIFT)) & SIM_SOPT1_CFG_RAMBPEN_MASK)

/*! @name CTRL_REG - System Control Register */
#define SIM_CTRL_REG_NMIDIS_MASK                 (0x1U)
#define SIM_CTRL_REG_NMIDIS_SHIFT                (0U)
#define SIM_CTRL_REG_NMIDIS(x)                   (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_NMIDIS_SHIFT)) & SIM_CTRL_REG_NMIDIS_MASK)
#define SIM_CTRL_REG_PLLVLPEN_MASK               (0x2U)
#define SIM_CTRL_REG_PLLVLPEN_SHIFT              (1U)
#define SIM_CTRL_REG_PLLVLPEN(x)                 (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_PLLVLPEN_SHIFT)) & SIM_CTRL_REG_PLLVLPEN_MASK)
#define SIM_CTRL_REG_ADCTRGSEL_MASK              (0x18U)
#define SIM_CTRL_REG_ADCTRGSEL_SHIFT             (3U)
#define SIM_CTRL_REG_ADCTRGSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_ADCTRGSEL_SHIFT)) & SIM_CTRL_REG_ADCTRGSEL_MASK)
#define SIM_CTRL_REG_CLKOUT_MASK                 (0xE0U)
#define SIM_CTRL_REG_CLKOUT_SHIFT                (5U)
#define SIM_CTRL_REG_CLKOUT(x)                   (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_CLKOUT_SHIFT)) & SIM_CTRL_REG_CLKOUT_MASK)
#define SIM_CTRL_REG_SPI0_INV0_MASK              (0x100U)
#define SIM_CTRL_REG_SPI0_INV0_SHIFT             (8U)
#define SIM_CTRL_REG_SPI0_INV0(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI0_INV0_SHIFT)) & SIM_CTRL_REG_SPI0_INV0_MASK)
#define SIM_CTRL_REG_SPI0_INV1_MASK              (0x200U)
#define SIM_CTRL_REG_SPI0_INV1_SHIFT             (9U)
#define SIM_CTRL_REG_SPI0_INV1(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI0_INV1_SHIFT)) & SIM_CTRL_REG_SPI0_INV1_MASK)
#define SIM_CTRL_REG_SPI0_INV2_MASK              (0x400U)
#define SIM_CTRL_REG_SPI0_INV2_SHIFT             (10U)
#define SIM_CTRL_REG_SPI0_INV2(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI0_INV2_SHIFT)) & SIM_CTRL_REG_SPI0_INV2_MASK)
#define SIM_CTRL_REG_SPI0_INV3_MASK              (0x800U)
#define SIM_CTRL_REG_SPI0_INV3_SHIFT             (11U)
#define SIM_CTRL_REG_SPI0_INV3(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI0_INV3_SHIFT)) & SIM_CTRL_REG_SPI0_INV3_MASK)
#define SIM_CTRL_REG_SPI1_INV0_MASK              (0x1000U)
#define SIM_CTRL_REG_SPI1_INV0_SHIFT             (12U)
#define SIM_CTRL_REG_SPI1_INV0(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI1_INV0_SHIFT)) & SIM_CTRL_REG_SPI1_INV0_MASK)
#define SIM_CTRL_REG_SPI1_INV1_MASK              (0x2000U)
#define SIM_CTRL_REG_SPI1_INV1_SHIFT             (13U)
#define SIM_CTRL_REG_SPI1_INV1(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI1_INV1_SHIFT)) & SIM_CTRL_REG_SPI1_INV1_MASK)
#define SIM_CTRL_REG_SPI1_INV2_MASK              (0x4000U)
#define SIM_CTRL_REG_SPI1_INV2_SHIFT             (14U)
#define SIM_CTRL_REG_SPI1_INV2(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI1_INV2_SHIFT)) & SIM_CTRL_REG_SPI1_INV2_MASK)
#define SIM_CTRL_REG_SPI1_INV3_MASK              (0x8000U)
#define SIM_CTRL_REG_SPI1_INV3_SHIFT             (15U)
#define SIM_CTRL_REG_SPI1_INV3(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_SPI1_INV3_SHIFT)) & SIM_CTRL_REG_SPI1_INV3_MASK)
#define SIM_CTRL_REG_PLLFLLSEL_MASK              (0x30000U)
#define SIM_CTRL_REG_PLLFLLSEL_SHIFT             (16U)
#define SIM_CTRL_REG_PLLFLLSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_PLLFLLSEL_SHIFT)) & SIM_CTRL_REG_PLLFLLSEL_MASK)
#define SIM_CTRL_REG_XBARCLKOUT_MASK             (0xE00000U)
#define SIM_CTRL_REG_XBARCLKOUT_SHIFT            (21U)
#define SIM_CTRL_REG_XBARCLKOUT(x)               (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_XBARCLKOUT_SHIFT)) & SIM_CTRL_REG_XBARCLKOUT_MASK)
#define SIM_CTRL_REG_AFEOUTCLKSEL_MASK           (0x1000000U)
#define SIM_CTRL_REG_AFEOUTCLKSEL_SHIFT          (24U)
#define SIM_CTRL_REG_AFEOUTCLKSEL(x)             (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_AFEOUTCLKSEL_SHIFT)) & SIM_CTRL_REG_AFEOUTCLKSEL_MASK)
#define SIM_CTRL_REG_LPUARTSRC_MASK              (0xC000000U)
#define SIM_CTRL_REG_LPUARTSRC_SHIFT             (26U)
#define SIM_CTRL_REG_LPUARTSRC(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_LPUARTSRC_SHIFT)) & SIM_CTRL_REG_LPUARTSRC_MASK)
#define SIM_CTRL_REG_TMRFREEZE_MASK              (0x80000000U)
#define SIM_CTRL_REG_TMRFREEZE_SHIFT             (31U)
#define SIM_CTRL_REG_TMRFREEZE(x)                (((uint32_t)(((uint32_t)(x)) << SIM_CTRL_REG_TMRFREEZE_SHIFT)) & SIM_CTRL_REG_TMRFREEZE_MASK)

/*! @name SDID - System Device Identification Register */
#define SIM_SDID_PINID_MASK                      (0xFU)
#define SIM_SDID_PINID_SHIFT                     (0U)
#define SIM_SDID_PINID(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SDID_PINID_SHIFT)) & SIM_SDID_PINID_MASK)
#define SIM_SDID_DIEID_MASK                      (0xF0U)
#define SIM_SDID_DIEID_SHIFT                     (4U)
#define SIM_SDID_DIEID(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SDID_DIEID_SHIFT)) & SIM_SDID_DIEID_MASK)
#define SIM_SDID_REVID_MASK                      (0xF00U)
#define SIM_SDID_REVID_SHIFT                     (8U)
#define SIM_SDID_REVID(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SDID_REVID_SHIFT)) & SIM_SDID_REVID_MASK)
#define SIM_SDID_SRAMSIZE_MASK                   (0xF000U)
#define SIM_SDID_SRAMSIZE_SHIFT                  (12U)
#define SIM_SDID_SRAMSIZE(x)                     (((uint32_t)(((uint32_t)(x)) << SIM_SDID_SRAMSIZE_SHIFT)) & SIM_SDID_SRAMSIZE_MASK)
#define SIM_SDID_ATTR_MASK                       (0xF0000U)
#define SIM_SDID_ATTR_SHIFT                      (16U)
#define SIM_SDID_ATTR(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SDID_ATTR_SHIFT)) & SIM_SDID_ATTR_MASK)
#define SIM_SDID_SERIESID_MASK                   (0xF00000U)
#define SIM_SDID_SERIESID_SHIFT                  (20U)
#define SIM_SDID_SERIESID(x)                     (((uint32_t)(((uint32_t)(x)) << SIM_SDID_SERIESID_SHIFT)) & SIM_SDID_SERIESID_MASK)
#define SIM_SDID_SUBFAMID_MASK                   (0xF000000U)
#define SIM_SDID_SUBFAMID_SHIFT                  (24U)
#define SIM_SDID_SUBFAMID(x)                     (((uint32_t)(((uint32_t)(x)) << SIM_SDID_SUBFAMID_SHIFT)) & SIM_SDID_SUBFAMID_MASK)
#define SIM_SDID_FAMID_MASK                      (0xF0000000U)
#define SIM_SDID_FAMID_SHIFT                     (28U)
#define SIM_SDID_FAMID(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SDID_FAMID_SHIFT)) & SIM_SDID_FAMID_MASK)

/*! @name SCGC4 - System Clock Gating Control Register 4 */
#define SIM_SCGC4_EWM_MASK                       (0x2U)
#define SIM_SCGC4_EWM_SHIFT                      (1U)
#define SIM_SCGC4_EWM(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_EWM_SHIFT)) & SIM_SCGC4_EWM_MASK)
#define SIM_SCGC4_I2C0_MASK                      (0x80U)
#define SIM_SCGC4_I2C0_SHIFT                     (7U)
#define SIM_SCGC4_I2C0(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_I2C0_SHIFT)) & SIM_SCGC4_I2C0_MASK)
#define SIM_SCGC4_I2C1_MASK                      (0x100U)
#define SIM_SCGC4_I2C1_SHIFT                     (8U)
#define SIM_SCGC4_I2C1(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_I2C1_SHIFT)) & SIM_SCGC4_I2C1_MASK)
#define SIM_SCGC4_UART0_MASK                     (0x400U)
#define SIM_SCGC4_UART0_SHIFT                    (10U)
#define SIM_SCGC4_UART0(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_UART0_SHIFT)) & SIM_SCGC4_UART0_MASK)
#define SIM_SCGC4_UART1_MASK                     (0x800U)
#define SIM_SCGC4_UART1_SHIFT                    (11U)
#define SIM_SCGC4_UART1(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_UART1_SHIFT)) & SIM_SCGC4_UART1_MASK)
#define SIM_SCGC4_UART2_MASK                     (0x1000U)
#define SIM_SCGC4_UART2_SHIFT                    (12U)
#define SIM_SCGC4_UART2(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_UART2_SHIFT)) & SIM_SCGC4_UART2_MASK)
#define SIM_SCGC4_UART3_MASK                     (0x2000U)
#define SIM_SCGC4_UART3_SHIFT                    (13U)
#define SIM_SCGC4_UART3(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_UART3_SHIFT)) & SIM_SCGC4_UART3_MASK)
#define SIM_SCGC4_VREF_MASK                      (0x8000U)
#define SIM_SCGC4_VREF_SHIFT                     (15U)
#define SIM_SCGC4_VREF(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_VREF_SHIFT)) & SIM_SCGC4_VREF_MASK)
#define SIM_SCGC4_CMP_MASK                       (0x40000U)
#define SIM_SCGC4_CMP_SHIFT                      (18U)
#define SIM_SCGC4_CMP(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_CMP_SHIFT)) & SIM_SCGC4_CMP_MASK)
#define SIM_SCGC4_SPI0_MASK                      (0x200000U)
#define SIM_SCGC4_SPI0_SHIFT                     (21U)
#define SIM_SCGC4_SPI0(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_SPI0_SHIFT)) & SIM_SCGC4_SPI0_MASK)
#define SIM_SCGC4_SPI1_MASK                      (0x400000U)
#define SIM_SCGC4_SPI1_SHIFT                     (22U)
#define SIM_SCGC4_SPI1(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC4_SPI1_SHIFT)) & SIM_SCGC4_SPI1_MASK)

/*! @name SCGC5 - System Clock Gating Control Register 5 */
#define SIM_SCGC5_SLCD_MASK                      (0x8U)
#define SIM_SCGC5_SLCD_SHIFT                     (3U)
#define SIM_SCGC5_SLCD(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_SLCD_SHIFT)) & SIM_SCGC5_SLCD_MASK)
#define SIM_SCGC5_PORTA_MASK                     (0x40U)
#define SIM_SCGC5_PORTA_SHIFT                    (6U)
#define SIM_SCGC5_PORTA(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTA_SHIFT)) & SIM_SCGC5_PORTA_MASK)
#define SIM_SCGC5_PORTB_MASK                     (0x80U)
#define SIM_SCGC5_PORTB_SHIFT                    (7U)
#define SIM_SCGC5_PORTB(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTB_SHIFT)) & SIM_SCGC5_PORTB_MASK)
#define SIM_SCGC5_PORTC_MASK                     (0x100U)
#define SIM_SCGC5_PORTC_SHIFT                    (8U)
#define SIM_SCGC5_PORTC(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTC_SHIFT)) & SIM_SCGC5_PORTC_MASK)
#define SIM_SCGC5_PORTD_MASK                     (0x200U)
#define SIM_SCGC5_PORTD_SHIFT                    (9U)
#define SIM_SCGC5_PORTD(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTD_SHIFT)) & SIM_SCGC5_PORTD_MASK)
#define SIM_SCGC5_PORTE_MASK                     (0x400U)
#define SIM_SCGC5_PORTE_SHIFT                    (10U)
#define SIM_SCGC5_PORTE(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTE_SHIFT)) & SIM_SCGC5_PORTE_MASK)
#define SIM_SCGC5_PORTF_MASK                     (0x800U)
#define SIM_SCGC5_PORTF_SHIFT                    (11U)
#define SIM_SCGC5_PORTF(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTF_SHIFT)) & SIM_SCGC5_PORTF_MASK)
#define SIM_SCGC5_PORTG_MASK                     (0x1000U)
#define SIM_SCGC5_PORTG_SHIFT                    (12U)
#define SIM_SCGC5_PORTG(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTG_SHIFT)) & SIM_SCGC5_PORTG_MASK)
#define SIM_SCGC5_PORTH_MASK                     (0x2000U)
#define SIM_SCGC5_PORTH_SHIFT                    (13U)
#define SIM_SCGC5_PORTH(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTH_SHIFT)) & SIM_SCGC5_PORTH_MASK)
#define SIM_SCGC5_PORTI_MASK                     (0x4000U)
#define SIM_SCGC5_PORTI_SHIFT                    (14U)
#define SIM_SCGC5_PORTI(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_PORTI_SHIFT)) & SIM_SCGC5_PORTI_MASK)
#define SIM_SCGC5_RTC_MASK                       (0x10000U)
#define SIM_SCGC5_RTC_SHIFT                      (16U)
#define SIM_SCGC5_RTC(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_RTC_SHIFT)) & SIM_SCGC5_RTC_MASK)
#define SIM_SCGC5_RTCREG_MASK                    (0x20000U)
#define SIM_SCGC5_RTCREG_SHIFT                   (17U)
#define SIM_SCGC5_RTCREG(x)                      (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_RTCREG_SHIFT)) & SIM_SCGC5_RTCREG_MASK)
#define SIM_SCGC5_XBAR_MASK                      (0x200000U)
#define SIM_SCGC5_XBAR_SHIFT                     (21U)
#define SIM_SCGC5_XBAR(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_XBAR_SHIFT)) & SIM_SCGC5_XBAR_MASK)
#define SIM_SCGC5_TMR0_MASK                      (0x800000U)
#define SIM_SCGC5_TMR0_SHIFT                     (23U)
#define SIM_SCGC5_TMR0(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_TMR0_SHIFT)) & SIM_SCGC5_TMR0_MASK)
#define SIM_SCGC5_TMR1_MASK                      (0x1000000U)
#define SIM_SCGC5_TMR1_SHIFT                     (24U)
#define SIM_SCGC5_TMR1(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_TMR1_SHIFT)) & SIM_SCGC5_TMR1_MASK)
#define SIM_SCGC5_TMR2_MASK                      (0x2000000U)
#define SIM_SCGC5_TMR2_SHIFT                     (25U)
#define SIM_SCGC5_TMR2(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_TMR2_SHIFT)) & SIM_SCGC5_TMR2_MASK)
#define SIM_SCGC5_TMR3_MASK                      (0x4000000U)
#define SIM_SCGC5_TMR3_SHIFT                     (26U)
#define SIM_SCGC5_TMR3(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC5_TMR3_SHIFT)) & SIM_SCGC5_TMR3_MASK)

/*! @name SCGC6 - System Clock Gating Control Register 6 */
#define SIM_SCGC6_FTFA_MASK                      (0x1U)
#define SIM_SCGC6_FTFA_SHIFT                     (0U)
#define SIM_SCGC6_FTFA(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_FTFA_SHIFT)) & SIM_SCGC6_FTFA_MASK)
#define SIM_SCGC6_DMACHMUX_MASK                  (0x2U)
#define SIM_SCGC6_DMACHMUX_SHIFT                 (1U)
#define SIM_SCGC6_DMACHMUX(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_DMACHMUX_SHIFT)) & SIM_SCGC6_DMACHMUX_MASK)
#define SIM_SCGC6_RNGA_MASK                      (0x200U)
#define SIM_SCGC6_RNGA_SHIFT                     (9U)
#define SIM_SCGC6_RNGA(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_RNGA_SHIFT)) & SIM_SCGC6_RNGA_MASK)
#define SIM_SCGC6_LPUART_MASK                    (0x400U)
#define SIM_SCGC6_LPUART_SHIFT                   (10U)
#define SIM_SCGC6_LPUART(x)                      (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_LPUART_SHIFT)) & SIM_SCGC6_LPUART_MASK)
#define SIM_SCGC6_ADC_MASK                       (0x800U)
#define SIM_SCGC6_ADC_SHIFT                      (11U)
#define SIM_SCGC6_ADC(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_ADC_SHIFT)) & SIM_SCGC6_ADC_MASK)
#define SIM_SCGC6_PIT0_MASK                      (0x2000U)
#define SIM_SCGC6_PIT0_SHIFT                     (13U)
#define SIM_SCGC6_PIT0(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PIT0_SHIFT)) & SIM_SCGC6_PIT0_MASK)
#define SIM_SCGC6_PIT1_MASK                      (0x4000U)
#define SIM_SCGC6_PIT1_SHIFT                     (14U)
#define SIM_SCGC6_PIT1(x)                        (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PIT1_SHIFT)) & SIM_SCGC6_PIT1_MASK)
#define SIM_SCGC6_AFE_MASK                       (0x10000U)
#define SIM_SCGC6_AFE_SHIFT                      (16U)
#define SIM_SCGC6_AFE(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_AFE_SHIFT)) & SIM_SCGC6_AFE_MASK)
#define SIM_SCGC6_CRC_MASK                       (0x100000U)
#define SIM_SCGC6_CRC_SHIFT                      (20U)
#define SIM_SCGC6_CRC(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_CRC_SHIFT)) & SIM_SCGC6_CRC_MASK)
#define SIM_SCGC6_PDB_MASK                       (0x400000U)
#define SIM_SCGC6_PDB_SHIFT                      (22U)
#define SIM_SCGC6_PDB(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PDB_SHIFT)) & SIM_SCGC6_PDB_MASK)
#define SIM_SCGC6_PORTJ_MASK                     (0x800000U)
#define SIM_SCGC6_PORTJ_SHIFT                    (23U)
#define SIM_SCGC6_PORTJ(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PORTJ_SHIFT)) & SIM_SCGC6_PORTJ_MASK)
#define SIM_SCGC6_PORTK_MASK                     (0x1000000U)
#define SIM_SCGC6_PORTK_SHIFT                    (24U)
#define SIM_SCGC6_PORTK(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PORTK_SHIFT)) & SIM_SCGC6_PORTK_MASK)
#define SIM_SCGC6_PORTL_MASK                     (0x2000000U)
#define SIM_SCGC6_PORTL_SHIFT                    (25U)
#define SIM_SCGC6_PORTL(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PORTL_SHIFT)) & SIM_SCGC6_PORTL_MASK)
#define SIM_SCGC6_PORTM_MASK                     (0x4000000U)
#define SIM_SCGC6_PORTM_SHIFT                    (26U)
#define SIM_SCGC6_PORTM(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_PORTM_SHIFT)) & SIM_SCGC6_PORTM_MASK)
#define SIM_SCGC6_LPTMR_MASK                     (0x10000000U)
#define SIM_SCGC6_LPTMR_SHIFT                    (28U)
#define SIM_SCGC6_LPTMR(x)                       (((uint32_t)(((uint32_t)(x)) << SIM_SCGC6_LPTMR_SHIFT)) & SIM_SCGC6_LPTMR_MASK)

/*! @name SCGC7 - System Clock Gating Control Register 7 */
#define SIM_SCGC7_MPU_MASK                       (0x1U)
#define SIM_SCGC7_MPU_SHIFT                      (0U)
#define SIM_SCGC7_MPU(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC7_MPU_SHIFT)) & SIM_SCGC7_MPU_MASK)
#define SIM_SCGC7_DMA_MASK                       (0x2U)
#define SIM_SCGC7_DMA_SHIFT                      (1U)
#define SIM_SCGC7_DMA(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC7_DMA_SHIFT)) & SIM_SCGC7_DMA_MASK)
#define SIM_SCGC7_CAU_MASK                       (0x4U)
#define SIM_SCGC7_CAU_SHIFT                      (2U)
#define SIM_SCGC7_CAU(x)                         (((uint32_t)(((uint32_t)(x)) << SIM_SCGC7_CAU_SHIFT)) & SIM_SCGC7_CAU_MASK)

/*! @name CLKDIV1 - System Clock Divider Register 1 */
#define SIM_CLKDIV1_FLASHCLKMODE_MASK            (0x10000U)
#define SIM_CLKDIV1_FLASHCLKMODE_SHIFT           (16U)
#define SIM_CLKDIV1_FLASHCLKMODE(x)              (((uint32_t)(((uint32_t)(x)) << SIM_CLKDIV1_FLASHCLKMODE_SHIFT)) & SIM_CLKDIV1_FLASHCLKMODE_MASK)
#define SIM_CLKDIV1_CLKDIVBUS_MASK               (0x3000000U)
#define SIM_CLKDIV1_CLKDIVBUS_SHIFT              (24U)
#define SIM_CLKDIV1_CLKDIVBUS(x)                 (((uint32_t)(((uint32_t)(x)) << SIM_CLKDIV1_CLKDIVBUS_SHIFT)) & SIM_CLKDIV1_CLKDIVBUS_MASK)
#define SIM_CLKDIV1_CLKDIVSYS_MASK               (0xF0000000U)
#define SIM_CLKDIV1_CLKDIVSYS_SHIFT              (28U)
#define SIM_CLKDIV1_CLKDIVSYS(x)                 (((uint32_t)(((uint32_t)(x)) << SIM_CLKDIV1_CLKDIVSYS_SHIFT)) & SIM_CLKDIV1_CLKDIVSYS_MASK)

/*! @name FCFG1 - Flash Configuration Register 1 */
#define SIM_FCFG1_FLASHDIS_MASK                  (0x1U)
#define SIM_FCFG1_FLASHDIS_SHIFT                 (0U)
#define SIM_FCFG1_FLASHDIS(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_FCFG1_FLASHDIS_SHIFT)) & SIM_FCFG1_FLASHDIS_MASK)
#define SIM_FCFG1_FLASHDOZE_MASK                 (0x2U)
#define SIM_FCFG1_FLASHDOZE_SHIFT                (1U)
#define SIM_FCFG1_FLASHDOZE(x)                   (((uint32_t)(((uint32_t)(x)) << SIM_FCFG1_FLASHDOZE_SHIFT)) & SIM_FCFG1_FLASHDOZE_MASK)
#define SIM_FCFG1_PFSIZE_MASK                    (0xF000000U)
#define SIM_FCFG1_PFSIZE_SHIFT                   (24U)
#define SIM_FCFG1_PFSIZE(x)                      (((uint32_t)(((uint32_t)(x)) << SIM_FCFG1_PFSIZE_SHIFT)) & SIM_FCFG1_PFSIZE_MASK)

/*! @name FCFG2 - Flash Configuration Register 2 */
#define SIM_FCFG2_MAXADDR_MASK                   (0x7F000000U)
#define SIM_FCFG2_MAXADDR_SHIFT                  (24U)
#define SIM_FCFG2_MAXADDR(x)                     (((uint32_t)(((uint32_t)(x)) << SIM_FCFG2_MAXADDR_SHIFT)) & SIM_FCFG2_MAXADDR_MASK)

/*! @name UIDH - Unique Identification Register High */
#define SIM_UIDH_UID127_96_MASK                  (0xFFFFFFFFU)
#define SIM_UIDH_UID127_96_SHIFT                 (0U)
#define SIM_UIDH_UID127_96(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_UIDH_UID127_96_SHIFT)) & SIM_UIDH_UID127_96_MASK)

/*! @name UIDMH - Unique Identification Register Mid-High */
#define SIM_UIDMH_UID95_64_MASK                  (0xFFFFFFFFU)
#define SIM_UIDMH_UID95_64_SHIFT                 (0U)
#define SIM_UIDMH_UID95_64(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_UIDMH_UID95_64_SHIFT)) & SIM_UIDMH_UID95_64_MASK)

/*! @name UIDML - Unique Identification Register Mid-Low */
#define SIM_UIDML_UID63_32_MASK                  (0xFFFFFFFFU)
#define SIM_UIDML_UID63_32_SHIFT                 (0U)
#define SIM_UIDML_UID63_32(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_UIDML_UID63_32_SHIFT)) & SIM_UIDML_UID63_32_MASK)

/*! @name UIDL - Unique Identification Register Low */
#define SIM_UIDL_UID31_0_MASK                    (0xFFFFFFFFU)
#define SIM_UIDL_UID31_0_SHIFT                   (0U)
#define SIM_UIDL_UID31_0(x)                      (((uint32_t)(((uint32_t)(x)) << SIM_UIDL_UID31_0_SHIFT)) & SIM_UIDL_UID31_0_MASK)

/*! @name MISC_CTL - Miscellaneous Control Register */
#define SIM_MISC_CTL_OSCON_MASK                  (0x1U)
#define SIM_MISC_CTL_OSCON_SHIFT                 (0U)
#define SIM_MISC_CTL_OSCON(x)                    (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_OSCON_SHIFT)) & SIM_MISC_CTL_OSCON_MASK)
#define SIM_MISC_CTL_PDBADCTRG_MASK              (0x2U)
#define SIM_MISC_CTL_PDBADCTRG_SHIFT             (1U)
#define SIM_MISC_CTL_PDBADCTRG(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_PDBADCTRG_SHIFT)) & SIM_MISC_CTL_PDBADCTRG_MASK)
#define SIM_MISC_CTL_DMADONESEL_MASK             (0xCU)
#define SIM_MISC_CTL_DMADONESEL_SHIFT            (2U)
#define SIM_MISC_CTL_DMADONESEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_DMADONESEL_SHIFT)) & SIM_MISC_CTL_DMADONESEL_MASK)
#define SIM_MISC_CTL_AFECLKSEL_MASK              (0x30U)
#define SIM_MISC_CTL_AFECLKSEL_SHIFT             (4U)
#define SIM_MISC_CTL_AFECLKSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_AFECLKSEL_SHIFT)) & SIM_MISC_CTL_AFECLKSEL_MASK)
#define SIM_MISC_CTL_AFECLKPADDIR_MASK           (0x40U)
#define SIM_MISC_CTL_AFECLKPADDIR_SHIFT          (6U)
#define SIM_MISC_CTL_AFECLKPADDIR(x)             (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_AFECLKPADDIR_SHIFT)) & SIM_MISC_CTL_AFECLKPADDIR_MASK)
#define SIM_MISC_CTL_UARTMODTYPE_MASK            (0x80U)
#define SIM_MISC_CTL_UARTMODTYPE_SHIFT           (7U)
#define SIM_MISC_CTL_UARTMODTYPE(x)              (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_UARTMODTYPE_SHIFT)) & SIM_MISC_CTL_UARTMODTYPE_MASK)
#define SIM_MISC_CTL_UART0IRSEL_MASK             (0x100U)
#define SIM_MISC_CTL_UART0IRSEL_SHIFT            (8U)
#define SIM_MISC_CTL_UART0IRSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_UART0IRSEL_SHIFT)) & SIM_MISC_CTL_UART0IRSEL_MASK)
#define SIM_MISC_CTL_UART1IRSEL_MASK             (0x200U)
#define SIM_MISC_CTL_UART1IRSEL_SHIFT            (9U)
#define SIM_MISC_CTL_UART1IRSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_UART1IRSEL_SHIFT)) & SIM_MISC_CTL_UART1IRSEL_MASK)
#define SIM_MISC_CTL_UART2IRSEL_MASK             (0x400U)
#define SIM_MISC_CTL_UART2IRSEL_SHIFT            (10U)
#define SIM_MISC_CTL_UART2IRSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_UART2IRSEL_SHIFT)) & SIM_MISC_CTL_UART2IRSEL_MASK)
#define SIM_MISC_CTL_UART3IRSEL_MASK             (0x800U)
#define SIM_MISC_CTL_UART3IRSEL_SHIFT            (11U)
#define SIM_MISC_CTL_UART3IRSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_UART3IRSEL_SHIFT)) & SIM_MISC_CTL_UART3IRSEL_MASK)
#define SIM_MISC_CTL_EWMINSEL_MASK               (0x4000U)
#define SIM_MISC_CTL_EWMINSEL_SHIFT              (14U)
#define SIM_MISC_CTL_EWMINSEL(x)                 (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_EWMINSEL_SHIFT)) & SIM_MISC_CTL_EWMINSEL_MASK)
#define SIM_MISC_CTL_TMR0PLLSEL_MASK             (0x8000U)
#define SIM_MISC_CTL_TMR0PLLSEL_SHIFT            (15U)
#define SIM_MISC_CTL_TMR0PLLSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR0PLLSEL_SHIFT)) & SIM_MISC_CTL_TMR0PLLSEL_MASK)
#define SIM_MISC_CTL_TMR0SCSEL_MASK              (0x10000U)
#define SIM_MISC_CTL_TMR0SCSEL_SHIFT             (16U)
#define SIM_MISC_CTL_TMR0SCSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR0SCSEL_SHIFT)) & SIM_MISC_CTL_TMR0SCSEL_MASK)
#define SIM_MISC_CTL_TMR1SCSEL_MASK              (0x20000U)
#define SIM_MISC_CTL_TMR1SCSEL_SHIFT             (17U)
#define SIM_MISC_CTL_TMR1SCSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR1SCSEL_SHIFT)) & SIM_MISC_CTL_TMR1SCSEL_MASK)
#define SIM_MISC_CTL_TMR2SCSEL_MASK              (0x40000U)
#define SIM_MISC_CTL_TMR2SCSEL_SHIFT             (18U)
#define SIM_MISC_CTL_TMR2SCSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR2SCSEL_SHIFT)) & SIM_MISC_CTL_TMR2SCSEL_MASK)
#define SIM_MISC_CTL_TMR3SCSEL_MASK              (0x80000U)
#define SIM_MISC_CTL_TMR3SCSEL_SHIFT             (19U)
#define SIM_MISC_CTL_TMR3SCSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR3SCSEL_SHIFT)) & SIM_MISC_CTL_TMR3SCSEL_MASK)
#define SIM_MISC_CTL_TMR0PCSSEL_MASK             (0x300000U)
#define SIM_MISC_CTL_TMR0PCSSEL_SHIFT            (20U)
#define SIM_MISC_CTL_TMR0PCSSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR0PCSSEL_SHIFT)) & SIM_MISC_CTL_TMR0PCSSEL_MASK)
#define SIM_MISC_CTL_TMR1PCSSEL_MASK             (0xC00000U)
#define SIM_MISC_CTL_TMR1PCSSEL_SHIFT            (22U)
#define SIM_MISC_CTL_TMR1PCSSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR1PCSSEL_SHIFT)) & SIM_MISC_CTL_TMR1PCSSEL_MASK)
#define SIM_MISC_CTL_TMR2PCSSEL_MASK             (0x3000000U)
#define SIM_MISC_CTL_TMR2PCSSEL_SHIFT            (24U)
#define SIM_MISC_CTL_TMR2PCSSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR2PCSSEL_SHIFT)) & SIM_MISC_CTL_TMR2PCSSEL_MASK)
#define SIM_MISC_CTL_TMR3PCSSEL_MASK             (0xC000000U)
#define SIM_MISC_CTL_TMR3PCSSEL_SHIFT            (26U)
#define SIM_MISC_CTL_TMR3PCSSEL(x)               (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_TMR3PCSSEL_SHIFT)) & SIM_MISC_CTL_TMR3PCSSEL_MASK)
#define SIM_MISC_CTL_RTCCLKSEL_MASK              (0x10000000U)
#define SIM_MISC_CTL_RTCCLKSEL_SHIFT             (28U)
#define SIM_MISC_CTL_RTCCLKSEL(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_RTCCLKSEL_SHIFT)) & SIM_MISC_CTL_RTCCLKSEL_MASK)
#define SIM_MISC_CTL_VREFBUFOUTEN_MASK           (0x20000000U)
#define SIM_MISC_CTL_VREFBUFOUTEN_SHIFT          (29U)
#define SIM_MISC_CTL_VREFBUFOUTEN(x)             (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_VREFBUFOUTEN_SHIFT)) & SIM_MISC_CTL_VREFBUFOUTEN_MASK)
#define SIM_MISC_CTL_VREFBUFINSEL_MASK           (0x40000000U)
#define SIM_MISC_CTL_VREFBUFINSEL_SHIFT          (30U)
#define SIM_MISC_CTL_VREFBUFINSEL(x)             (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_VREFBUFINSEL_SHIFT)) & SIM_MISC_CTL_VREFBUFINSEL_MASK)
#define SIM_MISC_CTL_VREFBUFPD_MASK              (0x80000000U)
#define SIM_MISC_CTL_VREFBUFPD_SHIFT             (31U)
#define SIM_MISC_CTL_VREFBUFPD(x)                (((uint32_t)(((uint32_t)(x)) << SIM_MISC_CTL_VREFBUFPD_SHIFT)) & SIM_MISC_CTL_VREFBUFPD_MASK)

/*! @name ADC_COMP0 - ADC Compensation Register 0 */
#define SIM_ADC_COMP0_ADCCOMPVAL0_MASK           (0xFFFFU)
#define SIM_ADC_COMP0_ADCCOMPVAL0_SHIFT          (0U)
#define SIM_ADC_COMP0_ADCCOMPVAL0(x)             (((uint32_t)(((uint32_t)(x)) << SIM_ADC_COMP0_ADCCOMPVAL0_SHIFT)) & SIM_ADC_COMP0_ADCCOMPVAL0_MASK)
#define SIM_ADC_COMP0_ADCCOMPVAL1_MASK           (0xFFFF0000U)
#define SIM_ADC_COMP0_ADCCOMPVAL1_SHIFT          (16U)
#define SIM_ADC_COMP0_ADCCOMPVAL1(x)             (((uint32_t)(((uint32_t)(x)) << SIM_ADC_COMP0_ADCCOMPVAL1_SHIFT)) & SIM_ADC_COMP0_ADCCOMPVAL1_MASK)

/*! @name ADC_COMP1 - ADC Compensation Register 1 */
#define SIM_ADC_COMP1_ADCCOMPVAL2_MASK           (0xFFFFU)
#define SIM_ADC_COMP1_ADCCOMPVAL2_SHIFT          (0U)
#define SIM_ADC_COMP1_ADCCOMPVAL2(x)             (((uint32_t)(((uint32_t)(x)) << SIM_ADC_COMP1_ADCCOMPVAL2_SHIFT)) & SIM_ADC_COMP1_ADCCOMPVAL2_MASK)


/*!
 * @}
 */ /* end of group SIM_Register_Masks */


/* SIM - Peripheral instance base addresses */
/** Peripheral SIM base address */
#define SIM_BASE                                 (0x4003E000u)
/** Peripheral SIM base pointer */
#define SIM                                      ((SIM_Type *)SIM_BASE)
/** Array initializer of SIM peripheral base addresses */
#define SIM_BASE_ADDRS                           { SIM_BASE }
/** Array initializer of SIM peripheral base pointers */
#define SIM_BASE_PTRS                            { SIM }

/*!
 * @}
 */ /* end of group SIM_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- SMC Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Peripheral_Access_Layer SMC Peripheral Access Layer
 * @{
 */

/** SMC - Register Layout Typedef */
typedef struct {
  __IO uint8_t PMPROT;                             /**< Power Mode Protection register, offset: 0x0 */
  __IO uint8_t PMCTRL;                             /**< Power Mode Control register, offset: 0x1 */
  __IO uint8_t STOPCTRL;                           /**< Stop Control Register, offset: 0x2 */
  __I  uint8_t PMSTAT;                             /**< Power Mode Status register, offset: 0x3 */
} SMC_Type;

/* ----------------------------------------------------------------------------
   -- SMC Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SMC_Register_Masks SMC Register Masks
 * @{
 */

/*! @name PMPROT - Power Mode Protection register */
#define SMC_PMPROT_AVLLS_MASK                    (0x2U)
#define SMC_PMPROT_AVLLS_SHIFT                   (1U)
#define SMC_PMPROT_AVLLS(x)                      (((uint8_t)(((uint8_t)(x)) << SMC_PMPROT_AVLLS_SHIFT)) & SMC_PMPROT_AVLLS_MASK)
#define SMC_PMPROT_AVLP_MASK                     (0x20U)
#define SMC_PMPROT_AVLP_SHIFT                    (5U)
#define SMC_PMPROT_AVLP(x)                       (((uint8_t)(((uint8_t)(x)) << SMC_PMPROT_AVLP_SHIFT)) & SMC_PMPROT_AVLP_MASK)

/*! @name PMCTRL - Power Mode Control register */
#define SMC_PMCTRL_STOPM_MASK                    (0x7U)
#define SMC_PMCTRL_STOPM_SHIFT                   (0U)
#define SMC_PMCTRL_STOPM(x)                      (((uint8_t)(((uint8_t)(x)) << SMC_PMCTRL_STOPM_SHIFT)) & SMC_PMCTRL_STOPM_MASK)
#define SMC_PMCTRL_STOPA_MASK                    (0x8U)
#define SMC_PMCTRL_STOPA_SHIFT                   (3U)
#define SMC_PMCTRL_STOPA(x)                      (((uint8_t)(((uint8_t)(x)) << SMC_PMCTRL_STOPA_SHIFT)) & SMC_PMCTRL_STOPA_MASK)
#define SMC_PMCTRL_RUNM_MASK                     (0x60U)
#define SMC_PMCTRL_RUNM_SHIFT                    (5U)
#define SMC_PMCTRL_RUNM(x)                       (((uint8_t)(((uint8_t)(x)) << SMC_PMCTRL_RUNM_SHIFT)) & SMC_PMCTRL_RUNM_MASK)

/*! @name STOPCTRL - Stop Control Register */
#define SMC_STOPCTRL_VLLSM_MASK                  (0x7U)
#define SMC_STOPCTRL_VLLSM_SHIFT                 (0U)
#define SMC_STOPCTRL_VLLSM(x)                    (((uint8_t)(((uint8_t)(x)) << SMC_STOPCTRL_VLLSM_SHIFT)) & SMC_STOPCTRL_VLLSM_MASK)
#define SMC_STOPCTRL_PORPO_MASK                  (0x20U)
#define SMC_STOPCTRL_PORPO_SHIFT                 (5U)
#define SMC_STOPCTRL_PORPO(x)                    (((uint8_t)(((uint8_t)(x)) << SMC_STOPCTRL_PORPO_SHIFT)) & SMC_STOPCTRL_PORPO_MASK)
#define SMC_STOPCTRL_PSTOPO_MASK                 (0xC0U)
#define SMC_STOPCTRL_PSTOPO_SHIFT                (6U)
#define SMC_STOPCTRL_PSTOPO(x)                   (((uint8_t)(((uint8_t)(x)) << SMC_STOPCTRL_PSTOPO_SHIFT)) & SMC_STOPCTRL_PSTOPO_MASK)

/*! @name PMSTAT - Power Mode Status register */
#define SMC_PMSTAT_PMSTAT_MASK                   (0xFFU)
#define SMC_PMSTAT_PMSTAT_SHIFT                  (0U)
#define SMC_PMSTAT_PMSTAT(x)                     (((uint8_t)(((uint8_t)(x)) << SMC_PMSTAT_PMSTAT_SHIFT)) & SMC_PMSTAT_PMSTAT_MASK)


/*!
 * @}
 */ /* end of group SMC_Register_Masks */


/* SMC - Peripheral instance base addresses */
/** Peripheral SMC base address */
#define SMC_BASE                                 (0x4007E000u)
/** Peripheral SMC base pointer */
#define SMC                                      ((SMC_Type *)SMC_BASE)
/** Array initializer of SMC peripheral base addresses */
#define SMC_BASE_ADDRS                           { SMC_BASE }
/** Array initializer of SMC peripheral base pointers */
#define SMC_BASE_PTRS                            { SMC }

/*!
 * @}
 */ /* end of group SMC_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- SPI Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Peripheral_Access_Layer SPI Peripheral Access Layer
 * @{
 */

/** SPI - Register Layout Typedef */
typedef struct {
  __IO uint8_t S;                                  /**< SPI Status Register, offset: 0x0 */
  __IO uint8_t BR;                                 /**< SPI Baud Rate Register, offset: 0x1 */
  __IO uint8_t C2;                                 /**< SPI Control Register 2, offset: 0x2 */
  __IO uint8_t C1;                                 /**< SPI Control Register 1, offset: 0x3 */
  __IO uint8_t ML;                                 /**< SPI Match Register low, offset: 0x4 */
  __IO uint8_t MH;                                 /**< SPI match register high, offset: 0x5 */
  __IO uint8_t DL;                                 /**< SPI Data Register low, offset: 0x6 */
  __IO uint8_t DH;                                 /**< SPI data register high, offset: 0x7 */
       uint8_t RESERVED_0[2];
  __IO uint8_t CI;                                 /**< SPI clear interrupt register, offset: 0xA */
  __IO uint8_t C3;                                 /**< SPI control register 3, offset: 0xB */
} SPI_Type;

/* ----------------------------------------------------------------------------
   -- SPI Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SPI_Register_Masks SPI Register Masks
 * @{
 */

/*! @name S - SPI Status Register */
#define SPI_S_RFIFOEF_MASK                       (0x1U)
#define SPI_S_RFIFOEF_SHIFT                      (0U)
#define SPI_S_RFIFOEF(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_S_RFIFOEF_SHIFT)) & SPI_S_RFIFOEF_MASK)
#define SPI_S_TXFULLF_MASK                       (0x2U)
#define SPI_S_TXFULLF_SHIFT                      (1U)
#define SPI_S_TXFULLF(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_S_TXFULLF_SHIFT)) & SPI_S_TXFULLF_MASK)
#define SPI_S_TNEAREF_MASK                       (0x4U)
#define SPI_S_TNEAREF_SHIFT                      (2U)
#define SPI_S_TNEAREF(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_S_TNEAREF_SHIFT)) & SPI_S_TNEAREF_MASK)
#define SPI_S_RNFULLF_MASK                       (0x8U)
#define SPI_S_RNFULLF_SHIFT                      (3U)
#define SPI_S_RNFULLF(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_S_RNFULLF_SHIFT)) & SPI_S_RNFULLF_MASK)
#define SPI_S_MODF_MASK                          (0x10U)
#define SPI_S_MODF_SHIFT                         (4U)
#define SPI_S_MODF(x)                            (((uint8_t)(((uint8_t)(x)) << SPI_S_MODF_SHIFT)) & SPI_S_MODF_MASK)
#define SPI_S_SPTEF_MASK                         (0x20U)
#define SPI_S_SPTEF_SHIFT                        (5U)
#define SPI_S_SPTEF(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_S_SPTEF_SHIFT)) & SPI_S_SPTEF_MASK)
#define SPI_S_SPMF_MASK                          (0x40U)
#define SPI_S_SPMF_SHIFT                         (6U)
#define SPI_S_SPMF(x)                            (((uint8_t)(((uint8_t)(x)) << SPI_S_SPMF_SHIFT)) & SPI_S_SPMF_MASK)
#define SPI_S_SPRF_MASK                          (0x80U)
#define SPI_S_SPRF_SHIFT                         (7U)
#define SPI_S_SPRF(x)                            (((uint8_t)(((uint8_t)(x)) << SPI_S_SPRF_SHIFT)) & SPI_S_SPRF_MASK)

/*! @name BR - SPI Baud Rate Register */
#define SPI_BR_SPR_MASK                          (0xFU)
#define SPI_BR_SPR_SHIFT                         (0U)
#define SPI_BR_SPR(x)                            (((uint8_t)(((uint8_t)(x)) << SPI_BR_SPR_SHIFT)) & SPI_BR_SPR_MASK)
#define SPI_BR_SPPR_MASK                         (0x70U)
#define SPI_BR_SPPR_SHIFT                        (4U)
#define SPI_BR_SPPR(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_BR_SPPR_SHIFT)) & SPI_BR_SPPR_MASK)

/*! @name C2 - SPI Control Register 2 */
#define SPI_C2_SPC0_MASK                         (0x1U)
#define SPI_C2_SPC0_SHIFT                        (0U)
#define SPI_C2_SPC0(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_C2_SPC0_SHIFT)) & SPI_C2_SPC0_MASK)
#define SPI_C2_SPISWAI_MASK                      (0x2U)
#define SPI_C2_SPISWAI_SHIFT                     (1U)
#define SPI_C2_SPISWAI(x)                        (((uint8_t)(((uint8_t)(x)) << SPI_C2_SPISWAI_SHIFT)) & SPI_C2_SPISWAI_MASK)
#define SPI_C2_RXDMAE_MASK                       (0x4U)
#define SPI_C2_RXDMAE_SHIFT                      (2U)
#define SPI_C2_RXDMAE(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_C2_RXDMAE_SHIFT)) & SPI_C2_RXDMAE_MASK)
#define SPI_C2_BIDIROE_MASK                      (0x8U)
#define SPI_C2_BIDIROE_SHIFT                     (3U)
#define SPI_C2_BIDIROE(x)                        (((uint8_t)(((uint8_t)(x)) << SPI_C2_BIDIROE_SHIFT)) & SPI_C2_BIDIROE_MASK)
#define SPI_C2_MODFEN_MASK                       (0x10U)
#define SPI_C2_MODFEN_SHIFT                      (4U)
#define SPI_C2_MODFEN(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_C2_MODFEN_SHIFT)) & SPI_C2_MODFEN_MASK)
#define SPI_C2_TXDMAE_MASK                       (0x20U)
#define SPI_C2_TXDMAE_SHIFT                      (5U)
#define SPI_C2_TXDMAE(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_C2_TXDMAE_SHIFT)) & SPI_C2_TXDMAE_MASK)
#define SPI_C2_SPIMODE_MASK                      (0x40U)
#define SPI_C2_SPIMODE_SHIFT                     (6U)
#define SPI_C2_SPIMODE(x)                        (((uint8_t)(((uint8_t)(x)) << SPI_C2_SPIMODE_SHIFT)) & SPI_C2_SPIMODE_MASK)
#define SPI_C2_SPMIE_MASK                        (0x80U)
#define SPI_C2_SPMIE_SHIFT                       (7U)
#define SPI_C2_SPMIE(x)                          (((uint8_t)(((uint8_t)(x)) << SPI_C2_SPMIE_SHIFT)) & SPI_C2_SPMIE_MASK)

/*! @name C1 - SPI Control Register 1 */
#define SPI_C1_LSBFE_MASK                        (0x1U)
#define SPI_C1_LSBFE_SHIFT                       (0U)
#define SPI_C1_LSBFE(x)                          (((uint8_t)(((uint8_t)(x)) << SPI_C1_LSBFE_SHIFT)) & SPI_C1_LSBFE_MASK)
#define SPI_C1_SSOE_MASK                         (0x2U)
#define SPI_C1_SSOE_SHIFT                        (1U)
#define SPI_C1_SSOE(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_C1_SSOE_SHIFT)) & SPI_C1_SSOE_MASK)
#define SPI_C1_CPHA_MASK                         (0x4U)
#define SPI_C1_CPHA_SHIFT                        (2U)
#define SPI_C1_CPHA(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_C1_CPHA_SHIFT)) & SPI_C1_CPHA_MASK)
#define SPI_C1_CPOL_MASK                         (0x8U)
#define SPI_C1_CPOL_SHIFT                        (3U)
#define SPI_C1_CPOL(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_C1_CPOL_SHIFT)) & SPI_C1_CPOL_MASK)
#define SPI_C1_MSTR_MASK                         (0x10U)
#define SPI_C1_MSTR_SHIFT                        (4U)
#define SPI_C1_MSTR(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_C1_MSTR_SHIFT)) & SPI_C1_MSTR_MASK)
#define SPI_C1_SPTIE_MASK                        (0x20U)
#define SPI_C1_SPTIE_SHIFT                       (5U)
#define SPI_C1_SPTIE(x)                          (((uint8_t)(((uint8_t)(x)) << SPI_C1_SPTIE_SHIFT)) & SPI_C1_SPTIE_MASK)
#define SPI_C1_SPE_MASK                          (0x40U)
#define SPI_C1_SPE_SHIFT                         (6U)
#define SPI_C1_SPE(x)                            (((uint8_t)(((uint8_t)(x)) << SPI_C1_SPE_SHIFT)) & SPI_C1_SPE_MASK)
#define SPI_C1_SPIE_MASK                         (0x80U)
#define SPI_C1_SPIE_SHIFT                        (7U)
#define SPI_C1_SPIE(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_C1_SPIE_SHIFT)) & SPI_C1_SPIE_MASK)

/*! @name ML - SPI Match Register low */
#define SPI_ML_Bits_MASK                         (0xFFU)
#define SPI_ML_Bits_SHIFT                        (0U)
#define SPI_ML_Bits(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_ML_Bits_SHIFT)) & SPI_ML_Bits_MASK)

/*! @name MH - SPI match register high */
#define SPI_MH_Bits_MASK                         (0xFFU)
#define SPI_MH_Bits_SHIFT                        (0U)
#define SPI_MH_Bits(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_MH_Bits_SHIFT)) & SPI_MH_Bits_MASK)

/*! @name DL - SPI Data Register low */
#define SPI_DL_Bits_MASK                         (0xFFU)
#define SPI_DL_Bits_SHIFT                        (0U)
#define SPI_DL_Bits(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_DL_Bits_SHIFT)) & SPI_DL_Bits_MASK)

/*! @name DH - SPI data register high */
#define SPI_DH_Bits_MASK                         (0xFFU)
#define SPI_DH_Bits_SHIFT                        (0U)
#define SPI_DH_Bits(x)                           (((uint8_t)(((uint8_t)(x)) << SPI_DH_Bits_SHIFT)) & SPI_DH_Bits_MASK)

/*! @name CI - SPI clear interrupt register */
#define SPI_CI_SPRFCI_MASK                       (0x1U)
#define SPI_CI_SPRFCI_SHIFT                      (0U)
#define SPI_CI_SPRFCI(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_CI_SPRFCI_SHIFT)) & SPI_CI_SPRFCI_MASK)
#define SPI_CI_SPTEFCI_MASK                      (0x2U)
#define SPI_CI_SPTEFCI_SHIFT                     (1U)
#define SPI_CI_SPTEFCI(x)                        (((uint8_t)(((uint8_t)(x)) << SPI_CI_SPTEFCI_SHIFT)) & SPI_CI_SPTEFCI_MASK)
#define SPI_CI_RNFULLFCI_MASK                    (0x4U)
#define SPI_CI_RNFULLFCI_SHIFT                   (2U)
#define SPI_CI_RNFULLFCI(x)                      (((uint8_t)(((uint8_t)(x)) << SPI_CI_RNFULLFCI_SHIFT)) & SPI_CI_RNFULLFCI_MASK)
#define SPI_CI_TNEAREFCI_MASK                    (0x8U)
#define SPI_CI_TNEAREFCI_SHIFT                   (3U)
#define SPI_CI_TNEAREFCI(x)                      (((uint8_t)(((uint8_t)(x)) << SPI_CI_TNEAREFCI_SHIFT)) & SPI_CI_TNEAREFCI_MASK)
#define SPI_CI_RXFOF_MASK                        (0x10U)
#define SPI_CI_RXFOF_SHIFT                       (4U)
#define SPI_CI_RXFOF(x)                          (((uint8_t)(((uint8_t)(x)) << SPI_CI_RXFOF_SHIFT)) & SPI_CI_RXFOF_MASK)
#define SPI_CI_TXFOF_MASK                        (0x20U)
#define SPI_CI_TXFOF_SHIFT                       (5U)
#define SPI_CI_TXFOF(x)                          (((uint8_t)(((uint8_t)(x)) << SPI_CI_TXFOF_SHIFT)) & SPI_CI_TXFOF_MASK)
#define SPI_CI_RXFERR_MASK                       (0x40U)
#define SPI_CI_RXFERR_SHIFT                      (6U)
#define SPI_CI_RXFERR(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_CI_RXFERR_SHIFT)) & SPI_CI_RXFERR_MASK)
#define SPI_CI_TXFERR_MASK                       (0x80U)
#define SPI_CI_TXFERR_SHIFT                      (7U)
#define SPI_CI_TXFERR(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_CI_TXFERR_SHIFT)) & SPI_CI_TXFERR_MASK)

/*! @name C3 - SPI control register 3 */
#define SPI_C3_FIFOMODE_MASK                     (0x1U)
#define SPI_C3_FIFOMODE_SHIFT                    (0U)
#define SPI_C3_FIFOMODE(x)                       (((uint8_t)(((uint8_t)(x)) << SPI_C3_FIFOMODE_SHIFT)) & SPI_C3_FIFOMODE_MASK)
#define SPI_C3_RNFULLIEN_MASK                    (0x2U)
#define SPI_C3_RNFULLIEN_SHIFT                   (1U)
#define SPI_C3_RNFULLIEN(x)                      (((uint8_t)(((uint8_t)(x)) << SPI_C3_RNFULLIEN_SHIFT)) & SPI_C3_RNFULLIEN_MASK)
#define SPI_C3_TNEARIEN_MASK                     (0x4U)
#define SPI_C3_TNEARIEN_SHIFT                    (2U)
#define SPI_C3_TNEARIEN(x)                       (((uint8_t)(((uint8_t)(x)) << SPI_C3_TNEARIEN_SHIFT)) & SPI_C3_TNEARIEN_MASK)
#define SPI_C3_INTCLR_MASK                       (0x8U)
#define SPI_C3_INTCLR_SHIFT                      (3U)
#define SPI_C3_INTCLR(x)                         (((uint8_t)(((uint8_t)(x)) << SPI_C3_INTCLR_SHIFT)) & SPI_C3_INTCLR_MASK)
#define SPI_C3_RNFULLF_MARK_MASK                 (0x10U)
#define SPI_C3_RNFULLF_MARK_SHIFT                (4U)
#define SPI_C3_RNFULLF_MARK(x)                   (((uint8_t)(((uint8_t)(x)) << SPI_C3_RNFULLF_MARK_SHIFT)) & SPI_C3_RNFULLF_MARK_MASK)
#define SPI_C3_TNEAREF_MARK_MASK                 (0x20U)
#define SPI_C3_TNEAREF_MARK_SHIFT                (5U)
#define SPI_C3_TNEAREF_MARK(x)                   (((uint8_t)(((uint8_t)(x)) << SPI_C3_TNEAREF_MARK_SHIFT)) & SPI_C3_TNEAREF_MARK_MASK)


/*!
 * @}
 */ /* end of group SPI_Register_Masks */


/* SPI - Peripheral instance base addresses */
/** Peripheral SPI0 base address */
#define SPI0_BASE                                (0x40075000u)
/** Peripheral SPI0 base pointer */
#define SPI0                                     ((SPI_Type *)SPI0_BASE)
/** Peripheral SPI1 base address */
#define SPI1_BASE                                (0x40076000u)
/** Peripheral SPI1 base pointer */
#define SPI1                                     ((SPI_Type *)SPI1_BASE)
/** Array initializer of SPI peripheral base addresses */
#define SPI_BASE_ADDRS                           { SPI0_BASE, SPI1_BASE }
/** Array initializer of SPI peripheral base pointers */
#define SPI_BASE_PTRS                            { SPI0, SPI1 }
/** Interrupt vectors for the SPI peripheral type */
#define SPI_IRQS                                 { SPI0_SPI1_IRQn, SPI0_SPI1_IRQn }

/*!
 * @}
 */ /* end of group SPI_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- SYSMPU Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSMPU_Peripheral_Access_Layer SYSMPU Peripheral Access Layer
 * @{
 */

/** SYSMPU - Register Layout Typedef */
typedef struct {
  __IO uint32_t CESR;                              /**< Control/Error Status Register, offset: 0x0 */
       uint8_t RESERVED_0[12];
  struct {                                         /* offset: 0x10, array step: 0x8 */
    __I  uint32_t EAR;                               /**< Error Address Register, slave port n, array offset: 0x10, array step: 0x8 */
    __I  uint32_t EDR;                               /**< Error Detail Register, slave port n, array offset: 0x14, array step: 0x8 */
  } SP[2];
       uint8_t RESERVED_1[992];
  __IO uint32_t WORD[8][4];                        /**< Region Descriptor n, Word 0..Region Descriptor n, Word 3, array offset: 0x400, array step: index*0x10, index2*0x4 */
       uint8_t RESERVED_2[896];
  __IO uint32_t RGDAAC[8];                         /**< Region Descriptor Alternate Access Control n, array offset: 0x800, array step: 0x4 */
} SYSMPU_Type;

/* ----------------------------------------------------------------------------
   -- SYSMPU Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SYSMPU_Register_Masks SYSMPU Register Masks
 * @{
 */

/*! @name CESR - Control/Error Status Register */
#define SYSMPU_CESR_VLD_MASK                     (0x1U)
#define SYSMPU_CESR_VLD_SHIFT                    (0U)
#define SYSMPU_CESR_VLD(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_CESR_VLD_SHIFT)) & SYSMPU_CESR_VLD_MASK)
#define SYSMPU_CESR_NRGD_MASK                    (0xF00U)
#define SYSMPU_CESR_NRGD_SHIFT                   (8U)
#define SYSMPU_CESR_NRGD(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_CESR_NRGD_SHIFT)) & SYSMPU_CESR_NRGD_MASK)
#define SYSMPU_CESR_NSP_MASK                     (0xF000U)
#define SYSMPU_CESR_NSP_SHIFT                    (12U)
#define SYSMPU_CESR_NSP(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_CESR_NSP_SHIFT)) & SYSMPU_CESR_NSP_MASK)
#define SYSMPU_CESR_HRL_MASK                     (0xF0000U)
#define SYSMPU_CESR_HRL_SHIFT                    (16U)
#define SYSMPU_CESR_HRL(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_CESR_HRL_SHIFT)) & SYSMPU_CESR_HRL_MASK)
#define SYSMPU_CESR_SPERR_MASK                   (0xC0000000U)
#define SYSMPU_CESR_SPERR_SHIFT                  (30U)
#define SYSMPU_CESR_SPERR(x)                     (((uint32_t)(((uint32_t)(x)) << SYSMPU_CESR_SPERR_SHIFT)) & SYSMPU_CESR_SPERR_MASK)

/*! @name EAR - Error Address Register, slave port n */
#define SYSMPU_EAR_EADDR_MASK                    (0xFFFFFFFFU)
#define SYSMPU_EAR_EADDR_SHIFT                   (0U)
#define SYSMPU_EAR_EADDR(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_EAR_EADDR_SHIFT)) & SYSMPU_EAR_EADDR_MASK)

/* The count of SYSMPU_EAR */
#define SYSMPU_EAR_COUNT                         (2U)

/*! @name EDR - Error Detail Register, slave port n */
#define SYSMPU_EDR_ERW_MASK                      (0x1U)
#define SYSMPU_EDR_ERW_SHIFT                     (0U)
#define SYSMPU_EDR_ERW(x)                        (((uint32_t)(((uint32_t)(x)) << SYSMPU_EDR_ERW_SHIFT)) & SYSMPU_EDR_ERW_MASK)
#define SYSMPU_EDR_EATTR_MASK                    (0xEU)
#define SYSMPU_EDR_EATTR_SHIFT                   (1U)
#define SYSMPU_EDR_EATTR(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_EDR_EATTR_SHIFT)) & SYSMPU_EDR_EATTR_MASK)
#define SYSMPU_EDR_EMN_MASK                      (0xF0U)
#define SYSMPU_EDR_EMN_SHIFT                     (4U)
#define SYSMPU_EDR_EMN(x)                        (((uint32_t)(((uint32_t)(x)) << SYSMPU_EDR_EMN_SHIFT)) & SYSMPU_EDR_EMN_MASK)
#define SYSMPU_EDR_EPID_MASK                     (0xFF00U)
#define SYSMPU_EDR_EPID_SHIFT                    (8U)
#define SYSMPU_EDR_EPID(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_EDR_EPID_SHIFT)) & SYSMPU_EDR_EPID_MASK)
#define SYSMPU_EDR_EACD_MASK                     (0xFFFF0000U)
#define SYSMPU_EDR_EACD_SHIFT                    (16U)
#define SYSMPU_EDR_EACD(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_EDR_EACD_SHIFT)) & SYSMPU_EDR_EACD_MASK)

/* The count of SYSMPU_EDR */
#define SYSMPU_EDR_COUNT                         (2U)

/*! @name WORD - Region Descriptor n, Word 0..Region Descriptor n, Word 3 */
#define SYSMPU_WORD_VLD_MASK                     (0x1U)
#define SYSMPU_WORD_VLD_SHIFT                    (0U)
#define SYSMPU_WORD_VLD(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_VLD_SHIFT)) & SYSMPU_WORD_VLD_MASK)
#define SYSMPU_WORD_M0UM_MASK                    (0x7U)
#define SYSMPU_WORD_M0UM_SHIFT                   (0U)
#define SYSMPU_WORD_M0UM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M0UM_SHIFT)) & SYSMPU_WORD_M0UM_MASK)
#define SYSMPU_WORD_M0SM_MASK                    (0x18U)
#define SYSMPU_WORD_M0SM_SHIFT                   (3U)
#define SYSMPU_WORD_M0SM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M0SM_SHIFT)) & SYSMPU_WORD_M0SM_MASK)
#define SYSMPU_WORD_M0PE_MASK                    (0x20U)
#define SYSMPU_WORD_M0PE_SHIFT                   (5U)
#define SYSMPU_WORD_M0PE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M0PE_SHIFT)) & SYSMPU_WORD_M0PE_MASK)
#define SYSMPU_WORD_ENDADDR_MASK                 (0xFFFFFFE0U)
#define SYSMPU_WORD_ENDADDR_SHIFT                (5U)
#define SYSMPU_WORD_ENDADDR(x)                   (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_ENDADDR_SHIFT)) & SYSMPU_WORD_ENDADDR_MASK)
#define SYSMPU_WORD_SRTADDR_MASK                 (0xFFFFFFE0U)
#define SYSMPU_WORD_SRTADDR_SHIFT                (5U)
#define SYSMPU_WORD_SRTADDR(x)                   (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_SRTADDR_SHIFT)) & SYSMPU_WORD_SRTADDR_MASK)
#define SYSMPU_WORD_M1UM_MASK                    (0x1C0U)
#define SYSMPU_WORD_M1UM_SHIFT                   (6U)
#define SYSMPU_WORD_M1UM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M1UM_SHIFT)) & SYSMPU_WORD_M1UM_MASK)
#define SYSMPU_WORD_M1SM_MASK                    (0x600U)
#define SYSMPU_WORD_M1SM_SHIFT                   (9U)
#define SYSMPU_WORD_M1SM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M1SM_SHIFT)) & SYSMPU_WORD_M1SM_MASK)
#define SYSMPU_WORD_M1PE_MASK                    (0x800U)
#define SYSMPU_WORD_M1PE_SHIFT                   (11U)
#define SYSMPU_WORD_M1PE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M1PE_SHIFT)) & SYSMPU_WORD_M1PE_MASK)
#define SYSMPU_WORD_M2UM_MASK                    (0x7000U)
#define SYSMPU_WORD_M2UM_SHIFT                   (12U)
#define SYSMPU_WORD_M2UM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M2UM_SHIFT)) & SYSMPU_WORD_M2UM_MASK)
#define SYSMPU_WORD_M2SM_MASK                    (0x18000U)
#define SYSMPU_WORD_M2SM_SHIFT                   (15U)
#define SYSMPU_WORD_M2SM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M2SM_SHIFT)) & SYSMPU_WORD_M2SM_MASK)
#define SYSMPU_WORD_PIDMASK_MASK                 (0xFF0000U)
#define SYSMPU_WORD_PIDMASK_SHIFT                (16U)
#define SYSMPU_WORD_PIDMASK(x)                   (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_PIDMASK_SHIFT)) & SYSMPU_WORD_PIDMASK_MASK)
#define SYSMPU_WORD_M2PE_MASK                    (0x20000U)
#define SYSMPU_WORD_M2PE_SHIFT                   (17U)
#define SYSMPU_WORD_M2PE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M2PE_SHIFT)) & SYSMPU_WORD_M2PE_MASK)
#define SYSMPU_WORD_M3UM_MASK                    (0x1C0000U)
#define SYSMPU_WORD_M3UM_SHIFT                   (18U)
#define SYSMPU_WORD_M3UM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M3UM_SHIFT)) & SYSMPU_WORD_M3UM_MASK)
#define SYSMPU_WORD_M3SM_MASK                    (0x600000U)
#define SYSMPU_WORD_M3SM_SHIFT                   (21U)
#define SYSMPU_WORD_M3SM(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M3SM_SHIFT)) & SYSMPU_WORD_M3SM_MASK)
#define SYSMPU_WORD_M3PE_MASK                    (0x800000U)
#define SYSMPU_WORD_M3PE_SHIFT                   (23U)
#define SYSMPU_WORD_M3PE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M3PE_SHIFT)) & SYSMPU_WORD_M3PE_MASK)
#define SYSMPU_WORD_PID_MASK                     (0xFF000000U)
#define SYSMPU_WORD_PID_SHIFT                    (24U)
#define SYSMPU_WORD_PID(x)                       (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_PID_SHIFT)) & SYSMPU_WORD_PID_MASK)
#define SYSMPU_WORD_M4WE_MASK                    (0x1000000U)
#define SYSMPU_WORD_M4WE_SHIFT                   (24U)
#define SYSMPU_WORD_M4WE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M4WE_SHIFT)) & SYSMPU_WORD_M4WE_MASK)
#define SYSMPU_WORD_M4RE_MASK                    (0x2000000U)
#define SYSMPU_WORD_M4RE_SHIFT                   (25U)
#define SYSMPU_WORD_M4RE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M4RE_SHIFT)) & SYSMPU_WORD_M4RE_MASK)
#define SYSMPU_WORD_M5WE_MASK                    (0x4000000U)
#define SYSMPU_WORD_M5WE_SHIFT                   (26U)
#define SYSMPU_WORD_M5WE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M5WE_SHIFT)) & SYSMPU_WORD_M5WE_MASK)
#define SYSMPU_WORD_M5RE_MASK                    (0x8000000U)
#define SYSMPU_WORD_M5RE_SHIFT                   (27U)
#define SYSMPU_WORD_M5RE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M5RE_SHIFT)) & SYSMPU_WORD_M5RE_MASK)
#define SYSMPU_WORD_M6WE_MASK                    (0x10000000U)
#define SYSMPU_WORD_M6WE_SHIFT                   (28U)
#define SYSMPU_WORD_M6WE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M6WE_SHIFT)) & SYSMPU_WORD_M6WE_MASK)
#define SYSMPU_WORD_M6RE_MASK                    (0x20000000U)
#define SYSMPU_WORD_M6RE_SHIFT                   (29U)
#define SYSMPU_WORD_M6RE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M6RE_SHIFT)) & SYSMPU_WORD_M6RE_MASK)
#define SYSMPU_WORD_M7WE_MASK                    (0x40000000U)
#define SYSMPU_WORD_M7WE_SHIFT                   (30U)
#define SYSMPU_WORD_M7WE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M7WE_SHIFT)) & SYSMPU_WORD_M7WE_MASK)
#define SYSMPU_WORD_M7RE_MASK                    (0x80000000U)
#define SYSMPU_WORD_M7RE_SHIFT                   (31U)
#define SYSMPU_WORD_M7RE(x)                      (((uint32_t)(((uint32_t)(x)) << SYSMPU_WORD_M7RE_SHIFT)) & SYSMPU_WORD_M7RE_MASK)

/* The count of SYSMPU_WORD */
#define SYSMPU_WORD_COUNT                        (8U)

/* The count of SYSMPU_WORD */
#define SYSMPU_WORD_COUNT2                       (4U)

/*! @name RGDAAC - Region Descriptor Alternate Access Control n */
#define SYSMPU_RGDAAC_M0UM_MASK                  (0x7U)
#define SYSMPU_RGDAAC_M0UM_SHIFT                 (0U)
#define SYSMPU_RGDAAC_M0UM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M0UM_SHIFT)) & SYSMPU_RGDAAC_M0UM_MASK)
#define SYSMPU_RGDAAC_M0SM_MASK                  (0x18U)
#define SYSMPU_RGDAAC_M0SM_SHIFT                 (3U)
#define SYSMPU_RGDAAC_M0SM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M0SM_SHIFT)) & SYSMPU_RGDAAC_M0SM_MASK)
#define SYSMPU_RGDAAC_M0PE_MASK                  (0x20U)
#define SYSMPU_RGDAAC_M0PE_SHIFT                 (5U)
#define SYSMPU_RGDAAC_M0PE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M0PE_SHIFT)) & SYSMPU_RGDAAC_M0PE_MASK)
#define SYSMPU_RGDAAC_M1UM_MASK                  (0x1C0U)
#define SYSMPU_RGDAAC_M1UM_SHIFT                 (6U)
#define SYSMPU_RGDAAC_M1UM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M1UM_SHIFT)) & SYSMPU_RGDAAC_M1UM_MASK)
#define SYSMPU_RGDAAC_M1SM_MASK                  (0x600U)
#define SYSMPU_RGDAAC_M1SM_SHIFT                 (9U)
#define SYSMPU_RGDAAC_M1SM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M1SM_SHIFT)) & SYSMPU_RGDAAC_M1SM_MASK)
#define SYSMPU_RGDAAC_M1PE_MASK                  (0x800U)
#define SYSMPU_RGDAAC_M1PE_SHIFT                 (11U)
#define SYSMPU_RGDAAC_M1PE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M1PE_SHIFT)) & SYSMPU_RGDAAC_M1PE_MASK)
#define SYSMPU_RGDAAC_M2UM_MASK                  (0x7000U)
#define SYSMPU_RGDAAC_M2UM_SHIFT                 (12U)
#define SYSMPU_RGDAAC_M2UM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M2UM_SHIFT)) & SYSMPU_RGDAAC_M2UM_MASK)
#define SYSMPU_RGDAAC_M2SM_MASK                  (0x18000U)
#define SYSMPU_RGDAAC_M2SM_SHIFT                 (15U)
#define SYSMPU_RGDAAC_M2SM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M2SM_SHIFT)) & SYSMPU_RGDAAC_M2SM_MASK)
#define SYSMPU_RGDAAC_M2PE_MASK                  (0x20000U)
#define SYSMPU_RGDAAC_M2PE_SHIFT                 (17U)
#define SYSMPU_RGDAAC_M2PE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M2PE_SHIFT)) & SYSMPU_RGDAAC_M2PE_MASK)
#define SYSMPU_RGDAAC_M3UM_MASK                  (0x1C0000U)
#define SYSMPU_RGDAAC_M3UM_SHIFT                 (18U)
#define SYSMPU_RGDAAC_M3UM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M3UM_SHIFT)) & SYSMPU_RGDAAC_M3UM_MASK)
#define SYSMPU_RGDAAC_M3SM_MASK                  (0x600000U)
#define SYSMPU_RGDAAC_M3SM_SHIFT                 (21U)
#define SYSMPU_RGDAAC_M3SM(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M3SM_SHIFT)) & SYSMPU_RGDAAC_M3SM_MASK)
#define SYSMPU_RGDAAC_M3PE_MASK                  (0x800000U)
#define SYSMPU_RGDAAC_M3PE_SHIFT                 (23U)
#define SYSMPU_RGDAAC_M3PE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M3PE_SHIFT)) & SYSMPU_RGDAAC_M3PE_MASK)
#define SYSMPU_RGDAAC_M4WE_MASK                  (0x1000000U)
#define SYSMPU_RGDAAC_M4WE_SHIFT                 (24U)
#define SYSMPU_RGDAAC_M4WE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M4WE_SHIFT)) & SYSMPU_RGDAAC_M4WE_MASK)
#define SYSMPU_RGDAAC_M4RE_MASK                  (0x2000000U)
#define SYSMPU_RGDAAC_M4RE_SHIFT                 (25U)
#define SYSMPU_RGDAAC_M4RE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M4RE_SHIFT)) & SYSMPU_RGDAAC_M4RE_MASK)
#define SYSMPU_RGDAAC_M5WE_MASK                  (0x4000000U)
#define SYSMPU_RGDAAC_M5WE_SHIFT                 (26U)
#define SYSMPU_RGDAAC_M5WE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M5WE_SHIFT)) & SYSMPU_RGDAAC_M5WE_MASK)
#define SYSMPU_RGDAAC_M5RE_MASK                  (0x8000000U)
#define SYSMPU_RGDAAC_M5RE_SHIFT                 (27U)
#define SYSMPU_RGDAAC_M5RE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M5RE_SHIFT)) & SYSMPU_RGDAAC_M5RE_MASK)
#define SYSMPU_RGDAAC_M6WE_MASK                  (0x10000000U)
#define SYSMPU_RGDAAC_M6WE_SHIFT                 (28U)
#define SYSMPU_RGDAAC_M6WE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M6WE_SHIFT)) & SYSMPU_RGDAAC_M6WE_MASK)
#define SYSMPU_RGDAAC_M6RE_MASK                  (0x20000000U)
#define SYSMPU_RGDAAC_M6RE_SHIFT                 (29U)
#define SYSMPU_RGDAAC_M6RE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M6RE_SHIFT)) & SYSMPU_RGDAAC_M6RE_MASK)
#define SYSMPU_RGDAAC_M7WE_MASK                  (0x40000000U)
#define SYSMPU_RGDAAC_M7WE_SHIFT                 (30U)
#define SYSMPU_RGDAAC_M7WE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M7WE_SHIFT)) & SYSMPU_RGDAAC_M7WE_MASK)
#define SYSMPU_RGDAAC_M7RE_MASK                  (0x80000000U)
#define SYSMPU_RGDAAC_M7RE_SHIFT                 (31U)
#define SYSMPU_RGDAAC_M7RE(x)                    (((uint32_t)(((uint32_t)(x)) << SYSMPU_RGDAAC_M7RE_SHIFT)) & SYSMPU_RGDAAC_M7RE_MASK)

/* The count of SYSMPU_RGDAAC */
#define SYSMPU_RGDAAC_COUNT                      (8U)


/*!
 * @}
 */ /* end of group SYSMPU_Register_Masks */


/* SYSMPU - Peripheral instance base addresses */
/** Peripheral SYSMPU base address */
#define SYSMPU_BASE                              (0x4000A000u)
/** Peripheral SYSMPU base pointer */
#define SYSMPU                                   ((SYSMPU_Type *)SYSMPU_BASE)
/** Array initializer of SYSMPU peripheral base addresses */
#define SYSMPU_BASE_ADDRS                        { SYSMPU_BASE }
/** Array initializer of SYSMPU peripheral base pointers */
#define SYSMPU_BASE_PTRS                         { SYSMPU }

/*!
 * @}
 */ /* end of group SYSMPU_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- TMR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TMR_Peripheral_Access_Layer TMR Peripheral Access Layer
 * @{
 */

/** TMR - Register Layout Typedef */
typedef struct {
  __IO uint16_t COMP1;                             /**< Timer Channel Compare Register 1, offset: 0x0 */
  __IO uint16_t COMP2;                             /**< Timer Channel Compare Register 2, offset: 0x2 */
  __IO uint16_t CAPT;                              /**< Timer Channel Capture Register, offset: 0x4 */
  __IO uint16_t LOAD;                              /**< Timer Channel Load Register, offset: 0x6 */
  __IO uint16_t HOLD;                              /**< Timer Channel Hold Register, offset: 0x8 */
  __IO uint16_t CNTR;                              /**< Timer Channel Counter Register, offset: 0xA */
  __IO uint16_t CTRL;                              /**< Timer Channel Control Register, offset: 0xC */
  __IO uint16_t SCTRL;                             /**< Timer Channel Status and Control Register, offset: 0xE */
  __IO uint16_t CMPLD1;                            /**< Timer Channel Comparator Load Register 1, offset: 0x10 */
  __IO uint16_t CMPLD2;                            /**< Timer Channel Comparator Load Register 2, offset: 0x12 */
  __IO uint16_t CSCTRL;                            /**< Timer Channel Comparator Status and Control Register, offset: 0x14 */
  __IO uint16_t FILT;                              /**< Timer Channel Input Filter Register, offset: 0x16 */
       uint8_t RESERVED_0[6];
  __IO uint16_t ENBL;                              /**< Timer Channel Enable Register, offset: 0x1E */
} TMR_Type;

/* ----------------------------------------------------------------------------
   -- TMR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup TMR_Register_Masks TMR Register Masks
 * @{
 */

/*! @name COMP1 - Timer Channel Compare Register 1 */
#define TMR_COMP1_COMPARISON_1_MASK              (0xFFFFU)
#define TMR_COMP1_COMPARISON_1_SHIFT             (0U)
#define TMR_COMP1_COMPARISON_1(x)                (((uint16_t)(((uint16_t)(x)) << TMR_COMP1_COMPARISON_1_SHIFT)) & TMR_COMP1_COMPARISON_1_MASK)

/*! @name COMP2 - Timer Channel Compare Register 2 */
#define TMR_COMP2_COMPARISON_2_MASK              (0xFFFFU)
#define TMR_COMP2_COMPARISON_2_SHIFT             (0U)
#define TMR_COMP2_COMPARISON_2(x)                (((uint16_t)(((uint16_t)(x)) << TMR_COMP2_COMPARISON_2_SHIFT)) & TMR_COMP2_COMPARISON_2_MASK)

/*! @name CAPT - Timer Channel Capture Register */
#define TMR_CAPT_CAPTURE_MASK                    (0xFFFFU)
#define TMR_CAPT_CAPTURE_SHIFT                   (0U)
#define TMR_CAPT_CAPTURE(x)                      (((uint16_t)(((uint16_t)(x)) << TMR_CAPT_CAPTURE_SHIFT)) & TMR_CAPT_CAPTURE_MASK)

/*! @name LOAD - Timer Channel Load Register */
#define TMR_LOAD_LOAD_MASK                       (0xFFFFU)
#define TMR_LOAD_LOAD_SHIFT                      (0U)
#define TMR_LOAD_LOAD(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_LOAD_LOAD_SHIFT)) & TMR_LOAD_LOAD_MASK)

/*! @name HOLD - Timer Channel Hold Register */
#define TMR_HOLD_HOLD_MASK                       (0xFFFFU)
#define TMR_HOLD_HOLD_SHIFT                      (0U)
#define TMR_HOLD_HOLD(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_HOLD_HOLD_SHIFT)) & TMR_HOLD_HOLD_MASK)

/*! @name CNTR - Timer Channel Counter Register */
#define TMR_CNTR_COUNTER_MASK                    (0xFFFFU)
#define TMR_CNTR_COUNTER_SHIFT                   (0U)
#define TMR_CNTR_COUNTER(x)                      (((uint16_t)(((uint16_t)(x)) << TMR_CNTR_COUNTER_SHIFT)) & TMR_CNTR_COUNTER_MASK)

/*! @name CTRL - Timer Channel Control Register */
#define TMR_CTRL_OUTMODE_MASK                    (0x7U)
#define TMR_CTRL_OUTMODE_SHIFT                   (0U)
#define TMR_CTRL_OUTMODE(x)                      (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_OUTMODE_SHIFT)) & TMR_CTRL_OUTMODE_MASK)
#define TMR_CTRL_COINIT_MASK                     (0x8U)
#define TMR_CTRL_COINIT_SHIFT                    (3U)
#define TMR_CTRL_COINIT(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_COINIT_SHIFT)) & TMR_CTRL_COINIT_MASK)
#define TMR_CTRL_DIR_MASK                        (0x10U)
#define TMR_CTRL_DIR_SHIFT                       (4U)
#define TMR_CTRL_DIR(x)                          (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_DIR_SHIFT)) & TMR_CTRL_DIR_MASK)
#define TMR_CTRL_LENGTH_MASK                     (0x20U)
#define TMR_CTRL_LENGTH_SHIFT                    (5U)
#define TMR_CTRL_LENGTH(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_LENGTH_SHIFT)) & TMR_CTRL_LENGTH_MASK)
#define TMR_CTRL_ONCE_MASK                       (0x40U)
#define TMR_CTRL_ONCE_SHIFT                      (6U)
#define TMR_CTRL_ONCE(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_ONCE_SHIFT)) & TMR_CTRL_ONCE_MASK)
#define TMR_CTRL_SCS_MASK                        (0x180U)
#define TMR_CTRL_SCS_SHIFT                       (7U)
#define TMR_CTRL_SCS(x)                          (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_SCS_SHIFT)) & TMR_CTRL_SCS_MASK)
#define TMR_CTRL_PCS_MASK                        (0x1E00U)
#define TMR_CTRL_PCS_SHIFT                       (9U)
#define TMR_CTRL_PCS(x)                          (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_PCS_SHIFT)) & TMR_CTRL_PCS_MASK)
#define TMR_CTRL_CM_MASK                         (0xE000U)
#define TMR_CTRL_CM_SHIFT                        (13U)
#define TMR_CTRL_CM(x)                           (((uint16_t)(((uint16_t)(x)) << TMR_CTRL_CM_SHIFT)) & TMR_CTRL_CM_MASK)

/*! @name SCTRL - Timer Channel Status and Control Register */
#define TMR_SCTRL_OEN_MASK                       (0x1U)
#define TMR_SCTRL_OEN_SHIFT                      (0U)
#define TMR_SCTRL_OEN(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_OEN_SHIFT)) & TMR_SCTRL_OEN_MASK)
#define TMR_SCTRL_OPS_MASK                       (0x2U)
#define TMR_SCTRL_OPS_SHIFT                      (1U)
#define TMR_SCTRL_OPS(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_OPS_SHIFT)) & TMR_SCTRL_OPS_MASK)
#define TMR_SCTRL_FORCE_MASK                     (0x4U)
#define TMR_SCTRL_FORCE_SHIFT                    (2U)
#define TMR_SCTRL_FORCE(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_FORCE_SHIFT)) & TMR_SCTRL_FORCE_MASK)
#define TMR_SCTRL_VAL_MASK                       (0x8U)
#define TMR_SCTRL_VAL_SHIFT                      (3U)
#define TMR_SCTRL_VAL(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_VAL_SHIFT)) & TMR_SCTRL_VAL_MASK)
#define TMR_SCTRL_EEOF_MASK                      (0x10U)
#define TMR_SCTRL_EEOF_SHIFT                     (4U)
#define TMR_SCTRL_EEOF(x)                        (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_EEOF_SHIFT)) & TMR_SCTRL_EEOF_MASK)
#define TMR_SCTRL_MSTR_MASK                      (0x20U)
#define TMR_SCTRL_MSTR_SHIFT                     (5U)
#define TMR_SCTRL_MSTR(x)                        (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_MSTR_SHIFT)) & TMR_SCTRL_MSTR_MASK)
#define TMR_SCTRL_CAPTURE_MODE_MASK              (0xC0U)
#define TMR_SCTRL_CAPTURE_MODE_SHIFT             (6U)
#define TMR_SCTRL_CAPTURE_MODE(x)                (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_CAPTURE_MODE_SHIFT)) & TMR_SCTRL_CAPTURE_MODE_MASK)
#define TMR_SCTRL_INPUT_MASK                     (0x100U)
#define TMR_SCTRL_INPUT_SHIFT                    (8U)
#define TMR_SCTRL_INPUT(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_INPUT_SHIFT)) & TMR_SCTRL_INPUT_MASK)
#define TMR_SCTRL_IPS_MASK                       (0x200U)
#define TMR_SCTRL_IPS_SHIFT                      (9U)
#define TMR_SCTRL_IPS(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_IPS_SHIFT)) & TMR_SCTRL_IPS_MASK)
#define TMR_SCTRL_IEFIE_MASK                     (0x400U)
#define TMR_SCTRL_IEFIE_SHIFT                    (10U)
#define TMR_SCTRL_IEFIE(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_IEFIE_SHIFT)) & TMR_SCTRL_IEFIE_MASK)
#define TMR_SCTRL_IEF_MASK                       (0x800U)
#define TMR_SCTRL_IEF_SHIFT                      (11U)
#define TMR_SCTRL_IEF(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_IEF_SHIFT)) & TMR_SCTRL_IEF_MASK)
#define TMR_SCTRL_TOFIE_MASK                     (0x1000U)
#define TMR_SCTRL_TOFIE_SHIFT                    (12U)
#define TMR_SCTRL_TOFIE(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_TOFIE_SHIFT)) & TMR_SCTRL_TOFIE_MASK)
#define TMR_SCTRL_TOF_MASK                       (0x2000U)
#define TMR_SCTRL_TOF_SHIFT                      (13U)
#define TMR_SCTRL_TOF(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_TOF_SHIFT)) & TMR_SCTRL_TOF_MASK)
#define TMR_SCTRL_TCFIE_MASK                     (0x4000U)
#define TMR_SCTRL_TCFIE_SHIFT                    (14U)
#define TMR_SCTRL_TCFIE(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_TCFIE_SHIFT)) & TMR_SCTRL_TCFIE_MASK)
#define TMR_SCTRL_TCF_MASK                       (0x8000U)
#define TMR_SCTRL_TCF_SHIFT                      (15U)
#define TMR_SCTRL_TCF(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_SCTRL_TCF_SHIFT)) & TMR_SCTRL_TCF_MASK)

/*! @name CMPLD1 - Timer Channel Comparator Load Register 1 */
#define TMR_CMPLD1_COMPARATOR_LOAD_1_MASK        (0xFFFFU)
#define TMR_CMPLD1_COMPARATOR_LOAD_1_SHIFT       (0U)
#define TMR_CMPLD1_COMPARATOR_LOAD_1(x)          (((uint16_t)(((uint16_t)(x)) << TMR_CMPLD1_COMPARATOR_LOAD_1_SHIFT)) & TMR_CMPLD1_COMPARATOR_LOAD_1_MASK)

/*! @name CMPLD2 - Timer Channel Comparator Load Register 2 */
#define TMR_CMPLD2_COMPARATOR_LOAD_2_MASK        (0xFFFFU)
#define TMR_CMPLD2_COMPARATOR_LOAD_2_SHIFT       (0U)
#define TMR_CMPLD2_COMPARATOR_LOAD_2(x)          (((uint16_t)(((uint16_t)(x)) << TMR_CMPLD2_COMPARATOR_LOAD_2_SHIFT)) & TMR_CMPLD2_COMPARATOR_LOAD_2_MASK)

/*! @name CSCTRL - Timer Channel Comparator Status and Control Register */
#define TMR_CSCTRL_CL1_MASK                      (0x3U)
#define TMR_CSCTRL_CL1_SHIFT                     (0U)
#define TMR_CSCTRL_CL1(x)                        (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_CL1_SHIFT)) & TMR_CSCTRL_CL1_MASK)
#define TMR_CSCTRL_CL2_MASK                      (0xCU)
#define TMR_CSCTRL_CL2_SHIFT                     (2U)
#define TMR_CSCTRL_CL2(x)                        (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_CL2_SHIFT)) & TMR_CSCTRL_CL2_MASK)
#define TMR_CSCTRL_TCF1_MASK                     (0x10U)
#define TMR_CSCTRL_TCF1_SHIFT                    (4U)
#define TMR_CSCTRL_TCF1(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_TCF1_SHIFT)) & TMR_CSCTRL_TCF1_MASK)
#define TMR_CSCTRL_TCF2_MASK                     (0x20U)
#define TMR_CSCTRL_TCF2_SHIFT                    (5U)
#define TMR_CSCTRL_TCF2(x)                       (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_TCF2_SHIFT)) & TMR_CSCTRL_TCF2_MASK)
#define TMR_CSCTRL_TCF1EN_MASK                   (0x40U)
#define TMR_CSCTRL_TCF1EN_SHIFT                  (6U)
#define TMR_CSCTRL_TCF1EN(x)                     (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_TCF1EN_SHIFT)) & TMR_CSCTRL_TCF1EN_MASK)
#define TMR_CSCTRL_TCF2EN_MASK                   (0x80U)
#define TMR_CSCTRL_TCF2EN_SHIFT                  (7U)
#define TMR_CSCTRL_TCF2EN(x)                     (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_TCF2EN_SHIFT)) & TMR_CSCTRL_TCF2EN_MASK)
#define TMR_CSCTRL_OFLAG_MASK                    (0x100U)
#define TMR_CSCTRL_OFLAG_SHIFT                   (8U)
#define TMR_CSCTRL_OFLAG(x)                      (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_OFLAG_SHIFT)) & TMR_CSCTRL_OFLAG_MASK)
#define TMR_CSCTRL_UP_MASK                       (0x200U)
#define TMR_CSCTRL_UP_SHIFT                      (9U)
#define TMR_CSCTRL_UP(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_UP_SHIFT)) & TMR_CSCTRL_UP_MASK)
#define TMR_CSCTRL_TCI_MASK                      (0x400U)
#define TMR_CSCTRL_TCI_SHIFT                     (10U)
#define TMR_CSCTRL_TCI(x)                        (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_TCI_SHIFT)) & TMR_CSCTRL_TCI_MASK)
#define TMR_CSCTRL_ROC_MASK                      (0x800U)
#define TMR_CSCTRL_ROC_SHIFT                     (11U)
#define TMR_CSCTRL_ROC(x)                        (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_ROC_SHIFT)) & TMR_CSCTRL_ROC_MASK)
#define TMR_CSCTRL_ALT_LOAD_MASK                 (0x1000U)
#define TMR_CSCTRL_ALT_LOAD_SHIFT                (12U)
#define TMR_CSCTRL_ALT_LOAD(x)                   (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_ALT_LOAD_SHIFT)) & TMR_CSCTRL_ALT_LOAD_MASK)
#define TMR_CSCTRL_FAULT_MASK                    (0x2000U)
#define TMR_CSCTRL_FAULT_SHIFT                   (13U)
#define TMR_CSCTRL_FAULT(x)                      (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_FAULT_SHIFT)) & TMR_CSCTRL_FAULT_MASK)
#define TMR_CSCTRL_DBG_EN_MASK                   (0xC000U)
#define TMR_CSCTRL_DBG_EN_SHIFT                  (14U)
#define TMR_CSCTRL_DBG_EN(x)                     (((uint16_t)(((uint16_t)(x)) << TMR_CSCTRL_DBG_EN_SHIFT)) & TMR_CSCTRL_DBG_EN_MASK)

/*! @name FILT - Timer Channel Input Filter Register */
#define TMR_FILT_FILT_PER_MASK                   (0xFFU)
#define TMR_FILT_FILT_PER_SHIFT                  (0U)
#define TMR_FILT_FILT_PER(x)                     (((uint16_t)(((uint16_t)(x)) << TMR_FILT_FILT_PER_SHIFT)) & TMR_FILT_FILT_PER_MASK)
#define TMR_FILT_FILT_CNT_MASK                   (0x700U)
#define TMR_FILT_FILT_CNT_SHIFT                  (8U)
#define TMR_FILT_FILT_CNT(x)                     (((uint16_t)(((uint16_t)(x)) << TMR_FILT_FILT_CNT_SHIFT)) & TMR_FILT_FILT_CNT_MASK)

/*! @name ENBL - Timer Channel Enable Register */
#define TMR_ENBL_ENBL_MASK                       (0xFU)
#define TMR_ENBL_ENBL_SHIFT                      (0U)
#define TMR_ENBL_ENBL(x)                         (((uint16_t)(((uint16_t)(x)) << TMR_ENBL_ENBL_SHIFT)) & TMR_ENBL_ENBL_MASK)


/*!
 * @}
 */ /* end of group TMR_Register_Masks */


/* TMR - Peripheral instance base addresses */
/** Peripheral TMR0 base address */
#define TMR0_BASE                                (0x40057000u)
/** Peripheral TMR0 base pointer */
#define TMR0                                     ((TMR_Type *)TMR0_BASE)
/** Peripheral TMR1 base address */
#define TMR1_BASE                                (0x40058000u)
/** Peripheral TMR1 base pointer */
#define TMR1                                     ((TMR_Type *)TMR1_BASE)
/** Peripheral TMR2 base address */
#define TMR2_BASE                                (0x40059000u)
/** Peripheral TMR2 base pointer */
#define TMR2                                     ((TMR_Type *)TMR2_BASE)
/** Peripheral TMR3 base address */
#define TMR3_BASE                                (0x4005A000u)
/** Peripheral TMR3 base pointer */
#define TMR3                                     ((TMR_Type *)TMR3_BASE)
/** Array initializer of TMR peripheral base addresses */
#define TMR_BASE_ADDRS                           { TMR0_BASE, TMR1_BASE, TMR2_BASE, TMR3_BASE }
/** Array initializer of TMR peripheral base pointers */
#define TMR_BASE_PTRS                            { TMR0, TMR1, TMR2, TMR3 }
/** Interrupt vectors for the TMR peripheral type */
#define TMR_IRQS                                 { TMR0_IRQn, TMR1_IRQn, TMR2_IRQn, TMR3_IRQn }

/*!
 * @}
 */ /* end of group TMR_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- UART Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Peripheral_Access_Layer UART Peripheral Access Layer
 * @{
 */

/** UART - Register Layout Typedef */
typedef struct {
  __IO uint8_t BDH;                                /**< UART Baud Rate Registers: High, offset: 0x0 */
  __IO uint8_t BDL;                                /**< UART Baud Rate Registers: Low, offset: 0x1 */
  __IO uint8_t C1;                                 /**< UART Control Register 1, offset: 0x2 */
  __IO uint8_t C2;                                 /**< UART Control Register 2, offset: 0x3 */
  __I  uint8_t S1;                                 /**< UART Status Register 1, offset: 0x4 */
  __IO uint8_t S2;                                 /**< UART Status Register 2, offset: 0x5 */
  __IO uint8_t C3;                                 /**< UART Control Register 3, offset: 0x6 */
  __IO uint8_t D;                                  /**< UART Data Register, offset: 0x7 */
  __IO uint8_t MA1;                                /**< UART Match Address Registers 1, offset: 0x8 */
  __IO uint8_t MA2;                                /**< UART Match Address Registers 2, offset: 0x9 */
  __IO uint8_t C4;                                 /**< UART Control Register 4, offset: 0xA */
  __IO uint8_t C5;                                 /**< UART Control Register 5, offset: 0xB */
  __I  uint8_t ED;                                 /**< UART Extended Data Register, offset: 0xC */
  __IO uint8_t MODEM;                              /**< UART Modem Register, offset: 0xD */
       uint8_t RESERVED_0[2];
  __IO uint8_t PFIFO;                              /**< UART FIFO Parameters, offset: 0x10 */
  __IO uint8_t CFIFO;                              /**< UART FIFO Control Register, offset: 0x11 */
  __IO uint8_t SFIFO;                              /**< UART FIFO Status Register, offset: 0x12 */
  __IO uint8_t TWFIFO;                             /**< UART FIFO Transmit Watermark, offset: 0x13 */
  __I  uint8_t TCFIFO;                             /**< UART FIFO Transmit Count, offset: 0x14 */
  __IO uint8_t RWFIFO;                             /**< UART FIFO Receive Watermark, offset: 0x15 */
  __I  uint8_t RCFIFO;                             /**< UART FIFO Receive Count, offset: 0x16 */
       uint8_t RESERVED_1[1];
  __IO uint8_t C7816;                              /**< UART 7816 Control Register, offset: 0x18 */
  __IO uint8_t IE7816;                             /**< UART 7816 Interrupt Enable Register, offset: 0x19 */
  __IO uint8_t IS7816;                             /**< UART 7816 Interrupt Status Register, offset: 0x1A */
  __IO uint8_t WP7816;                             /**< UART 7816 Wait Parameter Register, offset: 0x1B */
  __IO uint8_t WN7816;                             /**< UART 7816 Wait N Register, offset: 0x1C */
  __IO uint8_t WF7816;                             /**< UART 7816 Wait FD Register, offset: 0x1D */
  __IO uint8_t ET7816;                             /**< UART 7816 Error Threshold Register, offset: 0x1E */
  __IO uint8_t TL7816;                             /**< UART 7816 Transmit Length Register, offset: 0x1F */
       uint8_t RESERVED_2[26];
  __IO uint8_t AP7816A_T0;                         /**< UART 7816 ATR Duration Timer Register A, offset: 0x3A */
  __IO uint8_t AP7816B_T0;                         /**< UART 7816 ATR Duration Timer Register B, offset: 0x3B */
  union {                                          /* offset: 0x3C */
    struct {                                         /* offset: 0x3C */
      __IO uint8_t WP7816A_T0;                         /**< UART 7816 Wait Parameter Register A, offset: 0x3C */
      __IO uint8_t WP7816B_T0;                         /**< UART 7816 Wait Parameter Register B, offset: 0x3D */
    } TYPE0;
    struct {                                         /* offset: 0x3C */
      __IO uint8_t WP7816A_T1;                         /**< UART 7816 Wait Parameter Register A, offset: 0x3C */
      __IO uint8_t WP7816B_T1;                         /**< UART 7816 Wait Parameter Register B, offset: 0x3D */
    } TYPE1;
  };
  __IO uint8_t WGP7816_T1;                         /**< UART 7816 Wait and Guard Parameter Register, offset: 0x3E */
  __IO uint8_t WP7816C_T1;                         /**< UART 7816 Wait Parameter Register C, offset: 0x3F */
} UART_Type;

/* ----------------------------------------------------------------------------
   -- UART Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup UART_Register_Masks UART Register Masks
 * @{
 */

/*! @name BDH - UART Baud Rate Registers: High */
#define UART_BDH_SBR_MASK                        (0x1FU)
#define UART_BDH_SBR_SHIFT                       (0U)
#define UART_BDH_SBR(x)                          (((uint8_t)(((uint8_t)(x)) << UART_BDH_SBR_SHIFT)) & UART_BDH_SBR_MASK)
#define UART_BDH_RXEDGIE_MASK                    (0x40U)
#define UART_BDH_RXEDGIE_SHIFT                   (6U)
#define UART_BDH_RXEDGIE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_BDH_RXEDGIE_SHIFT)) & UART_BDH_RXEDGIE_MASK)

/*! @name BDL - UART Baud Rate Registers: Low */
#define UART_BDL_SBR_MASK                        (0xFFU)
#define UART_BDL_SBR_SHIFT                       (0U)
#define UART_BDL_SBR(x)                          (((uint8_t)(((uint8_t)(x)) << UART_BDL_SBR_SHIFT)) & UART_BDL_SBR_MASK)

/*! @name C1 - UART Control Register 1 */
#define UART_C1_PT_MASK                          (0x1U)
#define UART_C1_PT_SHIFT                         (0U)
#define UART_C1_PT(x)                            (((uint8_t)(((uint8_t)(x)) << UART_C1_PT_SHIFT)) & UART_C1_PT_MASK)
#define UART_C1_PE_MASK                          (0x2U)
#define UART_C1_PE_SHIFT                         (1U)
#define UART_C1_PE(x)                            (((uint8_t)(((uint8_t)(x)) << UART_C1_PE_SHIFT)) & UART_C1_PE_MASK)
#define UART_C1_ILT_MASK                         (0x4U)
#define UART_C1_ILT_SHIFT                        (2U)
#define UART_C1_ILT(x)                           (((uint8_t)(((uint8_t)(x)) << UART_C1_ILT_SHIFT)) & UART_C1_ILT_MASK)
#define UART_C1_WAKE_MASK                        (0x8U)
#define UART_C1_WAKE_SHIFT                       (3U)
#define UART_C1_WAKE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C1_WAKE_SHIFT)) & UART_C1_WAKE_MASK)
#define UART_C1_M_MASK                           (0x10U)
#define UART_C1_M_SHIFT                          (4U)
#define UART_C1_M(x)                             (((uint8_t)(((uint8_t)(x)) << UART_C1_M_SHIFT)) & UART_C1_M_MASK)
#define UART_C1_RSRC_MASK                        (0x20U)
#define UART_C1_RSRC_SHIFT                       (5U)
#define UART_C1_RSRC(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C1_RSRC_SHIFT)) & UART_C1_RSRC_MASK)
#define UART_C1_LOOPS_MASK                       (0x80U)
#define UART_C1_LOOPS_SHIFT                      (7U)
#define UART_C1_LOOPS(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C1_LOOPS_SHIFT)) & UART_C1_LOOPS_MASK)

/*! @name C2 - UART Control Register 2 */
#define UART_C2_SBK_MASK                         (0x1U)
#define UART_C2_SBK_SHIFT                        (0U)
#define UART_C2_SBK(x)                           (((uint8_t)(((uint8_t)(x)) << UART_C2_SBK_SHIFT)) & UART_C2_SBK_MASK)
#define UART_C2_RWU_MASK                         (0x2U)
#define UART_C2_RWU_SHIFT                        (1U)
#define UART_C2_RWU(x)                           (((uint8_t)(((uint8_t)(x)) << UART_C2_RWU_SHIFT)) & UART_C2_RWU_MASK)
#define UART_C2_RE_MASK                          (0x4U)
#define UART_C2_RE_SHIFT                         (2U)
#define UART_C2_RE(x)                            (((uint8_t)(((uint8_t)(x)) << UART_C2_RE_SHIFT)) & UART_C2_RE_MASK)
#define UART_C2_TE_MASK                          (0x8U)
#define UART_C2_TE_SHIFT                         (3U)
#define UART_C2_TE(x)                            (((uint8_t)(((uint8_t)(x)) << UART_C2_TE_SHIFT)) & UART_C2_TE_MASK)
#define UART_C2_ILIE_MASK                        (0x10U)
#define UART_C2_ILIE_SHIFT                       (4U)
#define UART_C2_ILIE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C2_ILIE_SHIFT)) & UART_C2_ILIE_MASK)
#define UART_C2_RIE_MASK                         (0x20U)
#define UART_C2_RIE_SHIFT                        (5U)
#define UART_C2_RIE(x)                           (((uint8_t)(((uint8_t)(x)) << UART_C2_RIE_SHIFT)) & UART_C2_RIE_MASK)
#define UART_C2_TCIE_MASK                        (0x40U)
#define UART_C2_TCIE_SHIFT                       (6U)
#define UART_C2_TCIE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C2_TCIE_SHIFT)) & UART_C2_TCIE_MASK)
#define UART_C2_TIE_MASK                         (0x80U)
#define UART_C2_TIE_SHIFT                        (7U)
#define UART_C2_TIE(x)                           (((uint8_t)(((uint8_t)(x)) << UART_C2_TIE_SHIFT)) & UART_C2_TIE_MASK)

/*! @name S1 - UART Status Register 1 */
#define UART_S1_PF_MASK                          (0x1U)
#define UART_S1_PF_SHIFT                         (0U)
#define UART_S1_PF(x)                            (((uint8_t)(((uint8_t)(x)) << UART_S1_PF_SHIFT)) & UART_S1_PF_MASK)
#define UART_S1_FE_MASK                          (0x2U)
#define UART_S1_FE_SHIFT                         (1U)
#define UART_S1_FE(x)                            (((uint8_t)(((uint8_t)(x)) << UART_S1_FE_SHIFT)) & UART_S1_FE_MASK)
#define UART_S1_NF_MASK                          (0x4U)
#define UART_S1_NF_SHIFT                         (2U)
#define UART_S1_NF(x)                            (((uint8_t)(((uint8_t)(x)) << UART_S1_NF_SHIFT)) & UART_S1_NF_MASK)
#define UART_S1_OR_MASK                          (0x8U)
#define UART_S1_OR_SHIFT                         (3U)
#define UART_S1_OR(x)                            (((uint8_t)(((uint8_t)(x)) << UART_S1_OR_SHIFT)) & UART_S1_OR_MASK)
#define UART_S1_IDLE_MASK                        (0x10U)
#define UART_S1_IDLE_SHIFT                       (4U)
#define UART_S1_IDLE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_S1_IDLE_SHIFT)) & UART_S1_IDLE_MASK)
#define UART_S1_RDRF_MASK                        (0x20U)
#define UART_S1_RDRF_SHIFT                       (5U)
#define UART_S1_RDRF(x)                          (((uint8_t)(((uint8_t)(x)) << UART_S1_RDRF_SHIFT)) & UART_S1_RDRF_MASK)
#define UART_S1_TC_MASK                          (0x40U)
#define UART_S1_TC_SHIFT                         (6U)
#define UART_S1_TC(x)                            (((uint8_t)(((uint8_t)(x)) << UART_S1_TC_SHIFT)) & UART_S1_TC_MASK)
#define UART_S1_TDRE_MASK                        (0x80U)
#define UART_S1_TDRE_SHIFT                       (7U)
#define UART_S1_TDRE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_S1_TDRE_SHIFT)) & UART_S1_TDRE_MASK)

/*! @name S2 - UART Status Register 2 */
#define UART_S2_RAF_MASK                         (0x1U)
#define UART_S2_RAF_SHIFT                        (0U)
#define UART_S2_RAF(x)                           (((uint8_t)(((uint8_t)(x)) << UART_S2_RAF_SHIFT)) & UART_S2_RAF_MASK)
#define UART_S2_BRK13_MASK                       (0x4U)
#define UART_S2_BRK13_SHIFT                      (2U)
#define UART_S2_BRK13(x)                         (((uint8_t)(((uint8_t)(x)) << UART_S2_BRK13_SHIFT)) & UART_S2_BRK13_MASK)
#define UART_S2_RWUID_MASK                       (0x8U)
#define UART_S2_RWUID_SHIFT                      (3U)
#define UART_S2_RWUID(x)                         (((uint8_t)(((uint8_t)(x)) << UART_S2_RWUID_SHIFT)) & UART_S2_RWUID_MASK)
#define UART_S2_RXINV_MASK                       (0x10U)
#define UART_S2_RXINV_SHIFT                      (4U)
#define UART_S2_RXINV(x)                         (((uint8_t)(((uint8_t)(x)) << UART_S2_RXINV_SHIFT)) & UART_S2_RXINV_MASK)
#define UART_S2_MSBF_MASK                        (0x20U)
#define UART_S2_MSBF_SHIFT                       (5U)
#define UART_S2_MSBF(x)                          (((uint8_t)(((uint8_t)(x)) << UART_S2_MSBF_SHIFT)) & UART_S2_MSBF_MASK)
#define UART_S2_RXEDGIF_MASK                     (0x40U)
#define UART_S2_RXEDGIF_SHIFT                    (6U)
#define UART_S2_RXEDGIF(x)                       (((uint8_t)(((uint8_t)(x)) << UART_S2_RXEDGIF_SHIFT)) & UART_S2_RXEDGIF_MASK)

/*! @name C3 - UART Control Register 3 */
#define UART_C3_PEIE_MASK                        (0x1U)
#define UART_C3_PEIE_SHIFT                       (0U)
#define UART_C3_PEIE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C3_PEIE_SHIFT)) & UART_C3_PEIE_MASK)
#define UART_C3_FEIE_MASK                        (0x2U)
#define UART_C3_FEIE_SHIFT                       (1U)
#define UART_C3_FEIE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C3_FEIE_SHIFT)) & UART_C3_FEIE_MASK)
#define UART_C3_NEIE_MASK                        (0x4U)
#define UART_C3_NEIE_SHIFT                       (2U)
#define UART_C3_NEIE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C3_NEIE_SHIFT)) & UART_C3_NEIE_MASK)
#define UART_C3_ORIE_MASK                        (0x8U)
#define UART_C3_ORIE_SHIFT                       (3U)
#define UART_C3_ORIE(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C3_ORIE_SHIFT)) & UART_C3_ORIE_MASK)
#define UART_C3_TXINV_MASK                       (0x10U)
#define UART_C3_TXINV_SHIFT                      (4U)
#define UART_C3_TXINV(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C3_TXINV_SHIFT)) & UART_C3_TXINV_MASK)
#define UART_C3_TXDIR_MASK                       (0x20U)
#define UART_C3_TXDIR_SHIFT                      (5U)
#define UART_C3_TXDIR(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C3_TXDIR_SHIFT)) & UART_C3_TXDIR_MASK)
#define UART_C3_T8_MASK                          (0x40U)
#define UART_C3_T8_SHIFT                         (6U)
#define UART_C3_T8(x)                            (((uint8_t)(((uint8_t)(x)) << UART_C3_T8_SHIFT)) & UART_C3_T8_MASK)
#define UART_C3_R8_MASK                          (0x80U)
#define UART_C3_R8_SHIFT                         (7U)
#define UART_C3_R8(x)                            (((uint8_t)(((uint8_t)(x)) << UART_C3_R8_SHIFT)) & UART_C3_R8_MASK)

/*! @name D - UART Data Register */
#define UART_D_RT_MASK                           (0xFFU)
#define UART_D_RT_SHIFT                          (0U)
#define UART_D_RT(x)                             (((uint8_t)(((uint8_t)(x)) << UART_D_RT_SHIFT)) & UART_D_RT_MASK)

/*! @name MA1 - UART Match Address Registers 1 */
#define UART_MA1_MA_MASK                         (0xFFU)
#define UART_MA1_MA_SHIFT                        (0U)
#define UART_MA1_MA(x)                           (((uint8_t)(((uint8_t)(x)) << UART_MA1_MA_SHIFT)) & UART_MA1_MA_MASK)

/*! @name MA2 - UART Match Address Registers 2 */
#define UART_MA2_MA_MASK                         (0xFFU)
#define UART_MA2_MA_SHIFT                        (0U)
#define UART_MA2_MA(x)                           (((uint8_t)(((uint8_t)(x)) << UART_MA2_MA_SHIFT)) & UART_MA2_MA_MASK)

/*! @name C4 - UART Control Register 4 */
#define UART_C4_BRFA_MASK                        (0x1FU)
#define UART_C4_BRFA_SHIFT                       (0U)
#define UART_C4_BRFA(x)                          (((uint8_t)(((uint8_t)(x)) << UART_C4_BRFA_SHIFT)) & UART_C4_BRFA_MASK)
#define UART_C4_M10_MASK                         (0x20U)
#define UART_C4_M10_SHIFT                        (5U)
#define UART_C4_M10(x)                           (((uint8_t)(((uint8_t)(x)) << UART_C4_M10_SHIFT)) & UART_C4_M10_MASK)
#define UART_C4_MAEN2_MASK                       (0x40U)
#define UART_C4_MAEN2_SHIFT                      (6U)
#define UART_C4_MAEN2(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C4_MAEN2_SHIFT)) & UART_C4_MAEN2_MASK)
#define UART_C4_MAEN1_MASK                       (0x80U)
#define UART_C4_MAEN1_SHIFT                      (7U)
#define UART_C4_MAEN1(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C4_MAEN1_SHIFT)) & UART_C4_MAEN1_MASK)

/*! @name C5 - UART Control Register 5 */
#define UART_C5_ILDMAS_MASK                      (0x10U)
#define UART_C5_ILDMAS_SHIFT                     (4U)
#define UART_C5_ILDMAS(x)                        (((uint8_t)(((uint8_t)(x)) << UART_C5_ILDMAS_SHIFT)) & UART_C5_ILDMAS_MASK)
#define UART_C5_RDMAS_MASK                       (0x20U)
#define UART_C5_RDMAS_SHIFT                      (5U)
#define UART_C5_RDMAS(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C5_RDMAS_SHIFT)) & UART_C5_RDMAS_MASK)
#define UART_C5_TDMAS_MASK                       (0x80U)
#define UART_C5_TDMAS_SHIFT                      (7U)
#define UART_C5_TDMAS(x)                         (((uint8_t)(((uint8_t)(x)) << UART_C5_TDMAS_SHIFT)) & UART_C5_TDMAS_MASK)

/*! @name ED - UART Extended Data Register */
#define UART_ED_PARITYE_MASK                     (0x40U)
#define UART_ED_PARITYE_SHIFT                    (6U)
#define UART_ED_PARITYE(x)                       (((uint8_t)(((uint8_t)(x)) << UART_ED_PARITYE_SHIFT)) & UART_ED_PARITYE_MASK)
#define UART_ED_NOISY_MASK                       (0x80U)
#define UART_ED_NOISY_SHIFT                      (7U)
#define UART_ED_NOISY(x)                         (((uint8_t)(((uint8_t)(x)) << UART_ED_NOISY_SHIFT)) & UART_ED_NOISY_MASK)

/*! @name MODEM - UART Modem Register */
#define UART_MODEM_TXCTSE_MASK                   (0x1U)
#define UART_MODEM_TXCTSE_SHIFT                  (0U)
#define UART_MODEM_TXCTSE(x)                     (((uint8_t)(((uint8_t)(x)) << UART_MODEM_TXCTSE_SHIFT)) & UART_MODEM_TXCTSE_MASK)
#define UART_MODEM_TXRTSE_MASK                   (0x2U)
#define UART_MODEM_TXRTSE_SHIFT                  (1U)
#define UART_MODEM_TXRTSE(x)                     (((uint8_t)(((uint8_t)(x)) << UART_MODEM_TXRTSE_SHIFT)) & UART_MODEM_TXRTSE_MASK)
#define UART_MODEM_TXRTSPOL_MASK                 (0x4U)
#define UART_MODEM_TXRTSPOL_SHIFT                (2U)
#define UART_MODEM_TXRTSPOL(x)                   (((uint8_t)(((uint8_t)(x)) << UART_MODEM_TXRTSPOL_SHIFT)) & UART_MODEM_TXRTSPOL_MASK)
#define UART_MODEM_RXRTSE_MASK                   (0x8U)
#define UART_MODEM_RXRTSE_SHIFT                  (3U)
#define UART_MODEM_RXRTSE(x)                     (((uint8_t)(((uint8_t)(x)) << UART_MODEM_RXRTSE_SHIFT)) & UART_MODEM_RXRTSE_MASK)

/*! @name PFIFO - UART FIFO Parameters */
#define UART_PFIFO_RXFIFOSIZE_MASK               (0x7U)
#define UART_PFIFO_RXFIFOSIZE_SHIFT              (0U)
#define UART_PFIFO_RXFIFOSIZE(x)                 (((uint8_t)(((uint8_t)(x)) << UART_PFIFO_RXFIFOSIZE_SHIFT)) & UART_PFIFO_RXFIFOSIZE_MASK)
#define UART_PFIFO_RXFE_MASK                     (0x8U)
#define UART_PFIFO_RXFE_SHIFT                    (3U)
#define UART_PFIFO_RXFE(x)                       (((uint8_t)(((uint8_t)(x)) << UART_PFIFO_RXFE_SHIFT)) & UART_PFIFO_RXFE_MASK)
#define UART_PFIFO_TXFIFOSIZE_MASK               (0x70U)
#define UART_PFIFO_TXFIFOSIZE_SHIFT              (4U)
#define UART_PFIFO_TXFIFOSIZE(x)                 (((uint8_t)(((uint8_t)(x)) << UART_PFIFO_TXFIFOSIZE_SHIFT)) & UART_PFIFO_TXFIFOSIZE_MASK)
#define UART_PFIFO_TXFE_MASK                     (0x80U)
#define UART_PFIFO_TXFE_SHIFT                    (7U)
#define UART_PFIFO_TXFE(x)                       (((uint8_t)(((uint8_t)(x)) << UART_PFIFO_TXFE_SHIFT)) & UART_PFIFO_TXFE_MASK)

/*! @name CFIFO - UART FIFO Control Register */
#define UART_CFIFO_RXUFE_MASK                    (0x1U)
#define UART_CFIFO_RXUFE_SHIFT                   (0U)
#define UART_CFIFO_RXUFE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_CFIFO_RXUFE_SHIFT)) & UART_CFIFO_RXUFE_MASK)
#define UART_CFIFO_TXOFE_MASK                    (0x2U)
#define UART_CFIFO_TXOFE_SHIFT                   (1U)
#define UART_CFIFO_TXOFE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_CFIFO_TXOFE_SHIFT)) & UART_CFIFO_TXOFE_MASK)
#define UART_CFIFO_RXOFE_MASK                    (0x4U)
#define UART_CFIFO_RXOFE_SHIFT                   (2U)
#define UART_CFIFO_RXOFE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_CFIFO_RXOFE_SHIFT)) & UART_CFIFO_RXOFE_MASK)
#define UART_CFIFO_RXFLUSH_MASK                  (0x40U)
#define UART_CFIFO_RXFLUSH_SHIFT                 (6U)
#define UART_CFIFO_RXFLUSH(x)                    (((uint8_t)(((uint8_t)(x)) << UART_CFIFO_RXFLUSH_SHIFT)) & UART_CFIFO_RXFLUSH_MASK)
#define UART_CFIFO_TXFLUSH_MASK                  (0x80U)
#define UART_CFIFO_TXFLUSH_SHIFT                 (7U)
#define UART_CFIFO_TXFLUSH(x)                    (((uint8_t)(((uint8_t)(x)) << UART_CFIFO_TXFLUSH_SHIFT)) & UART_CFIFO_TXFLUSH_MASK)

/*! @name SFIFO - UART FIFO Status Register */
#define UART_SFIFO_RXUF_MASK                     (0x1U)
#define UART_SFIFO_RXUF_SHIFT                    (0U)
#define UART_SFIFO_RXUF(x)                       (((uint8_t)(((uint8_t)(x)) << UART_SFIFO_RXUF_SHIFT)) & UART_SFIFO_RXUF_MASK)
#define UART_SFIFO_TXOF_MASK                     (0x2U)
#define UART_SFIFO_TXOF_SHIFT                    (1U)
#define UART_SFIFO_TXOF(x)                       (((uint8_t)(((uint8_t)(x)) << UART_SFIFO_TXOF_SHIFT)) & UART_SFIFO_TXOF_MASK)
#define UART_SFIFO_RXOF_MASK                     (0x4U)
#define UART_SFIFO_RXOF_SHIFT                    (2U)
#define UART_SFIFO_RXOF(x)                       (((uint8_t)(((uint8_t)(x)) << UART_SFIFO_RXOF_SHIFT)) & UART_SFIFO_RXOF_MASK)
#define UART_SFIFO_RXEMPT_MASK                   (0x40U)
#define UART_SFIFO_RXEMPT_SHIFT                  (6U)
#define UART_SFIFO_RXEMPT(x)                     (((uint8_t)(((uint8_t)(x)) << UART_SFIFO_RXEMPT_SHIFT)) & UART_SFIFO_RXEMPT_MASK)
#define UART_SFIFO_TXEMPT_MASK                   (0x80U)
#define UART_SFIFO_TXEMPT_SHIFT                  (7U)
#define UART_SFIFO_TXEMPT(x)                     (((uint8_t)(((uint8_t)(x)) << UART_SFIFO_TXEMPT_SHIFT)) & UART_SFIFO_TXEMPT_MASK)

/*! @name TWFIFO - UART FIFO Transmit Watermark */
#define UART_TWFIFO_TXWATER_MASK                 (0xFFU)
#define UART_TWFIFO_TXWATER_SHIFT                (0U)
#define UART_TWFIFO_TXWATER(x)                   (((uint8_t)(((uint8_t)(x)) << UART_TWFIFO_TXWATER_SHIFT)) & UART_TWFIFO_TXWATER_MASK)

/*! @name TCFIFO - UART FIFO Transmit Count */
#define UART_TCFIFO_TXCOUNT_MASK                 (0xFFU)
#define UART_TCFIFO_TXCOUNT_SHIFT                (0U)
#define UART_TCFIFO_TXCOUNT(x)                   (((uint8_t)(((uint8_t)(x)) << UART_TCFIFO_TXCOUNT_SHIFT)) & UART_TCFIFO_TXCOUNT_MASK)

/*! @name RWFIFO - UART FIFO Receive Watermark */
#define UART_RWFIFO_RXWATER_MASK                 (0xFFU)
#define UART_RWFIFO_RXWATER_SHIFT                (0U)
#define UART_RWFIFO_RXWATER(x)                   (((uint8_t)(((uint8_t)(x)) << UART_RWFIFO_RXWATER_SHIFT)) & UART_RWFIFO_RXWATER_MASK)

/*! @name RCFIFO - UART FIFO Receive Count */
#define UART_RCFIFO_RXCOUNT_MASK                 (0xFFU)
#define UART_RCFIFO_RXCOUNT_SHIFT                (0U)
#define UART_RCFIFO_RXCOUNT(x)                   (((uint8_t)(((uint8_t)(x)) << UART_RCFIFO_RXCOUNT_SHIFT)) & UART_RCFIFO_RXCOUNT_MASK)

/*! @name C7816 - UART 7816 Control Register */
#define UART_C7816_ISO_7816E_MASK                (0x1U)
#define UART_C7816_ISO_7816E_SHIFT               (0U)
#define UART_C7816_ISO_7816E(x)                  (((uint8_t)(((uint8_t)(x)) << UART_C7816_ISO_7816E_SHIFT)) & UART_C7816_ISO_7816E_MASK)
#define UART_C7816_TTYPE_MASK                    (0x2U)
#define UART_C7816_TTYPE_SHIFT                   (1U)
#define UART_C7816_TTYPE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_C7816_TTYPE_SHIFT)) & UART_C7816_TTYPE_MASK)
#define UART_C7816_INIT_MASK                     (0x4U)
#define UART_C7816_INIT_SHIFT                    (2U)
#define UART_C7816_INIT(x)                       (((uint8_t)(((uint8_t)(x)) << UART_C7816_INIT_SHIFT)) & UART_C7816_INIT_MASK)
#define UART_C7816_ANACK_MASK                    (0x8U)
#define UART_C7816_ANACK_SHIFT                   (3U)
#define UART_C7816_ANACK(x)                      (((uint8_t)(((uint8_t)(x)) << UART_C7816_ANACK_SHIFT)) & UART_C7816_ANACK_MASK)
#define UART_C7816_ONACK_MASK                    (0x10U)
#define UART_C7816_ONACK_SHIFT                   (4U)
#define UART_C7816_ONACK(x)                      (((uint8_t)(((uint8_t)(x)) << UART_C7816_ONACK_SHIFT)) & UART_C7816_ONACK_MASK)

/*! @name IE7816 - UART 7816 Interrupt Enable Register */
#define UART_IE7816_RXTE_MASK                    (0x1U)
#define UART_IE7816_RXTE_SHIFT                   (0U)
#define UART_IE7816_RXTE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_IE7816_RXTE_SHIFT)) & UART_IE7816_RXTE_MASK)
#define UART_IE7816_TXTE_MASK                    (0x2U)
#define UART_IE7816_TXTE_SHIFT                   (1U)
#define UART_IE7816_TXTE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_IE7816_TXTE_SHIFT)) & UART_IE7816_TXTE_MASK)
#define UART_IE7816_GTVE_MASK                    (0x4U)
#define UART_IE7816_GTVE_SHIFT                   (2U)
#define UART_IE7816_GTVE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_IE7816_GTVE_SHIFT)) & UART_IE7816_GTVE_MASK)
#define UART_IE7816_ADTE_MASK                    (0x8U)
#define UART_IE7816_ADTE_SHIFT                   (3U)
#define UART_IE7816_ADTE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_IE7816_ADTE_SHIFT)) & UART_IE7816_ADTE_MASK)
#define UART_IE7816_INITDE_MASK                  (0x10U)
#define UART_IE7816_INITDE_SHIFT                 (4U)
#define UART_IE7816_INITDE(x)                    (((uint8_t)(((uint8_t)(x)) << UART_IE7816_INITDE_SHIFT)) & UART_IE7816_INITDE_MASK)
#define UART_IE7816_BWTE_MASK                    (0x20U)
#define UART_IE7816_BWTE_SHIFT                   (5U)
#define UART_IE7816_BWTE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_IE7816_BWTE_SHIFT)) & UART_IE7816_BWTE_MASK)
#define UART_IE7816_CWTE_MASK                    (0x40U)
#define UART_IE7816_CWTE_SHIFT                   (6U)
#define UART_IE7816_CWTE(x)                      (((uint8_t)(((uint8_t)(x)) << UART_IE7816_CWTE_SHIFT)) & UART_IE7816_CWTE_MASK)
#define UART_IE7816_WTE_MASK                     (0x80U)
#define UART_IE7816_WTE_SHIFT                    (7U)
#define UART_IE7816_WTE(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IE7816_WTE_SHIFT)) & UART_IE7816_WTE_MASK)

/*! @name IS7816 - UART 7816 Interrupt Status Register */
#define UART_IS7816_RXT_MASK                     (0x1U)
#define UART_IS7816_RXT_SHIFT                    (0U)
#define UART_IS7816_RXT(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IS7816_RXT_SHIFT)) & UART_IS7816_RXT_MASK)
#define UART_IS7816_TXT_MASK                     (0x2U)
#define UART_IS7816_TXT_SHIFT                    (1U)
#define UART_IS7816_TXT(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IS7816_TXT_SHIFT)) & UART_IS7816_TXT_MASK)
#define UART_IS7816_GTV_MASK                     (0x4U)
#define UART_IS7816_GTV_SHIFT                    (2U)
#define UART_IS7816_GTV(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IS7816_GTV_SHIFT)) & UART_IS7816_GTV_MASK)
#define UART_IS7816_ADT_MASK                     (0x8U)
#define UART_IS7816_ADT_SHIFT                    (3U)
#define UART_IS7816_ADT(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IS7816_ADT_SHIFT)) & UART_IS7816_ADT_MASK)
#define UART_IS7816_INITD_MASK                   (0x10U)
#define UART_IS7816_INITD_SHIFT                  (4U)
#define UART_IS7816_INITD(x)                     (((uint8_t)(((uint8_t)(x)) << UART_IS7816_INITD_SHIFT)) & UART_IS7816_INITD_MASK)
#define UART_IS7816_BWT_MASK                     (0x20U)
#define UART_IS7816_BWT_SHIFT                    (5U)
#define UART_IS7816_BWT(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IS7816_BWT_SHIFT)) & UART_IS7816_BWT_MASK)
#define UART_IS7816_CWT_MASK                     (0x40U)
#define UART_IS7816_CWT_SHIFT                    (6U)
#define UART_IS7816_CWT(x)                       (((uint8_t)(((uint8_t)(x)) << UART_IS7816_CWT_SHIFT)) & UART_IS7816_CWT_MASK)
#define UART_IS7816_WT_MASK                      (0x80U)
#define UART_IS7816_WT_SHIFT                     (7U)
#define UART_IS7816_WT(x)                        (((uint8_t)(((uint8_t)(x)) << UART_IS7816_WT_SHIFT)) & UART_IS7816_WT_MASK)

/*! @name WP7816 - UART 7816 Wait Parameter Register */
#define UART_WP7816_WTX_MASK                     (0xFFU)
#define UART_WP7816_WTX_SHIFT                    (0U)
#define UART_WP7816_WTX(x)                       (((uint8_t)(((uint8_t)(x)) << UART_WP7816_WTX_SHIFT)) & UART_WP7816_WTX_MASK)

/*! @name WN7816 - UART 7816 Wait N Register */
#define UART_WN7816_GTN_MASK                     (0xFFU)
#define UART_WN7816_GTN_SHIFT                    (0U)
#define UART_WN7816_GTN(x)                       (((uint8_t)(((uint8_t)(x)) << UART_WN7816_GTN_SHIFT)) & UART_WN7816_GTN_MASK)

/*! @name WF7816 - UART 7816 Wait FD Register */
#define UART_WF7816_GTFD_MASK                    (0xFFU)
#define UART_WF7816_GTFD_SHIFT                   (0U)
#define UART_WF7816_GTFD(x)                      (((uint8_t)(((uint8_t)(x)) << UART_WF7816_GTFD_SHIFT)) & UART_WF7816_GTFD_MASK)

/*! @name ET7816 - UART 7816 Error Threshold Register */
#define UART_ET7816_RXTHRESHOLD_MASK             (0xFU)
#define UART_ET7816_RXTHRESHOLD_SHIFT            (0U)
#define UART_ET7816_RXTHRESHOLD(x)               (((uint8_t)(((uint8_t)(x)) << UART_ET7816_RXTHRESHOLD_SHIFT)) & UART_ET7816_RXTHRESHOLD_MASK)
#define UART_ET7816_TXTHRESHOLD_MASK             (0xF0U)
#define UART_ET7816_TXTHRESHOLD_SHIFT            (4U)
#define UART_ET7816_TXTHRESHOLD(x)               (((uint8_t)(((uint8_t)(x)) << UART_ET7816_TXTHRESHOLD_SHIFT)) & UART_ET7816_TXTHRESHOLD_MASK)

/*! @name TL7816 - UART 7816 Transmit Length Register */
#define UART_TL7816_TLEN_MASK                    (0xFFU)
#define UART_TL7816_TLEN_SHIFT                   (0U)
#define UART_TL7816_TLEN(x)                      (((uint8_t)(((uint8_t)(x)) << UART_TL7816_TLEN_SHIFT)) & UART_TL7816_TLEN_MASK)

/*! @name AP7816A_T0 - UART 7816 ATR Duration Timer Register A */
#define UART_AP7816A_T0_ADTI_H_MASK              (0xFFU)
#define UART_AP7816A_T0_ADTI_H_SHIFT             (0U)
#define UART_AP7816A_T0_ADTI_H(x)                (((uint8_t)(((uint8_t)(x)) << UART_AP7816A_T0_ADTI_H_SHIFT)) & UART_AP7816A_T0_ADTI_H_MASK)

/*! @name AP7816B_T0 - UART 7816 ATR Duration Timer Register B */
#define UART_AP7816B_T0_ADTI_L_MASK              (0xFFU)
#define UART_AP7816B_T0_ADTI_L_SHIFT             (0U)
#define UART_AP7816B_T0_ADTI_L(x)                (((uint8_t)(((uint8_t)(x)) << UART_AP7816B_T0_ADTI_L_SHIFT)) & UART_AP7816B_T0_ADTI_L_MASK)

/*! @name WP7816A_T0 - UART 7816 Wait Parameter Register A */
#define UART_WP7816A_T0_WI_H_MASK                (0xFFU)
#define UART_WP7816A_T0_WI_H_SHIFT               (0U)
#define UART_WP7816A_T0_WI_H(x)                  (((uint8_t)(((uint8_t)(x)) << UART_WP7816A_T0_WI_H_SHIFT)) & UART_WP7816A_T0_WI_H_MASK)

/*! @name WP7816B_T0 - UART 7816 Wait Parameter Register B */
#define UART_WP7816B_T0_WI_L_MASK                (0xFFU)
#define UART_WP7816B_T0_WI_L_SHIFT               (0U)
#define UART_WP7816B_T0_WI_L(x)                  (((uint8_t)(((uint8_t)(x)) << UART_WP7816B_T0_WI_L_SHIFT)) & UART_WP7816B_T0_WI_L_MASK)

/*! @name WP7816A_T1 - UART 7816 Wait Parameter Register A */
#define UART_WP7816A_T1_BWI_H_MASK               (0xFFU)
#define UART_WP7816A_T1_BWI_H_SHIFT              (0U)
#define UART_WP7816A_T1_BWI_H(x)                 (((uint8_t)(((uint8_t)(x)) << UART_WP7816A_T1_BWI_H_SHIFT)) & UART_WP7816A_T1_BWI_H_MASK)

/*! @name WP7816B_T1 - UART 7816 Wait Parameter Register B */
#define UART_WP7816B_T1_BWI_L_MASK               (0xFFU)
#define UART_WP7816B_T1_BWI_L_SHIFT              (0U)
#define UART_WP7816B_T1_BWI_L(x)                 (((uint8_t)(((uint8_t)(x)) << UART_WP7816B_T1_BWI_L_SHIFT)) & UART_WP7816B_T1_BWI_L_MASK)

/*! @name WGP7816_T1 - UART 7816 Wait and Guard Parameter Register */
#define UART_WGP7816_T1_BGI_MASK                 (0xFU)
#define UART_WGP7816_T1_BGI_SHIFT                (0U)
#define UART_WGP7816_T1_BGI(x)                   (((uint8_t)(((uint8_t)(x)) << UART_WGP7816_T1_BGI_SHIFT)) & UART_WGP7816_T1_BGI_MASK)
#define UART_WGP7816_T1_CWI1_MASK                (0xF0U)
#define UART_WGP7816_T1_CWI1_SHIFT               (4U)
#define UART_WGP7816_T1_CWI1(x)                  (((uint8_t)(((uint8_t)(x)) << UART_WGP7816_T1_CWI1_SHIFT)) & UART_WGP7816_T1_CWI1_MASK)

/*! @name WP7816C_T1 - UART 7816 Wait Parameter Register C */
#define UART_WP7816C_T1_CWI2_MASK                (0x1FU)
#define UART_WP7816C_T1_CWI2_SHIFT               (0U)
#define UART_WP7816C_T1_CWI2(x)                  (((uint8_t)(((uint8_t)(x)) << UART_WP7816C_T1_CWI2_SHIFT)) & UART_WP7816C_T1_CWI2_MASK)


/*!
 * @}
 */ /* end of group UART_Register_Masks */


/* UART - Peripheral instance base addresses */
/** Peripheral UART0 base address */
#define UART0_BASE                               (0x4006A000u)
/** Peripheral UART0 base pointer */
#define UART0                                    ((UART_Type *)UART0_BASE)
/** Peripheral UART1 base address */
#define UART1_BASE                               (0x4006B000u)
/** Peripheral UART1 base pointer */
#define UART1                                    ((UART_Type *)UART1_BASE)
/** Peripheral UART2 base address */
#define UART2_BASE                               (0x4006C000u)
/** Peripheral UART2 base pointer */
#define UART2                                    ((UART_Type *)UART2_BASE)
/** Peripheral UART3 base address */
#define UART3_BASE                               (0x4006D000u)
/** Peripheral UART3 base pointer */
#define UART3                                    ((UART_Type *)UART3_BASE)
/** Array initializer of UART peripheral base addresses */
#define UART_BASE_ADDRS                          { UART0_BASE, UART1_BASE, UART2_BASE, UART3_BASE }
/** Array initializer of UART peripheral base pointers */
#define UART_BASE_PTRS                           { UART0, UART1, UART2, UART3 }
/** Interrupt vectors for the UART peripheral type */
#define UART_RX_TX_IRQS                          { UART0_UART1_UART2_UART3_IRQn, UART0_UART1_UART2_UART3_IRQn, UART0_UART1_UART2_UART3_IRQn, UART0_UART1_UART2_UART3_IRQn }
#define UART_ERR_IRQS                            { UART0_UART1_UART2_UART3_IRQn, UART0_UART1_UART2_UART3_IRQn, UART0_UART1_UART2_UART3_IRQn, UART0_UART1_UART2_UART3_IRQn }

/*!
 * @}
 */ /* end of group UART_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- VREF Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup VREF_Peripheral_Access_Layer VREF Peripheral Access Layer
 * @{
 */

/** VREF - Register Layout Typedef */
typedef struct {
  __IO uint8_t VREFH_TRM;                          /**< VREF Trim Register, offset: 0x0 */
  __IO uint8_t VREFH_SC;                           /**< VREF Status and Control Register, offset: 0x1 */
       uint8_t RESERVED_0[3];
  __IO uint8_t VREFL_TRM;                          /**< VREFL TRIM Register, offset: 0x5 */
} VREF_Type;

/* ----------------------------------------------------------------------------
   -- VREF Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup VREF_Register_Masks VREF Register Masks
 * @{
 */

/*! @name VREFH_TRM - VREF Trim Register */
#define VREF_VREFH_TRM_TRIM_MASK                 (0x3FU)
#define VREF_VREFH_TRM_TRIM_SHIFT                (0U)
#define VREF_VREFH_TRM_TRIM(x)                   (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_TRM_TRIM_SHIFT)) & VREF_VREFH_TRM_TRIM_MASK)
#define VREF_VREFH_TRM_CHOPEN_MASK               (0x40U)
#define VREF_VREFH_TRM_CHOPEN_SHIFT              (6U)
#define VREF_VREFH_TRM_CHOPEN(x)                 (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_TRM_CHOPEN_SHIFT)) & VREF_VREFH_TRM_CHOPEN_MASK)

/*! @name VREFH_SC - VREF Status and Control Register */
#define VREF_VREFH_SC_MODE_LV_MASK               (0x3U)
#define VREF_VREFH_SC_MODE_LV_SHIFT              (0U)
#define VREF_VREFH_SC_MODE_LV(x)                 (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_SC_MODE_LV_SHIFT)) & VREF_VREFH_SC_MODE_LV_MASK)
#define VREF_VREFH_SC_VREFST_MASK                (0x4U)
#define VREF_VREFH_SC_VREFST_SHIFT               (2U)
#define VREF_VREFH_SC_VREFST(x)                  (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_SC_VREFST_SHIFT)) & VREF_VREFH_SC_VREFST_MASK)
#define VREF_VREFH_SC_ICOMPEN_MASK               (0x20U)
#define VREF_VREFH_SC_ICOMPEN_SHIFT              (5U)
#define VREF_VREFH_SC_ICOMPEN(x)                 (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_SC_ICOMPEN_SHIFT)) & VREF_VREFH_SC_ICOMPEN_MASK)
#define VREF_VREFH_SC_REGEN_MASK                 (0x40U)
#define VREF_VREFH_SC_REGEN_SHIFT                (6U)
#define VREF_VREFH_SC_REGEN(x)                   (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_SC_REGEN_SHIFT)) & VREF_VREFH_SC_REGEN_MASK)
#define VREF_VREFH_SC_VREFEN_MASK                (0x80U)
#define VREF_VREFH_SC_VREFEN_SHIFT               (7U)
#define VREF_VREFH_SC_VREFEN(x)                  (((uint8_t)(((uint8_t)(x)) << VREF_VREFH_SC_VREFEN_SHIFT)) & VREF_VREFH_SC_VREFEN_MASK)

/*! @name VREFL_TRM - VREFL TRIM Register */
#define VREF_VREFL_TRM_VREFL_TRIM_MASK           (0x7U)
#define VREF_VREFL_TRM_VREFL_TRIM_SHIFT          (0U)
#define VREF_VREFL_TRM_VREFL_TRIM(x)             (((uint8_t)(((uint8_t)(x)) << VREF_VREFL_TRM_VREFL_TRIM_SHIFT)) & VREF_VREFL_TRM_VREFL_TRIM_MASK)
#define VREF_VREFL_TRM_VREFL_EN_MASK             (0x8U)
#define VREF_VREFL_TRM_VREFL_EN_SHIFT            (3U)
#define VREF_VREFL_TRM_VREFL_EN(x)               (((uint8_t)(((uint8_t)(x)) << VREF_VREFL_TRM_VREFL_EN_SHIFT)) & VREF_VREFL_TRM_VREFL_EN_MASK)
#define VREF_VREFL_TRM_VREFL_SEL_MASK            (0x10U)
#define VREF_VREFL_TRM_VREFL_SEL_SHIFT           (4U)
#define VREF_VREFL_TRM_VREFL_SEL(x)              (((uint8_t)(((uint8_t)(x)) << VREF_VREFL_TRM_VREFL_SEL_SHIFT)) & VREF_VREFL_TRM_VREFL_SEL_MASK)


/*!
 * @}
 */ /* end of group VREF_Register_Masks */


/* VREF - Peripheral instance base addresses */
/** Peripheral VREF base address */
#define VREF_BASE                                (0x4006F000u)
/** Peripheral VREF base pointer */
#define VREF                                     ((VREF_Type *)VREF_BASE)
/** Array initializer of VREF peripheral base addresses */
#define VREF_BASE_ADDRS                          { VREF_BASE }
/** Array initializer of VREF peripheral base pointers */
#define VREF_BASE_PTRS                           { VREF }

/*!
 * @}
 */ /* end of group VREF_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- WDOG Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Peripheral_Access_Layer WDOG Peripheral Access Layer
 * @{
 */

/** WDOG - Register Layout Typedef */
typedef struct {
  __IO uint16_t STCTRLH;                           /**< Watchdog Status and Control Register High, offset: 0x0 */
  __IO uint16_t STCTRLL;                           /**< Watchdog Status and Control Register Low, offset: 0x2 */
  __IO uint16_t TOVALH;                            /**< Watchdog Time-out Value Register High, offset: 0x4 */
  __IO uint16_t TOVALL;                            /**< Watchdog Time-out Value Register Low, offset: 0x6 */
  __IO uint16_t WINH;                              /**< Watchdog Window Register High, offset: 0x8 */
  __IO uint16_t WINL;                              /**< Watchdog Window Register Low, offset: 0xA */
  __IO uint16_t REFRESH;                           /**< Watchdog Refresh register, offset: 0xC */
  __IO uint16_t UNLOCK;                            /**< Watchdog Unlock register, offset: 0xE */
  __IO uint16_t TMROUTH;                           /**< Watchdog Timer Output Register High, offset: 0x10 */
  __IO uint16_t TMROUTL;                           /**< Watchdog Timer Output Register Low, offset: 0x12 */
  __IO uint16_t RSTCNT;                            /**< Watchdog Reset Count register, offset: 0x14 */
  __IO uint16_t PRESC;                             /**< Watchdog Prescaler register, offset: 0x16 */
} WDOG_Type;

/* ----------------------------------------------------------------------------
   -- WDOG Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup WDOG_Register_Masks WDOG Register Masks
 * @{
 */

/*! @name STCTRLH - Watchdog Status and Control Register High */
#define WDOG_STCTRLH_WDOGEN_MASK                 (0x1U)
#define WDOG_STCTRLH_WDOGEN_SHIFT                (0U)
#define WDOG_STCTRLH_WDOGEN(x)                   (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_WDOGEN_SHIFT)) & WDOG_STCTRLH_WDOGEN_MASK)
#define WDOG_STCTRLH_CLKSRC_MASK                 (0x2U)
#define WDOG_STCTRLH_CLKSRC_SHIFT                (1U)
#define WDOG_STCTRLH_CLKSRC(x)                   (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_CLKSRC_SHIFT)) & WDOG_STCTRLH_CLKSRC_MASK)
#define WDOG_STCTRLH_IRQRSTEN_MASK               (0x4U)
#define WDOG_STCTRLH_IRQRSTEN_SHIFT              (2U)
#define WDOG_STCTRLH_IRQRSTEN(x)                 (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_IRQRSTEN_SHIFT)) & WDOG_STCTRLH_IRQRSTEN_MASK)
#define WDOG_STCTRLH_WINEN_MASK                  (0x8U)
#define WDOG_STCTRLH_WINEN_SHIFT                 (3U)
#define WDOG_STCTRLH_WINEN(x)                    (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_WINEN_SHIFT)) & WDOG_STCTRLH_WINEN_MASK)
#define WDOG_STCTRLH_ALLOWUPDATE_MASK            (0x10U)
#define WDOG_STCTRLH_ALLOWUPDATE_SHIFT           (4U)
#define WDOG_STCTRLH_ALLOWUPDATE(x)              (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_ALLOWUPDATE_SHIFT)) & WDOG_STCTRLH_ALLOWUPDATE_MASK)
#define WDOG_STCTRLH_DBGEN_MASK                  (0x20U)
#define WDOG_STCTRLH_DBGEN_SHIFT                 (5U)
#define WDOG_STCTRLH_DBGEN(x)                    (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_DBGEN_SHIFT)) & WDOG_STCTRLH_DBGEN_MASK)
#define WDOG_STCTRLH_STOPEN_MASK                 (0x40U)
#define WDOG_STCTRLH_STOPEN_SHIFT                (6U)
#define WDOG_STCTRLH_STOPEN(x)                   (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_STOPEN_SHIFT)) & WDOG_STCTRLH_STOPEN_MASK)
#define WDOG_STCTRLH_TESTWDOG_MASK               (0x400U)
#define WDOG_STCTRLH_TESTWDOG_SHIFT              (10U)
#define WDOG_STCTRLH_TESTWDOG(x)                 (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_TESTWDOG_SHIFT)) & WDOG_STCTRLH_TESTWDOG_MASK)
#define WDOG_STCTRLH_TESTSEL_MASK                (0x800U)
#define WDOG_STCTRLH_TESTSEL_SHIFT               (11U)
#define WDOG_STCTRLH_TESTSEL(x)                  (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_TESTSEL_SHIFT)) & WDOG_STCTRLH_TESTSEL_MASK)
#define WDOG_STCTRLH_BYTESEL_MASK                (0x3000U)
#define WDOG_STCTRLH_BYTESEL_SHIFT               (12U)
#define WDOG_STCTRLH_BYTESEL(x)                  (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_BYTESEL_SHIFT)) & WDOG_STCTRLH_BYTESEL_MASK)
#define WDOG_STCTRLH_DISTESTWDOG_MASK            (0x4000U)
#define WDOG_STCTRLH_DISTESTWDOG_SHIFT           (14U)
#define WDOG_STCTRLH_DISTESTWDOG(x)              (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLH_DISTESTWDOG_SHIFT)) & WDOG_STCTRLH_DISTESTWDOG_MASK)

/*! @name STCTRLL - Watchdog Status and Control Register Low */
#define WDOG_STCTRLL_INTFLG_MASK                 (0x8000U)
#define WDOG_STCTRLL_INTFLG_SHIFT                (15U)
#define WDOG_STCTRLL_INTFLG(x)                   (((uint16_t)(((uint16_t)(x)) << WDOG_STCTRLL_INTFLG_SHIFT)) & WDOG_STCTRLL_INTFLG_MASK)

/*! @name TOVALH - Watchdog Time-out Value Register High */
#define WDOG_TOVALH_TOVALHIGH_MASK               (0xFFFFU)
#define WDOG_TOVALH_TOVALHIGH_SHIFT              (0U)
#define WDOG_TOVALH_TOVALHIGH(x)                 (((uint16_t)(((uint16_t)(x)) << WDOG_TOVALH_TOVALHIGH_SHIFT)) & WDOG_TOVALH_TOVALHIGH_MASK)

/*! @name TOVALL - Watchdog Time-out Value Register Low */
#define WDOG_TOVALL_TOVALLOW_MASK                (0xFFFFU)
#define WDOG_TOVALL_TOVALLOW_SHIFT               (0U)
#define WDOG_TOVALL_TOVALLOW(x)                  (((uint16_t)(((uint16_t)(x)) << WDOG_TOVALL_TOVALLOW_SHIFT)) & WDOG_TOVALL_TOVALLOW_MASK)

/*! @name WINH - Watchdog Window Register High */
#define WDOG_WINH_WINHIGH_MASK                   (0xFFFFU)
#define WDOG_WINH_WINHIGH_SHIFT                  (0U)
#define WDOG_WINH_WINHIGH(x)                     (((uint16_t)(((uint16_t)(x)) << WDOG_WINH_WINHIGH_SHIFT)) & WDOG_WINH_WINHIGH_MASK)

/*! @name WINL - Watchdog Window Register Low */
#define WDOG_WINL_WINLOW_MASK                    (0xFFFFU)
#define WDOG_WINL_WINLOW_SHIFT                   (0U)
#define WDOG_WINL_WINLOW(x)                      (((uint16_t)(((uint16_t)(x)) << WDOG_WINL_WINLOW_SHIFT)) & WDOG_WINL_WINLOW_MASK)

/*! @name REFRESH - Watchdog Refresh register */
#define WDOG_REFRESH_WDOGREFRESH_MASK            (0xFFFFU)
#define WDOG_REFRESH_WDOGREFRESH_SHIFT           (0U)
#define WDOG_REFRESH_WDOGREFRESH(x)              (((uint16_t)(((uint16_t)(x)) << WDOG_REFRESH_WDOGREFRESH_SHIFT)) & WDOG_REFRESH_WDOGREFRESH_MASK)

/*! @name UNLOCK - Watchdog Unlock register */
#define WDOG_UNLOCK_WDOGUNLOCK_MASK              (0xFFFFU)
#define WDOG_UNLOCK_WDOGUNLOCK_SHIFT             (0U)
#define WDOG_UNLOCK_WDOGUNLOCK(x)                (((uint16_t)(((uint16_t)(x)) << WDOG_UNLOCK_WDOGUNLOCK_SHIFT)) & WDOG_UNLOCK_WDOGUNLOCK_MASK)

/*! @name TMROUTH - Watchdog Timer Output Register High */
#define WDOG_TMROUTH_TIMEROUTHIGH_MASK           (0xFFFFU)
#define WDOG_TMROUTH_TIMEROUTHIGH_SHIFT          (0U)
#define WDOG_TMROUTH_TIMEROUTHIGH(x)             (((uint16_t)(((uint16_t)(x)) << WDOG_TMROUTH_TIMEROUTHIGH_SHIFT)) & WDOG_TMROUTH_TIMEROUTHIGH_MASK)

/*! @name TMROUTL - Watchdog Timer Output Register Low */
#define WDOG_TMROUTL_TIMEROUTLOW_MASK            (0xFFFFU)
#define WDOG_TMROUTL_TIMEROUTLOW_SHIFT           (0U)
#define WDOG_TMROUTL_TIMEROUTLOW(x)              (((uint16_t)(((uint16_t)(x)) << WDOG_TMROUTL_TIMEROUTLOW_SHIFT)) & WDOG_TMROUTL_TIMEROUTLOW_MASK)

/*! @name RSTCNT - Watchdog Reset Count register */
#define WDOG_RSTCNT_RSTCNT_MASK                  (0xFFFFU)
#define WDOG_RSTCNT_RSTCNT_SHIFT                 (0U)
#define WDOG_RSTCNT_RSTCNT(x)                    (((uint16_t)(((uint16_t)(x)) << WDOG_RSTCNT_RSTCNT_SHIFT)) & WDOG_RSTCNT_RSTCNT_MASK)

/*! @name PRESC - Watchdog Prescaler register */
#define WDOG_PRESC_PRESCVAL_MASK                 (0x700U)
#define WDOG_PRESC_PRESCVAL_SHIFT                (8U)
#define WDOG_PRESC_PRESCVAL(x)                   (((uint16_t)(((uint16_t)(x)) << WDOG_PRESC_PRESCVAL_SHIFT)) & WDOG_PRESC_PRESCVAL_MASK)


/*!
 * @}
 */ /* end of group WDOG_Register_Masks */


/* WDOG - Peripheral instance base addresses */
/** Peripheral WDOG base address */
#define WDOG_BASE                                (0x40053000u)
/** Peripheral WDOG base pointer */
#define WDOG                                     ((WDOG_Type *)WDOG_BASE)
/** Array initializer of WDOG peripheral base addresses */
#define WDOG_BASE_ADDRS                          { WDOG_BASE }
/** Array initializer of WDOG peripheral base pointers */
#define WDOG_BASE_PTRS                           { WDOG }
/** Interrupt vectors for the WDOG peripheral type */
#define WDOG_IRQS                                { WDOG_EWM_IRQn }

/*!
 * @}
 */ /* end of group WDOG_Peripheral_Access_Layer */


/* ----------------------------------------------------------------------------
   -- XBAR Peripheral Access Layer
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup XBAR_Peripheral_Access_Layer XBAR Peripheral Access Layer
 * @{
 */

/** XBAR - Register Layout Typedef */
typedef struct {
  __IO uint16_t SEL0;                              /**< Crossbar Select Register 0, offset: 0x0 */
  __IO uint16_t SEL1;                              /**< Crossbar Select Register 1, offset: 0x2 */
  __IO uint16_t SEL2;                              /**< Crossbar Select Register 2, offset: 0x4 */
  __IO uint16_t SEL3;                              /**< Crossbar Select Register 3, offset: 0x6 */
  __IO uint16_t SEL4;                              /**< Crossbar Select Register 4, offset: 0x8 */
  __IO uint16_t SEL5;                              /**< Crossbar Select Register 5, offset: 0xA */
  __IO uint16_t SEL6;                              /**< Crossbar Select Register 6, offset: 0xC */
  __IO uint16_t SEL7;                              /**< Crossbar Select Register 7, offset: 0xE */
  __IO uint16_t SEL8;                              /**< Crossbar Select Register 8, offset: 0x10 */
  __IO uint16_t SEL9;                              /**< Crossbar Select Register 9, offset: 0x12 */
  __IO uint16_t SEL10;                             /**< Crossbar Select Register 10, offset: 0x14 */
  __IO uint16_t SEL11;                             /**< Crossbar Select Register 11, offset: 0x16 */
  __IO uint16_t SEL12;                             /**< Crossbar Select Register 12, offset: 0x18 */
  __IO uint16_t SEL13;                             /**< Crossbar Select Register 13, offset: 0x1A */
  __IO uint16_t SEL14;                             /**< Crossbar Select Register 14, offset: 0x1C */
  __IO uint16_t SEL15;                             /**< Crossbar Select Register 15, offset: 0x1E */
  __IO uint16_t SEL16;                             /**< Crossbar Select Register 16, offset: 0x20 */
  __IO uint16_t SEL17;                             /**< Crossbar Select Register 17, offset: 0x22 */
  __IO uint16_t SEL18;                             /**< Crossbar Select Register 18, offset: 0x24 */
  __IO uint16_t SEL19;                             /**< Crossbar Select Register 19, offset: 0x26 */
  __IO uint16_t SEL20;                             /**< Crossbar Select Register 20, offset: 0x28 */
  __IO uint16_t SEL21;                             /**< Crossbar Select Register 21, offset: 0x2A */
  __IO uint16_t CTRL0;                             /**< Crossbar Control Register 0, offset: 0x2C */
  __IO uint16_t CTRL1;                             /**< Crossbar Control Register 1, offset: 0x2E */
} XBAR_Type;

/* ----------------------------------------------------------------------------
   -- XBAR Register Masks
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup XBAR_Register_Masks XBAR Register Masks
 * @{
 */

/*! @name SEL0 - Crossbar Select Register 0 */
#define XBAR_SEL0_SEL0_MASK                      (0x3FU)
#define XBAR_SEL0_SEL0_SHIFT                     (0U)
#define XBAR_SEL0_SEL0(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL0_SEL0_SHIFT)) & XBAR_SEL0_SEL0_MASK)
#define XBAR_SEL0_SEL1_MASK                      (0x3F00U)
#define XBAR_SEL0_SEL1_SHIFT                     (8U)
#define XBAR_SEL0_SEL1(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL0_SEL1_SHIFT)) & XBAR_SEL0_SEL1_MASK)

/*! @name SEL1 - Crossbar Select Register 1 */
#define XBAR_SEL1_SEL2_MASK                      (0x3FU)
#define XBAR_SEL1_SEL2_SHIFT                     (0U)
#define XBAR_SEL1_SEL2(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL1_SEL2_SHIFT)) & XBAR_SEL1_SEL2_MASK)
#define XBAR_SEL1_SEL3_MASK                      (0x3F00U)
#define XBAR_SEL1_SEL3_SHIFT                     (8U)
#define XBAR_SEL1_SEL3(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL1_SEL3_SHIFT)) & XBAR_SEL1_SEL3_MASK)

/*! @name SEL2 - Crossbar Select Register 2 */
#define XBAR_SEL2_SEL4_MASK                      (0x3FU)
#define XBAR_SEL2_SEL4_SHIFT                     (0U)
#define XBAR_SEL2_SEL4(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL2_SEL4_SHIFT)) & XBAR_SEL2_SEL4_MASK)
#define XBAR_SEL2_SEL5_MASK                      (0x3F00U)
#define XBAR_SEL2_SEL5_SHIFT                     (8U)
#define XBAR_SEL2_SEL5(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL2_SEL5_SHIFT)) & XBAR_SEL2_SEL5_MASK)

/*! @name SEL3 - Crossbar Select Register 3 */
#define XBAR_SEL3_SEL6_MASK                      (0x3FU)
#define XBAR_SEL3_SEL6_SHIFT                     (0U)
#define XBAR_SEL3_SEL6(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL3_SEL6_SHIFT)) & XBAR_SEL3_SEL6_MASK)
#define XBAR_SEL3_SEL7_MASK                      (0x3F00U)
#define XBAR_SEL3_SEL7_SHIFT                     (8U)
#define XBAR_SEL3_SEL7(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL3_SEL7_SHIFT)) & XBAR_SEL3_SEL7_MASK)

/*! @name SEL4 - Crossbar Select Register 4 */
#define XBAR_SEL4_SEL8_MASK                      (0x3FU)
#define XBAR_SEL4_SEL8_SHIFT                     (0U)
#define XBAR_SEL4_SEL8(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL4_SEL8_SHIFT)) & XBAR_SEL4_SEL8_MASK)
#define XBAR_SEL4_SEL9_MASK                      (0x3F00U)
#define XBAR_SEL4_SEL9_SHIFT                     (8U)
#define XBAR_SEL4_SEL9(x)                        (((uint16_t)(((uint16_t)(x)) << XBAR_SEL4_SEL9_SHIFT)) & XBAR_SEL4_SEL9_MASK)

/*! @name SEL5 - Crossbar Select Register 5 */
#define XBAR_SEL5_SEL10_MASK                     (0x3FU)
#define XBAR_SEL5_SEL10_SHIFT                    (0U)
#define XBAR_SEL5_SEL10(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL5_SEL10_SHIFT)) & XBAR_SEL5_SEL10_MASK)
#define XBAR_SEL5_SEL11_MASK                     (0x3F00U)
#define XBAR_SEL5_SEL11_SHIFT                    (8U)
#define XBAR_SEL5_SEL11(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL5_SEL11_SHIFT)) & XBAR_SEL5_SEL11_MASK)

/*! @name SEL6 - Crossbar Select Register 6 */
#define XBAR_SEL6_SEL12_MASK                     (0x3FU)
#define XBAR_SEL6_SEL12_SHIFT                    (0U)
#define XBAR_SEL6_SEL12(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL6_SEL12_SHIFT)) & XBAR_SEL6_SEL12_MASK)
#define XBAR_SEL6_SEL13_MASK                     (0x3F00U)
#define XBAR_SEL6_SEL13_SHIFT                    (8U)
#define XBAR_SEL6_SEL13(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL6_SEL13_SHIFT)) & XBAR_SEL6_SEL13_MASK)

/*! @name SEL7 - Crossbar Select Register 7 */
#define XBAR_SEL7_SEL14_MASK                     (0x3FU)
#define XBAR_SEL7_SEL14_SHIFT                    (0U)
#define XBAR_SEL7_SEL14(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL7_SEL14_SHIFT)) & XBAR_SEL7_SEL14_MASK)
#define XBAR_SEL7_SEL15_MASK                     (0x3F00U)
#define XBAR_SEL7_SEL15_SHIFT                    (8U)
#define XBAR_SEL7_SEL15(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL7_SEL15_SHIFT)) & XBAR_SEL7_SEL15_MASK)

/*! @name SEL8 - Crossbar Select Register 8 */
#define XBAR_SEL8_SEL16_MASK                     (0x3FU)
#define XBAR_SEL8_SEL16_SHIFT                    (0U)
#define XBAR_SEL8_SEL16(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL8_SEL16_SHIFT)) & XBAR_SEL8_SEL16_MASK)
#define XBAR_SEL8_SEL17_MASK                     (0x3F00U)
#define XBAR_SEL8_SEL17_SHIFT                    (8U)
#define XBAR_SEL8_SEL17(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL8_SEL17_SHIFT)) & XBAR_SEL8_SEL17_MASK)

/*! @name SEL9 - Crossbar Select Register 9 */
#define XBAR_SEL9_SEL18_MASK                     (0x3FU)
#define XBAR_SEL9_SEL18_SHIFT                    (0U)
#define XBAR_SEL9_SEL18(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL9_SEL18_SHIFT)) & XBAR_SEL9_SEL18_MASK)
#define XBAR_SEL9_SEL19_MASK                     (0x3F00U)
#define XBAR_SEL9_SEL19_SHIFT                    (8U)
#define XBAR_SEL9_SEL19(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_SEL9_SEL19_SHIFT)) & XBAR_SEL9_SEL19_MASK)

/*! @name SEL10 - Crossbar Select Register 10 */
#define XBAR_SEL10_SEL20_MASK                    (0x3FU)
#define XBAR_SEL10_SEL20_SHIFT                   (0U)
#define XBAR_SEL10_SEL20(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL10_SEL20_SHIFT)) & XBAR_SEL10_SEL20_MASK)
#define XBAR_SEL10_SEL21_MASK                    (0x3F00U)
#define XBAR_SEL10_SEL21_SHIFT                   (8U)
#define XBAR_SEL10_SEL21(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL10_SEL21_SHIFT)) & XBAR_SEL10_SEL21_MASK)

/*! @name SEL11 - Crossbar Select Register 11 */
#define XBAR_SEL11_SEL22_MASK                    (0x3FU)
#define XBAR_SEL11_SEL22_SHIFT                   (0U)
#define XBAR_SEL11_SEL22(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL11_SEL22_SHIFT)) & XBAR_SEL11_SEL22_MASK)
#define XBAR_SEL11_SEL23_MASK                    (0x3F00U)
#define XBAR_SEL11_SEL23_SHIFT                   (8U)
#define XBAR_SEL11_SEL23(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL11_SEL23_SHIFT)) & XBAR_SEL11_SEL23_MASK)

/*! @name SEL12 - Crossbar Select Register 12 */
#define XBAR_SEL12_SEL24_MASK                    (0x3FU)
#define XBAR_SEL12_SEL24_SHIFT                   (0U)
#define XBAR_SEL12_SEL24(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL12_SEL24_SHIFT)) & XBAR_SEL12_SEL24_MASK)
#define XBAR_SEL12_SEL25_MASK                    (0x3F00U)
#define XBAR_SEL12_SEL25_SHIFT                   (8U)
#define XBAR_SEL12_SEL25(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL12_SEL25_SHIFT)) & XBAR_SEL12_SEL25_MASK)

/*! @name SEL13 - Crossbar Select Register 13 */
#define XBAR_SEL13_SEL26_MASK                    (0x3FU)
#define XBAR_SEL13_SEL26_SHIFT                   (0U)
#define XBAR_SEL13_SEL26(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL13_SEL26_SHIFT)) & XBAR_SEL13_SEL26_MASK)
#define XBAR_SEL13_SEL27_MASK                    (0x3F00U)
#define XBAR_SEL13_SEL27_SHIFT                   (8U)
#define XBAR_SEL13_SEL27(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL13_SEL27_SHIFT)) & XBAR_SEL13_SEL27_MASK)

/*! @name SEL14 - Crossbar Select Register 14 */
#define XBAR_SEL14_SEL28_MASK                    (0x3FU)
#define XBAR_SEL14_SEL28_SHIFT                   (0U)
#define XBAR_SEL14_SEL28(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL14_SEL28_SHIFT)) & XBAR_SEL14_SEL28_MASK)
#define XBAR_SEL14_SEL29_MASK                    (0x3F00U)
#define XBAR_SEL14_SEL29_SHIFT                   (8U)
#define XBAR_SEL14_SEL29(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL14_SEL29_SHIFT)) & XBAR_SEL14_SEL29_MASK)

/*! @name SEL15 - Crossbar Select Register 15 */
#define XBAR_SEL15_SEL30_MASK                    (0x3FU)
#define XBAR_SEL15_SEL30_SHIFT                   (0U)
#define XBAR_SEL15_SEL30(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL15_SEL30_SHIFT)) & XBAR_SEL15_SEL30_MASK)
#define XBAR_SEL15_SEL31_MASK                    (0x3F00U)
#define XBAR_SEL15_SEL31_SHIFT                   (8U)
#define XBAR_SEL15_SEL31(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL15_SEL31_SHIFT)) & XBAR_SEL15_SEL31_MASK)

/*! @name SEL16 - Crossbar Select Register 16 */
#define XBAR_SEL16_SEL32_MASK                    (0x3FU)
#define XBAR_SEL16_SEL32_SHIFT                   (0U)
#define XBAR_SEL16_SEL32(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL16_SEL32_SHIFT)) & XBAR_SEL16_SEL32_MASK)
#define XBAR_SEL16_SEL33_MASK                    (0x3F00U)
#define XBAR_SEL16_SEL33_SHIFT                   (8U)
#define XBAR_SEL16_SEL33(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL16_SEL33_SHIFT)) & XBAR_SEL16_SEL33_MASK)

/*! @name SEL17 - Crossbar Select Register 17 */
#define XBAR_SEL17_SEL34_MASK                    (0x3FU)
#define XBAR_SEL17_SEL34_SHIFT                   (0U)
#define XBAR_SEL17_SEL34(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL17_SEL34_SHIFT)) & XBAR_SEL17_SEL34_MASK)
#define XBAR_SEL17_SEL35_MASK                    (0x3F00U)
#define XBAR_SEL17_SEL35_SHIFT                   (8U)
#define XBAR_SEL17_SEL35(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL17_SEL35_SHIFT)) & XBAR_SEL17_SEL35_MASK)

/*! @name SEL18 - Crossbar Select Register 18 */
#define XBAR_SEL18_SEL36_MASK                    (0x3FU)
#define XBAR_SEL18_SEL36_SHIFT                   (0U)
#define XBAR_SEL18_SEL36(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL18_SEL36_SHIFT)) & XBAR_SEL18_SEL36_MASK)
#define XBAR_SEL18_SEL37_MASK                    (0x3F00U)
#define XBAR_SEL18_SEL37_SHIFT                   (8U)
#define XBAR_SEL18_SEL37(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL18_SEL37_SHIFT)) & XBAR_SEL18_SEL37_MASK)

/*! @name SEL19 - Crossbar Select Register 19 */
#define XBAR_SEL19_SEL38_MASK                    (0x3FU)
#define XBAR_SEL19_SEL38_SHIFT                   (0U)
#define XBAR_SEL19_SEL38(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL19_SEL38_SHIFT)) & XBAR_SEL19_SEL38_MASK)
#define XBAR_SEL19_SEL39_MASK                    (0x3F00U)
#define XBAR_SEL19_SEL39_SHIFT                   (8U)
#define XBAR_SEL19_SEL39(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL19_SEL39_SHIFT)) & XBAR_SEL19_SEL39_MASK)

/*! @name SEL20 - Crossbar Select Register 20 */
#define XBAR_SEL20_SEL40_MASK                    (0x3FU)
#define XBAR_SEL20_SEL40_SHIFT                   (0U)
#define XBAR_SEL20_SEL40(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL20_SEL40_SHIFT)) & XBAR_SEL20_SEL40_MASK)
#define XBAR_SEL20_SEL41_MASK                    (0x3F00U)
#define XBAR_SEL20_SEL41_SHIFT                   (8U)
#define XBAR_SEL20_SEL41(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL20_SEL41_SHIFT)) & XBAR_SEL20_SEL41_MASK)

/*! @name SEL21 - Crossbar Select Register 21 */
#define XBAR_SEL21_SEL42_MASK                    (0x3FU)
#define XBAR_SEL21_SEL42_SHIFT                   (0U)
#define XBAR_SEL21_SEL42(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL21_SEL42_SHIFT)) & XBAR_SEL21_SEL42_MASK)
#define XBAR_SEL21_SEL43_MASK                    (0x3F00U)
#define XBAR_SEL21_SEL43_SHIFT                   (8U)
#define XBAR_SEL21_SEL43(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_SEL21_SEL43_SHIFT)) & XBAR_SEL21_SEL43_MASK)

/*! @name CTRL0 - Crossbar Control Register 0 */
#define XBAR_CTRL0_DEN0_MASK                     (0x1U)
#define XBAR_CTRL0_DEN0_SHIFT                    (0U)
#define XBAR_CTRL0_DEN0(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_DEN0_SHIFT)) & XBAR_CTRL0_DEN0_MASK)
#define XBAR_CTRL0_IEN0_MASK                     (0x2U)
#define XBAR_CTRL0_IEN0_SHIFT                    (1U)
#define XBAR_CTRL0_IEN0(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_IEN0_SHIFT)) & XBAR_CTRL0_IEN0_MASK)
#define XBAR_CTRL0_EDGE0_MASK                    (0xCU)
#define XBAR_CTRL0_EDGE0_SHIFT                   (2U)
#define XBAR_CTRL0_EDGE0(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_EDGE0_SHIFT)) & XBAR_CTRL0_EDGE0_MASK)
#define XBAR_CTRL0_STS0_MASK                     (0x10U)
#define XBAR_CTRL0_STS0_SHIFT                    (4U)
#define XBAR_CTRL0_STS0(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_STS0_SHIFT)) & XBAR_CTRL0_STS0_MASK)
#define XBAR_CTRL0_DEN1_MASK                     (0x100U)
#define XBAR_CTRL0_DEN1_SHIFT                    (8U)
#define XBAR_CTRL0_DEN1(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_DEN1_SHIFT)) & XBAR_CTRL0_DEN1_MASK)
#define XBAR_CTRL0_IEN1_MASK                     (0x200U)
#define XBAR_CTRL0_IEN1_SHIFT                    (9U)
#define XBAR_CTRL0_IEN1(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_IEN1_SHIFT)) & XBAR_CTRL0_IEN1_MASK)
#define XBAR_CTRL0_EDGE1_MASK                    (0xC00U)
#define XBAR_CTRL0_EDGE1_SHIFT                   (10U)
#define XBAR_CTRL0_EDGE1(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_EDGE1_SHIFT)) & XBAR_CTRL0_EDGE1_MASK)
#define XBAR_CTRL0_STS1_MASK                     (0x1000U)
#define XBAR_CTRL0_STS1_SHIFT                    (12U)
#define XBAR_CTRL0_STS1(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL0_STS1_SHIFT)) & XBAR_CTRL0_STS1_MASK)

/*! @name CTRL1 - Crossbar Control Register 1 */
#define XBAR_CTRL1_DEN2_MASK                     (0x1U)
#define XBAR_CTRL1_DEN2_SHIFT                    (0U)
#define XBAR_CTRL1_DEN2(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_DEN2_SHIFT)) & XBAR_CTRL1_DEN2_MASK)
#define XBAR_CTRL1_IEN2_MASK                     (0x2U)
#define XBAR_CTRL1_IEN2_SHIFT                    (1U)
#define XBAR_CTRL1_IEN2(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_IEN2_SHIFT)) & XBAR_CTRL1_IEN2_MASK)
#define XBAR_CTRL1_EDGE2_MASK                    (0xCU)
#define XBAR_CTRL1_EDGE2_SHIFT                   (2U)
#define XBAR_CTRL1_EDGE2(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_EDGE2_SHIFT)) & XBAR_CTRL1_EDGE2_MASK)
#define XBAR_CTRL1_STS2_MASK                     (0x10U)
#define XBAR_CTRL1_STS2_SHIFT                    (4U)
#define XBAR_CTRL1_STS2(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_STS2_SHIFT)) & XBAR_CTRL1_STS2_MASK)
#define XBAR_CTRL1_DEN3_MASK                     (0x100U)
#define XBAR_CTRL1_DEN3_SHIFT                    (8U)
#define XBAR_CTRL1_DEN3(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_DEN3_SHIFT)) & XBAR_CTRL1_DEN3_MASK)
#define XBAR_CTRL1_IEN3_MASK                     (0x200U)
#define XBAR_CTRL1_IEN3_SHIFT                    (9U)
#define XBAR_CTRL1_IEN3(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_IEN3_SHIFT)) & XBAR_CTRL1_IEN3_MASK)
#define XBAR_CTRL1_EDGE3_MASK                    (0xC00U)
#define XBAR_CTRL1_EDGE3_SHIFT                   (10U)
#define XBAR_CTRL1_EDGE3(x)                      (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_EDGE3_SHIFT)) & XBAR_CTRL1_EDGE3_MASK)
#define XBAR_CTRL1_STS3_MASK                     (0x1000U)
#define XBAR_CTRL1_STS3_SHIFT                    (12U)
#define XBAR_CTRL1_STS3(x)                       (((uint16_t)(((uint16_t)(x)) << XBAR_CTRL1_STS3_SHIFT)) & XBAR_CTRL1_STS3_MASK)


/*!
 * @}
 */ /* end of group XBAR_Register_Masks */


/* XBAR - Peripheral instance base addresses */
/** Peripheral XBAR base address */
#define XBAR_BASE                                (0x40055000u)
/** Peripheral XBAR base pointer */
#define XBAR                                     ((XBAR_Type *)XBAR_BASE)
/** Array initializer of XBAR peripheral base addresses */
#define XBAR_BASE_ADDRS                          { XBAR_BASE }
/** Array initializer of XBAR peripheral base pointers */
#define XBAR_BASE_PTRS                           { XBAR }
/** Interrupt vectors for the XBAR peripheral type */
#define XBAR_IRQS                                { XBAR_IRQn }

/*!
 * @}
 */ /* end of group XBAR_Peripheral_Access_Layer */


/*
** End of section using anonymous unions
*/

#if defined(__ARMCC_VERSION)
  #pragma pop
#elif defined(__CWCC__)
  #pragma pop
#elif defined(__GNUC__)
  /* leave anonymous unions enabled */
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma language=default
#else
  #error Not supported compiler type
#endif

/*!
 * @}
 */ /* end of group Peripheral_access_layer */


/* ----------------------------------------------------------------------------
   -- Macros for use with bit field definitions (xxx_SHIFT, xxx_MASK).
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup Bit_Field_Generic_Macros Macros for use with bit field definitions (xxx_SHIFT, xxx_MASK).
 * @{
 */

#if defined(__ARMCC_VERSION)
  #if (__ARMCC_VERSION >= 6010050)
    #pragma clang system_header
  #endif
#elif defined(__IAR_SYSTEMS_ICC__)
  #pragma system_include
#endif

/**
 * @brief Mask and left-shift a bit field value for use in a register bit range.
 * @param field Name of the register bit field.
 * @param value Value of the bit field.
 * @return Masked and shifted value.
 */
#define NXP_VAL2FLD(field, value)    (((value) << (field ## _SHIFT)) & (field ## _MASK))
/**
 * @brief Mask and right-shift a register value to extract a bit field value.
 * @param field Name of the register bit field.
 * @param value Value of the register.
 * @return Masked and shifted bit field value.
 */
#define NXP_FLD2VAL(field, value)    (((value) & (field ## _MASK)) >> (field ## _SHIFT))

/*!
 * @}
 */ /* end of group Bit_Field_Generic_Macros */


/* ----------------------------------------------------------------------------
   -- SDK Compatibility
   ---------------------------------------------------------------------------- */

/*!
 * @addtogroup SDK_Compatibility_Symbols SDK Compatibility
 * @{
 */

/* No SDK compatibility issues. */

/*!
 * @}
 */ /* end of group SDK_Compatibility_Symbols */


#endif  /* _MKM34Z7_H_ */

