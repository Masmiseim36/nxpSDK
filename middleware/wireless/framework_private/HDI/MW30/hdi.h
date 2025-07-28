/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _HDI_H_
#define _HDI_H_

/* -------------------------------------------------------------------------- */
/*                                  Includes                                  */
/* -------------------------------------------------------------------------- */

#include <stdint.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/*                              Public functions                              */
/* -------------------------------------------------------------------------- */

/*!
 * \brief Initialize HDI interface.
 *
 * \return int 0 for success other for error.
 */
int HDI_Init(void);

/*!
 * \brief Write data to HDI in a blocking way.
 *
 * \param[in] data pointer to the data to send to HDI interface.
 * \param[in] size size of the data.
 * \return int 0 for success other for error.
 */
int HDI_WriteBlocking(const uint8_t *data, size_t size);

/*!
 * \brief Read data from HDI interface in a blocking way.
 *
 * \param[in] out point to the output buffer.
 * \param[in] size size of the data buffer.
 * \return int 0 for success other for error.
 */
int HDI_ReadBlocking(uint8_t *out, size_t size);

/*!
 * \brief Set radio to mode BLE.
 *
 */
void HDI_Set_Mode_Ble(void);

/*!
 * \brief Set radio to mode Zigbee.
 *
 */
void HDI_Set_Mode_Zigbee(void);

/*!
 * \brief Set radio to mode GFSK.
 *
 */
void HDI_Set_Mode_Gfsk(void);

/*!
 * \brief Set radio to mode CS initiator role.
 *
 */
void HDI_Set_Mode_Initiator(void);

/*!
 * \brief Set radio to mode CS reflector role.
 *
 */
void HDI_Set_Mode_Reflector(void);

#endif /* _HDI_H_ */
