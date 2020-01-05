/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file frdm_stbc_dp5004_shield.h
 * @brief The frdm_stbc_dp5004_shield.h file declares mapping of the Kinetis
         Device peripherals to the frmd-ke15z and frmd-stbc-dp5004 shield
 */

#ifndef _FRDM_STBC_DP5004_SHIELD_H_
#define _FRDM_STBC_DP5004_SHIELD_H_

/* The shield name */
#define SHIELD_NAME "FRDM-STBC-DP5004"

// MPXV5004DP Sensor based parameters
#define MPXV5004DP_ZERO_ERROR 1185 /* Zero Error Offset. */
#define MPXV5004DP_PRESSURE_FROM_ADC_VALUE(x) (uint32_t)(x*1.25 - MPXV5004DP_ZERO_ERROR) /* Pressure in Pascals (Pa). */

/* Arduino Pin A0 (Sensor Analog Output Pin) maps to ADC0 Channel 0 on KE15Z (PTA0). */
#define MPXVDP5004_ADC12_CHANNEL  0U
#define MPXVDP5004_ADC12_CHANNEL_GROUP 0U

#endif /* _FRDM_STBC_DP5004_SHIELD_H_ */
