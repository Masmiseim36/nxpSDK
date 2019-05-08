//*****************************************************************************
// MKL82Z7 startup code for use with MCUXpresso IDE
//
// Version : 010818
//*****************************************************************************
//
// Copyright 2016-2018 NXP
//
// SPDX-License-Identifier: BSD-3-Clause
//*****************************************************************************

#if defined (DEBUG)
#pragma GCC push_options
#pragma GCC optimize ("Og")
#endif // (DEBUG)

#if defined (__cplusplus)
#ifdef __REDLIB__
#error Redlib does not support C++
#else
//*****************************************************************************
//
// The entry point for the C++ library startup
//
//*****************************************************************************
extern "C" {
    extern void __libc_init_array(void);
}
#endif
#endif

#define WEAK __attribute__ ((weak))
#define WEAK_AV __attribute__ ((weak, section(".after_vectors")))
#define ALIAS(f) __attribute__ ((weak, alias (#f)))

//*****************************************************************************
#if defined (__cplusplus)
extern "C" {
#endif

//*****************************************************************************
// Flash Configuration block : 16-byte flash configuration field that stores
// default protection settings (loaded on reset) and security information that
// allows the MCU to restrict access to the Flash Memory module.
// Placed at address 0x400 by the linker script.
//*****************************************************************************
__attribute__ ((used,section(".FlashConfig"))) const struct {
    unsigned int word1;
    unsigned int word2;
    unsigned int word3;
    unsigned int word4;
} Flash_Config = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF3DFE};
//*****************************************************************************
// Declaration of external SystemInit function
//*****************************************************************************
#if defined (__USE_CMSIS)
extern void SystemInit(void);
#endif // (__USE_CMSIS)

//*****************************************************************************
// Forward declaration of the core exception handlers.
// When the application defines a handler (with the same name), this will
// automatically take precedence over these weak definitions.
// If your application is a C++ one, then any interrupt handlers defined
// in C++ files within in your main application will need to have C linkage
// rather than C++ linkage. To do this, make sure that you are using extern "C"
// { .... } around the interrupt handler within your main application code.
//*****************************************************************************
     void ResetISR(void);
WEAK void NMI_Handler(void);
WEAK void HardFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);

//*****************************************************************************
// Forward declaration of the application IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over weak definitions below
//*****************************************************************************
WEAK void DMA0_DMA4_IRQHandler(void);
WEAK void DMA1_DMA5_IRQHandler(void);
WEAK void DMA2_DMA6_IRQHandler(void);
WEAK void DMA3_DMA7_IRQHandler(void);
WEAK void DMA_Error_IRQHandler(void);
WEAK void FLEXIO0_IRQHandler(void);
WEAK void TPM0_IRQHandler(void);
WEAK void TPM1_IRQHandler(void);
WEAK void TPM2_IRQHandler(void);
WEAK void PIT0_IRQHandler(void);
WEAK void SPI0_IRQHandler(void);
WEAK void EMVSIM0_IRQHandler(void);
WEAK void LPUART0_IRQHandler(void);
WEAK void LPUART1_IRQHandler(void);
WEAK void I2C0_IRQHandler(void);
WEAK void QSPI0_IRQHandler(void);
WEAK void Reserved32_IRQHandler(void);
WEAK void PORTA_IRQHandler(void);
WEAK void PORTB_IRQHandler(void);
WEAK void PORTC_IRQHandler(void);
WEAK void PORTD_IRQHandler(void);
WEAK void PORTE_IRQHandler(void);
WEAK void LLWU_IRQHandler(void);
WEAK void LTC0_IRQHandler(void);
WEAK void USB0_IRQHandler(void);
WEAK void ADC0_IRQHandler(void);
WEAK void LPTMR0_IRQHandler(void);
WEAK void RTC_Seconds_IRQHandler(void);
WEAK void INTMUX0_0_IRQHandler(void);
WEAK void INTMUX0_1_IRQHandler(void);
WEAK void INTMUX0_2_IRQHandler(void);
WEAK void INTMUX0_3_IRQHandler(void);
WEAK void LPTMR1_IRQHandler(void);
WEAK void Reserved49_IRQHandler(void);
WEAK void Reserved50_IRQHandler(void);
WEAK void Reserved51_IRQHandler(void);
WEAK void SPI1_IRQHandler(void);
WEAK void LPUART2_IRQHandler(void);
WEAK void EMVSIM1_IRQHandler(void);
WEAK void I2C1_IRQHandler(void);
WEAK void TSI0_IRQHandler(void);
WEAK void PMC_IRQHandler(void);
WEAK void FTFA_IRQHandler(void);
WEAK void MCG_IRQHandler(void);
WEAK void WDOG_EWM_IRQHandler(void);
WEAK void DAC0_IRQHandler(void);
WEAK void TRNG0_IRQHandler(void);
WEAK void Reserved63_IRQHandler(void);
WEAK void CMP0_IRQHandler(void);
WEAK void Reserved65_IRQHandler(void);
WEAK void RTC_Alarm_IRQHandler(void);
WEAK void Reserved67_IRQHandler(void);
WEAK void Reserved68_IRQHandler(void);
WEAK void Reserved69_IRQHandler(void);
WEAK void Reserved70_IRQHandler(void);
WEAK void Reserved71_IRQHandler(void);
WEAK void DMA4_IRQHandler(void);
WEAK void DMA5_IRQHandler(void);
WEAK void DMA6_IRQHandler(void);
WEAK void DMA7_IRQHandler(void);

//*****************************************************************************
// Forward declaration of the driver IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the driver
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//*****************************************************************************
void DMA0_DMA4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA1_DMA5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA2_DMA6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA3_DMA7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_Error_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FLEXIO0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TPM2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EMVSIM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2C0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void QSPI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved32_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LLWU_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LTC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_Seconds_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void INTMUX0_0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void INTMUX0_1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void INTMUX0_2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void INTMUX0_3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved49_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved50_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved51_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPUART2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void EMVSIM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2C1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TSI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PMC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTFA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MCG_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WDOG_EWM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TRNG0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved63_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved65_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_Alarm_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved67_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved68_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved69_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved70_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved71_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

//*****************************************************************************
// The entry point for the application.
// __main() is the entry point for Redlib based applications
// main() is the entry point for Newlib based applications
//*****************************************************************************
#if defined (__REDLIB__)
extern void __main(void);
#endif
extern int main(void);

//*****************************************************************************
// External declaration for the pointer to the stack top from the Linker Script
//*****************************************************************************
extern void _vStackTop(void);
//*****************************************************************************
#if defined (__cplusplus)
} // extern "C"
#endif
//*****************************************************************************
// The vector table.
// This relies on the linker script to place at correct location in memory.
//*****************************************************************************
extern void (* const g_pfnVectors[])(void);
extern void * __Vectors __attribute__ ((alias ("g_pfnVectors")));

__attribute__ ((used, section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
    // Core Level - CM0P
    &_vStackTop,                       // The initial stack pointer
    ResetISR,                          // The reset handler
    NMI_Handler,                       // The NMI handler
    HardFault_Handler,                 // The hard fault handler
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVCall handler
    0,                                 // Reserved
    0,                                 // Reserved
    PendSV_Handler,                    // The PendSV handler
    SysTick_Handler,                   // The SysTick handler

    // Chip Level - MKL82Z7
    DMA0_DMA4_IRQHandler,    // 16: DMA channel 0, 4 transfer complete
    DMA1_DMA5_IRQHandler,    // 17: DMA channel 1, 5 transfer complete
    DMA2_DMA6_IRQHandler,    // 18: DMA channel 2, 6 transfer complete
    DMA3_DMA7_IRQHandler,    // 19: DMA channel 3, 7 transfer complete
    DMA_Error_IRQHandler,    // 20: DMA channel 0 - 7 error
    FLEXIO0_IRQHandler,      // 21: Flexible IO
    TPM0_IRQHandler,         // 22: Timer/PWM module 0
    TPM1_IRQHandler,         // 23: Timer/PWM module 1
    TPM2_IRQHandler,         // 24: Timer/PWM module 2
    PIT0_IRQHandler,         // 25: Periodic Interrupt Timer 0
    SPI0_IRQHandler,         // 26: Serial Peripheral Interface 0
    EMVSIM0_IRQHandler,      // 27: EMVSIM0 common interrupt
    LPUART0_IRQHandler,      // 28: LPUART0 status and error
    LPUART1_IRQHandler,      // 29: LPUART1 status and error
    I2C0_IRQHandler,         // 30: Inter-Integrated Circuit 0
    QSPI0_IRQHandler,        // 31: QuadSPI0 interrupt
    Reserved32_IRQHandler,   // 32: Reserved interrupt
    PORTA_IRQHandler,        // 33: Pin detect Port A
    PORTB_IRQHandler,        // 34: Pin detect Port B
    PORTC_IRQHandler,        // 35: Pin detect Port C
    PORTD_IRQHandler,        // 36: Pin detect Port D
    PORTE_IRQHandler,        // 37: Pin detect Port E
    LLWU_IRQHandler,         // 38: Low Leakage Wakeup
    LTC0_IRQHandler,         // 39: Low power trusted cryptographic
    USB0_IRQHandler,         // 40: USB OTG interrupt
    ADC0_IRQHandler,         // 41: Analog-to-Digital Converter 0
    LPTMR0_IRQHandler,       // 42: Low-Power Timer 0
    RTC_Seconds_IRQHandler,  // 43: RTC seconds
    INTMUX0_0_IRQHandler,    // 44: Selectable peripheral interrupt INTMUX0-0
    INTMUX0_1_IRQHandler,    // 45: Selectable peripheral interrupt INTMUX0-1
    INTMUX0_2_IRQHandler,    // 46: Selectable peripheral interrupt INTMUX0-2
    INTMUX0_3_IRQHandler,    // 47: Selectable peripheral interrupt INTMUX0-3
    LPTMR1_IRQHandler,       // 48: Low-Power Timer 1 (INTMUX source IRQ0)
    Reserved49_IRQHandler,   // 49: Reserved interrupt
    Reserved50_IRQHandler,   // 50: Reserved interrupt
    Reserved51_IRQHandler,   // 51: Reserved interrupt
    SPI1_IRQHandler,         // 52: Serial Peripheral Interface 1 (INTMUX source IRQ4)
    LPUART2_IRQHandler,      // 53: LPUART2 status and error (INTMUX source IRQ5)
    EMVSIM1_IRQHandler,      // 54: EMVSIM1 common interrupt (INTMUX source IRQ6)
    I2C1_IRQHandler,         // 55: Inter-Integrated Circuit 1 (INTMUX source IRQ7)
    TSI0_IRQHandler,         // 56: Touch Sensing Input 0 (INTMUX source IRQ8)
    PMC_IRQHandler,          // 57: PMC controller low-voltage detect, low-voltage warning (INTMUX source IRQ9)
    FTFA_IRQHandler,         // 58: FTFA command complete/read collision (INTMUX source IRQ10)
    MCG_IRQHandler,          // 59: Multipurpose clock generator (INTMUX source IRQ11)
    WDOG_EWM_IRQHandler,     // 60: Single interrupt vector for  WDOG and EWM (INTMUX source IRQ12)
    DAC0_IRQHandler,         // 61: Digital-to-analog converter 0 (INTMUX source IRQ13)
    TRNG0_IRQHandler,        // 62: True randon number generator (INTMUX source IRQ14)
    Reserved63_IRQHandler,   // 63: Reserved interrupt
    CMP0_IRQHandler,         // 64: Comparator 0 (INTMUX source IRQ16)
    Reserved65_IRQHandler,   // 65: Reserved interrupt
    RTC_Alarm_IRQHandler,    // 66: Real time clock (INTMUX source IRQ18)
    Reserved67_IRQHandler,   // 67: Reserved interrupt
    Reserved68_IRQHandler,   // 68: Reserved interrupt
    Reserved69_IRQHandler,   // 69: Reserved interrupt
    Reserved70_IRQHandler,   // 70: Reserved interrupt
    Reserved71_IRQHandler,   // 71: Reserved interrupt
    DMA4_IRQHandler,         // 72: DMA channel 4 transfer complete (INTMUX source IRQ24)
    DMA5_IRQHandler,         // 73: DMA channel 5 transfer complete (INTMUX source IRQ25)
    DMA6_IRQHandler,         // 74: DMA channel 6 transfer complete (INTMUX source IRQ26)
    DMA7_IRQHandler,         // 75: DMA channel 7 transfer complete (INTMUX source IRQ27)

}; /* End of g_pfnVectors */

//*****************************************************************************
// Functions to carry out the initialization of RW and BSS data sections. These
// are written as separate functions rather than being inlined within the
// ResetISR() function in order to cope with MCUs with multiple banks of
// memory.
//*****************************************************************************
__attribute__ ((section(".after_vectors.init_data")))
void data_init(unsigned int romstart, unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int *pulSrc = (unsigned int*) romstart;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = *pulSrc++;
}

__attribute__ ((section(".after_vectors.init_bss")))
void bss_init(unsigned int start, unsigned int len) {
    unsigned int *pulDest = (unsigned int*) start;
    unsigned int loop;
    for (loop = 0; loop < len; loop = loop + 4)
        *pulDest++ = 0;
}

//*****************************************************************************
// The following symbols are constructs generated by the linker, indicating
// the location of various points in the "Global Section Table". This table is
// created by the linker via the Code Red managed linker script mechanism. It
// contains the load address, execution address and length of each RW data
// section and the execution and length of each BSS (zero initialized) section.
//*****************************************************************************
extern unsigned int __data_section_table;
extern unsigned int __data_section_table_end;
extern unsigned int __bss_section_table;
extern unsigned int __bss_section_table_end;

//*****************************************************************************
// Reset entry point for your code.
// Sets up a simple runtime environment and initializes the C/C++
// library.
//*****************************************************************************
__attribute__ ((section(".after_vectors.reset")))
void ResetISR(void) {

    // Disable interrupts
    __asm volatile ("cpsid i");

#if defined (__USE_CMSIS)
// If __USE_CMSIS defined, then call CMSIS SystemInit code
    SystemInit();

#else
    // Disable Watchdog
    //  Write 0xC520 to watchdog unlock register
    *((volatile unsigned short *)0x4005200E) = 0xC520;
    //  Followed by 0xD928 to complete the unlock
    *((volatile unsigned short *)0x4005200E) = 0xD928;
    // Now disable watchdog via STCTRLH register
    *((volatile unsigned short *)0x40052000) = 0x01D2u;
#endif // (__USE_CMSIS)

    //
    // Copy the data sections from flash to SRAM.
    //
    unsigned int LoadAddr, ExeAddr, SectionLen;
    unsigned int *SectionTableAddr;

    // Load base address of Global Section Table
    SectionTableAddr = &__data_section_table;

    // Copy the data sections from flash to SRAM.
    while (SectionTableAddr < &__data_section_table_end) {
        LoadAddr = *SectionTableAddr++;
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        data_init(LoadAddr, ExeAddr, SectionLen);
    }

    // At this point, SectionTableAddr = &__bss_section_table;
    // Zero fill the bss segment
    while (SectionTableAddr < &__bss_section_table_end) {
        ExeAddr = *SectionTableAddr++;
        SectionLen = *SectionTableAddr++;
        bss_init(ExeAddr, SectionLen);
    }

#if !defined (__USE_CMSIS)
// Assume that if __USE_CMSIS defined, then CMSIS SystemInit code
// will setup the VTOR register

    // Check to see if we are running the code from a non-zero
    // address (eg RAM, external flash), in which case we need
    // to modify the VTOR register to tell the CPU that the
    // vector table is located at a non-0x0 address.
    unsigned int * pSCB_VTOR = (unsigned int *) 0xE000ED08;
    if ((unsigned int *)g_pfnVectors!=(unsigned int *) 0x00000000) {
        *pSCB_VTOR = (unsigned int)g_pfnVectors;
    }
#endif // (__USE_CMSIS)

#if defined (__cplusplus)
    //
    // Call C++ library initialisation
    //
    __libc_init_array();
#endif

    // Reenable interrupts
    __asm volatile ("cpsie i");

#if defined (__REDLIB__)
    // Call the Redlib library, which in turn calls main()
    __main();
#else
    main();
#endif

    //
    // main() shouldn't return, but if it does, we'll just enter an infinite loop
    //
    while (1) {
        ;
    }
}

//*****************************************************************************
// Default core exception handlers. Override the ones here by defining your own
// handler routines in your application code.
//*****************************************************************************
WEAK_AV void NMI_Handler(void)
{ while(1) {}
}

WEAK_AV void HardFault_Handler(void)
{ while(1) {}
}

WEAK_AV void SVC_Handler(void)
{ while(1) {}
}

WEAK_AV void PendSV_Handler(void)
{ while(1) {}
}

WEAK_AV void SysTick_Handler(void)
{ while(1) {}
}

//*****************************************************************************
// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
//*****************************************************************************
WEAK_AV void IntDefaultHandler(void)
{ while(1) {}
}

//*****************************************************************************
// Default application exception handlers. Override the ones here by defining
// your own handler routines in your application code. These routines call
// driver exception handlers or IntDefaultHandler() if no driver exception
// handler is included.
//*****************************************************************************
WEAK_AV void DMA0_DMA4_IRQHandler(void)
{   DMA0_DMA4_DriverIRQHandler();
}

WEAK_AV void DMA1_DMA5_IRQHandler(void)
{   DMA1_DMA5_DriverIRQHandler();
}

WEAK_AV void DMA2_DMA6_IRQHandler(void)
{   DMA2_DMA6_DriverIRQHandler();
}

WEAK_AV void DMA3_DMA7_IRQHandler(void)
{   DMA3_DMA7_DriverIRQHandler();
}

WEAK_AV void DMA_Error_IRQHandler(void)
{   DMA_Error_DriverIRQHandler();
}

WEAK_AV void FLEXIO0_IRQHandler(void)
{   FLEXIO0_DriverIRQHandler();
}

WEAK_AV void TPM0_IRQHandler(void)
{   TPM0_DriverIRQHandler();
}

WEAK_AV void TPM1_IRQHandler(void)
{   TPM1_DriverIRQHandler();
}

WEAK_AV void TPM2_IRQHandler(void)
{   TPM2_DriverIRQHandler();
}

WEAK_AV void PIT0_IRQHandler(void)
{   PIT0_DriverIRQHandler();
}

WEAK_AV void SPI0_IRQHandler(void)
{   SPI0_DriverIRQHandler();
}

WEAK_AV void EMVSIM0_IRQHandler(void)
{   EMVSIM0_DriverIRQHandler();
}

WEAK_AV void LPUART0_IRQHandler(void)
{   LPUART0_DriverIRQHandler();
}

WEAK_AV void LPUART1_IRQHandler(void)
{   LPUART1_DriverIRQHandler();
}

WEAK_AV void I2C0_IRQHandler(void)
{   I2C0_DriverIRQHandler();
}

WEAK_AV void QSPI0_IRQHandler(void)
{   QSPI0_DriverIRQHandler();
}

WEAK_AV void Reserved32_IRQHandler(void)
{   Reserved32_DriverIRQHandler();
}

WEAK_AV void PORTA_IRQHandler(void)
{   PORTA_DriverIRQHandler();
}

WEAK_AV void PORTB_IRQHandler(void)
{   PORTB_DriverIRQHandler();
}

WEAK_AV void PORTC_IRQHandler(void)
{   PORTC_DriverIRQHandler();
}

WEAK_AV void PORTD_IRQHandler(void)
{   PORTD_DriverIRQHandler();
}

WEAK_AV void PORTE_IRQHandler(void)
{   PORTE_DriverIRQHandler();
}

WEAK_AV void LLWU_IRQHandler(void)
{   LLWU_DriverIRQHandler();
}

WEAK_AV void LTC0_IRQHandler(void)
{   LTC0_DriverIRQHandler();
}

WEAK_AV void USB0_IRQHandler(void)
{   USB0_DriverIRQHandler();
}

WEAK_AV void ADC0_IRQHandler(void)
{   ADC0_DriverIRQHandler();
}

WEAK_AV void LPTMR0_IRQHandler(void)
{   LPTMR0_DriverIRQHandler();
}

WEAK_AV void RTC_Seconds_IRQHandler(void)
{   RTC_Seconds_DriverIRQHandler();
}

WEAK_AV void INTMUX0_0_IRQHandler(void)
{   INTMUX0_0_DriverIRQHandler();
}

WEAK_AV void INTMUX0_1_IRQHandler(void)
{   INTMUX0_1_DriverIRQHandler();
}

WEAK_AV void INTMUX0_2_IRQHandler(void)
{   INTMUX0_2_DriverIRQHandler();
}

WEAK_AV void INTMUX0_3_IRQHandler(void)
{   INTMUX0_3_DriverIRQHandler();
}

WEAK_AV void LPTMR1_IRQHandler(void)
{   LPTMR1_DriverIRQHandler();
}

WEAK_AV void Reserved49_IRQHandler(void)
{   Reserved49_DriverIRQHandler();
}

WEAK_AV void Reserved50_IRQHandler(void)
{   Reserved50_DriverIRQHandler();
}

WEAK_AV void Reserved51_IRQHandler(void)
{   Reserved51_DriverIRQHandler();
}

WEAK_AV void SPI1_IRQHandler(void)
{   SPI1_DriverIRQHandler();
}

WEAK_AV void LPUART2_IRQHandler(void)
{   LPUART2_DriverIRQHandler();
}

WEAK_AV void EMVSIM1_IRQHandler(void)
{   EMVSIM1_DriverIRQHandler();
}

WEAK_AV void I2C1_IRQHandler(void)
{   I2C1_DriverIRQHandler();
}

WEAK_AV void TSI0_IRQHandler(void)
{   TSI0_DriverIRQHandler();
}

WEAK_AV void PMC_IRQHandler(void)
{   PMC_DriverIRQHandler();
}

WEAK_AV void FTFA_IRQHandler(void)
{   FTFA_DriverIRQHandler();
}

WEAK_AV void MCG_IRQHandler(void)
{   MCG_DriverIRQHandler();
}

WEAK_AV void WDOG_EWM_IRQHandler(void)
{   WDOG_EWM_DriverIRQHandler();
}

WEAK_AV void DAC0_IRQHandler(void)
{   DAC0_DriverIRQHandler();
}

WEAK_AV void TRNG0_IRQHandler(void)
{   TRNG0_DriverIRQHandler();
}

WEAK_AV void Reserved63_IRQHandler(void)
{   Reserved63_DriverIRQHandler();
}

WEAK_AV void CMP0_IRQHandler(void)
{   CMP0_DriverIRQHandler();
}

WEAK_AV void Reserved65_IRQHandler(void)
{   Reserved65_DriverIRQHandler();
}

WEAK_AV void RTC_Alarm_IRQHandler(void)
{   RTC_Alarm_DriverIRQHandler();
}

WEAK_AV void Reserved67_IRQHandler(void)
{   Reserved67_DriverIRQHandler();
}

WEAK_AV void Reserved68_IRQHandler(void)
{   Reserved68_DriverIRQHandler();
}

WEAK_AV void Reserved69_IRQHandler(void)
{   Reserved69_DriverIRQHandler();
}

WEAK_AV void Reserved70_IRQHandler(void)
{   Reserved70_DriverIRQHandler();
}

WEAK_AV void Reserved71_IRQHandler(void)
{   Reserved71_DriverIRQHandler();
}

WEAK_AV void DMA4_IRQHandler(void)
{   DMA4_DriverIRQHandler();
}

WEAK_AV void DMA5_IRQHandler(void)
{   DMA5_DriverIRQHandler();
}

WEAK_AV void DMA6_IRQHandler(void)
{   DMA6_DriverIRQHandler();
}

WEAK_AV void DMA7_IRQHandler(void)
{   DMA7_DriverIRQHandler();
}

//*****************************************************************************

#if defined (DEBUG)
#pragma GCC pop_options
#endif // (DEBUG)
