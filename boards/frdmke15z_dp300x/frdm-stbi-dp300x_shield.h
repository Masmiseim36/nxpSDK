/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file frdm-stbi-dp300x_shield.h
 * @brief The frdm-stbi-dp300x_shield.h file declares mapping of the Kinetis
         Device peripherals to the frdm-ke15z and frdm-stbi-dp300x shield
 */

#ifndef _FRDM_STBI_DP300x_SHIELD_H_
#define _FRDM_STBI_DP300x_SHIELD_H_

/* The shield name */
#define SHIELD_NAME "FRDM-STBI-DP300x"

// DIFF-P Conversion Factors
#define NPS3000VV_PRESSURE_DIV_FACTOR (8)
#define NPS3001DV_PRESSURE_DIV_FACTOR (8)
#define NPS3002VV_PRESSURE_DIV_FACTOR (4)
#define NPS3005DV_PRESSURE_DIV_FACTOR (4)

// DIFF-P Sensor Information
#define DIFF_P_I2C_ADDR 0x60
#define DIFF_P_INT1 D5
#define DIFF_P_INT2 D2
#define DIFF_P_CS D10

#endif /* _FRDM_STBI_DP300x_SHIELD_H_ */
