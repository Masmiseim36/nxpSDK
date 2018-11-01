/*
 * Copyright (c) 2013-2015 Freescale Semiconductor, Inc.
 * All rights reserved.
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

char code_section1[] =
    "/*\n\
* Copyright (c) 2013 - 15, Freescale Semiconductor, Inc.\n\
* All rights reserved\n\
*\n\
* Redistribution and use in source and binary forms, with or without modification,\n\
*are permitted provided that the following conditions are met :\n\
*\n\
* o Redistributions of source code must retain the above copyright notice, this list\n\
*   of conditions and the following disclaimer.\n\
*\n\
* o Redistributions in binary form must reproduce the above copyright notice, this\n\
*   list of conditions and the following disclaimer in the documentation and / or\n\
*   other materials provided with the distribution.\n\
*\n\
* o Neither the name of Freescale Semiconductor, Inc. nor the names of its\n\
*   contributors may be used to endorse or promote products derived from this\n\
*   software without specific prior written permission.\n\
*\n\
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND\n\
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED\n\
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE\n\
* DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR\n\
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES\n\
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;\n\
*LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON\n\
* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT\n\
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n\
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n\
*/\n\
\n\
/*!\n\
* @brief Device specific configuration file for Bootloader\n\
*\n\
* Provides a bootloader configuration structure and a global variable that\n\
* contains\n\
* the system bootloader configuration data.\n\
*/\n\
\n\
#include <stdint.h>\n\
\n\
/*!\n\
* @brief Defines the structure to set the Bootloader Configuration Area\n\
*\n\
* This type of variable is used to set the Bootloader Configuration Area\n\
* of the chip.\n\
*\n\
* Warning: some configuration may not work on the devices that donot support \n\
* corresponding feature.\n\
*/\n\
typedef struct BootloaderConfiguration\n\
{\n\
    uint32_t tag; //!< [00:03] Magic number to verify bootloader configuration is\n\
                  //! valid. Must be set to 'kcfg'.\n\
    uint32_t crcStartAddress; //!< [04:07] Start address for application image CRC\n\
                              //! check. If the bits are all set then\n\
                              //! bootloader by default will not perform any CRC \n\
                              //! check.\n\
    uint32_t crcByteCount; //!< [08:0b] Byte count for application image CRC\n\
                           //! check. If the bits are all set then\n\
                           //! bootloader by default will not prform any CRC check.\n\
    uint32_t crcExpectedValue; //!< [0c:0f] Expected CRC value for application CRC\n\
                               //! check. If the bits are all set then\n\
                               //! bootloader by default will not perform any CRC\n\
                               //! check.\n\
    uint8_t enabledPeripherals; //!< [10:10] Bitfield of peripherals to enable.\n\
                                //! bit 0 - LPUART, bit 1 - I2C, bit 2 - SPI,\n\
                                //! bit 3 - CAN, bit 4 - USB\n\
                                //! Bootloader will enable the peripheral if\n\
                                //! corresponding bit is set to 1.\n\
    uint8_t i2cSlaveAddress; //!< [11:11] If not 0xFF, used as the 7-bit I2C slave\n\
                             //! address. If 0xFF, defaults to 0x10\n\
                             //! for I2C slave address.\n\
    uint16_t peripheralDetectionTimeoutMs; //!< [12:13] Timeout in milliseconds\n\
                                           //! for active peripheral detection. If\n\
                                           //! 0xFFFF, defaults to 5 seconds.\n\
    uint16_t usbVid; //!< [14:15] Sets the USB Vendor ID reported by the device\n\
                     //! during enumeration. If 0xFFFF, it defaults to 0x15A2.\n\
    uint16_t usbPid; //!< [16:17] Sets the USB Product ID reported by the device\n\
                     //! during enumeration.\n\
    uint32_t usbStringsPointer; //!< [18:1b] Sets the USB Strings reported by the\n\
                                //! device during enumeration.\n\
    uint8_t clockFlags; //!< [1c:1c] The flags in the clockFlags configuration\n\
                        //! field are enabled if the corresponding bit is cleared (0).\n\
                        //! bit 0 - HighSpeed Enable high speed mode (i.e., 48 MHz).\n\
    uint8_t clockDivider; //!< [1d:1d] Inverted value of the divider to use for\n\
                          //! core and bus clocks when in high speed mode.\n\
    uint8_t bootFlags; //!< [1e:1e] If bit 0 is cleared, then Bootloader\n\
                       //! will jump to either Quad SPI Flash or internal flash\n\
                       //! image depending on FOPT BOOTSRC_SEL bits.\n\
                       //! If the bit is set, then Bootloader will prepare\n\
                       //! for host communication over serial peripherals.\n\
    uint8_t pad0; //!< [1f:1f] Reserved, set to 0xFF\n\
    uint32_t mmcauConfigPointer; //!< [20:23] A pointer to the MMCAU configuration\n\
                                 //! structure in memory.\n\
    uint32_t keyBlobPointer; //!< [24:27]Holds a pointer value to the key blob\n\
                             //! array used to configure OTFAD\n\
    uint8_t pad1; //!< [28:28] reserved\n\
    uint8_t canConfig1; //!< [29:29] ClkSel[1], PropSeg[3], SpeedIndex[4]\n\
    uint16_t canConfig2; //!< [2a:2b] Pdiv[8], Pseg1[3], Pseg2[3],  rjw[2]\n\
    uint16_t canTxId; //!< [2c:2d] txId\n\
    uint16_t canRxId; //!< [2e:2f] rxId\n\
    uint32_t qspi_config_block_pointer; //!< [30:33] A pointer to the QSPI config\n\
                                        //! block in internal flash array.\n\
    uint32_t reserved[3]; //!<[34:3f] reserved\n\
} bootloader_config_t;\n\
\n\
/*\n\
* Warning: To enable bootloader configuration, User still needs two extra steps :\n\
*\n\
*   Step1 : Enable BOOTLOADER_CONFIG.\n\
*   Example : #define BOOTLOADER_CONFIG 1\n\
*\n\
*   Step2 : Add configuration of the section \"BootloaderConfig\" to your\n\
*   linker configuration file.\n\
*   Example(IAR) :\n\
*       define symbol __application_startaddress = 0; // User-defined\n\
*       define symbol m_bootloader_config_start = __application_startaddress + 0x3C0;\n\
*       define symbol m_bootloader_config_end = __application_startaddress + 0x3FF;\n\
*       define region m_bootloader_config_region = mem:[from m_bootloader_config_start to m_bootloader_config_end];\n\
*       place in m_bootloader_config_region{ section BootloaderConfig };\n\
*\n\
*   Example(Keil) :\n\
*       #define __application_startaddress = 0 // User-defined\n\
*       #define m_bootloader_config_start      __application_startaddress + 0x3C0\n\
*       #define m_bootloader_config_size       0x00000040\n\
*\n\
*       LR_m_bootloader_config m_bootloader_config_start m_bootloader_config_size{\n\
*         ER_m_bootloader_config m_bootloader_config_start m_bootloader_config_size{ ; load address = execution address\n\
*           * (BootloaderConfig)\n\
*         }\n\
*       }\n\
*\n\
*   Example(ARM - GCC) :\n\
*       MEMORY\n\
*       {\n\
*           m_bootloader_config(RX) : ORIGIN = 0x000003C0, LENGTH = 0x00000040\n\
*       }\n\
*       .bootloader_config :\n\
*       {\n\
*           . = ALIGN(4);\n\
*           KEEP(*(.BootloaderConfig)) // Bootloader Configuration Area (BCA)\n\
*           . = ALIGN(4);\n\
*       } > m_bootloader_config\n\
*\n\
*/\n\
\n\
#ifdef BOOTLOADER_CONFIG\n\
/* Bootlader configuration area */\n\
#if defined(__IAR_SYSTEMS_ICC__)\n\
/* Pragma to place the Bootloader Configuration Array on correct location\n\
* defined in linker file. */\n\
#pragma language = extended\n\
#pragma location = \"BootloaderConfig\"\n\
__root const bootloader_config_t BootloaderConfig @\"BootloaderConfig\" =\n\
#elif defined(__GNUC__)\n\
__attribute__((section(\".BootloaderConfig\"))) const bootloader_config_t BootloaderConfig =\n\
#elif defined(__CC_ARM)\n\
__attribute__((section(\"BootloaderConfig\"))) const bootloader_config_t BootloaderConfig __attribute__((used)) =\n\
#else\n\
#error Unsupported compiler!\n\
#endif\n\
    {";

char code_section2[] =
    "\n    };\n\
#endif";