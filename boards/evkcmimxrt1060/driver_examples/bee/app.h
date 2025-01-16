/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define FLEXSPI_START_ADDR 0x60000000U
/* User key feature is not enabled in fuses on RT1060 */
#define DCP_USE_USER_KEY 0
/* FAC region configuration registers */
#define REG0_START_ADDR_GPR GPR18
#define REG0_END_ADDR_GPR   GPR19
#define REG0_DECRYPT_EN_GPR GPR11
#define REG0_DECRYPT_EN     IOMUXC_GPR_GPR11_BEE_DE_RX_EN(1) /* FlexSPI data decryption enabled for region-0 */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
