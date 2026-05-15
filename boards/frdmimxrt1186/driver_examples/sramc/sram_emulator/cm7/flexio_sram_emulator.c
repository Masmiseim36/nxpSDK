/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "flexio_sram_emulator.h"

/*******************************************************************************
 * Codes
 ******************************************************************************/
 /**
 * brief Initializes the FLEXIO with the provided configuration.
 *
 * This function configures the FLEXIO hardware to act as an SRAM emulator.
 * It enables the clock and configures shifters and timers.
 *
 * param base Pointer to the FlexIO SRAM access structure.
 * param config Pointer to the FlexIO configuration structure.
 */
void FLEXIO_SRAM_Init(FLEXIO_SRAM_Type *base, flexio_sram_config_t *config)
{
    assert(base != NULL);
    assert(config != NULL);

    FLEXIO_Type *flexio = base->flexio;
    flexio_shifter_config_t shifterConfig;
    flexio_timer_config_t timerConfig;
    uint32_t ctrlReg = 0;

    /* Clear the shifterConfig & timerConfig struct. */
    (void)memset(&shifterConfig, 0, sizeof(shifterConfig));
    (void)memset(&timerConfig, 0, sizeof(timerConfig));

#if !(defined(FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL) && FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL)
    /* Ungate flexio clock. */
    CLOCK_EnableClock(s_flexioClocks[FLEXIO_GetInstance(flexio)]);
#endif /* FSL_SDK_DISABLE_DRIVER_CLOCK_CONTROL */

    /* Configure FLEXIO SRAM */
    flexio->CTRL |= FLEXIO_CTRL_SWRST_MASK;
    flexio->CTRL &= ~FLEXIO_CTRL_SWRST_MASK;

    ctrlReg = flexio->CTRL;
    ctrlReg &= ~(FLEXIO_CTRL_DOZEN_MASK | FLEXIO_CTRL_DBGE_MASK | FLEXIO_CTRL_FASTACC_MASK | FLEXIO_CTRL_FLEXEN_MASK);
    ctrlReg |=
        (FLEXIO_CTRL_DBGE(config->enableInDebug ? 1U : 0U) | FLEXIO_CTRL_FASTACC(config->enableFastAccess ? 1U : 0U) |
         FLEXIO_CTRL_FLEXEN(config->enableFlexio ? 1U : 0U));
    if (!config->enableInDoze)
    {
        ctrlReg |= FLEXIO_CTRL_DOZEN_MASK;
    }
    flexio->CTRL = ctrlReg;

    /* Do hardware configuration. */
    /* 1. Configure the shifters for ADV. */
    shifterConfig.timerSelect = base->advTimerIndex;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect = base->daPinIndex;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeReceive;
    shifterConfig.parallelWidth = FLEXIO_SRAM_DATA_BUS_WIDTH - 1U;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    FLEXIO_SetShifterConfig(flexio, base->advShifterIndex, &shifterConfig);
    flexio->SHIFTCFG[base->advShifterIndex] |= FLEXIO_SHIFTCFG_LATST_MASK;

    /* 2. Configure the shifters for WE (master write). */
    shifterConfig.timerSelect = base->weTimerIndex;
    shifterConfig.timerPolarity = kFLEXIO_ShifterTimerPolarityOnNegitive;
    shifterConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    shifterConfig.pinSelect = base->daPinIndex;
    shifterConfig.pinPolarity = kFLEXIO_PinActiveHigh;
    shifterConfig.shifterMode = kFLEXIO_ShifterModeReceive;
    shifterConfig.parallelWidth = FLEXIO_SRAM_DATA_BUS_WIDTH - 1U;
    shifterConfig.inputSource = kFLEXIO_ShifterInputFromPin;
    shifterConfig.shifterStop = kFLEXIO_ShifterStopBitDisable;
    shifterConfig.shifterStart = kFLEXIO_ShifterStartBitDisabledLoadDataOnEnable;
    FLEXIO_SetShifterConfig(flexio, base->weShifterIndex, &shifterConfig);
    flexio->SHIFTCFG[base->weShifterIndex] |= FLEXIO_SHIFTCFG_LATST_MASK;

    /* 3. Configure the timer for ADV. */
    timerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_PININPUT(base->cePinIndex);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect = base->advPinIndex;
    timerConfig.pinPolarity = kFLEXIO_PinActiveLow;
    timerConfig.timerMode = kFLEXIO_TimerModeSingle16Bit;
    timerConfig.timerOutput = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnPinInputShiftPinInput;
    timerConfig.timerReset = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable = kFLEXIO_TimerEnableOnTriggerRisingEdge;
    timerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare = 1U;
    FLEXIO_SetTimerConfig(flexio, base->advTimerIndex, &timerConfig);

    /* 4. Configure the timer for WE (master write). */
    timerConfig.triggerSelect = FLEXIO_TIMER_TRIGGER_SEL_PININPUT(base->cePinIndex);
    timerConfig.triggerPolarity = kFLEXIO_TimerTriggerPolarityActiveLow;
    timerConfig.triggerSource = kFLEXIO_TimerTriggerSourceInternal;
    timerConfig.pinConfig = kFLEXIO_PinConfigOutputDisabled;
    timerConfig.pinSelect = base->wePinIndex;
    timerConfig.pinPolarity = kFLEXIO_PinActiveLow;
    timerConfig.timerMode = kFLEXIO_TimerModeSingle16Bit;
    timerConfig.timerOutput = kFLEXIO_TimerOutputZeroNotAffectedByReset;
    timerConfig.timerDecrement = kFLEXIO_TimerDecSrcOnPinInputShiftPinInput;
    timerConfig.timerReset = kFLEXIO_TimerResetNever;
    timerConfig.timerDisable = kFLEXIO_TimerDisableOnTimerCompare;
    timerConfig.timerEnable = kFLEXIO_TimerEnableOnTriggerRisingEdge;
    timerConfig.timerStop = kFLEXIO_TimerStopBitDisabled;
    timerConfig.timerStart = kFLEXIO_TimerStartBitDisabled;
    timerConfig.timerCompare = 1U;
    FLEXIO_SetTimerConfig(flexio, base->weTimerIndex, &timerConfig);
}

/**
 * brief Gets the default configuration for the FLEXIO module.
 *
 * This function initializes the provided configuration structure with default values.
 * These values can be used as a starting point for configuring the FLEXIO module
 * to act as an SRAM emulator.
 *
 * param config Pointer to the configuration structure to be initialized.
 */
void FLEXIO_SRAM_GetDefaultConfig(flexio_sram_config_t *config)
{
    config->enableFlexio = true;
    config->enableInDoze = false;
    config->enableInDebug = true;
    config->enableFastAccess = true;
}

/**
 * brief Starts the FLEXIO SRAM emulator by enabling specific interrupts.
 *
 * This function initializes and enables interrupts related to the WE shifter and OE pin
 * of the FLEXIO SRAM emulator. The WE shifter interrupt is used to handle write operations,
 * while the OE pin interrupts (both rising and falling edges) are used to handle read operations.
 *
 * param base Pointer to the FLEXIO SRAM access structure.
 */
void FLEXIO_SRAM_Start(FLEXIO_SRAM_Type *base)
{
    FLEXIO_Type *flexio = base->flexio;
    register uint32_t oePinMask = (uint32_t)1U << base->oePinIndex;

    /* Clear the WE shifter status flag to prepare for new interrupts */
    flexio->SHIFTSTAT |= (uint32_t)1U << base->weShifterIndex;
    /* Enable the interrupt for the WE shifter */
    flexio->SHIFTSIEN |= (uint32_t)1U << base->weShifterIndex;
    
    /* Initialize the OE pin level state */
    base->oeLevel = 1U;
    /* Clear the OE pin status flag */
    flexio->PINSTAT = oePinMask;
    /* Enable the interrupt for the OE pin */
    flexio->PINIEN = oePinMask;
    /* Enable the rising edge interrupt for the OE pin */
    flexio->PINREN = oePinMask;
    /* Enable the falling edge interrupt for the OE pin */
    flexio->PINFEN = oePinMask;
}

/**
 * brief Interrupt service routine for the FLEXIO SRAM emulator.
 *
 * This function handles interrupts related to the OE (Output Enable) pin and the WE (Write Enable) shifter.
 * When an OE pin interrupt occurs, it manages data transfer based on the OE pin state.
 * When a WE shifter interrupt occurs, it stores the received data at the appropriate address.
 *
 * param base Pointer to the FLEXIO SRAM access structure.
 */
AT_QUICKACCESS_SECTION_CODE(void FLEXIO_SRAM_ISR(FLEXIO_SRAM_Type *base))
{
    register FLEXIO_Type *flexio = base->flexio;
    register uint32_t oePinMask = (uint32_t)1U << base->oePinIndex;    
    register uint32_t oeFlagSet = flexio->PINSTAT & oePinMask;
    register uint32_t oePinLevel = flexio->PIN & oePinMask;
    register uint32_t address;
    register uint32_t rxData;
    
    /* When OE pin interrupt happens, send data according to address */
    if (oeFlagSet != 0U)
    {
        /* Check if the OE pin has transitioned from high to low */
        if (base->oeLevel != 0U && oePinLevel != oePinMask)
        {
            /* Read the address from the ADV shifter buffer, it will clear corresponding shifter status flag*/
#if (defined(FLEXIO_SRAM_DATA_BUS_WIDTH) && FLEXIO_SRAM_DATA_BUS_WIDTH == 8U)
            address = (flexio->SHIFTBUF[base->advShifterIndex] >> 24U) & 0xFFU; 
#else
            address = (flexio->SHIFTBUFHWS[base->advShifterIndex]) & 0xFFFFU;
#endif
            if (address < FLEXIO_SRAM_DATA_BUFFER_LEN)
            {
                /* Output the data at the specified address to the pins */
                flexio->PINOUTD = base->data[address];
                /* Enable all pin outputs */
#if (defined(FLEXIO_SRAM_DATA_BUS_WIDTH) && FLEXIO_SRAM_DATA_BUS_WIDTH == 8U)
                flexio->PINOUTE = 0xFFU;
#else
                flexio->PINOUTE = 0xFFFFU;
#endif
            }
            /* Update the OE pin level state */
            base->oeLevel = 0U;
        }
        else /* OE rising edge, stop transfer */
        {
            /* Disable all pin outputs */
#if (defined(FLEXIO_SRAM_DATA_BUS_WIDTH) && FLEXIO_SRAM_DATA_BUS_WIDTH == 8U)
            flexio->PINOUTDIS = 0xFFU;
#else
            flexio->PINOUTDIS = 0xFFFFU;
#endif
            /* Update the OE pin level state */
            base->oeLevel = 1U;
        }
        /* Clear the OE pin status flag */
        flexio->PINSTAT = oePinMask;
    }
    
    /* When WE shifter interrupt happends, store reveived data */
    /* Check if a WE shifter interrupt has occurred */
    if ((flexio->SHIFTSTAT & ((uint32_t)1U << base->weShifterIndex)) != 0U)
    {
        /* Read the address from the ADV shifter buffer, read the data from the WE shifter buffer */
#if (defined(FLEXIO_SRAM_DATA_BUS_WIDTH) && FLEXIO_SRAM_DATA_BUS_WIDTH == 8U)
        address = (flexio->SHIFTBUF[base->advShifterIndex] >> 24) & 0xFFU;
        rxData = (flexio->SHIFTBUF[base->weShifterIndex] >> 24) & 0xFFU;
#else
        address = (flexio->SHIFTBUFHWS[base->advShifterIndex]) & 0xFFFFU;
        rxData = (flexio->SHIFTBUFHWS[base->weShifterIndex]) & 0xFFFFU;
#endif
        if (address < FLEXIO_SRAM_DATA_BUFFER_LEN)
        {
            base->data[address] = rxData;
        }
        
    }
}
