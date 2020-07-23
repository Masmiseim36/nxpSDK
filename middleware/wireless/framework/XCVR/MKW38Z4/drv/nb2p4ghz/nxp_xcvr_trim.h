/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef FSL_XCVR_TRIM_H
/* clang-format off */
#define FSL_XCVR_TRIM_H
/* clang-format on */

#include "fsl_device_registers.h"
#include "fsl_common.h"   
#include "nxp2p4_xcvr.h"

/*!
 * @addtogroup xcvr_trim Radio Trim and Test Routines
 * @{
 */

/************************************************************************************
*************************************************************************************
* Public constant definitions
*************************************************************************************
************************************************************************************/
  

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
/* \brief  The enumerations used to define the I & Q channel selections. */
typedef enum
{
  I_CHANNEL  = 0,
  Q_CHANNEL = 1,
   NUM_I_Q_CHAN = 2
} IQ_t;

#if (0)
typedef enum /* Enumeration of ADC_GAIN_CAL 2 */
{
  NOMINAL2  = 0,
  BBF_NEG = 1,
  BBF_POS = 2,
  TZA_STEP_N0 = 3,
  TZA_STEP_N1 = 4,
  TZA_STEP_N2 = 5,
  TZA_STEP_N3 = 6,
  TZA_STEP_N4 = 7,
  TZA_STEP_N5 = 8,
  TZA_STEP_N6 = 9,
  TZA_STEP_N7 = 10,
  TZA_STEP_N8 = 11,
  TZA_STEP_N9 = 12,
  TZA_STEP_N10 = 13,
  TZA_STEP_P0 = 14,
  TZA_STEP_P1 = 15,
  TZA_STEP_P2 = 16,
  TZA_STEP_P3 = 17,
  TZA_STEP_P4 = 18,
  TZA_STEP_P5 = 19,
  TZA_STEP_P6 = 20,
  TZA_STEP_P7 = 21,
  TZA_STEP_P8 = 22,
  TZA_STEP_P9 = 23,
  TZA_STEP_P10 = 24,
  
  NUM_SWEEP_STEP_ENTRIES2 = 25 /* Including the baseline entry #0. */
} DAC_SWEEP_STEP2_t;
#else  /* refactoring of DAC_SWEEP_STEP2_t */
typedef uint8_t DAC_SWEEP_STEP2_t;
/* Enumeration of ADC_GAIN_CAL 2 */
#define NOMINAL2    (0U)
#define BBF_NEG     (1U)
#define BBF_POS     (2U)
#define TZA_STEP_N0 (3U)
#define TZA_STEP_N1 (4U)
#define TZA_STEP_N2 (5U)
#define TZA_STEP_N3 (6U)
#define TZA_STEP_N4 (7U)
#define TZA_STEP_N5 (8U)
#define TZA_STEP_N6 (9U)
#define TZA_STEP_N7 (10U)
#define TZA_STEP_N8 (11U)
#define TZA_STEP_N9 (12U)
#define TZA_STEP_N10 (13U)
#define TZA_STEP_P0 (14U)
#define TZA_STEP_P1 (15U)
#define TZA_STEP_P2 (16U)
#define TZA_STEP_P3 (17U)
#define TZA_STEP_P4 (18U)
#define TZA_STEP_P5 (19U)
#define TZA_STEP_P6 (20U)
#define TZA_STEP_P7 (21U)
#define TZA_STEP_P8 (22U)
#define TZA_STEP_P9 (23U)
#define TZA_STEP_P10 (24U)
#define NUM_SWEEP_STEP_ENTRIES2 (3U) /* Including the baseline entry #0. */

#endif /* refactoring of DAC_SWEEP_STEP2_t */

/* \brief  Defines an entry in an array of structs to describe TZA DCOC STEP and TZA_DCOC_STEP_RECIPROCAL. */
typedef struct
{
    uint16_t dcoc_step; 
    uint16_t dcoc_step_rcp; 
} TZAdcocstep_t;


typedef struct
{
    int8_t step_value; /* The offset from nominal DAC value (see sweep_step_values[]) */
    int16_t internal_measurement; /* The value (average code) measured from DMA samples. */
} GAIN_CALC_TBL_ENTRY2_T;

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#if defined(__cplusplus)
    extern "C" {
#endif /* __cplusplus */

/*! *********************************************************************************
* \brief  This function captures a set of digital samples of I and Q.
*  An external buffer is required to store the samples captured into Packet RAM before processing.
*
* \param[in] buffer_ptr Pointer to the location for storing num_samples 32bit words for processing.
* \param[in] sz_bytes Buffer size in bytes
* \pre The calling routine must have triggered a RX warmup before calling this function.
* \post This function enables AGC as a side effect. 
***********************************************************************************/      
void dbg_ram_dc_capture(int16_t * buffer_ptr, uint16_t sz_bytes);  
      
/*! *********************************************************************************
* \brief  This function calculates the average (DC value) based on a set of digital samples of I and Q.
*  An external buffer is required to store the samples captured into Packet RAM before processing.
*
* \param[in] i_avg Pointer to the location for storing the calculated average for I channel samples.
* \param[in] q_avg Pointer to the location for storing the calculated average for Q channel samples.
* \param[in] buffer_ptr Pointer to the buffer containing the IQ samples. See dbg_ram_dc_capture function.
* \param[in] num_samples The number of IQ pairs to capture for averaging
* \return  0 if an error occurred, 1 if passed.
***********************************************************************************/
uint8_t rx_dc_sample_average(int16_t * i_avg, int16_t * q_avg, int16_t * buffer_ptr, uint16_t num_samples);

/*!
 * @brief Function to sample the DC_EST register and return the average.
 *
 * This function samples the DC_EST register multiple times and performs an average to get an estimate
 * of the DC offset present in the signal.
 *
 * \param[out] i_avg Pointer to the location for storing the calculated average for I channel samples.
 * \param[out] q_avg Pointer to the location for storing the calculated average for Q channel samples.
 * \param[in] num_samples The number of samples to capture to calculate the average.
 * \return  0 if an error occurred, 1 if passed.
 */ 
uint8_t rx_dc_est_average(int16_t * i_avg, int16_t * q_avg, uint16_t num_samples);

#ifdef SUPPORT_IQ_DAC_TRIM
/*! *********************************************************************************
* @brief  This function performs a trim of the BBA DCOC DAC on the DUT
*
* \return status - 1 if passed, 0 if failed.
*
* \details
*   Requires the RX to be warmed up before this function is called.
*   
***********************************************************************************/
uint8_t rx_bba_dcoc_dac_trim_shortIQ(void);
#endif /* defined SUPPORT_IQ_DAC_TRIM */


/*! *********************************************************************************
* @brief  This function performs a trim of the BBA DCOC DAC on the DUT
*
* \return status - 1 if passed, 0 if failed.
*
* \details
*   Requires the RX to be warmed up before this function is called.
*   Must be performed before performing the ::DCOC_DAC_INIT_Cal() trim.
*   
***********************************************************************************/
uint8_t rx_bba_dcoc_dac_trim_DCest(void);

#ifdef GCOV_DO_COVERAGE /* routine is local except when testing code coverage */
/*! *********************************************************************************
* @brief  This function calculates TZA steps and reciprocals for DCOC DAC trim
*
* \param[in] temp_step - The temporary step value to start the calculation. 
* \param[in] bbf_dcoc_step - The bbf_dcoc_step value to start the calculation. 
* \param[in] tza_dcoc_step - The array to be updated by the calculation (11 elements long). 
* \return status - 1 if passed, 0 if failed.
*
* \details
*   Requires the RX to be warmed up before this function is called.
*   Must be performed after the ::rx_bba_dcoc_dac_trim_DCest() trim.
*   
***********************************************************************************/
uint8_t calc_tza_step_recip(float32_t temp_step, uint32_t bbf_dcoc_step, TZAdcocstep_t tza_dcoc_step[]);
#endif /* defined(GCOV_DO_COVERAGE) */

/*! *********************************************************************************
* @brief  This function performs a slope-sign seek trim on the DAC INIT
*
* \param[in] standalone_operation - boolean parameter indicating whether operation should be 
*  standalone or not. If standalone then RX warmup and warmdown are called internally.
* \return status - 1 if passed, 0 if failed.
*
* \details
*   Requires the RX to be warmed up before this function is called.
*   Must be performed after the ::rx_bba_dcoc_dac_trim_DCest() trim.
*   
***********************************************************************************/
void DCOC_DAC_INIT_Cal (bool standalone_operation);

#if defined(GCOV_DO_COVERAGE) /* routine is local except when testing code coverage */
/*! *********************************************************************************
* @brief  This function checks for DC on I and Q below the limits 
*
* \param[in] i_dc - The measured DC on I channel. 
* \param[in] q_dc - The measured DC on I channel. 
* \return status - true if Dc on either channel is still too high, false if DC on both channels is low enough.
*
* \details
*   Used by DCOC_DAC_INIT_Cal().
*   
***********************************************************************************/
bool dc_is_too_high(int16_t i_dc, int16_t q_dc);
#endif /* !defined(GCOV_DO_COVERAGE) */

/*! *********************************************************************************
* @brief  This function performs a pulse on the RX init 
*
* \details
*   Used during some trim functions to support propagation of new settings.
*   
***********************************************************************************/
void force_rx_init_pulse(void);

/* @} */

#if defined(__cplusplus)
}
#endif


#endif /* FSL_XCVR_TRIM_H */



