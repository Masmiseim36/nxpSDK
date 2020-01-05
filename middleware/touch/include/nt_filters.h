/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_FILTERS_H_
#define _NT_FILTERS_H_

#include "nt_types.h"

/**
 * \defgroup filter Filters
 * \ingroup ntapi
 * The filters data structure that is used in the NXP Touch library.
 *
 * \{
 */

#define NT_FILTER_MOVING_AVERAGE_MAX_ORDER  15
   
/* forward declaration */

/**
 * The butterworth filter input parameters.
 *
 */
struct nt_filter_fbutt {
    int32_t cutoff; /*!< The coefficient for the implemented butterworth filter polynomial. */
};

/**
 * The IIR filter input parameters.
 *
 */
struct nt_filter_iir {
    uint8_t coef1; /*!< Scale of the current and previous signals. When the coef is higher, the current signal has less strength. */
};

/**
 * The DC tracker filter input parameters.
 *
 */
struct nt_filter_dctracker {
    uint8_t rate; /*!< Rate of how fast the baseline is updated. The rate should be defined as a modulo of the system period. */
};

/**
 * The moving average filter input parameters.
 *
 */
struct nt_filter_moving_average {
  int32_t       n2_order;       /*!< The order 2^n moving average filter */
};


/** \} */ /* end of filters group*/

#endif
