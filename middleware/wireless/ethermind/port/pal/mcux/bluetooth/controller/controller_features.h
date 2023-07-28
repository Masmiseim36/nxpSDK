/*! *********************************************************************************
/* 
 * \file controller_feature.h
 * Description : This file includes APIs for feature specific to Deep-sleep and and IR
 * for BT controller modules.
 *
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 ********************************************************************************** */

#ifndef _CONTROLLER_FEATURES_H_
#define _CONTROLLER_FEATURES_H_

/*!
 * @addtogroup IOEXP_i2c
 * @{
 */

#include "BT_common.h"
#include "fsl_common.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#ifdef PCAL6408A_IO_EXP_ENABLE
/*PCAL6408A supported registers details wrto Datasheet PCAL6408A Rev3.2 */
enum {
	PCAL6408A_REG_INPUT_PORT                = 0x00,
	PCAL6408A_REG_OUTPUT_PORT               = 0x01,
	PCAL6408A_REG_POLARITY_INVERSION        = 0x02,
	PCAL6408A_REG_CONFIGURATION             = 0x03,
	PCAL6408A_REG_OUTPUT_DRIVE_STRENGTH_0   = 0x40,
	PCAL6408A_REG_OUTPUT_DRIVE_STRENGTH_1   = 0x41,
	PCAL6408A_REG_INPUT_LATCH               = 0x42,
	PCAL6408A_REG_PULL_UP_DOWN_ENABLE       = 0x43,
	PCAL6408A_REG_PULL_UP_DOWN_SELECT       = 0x44,
	PCAL6408A_REG_INTERRUPT_MASK            = 0x45,
	PCAL6408A_REG_INTERRUPT_STATUS          = 0x46,
	PCAL6408A_REG_OUTPUT_PORT_CONFIGURATION = 0x4f,
};

/*PCAL6408A supported output controllable ports details*/
typedef enum PCAL6408A_OUTPUT_PORTS{
	PCAL6408A_OUTPUT_P0 ,
	PCAL6408A_OUTPUT_P1,
	PCAL6408A_OUTPUT_P2,
	PCAL6408A_OUTPUT_P3,
	PCAL6408A_OUTPUT_P4,
	PCAL6408A_OUTPUT_P5,
	PCAL6408A_OUTPUT_P6,
	PCAL6408A_OUTPUT_P7
} pcal6408a_op_port_e;

/*IO Expander related instances*/
typedef struct pcal6408a_pins_cfg {
	uint8_t input_port_value;
	uint8_t output_port_value;
	uint8_t configured;
	uint8_t output_port_config;
	uint8_t pull_ups_selected;
	uint8_t pulls_enabled;
	uint8_t polarity;
	uint8_t ouput_drive_strength1;
	uint8_t ouput_drive_strength2;
}pcal6408a_pins_cfg_t;
#endif /*PCAL6408A_IO_EXP_ENABLE*/
/*******************************************************************************
 * API
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif

#ifdef PCAL6408A_IO_EXP_ENABLE
status_t PCAL6408A_Init(pcal6408a_pins_cfg_t *config);
status_t PCAL6408A_control_op_port(pcal6408a_op_port_e port_num, uint8_t signal);
status_t PCAL6408A_read_op_port(pcal6408a_op_port_e port_num, uint8_t* value);

#endif /*#PCAL6408A_IO_EXP_ENABLE*/

#if defined(__cplusplus)
}
#endif

/*! @} */
#endif /* _CONTROLLER_FEATURES_H_ */