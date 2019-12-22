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
 * Character array of NVRAM image generated from
 * bcm943455wlsagb_Type1MW_3.3v_180112.txt
 */

static const char wifi_nvram_image[] =
        "sromrev=11"                                                         "\x00"
        "vendid=0x14e4"                                                      "\x00"
        "devid=0x43ab"                                                       "\x00"
        "manfid=0x2d0"                                                       "\x00"
        "prodid=0x06e4"                                                      "\x00"
        NVRAM_GENERATED_MAC_ADDRESS                                          "\x00"
        "nocrc=1"                                                            "\x00"
        "boardtype=0x6e4"                                                    "\x00"
        "boardrev=0x1100"                                                    "\x00"
        "xtalfreq=37400"                                                     "\x00"
        "boardflags=0x00080001"                                              "\x00"
        "boardflags2=0x40000000"                                             "\x00"
        "boardflags3=0x48200100"                                             "\x00"
        "rxchain=1"                                                          "\x00"
        "txchain=1"                                                          "\x00"
        "aa2g=3"                                                             "\x00"
        "aa5g=3"                                                             "\x00"
        "tssipos5g=1"                                                        "\x00"
        "tssipos2g=1"                                                        "\x00"
        "AvVmid_c0=0,157,1,126,1,126,1,126,1,126"                            "\x00"
        "pa2ga0=-152,5969,-666"                                              "\x00"
        "pa2ga1=-143,4076,-546"                                              "\x00"
        "pa5ga0=-165,5736,-689,-169,5710,-692,-171,5713,-688,-173,5732,-689" "\x00"
        "itrsw=1"                                                            "\x00"
        "pdoffset40ma0=0x9999"                                               "\x00"
        "pdoffset80ma0=0x8888"                                               "\x00"
        "extpagain5g=2"                                                      "\x00"
        "extpagain2g=2"                                                      "\x00"
        "maxp2ga0=74"                                                        "\x00"
        "cckbw202gpo=0x0000"                                                 "\x00"
        "ofdmlrbw202gpo=0x0022"                                              "\x00"
        "dot11agofdmhrbw202gpo=0x8888"                                       "\x00"
        "mcsbw202gpo=0xaaaaa666"                                             "\x00"
        "maxp5ga0=76,76,76,76"                                               "\x00"
        "mcsbw205glpo=0x99999955"                                            "\x00"
        "mcsbw205gmpo=0x99999955"                                            "\x00"
        "mcsbw205ghpo=0x99999955"                                            "\x00"
        "mcsbw405glpo=0xff999555"                                            "\x00"
        "mcsbw405gmpo=0xff999555"                                            "\x00"
        "mcsbw405ghpo=0xff999555"                                            "\x00"
        "mcsbw805glpo=0xfffffbbb"                                            "\x00"
        "mcsbw805gmpo=0xfffffbbb"                                            "\x00"
        "mcsbw805ghpo=0xfffffbbb"                                            "\x00"
        "swctrlmap_2g=0x00040004,0x00020002,0x00040004,0x010a02,0x1ff"       "\x00"
        "swctrlmap_5g=0x00100010,0x00200020,0x00200020,0x010a02,0x2f4"       "\x00"
        "swctrlmapext_5g=0x00000000,0x00000000,0x00000000,0x000000,0x000"    "\x00"
        "swctrlmapext_2g=0x00000000,0x00000000,0x00000000,0x000000,0x000"    "\x00"
        "vcodivmode=1"                                                       "\x00"
        "cckdigfilttype=6"                                                   "\x00"
        "fdss_level_2g=2"                                                    "\x00"
        "fdss_level_5g=2"                                                    "\x00"
        "aga0=0x1"                                                           "\x00"
        "agbg0=0x1"                                                          "\x00"
        "ccode=0"                                                            "\x00"
        "ed_thresh2g=-65"                                                    "\x00"
        "ed_thresh5g=-65"                                                    "\x00"
        "pdoffsetcckma0=0x0000"                                              "\x00"
        "powoffs2gtna0=0,0,0,0,0,0,0,0,0,0,0,0,0,0"                          "\x00"
        "tempthresh=120"                                                     "\x00"
        "temps_hysteresis=15"                                                "\x00"
        "rawtempsense=0x1ff"                                                 "\x00"
        "tworangetssi2g=1"                                                   "\x00"
        "tworangetssi5g=0"                                                   "\x00"
        "lowpowerrange2g=0"                                                  "\x00"
        "lowpowerrange5g=0"                                                  "\x00"
        "ag0=1"                                                              "\x00"
        "ag1=1"                                                              "\x00"
        "mcslr5glpo=0x0000"                                                  "\x00"
        "mcslr5gmpo=0x0000"                                                  "\x00"
        "mcslr5ghpo=0x0000"                                                  "\x00"
        "sb20in40hrpo=0x0"                                                   "\x00"
        "sb20in80and160hr5glpo=0x0"                                          "\x00"
        "sb40and80hr5glpo=0x0"                                               "\x00"
        "sb20in80and160hr5gmpo=0x0"                                          "\x00"
        "sb40and80hr5gmpo=0x0"                                               "\x00"
        "sb20in80and160hr5ghpo=0x0"                                          "\x00"
        "sb40and80hr5ghpo=0x0"                                               "\x00"
        "sb20in40lrpo=0x0"                                                   "\x00"
        "sb20in80and160lr5glpo=0x0"                                          "\x00"
        "sb40and80lr5glpo=0x0"                                               "\x00"
        "sb20in80and160lr5gmpo=0x0"                                          "\x00"
        "sb40and80lr5gmpo=0x0"                                               "\x00"
        "sb20in80and160lr5ghpo=0x0"                                          "\x00"
        "sb40and80lr5ghpo=0x0"                                               "\x00"
        "dot11agduphrpo=0x0"                                                 "\x00"
        "dot11agduplrpo=0x0"                                                 "\x00"
        "phycal_tempdelta=25"                                                "\x00"
        "temps_period=15"                                                    "\x00"
        "btc_mode=1"                                                         "\x00"
        "sbpowoffs5g20mtna0=0,0,0,0"                                         "\x00"
        "sbpowoffs5g40mtna0=0,0,0,0"                                         "\x00"
        "sbpowoffs5g80mtna0=0,0,0,0 "                                        "\x00"
        "powoffs5g20mtna0=0,0,0,0,0,0,0"                                     "\x00"
        "powoffs5g40mtna0=0,0,0,0,0"                                         "\x00"
        "powoffs5g80mtna0=0,0,0,0,0"                                         "\x00"
        "ldo1=5"                                                             "\x00"
        "cbfilttype=2"                                                       "\x00"
        "xtal_swcapio=0x8855"                                                "\x00"
        "fdss_level_ch13=1,1"                                                "\x00"
        "swdiv_en=1"                                                         "\x00"
        "swdiv_swctrl_en=0"                                                  "\x00"
        "swdiv_gpio=5"                                                       "\x00"
        "\x00\x00";

#ifdef __cplusplus
} /*extern "C" */
#endif

#else /* ifndef INCLUDED_NVRAM_IMAGE_H_ */
#error Wi-Fi NVRAM image included twice
#endif /* ifndef INCLUDED_NVRAM_IMAGE_H_ */

