/*
 * Copyright 2022 - 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_DEVICE_MTP_CONFIG_H_
#define _USB_DEVICE_MTP_CONFIG_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
/* USB MTP config*/
/*buffer size for mtp example. the larger the buffer size ,the faster the data transfer speed is ,*/
/*the block size should be multiple of 512, the least value is 1024*/

#define USB_DEVICE_MTP_TRANSFER_BUFF_SIZE (512 * 9U)

/*${macro:end}*/

#endif /* _USB_DEVICE_MTP_CONFIG_H_ */
