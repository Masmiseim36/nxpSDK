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
extern const uint32_t c_stackTestFirstAddress;  /* defined in safety_*.c */
extern const uint32_t c_stackTestSecondAddress; /* defined in safety_*.c */

/* Test variable */
volatile uint32_t counter = 0;

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #pragma section =  ".safety_ram"
    #pragma section = ".pctest"

    safety_common_t g_sSafetyCommon @ ".safety_ram";
    wd_test_t g_sSafetyWdTest @ ".safety_ram";
    flash_runtime_test_parameters_t g_sFlashCrc @ ".safety_ram";
    flash_configuration_parameters_t g_sFlashConfig @ ".safety_ram";
    ram_test_t g_sSafetyRamTest @ ".safety_ram";
    ram_test_t g_sSafetyRamStackTest @ ".safety_ram";
    clock_test_t g_sSafetyClockTest @ ".safety_ram";

#elif (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #include "linker_config.h"

    uint32_t stack_pointer_addr = (uint32_t)__BOOT_STACK_ADDRESS;

    uint16_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

#else /* MCUXpresso */
    #include <cr_section_macros.h>

    uint16_t crcPostbuild; /* Checksum result calculated by srec_cat.exe in post-build phase */

    extern uint32_t __BOOT_STACK_ADDRESS; /* from Linker command file */
    uint32_t stack_pointer_addr = (uint32_t)&__BOOT_STACK_ADDRESS;

    extern uint32_t _safety_ram; /* from Linker command file */
    uint32_t psafetyRamSectionStart = (uint32_t)&_safety_ram;

    extern uint32_t _end_safety_ram; /* from Linker command file */
    uint32_t psafetyRamSectionEnd = (uint32_t)&_end_safety_ram;

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));
#endif

/*******************************************************************************
* Code
******************************************************************************/
/*!
* @brief main function
*
* @param void
*
* @return None
*/
int32_t main(void)
{
    /* Clock initialization */
    ClockInit();

    /* Watchdog test */
    SafetyWatchdogTest(&g_sSafetyCommon, &g_sSafetyWdTest);

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    uint32_t *safetyRamStart = __section_begin(".safety_ram");
    uint32_t *safetyRamEnd = __section_end(".safety_ram");

#elif (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    crcPostbuild = *((uint16_t *)CRC_VALUE_ADDR);
    uint32_t *safetyRamStart = (uint32_t *)m_safety_ram_start;
    uint32_t *safetyRamEnd   = (uint32_t *)m_safety_ram_end;

#else /* MCUXpresso */
    crcPostbuild = *((uint16_t *)CRC_VALUE_ADDR); 
    uint32_t *safetyRamStart = (uint32_t *)psafetyRamSectionStart;
    uint32_t *safetyRamEnd = (uint32_t *)psafetyRamSectionEnd;
#endif

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

#if FMSTR_SERIAL_ENABLE    
    SerialInit();

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
  
    FMSTR_Init();       /* initialize freemaster */
#endif
    
    g_sSafetyCommon.safetyErrors = 0;    /* clear the variable that records safety error codes */
    g_sSafetyCommon.fastIsrSafetySwitch = 0;

    /* Flash test init */
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
    
#if FLASH_TEST_ENABLED
    /* After-reset flash test */
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig);
    
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
#endif /* FLASH_TEST_ENABLED */

    /* Ram test init for Safety related RAM space */
    SafetyRamTestInit(&g_sSafetyRamTest, safetyRamStart, safetyRamEnd);

    /* Ram test init for Stack memory */
    SafetyRamTestInit(&g_sSafetyRamStackTest, (uint32_t *)c_stackTestFirstAddress, (uint32_t *)c_stackTestSecondAddress);

    /* Ram after-reset test for safety related memory*/
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    /* Ram after-reset test for Stack area */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamStackTest);
  
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
  
#if PC_TEST_ENABLED
    /* Program Counter test */
    SafetyPcTest(&g_sSafetyCommon, PC_TEST_PATTERN);
#endif

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
  
    /* CPU test after */
    SafetyCpuAfterResetTest(&g_sSafetyCommon);
  
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
  
    /* initialize Stack test */
    SafetyStackTestInit();

    /* Stack overflow and underflow test */
    SafetyStackTest(&g_sSafetyCommon);
  
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
  
#if ADC_TEST_ENABLED      
    AdcInit();
    
    #if WATCHDOG_ENABLED
      Watchdog_refresh; /* refreshing the watchdog */
    #endif
    
    /* initialize Analog test */
    SafetyAnalogTestInitialization();

    /* After-reset ADC test */
    for(uint8_t i = 0; i < 4; i++)     /* first iteration is init phase */
    {
        for(uint8_t y = 0; y < 40; y++) __asm("nop"); /* delay because of conversion time */
        SafetyAnalogTest(&g_sSafetyCommon);
        
        #if WATCHDOG_ENABLED
          Watchdog_refresh; /* refreshing the watchdog */
        #endif
    }
#endif /* ADC_TEST_ENABLED */
    
#ifdef _MKE15Z7_H_ 
#if TSI_TEST_ENABLED
    volatile uint32_t tsi_cnt_number = 0; /* Variables for runtime TSI test */

    /* Enable TSI peripheral clock */
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(0));
    PCC->CLKCFG[PCC_TSI0_INDEX] = (PCC_CLKCFG_CGC(TRUE) | PCC_CLKCFG_PCS(1));  /* System Oscillator Bus Clock */

    tsi_port_clock_enable(); /* This function enable clock to correspond  PORT */

    /* Put ALL items to INIT state */
    for (tsi_cnt_number = 0; tsi_safety_test_items[tsi_cnt_number] != NULL; tsi_cnt_number++)
    {
        FS_TSI_InputInit(tsi_safety_test_items[tsi_cnt_number]);
    }

    /* After-reset TSI test */
    for (tsi_cnt_number = 0; tsi_safety_test_items[tsi_cnt_number] != NULL ; tsi_cnt_number++)
    {
        while (FS_TSI_PASS_STIM != tsi_safety_test_items[tsi_cnt_number]->state)
        {
            SafetyTsiChanelTest(&g_sSafetyCommon, tsi_safety_test_items[tsi_cnt_number]);   
        }
        
        FS_TSI_InputInit(tsi_safety_test_items[tsi_cnt_number]); /* SET back to INIT state for next runtime check */
      
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif   
    
    }
    tsi_cnt_number = 0; /* Reset to 0 for RUNTIME test cnt */
#endif /* TSI_TEST_ENABLED */
#endif /* _MKE15Z7_H_ */

#if DIO_TEST_ENABLED
    /* Digital I/O test */
    SafetyDIOTestInit(&g_sSafetyCommon, dio_safety_test_items);
   
  #ifdef _MKE02Z4_H_
    SafetyDigitalOutputTest(&g_sSafetyCommon,dio_safety_test_items[0]);
    SafetyDigitalInputTest(&g_sSafetyCommon,dio_safety_test_items[1]);
  #else
  
    for (int i=0; dio_safety_test_items[i]!= 0; i++)
    {
        SafetyDigitalOutputTest(&g_sSafetyCommon,dio_safety_test_items[i]);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon,dio_safety_test_items[i], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon,dio_safety_test_items[i], DIO_SHORT_TO_VDD_TEST);
    }
   
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0],dio_safety_test_items[1], LOGICAL_ONE); 
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0],dio_safety_test_items[1], LOGICAL_ZERO);  
   
  #ifdef _MKE15Z7_H_ 	 
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[2],dio_safety_test_items[3], LOGICAL_ZERO);  
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[6],dio_safety_test_items[7], LOGICAL_ONE);  
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[8],dio_safety_test_items[9], LOGICAL_ZERO);  
  #endif  

  #endif /* _MKE02Z4_H_ */
#endif /* DIO_TEST_ENABLED */
   
#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif
   
#if CLOCK_TEST_ENABLED
    /* Initialize Clock test */
    SafetyClockTestInit(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif
    
    /* Initialize SysTick */
    SystickInit(SYSTICK_RELOAD_VALUE);
    
    /* Enable interrupts */
    __asm("CPSIE i");
  
    while(1)
    {
        /* Interruptable CPU registers test */
        SafetyCpuBackgroundTest(&g_sSafetyCommon);

        /* safety test of CPU CONTROL register, it cannot be placed in interrupt, thus interrupts must be disabled for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        g_sSafetyCommon.CPU_control_test_result = FS_CM0_CPU_Control();
        __asm("CPSIE i");
        if(g_sSafetyCommon.CPU_control_test_result == FS_FAIL_CPU_CONTROL)
        {
            g_sSafetyCommon.safetyErrors |= CPU_CONTROL_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

        /* safety test of CPU SP_PROCESS register, it cannot be placed in interrupt, thus interrupts must be disabled for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        FS_CM0_CPU_SPprocess();
        __asm("CPSIE i");
   
#if FLASH_TEST_ENABLED
        /* Runtime Flash test */
        SafetyFlashRuntimeTest(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig);
#endif
    
#if CLOCK_TEST_ENABLED
        /* Runtime Clock test */
        SafetyClockTestCheck(&g_sSafetyCommon, &g_sSafetyClockTest);
#endif
        
        /* Stack overflow and underflow test */
        SafetyStackTest(&g_sSafetyCommon);
           
#if ADC_TEST_ENABLED
        /* Runtime ADC test */
        SafetyAnalogTest(&g_sSafetyCommon);
#endif

#if TSI_TEST_ENABLED
  #ifdef _MKE15Z7_H_ 
        
        /* Runtime TSI test */
        if (FS_TSI_PASS_STIM != tsi_safety_test_items[tsi_cnt_number]->state)
        {
            SafetyTsiChanelTest(&g_sSafetyCommon, tsi_safety_test_items[tsi_cnt_number]);   
        }
        else if (tsi_safety_test_items[tsi_cnt_number + 1 ] != NULL)
        {
            FS_TSI_InputInit(tsi_safety_test_items[tsi_cnt_number]);
            tsi_cnt_number++;
            
            /* DIO TEST */
            /* Because DIO can Use the SAME port as TSI, we must call this test only when TSI not RUN! */
            safety_dio_runtime();
        }
        else
        {
            tsi_cnt_number = 0;
        }
  #endif /* _MKE15Z7_H_ */  
#else /* TSI_TEST_ENABLED */

#if DIO_TEST_ENABLED

        /* Digital I/O test */
        safety_dio_runtime();
        
#endif /* DIO_TEST_ENABLED */
#endif /* TSI_TEST_ENABLED */

#if FMSTR_SERIAL_ENABLE 
        
       FMSTR_Poll();  /* Freemaster cummunication */
#endif
    }
}

void safety_dio_runtime(void)
{
#ifdef _MKE02Z4_H_
     SafetyDigitalOutputTest(&g_sSafetyCommon,dio_safety_test_items[0]);
     SafetyDigitalInputTest(&g_sSafetyCommon,dio_safety_test_items[1]);
#else
     
    /* Static variable for indexing in items array */
    static uint8_t dio_cnt_number = 0; 

    if (dio_safety_test_items[dio_cnt_number] != NULL)
    {
        SafetyDigitalOutputTest(&g_sSafetyCommon,dio_safety_test_items[dio_cnt_number]);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon,dio_safety_test_items[dio_cnt_number], DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon,dio_safety_test_items[dio_cnt_number], DIO_SHORT_TO_VDD_TEST);
        
        /* In next call, test next DIO channel */
        dio_cnt_number++; 
    }
    else
    {
        dio_cnt_number = 0;
    }
#endif
  
}

/*!
* @brief Systick interrupt function
*
* @param void
*
* @return None
*/
void SYSTICK_Isr(void)
{
    counter++;

#if CLOCK_TEST_ENABLED
    /* Clock test function */
    SafetyClockTestIsr(&g_sSafetyClockTest);
#endif
    
    /* Safety tests which cannot be interrupted */
    SafetyIsrFunction(&g_sSafetyCommon, &g_sSafetyRamTest, &g_sSafetyRamStackTest);

    /* Refreshing the watchdog. For short period of interrupts, choose higher refresh ratio parameter */
    SafetyWatchdogRuntimeRefresh(&g_sSafetyWdTest);
}

