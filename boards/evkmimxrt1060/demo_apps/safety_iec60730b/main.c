/*******************************************************************************
*
* Copyright 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2019 NXP
*
* SPDX-License-Identifier: NXP Proprietary
* 
*
*******************************************************************************/

#include "safety_config.h"
#include "freemaster.h"
#include "safety_test_items.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void safety_dio_runtime(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Start and end addresses for March test applied to Stack area */ 
extern const uint32_t c_ui32StackTestFirstAddress;  /* Defined in safety_cm4_cm7_imxrt.c */  
extern const uint32_t c_ui32StackTestSecondAddress; /* Defined in safety_cm4_cm7_imxrt.c */

#if defined(__IAR_SYSTEMS_ICC__)

#pragma section =  ".safety_ram"
#pragma section =  ".safety_ram_crc"

wd_test_t g_sSafetyWdTest @ ".safety_ram";
safety_common_t g_sSafetyCommon @ ".safety_ram";
flash_runtime_test_parameters_t g_sFlashCrc @ ".safety_ram";
flash_configuration_parameters_t g_sFlashConfig @ ".safety_ram";
ram_test_t g_sSafetyRamTest @ ".safety_ram";
ram_test_t g_sSafetyRamStackTest @ ".safety_ram";
clock_test_t g_sSafetyClockTest @ ".safety_ram";

/* DCP flash test */
unsigned long g_contextSwitchingBuffer[52] @ ".safety_ram_crc"; 
volatile unsigned long g_timeToFailure @ ".safety_ram_crc";
flash_dcp_state_t g_sFlashDCPState @ ".safety_ram_crc";
ram_test_t g_sSafetyRamCRCTest @ ".safety_ram_crc";
dcp_channels_t g_dcpSafetyChannel @ ".safety_ram_crc";

#endif

#if defined(__GNUC__)
#include <cr_section_macros.h>

uint32_t ui32CrcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

extern uint32_t __BOOT_STACK_ADDRESS; /* from Linker command file */
uint32_t stack_pointer_addr = (uint32_t)&__BOOT_STACK_ADDRESS;

extern uint32_t _safety_ram; /* from Linker command file */
uint32_t pui32SafetyRamSectionStart = (uint32_t)&_safety_ram;
extern uint32_t _end_safety_ram; /* from Linker command file */
uint32_t pui32SafetyRamSectionEnd = (uint32_t)&_end_safety_ram;

extern uint32_t _safety_ram_crc; /* from Linker command file */
uint32_t pui32SafetyRamCRCSectionStart = (uint32_t)&_safety_ram_crc;
extern uint32_t _end_safety_ram_crc; /* from Linker command file */
uint32_t pui32SafetyRamCRCSectionEnd = (uint32_t)&_end_safety_ram_crc;

wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

/* DCP flash test */
unsigned long g_contextSwitchingBuffer[52] __attribute__((section(".safety_ram_crc")));
volatile unsigned long g_timeToFailure __attribute__((section(".safety_ram_crc")));
flash_dcp_state_t g_sFlashDCPState __attribute__((section(".safety_ram_crc")));
ram_test_t g_sSafetyRamCRCTest __attribute__((section(".safety_ram_crc")));
dcp_channels_t g_dcpSafetyChannel __attribute__((section(".safety_ram_crc")));

#endif

#if defined(__CC_ARM)
#include "linker_config.h"

uint32_t stack_pointer_addr = (uint32_t)__BOOT_STACK_ADDRESS;

uint32_t ui32CrcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

/* DCP flash test */
unsigned long g_contextSwitchingBuffer[52] __attribute__((section(".safety_ram_crc")));
volatile unsigned long g_timeToFailure __attribute__((section(".safety_ram_crc")));
flash_dcp_state_t g_sFlashDCPState __attribute__((section(".safety_ram_crc")));
ram_test_t g_sSafetyRamCRCTest __attribute__((section(".safety_ram_crc")));
dcp_channels_t g_dcpSafetyChannel __attribute__((section(".safety_ram_crc")));

#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    __asm("CPSID i"); /* Disable interrupts */
    
    g_sSafetyCommon.ui32SafetyErrors = 0; /* Clear the variable that records safety error codes */
    g_sSafetyCommon.ui32FastIsrSafetySwitch = 0;  
    
#if defined(__GNUC__)
  ui32CrcPostbuild =*((unsigned long*)CRC_VALUE_ADDR);
  uint32_t *ui32SafetyRamStart = (uint32_t *)pui32SafetyRamSectionStart;
  uint32_t *ui32SafetyRamEnd = (uint32_t *)pui32SafetyRamSectionEnd;
  uint32_t *ui32SafetyRamCRCStart = (uint32_t *)pui32SafetyRamCRCSectionStart;
  uint32_t *ui32SafetyRamCRCEnd = (uint32_t *)pui32SafetyRamCRCSectionEnd;
#endif

#if defined(__IAR_SYSTEMS_ICC__)
  uint32_t *ui32SafetyRamStart = __section_begin(".safety_ram");
  uint32_t *ui32SafetyRamEnd = __section_end(".safety_ram"); 
  uint32_t *ui32SafetyRamCRCStart = __section_begin(".safety_ram_crc");
  uint32_t *ui32SafetyRamCRCEnd = __section_end(".safety_ram_crc");
#endif
    
#if	defined(__CC_ARM)
  ui32CrcPostbuild =*((unsigned long*)CRC_VALUE_ADDR);
  uint32_t *ui32SafetyRamStart = (uint32_t *)m_safety_ram_start;
  uint32_t *ui32SafetyRamEnd 	 = (uint32_t *)m_safety_ram_end;	
  uint32_t *ui32SafetyRamCRCStart = (uint32_t *)m_safety_ram_crc_start;
  uint32_t *ui32SafetyRamCRCEnd = (uint32_t *)m_safety_ram_crc_end;
#endif	

    /****** DCP related initialization ******/
    for(unsigned i = 0; i < 52; i++)
    {
        g_contextSwitchingBuffer[i] = 0;
    }
    g_timeToFailure = 0;
    SafetyRamTestInit(&g_sSafetyRamCRCTest, ui32SafetyRamCRCStart, ui32SafetyRamCRCEnd);
    /****** DCP related initialization ******/

    /* Memory protection unit initialization */    
    MPUSetup();
        
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif

    /* Clock initialization */
    BOARD_InitBootClocks();   
    
    /* Watchdog test */
    SafetyWatchdogTest(&g_sSafetyWdTest); 
    
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif
    
    /* Pin initialization */
    BOARD_InitBootPins();

    /* Serial initialization */
    SerialInit(APPLICATION_UART, UART_BAUD_RATE, UART_CLOCK_INPUT);
    
    /* FreeMaster init */
    FMSTR_Init();

#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif 
 
    /* Digital I/O test */   
    for (unsigned short i = 0; dio_safety_test_items[i] != 0; i++)
    {
        SafetyDigitalOutputTestRT(&g_sSafetyCommon, dio_safety_test_items[i]);
        SafetyDigitalInputOutput_ShortSupplyTestRT(&g_sSafetyCommon, dio_safety_test_items[i], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTestRT(&g_sSafetyCommon, dio_safety_test_items[i], DIO_SHORT_TO_VDD_TEST);
    }
    
    SafetyDigitalInputOutput_ShortAdjTestRT(&g_sSafetyCommon, dio_safety_test_items[0], dio_safety_test_items[1], LOGICAL_ONE); 
    SafetyDigitalInputOutput_ShortAdjTestRT(&g_sSafetyCommon, dio_safety_test_items[0], dio_safety_test_items[1], LOGICAL_ZERO);  

#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif

    /* CPU test after */
    SafetyCpuAfterResetTest(&g_sSafetyCommon);      

#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif

#ifdef PC_TEST_ON
    /* Program Counter test */
    SafetyPcTest(&g_sSafetyCommon, PC_TEST_PATTERN);
#endif

#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif
    
    /* Flash test initialization */
    DCPInit(g_contextSwitchingBuffer);    
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig, &g_sFlashDCPState);
  
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif   
 
#ifdef FLASH_TEST_ON
    /* After-reset flash test */
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig, &g_sFlashDCPState);
  
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif
#endif /* FLASH_TEST_ON */
  
    /* Ram test init for Safety related RAM space */
    SafetyRamTestInit(&g_sSafetyRamTest, ui32SafetyRamStart, ui32SafetyRamEnd);

    /* Ram after-reset test for safety related memory */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);
    
    /* Ram test init for Stack memory */
    SafetyRamTestInit(&g_sSafetyRamStackTest, (uint32_t*)c_ui32StackTestFirstAddress , (uint32_t*)c_ui32StackTestSecondAddress);
  
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif
  
    /* Ram after-reset test for Stack area */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamStackTest);
  
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif
  
    /* initialize Stack test */ 
    SafetyStackTestInit(); 
    
#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif 
   
    /* Clock test initialization */
    SafetyClockTestInit(&g_sSafetyCommon, &g_sSafetyClockTest);
    
    /* SysTick initialization */
    SystickInitialisation(SYSTICK_RELOAD_VALUE);

#ifdef WATCHDOG_ON
    Watchdog_refresh; /********************************************************/
#endif
  
    __asm("CPSIE i"); /* Enable interrupts */
  
    while (1)
    {     
        /* Interruptable CPU registers test */
        SafetyCpuBackgroundTest(&g_sSafetyCommon);
        
        /* safety test of CPU CONTROL register, it cannot be placed in interrupt, thus interrupts must be disabled for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        #if __FPU_PRESENT
            g_sSafetyCommon.IEC60730B_cpu_control_test_result = IEC60730B_CM4_CM7_CPU_ControlTest_fpu();
        #else
            g_sSafetyCommon.IEC60730B_cpu_control_test_result = IEC60730B_CM4_CM7_CPU_ControlTest();
        #endif
        __asm("CPSIE i");
        if(g_sSafetyCommon.IEC60730B_cpu_control_test_result == IEC60730B_ST_CPU_CONTROL_FAIL)
        {
            g_sSafetyCommon.ui32SafetyErrors |= CPU_CONTROL_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

        /* safety test of CPU SP_PROCESS register, it cannot be placed in interrupt, thus interrupts must be disabled for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        g_sSafetyCommon.IEC60730B_cpu_sp_process_test_result = IEC60730B_CM4_CM7_CPU_SPprocessTest();
        __asm("CPSIE i");
        if(g_sSafetyCommon.IEC60730B_cpu_sp_process_test_result == IEC60730B_ST_CPU_SP_PROCESS_FAIL)
        {
            g_sSafetyCommon.ui32SafetyErrors |= CPU_SP_PROCESS_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }
        
        /* Runtime Clock test */
        SafetyClockTestCheck(&g_sSafetyCommon, &g_sSafetyClockTest);
        
        /* Stack overflow and underflow test */
        SafetyStackTest(&g_sSafetyCommon); 

#ifdef FLASH_TEST_ON  
        /* Runtime Flash test */
        SafetyFlashRuntimeTest(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig, &g_sFlashDCPState);      
#endif

        /* Digital I/O test */
        safety_dio_runtime();
        
        /* Freemaster communication */
        FMSTR_Poll();   
    }
}

/*!
 * @brief Runtime Digital I/O tests
 *
 * @param None
 *
 * @return None
 */
void safety_dio_runtime(void)
{
    static uint8_t dio_cnt_number = 0; /*Static variable for indexin in items array*/

    if (dio_safety_test_items[dio_cnt_number] != NULL)
    {
        SafetyDigitalOutputTestRT(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number]);
        SafetyDigitalInputOutput_ShortSupplyTestRT(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTestRT(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number], DIO_SHORT_TO_VDD_TEST);

        dio_cnt_number++;
    }
    else
    {
        dio_cnt_number = 0;
    }
}

/*!
 * @brief Systick interrupt service routine
 *
 * @param void
 *
 * @return None
 */
void SYSTICK_Isr(void)
{   
    #ifdef FLASH_TEST_ON
    /* Check if SAFETY_RAM_CRC was tested in the time limit */
    g_timeToFailure++;
    if (g_timeToFailure > RAM_CHECK_TIME_THRESHOLD)
    {
        /* Error */
        while (1) {}
    }
    
    /* Check if all DCP channels are inactive */
    if ((DCP->STAT & 0xF0000) == 0)
    {
        /* Test safety_ram_crc */
        SafetyRamRuntimeTest(&g_sSafetyCommon, &g_sSafetyRamCRCTest);
        
        /* Reset time to failure */
        g_timeToFailure = 0;
    }
    #endif
    
    /* Clock test */
    SafetyClockTestIsr(&g_sSafetyClockTest);
    
    /* Safety tests which cannot be interrupted */
    SafetyIsrFunction(&g_sSafetyCommon, &g_sSafetyRamTest, &g_sSafetyRamStackTest);
    
    /* Refreshing the watchdog. For short period of interrupts, choose higher refresh ratio parameter */
    SafetyWatchdogRuntimeRefresh(&g_sSafetyWdTest);
    
    FMSTR_Recorder();
}
