/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef __WIFI_SHIELD_H__
#define __WIFI_SHIELD_H__

#include "wifi_shield_common.h"

/* Select specific shield support */
#ifndef WIFISHIELD_IS
#define WIFISHIELD_IS WIFISHIELD_IS_GT202
#endif

/* Include shields support */
#if defined(WIFISHIELD_IS)
#   if (WIFISHIELD_IS) == (WIFISHIELD_IS_GT202)
#       include "wifi_shield_gt202.h"
#   else
#       error "Unsupported shield selected !"
#   endif
#else
#   error "No shield is selected !"
#endif
/* define IRQ priority level */
#ifndef WIFISHIELD_SPI_IRQ_PRIORITY
#   define WIFISHIELD_SPI_IRQ_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif
#ifndef WIFISHIELD_DMA_IRQ_PRIORITY
#   define WIFISHIELD_DMA_IRQ_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif
#ifndef WIFISHIELD_WLAN_IRQ_PRIORITY
#   define WIFISHIELD_WLAN_IRQ_PRIORITY (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#endif

/* Fn prototypes, which need to be implemented */
A_STATUS WIFISHIELD_Init(void);
A_STATUS WIFISHIELD_InitDrivers(void *param);
A_STATUS WIFISHIELD_DeinitDrivers(void *param);
A_STATUS WIFISHIELD_PowerUp(uint32_t enable);
void WIFISHIELD_NotifyDriverTask(void *param);

#endif
