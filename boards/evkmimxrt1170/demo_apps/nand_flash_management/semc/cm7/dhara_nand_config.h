/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DHARA_NAND_CONFIG_H
#define DHARA_NAND_CONFIG_H

/****************** Macro definitions ***************/
#define EXAMPLE_DHARA_GARBAGE_COLLECTION_RATIO 255
#define EXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE      11   /* 2048(1<<11) bytes per page. */
#define EXAMPLE_DHARA_NAND_LOG2_PAGE_PER_BLOCK 6    /* 64(1<<6) pages per block. */
#define EXAMPLE_DHARA_NAND_BLOCKS_NUMBER       2048 /* 2048 blocks per plain. */
#define EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC

#endif /* DHARA_NAND_CONFIG_H */
