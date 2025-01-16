/*
 * Copyright 2020, 2024 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef DHARA_NAND_CONFIG_H
#define DHARA_NAND_CONFIG_H

/****************** Macro definitions ***************/
#ifndef EXAMPLE_DHARA_GARBAGE_COLLECTION_RATIO
#define EXAMPLE_DHARA_GARBAGE_COLLECTION_RATIO 255
#endif
#ifndef EXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE
#define EXAMPLE_DHARA_NAND_LOG2_PAGE_SIZE      11   /* 2048(1<<11) bytes per page. */
#endif
#ifndef EXAMPLE_DHARA_NAND_LOG2_PAGE_PER_BLOCK
#define EXAMPLE_DHARA_NAND_LOG2_PAGE_PER_BLOCK 6    /* 64(1<<6) pages per block. */
#endif
#ifndef EXAMPLE_DHARA_NAND_BLOCKS_NUMBER
#define EXAMPLE_DHARA_NAND_BLOCKS_NUMBER       2048 /* 2048 blocks per plain. */
#endif
#ifndef EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
#define EXAMPLE_DHARA_ENABLE_SOFTWARE_ECC
#endif

#endif /* DHARA_NAND_CONFIG_H */
