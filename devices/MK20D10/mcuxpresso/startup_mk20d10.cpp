//*****************************************************************************
// MK20D10 startup code for use with MCUXpresso IDE
//
// Version : 050117
//*****************************************************************************
//
// Copyright(C) NXP Semiconductors, 2017
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// o Redistributions of source code must retain the above copyright notice, this list
//   of conditions and the following disclaimer.
//
// o Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or
//   other materials provided with the distribution.
//
// o Neither the name of copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
// automatically take precedence over these weak definitions
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
WEAK void FTFL_IRQHandler(void);
WEAK void Read_Collision_IRQHandler(void);
WEAK void LVD_LVW_IRQHandler(void);
WEAK void LLWU_IRQHandler(void);
WEAK void WDOG_EWM_IRQHandler(void);
WEAK void Reserved39_IRQHandler(void);
WEAK void I2C0_IRQHandler(void);
WEAK void I2C1_IRQHandler(void);
WEAK void SPI0_IRQHandler(void);
WEAK void SPI1_IRQHandler(void);
WEAK void SPI2_IRQHandler(void);
WEAK void CAN0_ORed_Message_buffer_IRQHandler(void);
WEAK void CAN0_Bus_Off_IRQHandler(void);
WEAK void CAN0_Error_IRQHandler(void);
WEAK void CAN0_Tx_Warning_IRQHandler(void);
WEAK void CAN0_Rx_Warning_IRQHandler(void);
WEAK void CAN0_Wake_Up_IRQHandler(void);
WEAK void I2S0_Tx_IRQHandler(void);
WEAK void I2S0_Rx_IRQHandler(void);
WEAK void CAN1_ORed_Message_buffer_IRQHandler(void);
WEAK void CAN1_Bus_Off_IRQHandler(void);
WEAK void CAN1_Error_IRQHandler(void);
WEAK void CAN1_Tx_Warning_IRQHandler(void);
WEAK void CAN1_Rx_Warning_IRQHandler(void);
WEAK void CAN1_Wake_Up_IRQHandler(void);
WEAK void Reserved59_IRQHandler(void);
WEAK void UART0_LON_IRQHandler(void);
WEAK void UART0_RX_TX_IRQHandler(void);
WEAK void UART0_ERR_IRQHandler(void);
WEAK void UART1_RX_TX_IRQHandler(void);
WEAK void UART1_ERR_IRQHandler(void);
WEAK void UART2_RX_TX_IRQHandler(void);
WEAK void UART2_ERR_IRQHandler(void);
WEAK void UART3_RX_TX_IRQHandler(void);
WEAK void UART3_ERR_IRQHandler(void);
WEAK void UART4_RX_TX_IRQHandler(void);
WEAK void UART4_ERR_IRQHandler(void);
WEAK void UART5_RX_TX_IRQHandler(void);
WEAK void UART5_ERR_IRQHandler(void);
WEAK void ADC0_IRQHandler(void);
WEAK void ADC1_IRQHandler(void);
WEAK void CMP0_IRQHandler(void);
WEAK void CMP1_IRQHandler(void);
WEAK void CMP2_IRQHandler(void);
WEAK void FTM0_IRQHandler(void);
WEAK void FTM1_IRQHandler(void);
WEAK void FTM2_IRQHandler(void);
WEAK void CMT_IRQHandler(void);
WEAK void RTC_IRQHandler(void);
WEAK void RTC_Seconds_IRQHandler(void);
WEAK void PIT0_IRQHandler(void);
WEAK void PIT1_IRQHandler(void);
WEAK void PIT2_IRQHandler(void);
WEAK void PIT3_IRQHandler(void);
WEAK void PDB0_IRQHandler(void);
WEAK void USB0_IRQHandler(void);
WEAK void USBDCD_IRQHandler(void);
WEAK void Reserved91_IRQHandler(void);
WEAK void Reserved92_IRQHandler(void);
WEAK void Reserved93_IRQHandler(void);
WEAK void Reserved94_IRQHandler(void);
WEAK void Reserved95_IRQHandler(void);
WEAK void SDHC_IRQHandler(void);
WEAK void DAC0_IRQHandler(void);
WEAK void DAC1_IRQHandler(void);
WEAK void TSI0_IRQHandler(void);
WEAK void MCG_IRQHandler(void);
WEAK void LPTMR0_IRQHandler(void);
WEAK void Reserved102_IRQHandler(void);
WEAK void PORTA_IRQHandler(void);
WEAK void PORTB_IRQHandler(void);
WEAK void PORTC_IRQHandler(void);
WEAK void PORTD_IRQHandler(void);
WEAK void PORTE_IRQHandler(void);
WEAK void Reserved108_IRQHandler(void);
WEAK void Reserved109_IRQHandler(void);
WEAK void SWI_IRQHandler(void);

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
void FTFL_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Read_Collision_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LVD_LVW_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LLWU_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void WDOG_EWM_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved39_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2C0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2C1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SPI2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2S0_Tx_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void I2S0_Rx_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CAN1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved59_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_LON_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART0_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART1_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART2_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART2_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART3_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART3_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART4_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART4_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART5_RX_TX_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void UART5_ERR_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void ADC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMP2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void FTM2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void CMT_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void RTC_Seconds_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT2_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PIT3_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PDB0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USB0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void USBDCD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved91_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved92_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved93_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved94_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved95_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SDHC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void DAC1_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void TSI0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void MCG_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void LPTMR0_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved102_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTA_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTB_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTC_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTD_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void PORTE_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved108_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void Reserved109_DriverIRQHandler(void) ALIAS(IntDefaultHandler);
void SWI_DriverIRQHandler(void) ALIAS(IntDefaultHandler);

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

    // Chip Level - MK20D10
    DMA0_IRQHandler,                      // 16 : DMA channel 0 transfer complete
    DMA1_IRQHandler,                      // 17 : DMA channel 1 transfer complete
    DMA2_IRQHandler,                      // 18 : DMA channel 2 transfer complete
    DMA3_IRQHandler,                      // 19 : DMA channel 3 transfer complete
    DMA4_IRQHandler,                      // 20 : DMA channel 4 transfer complete
    DMA5_IRQHandler,                      // 21 : DMA channel 5 transfer complete
    DMA6_IRQHandler,                      // 22 : DMA channel 6 transfer complete
    DMA7_IRQHandler,                      // 23 : DMA channel 7 transfer complete
    DMA8_IRQHandler,                      // 24 : DMA channel 8 transfer complete
    DMA9_IRQHandler,                      // 25 : DMA channel 9 transfer complete
    DMA10_IRQHandler,                     // 26 : DMA channel 10 transfer complete
    DMA11_IRQHandler,                     // 27 : DMA channel 11 transfer complete
    DMA12_IRQHandler,                     // 28 : DMA channel 12 transfer complete
    DMA13_IRQHandler,                     // 29 : DMA channel 13 transfer complete
    DMA14_IRQHandler,                     // 30 : DMA channel 14 transfer complete
    DMA15_IRQHandler,                     // 31 : DMA channel 15 transfer complete
    DMA_Error_IRQHandler,                 // 32 : DMA channel 0 - 15 error
    MCM_IRQHandler,                       // 33 : MCM normal interrupt
    FTFL_IRQHandler,                      // 34 : FTFL command complete
    Read_Collision_IRQHandler,            // 35 : FTFL read collision
    LVD_LVW_IRQHandler,                   // 36 : PMC controller low-voltage detect, low-voltage warning
    LLWU_IRQHandler,                      // 37 : Low leakage wakeup
    WDOG_EWM_IRQHandler,                  // 38 : Single interrupt vector for  WDOG and EWM
    Reserved39_IRQHandler,                // 39 : Reserved interrupt
    I2C0_IRQHandler,                      // 40 : Inter-integrated circuit 0
    I2C1_IRQHandler,                      // 41 : Inter-integrated circuit 1
    SPI0_IRQHandler,                      // 42 : Serial peripheral Interface 0
    SPI1_IRQHandler,                      // 43 : Serial peripheral Interface 1
    SPI2_IRQHandler,                      // 44 : Serial peripheral Interface 1
    CAN0_ORed_Message_buffer_IRQHandler,  // 45 : CAN0 ORed message buffers
    CAN0_Bus_Off_IRQHandler,              // 46 : CAN0 bus off
    CAN0_Error_IRQHandler,                // 47 : CAN0 error
    CAN0_Tx_Warning_IRQHandler,           // 48 : CAN0 Tx warning
    CAN0_Rx_Warning_IRQHandler,           // 49 : CAN0 Rx warning
    CAN0_Wake_Up_IRQHandler,              // 50 : CAN0 wake up
    I2S0_Tx_IRQHandler,                   // 51 : Integrated interchip sound 0 transmit interrupt
    I2S0_Rx_IRQHandler,                   // 52 : Integrated interchip sound 0 receive interrupt
    CAN1_ORed_Message_buffer_IRQHandler,  // 53 : CAN1 OR'd message buffers interrupt
    CAN1_Bus_Off_IRQHandler,              // 54 : CAN1 bus off interrupt
    CAN1_Error_IRQHandler,                // 55 : CAN1 error interrupt
    CAN1_Tx_Warning_IRQHandler,           // 56 : CAN1 Tx warning interrupt
    CAN1_Rx_Warning_IRQHandler,           // 57 : CAN1 Rx warning interrupt
    CAN1_Wake_Up_IRQHandler,              // 58 : CAN1 wake up interrupt
    Reserved59_IRQHandler,                // 59 : Reserved interrupt
    UART0_LON_IRQHandler,                 // 60 : UART0 LON
    UART0_RX_TX_IRQHandler,               // 61 : UART0 receive/transmit interrupt
    UART0_ERR_IRQHandler,                 // 62 : UART0 error interrupt
    UART1_RX_TX_IRQHandler,               // 63 : UART1 receive/transmit interrupt
    UART1_ERR_IRQHandler,                 // 64 : UART1 error interrupt
    UART2_RX_TX_IRQHandler,               // 65 : UART2 receive/transmit interrupt
    UART2_ERR_IRQHandler,                 // 66 : UART2 error interrupt
    UART3_RX_TX_IRQHandler,               // 67 : UART3 receive/transmit interrupt
    UART3_ERR_IRQHandler,                 // 68 : UART3 error interrupt
    UART4_RX_TX_IRQHandler,               // 69 : UART4 receive/transmit interrupt
    UART4_ERR_IRQHandler,                 // 70 : UART4 error interrupt
    UART5_RX_TX_IRQHandler,               // 71 : UART5 receive/transmit interrupt
    UART5_ERR_IRQHandler,                 // 72 : UART5 error interrupt
    ADC0_IRQHandler,                      // 73 : Analog-to-digital converter 0
    ADC1_IRQHandler,                      // 74 : Analog-to-digital converter 1
    CMP0_IRQHandler,                      // 75 : Comparator 0
    CMP1_IRQHandler,                      // 76 : Comparator 1
    CMP2_IRQHandler,                      // 77 : Comparator 2
    FTM0_IRQHandler,                      // 78 : FlexTimer module 0 fault, overflow and channels interrupt
    FTM1_IRQHandler,                      // 79 : FlexTimer module 1 fault, overflow and channels interrupt
    FTM2_IRQHandler,                      // 80 : FlexTimer module 2 fault, overflow and channels interrupt
    CMT_IRQHandler,                       // 81 : Carrier modulator transmitter
    RTC_IRQHandler,                       // 82 : Real time clock
    RTC_Seconds_IRQHandler,               // 83 : Real time clock seconds
    PIT0_IRQHandler,                      // 84 : Periodic interrupt timer channel 0
    PIT1_IRQHandler,                      // 85 : Periodic interrupt timer channel 1
    PIT2_IRQHandler,                      // 86 : Periodic interrupt timer channel 2
    PIT3_IRQHandler,                      // 87 : Periodic interrupt timer channel 3
    PDB0_IRQHandler,                      // 88 : Programmable delay block
    USB0_IRQHandler,                      // 89 : USB OTG interrupt
    USBDCD_IRQHandler,                    // 90 : USB charger detect
    Reserved91_IRQHandler,                // 91 : Reserved interrupt
    Reserved92_IRQHandler,                // 92 : Reserved interrupt
    Reserved93_IRQHandler,                // 93 : Reserved interrupt
    Reserved94_IRQHandler,                // 94 : Reserved interrupt
    Reserved95_IRQHandler,                // 95 : Reserved interrupt
    SDHC_IRQHandler,                      // 96 : Secured digital host controller
    DAC0_IRQHandler,                      // 97 : Digital-to-analog converter 0
    DAC1_IRQHandler,                      // 98 : Digital-to-analog converter 1
    TSI0_IRQHandler,                      // 99 : TSI0 Interrupt
    MCG_IRQHandler,                       // 100: Multipurpose clock generator
    LPTMR0_IRQHandler,                    // 101: Low power timer interrupt
    Reserved102_IRQHandler,               // 102: Reserved interrupt
    PORTA_IRQHandler,                     // 103: Port A interrupt
    PORTB_IRQHandler,                     // 104: Port B interrupt
    PORTC_IRQHandler,                     // 105: Port C interrupt
    PORTD_IRQHandler,                     // 106: Port D interrupt
    PORTE_IRQHandler,                     // 107: Port E interrupt
    Reserved108_IRQHandler,               // 108: Reserved interrupt
    Reserved109_IRQHandler,               // 109: Reserved interrupt
    SWI_IRQHandler,                       // 110: Software interrupt
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

WEAK void FTFL_IRQHandler(void)
{   FTFL_DriverIRQHandler();
}

WEAK void Read_Collision_IRQHandler(void)
{   Read_Collision_DriverIRQHandler();
}

WEAK void LVD_LVW_IRQHandler(void)
{   LVD_LVW_DriverIRQHandler();
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

WEAK void I2C1_IRQHandler(void)
{   I2C1_DriverIRQHandler();
}

WEAK void SPI0_IRQHandler(void)
{   SPI0_DriverIRQHandler();
}

WEAK void SPI1_IRQHandler(void)
{   SPI1_DriverIRQHandler();
}

WEAK void SPI2_IRQHandler(void)
{   SPI2_DriverIRQHandler();
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

WEAK void I2S0_Tx_IRQHandler(void)
{   I2S0_Tx_DriverIRQHandler();
}

WEAK void I2S0_Rx_IRQHandler(void)
{   I2S0_Rx_DriverIRQHandler();
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

WEAK void Reserved59_IRQHandler(void)
{   Reserved59_DriverIRQHandler();
}

WEAK void UART0_LON_IRQHandler(void)
{   UART0_LON_DriverIRQHandler();
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

WEAK void UART2_RX_TX_IRQHandler(void)
{   UART2_RX_TX_DriverIRQHandler();
}

WEAK void UART2_ERR_IRQHandler(void)
{   UART2_ERR_DriverIRQHandler();
}

WEAK void UART3_RX_TX_IRQHandler(void)
{   UART3_RX_TX_DriverIRQHandler();
}

WEAK void UART3_ERR_IRQHandler(void)
{   UART3_ERR_DriverIRQHandler();
}

WEAK void UART4_RX_TX_IRQHandler(void)
{   UART4_RX_TX_DriverIRQHandler();
}

WEAK void UART4_ERR_IRQHandler(void)
{   UART4_ERR_DriverIRQHandler();
}

WEAK void UART5_RX_TX_IRQHandler(void)
{   UART5_RX_TX_DriverIRQHandler();
}

WEAK void UART5_ERR_IRQHandler(void)
{   UART5_ERR_DriverIRQHandler();
}

WEAK void ADC0_IRQHandler(void)
{   ADC0_DriverIRQHandler();
}

WEAK void ADC1_IRQHandler(void)
{   ADC1_DriverIRQHandler();
}

WEAK void CMP0_IRQHandler(void)
{   CMP0_DriverIRQHandler();
}

WEAK void CMP1_IRQHandler(void)
{   CMP1_DriverIRQHandler();
}

WEAK void CMP2_IRQHandler(void)
{   CMP2_DriverIRQHandler();
}

WEAK void FTM0_IRQHandler(void)
{   FTM0_DriverIRQHandler();
}

WEAK void FTM1_IRQHandler(void)
{   FTM1_DriverIRQHandler();
}

WEAK void FTM2_IRQHandler(void)
{   FTM2_DriverIRQHandler();
}

WEAK void CMT_IRQHandler(void)
{   CMT_DriverIRQHandler();
}

WEAK void RTC_IRQHandler(void)
{   RTC_DriverIRQHandler();
}

WEAK void RTC_Seconds_IRQHandler(void)
{   RTC_Seconds_DriverIRQHandler();
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

WEAK void USB0_IRQHandler(void)
{   USB0_DriverIRQHandler();
}

WEAK void USBDCD_IRQHandler(void)
{   USBDCD_DriverIRQHandler();
}

WEAK void Reserved91_IRQHandler(void)
{   Reserved91_DriverIRQHandler();
}

WEAK void Reserved92_IRQHandler(void)
{   Reserved92_DriverIRQHandler();
}

WEAK void Reserved93_IRQHandler(void)
{   Reserved93_DriverIRQHandler();
}

WEAK void Reserved94_IRQHandler(void)
{   Reserved94_DriverIRQHandler();
}

WEAK void Reserved95_IRQHandler(void)
{   Reserved95_DriverIRQHandler();
}

WEAK void SDHC_IRQHandler(void)
{   SDHC_DriverIRQHandler();
}

WEAK void DAC0_IRQHandler(void)
{   DAC0_DriverIRQHandler();
}

WEAK void DAC1_IRQHandler(void)
{   DAC1_DriverIRQHandler();
}

WEAK void TSI0_IRQHandler(void)
{   TSI0_DriverIRQHandler();
}

WEAK void MCG_IRQHandler(void)
{   MCG_DriverIRQHandler();
}

WEAK void LPTMR0_IRQHandler(void)
{   LPTMR0_DriverIRQHandler();
}

WEAK void Reserved102_IRQHandler(void)
{   Reserved102_DriverIRQHandler();
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

WEAK void Reserved108_IRQHandler(void)
{   Reserved108_DriverIRQHandler();
}

WEAK void Reserved109_IRQHandler(void)
{   Reserved109_DriverIRQHandler();
}

WEAK void SWI_IRQHandler(void)
{   SWI_DriverIRQHandler();
}

//*****************************************************************************

#if defined (DEBUG)
#pragma GCC pop_options
#endif // (DEBUG)
