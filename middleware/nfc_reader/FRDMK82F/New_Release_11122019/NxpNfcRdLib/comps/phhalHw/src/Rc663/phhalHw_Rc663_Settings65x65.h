/*
*                     Copyright 2017, 2019, NXP
*
*       All rights are reserved. Reproduction in whole or in part is
*      prohibited without the written consent of the copyright owner.
*  NXP reserves the right to make changes without notice at any time.
* NXP makes no warranty, expressed, implied or statutory, including but
* not limited to any implied warranty of merchantability or fitness for any
*particular purpose, or that the use will not infringe any third party patent,
* copyright or trademark. NXP must not be liable for any loss or damage
*                          arising from its use.
*/

/** \file
* Rc663 EEPROM Protocol setting for 65x65 antenna.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*/

#ifndef RC663_EEPROM_DUMP_H
#define RC663_EEPROM_DUMP_H

/* Protocol : MIFARE product 106 */
#define PROTOCOL_MIFARE_106_DRVMOD_REG_A               /* EE Address */ 0x000000C0
#define PROTOCOL_MIFARE_106_DRVMOD_REG_V               /* Value      */ 0x8E
#define PROTOCOL_MIFARE_106_TXAMP_REG_A                /* EE Address */ 0x000000C1
#define PROTOCOL_MIFARE_106_TXAMP_REG_V                /* Value      */ 0x12
#define PROTOCOL_MIFARE_106_DRVCON_REG_A               /* EE Address */ 0x000000C2
#define PROTOCOL_MIFARE_106_DRVCON_REG_V               /* Value      */ 0x39
#define PROTOCOL_MIFARE_106_TXL_REG_A                  /* EE Address */ 0x000000C3
#define PROTOCOL_MIFARE_106_TXL_REG_V                  /* Value      */ 0x0A
#define PROTOCOL_MIFARE_106_TXCRCPRESET_REG_A          /* EE Address */ 0x000000C4
#define PROTOCOL_MIFARE_106_TXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_106_RXCRCPRESET_REG_A          /* EE Address */ 0x000000C5
#define PROTOCOL_MIFARE_106_RXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_106_TXDATANUM_REG_A            /* EE Address */ 0x000000C6
#define PROTOCOL_MIFARE_106_TXDATANUM_REG_V            /* Value      */ 0x0F
#define PROTOCOL_MIFARE_106_TXMODWIDTH_REG_A           /* EE Address */ 0x000000C7
#define PROTOCOL_MIFARE_106_TXMODWIDTH_REG_V           /* Value      */ 0x21
#define PROTOCOL_MIFARE_106_TXSYM10BURSTLEN_REG_A      /* EE Address */ 0x000000C8
#define PROTOCOL_MIFARE_106_TXSYM10BURSTLEN_REG_V      /* Value      */ 0x00
#define PROTOCOL_MIFARE_106_TXWAITCTRL_REG_A           /* EE Address */ 0x000000C9
#define PROTOCOL_MIFARE_106_TXWAITCTRL_REG_V           /* Value      */ 0xC0
#define PROTOCOL_MIFARE_106_TXWAITLO_REG_A             /* EE Address */ 0x000000CA
#define PROTOCOL_MIFARE_106_TXWAITLO_REG_V             /* Value      */ 0x12
#define PROTOCOL_MIFARE_106_FRAMECON_REG_A             /* EE Address */ 0x000000CB
#define PROTOCOL_MIFARE_106_FRAMECON_REG_V             /* Value      */ 0xCF
#define PROTOCOL_MIFARE_106_RXSOFD_REG_A               /* EE Address */ 0x000000CC
#define PROTOCOL_MIFARE_106_RXSOFD_REG_V               /* Value      */ 0x00
#define PROTOCOL_MIFARE_106_RXCTRL_REG_A               /* EE Address */ 0x000000CD
#define PROTOCOL_MIFARE_106_RXCTRL_REG_V               /* Value      */ 0x04
#define PROTOCOL_MIFARE_106_RXWAIT_REG_A               /* EE Address */ 0x000000CE
#define PROTOCOL_MIFARE_106_RXWAIT_REG_V               /* Value      */ 0x90
#define PROTOCOL_MIFARE_106_RXTHRESHOLD_REG_A          /* EE Address */ 0x000000CF
#define PROTOCOL_MIFARE_106_RXTHRESHOLD_REG_V          /* Value      */ 0x5C
#define PROTOCOL_MIFARE_106_RCV_REG_A                  /* EE Address */ 0x000000D0
#define PROTOCOL_MIFARE_106_RCV_REG_V                  /* Value      */ 0x12
#define PROTOCOL_MIFARE_106_RXANA_REG_A                /* EE Address */ 0x000000D1
#define PROTOCOL_MIFARE_106_RXANA_REG_V                /* Value      */ 0x0A

/* Protocol : MIFARE product 212 */
#define PROTOCOL_MIFARE_212_DRVMOD_REG_A               /* EE Address */ 0x000000D4
#define PROTOCOL_MIFARE_212_DRVMOD_REG_V               /* Value      */ 0x8E
#define PROTOCOL_MIFARE_212_TXAMP_REG_A                /* EE Address */ 0x000000D5
#define PROTOCOL_MIFARE_212_TXAMP_REG_V                /* Value      */ 0xD2
#define PROTOCOL_MIFARE_212_DRVCON_REG_A               /* EE Address */ 0x000000D6
#define PROTOCOL_MIFARE_212_DRVCON_REG_V               /* Value      */ 0x11
#define PROTOCOL_MIFARE_212_TXL_REG_A                  /* EE Address */ 0x000000D7
#define PROTOCOL_MIFARE_212_TXL_REG_V                  /* Value      */ 0x0A
#define PROTOCOL_MIFARE_212_TXCRCPRESET_REG_A          /* EE Address */ 0x000000D8
#define PROTOCOL_MIFARE_212_TXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_212_RXCRCPRESET_REG_A          /* EE Address */ 0x000000D9
#define PROTOCOL_MIFARE_212_RXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_212_TXDATANUM_REG_A            /* EE Address */ 0x000000DA
#define PROTOCOL_MIFARE_212_TXDATANUM_REG_V            /* Value      */ 0x0F
#define PROTOCOL_MIFARE_212_TXMODWIDTH_REG_A           /* EE Address */ 0x000000DB
#define PROTOCOL_MIFARE_212_TXMODWIDTH_REG_V           /* Value      */ 0x10
#define PROTOCOL_MIFARE_212_TXSYM10BURSTLEN_REG_A      /* EE Address */ 0x000000DC
#define PROTOCOL_MIFARE_212_TXSYM10BURSTLEN_REG_V      /* Value      */ 0x00
#define PROTOCOL_MIFARE_212_TXWAITCTRL_REG_A           /* EE Address */ 0x000000DD
#define PROTOCOL_MIFARE_212_TXWAITCTRL_REG_V           /* Value      */ 0xC0
#define PROTOCOL_MIFARE_212_TXWAITLO_REG_A             /* EE Address */ 0x000000DE
#define PROTOCOL_MIFARE_212_TXWAITLO_REG_V             /* Value      */ 0x12
#define PROTOCOL_MIFARE_212_FRAMECON_REG_A             /* EE Address */ 0x000000DF
#define PROTOCOL_MIFARE_212_FRAMECON_REG_V             /* Value      */ 0xCF
#define PROTOCOL_MIFARE_212_RXSOFD_REG_A               /* EE Address */ 0x000000E0
#define PROTOCOL_MIFARE_212_RXSOFD_REG_V               /* Value      */ 0x00
#define PROTOCOL_MIFARE_212_RXCTRL_REG_A               /* EE Address */ 0x000000E1
#define PROTOCOL_MIFARE_212_RXCTRL_REG_V               /* Value      */ 0x05
#define PROTOCOL_MIFARE_212_RXWAIT_REG_A               /* EE Address */ 0x000000E2
#define PROTOCOL_MIFARE_212_RXWAIT_REG_V               /* Value      */ 0x90
#define PROTOCOL_MIFARE_212_RXTHRESHOLD_REG_A          /* EE Address */ 0x000000E3
#define PROTOCOL_MIFARE_212_RXTHRESHOLD_REG_V          /* Value      */ 0x3C
#define PROTOCOL_MIFARE_212_RCV_REG_A                  /* EE Address */ 0x000000E4
#define PROTOCOL_MIFARE_212_RCV_REG_V                  /* Value      */ 0x12
#define PROTOCOL_MIFARE_212_RXANA_REG_A                /* EE Address */ 0x000000E5
#define PROTOCOL_MIFARE_212_RXANA_REG_V                /* Value      */ 0x0B

/* Protocol : MIFARE product 424 */
#define PROTOCOL_MIFARE_424_DRVMOD_REG_A               /* EE Address */ 0x000000E8
#define PROTOCOL_MIFARE_424_DRVMOD_REG_V               /* Value      */ 0x8F
#define PROTOCOL_MIFARE_424_TXAMP_REG_A                /* EE Address */ 0x000000E9
#define PROTOCOL_MIFARE_424_TXAMP_REG_V                /* Value      */ 0xDE
#define PROTOCOL_MIFARE_424_DRVCON_REG_A               /* EE Address */ 0x000000EA
#define PROTOCOL_MIFARE_424_DRVCON_REG_V               /* Value      */ 0x11
#define PROTOCOL_MIFARE_424_TXL_REG_A                  /* EE Address */ 0x000000EB
#define PROTOCOL_MIFARE_424_TXL_REG_V                  /* Value      */ 0x0F
#define PROTOCOL_MIFARE_424_TXCRCPRESET_REG_A          /* EE Address */ 0x000000EC
#define PROTOCOL_MIFARE_424_TXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_424_RXCRCPRESET_REG_A          /* EE Address */ 0x000000ED
#define PROTOCOL_MIFARE_424_RXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_424_TXDATANUM_REG_A            /* EE Address */ 0x000000EE
#define PROTOCOL_MIFARE_424_TXDATANUM_REG_V            /* Value      */ 0x0F
#define PROTOCOL_MIFARE_424_TXMODWIDTH_REG_A           /* EE Address */ 0x000000EF
#define PROTOCOL_MIFARE_424_TXMODWIDTH_REG_V           /* Value      */ 0x07
#define PROTOCOL_MIFARE_424_TXSYM10BURSTLEN_REG_A      /* EE Address */ 0x000000F0
#define PROTOCOL_MIFARE_424_TXSYM10BURSTLEN_REG_V      /* Value      */ 0x00
#define PROTOCOL_MIFARE_424_TXWAITCTRL_REG_A           /* EE Address */ 0x000000F1
#define PROTOCOL_MIFARE_424_TXWAITCTRL_REG_V           /* Value      */ 0xC0
#define PROTOCOL_MIFARE_424_TXWAITLO_REG_A             /* EE Address */ 0x000000F2
#define PROTOCOL_MIFARE_424_TXWAITLO_REG_V             /* Value      */ 0x12
#define PROTOCOL_MIFARE_424_FRAMECON_REG_A             /* EE Address */ 0x000000F3
#define PROTOCOL_MIFARE_424_FRAMECON_REG_V             /* Value      */ 0xCF
#define PROTOCOL_MIFARE_424_RXSOFD_REG_A               /* EE Address */ 0x000000F4
#define PROTOCOL_MIFARE_424_RXSOFD_REG_V               /* Value      */ 0x00
#define PROTOCOL_MIFARE_424_RXCTRL_REG_A               /* EE Address */ 0x000000F5
#define PROTOCOL_MIFARE_424_RXCTRL_REG_V               /* Value      */ 0x06
#define PROTOCOL_MIFARE_424_RXWAIT_REG_A               /* EE Address */ 0x000000F6
#define PROTOCOL_MIFARE_424_RXWAIT_REG_V               /* Value      */ 0x90
#define PROTOCOL_MIFARE_424_RXTHRESHOLD_REG_A          /* EE Address */ 0x000000F7
#define PROTOCOL_MIFARE_424_RXTHRESHOLD_REG_V          /* Value      */ 0x2B
#define PROTOCOL_MIFARE_424_RCV_REG_A                  /* EE Address */ 0x000000F8
#define PROTOCOL_MIFARE_424_RCV_REG_V                  /* Value      */ 0x12
#define PROTOCOL_MIFARE_424_RXANA_REG_A                /* EE Address */ 0x000000F9
#define PROTOCOL_MIFARE_424_RXANA_REG_V                /* Value      */ 0x0B

/* Protocol : MIFARE product 848 */
#define PROTOCOL_MIFARE_848_DRVMOD_REG_A               /* EE Address */ 0x00000100
#define PROTOCOL_MIFARE_848_DRVMOD_REG_V               /* Value      */ 0x8F
#define PROTOCOL_MIFARE_848_TXAMP_REG_A                /* EE Address */ 0x00000101
#define PROTOCOL_MIFARE_848_TXAMP_REG_V                /* Value      */ 0xDB
#define PROTOCOL_MIFARE_848_DRVCON_REG_A               /* EE Address */ 0x00000102
#define PROTOCOL_MIFARE_848_DRVCON_REG_V               /* Value      */ 0x21
#define PROTOCOL_MIFARE_848_TXL_REG_A                  /* EE Address */ 0x00000103
#define PROTOCOL_MIFARE_848_TXL_REG_V                  /* Value      */ 0x0F
#define PROTOCOL_MIFARE_848_TXCRCPRESET_REG_A          /* EE Address */ 0x00000104
#define PROTOCOL_MIFARE_848_TXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_848_RXCRCPRESET_REG_A          /* EE Address */ 0x00000105
#define PROTOCOL_MIFARE_848_RXCRCPRESET_REG_V          /* Value      */ 0x18
#define PROTOCOL_MIFARE_848_TXDATANUM_REG_A            /* EE Address */ 0x00000106
#define PROTOCOL_MIFARE_848_TXDATANUM_REG_V            /* Value      */ 0x0F
#define PROTOCOL_MIFARE_848_TXMODWIDTH_REG_A           /* EE Address */ 0x00000107
#define PROTOCOL_MIFARE_848_TXMODWIDTH_REG_V           /* Value      */ 0x02
#define PROTOCOL_MIFARE_848_TXSYM10BURSTLEN_REG_A      /* EE Address */ 0x00000108
#define PROTOCOL_MIFARE_848_TXSYM10BURSTLEN_REG_V      /* Value      */ 0x00
#define PROTOCOL_MIFARE_848_TXWAITCTRL_REG_A           /* EE Address */ 0x00000109
#define PROTOCOL_MIFARE_848_TXWAITCTRL_REG_V           /* Value      */ 0xC0
#define PROTOCOL_MIFARE_848_TXWAITLO_REG_A             /* EE Address */ 0x0000010A
#define PROTOCOL_MIFARE_848_TXWAITLO_REG_V             /* Value      */ 0x12
#define PROTOCOL_MIFARE_848_FRAMECON_REG_A             /* EE Address */ 0x0000010B
#define PROTOCOL_MIFARE_848_FRAMECON_REG_V             /* Value      */ 0xCF
#define PROTOCOL_MIFARE_848_RXSOFD_REG_A               /* EE Address */ 0x0000010C
#define PROTOCOL_MIFARE_848_RXSOFD_REG_V               /* Value      */ 0x00
#define PROTOCOL_MIFARE_848_RXCTRL_REG_A               /* EE Address */ 0x0000010D
#define PROTOCOL_MIFARE_848_RXCTRL_REG_V               /* Value      */ 0x07
#define PROTOCOL_MIFARE_848_RXWAIT_REG_A               /* EE Address */ 0x0000010E
#define PROTOCOL_MIFARE_848_RXWAIT_REG_V               /* Value      */ 0x90
#define PROTOCOL_MIFARE_848_RXTHRESHOLD_REG_A          /* EE Address */ 0x0000010F
#define PROTOCOL_MIFARE_848_RXTHRESHOLD_REG_V          /* Value      */ 0x3A
#define PROTOCOL_MIFARE_848_RCV_REG_A                  /* EE Address */ 0x00000110
#define PROTOCOL_MIFARE_848_RCV_REG_V                  /* Value      */ 0x12
#define PROTOCOL_MIFARE_848_RXANA_REG_A                /* EE Address */ 0x00000111
#define PROTOCOL_MIFARE_848_RXANA_REG_V                /* Value      */ 0x0B

/* Protocol : TypeB 106 */
#define PROTOCOL_TYPEB_106_DRVMOD_REG_A                /* EE Address */ 0x00000114
#define PROTOCOL_TYPEB_106_DRVMOD_REG_V                /* Value      */ 0x8F
#define PROTOCOL_TYPEB_106_TXAMP_REG_A                 /* EE Address */ 0x00000115
#define PROTOCOL_TYPEB_106_TXAMP_REG_V                 /* Value      */ 0x0E
#define PROTOCOL_TYPEB_106_DRVCON_REG_A                /* EE Address */ 0x00000116
#define PROTOCOL_TYPEB_106_DRVCON_REG_V                /* Value      */ 0x09
#define PROTOCOL_TYPEB_106_TXL_REG_A                   /* EE Address */ 0x00000117
#define PROTOCOL_TYPEB_106_TXL_REG_V                   /* Value      */ 0x0A
#define PROTOCOL_TYPEB_106_TXCRCPRESET_REG_A           /* EE Address */ 0x00000118
#define PROTOCOL_TYPEB_106_TXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_106_RXCRCPRESET_REG_A           /* EE Address */ 0x00000119
#define PROTOCOL_TYPEB_106_RXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_106_TXDATANUM_REG_A             /* EE Address */ 0x0000011A
#define PROTOCOL_TYPEB_106_TXDATANUM_REG_V             /* Value      */ 0x08
#define PROTOCOL_TYPEB_106_TXMODWIDTH_REG_A            /* EE Address */ 0x0000011B
#define PROTOCOL_TYPEB_106_TXMODWIDTH_REG_V            /* Value      */ 0x00
#define PROTOCOL_TYPEB_106_TXSYM10BURSTLEN_REG_A       /* EE Address */ 0x0000011C
#define PROTOCOL_TYPEB_106_TXSYM10BURSTLEN_REG_V       /* Value      */ 0x00
#define PROTOCOL_TYPEB_106_TXWAITCTRL_REG_A            /* EE Address */ 0x0000011D
#define PROTOCOL_TYPEB_106_TXWAITCTRL_REG_V            /* Value      */ 0x01
#define PROTOCOL_TYPEB_106_TXWAITLO_REG_A              /* EE Address */ 0x0000011E
#define PROTOCOL_TYPEB_106_TXWAITLO_REG_V              /* Value      */ 0x00
#define PROTOCOL_TYPEB_106_FRAMECON_REG_A              /* EE Address */ 0x0000011F
#define PROTOCOL_TYPEB_106_FRAMECON_REG_V              /* Value      */ 0x05
#define PROTOCOL_TYPEB_106_RXSOFD_REG_A                /* EE Address */ 0x00000120
#define PROTOCOL_TYPEB_106_RXSOFD_REG_V                /* Value      */ 0x00
#define PROTOCOL_TYPEB_106_RXCTRL_REG_A                /* EE Address */ 0x00000121
#define PROTOCOL_TYPEB_106_RXCTRL_REG_V                /* Value      */ 0x34
#define PROTOCOL_TYPEB_106_RXWAIT_REG_A                /* EE Address */ 0x00000122
#define PROTOCOL_TYPEB_106_RXWAIT_REG_V                /* Value      */ 0x90
#define PROTOCOL_TYPEB_106_RXTHRESHOLD_REG_A           /* EE Address */ 0x00000123
#define PROTOCOL_TYPEB_106_RXTHRESHOLD_REG_V           /* Value      */ 0x6F
#define PROTOCOL_TYPEB_106_RCV_REG_A                   /* EE Address */ 0x00000124
#define PROTOCOL_TYPEB_106_RCV_REG_V                   /* Value      */ 0x12
#define PROTOCOL_TYPEB_106_RXANA_REG_A                 /* EE Address */ 0x00000125
#define PROTOCOL_TYPEB_106_RXANA_REG_V                 /* Value      */ 0x03

/* Protocol : TypeB 212 */
#define PROTOCOL_TYPEB_212_DRVMOD_REG_A                /* EE Address */ 0x00000128
#define PROTOCOL_TYPEB_212_DRVMOD_REG_V                /* Value      */ 0x8F
#define PROTOCOL_TYPEB_212_TXAMP_REG_A                 /* EE Address */ 0x00000129
#define PROTOCOL_TYPEB_212_TXAMP_REG_V                 /* Value      */ 0x0E
#define PROTOCOL_TYPEB_212_DRVCON_REG_A                /* EE Address */ 0x0000012A
#define PROTOCOL_TYPEB_212_DRVCON_REG_V                /* Value      */ 0x09
#define PROTOCOL_TYPEB_212_TXL_REG_A                   /* EE Address */ 0x0000012B
#define PROTOCOL_TYPEB_212_TXL_REG_V                   /* Value      */ 0x0A
#define PROTOCOL_TYPEB_212_TXCRCPRESET_REG_A           /* EE Address */ 0x0000012C
#define PROTOCOL_TYPEB_212_TXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_212_RXCRCPRESET_REG_A           /* EE Address */ 0x0000012D
#define PROTOCOL_TYPEB_212_RXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_212_TXDATANUM_REG_A             /* EE Address */ 0x0000012E
#define PROTOCOL_TYPEB_212_TXDATANUM_REG_V             /* Value      */ 0x08
#define PROTOCOL_TYPEB_212_TXMODWIDTH_REG_A            /* EE Address */ 0x0000012F
#define PROTOCOL_TYPEB_212_TXMODWIDTH_REG_V            /* Value      */ 0x00
#define PROTOCOL_TYPEB_212_TXSYM10BURSTLEN_REG_A       /* EE Address */ 0x00000130
#define PROTOCOL_TYPEB_212_TXSYM10BURSTLEN_REG_V       /* Value      */ 0x00
#define PROTOCOL_TYPEB_212_TXWAITCTRL_REG_A            /* EE Address */ 0x00000131
#define PROTOCOL_TYPEB_212_TXWAITCTRL_REG_V            /* Value      */ 0x01
#define PROTOCOL_TYPEB_212_TXWAITLO_REG_A              /* EE Address */ 0x00000132
#define PROTOCOL_TYPEB_212_TXWAITLO_REG_V              /* Value      */ 0x00
#define PROTOCOL_TYPEB_212_FRAMECON_REG_A              /* EE Address */ 0x00000133
#define PROTOCOL_TYPEB_212_FRAMECON_REG_V              /* Value      */ 0x05
#define PROTOCOL_TYPEB_212_RXSOFD_REG_A                /* EE Address */ 0x00000134
#define PROTOCOL_TYPEB_212_RXSOFD_REG_V                /* Value      */ 0x00
#define PROTOCOL_TYPEB_212_RXCTRL_REG_A                /* EE Address */ 0x00000135
#define PROTOCOL_TYPEB_212_RXCTRL_REG_V                /* Value      */ 0x35
#define PROTOCOL_TYPEB_212_RXWAIT_REG_A                /* EE Address */ 0x00000136
#define PROTOCOL_TYPEB_212_RXWAIT_REG_V                /* Value      */ 0x90
#define PROTOCOL_TYPEB_212_RXTHRESHOLD_REG_A           /* EE Address */ 0x00000137
#define PROTOCOL_TYPEB_212_RXTHRESHOLD_REG_V           /* Value      */ 0x3F
#define PROTOCOL_TYPEB_212_RCV_REG_A                   /* EE Address */ 0x00000138
#define PROTOCOL_TYPEB_212_RCV_REG_V                   /* Value      */ 0x12
#define PROTOCOL_TYPEB_212_RXANA_REG_A                 /* EE Address */ 0x00000139
#define PROTOCOL_TYPEB_212_RXANA_REG_V                 /* Value      */ 0x03

/* Protocol : TypeB 424 */
#define PROTOCOL_TYPEB_424_DRVMOD_REG_A                /* EE Address */ 0x00000140
#define PROTOCOL_TYPEB_424_DRVMOD_REG_V                /* Value      */ 0x8F
#define PROTOCOL_TYPEB_424_TXAMP_REG_A                 /* EE Address */ 0x00000141
#define PROTOCOL_TYPEB_424_TXAMP_REG_V                 /* Value      */ 0x0F
#define PROTOCOL_TYPEB_424_DRVCON_REG_A                /* EE Address */ 0x00000142
#define PROTOCOL_TYPEB_424_DRVCON_REG_V                /* Value      */ 0x09
#define PROTOCOL_TYPEB_424_TXL_REG_A                   /* EE Address */ 0x00000143
#define PROTOCOL_TYPEB_424_TXL_REG_V                   /* Value      */ 0x0A
#define PROTOCOL_TYPEB_424_TXCRCPRESET_REG_A           /* EE Address */ 0x00000144
#define PROTOCOL_TYPEB_424_TXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_424_RXCRCPRESET_REG_A           /* EE Address */ 0x00000145
#define PROTOCOL_TYPEB_424_RXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_424_TXDATANUM_REG_A             /* EE Address */ 0x00000146
#define PROTOCOL_TYPEB_424_TXDATANUM_REG_V             /* Value      */ 0x08
#define PROTOCOL_TYPEB_424_TXMODWIDTH_REG_A            /* EE Address */ 0x00000147
#define PROTOCOL_TYPEB_424_TXMODWIDTH_REG_V            /* Value      */ 0x00
#define PROTOCOL_TYPEB_424_TXSYM10BURSTLEN_REG_A       /* EE Address */ 0x00000148
#define PROTOCOL_TYPEB_424_TXSYM10BURSTLEN_REG_V       /* Value      */ 0x00
#define PROTOCOL_TYPEB_424_TXWAITCTRL_REG_A            /* EE Address */ 0x00000149
#define PROTOCOL_TYPEB_424_TXWAITCTRL_REG_V            /* Value      */ 0x01
#define PROTOCOL_TYPEB_424_TXWAITLO_REG_A              /* EE Address */ 0x0000014A
#define PROTOCOL_TYPEB_424_TXWAITLO_REG_V              /* Value      */ 0x00
#define PROTOCOL_TYPEB_424_FRAMECON_REG_A              /* EE Address */ 0x0000014B
#define PROTOCOL_TYPEB_424_FRAMECON_REG_V              /* Value      */ 0x05
#define PROTOCOL_TYPEB_424_RXSOFD_REG_A                /* EE Address */ 0x0000014C
#define PROTOCOL_TYPEB_424_RXSOFD_REG_V                /* Value      */ 0x00
#define PROTOCOL_TYPEB_424_RXCTRL_REG_A                /* EE Address */ 0x0000014D
#define PROTOCOL_TYPEB_424_RXCTRL_REG_V                /* Value      */ 0x36
#define PROTOCOL_TYPEB_424_RXWAIT_REG_A                /* EE Address */ 0x0000014E
#define PROTOCOL_TYPEB_424_RXWAIT_REG_V                /* Value      */ 0x90
#define PROTOCOL_TYPEB_424_RXTHRESHOLD_REG_A           /* EE Address */ 0x0000014F
#define PROTOCOL_TYPEB_424_RXTHRESHOLD_REG_V           /* Value      */ 0x3F
#define PROTOCOL_TYPEB_424_RCV_REG_A                   /* EE Address */ 0x00000150
#define PROTOCOL_TYPEB_424_RCV_REG_V                   /* Value      */ 0x12
#define PROTOCOL_TYPEB_424_RXANA_REG_A                 /* EE Address */ 0x00000151
#define PROTOCOL_TYPEB_424_RXANA_REG_V                 /* Value      */ 0x03

/* Protocol : TypeB 848 */
#define PROTOCOL_TYPEB_848_DRVMOD_REG_A                /* EE Address */ 0x00000154
#define PROTOCOL_TYPEB_848_DRVMOD_REG_V                /* Value      */ 0x8F
#define PROTOCOL_TYPEB_848_TXAMP_REG_A                 /* EE Address */ 0x00000155
#define PROTOCOL_TYPEB_848_TXAMP_REG_V                 /* Value      */ 0x10
#define PROTOCOL_TYPEB_848_DRVCON_REG_A                /* EE Address */ 0x00000156
#define PROTOCOL_TYPEB_848_DRVCON_REG_V                /* Value      */ 0x09
#define PROTOCOL_TYPEB_848_TXL_REG_A                   /* EE Address */ 0x00000157
#define PROTOCOL_TYPEB_848_TXL_REG_V                   /* Value      */ 0x0A
#define PROTOCOL_TYPEB_848_TXCRCPRESET_REG_A           /* EE Address */ 0x00000158
#define PROTOCOL_TYPEB_848_TXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_848_RXCRCPRESET_REG_A           /* EE Address */ 0x00000159
#define PROTOCOL_TYPEB_848_RXCRCPRESET_REG_V           /* Value      */ 0x7B
#define PROTOCOL_TYPEB_848_TXDATANUM_REG_A             /* EE Address */ 0x0000015A
#define PROTOCOL_TYPEB_848_TXDATANUM_REG_V             /* Value      */ 0x08
#define PROTOCOL_TYPEB_848_TXMODWIDTH_REG_A            /* EE Address */ 0x0000015B
#define PROTOCOL_TYPEB_848_TXMODWIDTH_REG_V            /* Value      */ 0x00
#define PROTOCOL_TYPEB_848_TXSYM10BURSTLEN_REG_A       /* EE Address */ 0x0000015C
#define PROTOCOL_TYPEB_848_TXSYM10BURSTLEN_REG_V       /* Value      */ 0x00
#define PROTOCOL_TYPEB_848_TXWAITCTRL_REG_A            /* EE Address */ 0x0000015D
#define PROTOCOL_TYPEB_848_TXWAITCTRL_REG_V            /* Value      */ 0x01
#define PROTOCOL_TYPEB_848_TXWAITLO_REG_A              /* EE Address */ 0x0000015E
#define PROTOCOL_TYPEB_848_TXWAITLO_REG_V              /* Value      */ 0x00
#define PROTOCOL_TYPEB_848_FRAMECON_REG_A              /* EE Address */ 0x0000015F
#define PROTOCOL_TYPEB_848_FRAMECON_REG_V              /* Value      */ 0x05
#define PROTOCOL_TYPEB_848_RXSOFD_REG_A                /* EE Address */ 0x00000160
#define PROTOCOL_TYPEB_848_RXSOFD_REG_V                /* Value      */ 0x00
#define PROTOCOL_TYPEB_848_RXCTRL_REG_A                /* EE Address */ 0x00000161
#define PROTOCOL_TYPEB_848_RXCTRL_REG_V                /* Value      */ 0x37
#define PROTOCOL_TYPEB_848_RXWAIT_REG_A                /* EE Address */ 0x00000162
#define PROTOCOL_TYPEB_848_RXWAIT_REG_V                /* Value      */ 0x90
#define PROTOCOL_TYPEB_848_RXTHRESHOLD_REG_A           /* EE Address */ 0x00000163
#define PROTOCOL_TYPEB_848_RXTHRESHOLD_REG_V           /* Value      */ 0x3F
#define PROTOCOL_TYPEB_848_RCV_REG_A                   /* EE Address */ 0x00000164
#define PROTOCOL_TYPEB_848_RCV_REG_V                   /* Value      */ 0x12
#define PROTOCOL_TYPEB_848_RXANA_REG_A                 /* EE Address */ 0x00000165
#define PROTOCOL_TYPEB_848_RXANA_REG_V                 /* Value      */ 0x03

/* Protocol : FeliCa 212 */
#define PROTOCOL_FELICA_212_DRVMOD_REG_A               /* EE Address */ 0x00000168
#define PROTOCOL_FELICA_212_DRVMOD_REG_V               /* Value      */ 0x8F
#define PROTOCOL_FELICA_212_TXAMP_REG_A                /* EE Address */ 0x00000169
#define PROTOCOL_FELICA_212_TXAMP_REG_V                /* Value      */ 0x17
#define PROTOCOL_FELICA_212_DRVCON_REG_A               /* EE Address */ 0x0000016A
#define PROTOCOL_FELICA_212_DRVCON_REG_V               /* Value      */ 0x01
#define PROTOCOL_FELICA_212_TXL_REG_A                  /* EE Address */ 0x0000016B
#define PROTOCOL_FELICA_212_TXL_REG_V                  /* Value      */ 0x06
#define PROTOCOL_FELICA_212_TXCRCPRESET_REG_A          /* EE Address */ 0x0000016C
#define PROTOCOL_FELICA_212_TXCRCPRESET_REG_V          /* Value      */ 0x09
#define PROTOCOL_FELICA_212_RXCRCPRESET_REG_A          /* EE Address */ 0x0000016D
#define PROTOCOL_FELICA_212_RXCRCPRESET_REG_V          /* Value      */ 0x09
#define PROTOCOL_FELICA_212_TXDATANUM_REG_A            /* EE Address */ 0x0000016E
#define PROTOCOL_FELICA_212_TXDATANUM_REG_V            /* Value      */ 0x08
#define PROTOCOL_FELICA_212_TXMODWIDTH_REG_A           /* EE Address */ 0x0000016F
#define PROTOCOL_FELICA_212_TXMODWIDTH_REG_V           /* Value      */ 0x00
#define PROTOCOL_FELICA_212_TXSYM10BURSTLEN_REG_A      /* EE Address */ 0x00000170
#define PROTOCOL_FELICA_212_TXSYM10BURSTLEN_REG_V      /* Value      */ 0x03
#define PROTOCOL_FELICA_212_TXWAITCTRL_REG_A           /* EE Address */ 0x00000171
#define PROTOCOL_FELICA_212_TXWAITCTRL_REG_V           /* Value      */ 0x80
#define PROTOCOL_FELICA_212_TXWAITLO_REG_A             /* EE Address */ 0x00000172
#define PROTOCOL_FELICA_212_TXWAITLO_REG_V             /* Value      */ 0x12
#define PROTOCOL_FELICA_212_FRAMECON_REG_A             /* EE Address */ 0x00000173
#define PROTOCOL_FELICA_212_FRAMECON_REG_V             /* Value      */ 0x01
#define PROTOCOL_FELICA_212_RXSOFD_REG_A               /* EE Address */ 0x00000174
#define PROTOCOL_FELICA_212_RXSOFD_REG_V               /* Value      */ 0x00
#define PROTOCOL_FELICA_212_RXCTRL_REG_A               /* EE Address */ 0x00000175
#define PROTOCOL_FELICA_212_RXCTRL_REG_V               /* Value      */ 0x05
#define PROTOCOL_FELICA_212_RXWAIT_REG_A               /* EE Address */ 0x00000176
#define PROTOCOL_FELICA_212_RXWAIT_REG_V               /* Value      */ 0x86
#define PROTOCOL_FELICA_212_RXTHRESHOLD_REG_A          /* EE Address */ 0x00000177
#define PROTOCOL_FELICA_212_RXTHRESHOLD_REG_V          /* Value      */ 0x3F
#define PROTOCOL_FELICA_212_RCV_REG_A                  /* EE Address */ 0x00000178
#define PROTOCOL_FELICA_212_RCV_REG_V                  /* Value      */ 0x12
#define PROTOCOL_FELICA_212_RXANA_REG_A                /* EE Address */ 0x00000179
#define PROTOCOL_FELICA_212_RXANA_REG_V                /* Value      */ 0x02

/* Protocol : FeliCa 424 */
#define PROTOCOL_FELICA_424_DRVMOD_REG_A               /* EE Address */ 0x00000180
#define PROTOCOL_FELICA_424_DRVMOD_REG_V               /* Value      */ 0x8F
#define PROTOCOL_FELICA_424_TXAMP_REG_A                /* EE Address */ 0x00000181
#define PROTOCOL_FELICA_424_TXAMP_REG_V                /* Value      */ 0x17
#define PROTOCOL_FELICA_424_DRVCON_REG_A               /* EE Address */ 0x00000182
#define PROTOCOL_FELICA_424_DRVCON_REG_V               /* Value      */ 0x01
#define PROTOCOL_FELICA_424_TXL_REG_A                  /* EE Address */ 0x00000183
#define PROTOCOL_FELICA_424_TXL_REG_V                  /* Value      */ 0x06
#define PROTOCOL_FELICA_424_TXCRCPRESET_REG_A          /* EE Address */ 0x00000184
#define PROTOCOL_FELICA_424_TXCRCPRESET_REG_V          /* Value      */ 0x09
#define PROTOCOL_FELICA_424_RXCRCPRESET_REG_A          /* EE Address */ 0x00000185
#define PROTOCOL_FELICA_424_RXCRCPRESET_REG_V          /* Value      */ 0x09
#define PROTOCOL_FELICA_424_TXDATANUM_REG_A            /* EE Address */ 0x00000186
#define PROTOCOL_FELICA_424_TXDATANUM_REG_V            /* Value      */ 0x08
#define PROTOCOL_FELICA_424_TXMODWIDTH_REG_A           /* EE Address */ 0x00000187
#define PROTOCOL_FELICA_424_TXMODWIDTH_REG_V           /* Value      */ 0x00
#define PROTOCOL_FELICA_424_TXSYM10BURSTLEN_REG_A      /* EE Address */ 0x00000188
#define PROTOCOL_FELICA_424_TXSYM10BURSTLEN_REG_V      /* Value      */ 0x03
#define PROTOCOL_FELICA_424_TXWAITCTRL_REG_A           /* EE Address */ 0x00000189
#define PROTOCOL_FELICA_424_TXWAITCTRL_REG_V           /* Value      */ 0x80
#define PROTOCOL_FELICA_424_TXWAITLO_REG_A             /* EE Address */ 0x0000018A
#define PROTOCOL_FELICA_424_TXWAITLO_REG_V             /* Value      */ 0x12
#define PROTOCOL_FELICA_424_FRAMECON_REG_A             /* EE Address */ 0x0000018B
#define PROTOCOL_FELICA_424_FRAMECON_REG_V             /* Value      */ 0x01
#define PROTOCOL_FELICA_424_RXSOFD_REG_A               /* EE Address */ 0x0000018C
#define PROTOCOL_FELICA_424_RXSOFD_REG_V               /* Value      */ 0x00
#define PROTOCOL_FELICA_424_RXCTRL_REG_A               /* EE Address */ 0x0000018D
#define PROTOCOL_FELICA_424_RXCTRL_REG_V               /* Value      */ 0x06
#define PROTOCOL_FELICA_424_RXWAIT_REG_A               /* EE Address */ 0x0000018E
#define PROTOCOL_FELICA_424_RXWAIT_REG_V               /* Value      */ 0x86
#define PROTOCOL_FELICA_424_RXTHRESHOLD_REG_A          /* EE Address */ 0x0000018F
#define PROTOCOL_FELICA_424_RXTHRESHOLD_REG_V          /* Value      */ 0x3F
#define PROTOCOL_FELICA_424_RCV_REG_A                  /* EE Address */ 0x00000190
#define PROTOCOL_FELICA_424_RCV_REG_V                  /* Value      */ 0x12
#define PROTOCOL_FELICA_424_RXANA_REG_A                /* EE Address */ 0x00000191
#define PROTOCOL_FELICA_424_RXANA_REG_V                /* Value      */ 0x02

/* Protocol : SLI 1/4 SSC26 */
#define PROTOCOL_SLI_1_4_SSC26_DRVMOD_REG_A            /* EE Address */ 0x00000194
#define PROTOCOL_SLI_1_4_SSC26_DRVMOD_REG_V            /* Value      */ 0x89
#define PROTOCOL_SLI_1_4_SSC26_TXAMP_REG_A             /* EE Address */ 0x00000195
#define PROTOCOL_SLI_1_4_SSC26_TXAMP_REG_V             /* Value      */ 0x10
#define PROTOCOL_SLI_1_4_SSC26_DRVCON_REG_A            /* EE Address */ 0x00000196
#define PROTOCOL_SLI_1_4_SSC26_DRVCON_REG_V            /* Value      */ 0x09
#define PROTOCOL_SLI_1_4_SSC26_TXL_REG_A               /* EE Address */ 0x00000197
#define PROTOCOL_SLI_1_4_SSC26_TXL_REG_V               /* Value      */ 0x0A
#define PROTOCOL_SLI_1_4_SSC26_TXCRCPRESET_REG_A       /* EE Address */ 0x00000198
#define PROTOCOL_SLI_1_4_SSC26_TXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_SLI_1_4_SSC26_RXCRCPRESET_REG_A       /* EE Address */ 0x00000199
#define PROTOCOL_SLI_1_4_SSC26_RXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_SLI_1_4_SSC26_TXDATANUM_REG_A         /* EE Address */ 0x0000019A
#define PROTOCOL_SLI_1_4_SSC26_TXDATANUM_REG_V         /* Value      */ 0x08
#define PROTOCOL_SLI_1_4_SSC26_TXMODWIDTH_REG_A        /* EE Address */ 0x0000019B
#define PROTOCOL_SLI_1_4_SSC26_TXMODWIDTH_REG_V        /* Value      */ 0x00
#define PROTOCOL_SLI_1_4_SSC26_TXSYM10BURSTLEN_REG_A   /* EE Address */ 0x0000019C
#define PROTOCOL_SLI_1_4_SSC26_TXSYM10BURSTLEN_REG_V   /* Value      */ 0x00
#define PROTOCOL_SLI_1_4_SSC26_TXWAITCTRL_REG_A        /* EE Address */ 0x0000019D
#define PROTOCOL_SLI_1_4_SSC26_TXWAITCTRL_REG_V        /* Value      */ 0x88
#define PROTOCOL_SLI_1_4_SSC26_TXWAITLO_REG_A          /* EE Address */ 0x0000019E
#define PROTOCOL_SLI_1_4_SSC26_TXWAITLO_REG_V          /* Value      */ 0xA9
#define PROTOCOL_SLI_1_4_SSC26_FRAMECON_REG_A          /* EE Address */ 0x0000019F
#define PROTOCOL_SLI_1_4_SSC26_FRAMECON_REG_V          /* Value      */ 0x0F
#define PROTOCOL_SLI_1_4_SSC26_RXSOFD_REG_A            /* EE Address */ 0x000001A0
#define PROTOCOL_SLI_1_4_SSC26_RXSOFD_REG_V            /* Value      */ 0x00
#define PROTOCOL_SLI_1_4_SSC26_RXCTRL_REG_A            /* EE Address */ 0x000001A1
#define PROTOCOL_SLI_1_4_SSC26_RXCTRL_REG_V            /* Value      */ 0x02
#define PROTOCOL_SLI_1_4_SSC26_RXWAIT_REG_A            /* EE Address */ 0x000001A2
#define PROTOCOL_SLI_1_4_SSC26_RXWAIT_REG_V            /* Value      */ 0x9C
#define PROTOCOL_SLI_1_4_SSC26_RXTHRESHOLD_REG_A       /* EE Address */ 0x000001A3
#define PROTOCOL_SLI_1_4_SSC26_RXTHRESHOLD_REG_V       /* Value      */ 0x74
#define PROTOCOL_SLI_1_4_SSC26_RCV_REG_A               /* EE Address */ 0x000001A4
#define PROTOCOL_SLI_1_4_SSC26_RCV_REG_V               /* Value      */ 0x12
#define PROTOCOL_SLI_1_4_SSC26_RXANA_REG_A             /* EE Address */ 0x000001A5
#define PROTOCOL_SLI_1_4_SSC26_RXANA_REG_V             /* Value      */ 0x07

/* Protocol : SLI 1/4 SSC53 */
#define PROTOCOL_SLI_1_4_SSC53_DRVMOD_REG_A            /* EE Address */ 0x000001A8
#define PROTOCOL_SLI_1_4_SSC53_DRVMOD_REG_V            /* Value      */ 0x89
#define PROTOCOL_SLI_1_4_SSC53_TXAMP_REG_A             /* EE Address */ 0x000001A9
#define PROTOCOL_SLI_1_4_SSC53_TXAMP_REG_V             /* Value      */ 0x10
#define PROTOCOL_SLI_1_4_SSC53_DRVCON_REG_A            /* EE Address */ 0x000001AA
#define PROTOCOL_SLI_1_4_SSC53_DRVCON_REG_V            /* Value      */ 0x09
#define PROTOCOL_SLI_1_4_SSC53_TXL_REG_A               /* EE Address */ 0x000001AB
#define PROTOCOL_SLI_1_4_SSC53_TXL_REG_V               /* Value      */ 0x0A
#define PROTOCOL_SLI_1_4_SSC53_TXCRCPRESET_REG_A       /* EE Address */ 0x000001AC
#define PROTOCOL_SLI_1_4_SSC53_TXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_SLI_1_4_SSC53_RXCRCPRESET_REG_A       /* EE Address */ 0x000001AD
#define PROTOCOL_SLI_1_4_SSC53_RXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_SLI_1_4_SSC53_TXDATANUM_REG_A         /* EE Address */ 0x000001AE
#define PROTOCOL_SLI_1_4_SSC53_TXDATANUM_REG_V         /* Value      */ 0x08
#define PROTOCOL_SLI_1_4_SSC53_TXMODWIDTH_REG_A        /* EE Address */ 0x000001AF
#define PROTOCOL_SLI_1_4_SSC53_TXMODWIDTH_REG_V        /* Value      */ 0x00
#define PROTOCOL_SLI_1_4_SSC53_TXSYM10BURSTLEN_REG_A   /* EE Address */ 0x000001B0
#define PROTOCOL_SLI_1_4_SSC53_TXSYM10BURSTLEN_REG_V   /* Value      */ 0x00
#define PROTOCOL_SLI_1_4_SSC53_TXWAITCTRL_REG_A        /* EE Address */ 0x000001B1
#define PROTOCOL_SLI_1_4_SSC53_TXWAITCTRL_REG_V        /* Value      */ 0x88
#define PROTOCOL_SLI_1_4_SSC53_TXWAITLO_REG_A          /* EE Address */ 0x000001B2
#define PROTOCOL_SLI_1_4_SSC53_TXWAITLO_REG_V          /* Value      */ 0xA9
#define PROTOCOL_SLI_1_4_SSC53_FRAMECON_REG_A          /* EE Address */ 0x000001B3
#define PROTOCOL_SLI_1_4_SSC53_FRAMECON_REG_V          /* Value      */ 0x0F
#define PROTOCOL_SLI_1_4_SSC53_RXSOFD_REG_A            /* EE Address */ 0x000001B4
#define PROTOCOL_SLI_1_4_SSC53_RXSOFD_REG_V            /* Value      */ 0x00
#define PROTOCOL_SLI_1_4_SSC53_RXCTRL_REG_A            /* EE Address */ 0x000001B5
#define PROTOCOL_SLI_1_4_SSC53_RXCTRL_REG_V            /* Value      */ 0x03
#define PROTOCOL_SLI_1_4_SSC53_RXWAIT_REG_A            /* EE Address */ 0x000001B6
#define PROTOCOL_SLI_1_4_SSC53_RXWAIT_REG_V            /* Value      */ 0x9C
#define PROTOCOL_SLI_1_4_SSC53_RXTHRESHOLD_REG_A       /* EE Address */ 0x000001B7
#define PROTOCOL_SLI_1_4_SSC53_RXTHRESHOLD_REG_V       /* Value      */ 0x74
#define PROTOCOL_SLI_1_4_SSC53_RCV_REG_A               /* EE Address */ 0x000001B8
#define PROTOCOL_SLI_1_4_SSC53_RCV_REG_V               /* Value      */ 0x12
#define PROTOCOL_SLI_1_4_SSC53_RXANA_REG_A             /* EE Address */ 0x000001B9
#define PROTOCOL_SLI_1_4_SSC53_RXANA_REG_V             /* Value      */ 0x03

/* Protocol : SLI 1/256 DSC */
#define PROTOCOL_SLI_1_256_DSC_DRVMOD_REG_A            /* EE Address */ 0x000001C0
#define PROTOCOL_SLI_1_256_DSC_DRVMOD_REG_V            /* Value      */ 0x8E
#define PROTOCOL_SLI_1_256_DSC_TXAMP_REG_A             /* EE Address */ 0x000001C1
#define PROTOCOL_SLI_1_256_DSC_TXAMP_REG_V             /* Value      */ 0x10
#define PROTOCOL_SLI_1_256_DSC_DRVCON_REG_A            /* EE Address */ 0x000001C2
#define PROTOCOL_SLI_1_256_DSC_DRVCON_REG_V            /* Value      */ 0x01
#define PROTOCOL_SLI_1_256_DSC_TXL_REG_A               /* EE Address */ 0x000001C3
#define PROTOCOL_SLI_1_256_DSC_TXL_REG_V               /* Value      */ 0x06
#define PROTOCOL_SLI_1_256_DSC_TXCRCPRESET_REG_A       /* EE Address */ 0x000001C4
#define PROTOCOL_SLI_1_256_DSC_TXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_SLI_1_256_DSC_RXCRCPRESET_REG_A       /* EE Address */ 0x000001C5
#define PROTOCOL_SLI_1_256_DSC_RXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_SLI_1_256_DSC_TXDATANUM_REG_A         /* EE Address */ 0x000001C6
#define PROTOCOL_SLI_1_256_DSC_TXDATANUM_REG_V         /* Value      */ 0x08
#define PROTOCOL_SLI_1_256_DSC_TXMODWIDTH_REG_A        /* EE Address */ 0x000001C7
#define PROTOCOL_SLI_1_256_DSC_TXMODWIDTH_REG_V        /* Value      */ 0x00
#define PROTOCOL_SLI_1_256_DSC_TXSYM10BURSTLEN_REG_A   /* EE Address */ 0x000001C8
#define PROTOCOL_SLI_1_256_DSC_TXSYM10BURSTLEN_REG_V   /* Value      */ 0x00
#define PROTOCOL_SLI_1_256_DSC_TXWAITCTRL_REG_A        /* EE Address */ 0x000001C9
#define PROTOCOL_SLI_1_256_DSC_TXWAITCTRL_REG_V        /* Value      */ 0x88
#define PROTOCOL_SLI_1_256_DSC_TXWAITLO_REG_A          /* EE Address */ 0x000001CA
#define PROTOCOL_SLI_1_256_DSC_TXWAITLO_REG_V          /* Value      */ 0xA9
#define PROTOCOL_SLI_1_256_DSC_FRAMECON_REG_A          /* EE Address */ 0x000001CB
#define PROTOCOL_SLI_1_256_DSC_FRAMECON_REG_V          /* Value      */ 0x0F
#define PROTOCOL_SLI_1_256_DSC_RXSOFD_REG_A            /* EE Address */ 0x000001CC
#define PROTOCOL_SLI_1_256_DSC_RXSOFD_REG_V            /* Value      */ 0x00
#define PROTOCOL_SLI_1_256_DSC_RXCTRL_REG_A            /* EE Address */ 0x000001CD
#define PROTOCOL_SLI_1_256_DSC_RXCTRL_REG_V            /* Value      */ 0x02
#define PROTOCOL_SLI_1_256_DSC_RXWAIT_REG_A            /* EE Address */ 0x000001CE
#define PROTOCOL_SLI_1_256_DSC_RXWAIT_REG_V            /* Value      */ 0x10
#define PROTOCOL_SLI_1_256_DSC_RXTHRESHOLD_REG_A       /* EE Address */ 0x000001CF
#define PROTOCOL_SLI_1_256_DSC_RXTHRESHOLD_REG_V       /* Value      */ 0x44
#define PROTOCOL_SLI_1_256_DSC_RCV_REG_A               /* EE Address */ 0x000001D0
#define PROTOCOL_SLI_1_256_DSC_RCV_REG_V               /* Value      */ 0x12
#define PROTOCOL_SLI_1_256_DSC_RXANA_REG_A             /* EE Address */ 0x000001D1
#define PROTOCOL_SLI_1_256_DSC_RXANA_REG_V             /* Value      */ 0x06

/* Protocol : EPC / UID SSC26 */
#define PROTOCOL_EPC_UID_SSC26_DRVMOD_REG_A            /* EE Address */ 0x000001D4
#define PROTOCOL_EPC_UID_SSC26_DRVMOD_REG_V            /* Value      */ 0x8F
#define PROTOCOL_EPC_UID_SSC26_TXAMP_REG_A             /* EE Address */ 0x000001D5
#define PROTOCOL_EPC_UID_SSC26_TXAMP_REG_V             /* Value      */ 0x10
#define PROTOCOL_EPC_UID_SSC26_DRVCON_REG_A            /* EE Address */ 0x000001D6
#define PROTOCOL_EPC_UID_SSC26_DRVCON_REG_V            /* Value      */ 0x01
#define PROTOCOL_EPC_UID_SSC26_TXL_REG_A               /* EE Address */ 0x000001D7
#define PROTOCOL_EPC_UID_SSC26_TXL_REG_V               /* Value      */ 0x06
#define PROTOCOL_EPC_UID_SSC26_TXCRCPRESET_REG_A       /* EE Address */ 0x000001D8
#define PROTOCOL_EPC_UID_SSC26_TXCRCPRESET_REG_V       /* Value      */ 0x74
#define PROTOCOL_EPC_UID_SSC26_RXCRCPRESET_REG_A       /* EE Address */ 0x000001D9
#define PROTOCOL_EPC_UID_SSC26_RXCRCPRESET_REG_V       /* Value      */ 0x7B
#define PROTOCOL_EPC_UID_SSC26_TXDATANUM_REG_A         /* EE Address */ 0x000001DA
#define PROTOCOL_EPC_UID_SSC26_TXDATANUM_REG_V         /* Value      */ 0x18
#define PROTOCOL_EPC_UID_SSC26_TXMODWIDTH_REG_A        /* EE Address */ 0x000001DB
#define PROTOCOL_EPC_UID_SSC26_TXMODWIDTH_REG_V        /* Value      */ 0x00
#define PROTOCOL_EPC_UID_SSC26_TXSYM10BURSTLEN_REG_A   /* EE Address */ 0x000001DC
#define PROTOCOL_EPC_UID_SSC26_TXSYM10BURSTLEN_REG_V   /* Value      */ 0x00
#define PROTOCOL_EPC_UID_SSC26_TXWAITCTRL_REG_A        /* EE Address */ 0x000001DD
#define PROTOCOL_EPC_UID_SSC26_TXWAITCTRL_REG_V        /* Value      */ 0x50
#define PROTOCOL_EPC_UID_SSC26_TXWAITLO_REG_A          /* EE Address */ 0x000001DE
#define PROTOCOL_EPC_UID_SSC26_TXWAITLO_REG_V          /* Value      */ 0x5C
#define PROTOCOL_EPC_UID_SSC26_FRAMECON_REG_A          /* EE Address */ 0x000001DF
#define PROTOCOL_EPC_UID_SSC26_FRAMECON_REG_V          /* Value      */ 0x0F
#define PROTOCOL_EPC_UID_SSC26_RXSOFD_REG_A            /* EE Address */ 0x000001E0
#define PROTOCOL_EPC_UID_SSC26_RXSOFD_REG_V            /* Value      */ 0x00
#define PROTOCOL_EPC_UID_SSC26_RXCTRL_REG_A            /* EE Address */ 0x000001E1
#define PROTOCOL_EPC_UID_SSC26_RXCTRL_REG_V            /* Value      */ 0x03
#define PROTOCOL_EPC_UID_SSC26_RXWAIT_REG_A            /* EE Address */ 0x000001E2
#define PROTOCOL_EPC_UID_SSC26_RXWAIT_REG_V            /* Value      */ 0x10
#define PROTOCOL_EPC_UID_SSC26_RXTHRESHOLD_REG_A       /* EE Address */ 0x000001E3
#define PROTOCOL_EPC_UID_SSC26_RXTHRESHOLD_REG_V       /* Value      */ 0x4E
#define PROTOCOL_EPC_UID_SSC26_RCV_REG_A               /* EE Address */ 0x000001E4
#define PROTOCOL_EPC_UID_SSC26_RCV_REG_V               /* Value      */ 0x12
#define PROTOCOL_EPC_UID_SSC26_RXANA_REG_A             /* EE Address */ 0x000001E5
#define PROTOCOL_EPC_UID_SSC26_RXANA_REG_V             /* Value      */ 0x06

/* Protocol : EPC V2 2/424 */
#define PROTOCOL_EPC_V2_2_424_DRVMOD_REG_A             /* EE Address */ 0x000001E8
#define PROTOCOL_EPC_V2_2_424_DRVMOD_REG_V             /* Value      */ 0x8F
#define PROTOCOL_EPC_V2_2_424_TXAMP_REG_A              /* EE Address */ 0x000001E9
#define PROTOCOL_EPC_V2_2_424_TXAMP_REG_V              /* Value      */ 0x10
#define PROTOCOL_EPC_V2_2_424_DRVCON_REG_A             /* EE Address */ 0x000001EA
#define PROTOCOL_EPC_V2_2_424_DRVCON_REG_V             /* Value      */ 0x09
#define PROTOCOL_EPC_V2_2_424_TXL_REG_A                /* EE Address */ 0x000001EB
#define PROTOCOL_EPC_V2_2_424_TXL_REG_V                /* Value      */ 0x0A
#define PROTOCOL_EPC_V2_2_424_TXCRCPRESET_REG_A        /* EE Address */ 0x000001EC
#define PROTOCOL_EPC_V2_2_424_TXCRCPRESET_REG_V        /* Value      */ 0x11
#define PROTOCOL_EPC_V2_2_424_RXCRCPRESET_REG_A        /* EE Address */ 0x000001ED
#define PROTOCOL_EPC_V2_2_424_RXCRCPRESET_REG_V        /* Value      */ 0x91
#define PROTOCOL_EPC_V2_2_424_TXDATANUM_REG_A          /* EE Address */ 0x000001EE
#define PROTOCOL_EPC_V2_2_424_TXDATANUM_REG_V          /* Value      */ 0x09
#define PROTOCOL_EPC_V2_2_424_TXMODWIDTH_REG_A         /* EE Address */ 0x000001EF
#define PROTOCOL_EPC_V2_2_424_TXMODWIDTH_REG_V         /* Value      */ 0x00
#define PROTOCOL_EPC_V2_2_424_TXSYM10BURSTLEN_REG_A    /* EE Address */ 0x000001F0
#define PROTOCOL_EPC_V2_2_424_TXSYM10BURSTLEN_REG_V    /* Value      */ 0x00
#define PROTOCOL_EPC_V2_2_424_TXWAITCTRL_REG_A         /* EE Address */ 0x000001F1
#define PROTOCOL_EPC_V2_2_424_TXWAITCTRL_REG_V         /* Value      */ 0x80
#define PROTOCOL_EPC_V2_2_424_TXWAITLO_REG_A           /* EE Address */ 0x000001F2
#define PROTOCOL_EPC_V2_2_424_TXWAITLO_REG_V           /* Value      */ 0x12
#define PROTOCOL_EPC_V2_2_424_FRAMECON_REG_A           /* EE Address */ 0x000001F3
#define PROTOCOL_EPC_V2_2_424_FRAMECON_REG_V           /* Value      */ 0x01
#define PROTOCOL_EPC_V2_2_424_RXSOFD_REG_A             /* EE Address */ 0x000001F4
#define PROTOCOL_EPC_V2_2_424_RXSOFD_REG_V             /* Value      */ 0x00
#define PROTOCOL_EPC_V2_2_424_RXCTRL_REG_A             /* EE Address */ 0x000001F5
#define PROTOCOL_EPC_V2_2_424_RXCTRL_REG_V             /* Value      */ 0x04
#define PROTOCOL_EPC_V2_2_424_RXWAIT_REG_A             /* EE Address */ 0x000001F6
#define PROTOCOL_EPC_V2_2_424_RXWAIT_REG_V             /* Value      */ 0xA0
#define PROTOCOL_EPC_V2_2_424_RXTHRESHOLD_REG_A        /* EE Address */ 0x000001F7
#define PROTOCOL_EPC_V2_2_424_RXTHRESHOLD_REG_V        /* Value      */ 0x76
#define PROTOCOL_EPC_V2_2_424_RCV_REG_A                /* EE Address */ 0x000001F8
#define PROTOCOL_EPC_V2_2_424_RCV_REG_V                /* Value      */ 0x12
#define PROTOCOL_EPC_V2_2_424_RXANA_REG_A              /* EE Address */ 0x000001F9
#define PROTOCOL_EPC_V2_2_424_RXANA_REG_V              /* Value      */ 0x0F

/* Protocol : EPC V2 4/424 */
#define PROTOCOL_EPC_V2_4_424_DRVMOD_REG_A             /* EE Address */ 0x00000200
#define PROTOCOL_EPC_V2_4_424_DRVMOD_REG_V             /* Value      */ 0x8F
#define PROTOCOL_EPC_V2_4_424_TXAMP_REG_A              /* EE Address */ 0x00000201
#define PROTOCOL_EPC_V2_4_424_TXAMP_REG_V              /* Value      */ 0x10
#define PROTOCOL_EPC_V2_4_424_DRVCON_REG_A             /* EE Address */ 0x00000202
#define PROTOCOL_EPC_V2_4_424_DRVCON_REG_V             /* Value      */ 0x09
#define PROTOCOL_EPC_V2_4_424_TXL_REG_A                /* EE Address */ 0x00000203
#define PROTOCOL_EPC_V2_4_424_TXL_REG_V                /* Value      */ 0x0A
#define PROTOCOL_EPC_V2_4_424_TXCRCPRESET_REG_A        /* EE Address */ 0x00000204
#define PROTOCOL_EPC_V2_4_424_TXCRCPRESET_REG_V        /* Value      */ 0x11
#define PROTOCOL_EPC_V2_4_424_RXCRCPRESET_REG_A        /* EE Address */ 0x00000205
#define PROTOCOL_EPC_V2_4_424_RXCRCPRESET_REG_V        /* Value      */ 0x91
#define PROTOCOL_EPC_V2_4_424_TXDATANUM_REG_A          /* EE Address */ 0x00000206
#define PROTOCOL_EPC_V2_4_424_TXDATANUM_REG_V          /* Value      */ 0x09
#define PROTOCOL_EPC_V2_4_424_TXMODWIDTH_REG_A         /* EE Address */ 0x00000207
#define PROTOCOL_EPC_V2_4_424_TXMODWIDTH_REG_V         /* Value      */ 0x00
#define PROTOCOL_EPC_V2_4_424_TXSYM10BURSTLEN_REG_A    /* EE Address */ 0x00000208
#define PROTOCOL_EPC_V2_4_424_TXSYM10BURSTLEN_REG_V    /* Value      */ 0x00
#define PROTOCOL_EPC_V2_4_424_TXWAITCTRL_REG_A         /* EE Address */ 0x00000209
#define PROTOCOL_EPC_V2_4_424_TXWAITCTRL_REG_V         /* Value      */ 0x80
#define PROTOCOL_EPC_V2_4_424_TXWAITLO_REG_A           /* EE Address */ 0x0000020A
#define PROTOCOL_EPC_V2_4_424_TXWAITLO_REG_V           /* Value      */ 0x12
#define PROTOCOL_EPC_V2_4_424_FRAMECON_REG_A           /* EE Address */ 0x0000020B
#define PROTOCOL_EPC_V2_4_424_FRAMECON_REG_V           /* Value      */ 0x01
#define PROTOCOL_EPC_V2_4_424_RXSOFD_REG_A             /* EE Address */ 0x0000020C
#define PROTOCOL_EPC_V2_4_424_RXSOFD_REG_V             /* Value      */ 0x00
#define PROTOCOL_EPC_V2_4_424_RXCTRL_REG_A             /* EE Address */ 0x0000020D
#define PROTOCOL_EPC_V2_4_424_RXCTRL_REG_V             /* Value      */ 0x03
#define PROTOCOL_EPC_V2_4_424_RXWAIT_REG_A             /* EE Address */ 0x0000020E
#define PROTOCOL_EPC_V2_4_424_RXWAIT_REG_V             /* Value      */ 0xA0
#define PROTOCOL_EPC_V2_4_424_RXTHRESHOLD_REG_A        /* EE Address */ 0x0000020F
#define PROTOCOL_EPC_V2_4_424_RXTHRESHOLD_REG_V        /* Value      */ 0x56
#define PROTOCOL_EPC_V2_4_424_RCV_REG_A                /* EE Address */ 0x00000210
#define PROTOCOL_EPC_V2_4_424_RCV_REG_V                /* Value      */ 0x12
#define PROTOCOL_EPC_V2_4_424_RXANA_REG_A              /* EE Address */ 0x00000211
#define PROTOCOL_EPC_V2_4_424_RXANA_REG_V              /* Value      */ 0x0F

/* Protocol : EPC V2 2/848 */
#define PROTOCOL_EPC_V2_2_848_DRVMOD_REG_A             /* EE Address */ 0x00000214
#define PROTOCOL_EPC_V2_2_848_DRVMOD_REG_V             /* Value      */ 0x8F
#define PROTOCOL_EPC_V2_2_848_TXAMP_REG_A              /* EE Address */ 0x00000215
#define PROTOCOL_EPC_V2_2_848_TXAMP_REG_V              /* Value      */ 0xD0
#define PROTOCOL_EPC_V2_2_848_DRVCON_REG_A             /* EE Address */ 0x00000216
#define PROTOCOL_EPC_V2_2_848_DRVCON_REG_V             /* Value      */ 0x01
#define PROTOCOL_EPC_V2_2_848_TXL_REG_A                /* EE Address */ 0x00000217
#define PROTOCOL_EPC_V2_2_848_TXL_REG_V                /* Value      */ 0x0A
#define PROTOCOL_EPC_V2_2_848_TXCRCPRESET_REG_A        /* EE Address */ 0x00000218
#define PROTOCOL_EPC_V2_2_848_TXCRCPRESET_REG_V        /* Value      */ 0x11
#define PROTOCOL_EPC_V2_2_848_RXCRCPRESET_REG_A        /* EE Address */ 0x00000219
#define PROTOCOL_EPC_V2_2_848_RXCRCPRESET_REG_V        /* Value      */ 0x91
#define PROTOCOL_EPC_V2_2_848_TXDATANUM_REG_A          /* EE Address */ 0x0000021A
#define PROTOCOL_EPC_V2_2_848_TXDATANUM_REG_V          /* Value      */ 0x09
#define PROTOCOL_EPC_V2_2_848_TXMODWIDTH_REG_A         /* EE Address */ 0x0000021B
#define PROTOCOL_EPC_V2_2_848_TXMODWIDTH_REG_V         /* Value      */ 0x00
#define PROTOCOL_EPC_V2_2_848_TXSYM10BURSTLEN_REG_A    /* EE Address */ 0x0000021C
#define PROTOCOL_EPC_V2_2_848_TXSYM10BURSTLEN_REG_V    /* Value      */ 0x00
#define PROTOCOL_EPC_V2_2_848_TXWAITCTRL_REG_A         /* EE Address */ 0x0000021D
#define PROTOCOL_EPC_V2_2_848_TXWAITCTRL_REG_V         /* Value      */ 0x80
#define PROTOCOL_EPC_V2_2_848_TXWAITLO_REG_A           /* EE Address */ 0x0000021E
#define PROTOCOL_EPC_V2_2_848_TXWAITLO_REG_V           /* Value      */ 0x12
#define PROTOCOL_EPC_V2_2_848_FRAMECON_REG_A           /* EE Address */ 0x0000021F
#define PROTOCOL_EPC_V2_2_848_FRAMECON_REG_V           /* Value      */ 0x01
#define PROTOCOL_EPC_V2_2_848_RXSOFD_REG_A             /* EE Address */ 0x00000220
#define PROTOCOL_EPC_V2_2_848_RXSOFD_REG_V             /* Value      */ 0x00
#define PROTOCOL_EPC_V2_2_848_RXCTRL_REG_A             /* EE Address */ 0x00000221
#define PROTOCOL_EPC_V2_2_848_RXCTRL_REG_V             /* Value      */ 0x05
#define PROTOCOL_EPC_V2_2_848_RXWAIT_REG_A             /* EE Address */ 0x00000222
#define PROTOCOL_EPC_V2_2_848_RXWAIT_REG_V             /* Value      */ 0xA0
#define PROTOCOL_EPC_V2_2_848_RXTHRESHOLD_REG_A        /* EE Address */ 0x00000223
#define PROTOCOL_EPC_V2_2_848_RXTHRESHOLD_REG_V        /* Value      */ 0x26
#define PROTOCOL_EPC_V2_2_848_RCV_REG_A                /* EE Address */ 0x00000224
#define PROTOCOL_EPC_V2_2_848_RCV_REG_V                /* Value      */ 0x12
#define PROTOCOL_EPC_V2_2_848_RXANA_REG_A              /* EE Address */ 0x00000225
#define PROTOCOL_EPC_V2_2_848_RXANA_REG_V              /* Value      */ 0x0E

/* Protocol : EPC V2 4/848 */
#define PROTOCOL_EPC_V2_4_848_DRVMOD_REG_A             /* EE Address */ 0x00000228
#define PROTOCOL_EPC_V2_4_848_DRVMOD_REG_V             /* Value      */ 0x8F
#define PROTOCOL_EPC_V2_4_848_TXAMP_REG_A              /* EE Address */ 0x00000229
#define PROTOCOL_EPC_V2_4_848_TXAMP_REG_V              /* Value      */ 0x10
#define PROTOCOL_EPC_V2_4_848_DRVCON_REG_A             /* EE Address */ 0x0000022A
#define PROTOCOL_EPC_V2_4_848_DRVCON_REG_V             /* Value      */ 0x09
#define PROTOCOL_EPC_V2_4_848_TXL_REG_A                /* EE Address */ 0x0000022B
#define PROTOCOL_EPC_V2_4_848_TXL_REG_V                /* Value      */ 0x0A
#define PROTOCOL_EPC_V2_4_848_TXCRCPRESET_REG_A        /* EE Address */ 0x0000022C
#define PROTOCOL_EPC_V2_4_848_TXCRCPRESET_REG_V        /* Value      */ 0x11
#define PROTOCOL_EPC_V2_4_848_RXCRCPRESET_REG_A        /* EE Address */ 0x0000022D
#define PROTOCOL_EPC_V2_4_848_RXCRCPRESET_REG_V        /* Value      */ 0x91
#define PROTOCOL_EPC_V2_4_848_TXDATANUM_REG_A          /* EE Address */ 0x0000022E
#define PROTOCOL_EPC_V2_4_848_TXDATANUM_REG_V          /* Value      */ 0x09
#define PROTOCOL_EPC_V2_4_848_TXMODWIDTH_REG_A         /* EE Address */ 0x0000022F
#define PROTOCOL_EPC_V2_4_848_TXMODWIDTH_REG_V         /* Value      */ 0x00
#define PROTOCOL_EPC_V2_4_848_TXSYM10BURSTLEN_REG_A    /* EE Address */ 0x00000230
#define PROTOCOL_EPC_V2_4_848_TXSYM10BURSTLEN_REG_V    /* Value      */ 0x00
#define PROTOCOL_EPC_V2_4_848_TXWAITCTRL_REG_A         /* EE Address */ 0x00000231
#define PROTOCOL_EPC_V2_4_848_TXWAITCTRL_REG_V         /* Value      */ 0x80
#define PROTOCOL_EPC_V2_4_848_TXWAITLO_REG_A           /* EE Address */ 0x00000232
#define PROTOCOL_EPC_V2_4_848_TXWAITLO_REG_V           /* Value      */ 0x12
#define PROTOCOL_EPC_V2_4_848_FRAMECON_REG_A           /* EE Address */ 0x00000233
#define PROTOCOL_EPC_V2_4_848_FRAMECON_REG_V           /* Value      */ 0x01
#define PROTOCOL_EPC_V2_4_848_RXSOFD_REG_A             /* EE Address */ 0x00000234
#define PROTOCOL_EPC_V2_4_848_RXSOFD_REG_V             /* Value      */ 0x00
#define PROTOCOL_EPC_V2_4_848_RXCTRL_REG_A             /* EE Address */ 0x00000235
#define PROTOCOL_EPC_V2_4_848_RXCTRL_REG_V             /* Value      */ 0x04
#define PROTOCOL_EPC_V2_4_848_RXWAIT_REG_A             /* EE Address */ 0x00000236
#define PROTOCOL_EPC_V2_4_848_RXWAIT_REG_V             /* Value      */ 0xA0
#define PROTOCOL_EPC_V2_4_848_RXTHRESHOLD_REG_A        /* EE Address */ 0x00000237
#define PROTOCOL_EPC_V2_4_848_RXTHRESHOLD_REG_V        /* Value      */ 0x56
#define PROTOCOL_EPC_V2_4_848_RCV_REG_A                /* EE Address */ 0x00000238
#define PROTOCOL_EPC_V2_4_848_RCV_REG_V                /* Value      */ 0x12
#define PROTOCOL_EPC_V2_4_848_RXANA_REG_A              /* EE Address */ 0x00000239
#define PROTOCOL_EPC_V2_4_848_RXANA_REG_V              /* Value      */ 0x0F

/* Protocol : Jewel */
#define PROTOCOL_JEWEL_DRVMOD_REG_A                    /* EE Address */ 0x00000240
#define PROTOCOL_JEWEL_DRVMOD_REG_V                    /* Value      */ 0x8E
#define PROTOCOL_JEWEL_TXAMP_REG_A                     /* EE Address */ 0x00000241
#define PROTOCOL_JEWEL_TXAMP_REG_V                     /* Value      */ 0x15
#define PROTOCOL_JEWEL_DRVCON_REG_A                    /* EE Address */ 0x00000242
#define PROTOCOL_JEWEL_DRVCON_REG_V                    /* Value      */ 0x11
#define PROTOCOL_JEWEL_TXL_REG_A                       /* EE Address */ 0x00000243
#define PROTOCOL_JEWEL_TXL_REG_V                       /* Value      */ 0x06
#define PROTOCOL_JEWEL_TXCRCPRESET_REG_A               /* EE Address */ 0x00000244
#define PROTOCOL_JEWEL_TXCRCPRESET_REG_V               /* Value      */ 0x18
#define PROTOCOL_JEWEL_RXCRCPRESET_REG_A               /* EE Address */ 0x00000245
#define PROTOCOL_JEWEL_RXCRCPRESET_REG_V               /* Value      */ 0x18
#define PROTOCOL_JEWEL_TXDATANUM_REG_A                 /* EE Address */ 0x00000246
#define PROTOCOL_JEWEL_TXDATANUM_REG_V                 /* Value      */ 0x0F
#define PROTOCOL_JEWEL_TXMODWIDTH_REG_A                /* EE Address */ 0x00000247
#define PROTOCOL_JEWEL_TXMODWIDTH_REG_V                /* Value      */ 0x20
#define PROTOCOL_JEWEL_TXSYM10BURSTLEN_REG_A           /* EE Address */ 0x00000248
#define PROTOCOL_JEWEL_TXSYM10BURSTLEN_REG_V           /* Value      */ 0x00
#define PROTOCOL_JEWEL_TXWAITCTRL_REG_A                /* EE Address */ 0x00000249
#define PROTOCOL_JEWEL_TXWAITCTRL_REG_V                /* Value      */ 0x40
#define PROTOCOL_JEWEL_TXWAITLO_REG_A                  /* EE Address */ 0x0000024A
#define PROTOCOL_JEWEL_TXWAITLO_REG_V                  /* Value      */ 0x09
#define PROTOCOL_JEWEL_FRAMECON_REG_A                  /* EE Address */ 0x0000024B
#define PROTOCOL_JEWEL_FRAMECON_REG_V                  /* Value      */ 0x4F
#define PROTOCOL_JEWEL_RXSOFD_REG_A                    /* EE Address */ 0x0000024C
#define PROTOCOL_JEWEL_RXSOFD_REG_V                    /* Value      */ 0x00
#define PROTOCOL_JEWEL_RXCTRL_REG_A                    /* EE Address */ 0x0000024D
#define PROTOCOL_JEWEL_RXCTRL_REG_V                    /* Value      */ 0x04
#define PROTOCOL_JEWEL_RXWAIT_REG_A                    /* EE Address */ 0x0000024E
#define PROTOCOL_JEWEL_RXWAIT_REG_V                    /* Value      */ 0x8F
#define PROTOCOL_JEWEL_RXTHRESHOLD_REG_A               /* EE Address */ 0x0000024F
#define PROTOCOL_JEWEL_RXTHRESHOLD_REG_V               /* Value      */ 0x32
#define PROTOCOL_JEWEL_RCV_REG_A                       /* EE Address */ 0x00000250
#define PROTOCOL_JEWEL_RCV_REG_V                       /* Value      */ 0x12
#define PROTOCOL_JEWEL_RXANA_REG_A                     /* EE Address */ 0x00000251
#define PROTOCOL_JEWEL_RXANA_REG_V                     /* Value      */ 0x0A

/* Protocol : TypeB EMVCO 106 */
#define PROTOCOL_TYPEB_EMVCO_106_DRVMOD_REG_A          /* EE Address */ 0x00000254
#define PROTOCOL_TYPEB_EMVCO_106_DRVMOD_REG_V          /* Value      */ 0x8F
#define PROTOCOL_TYPEB_EMVCO_106_TXAMP_REG_A           /* EE Address */ 0x00000255
#define PROTOCOL_TYPEB_EMVCO_106_TXAMP_REG_V           /* Value      */ 0x0E
#define PROTOCOL_TYPEB_EMVCO_106_DRVCON_REG_A          /* EE Address */ 0x00000256
#define PROTOCOL_TYPEB_EMVCO_106_DRVCON_REG_V          /* Value      */ 0x09
#define PROTOCOL_TYPEB_EMVCO_106_TXL_REG_A             /* EE Address */ 0x00000257
#define PROTOCOL_TYPEB_EMVCO_106_TXL_REG_V             /* Value      */ 0x0A
#define PROTOCOL_TYPEB_EMVCO_106_TXCRCPRESET_REG_A     /* EE Address */ 0x00000258
#define PROTOCOL_TYPEB_EMVCO_106_TXCRCPRESET_REG_V     /* Value      */ 0x7B
#define PROTOCOL_TYPEB_EMVCO_106_RXCRCPRESET_REG_A     /* EE Address */ 0x00000259
#define PROTOCOL_TYPEB_EMVCO_106_RXCRCPRESET_REG_V     /* Value      */ 0x7B
#define PROTOCOL_TYPEB_EMVCO_106_TXDATANUM_REG_A       /* EE Address */ 0x0000025A
#define PROTOCOL_TYPEB_EMVCO_106_TXDATANUM_REG_V       /* Value      */ 0x08
#define PROTOCOL_TYPEB_EMVCO_106_TXMODWIDTH_REG_A      /* EE Address */ 0x0000025B
#define PROTOCOL_TYPEB_EMVCO_106_TXMODWIDTH_REG_V      /* Value      */ 0x00
#define PROTOCOL_TYPEB_EMVCO_106_TXSYM10BURSTLEN_REG_A /* EE Address */ 0x0000025C
#define PROTOCOL_TYPEB_EMVCO_106_TXSYM10BURSTLEN_REG_V /* Value      */ 0x00
#define PROTOCOL_TYPEB_EMVCO_106_TXWAITCTRL_REG_A      /* EE Address */ 0x0000025D
#define PROTOCOL_TYPEB_EMVCO_106_TXWAITCTRL_REG_V      /* Value      */ 0x01
#define PROTOCOL_TYPEB_EMVCO_106_TXWAITLO_REG_A        /* EE Address */ 0x0000025E
#define PROTOCOL_TYPEB_EMVCO_106_TXWAITLO_REG_V        /* Value      */ 0x00
#define PROTOCOL_TYPEB_EMVCO_106_FRAMECON_REG_A        /* EE Address */ 0x0000025F
#define PROTOCOL_TYPEB_EMVCO_106_FRAMECON_REG_V        /* Value      */ 0x05
#define PROTOCOL_TYPEB_EMVCO_106_RXSOFD_REG_A          /* EE Address */ 0x00000260
#define PROTOCOL_TYPEB_EMVCO_106_RXSOFD_REG_V          /* Value      */ 0x00
#define PROTOCOL_TYPEB_EMVCO_106_RXCTRL_REG_A          /* EE Address */ 0x00000261
#define PROTOCOL_TYPEB_EMVCO_106_RXCTRL_REG_V          /* Value      */ 0x34
#define PROTOCOL_TYPEB_EMVCO_106_RXWAIT_REG_A          /* EE Address */ 0x00000262
#define PROTOCOL_TYPEB_EMVCO_106_RXWAIT_REG_V          /* Value      */ 0x90
#define PROTOCOL_TYPEB_EMVCO_106_RXTHRESHOLD_REG_A     /* EE Address */ 0x00000263
#define PROTOCOL_TYPEB_EMVCO_106_RXTHRESHOLD_REG_V     /* Value      */ 0x9F
#define PROTOCOL_TYPEB_EMVCO_106_RCV_REG_A             /* EE Address */ 0x00000264
#define PROTOCOL_TYPEB_EMVCO_106_RCV_REG_V             /* Value      */ 0x12
#define PROTOCOL_TYPEB_EMVCO_106_RXANA_REG_A           /* EE Address */ 0x00000265
#define PROTOCOL_TYPEB_EMVCO_106_RXANA_REG_V           /* Value      */ 0x03

#endif /* RC663_EEPROM_DUMP_H */
