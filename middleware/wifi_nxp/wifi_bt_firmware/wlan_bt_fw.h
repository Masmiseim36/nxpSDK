/*
 *  Copyright 2021 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
 *
 */

#ifndef __WLAN_BT_FW_H__
#define __WLAN_BT_FW_H__

#if defined(SD8801)
#include "sd8801_wlan.h"
#elif defined(SD8978)
#include "sduartIW416_wlan_bt.h"
#elif defined(SD8987)
#include "sduart8987_wlan_bt.h"
#elif defined(IW61x)
#include "sduart_nw61x.h"
#elif defined(RW610)
static const unsigned char *wlan_fw_bin   = (void *)0;
static const unsigned int wlan_fw_bin_len = 0;
#endif

#endif /* __WLAN_BT_FW_H__ */
