/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "nxp2p4_xcvr.h"
#include "nxp_xcvr_trim.h"
#include "dbg_ram_capture.h"
#include "math.h"
#include "nxp_xcvr_common_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define NUM_IQ_SAMPLES_DC_MEASURE   (128U)   /*!< Number of IQ sample pairs to capture for DC measurement */
#define CAL_SETTLE_TIME (10U)                /*!< Count of loop cycles to wait for calibration to settle */

/*! @brief Channel state variables for slope seek algorithm. */
typedef struct
{
    int16_t dc_meas;            /*!< DC measurement value */
    int16_t dc_meas_p;          /*!< Previous DC measurement value */
    uint8_t curr_tza_dac;       /*!< Current TZA DAC value */
    uint8_t curr_bba_dac;       /*!< Current BBA DAC value */
    uint8_t p_tza_dac;          /*!< Prior TZA DAC value */
    uint8_t p_bba_dac;          /*!< Prior BBA DAC  value */
    bool TZA_OK;                /*!< TZA channel sweep complete flag */
    bool BBA_OK;                /*!< BBA channel sweep complete flag */
} xcvr_channel_state_t;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
#ifdef SUPPORT_IQ_DAC_TRIM
static void DC_Measure_short(IQ_t chan, DAC_SWEEP_STEP2_t dcoc_init_val);
#endif /* defined SUPPORT_IQ_DAC_TRIM */
static float32_t calc_dcoc_dac_step(GAIN_CALC_TBL_ENTRY2_T * meas_ptr, GAIN_CALC_TBL_ENTRY2_T * baseline_meas_ptr );
void chan_step_sweep(xcvr_channel_state_t * chan, uint8_t step_num);
void init_chan_state(xcvr_channel_state_t * chan);
void tza_chan_sweep(xcvr_channel_state_t * chan, uint8_t step_num);
void bba_chan_sweep(xcvr_channel_state_t * chan, uint8_t step_num);


/*!
 * @brief Function to implement a delay loop.
 *
 * This function counts 32x the input value in a delay loop. It is dependent on the core clock frequency.
 *
 */ 
static void XcvrCalDelay(uint32_t delaytime);

#if defined(__GNUC__)
/* Define the prototype for roundf() so it links properly */
float32_t roundf( float32_t arg );
#endif

#ifndef GCOV_DO_COVERAGE /* routine is local except when testing code coverage */
static uint8_t calc_tza_step_recip(float32_t temp_step, uint32_t bbf_dcoc_step, TZAdcocstep_t tza_dcoc_step[]);
static bool dc_is_too_high(int16_t i_dc, int16_t q_dc);
#endif /* !defined(GCOV_DO_COVERAGE) */

/*******************************************************************************
 * Variables
 ******************************************************************************/
static GAIN_CALC_TBL_ENTRY2_T measurement_tbl2[NUM_I_Q_CHAN][NUM_SWEEP_STEP_ENTRIES2]; 
static const int8_t sweep_step_values2[NUM_SWEEP_STEP_ENTRIES2] = 
{ 
    0, /* Baseline entry is first and not used in this table */
  -16,
  +16,
};

/*******************************************************************************
 * Macros
 ******************************************************************************/
#if (0)
#define ISIGN(x) !((uint16_t)(x) & 0x8000U)
#else
#define ISIGN(x) (((uint16_t)(x) & 0x8000U) == 0U) /* works as a boolean */
#endif

#define ABS(x) (((x) > 0) ? ((uint16_t)(x)) :((uint16_t) -(x)))

/*******************************************************************************
 * Code
 ******************************************************************************/

#ifdef SUPPORT_IQ_DAC_TRIM
/*! *********************************************************************************
* \brief  This function performs a trim of the BBA DCOC DAC on the DUT
*
* \return status - 1 if passed, 0 if failed.
*
* \ingroup PublicAPIs
*
* \details
*   Requires the RX to be warmed up before this function is called.
*   
***********************************************************************************/
uint8_t rx_bba_dcoc_dac_trim_shortIQ(void)
{
    uint8_t i;
    float32_t temp_mi = 0.0F;
    float32_t temp_mq = 0.0F;
    float32_t temp_pi = 0.0F;
    float32_t temp_pq = 0.0F;
    float32_t temp_step = 0.0F;
    uint8_t bbf_dacinit_i;
    uint8_t bbf_dacinit_q;
    uint32_t dcoc_init_reg_value_dcgain = 0x80802020U; /* Used in 2nd & 3rd Generation DCOC Trims only. */
    uint32_t bbf_dcoc_step;
    uint32_t bbf_dcoc_step_rcp;
    TZAdcocstep_t tza_dcoc_step[11];
    uint8_t status = 0;
    
    /* Save register values. */  
    uint32_t dcoc_ctrl_0_stack;
    uint32_t dcoc_ctrl_1_stack;
    uint32_t agc_ctrl_1_stack;
    uint32_t rx_dig_ctrl_stack;
    uint32_t dcoc_cal_gain_state;
    
    XcvrCalDelay(1000);
    dcoc_ctrl_0_stack = (uint32_t)(XCVR_RX_DIG->DCOC_CTRL_0); /* Save state of DCOC_CTRL_0 for later restore. */
    dcoc_ctrl_1_stack = (uint32_t)(XCVR_RX_DIG->DCOC_CTRL_1); /* Save state of DCOC_CTRL_1 for later restore. */
    rx_dig_ctrl_stack =  (uint32_t)(XCVR_RX_DIG->RX_DIG_CTRL); /* Save state of RX_DIG_CTRL for later restore. */
    agc_ctrl_1_stack = (uint32_t)(XCVR_RX_DIG->AGC_CTRL_1); /* Save state of RX_DIG_CTRL for later restore. */
    dcoc_cal_gain_state = (uint32_t)(XCVR_RX_DIG->DCOC_CAL_GAIN); /* Save state of DCOC_CAL_GAIN for later restore. */

    /* Ensure AGC, DCOC and RX_DIG_CTRL is in correct mode. */
    XCVR_RX_DIG->RX_DIG_CTRL &= ~(XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN_MASK) ;             /* Turn OFF AGC */
    XCVR_RX_DIG->AGC_CTRL_1 |= (XCVR_RX_DIG_AGC_CTRL_1_USER_LNA_GAIN_EN_MASK |  /* Set LNA Manual Gain */
                        XCVR_RX_DIG_AGC_CTRL_1_USER_BBA_GAIN_EN_MASK);   /* Set BBA Manual Gain */
    XCVR_RX_DIG->RX_DIG_CTRL &= ~(XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_CAL_EN_MASK);  /* Enable HW DC Calibration -- Disable for SW-DCOC */
    XCVR_RX_DIG->DCOC_CTRL_0 |= XCVR_RX_DIG_DCOC_CTRL_0_DCOC_MAN_MASK;           /* Enable  Manual DCOC */
    /* DCOC_CTRL_0 @ 4005_C02C -- Define default DCOC DAC settings in manual mode. */
    XCVR_RX_DIG->DCOC_DAC_INIT = XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(0x20) | 
                          XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(0x20) |  
                          XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(0x80) | 
                          XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(0x80);
    /* Set DCOC Tracking State. */
    XCVR_RX_DIG->DCOC_CTRL_0 &= ~(XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_SRC_MASK); /* Disables DCOC Tracking when set to 0 */
    /* Apply Manual Gain. */
    XCVR_RX_DIG->AGC_CTRL_1 = XCVR_RX_DIG_AGC_CTRL_1_USER_LNA_GAIN_EN(1) | 
                          XCVR_RX_DIG_AGC_CTRL_1_USER_BBA_GAIN_EN(1) | 
                          XCVR_RX_DIG_AGC_CTRL_1_LNA_USER_GAIN(0x02) | 
                          XCVR_RX_DIG_AGC_CTRL_1_BBA_USER_GAIN(0x00) ;
    XcvrCalDelay(CAL_SETTLE_TIME);

    dcoc_init_reg_value_dcgain = XCVR_RX_DIG->DCOC_DAC_INIT; /* Capture DC null setting. */
    bbf_dacinit_i = (uint8_t)(dcoc_init_reg_value_dcgain & 0x000000FFU);
    bbf_dacinit_q = (uint8_t)((dcoc_init_reg_value_dcgain & 0x0000FF00U)>>8);

    DC_Measure_short(I_CHANNEL, NOMINAL2);
    DC_Measure_short(Q_CHANNEL, NOMINAL2);

    /* SWEEP Q CHANNEL */
    /* BBF NEG STEP */
    XCVR_RX_DIG->DCOC_DAC_INIT = (XCVR_RX_DIG->DCOC_DAC_INIT & ~XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q_MASK) | XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q((uint32_t)bbf_dacinit_q-16UL);
    XcvrCalDelay(CAL_SETTLE_TIME);
    DC_Measure_short(Q_CHANNEL, BBF_NEG);

    /* BBF POS STEP */
    XCVR_RX_DIG->DCOC_DAC_INIT = (XCVR_RX_DIG->DCOC_DAC_INIT & ~XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q_MASK) | XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q((uint32_t)bbf_dacinit_q+16UL);
    XcvrCalDelay(CAL_SETTLE_TIME);
    DC_Measure_short(Q_CHANNEL, BBF_POS);

    XCVR_RX_DIG->DCOC_DAC_INIT = dcoc_init_reg_value_dcgain; /* Return DAC setting to initial. */
    XcvrCalDelay(CAL_SETTLE_TIME);  

    /* SWEEP I CHANNEL */
    /* BBF NEG STEP */
    XCVR_RX_DIG->DCOC_DAC_INIT = (XCVR_RX_DIG->DCOC_DAC_INIT & ~XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I_MASK) | XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I((uint32_t)bbf_dacinit_i-16UL); 
    XcvrCalDelay(CAL_SETTLE_TIME);
    DC_Measure_short(I_CHANNEL, BBF_NEG);
    /* BBF POS STEP  */
    XCVR_RX_DIG->DCOC_DAC_INIT = (XCVR_RX_DIG->DCOC_DAC_INIT & ~XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I_MASK) | XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I((uint32_t)bbf_dacinit_i+16UL); 
    XcvrCalDelay(CAL_SETTLE_TIME);
    DC_Measure_short(I_CHANNEL, BBF_POS);

    XCVR_RX_DIG->DCOC_DAC_INIT = dcoc_init_reg_value_dcgain; /* Return DACs to initial. */
    XcvrCalDelay(CAL_SETTLE_TIME);

    /* Calculate BBF DCOC STEPS, RECIPROCALS */
    temp_mi = calc_dcoc_dac_step(&measurement_tbl2[I_CHANNEL][BBF_NEG], &measurement_tbl2[I_CHANNEL][NOMINAL2]);
    temp_mq = calc_dcoc_dac_step(&measurement_tbl2[Q_CHANNEL][BBF_NEG], &measurement_tbl2[Q_CHANNEL][NOMINAL2]);
    temp_pi = calc_dcoc_dac_step(&measurement_tbl2[I_CHANNEL][BBF_POS], &measurement_tbl2[I_CHANNEL][NOMINAL2]);
    temp_pq = calc_dcoc_dac_step(&measurement_tbl2[Q_CHANNEL][BBF_POS], &measurement_tbl2[Q_CHANNEL][NOMINAL2]);

    temp_step = (temp_mi+temp_pi+temp_mq+temp_pq)/4.0F;

    bbf_dcoc_step = (uint32_t)roundf(temp_step*8.0F);
    if ((bbf_dcoc_step>265U) && (bbf_dcoc_step<305U)) 
    {
        bbf_dcoc_step_rcp = (uint32_t)roundf((float32_t)0x8000U/temp_step);
        /* Calculate TZA DCOC STEPS & RECIPROCALS and IQ_DC_GAIN_MISMATCH. */
        for (i = TZA_STEP_N0; i <= TZA_STEP_N10; i++) /* Relying on enumeration ordering. */
        {
            float32_t this_step;
            float32_t last_step;
            uint32_t temp32;
            /* Calculate TZA DCOC STEPSIZE & its RECIPROCAL. */
            switch(i)
            {
              case TZA_STEP_N0: 
                this_step = (float32_t)(((float32_t)bbf_dcoc_step)/8.0F);
                last_step = 3.6F;
                break;
              case TZA_STEP_N1:
                temp32 = (xcvr_common_config.dcoc_tza_step_1>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_0>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N2: 
                temp32 = (xcvr_common_config.dcoc_tza_step_2>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_1>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N3: 
                temp32 = (xcvr_common_config.dcoc_tza_step_3>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_2>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N4: 
                temp32 = (xcvr_common_config.dcoc_tza_step_4>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_3>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N5: 
                temp32 = (xcvr_common_config.dcoc_tza_step_5>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_4>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N6: 
                temp32 = (xcvr_common_config.dcoc_tza_step_6>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_5>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N7: 
                temp32 = (xcvr_common_config.dcoc_tza_step_7>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_6>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N8: 
                temp32 = (xcvr_common_config.dcoc_tza_step_8>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_7>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N9: 
                temp32 = (xcvr_common_config.dcoc_tza_step_9>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_8>>16);
                last_step = (float32_t)temp32;
                break;
              case TZA_STEP_N10: 
                temp32 = (xcvr_common_config.dcoc_tza_step_10>>16);
                this_step = (float32_t)temp32;
                temp32 = (xcvr_common_config.dcoc_tza_step_9>>16);
                last_step = (float32_t)temp32;
                break;
              default:
                  this_step = (((float32_t)bbf_dcoc_step/8.0F));
                  last_step = 3.6F;
                break;  
          }
          temp_step = temp_step * (this_step/last_step);
          
          tza_dcoc_step[i-TZA_STEP_N0].dcoc_step = (uint16_t)roundf(temp_step*8.0F);
          tza_dcoc_step[i-TZA_STEP_N0].dcoc_step_rcp = (uint16_t)roundf((float32_t)0x8000/temp_step);
        }

        /* Make the trims active. */
        XCVR_RX_DIG->DCOC_BBA_STEP = XCVR_RX_DIG_DCOC_BBA_STEP_BBA_DCOC_STEP(bbf_dcoc_step) | XCVR_RX_DIG_DCOC_BBA_STEP_BBA_DCOC_STEP_RECIP(bbf_dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_0 = XCVR_RX_DIG_DCOC_TZA_STEP_0_DCOC_TZA_STEP_GAIN_0(tza_dcoc_step[0].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_0_DCOC_TZA_STEP_RCP_0(tza_dcoc_step[0].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_1 = XCVR_RX_DIG_DCOC_TZA_STEP_1_DCOC_TZA_STEP_GAIN_1(tza_dcoc_step[1].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_1_DCOC_TZA_STEP_RCP_1(tza_dcoc_step[1].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_2 = XCVR_RX_DIG_DCOC_TZA_STEP_2_DCOC_TZA_STEP_GAIN_2(tza_dcoc_step[2].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_2_DCOC_TZA_STEP_RCP_2(tza_dcoc_step[2].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_3 = XCVR_RX_DIG_DCOC_TZA_STEP_3_DCOC_TZA_STEP_GAIN_3(tza_dcoc_step[3].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_3_DCOC_TZA_STEP_RCP_3(tza_dcoc_step[3].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_4 = XCVR_RX_DIG_DCOC_TZA_STEP_4_DCOC_TZA_STEP_GAIN_4(tza_dcoc_step[4].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_4_DCOC_TZA_STEP_RCP_4(tza_dcoc_step[4].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_5 = XCVR_RX_DIG_DCOC_TZA_STEP_5_DCOC_TZA_STEP_GAIN_5(tza_dcoc_step[5].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_5_DCOC_TZA_STEP_RCP_5(tza_dcoc_step[5].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_6 = XCVR_RX_DIG_DCOC_TZA_STEP_6_DCOC_TZA_STEP_GAIN_6(tza_dcoc_step[6].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_6_DCOC_TZA_STEP_RCP_6(tza_dcoc_step[6].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_7 = XCVR_RX_DIG_DCOC_TZA_STEP_7_DCOC_TZA_STEP_GAIN_7(tza_dcoc_step[7].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_7_DCOC_TZA_STEP_RCP_7(tza_dcoc_step[7].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_8 = XCVR_RX_DIG_DCOC_TZA_STEP_8_DCOC_TZA_STEP_GAIN_8(tza_dcoc_step[8].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_8_DCOC_TZA_STEP_RCP_8(tza_dcoc_step[8].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_9 = XCVR_RX_DIG_DCOC_TZA_STEP_9_DCOC_TZA_STEP_GAIN_9(tza_dcoc_step[9].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_9_DCOC_TZA_STEP_RCP_9(tza_dcoc_step[9].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_10 = XCVR_RX_DIG_DCOC_TZA_STEP_10_DCOC_TZA_STEP_GAIN_10(tza_dcoc_step[10].dcoc_step) | XCVR_RX_DIG_DCOC_TZA_STEP_10_DCOC_TZA_STEP_RCP_10(tza_dcoc_step[10].dcoc_step_rcp) ;

        status = 1; /* Success */
    }
    else
    {
        status = 0; /* Failure */
    }

    /* Restore Registers. */
    XCVR_RX_DIG->DCOC_CTRL_0 = dcoc_ctrl_0_stack;              /* Restore DCOC_CTRL_0 state to prior settings. */
    XCVR_RX_DIG->DCOC_CTRL_1 = dcoc_ctrl_1_stack;              /* Restore DCOC_CTRL_1 state to prior settings. */
    XCVR_RX_DIG->RX_DIG_CTRL = rx_dig_ctrl_stack;              /* Restore RX_DIG_CTRL state to prior settings. */
    XCVR_RX_DIG->DCOC_CAL_GAIN = dcoc_cal_gain_state;          /* Restore DCOC_CAL_GAIN state to prior setting. */
    XCVR_RX_DIG->AGC_CTRL_1 = agc_ctrl_1_stack;                /* Save state of RX_DIG_CTRL for later restore. */
    
    return status;
    
}


/*! *********************************************************************************
* \brief  This function performs one point of the DC GAIN calibration process on the DUT
*
* \param[in] chan - whether the I or Q channel is being tested.
* \param[in] dcoc_init_val - the value being set in the ***DCOC_INIT_* register by the parent.
*
* \details
*   Relies on a static array to store each point of data for later processing in ::DC_GainCalc().
*   
***********************************************************************************/
static void DC_Measure_short(IQ_t chan, DAC_SWEEP_STEP2_t dcoc_init_val)
{
    int16_t dc_meas_i = 0;
    int16_t dc_meas_q = 0;
    int16_t sample_buffer[2*NUM_IQ_SAMPLES_DC_MEASURE];

    /* Capture I and Q data (smaller number of samples) */
    dbg_ram_dc_capture(&sample_buffer[0], (4U*NUM_IQ_SAMPLES_DC_MEASURE));

    /* Average IQ samples with DC offset */
    rx_dc_sample_average(&dc_meas_i, &dc_meas_q, &sample_buffer[0], NUM_IQ_SAMPLES_DC_MEASURE);

    /* Set sweep step value */
    measurement_tbl2[chan][dcoc_init_val].step_value = sweep_step_values2[dcoc_init_val];

    /* Set the corresponding DC measurement value */
    if (chan == I_CHANNEL)
    {
        measurement_tbl2[chan][dcoc_init_val].internal_measurement = dc_meas_i;
    }
    else
    {
        measurement_tbl2[chan][dcoc_init_val].internal_measurement = dc_meas_q;
    }
}  
#endif /* defined SUPPORT_IQ_DAC_TRIM */

/*! *********************************************************************************
* \brief  This function calculates one point of DC DAC step based on digital samples of I or Q.
*
* \param[in] meas_ptr - pointer to the structure containing the measured data from internal measurement.
* \param[in] baseline_meas_ptr - pointer to the structure containing the baseline measured data from internal measurement.
*
* \return result of the calculation, the measurement DCOC DAC step value for this measurement point.
*   
***********************************************************************************/
static float32_t calc_dcoc_dac_step(GAIN_CALC_TBL_ENTRY2_T * meas_ptr, GAIN_CALC_TBL_ENTRY2_T * baseline_meas_ptr )
{
    static float32_t dc_step = 1.0F;
    static int16_t norm_dc_code;

    /* Normalize internal measurement */
    norm_dc_code = meas_ptr->internal_measurement-baseline_meas_ptr->internal_measurement;
    dc_step = (float32_t)(norm_dc_code)/(float32_t)(meas_ptr->step_value);
    dc_step = (dc_step < 0.0F)? -dc_step: dc_step; /* Steps are always positive */

    return dc_step;
}


/*! *********************************************************************************
* \brief  Temporary delay function 
*
* \param[in] none.
*
* \return none.
*
* \details
*
***********************************************************************************/
static void XcvrCalDelay(uint32_t delaytime)
{
    uint32_t count = delaytime*32U;
    while(count > 0U) /* Time delay is roughly in uSec. */
    {
        count--;
    }
}    

void dbg_ram_dc_capture(int16_t * buffer_ptr, uint16_t sz_bytes)
{
    dbgRamStatus_t status;
    static dbg_ram_trigger_config_t dc_trigger_config =
    {
        .start_trig = NO_START_TRIG,/*!< The start trigger to cause samples to be captured. */
        .stop_trig = NO_STOP_TRIG, /*!< The stop trigger to cause samples capture to end. */
        .start_delay = 0U,             /*!< The delay time to be inserted after a start trigger, before samples are captured. */
        .decimation = DBG_RAM_NO_DECIMATE,   /*!< The decimation factor to be applied during sample capture. */
        .start_capture_edge = DBG_RAM_EDGE_RISING,  /*!< The edge on which start trigger should be applied (rising/falling). */
        .stop_capture_edge = DBG_RAM_EDGE_RISING,  /*!< The edge on which stop trigger should be applied (rising/falling). */
    };
    static dbg_ram_capture_config_t dc_capture_config;
    dc_capture_config.buffer_sz_bytes = sz_bytes;       /*!< Number of bytes to be captured. */   
    dc_capture_config.result_buffer = buffer_ptr;       /*!< The location in system RAM where the results should be copied. */
    dc_capture_config.dbg_ram_start_addr = (int16_t *)TX_PACKET_RAM_BASE;  /*!< The start location in DBG RAM where radio samples are captured. */
      
    /* Assume this has been called *AFTER* RxWu has completed. */
  
    /* Wait for TSM to reach the end of warmup (unless you want to capture some samples during DCOC cal phase) */
    XCVR_WaitRxWu();
    
    status = dbg_ram_init(&dc_capture_config);
    (void)status;
    /* Argument below is # of bytes, so *2 (I+Q) and *2 (2bytes/sample) */
    status = dbg_ram_start_capture(DBG_RAM_PAGE_RXDIGIQ_ALT, &dc_trigger_config);
    (void)status;
    dbg_ram_wait_for_complete();
    status = dbg_ram_postproc_capture(&dc_capture_config);
    (void)status;
    dbg_ram_release();
}


uint8_t rx_dc_sample_average(int16_t * i_avg, int16_t * q_avg, int16_t * buffer_ptr, uint16_t num_samples)
{
    uint16_t i;    
    float32_t avg_i = 0.0F;
    float32_t avg_q = 0.0F;
    uint8_t status;
    
    if ((buffer_ptr != NULLPTR) && (i_avg != NULLPTR) && (q_avg != NULLPTR))    
    {
        // loop over number of samples
        for (i=0; i < num_samples; i++)
        {
          avg_i += (float32_t) ( *(buffer_ptr + i*2U    ));  // sum I
          avg_q += (float32_t) ( *(buffer_ptr + i*2U + 1U));  // sum Q
        }
        
        // divide sum by number of samples
        avg_i = avg_i / (float32_t) num_samples;
        avg_q = avg_q / (float32_t) num_samples;
      
        *i_avg=(int16_t)avg_i;
        *q_avg=(int16_t)avg_q;
        status = 1U;
    }
    else
    {
        status = 0U;
    }

    return status;
}

uint8_t rx_dc_est_average(int16_t * i_avg, int16_t * q_avg, uint16_t num_samples)
{
    float32_t avg_i = 0.0F;
    float32_t avg_q = 0.0F;
    uint16_t i = 0;
    uint16_t temp16;
    static uint32_t dc_temp;
    static int16_t dc_meas_i = 0;
    static int16_t dc_meas_q = 0;
    uint8_t status;
        
    if ((i_avg != NULLPTR) && (q_avg != NULLPTR))    
    {
        /* Capture DC_EST register values */
        /* Calculate the average of the I and Q channels from DC_EST and return the values via pointers */
        /* Wait for TSM to reach the end of warmup (unless you want to capture some samples during DCOC cal phase). */
        XCVR_WaitRxWu();
        
        /* Read DCOC DC EST register. */
        for ( i=0 ; i<num_samples; i++)
        {
            dc_temp = XCVR_RX_DIG->DCOC_DC_EST;
            temp16 = (uint16_t)(dc_temp & XCVR_RX_DIG_DCOC_DC_EST_DC_EST_I_MASK); /*  Get lower 16 bits of sample */
            temp16 |= (((temp16 & 0x800U) == 0x800U) ? 0xF000U : 0x0U); /* Sign extend from 12 to 16 bits. */
            dc_meas_i = (int16_t)(temp16); /* Make it signed */
            avg_i +=  (float32_t) dc_meas_i; /* Running total for I samples */

            temp16 = (uint16_t)((dc_temp & XCVR_RX_DIG_DCOC_DC_EST_DC_EST_Q_MASK) >> XCVR_RX_DIG_DCOC_DC_EST_DC_EST_Q_SHIFT); /*  Get upper 16 bits of sample */
            temp16 |= (((temp16 & 0x800U) == 0x800U) ? 0xF000U : 0x0U); /* Sign extend from 12 to 16 bits. */
            dc_meas_q = (int16_t)(temp16); /* Make it signed */
            avg_q += (float32_t) dc_meas_q; /* Running total for Q samples */
        }
        avg_i /= (float32_t) num_samples; /* Convert running total to average for I channel */
        avg_q /= (float32_t) num_samples; /* Convert running total to average for Q channel */
        
        *i_avg=(int16_t)avg_i;
        *q_avg=(int16_t)avg_q;
        status = 1U;
    }
    else
    {
        status = 0U;
    }

    return status;
}

uint8_t rx_bba_dcoc_dac_trim_DCest(void) 
{

    float32_t temp_mi = 0.0F;
    float32_t temp_mq = 0.0F;
    float32_t temp_pi = 0.0F;
    float32_t temp_pq = 0.0F;
    float32_t temp_step = 0.0F;

    uint32_t bbf_dcoc_step;
    TZAdcocstep_t tza_dcoc_step[11];
    uint8_t status = 0;

    uint8_t bbf_dacinit_i, bbf_dacinit_q;
    uint8_t tza_dacinit_i, tza_dacinit_q;
    int16_t dc_meas_i = 0;
    int16_t dc_meas_q = 0;
    uint32_t dcoc_init_reg_value_dcgain = 0x80802020U; /* Used in 2nd & 3rd Generation DCOC Trims only */
    uint32_t temp;    

    uint32_t dcoc_ctrl_0_stack;
    uint32_t dcoc_ctrl_1_stack;
    uint32_t agc_ctrl_1_stack;
    uint32_t rx_dig_ctrl_stack;
    uint32_t dcoc_cal_gain_state;
     /* Trim the DCOC DAC step sizes */
   
    /* Save register */
    dcoc_ctrl_0_stack = XCVR_RX_DIG->DCOC_CTRL_0; /* Save state of DCOC_CTRL_0 for later restore */
    dcoc_ctrl_1_stack = XCVR_RX_DIG->DCOC_CTRL_1; /* Save state of DCOC_CTRL_1 for later restore */
    rx_dig_ctrl_stack =  XCVR_RX_DIG->RX_DIG_CTRL; /* Save state of RX_DIG_CTRL for later restore */
    agc_ctrl_1_stack = XCVR_RX_DIG->AGC_CTRL_1; /* Save state of RX_DIG_CTRL for later restore */
    dcoc_cal_gain_state = XCVR_RX_DIG->DCOC_CAL_GAIN; /* Save state of DCOC_CAL_GAIN for later restore */

    /* Register config */
    /* Ensure AGC, DCOC and RX_DIG_CTRL is in correct mode */
    temp = XCVR_RX_DIG->RX_DIG_CTRL;
    temp &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN_MASK; /* Turn OFF AGC */
    temp &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_CAL_EN_MASK; /* Disable for SW control of DCOC */
    temp &= ~XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN_MASK; /* Disable for SW control of DCOC */
    XCVR_RX_DIG->RX_DIG_CTRL = temp;

    XCVR_RX_DIG->AGC_CTRL_1 = XCVR_RX_DIG_AGC_CTRL_1_USER_LNA_GAIN_EN(1UL) | /* Enable LNA Manual Gain */
                         XCVR_RX_DIG_AGC_CTRL_1_USER_BBA_GAIN_EN(1UL) | /* Enable BBA Manual Gain */
                         XCVR_RX_DIG_AGC_CTRL_1_LNA_USER_GAIN(0x0UL) | /* Set LNA Manual Gain */
                         XCVR_RX_DIG_AGC_CTRL_1_BBA_USER_GAIN(0x0UL); /* Set BBA Manual Gain */

    /* DCOC_CTRL_0 @ 4005_C02C -- Define default DCOC DAC settings in manual mode */
    temp = XCVR_RX_DIG->DCOC_CTRL_0;
    temp |=  XCVR_RX_DIG_DCOC_CTRL_0_DCOC_MAN(1UL) | /* Enable  Manual DCOC */
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_SRC(1UL) | /* Ensure DCOC Tracking is enabled */
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_TRK_EST_OVR(1UL) | /* Enable DC Estimator */
                XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_EN(1UL); /* Ensure DC correction is enabled */
    temp &=~ XCVR_RX_DIG_DCOC_CTRL_0_TRACK_FROM_ZERO_MASK; /* Ensure track from zero is disabled during the estimation */
    XCVR_RX_DIG->DCOC_CTRL_0 = temp;

    XCVR_RX_DIG->DCOC_DAC_INIT = XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(0x20) | 
                           XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(0x20) |  
                           XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(0x80) | 
                           XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(0x80);

    /* Clear any prior DC compensation from tracking block */
    XCVR_RX_DIG->DCOC_DIG_MAN = 0x0U;
    force_rx_init_pulse();
    XcvrCalDelay(CAL_SETTLE_TIME*4U);

    /* Set default DCOC DAC INIT Value */
    dcoc_init_reg_value_dcgain = XCVR_RX_DIG->DCOC_DAC_INIT; /* Store DCOC DAC INIT values */
    bbf_dacinit_i = (uint8_t)(dcoc_init_reg_value_dcgain & 0x000000FFU);
    bbf_dacinit_q = (uint8_t)((dcoc_init_reg_value_dcgain & 0x0000FF00U)>>8);
    tza_dacinit_i = (uint8_t)((dcoc_init_reg_value_dcgain & 0x00FF0000U)>>16);
    tza_dacinit_q = (uint8_t)(dcoc_init_reg_value_dcgain >> 24);

    XcvrCalDelay(CAL_SETTLE_TIME*4U); 
    status = rx_dc_est_average(&dc_meas_i, &dc_meas_q, 64);
    (void)status;
    measurement_tbl2[I_CHANNEL][NOMINAL2].step_value = sweep_step_values2[NOMINAL2];
    measurement_tbl2[Q_CHANNEL][NOMINAL2].step_value = sweep_step_values2[NOMINAL2];
    measurement_tbl2[I_CHANNEL][NOMINAL2].internal_measurement = dc_meas_i;
    measurement_tbl2[Q_CHANNEL][NOMINAL2].internal_measurement = dc_meas_q;

    /* SWEEP I/Q CHANNEL */
    /* BBF NEG STEP */
    XCVR_RX_DIG->DCOC_DAC_INIT =  XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(bbf_dacinit_i-16UL) |
                             XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(bbf_dacinit_q-16UL) |
                             XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(tza_dacinit_i) |
                             XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(tza_dacinit_q);
    XcvrCalDelay(CAL_SETTLE_TIME*2U);
    status = rx_dc_est_average(&dc_meas_i, &dc_meas_q, 64);
    (void)status;
    measurement_tbl2[I_CHANNEL][BBF_NEG].step_value = -16;
    measurement_tbl2[Q_CHANNEL][BBF_NEG].step_value = -16;
    measurement_tbl2[I_CHANNEL][BBF_NEG].internal_measurement = dc_meas_i;
    measurement_tbl2[Q_CHANNEL][BBF_NEG].internal_measurement = dc_meas_q;


    /* BBF POS STEP */
    XCVR_RX_DIG->DCOC_DAC_INIT =  XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(bbf_dacinit_i+16UL) |
                             XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(bbf_dacinit_q+16UL) |
                             XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(tza_dacinit_i) |
                             XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(tza_dacinit_q);
    XcvrCalDelay(CAL_SETTLE_TIME*2U);
    status = rx_dc_est_average(&dc_meas_i, &dc_meas_q, 64);
    (void)status;
    measurement_tbl2[I_CHANNEL][BBF_POS].step_value = +16;
    measurement_tbl2[Q_CHANNEL][BBF_POS].step_value = +16;
    measurement_tbl2[I_CHANNEL][BBF_POS].internal_measurement = dc_meas_i;
    measurement_tbl2[Q_CHANNEL][BBF_POS].internal_measurement = dc_meas_q;

    XCVR_RX_DIG->DCOC_DAC_INIT = dcoc_init_reg_value_dcgain; /* Return DAC setting to initial */

    /* Calculate BBF DCOC STEPS, RECIPROCALS */
    temp_mi = calc_dcoc_dac_step(&measurement_tbl2[I_CHANNEL][BBF_NEG], &measurement_tbl2[I_CHANNEL][NOMINAL2]);
    temp_mq = calc_dcoc_dac_step(&measurement_tbl2[Q_CHANNEL][BBF_NEG], &measurement_tbl2[Q_CHANNEL][NOMINAL2]);
    temp_pi = calc_dcoc_dac_step(&measurement_tbl2[I_CHANNEL][BBF_POS], &measurement_tbl2[I_CHANNEL][NOMINAL2]);
    temp_pq = calc_dcoc_dac_step(&measurement_tbl2[Q_CHANNEL][BBF_POS], &measurement_tbl2[Q_CHANNEL][NOMINAL2]);
    temp_step = (temp_mi+temp_pi+temp_mq+temp_pq)/4.0F;
    bbf_dcoc_step = (uint32_t)roundf(temp_step*8.0F);

    status = calc_tza_step_recip(temp_step, bbf_dcoc_step, &tza_dcoc_step[0]);

    /* Restore Registers */
    XCVR_RX_DIG->DCOC_CTRL_0 = dcoc_ctrl_0_stack; /* Restore DCOC_CTRL_0 state to prior settings */
    XCVR_RX_DIG->DCOC_CTRL_1 = dcoc_ctrl_1_stack; /* Restore DCOC_CTRL_1 state to prior settings */
    XCVR_RX_DIG->RX_DIG_CTRL = rx_dig_ctrl_stack; /* Restore RX_DIG_CTRL state to prior settings */
    XCVR_RX_DIG->DCOC_CAL_GAIN = dcoc_cal_gain_state; /* Restore DCOC_CAL_GAIN state to prior setting */
    XCVR_RX_DIG->AGC_CTRL_1 = agc_ctrl_1_stack; /* Save state of RX_DIG_CTRL for later restore */

    return status;
}


#if !defined(GCOV_DO_COVERAGE) /* routine is local except when testing code coverage */
static uint8_t calc_tza_step_recip(float32_t temp_step, uint32_t bbf_dcoc_step, TZAdcocstep_t tza_dcoc_step[])
#else
uint8_t calc_tza_step_recip(float32_t temp_step, uint32_t bbf_dcoc_step, TZAdcocstep_t tza_dcoc_step[])
#endif /* !defined(GCOV_DO_COVERAGE) */
{
    uint8_t status = 0U;
    uint8_t i;
    uint32_t bbf_dcoc_step_rcp;
    if ((bbf_dcoc_step>250U) && (bbf_dcoc_step<305U)) 
    {
        bbf_dcoc_step_rcp = (uint32_t)roundf((float32_t)0x8000U/temp_step);
        /* Calculate TZA DCOC STEPS & RECIPROCALS and IQ_DC_GAIN_MISMATCH */
        for (i = TZA_STEP_N0; i <= TZA_STEP_N10; i++)
        {
            float32_t this_step = 1.0F; /* Ensure the local variable is initialized */
            float32_t last_step = 28.8F;  /* Ensure the local variable is initialized */
            uint32_t temp32;
           /* Calculate TZA DCOC STEPSIZE & its RECIPROCAL */
           switch(i){
             case TZA_STEP_N0: 
               temp_step = (float32_t)bbf_dcoc_step;
               this_step = (float32_t)1.0;
               last_step =  28.8F; /* implementing division by 8 then division by 3.6 (8*3.6=28.8) in one step */
               break;
             case TZA_STEP_N1: 
               temp32 = (xcvr_common_config.dcoc_tza_step_1>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_0>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N2: 
               temp32 = (xcvr_common_config.dcoc_tza_step_2>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_1>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N3: 
               temp32 = (xcvr_common_config.dcoc_tza_step_3>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_2>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N4: 
               temp32 = (xcvr_common_config.dcoc_tza_step_4>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_3>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N5: 
               temp32 = (xcvr_common_config.dcoc_tza_step_5>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_4>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N6: 
               temp32 = (xcvr_common_config.dcoc_tza_step_6>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_5>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N7: 
               temp32 = (xcvr_common_config.dcoc_tza_step_7>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_6>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N8: 
               temp32 = (xcvr_common_config.dcoc_tza_step_8>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_7>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N9: 
               temp32 = (xcvr_common_config.dcoc_tza_step_9>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_8>>16);
               last_step = (float32_t)temp32;
               break;
             case TZA_STEP_N10: 
               temp32 = (xcvr_common_config.dcoc_tza_step_10>>16);
               this_step = (float32_t)temp32;
               temp32 = (xcvr_common_config.dcoc_tza_step_9>>16);
               last_step = (float32_t)temp32;
               break;
             default:
//               this_step = ((float32_t)bbf_dcoc_step/8.0F);
//               last_step = 3.6F;
               break;
          }
          temp_step = temp_step * (this_step/last_step); /* update temp_step every time through the loop */
     
          tza_dcoc_step[i-TZA_STEP_N0].dcoc_step = (uint16_t)roundf(temp_step*8.0F);
          tza_dcoc_step[i-TZA_STEP_N0].dcoc_step_rcp = (uint16_t)roundf((float32_t)0x8000/temp_step);
        }
        /* Make the trims active */
        XCVR_RX_DIG->DCOC_BBA_STEP = XCVR_RX_DIG_DCOC_BBA_STEP_BBA_DCOC_STEP(bbf_dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_BBA_STEP_BBA_DCOC_STEP_RECIP(bbf_dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_0 = XCVR_RX_DIG_DCOC_TZA_STEP_0_DCOC_TZA_STEP_GAIN_0((uint32_t)tza_dcoc_step[0].dcoc_step*10UL) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_0_DCOC_TZA_STEP_RCP_0((uint32_t)tza_dcoc_step[0].dcoc_step_rcp/10UL) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_1 = XCVR_RX_DIG_DCOC_TZA_STEP_1_DCOC_TZA_STEP_GAIN_1(tza_dcoc_step[1].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_1_DCOC_TZA_STEP_RCP_1(tza_dcoc_step[1].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_2 = XCVR_RX_DIG_DCOC_TZA_STEP_2_DCOC_TZA_STEP_GAIN_2(tza_dcoc_step[2].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_2_DCOC_TZA_STEP_RCP_2(tza_dcoc_step[2].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_3 = XCVR_RX_DIG_DCOC_TZA_STEP_3_DCOC_TZA_STEP_GAIN_3(tza_dcoc_step[3].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_3_DCOC_TZA_STEP_RCP_3(tza_dcoc_step[3].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_4 = XCVR_RX_DIG_DCOC_TZA_STEP_4_DCOC_TZA_STEP_GAIN_4(tza_dcoc_step[4].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_4_DCOC_TZA_STEP_RCP_4(tza_dcoc_step[4].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_5 = XCVR_RX_DIG_DCOC_TZA_STEP_5_DCOC_TZA_STEP_GAIN_5(tza_dcoc_step[5].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_5_DCOC_TZA_STEP_RCP_5(tza_dcoc_step[5].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_6 = XCVR_RX_DIG_DCOC_TZA_STEP_6_DCOC_TZA_STEP_GAIN_6(tza_dcoc_step[6].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_6_DCOC_TZA_STEP_RCP_6(tza_dcoc_step[6].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_7 = XCVR_RX_DIG_DCOC_TZA_STEP_7_DCOC_TZA_STEP_GAIN_7(tza_dcoc_step[7].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_7_DCOC_TZA_STEP_RCP_7(tza_dcoc_step[7].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_8 = XCVR_RX_DIG_DCOC_TZA_STEP_8_DCOC_TZA_STEP_GAIN_8(tza_dcoc_step[8].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_8_DCOC_TZA_STEP_RCP_8(tza_dcoc_step[8].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_9 = XCVR_RX_DIG_DCOC_TZA_STEP_9_DCOC_TZA_STEP_GAIN_9(tza_dcoc_step[9].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_9_DCOC_TZA_STEP_RCP_9(tza_dcoc_step[9].dcoc_step_rcp) ;
        XCVR_RX_DIG->DCOC_TZA_STEP_10 = XCVR_RX_DIG_DCOC_TZA_STEP_10_DCOC_TZA_STEP_GAIN_10(tza_dcoc_step[10].dcoc_step) | 
                                        XCVR_RX_DIG_DCOC_TZA_STEP_10_DCOC_TZA_STEP_RCP_10(tza_dcoc_step[10].dcoc_step_rcp) ;
        status = 1U; /* Success */
    }
    else
    {
        status = 0U; /* Failure */
    }

    return status;
}


void init_chan_state(xcvr_channel_state_t * chan)
{
    if (chan != NULLPTR) /* Make sure pointer is valid */
    {
        /* Set initial DC measurements to out of range */
        chan->dc_meas = 2000;
        chan->dc_meas_p = 2000;
        /* Init the completion flags to false */
        chan->TZA_OK = false;
        chan->BBA_OK = false;
        /* Start DAC settings at default 8802020 */
        chan->curr_bba_dac = 0x20U;
        chan->curr_tza_dac = 0x80U;
        chan->p_bba_dac = 0x20U;
        chan->p_tza_dac = 0x80U;
    }
}
void tza_chan_sweep(xcvr_channel_state_t * chan, uint8_t step_num)
{
    /* check whether the TZA step is done (when the DC measurement sign changes */
    if ((ISIGN(chan->dc_meas) != ISIGN(chan->dc_meas_p)) && (step_num > 0U))
    {    
        if (ABS(chan->dc_meas) != MIN(ABS(chan->dc_meas), ABS(chan->dc_meas_p)))
        {
            chan->curr_tza_dac = chan->p_tza_dac; /* Use the prior value */
        }
        chan->TZA_OK = true;           
    }
    else 
    {
        chan->p_tza_dac = chan->curr_tza_dac;        
        if (ISIGN(chan->dc_meas)) /* If positive */
        {
            chan->curr_tza_dac--; /* step the TZA DAC down */
        }
        else 
        {
            chan->curr_tza_dac++; /* step the TZA DAC up */
        }
    }
}

void bba_chan_sweep(xcvr_channel_state_t * chan, uint8_t step_num)
{
    /* check whether the BBA step is done (when the DC measurement sign changes */
    if ((ISIGN(chan->dc_meas) != ISIGN(chan->dc_meas_p)) && (chan->curr_bba_dac != 0x20U))
    {   
        if (ABS(chan->dc_meas) != MIN(ABS(chan->dc_meas), ABS(chan->dc_meas_p)))
        {
            chan->curr_bba_dac = chan->p_bba_dac; /* Use the prior value */
        }
        chan->BBA_OK = true;           
    }
    else 
    {
        chan->p_bba_dac = chan->curr_bba_dac;
        if (ISIGN(chan->dc_meas)) /* If positif */
        {
            chan->curr_bba_dac--; /* step the BBA DAC down */
        }
        else 
        {
            chan->curr_bba_dac++; /* step the BBA DAC up */
        }
    }
}

void chan_step_sweep(xcvr_channel_state_t * chan, uint8_t step_num)
{
    /* Sweep TZA */
    if (!chan->TZA_OK)
    {   
        tza_chan_sweep(chan, step_num); /* Try another TZA step */
    }
    else /* Sweep BBA  */
    {
        if (!chan->BBA_OK)
        {  
            bba_chan_sweep(chan, step_num); /* Try another BBA step */
        }
    }

}

void DCOC_DAC_INIT_Cal (bool standalone_operation)
{
    xcvr_channel_state_t i_chan;
    xcvr_channel_state_t q_chan;
    init_chan_state(&i_chan);       /* initialize algorithm state for I channel */
    init_chan_state(&q_chan);       /* initialize algorithm state for Q channel */
    uint8_t i = 0;
    uint8_t bba_gain = 11;
    uint8_t status;    
    uint32_t dcoc_ctrl_0_stack;
    uint32_t dcoc_ctrl_1_stack;
    uint32_t agc_ctrl_1_stack;
    uint32_t rx_dig_ctrl_stack;
    uint32_t dcoc_cal_gain_state;
    uint32_t xcvr_ctrl_stack = 0U;
    xcvrStatus_t xcvr_status;
    uint32_t temp;

    /* Save registers */
    dcoc_ctrl_0_stack = XCVR_RX_DIG->DCOC_CTRL_0; /* Save state of DCOC_CTRL_0 for later restore */
    dcoc_ctrl_1_stack = XCVR_RX_DIG->DCOC_CTRL_1; /* Save state of DCOC_CTRL_1 for later restore */
    rx_dig_ctrl_stack =  XCVR_RX_DIG->RX_DIG_CTRL; /* Save state of RX_DIG_CTRL for later restore */
    agc_ctrl_1_stack = XCVR_RX_DIG->AGC_CTRL_1; /* Save state of RX_DIG_CTRL for later restore */
    dcoc_cal_gain_state = XCVR_RX_DIG->DCOC_CAL_GAIN; /* Save state of DCOC_CAL_GAIN for later restore */
    
     /* WarmUp */
     if (standalone_operation)
     {
         temp = XCVR_PLL_DIG->CHAN_MAP;
         xcvr_ctrl_stack = temp;
         temp &= ~(XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT_MASK);
         temp |= XCVR_PLL_DIG_CHAN_MAP_BAND_SELECT(0);
         XCVR_PLL_DIG->CHAN_MAP = temp;
         xcvr_status = XCVR_OverrideChannel(12); /* Calibrate on channel #12, 2.426 GHz in BLE map */
         (void) xcvr_status; /* Workaround unused variable warnings in GCC */
         XCVR_ForceRxWu();
         XcvrCalDelay(2000U);
     } 
    
     /* Register config */
     /* Ensure AGC, DCOC and RX_DIG_CTRL is in correct mode */
     temp = XCVR_RX_DIG->RX_DIG_CTRL;
     temp &= ~(XCVR_RX_DIG_RX_DIG_CTRL_RX_AGC_EN_MASK |       /* Turn OFF AGC */
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DCOC_CAL_EN_MASK | /* Disable for SW control of DCOC */
                XCVR_RX_DIG_RX_DIG_CTRL_RX_DC_RESID_EN_MASK); /* Disable for SW control of DCOC */
     XCVR_RX_DIG->RX_DIG_CTRL = temp;
     
     XCVR_RX_DIG->AGC_CTRL_1 = XCVR_RX_DIG_AGC_CTRL_1_USER_LNA_GAIN_EN(1) | /* Enable LNA Manual Gain */
                        XCVR_RX_DIG_AGC_CTRL_1_USER_BBA_GAIN_EN(1) | /* Enable BBA Manual Gain */
                        XCVR_RX_DIG_AGC_CTRL_1_LNA_USER_GAIN(0x0) | /* Set LNA Manual Gain */
                        XCVR_RX_DIG_AGC_CTRL_1_BBA_USER_GAIN(0x0); /* Set BBA Manual Gain */
    
     /* DCOC_CTRL_0 @ 4005_C02C -- Define default DCOC DAC settings in manual mode */
     temp = XCVR_RX_DIG->DCOC_CTRL_0;
     temp |= XCVR_RX_DIG_DCOC_CTRL_0_DCOC_MAN(1) | /* Enable  Manual DCOC */
            XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_SRC(1) | /* Ensure DCOC Tracking is enabled */
            XCVR_RX_DIG_DCOC_CTRL_0_DCOC_TRK_EST_OVR(1) | /* Enable DC Estimator */
            XCVR_RX_DIG_DCOC_CTRL_0_DCOC_CORRECT_EN(1); /* Ensure DC correction is enabled */
     temp &= ~(XCVR_RX_DIG_DCOC_CTRL_0_TRACK_FROM_ZERO_MASK); /* Ensure track from zero is disabled during the estimation */
     XCVR_RX_DIG->DCOC_CTRL_0 = temp;
    
     XcvrCalDelay(CAL_SETTLE_TIME);
         
     /* Set default DCOC DAC INIT Value */        
     /* LNA and BBA DAC Sweep */

     /* Perform a first DC measurement to ensure that measurement is not clipping */
     XCVR_RX_DIG->DCOC_DAC_INIT = XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(i_chan.curr_bba_dac) |
                                     XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(q_chan.curr_bba_dac) |
                                     XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(i_chan.curr_tza_dac) |
                                     XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(q_chan.curr_tza_dac);
     do
     {
         bba_gain--;
         /* Set DAC user gain */
         XCVR_RX_DIG->AGC_CTRL_1 = XCVR_RX_DIG_AGC_CTRL_1_USER_LNA_GAIN_EN(1UL) |
                                 XCVR_RX_DIG_AGC_CTRL_1_LNA_USER_GAIN(0UL) | /* 2 */
                                 XCVR_RX_DIG_AGC_CTRL_1_USER_BBA_GAIN_EN(1UL) |
                                 XCVR_RX_DIG_AGC_CTRL_1_BBA_USER_GAIN(bba_gain) ; /* 10 */
         XcvrCalDelay(CAL_SETTLE_TIME*2U);
         status = rx_dc_est_average(&i_chan.dc_meas, &q_chan.dc_meas, 64);
         (void)status;
     } while (dc_is_too_high(i_chan.dc_meas, q_chan.dc_meas));
         
         
     for( i = 0; i<0x0FU; i++)
     {
         
         /* I channel :  */
         chan_step_sweep(&i_chan, i);
     
         /* Q channel : */
         chan_step_sweep(&q_chan, i);


         /* DC OK break : */
         if (i_chan.TZA_OK && q_chan.TZA_OK && i_chan.BBA_OK && q_chan.BBA_OK)
         {
             break;
         }
         
         i_chan.dc_meas_p = i_chan.dc_meas; /* Store as previous value */
         q_chan.dc_meas_p = q_chan.dc_meas; /* Store as previous value */
         XCVR_RX_DIG->DCOC_DAC_INIT = XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(i_chan.curr_bba_dac) |
                                     XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(q_chan.curr_bba_dac) |
                                     XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(i_chan.curr_tza_dac) |
                                     XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(q_chan.curr_tza_dac);
         XcvrCalDelay(CAL_SETTLE_TIME*2U);
         status = rx_dc_est_average(&i_chan.dc_meas, &q_chan.dc_meas, 64);
         (void)status;
     }
     
     /* Apply optimized DCOC DAC INIT : */
     XCVR_RX_DIG->DCOC_DAC_INIT =  XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_I(i_chan.curr_bba_dac) |
                                  XCVR_RX_DIG_DCOC_DAC_INIT_BBA_DCOC_INIT_Q(q_chan.curr_bba_dac) |
                                  XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_I(i_chan.curr_tza_dac) |
                                  XCVR_RX_DIG_DCOC_DAC_INIT_TZA_DCOC_INIT_Q(q_chan.curr_tza_dac);
    
     /* WarmDown */
     if (standalone_operation)
     {
         XCVR_ForceRxWd(); /* Don't leave the receiver running. */
         XcvrCalDelay(200);
         xcvr_status = XCVR_OverrideChannel(0xFF); /* Release channel overrides */
         (void) xcvr_status; /* Workaround unused variable warnings in GCC */
         XCVR_PLL_DIG->CHAN_MAP = xcvr_ctrl_stack;
     }
     
     /* Restore register */
     XCVR_RX_DIG->DCOC_CTRL_0 = dcoc_ctrl_0_stack; /* Restore DCOC_CTRL_0 state to prior settings */
     XCVR_RX_DIG->DCOC_CTRL_1 = dcoc_ctrl_1_stack; /* Restore DCOC_CTRL_1 state to prior settings */
     XCVR_RX_DIG->RX_DIG_CTRL = rx_dig_ctrl_stack; /* Restore RX_DIG_CTRL state to prior settings */
     XCVR_RX_DIG->DCOC_CAL_GAIN = dcoc_cal_gain_state; /* Restore DCOC_CAL_GAIN state to prior setting */
     XCVR_RX_DIG->AGC_CTRL_1 = agc_ctrl_1_stack; /* Save state of RX_DIG_CTRL for later restore */

    /* Resets the state of RX after registers are restored */
    force_rx_init_pulse();
    XcvrCalDelay(CAL_SETTLE_TIME*4U);


}

#if !defined(GCOV_DO_COVERAGE) /* routine is local except when testing code coverage */
static bool dc_is_too_high(int16_t i_dc, int16_t q_dc)
#else
bool dc_is_too_high(int16_t i_dc, int16_t q_dc)
#endif /* !defined(GCOV_DO_COVERAGE) */
{
    return ((ABS(i_dc) > 1900U) || (ABS(q_dc) > 1900U));
}


void force_rx_init_pulse(void)
{
    // Enable RX_INIT override
    XCVR_TSM->OVRD2 &= ~(XCVR_TSM_OVRD2_RX_INIT_OVRD_MASK);  
    XCVR_TSM->OVRD2 |= XCVR_TSM_OVRD2_RX_INIT_OVRD_EN_MASK;  

    // Assert RX_INIT override
    XCVR_TSM->OVRD2 |= XCVR_TSM_OVRD2_RX_INIT_OVRD_MASK;  
  
    XcvrCalDelay(2U);

    // De-assert RX_INIT override
    // Disable RX_INIT override
    XCVR_TSM->OVRD2 &= ~(XCVR_TSM_OVRD2_RX_INIT_OVRD_MASK | XCVR_TSM_OVRD2_RX_INIT_OVRD_EN_MASK);  
}

