/*
 * Copyright 2019 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_config.h"
#include "freemaster.h"
#include "safety_test_items.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void SYSTICK_Isr(void);
void safety_dio_runtime(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* Start and end addresses for March test applied to Stack area */ 
extern const uint32_t c_stackTestFirstAddress;  /* Defined in safety_cm7_imxrt.c */  
extern const uint32_t c_stackTestSecondAddress; /* Defined in safety_cm7_imxrt.c */

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
    uint32_t g_contextSwitchingBuffer[52] @ ".safety_ram_crc"; 
    volatile uint32_t g_timeToFailure @ ".safety_ram_crc";
    fs_flash_dcp_state_t g_sFlashDCPState @ ".safety_ram_crc";
    ram_test_t g_sSafetyRamCRCTest @ ".safety_ram_crc";
    fs_flash_dcp_channels_t g_dcpSafetyChannel @ ".safety_ram_crc";
#endif

#if defined(__GNUC__)
    #include <cr_section_macros.h>

    uint32_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    extern uint32_t __BOOT_STACK_ADDRESS; /* from Linker command file */
    uint32_t stack_pointer_addr = (uint32_t)&__BOOT_STACK_ADDRESS;

    extern uint32_t _safety_ram; /* from Linker command file */
    uint32_t safetyRamSectionStart = (uint32_t)&_safety_ram;

    extern uint32_t _end_safety_ram; /* from Linker command file */
    uint32_t safetyRamSectionEnd = (uint32_t)&_end_safety_ram;

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));
    uint32_t g_contextSwitchingBuffer[52] __attribute__((section(".safety_ram")));
    fs_flash_dcp_state_t g_sFlashDCPState __attribute__((section(".safety_ram")));
    fs_flash_dcp_channels_t g_dcpSafetyChannel __attribute__((section(".safety_ram")));
#endif

#if defined(__ARMCC_VERSION)
    #include "linker_config.h"

    uint32_t stack_pointer_addr = (uint32_t)__BOOT_STACK_ADDRESS;

    uint32_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));
    uint32_t g_contextSwitchingBuffer[52] __attribute__((section(".safety_ram")));
    fs_flash_dcp_state_t g_sFlashDCPState __attribute__((section(".safety_ram")));
    fs_flash_dcp_channels_t g_dcpSafetyChannel __attribute__((section(".safety_ram")));
#endif

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int32_t main(void)
{
    __asm("CPSID i"); /* Disable interrupts */
    
    g_sSafetyCommon.safetyErrors = 0; /* Clear the variable that records safety error codes */
    g_sSafetyCommon.fastIsrSafetySwitch = 0;  
    
#if defined(__GNUC__)
    crcPostbuild =*((uint32_t *)CRC_VALUE_ADDR);
    uint32_t *safetyRamStart = (uint32_t *)safetyRamSectionStart;
    uint32_t *safetyRamEnd = (uint32_t *)safetyRamSectionEnd;
#endif

#if defined(__IAR_SYSTEMS_ICC__)
    uint32_t *safetyRamStart = __section_begin(".safety_ram");
    uint32_t *safetyRamEnd = __section_end(".safety_ram"); 
    
    /****** DCP related initialization ******/
    for(uint16_t i = 0; i < 52; i++)
    {
        g_contextSwitchingBuffer[i] = 0;
    }
    g_timeToFailure = 0;
    uint32_t *safetyRamCRCStart = __section_begin(".safety_ram_crc");
    uint32_t *safetyRamCRCEnd = __section_end(".safety_ram_crc");
    SafetyRamTestInit(&g_sSafetyRamCRCTest, safetyRamCRCStart, safetyRamCRCEnd);    
    /****** DCP related initialization ******/
#endif
    
#if	defined(__ARMCC_VERSION)
    crcPostbuild = *((uint32_t *)CRC_VALUE_ADDR);
    uint32_t *safetyRamStart = (uint32_t *)m_safety_ram_start;
    uint32_t *safetyRamEnd 	 = (uint32_t *)m_safety_ram_end;	
#endif	

    /* Memory protection unit initialization */    
    MPUSetup();
        
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif
    
    /* Clock initialization */
    ClockInit();
  
    /* Pin initialization */
    BOARD_InitBootPins();
    
    /* Watchdog test */
    SafetyWatchdogTest(&g_sSafetyWdTest); 
    
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif

#if DIO_TEST_ENABLED
    /* Digital I/O test */    
    for (uint16_t i = 0; dio_safety_test_items[i] != 0; i++)
    {
        SafetyDigitalOutputTest(&g_sSafetyCommon, dio_safety_test_items[i]);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, dio_safety_test_items[i], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, dio_safety_test_items[i], DIO_SHORT_TO_VDD_TEST);
    }
    
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0], dio_safety_test_items[1], LOGICAL_ONE); 
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0], dio_safety_test_items[1], LOGICAL_ZERO);  
#endif /* DIO_TEST_ENABLED */

    SerialInit(APPLICATION_SERIAL_BASE, SERIAL_BAUD_RATE, SERIAL_CLOCK);
    
    /* FreeMaster init */
    FMSTR_Init();

#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif

    /* CPU test after */
    SafetyCpuAfterResetTest(&g_sSafetyCommon);      

#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif

#if PC_TEST_ENABLED
    /* Program Counter test */
    SafetyPcTest(&g_sSafetyCommon, PC_TEST_PATTERN);
#endif

#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif
    
    /* Flash test initialization */
    DCPInit(g_contextSwitchingBuffer);    
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig, &g_sFlashDCPState);
  
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif   
 
#if FLASH_TEST_ENABLED
    /* After-reset flash test */
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig, &g_sFlashDCPState);
  
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif
#endif /* FLASH_TEST_ENABLED */
  
    /* Ram test init for Safety related RAM space */
    SafetyRamTestInit(&g_sSafetyRamTest, safetyRamStart, safetyRamEnd);

    /* Ram after-reset test for safety related memory */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);
    
    /* Ram test init for Stack memory */
    SafetyRamTestInit(&g_sSafetyRamStackTest, (uint32_t*)c_stackTestFirstAddress , (uint32_t*)c_stackTestSecondAddress);
  
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif
  
    /* Ram after-reset test for Stack area */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamStackTest);
  
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif
  
    /* initialize Stack test */ 
    SafetyStackTestInit(); 
    
    /* Stack overflow and underflow test */
    SafetyStackTest(&g_sSafetyCommon); 
    
#if WATCHDOG_ENABLED
  Watchdog_refresh; /********************************************************/
#endif 
   
#if CLOCK_TEST_ENABLED
    /* Clock test initialization */
    SafetyClockTestInit(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif

    /* SysTick initialization */
    SystickInitialisation(SYSTICK_RELOAD_VALUE);

#if WATCHDOG_ENABLED
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
            g_sSafetyCommon.CPU_control_test_result = FS_CM4_CM7_CPU_ControlFpu();
        #else
            g_sSafetyCommon.CPU_control_test_result = FS_CM4_CM7_CPU_Control();
        #endif
        __asm("CPSIE i");
        if(g_sSafetyCommon.CPU_control_test_result == FS_CPU_CONTROL_FAIL)
        {
            g_sSafetyCommon.safetyErrors |= CPU_CONTROL_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

        /* safety test of CPU SP_PROCESS register, it cannot be placed in interrupt, thus interrupts must be disabled for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        g_sSafetyCommon.CPU_sp_process_test_result = FS_CM4_CM7_CPU_SPprocess();
        __asm("CPSIE i");
        if(g_sSafetyCommon.CPU_sp_process_test_result == FS_CPU_SP_PROCESS_FAIL)
        {
            g_sSafetyCommon.safetyErrors |= CPU_SP_PROCESS_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

#if CLOCK_TEST_ENABLED
        /* Runtime Clock test */
        SafetyClockTestCheck(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif

        /* Stack overflow and underflow test */
        SafetyStackTest(&g_sSafetyCommon); 

#if FLASH_TEST_ENABLED
        /* Runtime Flash test */
        SafetyFlashRuntimeTest(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig, &g_sFlashDCPState);      
#endif

#if DIO_TEST_ENABLED
        /* Digital I/O test */
        safety_dio_runtime();
#endif

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
        SafetyDigitalOutputTest(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number]);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number], DIO_SHORT_TO_VDD_TEST);

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
#if (defined(NDEBUG) && defined(__IAR_SYSTEMS_ICC__))
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

#if CLOCK_TEST_ENABLED
    /* Clock test */
    SafetyClockTestIsr(&g_sSafetyClockTest);
#endif

    /* Safety tests which cannot be interrupted */
    SafetyIsrFunction(&g_sSafetyCommon, &g_sSafetyRamTest, &g_sSafetyRamStackTest);
    
    /* Refreshing the watchdog. For short period of interrupts, choose higher refresh ratio parameter */
    SafetyWatchdogRuntimeRefresh(&g_sSafetyWdTest);
}
