/*
 * Copyright 2021 NXP.
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

/* Test variable */
volatile uint32_t counter = 0;

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    #pragma section =  ".safety_ram"
#if defined(DCP_BASE)    
    #pragma section =  ".safety_ram_crc"
#endif

    wd_test_t g_sSafetyWdTest @ ".safety_ram";
    safety_common_t g_sSafetyCommon @ ".safety_ram";
    flash_runtime_test_parameters_t g_sFlashCrc @ ".safety_ram";
    flash_configuration_parameters_t g_sFlashConfig @ ".safety_ram";
    ram_test_t g_sSafetyRamTest @ ".safety_ram";
    ram_test_t g_sSafetyRamStackTest @ ".safety_ram";
    clock_test_t g_sSafetyClockTest @ ".safety_ram";

/* DCP flash test */
#if defined(DCP_BASE)
    uint32_t g_contextSwitchingBuffer[52] @ ".safety_ram_crc"; 
    volatile uint32_t g_flashTestTTF @ ".safety_ram_crc";
    fs_flash_dcp_state_t g_sFlashDCPState @ ".safety_ram_crc";
    fs_flash_dcp_channels_t g_dcpSafetyChannel @ ".safety_ram_crc";
    ram_test_t g_sSafetyRamCRCTest @ ".safety_ram_crc";
#endif

#elif (defined(__GNUC__) && ( __ARMCC_VERSION >= 6010050)) /* KEIL */
    #include "linker_config.h"

    uint32_t stack_pointer_addr = (uint32_t)__BOOT_STACK_ADDRESS;

    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

#if defined(DCP_BASE) 
    /* DCP flash test */
    uint32_t g_contextSwitchingBuffer[52] __attribute__((section(".safety_ram_crc")));
    volatile uint32_t g_flashTestTTF __attribute__((section(".safety_ram_crc")));
    fs_flash_dcp_state_t g_sFlashDCPState __attribute__((section(".safety_ram_crc")));
    fs_flash_dcp_channels_t g_dcpSafetyChannel __attribute__((section(".safety_ram_crc")));
    ram_test_t g_sSafetyRamCRCTest __attribute__((section(".safety_ram_crc")));
#endif

#else /* MCUXpresso */
//    #include <cr_section_macros.h>

    extern uint32_t __BOOT_STACK_ADDRESS; /* from Linker command file */
    uint32_t stack_pointer_addr = (uint32_t)&__BOOT_STACK_ADDRESS;

    extern uint32_t m_sec_fs_ram_start; /* from Linker command file */
    uint32_t m_safety_ram_start = (uint32_t)&m_sec_fs_ram_start;
    extern uint32_t m_sec_fs_ram_end; /* from Linker command file */
    uint32_t m_safety_ram_end = (uint32_t)&m_sec_fs_ram_end;

    extern uint32_t m_sec_fs_ram_crc_start; /* from Linker command file */
    uint32_t m_safety_ram_crc_start = (uint32_t)&m_sec_fs_ram_crc_start;
    extern uint32_t m_sec_fs_ram_crc_end; /* from Linker command file */
    uint32_t m_safety_ram_crc_end = (uint32_t)&m_sec_fs_ram_crc_end;


    wd_test_t g_sSafetyWdTest __attribute__((section(".safety_ram")));
    safety_common_t g_sSafetyCommon __attribute__((section(".safety_ram")));
    clock_test_t g_sSafetyClockTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamTest __attribute__((section(".safety_ram")));
    ram_test_t g_sSafetyRamStackTest __attribute__((section(".safety_ram")));
    flash_runtime_test_parameters_t g_sFlashCrc __attribute__((section(".safety_ram")));
    flash_configuration_parameters_t g_sFlashConfig __attribute__((section(".safety_ram")));

#if defined(DCP_BASE)     
    /* DCP flash test */
    uint32_t g_contextSwitchingBuffer[52] __attribute__((section(".safety_ram_crc")));
    volatile uint32_t g_flashTestTTF __attribute__((section(".safety_ram_crc")));
    fs_flash_dcp_state_t g_sFlashDCPState __attribute__((section(".safety_ram_crc")));
    fs_flash_dcp_channels_t g_dcpSafetyChannel __attribute__((section(".safety_ram_crc")));
    ram_test_t g_sSafetyRamCRCTest __attribute__((section(".safety_ram_crc")));
#endif

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

    g_sSafetyCommon.safetyErrors        = 0; /* Clear the variable that records safety error codes */
    g_sSafetyCommon.fastIsrSafetySwitch = 0;
#if defined(DCP_BASE)
    g_flashTestTTF = 0;
#endif

#if defined(__IAR_SYSTEMS_ICC__) /* IAR */
    uint32_t *safetyRamStart = __section_begin(".safety_ram");
    uint32_t *safetyRamEnd   = __section_end(".safety_ram");

#if defined(DCP_BASE)
    /* DCP-related memory */
    uint32_t *safetyRamCRCStart = __section_begin(".safety_ram_crc");
    uint32_t *safetyRamCRCEnd   = __section_end(".safety_ram_crc");
#endif

#elif (defined(__GNUC__) && (__ARMCC_VERSION >= 6010050)) /* KEIL */
    uint32_t *safetyRamStart = (uint32_t *)m_safety_ram_start;
    uint32_t *safetyRamEnd   = (uint32_t *)m_safety_ram_end;

#if defined(DCP_BASE)
    /* DCP-related memory */
    uint32_t *safetyRamCRCStart = (uint32_t *)m_safety_ram_crc_start;
    uint32_t *safetyRamCRCEnd   = (uint32_t *)m_safety_ram_crc_end;
#endif

#else /* MCUXpresso */
    uint32_t *safetyRamStart = (uint32_t *)m_safety_ram_start;
    uint32_t *safetyRamEnd   = (uint32_t *)m_safety_ram_end;

#if defined(DCP_BASE)
    /* DCP-related memory */
    uint32_t *safetyRamCRCStart = (uint32_t *)m_safety_ram_crc_start;
    uint32_t *safetyRamCRCEnd   = (uint32_t *)m_safety_ram_crc_end;
#endif
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
    SafetyWatchdogTest(&g_sSafetyCommon, &g_sSafetyWdTest);

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

    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0], dio_safety_test_items[1],
                                          LOGICAL_ONE);
    SafetyDigitalInputOutput_ShortAdjTest(&g_sSafetyCommon, dio_safety_test_items[0], dio_safety_test_items[1],
                                          LOGICAL_ZERO);
#endif /* DIO_TEST_ENABLED */

#if FMSTR_SERIAL_ENABLE
    SerialInit();

    /* FreeMaster init */
    FMSTR_Init();
#endif

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


#if defined(DCP_BASE)
    /* Flash test initialization */
    DCPInit(g_contextSwitchingBuffer);
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig, &g_sFlashDCPState);
#else
    /* Flash test initialization */
    SafetyFlashTestInit(&g_sFlashCrc, &g_sFlashConfig, NULL);
#endif


#if WATCHDOG_ENABLED
    Watchdog_refresh; /********************************************************/
#endif

#if FLASH_TEST_ENABLED
#if defined(DCP_BASE)
    /* After-reset flash test */
    SafetyFlashAfterResetTest_DCP(&g_sSafetyCommon, &g_sFlashConfig, &g_sFlashDCPState);
#else
    /* After-reset flash test */
    SafetyFlashAfterResetTest(&g_sSafetyCommon, &g_sFlashConfig);
#endif

#if WATCHDOG_ENABLED
    Watchdog_refresh; /********************************************************/
#endif
#endif /* FLASH_TEST_ENABLED */

    /* Ram test init for Safety related RAM space */
    SafetyRamTestInit(&g_sSafetyRamTest, safetyRamStart, safetyRamEnd);

    /* Ram after-reset test for safety related memory */
    SafetyRamAfterResetTest(&g_sSafetyCommon, &g_sSafetyRamTest);

    /* Ram test init for Stack memory */
    SafetyRamTestInit(&g_sSafetyRamStackTest, (uint32_t *)c_stackTestFirstAddress,
                      (uint32_t *)c_stackTestSecondAddress);
#if defined(DCP_BASE)
    /* Ram test init for DCP-related memory */
    SafetyRamTestInit(&g_sSafetyRamCRCTest, safetyRamCRCStart, safetyRamCRCEnd);
#endif

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

#if ADC_TEST_ENABLED
    AdcInit();

#if WATCHDOG_ENABLED
    Watchdog_refresh; /* refreshing the watchdog */
#endif

    /* initialize Analog test */
    SafetyAnalogTestInitialization();

    /* After-reset ADC test */
    for (uint8_t i = 0; i < 4; i++) /* first iteration is init phase */
    {
        for (uint8_t y = 0; y < 100; y++)
            __asm("nop"); /* delay because of conversion time */
        SafetyAnalogTest(&g_sSafetyCommon);
        
        #if WATCHDOG_ENABLED
          Watchdog_refresh; /* refreshing the watchdog */
        #endif
    }
#endif /* ADC_TEST_ENABLED */

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

        /* safety test of CPU CONTROL register, it cannot be placed in interrupt, thus interrupts must be disabled for a
         * while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        #if __FPU_PRESENT
            g_sSafetyCommon.CPU_control_test_result = FS_CM4_CM7_CPU_ControlFpu();
        #else
            g_sSafetyCommon.CPU_control_test_result = FS_CM4_CM7_CPU_Control();
        #endif
        __asm("CPSIE i");
        if (g_sSafetyCommon.CPU_control_test_result == FS_FAIL_CPU_CONTROL)
        {
            g_sSafetyCommon.safetyErrors |= CPU_CONTROL_ERROR;
            SafetyErrorHandling(&g_sSafetyCommon);
        }

        /* safety test of CPU SP_PROCESS register, it cannot be placed in interrupt, thus interrupts must be disabled
         * for a while */
        /* - see IEC60730 library documentation for CPU errors handling ! */
        __asm("CPSID i");
        FS_CM4_CM7_CPU_SPprocess();
        __asm("CPSIE i");

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

#if FLASH_TEST_ENABLED
        /* Runtime Flash test */
#if defined(DCP_BASE)
        SafetyFlashRuntimeTest_DCP(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig, &g_sFlashDCPState);
#else
        SafetyFlashRuntimeTest(&g_sSafetyCommon, &g_sFlashCrc, &g_sFlashConfig);
#endif
#endif

#if DIO_TEST_ENABLED
        /* Digital I/O test */
        safety_dio_runtime();
#endif

#if FMSTR_SERIAL_ENABLE

        FMSTR_Poll(); /* Freemaster cummunication */
#endif

        development_test_terminate(); /* For example validation during development */
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
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number],
                                                 DIO_SHORT_TO_GND_TEST);
        SafetyDigitalInputOutput_ShortSupplyTest(&g_sSafetyCommon, dio_safety_test_items[dio_cnt_number],
                                                 DIO_SHORT_TO_VDD_TEST);

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
    counter++;

#if FLASH_TEST_ENABLED
#if defined(DCP_BASE)
    /* Check if SAFETY_RAM_CRC was tested in the time limit */
    g_flashTestTTF++;

    if (g_flashTestTTF > RAM_CRC_CHECK_TIME_THRESHOLD)
    {
        /* Error */
        g_sSafetyCommon.safetyErrors |= FLASH_TEST_RAM_CRC_ERROR;
        SafetyErrorHandling(&g_sSafetyCommon);
    }

    /* Check if all DCP channels are inactive */
    if ((FLASH_USED_DCP->STAT & 0xF0000) == 0)
    {
        /* Test safety_ram_crc */
        SafetyRamRuntimeTest(&g_sSafetyCommon, &g_sSafetyRamCRCTest);

        /* Reset time to failure */
        g_flashTestTTF = 0;
    }
#endif
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
