/*
*         Copyright (c), NXP Semiconductors Gratkorn / Austria
*
*                     (C)NXP Semiconductors
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
* AES implementation of the Symmetric Cryptography Library.
* $Author$
* $Revision$ (v06.01.00)
* $Date$
*
* History:
*  SLe: Generated 01.12.2009
*  SLe: Alpha Release 10.01.2010
*
*/

#include <ph_Status.h>
#include <phCryptoSym.h>

#ifdef NXPBUILD__PH_CRYPTOSYM_SW

#ifdef PH_CRYPTOSYM_SW_AES

#include "phCryptoSym_Sw_Aes.h"
#include "phCryptoSym_Sw_Aes_Int.h"

static  const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_sboxTable[256] = {           \
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, \
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, \
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, \
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, \
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, \
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, \
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, \
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, \
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, \
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, \
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, \
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, \
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, \
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, \
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, \
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_invSboxTable[256] = {         \
0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, \
0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, \
0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, \
0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, \
0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, \
0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, \
0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06, \
0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b, \
0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, \
0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, \
0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, \
0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, \
0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, \
0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, \
0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, \
0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_times2[256] = {  \
0,   2,   4,   6,   8,  10,  12,  14,  16,  18,  20,  22,  24,  26,  28,  30,   \
32,  34,  36,  38,  40,  42,  44,  46,  48,  50,  52,  54,  56,  58,  60,  62,  \
64,  66,  68,  70,  72,  74,  76,  78,  80,  82,  84,  86,  88,  90,  92,  94,  \
96,  98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126,  \
128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158, \
160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190, \
192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222, \
224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254, \
27,  25,  31,  29,  19,  17,  23,  21,  11,   9,  15,  13,   3,   1,   7,   5,  \
59,  57,  63,  61,  51,  49,  55,  53,  43,  41,  47,  45,  35,  33,  39,  37,  \
91,  89,  95,  93,  83,  81,  87,  85,  75,  73,  79,  77,  67,  65,  71,  69,  \
123, 121, 127, 125, 115, 113, 119, 117, 107, 105, 111, 109,  99,  97, 103, 101, \
155, 153, 159, 157, 147, 145, 151, 149, 139, 137, 143, 141, 131, 129, 135, 133, \
187, 185, 191, 189, 179, 177, 183, 181, 171, 169, 175, 173, 163, 161, 167, 165, \
219, 217, 223, 221, 211, 209, 215, 213, 203, 201, 207, 205, 195, 193, 199, 197, \
251, 249, 255, 253, 243, 241, 247, 245, 235, 233, 239, 237, 227, 225, 231, 229};

#ifndef PH_CRYPTOSYM_SW_ROM_OPTIMIZATION

static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_times3[256] = {  \
0,   3,   6,   5,  12,  15,  10,   9,  24,  27,  30,  29,  20,  23,  18,  17,  \
48,  51,  54,  53,  60,  63,  58,  57,  40,  43,  46,  45,  36,  39,  34,  33,  \
96,  99, 102, 101, 108, 111, 106, 105, 120, 123, 126, 125, 116, 119, 114, 113,  \
80,  83,  86,  85,  92,  95,  90,  89,  72,  75,  78,  77,  68,  71,  66,  65,  \
192, 195, 198, 197, 204, 207, 202, 201, 216, 219, 222, 221, 212, 215, 210, 209, \
240, 243, 246, 245, 252, 255, 250, 249, 232, 235, 238, 237, 228, 231, 226, 225, \
160, 163, 166, 165, 172, 175, 170, 169, 184, 187, 190, 189, 180, 183, 178, 177, \
144, 147, 150, 149, 156, 159, 154, 153, 136, 139, 142, 141, 132, 135, 130, 129, \
155, 152, 157, 158, 151, 148, 145, 146, 131, 128, 133, 134, 143, 140, 137, 138, \
171, 168, 173, 174, 167, 164, 161, 162, 179, 176, 181, 182, 191, 188, 185, 186, \
251, 248, 253, 254, 247, 244, 241, 242, 227, 224, 229, 230, 239, 236, 233, 234, \
203, 200, 205, 206, 199, 196, 193, 194, 211, 208, 213, 214, 223, 220, 217, 218, \
91,  88,  93,  94,  87,  84,  81,  82,  67,  64,  69,  70,  79,  76,  73,  74,  \
107, 104, 109, 110, 103, 100,  97,  98, 115, 112, 117, 118, 127, 124, 121, 122, \
59,   56,  61,  62,  55,  52,  49,  50,  35,  32,  37,  38,  47,  44,  41,  42, \
11,    8,  13,  14,   7,   4,   1,   2,  19,  16,  21,  22,  31,  28,  25,  26};
#endif /* PH_CRYPTOSYM_SW_ROM_OPTIMIZATION */
static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_times9[256] = {  \
0,   9,  18,  27,  36,  45,  54,  63,  72,  65,  90,  83, 108, 101, 126, 119,   \
144, 153, 130, 139, 180, 189, 166, 175, 216, 209, 202, 195, 252, 245, 238, 231, \
59,  50,  41,  32,  31,  22,  13,   4, 115, 122,  97, 104,  87,  94,  69,  76,  \
171, 162, 185, 176, 143, 134, 157, 148, 227, 234, 241, 248, 199, 206, 213, 220, \
118, 127, 100, 109,  82,  91,  64,  73,  62,  55,  44,  37,  26,  19,   8,   1, \
230, 239, 244, 253, 194, 203, 208, 217, 174, 167, 188, 181, 138, 131, 152, 145, \
77,  68,  95,  86, 105,  96, 123, 114,   5,  12,  23,  30,  33,  40,  51,  58,  \
221, 212, 207, 198, 249, 240, 235, 226, 149, 156, 135, 142, 177, 184, 163, 170, \
236, 229, 254, 247, 200, 193, 218, 211, 164, 173, 182, 191, 128, 137, 146, 155, \
124, 117, 110, 103,  88,  81,  74,  67,  52,  61,  38,  47,  16,  25,   2,  11, \
215, 222, 197, 204, 243, 250, 225, 232, 159, 150, 141, 132, 187, 178, 169, 160, \
71,  78,  85,  92,  99, 106, 113, 120,  15,   6,  29,  20,  43,  34,  57,  48,  \
154, 147, 136, 129, 190, 183, 172, 165, 210, 219, 192, 201, 246, 255, 228, 237, \
10,   3,  24,  17,  46,  39,  60,  53,  66,  75,  80,  89, 102, 111, 116, 125,  \
161, 168, 179, 186, 133, 140, 151, 158, 233, 224, 251, 242, 205, 196, 223, 214, \
49,  56,  35,  42,  21,  28,   7,  14, 121, 112, 107,  98,  93,  84,  79,  70};

#ifndef PH_CRYPTOSYM_SW_ROM_OPTIMIZATION
static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_timesB[256] = { \
0,  11,  22,  29,  44,  39,  58,  49,  88,  83,  78,  69, 116, 127,  98, 105,  \
176, 187, 166, 173, 156, 151, 138, 129, 232, 227, 254, 245, 196, 207, 210, 217,\
123, 112, 109, 102,  87,  92,  65,  74,  35,  40,  53, 62,   15,   4,  25,  18,\
203, 192, 221, 214, 231, 236, 241, 250, 147, 152, 133, 142, 191, 180, 169, 162,\
246, 253, 224, 235, 218, 209, 204, 199, 174, 165, 184, 179, 130, 137, 148, 159,\
70,  77,  80,  91, 106,  97, 124, 119,  30,  21,   8,   3,  50,  57,  36,  47, \
141, 134, 155, 144, 161, 170, 183, 188, 213, 222, 195, 200, 249, 242, 239, 228,\
61,  54,  43,  32,  17,  26,   7,  12, 101, 110, 115, 120,  73,  66,  95,  84, \
247, 252, 225, 234, 219, 208, 205, 198, 175, 164, 185, 178, 131, 136, 149, 158,\
71,  76,  81,  90, 107,  96, 125, 118,  31,  20,   9,   2,  51,  56,  37,  46, \
140, 135, 154, 145, 160, 171, 182, 189, 212, 223, 194, 201, 248, 243, 238, 229,\
60,  55,  42,  33,  16,  27,   6,  13, 100, 111, 114, 121,  72,  67,  94,  85, \
1,  10,  23,  28,  45,  38,  59,  48,  89,  82,  79,  68, 117, 126,  99, 104,  \
177, 186, 167, 172, 157, 150, 139, 128, 233, 226, 255, 244, 197, 206, 211, 216,\
122, 113, 108, 103,  86,  93,  64,  75,  34,  41,  52,  63,  14,   5,  24,  19,\
202, 193, 220, 215, 230, 237, 240, 251, 146, 153, 132, 143, 190, 181, 168, 163};
#endif /* PH_CRYPTOSYM_SW_ROM_OPTIMIZATION */
#ifndef PH_CRYPTOSYM_SW_ROM_OPTIMIZATION
static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_timesD[256] = { \
0,  13,  26,  23,  52,  57,  46,  35, 104, 101, 114, 127,  92,  81,  70,  75,  \
208, 221, 202, 199, 228, 233, 254, 243, 184, 181, 162, 175, 140, 129, 150, 155,\
187, 182, 161, 172, 143, 130, 149, 152, 211, 222, 201, 196, 231, 234, 253, 240,\
107, 102, 113, 124,  95,  82,  69,  72,   3,  14,  25,  20,  55,  58,  45,  32,\
109,  96, 119, 122,  89,  84,  67,  78,   5,   8,  31,  18,  49,  60,  43,  38,\
189, 176, 167, 170, 137, 132, 147, 158, 213, 216, 207, 194, 225, 236, 251, 246,\
214, 219, 204, 193, 226, 239, 248, 245, 190, 179, 164, 169, 138, 135, 144, 157,\
6,  11,  28,  17,  50,  63,  40,  37, 110,  99, 116, 121,  90,  87,  64,  77,  \
218, 215, 192, 205, 238, 227, 244, 249, 178, 191, 168, 165, 134, 139, 156, 145,\
10,   7,  16,  29,  62,  51,  36,  41,  98, 111, 120, 117,  86,  91,  76,  65, \
97, 108, 123, 118,  85,  88,  79,  66,   9,   4,  19,  30,  61,  48,  39,  42, \
177, 188, 171, 166, 133, 136, 159, 146, 217, 212, 195, 206, 237, 224, 247, 250,\
183, 186, 173, 160, 131, 142, 153, 148, 223, 210, 197, 200, 235, 230, 241, 252,\
103, 106, 125, 112,  83,  94,  73,  68,  15,   2,  21,  24,  59,  54,  33,  44,\
12,   1,  22,  27,  56,  53,  34,  47, 100, 105, 126, 115,  80,  93,  74,  71, \
220, 209, 198, 203, 232, 229, 242, 255, 180, 185, 174, 163, 128, 141, 154, 151};


static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_timesE[256] = { \
0,  14,  28,  18,  56,  54,  36,  42, 112, 126, 108,  98,  72,  70,  84,  90,  \
224, 238, 252, 242, 216, 214, 196, 202, 144, 158, 140, 130, 168, 166, 180, 186,\
219, 213, 199, 201, 227, 237, 255, 241, 171, 165, 183, 185, 147, 157, 143, 129,\
59,  53,  39,  41,   3,  13,  31,  17,  75,  69,  87,  89, 115, 125, 111,  97, \
173, 163, 177, 191, 149, 155, 137, 135, 221, 211, 193, 207, 229, 235, 249, 247,\
77,  67,  81,  95, 117, 123, 105, 103,  61,  51,  33,  47,   5,  11,  25,  23, \
118, 120, 106, 100,  78,  64,  82,  92,   6,   8,  26,  20,  62,  48,  34,  44,\
150, 152, 138, 132, 174, 160, 178, 188, 230, 232, 250, 244, 222, 208, 194, 204,\
65,  79,  93,  83, 121, 119, 101, 107,  49,  63,  45,  35,   9,   7,  21,  27, \
161, 175, 189, 179, 153, 151, 133, 139, 209, 223, 205, 195, 233, 231, 245, 251,\
154, 148, 134, 136, 162, 172, 190, 176, 234, 228, 246, 248, 210, 220, 206, 192,\
122, 116, 102, 104,  66,  76,  94,  80,  10,   4,  22,  24,  50,  60,  46,  32,\
236, 226, 240, 254, 212, 218, 200, 198, 156, 146, 128, 142, 164, 170, 184, 182,\
12,   2,  16,  30,  52,  58,  40,  38, 124, 114,  96, 110,  68,  74,  88,  86, \
55,  57,  43,  37,  15,   1,  19,  29,  71,  73,  91,  85, 127, 113,  99, 109, \
215, 217, 203, 197, 239, 225, 243, 253, 167, 169, 187, 181, 159, 145, 131, 141};
#else
static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_timesC[256] = { \
0,  12,  24,  20,  48,  60,  40,  36,  96, 108, 120, 116,  80,  92,  72,  68,  \
192, 204, 216, 212, 240, 252, 232, 228, 160, 172, 184, 180, 144, 156, 136, 132,\
155, 151, 131, 143, 171, 167, 179, 191, 251, 247, 227, 239, 203, 199, 211, 223,\
91,  87,  67,  79, 107, 103, 115, 127,  59,  55,  35,  47,  11,   7,  19,  31, \
45,  33,  53,  57,  29,  17,   5,   9,  77,  65,  85,  89, 125, 113, 101, 105, \
237, 225, 245, 249, 221, 209, 197, 201, 141, 129, 149, 153, 189, 177, 165, 169,\
182, 186, 174, 162, 134, 138, 158, 146, 214, 218, 206, 194, 230, 234, 254, 242,\
118, 122, 110,  98,  70,  74,  94,  82,  22,  26,  14,   2,  38,  42,  62,  50,\
90,  86,  66,  78, 106, 102, 114, 126,  58,  54,  34,  46,  10,   6,  18,  30, \
154, 150, 130, 142, 170, 166, 178, 190, 250, 246, 226, 238, 202, 198, 210, 222,\
193, 205, 217, 213, 241, 253, 233, 229, 161, 173, 185, 181, 145, 157, 137, 133,\
1,  13,  25,  21,  49,  61,  41,  37,  97, 109, 121, 117,  81,  93,  73,  69,  \
119, 123, 111,  99,  71,  75,  95,  83,  23,  27,  15,   3,  39,  43,  63,  51,\
183, 187, 175, 163, 135, 139, 159, 147, 215, 219, 207, 195, 231, 235, 255, 243,\
236, 224, 244, 248, 220, 208, 196, 200, 140, 128, 148, 152, 188, 176, 164, 168,\
44,  32,  52,  56,  28,  16,   4,   8,  76,  64,  84,  88, 124, 112, 100, 104};
#endif /* PH_CRYPTOSYM_SW_ROM_OPTIMIZATION */
static const uint8_t PH_CRYPTOSYM_SW_CONST_ROM phCryptoSym_Sw_Aes_Rcon[10] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};

phStatus_t phCryptoSym_Sw_Aes_KeyExpansion(
    phCryptoSym_Sw_DataParams_t * pDataParams,
    const uint8_t * pKey,
    uint8_t bNkCurrent,
    uint8_t bNkMax
    )
{
#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
    uint8_t i;
    uint8_t rconId = 0;
    uint8_t PH_MEMLOC_BUF * pKeyPtrNmNk = pDataParams->pKey;
    uint8_t PH_MEMLOC_BUF * pKeyPtrNm1 = &pDataParams->pKey[((bNkCurrent) << 2U) - 4];
    uint8_t PH_MEMLOC_BUF * pCurKeyPtr = &pDataParams->pKey[(bNkCurrent) << 2U];

    /* First copy the key provided... */
    (void)memcpy(pDataParams->pKey, pKey, (size_t)(bNkCurrent << 2U));

    /* In this loop, always three pointers are valid:
    - pKeyPtrNmNk: Points to the byte Nk*4 to the left of the pCurKeyPtr.
    - pKeyPtrNm1: Points to the byte 4 positions to the left of pCurKeyPtr
    - pCurKeyPtr: Points to the next byte to be calculated
    */

    for (i= bNkCurrent; i < bNkMax; i++)
    {
        if ( (i % bNkCurrent) == 0U)
        {
            /* Calculate w[i]=w[i-Nk] xor (SubWord(RotWord(w[i-1])) xor Rcon[i/Nk] */
            pKeyPtrNm1++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++] ^ phCryptoSym_Sw_Aes_Rcon[rconId++] ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++] ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++]^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*(pKeyPtrNm1-4)]^ *pKeyPtrNmNk++;
        }else if ((bNkCurrent == 8U) && ((i % bNkCurrent) == 4U))
        {
            /* Calculate w[i]=w[i-Nk] xor SubWord(w[i-1]) */
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++] ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++] ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++] ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = phCryptoSym_Sw_Aes_sboxTable[*pKeyPtrNm1++] ^ *pKeyPtrNmNk++;
        }else
        {
            /* Calculate w[i]=w[i-Nk] xor w[i-1] */
            *pCurKeyPtr++ = *pKeyPtrNm1++ ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = *pKeyPtrNm1++ ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = *pKeyPtrNm1++ ^ *pKeyPtrNmNk++;
            *pCurKeyPtr++ = *pKeyPtrNm1++ ^ *pKeyPtrNmNk++;
        }
    }
#else
    /* satisfy compiler */
    if (0U != (bNkMax))
    {
        /* Noting to do */;
    }

    /* We only need to copy the key provided... */
    (void)memcpy(pDataParams->pKey, pKey, (size_t)(((uint32_t)bNkCurrent) << 2U));
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */
    return PH_ERR_SUCCESS;
}


phStatus_t phCryptoSym_Sw_Aes_EncryptBlock(
    phCryptoSym_Sw_DataParams_t * pDataParams,
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock,
    uint8_t bNumRounds
    )
{
    /* Implementation details can be found in "Figure 5. Pseudo code for cipher" of Fips 197. */
    /* First basic decision at compile time has to define whether online or offline key scheduling shall be used....*/
#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
    uint8_t i;
    /* AddRoundKey(state, w[0, Nb-1])  See Sec. 5.1.4*/
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, pDataParams->pKey, 0);
#else
    uint8_t i;
    uint8_t bNk;
    uint8_t bCurrentNk = 0;
    uint8_t PH_CRYTOSYM_SW_FAST_RAM bTmpKey[PH_CRYPTOSYM_AES256_KEY_SIZE];

    /* In case of online key scheduling, the key needs to be copied into a temporary array and the bNk as well as the
    bCurrentNk have to be provided to the add round key function */
    switch(pDataParams->wKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        bNk = 4;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        bNk = 6;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        bNk = 8;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_CRYPTOSYM);
    }
    /* Recopy the key */
    (void)memcpy(bTmpKey, pDataParams->pKey, (size_t)(((uint32_t)bNk) << 2U));

    /* AddRoundKey(state, w[0, Nb-1])  See Sec. 5.1.4*/
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bCurrentNk, bNk, PH_CRYPTOSYM_KEYSCHEDULE_ENCRYPTION);
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */

    /* for round = 1 step 1 to Nr-1 */
    for (i=1; i< bNumRounds; i++)
    {
        /* SubBytes(state)  See Sec. 5.1.1 */
        /* ShiftRows(state)  See Sec. 5.1.2*/
        phCryptoSym_Sw_Aes_SubBytesShiftRows(pBlock);
        /* MixColumns(state)  See Sec. 5.1.3 */
        phCryptoSym_Sw_Aes_MixColumns(pBlock);
#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
        /* AddRoundKey(state, w[round*Nb, (round+1U)*Nb-1]) */
        phCryptoSym_Sw_Aes_AddRoundKey(pBlock, pDataParams->pKey, i);
#else
        /* AddRoundKey(state, w[round*Nb, (round+1U)*Nb-1]) */
        phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bCurrentNk, bNk, PH_CRYPTOSYM_KEYSCHEDULE_ENCRYPTION);
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */
    } /* end for */

    /* SubBytes(state) */
    /* ShiftRows(state) */
    phCryptoSym_Sw_Aes_SubBytesShiftRows(pBlock);

#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
    /* AddRoundKey(state, w[Nr*Nb, (Nr+1U)*Nb-1]) */
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, pDataParams->pKey, i);
#else
    /* AddRoundKey(state, w[Nr*Nb, (Nr+1U)*Nb-1]) */
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bCurrentNk, bNk, PH_CRYPTOSYM_KEYSCHEDULE_ENCRYPTION);

    /* Clear TmpKey */
    (void)memset(bTmpKey, 0x00, (size_t)sizeof(bTmpKey));
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */

    return PH_ERR_SUCCESS;
}

phStatus_t phCryptoSym_Sw_Aes_DecryptBlock(
    phCryptoSym_Sw_DataParams_t * pDataParams,
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pBlock,
    uint8_t bNumRounds
    )
{
    /* Implementation details can be found in "Figure 12. Pseudo code for the inverse cipher" of Fips 197. */
    /* First basic decision at compile time has to define whether online or offline key scheduling shall be used....*/
#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
    uint8_t i;
    /* AddRoundKey(state, w[Nr*Nb, (Nr+1U)*Nb-1])  See Sec. 5.1.4 */
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, pDataParams->pKey, bNumRounds);
#else
    uint8_t i;
    uint8_t bNk;
    uint8_t bCurrentNk;
    uint8_t bTmp;
    uint8_t PH_CRYTOSYM_SW_FAST_RAM bTmpKey[PH_CRYPTOSYM_AES256_KEY_SIZE];

    /* In case of online key scheduling, the key needs to be copied into a temporary array and the bNk as well as the
    bCurrentNk have to be provided to the add round key function. In addition, the complete key expansion has to be
    performed upfront because the first round key needed is the one corresponding to round 10/12/14. */

    switch(pDataParams->wKeyType)
    {
    case PH_CRYPTOSYM_KEY_TYPE_AES128:
        bNk = 4;
        bCurrentNk = 43;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES192:
        bNk = 6;
        bCurrentNk = 51;
        break;
    case PH_CRYPTOSYM_KEY_TYPE_AES256:
        bNk = 8;
        bCurrentNk = 59;
        break;
    default:
        return PH_ADD_COMPCODE_FIXED(PH_ERR_INTERNAL_ERROR, PH_COMP_CRYPTOSYM);
    }

    /* Recopy the key */
    (void)memcpy(bTmpKey, pDataParams->pKey, (size_t)(((uint32_t)bNk) << 2U));


    /* Perform complete key expansion upfront. */
    bTmp = 0;
    for (i=0;i<=bNumRounds;i++)
    {
        phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bTmp, bNk, PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION_PREPARE);
    }
    /* Now, bTmpKey contains the last round key. */

    /* AddRoundKey(state, w[Nr*Nb, (Nr+1U)*Nb-1])  See Sec. 5.1.4 */
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bCurrentNk, bNk, PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION);
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */

    /* for round = Nr-1 step -1 downto 1 */
    for (i=bNumRounds - 1U; i > 0U; i--)
    {
        /* InvShiftRows(state)  See Sec. 5.3.1 */
        /* InvSubBytes(state)  See Sec. 5.3.2  */
        phCryptoSym_Sw_Aes_InvSubBytesShiftRows(pBlock);
#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
        /* AddRoundKey(state, w[round*Nb, (round+1U)*Nb-1]) */
        phCryptoSym_Sw_Aes_AddRoundKey(pBlock, pDataParams->pKey, i);
#else
        /* AddRoundKey(state, w[round*Nb, (round+1U)*Nb-1]) */
        phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bCurrentNk, bNk, PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION);
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */
        /* InvMixColumns(state)  See Sec. 5.3.3 */
        phCryptoSym_Sw_Aes_InvMixColumns(pBlock);
    } /* end for */

    /* InvShiftRows(state) */
    /* InvSubBytes(state)  */
    phCryptoSym_Sw_Aes_InvSubBytesShiftRows(pBlock);
#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
    /* AddRoundKey(state, w[0, Nb-1]) */
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, pDataParams->pKey, i);
#else
    /* AddRoundKey(state, w[0, Nb-1]) */
    phCryptoSym_Sw_Aes_AddRoundKey(pBlock, bTmpKey, &bCurrentNk, bNk, PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION);
    /* Clear TmpKey */
    (void)memset(bTmpKey, 0x00, (size_t)sizeof(bTmpKey));
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */
    return PH_ERR_SUCCESS;
}

void phCryptoSym_Sw_Aes_SubBytesShiftRows(uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState)
{
    uint8_t PH_CRYTOSYM_SW_FAST_RAM tmp;
    /* For details, see section Sec. 5.1.1 and See Sec. 5.1.2 in FIPS-197 */

    pState[0] = phCryptoSym_Sw_Aes_sboxTable[pState[0]];    /* Row 1: No shift */
    pState[4] = phCryptoSym_Sw_Aes_sboxTable[pState[4]];    /* Row 1: No shift */
    pState[8] = phCryptoSym_Sw_Aes_sboxTable[pState[8]];    /* Row 1: No shift */
    pState[12] = phCryptoSym_Sw_Aes_sboxTable[pState[12]];  /* Row 1: No shift */

    tmp = phCryptoSym_Sw_Aes_sboxTable[pState[1]];          /* Row 2: Shift 1 Position to the left */
    pState[1] = phCryptoSym_Sw_Aes_sboxTable[pState[5]];    /* Row 2: Shift 1 Position to the left */
    pState[5] = phCryptoSym_Sw_Aes_sboxTable[pState[9]];    /* Row 2: Shift 1 Position to the left */
    pState[9] = phCryptoSym_Sw_Aes_sboxTable[pState[13]];   /* Row 2: Shift 1 Position to the left */
    pState[13] = tmp;                                       /* Row 2: Shift 1 Position to the left */

    tmp = phCryptoSym_Sw_Aes_sboxTable[pState[2]];          /* Row 3: Shift 2 Position to the left */
    pState[2] = phCryptoSym_Sw_Aes_sboxTable[pState[10]];   /* Row 3: Shift 2 Position to the left */
    pState[10] = tmp;                                       /* Row 3: Shift 2 Position to the left */
    tmp = phCryptoSym_Sw_Aes_sboxTable[pState[6]];          /* Row 3: Shift 2 Position to the left */
    pState[6] = phCryptoSym_Sw_Aes_sboxTable[pState[14]];   /* Row 3: Shift 2 Position to the left */
    pState[14] = tmp;                                       /* Row 3: Shift 2 Position to the left */

    tmp = phCryptoSym_Sw_Aes_sboxTable[pState[15]];         /* Row 4: Shift 3 Position to the left */
    pState[15] = phCryptoSym_Sw_Aes_sboxTable[pState[11]];  /* Row 4: Shift 3 Position to the left */
    pState[11] = phCryptoSym_Sw_Aes_sboxTable[pState[7]];   /* Row 4: Shift 3 Position to the left */
    pState[7] = phCryptoSym_Sw_Aes_sboxTable[pState[3]];    /* Row 4: Shift 3 Position to the left */
    pState[3] = tmp;                                        /* Row 4: Shift 3 Position to the left */
}

void phCryptoSym_Sw_Aes_InvSubBytesShiftRows(uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState)
{
    uint8_t PH_CRYTOSYM_SW_FAST_RAM tmp;

    /* For details, see section Sec. 5.3.1 and See Sec. 5.3.2 in FIPS-197 */
    pState[0] = phCryptoSym_Sw_Aes_invSboxTable[pState[0]];        /* Row 1: No shift */
    pState[4] = phCryptoSym_Sw_Aes_invSboxTable[pState[4]];     /* Row 1: No shift */
    pState[8] = phCryptoSym_Sw_Aes_invSboxTable[pState[8]];     /* Row 1: No shift */
    pState[12] = phCryptoSym_Sw_Aes_invSboxTable[pState[12]];   /* Row 1: No shift */

    tmp = phCryptoSym_Sw_Aes_invSboxTable[pState[13]];          /* Row 2: Shift 1 Position to the right */
    pState[13] = phCryptoSym_Sw_Aes_invSboxTable[pState[9]];    /* Row 2: Shift 1 Position to the right */
    pState[9] = phCryptoSym_Sw_Aes_invSboxTable[pState[5]];     /* Row 2: Shift 1 Position to the right */
    pState[5] = phCryptoSym_Sw_Aes_invSboxTable[pState[1]];     /* Row 2: Shift 1 Position to the right */
    pState[1] = tmp;                                            /* Row 2: Shift 1 Position to the right */

    tmp = phCryptoSym_Sw_Aes_invSboxTable[pState[2]];           /* Row 3: Shift 2 Position to the right */
    pState[2] = phCryptoSym_Sw_Aes_invSboxTable[pState[10]];    /* Row 3: Shift 2 Position to the right */
    pState[10] = tmp;                                           /* Row 3: Shift 2 Position to the right */
    tmp = phCryptoSym_Sw_Aes_invSboxTable[pState[6]];           /* Row 3: Shift 2 Position to the right */
    pState[6] = phCryptoSym_Sw_Aes_invSboxTable[pState[14]];    /* Row 3: Shift 2 Position to the right */
    pState[14] = tmp;                                           /* Row 3: Shift 2 Position to the right */

    tmp = phCryptoSym_Sw_Aes_invSboxTable[pState[3]];           /* Row 4: Shift 3 Position to the right */
    pState[3] = phCryptoSym_Sw_Aes_invSboxTable[pState[7]];     /* Row 4: Shift 3 Position to the right */
    pState[7] = phCryptoSym_Sw_Aes_invSboxTable[pState[11]];    /* Row 4: Shift 3 Position to the right */
    pState[11] = phCryptoSym_Sw_Aes_invSboxTable[pState[15]];   /* Row 4: Shift 3 Position to the right */
    pState[15] = tmp;                                           /* Row 4: Shift 3 Position to the right */
}

void phCryptoSym_Sw_Aes_MixColumns(uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState)
{
    uint8_t PH_CRYTOSYM_SW_FAST_RAM tmp[4];
    uint8_t i;

    /* For details, see section Sec. 5.1.3 in FIPS-197 */

    /* Generally, the calculation speed is increased by operating with lookup tables. */
    /* in case of ROM_OPTIMIZATION enabled, the following equation is calculated online: */
    /* times3 = times2 xor times1 */
    for (i=0;i<4U;i++)
    {
        /* In each iteration, the following equation is calculated: */
        /* [ s0,c'] [ 2 3 1 1 ][ s0,c] */
        /* [ s1,c']=[ 1 2 3 1 ][ s1,c] */
        /* [ s2,c'] [ 1 1 2 3 ][ s2,c] */
        /* [ s3,c'] [ 3 1 1 2 ][ s3,c] */
#ifndef PH_CRYPTOSYM_SW_ROM_OPTIMIZATION
        tmp[0] = phCryptoSym_Sw_Aes_times2[pState[0]] ^ phCryptoSym_Sw_Aes_times3[pState[1]] ^ pState[2] ^ pState[3];
        tmp[1] = pState[0] ^ phCryptoSym_Sw_Aes_times2[pState[1]] ^ phCryptoSym_Sw_Aes_times3[pState[2]] ^ pState[3];
        tmp[2] = pState[0] ^ pState[1] ^ phCryptoSym_Sw_Aes_times2[pState[2]] ^ phCryptoSym_Sw_Aes_times3[pState[3]];
        tmp[3] = phCryptoSym_Sw_Aes_times3[pState[0]] ^ pState[1] ^ pState[2] ^ phCryptoSym_Sw_Aes_times2[pState[3]];
#else
        tmp[0] = phCryptoSym_Sw_Aes_times2[pState[0]] ^ (phCryptoSym_Sw_Aes_times2[pState[1]] ^ pState[1]) ^ pState[2] ^ pState[3];
        tmp[1] = pState[0] ^ phCryptoSym_Sw_Aes_times2[pState[1]] ^ (phCryptoSym_Sw_Aes_times2[pState[2]] ^ pState[2]) ^ pState[3];
        tmp[2] = pState[0] ^ pState[1] ^ phCryptoSym_Sw_Aes_times2[pState[2]] ^ (phCryptoSym_Sw_Aes_times2[pState[3]] ^ pState[3]);
        tmp[3] = (phCryptoSym_Sw_Aes_times2[pState[0]] ^ pState[0]) ^ pState[1] ^ pState[2] ^ phCryptoSym_Sw_Aes_times2[pState[3]];
#endif /* PH_CRYPTOSYM_SW_ROM_OPTIMIZATION */
        *pState++ = tmp[0];
        *pState++ = tmp[1];
        *pState++ = tmp[2];
        *pState++ = tmp[3];
    }
}

void phCryptoSym_Sw_Aes_InvMixColumns(uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState)
{
    uint8_t PH_CRYTOSYM_SW_FAST_RAM tmp[4];
    uint8_t i;

    /* For details, see section Sec. 5.3.3 in FIPS-197 */

    /* Generally, the calculation speed is increased by operating with lookup tables. */
    /* in case of ROM_OPTIMIZATION enabled, the following equation is calculated online: */
    /* timesB = times9 xor times2 */
    /* timesD = timesC xor times1 */
    /* timesE = timesC xor times2 */
    for (i=0;i<4U;i++)
    {
        /* In each iteration, the following equation is calculated: */
        /* [ s0,c'] [ e b d 9 ][ s0,c] */
        /* [ s1,c']=[ 9 e b d ][ s1,c] */
        /* [ s2,c'] [ d 9 e b ][ s2,c] */
        /* [ s3,c'] [ b d 9 e ][ s3,c] */
#ifndef PH_CRYPTOSYM_SW_ROM_OPTIMIZATION
        tmp[0] = phCryptoSym_Sw_Aes_timesE[pState[0]] ^ phCryptoSym_Sw_Aes_timesB[pState[1]] ^ phCryptoSym_Sw_Aes_timesD[pState[2]] ^ phCryptoSym_Sw_Aes_times9[pState[3]];
        tmp[1] = phCryptoSym_Sw_Aes_times9[pState[0]] ^ phCryptoSym_Sw_Aes_timesE[pState[1]] ^ phCryptoSym_Sw_Aes_timesB[pState[2]] ^ phCryptoSym_Sw_Aes_timesD[pState[3]];
        tmp[2] = phCryptoSym_Sw_Aes_timesD[pState[0]] ^ phCryptoSym_Sw_Aes_times9[pState[1]] ^ phCryptoSym_Sw_Aes_timesE[pState[2]] ^ phCryptoSym_Sw_Aes_timesB[pState[3]];
        tmp[3] = phCryptoSym_Sw_Aes_timesB[pState[0]] ^ phCryptoSym_Sw_Aes_timesD[pState[1]] ^ phCryptoSym_Sw_Aes_times9[pState[2]] ^ phCryptoSym_Sw_Aes_timesE[pState[3]];
#else
        tmp[0] = (phCryptoSym_Sw_Aes_timesC[pState[0]] ^ phCryptoSym_Sw_Aes_times2[pState[0]]) ^ (phCryptoSym_Sw_Aes_times9[pState[1]] ^ phCryptoSym_Sw_Aes_times2[pState[1]]) ^ (phCryptoSym_Sw_Aes_timesC[pState[2]] ^ pState[2]) ^ phCryptoSym_Sw_Aes_times9[pState[3]];
        tmp[1] = phCryptoSym_Sw_Aes_times9[pState[0]] ^ (phCryptoSym_Sw_Aes_timesC[pState[1]] ^ phCryptoSym_Sw_Aes_times2[pState[1]]) ^ (phCryptoSym_Sw_Aes_times9[pState[2]] ^ phCryptoSym_Sw_Aes_times2[pState[2]]) ^ (phCryptoSym_Sw_Aes_timesC[pState[3]] ^ pState[3]);
        tmp[2] = (phCryptoSym_Sw_Aes_timesC[pState[0]] ^ pState[0])^ phCryptoSym_Sw_Aes_times9[pState[1]] ^ (phCryptoSym_Sw_Aes_timesC[pState[2]] ^ phCryptoSym_Sw_Aes_times2[pState[2]]) ^ (phCryptoSym_Sw_Aes_times9[pState[3]] ^ phCryptoSym_Sw_Aes_times2[pState[3]]);
        tmp[3] = (phCryptoSym_Sw_Aes_times9[pState[0]] ^ phCryptoSym_Sw_Aes_times2[pState[0]]) ^ (phCryptoSym_Sw_Aes_timesC[pState[1]] ^ pState[1]) ^ phCryptoSym_Sw_Aes_times9[pState[2]] ^ (phCryptoSym_Sw_Aes_timesC[pState[3]] ^ phCryptoSym_Sw_Aes_times2[pState[3]]);
#endif /* PH_CRYPTOSYM_SW_ROM_OPTIMIZATION */
        *pState++ = tmp[0];
        *pState++ = tmp[1];
        *pState++ = tmp[2];
        *pState++ = tmp[3];
    }
}
#ifdef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
void phCryptoSym_Sw_Aes_AddRoundKey(uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState, uint8_t PH_CRYTOSYM_SW_FAST_RAM * pKey, uint8_t * pNkCurrent, uint8_t bNk, uint8_t bMode)
{
    uint8_t i = 0;
    uint8_t PH_CRYTOSYM_SW_FAST_RAM * pCurKeyPtr;
    uint8_t PH_CRYTOSYM_SW_FAST_RAM bOffset;
    uint8_t PH_CRYTOSYM_SW_FAST_RAM bModulus;

    /* NOTE: As pKey is an array of size bNk, the byte of the last round w[i-nk]is always in the same position as the byte currently operated on. */
    /* For each call of the function, 16 bytes of round key need to be calculated */
    for (i=0;i<4U;i++)
    {
        /* Find out, if we are in a "first" column, a "special" column or a "standard" column. */
        bModulus = *pNkCurrent % bNk;

        /* Generally the pCurKeyPtr can be calculated directly out of the bModulus */
        pCurKeyPtr = &pKey[bModulus << 2U];

        /* In case of decryption, xoring has to be done upfront. */
        if (bMode == PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION)
        {
            pState[((3U-i)*4U)] ^= *(pCurKeyPtr + 0U);
            pState[((3U-i)*4U)+1U] ^= *(pCurKeyPtr + 1U);
            pState[((3U-i)*4U)+2U] ^= *(pCurKeyPtr + 2U);
            pState[((3U-i)*4U)+3U] ^= *(pCurKeyPtr + 3U);
        }

        if (*pNkCurrent < bNk)
        {
            /* w[i] = word(key[4U*i], key[4U*i+1U], key[4U*i+2U], key[4U*i+3U]) */
            /* Just increment the current key pointer, as the key is already stored internally...*/
            pCurKeyPtr += 4U;
        }else
        {
            if ( bModulus == 0U)
            {
                /* First find the offset to w[i-1] */
                bOffset = (uint8_t)(((uint8_t)((uint8_t)bNk-1U)<<2U) + 1U);
                /* now calculate the following calculation: */
                /* w[i] = w[i-Nk] xor SubWord(RotWord(w[i-1])) xor Rcon[i/Nk] */
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr+bOffset)] ^ phCryptoSym_Sw_Aes_Rcon[(*pNkCurrent)/bNk - 1U];
                pCurKeyPtr++;
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr+bOffset)];
                pCurKeyPtr++;
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr+bOffset)];
                pCurKeyPtr++;
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr+bOffset - 4U)];
                pCurKeyPtr++;
            }else if ((bNk == 8U) && ((*pNkCurrent % bNk) == 4U))
            {
                /* w[i] = w[i-Nk] xor SubWord(w[i-1]) */
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr-4U)];
                pCurKeyPtr++;
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr-4U)];
                pCurKeyPtr++;
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr-4U)];
                pCurKeyPtr++;
                *pCurKeyPtr ^= phCryptoSym_Sw_Aes_sboxTable[*(pCurKeyPtr-4U)];
                pCurKeyPtr++;
            }else
            {
                /* w[i] = w[i-Nk] xor w[i-1] */
                *pCurKeyPtr ^= *(pCurKeyPtr-4U);
                pCurKeyPtr++;
                *pCurKeyPtr ^= *(pCurKeyPtr-4U);
                pCurKeyPtr++;
                *pCurKeyPtr ^= *(pCurKeyPtr-4U);
                pCurKeyPtr++;
                *pCurKeyPtr ^= *(pCurKeyPtr-4U);
                pCurKeyPtr++;
            }
        }

        /* In case of encryption and key initialization for decryption, we have to increment, else we have to decrement NkCurrent */
        if (bMode != PH_CRYPTOSYM_KEYSCHEDULE_DECRYPTION)
        {
            (*pNkCurrent)++;
        }else
        {
            (*pNkCurrent)--;
        }

        /* In case of encryption, xoring has to be done at the end. */
        if (bMode == PH_CRYPTOSYM_KEYSCHEDULE_ENCRYPTION)
        {
            pState[(i*4U)] ^= *(pCurKeyPtr - 4U);
            pState[(i*4U)+1U] ^= *(pCurKeyPtr - 3U);
            pState[(i*4U)+2U] ^= *(pCurKeyPtr - 2U);
            pState[(i*4U)+3U] ^= *(pCurKeyPtr - 1U);
        }
    }
}
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */

#ifndef PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING
void phCryptoSym_Sw_Aes_AddRoundKey(uint8_t PH_CRYTOSYM_SW_FAST_RAM * pState, uint8_t PH_MEMLOC_BUF * pKey, uint8_t bCnt)
{
    uint8_t PH_CRYTOSYM_SW_FAST_RAM i;
    /* As pKey points to the beginning of the completely expanded key, the correct index has to be identified. */
    uint8_t PH_MEMLOC_BUF * pRoundKey = &pKey[bCnt << 4U];
    /* Perform the simple round key xoring as specified in 5.1.4 AddRoundKey() Transformation of FIPS-197. */
    for (i=0;i<16U;i++)
    {
        pState[i] ^= pRoundKey[i];
    }
}
#endif /* PH_CRYPTOSYM_SW_ONLINE_KEYSCHEDULING */

#endif /* PH_CRYPTOSYM_SW_AES */

#endif /* NXPBUILD__PH_CRYPTOSYM_SW */
