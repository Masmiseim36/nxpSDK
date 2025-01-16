/*
 * Copyright 2021-2023 NXP
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
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
status_t BOARD_GetCore1ImageAddrSize(uint32_t *pImageSrcAddr,
                                     uint32_t *pImageDestAddr,
                                     uint32_t *pImageSize,
                                     uint32_t *pImageBootAddr);
void BOARD_PrepareCore1(uint32_t image_src_addr, uint32_t image_dest_addr, uint32_t image_size, uint32_t boot_addr);
/*${prototype:end}*/

#endif /* _APP_H_ */
