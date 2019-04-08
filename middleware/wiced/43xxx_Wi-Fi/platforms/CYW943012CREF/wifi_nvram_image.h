/*
 * $ Copyright Cypress Semiconductor $
 */

#ifndef INCLUDED_NVRAM_IMAGE_H_
#define INCLUDED_NVRAM_IMAGE_H_

#include <string.h>
#include <stdint.h>
#include "../generated_mac_address.txt"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Character array of NVRAM image
 * Generated from cyw943012a0fcref_3_ipa.txt
 */

static const char wifi_nvram_image[] =
        "sromrev=11"                                                               "\x00"
        "cckdigfilttype=4"                                                         "\x00"
        "cckpwroffset0=0"                                                          "\x00"
        "bphyscale=0x20"                                                           "\x00"
        "boardflags3=0x40000100"                                                   "\x00"
        "vendid=0x14e4"                                                            "\x00"
        "devid=0xA804"                                                             "\x00"
        "manfid=0x2d0"                                                             "\x00"
        "prodid=0x052e"                                                            "\x00"
        NVRAM_GENERATED_MAC_ADDRESS                                                "\x00"
        "mac_clkgating=1"                                                          "\x00"
        "lpflags=0x00000000"                                                       "\x00"
        "nocrc=1"                                                                  "\x00"
        "boardtype=0x0843"                                                         "\x00"
        "boardrev=0x1301"                                                          "\x00"
        "xtalfreq=37400"                                                           "\x00"
        "boardflags2=0xc0000000"                                                   "\x00"
        "boardflags=0x00000000"                                                    "\x00"
        "etmode=0x11"                                                              "\x00"
        "extpagain2g=2"                                                            "\x00"
        "extpagain5g=2"                                                            "\x00"
        "ccode=0"                                                                  "\x00"
        "regrev=0"                                                                 "\x00"
        "antswitch = 0"                                                            "\x00"
        "rxgains2gelnagaina0=0"                                                    "\x00"
        "rxgains2gtrisoa0=15"                                                      "\x00"
        "rxgains2gtrelnabypa0=0"                                                   "\x00"
        "rxgains5gelnagaina0=0"                                                    "\x00"
        "rxgains5gtrisoa0=9"                                                       "\x00"
        "rxgains5gtrelnabypa0=0"                                                   "\x00"
        "pdgain5g=0"                                                               "\x00"
        "pdgain2g=0"                                                               "\x00"
        "tworangetssi2g=0"                                                         "\x00"
        "tworangetssi5g=0"                                                         "\x00"
        "rxchain=1"                                                                "\x00"
        "txchain=1"                                                                "\x00"
        "aa2g=1"                                                                   "\x00"
        "aa5g=1"                                                                   "\x00"
        "tssipos5g=0"                                                              "\x00"
        "tssipos2g=0"                                                              "\x00"
        "femctrl=17"                                                               "\x00"
        "subband5gver=4"                                                           "\x00"
        "pa2ga0=-108,5627,-638"                                                    "\x00"
        "pa5ga0=45,6957,-704,55,6994,-690,28,6922,-723,50,7043,-742"               "\x00"
        "pdoffset40ma0=0"                                                          "\x00"
        "pdoffset80ma0=0"                                                          "\x00"
        "lowpowerrange2g=0"                                                        "\x00"
        "lowpowerrange5g=0"                                                        "\x00"
        "ed_thresh2g=-63"                                                          "\x00"
        "ed_thresh5g=-63"                                                          "\x00"
        "swctrlmap_2g=0x80a080a0,0x00000020,0x0000001,0x080008,0x3ff"              "\x00"
        "swctrlmapext_2g=0x01010101,0x02020202,0x00000000,0x100010,0x003"          "\x00"
        "swctrlmap_5g=0x00200020,0x00000020,0x00000001,0x190019,0x3ff"             "\x00"
        "swctrlmapext_5g=0x00000000,0x01010101,0x00000000,0x110011,0x003"          "\x00"
        "ulpnap=0"                                                                 "\x00"
        "ulpadc=1"                                                                 "\x00"
        "ds1_nap=0"                                                                "\x00"
        "ssagc_en=0"                                                               "\x00"
        "spurcan_ch_list_MHz=2427,2432,2437,2462,5580,5805"                        "\x00"
        "spurcan_sp_freq_KHz=4000,-1000,-6000,12789,-14789,-16000"                 "\x00"
        "spurcan_NumSpur=6"                                                        "\x00"
        "epacal2g=0"                                                               "\x00"
        "epacal5g=0"                                                               "\x00"
        "papdcck=0"                                                                "\x00"
        "epacal2g_mask=0x3fff"                                                     "\x00"
        "maxp2ga0=82"                                                              "\x00"
        "ofdmlrbw202gpo=0x0031"                                                    "\x00"
        "dot11agofdmhrbw202gpo=0x7755"                                             "\x00"
        "mcsbw202gpo=0x99998775"                                                   "\x00"
        "maxp5ga0=82,82,82,82"                                                     "\x00"
        "mcsbw205glpo=0xffc97777"                                                  "\x00"
        "mcsbw205gmpo=0xffc97777"                                                  "\x00"
        "mcsbw205ghpo=0xffc97777"                                                  "\x00"
        "femctrlwar=0"                                                             "\x00"
        "use5gpllfor2g=1"                                                          "\x00"
        "pt5db_gaintbl=0"                                                          "\x00"
        "txwbpapden=1"                                                             "\x00"
        "wb_rxattn=0x0303"                                                         "\x00"
        "wb_txattn=0x0202"                                                         "\x00"
        "wb_papdcalidx=0x1010"                                                     "\x00"
        "wb_papdcalidx_5g=0x00101010"                                              "\x00"
        "wb_eps_offset=0x01bf01af"                                                 "\x00"
        "wb_eps_offset_5g=0x01bf01bf"                                              "\x00"
        "wb_bbmult=0x54"                                                           "\x00"
        "wb_bbmult_5g=0x00242424"                                                  "\x00"
        "wb_calref_db=0x1C26"                                                      "\x00"
        "wb_tia_gain_mode=0x0606"                                                  "\x00"
        "wb_txbuf_offset=0x1e1e"                                                   "\x00"
        "wb_frac_del=0x78ae"                                                       "\x00"
        "wb_g_frac_bits=0xaa"                                                      "\x00"
        "nb_rxattn=0x0303"                                                         "\x00"
        "nb_txattn=0x0303"                                                         "\x00"
        "nb_papdcalidx=0x1010"                                                     "\x00"
        "nb_eps_offset=0x01d301af"                                                 "\x00"
        "nb_tia_gain_mode=0x0000"                                                  "\x00"
        "nb_bbmult=0x2054"                                                         "\x00"
        "AvVmid_c0=6,100,7,70,7,70,7,70,7,70"                                      "\x00"
        "tssisleep_en=0x5"                                                         "\x00"
        "lpo_select=4"                                                             "\x00"
        "ATErcalmode=1"                                                            "\x00"
        "swdiv_en=1"                                                               "\x00"
        "swdiv_gpio=0"                                                             "\x00"
        "swdiv_swctrl_en=2"                                                        "\x00"
        "swdiv_swctrl_ant0=0"                                                      "\x00"
        "swdiv_swctrl_ant1=1"                                                      "\x00"
        "swdiv_antmap2g_main=1"                                                    "\x00"
        "swdiv_antmap5g_main=1"                                                    "\x00"
        "swdiv_snrlim=10000"                                                       "\x00"
        "swdiv_thresh=3000"                                                        "\x00"
        "swdiv_snrthresh=24"                                                       "\x00"
        "paprrmcsgamma2g=450,500,550,600,675,950,950,950,950,950,950,950"          "\x00"
        "paprrmcsgamma5g20=450,500,550,600,800,950,1100,1100,1100,1100,1100,1100"  "\x00"
        "bcntrim=9"                                                                "\x00"
        "deadman_to=722250000"                                                     "\x00"
        "rssicorrnorm5g_c0=-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10,-10"        "\x00"
        "lhlgpio_cfg=0x00000000,0x00000000"                                         "\x00"
        "\x00\x00";

#ifdef __cplusplus
} /*extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
#error Wi-Fi NVRAM image included twice
#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
