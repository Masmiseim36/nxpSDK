/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _USB_DEVICE_MSC_CONFIG_H_
#define _USB_DEVICE_MSC_CONFIG_H_
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*${macro:start}*/
/* USB MSC config*/
/*buffer size for sd card example. the larger the buffer size ,the faster the data transfer speed is ,*/
/*the block size should be multiple of 512, the least value is 512*/

#define USB_DEVICE_MSC_WRITE_BUFF_SIZE (512 * 64U)
#define USB_DEVICE_MSC_READ_BUFF_SIZE  (512 * 64U)

/*${macro:end}*/

#endif /* _USB_DEVICE_MSC_CONFIG_H_ */
