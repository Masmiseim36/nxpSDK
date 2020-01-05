/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_KEYDETECTOR_H_
#define _NT_KEYDETECTOR_H_

 /**
 * \defgroup detectors Key Detectors
 * \ingroup ntapi
 *
 * Key Detectors represent different types of signal-processing algorithms;
 * the primary purpose of a key detector algorithm is to determine, whether an electrode has been
 * touched or not, calculate the normalized signal, and provide all these information
 * to the \ref controls layer. The Controls layer is then able to detect much more
 * complex finger gestures, such as a slider swing or a key press within a multiplexed keypad.
 *
 * As an input, the Key Detector gets the raw electrode signal value obtained from
 * the data-acquisition algorithm, wrapped by one of the \ref modules instance.
 * The output values and intermediate calculated parameters needed by the Key Detector
 * layer are contained within a structure type, derived from the nt_electrode type. See
 * more information in the \ref electrodes section.
 *
 * In addition to signal processing, the Key Detector also detects, reports, and acts
 * on fault conditions during the scanning process. Two main fault conditions are
 * reported as electrode short-circuit to supply voltage (capacitance too small),
 * or short-circuit to ground (capacitance too high).
 */

/* forward declaration */
struct nt_electrode_data;

union nt_keydetector_params
{
  const struct nt_keydetector_afid     * afid;    /*!< AFID keydetectror pointer */ 
  const struct nt_keydetector_safa     * safa;    /*!< SAFA keydetectror pointer */
  const struct nt_keydetector_usafa    * usafa;   /*!< USAFA keydetectror pointer */
  const void                           * general; /*!< Pointer for general use */
};




/**
 * The key detector interface structure represents the NXP Touch library Key
 * Detector algorithm interface. The context data of the key detectors are stored in the
 * \ref electrodes application objects.
 *
 */
struct nt_keydetector_interface {
  int32_t (* nt_keydetector_init)(struct nt_electrode_data *electrode);        /*!< Key Detector initialization function pointer */
  void (* nt_keydetector_enable)(struct nt_electrode_data *electrode, uint32_t touch); /*!< Key Detector enable function pointer */
  void (* nt_keydetector_process)(struct nt_electrode_data *electrode);        /*!< Key Detector process function pointer       */
  void (* nt_keydetector_measure)(struct nt_electrode_data *electrode, uint32_t signal); /*!< Key Detector measure function pointer */
  void (* nt_keydetector_reset)(struct nt_electrode_data *electrode);          /*!< Key Detector reset function pointer       */
  const char* name;                                                            /*!< A name of the variable of this type, used for FreeMASTER support purposes. */
  const uint32_t params_size;                                                  /*!< Structure size */ 
};



#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

/** \} end of detectors group */

#endif /* NT_KEYDETECTOR_H_ */
