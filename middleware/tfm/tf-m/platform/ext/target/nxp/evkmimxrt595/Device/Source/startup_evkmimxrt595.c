/*
 * Copyright (c) 2022 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * This file is derivative of CMSIS V5.9.0 startup_ARMCM33.c
 * Git SHA: 2b7495b8535bdcb306dac29b9ded4cfb679d7e5c
 */

/* NS linker scripts using the default CMSIS style naming conventions, while the
 * secure and bl2 linker scripts remain untouched (region.h compatibility).
 * To be compatible with the untouched files (which using ARMCLANG naming style),
 * we have to override __INITIAL_SP and __STACK_LIMIT labels. */
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U) 
#include "cmsis_override.h"
#endif

#include "cmsis.h"

/*----------------------------------------------------------------------------
  External References
 *----------------------------------------------------------------------------*/
extern uint32_t __INITIAL_SP;
extern uint32_t __STACK_LIMIT;
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
extern uint64_t __STACK_SEAL;
#endif

typedef void(*VECTOR_TABLE_Type)(void);

extern void __PROGRAM_START(void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Internal References
 *----------------------------------------------------------------------------*/
void Reset_Handler  (void) __NO_RETURN;

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
#define DEFAULT_IRQ_HANDLER(handler_name)  \
void __WEAK handler_name(void) __NO_RETURN; \
void handler_name(void) { \
    while(1); \
}

/* Exceptions */
DEFAULT_IRQ_HANDLER(NMI_Handler)
DEFAULT_IRQ_HANDLER(HardFault_Handler)
DEFAULT_IRQ_HANDLER(MemManage_Handler)
DEFAULT_IRQ_HANDLER(BusFault_Handler)
DEFAULT_IRQ_HANDLER(UsageFault_Handler)
DEFAULT_IRQ_HANDLER(SecureFault_Handler)
DEFAULT_IRQ_HANDLER(SVC_Handler)
DEFAULT_IRQ_HANDLER(DebugMon_Handler)
DEFAULT_IRQ_HANDLER(PendSV_Handler)
DEFAULT_IRQ_HANDLER(SysTick_Handler)

DEFAULT_IRQ_HANDLER(WDT0_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA0_IRQHandler)
DEFAULT_IRQ_HANDLER(GPIO_INTA_IRQHandler)
DEFAULT_IRQ_HANDLER(GPIO_INTB_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT0_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT1_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT2_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT3_IRQHandler)
DEFAULT_IRQ_HANDLER(UTICK0_IRQHandler)
DEFAULT_IRQ_HANDLER(MRT0_IRQHandler)
DEFAULT_IRQ_HANDLER(CTIMER0_IRQHandler)
DEFAULT_IRQ_HANDLER(CTIMER1_IRQHandler)
DEFAULT_IRQ_HANDLER(SCT0_IRQHandler)
DEFAULT_IRQ_HANDLER(CTIMER3_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM0_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM1_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM2_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM3_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM4_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM5_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM14_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM15_IRQHandler)
DEFAULT_IRQ_HANDLER(ADC0_IRQHandler)
DEFAULT_IRQ_HANDLER(Reserved39_IRQHandler)
DEFAULT_IRQ_HANDLER(ACMP_IRQHandler)
DEFAULT_IRQ_HANDLER(DMIC0_IRQHandler)
DEFAULT_IRQ_HANDLER(Reserved42_IRQHandler)
DEFAULT_IRQ_HANDLER(HYPERVISOR_IRQHandler)
DEFAULT_IRQ_HANDLER(SECURE_VIOLATION_IRQHandler)
DEFAULT_IRQ_HANDLER(HWVAD0_IRQHandler)
DEFAULT_IRQ_HANDLER(Reserved46_IRQHandler)
DEFAULT_IRQ_HANDLER(RNG_IRQHandler)
DEFAULT_IRQ_HANDLER(RTC_IRQHandler)
DEFAULT_IRQ_HANDLER(DSP_TIE_EXPSTATE1_IRQHandler)
DEFAULT_IRQ_HANDLER(MU_A_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT4_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT5_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT6_IRQHandler)
DEFAULT_IRQ_HANDLER(PIN_INT7_IRQHandler)
DEFAULT_IRQ_HANDLER(CTIMER2_IRQHandler)
DEFAULT_IRQ_HANDLER(CTIMER4_IRQHandler)
DEFAULT_IRQ_HANDLER(OS_EVENT_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXSPI0_FLEXSPI1_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM6_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM7_IRQHandler)
DEFAULT_IRQ_HANDLER(USDHC0_IRQHandler)
DEFAULT_IRQ_HANDLER(USDHC1_IRQHandler)
DEFAULT_IRQ_HANDLER(SGPIO_INTA_IRQHandler)
DEFAULT_IRQ_HANDLER(SGPIO_INTB_IRQHandler)
DEFAULT_IRQ_HANDLER(I3C0_IRQHandler)
DEFAULT_IRQ_HANDLER(USB0_IRQHandler)
DEFAULT_IRQ_HANDLER(USB0_NEEDCLK_IRQHandler)
DEFAULT_IRQ_HANDLER(WDT1_IRQHandler)
DEFAULT_IRQ_HANDLER(USB_PHYDCD_IRQHandler)
DEFAULT_IRQ_HANDLER(DMA1_IRQHandler)
DEFAULT_IRQ_HANDLER(PUF_IRQHandler)
DEFAULT_IRQ_HANDLER(POWERQUAD_IRQHandler)
DEFAULT_IRQ_HANDLER(CASPER_IRQHandler)
DEFAULT_IRQ_HANDLER(PMU_PMIC_IRQHandler)
DEFAULT_IRQ_HANDLER(HASHCRYPT_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM8_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM9_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM10_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM11_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM12_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM13_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXCOMM16_IRQHandler)
DEFAULT_IRQ_HANDLER(I3C1_IRQHandler)
DEFAULT_IRQ_HANDLER(FLEXIO_IRQHandler)
DEFAULT_IRQ_HANDLER(LCDIF_IRQHandler)
DEFAULT_IRQ_HANDLER(GPU_IRQHandler)
DEFAULT_IRQ_HANDLER(MIPI_IRQHandler)
DEFAULT_IRQ_HANDLER(Reserved88_IRQHandler)
DEFAULT_IRQ_HANDLER(SDMA_IRQHandler)

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/

#if defined ( __GNUC__ )
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

extern const VECTOR_TABLE_Type __VECTOR_TABLE[];
       const VECTOR_TABLE_Type __VECTOR_TABLE[] __VECTOR_TABLE_ATTRIBUTE = {
  (VECTOR_TABLE_Type)(&__INITIAL_SP),            /*      Initial Stack Pointer */
  Reset_Handler,                     /* Reset Handler */
  NMI_Handler,                       /* NMI Handler*/
  HardFault_Handler,                 /* Hard Fault Handler*/
  MemManage_Handler,                 /* MPU Fault Handler*/
  BusFault_Handler,                  /* Bus Fault Handler*/
  UsageFault_Handler,                /* Usage Fault Handler*/
  SecureFault_Handler,               /* Secure Fault Handler */
  0,                                 /* Reserved*/
  0,                                 /* Reserved*/
  0,                                 /* Reserved*/
  SVC_Handler,                       /* SVCall Handler*/
  DebugMon_Handler,                  /* Debug Monitor Handler*/
  0,                                 /* Reserved*/
  PendSV_Handler,                    /* PendSV Handler*/
  SysTick_Handler,                   /* SysTick Handler*/

/* External Interrupts*/
  WDT0_IRQHandler,                   /* Watchdog timer interrupt */
  DMA0_IRQHandler,                   /* DMA interrupt */
  GPIO_INTA_IRQHandler,              /* GPIO Interrupt A */
  GPIO_INTB_IRQHandler,              /* GPIO Interrupt B */
  PIN_INT0_IRQHandler,               /* General Purpose Input/Output interrupt 0 */
  PIN_INT1_IRQHandler,               /* General Purpose Input/Output interrupt 1 */
  PIN_INT2_IRQHandler,               /* General Purpose Input/Output interrupt 2 */
  PIN_INT3_IRQHandler,               /* General Purpose Input/Output interrupt 3 */
  UTICK0_IRQHandler,                 /* Micro-tick Timer */
  MRT0_IRQHandler,                   /* Multi-Rate Timer */
  CTIMER0_IRQHandler,                /* Standard counter/timer CTIMER0 */
  CTIMER1_IRQHandler,                /* Standard counter/timer CTIMER1 */
  SCT0_IRQHandler,                   /* SCTimer/PWM */
  CTIMER3_IRQHandler,                /* Standard counter/timer CTIMER3 */
  FLEXCOMM0_IRQHandler,              /* Flexcomm Interface 0 (USART, SPI, I2C, I2S) */
  FLEXCOMM1_IRQHandler,              /* Flexcomm Interface 1 (USART, SPI, I2C, I2S) */
  FLEXCOMM2_IRQHandler,              /* Flexcomm Interface 2 (USART, SPI, I2C, I2S) */
  FLEXCOMM3_IRQHandler,              /* Flexcomm Interface 3 (USART, SPI, I2C, I2S) */
  FLEXCOMM4_IRQHandler,              /* Flexcomm Interface 4 (USART, SPI, I2C, I2S) */
  FLEXCOMM5_IRQHandler,              /* Flexcomm Interface 5 (USART, SPI, I2C, I2S) */
  FLEXCOMM14_IRQHandler,             /* Flexcomm Interface 14 (SPI only) */
  FLEXCOMM15_IRQHandler,             /* Flexcomm Interface 15 (I2C only) */
  ADC0_IRQHandler,                   /* ADC0 */
  Reserved39_IRQHandler,             /* Reserved interrupt */
  ACMP_IRQHandler,                   /* Analog comparator */
  DMIC0_IRQHandler,                  /* Digital microphone and DMIC subsystem */
  Reserved42_IRQHandler,             /* Reserved interrupt */
  HYPERVISOR_IRQHandler,             /* Hypervisor */
  SECURE_VIOLATION_IRQHandler,       /* Secure violation */
  HWVAD0_IRQHandler,                 /* Hardware Voice Activity Detector */
  Reserved46_IRQHandler,             /* Reserved interrupt */
  RNG_IRQHandler,                    /* Random number Generator */
  RTC_IRQHandler,                    /* RTC alarm and wake-up */
  DSP_TIE_EXPSTATE1_IRQHandler,      /* DSP interrupt */
  MU_A_IRQHandler,                   /* Messaging Unit port A for CM33 */
  PIN_INT4_IRQHandler,               /* Pin interrupt 4 or pattern match engine slice 4 int */
  PIN_INT5_IRQHandler,               /* Pin interrupt 5 or pattern match engine slice 5 int */
  PIN_INT6_IRQHandler,               /* Pin interrupt 6 or pattern match engine slice 6 int */
  PIN_INT7_IRQHandler,               /* Pin interrupt 7 or pattern match engine slice 7 int */
  CTIMER2_IRQHandler,                /* Standard counter/timer CTIMER2 */
  CTIMER4_IRQHandler,                /* Standard counter/timer CTIMER4 */
  OS_EVENT_IRQHandler,               /* OS event timer */
  FLEXSPI0_FLEXSPI1_IRQHandler,      /* FLEXSPI interface */
  FLEXCOMM6_IRQHandler,              /* Flexcomm Interface 6 (USART, SPI, I2C, I2S) */
  FLEXCOMM7_IRQHandler,              /* Flexcomm Interface 7 (USART, SPI, I2C, I2S) */
  USDHC0_IRQHandler,                 /* USDHC0 (Enhanced SDHC) interrupt request */
  USDHC1_IRQHandler,                 /* USDHC1 (Enhanced SDHC) interrupt request */
  SGPIO_INTA_IRQHandler,             /* Secure GPIO interrupt A */
  SGPIO_INTB_IRQHandler,             /* Secure GPIO interrupt B */
  I3C0_IRQHandler,                   /* I3C interface 0 */
  USB0_IRQHandler,                   /* High-speed USB device/host */
  USB0_NEEDCLK_IRQHandler,           /* USB Activity Wake-up Interrupt */
  WDT1_IRQHandler,                   /* Windowed watchdog timer 1 (HiFi 4 watchdog) */
  USB_PHYDCD_IRQHandler,             /* USBPHY DCD */
  DMA1_IRQHandler,                   /* DMA controller 1 (non-secure or HiFi 4 DMA) */
  PUF_IRQHandler,                    /* Physical Unclonable Function */
  POWERQUAD_IRQHandler,              /* PowerQuad math coprocessor */
  CASPER_IRQHandler,                 /* Casper cryptographic coprocessor */
  PMU_PMIC_IRQHandler,               /* Power management IC */
  HASHCRYPT_IRQHandler,              /* Hash-AES unit */
  FLEXCOMM8_IRQHandler,              /* FlexComm interrupt */
  FLEXCOMM9_IRQHandler,              /* FlexComm interrupt */
  FLEXCOMM10_IRQHandler,             /* FlexComm interrupt */
  FLEXCOMM11_IRQHandler,             /* FlexComm interrupt */
  FLEXCOMM12_IRQHandler,             /* FlexComm interrupt */
  FLEXCOMM13_IRQHandler,             /* FlexComm interrupt */
  FLEXCOMM16_IRQHandler,             /* FlexComm interrupt */
  I3C1_IRQHandler,                   /* Improved Inter Integrated Circuit 1 interrupt */
  FLEXIO_IRQHandler,                 /* Flexible I/O interrupt */
  LCDIF_IRQHandler,                  /* Liquid Crystal Display interface interrupt */
  GPU_IRQHandler,                    /* Graphics Processor Unit interrupt */
  MIPI_IRQHandler,                   /* MIPI interrupt */
  Reserved88_IRQHandler,             /* Reserved interrupt */
  SDMA_IRQHandler,                   /* Smart DMA Engine Controller interrupt */
};

#if defined ( __GNUC__ )
#pragma GCC diagnostic pop
#endif

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void)
{
#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __disable_irq();
#endif
    __set_PSP((uint32_t)(&__INITIAL_SP));

    __set_MSPLIM((uint32_t)(&__STACK_LIMIT));
    __set_PSPLIM((uint32_t)(&__STACK_LIMIT));

#if defined (__ARM_FEATURE_CMSE) && (__ARM_FEATURE_CMSE == 3U)
    __TZ_set_STACKSEAL_S((uint32_t *)(&__STACK_SEAL));
#endif

    SystemInit();                             /* CMSIS System Initialization */
    __PROGRAM_START();                        /* Enter PreMain (C library entry point) */
}
