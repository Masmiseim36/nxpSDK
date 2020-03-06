/*
 * Copyright 2018 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TZM_CONFIG_H_
#define _TZM_CONFIG_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
#define CODE_START_NS 0x000C0000
#define CODE_SIZE_NS 0x00080000
#define CODE_FLASH_START_NS 0x08100000
#define CODE_FLASH_SIZE_NS 0x00100000
#define CODE_START_NSC 0x100BFE00
#define CODE_SIZE_NSC 0x00000200
#define CODE_FLASH_START_NSC 0x180FFE00
#define CODE_FLASH_SIZE_NSC 0x00000200
#define DATA_START_NS 0x20180000
#define DATA_SIZE_NS 0x00080000

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/* Initialize TrustZone */
void BOARD_InitTrustZone();

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _TZM_CONFIG_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
