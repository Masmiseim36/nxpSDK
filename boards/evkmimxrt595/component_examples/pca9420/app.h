/*
 * Copyright 2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

#include "fsl_pca9420.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define DEMO_PCA9420_INTB_HANDLER PMU_PMIC_IRQHandler
#define DEMO_PCA9420_INTB_IRQ     PMU_PMIC_IRQn
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void BOARD_ConfigPMICModes(pca9420_modecfg_t *cfg, uint32_t num);
/*${prototype:end}*/

#endif /* _APP_H_ */
