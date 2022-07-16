/** @file firmware_dnld.h
 *
 *  @brief firmware download on SDIO API related header file
 *
 *  Copyright 2021-2022 NXP
 *
 *  Licensed under the LA_OPT_NXP_Software_License.txt (the "Agreement")
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
