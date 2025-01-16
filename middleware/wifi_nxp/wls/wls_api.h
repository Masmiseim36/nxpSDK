#if CONFIG_WLS_CSI_PROC
/** @file wls_api.h
 *
 * @brief This file contains header file file for WLS processing fixed-point version API
 *
 * Copyright 2023-2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/************************************************************************
 * Header file for WLS processing fixed-point version API
 ************************************************************************/
#ifndef WLS_API_H
#define WLS_API_H

#include "wls_structure_defs.h"
#ifdef DFW_CSI_PROC
#include "dsp_cmd.h"
#endif

int wls_process_csi(unsigned int *bufferMemory,
                    unsigned int *fftInBuffer,
                    hal_wls_packet_params_t *packetparams,
                    hal_wls_processing_input_params_t *inputVals,
                    unsigned int *resArray);
void wls_unpack_csi(unsigned int *bufferMemory,
                    unsigned int *outBuffer,
                    hal_wls_packet_params_t *packetparams,
                    hal_wls_processing_input_params_t *inputVals,
                    unsigned int *totalpower);
int wls_calculate_toa(unsigned int *headerBuffer,
                      unsigned int *fftInBuffer,
                      unsigned int *fftBuffer,
                      unsigned int *totalpower,
                      hal_wls_packet_params_t *packetparams,
                      hal_wls_processing_input_params_t *inputVals);

void wls_intialize_reference(unsigned int *headerBuffer, unsigned int *fftInBuffer, float *fftRefBuffer);
float wls_update_cross_corr_pi_calc(unsigned int *headerBuffer,
                                    unsigned int *fftInBuffer,
                                    float *fftRefBuffer,
                                    unsigned int *tempBuffer);

#endif
#endif /* CONFIG_WLS_CSI_PROC */
