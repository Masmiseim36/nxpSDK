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

extern t_u32 ioport_g;

int32_t firmware_download(const uint8_t *fw_start_addr, const size_t size);

#endif //_FIMRWARE_DNLD_H_
