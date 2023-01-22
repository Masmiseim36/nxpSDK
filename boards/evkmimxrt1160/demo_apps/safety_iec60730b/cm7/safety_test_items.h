/*
 * Copyright 2021 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _SAFETY_TEST_ITEMS_H_
#define _SAFETY_TEST_ITEMS_H_

/* NULL terminated array of pointers to dio_test_t items for safety DIO test */
extern fs_dio_test_imx_t *g_dio_safety_test_items[];

/* NULL terminated array of pointers to aio_test_t items for safety AIO test */
extern fs_aio_test_a1_t *g_aio_safety_test_items[];

#endif /* _SAFETY_TEST_ITEMS_H_ */
