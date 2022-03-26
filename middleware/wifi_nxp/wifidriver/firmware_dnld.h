/** @file firmware_dnld.h
 *
 *  @brief firmware download on SDIO API related header file
 *
 *  Copyright 2021-2022 NXP
 *
 *  NXP CONFIDENTIAL
 *  The source code contained or described herein and all documents related to
 *  the source code ("Materials") are owned by NXP, its
 *  suppliers and/or its licensors. Title to the Materials remains with NXP,
 *  its suppliers and/or its licensors. The Materials contain
 *  trade secrets and proprietary and confidential information of NXP, its
 *  suppliers and/or its licensors. The Materials are protected by worldwide copyright
 *  and trade secret laws and treaty provisions. No part of the Materials may be
 *  used, copied, reproduced, modified, published, uploaded, posted,
 *  transmitted, distributed, or disclosed in any way without NXP's prior
 *  express written permission.
 *
 *  No license under any patent, copyright, trade secret or other intellectual
 *  property right is granted to or conferred upon you by disclosure or delivery
 *  of the Materials, either expressly, by implication, inducement, estoppel or
 *  otherwise. Any license under such intellectual property rights must be
 *  express and approved by NXP in writing.
 *
 */
#ifndef _FIMRWARE_DNLD_H_
#define _FIMRWARE_DNLD_H_

/** Card Control Registers : Function 1 Block size 0 */
#define FN1_BLOCK_SIZE_0 0x110
/** Card Control Registers : Function 1 Block size 1 */
#define FN1_BLOCK_SIZE_1 0x111

#define SDIO_OUTBUF_LEN 2048U

/** The number of times to try when polling for status bits */
#define MAX_POLL_TRIES 100U

/** Firmware ready */
#define FIRMWARE_READY 0xfedcU

#define fwdnld_io_e(...) wmlog_e("wifi_io", ##__VA_ARGS__)
#define fwdnld_io_w(...) wmlog_w("wifi_io", ##__VA_ARGS__)

#ifdef CONFIG_FWDNLD_IO_DEBUG
#define fwdnld_io_d(...) wmlog("wifi_io", ##__VA_ARGS__)
#else
#define fwdnld_io_d(...)
#endif /* ! CONFIG_DWDNLD_IO_DEBUG */


typedef struct wlanfw_hdr
{
    t_u32 magic_number;
    t_u32 len;
} wlanfw_hdr_type;

enum wlan_fw_storage_type
{
    WLAN_FW_IN_FLASH = 0,
    WLAN_FW_IN_RAM,
};

int32_t firmware_download(enum wlan_fw_storage_type st, const uint8_t *fw_ram_start_addr, const size_t size);

#endif //_FIMRWARE_DNLD_H_
