/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __SDCARD_H__
#define __SDCARD_H__

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*******************************************************************************
 * API
 ******************************************************************************/

/*
 * Mount the SD card.
 *
 * Return 0 if successed, otherwise returns negtive value.
 */
int MOUNT_SDCard(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif
