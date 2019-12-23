//*****************************************************************************
// MKV42F16 startup code for use with MCUXpresso IDE
//
// Version : 180919
//*****************************************************************************
//
// Copyright 2016-2019 NXP
// All rights reserved.
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
} Flash_Config = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFE};
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
WEAK void MemManage_Handler(void);
WEAK void BusFault_Handler(void);
WEAK void UsageFault_Handler(void);
WEAK void SVC_Handler(void);
WEAK void DebugMon_Handler(void);
WEAK void PendSV_Handler(void);
WEAK void SysTick_Handler(void);
WEAK void IntDefaultHandler(void);

//*****************************************************************************
// Forward declaration of the application IRQ handlers. When the application
// defines a handler (with the same name), this will automatically take
// precedence over weak definitions below
//*****************************************************************************
WEAK void DMA0_IRQHandler(void);
WEAK void DMA1_IRQHandler(void);
WEAK void DMA2_IRQHandler(void);
WEAK void DMA3_IRQHandler(void);
WEAK void DMA4_IRQHandler(void);
WEAK void DMA5_IRQHandler(void);
WEAK void DMA6_IRQHandler(void);
WEAK void DMA7_IRQHandler(void);
WEAK void DMA8_IRQHandler(void);
WEAK void DMA9_IRQHandler(void);
WEAK void DMA10_IRQHandler(void);
WEAK void DMA11_IRQHandler(void);
WEAK void DMA12_IRQHandler(void);
WEAK void DMA13_IRQHandler(void);
WEAK void DMA14_IRQHandler(void);
WEAK void DMA15_IRQHandler(void);
WEAK void DMA_Error_IRQHandler(void);
WEAK void MCM_IRQHandler(void);
WEAK void FTFA_IRQHandler(void);
WEAK void FTFA_Collision_IRQHandler(void);
WEAK void PMC_IRQHandler(void);
WEAK void LLWU_IRQHandler(void);
WEAK void WDOG_EWM_IRQHandler(void);
WEAK void Reserved39_IRQHandler(void);
WEAK void I2C0_IRQHandler(void);
WEAK void Reserved41_IRQHandler(void);
WEAK void SPI0_IRQHandler(void);
WEAK void Reserved43_IRQHandler(void);
WEAK void Reserved44_IRQHandler(void);
WEAK void Reserved45_IRQHandler(void);
WEAK void Reserved46_IRQHandler(void);
WEAK void UART0_RX_TX_IRQHandler(void);
WEAK void UART0_ERR_IRQHandler(void);
WEAK void UART1_RX_TX_IRQHandler(void);
WEAK void UART1_ERR_IRQHandler(void);
WEAK void Reserved51_IRQHandler(void);
WEAK void Reserved52_IRQHandler(void);
WEAK void Reserved53_IRQHandler(void);
WEAK void ADC_ERR_IRQHandler(void);
WEAK void ADCA_IRQHandler(void);
WEAK void CMP0_IRQHandler(void);
WEAK void CMP1_IRQHandler(void);
WEAK void FTM0_IRQHandler(void);
WEAK void FTM1_IRQHandler(void);
WEAK void Reserved60_IRQHandler(void);
WEAK void Reserved61_IRQHandler(void);
WEAK void Reserved62_IRQHandler(void);
WEAK void Reserved63_IRQHandler(void);
WEAK void PIT0_IRQHandler(void);
WEAK void PIT1_IRQHandler(void);
WEAK void PIT2_IRQHandler(void);
WEAK void PIT3_IRQHandler(void);
WEAK void PDB0_IRQHandler(void);
WEAK void Reserved69_IRQHandler(void);
WEAK void XBARA_IRQHandler(void);
WEAK void PDB1_IRQHandler(void);
WEAK void Reserved72_IRQHandler(void);
WEAK void MCG_IRQHandler(void);
WEAK void LPTMR0_IRQHandler(void);
WEAK void PORTA_IRQHandler(void);
WEAK void PORTB_IRQHandler(void);
WEAK void PORTC_IRQHandler(void);
WEAK void PORTD_IRQHandler(void);
WEAK void PORTE_IRQHandler(void);
WEAK void SWI_IRQHandler(void);
WEAK void Reserved81_IRQHandler(void);
WEAK void ENC0_COMPARE_IRQHandler(void);
WEAK void ENC0_HOME_IRQHandler(void);
WEAK void ENC0_WDOG_SAB_IRQHandler(void);
WEAK void ENC0_INDEX_IRQHandler(void);
WEAK void CMP2_IRQHandler(void);
WEAK void FTM3_IRQHandler(void);
WEAK void Reserved88_IRQHandler(void);
WEAK void ADCB_IRQHandler(void);
WEAK void Reserved90_IRQHandler(void);
WEAK void CAN0_ORed_Message_buffer_IRQHandler(void);
WEAK void CAN0_Bus_Off_IRQHandler(void);
WEAK void CAN0_Error_IRQHandler(void);
WEAK void CAN0_Tx_Warning_IRQHandler(void);
WEAK void CAN0_Rx_Warning_IRQHandler(void);
WEAK void CAN0_Wake_Up_IRQHandler(void);
WEAK void Reserved97_IRQHandler(void);
WEAK void Reserved98_IRQHandler(void);
WEAK void Reserved99_IRQHandler(void);
WEAK void Reserved100_IRQHandler(void);
WEAK void Reserved101_IRQHandler(void);
WEAK void Reserved102_IRQHandler(void);
WEAK void Reserved103_IRQHandler(void);
WEAK void Reserved104_IRQHandler(void);
WEAK void Reserved105_IRQHandler(void);
WEAK void Reserved106_IRQHandler(void);
WEAK void Reserved107_IRQHandler(void);
WEAK void CMP3_IRQHandler(void);
WEAK void Reserved109_IRQHandler(void);
WEAK void CAN1_ORed_Message_buffer_IRQHandler(void);
WEAK void CAN1_Bus_Off_IRQHandler(void);
WEAK void CAN1_Error_IRQHandler(void);
WEAK void CAN1_Tx_Warning_IRQHandler(void);
WEAK void CAN1_Rx_Warning_IRQHandler(void);
WEAK void CAN1_Wake_Up_IRQHandler(void);

//*****************************************************************************
// Forward declaration of the driver IRQ handlers. These are aliased
// to the IntDefaultHandler, which is a 'forever' loop. When the driver
// defines a handler (with the same name), this will automatically take
// precedence over these weak definitions
//*****************************************************************************
void DMA0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA4_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA5_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA6_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA7_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA8_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA9_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA10_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA11_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA12_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA13_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA14_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA15_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DMA_Error_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MCM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTFA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTFA_Collision_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PMC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LLWU_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WDOG_EWM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved39_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2C0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved41_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved43_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved44_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved45_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved46_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved51_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved52_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved53_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADCA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved60_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved61_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved62_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved63_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDB0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved69_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void XBARA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDB1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved72_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MCG_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SWI_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved81_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_COMPARE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_HOME_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_WDOG_SAB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ENC0_INDEX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved88_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADCB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved90_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved97_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved98_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved99_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved100_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved101_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved102_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved103_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved104_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved105_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved106_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved107_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved109_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

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
    // Core Level - CM4
    &_vStackTop,                       // The initial stack pointer
    ResetISR,                          // The reset handler
    NMI_Handler,                       // The NMI handler
    HardFault_Handler,                 // The hard fault handler
    MemManage_Handler,                 // The MPU fault handler
    BusFault_Handler,                  // The bus fault handler
    UsageFault_Handler,                // The usage fault handler
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    0,                                 // Reserved
    SVC_Handler,                       // SVCall handler
    DebugMon_Handler,                  // Debug monitor handler
    0,                                 // Reserved
    PendSV_Handler,                    // The PendSV handler
    SysTick_Handler,                   // The SysTick handler

    // Chip Level - MKV42F16
    DMA0_IRQHandler,                      // 16 : DMA channel 0, 16 transfer complete
    DMA1_IRQHandler,                      // 17 : DMA channel 1, 17 transfer complete
    DMA2_IRQHandler,                      // 18 : DMA channel 2, 18 transfer complete
    DMA3_IRQHandler,                      // 19 : DMA channel 3, 19 transfer complete
    DMA4_IRQHandler,                      // 20 : DMA channel 4, 20 transfer complete
    DMA5_IRQHandler,                      // 21 : DMA channel 5, 21 transfer complete
    DMA6_IRQHandler,                      // 22 : DMA channel 6, 22 transfer complete
    DMA7_IRQHandler,                      // 23 : DMA channel 7, 23 transfer complete
    DMA8_IRQHandler,                      // 24 : DMA channel 8, 24 transfer complete
    DMA9_IRQHandler,                      // 25 : DMA channel 9, 25 transfer complete
    DMA10_IRQHandler,                     // 26 : DMA channel 10, 26 transfer complete
    DMA11_IRQHandler,                     // 27 : DMA channel 11, 27 transfer complete
    DMA12_IRQHandler,                     // 28 : DMA channel 12, 28 transfer complete
    DMA13_IRQHandler,                     // 29 : DMA channel 13, 29 transfer complete
    DMA14_IRQHandler,                     // 30 : DMA channel 14, 30 transfer complete
    DMA15_IRQHandler,                     // 31 : DMA channel 15, 31 transfer complete
    DMA_Error_IRQHandler,                 // 32 : DMA error interrupt channels 0-1531
    MCM_IRQHandler,                       // 33 : MCM interrupt
    FTFA_IRQHandler,                      // 34 : Command complete
    FTFA_Collision_IRQHandler,            // 35 : Read collision
    PMC_IRQHandler,                       // 36 : Low-voltage detect, low-voltage warning
    LLWU_IRQHandler,                      // 37 : Low Leakage Wakeup
    WDOG_EWM_IRQHandler,                  // 38 : Both watchdog modules share this interrupt
    Reserved39_IRQHandler,                // 39 : Reserved interrupt
    I2C0_IRQHandler,                      // 40 : I2C0
    Reserved41_IRQHandler,                // 41 : Reserved interrupt
    SPI0_IRQHandler,                      // 42 : SPI0
    Reserved43_IRQHandler,                // 43 : Reserved interrupt
    Reserved44_IRQHandler,                // 44 : Reserved interrupt
    Reserved45_IRQHandler,                // 45 : Reserved interrupt
    Reserved46_IRQHandler,                // 46 : Reserved interrupt
    UART0_RX_TX_IRQHandler,               // 47 : UART0 status sources
    UART0_ERR_IRQHandler,                 // 48 : UART0 error sources
    UART1_RX_TX_IRQHandler,               // 49 : UART1 status sources
    UART1_ERR_IRQHandler,                 // 50 : UART1 error sources
    Reserved51_IRQHandler,                // 51 : Reserved interrupt
    Reserved52_IRQHandler,                // 52 : Reserved interrupt
    Reserved53_IRQHandler,                // 53 : Reserved interrupt
    ADC_ERR_IRQHandler,                   // 54 : ADC_ERR A and B ( zero cross, high/low limit)
    ADCA_IRQHandler,                      // 55 : ADCA Scan complete
    CMP0_IRQHandler,                      // 56 : CMP0
    CMP1_IRQHandler,                      // 57 : CMP1
    FTM0_IRQHandler,                      // 58 : FTM0 8 channels
    FTM1_IRQHandler,                      // 59 : FTM1 2 channels
    Reserved60_IRQHandler,                // 60 : Reserved interrupt
    Reserved61_IRQHandler,                // 61 : Reserved interrupt
    Reserved62_IRQHandler,                // 62 : Reserved interrupt
    Reserved63_IRQHandler,                // 63 : Reserved interrupt
    PIT0_IRQHandler,                      // 64 : PIT Channel 0
    PIT1_IRQHandler,                      // 65 : PIT Channel 1
    PIT2_IRQHandler,                      // 66 : PIT Channel 2
    PIT3_IRQHandler,                      // 67 : PIT Channel 3
    PDB0_IRQHandler,                      // 68 : PDB0
    Reserved69_IRQHandler,                // 69 : Reserved interrupt
    XBARA_IRQHandler,                     // 70 : XBARA
    PDB1_IRQHandler,                      // 71 : PDB1
    Reserved72_IRQHandler,                // 72 : Reserved interrupt
    MCG_IRQHandler,                       // 73 : MCG
    LPTMR0_IRQHandler,                    // 74 : LPTMR0
    PORTA_IRQHandler,                     // 75 : Pin detect (Port A)
    PORTB_IRQHandler,                     // 76 : Pin detect (Port B)
    PORTC_IRQHandler,                     // 77 : Pin detect (Port C)
    PORTD_IRQHandler,                     // 78 : Pin detect (Port D)
    PORTE_IRQHandler,                     // 79 : Pin detect (Port E)
    SWI_IRQHandler,                       // 80 : Software
    Reserved81_IRQHandler,                // 81 : Reserved interrupt
    ENC0_COMPARE_IRQHandler,              // 82 : ENC0 Compare
    ENC0_HOME_IRQHandler,                 // 83 : ENC0 Home
    ENC0_WDOG_SAB_IRQHandler,             // 84 : ENC0 Watchdog/Simultaneous A and B change
    ENC0_INDEX_IRQHandler,                // 85 : ENC0 Index/Roll over/Roll Under
    CMP2_IRQHandler,                      // 86 : CMP2
    FTM3_IRQHandler,                      // 87 : FTM3 8 channels
    Reserved88_IRQHandler,                // 88 : Reserved interrupt
    ADCB_IRQHandler,                      // 89 : ADCB Scan complete
    Reserved90_IRQHandler,                // 90 : Reserved interrupt
    CAN0_ORed_Message_buffer_IRQHandler,  // 91 : FLexCAN0 OR'ed Message buffer (0-15)
    CAN0_Bus_Off_IRQHandler,              // 92 : FLexCAN0 Bus Off
    CAN0_Error_IRQHandler,                // 93 : FLexCAN0 Error
    CAN0_Tx_Warning_IRQHandler,           // 94 : FLexCAN0 Transmit Warning
    CAN0_Rx_Warning_IRQHandler,           // 95 : FLexCAN0 Receive Warning
    CAN0_Wake_Up_IRQHandler,              // 96 : FLexCAN0 Wake Up
    Reserved97_IRQHandler,                // 97 : Reserved interrupt
    Reserved98_IRQHandler,                // 98 : Reserved interrupt
    Reserved99_IRQHandler,                // 99 : Reserved interrupt
    Reserved100_IRQHandler,               // 100: Reserved interrupt
    Reserved101_IRQHandler,               // 101: Reserved interrupt
    Reserved102_IRQHandler,               // 102: Reserved interrupt
    Reserved103_IRQHandler,               // 103: Reserved interrupt
    Reserved104_IRQHandler,               // 104: Reserved interrupt
    Reserved105_IRQHandler,               // 105: Reserved interrupt
    Reserved106_IRQHandler,               // 106: Reserved interrupt
    Reserved107_IRQHandler,               // 107: Reserved interrupt
    CMP3_IRQHandler,                      // 108: CMP3
    Reserved109_IRQHandler,               // 109: Reserved interrupt
    CAN1_ORed_Message_buffer_IRQHandler,  // 110: FLexCAN1 OR'ed Message buffer (0-15)
    CAN1_Bus_Off_IRQHandler,              // 111: FLexCAN1 Bus Off
    CAN1_Error_IRQHandler,                // 112: FLexCAN1 Error
    CAN1_Tx_Warning_IRQHandler,           // 113: FLexCAN1 Transmit Warning
    CAN1_Rx_Warning_IRQHandler,           // 114: FLexCAN1 Receive Warning
    CAN1_Wake_Up_IRQHandler,              // 115: FLexCAN1 Wake Up

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
// will enable the FPU
#if defined (__VFP_FP__) && !defined (__SOFTFP__)
    //
    // Code to enable the Cortex-M4 FPU only included
    // if appropriate build options have been selected.
    // Code taken from Section 7.1, Cortex-M4 TRM (DDI0439C)
    //
    // Read CPACR (located at address 0xE000ED88)
    // Set bits 20-23 to enable CP10 and CP11 coprocessors
    // Write back the modified value to the CPACR
    asm volatile ("LDR.W R0, =0xE000ED88\n\t"
                  "LDR R1, [R0]\n\t"
                  "ORR R1, R1, #(0xF << 20)\n\t"
                  "STR R1, [R0]");
#endif // (__VFP_FP__) && !(__SOFTFP__)
#endif // (__USE_CMSIS)


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

WEAK_AV void MemManage_Handler(void)
{ while(1) {}
}

WEAK_AV void BusFault_Handler(void)
{ while(1) {}
}

WEAK_AV void UsageFault_Handler(void)
{ while(1) {}
}

WEAK_AV void SVC_Handler(void)
{ while(1) {}
}

WEAK_AV void DebugMon_Handler(void)
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
WEAK void DMA0_IRQHandler(void)
{   DMA0_DriverIRQHandler();
}

WEAK void DMA1_IRQHandler(void)
{   DMA1_DriverIRQHandler();
}

WEAK void DMA2_IRQHandler(void)
{   DMA2_DriverIRQHandler();
}

WEAK void DMA3_IRQHandler(void)
{   DMA3_DriverIRQHandler();
}

WEAK void DMA4_IRQHandler(void)
{   DMA4_DriverIRQHandler();
}

WEAK void DMA5_IRQHandler(void)
{   DMA5_DriverIRQHandler();
}

WEAK void DMA6_IRQHandler(void)
{   DMA6_DriverIRQHandler();
}

WEAK void DMA7_IRQHandler(void)
{   DMA7_DriverIRQHandler();
}

WEAK void DMA8_IRQHandler(void)
{   DMA8_DriverIRQHandler();
}

WEAK void DMA9_IRQHandler(void)
{   DMA9_DriverIRQHandler();
}

WEAK void DMA10_IRQHandler(void)
{   DMA10_DriverIRQHandler();
}

WEAK void DMA11_IRQHandler(void)
{   DMA11_DriverIRQHandler();
}

WEAK void DMA12_IRQHandler(void)
{   DMA12_DriverIRQHandler();
}

WEAK void DMA13_IRQHandler(void)
{   DMA13_DriverIRQHandler();
}

WEAK void DMA14_IRQHandler(void)
{   DMA14_DriverIRQHandler();
}

WEAK void DMA15_IRQHandler(void)
{   DMA15_DriverIRQHandler();
}

WEAK void DMA_Error_IRQHandler(void)
{   DMA_Error_DriverIRQHandler();
}

WEAK void MCM_IRQHandler(void)
{   MCM_DriverIRQHandler();
}

WEAK void FTFA_IRQHandler(void)
{   FTFA_DriverIRQHandler();
}

WEAK void FTFA_Collision_IRQHandler(void)
{   FTFA_Collision_DriverIRQHandler();
}

WEAK void PMC_IRQHandler(void)
{   PMC_DriverIRQHandler();
}

WEAK void LLWU_IRQHandler(void)
{   LLWU_DriverIRQHandler();
}

WEAK void WDOG_EWM_IRQHandler(void)
{   WDOG_EWM_DriverIRQHandler();
}

WEAK void Reserved39_IRQHandler(void)
{   Reserved39_DriverIRQHandler();
}

WEAK void I2C0_IRQHandler(void)
{   I2C0_DriverIRQHandler();
}

WEAK void Reserved41_IRQHandler(void)
{   Reserved41_DriverIRQHandler();
}

WEAK void SPI0_IRQHandler(void)
{   SPI0_DriverIRQHandler();
}

WEAK void Reserved43_IRQHandler(void)
{   Reserved43_DriverIRQHandler();
}

WEAK void Reserved44_IRQHandler(void)
{   Reserved44_DriverIRQHandler();
}

WEAK void Reserved45_IRQHandler(void)
{   Reserved45_DriverIRQHandler();
}

WEAK void Reserved46_IRQHandler(void)
{   Reserved46_DriverIRQHandler();
}

WEAK void UART0_RX_TX_IRQHandler(void)
{   UART0_RX_TX_DriverIRQHandler();
}

WEAK void UART0_ERR_IRQHandler(void)
{   UART0_ERR_DriverIRQHandler();
}

WEAK void UART1_RX_TX_IRQHandler(void)
{   UART1_RX_TX_DriverIRQHandler();
}

WEAK void UART1_ERR_IRQHandler(void)
{   UART1_ERR_DriverIRQHandler();
}

WEAK void Reserved51_IRQHandler(void)
{   Reserved51_DriverIRQHandler();
}

WEAK void Reserved52_IRQHandler(void)
{   Reserved52_DriverIRQHandler();
}

WEAK void Reserved53_IRQHandler(void)
{   Reserved53_DriverIRQHandler();
}

WEAK void ADC_ERR_IRQHandler(void)
{   ADC_ERR_DriverIRQHandler();
}

WEAK void ADCA_IRQHandler(void)
{   ADCA_DriverIRQHandler();
}

WEAK void CMP0_IRQHandler(void)
{   CMP0_DriverIRQHandler();
}

WEAK void CMP1_IRQHandler(void)
{   CMP1_DriverIRQHandler();
}

WEAK void FTM0_IRQHandler(void)
{   FTM0_DriverIRQHandler();
}

WEAK void FTM1_IRQHandler(void)
{   FTM1_DriverIRQHandler();
}

WEAK void Reserved60_IRQHandler(void)
{   Reserved60_DriverIRQHandler();
}

WEAK void Reserved61_IRQHandler(void)
{   Reserved61_DriverIRQHandler();
}

WEAK void Reserved62_IRQHandler(void)
{   Reserved62_DriverIRQHandler();
}

WEAK void Reserved63_IRQHandler(void)
{   Reserved63_DriverIRQHandler();
}

WEAK void PIT0_IRQHandler(void)
{   PIT0_DriverIRQHandler();
}

WEAK void PIT1_IRQHandler(void)
{   PIT1_DriverIRQHandler();
}

WEAK void PIT2_IRQHandler(void)
{   PIT2_DriverIRQHandler();
}

WEAK void PIT3_IRQHandler(void)
{   PIT3_DriverIRQHandler();
}

WEAK void PDB0_IRQHandler(void)
{   PDB0_DriverIRQHandler();
}

WEAK void Reserved69_IRQHandler(void)
{   Reserved69_DriverIRQHandler();
}

WEAK void XBARA_IRQHandler(void)
{   XBARA_DriverIRQHandler();
}

WEAK void PDB1_IRQHandler(void)
{   PDB1_DriverIRQHandler();
}

WEAK void Reserved72_IRQHandler(void)
{   Reserved72_DriverIRQHandler();
}

WEAK void MCG_IRQHandler(void)
{   MCG_DriverIRQHandler();
}

WEAK void LPTMR0_IRQHandler(void)
{   LPTMR0_DriverIRQHandler();
}

WEAK void PORTA_IRQHandler(void)
{   PORTA_DriverIRQHandler();
}

WEAK void PORTB_IRQHandler(void)
{   PORTB_DriverIRQHandler();
}

WEAK void PORTC_IRQHandler(void)
{   PORTC_DriverIRQHandler();
}

WEAK void PORTD_IRQHandler(void)
{   PORTD_DriverIRQHandler();
}

WEAK void PORTE_IRQHandler(void)
{   PORTE_DriverIRQHandler();
}

WEAK void SWI_IRQHandler(void)
{   SWI_DriverIRQHandler();
}

WEAK void Reserved81_IRQHandler(void)
{   Reserved81_DriverIRQHandler();
}

WEAK void ENC0_COMPARE_IRQHandler(void)
{   ENC0_COMPARE_DriverIRQHandler();
}

WEAK void ENC0_HOME_IRQHandler(void)
{   ENC0_HOME_DriverIRQHandler();
}

WEAK void ENC0_WDOG_SAB_IRQHandler(void)
{   ENC0_WDOG_SAB_DriverIRQHandler();
}

WEAK void ENC0_INDEX_IRQHandler(void)
{   ENC0_INDEX_DriverIRQHandler();
}

WEAK void CMP2_IRQHandler(void)
{   CMP2_DriverIRQHandler();
}

WEAK void FTM3_IRQHandler(void)
{   FTM3_DriverIRQHandler();
}

WEAK void Reserved88_IRQHandler(void)
{   Reserved88_DriverIRQHandler();
}

WEAK void ADCB_IRQHandler(void)
{   ADCB_DriverIRQHandler();
}

WEAK void Reserved90_IRQHandler(void)
{   Reserved90_DriverIRQHandler();
}

WEAK void CAN0_ORed_Message_buffer_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void CAN0_Bus_Off_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void CAN0_Error_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void CAN0_Tx_Warning_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void CAN0_Rx_Warning_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void CAN0_Wake_Up_IRQHandler(void)
{   CAN0_DriverIRQHandler();
}

WEAK void Reserved97_IRQHandler(void)
{   Reserved97_DriverIRQHandler();
}

WEAK void Reserved98_IRQHandler(void)
{   Reserved98_DriverIRQHandler();
}

WEAK void Reserved99_IRQHandler(void)
{   Reserved99_DriverIRQHandler();
}

WEAK void Reserved100_IRQHandler(void)
{   Reserved100_DriverIRQHandler();
}

WEAK void Reserved101_IRQHandler(void)
{   Reserved101_DriverIRQHandler();
}

WEAK void Reserved102_IRQHandler(void)
{   Reserved102_DriverIRQHandler();
}

WEAK void Reserved103_IRQHandler(void)
{   Reserved103_DriverIRQHandler();
}

WEAK void Reserved104_IRQHandler(void)
{   Reserved104_DriverIRQHandler();
}

WEAK void Reserved105_IRQHandler(void)
{   Reserved105_DriverIRQHandler();
}

WEAK void Reserved106_IRQHandler(void)
{   Reserved106_DriverIRQHandler();
}

WEAK void Reserved107_IRQHandler(void)
{   Reserved107_DriverIRQHandler();
}

WEAK void CMP3_IRQHandler(void)
{   CMP3_DriverIRQHandler();
}

WEAK void Reserved109_IRQHandler(void)
{   Reserved109_DriverIRQHandler();
}

WEAK void CAN1_ORed_Message_buffer_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void CAN1_Bus_Off_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void CAN1_Error_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void CAN1_Tx_Warning_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void CAN1_Rx_Warning_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

WEAK void CAN1_Wake_Up_IRQHandler(void)
{   CAN1_DriverIRQHandler();
}

//*****************************************************************************

#if defined (DEBUG)
#pragma GCC pop_options
#endif // (DEBUG)
