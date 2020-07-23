/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "safety_periph_init.h"
#include "safety_cfg.h"
#include "safety_flow_check.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* GPIO output pin initial configuration table entry type. */
typedef struct _gpio_output_init
{
    uint32_t ui32Idx  : 3;  /* The GPIO periphery index (0 = GPIOA, 4 = GPIOE). */
    uint32_t ui32Pin  : 5;  /* The GPIO pin number. */
    uint32_t ui32Val  : 1;  /* The GPIO output initial value. */
} gpio_output_init_t;
/*******************************************************************************
 * Variables
 ******************************************************************************/

/* GPIO output pin initial configuration table. */
const gpio_output_init_t c_psGPIOInitTable[] = PIN_GPIO_INIT_TABLE;


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*!
 * @brief   Initialize all MCU internal clocks.
 *
 * @param   void
 *
 * @return  none
 */
static void fsInitClock(void);

/*!
 * @brief   Initialize MCU pin multiplexers and initial GPIO pin states.
 *
 * @param   void
 *
 * @return  none
 */
static void fsInitPins(void);

/*!
 * @brief   Initialization of LPTMR periphery, which is used for the safety
 *          clock and watchdog test.
 *
 * @details The LPTMR has to be clocked from separate source to ensure
 *          independence for clock and watchdog test! By default the LPTMR
 *          is clocked from MCGIRCLK. The MCGIRCLK frequency should then be
 *          set or divided to ensure, that no more than 32768 tick are generated
 *          per one watchdog timeout period.
 *
 * @param   void
 *
 * @return  none
 */
static void fsInitLPTMR(void);



/*******************************************************************************
 * Code - functions with global scope
 ******************************************************************************/

/*!
 * Motor control driver main initialization function. Peripherals
 * required for safe motor-control functionality are configured.
 */
void FSPER_fsInit(void)
{
    /* Initialize clocks. */
    fsInitClock();

    /* Initialize all pins. */
    fsInitPins();

    /* Initialize low-power timer. */
    fsInitLPTMR();

    /* Enable clock for the CRC periphery. */
    SIM->SCGC6 |= SIM_SCGC6_CRC_MASK;
}

/*!
 * WATCHDOG configure and enable function.
 */
void FSPER_fsWatchdogEnable(void)
{
    /* Perform unlock sequence. */
    WDOG->UNLOCK = 0xC520U;
    WDOG->UNLOCK = 0xD928U;

    /* Wait at least one cycle after unlocking. */
    __asm("nop");

    /* Configure low and high timeout values. */
    WDOG->TOVALL = (uint16_t) FS_CFG_WDOG_TOUT_VALUE;
    WDOG->TOVALH = (uint16_t)(FS_CFG_WDOG_TOUT_VALUE >> 16U);

    /* Clear watchdog source clock prescaler to use 1 kHz. */
    WDOG->PRESC = 0U;

    /* Enable watchdog for RUN, WAIT, and STOP modes.
       Select LPO internal 1 kHz clock as WDOG source.  */
    WDOG->STCTRLH = WDOG_STCTRLH_WAITEN_MASK |
                    WDOG_STCTRLH_STOPEN_MASK |
                    WDOG_STCTRLH_WDOGEN_MASK;

    /* Watchdog periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_WDOG_INIT);
}

/*!
 * WATCHDOG disable function.
 */
void FSPER_fsWatchdogDisable(void)
{
    uint32_t ui32fsPrimaskReg;

    /* Disable all interrupts. */
    ui32fsPrimaskReg = __get_PRIMASK();
    __disable_irq();

    /* Perform the unlock sequence. */
    WDOG->UNLOCK = 0xC520U;
    WDOG->UNLOCK = 0xD928U;

    /* Disable the watchdog. */
    WDOG->STCTRLH &= (uint16_t)(~((uint16_t)WDOG_STCTRLH_WDOGEN_MASK));

    /* Enable the interrupts. */
    __set_PRIMASK(ui32fsPrimaskReg);

    /* Watchdog periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_WDOG_INIT);
}




/*******************************************************************************
 * Code - functions with local scope
 ******************************************************************************/
/*!
 * Initialize configuration structure with various MCU internal clock
 * values.
 */
static void fsInitClock(void)
{
    /* -------------------- Enable High-Speed Run mode. --------------------- */
    /* Allow the High-Speed Run mode. */
    SMC->PMPROT = SMC_PMPROT_AHSRUN_MASK;

    /* Configure the High-Speed Run mode. */
    SMC->PMCTRL = SMC_PMCTRL_RUNM(3U);

    /* Wait for High-Speed Run mode to start. */
    while(SMC->PMSTAT != 0x80U)
    {
    }

    /* ----- Configure MCGIRCLK and use it as temporary MCGOUTCLK source. ----*/
    /* Configure the clock dividers. */
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(CLOCK_SYSTEM_OUTDIV  - 1UL) |
                   SIM_CLKDIV1_OUTDIV2(CLOCK_BUS_OUTDIV     - 1UL) |
                   SIM_CLKDIV1_OUTDIV3(CLOCK_FLEXBUS_OUTDIV - 1UL) |
                   SIM_CLKDIV1_OUTDIV4(CLOCK_FLASH_OUTDIV   - 1UL);

    /* Set the MCGIRCLK divider (FCRDIV setup). */
    MCG->SC = MCG_SC_FCRDIV(CLOCK_MCGIRCLK_DIV);

    /* Configure the MCGIRCLK:
       -Route the fast internal reference clock FAST_IRCLK to MCGIRCLK (IRCS = 1).
       -PLL is not disabled in bypass modes (LP = 0).
       -Add fast internal reference clock fine trim bit (FCFTRIM = 1).
       -Disable the loss of OSC clock reset (LOCRE0 = 0). */
    MCG->C2 = MCG_C2_IRCS_MASK | MCG_C2_FCFTRIM_MASK;

    /* Route the clocks as follows:
       -Configure CLKS to route the MCGIRCLK to MCGOUTCLK (CLKS = 1).
       -Activate the MCGIRCLK (IRCLKEN = 1). */
    MCG->C1 = MCG_C1_CLKS(1U) | MCG_C1_IRCLKEN_MASK;

    /* -------------------------- Initialize PLL. --------------------------- */
    /* Select the internal IRC48MCLK oscillator as PLL input clock. */
    MCG->C7 = MCG_C7_OSCSEL(2U);

    /* Set the PLL divide factor into PRDIV0 (divided input clock should be in
       2-4 MHz range, which for 48 MHz input clock means 12-24 divider). */
    MCG->C5 = MCG_C5_PRDIV0(CLOCK_PLL_DIV - 1U);

    /* Set the PLL multiplier into VDIV0.*/
    MCG->C6 = MCG_C6_VDIV0(CLOCK_PLL_MULT - 24U);

    /* Enable the PLL using PLLCLKEN0 bit. */
    MCG->C5 |= MCG_C5_PLLCLKEN0_MASK;

    /* Wait for PLL to lock. */
    while(0x00U == (MCG->S & MCG_S_LOCK0_MASK))
    {
    }

    /* Select the PLL as clock source via PLLS. */
    MCG->C6 |= MCG_C6_PLLS_MASK;

    /* Wait for PLL to be selected. */
    while(0x00U == (MCG->S & MCG_S_PLLST_MASK))
    {
    }

    /* Configure CLKS to route the MCGPLLCLK to MCGOUTCLK (CLKS = 0). */
    MCG->C1 = MCG_C1_CLKS(0U) | MCG_C1_IRCLKEN_MASK;

    /* Make sure that PLL was selected. */
    while(MCG_S_CLKST_MASK != (MCG->S & MCG_S_CLKST_MASK))
    {
    }


    /* Use the LPO clock as OSC32KSEL clock source. */
    SIM->SOPT1 = SIM_SOPT1_OSC32KSEL(3U);

    /* Configure CLKOUT to output the MCGIRCLK and route IRC48MCLK to
       PLL/FLL/IRC48 clock. */
    SIM->SOPT2 = SIM_SOPT2_CLKOUTSEL(4U) | SIM_SOPT2_PLLFLLSEL(3U);

    /* MCU clock initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSPER_INIT_CLK);
}


/*!
 * Initialize configuration structure with various MCU internal clock
 * values.
 */
static void fsInitPins(void)
{
    /* The GPIO output pin initialization cycle help variables (address and
       counter). */
    uint32_t  ui32fsAddr, ui32fsI;

    /* Enable clocks to all ports. */
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK |
                   SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK |
                   SIM_SCGC5_PORTE_MASK);

    /* Use PDB as ADC0 and ADC1 trigger source. */
    SIM->SOPT7 = 0UL;

    /* ------------------- Initialize FTM PWM pins. ------------------------- */

    /* Configure PWM FTM0 output ports. */
    PIN_PWM0_PORT->PCR[PIN_PWM0_PIN] = PORT_PCR_MUX(PIN_PWM0_MUX);
    PIN_PWM1_PORT->PCR[PIN_PWM1_PIN] = PORT_PCR_MUX(PIN_PWM1_MUX);
    PIN_PWM2_PORT->PCR[PIN_PWM2_PIN] = PORT_PCR_MUX(PIN_PWM2_MUX);
    PIN_PWM3_PORT->PCR[PIN_PWM3_PIN] = PORT_PCR_MUX(PIN_PWM3_MUX);
    PIN_PWM4_PORT->PCR[PIN_PWM4_PIN] = PORT_PCR_MUX(PIN_PWM4_MUX);
    PIN_PWM5_PORT->PCR[PIN_PWM5_PIN] = PORT_PCR_MUX(PIN_PWM5_MUX);

    /* No multi-FTM periphery initialization and no synchronization of FTM
       triggers. */
    SIM->SOPT8 = 0UL;

    /* Configure the FTM0 over-current FAULT_1 input signal. */
    PIN_FAULT_1_PORT->PCR[PIN_FAULT_1_PIN] = PORT_PCR_MUX(PIN_FAULT_1_MUX);

    /* Make sure that FTM0_FLT1 pin drives the FTM0 fault 1. */
    SIM->SOPT4 = 0UL;


    /* ---------------------- Initialize UART pins. ------------------------- */
    /* Configure UART1 ports. */
    PIN_UART_RXD_PORT->PCR[PIN_UART_RXD_PIN] = PORT_PCR_MUX(PIN_UART_RXD_MUX);
    PIN_UART_TXD_PORT->PCR[PIN_UART_TXD_PIN] = PORT_PCR_MUX(PIN_UART_TXD_MUX);

    /* Select defaul UART pins as data sources. */
    SIM->SOPT5 = 0UL;

    /* -------------------- Initialize GPIO output pins. -------------------- */
    /*! @note Initialized state of all PDOR registers is presumed. All previous
              PORT setting of the pin will be overwritten. */
    /* Go through the GPIO output initialization table. */
    for(ui32fsI = 0UL; ui32fsI < (sizeof(c_psGPIOInitTable)/sizeof(gpio_output_init_t)); ui32fsI++)
    {
        /* Get the GPIO address by masking lower five bits of the GPIO init table entry. */
        ui32fsAddr = (c_psGPIOInitTable[ui32fsI].ui32Idx *
            ((uint32_t)(GPIOB) - (uint32_t)(GPIOA))) + (uint32_t)(GPIOA);

        /* Set the initial output GPIO pin value. */
        ((GPIO_Type*)ui32fsAddr)->PDOR |=
            ((uint32_t)c_psGPIOInitTable[ui32fsI].ui32Val) <<
                       c_psGPIOInitTable[ui32fsI].ui32Pin;

        /* Configure the GPIO pin direction as output. */
        ((GPIO_Type*)ui32fsAddr)->PDDR |= 1UL << c_psGPIOInitTable[ui32fsI].ui32Pin;

        /* Get the PORT address using the GPIO init table entry index value. */
        ui32fsAddr = (c_psGPIOInitTable[ui32fsI].ui32Idx *
            ((uint32_t)(PORTB) - (uint32_t)(PORTA))) + (uint32_t)(PORTA);

        /* Configure the pin PORT to be controlled by GPIO. */
        ((PORT_Type*)ui32fsAddr)->PCR[c_psGPIOInitTable[ui32fsI].ui32Pin] = PORT_PCR_MUX(1U);
    }

    /* MCU pin initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSPER_INIT_PIN);
}


/*!
 * LPTMR peripheral configuration.
 */
static void fsInitLPTMR(void)
{
    /* Enable clock for LPTMR. */
    SIM->SCGC5 |= SIM_SCGC5_LPTMR_MASK;

    /* Use time counter mode. */
    LPTMR0->CSR = 0U;

    /* CNR reset on timer overflow. */
    LPTMR0->CSR = LPTMR_CSR_TCF_MASK | LPTMR_CSR_TFC_MASK;

    /* Bypass the prescaler. */
    LPTMR0->PSR |= LPTMR_PSR_PBYP_MASK;

    /* Select the MCGIRCLK as source clock. */
    LPTMR0->PSR &= (~LPTMR_PSR_PCS_MASK);

    /* Clear the prescaler - no division is necessary. */
    LPTMR0->PSR |= LPTMR_PSR_PCS(0U);

    /* Clear the compare and timer registers. */
    LPTMR0->CMR = 0U;
    LPTMR0->CNR = 0U;

    /* Start the timer. */
    LPTMR0->CSR &= (uint32_t)(~((uint32_t)LPTMR_CSR_TEN_MASK));
    LPTMR0->CSR |= LPTMR_CSR_TEN_MASK;

    /* Backup timer periphery initialization node. */
    FS_FLOW_CHK_NODE(g_sfsFlowChkAR, FS_FLCK_AR_NODE_FSPER_INIT_TMR);
}
