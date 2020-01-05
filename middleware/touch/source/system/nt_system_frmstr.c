/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "nt_types.h"

#if NT_FREEMASTER_SUPPORT != 0

/* System includes */
#include "nt_system.h"
#include "../source/system/nt_system_prv.h"
#include "../source/system/nt_system_mem_prv.h"

/* Controls includes */
#include "nt_controls.h"
#include "../source/controls/nt_controls_prv.h"
#include "nt_control_arotary.h"
#include "../source/controls/nt_control_arotary_prv.h"
#include "nt_control_aslider.h"
#include "../source/controls/nt_control_aslider_prv.h"
#include "nt_control_keypad.h"
#include "../source/controls/nt_control_keypad_prv.h"
#include "nt_control_rotary.h"
#include "../source/controls/nt_control_rotary_prv.h"
#include "nt_control_slider.h"
#include "../source/controls/nt_control_slider_prv.h"
#include "nt_control_proxi.h"
#include "../source/controls/nt_control_proxi_prv.h"
#include "nt_control_matrix.h"
#include "../source/controls/nt_control_matrix_prv.h"
/* Electrodes includes */
#include "nt_electrodes.h"
#include "../source/electrodes/nt_electrodes_prv.h"

/* Modules includes */
#include "nt_modules.h"
#include "../source/modules/nt_modules_prv.h"
#if defined(CPU_QN908X) || defined(CPU_QN9080C)
  #include "nt_module_cs.h"
  #include "../source/modules/nt_module_cs_prv.h"
#else
  #include "nt_module_tsi.h"
  #include "../source/modules/nt_module_tsi_prv.h"
#endif
#include "../source/modules/nt_module_gpio_prv.h"
#include "../source/modules/nt_module_gpioint_prv.h"

/* Key-detectors includes */
#include "../source/keydetectors/nt_keydetector_safa_prv.h"
#include "../source/keydetectors/nt_keydetector_usafa_prv.h"
#include "../source/keydetectors/nt_keydetector_afid_prv.h"

/* If the error occur that the compiler can't find this header file. So it means 
 * that the FreeMASTER is not part of the project and there is two possible solution:
 *  - Add FreeMASTER into project and it can be use to debug
 *  - remove this file from project.
*/
#include "freemaster.h" 

#if FMSTR_DISABLE != 0
  #error The FreeMASTER is disabled, In this case the NT FreeMASTER support (NT_FREEMASTER_SUPPORT) should be also disabled.
#endif

#if FMSTR_USE_TSA == 0
  #error The FreeMASTER TSA tables are disabled, the NT required this functionality, enable FMSTR_USE_TSA option.
#endif

#ifndef FMSTR_USE_TSA_DYNAMIC
  #error The FreeMASTER TSA dynamic tables are disabled, the NT required this functionality, enable FMSTR_USE_TSA_DYNAMIC option.
#elif FMSTR_USE_TSA_DYNAMIC == 0
  #error The FreeMASTER TSA dynamic tables are disabled, the NT required this functionality, enable FMSTR_USE_TSA_DYNAMIC option.
#endif

extern struct nt_kernel nt_kernel_data;

FMSTR_TSA_TABLE_BEGIN(nt_frmstr_tsa_table)

  FMSTR_TSA_RO_VAR(nt_kernel_data, FMSTR_TSA_USERTYPE(struct nt_kernel))

/*******************************************************************
*                       SYSTEM types
*******************************************************************/

/******************* nt_system_prv.h ******************************/

  FMSTR_TSA_STRUCT(struct nt_system)
  FMSTR_TSA_MEMBER(struct nt_system, controls,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_system, modules,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_system, time_period,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_system, init_time,   FMSTR_TSA_UINT16)

/* nt_kernel */
  FMSTR_TSA_STRUCT(struct nt_kernel)
  FMSTR_TSA_MEMBER(struct nt_kernel, rom,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_kernel, controls,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_kernel, modules,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_kernel, controls_cnt,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_kernel, modules_cnt,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_kernel, time_counter,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_kernel, memory,   FMSTR_TSA_USERTYPE(struct nt_mem))

/*************** nt_system_mem_prv.h ******************************/
/* nt_mem */
  FMSTR_TSA_STRUCT(struct nt_mem)
  FMSTR_TSA_MEMBER(struct nt_mem, pool,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_mem, pool_size,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_mem, free_pointer,   FMSTR_TSA_POINTER) 

/*******************************************************************
*                       MODULES types
*******************************************************************/
#if (FSL_FEATURE_TSI_VERSION == 5)
  FMSTR_TSA_STRUCT(tsi_selfCap_config_t)
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, enableSensitivity,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, enableShield,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, commonConfig,   FMSTR_TSA_USERTYPE(tsi_common_config_t))
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, xdn,   FMSTR_TSA_USERTYPE(tsi_sensitivity_xdn_option_t))
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, ctrim,   FMSTR_TSA_USERTYPE(tsi_sensitivity_ctrim_option_t))
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, inputCurrent,   FMSTR_TSA_USERTYPE(tsi_current_multiple_input_t))
  FMSTR_TSA_MEMBER(tsi_selfCap_config_t, chargeCurrent,   FMSTR_TSA_USERTYPE(tsi_current_multiple_input_t))

  FMSTR_TSA_STRUCT(tsi_common_config_t)
  FMSTR_TSA_MEMBER(tsi_common_config_t, mainClock,   FMSTR_TSA_USERTYPE(tsi_main_clock_selection_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, mode,   FMSTR_TSA_USERTYPE(tsi_sensing_mode_selection_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, dvolt,   FMSTR_TSA_USERTYPE(tsi_dvolt_option_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, cutoff,   FMSTR_TSA_USERTYPE(tsi_sinc_cutoff_div_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, order,   FMSTR_TSA_USERTYPE(tsi_sinc_filter_order_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, decimation,   FMSTR_TSA_USERTYPE(tsi_sinc_decimation_value_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, chargeNum,   FMSTR_TSA_USERTYPE(tsi_ssc_charge_num_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, prbsOutsel,   FMSTR_TSA_USERTYPE(tsi_ssc_prbs_outsel_t))  
  FMSTR_TSA_MEMBER(tsi_common_config_t, noChargeNum,   FMSTR_TSA_USERTYPE(tsi_ssc_nocharge_num_t))
  FMSTR_TSA_MEMBER(tsi_common_config_t, ssc_mode,   FMSTR_TSA_USERTYPE(tsi_ssc_mode_t))  
  FMSTR_TSA_MEMBER(tsi_common_config_t, ssc_prescaler,   FMSTR_TSA_USERTYPE(tsi_ssc_prescaler_t))   

  FMSTR_TSA_STRUCT(tsi_mutualCap_config_t)
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, enableNmosMirror,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, commonConfig,   FMSTR_TSA_USERTYPE(tsi_common_config_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, preCurrent,   FMSTR_TSA_USERTYPE(tsi_mutual_pre_current_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, preResistor,   FMSTR_TSA_USERTYPE(tsi_mutual_pre_resistor_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, senseResistor,   FMSTR_TSA_USERTYPE(tsi_mutual_sense_resistor_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, boostCurrent,   FMSTR_TSA_USERTYPE(tsi_mutual_sense_boost_current_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, txDriveMode,   FMSTR_TSA_USERTYPE(tsi_mutual_tx_drive_mode_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, pmosLeftCurrent,   FMSTR_TSA_USERTYPE(tsi_mutual_pmos_current_left_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, pmosRightCurrent,   FMSTR_TSA_USERTYPE(tsi_mutual_pmos_current_right_t))
  FMSTR_TSA_MEMBER(tsi_mutualCap_config_t, nmosCurrent,   FMSTR_TSA_USERTYPE(tsi_mutual_nmos_current_t))

  FMSTR_TSA_STRUCT(tsi_config_t)
  FMSTR_TSA_MEMBER(tsi_config_t, configSelfCap,   FMSTR_TSA_USERTYPE(tsi_selfCap_config_t))
  FMSTR_TSA_MEMBER(tsi_config_t, configMutual,   FMSTR_TSA_USERTYPE(tsi_mutualCap_config_t))
  FMSTR_TSA_MEMBER(tsi_config_t, thresl,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(tsi_config_t, thresh,   FMSTR_TSA_UINT16)

  FMSTR_TSA_STRUCT(nt_tsi_recalib_config_t)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, SelfRangeMin,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, SelfRangeMax,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, SelfRangeDeltaMin,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, SelfRangeDeltaMax,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, MutRangeMin,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, MutRangeMax,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, MutRangeDeltaMin,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(nt_tsi_recalib_config_t, MutRangeDeltaMax,   FMSTR_TSA_UINT32)
#endif /* FSL_FEATURE_TSI_VERSION == 5 */

#if (FSL_FEATURE_TSI_VERSION == 4)
  FMSTR_TSA_STRUCT(tsi_config_t)
  FMSTR_TSA_MEMBER(tsi_config_t, thresh,    FMSTR_TSA_UINT16)    
  FMSTR_TSA_MEMBER(tsi_config_t, thresl,    FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(tsi_config_t, prescaler, FMSTR_TSA_USERTYPE(tsi_electrode_osc_prescaler_t))
  FMSTR_TSA_MEMBER(tsi_config_t, extchrg,   FMSTR_TSA_USERTYPE(tsi_external_osc_charge_current_t))
  FMSTR_TSA_MEMBER(tsi_config_t, refchrg,   FMSTR_TSA_USERTYPE(tsi_reference_osc_charge_current_t))
  FMSTR_TSA_MEMBER(tsi_config_t, nscn,      FMSTR_TSA_USERTYPE(tsi_n_consecutive_scans_t))
  FMSTR_TSA_MEMBER(tsi_config_t, mode,      FMSTR_TSA_USERTYPE(tsi_analog_mode_t))   
  FMSTR_TSA_MEMBER(tsi_config_t, dvolt,     FMSTR_TSA_USERTYPE(tsi_osc_voltage_rails_t))
  FMSTR_TSA_MEMBER(tsi_config_t, resistor,  FMSTR_TSA_USERTYPE(tsi_series_resistor_t))
  FMSTR_TSA_MEMBER(tsi_config_t, filter,    FMSTR_TSA_USERTYPE(tsi_filter_bits_t))   
#endif /* FSL_FEATURE_TSI_VERSION == 4 */
    
#if defined(CPU_QN908X) || defined(CPU_QN9080C)
  FMSTR_TSA_STRUCT(cs_config_t)
  FMSTR_TSA_MEMBER(cs_config_t, activeClockDivider, FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(cs_config_t, activeDetectPeriod, FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(cs_config_t, activeIdlePeriod,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(cs_config_t, activeOscFreq,      FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(cs_config_t, activeCounterGain,  FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(cs_config_t, lowPowerChannelNum, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(cs_config_t, lowPowerThreshold,  FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(cs_config_t, lowPowerDebonceNum, FMSTR_TSA_UINT16) 
  FMSTR_TSA_MEMBER(cs_config_t, lowPowerIdlePeriod, FMSTR_TSA_UINT16)  
  FMSTR_TSA_MEMBER(cs_config_t, lowPowerOscFreq,    FMSTR_TSA_UINT16)   
#endif /* defined(CPU_QN908X) || defined(CPU_QN9080C) */

/************************ nt_modules.h *****************************/
/* nt_module */
  FMSTR_TSA_STRUCT(struct nt_module)
  FMSTR_TSA_MEMBER(struct nt_module, interface,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module, electrodes,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module, module_params,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module, config,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module, recalib_config,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module, instance,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_module, wtrmark_hi,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_module, wtrmark_lo,   FMSTR_TSA_UINT16)

/********************** nt_modules_prv.h ***************************/
/* nt_module_data */
  FMSTR_TSA_STRUCT(struct nt_module_data)
  FMSTR_TSA_MEMBER(struct nt_module_data, rom,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module_data, electrodes,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_module_data, active_mode,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_module_data, flags,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_module_data, electrodes_cnt,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_module_data, special_data,   FMSTR_TSA_POINTER)    
     
#if defined(CPU_QN908X) || defined(CPU_QN9080C)
  FMSTR_TSA_STRUCT(struct nt_module_cs_data)
  FMSTR_TSA_MEMBER(struct nt_module_cs_data, cs_state,   FMSTR_TSA_UINT32)  
#else
/********************* nt_module_tsi.h ***************************/
/* nt_module_tsi_params */
  FMSTR_TSA_STRUCT(struct nt_module_tsi_params)
  FMSTR_TSA_MEMBER(struct nt_module_tsi_params, noise,   FMSTR_TSA_USERTYPE(nt_module_tsi_noise))

/******************** nt_module_tsi_prv.h *************************/
/* nt_module_tsi_noise_data */
  FMSTR_TSA_STRUCT(struct nt_module_tsi_noise_data)
  FMSTR_TSA_MEMBER(struct nt_module_tsi_noise_data, filter_state,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_module_tsi_noise_data, noise,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_module_tsi_noise_data, touch_threshold,   FMSTR_TSA_UINT8)
           
 /* nt_module_tsi_data */
  FMSTR_TSA_STRUCT(struct nt_module_tsi_data)
  FMSTR_TSA_MEMBER(struct nt_module_tsi_data, tsi_state,   FMSTR_TSA_UINT32)     
#endif  /* defined(CPU_QN908X) || defined(CPU_QN9080C) */   

/******************** nt_module_gpioint_prv.h *************************/
/* nt_module_gpioint_data */
  FMSTR_TSA_STRUCT(struct nt_module_gpioint_data)
  FMSTR_TSA_MEMBER(struct nt_module_gpioint_data, pen,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_module_gpioint_data, measured_pin,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_module_gpioint_data, scan_status,   FMSTR_TSA_UINT8)

/********************* nt_module_gpio_prv.h ***************************/
/* nt_module_gpio_data */
  FMSTR_TSA_STRUCT(struct nt_module_gpio_data)
  FMSTR_TSA_MEMBER(struct nt_module_gpio_data, pen,   FMSTR_TSA_UINT32)

/*******************************************************************
*                       ELECTRODES types
*******************************************************************/

/********************** nt_electrodes.h ***************************/
/* nt_electrode_status */
  FMSTR_TSA_STRUCT(struct nt_electrode_status)
  FMSTR_TSA_MEMBER(struct nt_electrode_status, time_stamp,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_electrode_status, state,   FMSTR_TSA_UINT8)

/* nt_electrode */
  FMSTR_TSA_STRUCT(struct nt_electrode)
  FMSTR_TSA_MEMBER(struct nt_electrode, shielding_electrode,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_electrode, multiplier,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_electrode, divider,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_electrode, shield_threshold,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_electrode, shield_gain,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_electrode, shield_attn,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_electrode, shield_sens,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_electrode, baseline_level,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_electrode, pin_input,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_electrode, keydetector_interface,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_electrode, keydetector_params,   FMSTR_TSA_POINTER)

/***************** nt_electrodes_prv.h ****************************/
/* nt_electrode_data */
  FMSTR_TSA_STRUCT(struct nt_electrode_data)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, rom,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, module_data,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, signal,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, raw_signal,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, baseline,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, status,   FMSTR_TSA_USERTYPE(nt_electrode_status))
  FMSTR_TSA_MEMBER(struct nt_electrode_data, status_index,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, flags,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, keydetector_data,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, special_data,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_electrode_data, shielding_electrode,   FMSTR_TSA_POINTER)

 /* nt_electrode_special_data */
  FMSTR_TSA_STRUCT(union nt_electrode_special_data)
  FMSTR_TSA_MEMBER(union nt_electrode_special_data, tsi_noise,   FMSTR_TSA_UINT32)  

/*******************************************************************
*                       KEYDETECTORS types
*******************************************************************/
  FMSTR_TSA_STRUCT(struct nt_filter_iir)
  FMSTR_TSA_MEMBER(struct nt_filter_iir, coef1,   FMSTR_TSA_UINT8)
  FMSTR_TSA_STRUCT(struct nt_filter_fbutt)
  FMSTR_TSA_MEMBER(struct nt_filter_fbutt, cutoff,   FMSTR_TSA_SINT32)
  FMSTR_TSA_STRUCT(struct nt_filter_moving_average)
  FMSTR_TSA_MEMBER(struct nt_filter_moving_average, n2_order,   FMSTR_TSA_SINT32)
  FMSTR_TSA_STRUCT(struct nt_keydetector_afid_asc)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_asc, touch_threshold_fall_rate,   FMSTR_TSA_SINT16)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_asc, noise_resets_minimum,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_asc, resets_for_touch,   FMSTR_TSA_SINT16)

  FMSTR_TSA_STRUCT(struct nt_keydetector_afid)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid, reset_rate,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid, signal_filter,   FMSTR_TSA_USERTYPE(struct nt_filter_iir))
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid, fast_signal_filter,   FMSTR_TSA_USERTYPE(struct nt_filter_fbutt))
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid, slow_signal_filter,   FMSTR_TSA_USERTYPE(struct nt_filter_fbutt))
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid, base_avrg,   FMSTR_TSA_USERTYPE(struct nt_filter_moving_average))
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid, asc,   FMSTR_TSA_USERTYPE(struct nt_keydetector_afid_asc))


  FMSTR_TSA_STRUCT(struct nt_keydetector_safa)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, signal_filter,   FMSTR_TSA_USERTYPE(struct nt_filter_iir))
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, base_avrg,   FMSTR_TSA_USERTYPE(struct nt_filter_moving_average))
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, non_activity_avrg,   FMSTR_TSA_USERTYPE(struct nt_filter_moving_average))
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, entry_event_cnt,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, deadband_cnt,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, signal_to_noise_ratio,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa, min_noise_limit,   FMSTR_TSA_UINT32)

  FMSTR_TSA_STRUCT(struct nt_keydetector_usafa)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, signal_filter,   FMSTR_TSA_USERTYPE(struct nt_filter_iir))
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, base_avrg,   FMSTR_TSA_USERTYPE(struct nt_filter_moving_average))
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, non_activity_avrg,   FMSTR_TSA_USERTYPE(struct nt_filter_moving_average))
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, entry_event_cnt,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, deadband_cnt,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, signal_to_noise_ratio,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, min_noise_limit,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, dc_track_enabled,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa, dc_track_cnt,   FMSTR_TSA_UINT32)


  /************** nt_keydetector_safa_prv.h *************************/
/* nt_keydetector_safa_data */
  FMSTR_TSA_STRUCT(struct nt_keydetector_safa_data)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, filter_state,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, predicted_signal_avrg,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, noise,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, predicted_signal,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, entry_event_cnt,   FMSTR_TSA_SINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, deadband_cnt,   FMSTR_TSA_SINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, deadband_h,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_safa_data, deadband_l,   FMSTR_TSA_UINT32) 
/************** nt_keydetector_usafa_prv.h *************************/
/* nt_keydetector_usafa_data */
  FMSTR_TSA_STRUCT(struct nt_keydetector_usafa_data)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, filter_state,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, predicted_signal_avrg,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, noise,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, predicted_signal,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, entry_event_cnt,   FMSTR_TSA_SINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, deadband_cnt,   FMSTR_TSA_SINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, deadband_h,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_usafa_data, deadband_l,   FMSTR_TSA_UINT32)    
/************** nt_keydetector_afid_prv.h *************************/
/* nt_keydetector_afid_data */
  FMSTR_TSA_STRUCT(struct nt_keydetector_afid_data)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, slow_signal,   FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, asc,   FMSTR_TSA_USERTYPE(nt_keydetector_afid_asc_data))
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, filter_state,   FMSTR_TSA_UINT8)  
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, integration_value,   FMSTR_TSA_SINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, touch_threshold,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, touch_reset_counter,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_data, release_reset_counter,   FMSTR_TSA_UINT32)
  
/* nt_keydetector_afid_asc_data */
  FMSTR_TSA_STRUCT(struct nt_keydetector_afid_asc_data)
  FMSTR_TSA_MEMBER(struct nt_keydetector_afid_asc_data, max_resets,   FMSTR_TSA_SINT32) 

/*******************************************************************
*                       FILTERS types
*******************************************************************/

/*******************************************************************
*                       CONTROLS types
*******************************************************************/
    
/************************* nt_controls.h ***************************/
/* nt_control */
  FMSTR_TSA_STRUCT(struct nt_control)
  FMSTR_TSA_MEMBER(struct nt_control, interface,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_control, electrodes,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_control, control_params,   FMSTR_TSA_POINTER)

/***************** nt_controls_prv.h ******************************/
/* nt_control_data */
  FMSTR_TSA_STRUCT(struct nt_control_data)
  FMSTR_TSA_MEMBER(struct nt_control_data, rom,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_control_data, data,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_control_data, electrodes,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_control_data, flags,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_control_data, electrodes_size,   FMSTR_TSA_UINT8)
    
/********************* nt_control_arotary.h ***************************/
/* nt_control_arotary */
  FMSTR_TSA_STRUCT(struct nt_control_arotary)
  FMSTR_TSA_MEMBER(struct nt_control_arotary, range,   FMSTR_TSA_UINT8)

/********************* nt_control_arotary_prv.h ***************************/
/* nt_control_arotary */
  FMSTR_TSA_STRUCT(struct nt_control_arotary_data)
  FMSTR_TSA_MEMBER(struct nt_control_arotary_data, position,   FMSTR_TSA_UINT8)
   
/********************* nt_control_aslider.h ***************************/
/* nt_control_aslider */
  FMSTR_TSA_STRUCT(struct nt_control_aslider)
  FMSTR_TSA_MEMBER(struct nt_control_aslider, range,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_aslider, insensitivity,   FMSTR_TSA_UINT8)

/********************* nt_control_arotary_prv.h ***************************/
/* nt_control_arotary */
  FMSTR_TSA_STRUCT(struct nt_control_aslider_data)
  FMSTR_TSA_MEMBER(struct nt_control_aslider_data, position,   FMSTR_TSA_UINT8)
    
/********************* nt_control_rotary_prv.h ***************************/
/* nt_control_arotary */
  FMSTR_TSA_STRUCT(struct nt_control_rotary_data)
  FMSTR_TSA_MEMBER(struct nt_control_rotary_data, position,   FMSTR_TSA_UINT8)
    
/********************* nt_control_keypad.h ***************************/
/* nt_control_keypad */
  FMSTR_TSA_STRUCT(struct nt_control_keypad)
  FMSTR_TSA_MEMBER(struct nt_control_keypad, groups,   FMSTR_TSA_POINTER)
  FMSTR_TSA_MEMBER(struct nt_control_keypad, groups_size,   FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_keypad, multi_touch,   FMSTR_TSA_POINTER)

/********************* nt_control_keypad_prv.h ***************************/
/* nt_control_keypad_data */
  FMSTR_TSA_STRUCT(struct nt_control_keypad_data)
  FMSTR_TSA_MEMBER(struct nt_control_keypad_data, last_state,   FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_control_keypad_data, autorepeat_rate,   FMSTR_TSA_UINT16)

/********************* nt_control_slider_prv.h ***************************/
/* nt_control_slider */
  FMSTR_TSA_STRUCT(struct nt_control_slider_data)
  FMSTR_TSA_MEMBER(struct nt_control_slider_data, position,   FMSTR_TSA_UINT8)

/********************* nt_control_proxi_prv.h ***************************/
/* nt_control_proxi */
  FMSTR_TSA_STRUCT(struct nt_control_proxi)
  FMSTR_TSA_MEMBER(struct nt_control_proxi, range, FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi, scale, FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_control_proxi, threshold, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_proxi, insensitivity, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_proxi, gesture, FMSTR_TSA_POINTER)
  FMSTR_TSA_STRUCT(struct nt_control_proxi_data)
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proximity, FMSTR_TSA_SINT8) 
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, index, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[0],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[0],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[0],  FMSTR_TSA_UINT8)     
#if NT_PROXI_ELECTRODES > 1    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[1],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[1],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[1],  FMSTR_TSA_UINT8)  
#endif
#if NT_PROXI_ELECTRODES > 2    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[2],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[2],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[2],  FMSTR_TSA_UINT8)     
#endif
#if NT_PROXI_ELECTRODES > 3    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[3],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[3],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[3],  FMSTR_TSA_UINT8) 
#endif
#if NT_PROXI_ELECTRODES > 4    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[4],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[4],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[4],  FMSTR_TSA_UINT8) 
#endif  
#if NT_PROXI_ELECTRODES > 5    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[5],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[5],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[5],  FMSTR_TSA_UINT8) 
#endif
#if NT_PROXI_ELECTRODES > 6    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[6],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[6],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[6],  FMSTR_TSA_UINT8) 
#endif    
#if NT_PROXI_ELECTRODES > 7    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_max[7],  FMSTR_TSA_UINT16)      
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_curr[7],  FMSTR_TSA_UINT8)    
  FMSTR_TSA_MEMBER(struct nt_control_proxi_data, proxi_movement[7],  FMSTR_TSA_UINT8) 
#endif
  /********************* nt_control_matrix.h ***************************/
  /* nt_control_matrix */
  FMSTR_TSA_STRUCT(struct nt_control_matrix)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, double_tap_interval, FMSTR_TSA_UINT32)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, dissimilarity_max, FMSTR_TSA_UINT16)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, touchpad_size_horizontal, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, touchpad_size_vertical, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, sensing_mode, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, increase_resolution, FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix, gesture, FMSTR_TSA_USERTYPE(nt_coordinates64_xy))
  FMSTR_TSA_STRUCT(nt_coordinates64_xy)
  FMSTR_TSA_MEMBER(nt_coordinates64_xy, x, FMSTR_TSA_UINT64)
  FMSTR_TSA_MEMBER(nt_coordinates64_xy, y, FMSTR_TSA_UINT64)
  FMSTR_TSA_STRUCT(struct nt_control_matrix_data)
  FMSTR_TSA_MEMBER(struct nt_control_matrix_data, last_electode_states,  FMSTR_TSA_UINT64)
  FMSTR_TSA_MEMBER(struct nt_control_matrix_data, recognized_gesture,    FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix_data, actual_position[0].x,  FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix_data, actual_position[0].y,  FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix_data, actual_position[1].x,  FMSTR_TSA_UINT8)
  FMSTR_TSA_MEMBER(struct nt_control_matrix_data, actual_position[1].y,  FMSTR_TSA_UINT8)
    
FMSTR_TSA_TABLE_END()
    
int32_t _nt_freemaster_init(void)
{
/* This is workaround, here could be code that recognise count of really used interfaces */
  #define NT_MAXIMAL_COUNT_OF_INTERFACES        12
  
  uint8_t * p_freemaster_tsa_buffer = _nt_mem_alloc(NT_MAXIMAL_COUNT_OF_INTERFACES * sizeof(FMSTR_TSA_ENTRY));
  
  if(p_freemaster_tsa_buffer == NULL)
  {
    return NT_OUT_OF_MEMORY;
  }
  
  FMSTR_SetUpTsaBuff(p_freemaster_tsa_buffer, NT_MAXIMAL_COUNT_OF_INTERFACES * sizeof(FMSTR_TSA_ENTRY));
  
  return NT_SUCCESS;
}
 
int32_t _nt_freemaster_add_variable(const char * name, const char * type_name, void* address, uint32_t size)
{
  if(FMSTR_TsaAddVar(name, type_name, address, size, FMSTR_TSA_INFO_RO_VAR) == 0)
  {
    return NT_FAILURE;
  }
  return NT_SUCCESS;
}

#else
/* Section of FreeMASTER support function for case that FreeMASTER is not enebled */

int32_t _nt_freemaster_init(void)
{
  return NT_SUCCESS;
}

int32_t _nt_freemaster_add_variable(const char * name, const char * type_name, void* address, uint32_t size)
{
  (void)name;
  (void)type_name;
  (void)address;
  (void)size;
  
  return NT_SUCCESS;
}

#endif
