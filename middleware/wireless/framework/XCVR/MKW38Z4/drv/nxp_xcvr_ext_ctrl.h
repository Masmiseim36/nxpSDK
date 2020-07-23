/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef NXP_XCVR_EXT_CTRL_H
/* clang-format off */
#define NXP_XCVR_EXT_CTRL_H
/* clang-format on */

#include "fsl_common.h"
#include "nxp2p4_xcvr.h"

/*!
 * @addtogroup xcvr_pa_coex PA, FEM, FAD, Localization and Co-existence Routines
 * @{
 */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifndef TEST_BUILD_COEX
#define TEST_BUILD_COEX     (0)  /* Testing ability to build coex support software */
#endif /* TEST_BUILD_COEX */

#if defined(gMWS_UseCoexistence_d)
#if gMWS_UseCoexistence_d
#include "MWS.h"
#endif /* gMWS_UseCoexistence_d */
#else
#define gMWS_UseCoexistence_d 0
#endif /* defined(gMWS_UseCoexistence_d) */

#if (TEST_BUILD_COEX) /* Only used to test building coexistence without the MWS include file */
#define gMWS_CoexRfActiveAssertTime_d       (100U)
#define gMWS_CoexPrioSignalTime_d           (40U)
/* Supported coexistence models */
#define gMWS_Coex_Status_Prio_d             (0)
#define gMWS_Coex_Prio_Only_d               (1)
/* Coexistence model to be handled */
#ifndef gMWS_Coex_Model_d
#define gMWS_Coex_Model_d    gMWS_Coex_Status_Prio_d
#endif /* gMWS_Coex_Model_d */
#endif /* TEST_BUILD_COEX */

/* Maximum value accepted for some LCL configuration */
#define XCVR_LCL_SAMPLES_MAX_LEN 32U
   
 /*! @brief  Coexistence arbitration priority settings. */
typedef enum
{
    XCVR_COEX_LOW_PRIO  = 0U,
    XCVR_COEX_HIGH_PRIO = 1U
}   XCVR_COEX_PRIORITY_T;

/*! @brief  PA/FEM control mode settings. */
typedef enum
{
    XCVR_ANTX_SINGLE_MODE       = 0U,
    XCVR_ANTX_DUAL_MODE         = 1U
}   XCVR_ANTX_MODE_T;

/*! @brief  PA/FEM GPIO vs TSM settings. */
typedef enum
{
    XCVR_FAD_TSM_GPIO           = 0U,
    XCVR_FAD_STATE_MACHINE      = 1U
}   XCVR_FAD_NOT_GPIO_MODE_T;

/*! @brief  PA/FEM RX/TX polarity settings. */
typedef enum
{
    XCVR_FAD_ACTIVE_HIGH        = 0U,
    XCVR_FAD_ACTIVE_LOW         = 1U
}   XCVR_RX_TX_POLARITY_MODE_T;

/*! @brief  PA/FEM RX/TX polarity settings. */
typedef enum
{
    XCVR_FAD_OVRD_SEL_ANT_A     = 0U,
    XCVR_FAD_OVRD_SEL_ANT_B     = 1U
}   XCVR_FAD_OVRD_ANT_A_B_SEL_MODE_T;

/*!@brief TX & RX PA/FEM function settings for PA/FEM control over coexistence pins.  */
typedef enum
{
    NO_FUNC             = 0U,
    TX_FUNC             = 1U,
    RX_FUNC             = 2U,
    BOTH_TX_RX_FUNC     = 3U
} tx_rx_coex_pin_func_t;

/*! @brief PA/FEM  configuration structure. */
typedef struct
{
    XCVR_ANTX_MODE_T op_mode;                           /*!< operating mode for the PA/FEM interface */
    uint8_t ant_sel_pins_enable;                        /*!< control whether ANT_A & ANT_B pads should be active (0=Disabled, 1=Enabled) */
    uint8_t tx_rx_switch_pins_enable;                   /*!< control whether TX and RX_SWITCH pads should be active (0=Disabled, 1=Enabled) */
    uint8_t high_z_enable;                              /*!< control whether FEM/PA  pads should use High Z (0=Disabled, 1=Enabled) */
    uint8_t use_fad_state_machine;                      /*!< control whether FAD state machine is active (0=Disabled, 1=Enabled) */
    XCVR_FAD_NOT_GPIO_MODE_T ant_a_pad_control;         /*!< control whether ANT_A pad should be controlled by FAD state machine or TSM GPIO  */
    XCVR_FAD_NOT_GPIO_MODE_T ant_b_pad_control;         /*!< control whether ANT_B pad should be controlled by FAD state machine or TSM GPIO  */
    XCVR_FAD_NOT_GPIO_MODE_T tx_switch_pad_control;     /*!< control whether TX_SWITCH pad should be controlled by FAD state machine or TSM GPIO  */
    XCVR_FAD_NOT_GPIO_MODE_T rx_switch_pad_control;     /*!< control whether RX_SWITCH pad should be controlled by FAD state machine or TSM GPIO  */
    uint8_t pa_tx_wu;                                   /*!< Number of usec to start external PA warmup ahead of internal PA warmup (ramp) start  */
    uint8_t pa_tx_wd;                                   /*!< Number of usec to start external PA warmdown ahead of internal PA warmdown (ramp) completion  */
    uint8_t lna_rx_wu;                                  /*!< Number of usec to start external LNA warmup ahead of internal LNA warmup  start  */
    uint8_t lna_rx_wd;                                  /*!< Number of usec to start external LNA warmdown ahead of internal LNA warmdown completion  */
    XCVR_RX_TX_POLARITY_MODE_T tx_switch_pol_control;   /*!< control whether TX_SWITCH pad should be active high or low  (0=Active HIGH, 1=Active LOW) */
    XCVR_RX_TX_POLARITY_MODE_T rx_switch_pol_control;   /*!< control whether RX_SWITCH pad should be active high or low  (0=Active HIGH, 1=Active LOW) */
} xcvr_pa_fem_config_t;
    

/*! @brief status return codes for LCL module. */
typedef enum
{
    gXcvrLclStatusSuccess           = 0U,   /*!< Success */
    gXcvrLclStatusInvalidArgs       = 1U,   /*!< Invalid arguments */
    gXcvrLclStatusFail              = 2U,   /*!< Fail */
} xcvrLclStatus_t;

/*! @brief modes for LCL module. */
typedef enum
{
    gLclRxTxNone        = 0U, /*!< neither RX nor TX mode */
    gLclTxMode          = 1U,   /*!< RX mode */
    gLclRxMode          = 2U,   /*!< TX mode */
    gLclRxTxMode        = 3U, /*!< RX & TX mode */
    gLclRxTxInvalid     = 4U /*!< RX & TX invalid */
} lclRxTxMode_t;

/*! @brief RX trigger types for LCL module. */
typedef enum
{
    lclRxTriggerSoftware        = 0U,    /*!< Software Trigger */
    lclRxTriggerPatternFound    = 1U,    /*!< Localization control: pattern found */
    lclRxTriggerCrcComplete     = 2U,    /*!< CRC Complete */
    lclRxTriggerCrcPass         = 3U,    /*!< CRC Pass */
    lclRxTriggerCtePresent      = 4U,    /*!< GenericLL: cte_present */
    lclRxTriggerAccessAddressFound = 5U, /*!< Gen4 PHY: aa_fnd_to_ll */
    lclRxTriggerInvalid         = 6U     /*!< Invalid */
} lclRxTrigger_t;

/*! @brief TX trigger types for LCL module. */
typedef enum
{
    lclTxTriggerSoftware        = 0U,     /*!< Software Trigger */
    lclTxTriggerPatternFound    = 1U,     /*!< Localization control: pattern found */
    lclTxTriggerCrcComplete     = 2U,     /*!< CRC Complete */
    lclTxTriggerPaWuComplete    = 3U,     /*!< PA Warmup Complete */
    lclTxTriggerRbmeTxDonePre   = 4U,     /*!< RBME TX Done Pre (last bit is out of RBME) */
    lclTxTriggerInvalid         = 5U      /*!< Invalid */
} lclTxTrigger_t;

/*! @brief Antenna Switching configuration structure for LCL module. */
typedef struct
{
    uint16_t numberOfSwitchesInPattern;         /*!< Number of antenna switches in localization pattern */
    bool lantSwInvert;                          /*!< Selects polarity on lant_sw signal */
    bool lantSwWiggle;                          /*!< Enables SW wiggle output on lant_sw signal */
    lclTxTrigger_t txTrig;                      /*!< TX trigger configuration */ 
    lclRxTrigger_t rxTrig;                      /*!< RX trigger configuration */
    
    uint8_t  samplesPerInterval;                /*!< Number of samples per interval */
    uint8_t  lowPeriodDuration;                 /*!< Duration of switching period (low) in intervals */
    uint8_t  highPeriodDuration;                /*!< Duration of sampling period (high) in intervals */
    uint16_t triggerDelay;                      /*!< Delay between trigger and antenna switching start in us */
    uint8_t  triggerOffset;                     /*!< Extra delay between trigger and antenna switching start in samples */    
    
} lclAntennaSwitchingPatternConfig_t;

/*! @brief Pattern Matching Number of Bytes Enumerator. */
typedef enum
{
    lclPatternMatchNumBytes4 = 0U,       /*!< pattern to match 4 bytes long */
    lclPatternMatchNumBytes5 = 1U,       /*!< pattern to match 5 bytes long */
    lclPatternMatchNumBytes6 = 2U,       /*!< pattern to match 6 bytes long */
    lclPatternMatchNumBytes8 = 3U,       /*!< pattern to match 8 bytes long */
    lclPatternMatchNumBytesInvalid = 4U, /*!< Invalid pattern match length */
} lclPatternMatchNumBytes_t;

/*! @brief Pattern matching configuration structure for LCL module. */
typedef struct
{
    lclPatternMatchNumBytes_t  numBytes;         /*!< The pattern can be 4, 5, 6, or 8 bytes long */
    uint8_t  pattern[8];                         /*!< Pattern arrray */
} lclPatternMatchConfig_t;

/*******************************************************************************
 * API
 ******************************************************************************/
xcvrStatus_t XCVR_CoexistenceInit(void);
xcvrStatus_t XCVR_CoexistenceSetPriority(XCVR_COEX_PRIORITY_T rxPriority, XCVR_COEX_PRIORITY_T txPriority);
void XCVR_CoexistenceSaveRestoreTimings(uint8_t saveTimings);

/*!
 * @brief Function to initialize support for external PA and FEM module control.
 *
 * This function initializes the PA and FEM control register and timing registers before TX or RX operations using those modules.
 *
 * @param pa_fem_settings_ptr the pointer to a settings structure for PA/FEM initialization.
 * 
 * @return The status of the init process.
 * 
 */
xcvrStatus_t XCVR_ExternalFadPaFemInit(xcvr_pa_fem_config_t * pa_fem_settings_ptr);

/*!
 * @brief Function to de-initialize support for external PA and FEM module control.
 *
 * This function resets the PA and FEM control register and timing registers to allow them to be used for other purposes if required.
 *
 * @return The status of the init process.
 * 
 */
xcvrStatus_t XCVR_ExternalFadPaFemDeInit(void);

/*!
 * @brief Function to over-ride the antenna selection when FAD is active .
 *
 * This function enables the over-ride of the FAD antenna selection and sets the software requested selection.
 *
 * @return The status of the over-ride process.
 * 
 */
void XCVR_ExtAntOvrd(XCVR_FAD_OVRD_ANT_A_B_SEL_MODE_T antenna_sel);

/*!
 * @brief Function to release any over-ride the antenna selection and return the antenna to  FAD control .
 *
 * This function enables the over-ride of the FAD antenna selection and sets the software requested selection.
 *
 */
void XCVR_ExtAntRelease(void);

/*!
 * @brief Function to initialize support for external PA and FEM module control using Coexistence pins.
 *
 * This function initializes the PA and FEM control register and timing registers before TX or RX operations using those modules. It
 * sets up to use one or both of the RF_STATUS and RF_PRIORITY pins for PA/FEM control instead of coexistence.
 *
 * @param test_settings The pointer to a settings structure for PA/FEM initialization.
 * @param rf_status_func t The function to be applies on the RF_STATUS pin.
 * @param rf_priority_func The function to be applies on the RF_PRIORITY pin.
 * @note
 *  This function is intended to be used when ::XCVR_ExternalFadPaFemInit() cannot be used (if preservation of debugger connection
 *  is required. 
 *  This function does NOT perform any pin muxing settings, that must be performed by a higher layer of software.
 * 
 */
xcvrStatus_t XCVR_FadPaFemOnCoexInit(xcvr_pa_fem_config_t * test_settings, tx_rx_coex_pin_func_t rf_status_func, tx_rx_coex_pin_func_t rf_priority_func);

/*!
 * @brief Function to de-initialize support for external PA and FEM module control over coexistence pins.
 *
 * This function resets the PA and FEM control register and timing registers to allow them to be used for other purposes if required.
 *
 */
void XCVR_FadPaFemOnCoexDeInit(void);


/*!
 * @brief Handles LCL module programming for AoA/AoD antenna switching.
 *
 * @param mode mode (RX/AoA or TX/AoD) for Antenna Switching
 * @param pConfig pointer to configuration structure 
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_AntennaSwitchInit(lclRxTxMode_t mode, lclAntennaSwitchingPatternConfig_t *pConfig);

/*!
 * @brief Read LCL module settings for AoA/AoD antenna switching 
 *
 * @param mode mode (RX/AoA or TX/AoD) for Antenna Switching
 * @param pConfig pointer to configuration structure 
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_AntennaSwitchRead(lclRxTxMode_t mode, lclAntennaSwitchingPatternConfig_t *pConfig);

/*!
 * @brief Enable LCL module settings for AoA/AoD antenna switching 
 *
 * @param enTXmode enable/disable TX mode
 * @param enRXmode enable/disable RX mode
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_AntennaSwitchEn(bool enTXmode, bool enRXmode);

/*!
 * @brief Block RX/TX for AoA/AoD antenna switching 
 *
 * @param blockTXmode block/unblock TX mode
 * @param blockRXmode block/unblock RX mode
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_AntennaSwitchBlock(bool blockTXmode, bool blockRXmode);

/*!
 * @brief Read LCL mode settings for AoA/AoD antenna switching 
 *
 * @param mode mode (RX/AoA or TX/AoD) for Antenna Switching
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_AntennaSwitchModeRead(lclRxTxMode_t *mode);

/*!
 * @brief Handles LCL module programming for patterm matching
 *
 * @param mode mode (RX or TX) for patterm matching 
 * @param pConfig pointer to configuration structure 
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_PatternMatchInit(lclRxTxMode_t mode, lclPatternMatchConfig_t *pConfig);

/*!
 * @brief Read LCL module programming for patterm matching
 *
 * @param pConfig pointer to configuration structure 
 * @return the status of the request, success or invalid parameter
 */
xcvrLclStatus_t XCVR_LCL_PatternMatchRead( lclPatternMatchConfig_t *pConfig );
  
/*!
 * @brief Read LCL module mode for patterm matching
 *
 * @param mode mode (RX or TX) for patterm matching 
 * @return the status of the request, success or invalid parameter
 *
 */
xcvrLclStatus_t XCVR_LCL_PatternMatchModeRead(lclRxTxMode_t *mode);

/*!
 * @brief Enable LCL module mode for patterm matching
 *
 * @param mode mode (RX or TX) for patterm matching 
 *
 */
xcvrLclStatus_t XCVR_LCL_PatternMatchEn(lclRxTxMode_t mode);

/*!
 * @brief Disable LCL module mode for patterm matching
 *
 * @param mode mode (RX or TX) for patterm matching 
 *
 */
xcvrLclStatus_t XCVR_LCL_PatternMatchDisable(lclRxTxMode_t mode);

/*!
 * @brief Forces a manual trigger to the LCL module.
 *
 */
void XCVR_LCL_SwTriggerAssert(void);

/*!
 *
 * @brief This function turns off the localization module.
 *        Affects both antenna switching and pattern matching modes.
 * 
 */
void XCVR_LCL_DeInit(void);

/*!
 * @brief Function read whether the localization module is enabled or not.
 *
 * This function reads the status of the localization module.
 *
 * @return The status of the localization module.
 * 
 */
bool XCVR_LCL_GetEn(void);


#if defined(__cplusplus)
extern "C" {
#endif

/* @} */

#if defined(__cplusplus)
}
#endif

/*! @}*/

#endif /* NXP_XCVR_EXT_CTRL_H */



