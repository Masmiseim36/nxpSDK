/*
 * Copyright 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _NT_FILTERS_PRV_H_
#define _NT_FILTERS_PRV_H_

#include "nt_filters.h"

/**
 * \defgroup filter_private Filters
 * \ingroup ntapi_private
 * The filters data structure that is used in the NXP Touch library.
 *
 * \{
 */

/**
 * The filter state definition.
 *
 */
enum nt_filter_state {
    NT_FILTER_STATE_INIT = 0,   /*!< The filter is initialized. */
    NT_FILTER_STATE_RUN  = 1    /*!< The filter is running correctly. */
};

/**
 * The butterworth filter context data.
 *
 */
struct nt_filter_fbutt_data {
    int32_t y;                  /*!< The last result of the filter. */
    int16_t x;                  /*!< The previous input value. */
    int32_t coeficient;         /** Coefficient computed by the input parameters. */
};


/**
 * The moving average filter context data.
 *
 */
struct nt_filter_moving_average_data {
  int32_t       sum;       /*!< The sum of the filter data */ 
};

/**
 * \defgroup filter_api_private API Functions
 * \ingroup filter_private
 * General Private Function definition of filters.
 *
 * \{
 */


#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief        Gets the absolute value.
 * \param lsrc   Input signed 32-bit number.
 * \return       Unsigned 32-bit absolute value of the input number.
 */
uint32_t _nt_abs_int32(int32_t lsrc);
  
/**
 * \brief        Initialize the ButterWorth filter for the first use.
 * \param rom    Pointer to the nt_filter_fbutt structure.
 * \param ram    Pointer to the nt_filter_fbutt_data.
 * \param signal Input signal.
 * \return none
 */
void _nt_filter_fbutt_init(const struct nt_filter_fbutt *rom, 
                           struct nt_filter_fbutt_data *ram, uint32_t signal);

/**
 * \brief Process signal fbutt filter.
 * \param ram    Pointer to the nt_filter_fbutt_data structure.
 * \param signal Input signal.
 * \return Filtered signal.
 *
 * Returns signal equal
 *
 */
uint16_t _nt_filter_fbutt_process(struct nt_filter_fbutt_data *ram,
                                 uint16_t signal);
/**
 * \brief Process signal IIR filter
 * \param rom             Pointer to nt_filter_iir
 * \param signal          Current signal.
 * \param previous_signal Previous signal
 * \return signal
 *
 * Process the signal, using the following equation:
 * y(n)=(1 / (coef + 1) * current signal + (coef / (coef + 1) * previous_signal)
 */
uint32_t _nt_filter_iir_process(const struct nt_filter_iir *rom,
                               uint32_t signal, uint32_t previous_signal);

/**
 * \brief This function initialize moving average filter.
 * \param rom Pointer to nt_filter_moving_average structure.
 * \param ram Pointer to nt_filter_moving_average_data structure.
 * \param value Input initial value.
 * \return result of operation (0 - OK, otherwise - FALSE).
 */
int32_t _nt_filter_moving_average_init(const struct nt_filter_moving_average *rom,
                                    struct nt_filter_moving_average_data *ram,
                                    uint16_t value);
/**
 * \brief This function compute moving average filter.
 * \param rom Pointer to nt_filter_moving_average structure.
 * \param ram Pointer to nt_filter_moving_average_data structure.
 * \param value Input new value.
 * \return Current value of the moving average filter.
 */ 
uint32_t _nt_filter_moving_average_process(const struct nt_filter_moving_average *rom,
                                        struct nt_filter_moving_average_data *ram,
                                        uint16_t value);
/**
 * \brief This function compute absolute value (16-bit version).
 * \param value Input signed value.
 * \return Absolute unsigned value of input.
 */ 
uint16_t _nt_filter_abs(int16_t value);


/**
 * \brief This function returns pos value (16-bit version).
 * \param value Input signed value.
 * \return value of input,if positive, else returns 0
 */ 
uint16_t _nt_filter_pos(int16_t value);

/**
 * \brief This function limit the input value in allowed range (16-bit version).
 * \param value Input value.
 * \param limit_l Limitation low range border.
 * \param limit_h Limitation high range border.
 * \return Result value.
 */ 
uint16_t _nt_filter_limit_u(int32_t value, uint16_t limit_l, uint16_t limit_h);

/**
 * \brief This function limit the input value in allowed range (16-bit version).
 * \param value Input value.
 * \param limit Limitation range.
 * \return Result value.
 */
int32_t _nt_filter_range_s(int32_t value, uint32_t limit);

/**
 * \brief This function make dead range for input value out of the allowed range (16-bit version).
 * \param value Input value.
 * \param base Base value of deadband range.
 * \param range Range of the deadband range (one half).
 * \return Result value out of the deadband range.
 */ 
uint16_t _nt_filter_deadrange_u(uint16_t value, uint16_t base, uint16_t range);

/**
 * \brief This function make positive dead range for input value out of the allowed range (16-bit version).
 * \param value Input value.
 * \param base Base value of deadband range.
 * \param range Range of the deadband range.
 * \return Result value out of the deadband range.
 */ 
uint16_t _nt_filter_deadrange_p(uint16_t value, uint16_t base, uint16_t range);

/**
 * \brief This function checks if input value is inside of the deadband range (16-bit version).
 * \param value Input value.
 * \param base Base value of deadband range.
 * \param range Range of the deadband range (one half).
 * \return Result TRUE - value is in deadband range.
                  FALSE - value is out of deadband range.
 */
int32_t _nt_filter_is_deadrange_u(uint16_t value, uint16_t base, uint16_t range);

#ifdef __cplusplus
}
#endif

/** \} */ /* end of filter_api_private group */
/** \} */ /* end of filters_private group */

#endif /* _NT_FILTERS_PRV_H_*/
