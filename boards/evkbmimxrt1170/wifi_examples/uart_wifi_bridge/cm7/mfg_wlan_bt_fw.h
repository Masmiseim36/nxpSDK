/*
 *  Copyright 2021 NXP
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __MFG_WLAN_BT_FW_H__
#define __MFG_WLAN_BT_FW_H__

#include <wifi_config_default.h>

#if defined(SD8978)
#include "sduartIW416_mfg_wlan_bt.h"
#elif defined(SD8987)
#include "sduart8987_mfg_wlan_bt.h"
#elif defined(SD9177)
#ifdef CONFIG_BT_IND_DNLD
#include "uart_nw61x_mfg_se.h"
#else
#include "sduart_nw61x_mfg_se.h"
#endif
#elif defined(RW610)
const unsigned char *wlan_fw_bin   = (void *)0;
const unsigned int wlan_fw_bin_len = 0;
#elif defined(IW610)
#if !(CONFIG_WIFI_IND_DNLD) && !defined(CONFIG_BT_IND_DNLD)
#include "sduartspi_iw610_mfg_se.h"
#elif defined(CONFIG_BT_IND_DNLD)
#include "uartspi_iw610_mfg_se.h"
#else
#include "sd_iw610_mfg_se.h"
#endif
#endif

#endif /* __MFG_WLAN_BT_FW_H__ */
