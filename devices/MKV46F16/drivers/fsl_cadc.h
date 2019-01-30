/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_CADC_H_
#define _FSL_CADC_H_

#include "fsl_common.h"

/*!
 * @addtogroup cadc
 * @{
 */


/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief CADC driver version 2.0.0. */
#define FSL_CADC_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

#define CADC_SAMPLE_MASK(index) (1U << (index)) /*!< Convert index to mask for sample slot. */

/*!
 * @brief CADC status flags.
 */
enum _cadc_status_flags
{
    kCADC_ZeroCrossingFlag = (1U << 0U),         /*!< Zero crossing. */
    kCADC_HighLimitFlag = (1U << 2U),            /*!< High limit. */
    kCADC_LowLimitFlag = (1U << 1U),             /*!< Low limit. */
    kCADC_ConverterAInProgressFlag = (1U << 3U), /*!< Conversion in progress, converter A. */
    kCADC_ConverterBInProgressFlag = (1U << 4U), /*!< Conversion in progress, converter B. */
    kCADC_ConverterAEndOfScanFlag = (1U << 5U),  /*!< End of scan, converter A. */
    kCADC_ConverterBEndOfScanFlag = (1U << 6U),  /*!< End of scan, converter B. */
    kCADC_ConverterAPowerDownFlag = (1U << 7U),  /*!< The converter is powered down, converter A. */
    kCADC_ConverterBPowerDownFlag = (1U << 8U),  /*!< The converter is powered down, converter B. */
};

/*!
 * @brief CADC Interrupts.
 */
enum _cadc_interrupt_enable
{
    kCADC_ZeroCrossingInterruptEnable = (1U << 0U),        /*!< Zero crossing interrupt. */
    kCADC_HighLimitInterruptEnable = (1U << 1U),           /*!< High limit interrupt. */
    kCADC_LowLimitInterruptEnable = (1U << 2U),            /*!< Low limit interrupt. */
    kCADC_ConverterAEndOfScanInterruptEnable = (1U << 3U), /*!< End of scan interrupt, converter A. */
    kCADC_ConverterBEndOfScanInterruptEnable = (1U << 4U), /*!< End of scan interrupt, converter B.*/
};

/*!
 * @brief CADC Converter identifier.
 */
enum _cadc_converter_id
{
    kCADC_ConverterA = (1U << 0U), /*!< Converter A. */
    kCADC_ConverterB = (1U << 1U), /*!< Converter B. */
};

/*!
 * @brief Defines the enumeration for dual converter's scan mode.
 */
typedef enum _cadc_dual_converter_scan_mode
{
    kCADC_DualConverterWorkAsOnceSequential = 0U,      /*!< Once (single) sequential. */
    kCADC_DualConverterWorkAsOnceParallel = 1U,        /*!< Once parallel. */
    kCADC_DualConverterWorkAsLoopSequential = 2U,      /*!< Loop sequential. */
    kCADC_DualConverterWorkAsLoopParallel = 3U,        /*!< Loop parallel. */
    kCADC_DualConverterWorkAsTriggeredSequential = 4U, /*!< Triggered sequential. */
    kCADC_DualConverterWorkAsTriggeredParallel = 5U,   /*!< Triggered parallel. */
} cadc_dual_converter_scan_mode_t;

/*!
 * @brief Defines the enumeration for DMA trigger's source.
 */
typedef enum _cadc_dma_trigger_source
{
    kCADC_DMATriggerSourceAsEndOfScan = 0U,   /*!< DMA trigger source is end of scan interrupt. */
    kCADC_DMATriggerSourceAsSampleReady = 1U, /*!< DMA trigger source is RDY bits. */
} cadc_dma_trigger_source_t;

/*!
 * @brief Defines the enumeration for converter's reference voltage source.
 */
typedef enum _cadc_reference_voltage_source
{
    kCADC_ReferenceVoltageVrefPad = 0U,    /*!< VREF pin. */
    kCADC_ReferenceVoltageChannelPad = 1U, /*!< ANx2 or ANx3 pin. */
} cadc_reference_voltage_source_t;

/*!
 * @brief Defines the enumeration for channel's input gain.
 */
typedef enum _cadc_channel_gain
{
    kCADC_ChannelGainx1 = 0U, /*!< Gain x1. */
    kCADC_ChannelGainx2 = 1U, /*!< Gain x2. */
    kCADC_ChannelGainx4 = 2U, /*!< Gain x4. */
} cadc_channel_gain_t;

/*!
 * @brief Defines the enumeration for the converter's speed mode.
 */
typedef enum _cadc_speed_mode
{
    kCADC_SpeedMode0 = 0U, /*!< Conversion clock frequency <= 6.25 MHz; current consumption per converter = 6 mA. */
    kCADC_SpeedMode1 = 1U, /*!< Conversion clock frequency <= 12.25 MHz; current consumption per converter = 10.8 mA. */
    kCADC_SpeedMode2 = 2U, /*!< Conversion clock frequency <= 18.75 MHz; current consumption per converter = 18 mA. */
    kCADC_SpeedMode3 = 3U, /*!< Conversion clock frequency <= 25 MHz; current consumption per converter = 25.2 mA. */
} cadc_speed_mode_t;

/*!
 * @brief Defines the enumeration for the sample slot's zero crossing event.
 */
typedef enum _cadc_zero_crossing_mode
{
    kCADC_ZeroCorssingDisabled = 0U,          /*!< Zero Crossing disabled. */
    kCADC_ZeroCorssingForPtoNSign = 1U,       /*!< Zero Crossing enabled for positive to negative sign change. */
    kCADC_ZeroCorssingForNtoPSign = 2U,       /*!< Zero Crossing enabled for negative to positive sign change. */
    kCADC_ZeroCorssingForAnySignChanged = 3U, /*!< Zero Crossing enabled for any sign change. */
} cadc_zero_crossing_mode_t;

/*!
 * @brief Defines the enumeration for converter's work mode in idle mode.
 */
typedef enum _cadc_idle_work_mode
{
    kCADC_IdleKeepNormal = 0U, /*!< Keep normal. */
    kCADC_IdleAutoStandby,     /*!< Fall into standby mode automatically. */
    kCADC_IdleAutoPowerDown,   /*!< Fall into power down mode automatically. */
} cadc_idle_work_mode_t;

/*!
 * @brief Defines the structure for configuring the CyclicADC's common setting.
 */
typedef struct _cadc_config
{
    cadc_dual_converter_scan_mode_t dualConverterScanMode; /*!< Dual converter's scan mode. */
    bool enableSimultaneousMode;                           /*!< Using Simultaneous mode. */
    cadc_dma_trigger_source_t DMATriggerSoruce;            /*!< DMA trigger source. */
    cadc_idle_work_mode_t idleWorkMode;                    /*!< Converter's work mode when in idle. */
    uint16_t powerUpDelay; /*!< Delay clock count to wait for the clock is stable. Available range is 0-63.*/
} cadc_config_t;

/*!
 * @brief Define the structure for configuring each converter.
 */
typedef struct _cadc_converter_config
{
    uint16_t clockDivisor; /*!< Converter's clock divisor for the clock source. Available setting range is 0-63. */
    cadc_speed_mode_t speedMode;                                /*!< Converter's speed mode. */
    cadc_reference_voltage_source_t highReferenceVoltageSource; /*!< High voltage reference source. */
    cadc_reference_voltage_source_t lowReferenceVoltageSource;  /*!< Low reference voltage source. */
    uint16_t sampleWindowCount; /*!< Sample Window Count. Available setting range is 0-7. */
} cadc_converter_config_t;

/*!
 * @brief Defines the structure for configuring the sample slot.
 */
typedef struct _cadc_sample_config
{
    /* Conversion channel setting. */
    uint16_t channelNumber;          /*!< Channel number. The available range is 0-15.*/
    bool enableDifferentialPair;     /*!< Use differential sample input or not. */
    cadc_channel_gain_t channelGain; /*!< Input gain. */

    /* sample result setting. */
    cadc_zero_crossing_mode_t zeroCrossingMode; /*!< Zero crossing mode. */
    uint16_t highLimitValue; /*!< High limit value. Original value formation as hardware register, with 3-bits left
                                shifted. */
    uint16_t
        lowLimitValue; /*!< Low limit value. Original value formation as hardware register, with 3-bits left shifted. */
    uint16_t offsetValue; /*!< Offset value. Original value formation as hardware register, with 3-bits left shifted. */
    bool enableWaitSync;  /*!< Wait for sync input to launch this sample's conversion. */
} cadc_sample_config_t;

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @name Initialization and deinitialization
 * @{
 */

/*!
 * @brief Initializes the CADC module.
 *
 * This function is to make the initialization for using CADC module.
 * The operations are:
 *  - Enable the clock for CADC.
 *  - Set the global settings for CADC converter.
 *
 * @param base   CADC peripheral base address.
 * @param config Pointer to configuration structure. See "cadc_config_t".
 */
void CADC_Init(ADC_Type *base, const cadc_config_t *config);

/*!
 * @brief Gets an available pre-defined settings for module's configuration.
 *
 * This function initializes the module's configuration structure with an available settings.
 * The default value are:
 * @code
 *   config->dualConverterScanMode = kCADC_DualConverterWorkAsTriggeredParallel;
 *   config->enableSimultaneousMode = true;
 *   config->DMATriggerSoruce = kCADC_DMATriggerSourceAsEndOfScan;
 *   config->idleWorkMode = kCADC_IdleKeepNormal;
 *   config->powerUpDelay = 26U;
 * @endcode
 * @param config Pointer to configuration structure. See "cadc_config_t"
 */
void CADC_GetDefaultConfig(cadc_config_t *config);

/*!
 * @brief Deinitializes the CADC module.
 *
 * This function is to make the de-initialization for using CADC module.
 * The operations are:
 *  - Power down both converter.
 *  - Disable the clock for CADC.
 *
 * @param base CADC peripheral base address.
 */
void CADC_Deinit(ADC_Type *base);

/* @} */

/*!
 * @name Each Converter
 * @{
 */
/*!
 * @brief Configures the converter.
 *
 * @param base          CADC peripheral base address.
 * @param converterMask Mask for converters to be configured. See "_cadc_converter_id".
 * @param config        Pointer to configuration structure. See "cadc_converter_config_t".
 */
void CADC_SetConverterConfig(ADC_Type *base, uint16_t converterMask, const cadc_converter_config_t *config);

/*!
 * @brief Gets an available pre-defined settings for each converter's configuration.
 *
 * This function initializes each converter's configuration structure with an available settings.
 * The default value are:
 * @code
 *   config->clockDivisor = 4U;
 *   config->speedMode = kCADC_SpeedMode0;
 *   config->highReferenceVoltageSource = kCADC_ReferenceVoltageVrefPad;
 *   config->lowReferenceVoltageSource = kCADC_ReferenceVoltageVrefPad;
 *   config->sampleWindowCount = 0U;
 * @endcode
 * @param config Pointer to configuration structure. See "cadc_converter_config_t"
 */
void CADC_GetDefaultConverterConfig(cadc_converter_config_t *config);

/*!
 * @brief Enables the converter's conversion.
 *
 * This function is to enable the converter's conversion. The conversion should only be launched after the converter is
 * enabled. When this feature is asserted to be "false", the current scan is stopped and no further scans can start. All
 * the software trigger and hardware trigger are ignored.
 *
 * @param base          CADC peripheral base address.
 * @param converterMask Mask for converters to be operated. See "_cadc_converter_id".
 * @param enable        Enable the power for converter.
 */
void CADC_EnableConverter(ADC_Type *base, uint16_t converterMask, bool enable);

/*!
 * @brief Enables the input of external sync signal.
 *
 * This function is to enable the input of external sync signal. The external sync signal could be used to trigger the
 * conversion if the hardware trigger related setting is used.
 * Note: When in "Once" scan mode, this gate would be off automatically after an available sync is received. User needs
 * to enable the input again manually if another sync signal is wanted.
 *
 * @param base          CADC peripheral base address.
 * @param converterMask Mask for converters to be operated. See "_cadc_converter_id".
 * @param enable        Enable the feature or not.
 */
void CADC_EnableConverterSyncInput(ADC_Type *base, uint16_t converterMask, bool enable);

/*!
 * @brief Enables power for the converter.
 *
 * This function is to enable the power for the converter. The converter should be powered on before conversion.
 * Once this API is called, the converter would be powered on after a few moment (so-called power up delay), so that
 * the power would be stable.
 *
 * @param base          CADC peripheral base address.
 * @param converterMask Mask for converters to be operated. See "_cadc_converter_id".
 * @param enable        Enable the feature or not.
 */
void CADC_EnableConverterPower(ADC_Type *base, uint16_t converterMask, bool enable);

/*!
 * @brief Triggers the converter by software trigger.
 *
 * This function is to do the software trigger to the converter. The software trigger can used to start a conversion
 * sequence.
 *
 * @param base          CADC peripheral base address.
 * @param converterMask Mask for converters to be operated. See "_cadc_converter_id".
 */
void CADC_DoSoftwareTriggerConverter(ADC_Type *base, uint16_t converterMask);

/*!
 * @brief Enables the DMA feature.
 *
 * @param base          CADC peripheral base address.
 * @param converterMask Mask for converters to be operated. See "_cadc_converter_id".
 * @param enable        Enable the feature or not.
 */
void CADC_EnableConverterDMA(ADC_Type *base, uint16_t converterMask, bool enable);

/*!
 * @brief Enables the interrupts.
 *
 * @param base CADC peripheral base address.
 * @param mask Mask value for interrupt events. See "_cadc_interrupt_enable".
 */
void CADC_EnableInterrupts(ADC_Type *base, uint16_t mask);

/*!
 * @brief Disables the interrupts.
 *
 * @param base CADC peripheral base address.
 * @param mask Mask value for interrupt events. See "_cadc_interrupt_enable".
 */
void CADC_DisableInterrupts(ADC_Type *base, uint16_t mask);

/*!
 * @brief  Gets the status flags.
 *
 * @param  base CADC peripheral base address.
 *
 * @return      Mask value for the event flags. See "_cadc_status_flags".
 */
uint16_t CADC_GetStatusFlags(ADC_Type *base);

/*!
 * @brief Clears the status flags.
 *
 * @param base  CADC peripheral base address.
 * @param flags Mask value for the event flags to be cleared. See "_cadc_status_flags".
 */
void CADC_ClearStatusFlags(ADC_Type *base, uint16_t flags);

/* @} */

/*!
 * @name Each Sample
 * @{
 */

/*!
 * @brief Configures the sample slot.
 *
 * @param base        CADC peripheral base address.
 * @param sampleIndex Index of sample slot in conversion sequence. Available range is 0-15.
 * @param config      Pointer to configuration structure. See "cadc_sample_config_t"
 */
void CADC_SetSampleConfig(ADC_Type *base, uint16_t sampleIndex, const cadc_sample_config_t *config);

/*!
 * @brief Enables the sample slot.
 *
 * This function is to enable the sample slot. Only the enabled sample slot can join the conversion sequence.
 *
 * @param base       CADC peripheral base address.
 * @param sampleMask Mask value of sample slots in conversion sequence. Each bit is responding to a sample slot.
 * @param enable     Enable the feature or not.
 */
static inline void CADC_EnableSample(ADC_Type *base, uint16_t sampleMask, bool enable)
{
    if (enable)
    {
        base->SDIS &= ~ADC_SDIS_DS(sampleMask);
    }
    else
    {
        base->SDIS |= ADC_SDIS_DS(sampleMask);
    }
}

/*!
 * @brief Enables the interrupt for each sample slot when its result is ready.
 *
 * @param base       CADC peripheral base address.
 * @param sampleMask Mask value of sample slots in conversion sequence. Each bit is responding to a sample slot.
 * @param enable     Enable the feature or not.
 */
static inline void CADC_EnableSampleResultReadyInterrupts(ADC_Type *base, uint16_t sampleMask, bool enable)
{
    if (enable)
    {
        base->SCHLTEN |= ADC_SCHLTEN_SCHLTEN(sampleMask);
    }
    else
    {
        base->SCHLTEN &= ~ADC_SCHLTEN_SCHLTEN(sampleMask);
    }
}

/*!
 * @brief  Returns the sample ready flags of sample slots.
 *
 * @param  base CADC peripheral base address.
 *
 * @return      Mask value for the sample slots if their result are ready.
 */
static inline uint16_t CADC_GetSampleReadyStatusFlags(ADC_Type *base)
{
    return (ADC_RDY_RDY_MASK & base->RDY);
}

/*!
 * @brief  Gets the low limit flags of sample slots.
 *
 * @param  base CADC peripheral base address.
 *
 * @return      Mask value for the sample slots if their results exceed the low limit.
 */
static inline uint16_t CADC_GetSampleLowLimitStatusFlags(ADC_Type *base)
{
    return (ADC_LOLIMSTAT_LLS_MASK & base->LOLIMSTAT);
}

/*!
 * @brief Clears low limit flags of sample slots.
 *
 * @param base       CADC peripheral base address.
 * @param sampleMask Mask value for the sample slots' flags to be cleared.
 */
static inline void CADC_ClearSampleLowLimitStatusFlags(ADC_Type *base, uint16_t sampleMask)
{
    base->LOLIMSTAT = ADC_LOLIMSTAT_LLS(sampleMask);
}

/*!
 * @brief  Gets the high limit flags of sample slots.
 *
 * @param  base CADC peripheral base address.
 *
 * @return      Mask value for the sample slots if their results exceed the high limit.
 */
static inline uint16_t CADC_GetSampleHighLimitStatusFlags(ADC_Type *base)
{
    return (ADC_HILIMSTAT_HLS_MASK & base->HILIMSTAT);
}

/*!
 * @brief Clears the high limit flags of sample slots.
 *
 * @param base       CADC peripheral base address.
 * @param sampleMask Mask value for the sample slots to be cleared flags.
 */
static inline void CADC_ClearSampleHighLimitStatusFlags(ADC_Type *base, uint16_t sampleMask)
{
    base->HILIMSTAT = ADC_HILIMSTAT_HLS(sampleMask);
}

/*!
 * @brief  Gets the zero crossing flags of sample slots.
 *
 * @param  base CADC peripheral base address.
 *
 * @return      Mask value for the sample slots if their results cause the zero crossing event.
 */
static inline uint16_t CADC_GetSampleZeroCrossingStatusFlags(ADC_Type *base)
{
    return (ADC_ZXSTAT_ZCS_MASK & base->ZXSTAT);
}

/*!
 * @brief Clears zero crossing flags of sample slots.
 *
 * @param base       CADC peripheral base address.
 * @param sampleMask Mask value for the sample slots to be cleared flags.
 */
static inline void CADC_ClearSampleZeroCrossingStatusFlags(ADC_Type *base, uint16_t sampleMask)
{
    base->ZXSTAT = ADC_ZXSTAT_ZCS(sampleMask);
}

/*!
 * @brief  Gets the sample result value.
 *
 * This function is to get the sample result value. This returned value keeps it original formation just like in
 * hardware result register. It includes the sign bit as the MSB and 3-bit left shifted value.
 *
 * @param  base        CADC peripheral base address.
 * @param  sampleIndex Index of sample slot.
 *
 * @return             Sample's conversion value.
 */
static inline uint16_t CADC_GetSampleResultValue(ADC_Type *base, uint16_t sampleIndex)
{
    return base->RSLT[sampleIndex];
}

/* @} */

#if defined(__cplusplus)
}
#endif
/*!
 * @}
 */
#endif /* _FSL_CADC_H_ */
