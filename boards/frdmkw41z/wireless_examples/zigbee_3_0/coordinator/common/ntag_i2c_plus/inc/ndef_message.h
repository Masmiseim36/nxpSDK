/*
* The Clear BSD License
* Copyright (c) 2017-2018, NXP Semiconductor
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
*
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
*
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
*
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef NDEF_MESSAGE_H
#define NDEF_MESSAGE_H

/*
 # NDEF message:
  [00] 91 02 30 53 70 91 01 18 54 05 64 65 2D 44 45 4E |..0Sp...T.de-DEN|
  [10] 58 50 20 53 65 6D 69 63 6F 6E 64 75 63 74 6F 72 |XP Semiconductor|
  [20] 73 11 01 09 55 01 6E 78 70 2E 63 6F 6D 2F 51 03 |s...U.nxp.com/Q.|
  [30] 01 61 63 74 00 11 01 15 54 05 64 65 2D 44 45 5A |.act....T.de-DEZ|
  [40] 42 5F 49 4E 53 54 41 4C 4C 5F 43 4F 44 45 11 01 |B_INSTALL_CODE..|
  [50] 26 54 05 64 65 2D 44 45 30 30 31 31 32 32 33 33 |&T.de-DE00112233|
  [60] 34 34 35 35 36 36 37 37 30 30 31 31 32 32 33 33 |4455667700112233|
  [70] 34 34 35 35 36 36 37 37 11 01 12 54 05 64 65 2D |44556677...T.de-|
  [80] 44 45 44 45 56 5F 45 58 54 5F 41 44 44 52 51 01 |DEDEV_EXT_ADDRQ.|
  [90] 16 54 05 64 65 2D 44 45 30 30 31 31 32 32 33 33 |.T.de-DE00112233|
  [A0] 34 34 35 35 36 36 37 37                         |44556677        |
 */
static const uint8_t au8NTAG_NDEF_Msg[] = {
/*[00]*/ 0x03, 0xA8, 0x91, 0x02, 0x30, 0x53, 0x70, 0x91, 0x01, 0x18, 0x54, 0x05, 0x64, 0x65, 0x2D, 0x44,
/*[10]*/ 0x45, 0x4E, 0x58, 0x50, 0x20, 0x53, 0x65, 0x6D, 0x69, 0x63, 0x6F, 0x6E, 0x64, 0x75, 0x63, 0x74,
/*[20]*/ 0x6F, 0x72, 0x73, 0x11, 0x01, 0x09, 0x55, 0x01, 0x6E, 0x78, 0x70, 0x2E, 0x63, 0x6F, 0x6D, 0x2F,
/*[30]*/ 0x51, 0x03, 0x01, 0x61, 0x63, 0x74, 0x00, 0x11, 0x01, 0x15, 0x54, 0x05, 0x64, 0x65, 0x2D, 0x44,
/*[40]*/ 0x45, 0x5A, 0x42, 0x5F, 0x49, 0x4E, 0x53, 0x54, 0x41, 0x4C, 0x4C, 0x5F, 0x43, 0x4F, 0x44, 0x45,
/*[50]*/ 0x11, 0x01, 0x26, 0x54, 0x05, 0x64, 0x65, 0x2D, 0x44, 0x45, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32,
/*[60]*/ 0x33, 0x33, 0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32,
/*[70]*/ 0x33, 0x33, 0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, 0x11, 0x01, 0x12, 0x54, 0x05, 0x64,
/*[80]*/ 0x65, 0x2D, 0x44, 0x45, 0x44, 0x45, 0x56, 0x5F, 0x45, 0x58, 0x54, 0x5F, 0x41, 0x44, 0x44, 0x52,
/*[90]*/ 0x51, 0x01, 0x16, 0x54, 0x05, 0x64, 0x65, 0x2D, 0x44, 0x45, 0x30, 0x30, 0x31, 0x31, 0x32, 0x32,
/*[A0]*/ 0x33, 0x33, 0x34, 0x34, 0x35, 0x35, 0x36, 0x36, 0x37, 0x37, 0xFE, 0x00
};

/* Defines for Install Code Identifier */
#define gZB_INSTALL_CODE_IDENTIFIER_IDX_c    0x41
#define gZB_INSTALL_CODE_IDENTIFIER_LEN_c    0xF

/* Defines for Install Code */
#define gZB_INSTALL_CODE_IDX_c               0x5A
#define gZB_INSTALL_CODE_LEN_c               0x20

/* Defines for Extended Address Identifier */
#define gZB_EXT_ADDR_IDENTIFIER_IDX_c      0x84
#define gZB_EXT_ADDR_IDENTIFIER_LEN_c      0xC

/* Defines for Extended Address */
#define gZB_EXT_ADDR_IDX_c      0x9A
#define gZB_EXT_ADDR_LEN_c      0x10

#define gZB_INSTALL_CODE_PAGE_c    0x50

/* Null block definition */
static const uint8_t Null_Block[] =      {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

/* Default page 56 content definition */
static const uint8_t Default_Page_56[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
/* Default page 57 content definition */
static const uint8_t Default_Page_57[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
/* Default page 58 content definition */
static const uint8_t Default_Page_58[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#endif /* NDEF_MESSAGE_H */
